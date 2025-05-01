/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _PCIERCCFG_H_
#define _PCIERCCFG_H_


/***********************************************************
* Register Name : ADAPTER_ID_W
* Register Description :
* Adapter ID register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the Subsystem Vendor ID value. Specified by the vendor.
#define ADAPTER_ID_W_SUBSYSTEM_VENDOR_ID_OFFSET      0
#define ADAPTER_ID_W_SUBSYSTEM_VENDOR_ID_MASK        0xffff

// Bitfield Description : Controls the Subsystem ID value. Specified by the vendor.
#define ADAPTER_ID_W_SUBSYSTEM_ID_OFFSET      16
#define ADAPTER_ID_W_SUBSYSTEM_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SUBSYSTEM_VENDOR_ID:16;
    UINT32                            SUBSYSTEM_ID:16;
  } Field;
  UINT32 Value;
} ADAPTER_ID_W_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_ADAPTER_ID_W_ADDRESS    0x1110004cUL

#define SMN_FUNC0_PCIE1_ADAPTER_ID_W_ADDRESS    0x1120004cUL

#define SMN_FUNC0_PCIE2_ADAPTER_ID_W_ADDRESS    0x1130004cUL

#define SMN_FUNC0_PCIE3_ADAPTER_ID_W_ADDRESS    0x1140004cUL

#define SMN_FUNC1_PCIE0_ADAPTER_ID_W_ADDRESS    0x1110104cUL

#define SMN_FUNC1_PCIE1_ADAPTER_ID_W_ADDRESS    0x1120104cUL

#define SMN_FUNC2_PCIE0_ADAPTER_ID_W_ADDRESS    0x1110204cUL

#define SMN_FUNC2_PCIE1_ADAPTER_ID_W_ADDRESS    0x1120204cUL

#define SMN_FUNC3_PCIE0_ADAPTER_ID_W_ADDRESS    0x1110304cUL

#define SMN_FUNC3_PCIE1_ADAPTER_ID_W_ADDRESS    0x1120304cUL

#define SMN_FUNC4_PCIE0_ADAPTER_ID_W_ADDRESS    0x1110404cUL

#define SMN_FUNC4_PCIE1_ADAPTER_ID_W_ADDRESS    0x1120404cUL

#define SMN_FUNC5_PCIE1_ADAPTER_ID_W_ADDRESS    0x1120504cUL


/***********************************************************
* Register Name : ADP_PCIE_CS_0
* Register Description :
* PCIe Adapter Layer Configuration Capability for PCIE_over_USB4
* Visibility : 0x2
************************************************************/

// Bitfield Description : contain the Doubleword index of the next Capability in the Adapter Configuration Space or 00h if no other items exist.
#define ADP_PCIE_CS_0_NEXT_PTR_OFFSET      0
#define ADP_PCIE_CS_0_NEXT_PTR_MASK        0xff

// Bitfield Description : USB4 defined ID number that indicating this is a Protocol Adapter Configuration Capability.
#define ADP_PCIE_CS_0_CAP_ID_OFFSET      8
#define ADP_PCIE_CS_0_CAP_ID_MASK        0xff00

// Bitfield Description : Indicates if the PCIe LTSSM Link is Up
#define ADP_PCIE_CS_0_ADP_PCIE_LINK_OFFSET      16
#define ADP_PCIE_CS_0_ADP_PCIE_LINK_MASK        0x10000

// Bitfield Description : Indicates if the PCIe Transmitters are in the Electrical Idle state.
#define ADP_PCIE_CS_0_ADP_PCIE_TX_EI_OFFSET      17
#define ADP_PCIE_CS_0_ADP_PCIE_TX_EI_MASK        0x20000

// Bitfield Description : Indicates if the PCIe Receiver are in the Electrical Idle state.
#define ADP_PCIE_CS_0_ADP_PCIE_RX_EI_OFFSET      18
#define ADP_PCIE_CS_0_ADP_PCIE_RX_EI_MASK        0x40000

// Bitfield Description : Indicate if the attached PCIe Switch Port is in PCIe Warm Reset
#define ADP_PCIE_CS_0_ADP_PCIE_RST_OFFSET      19
#define ADP_PCIE_CS_0_ADP_PCIE_RST_MASK        0x80000

// Bitfield Description : Reserved.
#define ADP_PCIE_CS_0_Reserved_24_20_OFFSET      20
#define ADP_PCIE_CS_0_Reserved_24_20_MASK        0x1f00000

// Bitfield Description : Indicates the state of the LTSSM.
#define ADP_PCIE_CS_0_ADP_PCIE_LTSSM_OFFSET      25
#define ADP_PCIE_CS_0_ADP_PCIE_LTSSM_MASK        0x1e000000

// Bitfield Description : Reserved.
#define ADP_PCIE_CS_0_Reserved_30_29_OFFSET      29
#define ADP_PCIE_CS_0_Reserved_30_29_MASK        0x60000000

// Bitfield Description : Used by the Connection Manager to indicate in-band presence detect.  This enables the PCIe Adapter to send PCIe Tunneled Packets.
#define ADP_PCIE_CS_0_ADP_PCIE_PE_OFFSET      31
#define ADP_PCIE_CS_0_ADP_PCIE_PE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NEXT_PTR:8;
    UINT32                            CAP_ID:8;
    UINT32                            ADP_PCIE_LINK:1;
    UINT32                            ADP_PCIE_TX_EI:1;
    UINT32                            ADP_PCIE_RX_EI:1;
    UINT32                            ADP_PCIE_RST:1;
    UINT32                            Reserved_24_20:5;
    UINT32                            ADP_PCIE_LTSSM:4;
    UINT32                            Reserved_30_29:2;
    UINT32                            ADP_PCIE_PE:1;
  } Field;
  UINT32 Value;
} ADP_PCIE_CS_0_STRUCT;

#define SMN_FUNC0_PCIE2_ADP_PCIE_CS_0_ADDRESS    0x11300580UL

#define SMN_FUNC0_PCIE3_ADP_PCIE_CS_0_ADDRESS    0x11400580UL


/***********************************************************
* Register Name : BASE_ADDR_1
* Register Description :
* PCI CFG BAR 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Base Address Register.
#define BASE_ADDR_1_BASE_ADDR_OFFSET      0
#define BASE_ADDR_1_BASE_ADDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BASE_ADDR:32;
  } Field;
  UINT32 Value;
} BASE_ADDR_1_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_BASE_ADDR_1_ADDRESS    0x11100010UL

#define SMN_FUNC0_PCIE1_BASE_ADDR_1_ADDRESS    0x11200010UL

#define SMN_FUNC0_PCIE2_BASE_ADDR_1_ADDRESS    0x11300010UL

#define SMN_FUNC0_PCIE3_BASE_ADDR_1_ADDRESS    0x11400010UL

#define SMN_FUNC1_PCIE0_BASE_ADDR_1_ADDRESS    0x11101010UL

#define SMN_FUNC1_PCIE1_BASE_ADDR_1_ADDRESS    0x11201010UL

#define SMN_FUNC2_PCIE0_BASE_ADDR_1_ADDRESS    0x11102010UL

#define SMN_FUNC2_PCIE1_BASE_ADDR_1_ADDRESS    0x11202010UL

#define SMN_FUNC3_PCIE0_BASE_ADDR_1_ADDRESS    0x11103010UL

#define SMN_FUNC3_PCIE1_BASE_ADDR_1_ADDRESS    0x11203010UL

#define SMN_FUNC4_PCIE0_BASE_ADDR_1_ADDRESS    0x11104010UL

#define SMN_FUNC4_PCIE1_BASE_ADDR_1_ADDRESS    0x11204010UL

#define SMN_FUNC5_PCIE1_BASE_ADDR_1_ADDRESS    0x11205010UL


/***********************************************************
* Register Name : BASE_ADDR_2
* Register Description :
* PCI CFG BAR 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Base Address Register.
#define BASE_ADDR_2_BASE_ADDR_OFFSET      0
#define BASE_ADDR_2_BASE_ADDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BASE_ADDR:32;
  } Field;
  UINT32 Value;
} BASE_ADDR_2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_BASE_ADDR_2_ADDRESS    0x11100014UL

#define SMN_FUNC0_PCIE1_BASE_ADDR_2_ADDRESS    0x11200014UL

#define SMN_FUNC0_PCIE2_BASE_ADDR_2_ADDRESS    0x11300014UL

#define SMN_FUNC0_PCIE3_BASE_ADDR_2_ADDRESS    0x11400014UL

#define SMN_FUNC1_PCIE0_BASE_ADDR_2_ADDRESS    0x11101014UL

#define SMN_FUNC1_PCIE1_BASE_ADDR_2_ADDRESS    0x11201014UL

#define SMN_FUNC2_PCIE0_BASE_ADDR_2_ADDRESS    0x11102014UL

#define SMN_FUNC2_PCIE1_BASE_ADDR_2_ADDRESS    0x11202014UL

#define SMN_FUNC3_PCIE0_BASE_ADDR_2_ADDRESS    0x11103014UL

#define SMN_FUNC3_PCIE1_BASE_ADDR_2_ADDRESS    0x11203014UL

#define SMN_FUNC4_PCIE0_BASE_ADDR_2_ADDRESS    0x11104014UL

#define SMN_FUNC4_PCIE1_BASE_ADDR_2_ADDRESS    0x11204014UL

#define SMN_FUNC5_PCIE1_BASE_ADDR_2_ADDRESS    0x11205014UL


/***********************************************************
* Register Name : BASE_CLASS
* Register Description :
* The Base Class Code register is used with the Sub-Class Code and the Programming Interface registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Identifies broadly the type of operation the function performs.
#define BASE_CLASS_BASE_CLASS_OFFSET      0
#define BASE_CLASS_BASE_CLASS_MASK        0xff

typedef union {
  struct {
    UINT8                            BASE_CLASS:8;
  } Field;
  UINT8 Value;
} BASE_CLASS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_BASE_CLASS_ADDRESS    0x1110000bUL

#define SMN_FUNC0_PCIE1_BASE_CLASS_ADDRESS    0x1120000bUL

#define SMN_FUNC0_PCIE2_BASE_CLASS_ADDRESS    0x1130000bUL

#define SMN_FUNC0_PCIE3_BASE_CLASS_ADDRESS    0x1140000bUL

#define SMN_FUNC1_PCIE0_BASE_CLASS_ADDRESS    0x1110100bUL

#define SMN_FUNC1_PCIE1_BASE_CLASS_ADDRESS    0x1120100bUL

#define SMN_FUNC2_PCIE0_BASE_CLASS_ADDRESS    0x1110200bUL

#define SMN_FUNC2_PCIE1_BASE_CLASS_ADDRESS    0x1120200bUL

#define SMN_FUNC3_PCIE0_BASE_CLASS_ADDRESS    0x1110300bUL

#define SMN_FUNC3_PCIE1_BASE_CLASS_ADDRESS    0x1120300bUL

#define SMN_FUNC4_PCIE0_BASE_CLASS_ADDRESS    0x1110400bUL

#define SMN_FUNC4_PCIE1_BASE_CLASS_ADDRESS    0x1120400bUL

#define SMN_FUNC5_PCIE1_BASE_CLASS_ADDRESS    0x1120500bUL


/***********************************************************
* Register Name : BIST
* Register Description :
* Used for control and status of built-in self tests.
* Visibility : 0x2
************************************************************/

// Bitfield Description : If the function supports BIST, indicates the test result, otherwise must be 0.
#define BIST_BIST_COMP_OFFSET      0
#define BIST_BIST_COMP_MASK        0xf

// Bitfield Description : 
#define BIST_Reserved_5_4_OFFSET      4
#define BIST_Reserved_5_4_MASK        0x30

// Bitfield Description : If the function supports BIST, initiates the test run, otherwise must be 0.
#define BIST_BIST_STRT_OFFSET      6
#define BIST_BIST_STRT_MASK        0x40

// Bitfield Description : Indicates if the function supports BIST.
#define BIST_BIST_CAP_OFFSET      7
#define BIST_BIST_CAP_MASK        0x80

typedef union {
  struct {
    UINT8                            BIST_COMP:4;
    UINT8                            Reserved_5_4:2;
    UINT8                            BIST_STRT:1;
    UINT8                            BIST_CAP:1;
  } Field;
  UINT8 Value;
} BIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_BIST_ADDRESS    0x1110000fUL

#define SMN_FUNC0_PCIE1_BIST_ADDRESS    0x1120000fUL

#define SMN_FUNC0_PCIE2_BIST_ADDRESS    0x1130000fUL

#define SMN_FUNC0_PCIE3_BIST_ADDRESS    0x1140000fUL

#define SMN_FUNC1_PCIE0_BIST_ADDRESS    0x1110100fUL

#define SMN_FUNC1_PCIE1_BIST_ADDRESS    0x1120100fUL

#define SMN_FUNC2_PCIE0_BIST_ADDRESS    0x1110200fUL

#define SMN_FUNC2_PCIE1_BIST_ADDRESS    0x1120200fUL

#define SMN_FUNC3_PCIE0_BIST_ADDRESS    0x1110300fUL

#define SMN_FUNC3_PCIE1_BIST_ADDRESS    0x1120300fUL

#define SMN_FUNC4_PCIE0_BIST_ADDRESS    0x1110400fUL

#define SMN_FUNC4_PCIE1_BIST_ADDRESS    0x1120400fUL

#define SMN_FUNC5_PCIE1_BIST_ADDRESS    0x1120500fUL


/***********************************************************
* Register Name : BRIDGE_CNTL
* Register Description :
* Provides control over a port's ability to forward bus cycles between its primary and secondary interfaces.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the ability of a function to log errors in the Master Data Parity Error bit of the Secondary Status register.
#define BRIDGE_CNTL_PARITY_RESPONSE_EN_OFFSET      0
#define BRIDGE_CNTL_PARITY_RESPONSE_EN_MASK        0x1

// Bitfield Description : Controls the forwarding of ERR_COR, ERR_NONFATAL and ERR_FATAL from secondary interface to primary interface.
#define BRIDGE_CNTL_SERR_EN_OFFSET      1
#define BRIDGE_CNTL_SERR_EN_MASK        0x2

// Bitfield Description : Disables the bridge from forwarding I/O addresses, in the top 768 bytes of each 1 kB block in the I/O address range, from primary interface to secondary interface.
#define BRIDGE_CNTL_ISA_EN_OFFSET      2
#define BRIDGE_CNTL_ISA_EN_MASK        0x4

// Bitfield Description : Controls the bridge's ability to forward VGA compatible addresses from primary interface to secondary interface.
#define BRIDGE_CNTL_VGA_EN_OFFSET      3
#define BRIDGE_CNTL_VGA_EN_MASK        0x8

// Bitfield Description : For a Port with VGA Enable set, enables the bridge to provide 16-bit decoding of VGA I/O addresses, rather than 10-bit decoding.
#define BRIDGE_CNTL_VGA_DEC_OFFSET      4
#define BRIDGE_CNTL_VGA_DEC_MASK        0x10

// Bitfield Description : Does not apply to PCI Express.
#define BRIDGE_CNTL_MASTER_ABORT_MODE_OFFSET      5
#define BRIDGE_CNTL_MASTER_ABORT_MODE_MASK        0x20

// Bitfield Description : Triggers a hot reset on the corresponding Port.
#define BRIDGE_CNTL_SECONDARY_BUS_RESET_OFFSET      6
#define BRIDGE_CNTL_SECONDARY_BUS_RESET_MASK        0x40

// Bitfield Description : Does not apply to PCI Express.
#define BRIDGE_CNTL_FAST_B2B_EN_OFFSET      7
#define BRIDGE_CNTL_FAST_B2B_EN_MASK        0x80

// Bitfield Description : Does not apply to PCI Express.
#define BRIDGE_CNTL_PRIMARY_DISCARD_TIMER_OFFSET      8
#define BRIDGE_CNTL_PRIMARY_DISCARD_TIMER_MASK        0x100

// Bitfield Description : Does not apply to PCI Express.
#define BRIDGE_CNTL_SECONDARY_DISCARD_TIMER_OFFSET      9
#define BRIDGE_CNTL_SECONDARY_DISCARD_TIMER_MASK        0x200

// Bitfield Description : Does not apply to PCI Express.
#define BRIDGE_CNTL_DISCARD_TIMER_STATUS_OFFSET      10
#define BRIDGE_CNTL_DISCARD_TIMER_STATUS_MASK        0x400

// Bitfield Description : Does not apply to PCI Express.
#define BRIDGE_CNTL_DISCARD_TIMER_SERR_ENABLE_OFFSET      11
#define BRIDGE_CNTL_DISCARD_TIMER_SERR_ENABLE_MASK        0x800

// Bitfield Description : 
#define BRIDGE_CNTL_Reserved_15_12_OFFSET      12
#define BRIDGE_CNTL_Reserved_15_12_MASK        0xf000

typedef union {
  struct {
    UINT16                            PARITY_RESPONSE_EN:1;
    UINT16                            SERR_EN:1;
    UINT16                            ISA_EN:1;
    UINT16                            VGA_EN:1;
    UINT16                            VGA_DEC:1;
    UINT16                            MASTER_ABORT_MODE:1;
    UINT16                            SECONDARY_BUS_RESET:1;
    UINT16                            FAST_B2B_EN:1;
    UINT16                            PRIMARY_DISCARD_TIMER:1;
    UINT16                            SECONDARY_DISCARD_TIMER:1;
    UINT16                            DISCARD_TIMER_STATUS:1;
    UINT16                            DISCARD_TIMER_SERR_ENABLE:1;
    UINT16                            Reserved_15_12:4;
  } Field;
  UINT16 Value;
} BRIDGE_CNTL_STRUCT;

#define SMN_FUNC0_PCIE0_BRIDGE_CNTL_ADDRESS    0x1110003eUL

#define SMN_FUNC0_PCIE1_BRIDGE_CNTL_ADDRESS    0x1120003eUL

#define SMN_FUNC0_PCIE2_BRIDGE_CNTL_ADDRESS    0x1130003eUL

#define SMN_FUNC0_PCIE3_BRIDGE_CNTL_ADDRESS    0x1140003eUL

#define SMN_FUNC1_PCIE0_BRIDGE_CNTL_ADDRESS    0x1110103eUL

#define SMN_FUNC1_PCIE1_BRIDGE_CNTL_ADDRESS    0x1120103eUL

#define SMN_FUNC2_PCIE0_BRIDGE_CNTL_ADDRESS    0x1110203eUL

#define SMN_FUNC2_PCIE1_BRIDGE_CNTL_ADDRESS    0x1120203eUL

#define SMN_FUNC3_PCIE0_BRIDGE_CNTL_ADDRESS    0x1110303eUL

#define SMN_FUNC3_PCIE1_BRIDGE_CNTL_ADDRESS    0x1120303eUL

#define SMN_FUNC4_PCIE0_BRIDGE_CNTL_ADDRESS    0x1110403eUL

#define SMN_FUNC4_PCIE1_BRIDGE_CNTL_ADDRESS    0x1120403eUL

#define SMN_FUNC5_PCIE1_BRIDGE_CNTL_ADDRESS    0x1120503eUL


/***********************************************************
* Register Name : CACHE_LINE
* Register Description :
* Cache Line Size register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Records the system cacheline size in units of DWORDs.
#define CACHE_LINE_CACHE_LINE_SIZE_OFFSET      0
#define CACHE_LINE_CACHE_LINE_SIZE_MASK        0xff

typedef union {
  struct {
    UINT8                            CACHE_LINE_SIZE:8;
  } Field;
  UINT8 Value;
} CACHE_LINE_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_CACHE_LINE_ADDRESS    0x1110000cUL

#define SMN_FUNC0_PCIE1_CACHE_LINE_ADDRESS    0x1120000cUL

#define SMN_FUNC0_PCIE2_CACHE_LINE_ADDRESS    0x1130000cUL

#define SMN_FUNC0_PCIE3_CACHE_LINE_ADDRESS    0x1140000cUL

#define SMN_FUNC1_PCIE0_CACHE_LINE_ADDRESS    0x1110100cUL

#define SMN_FUNC1_PCIE1_CACHE_LINE_ADDRESS    0x1120100cUL

#define SMN_FUNC2_PCIE0_CACHE_LINE_ADDRESS    0x1110200cUL

#define SMN_FUNC2_PCIE1_CACHE_LINE_ADDRESS    0x1120200cUL

#define SMN_FUNC3_PCIE0_CACHE_LINE_ADDRESS    0x1110300cUL

#define SMN_FUNC3_PCIE1_CACHE_LINE_ADDRESS    0x1120300cUL

#define SMN_FUNC4_PCIE0_CACHE_LINE_ADDRESS    0x1110400cUL

#define SMN_FUNC4_PCIE1_CACHE_LINE_ADDRESS    0x1120400cUL

#define SMN_FUNC5_PCIE1_CACHE_LINE_ADDRESS    0x1120500cUL


/***********************************************************
* Register Name : CAP_PTR
* Register Description :
* Capability Pointer.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Pointer to a linked list of additional capabilities implemented by this function, or 00h if no such items exist.
#define CAP_PTR_CAP_PTR_OFFSET      0
#define CAP_PTR_CAP_PTR_MASK        0xff

typedef union {
  struct {
    UINT8                            CAP_PTR:8;
  } Field;
  UINT8 Value;
} CAP_PTR_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_CAP_PTR_ADDRESS    0x11100034UL

#define SMN_FUNC0_PCIE1_CAP_PTR_ADDRESS    0x11200034UL

#define SMN_FUNC0_PCIE2_CAP_PTR_ADDRESS    0x11300034UL

#define SMN_FUNC0_PCIE3_CAP_PTR_ADDRESS    0x11400034UL

#define SMN_FUNC1_PCIE0_CAP_PTR_ADDRESS    0x11101034UL

#define SMN_FUNC1_PCIE1_CAP_PTR_ADDRESS    0x11201034UL

#define SMN_FUNC2_PCIE0_CAP_PTR_ADDRESS    0x11102034UL

#define SMN_FUNC2_PCIE1_CAP_PTR_ADDRESS    0x11202034UL

#define SMN_FUNC3_PCIE0_CAP_PTR_ADDRESS    0x11103034UL

#define SMN_FUNC3_PCIE1_CAP_PTR_ADDRESS    0x11203034UL

#define SMN_FUNC4_PCIE0_CAP_PTR_ADDRESS    0x11104034UL

#define SMN_FUNC4_PCIE1_CAP_PTR_ADDRESS    0x11204034UL

#define SMN_FUNC5_PCIE1_CAP_PTR_ADDRESS    0x11205034UL


/***********************************************************
* Register Name : COMMAND
* Register Description :
* Provides control over a function's ability to generate and respond to bus cycles.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the ability of a function to accept I/O Space accesses.
#define COMMAND_IO_ACCESS_EN_OFFSET      0
#define COMMAND_IO_ACCESS_EN_MASK        0x1

// Bitfield Description : Controls the ability of a function to accept Memory Space accesses.
#define COMMAND_MEM_ACCESS_EN_OFFSET      1
#define COMMAND_MEM_ACCESS_EN_MASK        0x2

// Bitfield Description : Controls the ability of a function to issue Memory and I/O Read/Write Requests, and the ability of a Port to forward Memory and I/O Read/Write Requests in the upstream direction.
#define COMMAND_BUS_MASTER_EN_OFFSET      2
#define COMMAND_BUS_MASTER_EN_MASK        0x4

// Bitfield Description : Does not apply to PCI Express.
#define COMMAND_SPECIAL_CYCLE_EN_OFFSET      3
#define COMMAND_SPECIAL_CYCLE_EN_MASK        0x8

// Bitfield Description : Does not apply to PCI Express.
#define COMMAND_MEM_WRITE_INVALIDATE_EN_OFFSET      4
#define COMMAND_MEM_WRITE_INVALIDATE_EN_MASK        0x10

// Bitfield Description : Does not apply to PCI Express.
#define COMMAND_PAL_SNOOP_EN_OFFSET      5
#define COMMAND_PAL_SNOOP_EN_MASK        0x20

// Bitfield Description : Controls the ability of a function to log errors in the Master Data Parity Error bit of the Status register.
#define COMMAND_PARITY_ERROR_RESPONSE_OFFSET      6
#define COMMAND_PARITY_ERROR_RESPONSE_MASK        0x40

// Bitfield Description : Does not apply to PCI Express.
#define COMMAND_AD_STEPPING_OFFSET      7
#define COMMAND_AD_STEPPING_MASK        0x80

// Bitfield Description : Controls the ability of a function to signal upstream, Non-fatal and Fatal errors detected by the function, and the ability of a Port to forward upstream, Non-fatal and Fatal signals received by the Port's secondary interface.
#define COMMAND_SERR_EN_OFFSET      8
#define COMMAND_SERR_EN_MASK        0x100

// Bitfield Description : Does not apply to PCI Express.
#define COMMAND_FAST_B2B_EN_OFFSET      9
#define COMMAND_FAST_B2B_EN_MASK        0x200

// Bitfield Description : Controls the ability of a function to generate INTx interrupt Messages.
#define COMMAND_INT_DIS_OFFSET      10
#define COMMAND_INT_DIS_MASK        0x400

// Bitfield Description : 
#define COMMAND_Reserved_15_11_OFFSET      11
#define COMMAND_Reserved_15_11_MASK        0xf800

typedef union {
  struct {
    UINT16                            IO_ACCESS_EN:1;
    UINT16                            MEM_ACCESS_EN:1;
    UINT16                            BUS_MASTER_EN:1;
    UINT16                            SPECIAL_CYCLE_EN:1;
    UINT16                            MEM_WRITE_INVALIDATE_EN:1;
    UINT16                            PAL_SNOOP_EN:1;
    UINT16                            PARITY_ERROR_RESPONSE:1;
    UINT16                            AD_STEPPING:1;
    UINT16                            SERR_EN:1;
    UINT16                            FAST_B2B_EN:1;
    UINT16                            INT_DIS:1;
    UINT16                            Reserved_15_11:5;
  } Field;
  UINT16 Value;
} COMMAND_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_COMMAND_ADDRESS    0x11100004UL

#define SMN_FUNC0_PCIE1_COMMAND_ADDRESS    0x11200004UL

#define SMN_FUNC0_PCIE2_COMMAND_ADDRESS    0x11300004UL

#define SMN_FUNC0_PCIE3_COMMAND_ADDRESS    0x11400004UL

#define SMN_FUNC1_PCIE0_COMMAND_ADDRESS    0x11101004UL

#define SMN_FUNC1_PCIE1_COMMAND_ADDRESS    0x11201004UL

#define SMN_FUNC2_PCIE0_COMMAND_ADDRESS    0x11102004UL

#define SMN_FUNC2_PCIE1_COMMAND_ADDRESS    0x11202004UL

#define SMN_FUNC3_PCIE0_COMMAND_ADDRESS    0x11103004UL

#define SMN_FUNC3_PCIE1_COMMAND_ADDRESS    0x11203004UL

#define SMN_FUNC4_PCIE0_COMMAND_ADDRESS    0x11104004UL

#define SMN_FUNC4_PCIE1_COMMAND_ADDRESS    0x11204004UL

#define SMN_FUNC5_PCIE1_COMMAND_ADDRESS    0x11205004UL


/***********************************************************
* Register Name : DATA_LINK_FEATURE_CAP
* Register Description :
* Data Link Feature Capabilities register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates which Data Link Features the Port supports. This bit is the lower part of a field that is a bit vector that covers all supported features: Bit 0 is Scaled Flow Control Supported. If Data Link Feature Exchange Enable bit is set, the entire bit vector's value is sent in the Port's transmitted Data Link Feature DLLP.
#define DATA_LINK_FEATURE_CAP_LOCAL_DLF_SCALED_FLOW_CONTROL_SUPPORTED_OFFSET      0
#define DATA_LINK_FEATURE_CAP_LOCAL_DLF_SCALED_FLOW_CONTROL_SUPPORTED_MASK        0x1

// Bitfield Description : Indicates which Data Link Features the Port supports. This bit is the upper part of a field that is a bit vector that covers all supported features. If Data Link Feature Exchange Enable bit is set, the entire bit vector's value is sent in the Port's transmitted Data Link Feature DLLP.
#define DATA_LINK_FEATURE_CAP_LOCAL_DLF_SUPPORTED_22_1_OFFSET      1
#define DATA_LINK_FEATURE_CAP_LOCAL_DLF_SUPPORTED_22_1_MASK        0x7ffffe

// Bitfield Description : 
#define DATA_LINK_FEATURE_CAP_Reserved_30_23_OFFSET      23
#define DATA_LINK_FEATURE_CAP_Reserved_30_23_MASK        0x7f800000

// Bitfield Description : Indicates if the Port will perform the Data Link Feature Exchange during Data Link Layer activation.
#define DATA_LINK_FEATURE_CAP_DLF_EXCHANGE_ENABLE_OFFSET      31
#define DATA_LINK_FEATURE_CAP_DLF_EXCHANGE_ENABLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            LOCAL_DLF_SCALED_FLOW_CONTROL_SUPPORTED:1;
    UINT32                            LOCAL_DLF_SUPPORTED_22_1:22;
    UINT32                            Reserved_30_23:8;
    UINT32                            DLF_EXCHANGE_ENABLE:1;
  } Field;
  UINT32 Value;
} DATA_LINK_FEATURE_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_DATA_LINK_FEATURE_CAP_ADDRESS    0x11100404UL

#define SMN_FUNC0_PCIE1_DATA_LINK_FEATURE_CAP_ADDRESS    0x11200404UL

#define SMN_FUNC0_PCIE2_DATA_LINK_FEATURE_CAP_ADDRESS    0x11300404UL

#define SMN_FUNC0_PCIE3_DATA_LINK_FEATURE_CAP_ADDRESS    0x11400404UL

#define SMN_FUNC1_PCIE0_DATA_LINK_FEATURE_CAP_ADDRESS    0x11101404UL

#define SMN_FUNC1_PCIE1_DATA_LINK_FEATURE_CAP_ADDRESS    0x11201404UL

#define SMN_FUNC2_PCIE0_DATA_LINK_FEATURE_CAP_ADDRESS    0x11102404UL

#define SMN_FUNC2_PCIE1_DATA_LINK_FEATURE_CAP_ADDRESS    0x11202404UL

#define SMN_FUNC3_PCIE0_DATA_LINK_FEATURE_CAP_ADDRESS    0x11103404UL

#define SMN_FUNC3_PCIE1_DATA_LINK_FEATURE_CAP_ADDRESS    0x11203404UL

#define SMN_FUNC4_PCIE0_DATA_LINK_FEATURE_CAP_ADDRESS    0x11104404UL

#define SMN_FUNC4_PCIE1_DATA_LINK_FEATURE_CAP_ADDRESS    0x11204404UL

#define SMN_FUNC5_PCIE1_DATA_LINK_FEATURE_CAP_ADDRESS    0x11205404UL


/***********************************************************
* Register Name : DATA_LINK_FEATURE_STATUS
* Register Description :
* Data Link Feature Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Captures which Data Link Features the Remote Port supports. This field is a bit vector that covers all supported features: Bit 0 is Scaled Flow Control Supported. If Data Link Feature Exchange Enable bit is set, this field's value is from the Port's received Data Link Feature DLLP.
#define DATA_LINK_FEATURE_STATUS_REMOTE_DLF_SUPPORTED_OFFSET      0
#define DATA_LINK_FEATURE_STATUS_REMOTE_DLF_SUPPORTED_MASK        0x7fffff

// Bitfield Description : 
#define DATA_LINK_FEATURE_STATUS_Reserved_30_23_OFFSET      23
#define DATA_LINK_FEATURE_STATUS_Reserved_30_23_MASK        0x7f800000

// Bitfield Description : Indicates if the Port received a Data Link Feature DLLP during DL_Feature negotiation, containing meaningful Remote Data Link Feature Supported and Remote Data Link Feature Ack fields. The bit is cleared when the Data Link Layer enters the DL_Inactive state.
#define DATA_LINK_FEATURE_STATUS_REMOTE_DLF_SUPPORTED_VALID_OFFSET      31
#define DATA_LINK_FEATURE_STATUS_REMOTE_DLF_SUPPORTED_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            REMOTE_DLF_SUPPORTED:23;
    UINT32                            Reserved_30_23:8;
    UINT32                            REMOTE_DLF_SUPPORTED_VALID:1;
  } Field;
  UINT32 Value;
} DATA_LINK_FEATURE_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11100408UL

#define SMN_FUNC0_PCIE1_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11200408UL

#define SMN_FUNC0_PCIE2_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11300408UL

#define SMN_FUNC0_PCIE3_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11400408UL

#define SMN_FUNC1_PCIE0_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11101408UL

#define SMN_FUNC1_PCIE1_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11201408UL

#define SMN_FUNC2_PCIE0_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11102408UL

#define SMN_FUNC2_PCIE1_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11202408UL

#define SMN_FUNC3_PCIE0_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11103408UL

#define SMN_FUNC3_PCIE1_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11203408UL

#define SMN_FUNC4_PCIE0_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11104408UL

#define SMN_FUNC4_PCIE1_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11204408UL

#define SMN_FUNC5_PCIE1_DATA_LINK_FEATURE_STATUS_ADDRESS    0x11205408UL


/***********************************************************
* Register Name : DEVICE_CAP
* Register Description :
* The Device Capabilities register identifies device specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the maximum payload size that the function can support for TLPs.
#define DEVICE_CAP_MAX_PAYLOAD_SUPPORT_OFFSET      0
#define DEVICE_CAP_MAX_PAYLOAD_SUPPORT_MASK        0x7

// Bitfield Description : Indicates the support for use of unclaimed function numbers to extend the number of outstanding transactions allowed by logically combining unclaimed function numbers with the Tag identifier.
#define DEVICE_CAP_PHANTOM_FUNC_OFFSET      3
#define DEVICE_CAP_PHANTOM_FUNC_MASK        0x18

// Bitfield Description : When 10-Bit Tag Requester Supported bit is clear, indicates the maximum supported size of the Tag field as a Requester.
#define DEVICE_CAP_EXTENDED_TAG_OFFSET      5
#define DEVICE_CAP_EXTENDED_TAG_MASK        0x20

// Bitfield Description : For non-Endpoints this field is Reserved.
#define DEVICE_CAP_L0S_ACCEPTABLE_LATENCY_OFFSET      6
#define DEVICE_CAP_L0S_ACCEPTABLE_LATENCY_MASK        0x1c0

// Bitfield Description : For non-Endpoints this field is Reserved.
#define DEVICE_CAP_L1_ACCEPTABLE_LATENCY_OFFSET      9
#define DEVICE_CAP_L1_ACCEPTABLE_LATENCY_MASK        0xe00

// Bitfield Description : 
#define DEVICE_CAP_Reserved_14_12_OFFSET      12
#define DEVICE_CAP_Reserved_14_12_MASK        0x7000

// Bitfield Description : Indicates the function implements Role-Based Error Reporting functionality.
#define DEVICE_CAP_ROLE_BASED_ERR_REPORTING_OFFSET      15
#define DEVICE_CAP_ROLE_BASED_ERR_REPORTING_MASK        0x8000

// Bitfield Description : Indicates if the function supports the ERR_COR Subclass field in ERR_COR messages.
#define DEVICE_CAP_ERR_COR_SUBCLASS_CAPABLE_OFFSET      16
#define DEVICE_CAP_ERR_COR_SUBCLASS_CAPABLE_MASK        0x10000

// Bitfield Description : 
#define DEVICE_CAP_Reserved_17_17_OFFSET      17
#define DEVICE_CAP_Reserved_17_17_MASK        0x20000

// Bitfield Description : (Upstream Ports only) In combination with the Slot Power Limit Scale value, indicates the upper limit on power supplied by slot.
#define DEVICE_CAP_CAPTURED_SLOT_POWER_LIMIT_OFFSET      18
#define DEVICE_CAP_CAPTURED_SLOT_POWER_LIMIT_MASK        0x3fc0000

// Bitfield Description : (Upstream Ports only) Indicates the scale used for the Slot Power Limit Value.
#define DEVICE_CAP_CAPTURED_SLOT_POWER_SCALE_OFFSET      26
#define DEVICE_CAP_CAPTURED_SLOT_POWER_SCALE_MASK        0xc000000

// Bitfield Description : (Endpoints only) Indicates if the function supports Function Level Resets.
// For non-Endpoints this field is Reserved.
#define DEVICE_CAP_FLR_CAPABLE_OFFSET      28
#define DEVICE_CAP_FLR_CAPABLE_MASK        0x10000000

// Bitfield Description : 
#define DEVICE_CAP_Reserved_31_29_OFFSET      29
#define DEVICE_CAP_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            MAX_PAYLOAD_SUPPORT:3;
    UINT32                            PHANTOM_FUNC:2;
    UINT32                            EXTENDED_TAG:1;
    UINT32                            L0S_ACCEPTABLE_LATENCY:3;
    UINT32                            L1_ACCEPTABLE_LATENCY:3;
    UINT32                            Reserved_14_12:3;
    UINT32                            ROLE_BASED_ERR_REPORTING:1;
    UINT32                            ERR_COR_SUBCLASS_CAPABLE:1;
    UINT32                            Reserved_17_17:1;
    UINT32                            CAPTURED_SLOT_POWER_LIMIT:8;
    UINT32                            CAPTURED_SLOT_POWER_SCALE:2;
    UINT32                            FLR_CAPABLE:1;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} DEVICE_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_DEVICE_CAP_ADDRESS    0x1110005cUL

#define SMN_FUNC0_PCIE1_DEVICE_CAP_ADDRESS    0x1120005cUL

#define SMN_FUNC0_PCIE2_DEVICE_CAP_ADDRESS    0x1130005cUL

#define SMN_FUNC0_PCIE3_DEVICE_CAP_ADDRESS    0x1140005cUL

#define SMN_FUNC1_PCIE0_DEVICE_CAP_ADDRESS    0x1110105cUL

#define SMN_FUNC1_PCIE1_DEVICE_CAP_ADDRESS    0x1120105cUL

#define SMN_FUNC2_PCIE0_DEVICE_CAP_ADDRESS    0x1110205cUL

#define SMN_FUNC2_PCIE1_DEVICE_CAP_ADDRESS    0x1120205cUL

#define SMN_FUNC3_PCIE0_DEVICE_CAP_ADDRESS    0x1110305cUL

#define SMN_FUNC3_PCIE1_DEVICE_CAP_ADDRESS    0x1120305cUL

#define SMN_FUNC4_PCIE0_DEVICE_CAP_ADDRESS    0x1110405cUL

#define SMN_FUNC4_PCIE1_DEVICE_CAP_ADDRESS    0x1120405cUL

#define SMN_FUNC5_PCIE1_DEVICE_CAP_ADDRESS    0x1120505cUL


/***********************************************************
* Register Name : DEVICE_CAP2
* Register Description :
* The Device Capabilities 2 register identifies device specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if a Requester supports the Completion Timeout programmability mechanism. For functions that don't issue requests, this field is Reserved.
#define DEVICE_CAP2_CPL_TIMEOUT_RANGE_SUPPORTED_OFFSET      0
#define DEVICE_CAP2_CPL_TIMEOUT_RANGE_SUPPORTED_MASK        0xf

// Bitfield Description : Indicates if a Requester supports the Completion Timeout disable mechanism. For functions that don't issue requests, this field is Reserved.
#define DEVICE_CAP2_CPL_TIMEOUT_DIS_SUPPORTED_OFFSET      4
#define DEVICE_CAP2_CPL_TIMEOUT_DIS_SUPPORTED_MASK        0x10

// Bitfield Description : (Downstream Ports only) Indicates if a Port supports ARI forwarding.
// For Upstream Ports, this bit is Reserved.
#define DEVICE_CAP2_ARI_FORWARDING_SUPPORTED_OFFSET      5
#define DEVICE_CAP2_ARI_FORWARDING_SUPPORTED_MASK        0x20

// Bitfield Description : (Downstream Ports or Switch Upstream Ports only) Indicates if a Port supports AtomicOp routing.
// For Endpoints, this bit is Reserved.
#define DEVICE_CAP2_ATOMICOP_ROUTING_SUPPORTED_OFFSET      6
#define DEVICE_CAP2_ATOMICOP_ROUTING_SUPPORTED_MASK        0x40

// Bitfield Description : Indicates if a function supports being a 32-bit AtomicOp completer.
#define DEVICE_CAP2_ATOMICOP_32CMPLT_SUPPORTED_OFFSET      7
#define DEVICE_CAP2_ATOMICOP_32CMPLT_SUPPORTED_MASK        0x80

// Bitfield Description : Indicates if a function supports being a 64-bit AtomicOp completer.
#define DEVICE_CAP2_ATOMICOP_64CMPLT_SUPPORTED_OFFSET      8
#define DEVICE_CAP2_ATOMICOP_64CMPLT_SUPPORTED_MASK        0x100

// Bitfield Description : Indicates if a function supports being a 128-bit AtomicOp completer.
#define DEVICE_CAP2_CAS128_CMPLT_SUPPORTED_OFFSET      9
#define DEVICE_CAP2_CAS128_CMPLT_SUPPORTED_MASK        0x200

// Bitfield Description : (Downstream Ports or Switch Upstream Ports only) Indicates if a routing element supports never allowing posted requests with Relaxed Ordering attribute bit set, to pass each other.
// For Endpoints, this bit is Reserved.
#define DEVICE_CAP2_NO_RO_ENABLED_P2P_PASSING_OFFSET      10
#define DEVICE_CAP2_NO_RO_ENABLED_P2P_PASSING_MASK        0x400

// Bitfield Description : Indicates if the function supports the Latency Tolerance Reporting mechanism.
#define DEVICE_CAP2_LTR_SUPPORTED_OFFSET      11
#define DEVICE_CAP2_LTR_SUPPORTED_MASK        0x800

// Bitfield Description : For Root Ports and Endpoints, indicates if the Completer supports TPH or Extended TPH. For all other functions, this field is Reserved.
#define DEVICE_CAP2_TPH_CPLR_SUPPORTED_OFFSET      12
#define DEVICE_CAP2_TPH_CPLR_SUPPORTED_MASK        0x3000

// Bitfield Description : For Root Ports, indicates if the Root Port supports LN protocol as an LN Completer.
#define DEVICE_CAP2_LN_SYSTEM_CLS_OFFSET      14
#define DEVICE_CAP2_LN_SYSTEM_CLS_MASK        0xc000

// Bitfield Description : Indicates the maximum supported size of the Tag field as a Completer.
#define DEVICE_CAP2_TEN_BIT_TAG_COMPLETER_SUPPORTED_OFFSET      16
#define DEVICE_CAP2_TEN_BIT_TAG_COMPLETER_SUPPORTED_MASK        0x10000

// Bitfield Description : Indicates the maximum supported size of the Tag field as a Requester.
#define DEVICE_CAP2_TEN_BIT_TAG_REQUESTER_SUPPORTED_OFFSET      17
#define DEVICE_CAP2_TEN_BIT_TAG_REQUESTER_SUPPORTED_MASK        0x20000

// Bitfield Description : Indicates if the function supports Optimized Buffer Flush/Fill signaling.
#define DEVICE_CAP2_OBFF_SUPPORTED_OFFSET      18
#define DEVICE_CAP2_OBFF_SUPPORTED_MASK        0xc0000

// Bitfield Description : Indicates if the function supports the 3-bit definition of the Fmt field. When not set, the function supports only the 2-bit definition of the Fmt field.
#define DEVICE_CAP2_EXTENDED_FMT_FIELD_SUPPORTED_OFFSET      20
#define DEVICE_CAP2_EXTENDED_FMT_FIELD_SUPPORTED_MASK        0x100000

// Bitfield Description : Indicates if the function supports receiving End-End TLP Prefixes.
#define DEVICE_CAP2_END_END_TLP_PREFIX_SUPPORTED_OFFSET      21
#define DEVICE_CAP2_END_END_TLP_PREFIX_SUPPORTED_MASK        0x200000

// Bitfield Description : Indicates the maximum number of End-End TLP Prefixes supported by a function with End-End TLP Prefix Supported set. If End-End TLP Prefix Supported bit is not set, this field is Reserved.
#define DEVICE_CAP2_MAX_END_END_TLP_PREFIXES_OFFSET      22
#define DEVICE_CAP2_MAX_END_END_TLP_PREFIXES_MASK        0xc00000

// Bitfield Description : (Upstream Ports only) Indicates if the function supports the Emergency Power Reduction mechanism.
// For Downstream Ports, this bit is Reserved.
#define DEVICE_CAP2_EMER_POWER_REDUCTION_SUPPORTED_OFFSET      24
#define DEVICE_CAP2_EMER_POWER_REDUCTION_SUPPORTED_MASK        0x3000000

// Bitfield Description : (Upstream Ports only) Indicates if the function requires software initialization upon exiting the Emergency Power Reduction state.
// For Downstream Ports, this bit is Reserved.
#define DEVICE_CAP2_EMER_POWER_REDUCTION_INIT_REQ_OFFSET      26
#define DEVICE_CAP2_EMER_POWER_REDUCTION_INIT_REQ_MASK        0x4000000

// Bitfield Description : 
#define DEVICE_CAP2_Reserved_30_27_OFFSET      27
#define DEVICE_CAP2_Reserved_30_27_MASK        0x78000000

// Bitfield Description : Indicates if the function supports Function Readiness Status capability.
#define DEVICE_CAP2_FRS_SUPPORTED_OFFSET      31
#define DEVICE_CAP2_FRS_SUPPORTED_MASK        0x80000000

typedef union {
  struct {
    UINT32                            CPL_TIMEOUT_RANGE_SUPPORTED:4;
    UINT32                            CPL_TIMEOUT_DIS_SUPPORTED:1;
    UINT32                            ARI_FORWARDING_SUPPORTED:1;
    UINT32                            ATOMICOP_ROUTING_SUPPORTED:1;
    UINT32                            ATOMICOP_32CMPLT_SUPPORTED:1;
    UINT32                            ATOMICOP_64CMPLT_SUPPORTED:1;
    UINT32                            CAS128_CMPLT_SUPPORTED:1;
    UINT32                            NO_RO_ENABLED_P2P_PASSING:1;
    UINT32                            LTR_SUPPORTED:1;
    UINT32                            TPH_CPLR_SUPPORTED:2;
    UINT32                            LN_SYSTEM_CLS:2;
    UINT32                            TEN_BIT_TAG_COMPLETER_SUPPORTED:1;
    UINT32                            TEN_BIT_TAG_REQUESTER_SUPPORTED:1;
    UINT32                            OBFF_SUPPORTED:2;
    UINT32                            EXTENDED_FMT_FIELD_SUPPORTED:1;
    UINT32                            END_END_TLP_PREFIX_SUPPORTED:1;
    UINT32                            MAX_END_END_TLP_PREFIXES:2;
    UINT32                            EMER_POWER_REDUCTION_SUPPORTED:2;
    UINT32                            EMER_POWER_REDUCTION_INIT_REQ:1;
    UINT32                            Reserved_30_27:4;
    UINT32                            FRS_SUPPORTED:1;
  } Field;
  UINT32 Value;
} DEVICE_CAP2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_DEVICE_CAP2_ADDRESS    0x1110007cUL

#define SMN_FUNC0_PCIE1_DEVICE_CAP2_ADDRESS    0x1120007cUL

#define SMN_FUNC0_PCIE2_DEVICE_CAP2_ADDRESS    0x1130007cUL

#define SMN_FUNC0_PCIE3_DEVICE_CAP2_ADDRESS    0x1140007cUL

#define SMN_FUNC1_PCIE0_DEVICE_CAP2_ADDRESS    0x1110107cUL

#define SMN_FUNC1_PCIE1_DEVICE_CAP2_ADDRESS    0x1120107cUL

#define SMN_FUNC2_PCIE0_DEVICE_CAP2_ADDRESS    0x1110207cUL

#define SMN_FUNC2_PCIE1_DEVICE_CAP2_ADDRESS    0x1120207cUL

#define SMN_FUNC3_PCIE0_DEVICE_CAP2_ADDRESS    0x1110307cUL

#define SMN_FUNC3_PCIE1_DEVICE_CAP2_ADDRESS    0x1120307cUL

#define SMN_FUNC4_PCIE0_DEVICE_CAP2_ADDRESS    0x1110407cUL

#define SMN_FUNC4_PCIE1_DEVICE_CAP2_ADDRESS    0x1120407cUL

#define SMN_FUNC5_PCIE1_DEVICE_CAP2_ADDRESS    0x1120507cUL


/***********************************************************
* Register Name : DEVICE_CNTL
* Register Description :
* The Device Control register controls device specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls signaling of correctable errors.
#define DEVICE_CNTL_CORR_ERR_EN_OFFSET      0
#define DEVICE_CNTL_CORR_ERR_EN_MASK        0x1

// Bitfield Description : Controls signaling of Non-fatal errors.
#define DEVICE_CNTL_NON_FATAL_ERR_EN_OFFSET      1
#define DEVICE_CNTL_NON_FATAL_ERR_EN_MASK        0x2

// Bitfield Description : Controls signaling of Fatal errors.
#define DEVICE_CNTL_FATAL_ERR_EN_OFFSET      2
#define DEVICE_CNTL_FATAL_ERR_EN_MASK        0x4

// Bitfield Description : Controls signaling of Unsupported Requests.
#define DEVICE_CNTL_USR_REPORT_EN_OFFSET      3
#define DEVICE_CNTL_USR_REPORT_EN_MASK        0x8

// Bitfield Description : Controls if the function is permitted to set the Relaxed Ordering bit in the Attributes field of transactions it initiates that do not require strong write ordering.
#define DEVICE_CNTL_RELAXED_ORD_EN_OFFSET      4
#define DEVICE_CNTL_RELAXED_ORD_EN_MASK        0x10

// Bitfield Description : Controls the maximum TLP payload size for the function.
#define DEVICE_CNTL_MAX_PAYLOAD_SIZE_OFFSET      5
#define DEVICE_CNTL_MAX_PAYLOAD_SIZE_MASK        0xe0

// Bitfield Description : When 10-Bit Tag Requester Enable bit is clear, controls if a function is permitted to use an 8-bit Tag field as a Requester. When both bits are disabled the function is restricted to use a 5-bit Tag field.
#define DEVICE_CNTL_EXTENDED_TAG_EN_OFFSET      8
#define DEVICE_CNTL_EXTENDED_TAG_EN_MASK        0x100

// Bitfield Description : When 10-Bit Tag Requester Enable bit is clear, controls if a function is permitted to use unclaimed function numbers as Phantom Functions to extend the number of outstanding transaction identifiers.
#define DEVICE_CNTL_PHANTOM_FUNC_EN_OFFSET      9
#define DEVICE_CNTL_PHANTOM_FUNC_EN_MASK        0x200

// Bitfield Description : Controls if a function is permitted to draw AUX power independent of PME AUX power.
#define DEVICE_CNTL_AUX_POWER_PM_EN_OFFSET      10
#define DEVICE_CNTL_AUX_POWER_PM_EN_MASK        0x400

// Bitfield Description : Controls if the function is permitted to set the No Snoop bit in the Requester Attributes of transactions it initiates that do not require hardware enforced cache coherency.
#define DEVICE_CNTL_NO_SNOOP_EN_OFFSET      11
#define DEVICE_CNTL_NO_SNOOP_EN_MASK        0x800

// Bitfield Description : Controls the maximum Read Request size for the function as a Requester.
#define DEVICE_CNTL_MAX_READ_REQUEST_SIZE_OFFSET      12
#define DEVICE_CNTL_MAX_READ_REQUEST_SIZE_MASK        0x7000

// Bitfield Description : (Bridges only) Controls if a PCI Express to PCI/PCI-X Bridge is permitted to return Configuration Retry Status.
// (Endpoints only) A write of 1 to this bit initiates Function Level Reset to the function. Reads always return 0.
// For non-Bridges and non-Endpoints this bit is Reserved.
#define DEVICE_CNTL_BRIDGE_CFG_RETRY_EN_OFFSET      15
#define DEVICE_CNTL_BRIDGE_CFG_RETRY_EN_MASK        0x8000

typedef union {
  struct {
    UINT16                            CORR_ERR_EN:1;
    UINT16                            NON_FATAL_ERR_EN:1;
    UINT16                            FATAL_ERR_EN:1;
    UINT16                            USR_REPORT_EN:1;
    UINT16                            RELAXED_ORD_EN:1;
    UINT16                            MAX_PAYLOAD_SIZE:3;
    UINT16                            EXTENDED_TAG_EN:1;
    UINT16                            PHANTOM_FUNC_EN:1;
    UINT16                            AUX_POWER_PM_EN:1;
    UINT16                            NO_SNOOP_EN:1;
    UINT16                            MAX_READ_REQUEST_SIZE:3;
    UINT16                            BRIDGE_CFG_RETRY_EN:1;
  } Field;
  UINT16 Value;
} DEVICE_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_DEVICE_CNTL_ADDRESS    0x11100060UL

#define SMN_FUNC0_PCIE1_DEVICE_CNTL_ADDRESS    0x11200060UL

#define SMN_FUNC0_PCIE2_DEVICE_CNTL_ADDRESS    0x11300060UL

#define SMN_FUNC0_PCIE3_DEVICE_CNTL_ADDRESS    0x11400060UL

#define SMN_FUNC1_PCIE0_DEVICE_CNTL_ADDRESS    0x11101060UL

#define SMN_FUNC1_PCIE1_DEVICE_CNTL_ADDRESS    0x11201060UL

#define SMN_FUNC2_PCIE0_DEVICE_CNTL_ADDRESS    0x11102060UL

#define SMN_FUNC2_PCIE1_DEVICE_CNTL_ADDRESS    0x11202060UL

#define SMN_FUNC3_PCIE0_DEVICE_CNTL_ADDRESS    0x11103060UL

#define SMN_FUNC3_PCIE1_DEVICE_CNTL_ADDRESS    0x11203060UL

#define SMN_FUNC4_PCIE0_DEVICE_CNTL_ADDRESS    0x11104060UL

#define SMN_FUNC4_PCIE1_DEVICE_CNTL_ADDRESS    0x11204060UL

#define SMN_FUNC5_PCIE1_DEVICE_CNTL_ADDRESS    0x11205060UL


/***********************************************************
* Register Name : DEVICE_CNTL2
* Register Description :
* The Device Control 2 register controls device specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the completion timeout value for the function.
#define DEVICE_CNTL2_CPL_TIMEOUT_VALUE_OFFSET      0
#define DEVICE_CNTL2_CPL_TIMEOUT_VALUE_MASK        0xf

// Bitfield Description : Controls the ability of a function to use the completion timeout mechanism.
#define DEVICE_CNTL2_CPL_TIMEOUT_DIS_OFFSET      4
#define DEVICE_CNTL2_CPL_TIMEOUT_DIS_MASK        0x10

// Bitfield Description : (Downstream Ports only) Controls the ability of a Port to allow Configuration requests for Device Numbers greater than 0, when converting from a Type 1 Configuration cycle to a Type 0 Configuration cycle.
// For Upstream Ports, this bit is Reserved.
#define DEVICE_CNTL2_ARI_FORWARDING_EN_OFFSET      5
#define DEVICE_CNTL2_ARI_FORWARDING_EN_MASK        0x20

// Bitfield Description : Controls the ability of a function to initiate AtomicOp requests.
#define DEVICE_CNTL2_ATOMICOP_REQUEST_EN_OFFSET      6
#define DEVICE_CNTL2_ATOMICOP_REQUEST_EN_MASK        0x40

// Bitfield Description : (Downstream Ports or Switch Upstream Ports only) Controls the ability of a Port to block AtomicOps that target going out of this Egress Port.
// For Endpoints, this bit is Reserved.
#define DEVICE_CNTL2_ATOMICOP_EGRESS_BLOCKING_OFFSET      7
#define DEVICE_CNTL2_ATOMICOP_EGRESS_BLOCKING_MASK        0x80

// Bitfield Description : Controls if the function is permitted to set the IDO bit in the Attributes field of requests it initiates.
#define DEVICE_CNTL2_IDO_REQUEST_ENABLE_OFFSET      8
#define DEVICE_CNTL2_IDO_REQUEST_ENABLE_MASK        0x100

// Bitfield Description : Controls if the function is permitted to set the IDO bit in the Attributes field of completions it returns.
#define DEVICE_CNTL2_IDO_COMPLETION_ENABLE_OFFSET      9
#define DEVICE_CNTL2_IDO_COMPLETION_ENABLE_MASK        0x200

// Bitfield Description : Controls if the Port is permitted to use the Latency Tolerance Reporting mechanism.
#define DEVICE_CNTL2_LTR_EN_OFFSET      10
#define DEVICE_CNTL2_LTR_EN_MASK        0x400

// Bitfield Description : (Upstream Ports only) Triggers the function to enter the Emergency Power Reduction state.
// For Downstream Ports, this bit is Reserved.
#define DEVICE_CNTL2_EMER_POWER_REDUCTION_REQUEST_OFFSET      11
#define DEVICE_CNTL2_EMER_POWER_REDUCTION_REQUEST_MASK        0x800

// Bitfield Description : Controls if a function is permitted to use a 10-bit Tag field as a Requester. When disabled the function uses the setting in the Extended Tag Field Enable bit.
#define DEVICE_CNTL2_TEN_BIT_TAG_REQUESTER_ENABLE_OFFSET      12
#define DEVICE_CNTL2_TEN_BIT_TAG_REQUESTER_ENABLE_MASK        0x1000

// Bitfield Description : Controls if the function is permitted to use Optimized Buffer Flush/Fill signaling.
#define DEVICE_CNTL2_OBFF_EN_OFFSET      13
#define DEVICE_CNTL2_OBFF_EN_MASK        0x6000

// Bitfield Description : (Downstream Ports or Switch Upstream Ports only) Controls the ability of a Port to block TLPs containing End-End TLP Prefixes that target going out of this Egress Port.
// For Endpoints, this bit is Reserved.
#define DEVICE_CNTL2_END_END_TLP_PREFIX_BLOCKING_OFFSET      15
#define DEVICE_CNTL2_END_END_TLP_PREFIX_BLOCKING_MASK        0x8000

typedef union {
  struct {
    UINT16                            CPL_TIMEOUT_VALUE:4;
    UINT16                            CPL_TIMEOUT_DIS:1;
    UINT16                            ARI_FORWARDING_EN:1;
    UINT16                            ATOMICOP_REQUEST_EN:1;
    UINT16                            ATOMICOP_EGRESS_BLOCKING:1;
    UINT16                            IDO_REQUEST_ENABLE:1;
    UINT16                            IDO_COMPLETION_ENABLE:1;
    UINT16                            LTR_EN:1;
    UINT16                            EMER_POWER_REDUCTION_REQUEST:1;
    UINT16                            TEN_BIT_TAG_REQUESTER_ENABLE:1;
    UINT16                            OBFF_EN:2;
    UINT16                            END_END_TLP_PREFIX_BLOCKING:1;
  } Field;
  UINT16 Value;
} DEVICE_CNTL2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_DEVICE_CNTL2_ADDRESS    0x11100080UL

#define SMN_FUNC0_PCIE1_DEVICE_CNTL2_ADDRESS    0x11200080UL

#define SMN_FUNC0_PCIE2_DEVICE_CNTL2_ADDRESS    0x11300080UL

#define SMN_FUNC0_PCIE3_DEVICE_CNTL2_ADDRESS    0x11400080UL

#define SMN_FUNC1_PCIE0_DEVICE_CNTL2_ADDRESS    0x11101080UL

#define SMN_FUNC1_PCIE1_DEVICE_CNTL2_ADDRESS    0x11201080UL

#define SMN_FUNC2_PCIE0_DEVICE_CNTL2_ADDRESS    0x11102080UL

#define SMN_FUNC2_PCIE1_DEVICE_CNTL2_ADDRESS    0x11202080UL

#define SMN_FUNC3_PCIE0_DEVICE_CNTL2_ADDRESS    0x11103080UL

#define SMN_FUNC3_PCIE1_DEVICE_CNTL2_ADDRESS    0x11203080UL

#define SMN_FUNC4_PCIE0_DEVICE_CNTL2_ADDRESS    0x11104080UL

#define SMN_FUNC4_PCIE1_DEVICE_CNTL2_ADDRESS    0x11204080UL

#define SMN_FUNC5_PCIE1_DEVICE_CNTL2_ADDRESS    0x11205080UL


/***********************************************************
* Register Name : DEVICE_ID
* Register Description :
* Device ID register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Identifies the particular device.
#define DEVICE_ID_DEVICE_ID_OFFSET      0
#define DEVICE_ID_DEVICE_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            DEVICE_ID:16;
  } Field;
  UINT16 Value;
} DEVICE_ID_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_DEVICE_ID_ADDRESS    0x11100002UL

#define SMN_FUNC0_PCIE1_DEVICE_ID_ADDRESS    0x11200002UL

#define SMN_FUNC0_PCIE2_DEVICE_ID_ADDRESS    0x11300002UL

#define SMN_FUNC0_PCIE3_DEVICE_ID_ADDRESS    0x11400002UL

#define SMN_FUNC1_PCIE0_DEVICE_ID_ADDRESS    0x11101002UL

#define SMN_FUNC1_PCIE1_DEVICE_ID_ADDRESS    0x11201002UL

#define SMN_FUNC2_PCIE0_DEVICE_ID_ADDRESS    0x11102002UL

#define SMN_FUNC2_PCIE1_DEVICE_ID_ADDRESS    0x11202002UL

#define SMN_FUNC3_PCIE0_DEVICE_ID_ADDRESS    0x11103002UL

#define SMN_FUNC3_PCIE1_DEVICE_ID_ADDRESS    0x11203002UL

#define SMN_FUNC4_PCIE0_DEVICE_ID_ADDRESS    0x11104002UL

#define SMN_FUNC4_PCIE1_DEVICE_ID_ADDRESS    0x11204002UL

#define SMN_FUNC5_PCIE1_DEVICE_ID_ADDRESS    0x11205002UL


/***********************************************************
* Register Name : DEVICE_STATUS
* Register Description :
* The Device Status register provides information about device specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates status of correctable errors detected.
#define DEVICE_STATUS_CORR_ERR_OFFSET      0
#define DEVICE_STATUS_CORR_ERR_MASK        0x1

// Bitfield Description : Indicates status of Non-fatal errors detected.
#define DEVICE_STATUS_NON_FATAL_ERR_OFFSET      1
#define DEVICE_STATUS_NON_FATAL_ERR_MASK        0x2

// Bitfield Description : Indicates status of Fatal errors detected.
#define DEVICE_STATUS_FATAL_ERR_OFFSET      2
#define DEVICE_STATUS_FATAL_ERR_MASK        0x4

// Bitfield Description : Indicates if the function received an Unsupported Request.
#define DEVICE_STATUS_USR_DETECTED_OFFSET      3
#define DEVICE_STATUS_USR_DETECTED_MASK        0x8

// Bitfield Description : Functions that require AUX power report this bit as set if AUX power is detected by the function.
#define DEVICE_STATUS_AUX_PWR_OFFSET      4
#define DEVICE_STATUS_AUX_PWR_MASK        0x10

// Bitfield Description : Indicates if the function has issued Non-Posted Requests, on its own behalf, which have not been completed.
#define DEVICE_STATUS_TRANSACTIONS_PEND_OFFSET      5
#define DEVICE_STATUS_TRANSACTIONS_PEND_MASK        0x20

// Bitfield Description : (Upstream Ports only) When Emergency Power Reduction Supported field is 1 or 2, indicates if the function has entered the Emergency Power Reduction state.
// For Downstream Ports, this bit is Reserved.
#define DEVICE_STATUS_EMER_POWER_REDUCTION_DETECTED_OFFSET      6
#define DEVICE_STATUS_EMER_POWER_REDUCTION_DETECTED_MASK        0x40

// Bitfield Description : 
#define DEVICE_STATUS_Reserved_15_7_OFFSET      7
#define DEVICE_STATUS_Reserved_15_7_MASK        0xff80

typedef union {
  struct {
    UINT16                            CORR_ERR:1;
    UINT16                            NON_FATAL_ERR:1;
    UINT16                            FATAL_ERR:1;
    UINT16                            USR_DETECTED:1;
    UINT16                            AUX_PWR:1;
    UINT16                            TRANSACTIONS_PEND:1;
    UINT16                            EMER_POWER_REDUCTION_DETECTED:1;
    UINT16                            Reserved_15_7:9;
  } Field;
  UINT16 Value;
} DEVICE_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_DEVICE_STATUS_ADDRESS    0x11100062UL

#define SMN_FUNC0_PCIE1_DEVICE_STATUS_ADDRESS    0x11200062UL

#define SMN_FUNC0_PCIE2_DEVICE_STATUS_ADDRESS    0x11300062UL

#define SMN_FUNC0_PCIE3_DEVICE_STATUS_ADDRESS    0x11400062UL

#define SMN_FUNC1_PCIE0_DEVICE_STATUS_ADDRESS    0x11101062UL

#define SMN_FUNC1_PCIE1_DEVICE_STATUS_ADDRESS    0x11201062UL

#define SMN_FUNC2_PCIE0_DEVICE_STATUS_ADDRESS    0x11102062UL

#define SMN_FUNC2_PCIE1_DEVICE_STATUS_ADDRESS    0x11202062UL

#define SMN_FUNC3_PCIE0_DEVICE_STATUS_ADDRESS    0x11103062UL

#define SMN_FUNC3_PCIE1_DEVICE_STATUS_ADDRESS    0x11203062UL

#define SMN_FUNC4_PCIE0_DEVICE_STATUS_ADDRESS    0x11104062UL

#define SMN_FUNC4_PCIE1_DEVICE_STATUS_ADDRESS    0x11204062UL

#define SMN_FUNC5_PCIE1_DEVICE_STATUS_ADDRESS    0x11205062UL


/***********************************************************
* Register Name : DEVICE_STATUS2
* Register Description :
* The Device Status 2 register provides information about device specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved.
#define DEVICE_STATUS2_Reserved_15_0_OFFSET      0
#define DEVICE_STATUS2_Reserved_15_0_MASK        0xffff

typedef union {
  struct {
    UINT16                            Reserved_15_0:16;
  } Field;
  UINT16 Value;
} DEVICE_STATUS2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_DEVICE_STATUS2_ADDRESS    0x11100082UL

#define SMN_FUNC0_PCIE1_DEVICE_STATUS2_ADDRESS    0x11200082UL

#define SMN_FUNC0_PCIE2_DEVICE_STATUS2_ADDRESS    0x11300082UL

#define SMN_FUNC0_PCIE3_DEVICE_STATUS2_ADDRESS    0x11400082UL

#define SMN_FUNC1_PCIE0_DEVICE_STATUS2_ADDRESS    0x11101082UL

#define SMN_FUNC1_PCIE1_DEVICE_STATUS2_ADDRESS    0x11201082UL

#define SMN_FUNC2_PCIE0_DEVICE_STATUS2_ADDRESS    0x11102082UL

#define SMN_FUNC2_PCIE1_DEVICE_STATUS2_ADDRESS    0x11202082UL

#define SMN_FUNC3_PCIE0_DEVICE_STATUS2_ADDRESS    0x11103082UL

#define SMN_FUNC3_PCIE1_DEVICE_STATUS2_ADDRESS    0x11203082UL

#define SMN_FUNC4_PCIE0_DEVICE_STATUS2_ADDRESS    0x11104082UL

#define SMN_FUNC4_PCIE1_DEVICE_STATUS2_ADDRESS    0x11204082UL

#define SMN_FUNC5_PCIE1_DEVICE_STATUS2_ADDRESS    0x11205082UL


/***********************************************************
* Register Name : ESM_LANE_EQUALIZATION_CNTL_20GT
* Register Description :
* ESM Lane 0 Equalization Control for 20.0 GT/s register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Selects the Downstream Port's transmitter preset for initial operation at 20.0 GT/s. For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define ESM_LANE_EQUALIZATION_CNTL_20GT_ESM_LANE_DSP_20GT_TX_PRESET_OFFSET      0
#define ESM_LANE_EQUALIZATION_CNTL_20GT_ESM_LANE_DSP_20GT_TX_PRESET_MASK        0xf

// Bitfield Description : Selects the transmitter preset value that the Downstream Port requests the other side to use for initial operation at 20.0 GT/s.
// (Upstream Ports only) captures the transmitter preset value that the Upstream Port received from the other side for initial operation at 20.0 GT/s.
#define ESM_LANE_EQUALIZATION_CNTL_20GT_ESM_LANE_USP_20GT_TX_PRESET_OFFSET      4
#define ESM_LANE_EQUALIZATION_CNTL_20GT_ESM_LANE_USP_20GT_TX_PRESET_MASK        0xf0

typedef union {
  struct {
    UINT8                            ESM_LANE_DSP_20GT_TX_PRESET:4;
    UINT8                            ESM_LANE_USP_20GT_TX_PRESET:4;
  } Field;
  UINT8 Value;
} ESM_LANE_EQUALIZATION_CNTL_20GT_STRUCT;

#define SMN_FUNC0_PCIE0_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004a4UL

#define SMN_FUNC0_PCIE0_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004aeUL

#define SMN_FUNC0_PCIE0_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004afUL

#define SMN_FUNC0_PCIE0_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004b0UL

#define SMN_FUNC0_PCIE0_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004b1UL

#define SMN_FUNC0_PCIE0_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004b2UL

#define SMN_FUNC0_PCIE0_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004b3UL

#define SMN_FUNC0_PCIE0_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004a5UL

#define SMN_FUNC0_PCIE0_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004a6UL

#define SMN_FUNC0_PCIE0_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004a7UL

#define SMN_FUNC0_PCIE0_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004a8UL

#define SMN_FUNC0_PCIE0_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004a9UL

#define SMN_FUNC0_PCIE0_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004aaUL

#define SMN_FUNC0_PCIE0_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004abUL

#define SMN_FUNC0_PCIE0_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004acUL

#define SMN_FUNC0_PCIE0_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111004adUL

#define SMN_FUNC0_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004a4UL

#define SMN_FUNC0_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004aeUL

#define SMN_FUNC0_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004afUL

#define SMN_FUNC0_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004b0UL

#define SMN_FUNC0_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004b1UL

#define SMN_FUNC0_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004b2UL

#define SMN_FUNC0_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004b3UL

#define SMN_FUNC0_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004a5UL

#define SMN_FUNC0_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004a6UL

#define SMN_FUNC0_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004a7UL

#define SMN_FUNC0_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004a8UL

#define SMN_FUNC0_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004a9UL

#define SMN_FUNC0_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004aaUL

#define SMN_FUNC0_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004abUL

#define SMN_FUNC0_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004acUL

#define SMN_FUNC0_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112004adUL

#define SMN_FUNC0_PCIE2_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004a4UL

#define SMN_FUNC0_PCIE2_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004aeUL

#define SMN_FUNC0_PCIE2_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004afUL

#define SMN_FUNC0_PCIE2_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004b0UL

#define SMN_FUNC0_PCIE2_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004b1UL

#define SMN_FUNC0_PCIE2_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004b2UL

#define SMN_FUNC0_PCIE2_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004b3UL

#define SMN_FUNC0_PCIE2_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004a5UL

#define SMN_FUNC0_PCIE2_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004a6UL

#define SMN_FUNC0_PCIE2_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004a7UL

#define SMN_FUNC0_PCIE2_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004a8UL

#define SMN_FUNC0_PCIE2_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004a9UL

#define SMN_FUNC0_PCIE2_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004aaUL

#define SMN_FUNC0_PCIE2_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004abUL

#define SMN_FUNC0_PCIE2_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004acUL

#define SMN_FUNC0_PCIE2_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x113004adUL

#define SMN_FUNC0_PCIE3_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004a4UL

#define SMN_FUNC0_PCIE3_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004aeUL

#define SMN_FUNC0_PCIE3_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004afUL

#define SMN_FUNC0_PCIE3_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004b0UL

#define SMN_FUNC0_PCIE3_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004b1UL

#define SMN_FUNC0_PCIE3_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004b2UL

#define SMN_FUNC0_PCIE3_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004b3UL

#define SMN_FUNC0_PCIE3_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004a5UL

#define SMN_FUNC0_PCIE3_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004a6UL

#define SMN_FUNC0_PCIE3_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004a7UL

#define SMN_FUNC0_PCIE3_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004a8UL

#define SMN_FUNC0_PCIE3_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004a9UL

#define SMN_FUNC0_PCIE3_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004aaUL

#define SMN_FUNC0_PCIE3_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004abUL

#define SMN_FUNC0_PCIE3_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004acUL

#define SMN_FUNC0_PCIE3_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x114004adUL

#define SMN_FUNC1_PCIE0_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014a4UL

#define SMN_FUNC1_PCIE0_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014aeUL

#define SMN_FUNC1_PCIE0_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014afUL

#define SMN_FUNC1_PCIE0_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014b0UL

#define SMN_FUNC1_PCIE0_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014b1UL

#define SMN_FUNC1_PCIE0_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014b2UL

#define SMN_FUNC1_PCIE0_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014b3UL

#define SMN_FUNC1_PCIE0_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014a5UL

#define SMN_FUNC1_PCIE0_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014a6UL

#define SMN_FUNC1_PCIE0_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014a7UL

#define SMN_FUNC1_PCIE0_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014a8UL

#define SMN_FUNC1_PCIE0_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014a9UL

#define SMN_FUNC1_PCIE0_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014aaUL

#define SMN_FUNC1_PCIE0_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014abUL

#define SMN_FUNC1_PCIE0_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014acUL

#define SMN_FUNC1_PCIE0_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111014adUL

#define SMN_FUNC1_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014a4UL

#define SMN_FUNC1_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014aeUL

#define SMN_FUNC1_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014afUL

#define SMN_FUNC1_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014b0UL

#define SMN_FUNC1_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014b1UL

#define SMN_FUNC1_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014b2UL

#define SMN_FUNC1_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014b3UL

#define SMN_FUNC1_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014a5UL

#define SMN_FUNC1_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014a6UL

#define SMN_FUNC1_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014a7UL

#define SMN_FUNC1_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014a8UL

#define SMN_FUNC1_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014a9UL

#define SMN_FUNC1_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014aaUL

#define SMN_FUNC1_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014abUL

#define SMN_FUNC1_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014acUL

#define SMN_FUNC1_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112014adUL

#define SMN_FUNC2_PCIE0_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024a4UL

#define SMN_FUNC2_PCIE0_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024aeUL

#define SMN_FUNC2_PCIE0_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024afUL

#define SMN_FUNC2_PCIE0_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024b0UL

#define SMN_FUNC2_PCIE0_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024b1UL

#define SMN_FUNC2_PCIE0_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024b2UL

#define SMN_FUNC2_PCIE0_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024b3UL

#define SMN_FUNC2_PCIE0_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024a5UL

#define SMN_FUNC2_PCIE0_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024a6UL

#define SMN_FUNC2_PCIE0_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024a7UL

#define SMN_FUNC2_PCIE0_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024a8UL

#define SMN_FUNC2_PCIE0_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024a9UL

#define SMN_FUNC2_PCIE0_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024aaUL

#define SMN_FUNC2_PCIE0_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024abUL

#define SMN_FUNC2_PCIE0_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024acUL

#define SMN_FUNC2_PCIE0_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111024adUL

#define SMN_FUNC2_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024a4UL

#define SMN_FUNC2_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024aeUL

#define SMN_FUNC2_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024afUL

#define SMN_FUNC2_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024b0UL

#define SMN_FUNC2_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024b1UL

#define SMN_FUNC2_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024b2UL

#define SMN_FUNC2_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024b3UL

#define SMN_FUNC2_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024a5UL

#define SMN_FUNC2_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024a6UL

#define SMN_FUNC2_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024a7UL

#define SMN_FUNC2_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024a8UL

#define SMN_FUNC2_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024a9UL

#define SMN_FUNC2_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024aaUL

#define SMN_FUNC2_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024abUL

#define SMN_FUNC2_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024acUL

#define SMN_FUNC2_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112024adUL

#define SMN_FUNC3_PCIE0_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034a4UL

#define SMN_FUNC3_PCIE0_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034aeUL

#define SMN_FUNC3_PCIE0_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034afUL

#define SMN_FUNC3_PCIE0_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034b0UL

#define SMN_FUNC3_PCIE0_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034b1UL

#define SMN_FUNC3_PCIE0_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034b2UL

#define SMN_FUNC3_PCIE0_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034b3UL

#define SMN_FUNC3_PCIE0_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034a5UL

#define SMN_FUNC3_PCIE0_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034a6UL

#define SMN_FUNC3_PCIE0_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034a7UL

#define SMN_FUNC3_PCIE0_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034a8UL

#define SMN_FUNC3_PCIE0_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034a9UL

#define SMN_FUNC3_PCIE0_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034aaUL

#define SMN_FUNC3_PCIE0_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034abUL

#define SMN_FUNC3_PCIE0_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034acUL

#define SMN_FUNC3_PCIE0_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111034adUL

#define SMN_FUNC3_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034a4UL

#define SMN_FUNC3_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034aeUL

#define SMN_FUNC3_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034afUL

#define SMN_FUNC3_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034b0UL

#define SMN_FUNC3_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034b1UL

#define SMN_FUNC3_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034b2UL

#define SMN_FUNC3_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034b3UL

#define SMN_FUNC3_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034a5UL

#define SMN_FUNC3_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034a6UL

#define SMN_FUNC3_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034a7UL

#define SMN_FUNC3_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034a8UL

#define SMN_FUNC3_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034a9UL

#define SMN_FUNC3_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034aaUL

#define SMN_FUNC3_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034abUL

#define SMN_FUNC3_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034acUL

#define SMN_FUNC3_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112034adUL

#define SMN_FUNC4_PCIE0_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044a4UL

#define SMN_FUNC4_PCIE0_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044aeUL

#define SMN_FUNC4_PCIE0_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044afUL

#define SMN_FUNC4_PCIE0_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044b0UL

#define SMN_FUNC4_PCIE0_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044b1UL

#define SMN_FUNC4_PCIE0_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044b2UL

#define SMN_FUNC4_PCIE0_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044b3UL

#define SMN_FUNC4_PCIE0_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044a5UL

#define SMN_FUNC4_PCIE0_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044a6UL

#define SMN_FUNC4_PCIE0_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044a7UL

#define SMN_FUNC4_PCIE0_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044a8UL

#define SMN_FUNC4_PCIE0_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044a9UL

#define SMN_FUNC4_PCIE0_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044aaUL

#define SMN_FUNC4_PCIE0_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044abUL

#define SMN_FUNC4_PCIE0_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044acUL

#define SMN_FUNC4_PCIE0_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x111044adUL

#define SMN_FUNC4_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044a4UL

#define SMN_FUNC4_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044aeUL

#define SMN_FUNC4_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044afUL

#define SMN_FUNC4_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044b0UL

#define SMN_FUNC4_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044b1UL

#define SMN_FUNC4_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044b2UL

#define SMN_FUNC4_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044b3UL

#define SMN_FUNC4_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044a5UL

#define SMN_FUNC4_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044a6UL

#define SMN_FUNC4_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044a7UL

#define SMN_FUNC4_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044a8UL

#define SMN_FUNC4_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044a9UL

#define SMN_FUNC4_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044aaUL

#define SMN_FUNC4_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044abUL

#define SMN_FUNC4_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044acUL

#define SMN_FUNC4_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112044adUL

#define SMN_FUNC5_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054a4UL

#define SMN_FUNC5_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054aeUL

#define SMN_FUNC5_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054afUL

#define SMN_FUNC5_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054b0UL

#define SMN_FUNC5_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054b1UL

#define SMN_FUNC5_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054b2UL

#define SMN_FUNC5_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054b3UL

#define SMN_FUNC5_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054a5UL

#define SMN_FUNC5_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054a6UL

#define SMN_FUNC5_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054a7UL

#define SMN_FUNC5_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054a8UL

#define SMN_FUNC5_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054a9UL

#define SMN_FUNC5_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054aaUL

#define SMN_FUNC5_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054abUL

#define SMN_FUNC5_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054acUL

#define SMN_FUNC5_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_20GT_ADDRESS    0x112054adUL


/***********************************************************
* Register Name : ESM_LANE_EQUALIZATION_CNTL_25GT
* Register Description :
* ESM Lane 0 Equalization Control for 25.0 GT/s register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Selects the Downstream Port's transmitter preset for initial operation at 25.0 GT/s. For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define ESM_LANE_EQUALIZATION_CNTL_25GT_ESM_LANE_DSP_25GT_TX_PRESET_OFFSET      0
#define ESM_LANE_EQUALIZATION_CNTL_25GT_ESM_LANE_DSP_25GT_TX_PRESET_MASK        0xf

// Bitfield Description : Selects the transmitter preset value that the Downstream Port requests the other side to use for initial operation at 25.0 GT/s.
// (Upstream Ports only) captures the transmitter preset value that the Upstream Port received from the other side for initial operation at 25.0 GT/s.
#define ESM_LANE_EQUALIZATION_CNTL_25GT_ESM_LANE_USP_25GT_TX_PRESET_OFFSET      4
#define ESM_LANE_EQUALIZATION_CNTL_25GT_ESM_LANE_USP_25GT_TX_PRESET_MASK        0xf0

typedef union {
  struct {
    UINT8                            ESM_LANE_DSP_25GT_TX_PRESET:4;
    UINT8                            ESM_LANE_USP_25GT_TX_PRESET:4;
  } Field;
  UINT8 Value;
} ESM_LANE_EQUALIZATION_CNTL_25GT_STRUCT;

#define SMN_FUNC0_PCIE0_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004b4UL

#define SMN_FUNC0_PCIE0_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004beUL

#define SMN_FUNC0_PCIE0_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004bfUL

#define SMN_FUNC0_PCIE0_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004c0UL

#define SMN_FUNC0_PCIE0_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004c1UL

#define SMN_FUNC0_PCIE0_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004c2UL

#define SMN_FUNC0_PCIE0_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004c3UL

#define SMN_FUNC0_PCIE0_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004b5UL

#define SMN_FUNC0_PCIE0_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004b6UL

#define SMN_FUNC0_PCIE0_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004b7UL

#define SMN_FUNC0_PCIE0_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004b8UL

#define SMN_FUNC0_PCIE0_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004b9UL

#define SMN_FUNC0_PCIE0_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004baUL

#define SMN_FUNC0_PCIE0_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004bbUL

#define SMN_FUNC0_PCIE0_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004bcUL

#define SMN_FUNC0_PCIE0_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111004bdUL

#define SMN_FUNC0_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004b4UL

#define SMN_FUNC0_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004beUL

#define SMN_FUNC0_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004bfUL

#define SMN_FUNC0_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004c0UL

#define SMN_FUNC0_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004c1UL

#define SMN_FUNC0_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004c2UL

#define SMN_FUNC0_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004c3UL

#define SMN_FUNC0_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004b5UL

#define SMN_FUNC0_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004b6UL

#define SMN_FUNC0_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004b7UL

#define SMN_FUNC0_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004b8UL

#define SMN_FUNC0_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004b9UL

#define SMN_FUNC0_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004baUL

#define SMN_FUNC0_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004bbUL

#define SMN_FUNC0_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004bcUL

#define SMN_FUNC0_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112004bdUL

#define SMN_FUNC0_PCIE2_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004b4UL

#define SMN_FUNC0_PCIE2_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004beUL

#define SMN_FUNC0_PCIE2_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004bfUL

#define SMN_FUNC0_PCIE2_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004c0UL

#define SMN_FUNC0_PCIE2_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004c1UL

#define SMN_FUNC0_PCIE2_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004c2UL

#define SMN_FUNC0_PCIE2_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004c3UL

#define SMN_FUNC0_PCIE2_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004b5UL

#define SMN_FUNC0_PCIE2_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004b6UL

#define SMN_FUNC0_PCIE2_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004b7UL

#define SMN_FUNC0_PCIE2_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004b8UL

#define SMN_FUNC0_PCIE2_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004b9UL

#define SMN_FUNC0_PCIE2_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004baUL

#define SMN_FUNC0_PCIE2_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004bbUL

#define SMN_FUNC0_PCIE2_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004bcUL

#define SMN_FUNC0_PCIE2_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x113004bdUL

#define SMN_FUNC0_PCIE3_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004b4UL

#define SMN_FUNC0_PCIE3_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004beUL

#define SMN_FUNC0_PCIE3_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004bfUL

#define SMN_FUNC0_PCIE3_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004c0UL

#define SMN_FUNC0_PCIE3_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004c1UL

#define SMN_FUNC0_PCIE3_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004c2UL

#define SMN_FUNC0_PCIE3_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004c3UL

#define SMN_FUNC0_PCIE3_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004b5UL

#define SMN_FUNC0_PCIE3_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004b6UL

#define SMN_FUNC0_PCIE3_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004b7UL

#define SMN_FUNC0_PCIE3_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004b8UL

#define SMN_FUNC0_PCIE3_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004b9UL

#define SMN_FUNC0_PCIE3_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004baUL

#define SMN_FUNC0_PCIE3_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004bbUL

#define SMN_FUNC0_PCIE3_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004bcUL

#define SMN_FUNC0_PCIE3_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x114004bdUL

#define SMN_FUNC1_PCIE0_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014b4UL

#define SMN_FUNC1_PCIE0_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014beUL

#define SMN_FUNC1_PCIE0_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014bfUL

#define SMN_FUNC1_PCIE0_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014c0UL

#define SMN_FUNC1_PCIE0_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014c1UL

#define SMN_FUNC1_PCIE0_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014c2UL

#define SMN_FUNC1_PCIE0_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014c3UL

#define SMN_FUNC1_PCIE0_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014b5UL

#define SMN_FUNC1_PCIE0_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014b6UL

#define SMN_FUNC1_PCIE0_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014b7UL

#define SMN_FUNC1_PCIE0_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014b8UL

#define SMN_FUNC1_PCIE0_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014b9UL

#define SMN_FUNC1_PCIE0_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014baUL

#define SMN_FUNC1_PCIE0_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014bbUL

#define SMN_FUNC1_PCIE0_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014bcUL

#define SMN_FUNC1_PCIE0_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111014bdUL

#define SMN_FUNC1_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014b4UL

#define SMN_FUNC1_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014beUL

#define SMN_FUNC1_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014bfUL

#define SMN_FUNC1_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014c0UL

#define SMN_FUNC1_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014c1UL

#define SMN_FUNC1_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014c2UL

#define SMN_FUNC1_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014c3UL

#define SMN_FUNC1_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014b5UL

#define SMN_FUNC1_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014b6UL

#define SMN_FUNC1_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014b7UL

#define SMN_FUNC1_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014b8UL

#define SMN_FUNC1_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014b9UL

#define SMN_FUNC1_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014baUL

#define SMN_FUNC1_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014bbUL

#define SMN_FUNC1_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014bcUL

#define SMN_FUNC1_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112014bdUL

#define SMN_FUNC2_PCIE0_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024b4UL

#define SMN_FUNC2_PCIE0_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024beUL

#define SMN_FUNC2_PCIE0_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024bfUL

#define SMN_FUNC2_PCIE0_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024c0UL

#define SMN_FUNC2_PCIE0_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024c1UL

#define SMN_FUNC2_PCIE0_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024c2UL

#define SMN_FUNC2_PCIE0_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024c3UL

#define SMN_FUNC2_PCIE0_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024b5UL

#define SMN_FUNC2_PCIE0_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024b6UL

#define SMN_FUNC2_PCIE0_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024b7UL

#define SMN_FUNC2_PCIE0_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024b8UL

#define SMN_FUNC2_PCIE0_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024b9UL

#define SMN_FUNC2_PCIE0_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024baUL

#define SMN_FUNC2_PCIE0_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024bbUL

#define SMN_FUNC2_PCIE0_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024bcUL

#define SMN_FUNC2_PCIE0_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111024bdUL

#define SMN_FUNC2_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024b4UL

#define SMN_FUNC2_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024beUL

#define SMN_FUNC2_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024bfUL

#define SMN_FUNC2_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024c0UL

#define SMN_FUNC2_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024c1UL

#define SMN_FUNC2_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024c2UL

#define SMN_FUNC2_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024c3UL

#define SMN_FUNC2_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024b5UL

#define SMN_FUNC2_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024b6UL

#define SMN_FUNC2_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024b7UL

#define SMN_FUNC2_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024b8UL

#define SMN_FUNC2_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024b9UL

#define SMN_FUNC2_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024baUL

#define SMN_FUNC2_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024bbUL

#define SMN_FUNC2_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024bcUL

#define SMN_FUNC2_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112024bdUL

#define SMN_FUNC3_PCIE0_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034b4UL

#define SMN_FUNC3_PCIE0_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034beUL

#define SMN_FUNC3_PCIE0_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034bfUL

#define SMN_FUNC3_PCIE0_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034c0UL

#define SMN_FUNC3_PCIE0_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034c1UL

#define SMN_FUNC3_PCIE0_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034c2UL

#define SMN_FUNC3_PCIE0_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034c3UL

#define SMN_FUNC3_PCIE0_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034b5UL

#define SMN_FUNC3_PCIE0_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034b6UL

#define SMN_FUNC3_PCIE0_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034b7UL

#define SMN_FUNC3_PCIE0_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034b8UL

#define SMN_FUNC3_PCIE0_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034b9UL

#define SMN_FUNC3_PCIE0_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034baUL

#define SMN_FUNC3_PCIE0_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034bbUL

#define SMN_FUNC3_PCIE0_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034bcUL

#define SMN_FUNC3_PCIE0_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111034bdUL

#define SMN_FUNC3_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034b4UL

#define SMN_FUNC3_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034beUL

#define SMN_FUNC3_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034bfUL

#define SMN_FUNC3_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034c0UL

#define SMN_FUNC3_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034c1UL

#define SMN_FUNC3_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034c2UL

#define SMN_FUNC3_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034c3UL

#define SMN_FUNC3_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034b5UL

#define SMN_FUNC3_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034b6UL

#define SMN_FUNC3_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034b7UL

#define SMN_FUNC3_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034b8UL

#define SMN_FUNC3_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034b9UL

#define SMN_FUNC3_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034baUL

#define SMN_FUNC3_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034bbUL

#define SMN_FUNC3_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034bcUL

#define SMN_FUNC3_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112034bdUL

#define SMN_FUNC4_PCIE0_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044b4UL

#define SMN_FUNC4_PCIE0_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044beUL

#define SMN_FUNC4_PCIE0_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044bfUL

#define SMN_FUNC4_PCIE0_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044c0UL

#define SMN_FUNC4_PCIE0_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044c1UL

#define SMN_FUNC4_PCIE0_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044c2UL

#define SMN_FUNC4_PCIE0_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044c3UL

#define SMN_FUNC4_PCIE0_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044b5UL

#define SMN_FUNC4_PCIE0_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044b6UL

#define SMN_FUNC4_PCIE0_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044b7UL

#define SMN_FUNC4_PCIE0_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044b8UL

#define SMN_FUNC4_PCIE0_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044b9UL

#define SMN_FUNC4_PCIE0_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044baUL

#define SMN_FUNC4_PCIE0_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044bbUL

#define SMN_FUNC4_PCIE0_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044bcUL

#define SMN_FUNC4_PCIE0_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x111044bdUL

#define SMN_FUNC4_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044b4UL

#define SMN_FUNC4_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044beUL

#define SMN_FUNC4_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044bfUL

#define SMN_FUNC4_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044c0UL

#define SMN_FUNC4_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044c1UL

#define SMN_FUNC4_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044c2UL

#define SMN_FUNC4_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044c3UL

#define SMN_FUNC4_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044b5UL

#define SMN_FUNC4_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044b6UL

#define SMN_FUNC4_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044b7UL

#define SMN_FUNC4_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044b8UL

#define SMN_FUNC4_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044b9UL

#define SMN_FUNC4_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044baUL

#define SMN_FUNC4_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044bbUL

#define SMN_FUNC4_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044bcUL

#define SMN_FUNC4_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112044bdUL

#define SMN_FUNC5_PCIE1_N0_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054b4UL

#define SMN_FUNC5_PCIE1_N10_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054beUL

#define SMN_FUNC5_PCIE1_N11_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054bfUL

#define SMN_FUNC5_PCIE1_N12_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054c0UL

#define SMN_FUNC5_PCIE1_N13_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054c1UL

#define SMN_FUNC5_PCIE1_N14_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054c2UL

#define SMN_FUNC5_PCIE1_N15_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054c3UL

#define SMN_FUNC5_PCIE1_N1_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054b5UL

#define SMN_FUNC5_PCIE1_N2_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054b6UL

#define SMN_FUNC5_PCIE1_N3_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054b7UL

#define SMN_FUNC5_PCIE1_N4_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054b8UL

#define SMN_FUNC5_PCIE1_N5_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054b9UL

#define SMN_FUNC5_PCIE1_N6_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054baUL

#define SMN_FUNC5_PCIE1_N7_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054bbUL

#define SMN_FUNC5_PCIE1_N8_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054bcUL

#define SMN_FUNC5_PCIE1_N9_ESM_LANE_EQUALIZATION_CNTL_25GT_ADDRESS    0x112054bdUL


/***********************************************************
* Register Name : EXT_BRIDGE_CNTL
* Register Description :
* External Bridge Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls IO port 80 decoding.
#define EXT_BRIDGE_CNTL_IO_PORT_80_EN_OFFSET      0
#define EXT_BRIDGE_CNTL_IO_PORT_80_EN_MASK        0x1

// Bitfield Description : 
#define EXT_BRIDGE_CNTL_Reserved_7_1_OFFSET      1
#define EXT_BRIDGE_CNTL_Reserved_7_1_MASK        0xfe

typedef union {
  struct {
    UINT8                            IO_PORT_80_EN:1;
    UINT8                            Reserved_7_1:7;
  } Field;
  UINT8 Value;
} EXT_BRIDGE_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_EXT_BRIDGE_CNTL_ADDRESS    0x11100040UL

#define SMN_FUNC0_PCIE1_EXT_BRIDGE_CNTL_ADDRESS    0x11200040UL

#define SMN_FUNC0_PCIE2_EXT_BRIDGE_CNTL_ADDRESS    0x11300040UL

#define SMN_FUNC0_PCIE3_EXT_BRIDGE_CNTL_ADDRESS    0x11400040UL

#define SMN_FUNC1_PCIE0_EXT_BRIDGE_CNTL_ADDRESS    0x11101040UL

#define SMN_FUNC1_PCIE1_EXT_BRIDGE_CNTL_ADDRESS    0x11201040UL

#define SMN_FUNC2_PCIE0_EXT_BRIDGE_CNTL_ADDRESS    0x11102040UL

#define SMN_FUNC2_PCIE1_EXT_BRIDGE_CNTL_ADDRESS    0x11202040UL

#define SMN_FUNC3_PCIE0_EXT_BRIDGE_CNTL_ADDRESS    0x11103040UL

#define SMN_FUNC3_PCIE1_EXT_BRIDGE_CNTL_ADDRESS    0x11203040UL

#define SMN_FUNC4_PCIE0_EXT_BRIDGE_CNTL_ADDRESS    0x11104040UL

#define SMN_FUNC4_PCIE1_EXT_BRIDGE_CNTL_ADDRESS    0x11204040UL

#define SMN_FUNC5_PCIE1_EXT_BRIDGE_CNTL_ADDRESS    0x11205040UL


/***********************************************************
* Register Name : FRS_MSG_QUEUE
* Register Description :
* FRS Message Queue register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the Requester ID indicated in the oldest received or generated FRS Message within the FRS Message Queue.
#define FRS_MSG_QUEUE_FRS_MSG_QUEUE_FUNCTION_ID_OFFSET      0
#define FRS_MSG_QUEUE_FRS_MSG_QUEUE_FUNCTION_ID_MASK        0xffff

// Bitfield Description : Indicates the FRS Reason indicated in the oldest received or generated FRS Message within the FRS Message Queue.
#define FRS_MSG_QUEUE_FRS_MSG_QUEUE_REASON_OFFSET      16
#define FRS_MSG_QUEUE_FRS_MSG_QUEUE_REASON_MASK        0xf0000

// Bitfield Description : Indicates the current number of FRS Messages in the FRS Queue.
#define FRS_MSG_QUEUE_FRS_QUEUE_DEPTH_OFFSET      20
#define FRS_MSG_QUEUE_FRS_QUEUE_DEPTH_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            FRS_MSG_QUEUE_FUNCTION_ID:16;
    UINT32                            FRS_MSG_QUEUE_REASON:4;
    UINT32                            FRS_QUEUE_DEPTH:12;
  } Field;
  UINT32 Value;
} FRS_MSG_QUEUE_STRUCT;

#define SMN_FUNC0_PCIE0_FRS_MSG_QUEUE_ADDRESS    0x1110056cUL

#define SMN_FUNC0_PCIE1_FRS_MSG_QUEUE_ADDRESS    0x1120056cUL

#define SMN_FUNC0_PCIE2_FRS_MSG_QUEUE_ADDRESS    0x1130056cUL

#define SMN_FUNC0_PCIE3_FRS_MSG_QUEUE_ADDRESS    0x1140056cUL

#define SMN_FUNC1_PCIE0_FRS_MSG_QUEUE_ADDRESS    0x1110156cUL

#define SMN_FUNC1_PCIE1_FRS_MSG_QUEUE_ADDRESS    0x1120156cUL

#define SMN_FUNC2_PCIE0_FRS_MSG_QUEUE_ADDRESS    0x1110256cUL

#define SMN_FUNC2_PCIE1_FRS_MSG_QUEUE_ADDRESS    0x1120256cUL

#define SMN_FUNC3_PCIE0_FRS_MSG_QUEUE_ADDRESS    0x1110356cUL

#define SMN_FUNC3_PCIE1_FRS_MSG_QUEUE_ADDRESS    0x1120356cUL

#define SMN_FUNC4_PCIE0_FRS_MSG_QUEUE_ADDRESS    0x1110456cUL

#define SMN_FUNC4_PCIE1_FRS_MSG_QUEUE_ADDRESS    0x1120456cUL

#define SMN_FUNC5_PCIE1_FRS_MSG_QUEUE_ADDRESS    0x1120556cUL


/***********************************************************
* Register Name : FRS_QUEUEING_CAP
* Register Description :
* FRS Queueing Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the supported depth of the FRS Queue.
#define FRS_QUEUEING_CAP_FRS_QUEUE_MAX_DEPTH_OFFSET      0
#define FRS_QUEUEING_CAP_FRS_QUEUE_MAX_DEPTH_MASK        0xfff

// Bitfield Description : 
#define FRS_QUEUEING_CAP_Reserved_15_12_OFFSET      12
#define FRS_QUEUEING_CAP_Reserved_15_12_MASK        0xf000

// Bitfield Description : Indicates the MSI/MSI-X vector used for interrupts generated in association with any of the status bits in the FRS Queueing capability structure.
#define FRS_QUEUEING_CAP_FRS_INTR_MSG_NUM_OFFSET      16
#define FRS_QUEUEING_CAP_FRS_INTR_MSG_NUM_MASK        0x1f0000

// Bitfield Description : 
#define FRS_QUEUEING_CAP_Reserved_31_21_OFFSET      21
#define FRS_QUEUEING_CAP_Reserved_31_21_MASK        0xffe00000

typedef union {
  struct {
    UINT32                            FRS_QUEUE_MAX_DEPTH:12;
    UINT32                            Reserved_15_12:4;
    UINT32                            FRS_INTR_MSG_NUM:5;
    UINT32                            Reserved_31_21:11;
  } Field;
  UINT32 Value;
} FRS_QUEUEING_CAP_STRUCT;

#define SMN_FUNC0_PCIE0_FRS_QUEUEING_CAP_ADDRESS    0x11100564UL

#define SMN_FUNC0_PCIE1_FRS_QUEUEING_CAP_ADDRESS    0x11200564UL

#define SMN_FUNC0_PCIE2_FRS_QUEUEING_CAP_ADDRESS    0x11300564UL

#define SMN_FUNC0_PCIE3_FRS_QUEUEING_CAP_ADDRESS    0x11400564UL

#define SMN_FUNC1_PCIE0_FRS_QUEUEING_CAP_ADDRESS    0x11101564UL

#define SMN_FUNC1_PCIE1_FRS_QUEUEING_CAP_ADDRESS    0x11201564UL

#define SMN_FUNC2_PCIE0_FRS_QUEUEING_CAP_ADDRESS    0x11102564UL

#define SMN_FUNC2_PCIE1_FRS_QUEUEING_CAP_ADDRESS    0x11202564UL

#define SMN_FUNC3_PCIE0_FRS_QUEUEING_CAP_ADDRESS    0x11103564UL

#define SMN_FUNC3_PCIE1_FRS_QUEUEING_CAP_ADDRESS    0x11203564UL

#define SMN_FUNC4_PCIE0_FRS_QUEUEING_CAP_ADDRESS    0x11104564UL

#define SMN_FUNC4_PCIE1_FRS_QUEUEING_CAP_ADDRESS    0x11204564UL

#define SMN_FUNC5_PCIE1_FRS_QUEUEING_CAP_ADDRESS    0x11205564UL


/***********************************************************
* Register Name : FRS_QUEUEING_CNTL
* Register Description :
* FRS Queueing Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the Port generates an interrupt to indicate that the FRS Message Received, or the FRS Message Overflow bits have been set.
#define FRS_QUEUEING_CNTL_FRS_INT_EN_OFFSET      0
#define FRS_QUEUEING_CNTL_FRS_INT_EN_MASK        0x1

// Bitfield Description : 
#define FRS_QUEUEING_CNTL_Reserved_15_1_OFFSET      1
#define FRS_QUEUEING_CNTL_Reserved_15_1_MASK        0xfffe

typedef union {
  struct {
    UINT16                            FRS_INT_EN:1;
    UINT16                            Reserved_15_1:15;
  } Field;
  UINT16 Value;
} FRS_QUEUEING_CNTL_STRUCT;

#define SMN_FUNC0_PCIE0_FRS_QUEUEING_CNTL_ADDRESS    0x1110056aUL

#define SMN_FUNC0_PCIE1_FRS_QUEUEING_CNTL_ADDRESS    0x1120056aUL

#define SMN_FUNC0_PCIE2_FRS_QUEUEING_CNTL_ADDRESS    0x1130056aUL

#define SMN_FUNC0_PCIE3_FRS_QUEUEING_CNTL_ADDRESS    0x1140056aUL

#define SMN_FUNC1_PCIE0_FRS_QUEUEING_CNTL_ADDRESS    0x1110156aUL

#define SMN_FUNC1_PCIE1_FRS_QUEUEING_CNTL_ADDRESS    0x1120156aUL

#define SMN_FUNC2_PCIE0_FRS_QUEUEING_CNTL_ADDRESS    0x1110256aUL

#define SMN_FUNC2_PCIE1_FRS_QUEUEING_CNTL_ADDRESS    0x1120256aUL

#define SMN_FUNC3_PCIE0_FRS_QUEUEING_CNTL_ADDRESS    0x1110356aUL

#define SMN_FUNC3_PCIE1_FRS_QUEUEING_CNTL_ADDRESS    0x1120356aUL

#define SMN_FUNC4_PCIE0_FRS_QUEUEING_CNTL_ADDRESS    0x1110456aUL

#define SMN_FUNC4_PCIE1_FRS_QUEUEING_CNTL_ADDRESS    0x1120456aUL

#define SMN_FUNC5_PCIE1_FRS_QUEUEING_CNTL_ADDRESS    0x1120556aUL


/***********************************************************
* Register Name : FRS_QUEUEING_ENH_CAP_LIST
* Register Description :
* FRS Queueing Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define FRS_QUEUEING_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define FRS_QUEUEING_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define FRS_QUEUEING_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define FRS_QUEUEING_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define FRS_QUEUEING_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define FRS_QUEUEING_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} FRS_QUEUEING_ENH_CAP_LIST_STRUCT;

#define SMN_FUNC0_PCIE0_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11100560UL

#define SMN_FUNC0_PCIE1_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11200560UL

#define SMN_FUNC0_PCIE2_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11300560UL

#define SMN_FUNC0_PCIE3_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11400560UL

#define SMN_FUNC1_PCIE0_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11101560UL

#define SMN_FUNC1_PCIE1_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11201560UL

#define SMN_FUNC2_PCIE0_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11102560UL

#define SMN_FUNC2_PCIE1_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11202560UL

#define SMN_FUNC3_PCIE0_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11103560UL

#define SMN_FUNC3_PCIE1_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11203560UL

#define SMN_FUNC4_PCIE0_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11104560UL

#define SMN_FUNC4_PCIE1_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11204560UL

#define SMN_FUNC5_PCIE1_FRS_QUEUEING_ENH_CAP_LIST_ADDRESS    0x11205560UL


/***********************************************************
* Register Name : FRS_QUEUEING_STATUS
* Register Description :
* FRS Queueing Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if a new FRS Message is received or generated.
#define FRS_QUEUEING_STATUS_FRS_MSG_RCVD_OFFSET      0
#define FRS_QUEUEING_STATUS_FRS_MSG_RCVD_MASK        0x1

// Bitfield Description : Indicates if a new FRS Message is received or generated, when the FRS Queue is full.
#define FRS_QUEUEING_STATUS_FRS_MSG_OVFL_OFFSET      1
#define FRS_QUEUEING_STATUS_FRS_MSG_OVFL_MASK        0x2

// Bitfield Description : 
#define FRS_QUEUEING_STATUS_Reserved_15_2_OFFSET      2
#define FRS_QUEUEING_STATUS_Reserved_15_2_MASK        0xfffc

typedef union {
  struct {
    UINT16                            FRS_MSG_RCVD:1;
    UINT16                            FRS_MSG_OVFL:1;
    UINT16                            Reserved_15_2:14;
  } Field;
  UINT16 Value;
} FRS_QUEUEING_STATUS_STRUCT;

#define SMN_FUNC0_PCIE0_FRS_QUEUEING_STATUS_ADDRESS    0x11100568UL

#define SMN_FUNC0_PCIE1_FRS_QUEUEING_STATUS_ADDRESS    0x11200568UL

#define SMN_FUNC0_PCIE2_FRS_QUEUEING_STATUS_ADDRESS    0x11300568UL

#define SMN_FUNC0_PCIE3_FRS_QUEUEING_STATUS_ADDRESS    0x11400568UL

#define SMN_FUNC1_PCIE0_FRS_QUEUEING_STATUS_ADDRESS    0x11101568UL

#define SMN_FUNC1_PCIE1_FRS_QUEUEING_STATUS_ADDRESS    0x11201568UL

#define SMN_FUNC2_PCIE0_FRS_QUEUEING_STATUS_ADDRESS    0x11102568UL

#define SMN_FUNC2_PCIE1_FRS_QUEUEING_STATUS_ADDRESS    0x11202568UL

#define SMN_FUNC3_PCIE0_FRS_QUEUEING_STATUS_ADDRESS    0x11103568UL

#define SMN_FUNC3_PCIE1_FRS_QUEUEING_STATUS_ADDRESS    0x11203568UL

#define SMN_FUNC4_PCIE0_FRS_QUEUEING_STATUS_ADDRESS    0x11104568UL

#define SMN_FUNC4_PCIE1_FRS_QUEUEING_STATUS_ADDRESS    0x11204568UL

#define SMN_FUNC5_PCIE1_FRS_QUEUEING_STATUS_ADDRESS    0x11205568UL


/***********************************************************
* Register Name : HEADER
* Register Description :
* Configuration Space header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Identifies the layout of the Configuration Space header.
#define HEADER_HEADER_TYPE_OFFSET      0
#define HEADER_HEADER_TYPE_MASK        0x7f

// Bitfield Description : Identifies if the device contains multiple functions.
#define HEADER_DEVICE_TYPE_OFFSET      7
#define HEADER_DEVICE_TYPE_MASK        0x80

typedef union {
  struct {
    UINT8                            HEADER_TYPE:7;
    UINT8                            DEVICE_TYPE:1;
  } Field;
  UINT8 Value;
} HEADER_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_HEADER_ADDRESS    0x1110000eUL

#define SMN_FUNC0_PCIE1_HEADER_ADDRESS    0x1120000eUL

#define SMN_FUNC0_PCIE2_HEADER_ADDRESS    0x1130000eUL

#define SMN_FUNC0_PCIE3_HEADER_ADDRESS    0x1140000eUL

#define SMN_FUNC1_PCIE0_HEADER_ADDRESS    0x1110100eUL

#define SMN_FUNC1_PCIE1_HEADER_ADDRESS    0x1120100eUL

#define SMN_FUNC2_PCIE0_HEADER_ADDRESS    0x1110200eUL

#define SMN_FUNC2_PCIE1_HEADER_ADDRESS    0x1120200eUL

#define SMN_FUNC3_PCIE0_HEADER_ADDRESS    0x1110300eUL

#define SMN_FUNC3_PCIE1_HEADER_ADDRESS    0x1120300eUL

#define SMN_FUNC4_PCIE0_HEADER_ADDRESS    0x1110400eUL

#define SMN_FUNC4_PCIE1_HEADER_ADDRESS    0x1120400eUL

#define SMN_FUNC5_PCIE1_HEADER_ADDRESS    0x1120500eUL


/***********************************************************
* Register Name : INTERRUPT_LINE
* Register Description :
* Interrupt Line register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Communicates interrupt line routing information.
#define INTERRUPT_LINE_INTERRUPT_LINE_OFFSET      0
#define INTERRUPT_LINE_INTERRUPT_LINE_MASK        0xff

typedef union {
  struct {
    UINT8                            INTERRUPT_LINE:8;
  } Field;
  UINT8 Value;
} INTERRUPT_LINE_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_INTERRUPT_LINE_ADDRESS    0x1110003cUL

#define SMN_FUNC0_PCIE1_INTERRUPT_LINE_ADDRESS    0x1120003cUL

#define SMN_FUNC0_PCIE2_INTERRUPT_LINE_ADDRESS    0x1130003cUL

#define SMN_FUNC0_PCIE3_INTERRUPT_LINE_ADDRESS    0x1140003cUL

#define SMN_FUNC1_PCIE0_INTERRUPT_LINE_ADDRESS    0x1110103cUL

#define SMN_FUNC1_PCIE1_INTERRUPT_LINE_ADDRESS    0x1120103cUL

#define SMN_FUNC2_PCIE0_INTERRUPT_LINE_ADDRESS    0x1110203cUL

#define SMN_FUNC2_PCIE1_INTERRUPT_LINE_ADDRESS    0x1120203cUL

#define SMN_FUNC3_PCIE0_INTERRUPT_LINE_ADDRESS    0x1110303cUL

#define SMN_FUNC3_PCIE1_INTERRUPT_LINE_ADDRESS    0x1120303cUL

#define SMN_FUNC4_PCIE0_INTERRUPT_LINE_ADDRESS    0x1110403cUL

#define SMN_FUNC4_PCIE1_INTERRUPT_LINE_ADDRESS    0x1120403cUL

#define SMN_FUNC5_PCIE1_INTERRUPT_LINE_ADDRESS    0x1120503cUL


/***********************************************************
* Register Name : INTERRUPT_PIN
* Register Description :
* Interrupt Pin register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Identifies the legacy interrupt Message(s) the function uses.
#define INTERRUPT_PIN_INTERRUPT_PIN_OFFSET      0
#define INTERRUPT_PIN_INTERRUPT_PIN_MASK        0xff

typedef union {
  struct {
    UINT8                            INTERRUPT_PIN:8;
  } Field;
  UINT8 Value;
} INTERRUPT_PIN_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_INTERRUPT_PIN_ADDRESS    0x1110003dUL

#define SMN_FUNC0_PCIE1_INTERRUPT_PIN_ADDRESS    0x1120003dUL

#define SMN_FUNC0_PCIE2_INTERRUPT_PIN_ADDRESS    0x1130003dUL

#define SMN_FUNC0_PCIE3_INTERRUPT_PIN_ADDRESS    0x1140003dUL

#define SMN_FUNC1_PCIE0_INTERRUPT_PIN_ADDRESS    0x1110103dUL

#define SMN_FUNC1_PCIE1_INTERRUPT_PIN_ADDRESS    0x1120103dUL

#define SMN_FUNC2_PCIE0_INTERRUPT_PIN_ADDRESS    0x1110203dUL

#define SMN_FUNC2_PCIE1_INTERRUPT_PIN_ADDRESS    0x1120203dUL

#define SMN_FUNC3_PCIE0_INTERRUPT_PIN_ADDRESS    0x1110303dUL

#define SMN_FUNC3_PCIE1_INTERRUPT_PIN_ADDRESS    0x1120303dUL

#define SMN_FUNC4_PCIE0_INTERRUPT_PIN_ADDRESS    0x1110403dUL

#define SMN_FUNC4_PCIE1_INTERRUPT_PIN_ADDRESS    0x1120403dUL

#define SMN_FUNC5_PCIE1_INTERRUPT_PIN_ADDRESS    0x1120503dUL


/***********************************************************
* Register Name : IO_BASE_LIMIT
* Register Description :
* Defines an I/O address range which is used by the bridge to determine when to forward I/O transactions from one interface to the other. For 32-bit addressing type, combines with I/O Base Upper 16 Bits and I/O Limit Upper 16 Bits registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : I/O Base Addressing Type.
#define IO_BASE_LIMIT_IO_BASE_TYPE_OFFSET      0
#define IO_BASE_LIMIT_IO_BASE_TYPE_MASK        0xf

// Bitfield Description : Bits 15-12 for range starting I/O address.
#define IO_BASE_LIMIT_IO_BASE_OFFSET      4
#define IO_BASE_LIMIT_IO_BASE_MASK        0xf0

// Bitfield Description : I/O Limit Addressing Type.
#define IO_BASE_LIMIT_IO_LIMIT_TYPE_OFFSET      8
#define IO_BASE_LIMIT_IO_LIMIT_TYPE_MASK        0xf00

// Bitfield Description : Bits 15-12 for range ending I/O address.
#define IO_BASE_LIMIT_IO_LIMIT_OFFSET      12
#define IO_BASE_LIMIT_IO_LIMIT_MASK        0xf000

typedef union {
  struct {
    UINT16                            IO_BASE_TYPE:4;
    UINT16                            IO_BASE:4;
    UINT16                            IO_LIMIT_TYPE:4;
    UINT16                            IO_LIMIT:4;
  } Field;
  UINT16 Value;
} IO_BASE_LIMIT_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_IO_BASE_LIMIT_ADDRESS    0x1110001cUL

#define SMN_FUNC0_PCIE1_IO_BASE_LIMIT_ADDRESS    0x1120001cUL

#define SMN_FUNC0_PCIE2_IO_BASE_LIMIT_ADDRESS    0x1130001cUL

#define SMN_FUNC0_PCIE3_IO_BASE_LIMIT_ADDRESS    0x1140001cUL

#define SMN_FUNC1_PCIE0_IO_BASE_LIMIT_ADDRESS    0x1110101cUL

#define SMN_FUNC1_PCIE1_IO_BASE_LIMIT_ADDRESS    0x1120101cUL

#define SMN_FUNC2_PCIE0_IO_BASE_LIMIT_ADDRESS    0x1110201cUL

#define SMN_FUNC2_PCIE1_IO_BASE_LIMIT_ADDRESS    0x1120201cUL

#define SMN_FUNC3_PCIE0_IO_BASE_LIMIT_ADDRESS    0x1110301cUL

#define SMN_FUNC3_PCIE1_IO_BASE_LIMIT_ADDRESS    0x1120301cUL

#define SMN_FUNC4_PCIE0_IO_BASE_LIMIT_ADDRESS    0x1110401cUL

#define SMN_FUNC4_PCIE1_IO_BASE_LIMIT_ADDRESS    0x1120401cUL

#define SMN_FUNC5_PCIE1_IO_BASE_LIMIT_ADDRESS    0x1120501cUL


/***********************************************************
* Register Name : IO_BASE_LIMIT_HI
* Register Description :
* For 32-bit addressing type, combines with I/O Base and I/O Limit registers to define an I/O address range which is used by the bridge to determine when to forward I/O transactions from one interface to the other.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 16 bits for range starting 32-bit I/O address.
#define IO_BASE_LIMIT_HI_IO_BASE_31_16_OFFSET      0
#define IO_BASE_LIMIT_HI_IO_BASE_31_16_MASK        0xffff

// Bitfield Description : Upper 16 bits for range ending 32-bit I/O address.
#define IO_BASE_LIMIT_HI_IO_LIMIT_31_16_OFFSET      16
#define IO_BASE_LIMIT_HI_IO_LIMIT_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            IO_BASE_31_16:16;
    UINT32                            IO_LIMIT_31_16:16;
  } Field;
  UINT32 Value;
} IO_BASE_LIMIT_HI_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_IO_BASE_LIMIT_HI_ADDRESS    0x11100030UL

#define SMN_FUNC0_PCIE1_IO_BASE_LIMIT_HI_ADDRESS    0x11200030UL

#define SMN_FUNC0_PCIE2_IO_BASE_LIMIT_HI_ADDRESS    0x11300030UL

#define SMN_FUNC0_PCIE3_IO_BASE_LIMIT_HI_ADDRESS    0x11400030UL

#define SMN_FUNC1_PCIE0_IO_BASE_LIMIT_HI_ADDRESS    0x11101030UL

#define SMN_FUNC1_PCIE1_IO_BASE_LIMIT_HI_ADDRESS    0x11201030UL

#define SMN_FUNC2_PCIE0_IO_BASE_LIMIT_HI_ADDRESS    0x11102030UL

#define SMN_FUNC2_PCIE1_IO_BASE_LIMIT_HI_ADDRESS    0x11202030UL

#define SMN_FUNC3_PCIE0_IO_BASE_LIMIT_HI_ADDRESS    0x11103030UL

#define SMN_FUNC3_PCIE1_IO_BASE_LIMIT_HI_ADDRESS    0x11203030UL

#define SMN_FUNC4_PCIE0_IO_BASE_LIMIT_HI_ADDRESS    0x11104030UL

#define SMN_FUNC4_PCIE1_IO_BASE_LIMIT_HI_ADDRESS    0x11204030UL

#define SMN_FUNC5_PCIE1_IO_BASE_LIMIT_HI_ADDRESS    0x11205030UL


/***********************************************************
* Register Name : LANE_EQUALIZATION_CNTL_16GT
* Register Description :
* 16.0 GT/s Lane 0 Equalization Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Selects the Downstream Port's transmitter preset for initial operation at 16.0 GT/s. For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define LANE_EQUALIZATION_CNTL_16GT_LANE_DSP_16GT_TX_PRESET_OFFSET      0
#define LANE_EQUALIZATION_CNTL_16GT_LANE_DSP_16GT_TX_PRESET_MASK        0xf

// Bitfield Description : Selects the transmitter preset value that the Downstream Port requests the other side to use for initial operation at 16.0 GT/s.
// (Upstream Ports only) captures the transmitter preset value that the Upstream Port received from the other side for initial operation at 16.0 GT/s.
#define LANE_EQUALIZATION_CNTL_16GT_LANE_USP_16GT_TX_PRESET_OFFSET      4
#define LANE_EQUALIZATION_CNTL_16GT_LANE_USP_16GT_TX_PRESET_MASK        0xf0

typedef union {
  struct {
    UINT8                            LANE_DSP_16GT_TX_PRESET:4;
    UINT8                            LANE_USP_16GT_TX_PRESET:4;
  } Field;
  UINT8 Value;
} LANE_EQUALIZATION_CNTL_16GT_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11100430UL

#define SMN_FUNC0_PCIE0_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110043aUL

#define SMN_FUNC0_PCIE0_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110043bUL

#define SMN_FUNC0_PCIE0_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110043cUL

#define SMN_FUNC0_PCIE0_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110043dUL

#define SMN_FUNC0_PCIE0_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110043eUL

#define SMN_FUNC0_PCIE0_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110043fUL

#define SMN_FUNC0_PCIE0_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11100431UL

#define SMN_FUNC0_PCIE0_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11100432UL

#define SMN_FUNC0_PCIE0_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11100433UL

#define SMN_FUNC0_PCIE0_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11100434UL

#define SMN_FUNC0_PCIE0_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11100435UL

#define SMN_FUNC0_PCIE0_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11100436UL

#define SMN_FUNC0_PCIE0_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11100437UL

#define SMN_FUNC0_PCIE0_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11100438UL

#define SMN_FUNC0_PCIE0_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11100439UL

#define SMN_FUNC0_PCIE1_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11200430UL

#define SMN_FUNC0_PCIE1_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120043aUL

#define SMN_FUNC0_PCIE1_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120043bUL

#define SMN_FUNC0_PCIE1_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120043cUL

#define SMN_FUNC0_PCIE1_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120043dUL

#define SMN_FUNC0_PCIE1_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120043eUL

#define SMN_FUNC0_PCIE1_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120043fUL

#define SMN_FUNC0_PCIE1_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11200431UL

#define SMN_FUNC0_PCIE1_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11200432UL

#define SMN_FUNC0_PCIE1_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11200433UL

#define SMN_FUNC0_PCIE1_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11200434UL

#define SMN_FUNC0_PCIE1_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11200435UL

#define SMN_FUNC0_PCIE1_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11200436UL

#define SMN_FUNC0_PCIE1_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11200437UL

#define SMN_FUNC0_PCIE1_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11200438UL

#define SMN_FUNC0_PCIE1_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11200439UL

#define SMN_FUNC0_PCIE2_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11300430UL

#define SMN_FUNC0_PCIE2_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1130043aUL

#define SMN_FUNC0_PCIE2_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1130043bUL

#define SMN_FUNC0_PCIE2_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1130043cUL

#define SMN_FUNC0_PCIE2_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1130043dUL

#define SMN_FUNC0_PCIE2_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1130043eUL

#define SMN_FUNC0_PCIE2_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1130043fUL

#define SMN_FUNC0_PCIE2_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11300431UL

#define SMN_FUNC0_PCIE2_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11300432UL

#define SMN_FUNC0_PCIE2_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11300433UL

#define SMN_FUNC0_PCIE2_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11300434UL

#define SMN_FUNC0_PCIE2_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11300435UL

#define SMN_FUNC0_PCIE2_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11300436UL

#define SMN_FUNC0_PCIE2_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11300437UL

#define SMN_FUNC0_PCIE2_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11300438UL

#define SMN_FUNC0_PCIE2_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11300439UL

#define SMN_FUNC0_PCIE3_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11400430UL

#define SMN_FUNC0_PCIE3_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1140043aUL

#define SMN_FUNC0_PCIE3_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1140043bUL

#define SMN_FUNC0_PCIE3_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1140043cUL

#define SMN_FUNC0_PCIE3_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1140043dUL

#define SMN_FUNC0_PCIE3_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1140043eUL

#define SMN_FUNC0_PCIE3_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1140043fUL

#define SMN_FUNC0_PCIE3_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11400431UL

#define SMN_FUNC0_PCIE3_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11400432UL

#define SMN_FUNC0_PCIE3_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11400433UL

#define SMN_FUNC0_PCIE3_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11400434UL

#define SMN_FUNC0_PCIE3_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11400435UL

#define SMN_FUNC0_PCIE3_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11400436UL

#define SMN_FUNC0_PCIE3_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11400437UL

#define SMN_FUNC0_PCIE3_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11400438UL

#define SMN_FUNC0_PCIE3_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11400439UL

#define SMN_FUNC1_PCIE0_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11101430UL

#define SMN_FUNC1_PCIE0_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110143aUL

#define SMN_FUNC1_PCIE0_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110143bUL

#define SMN_FUNC1_PCIE0_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110143cUL

#define SMN_FUNC1_PCIE0_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110143dUL

#define SMN_FUNC1_PCIE0_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110143eUL

#define SMN_FUNC1_PCIE0_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110143fUL

#define SMN_FUNC1_PCIE0_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11101431UL

#define SMN_FUNC1_PCIE0_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11101432UL

#define SMN_FUNC1_PCIE0_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11101433UL

#define SMN_FUNC1_PCIE0_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11101434UL

#define SMN_FUNC1_PCIE0_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11101435UL

#define SMN_FUNC1_PCIE0_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11101436UL

#define SMN_FUNC1_PCIE0_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11101437UL

#define SMN_FUNC1_PCIE0_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11101438UL

#define SMN_FUNC1_PCIE0_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11101439UL

#define SMN_FUNC1_PCIE1_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11201430UL

#define SMN_FUNC1_PCIE1_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120143aUL

#define SMN_FUNC1_PCIE1_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120143bUL

#define SMN_FUNC1_PCIE1_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120143cUL

#define SMN_FUNC1_PCIE1_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120143dUL

#define SMN_FUNC1_PCIE1_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120143eUL

#define SMN_FUNC1_PCIE1_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120143fUL

#define SMN_FUNC1_PCIE1_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11201431UL

#define SMN_FUNC1_PCIE1_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11201432UL

#define SMN_FUNC1_PCIE1_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11201433UL

#define SMN_FUNC1_PCIE1_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11201434UL

#define SMN_FUNC1_PCIE1_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11201435UL

#define SMN_FUNC1_PCIE1_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11201436UL

#define SMN_FUNC1_PCIE1_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11201437UL

#define SMN_FUNC1_PCIE1_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11201438UL

#define SMN_FUNC1_PCIE1_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11201439UL

#define SMN_FUNC2_PCIE0_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11102430UL

#define SMN_FUNC2_PCIE0_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110243aUL

#define SMN_FUNC2_PCIE0_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110243bUL

#define SMN_FUNC2_PCIE0_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110243cUL

#define SMN_FUNC2_PCIE0_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110243dUL

#define SMN_FUNC2_PCIE0_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110243eUL

#define SMN_FUNC2_PCIE0_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110243fUL

#define SMN_FUNC2_PCIE0_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11102431UL

#define SMN_FUNC2_PCIE0_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11102432UL

#define SMN_FUNC2_PCIE0_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11102433UL

#define SMN_FUNC2_PCIE0_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11102434UL

#define SMN_FUNC2_PCIE0_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11102435UL

#define SMN_FUNC2_PCIE0_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11102436UL

#define SMN_FUNC2_PCIE0_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11102437UL

#define SMN_FUNC2_PCIE0_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11102438UL

#define SMN_FUNC2_PCIE0_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11102439UL

#define SMN_FUNC2_PCIE1_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11202430UL

#define SMN_FUNC2_PCIE1_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120243aUL

#define SMN_FUNC2_PCIE1_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120243bUL

#define SMN_FUNC2_PCIE1_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120243cUL

#define SMN_FUNC2_PCIE1_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120243dUL

#define SMN_FUNC2_PCIE1_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120243eUL

#define SMN_FUNC2_PCIE1_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120243fUL

#define SMN_FUNC2_PCIE1_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11202431UL

#define SMN_FUNC2_PCIE1_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11202432UL

#define SMN_FUNC2_PCIE1_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11202433UL

#define SMN_FUNC2_PCIE1_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11202434UL

#define SMN_FUNC2_PCIE1_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11202435UL

#define SMN_FUNC2_PCIE1_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11202436UL

#define SMN_FUNC2_PCIE1_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11202437UL

#define SMN_FUNC2_PCIE1_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11202438UL

#define SMN_FUNC2_PCIE1_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11202439UL

#define SMN_FUNC3_PCIE0_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11103430UL

#define SMN_FUNC3_PCIE0_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110343aUL

#define SMN_FUNC3_PCIE0_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110343bUL

#define SMN_FUNC3_PCIE0_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110343cUL

#define SMN_FUNC3_PCIE0_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110343dUL

#define SMN_FUNC3_PCIE0_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110343eUL

#define SMN_FUNC3_PCIE0_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110343fUL

#define SMN_FUNC3_PCIE0_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11103431UL

#define SMN_FUNC3_PCIE0_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11103432UL

#define SMN_FUNC3_PCIE0_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11103433UL

#define SMN_FUNC3_PCIE0_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11103434UL

#define SMN_FUNC3_PCIE0_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11103435UL

#define SMN_FUNC3_PCIE0_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11103436UL

#define SMN_FUNC3_PCIE0_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11103437UL

#define SMN_FUNC3_PCIE0_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11103438UL

#define SMN_FUNC3_PCIE0_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11103439UL

#define SMN_FUNC3_PCIE1_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11203430UL

#define SMN_FUNC3_PCIE1_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120343aUL

#define SMN_FUNC3_PCIE1_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120343bUL

#define SMN_FUNC3_PCIE1_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120343cUL

#define SMN_FUNC3_PCIE1_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120343dUL

#define SMN_FUNC3_PCIE1_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120343eUL

#define SMN_FUNC3_PCIE1_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120343fUL

#define SMN_FUNC3_PCIE1_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11203431UL

#define SMN_FUNC3_PCIE1_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11203432UL

#define SMN_FUNC3_PCIE1_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11203433UL

#define SMN_FUNC3_PCIE1_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11203434UL

#define SMN_FUNC3_PCIE1_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11203435UL

#define SMN_FUNC3_PCIE1_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11203436UL

#define SMN_FUNC3_PCIE1_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11203437UL

#define SMN_FUNC3_PCIE1_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11203438UL

#define SMN_FUNC3_PCIE1_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11203439UL

#define SMN_FUNC4_PCIE0_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11104430UL

#define SMN_FUNC4_PCIE0_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110443aUL

#define SMN_FUNC4_PCIE0_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110443bUL

#define SMN_FUNC4_PCIE0_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110443cUL

#define SMN_FUNC4_PCIE0_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110443dUL

#define SMN_FUNC4_PCIE0_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110443eUL

#define SMN_FUNC4_PCIE0_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1110443fUL

#define SMN_FUNC4_PCIE0_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11104431UL

#define SMN_FUNC4_PCIE0_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11104432UL

#define SMN_FUNC4_PCIE0_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11104433UL

#define SMN_FUNC4_PCIE0_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11104434UL

#define SMN_FUNC4_PCIE0_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11104435UL

#define SMN_FUNC4_PCIE0_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11104436UL

#define SMN_FUNC4_PCIE0_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11104437UL

#define SMN_FUNC4_PCIE0_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11104438UL

#define SMN_FUNC4_PCIE0_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11104439UL

#define SMN_FUNC4_PCIE1_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11204430UL

#define SMN_FUNC4_PCIE1_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120443aUL

#define SMN_FUNC4_PCIE1_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120443bUL

#define SMN_FUNC4_PCIE1_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120443cUL

#define SMN_FUNC4_PCIE1_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120443dUL

#define SMN_FUNC4_PCIE1_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120443eUL

#define SMN_FUNC4_PCIE1_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120443fUL

#define SMN_FUNC4_PCIE1_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11204431UL

#define SMN_FUNC4_PCIE1_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11204432UL

#define SMN_FUNC4_PCIE1_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11204433UL

#define SMN_FUNC4_PCIE1_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11204434UL

#define SMN_FUNC4_PCIE1_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11204435UL

#define SMN_FUNC4_PCIE1_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11204436UL

#define SMN_FUNC4_PCIE1_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11204437UL

#define SMN_FUNC4_PCIE1_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11204438UL

#define SMN_FUNC4_PCIE1_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11204439UL

#define SMN_FUNC5_PCIE1_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11205430UL

#define SMN_FUNC5_PCIE1_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120543aUL

#define SMN_FUNC5_PCIE1_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120543bUL

#define SMN_FUNC5_PCIE1_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120543cUL

#define SMN_FUNC5_PCIE1_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120543dUL

#define SMN_FUNC5_PCIE1_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120543eUL

#define SMN_FUNC5_PCIE1_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1120543fUL

#define SMN_FUNC5_PCIE1_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11205431UL

#define SMN_FUNC5_PCIE1_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11205432UL

#define SMN_FUNC5_PCIE1_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11205433UL

#define SMN_FUNC5_PCIE1_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11205434UL

#define SMN_FUNC5_PCIE1_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11205435UL

#define SMN_FUNC5_PCIE1_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11205436UL

#define SMN_FUNC5_PCIE1_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11205437UL

#define SMN_FUNC5_PCIE1_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11205438UL

#define SMN_FUNC5_PCIE1_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x11205439UL


/***********************************************************
* Register Name : LANE_EQUALIZATION_CNTL_32GT
* Register Description :
* 32.0 GT/s Lane 0 Equalization Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Selects the Downstream Port's transmitter preset for initial operation at 32.0 GT/s. For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define LANE_EQUALIZATION_CNTL_32GT_LANE_DSP_32GT_TX_PRESET_OFFSET      0
#define LANE_EQUALIZATION_CNTL_32GT_LANE_DSP_32GT_TX_PRESET_MASK        0xf

// Bitfield Description : Selects the transmitter preset value that the Downstream Port requests the other side to use for initial operation at 32.0 GT/s.
// (Upstream Ports only) captures the transmitter preset value that the Upstream Port received from the other side for initial operation at 32.0 GT/s.
#define LANE_EQUALIZATION_CNTL_32GT_LANE_USP_32GT_TX_PRESET_OFFSET      4
#define LANE_EQUALIZATION_CNTL_32GT_LANE_USP_32GT_TX_PRESET_MASK        0xf0

typedef union {
  struct {
    UINT8                            LANE_DSP_32GT_TX_PRESET:4;
    UINT8                            LANE_USP_32GT_TX_PRESET:4;
  } Field;
  UINT8 Value;
} LANE_EQUALIZATION_CNTL_32GT_STRUCT;

#define SMN_FUNC0_PCIE0_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11100520UL

#define SMN_FUNC0_PCIE0_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110052aUL

#define SMN_FUNC0_PCIE0_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110052bUL

#define SMN_FUNC0_PCIE0_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110052cUL

#define SMN_FUNC0_PCIE0_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110052dUL

#define SMN_FUNC0_PCIE0_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110052eUL

#define SMN_FUNC0_PCIE0_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110052fUL

#define SMN_FUNC0_PCIE0_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11100521UL

#define SMN_FUNC0_PCIE0_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11100522UL

#define SMN_FUNC0_PCIE0_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11100523UL

#define SMN_FUNC0_PCIE0_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11100524UL

#define SMN_FUNC0_PCIE0_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11100525UL

#define SMN_FUNC0_PCIE0_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11100526UL

#define SMN_FUNC0_PCIE0_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11100527UL

#define SMN_FUNC0_PCIE0_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11100528UL

#define SMN_FUNC0_PCIE0_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11100529UL

#define SMN_FUNC0_PCIE1_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11200520UL

#define SMN_FUNC0_PCIE1_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120052aUL

#define SMN_FUNC0_PCIE1_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120052bUL

#define SMN_FUNC0_PCIE1_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120052cUL

#define SMN_FUNC0_PCIE1_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120052dUL

#define SMN_FUNC0_PCIE1_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120052eUL

#define SMN_FUNC0_PCIE1_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120052fUL

#define SMN_FUNC0_PCIE1_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11200521UL

#define SMN_FUNC0_PCIE1_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11200522UL

#define SMN_FUNC0_PCIE1_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11200523UL

#define SMN_FUNC0_PCIE1_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11200524UL

#define SMN_FUNC0_PCIE1_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11200525UL

#define SMN_FUNC0_PCIE1_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11200526UL

#define SMN_FUNC0_PCIE1_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11200527UL

#define SMN_FUNC0_PCIE1_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11200528UL

#define SMN_FUNC0_PCIE1_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11200529UL

#define SMN_FUNC0_PCIE2_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11300520UL

#define SMN_FUNC0_PCIE2_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1130052aUL

#define SMN_FUNC0_PCIE2_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1130052bUL

#define SMN_FUNC0_PCIE2_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1130052cUL

#define SMN_FUNC0_PCIE2_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1130052dUL

#define SMN_FUNC0_PCIE2_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1130052eUL

#define SMN_FUNC0_PCIE2_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1130052fUL

#define SMN_FUNC0_PCIE2_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11300521UL

#define SMN_FUNC0_PCIE2_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11300522UL

#define SMN_FUNC0_PCIE2_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11300523UL

#define SMN_FUNC0_PCIE2_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11300524UL

#define SMN_FUNC0_PCIE2_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11300525UL

#define SMN_FUNC0_PCIE2_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11300526UL

#define SMN_FUNC0_PCIE2_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11300527UL

#define SMN_FUNC0_PCIE2_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11300528UL

#define SMN_FUNC0_PCIE2_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11300529UL

#define SMN_FUNC0_PCIE3_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11400520UL

#define SMN_FUNC0_PCIE3_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1140052aUL

#define SMN_FUNC0_PCIE3_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1140052bUL

#define SMN_FUNC0_PCIE3_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1140052cUL

#define SMN_FUNC0_PCIE3_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1140052dUL

#define SMN_FUNC0_PCIE3_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1140052eUL

#define SMN_FUNC0_PCIE3_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1140052fUL

#define SMN_FUNC0_PCIE3_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11400521UL

#define SMN_FUNC0_PCIE3_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11400522UL

#define SMN_FUNC0_PCIE3_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11400523UL

#define SMN_FUNC0_PCIE3_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11400524UL

#define SMN_FUNC0_PCIE3_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11400525UL

#define SMN_FUNC0_PCIE3_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11400526UL

#define SMN_FUNC0_PCIE3_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11400527UL

#define SMN_FUNC0_PCIE3_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11400528UL

#define SMN_FUNC0_PCIE3_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11400529UL

#define SMN_FUNC1_PCIE0_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11101520UL

#define SMN_FUNC1_PCIE0_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110152aUL

#define SMN_FUNC1_PCIE0_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110152bUL

#define SMN_FUNC1_PCIE0_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110152cUL

#define SMN_FUNC1_PCIE0_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110152dUL

#define SMN_FUNC1_PCIE0_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110152eUL

#define SMN_FUNC1_PCIE0_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110152fUL

#define SMN_FUNC1_PCIE0_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11101521UL

#define SMN_FUNC1_PCIE0_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11101522UL

#define SMN_FUNC1_PCIE0_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11101523UL

#define SMN_FUNC1_PCIE0_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11101524UL

#define SMN_FUNC1_PCIE0_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11101525UL

#define SMN_FUNC1_PCIE0_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11101526UL

#define SMN_FUNC1_PCIE0_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11101527UL

#define SMN_FUNC1_PCIE0_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11101528UL

#define SMN_FUNC1_PCIE0_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11101529UL

#define SMN_FUNC1_PCIE1_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11201520UL

#define SMN_FUNC1_PCIE1_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120152aUL

#define SMN_FUNC1_PCIE1_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120152bUL

#define SMN_FUNC1_PCIE1_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120152cUL

#define SMN_FUNC1_PCIE1_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120152dUL

#define SMN_FUNC1_PCIE1_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120152eUL

#define SMN_FUNC1_PCIE1_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120152fUL

#define SMN_FUNC1_PCIE1_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11201521UL

#define SMN_FUNC1_PCIE1_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11201522UL

#define SMN_FUNC1_PCIE1_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11201523UL

#define SMN_FUNC1_PCIE1_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11201524UL

#define SMN_FUNC1_PCIE1_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11201525UL

#define SMN_FUNC1_PCIE1_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11201526UL

#define SMN_FUNC1_PCIE1_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11201527UL

#define SMN_FUNC1_PCIE1_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11201528UL

#define SMN_FUNC1_PCIE1_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11201529UL

#define SMN_FUNC2_PCIE0_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11102520UL

#define SMN_FUNC2_PCIE0_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110252aUL

#define SMN_FUNC2_PCIE0_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110252bUL

#define SMN_FUNC2_PCIE0_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110252cUL

#define SMN_FUNC2_PCIE0_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110252dUL

#define SMN_FUNC2_PCIE0_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110252eUL

#define SMN_FUNC2_PCIE0_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110252fUL

#define SMN_FUNC2_PCIE0_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11102521UL

#define SMN_FUNC2_PCIE0_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11102522UL

#define SMN_FUNC2_PCIE0_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11102523UL

#define SMN_FUNC2_PCIE0_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11102524UL

#define SMN_FUNC2_PCIE0_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11102525UL

#define SMN_FUNC2_PCIE0_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11102526UL

#define SMN_FUNC2_PCIE0_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11102527UL

#define SMN_FUNC2_PCIE0_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11102528UL

#define SMN_FUNC2_PCIE0_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11102529UL

#define SMN_FUNC2_PCIE1_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11202520UL

#define SMN_FUNC2_PCIE1_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120252aUL

#define SMN_FUNC2_PCIE1_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120252bUL

#define SMN_FUNC2_PCIE1_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120252cUL

#define SMN_FUNC2_PCIE1_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120252dUL

#define SMN_FUNC2_PCIE1_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120252eUL

#define SMN_FUNC2_PCIE1_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120252fUL

#define SMN_FUNC2_PCIE1_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11202521UL

#define SMN_FUNC2_PCIE1_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11202522UL

#define SMN_FUNC2_PCIE1_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11202523UL

#define SMN_FUNC2_PCIE1_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11202524UL

#define SMN_FUNC2_PCIE1_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11202525UL

#define SMN_FUNC2_PCIE1_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11202526UL

#define SMN_FUNC2_PCIE1_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11202527UL

#define SMN_FUNC2_PCIE1_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11202528UL

#define SMN_FUNC2_PCIE1_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11202529UL

#define SMN_FUNC3_PCIE0_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11103520UL

#define SMN_FUNC3_PCIE0_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110352aUL

#define SMN_FUNC3_PCIE0_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110352bUL

#define SMN_FUNC3_PCIE0_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110352cUL

#define SMN_FUNC3_PCIE0_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110352dUL

#define SMN_FUNC3_PCIE0_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110352eUL

#define SMN_FUNC3_PCIE0_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110352fUL

#define SMN_FUNC3_PCIE0_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11103521UL

#define SMN_FUNC3_PCIE0_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11103522UL

#define SMN_FUNC3_PCIE0_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11103523UL

#define SMN_FUNC3_PCIE0_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11103524UL

#define SMN_FUNC3_PCIE0_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11103525UL

#define SMN_FUNC3_PCIE0_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11103526UL

#define SMN_FUNC3_PCIE0_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11103527UL

#define SMN_FUNC3_PCIE0_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11103528UL

#define SMN_FUNC3_PCIE0_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11103529UL

#define SMN_FUNC3_PCIE1_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11203520UL

#define SMN_FUNC3_PCIE1_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120352aUL

#define SMN_FUNC3_PCIE1_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120352bUL

#define SMN_FUNC3_PCIE1_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120352cUL

#define SMN_FUNC3_PCIE1_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120352dUL

#define SMN_FUNC3_PCIE1_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120352eUL

#define SMN_FUNC3_PCIE1_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120352fUL

#define SMN_FUNC3_PCIE1_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11203521UL

#define SMN_FUNC3_PCIE1_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11203522UL

#define SMN_FUNC3_PCIE1_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11203523UL

#define SMN_FUNC3_PCIE1_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11203524UL

#define SMN_FUNC3_PCIE1_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11203525UL

#define SMN_FUNC3_PCIE1_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11203526UL

#define SMN_FUNC3_PCIE1_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11203527UL

#define SMN_FUNC3_PCIE1_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11203528UL

#define SMN_FUNC3_PCIE1_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11203529UL

#define SMN_FUNC4_PCIE0_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11104520UL

#define SMN_FUNC4_PCIE0_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110452aUL

#define SMN_FUNC4_PCIE0_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110452bUL

#define SMN_FUNC4_PCIE0_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110452cUL

#define SMN_FUNC4_PCIE0_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110452dUL

#define SMN_FUNC4_PCIE0_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110452eUL

#define SMN_FUNC4_PCIE0_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1110452fUL

#define SMN_FUNC4_PCIE0_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11104521UL

#define SMN_FUNC4_PCIE0_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11104522UL

#define SMN_FUNC4_PCIE0_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11104523UL

#define SMN_FUNC4_PCIE0_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11104524UL

#define SMN_FUNC4_PCIE0_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11104525UL

#define SMN_FUNC4_PCIE0_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11104526UL

#define SMN_FUNC4_PCIE0_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11104527UL

#define SMN_FUNC4_PCIE0_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11104528UL

#define SMN_FUNC4_PCIE0_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11104529UL

#define SMN_FUNC4_PCIE1_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11204520UL

#define SMN_FUNC4_PCIE1_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120452aUL

#define SMN_FUNC4_PCIE1_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120452bUL

#define SMN_FUNC4_PCIE1_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120452cUL

#define SMN_FUNC4_PCIE1_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120452dUL

#define SMN_FUNC4_PCIE1_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120452eUL

#define SMN_FUNC4_PCIE1_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120452fUL

#define SMN_FUNC4_PCIE1_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11204521UL

#define SMN_FUNC4_PCIE1_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11204522UL

#define SMN_FUNC4_PCIE1_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11204523UL

#define SMN_FUNC4_PCIE1_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11204524UL

#define SMN_FUNC4_PCIE1_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11204525UL

#define SMN_FUNC4_PCIE1_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11204526UL

#define SMN_FUNC4_PCIE1_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11204527UL

#define SMN_FUNC4_PCIE1_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11204528UL

#define SMN_FUNC4_PCIE1_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11204529UL

#define SMN_FUNC5_PCIE1_N0_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11205520UL

#define SMN_FUNC5_PCIE1_N10_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120552aUL

#define SMN_FUNC5_PCIE1_N11_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120552bUL

#define SMN_FUNC5_PCIE1_N12_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120552cUL

#define SMN_FUNC5_PCIE1_N13_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120552dUL

#define SMN_FUNC5_PCIE1_N14_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120552eUL

#define SMN_FUNC5_PCIE1_N15_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x1120552fUL

#define SMN_FUNC5_PCIE1_N1_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11205521UL

#define SMN_FUNC5_PCIE1_N2_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11205522UL

#define SMN_FUNC5_PCIE1_N3_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11205523UL

#define SMN_FUNC5_PCIE1_N4_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11205524UL

#define SMN_FUNC5_PCIE1_N5_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11205525UL

#define SMN_FUNC5_PCIE1_N6_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11205526UL

#define SMN_FUNC5_PCIE1_N7_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11205527UL

#define SMN_FUNC5_PCIE1_N8_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11205528UL

#define SMN_FUNC5_PCIE1_N9_LANE_EQUALIZATION_CNTL_32GT_ADDRESS    0x11205529UL


/***********************************************************
* Register Name : LATENCY
* Register Description :
* Master Latency Timer register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Does not apply to PCI Express.
#define LATENCY_LATENCY_TIMER_OFFSET      0
#define LATENCY_LATENCY_TIMER_MASK        0xff

typedef union {
  struct {
    UINT8                            LATENCY_TIMER:8;
  } Field;
  UINT8 Value;
} LATENCY_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LATENCY_ADDRESS    0x1110000dUL

#define SMN_FUNC0_PCIE1_LATENCY_ADDRESS    0x1120000dUL

#define SMN_FUNC0_PCIE2_LATENCY_ADDRESS    0x1130000dUL

#define SMN_FUNC0_PCIE3_LATENCY_ADDRESS    0x1140000dUL

#define SMN_FUNC1_PCIE0_LATENCY_ADDRESS    0x1110100dUL

#define SMN_FUNC1_PCIE1_LATENCY_ADDRESS    0x1120100dUL

#define SMN_FUNC2_PCIE0_LATENCY_ADDRESS    0x1110200dUL

#define SMN_FUNC2_PCIE1_LATENCY_ADDRESS    0x1120200dUL

#define SMN_FUNC3_PCIE0_LATENCY_ADDRESS    0x1110300dUL

#define SMN_FUNC3_PCIE1_LATENCY_ADDRESS    0x1120300dUL

#define SMN_FUNC4_PCIE0_LATENCY_ADDRESS    0x1110400dUL

#define SMN_FUNC4_PCIE1_LATENCY_ADDRESS    0x1120400dUL

#define SMN_FUNC5_PCIE1_LATENCY_ADDRESS    0x1120500dUL


/***********************************************************
* Register Name : LINK_CAP
* Register Description :
* The Link Capabilities register identifies Link specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the maximum speed of the Link.
#define LINK_CAP_LINK_SPEED_OFFSET      0
#define LINK_CAP_LINK_SPEED_MASK        0xf

// Bitfield Description : Indicates the maximum width of the Link.
#define LINK_CAP_LINK_WIDTH_OFFSET      4
#define LINK_CAP_LINK_WIDTH_MASK        0x3f0

// Bitfield Description : Indicates the level of ASPM supported on the Link.
#define LINK_CAP_PM_SUPPORT_OFFSET      10
#define LINK_CAP_PM_SUPPORT_MASK        0xc00

// Bitfield Description : Indicates the L0s exit latency for the Link. The value reported indicates the length of time this Port requires to complete transition from L0s to L0.
#define LINK_CAP_L0S_EXIT_LATENCY_OFFSET      12
#define LINK_CAP_L0S_EXIT_LATENCY_MASK        0x7000

// Bitfield Description : Indicates the L1 exit latency for the Link. The value reported indicates the length of time this Port requires to complete transition from L1 to L0.
#define LINK_CAP_L1_EXIT_LATENCY_OFFSET      15
#define LINK_CAP_L1_EXIT_LATENCY_MASK        0x38000

// Bitfield Description : (Upstream Ports only) Indicates if the Port tolerates removal of REFCLK via the CLKREQ# mechanism when the Link is in L1 or L2/L3 Ready.
// For Downstream Ports, this bit is Reserved.
#define LINK_CAP_CLOCK_POWER_MANAGEMENT_OFFSET      18
#define LINK_CAP_CLOCK_POWER_MANAGEMENT_MASK        0x40000

// Bitfield Description : (Downstream Ports only) Indicates if the Port supports the detecting and reporting of a Surprise Down error condition.
// For Upstream Ports, this bit is Reserved.
#define LINK_CAP_SURPRISE_DOWN_ERR_REPORTING_OFFSET      19
#define LINK_CAP_SURPRISE_DOWN_ERR_REPORTING_MASK        0x80000

// Bitfield Description : (Downstream Ports only) Indicates if the Port supports the reporting of DL_Active state of the Data Link Control and Management State Machine.
// For Upstream Ports, this bit is Reserved.
#define LINK_CAP_DL_ACTIVE_REPORTING_CAPABLE_OFFSET      20
#define LINK_CAP_DL_ACTIVE_REPORTING_CAPABLE_MASK        0x100000

// Bitfield Description : (Downstream Ports only) Indicates if the Port supports the Link Bandwidth Notification status and interrupt mechanisms.
// For Upstream Ports, this bit is Reserved.
#define LINK_CAP_LINK_BW_NOTIFICATION_CAP_OFFSET      21
#define LINK_CAP_LINK_BW_NOTIFICATION_CAP_MASK        0x200000

// Bitfield Description : Indicates if the Port supports the ASPM Optionality functionality.
#define LINK_CAP_ASPM_OPTIONALITY_COMPLIANCE_OFFSET      22
#define LINK_CAP_ASPM_OPTIONALITY_COMPLIANCE_MASK        0x400000

// Bitfield Description : 
#define LINK_CAP_Reserved_23_23_OFFSET      23
#define LINK_CAP_Reserved_23_23_MASK        0x800000

// Bitfield Description : Indicates the Port number for the Link.
#define LINK_CAP_PORT_NUMBER_OFFSET      24
#define LINK_CAP_PORT_NUMBER_MASK        0xff000000

typedef union {
  struct {
    UINT32                            LINK_SPEED:4;
    UINT32                            LINK_WIDTH:6;
    UINT32                            PM_SUPPORT:2;
    UINT32                            L0S_EXIT_LATENCY:3;
    UINT32                            L1_EXIT_LATENCY:3;
    UINT32                            CLOCK_POWER_MANAGEMENT:1;
    UINT32                            SURPRISE_DOWN_ERR_REPORTING:1;
    UINT32                            DL_ACTIVE_REPORTING_CAPABLE:1;
    UINT32                            LINK_BW_NOTIFICATION_CAP:1;
    UINT32                            ASPM_OPTIONALITY_COMPLIANCE:1;
    UINT32                            Reserved_23_23:1;
    UINT32                            PORT_NUMBER:8;
  } Field;
  UINT32 Value;
} LINK_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_CAP_ADDRESS    0x11100064UL

#define SMN_FUNC0_PCIE1_LINK_CAP_ADDRESS    0x11200064UL

#define SMN_FUNC0_PCIE2_LINK_CAP_ADDRESS    0x11300064UL

#define SMN_FUNC0_PCIE3_LINK_CAP_ADDRESS    0x11400064UL

#define SMN_FUNC1_PCIE0_LINK_CAP_ADDRESS    0x11101064UL

#define SMN_FUNC1_PCIE1_LINK_CAP_ADDRESS    0x11201064UL

#define SMN_FUNC2_PCIE0_LINK_CAP_ADDRESS    0x11102064UL

#define SMN_FUNC2_PCIE1_LINK_CAP_ADDRESS    0x11202064UL

#define SMN_FUNC3_PCIE0_LINK_CAP_ADDRESS    0x11103064UL

#define SMN_FUNC3_PCIE1_LINK_CAP_ADDRESS    0x11203064UL

#define SMN_FUNC4_PCIE0_LINK_CAP_ADDRESS    0x11104064UL

#define SMN_FUNC4_PCIE1_LINK_CAP_ADDRESS    0x11204064UL

#define SMN_FUNC5_PCIE1_LINK_CAP_ADDRESS    0x11205064UL


/***********************************************************
* Register Name : LINK_CAP2
* Register Description :
* The Link Capabilities 2 register identifies Link specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define LINK_CAP2_Reserved_0_0_OFFSET      0
#define LINK_CAP2_Reserved_0_0_MASK        0x1

// Bitfield Description : Indicates the link speeds supported by the Port.
#define LINK_CAP2_SUPPORTED_LINK_SPEED_OFFSET      1
#define LINK_CAP2_SUPPORTED_LINK_SPEED_MASK        0xfe

// Bitfield Description : Indicates if the Port supports crosslinks.
#define LINK_CAP2_CROSSLINK_SUPPORTED_OFFSET      8
#define LINK_CAP2_CROSSLINK_SUPPORTED_MASK        0x100

// Bitfield Description : Indicates if the Port supports both SRIS and software control of the SKP ordered set transmission scheduling rate for the indicated speed(s).
#define LINK_CAP2_LOWER_SKP_OS_GEN_SUPPORT_OFFSET      9
#define LINK_CAP2_LOWER_SKP_OS_GEN_SUPPORT_MASK        0xfe00

// Bitfield Description : Indicates if the Port supports both SRIS and receiving SKP ordered sets at the SRNS rate, while running in SRIS, for the indicated speed(s).
#define LINK_CAP2_LOWER_SKP_OS_RCV_SUPPORT_OFFSET      16
#define LINK_CAP2_LOWER_SKP_OS_RCV_SUPPORT_MASK        0x7f0000

// Bitfield Description : Indicates if the Port supports Retimer presence detection.
#define LINK_CAP2_RTM1_PRESENCE_DET_SUPPORT_OFFSET      23
#define LINK_CAP2_RTM1_PRESENCE_DET_SUPPORT_MASK        0x800000

// Bitfield Description : Indicates if the Port supports two Retimers presence detection.
#define LINK_CAP2_RTM2_PRESENCE_DET_SUPPORT_OFFSET      24
#define LINK_CAP2_RTM2_PRESENCE_DET_SUPPORT_MASK        0x1000000

// Bitfield Description : 
#define LINK_CAP2_Reserved_30_25_OFFSET      25
#define LINK_CAP2_Reserved_30_25_MASK        0x7e000000

// Bitfield Description : Indicates if the function supports Device Readiness Status capability.
#define LINK_CAP2_DRS_SUPPORTED_OFFSET      31
#define LINK_CAP2_DRS_SUPPORTED_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_0_0:1;
    UINT32                            SUPPORTED_LINK_SPEED:7;
    UINT32                            CROSSLINK_SUPPORTED:1;
    UINT32                            LOWER_SKP_OS_GEN_SUPPORT:7;
    UINT32                            LOWER_SKP_OS_RCV_SUPPORT:7;
    UINT32                            RTM1_PRESENCE_DET_SUPPORT:1;
    UINT32                            RTM2_PRESENCE_DET_SUPPORT:1;
    UINT32                            Reserved_30_25:6;
    UINT32                            DRS_SUPPORTED:1;
  } Field;
  UINT32 Value;
} LINK_CAP2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_CAP2_ADDRESS    0x11100084UL

#define SMN_FUNC0_PCIE1_LINK_CAP2_ADDRESS    0x11200084UL

#define SMN_FUNC0_PCIE2_LINK_CAP2_ADDRESS    0x11300084UL

#define SMN_FUNC0_PCIE3_LINK_CAP2_ADDRESS    0x11400084UL

#define SMN_FUNC1_PCIE0_LINK_CAP2_ADDRESS    0x11101084UL

#define SMN_FUNC1_PCIE1_LINK_CAP2_ADDRESS    0x11201084UL

#define SMN_FUNC2_PCIE0_LINK_CAP2_ADDRESS    0x11102084UL

#define SMN_FUNC2_PCIE1_LINK_CAP2_ADDRESS    0x11202084UL

#define SMN_FUNC3_PCIE0_LINK_CAP2_ADDRESS    0x11103084UL

#define SMN_FUNC3_PCIE1_LINK_CAP2_ADDRESS    0x11203084UL

#define SMN_FUNC4_PCIE0_LINK_CAP2_ADDRESS    0x11104084UL

#define SMN_FUNC4_PCIE1_LINK_CAP2_ADDRESS    0x11204084UL

#define SMN_FUNC5_PCIE1_LINK_CAP2_ADDRESS    0x11205084UL


/***********************************************************
* Register Name : LINK_CAP_16GT
* Register Description :
* The 16.0 GT/s Capabilities register identifies Physical Layer 16.0 GT/s specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved.
#define LINK_CAP_16GT_Reserved_31_0_OFFSET      0
#define LINK_CAP_16GT_Reserved_31_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Reserved_31_0:32;
  } Field;
  UINT32 Value;
} LINK_CAP_16GT_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_CAP_16GT_ADDRESS    0x11100414UL

#define SMN_FUNC0_PCIE1_LINK_CAP_16GT_ADDRESS    0x11200414UL

#define SMN_FUNC0_PCIE2_LINK_CAP_16GT_ADDRESS    0x11300414UL

#define SMN_FUNC0_PCIE3_LINK_CAP_16GT_ADDRESS    0x11400414UL

#define SMN_FUNC1_PCIE0_LINK_CAP_16GT_ADDRESS    0x11101414UL

#define SMN_FUNC1_PCIE1_LINK_CAP_16GT_ADDRESS    0x11201414UL

#define SMN_FUNC2_PCIE0_LINK_CAP_16GT_ADDRESS    0x11102414UL

#define SMN_FUNC2_PCIE1_LINK_CAP_16GT_ADDRESS    0x11202414UL

#define SMN_FUNC3_PCIE0_LINK_CAP_16GT_ADDRESS    0x11103414UL

#define SMN_FUNC3_PCIE1_LINK_CAP_16GT_ADDRESS    0x11203414UL

#define SMN_FUNC4_PCIE0_LINK_CAP_16GT_ADDRESS    0x11104414UL

#define SMN_FUNC4_PCIE1_LINK_CAP_16GT_ADDRESS    0x11204414UL

#define SMN_FUNC5_PCIE1_LINK_CAP_16GT_ADDRESS    0x11205414UL


/***********************************************************
* Register Name : LINK_CAP_32GT
* Register Description :
* The 32.0 GT/s Capabilities register identifies Physical Layer 32.0 GT/s specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if this port supports controlling if link equalization for link speeds other than the highest common supported link speed are bypassed.
#define LINK_CAP_32GT_EQ_BYPASS_TO_HIGHEST_RATE_SUPPORTED_OFFSET      0
#define LINK_CAP_32GT_EQ_BYPASS_TO_HIGHEST_RATE_SUPPORTED_MASK        0x1

// Bitfield Description : Indicates if this port supports controlling if link equalization is needed.
#define LINK_CAP_32GT_NO_EQ_NEEDED_SUPPORTED_OFFSET      1
#define LINK_CAP_32GT_NO_EQ_NEEDED_SUPPORTED_MASK        0x2

// Bitfield Description : 
#define LINK_CAP_32GT_Reserved_7_2_OFFSET      2
#define LINK_CAP_32GT_Reserved_7_2_MASK        0xfc

// Bitfield Description : Indicates if this port supports standard PCI Express Training Set usage mode.
#define LINK_CAP_32GT_MODIFIED_TS_USAGE_MODE0_SUPPORTED_OFFSET      8
#define LINK_CAP_32GT_MODIFIED_TS_USAGE_MODE0_SUPPORTED_MASK        0x100

// Bitfield Description : Indicates if this port supports sending and receiving vendor specific Training Set Messages.
#define LINK_CAP_32GT_MODIFIED_TS_USAGE_MODE1_SUPPORTED_OFFSET      9
#define LINK_CAP_32GT_MODIFIED_TS_USAGE_MODE1_SUPPORTED_MASK        0x200

// Bitfield Description : Indicates if this port supports negotiating to use alternate Training Set protocols.
#define LINK_CAP_32GT_MODIFIED_TS_USAGE_MODE2_SUPPORTED_OFFSET      10
#define LINK_CAP_32GT_MODIFIED_TS_USAGE_MODE2_SUPPORTED_MASK        0x400

// Bitfield Description : Reserved.
#define LINK_CAP_32GT_MODIFIED_TS_RESERVED_USAGE_MODES_OFFSET      11
#define LINK_CAP_32GT_MODIFIED_TS_RESERVED_USAGE_MODES_MASK        0xf800

// Bitfield Description : 
#define LINK_CAP_32GT_Reserved_31_16_OFFSET      16
#define LINK_CAP_32GT_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            EQ_BYPASS_TO_HIGHEST_RATE_SUPPORTED:1;
    UINT32                            NO_EQ_NEEDED_SUPPORTED:1;
    UINT32                            Reserved_7_2:6;
    UINT32                            MODIFIED_TS_USAGE_MODE0_SUPPORTED:1;
    UINT32                            MODIFIED_TS_USAGE_MODE1_SUPPORTED:1;
    UINT32                            MODIFIED_TS_USAGE_MODE2_SUPPORTED:1;
    UINT32                            MODIFIED_TS_RESERVED_USAGE_MODES:5;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} LINK_CAP_32GT_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_CAP_32GT_ADDRESS    0x11100504UL

#define SMN_FUNC0_PCIE1_LINK_CAP_32GT_ADDRESS    0x11200504UL

#define SMN_FUNC0_PCIE2_LINK_CAP_32GT_ADDRESS    0x11300504UL

#define SMN_FUNC0_PCIE3_LINK_CAP_32GT_ADDRESS    0x11400504UL

#define SMN_FUNC1_PCIE0_LINK_CAP_32GT_ADDRESS    0x11101504UL

#define SMN_FUNC1_PCIE1_LINK_CAP_32GT_ADDRESS    0x11201504UL

#define SMN_FUNC2_PCIE0_LINK_CAP_32GT_ADDRESS    0x11102504UL

#define SMN_FUNC2_PCIE1_LINK_CAP_32GT_ADDRESS    0x11202504UL

#define SMN_FUNC3_PCIE0_LINK_CAP_32GT_ADDRESS    0x11103504UL

#define SMN_FUNC3_PCIE1_LINK_CAP_32GT_ADDRESS    0x11203504UL

#define SMN_FUNC4_PCIE0_LINK_CAP_32GT_ADDRESS    0x11104504UL

#define SMN_FUNC4_PCIE1_LINK_CAP_32GT_ADDRESS    0x11204504UL

#define SMN_FUNC5_PCIE1_LINK_CAP_32GT_ADDRESS    0x11205504UL


/***********************************************************
* Register Name : LINK_CNTL
* Register Description :
* The Link Control register controls Link specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the level of ASPM enabled on the Link.
#define LINK_CNTL_PM_CONTROL_OFFSET      0
#define LINK_CNTL_PM_CONTROL_MASK        0x3

// Bitfield Description : Controls the interpretation of the Propagation Delay[31:0] field of the PTM Message. For devices that don't support PTM, this bit is Reserved.
#define LINK_CNTL_PTM_PROP_DELAY_ADAPT_INTER_B_OFFSET      2
#define LINK_CNTL_PTM_PROP_DELAY_ADAPT_INTER_B_MASK        0x4

// Bitfield Description : Indicates the RCB value for the Root Port sourcing the topology associated with the Link.
// For Switches, this bit is Reserved.
#define LINK_CNTL_READ_CPL_BOUNDARY_OFFSET      3
#define LINK_CNTL_READ_CPL_BOUNDARY_MASK        0x8

// Bitfield Description : (Downstream Ports only) Disables the Link when set to 1, by directing the Physical Layer LTSSM to the Disabled state.
// For Upstream Ports, this bit is Reserved.
#define LINK_CNTL_LINK_DIS_OFFSET      4
#define LINK_CNTL_LINK_DIS_MASK        0x10

// Bitfield Description : (Downstream Ports only) A write of 1 to this bit initiates Link retraining by directing the Physical Layer LTSSM to the Recovery state. Reads always return 0.
// For Upstream Ports, this bit is Reserved.
#define LINK_CNTL_RETRAIN_LINK_OFFSET      5
#define LINK_CNTL_RETRAIN_LINK_MASK        0x20

// Bitfield Description : Indicates if this component and the component at the opposite end of this Link are operating with a distributed common reference clock.
#define LINK_CNTL_COMMON_CLOCK_CFG_OFFSET      6
#define LINK_CNTL_COMMON_CLOCK_CFG_MASK        0x40

// Bitfield Description : Forces the transmission of additional ordered sets, exiting the L0s or Recovery states.
#define LINK_CNTL_EXTENDED_SYNC_OFFSET      7
#define LINK_CNTL_EXTENDED_SYNC_MASK        0x80

// Bitfield Description : (Upstream Ports only) Controls if the function is permitted to use CLKREQ# signal to power manage the Link clock.
// For Downstream Ports, this bit is Reserved.
#define LINK_CNTL_CLOCK_POWER_MANAGEMENT_EN_OFFSET      8
#define LINK_CNTL_CLOCK_POWER_MANAGEMENT_EN_MASK        0x100

// Bitfield Description : Controls the Port's ability to autonomously direct changes in link width.
#define LINK_CNTL_HW_AUTONOMOUS_WIDTH_DISABLE_OFFSET      9
#define LINK_CNTL_HW_AUTONOMOUS_WIDTH_DISABLE_MASK        0x200

// Bitfield Description : (Downstream Ports only) Controls if the function generates an interrupt to indicate that the Link Bandwidth Management Status bit has been set.
// For Upstream Ports, this bit is Reserved.
#define LINK_CNTL_LINK_BW_MANAGEMENT_INT_EN_OFFSET      10
#define LINK_CNTL_LINK_BW_MANAGEMENT_INT_EN_MASK        0x400

// Bitfield Description : (Downstream Ports only) Controls if the function generates an interrupt to indicate that the Link Autonomous Bandwidth Status bit has been set.
// For Upstream Ports, this bit is Reserved.
#define LINK_CNTL_LINK_AUTONOMOUS_BW_INT_EN_OFFSET      11
#define LINK_CNTL_LINK_AUTONOMOUS_BW_INT_EN_MASK        0x800

// Bitfield Description : 
#define LINK_CNTL_Reserved_13_12_OFFSET      12
#define LINK_CNTL_Reserved_13_12_MASK        0x3000

// Bitfield Description : (Downstream Ports only) When DRS Supported bit is set, controls the mechanism used to report reception of a DRS Message.
// For Upstream Ports, this field is Reserved.
#define LINK_CNTL_DRS_SIGNALING_CONTROL_OFFSET      14
#define LINK_CNTL_DRS_SIGNALING_CONTROL_MASK        0xc000

typedef union {
  struct {
    UINT16                            PM_CONTROL:2;
    UINT16                            PTM_PROP_DELAY_ADAPT_INTER_B:1;
    UINT16                            READ_CPL_BOUNDARY:1;
    UINT16                            LINK_DIS:1;
    UINT16                            RETRAIN_LINK:1;
    UINT16                            COMMON_CLOCK_CFG:1;
    UINT16                            EXTENDED_SYNC:1;
    UINT16                            CLOCK_POWER_MANAGEMENT_EN:1;
    UINT16                            HW_AUTONOMOUS_WIDTH_DISABLE:1;
    UINT16                            LINK_BW_MANAGEMENT_INT_EN:1;
    UINT16                            LINK_AUTONOMOUS_BW_INT_EN:1;
    UINT16                            Reserved_13_12:2;
    UINT16                            DRS_SIGNALING_CONTROL:2;
  } Field;
  UINT16 Value;
} LINK_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_CNTL_ADDRESS    0x11100068UL

#define SMN_FUNC0_PCIE1_LINK_CNTL_ADDRESS    0x11200068UL

#define SMN_FUNC0_PCIE2_LINK_CNTL_ADDRESS    0x11300068UL

#define SMN_FUNC0_PCIE3_LINK_CNTL_ADDRESS    0x11400068UL

#define SMN_FUNC1_PCIE0_LINK_CNTL_ADDRESS    0x11101068UL

#define SMN_FUNC1_PCIE1_LINK_CNTL_ADDRESS    0x11201068UL

#define SMN_FUNC2_PCIE0_LINK_CNTL_ADDRESS    0x11102068UL

#define SMN_FUNC2_PCIE1_LINK_CNTL_ADDRESS    0x11202068UL

#define SMN_FUNC3_PCIE0_LINK_CNTL_ADDRESS    0x11103068UL

#define SMN_FUNC3_PCIE1_LINK_CNTL_ADDRESS    0x11203068UL

#define SMN_FUNC4_PCIE0_LINK_CNTL_ADDRESS    0x11104068UL

#define SMN_FUNC4_PCIE1_LINK_CNTL_ADDRESS    0x11204068UL

#define SMN_FUNC5_PCIE1_LINK_CNTL_ADDRESS    0x11205068UL


/***********************************************************
* Register Name : LINK_CNTL2
* Register Description :
* The Link Control 2 register controls Link specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : (Downstream Ports only) Controls the upper limit on the Link operational speed by restricting the data rate values advertised by the upstream component. When using Enter Compliance bit, this field controls the Port's target compliance mode speed. This field is sticky.
#define LINK_CNTL2_TARGET_LINK_SPEED_OFFSET      0
#define LINK_CNTL2_TARGET_LINK_SPEED_MASK        0xf

// Bitfield Description : Forces a Port to enter compliance mode, when it receives a hot reset. This bit is sticky, but for Upstream Ports only is cleared if the link state goes to Detect.
#define LINK_CNTL2_ENTER_COMPLIANCE_OFFSET      4
#define LINK_CNTL2_ENTER_COMPLIANCE_MASK        0x10

// Bitfield Description : Controls the Port's ability to autonomously direct changes in link speed. This bit is sticky.
#define LINK_CNTL2_HW_AUTONOMOUS_SPEED_DISABLE_OFFSET      5
#define LINK_CNTL2_HW_AUTONOMOUS_SPEED_DISABLE_MASK        0x20

// Bitfield Description : (Downstream Ports only) At 5.0 GT/s only, controls the Port's transmitter de-emphasis level.
#define LINK_CNTL2_SELECTABLE_DEEMPHASIS_OFFSET      6
#define LINK_CNTL2_SELECTABLE_DEEMPHASIS_MASK        0x40

// Bitfield Description : Controls the value of the non-deemphasized voltage level at the Port's transmitter pins. This field is sticky.
#define LINK_CNTL2_XMIT_MARGIN_OFFSET      7
#define LINK_CNTL2_XMIT_MARGIN_MASK        0x380

// Bitfield Description : Controls which compliance pattern the Port transmits, when Enter Compliance bit is used to enter compliance mode. This bit is sticky.
#define LINK_CNTL2_ENTER_MOD_COMPLIANCE_OFFSET      10
#define LINK_CNTL2_ENTER_MOD_COMPLIANCE_MASK        0x400

// Bitfield Description : At 5.0 GT/s or lower only, controls if the Port's transmitter inserts SKP ordered sets when sending either compliance pattern. This bit is sticky.
#define LINK_CNTL2_COMPLIANCE_SOS_OFFSET      11
#define LINK_CNTL2_COMPLIANCE_SOS_MASK        0x800

// Bitfield Description : At 5.0 GT/s only, when using Enter Compliance bit, this field controls the Port's transmitter de-emphasis level in compliance mode.
// At 8.0 GT/s or higher only, when using Enter Compliance bit, this field controls the Port's transmitter Preset setting in compliance mode. This field is sticky.
#define LINK_CNTL2_COMPLIANCE_DEEMPHASIS_OFFSET      12
#define LINK_CNTL2_COMPLIANCE_DEEMPHASIS_MASK        0xf000

typedef union {
  struct {
    UINT16                            TARGET_LINK_SPEED:4;
    UINT16                            ENTER_COMPLIANCE:1;
    UINT16                            HW_AUTONOMOUS_SPEED_DISABLE:1;
    UINT16                            SELECTABLE_DEEMPHASIS:1;
    UINT16                            XMIT_MARGIN:3;
    UINT16                            ENTER_MOD_COMPLIANCE:1;
    UINT16                            COMPLIANCE_SOS:1;
    UINT16                            COMPLIANCE_DEEMPHASIS:4;
  } Field;
  UINT16 Value;
} LINK_CNTL2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_CNTL2_ADDRESS    0x11100088UL

#define SMN_FUNC0_PCIE1_LINK_CNTL2_ADDRESS    0x11200088UL

#define SMN_FUNC0_PCIE2_LINK_CNTL2_ADDRESS    0x11300088UL

#define SMN_FUNC0_PCIE3_LINK_CNTL2_ADDRESS    0x11400088UL

#define SMN_FUNC1_PCIE0_LINK_CNTL2_ADDRESS    0x11101088UL

#define SMN_FUNC1_PCIE1_LINK_CNTL2_ADDRESS    0x11201088UL

#define SMN_FUNC2_PCIE0_LINK_CNTL2_ADDRESS    0x11102088UL

#define SMN_FUNC2_PCIE1_LINK_CNTL2_ADDRESS    0x11202088UL

#define SMN_FUNC3_PCIE0_LINK_CNTL2_ADDRESS    0x11103088UL

#define SMN_FUNC3_PCIE1_LINK_CNTL2_ADDRESS    0x11203088UL

#define SMN_FUNC4_PCIE0_LINK_CNTL2_ADDRESS    0x11104088UL

#define SMN_FUNC4_PCIE1_LINK_CNTL2_ADDRESS    0x11204088UL

#define SMN_FUNC5_PCIE1_LINK_CNTL2_ADDRESS    0x11205088UL


/***********************************************************
* Register Name : LINK_CNTL_16GT
* Register Description :
* The 16.0 GT/s Control register controls Physical Layer 16.0 GT/s specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved.
#define LINK_CNTL_16GT_Reserved_31_0_OFFSET      0
#define LINK_CNTL_16GT_Reserved_31_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Reserved_31_0:32;
  } Field;
  UINT32 Value;
} LINK_CNTL_16GT_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_CNTL_16GT_ADDRESS    0x11100418UL

#define SMN_FUNC0_PCIE1_LINK_CNTL_16GT_ADDRESS    0x11200418UL

#define SMN_FUNC0_PCIE2_LINK_CNTL_16GT_ADDRESS    0x11300418UL

#define SMN_FUNC0_PCIE3_LINK_CNTL_16GT_ADDRESS    0x11400418UL

#define SMN_FUNC1_PCIE0_LINK_CNTL_16GT_ADDRESS    0x11101418UL

#define SMN_FUNC1_PCIE1_LINK_CNTL_16GT_ADDRESS    0x11201418UL

#define SMN_FUNC2_PCIE0_LINK_CNTL_16GT_ADDRESS    0x11102418UL

#define SMN_FUNC2_PCIE1_LINK_CNTL_16GT_ADDRESS    0x11202418UL

#define SMN_FUNC3_PCIE0_LINK_CNTL_16GT_ADDRESS    0x11103418UL

#define SMN_FUNC3_PCIE1_LINK_CNTL_16GT_ADDRESS    0x11203418UL

#define SMN_FUNC4_PCIE0_LINK_CNTL_16GT_ADDRESS    0x11104418UL

#define SMN_FUNC4_PCIE1_LINK_CNTL_16GT_ADDRESS    0x11204418UL

#define SMN_FUNC5_PCIE1_LINK_CNTL_16GT_ADDRESS    0x11205418UL


/***********************************************************
* Register Name : LINK_CNTL_32GT
* Register Description :
* The 32.0 GT/s Control register controls Physical Layer 32.0 GT/s specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls a port's ability to advertise that link equalization for link speeds other than the highest common supported link speed is bypassed. This bit is sticky.
#define LINK_CNTL_32GT_EQ_BYPASS_TO_HIGHEST_RATE_DIS_OFFSET      0
#define LINK_CNTL_32GT_EQ_BYPASS_TO_HIGHEST_RATE_DIS_MASK        0x1

// Bitfield Description : Controls a port's ability to advertise that link equalization is needed. This bit is sticky.
#define LINK_CNTL_32GT_NO_EQ_NEEDED_DIS_OFFSET      1
#define LINK_CNTL_32GT_NO_EQ_NEEDED_DIS_MASK        0x2

// Bitfield Description : 
#define LINK_CNTL_32GT_Reserved_7_2_OFFSET      2
#define LINK_CNTL_32GT_Reserved_7_2_MASK        0xfc

// Bitfield Description : (Downstream Ports only) Configures the Training Set usage mode by selecting one of the supported Training Set usage modes. This bit is sticky.
// For Upstream Ports, this bit is Reserved.
#define LINK_CNTL_32GT_MODIFIED_TS_USAGE_MODE_SEL_OFFSET      8
#define LINK_CNTL_32GT_MODIFIED_TS_USAGE_MODE_SEL_MASK        0x700

// Bitfield Description : 
#define LINK_CNTL_32GT_Reserved_31_11_OFFSET      11
#define LINK_CNTL_32GT_Reserved_31_11_MASK        0xfffff800

typedef union {
  struct {
    UINT32                            EQ_BYPASS_TO_HIGHEST_RATE_DIS:1;
    UINT32                            NO_EQ_NEEDED_DIS:1;
    UINT32                            Reserved_7_2:6;
    UINT32                            MODIFIED_TS_USAGE_MODE_SEL:3;
    UINT32                            Reserved_31_11:21;
  } Field;
  UINT32 Value;
} LINK_CNTL_32GT_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_CNTL_32GT_ADDRESS    0x11100508UL

#define SMN_FUNC0_PCIE1_LINK_CNTL_32GT_ADDRESS    0x11200508UL

#define SMN_FUNC0_PCIE2_LINK_CNTL_32GT_ADDRESS    0x11300508UL

#define SMN_FUNC0_PCIE3_LINK_CNTL_32GT_ADDRESS    0x11400508UL

#define SMN_FUNC1_PCIE0_LINK_CNTL_32GT_ADDRESS    0x11101508UL

#define SMN_FUNC1_PCIE1_LINK_CNTL_32GT_ADDRESS    0x11201508UL

#define SMN_FUNC2_PCIE0_LINK_CNTL_32GT_ADDRESS    0x11102508UL

#define SMN_FUNC2_PCIE1_LINK_CNTL_32GT_ADDRESS    0x11202508UL

#define SMN_FUNC3_PCIE0_LINK_CNTL_32GT_ADDRESS    0x11103508UL

#define SMN_FUNC3_PCIE1_LINK_CNTL_32GT_ADDRESS    0x11203508UL

#define SMN_FUNC4_PCIE0_LINK_CNTL_32GT_ADDRESS    0x11104508UL

#define SMN_FUNC4_PCIE1_LINK_CNTL_32GT_ADDRESS    0x11204508UL

#define SMN_FUNC5_PCIE1_LINK_CNTL_32GT_ADDRESS    0x11205508UL


/***********************************************************
* Register Name : LINK_STATUS
* Register Description :
* The Link Status register provides information about Link specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the negotiated speed of the Link.
#define LINK_STATUS_CURRENT_LINK_SPEED_OFFSET      0
#define LINK_STATUS_CURRENT_LINK_SPEED_MASK        0xf

// Bitfield Description : Indicates the negotiated width of the Link.
#define LINK_STATUS_NEGOTIATED_LINK_WIDTH_OFFSET      4
#define LINK_STATUS_NEGOTIATED_LINK_WIDTH_MASK        0x3f0

// Bitfield Description : 
#define LINK_STATUS_Reserved_10_10_OFFSET      10
#define LINK_STATUS_Reserved_10_10_MASK        0x400

// Bitfield Description : (Downstream Ports only) Indicates that Link training is in progress (Physical Layer LTSSM in Configuration or Recovery state) or that 1 was written to the Retrain Link bit but Link training has not yet begun. Hardware clears this bit once Link training is complete.
// For Upstream Ports, this bit is Reserved.
#define LINK_STATUS_LINK_TRAINING_OFFSET      11
#define LINK_STATUS_LINK_TRAINING_MASK        0x800

// Bitfield Description : Indicates if the component uses the same physical reference clock that the platform provides on the connector.
#define LINK_STATUS_SLOT_CLOCK_CFG_OFFSET      12
#define LINK_STATUS_SLOT_CLOCK_CFG_MASK        0x1000

// Bitfield Description : If Data Link Layer Link Active Capability is supported and when the optionally supported SFI DLL State Mask bit is clear, indicates the status of the Data Link Control and Management State Machine.
#define LINK_STATUS_DL_ACTIVE_OFFSET      13
#define LINK_STATUS_DL_ACTIVE_MASK        0x2000

// Bitfield Description : Indicates that either of the following has occurred without the Port transitioning through DL_Down status:
// A Link retraining has completed following a write of 1 to the Retrain Link bit.
// Hardware has changed the Link's speed or width to attempt to correct unreliable Link operation, either through an LTSSM timeout or a higher level process.
#define LINK_STATUS_LINK_BW_MANAGEMENT_STATUS_OFFSET      14
#define LINK_STATUS_LINK_BW_MANAGEMENT_STATUS_MASK        0x4000

// Bitfield Description : Indicates that hardware has autonomously changed the Link's speed or width, without the Port transitioning through DL_Down status, for reasons other than to attempt to correct unreliable Link operation.
#define LINK_STATUS_LINK_AUTONOMOUS_BW_STATUS_OFFSET      15
#define LINK_STATUS_LINK_AUTONOMOUS_BW_STATUS_MASK        0x8000

typedef union {
  struct {
    UINT16                            CURRENT_LINK_SPEED:4;
    UINT16                            NEGOTIATED_LINK_WIDTH:6;
    UINT16                            Reserved_10_10:1;
    UINT16                            LINK_TRAINING:1;
    UINT16                            SLOT_CLOCK_CFG:1;
    UINT16                            DL_ACTIVE:1;
    UINT16                            LINK_BW_MANAGEMENT_STATUS:1;
    UINT16                            LINK_AUTONOMOUS_BW_STATUS:1;
  } Field;
  UINT16 Value;
} LINK_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_STATUS_ADDRESS    0x1110006aUL

#define SMN_FUNC0_PCIE1_LINK_STATUS_ADDRESS    0x1120006aUL

#define SMN_FUNC0_PCIE2_LINK_STATUS_ADDRESS    0x1130006aUL

#define SMN_FUNC0_PCIE3_LINK_STATUS_ADDRESS    0x1140006aUL

#define SMN_FUNC1_PCIE0_LINK_STATUS_ADDRESS    0x1110106aUL

#define SMN_FUNC1_PCIE1_LINK_STATUS_ADDRESS    0x1120106aUL

#define SMN_FUNC2_PCIE0_LINK_STATUS_ADDRESS    0x1110206aUL

#define SMN_FUNC2_PCIE1_LINK_STATUS_ADDRESS    0x1120206aUL

#define SMN_FUNC3_PCIE0_LINK_STATUS_ADDRESS    0x1110306aUL

#define SMN_FUNC3_PCIE1_LINK_STATUS_ADDRESS    0x1120306aUL

#define SMN_FUNC4_PCIE0_LINK_STATUS_ADDRESS    0x1110406aUL

#define SMN_FUNC4_PCIE1_LINK_STATUS_ADDRESS    0x1120406aUL

#define SMN_FUNC5_PCIE1_LINK_STATUS_ADDRESS    0x1120506aUL


/***********************************************************
* Register Name : LINK_STATUS2
* Register Description :
* The Link Status 2 register provides information about Link specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : At 5.0 GT/s only, indicates the Port's transmitter de-emphasis level.
#define LINK_STATUS2_CUR_DEEMPHASIS_LEVEL_OFFSET      0
#define LINK_STATUS2_CUR_DEEMPHASIS_LEVEL_MASK        0x1

// Bitfield Description : Indicates if the 8.0 GT/s Transmitter Equalization procedure has completed. This bit is sticky.
#define LINK_STATUS2_EQUALIZATION_COMPLETE_8GT_OFFSET      1
#define LINK_STATUS2_EQUALIZATION_COMPLETE_8GT_MASK        0x2

// Bitfield Description : Indicates if Phase 1 of the 8.0 GT/s Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS2_EQUALIZATION_PHASE1_SUCCESS_8GT_OFFSET      2
#define LINK_STATUS2_EQUALIZATION_PHASE1_SUCCESS_8GT_MASK        0x4

// Bitfield Description : Indicates if Phase 2 of the 8.0 GT/s Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS2_EQUALIZATION_PHASE2_SUCCESS_8GT_OFFSET      3
#define LINK_STATUS2_EQUALIZATION_PHASE2_SUCCESS_8GT_MASK        0x8

// Bitfield Description : Indicates if Phase 3 of the 8.0 GT/s Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS2_EQUALIZATION_PHASE3_SUCCESS_8GT_OFFSET      4
#define LINK_STATUS2_EQUALIZATION_PHASE3_SUCCESS_8GT_MASK        0x10

// Bitfield Description : Indicates if hardware requests 8.0 GT/s link equalization to be performed. This bit is sticky.
#define LINK_STATUS2_LINK_EQUALIZATION_REQUEST_8GT_OFFSET      5
#define LINK_STATUS2_LINK_EQUALIZATION_REQUEST_8GT_MASK        0x20

// Bitfield Description : When Retimer Presence Detect Supported bit is set, indicates if the Port detected a Retimer in the most recent link negotiation. This bit is sticky.
#define LINK_STATUS2_RTM1_PRESENCE_DET_OFFSET      6
#define LINK_STATUS2_RTM1_PRESENCE_DET_MASK        0x40

// Bitfield Description : When Two Retimers Presence Detect Supported bit is set, indicates if the Port detected two Retimers in the most recent link negotiation. This bit is sticky.
#define LINK_STATUS2_RTM2_PRESENCE_DET_OFFSET      7
#define LINK_STATUS2_RTM2_PRESENCE_DET_MASK        0x80

// Bitfield Description : Indicates the state of the Port's crosslink negotiation (if supported).
#define LINK_STATUS2_CROSSLINK_RESOLUTION_OFFSET      8
#define LINK_STATUS2_CROSSLINK_RESOLUTION_MASK        0x300

// Bitfield Description : 
#define LINK_STATUS2_Reserved_11_10_OFFSET      10
#define LINK_STATUS2_Reserved_11_10_MASK        0xc00

// Bitfield Description : (Downstream Ports only) When DRS Supported bit is set, this field indicates the presence and DRS state of the downstream component (if any).
// For Upstream Ports, this bit is Reserved.
#define LINK_STATUS2_DOWNSTREAM_COMPONENT_PRESENCE_OFFSET      12
#define LINK_STATUS2_DOWNSTREAM_COMPONENT_PRESENCE_MASK        0x7000

// Bitfield Description : (Downstream Ports only) When DRS Supported bit is set and When the optionally supported SFI DRS Mask bit is clear, indicates if the Port has received a DRS Message.
// For Upstream Ports, this bit is Reserved.
#define LINK_STATUS2_DRS_MESSAGE_RECEIVED_OFFSET      15
#define LINK_STATUS2_DRS_MESSAGE_RECEIVED_MASK        0x8000

typedef union {
  struct {
    UINT16                            CUR_DEEMPHASIS_LEVEL:1;
    UINT16                            EQUALIZATION_COMPLETE_8GT:1;
    UINT16                            EQUALIZATION_PHASE1_SUCCESS_8GT:1;
    UINT16                            EQUALIZATION_PHASE2_SUCCESS_8GT:1;
    UINT16                            EQUALIZATION_PHASE3_SUCCESS_8GT:1;
    UINT16                            LINK_EQUALIZATION_REQUEST_8GT:1;
    UINT16                            RTM1_PRESENCE_DET:1;
    UINT16                            RTM2_PRESENCE_DET:1;
    UINT16                            CROSSLINK_RESOLUTION:2;
    UINT16                            Reserved_11_10:2;
    UINT16                            DOWNSTREAM_COMPONENT_PRESENCE:3;
    UINT16                            DRS_MESSAGE_RECEIVED:1;
  } Field;
  UINT16 Value;
} LINK_STATUS2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_STATUS2_ADDRESS    0x1110008aUL

#define SMN_FUNC0_PCIE1_LINK_STATUS2_ADDRESS    0x1120008aUL

#define SMN_FUNC0_PCIE2_LINK_STATUS2_ADDRESS    0x1130008aUL

#define SMN_FUNC0_PCIE3_LINK_STATUS2_ADDRESS    0x1140008aUL

#define SMN_FUNC1_PCIE0_LINK_STATUS2_ADDRESS    0x1110108aUL

#define SMN_FUNC1_PCIE1_LINK_STATUS2_ADDRESS    0x1120108aUL

#define SMN_FUNC2_PCIE0_LINK_STATUS2_ADDRESS    0x1110208aUL

#define SMN_FUNC2_PCIE1_LINK_STATUS2_ADDRESS    0x1120208aUL

#define SMN_FUNC3_PCIE0_LINK_STATUS2_ADDRESS    0x1110308aUL

#define SMN_FUNC3_PCIE1_LINK_STATUS2_ADDRESS    0x1120308aUL

#define SMN_FUNC4_PCIE0_LINK_STATUS2_ADDRESS    0x1110408aUL

#define SMN_FUNC4_PCIE1_LINK_STATUS2_ADDRESS    0x1120408aUL

#define SMN_FUNC5_PCIE1_LINK_STATUS2_ADDRESS    0x1120508aUL


/***********************************************************
* Register Name : LINK_STATUS_16GT
* Register Description :
* The 16.0 GT/s Status register provides information about Physical Layer 16.0 GT/s specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the 16.0 GT/s Transmitter Equalization procedure has completed. This bit is sticky.
#define LINK_STATUS_16GT_EQUALIZATION_COMPLETE_16GT_OFFSET      0
#define LINK_STATUS_16GT_EQUALIZATION_COMPLETE_16GT_MASK        0x1

// Bitfield Description : Indicates if Phase 1 of the 16.0 GT/s Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS_16GT_EQUALIZATION_PHASE1_SUCCESS_16GT_OFFSET      1
#define LINK_STATUS_16GT_EQUALIZATION_PHASE1_SUCCESS_16GT_MASK        0x2

// Bitfield Description : Indicates if Phase 2 of the 16.0 GT/s Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS_16GT_EQUALIZATION_PHASE2_SUCCESS_16GT_OFFSET      2
#define LINK_STATUS_16GT_EQUALIZATION_PHASE2_SUCCESS_16GT_MASK        0x4

// Bitfield Description : Indicates if Phase 3 of the 16.0 GT/s Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS_16GT_EQUALIZATION_PHASE3_SUCCESS_16GT_OFFSET      3
#define LINK_STATUS_16GT_EQUALIZATION_PHASE3_SUCCESS_16GT_MASK        0x8

// Bitfield Description : Indicates if hardware requests 16.0 GT/s link equalization to be performed. This bit is sticky.
#define LINK_STATUS_16GT_LINK_EQUALIZATION_REQUEST_16GT_OFFSET      4
#define LINK_STATUS_16GT_LINK_EQUALIZATION_REQUEST_16GT_MASK        0x10

// Bitfield Description : 
#define LINK_STATUS_16GT_Reserved_31_5_OFFSET      5
#define LINK_STATUS_16GT_Reserved_31_5_MASK        0xffffffe0

typedef union {
  struct {
    UINT32                            EQUALIZATION_COMPLETE_16GT:1;
    UINT32                            EQUALIZATION_PHASE1_SUCCESS_16GT:1;
    UINT32                            EQUALIZATION_PHASE2_SUCCESS_16GT:1;
    UINT32                            EQUALIZATION_PHASE3_SUCCESS_16GT:1;
    UINT32                            LINK_EQUALIZATION_REQUEST_16GT:1;
    UINT32                            Reserved_31_5:27;
  } Field;
  UINT32 Value;
} LINK_STATUS_16GT_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_STATUS_16GT_ADDRESS    0x1110041cUL

#define SMN_FUNC0_PCIE1_LINK_STATUS_16GT_ADDRESS    0x1120041cUL

#define SMN_FUNC0_PCIE2_LINK_STATUS_16GT_ADDRESS    0x1130041cUL

#define SMN_FUNC0_PCIE3_LINK_STATUS_16GT_ADDRESS    0x1140041cUL

#define SMN_FUNC1_PCIE0_LINK_STATUS_16GT_ADDRESS    0x1110141cUL

#define SMN_FUNC1_PCIE1_LINK_STATUS_16GT_ADDRESS    0x1120141cUL

#define SMN_FUNC2_PCIE0_LINK_STATUS_16GT_ADDRESS    0x1110241cUL

#define SMN_FUNC2_PCIE1_LINK_STATUS_16GT_ADDRESS    0x1120241cUL

#define SMN_FUNC3_PCIE0_LINK_STATUS_16GT_ADDRESS    0x1110341cUL

#define SMN_FUNC3_PCIE1_LINK_STATUS_16GT_ADDRESS    0x1120341cUL

#define SMN_FUNC4_PCIE0_LINK_STATUS_16GT_ADDRESS    0x1110441cUL

#define SMN_FUNC4_PCIE1_LINK_STATUS_16GT_ADDRESS    0x1120441cUL

#define SMN_FUNC5_PCIE1_LINK_STATUS_16GT_ADDRESS    0x1120541cUL


/***********************************************************
* Register Name : LINK_STATUS_32GT
* Register Description :
* The 32.0 GT/s Status register provides information about Physical Layer 32.0 GT/s specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the 32.0 GT/s Transmitter Equalization procedure has completed. This bit is sticky.
#define LINK_STATUS_32GT_EQUALIZATION_COMPLETE_32GT_OFFSET      0
#define LINK_STATUS_32GT_EQUALIZATION_COMPLETE_32GT_MASK        0x1

// Bitfield Description : Indicates if Phase 1 of the 32.0 GT/s Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS_32GT_EQUALIZATION_PHASE1_SUCCESS_32GT_OFFSET      1
#define LINK_STATUS_32GT_EQUALIZATION_PHASE1_SUCCESS_32GT_MASK        0x2

// Bitfield Description : Indicates if Phase 2 of the 32.0 GT/s Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS_32GT_EQUALIZATION_PHASE2_SUCCESS_32GT_OFFSET      2
#define LINK_STATUS_32GT_EQUALIZATION_PHASE2_SUCCESS_32GT_MASK        0x4

// Bitfield Description : Indicates if Phase 3 of the 32.0 GT/s Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS_32GT_EQUALIZATION_PHASE3_SUCCESS_32GT_OFFSET      3
#define LINK_STATUS_32GT_EQUALIZATION_PHASE3_SUCCESS_32GT_MASK        0x8

// Bitfield Description : Indicates if hardware requests 32.0 GT/s link equalization to be performed. This bit is sticky.
#define LINK_STATUS_32GT_LINK_EQUALIZATION_REQUEST_32GT_OFFSET      4
#define LINK_STATUS_32GT_LINK_EQUALIZATION_REQUEST_32GT_MASK        0x10

// Bitfield Description : Indicates if a Modified TS1 or Modified TS2 was received.
#define LINK_STATUS_32GT_MODIFIED_TS_RECEIVED_OFFSET      5
#define LINK_STATUS_32GT_MODIFIED_TS_RECEIVED_MASK        0x20

// Bitfield Description : Contains the Enhanced Link Behavior Control bits from the most recent TS1 or TS2 received in the Polling or Configuration states.
#define LINK_STATUS_32GT_RECEIVED_ENHANCED_LINK_BEHAVIOR_CNTL_OFFSET      6
#define LINK_STATUS_32GT_RECEIVED_ENHANCED_LINK_BEHAVIOR_CNTL_MASK        0xc0

// Bitfield Description : Indicates whether the receiver asked this transmitter to enable Precoding.
#define LINK_STATUS_32GT_TRANSMITTER_PRECODING_ON_OFFSET      8
#define LINK_STATUS_32GT_TRANSMITTER_PRECODING_ON_MASK        0x100

// Bitfield Description : Indicates if the Port will request the transmitter to use Precoding by setting the Precoding Request bit in TS1s/TS2s transmitted prior to entry to Recovery.Speed.
#define LINK_STATUS_32GT_TRANSMITTER_PRECODE_REQUEST_OFFSET      9
#define LINK_STATUS_32GT_TRANSMITTER_PRECODE_REQUEST_MASK        0x200

// Bitfield Description : Indicates if a Modified TS1 or Modified TS2 was received, containing the No Equalization Needed bit set. Also indicates if a non-modified TS1 or non-modified TS2 was received, containing the No Equalization Needed encoding.
#define LINK_STATUS_32GT_NO_EQ_NEEDED_RECEIVED_OFFSET      10
#define LINK_STATUS_32GT_NO_EQ_NEEDED_RECEIVED_MASK        0x400

// Bitfield Description : 
#define LINK_STATUS_32GT_Reserved_31_11_OFFSET      11
#define LINK_STATUS_32GT_Reserved_31_11_MASK        0xfffff800

typedef union {
  struct {
    UINT32                            EQUALIZATION_COMPLETE_32GT:1;
    UINT32                            EQUALIZATION_PHASE1_SUCCESS_32GT:1;
    UINT32                            EQUALIZATION_PHASE2_SUCCESS_32GT:1;
    UINT32                            EQUALIZATION_PHASE3_SUCCESS_32GT:1;
    UINT32                            LINK_EQUALIZATION_REQUEST_32GT:1;
    UINT32                            MODIFIED_TS_RECEIVED:1;
    UINT32                            RECEIVED_ENHANCED_LINK_BEHAVIOR_CNTL:2;
    UINT32                            TRANSMITTER_PRECODING_ON:1;
    UINT32                            TRANSMITTER_PRECODE_REQUEST:1;
    UINT32                            NO_EQ_NEEDED_RECEIVED:1;
    UINT32                            Reserved_31_11:21;
  } Field;
  UINT32 Value;
} LINK_STATUS_32GT_STRUCT;

#define SMN_FUNC0_PCIE0_LINK_STATUS_32GT_ADDRESS    0x1110050cUL

#define SMN_FUNC0_PCIE1_LINK_STATUS_32GT_ADDRESS    0x1120050cUL

#define SMN_FUNC0_PCIE2_LINK_STATUS_32GT_ADDRESS    0x1130050cUL

#define SMN_FUNC0_PCIE3_LINK_STATUS_32GT_ADDRESS    0x1140050cUL

#define SMN_FUNC1_PCIE0_LINK_STATUS_32GT_ADDRESS    0x1110150cUL

#define SMN_FUNC1_PCIE1_LINK_STATUS_32GT_ADDRESS    0x1120150cUL

#define SMN_FUNC2_PCIE0_LINK_STATUS_32GT_ADDRESS    0x1110250cUL

#define SMN_FUNC2_PCIE1_LINK_STATUS_32GT_ADDRESS    0x1120250cUL

#define SMN_FUNC3_PCIE0_LINK_STATUS_32GT_ADDRESS    0x1110350cUL

#define SMN_FUNC3_PCIE1_LINK_STATUS_32GT_ADDRESS    0x1120350cUL

#define SMN_FUNC4_PCIE0_LINK_STATUS_32GT_ADDRESS    0x1110450cUL

#define SMN_FUNC4_PCIE1_LINK_STATUS_32GT_ADDRESS    0x1120450cUL

#define SMN_FUNC5_PCIE1_LINK_STATUS_32GT_ADDRESS    0x1120550cUL


/***********************************************************
* Register Name : LOCAL_PARITY_MISMATCH_STATUS_16GT
* Register Description :
* 16.0 GT/s Local Data Parity Mismatch Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the corresponding lane detected a data parity mismatch from the local receiver. This field is a bit vector that covers all supported lanes. Bit 0 corresponds to the default lane 0, and bit ordering is invariant to link width and lane reversal negotiation. This field is sticky.
#define LOCAL_PARITY_MISMATCH_STATUS_16GT_LOCAL_PARITY_MISMATCH_STATUS_BITS_OFFSET      0
#define LOCAL_PARITY_MISMATCH_STATUS_16GT_LOCAL_PARITY_MISMATCH_STATUS_BITS_MASK        0xffff

// Bitfield Description : 
#define LOCAL_PARITY_MISMATCH_STATUS_16GT_Reserved_31_16_OFFSET      16
#define LOCAL_PARITY_MISMATCH_STATUS_16GT_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            LOCAL_PARITY_MISMATCH_STATUS_BITS:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} LOCAL_PARITY_MISMATCH_STATUS_16GT_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11100420UL

#define SMN_FUNC0_PCIE1_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11200420UL

#define SMN_FUNC0_PCIE2_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11300420UL

#define SMN_FUNC0_PCIE3_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11400420UL

#define SMN_FUNC1_PCIE0_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11101420UL

#define SMN_FUNC1_PCIE1_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11201420UL

#define SMN_FUNC2_PCIE0_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11102420UL

#define SMN_FUNC2_PCIE1_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11202420UL

#define SMN_FUNC3_PCIE0_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11103420UL

#define SMN_FUNC3_PCIE1_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11203420UL

#define SMN_FUNC4_PCIE0_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11104420UL

#define SMN_FUNC4_PCIE1_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11204420UL

#define SMN_FUNC5_PCIE1_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11205420UL


/***********************************************************
* Register Name : MARGINING_LANE_CNTL
* Register Description :
* Lane 0 Margining Lane Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When Usage Model bit is clear, controls the value sent in the Receiver Number field of transmitted Control SKP ordered sets. The field is cleared when the Port goes DL_Down.
#define MARGINING_LANE_CNTL_RECEIVER_NUMBER_OFFSET      0
#define MARGINING_LANE_CNTL_RECEIVER_NUMBER_MASK        0x7

// Bitfield Description : When Usage Model bit is clear, controls the value sent in the Margin Type field of transmitted Control SKP ordered sets. The field is cleared when the Port goes DL_Down.
#define MARGINING_LANE_CNTL_MARGIN_TYPE_OFFSET      3
#define MARGINING_LANE_CNTL_MARGIN_TYPE_MASK        0x38

// Bitfield Description : Controls the value sent in the Usage Model bit of transmitted Control SKP ordered sets. The field is cleared when the Port goes DL_Down.
#define MARGINING_LANE_CNTL_USAGE_MODEL_OFFSET      6
#define MARGINING_LANE_CNTL_USAGE_MODEL_MASK        0x40

// Bitfield Description : 
#define MARGINING_LANE_CNTL_Reserved_7_7_OFFSET      7
#define MARGINING_LANE_CNTL_Reserved_7_7_MASK        0x80

// Bitfield Description : When Usage Model bit is clear, controls the value sent in the Margin Payload field of transmitted Control SKP ordered sets. The field returns to its default value when the Port goes DL_Down.
#define MARGINING_LANE_CNTL_MARGIN_PAYLOAD_OFFSET      8
#define MARGINING_LANE_CNTL_MARGIN_PAYLOAD_MASK        0xff00

typedef union {
  struct {
    UINT16                            RECEIVER_NUMBER:3;
    UINT16                            MARGIN_TYPE:3;
    UINT16                            USAGE_MODEL:1;
    UINT16                            Reserved_7_7:1;
    UINT16                            MARGIN_PAYLOAD:8;
  } Field;
  UINT16 Value;
} MARGINING_LANE_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_N0_MARGINING_LANE_CNTL_ADDRESS    0x11100448UL

#define SMN_FUNC0_PCIE0_N10_MARGINING_LANE_CNTL_ADDRESS    0x11100470UL

#define SMN_FUNC0_PCIE0_N11_MARGINING_LANE_CNTL_ADDRESS    0x11100474UL

#define SMN_FUNC0_PCIE0_N12_MARGINING_LANE_CNTL_ADDRESS    0x11100478UL

#define SMN_FUNC0_PCIE0_N13_MARGINING_LANE_CNTL_ADDRESS    0x1110047cUL

#define SMN_FUNC0_PCIE0_N14_MARGINING_LANE_CNTL_ADDRESS    0x11100480UL

#define SMN_FUNC0_PCIE0_N15_MARGINING_LANE_CNTL_ADDRESS    0x11100484UL

#define SMN_FUNC0_PCIE0_N1_MARGINING_LANE_CNTL_ADDRESS    0x1110044cUL

#define SMN_FUNC0_PCIE0_N2_MARGINING_LANE_CNTL_ADDRESS    0x11100450UL

#define SMN_FUNC0_PCIE0_N3_MARGINING_LANE_CNTL_ADDRESS    0x11100454UL

#define SMN_FUNC0_PCIE0_N4_MARGINING_LANE_CNTL_ADDRESS    0x11100458UL

#define SMN_FUNC0_PCIE0_N5_MARGINING_LANE_CNTL_ADDRESS    0x1110045cUL

#define SMN_FUNC0_PCIE0_N6_MARGINING_LANE_CNTL_ADDRESS    0x11100460UL

#define SMN_FUNC0_PCIE0_N7_MARGINING_LANE_CNTL_ADDRESS    0x11100464UL

#define SMN_FUNC0_PCIE0_N8_MARGINING_LANE_CNTL_ADDRESS    0x11100468UL

#define SMN_FUNC0_PCIE0_N9_MARGINING_LANE_CNTL_ADDRESS    0x1110046cUL

#define SMN_FUNC0_PCIE1_N0_MARGINING_LANE_CNTL_ADDRESS    0x11200448UL

#define SMN_FUNC0_PCIE1_N10_MARGINING_LANE_CNTL_ADDRESS    0x11200470UL

#define SMN_FUNC0_PCIE1_N11_MARGINING_LANE_CNTL_ADDRESS    0x11200474UL

#define SMN_FUNC0_PCIE1_N12_MARGINING_LANE_CNTL_ADDRESS    0x11200478UL

#define SMN_FUNC0_PCIE1_N13_MARGINING_LANE_CNTL_ADDRESS    0x1120047cUL

#define SMN_FUNC0_PCIE1_N14_MARGINING_LANE_CNTL_ADDRESS    0x11200480UL

#define SMN_FUNC0_PCIE1_N15_MARGINING_LANE_CNTL_ADDRESS    0x11200484UL

#define SMN_FUNC0_PCIE1_N1_MARGINING_LANE_CNTL_ADDRESS    0x1120044cUL

#define SMN_FUNC0_PCIE1_N2_MARGINING_LANE_CNTL_ADDRESS    0x11200450UL

#define SMN_FUNC0_PCIE1_N3_MARGINING_LANE_CNTL_ADDRESS    0x11200454UL

#define SMN_FUNC0_PCIE1_N4_MARGINING_LANE_CNTL_ADDRESS    0x11200458UL

#define SMN_FUNC0_PCIE1_N5_MARGINING_LANE_CNTL_ADDRESS    0x1120045cUL

#define SMN_FUNC0_PCIE1_N6_MARGINING_LANE_CNTL_ADDRESS    0x11200460UL

#define SMN_FUNC0_PCIE1_N7_MARGINING_LANE_CNTL_ADDRESS    0x11200464UL

#define SMN_FUNC0_PCIE1_N8_MARGINING_LANE_CNTL_ADDRESS    0x11200468UL

#define SMN_FUNC0_PCIE1_N9_MARGINING_LANE_CNTL_ADDRESS    0x1120046cUL

#define SMN_FUNC0_PCIE2_N0_MARGINING_LANE_CNTL_ADDRESS    0x11300448UL

#define SMN_FUNC0_PCIE2_N10_MARGINING_LANE_CNTL_ADDRESS    0x11300470UL

#define SMN_FUNC0_PCIE2_N11_MARGINING_LANE_CNTL_ADDRESS    0x11300474UL

#define SMN_FUNC0_PCIE2_N12_MARGINING_LANE_CNTL_ADDRESS    0x11300478UL

#define SMN_FUNC0_PCIE2_N13_MARGINING_LANE_CNTL_ADDRESS    0x1130047cUL

#define SMN_FUNC0_PCIE2_N14_MARGINING_LANE_CNTL_ADDRESS    0x11300480UL

#define SMN_FUNC0_PCIE2_N15_MARGINING_LANE_CNTL_ADDRESS    0x11300484UL

#define SMN_FUNC0_PCIE2_N1_MARGINING_LANE_CNTL_ADDRESS    0x1130044cUL

#define SMN_FUNC0_PCIE2_N2_MARGINING_LANE_CNTL_ADDRESS    0x11300450UL

#define SMN_FUNC0_PCIE2_N3_MARGINING_LANE_CNTL_ADDRESS    0x11300454UL

#define SMN_FUNC0_PCIE2_N4_MARGINING_LANE_CNTL_ADDRESS    0x11300458UL

#define SMN_FUNC0_PCIE2_N5_MARGINING_LANE_CNTL_ADDRESS    0x1130045cUL

#define SMN_FUNC0_PCIE2_N6_MARGINING_LANE_CNTL_ADDRESS    0x11300460UL

#define SMN_FUNC0_PCIE2_N7_MARGINING_LANE_CNTL_ADDRESS    0x11300464UL

#define SMN_FUNC0_PCIE2_N8_MARGINING_LANE_CNTL_ADDRESS    0x11300468UL

#define SMN_FUNC0_PCIE2_N9_MARGINING_LANE_CNTL_ADDRESS    0x1130046cUL

#define SMN_FUNC0_PCIE3_N0_MARGINING_LANE_CNTL_ADDRESS    0x11400448UL

#define SMN_FUNC0_PCIE3_N10_MARGINING_LANE_CNTL_ADDRESS    0x11400470UL

#define SMN_FUNC0_PCIE3_N11_MARGINING_LANE_CNTL_ADDRESS    0x11400474UL

#define SMN_FUNC0_PCIE3_N12_MARGINING_LANE_CNTL_ADDRESS    0x11400478UL

#define SMN_FUNC0_PCIE3_N13_MARGINING_LANE_CNTL_ADDRESS    0x1140047cUL

#define SMN_FUNC0_PCIE3_N14_MARGINING_LANE_CNTL_ADDRESS    0x11400480UL

#define SMN_FUNC0_PCIE3_N15_MARGINING_LANE_CNTL_ADDRESS    0x11400484UL

#define SMN_FUNC0_PCIE3_N1_MARGINING_LANE_CNTL_ADDRESS    0x1140044cUL

#define SMN_FUNC0_PCIE3_N2_MARGINING_LANE_CNTL_ADDRESS    0x11400450UL

#define SMN_FUNC0_PCIE3_N3_MARGINING_LANE_CNTL_ADDRESS    0x11400454UL

#define SMN_FUNC0_PCIE3_N4_MARGINING_LANE_CNTL_ADDRESS    0x11400458UL

#define SMN_FUNC0_PCIE3_N5_MARGINING_LANE_CNTL_ADDRESS    0x1140045cUL

#define SMN_FUNC0_PCIE3_N6_MARGINING_LANE_CNTL_ADDRESS    0x11400460UL

#define SMN_FUNC0_PCIE3_N7_MARGINING_LANE_CNTL_ADDRESS    0x11400464UL

#define SMN_FUNC0_PCIE3_N8_MARGINING_LANE_CNTL_ADDRESS    0x11400468UL

#define SMN_FUNC0_PCIE3_N9_MARGINING_LANE_CNTL_ADDRESS    0x1140046cUL

#define SMN_FUNC1_PCIE0_N0_MARGINING_LANE_CNTL_ADDRESS    0x11101448UL

#define SMN_FUNC1_PCIE0_N10_MARGINING_LANE_CNTL_ADDRESS    0x11101470UL

#define SMN_FUNC1_PCIE0_N11_MARGINING_LANE_CNTL_ADDRESS    0x11101474UL

#define SMN_FUNC1_PCIE0_N12_MARGINING_LANE_CNTL_ADDRESS    0x11101478UL

#define SMN_FUNC1_PCIE0_N13_MARGINING_LANE_CNTL_ADDRESS    0x1110147cUL

#define SMN_FUNC1_PCIE0_N14_MARGINING_LANE_CNTL_ADDRESS    0x11101480UL

#define SMN_FUNC1_PCIE0_N15_MARGINING_LANE_CNTL_ADDRESS    0x11101484UL

#define SMN_FUNC1_PCIE0_N1_MARGINING_LANE_CNTL_ADDRESS    0x1110144cUL

#define SMN_FUNC1_PCIE0_N2_MARGINING_LANE_CNTL_ADDRESS    0x11101450UL

#define SMN_FUNC1_PCIE0_N3_MARGINING_LANE_CNTL_ADDRESS    0x11101454UL

#define SMN_FUNC1_PCIE0_N4_MARGINING_LANE_CNTL_ADDRESS    0x11101458UL

#define SMN_FUNC1_PCIE0_N5_MARGINING_LANE_CNTL_ADDRESS    0x1110145cUL

#define SMN_FUNC1_PCIE0_N6_MARGINING_LANE_CNTL_ADDRESS    0x11101460UL

#define SMN_FUNC1_PCIE0_N7_MARGINING_LANE_CNTL_ADDRESS    0x11101464UL

#define SMN_FUNC1_PCIE0_N8_MARGINING_LANE_CNTL_ADDRESS    0x11101468UL

#define SMN_FUNC1_PCIE0_N9_MARGINING_LANE_CNTL_ADDRESS    0x1110146cUL

#define SMN_FUNC1_PCIE1_N0_MARGINING_LANE_CNTL_ADDRESS    0x11201448UL

#define SMN_FUNC1_PCIE1_N10_MARGINING_LANE_CNTL_ADDRESS    0x11201470UL

#define SMN_FUNC1_PCIE1_N11_MARGINING_LANE_CNTL_ADDRESS    0x11201474UL

#define SMN_FUNC1_PCIE1_N12_MARGINING_LANE_CNTL_ADDRESS    0x11201478UL

#define SMN_FUNC1_PCIE1_N13_MARGINING_LANE_CNTL_ADDRESS    0x1120147cUL

#define SMN_FUNC1_PCIE1_N14_MARGINING_LANE_CNTL_ADDRESS    0x11201480UL

#define SMN_FUNC1_PCIE1_N15_MARGINING_LANE_CNTL_ADDRESS    0x11201484UL

#define SMN_FUNC1_PCIE1_N1_MARGINING_LANE_CNTL_ADDRESS    0x1120144cUL

#define SMN_FUNC1_PCIE1_N2_MARGINING_LANE_CNTL_ADDRESS    0x11201450UL

#define SMN_FUNC1_PCIE1_N3_MARGINING_LANE_CNTL_ADDRESS    0x11201454UL

#define SMN_FUNC1_PCIE1_N4_MARGINING_LANE_CNTL_ADDRESS    0x11201458UL

#define SMN_FUNC1_PCIE1_N5_MARGINING_LANE_CNTL_ADDRESS    0x1120145cUL

#define SMN_FUNC1_PCIE1_N6_MARGINING_LANE_CNTL_ADDRESS    0x11201460UL

#define SMN_FUNC1_PCIE1_N7_MARGINING_LANE_CNTL_ADDRESS    0x11201464UL

#define SMN_FUNC1_PCIE1_N8_MARGINING_LANE_CNTL_ADDRESS    0x11201468UL

#define SMN_FUNC1_PCIE1_N9_MARGINING_LANE_CNTL_ADDRESS    0x1120146cUL

#define SMN_FUNC2_PCIE0_N0_MARGINING_LANE_CNTL_ADDRESS    0x11102448UL

#define SMN_FUNC2_PCIE0_N10_MARGINING_LANE_CNTL_ADDRESS    0x11102470UL

#define SMN_FUNC2_PCIE0_N11_MARGINING_LANE_CNTL_ADDRESS    0x11102474UL

#define SMN_FUNC2_PCIE0_N12_MARGINING_LANE_CNTL_ADDRESS    0x11102478UL

#define SMN_FUNC2_PCIE0_N13_MARGINING_LANE_CNTL_ADDRESS    0x1110247cUL

#define SMN_FUNC2_PCIE0_N14_MARGINING_LANE_CNTL_ADDRESS    0x11102480UL

#define SMN_FUNC2_PCIE0_N15_MARGINING_LANE_CNTL_ADDRESS    0x11102484UL

#define SMN_FUNC2_PCIE0_N1_MARGINING_LANE_CNTL_ADDRESS    0x1110244cUL

#define SMN_FUNC2_PCIE0_N2_MARGINING_LANE_CNTL_ADDRESS    0x11102450UL

#define SMN_FUNC2_PCIE0_N3_MARGINING_LANE_CNTL_ADDRESS    0x11102454UL

#define SMN_FUNC2_PCIE0_N4_MARGINING_LANE_CNTL_ADDRESS    0x11102458UL

#define SMN_FUNC2_PCIE0_N5_MARGINING_LANE_CNTL_ADDRESS    0x1110245cUL

#define SMN_FUNC2_PCIE0_N6_MARGINING_LANE_CNTL_ADDRESS    0x11102460UL

#define SMN_FUNC2_PCIE0_N7_MARGINING_LANE_CNTL_ADDRESS    0x11102464UL

#define SMN_FUNC2_PCIE0_N8_MARGINING_LANE_CNTL_ADDRESS    0x11102468UL

#define SMN_FUNC2_PCIE0_N9_MARGINING_LANE_CNTL_ADDRESS    0x1110246cUL

#define SMN_FUNC2_PCIE1_N0_MARGINING_LANE_CNTL_ADDRESS    0x11202448UL

#define SMN_FUNC2_PCIE1_N10_MARGINING_LANE_CNTL_ADDRESS    0x11202470UL

#define SMN_FUNC2_PCIE1_N11_MARGINING_LANE_CNTL_ADDRESS    0x11202474UL

#define SMN_FUNC2_PCIE1_N12_MARGINING_LANE_CNTL_ADDRESS    0x11202478UL

#define SMN_FUNC2_PCIE1_N13_MARGINING_LANE_CNTL_ADDRESS    0x1120247cUL

#define SMN_FUNC2_PCIE1_N14_MARGINING_LANE_CNTL_ADDRESS    0x11202480UL

#define SMN_FUNC2_PCIE1_N15_MARGINING_LANE_CNTL_ADDRESS    0x11202484UL

#define SMN_FUNC2_PCIE1_N1_MARGINING_LANE_CNTL_ADDRESS    0x1120244cUL

#define SMN_FUNC2_PCIE1_N2_MARGINING_LANE_CNTL_ADDRESS    0x11202450UL

#define SMN_FUNC2_PCIE1_N3_MARGINING_LANE_CNTL_ADDRESS    0x11202454UL

#define SMN_FUNC2_PCIE1_N4_MARGINING_LANE_CNTL_ADDRESS    0x11202458UL

#define SMN_FUNC2_PCIE1_N5_MARGINING_LANE_CNTL_ADDRESS    0x1120245cUL

#define SMN_FUNC2_PCIE1_N6_MARGINING_LANE_CNTL_ADDRESS    0x11202460UL

#define SMN_FUNC2_PCIE1_N7_MARGINING_LANE_CNTL_ADDRESS    0x11202464UL

#define SMN_FUNC2_PCIE1_N8_MARGINING_LANE_CNTL_ADDRESS    0x11202468UL

#define SMN_FUNC2_PCIE1_N9_MARGINING_LANE_CNTL_ADDRESS    0x1120246cUL

#define SMN_FUNC3_PCIE0_N0_MARGINING_LANE_CNTL_ADDRESS    0x11103448UL

#define SMN_FUNC3_PCIE0_N10_MARGINING_LANE_CNTL_ADDRESS    0x11103470UL

#define SMN_FUNC3_PCIE0_N11_MARGINING_LANE_CNTL_ADDRESS    0x11103474UL

#define SMN_FUNC3_PCIE0_N12_MARGINING_LANE_CNTL_ADDRESS    0x11103478UL

#define SMN_FUNC3_PCIE0_N13_MARGINING_LANE_CNTL_ADDRESS    0x1110347cUL

#define SMN_FUNC3_PCIE0_N14_MARGINING_LANE_CNTL_ADDRESS    0x11103480UL

#define SMN_FUNC3_PCIE0_N15_MARGINING_LANE_CNTL_ADDRESS    0x11103484UL

#define SMN_FUNC3_PCIE0_N1_MARGINING_LANE_CNTL_ADDRESS    0x1110344cUL

#define SMN_FUNC3_PCIE0_N2_MARGINING_LANE_CNTL_ADDRESS    0x11103450UL

#define SMN_FUNC3_PCIE0_N3_MARGINING_LANE_CNTL_ADDRESS    0x11103454UL

#define SMN_FUNC3_PCIE0_N4_MARGINING_LANE_CNTL_ADDRESS    0x11103458UL

#define SMN_FUNC3_PCIE0_N5_MARGINING_LANE_CNTL_ADDRESS    0x1110345cUL

#define SMN_FUNC3_PCIE0_N6_MARGINING_LANE_CNTL_ADDRESS    0x11103460UL

#define SMN_FUNC3_PCIE0_N7_MARGINING_LANE_CNTL_ADDRESS    0x11103464UL

#define SMN_FUNC3_PCIE0_N8_MARGINING_LANE_CNTL_ADDRESS    0x11103468UL

#define SMN_FUNC3_PCIE0_N9_MARGINING_LANE_CNTL_ADDRESS    0x1110346cUL

#define SMN_FUNC3_PCIE1_N0_MARGINING_LANE_CNTL_ADDRESS    0x11203448UL

#define SMN_FUNC3_PCIE1_N10_MARGINING_LANE_CNTL_ADDRESS    0x11203470UL

#define SMN_FUNC3_PCIE1_N11_MARGINING_LANE_CNTL_ADDRESS    0x11203474UL

#define SMN_FUNC3_PCIE1_N12_MARGINING_LANE_CNTL_ADDRESS    0x11203478UL

#define SMN_FUNC3_PCIE1_N13_MARGINING_LANE_CNTL_ADDRESS    0x1120347cUL

#define SMN_FUNC3_PCIE1_N14_MARGINING_LANE_CNTL_ADDRESS    0x11203480UL

#define SMN_FUNC3_PCIE1_N15_MARGINING_LANE_CNTL_ADDRESS    0x11203484UL

#define SMN_FUNC3_PCIE1_N1_MARGINING_LANE_CNTL_ADDRESS    0x1120344cUL

#define SMN_FUNC3_PCIE1_N2_MARGINING_LANE_CNTL_ADDRESS    0x11203450UL

#define SMN_FUNC3_PCIE1_N3_MARGINING_LANE_CNTL_ADDRESS    0x11203454UL

#define SMN_FUNC3_PCIE1_N4_MARGINING_LANE_CNTL_ADDRESS    0x11203458UL

#define SMN_FUNC3_PCIE1_N5_MARGINING_LANE_CNTL_ADDRESS    0x1120345cUL

#define SMN_FUNC3_PCIE1_N6_MARGINING_LANE_CNTL_ADDRESS    0x11203460UL

#define SMN_FUNC3_PCIE1_N7_MARGINING_LANE_CNTL_ADDRESS    0x11203464UL

#define SMN_FUNC3_PCIE1_N8_MARGINING_LANE_CNTL_ADDRESS    0x11203468UL

#define SMN_FUNC3_PCIE1_N9_MARGINING_LANE_CNTL_ADDRESS    0x1120346cUL

#define SMN_FUNC4_PCIE0_N0_MARGINING_LANE_CNTL_ADDRESS    0x11104448UL

#define SMN_FUNC4_PCIE0_N10_MARGINING_LANE_CNTL_ADDRESS    0x11104470UL

#define SMN_FUNC4_PCIE0_N11_MARGINING_LANE_CNTL_ADDRESS    0x11104474UL

#define SMN_FUNC4_PCIE0_N12_MARGINING_LANE_CNTL_ADDRESS    0x11104478UL

#define SMN_FUNC4_PCIE0_N13_MARGINING_LANE_CNTL_ADDRESS    0x1110447cUL

#define SMN_FUNC4_PCIE0_N14_MARGINING_LANE_CNTL_ADDRESS    0x11104480UL

#define SMN_FUNC4_PCIE0_N15_MARGINING_LANE_CNTL_ADDRESS    0x11104484UL

#define SMN_FUNC4_PCIE0_N1_MARGINING_LANE_CNTL_ADDRESS    0x1110444cUL

#define SMN_FUNC4_PCIE0_N2_MARGINING_LANE_CNTL_ADDRESS    0x11104450UL

#define SMN_FUNC4_PCIE0_N3_MARGINING_LANE_CNTL_ADDRESS    0x11104454UL

#define SMN_FUNC4_PCIE0_N4_MARGINING_LANE_CNTL_ADDRESS    0x11104458UL

#define SMN_FUNC4_PCIE0_N5_MARGINING_LANE_CNTL_ADDRESS    0x1110445cUL

#define SMN_FUNC4_PCIE0_N6_MARGINING_LANE_CNTL_ADDRESS    0x11104460UL

#define SMN_FUNC4_PCIE0_N7_MARGINING_LANE_CNTL_ADDRESS    0x11104464UL

#define SMN_FUNC4_PCIE0_N8_MARGINING_LANE_CNTL_ADDRESS    0x11104468UL

#define SMN_FUNC4_PCIE0_N9_MARGINING_LANE_CNTL_ADDRESS    0x1110446cUL

#define SMN_FUNC4_PCIE1_N0_MARGINING_LANE_CNTL_ADDRESS    0x11204448UL

#define SMN_FUNC4_PCIE1_N10_MARGINING_LANE_CNTL_ADDRESS    0x11204470UL

#define SMN_FUNC4_PCIE1_N11_MARGINING_LANE_CNTL_ADDRESS    0x11204474UL

#define SMN_FUNC4_PCIE1_N12_MARGINING_LANE_CNTL_ADDRESS    0x11204478UL

#define SMN_FUNC4_PCIE1_N13_MARGINING_LANE_CNTL_ADDRESS    0x1120447cUL

#define SMN_FUNC4_PCIE1_N14_MARGINING_LANE_CNTL_ADDRESS    0x11204480UL

#define SMN_FUNC4_PCIE1_N15_MARGINING_LANE_CNTL_ADDRESS    0x11204484UL

#define SMN_FUNC4_PCIE1_N1_MARGINING_LANE_CNTL_ADDRESS    0x1120444cUL

#define SMN_FUNC4_PCIE1_N2_MARGINING_LANE_CNTL_ADDRESS    0x11204450UL

#define SMN_FUNC4_PCIE1_N3_MARGINING_LANE_CNTL_ADDRESS    0x11204454UL

#define SMN_FUNC4_PCIE1_N4_MARGINING_LANE_CNTL_ADDRESS    0x11204458UL

#define SMN_FUNC4_PCIE1_N5_MARGINING_LANE_CNTL_ADDRESS    0x1120445cUL

#define SMN_FUNC4_PCIE1_N6_MARGINING_LANE_CNTL_ADDRESS    0x11204460UL

#define SMN_FUNC4_PCIE1_N7_MARGINING_LANE_CNTL_ADDRESS    0x11204464UL

#define SMN_FUNC4_PCIE1_N8_MARGINING_LANE_CNTL_ADDRESS    0x11204468UL

#define SMN_FUNC4_PCIE1_N9_MARGINING_LANE_CNTL_ADDRESS    0x1120446cUL

#define SMN_FUNC5_PCIE1_N0_MARGINING_LANE_CNTL_ADDRESS    0x11205448UL

#define SMN_FUNC5_PCIE1_N10_MARGINING_LANE_CNTL_ADDRESS    0x11205470UL

#define SMN_FUNC5_PCIE1_N11_MARGINING_LANE_CNTL_ADDRESS    0x11205474UL

#define SMN_FUNC5_PCIE1_N12_MARGINING_LANE_CNTL_ADDRESS    0x11205478UL

#define SMN_FUNC5_PCIE1_N13_MARGINING_LANE_CNTL_ADDRESS    0x1120547cUL

#define SMN_FUNC5_PCIE1_N14_MARGINING_LANE_CNTL_ADDRESS    0x11205480UL

#define SMN_FUNC5_PCIE1_N15_MARGINING_LANE_CNTL_ADDRESS    0x11205484UL

#define SMN_FUNC5_PCIE1_N1_MARGINING_LANE_CNTL_ADDRESS    0x1120544cUL

#define SMN_FUNC5_PCIE1_N2_MARGINING_LANE_CNTL_ADDRESS    0x11205450UL

#define SMN_FUNC5_PCIE1_N3_MARGINING_LANE_CNTL_ADDRESS    0x11205454UL

#define SMN_FUNC5_PCIE1_N4_MARGINING_LANE_CNTL_ADDRESS    0x11205458UL

#define SMN_FUNC5_PCIE1_N5_MARGINING_LANE_CNTL_ADDRESS    0x1120545cUL

#define SMN_FUNC5_PCIE1_N6_MARGINING_LANE_CNTL_ADDRESS    0x11205460UL

#define SMN_FUNC5_PCIE1_N7_MARGINING_LANE_CNTL_ADDRESS    0x11205464UL

#define SMN_FUNC5_PCIE1_N8_MARGINING_LANE_CNTL_ADDRESS    0x11205468UL

#define SMN_FUNC5_PCIE1_N9_MARGINING_LANE_CNTL_ADDRESS    0x1120546cUL


/***********************************************************
* Register Name : MARGINING_LANE_STATUS
* Register Description :
* Lane 0 Margining Lane Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Captures the value from the Receiver Number field of received Control SKP ordered sets that have the Usage Model bit clear. The field is cleared when the Port goes DL_Down.
#define MARGINING_LANE_STATUS_RECEIVER_NUMBER_STATUS_OFFSET      0
#define MARGINING_LANE_STATUS_RECEIVER_NUMBER_STATUS_MASK        0x7

// Bitfield Description : Captures the value from the Margin Type field of received Control SKP ordered sets that have the Usage Model bit clear. The field is cleared when the Port goes DL_Down.
#define MARGINING_LANE_STATUS_MARGIN_TYPE_STATUS_OFFSET      3
#define MARGINING_LANE_STATUS_MARGIN_TYPE_STATUS_MASK        0x38

// Bitfield Description : Captures the value from the Usage Model bit of received Control SKP ordered sets. The field is cleared when the Port goes DL_Down.
#define MARGINING_LANE_STATUS_USAGE_MODEL_STATUS_OFFSET      6
#define MARGINING_LANE_STATUS_USAGE_MODEL_STATUS_MASK        0x40

// Bitfield Description : 
#define MARGINING_LANE_STATUS_Reserved_7_7_OFFSET      7
#define MARGINING_LANE_STATUS_Reserved_7_7_MASK        0x80

// Bitfield Description : Captures the value from the Margin Payload field of received Control SKP ordered sets that have the Usage Model bit clear. The field is cleared when the Port goes DL_Down.
#define MARGINING_LANE_STATUS_MARGIN_PAYLOAD_STATUS_OFFSET      8
#define MARGINING_LANE_STATUS_MARGIN_PAYLOAD_STATUS_MASK        0xff00

typedef union {
  struct {
    UINT16                            RECEIVER_NUMBER_STATUS:3;
    UINT16                            MARGIN_TYPE_STATUS:3;
    UINT16                            USAGE_MODEL_STATUS:1;
    UINT16                            Reserved_7_7:1;
    UINT16                            MARGIN_PAYLOAD_STATUS:8;
  } Field;
  UINT16 Value;
} MARGINING_LANE_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_N0_MARGINING_LANE_STATUS_ADDRESS    0x1110044aUL

#define SMN_FUNC0_PCIE0_N10_MARGINING_LANE_STATUS_ADDRESS    0x11100472UL

#define SMN_FUNC0_PCIE0_N11_MARGINING_LANE_STATUS_ADDRESS    0x11100476UL

#define SMN_FUNC0_PCIE0_N12_MARGINING_LANE_STATUS_ADDRESS    0x1110047aUL

#define SMN_FUNC0_PCIE0_N13_MARGINING_LANE_STATUS_ADDRESS    0x1110047eUL

#define SMN_FUNC0_PCIE0_N14_MARGINING_LANE_STATUS_ADDRESS    0x11100482UL

#define SMN_FUNC0_PCIE0_N15_MARGINING_LANE_STATUS_ADDRESS    0x11100486UL

#define SMN_FUNC0_PCIE0_N1_MARGINING_LANE_STATUS_ADDRESS    0x1110044eUL

#define SMN_FUNC0_PCIE0_N2_MARGINING_LANE_STATUS_ADDRESS    0x11100452UL

#define SMN_FUNC0_PCIE0_N3_MARGINING_LANE_STATUS_ADDRESS    0x11100456UL

#define SMN_FUNC0_PCIE0_N4_MARGINING_LANE_STATUS_ADDRESS    0x1110045aUL

#define SMN_FUNC0_PCIE0_N5_MARGINING_LANE_STATUS_ADDRESS    0x1110045eUL

#define SMN_FUNC0_PCIE0_N6_MARGINING_LANE_STATUS_ADDRESS    0x11100462UL

#define SMN_FUNC0_PCIE0_N7_MARGINING_LANE_STATUS_ADDRESS    0x11100466UL

#define SMN_FUNC0_PCIE0_N8_MARGINING_LANE_STATUS_ADDRESS    0x1110046aUL

#define SMN_FUNC0_PCIE0_N9_MARGINING_LANE_STATUS_ADDRESS    0x1110046eUL

#define SMN_FUNC0_PCIE1_N0_MARGINING_LANE_STATUS_ADDRESS    0x1120044aUL

#define SMN_FUNC0_PCIE1_N10_MARGINING_LANE_STATUS_ADDRESS    0x11200472UL

#define SMN_FUNC0_PCIE1_N11_MARGINING_LANE_STATUS_ADDRESS    0x11200476UL

#define SMN_FUNC0_PCIE1_N12_MARGINING_LANE_STATUS_ADDRESS    0x1120047aUL

#define SMN_FUNC0_PCIE1_N13_MARGINING_LANE_STATUS_ADDRESS    0x1120047eUL

#define SMN_FUNC0_PCIE1_N14_MARGINING_LANE_STATUS_ADDRESS    0x11200482UL

#define SMN_FUNC0_PCIE1_N15_MARGINING_LANE_STATUS_ADDRESS    0x11200486UL

#define SMN_FUNC0_PCIE1_N1_MARGINING_LANE_STATUS_ADDRESS    0x1120044eUL

#define SMN_FUNC0_PCIE1_N2_MARGINING_LANE_STATUS_ADDRESS    0x11200452UL

#define SMN_FUNC0_PCIE1_N3_MARGINING_LANE_STATUS_ADDRESS    0x11200456UL

#define SMN_FUNC0_PCIE1_N4_MARGINING_LANE_STATUS_ADDRESS    0x1120045aUL

#define SMN_FUNC0_PCIE1_N5_MARGINING_LANE_STATUS_ADDRESS    0x1120045eUL

#define SMN_FUNC0_PCIE1_N6_MARGINING_LANE_STATUS_ADDRESS    0x11200462UL

#define SMN_FUNC0_PCIE1_N7_MARGINING_LANE_STATUS_ADDRESS    0x11200466UL

#define SMN_FUNC0_PCIE1_N8_MARGINING_LANE_STATUS_ADDRESS    0x1120046aUL

#define SMN_FUNC0_PCIE1_N9_MARGINING_LANE_STATUS_ADDRESS    0x1120046eUL

#define SMN_FUNC0_PCIE2_N0_MARGINING_LANE_STATUS_ADDRESS    0x1130044aUL

#define SMN_FUNC0_PCIE2_N10_MARGINING_LANE_STATUS_ADDRESS    0x11300472UL

#define SMN_FUNC0_PCIE2_N11_MARGINING_LANE_STATUS_ADDRESS    0x11300476UL

#define SMN_FUNC0_PCIE2_N12_MARGINING_LANE_STATUS_ADDRESS    0x1130047aUL

#define SMN_FUNC0_PCIE2_N13_MARGINING_LANE_STATUS_ADDRESS    0x1130047eUL

#define SMN_FUNC0_PCIE2_N14_MARGINING_LANE_STATUS_ADDRESS    0x11300482UL

#define SMN_FUNC0_PCIE2_N15_MARGINING_LANE_STATUS_ADDRESS    0x11300486UL

#define SMN_FUNC0_PCIE2_N1_MARGINING_LANE_STATUS_ADDRESS    0x1130044eUL

#define SMN_FUNC0_PCIE2_N2_MARGINING_LANE_STATUS_ADDRESS    0x11300452UL

#define SMN_FUNC0_PCIE2_N3_MARGINING_LANE_STATUS_ADDRESS    0x11300456UL

#define SMN_FUNC0_PCIE2_N4_MARGINING_LANE_STATUS_ADDRESS    0x1130045aUL

#define SMN_FUNC0_PCIE2_N5_MARGINING_LANE_STATUS_ADDRESS    0x1130045eUL

#define SMN_FUNC0_PCIE2_N6_MARGINING_LANE_STATUS_ADDRESS    0x11300462UL

#define SMN_FUNC0_PCIE2_N7_MARGINING_LANE_STATUS_ADDRESS    0x11300466UL

#define SMN_FUNC0_PCIE2_N8_MARGINING_LANE_STATUS_ADDRESS    0x1130046aUL

#define SMN_FUNC0_PCIE2_N9_MARGINING_LANE_STATUS_ADDRESS    0x1130046eUL

#define SMN_FUNC0_PCIE3_N0_MARGINING_LANE_STATUS_ADDRESS    0x1140044aUL

#define SMN_FUNC0_PCIE3_N10_MARGINING_LANE_STATUS_ADDRESS    0x11400472UL

#define SMN_FUNC0_PCIE3_N11_MARGINING_LANE_STATUS_ADDRESS    0x11400476UL

#define SMN_FUNC0_PCIE3_N12_MARGINING_LANE_STATUS_ADDRESS    0x1140047aUL

#define SMN_FUNC0_PCIE3_N13_MARGINING_LANE_STATUS_ADDRESS    0x1140047eUL

#define SMN_FUNC0_PCIE3_N14_MARGINING_LANE_STATUS_ADDRESS    0x11400482UL

#define SMN_FUNC0_PCIE3_N15_MARGINING_LANE_STATUS_ADDRESS    0x11400486UL

#define SMN_FUNC0_PCIE3_N1_MARGINING_LANE_STATUS_ADDRESS    0x1140044eUL

#define SMN_FUNC0_PCIE3_N2_MARGINING_LANE_STATUS_ADDRESS    0x11400452UL

#define SMN_FUNC0_PCIE3_N3_MARGINING_LANE_STATUS_ADDRESS    0x11400456UL

#define SMN_FUNC0_PCIE3_N4_MARGINING_LANE_STATUS_ADDRESS    0x1140045aUL

#define SMN_FUNC0_PCIE3_N5_MARGINING_LANE_STATUS_ADDRESS    0x1140045eUL

#define SMN_FUNC0_PCIE3_N6_MARGINING_LANE_STATUS_ADDRESS    0x11400462UL

#define SMN_FUNC0_PCIE3_N7_MARGINING_LANE_STATUS_ADDRESS    0x11400466UL

#define SMN_FUNC0_PCIE3_N8_MARGINING_LANE_STATUS_ADDRESS    0x1140046aUL

#define SMN_FUNC0_PCIE3_N9_MARGINING_LANE_STATUS_ADDRESS    0x1140046eUL

#define SMN_FUNC1_PCIE0_N0_MARGINING_LANE_STATUS_ADDRESS    0x1110144aUL

#define SMN_FUNC1_PCIE0_N10_MARGINING_LANE_STATUS_ADDRESS    0x11101472UL

#define SMN_FUNC1_PCIE0_N11_MARGINING_LANE_STATUS_ADDRESS    0x11101476UL

#define SMN_FUNC1_PCIE0_N12_MARGINING_LANE_STATUS_ADDRESS    0x1110147aUL

#define SMN_FUNC1_PCIE0_N13_MARGINING_LANE_STATUS_ADDRESS    0x1110147eUL

#define SMN_FUNC1_PCIE0_N14_MARGINING_LANE_STATUS_ADDRESS    0x11101482UL

#define SMN_FUNC1_PCIE0_N15_MARGINING_LANE_STATUS_ADDRESS    0x11101486UL

#define SMN_FUNC1_PCIE0_N1_MARGINING_LANE_STATUS_ADDRESS    0x1110144eUL

#define SMN_FUNC1_PCIE0_N2_MARGINING_LANE_STATUS_ADDRESS    0x11101452UL

#define SMN_FUNC1_PCIE0_N3_MARGINING_LANE_STATUS_ADDRESS    0x11101456UL

#define SMN_FUNC1_PCIE0_N4_MARGINING_LANE_STATUS_ADDRESS    0x1110145aUL

#define SMN_FUNC1_PCIE0_N5_MARGINING_LANE_STATUS_ADDRESS    0x1110145eUL

#define SMN_FUNC1_PCIE0_N6_MARGINING_LANE_STATUS_ADDRESS    0x11101462UL

#define SMN_FUNC1_PCIE0_N7_MARGINING_LANE_STATUS_ADDRESS    0x11101466UL

#define SMN_FUNC1_PCIE0_N8_MARGINING_LANE_STATUS_ADDRESS    0x1110146aUL

#define SMN_FUNC1_PCIE0_N9_MARGINING_LANE_STATUS_ADDRESS    0x1110146eUL

#define SMN_FUNC1_PCIE1_N0_MARGINING_LANE_STATUS_ADDRESS    0x1120144aUL

#define SMN_FUNC1_PCIE1_N10_MARGINING_LANE_STATUS_ADDRESS    0x11201472UL

#define SMN_FUNC1_PCIE1_N11_MARGINING_LANE_STATUS_ADDRESS    0x11201476UL

#define SMN_FUNC1_PCIE1_N12_MARGINING_LANE_STATUS_ADDRESS    0x1120147aUL

#define SMN_FUNC1_PCIE1_N13_MARGINING_LANE_STATUS_ADDRESS    0x1120147eUL

#define SMN_FUNC1_PCIE1_N14_MARGINING_LANE_STATUS_ADDRESS    0x11201482UL

#define SMN_FUNC1_PCIE1_N15_MARGINING_LANE_STATUS_ADDRESS    0x11201486UL

#define SMN_FUNC1_PCIE1_N1_MARGINING_LANE_STATUS_ADDRESS    0x1120144eUL

#define SMN_FUNC1_PCIE1_N2_MARGINING_LANE_STATUS_ADDRESS    0x11201452UL

#define SMN_FUNC1_PCIE1_N3_MARGINING_LANE_STATUS_ADDRESS    0x11201456UL

#define SMN_FUNC1_PCIE1_N4_MARGINING_LANE_STATUS_ADDRESS    0x1120145aUL

#define SMN_FUNC1_PCIE1_N5_MARGINING_LANE_STATUS_ADDRESS    0x1120145eUL

#define SMN_FUNC1_PCIE1_N6_MARGINING_LANE_STATUS_ADDRESS    0x11201462UL

#define SMN_FUNC1_PCIE1_N7_MARGINING_LANE_STATUS_ADDRESS    0x11201466UL

#define SMN_FUNC1_PCIE1_N8_MARGINING_LANE_STATUS_ADDRESS    0x1120146aUL

#define SMN_FUNC1_PCIE1_N9_MARGINING_LANE_STATUS_ADDRESS    0x1120146eUL

#define SMN_FUNC2_PCIE0_N0_MARGINING_LANE_STATUS_ADDRESS    0x1110244aUL

#define SMN_FUNC2_PCIE0_N10_MARGINING_LANE_STATUS_ADDRESS    0x11102472UL

#define SMN_FUNC2_PCIE0_N11_MARGINING_LANE_STATUS_ADDRESS    0x11102476UL

#define SMN_FUNC2_PCIE0_N12_MARGINING_LANE_STATUS_ADDRESS    0x1110247aUL

#define SMN_FUNC2_PCIE0_N13_MARGINING_LANE_STATUS_ADDRESS    0x1110247eUL

#define SMN_FUNC2_PCIE0_N14_MARGINING_LANE_STATUS_ADDRESS    0x11102482UL

#define SMN_FUNC2_PCIE0_N15_MARGINING_LANE_STATUS_ADDRESS    0x11102486UL

#define SMN_FUNC2_PCIE0_N1_MARGINING_LANE_STATUS_ADDRESS    0x1110244eUL

#define SMN_FUNC2_PCIE0_N2_MARGINING_LANE_STATUS_ADDRESS    0x11102452UL

#define SMN_FUNC2_PCIE0_N3_MARGINING_LANE_STATUS_ADDRESS    0x11102456UL

#define SMN_FUNC2_PCIE0_N4_MARGINING_LANE_STATUS_ADDRESS    0x1110245aUL

#define SMN_FUNC2_PCIE0_N5_MARGINING_LANE_STATUS_ADDRESS    0x1110245eUL

#define SMN_FUNC2_PCIE0_N6_MARGINING_LANE_STATUS_ADDRESS    0x11102462UL

#define SMN_FUNC2_PCIE0_N7_MARGINING_LANE_STATUS_ADDRESS    0x11102466UL

#define SMN_FUNC2_PCIE0_N8_MARGINING_LANE_STATUS_ADDRESS    0x1110246aUL

#define SMN_FUNC2_PCIE0_N9_MARGINING_LANE_STATUS_ADDRESS    0x1110246eUL

#define SMN_FUNC2_PCIE1_N0_MARGINING_LANE_STATUS_ADDRESS    0x1120244aUL

#define SMN_FUNC2_PCIE1_N10_MARGINING_LANE_STATUS_ADDRESS    0x11202472UL

#define SMN_FUNC2_PCIE1_N11_MARGINING_LANE_STATUS_ADDRESS    0x11202476UL

#define SMN_FUNC2_PCIE1_N12_MARGINING_LANE_STATUS_ADDRESS    0x1120247aUL

#define SMN_FUNC2_PCIE1_N13_MARGINING_LANE_STATUS_ADDRESS    0x1120247eUL

#define SMN_FUNC2_PCIE1_N14_MARGINING_LANE_STATUS_ADDRESS    0x11202482UL

#define SMN_FUNC2_PCIE1_N15_MARGINING_LANE_STATUS_ADDRESS    0x11202486UL

#define SMN_FUNC2_PCIE1_N1_MARGINING_LANE_STATUS_ADDRESS    0x1120244eUL

#define SMN_FUNC2_PCIE1_N2_MARGINING_LANE_STATUS_ADDRESS    0x11202452UL

#define SMN_FUNC2_PCIE1_N3_MARGINING_LANE_STATUS_ADDRESS    0x11202456UL

#define SMN_FUNC2_PCIE1_N4_MARGINING_LANE_STATUS_ADDRESS    0x1120245aUL

#define SMN_FUNC2_PCIE1_N5_MARGINING_LANE_STATUS_ADDRESS    0x1120245eUL

#define SMN_FUNC2_PCIE1_N6_MARGINING_LANE_STATUS_ADDRESS    0x11202462UL

#define SMN_FUNC2_PCIE1_N7_MARGINING_LANE_STATUS_ADDRESS    0x11202466UL

#define SMN_FUNC2_PCIE1_N8_MARGINING_LANE_STATUS_ADDRESS    0x1120246aUL

#define SMN_FUNC2_PCIE1_N9_MARGINING_LANE_STATUS_ADDRESS    0x1120246eUL

#define SMN_FUNC3_PCIE0_N0_MARGINING_LANE_STATUS_ADDRESS    0x1110344aUL

#define SMN_FUNC3_PCIE0_N10_MARGINING_LANE_STATUS_ADDRESS    0x11103472UL

#define SMN_FUNC3_PCIE0_N11_MARGINING_LANE_STATUS_ADDRESS    0x11103476UL

#define SMN_FUNC3_PCIE0_N12_MARGINING_LANE_STATUS_ADDRESS    0x1110347aUL

#define SMN_FUNC3_PCIE0_N13_MARGINING_LANE_STATUS_ADDRESS    0x1110347eUL

#define SMN_FUNC3_PCIE0_N14_MARGINING_LANE_STATUS_ADDRESS    0x11103482UL

#define SMN_FUNC3_PCIE0_N15_MARGINING_LANE_STATUS_ADDRESS    0x11103486UL

#define SMN_FUNC3_PCIE0_N1_MARGINING_LANE_STATUS_ADDRESS    0x1110344eUL

#define SMN_FUNC3_PCIE0_N2_MARGINING_LANE_STATUS_ADDRESS    0x11103452UL

#define SMN_FUNC3_PCIE0_N3_MARGINING_LANE_STATUS_ADDRESS    0x11103456UL

#define SMN_FUNC3_PCIE0_N4_MARGINING_LANE_STATUS_ADDRESS    0x1110345aUL

#define SMN_FUNC3_PCIE0_N5_MARGINING_LANE_STATUS_ADDRESS    0x1110345eUL

#define SMN_FUNC3_PCIE0_N6_MARGINING_LANE_STATUS_ADDRESS    0x11103462UL

#define SMN_FUNC3_PCIE0_N7_MARGINING_LANE_STATUS_ADDRESS    0x11103466UL

#define SMN_FUNC3_PCIE0_N8_MARGINING_LANE_STATUS_ADDRESS    0x1110346aUL

#define SMN_FUNC3_PCIE0_N9_MARGINING_LANE_STATUS_ADDRESS    0x1110346eUL

#define SMN_FUNC3_PCIE1_N0_MARGINING_LANE_STATUS_ADDRESS    0x1120344aUL

#define SMN_FUNC3_PCIE1_N10_MARGINING_LANE_STATUS_ADDRESS    0x11203472UL

#define SMN_FUNC3_PCIE1_N11_MARGINING_LANE_STATUS_ADDRESS    0x11203476UL

#define SMN_FUNC3_PCIE1_N12_MARGINING_LANE_STATUS_ADDRESS    0x1120347aUL

#define SMN_FUNC3_PCIE1_N13_MARGINING_LANE_STATUS_ADDRESS    0x1120347eUL

#define SMN_FUNC3_PCIE1_N14_MARGINING_LANE_STATUS_ADDRESS    0x11203482UL

#define SMN_FUNC3_PCIE1_N15_MARGINING_LANE_STATUS_ADDRESS    0x11203486UL

#define SMN_FUNC3_PCIE1_N1_MARGINING_LANE_STATUS_ADDRESS    0x1120344eUL

#define SMN_FUNC3_PCIE1_N2_MARGINING_LANE_STATUS_ADDRESS    0x11203452UL

#define SMN_FUNC3_PCIE1_N3_MARGINING_LANE_STATUS_ADDRESS    0x11203456UL

#define SMN_FUNC3_PCIE1_N4_MARGINING_LANE_STATUS_ADDRESS    0x1120345aUL

#define SMN_FUNC3_PCIE1_N5_MARGINING_LANE_STATUS_ADDRESS    0x1120345eUL

#define SMN_FUNC3_PCIE1_N6_MARGINING_LANE_STATUS_ADDRESS    0x11203462UL

#define SMN_FUNC3_PCIE1_N7_MARGINING_LANE_STATUS_ADDRESS    0x11203466UL

#define SMN_FUNC3_PCIE1_N8_MARGINING_LANE_STATUS_ADDRESS    0x1120346aUL

#define SMN_FUNC3_PCIE1_N9_MARGINING_LANE_STATUS_ADDRESS    0x1120346eUL

#define SMN_FUNC4_PCIE0_N0_MARGINING_LANE_STATUS_ADDRESS    0x1110444aUL

#define SMN_FUNC4_PCIE0_N10_MARGINING_LANE_STATUS_ADDRESS    0x11104472UL

#define SMN_FUNC4_PCIE0_N11_MARGINING_LANE_STATUS_ADDRESS    0x11104476UL

#define SMN_FUNC4_PCIE0_N12_MARGINING_LANE_STATUS_ADDRESS    0x1110447aUL

#define SMN_FUNC4_PCIE0_N13_MARGINING_LANE_STATUS_ADDRESS    0x1110447eUL

#define SMN_FUNC4_PCIE0_N14_MARGINING_LANE_STATUS_ADDRESS    0x11104482UL

#define SMN_FUNC4_PCIE0_N15_MARGINING_LANE_STATUS_ADDRESS    0x11104486UL

#define SMN_FUNC4_PCIE0_N1_MARGINING_LANE_STATUS_ADDRESS    0x1110444eUL

#define SMN_FUNC4_PCIE0_N2_MARGINING_LANE_STATUS_ADDRESS    0x11104452UL

#define SMN_FUNC4_PCIE0_N3_MARGINING_LANE_STATUS_ADDRESS    0x11104456UL

#define SMN_FUNC4_PCIE0_N4_MARGINING_LANE_STATUS_ADDRESS    0x1110445aUL

#define SMN_FUNC4_PCIE0_N5_MARGINING_LANE_STATUS_ADDRESS    0x1110445eUL

#define SMN_FUNC4_PCIE0_N6_MARGINING_LANE_STATUS_ADDRESS    0x11104462UL

#define SMN_FUNC4_PCIE0_N7_MARGINING_LANE_STATUS_ADDRESS    0x11104466UL

#define SMN_FUNC4_PCIE0_N8_MARGINING_LANE_STATUS_ADDRESS    0x1110446aUL

#define SMN_FUNC4_PCIE0_N9_MARGINING_LANE_STATUS_ADDRESS    0x1110446eUL

#define SMN_FUNC4_PCIE1_N0_MARGINING_LANE_STATUS_ADDRESS    0x1120444aUL

#define SMN_FUNC4_PCIE1_N10_MARGINING_LANE_STATUS_ADDRESS    0x11204472UL

#define SMN_FUNC4_PCIE1_N11_MARGINING_LANE_STATUS_ADDRESS    0x11204476UL

#define SMN_FUNC4_PCIE1_N12_MARGINING_LANE_STATUS_ADDRESS    0x1120447aUL

#define SMN_FUNC4_PCIE1_N13_MARGINING_LANE_STATUS_ADDRESS    0x1120447eUL

#define SMN_FUNC4_PCIE1_N14_MARGINING_LANE_STATUS_ADDRESS    0x11204482UL

#define SMN_FUNC4_PCIE1_N15_MARGINING_LANE_STATUS_ADDRESS    0x11204486UL

#define SMN_FUNC4_PCIE1_N1_MARGINING_LANE_STATUS_ADDRESS    0x1120444eUL

#define SMN_FUNC4_PCIE1_N2_MARGINING_LANE_STATUS_ADDRESS    0x11204452UL

#define SMN_FUNC4_PCIE1_N3_MARGINING_LANE_STATUS_ADDRESS    0x11204456UL

#define SMN_FUNC4_PCIE1_N4_MARGINING_LANE_STATUS_ADDRESS    0x1120445aUL

#define SMN_FUNC4_PCIE1_N5_MARGINING_LANE_STATUS_ADDRESS    0x1120445eUL

#define SMN_FUNC4_PCIE1_N6_MARGINING_LANE_STATUS_ADDRESS    0x11204462UL

#define SMN_FUNC4_PCIE1_N7_MARGINING_LANE_STATUS_ADDRESS    0x11204466UL

#define SMN_FUNC4_PCIE1_N8_MARGINING_LANE_STATUS_ADDRESS    0x1120446aUL

#define SMN_FUNC4_PCIE1_N9_MARGINING_LANE_STATUS_ADDRESS    0x1120446eUL

#define SMN_FUNC5_PCIE1_N0_MARGINING_LANE_STATUS_ADDRESS    0x1120544aUL

#define SMN_FUNC5_PCIE1_N10_MARGINING_LANE_STATUS_ADDRESS    0x11205472UL

#define SMN_FUNC5_PCIE1_N11_MARGINING_LANE_STATUS_ADDRESS    0x11205476UL

#define SMN_FUNC5_PCIE1_N12_MARGINING_LANE_STATUS_ADDRESS    0x1120547aUL

#define SMN_FUNC5_PCIE1_N13_MARGINING_LANE_STATUS_ADDRESS    0x1120547eUL

#define SMN_FUNC5_PCIE1_N14_MARGINING_LANE_STATUS_ADDRESS    0x11205482UL

#define SMN_FUNC5_PCIE1_N15_MARGINING_LANE_STATUS_ADDRESS    0x11205486UL

#define SMN_FUNC5_PCIE1_N1_MARGINING_LANE_STATUS_ADDRESS    0x1120544eUL

#define SMN_FUNC5_PCIE1_N2_MARGINING_LANE_STATUS_ADDRESS    0x11205452UL

#define SMN_FUNC5_PCIE1_N3_MARGINING_LANE_STATUS_ADDRESS    0x11205456UL

#define SMN_FUNC5_PCIE1_N4_MARGINING_LANE_STATUS_ADDRESS    0x1120545aUL

#define SMN_FUNC5_PCIE1_N5_MARGINING_LANE_STATUS_ADDRESS    0x1120545eUL

#define SMN_FUNC5_PCIE1_N6_MARGINING_LANE_STATUS_ADDRESS    0x11205462UL

#define SMN_FUNC5_PCIE1_N7_MARGINING_LANE_STATUS_ADDRESS    0x11205466UL

#define SMN_FUNC5_PCIE1_N8_MARGINING_LANE_STATUS_ADDRESS    0x1120546aUL

#define SMN_FUNC5_PCIE1_N9_MARGINING_LANE_STATUS_ADDRESS    0x1120546eUL


/***********************************************************
* Register Name : MARGINING_PORT_CAP
* Register Description :
* Margining Port Capabilities register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port requires hardware-specific software in order to support margining.
#define MARGINING_PORT_CAP_MARGINING_USES_SOFTWARE_OFFSET      0
#define MARGINING_PORT_CAP_MARGINING_USES_SOFTWARE_MASK        0x1

// Bitfield Description : 
#define MARGINING_PORT_CAP_Reserved_15_1_OFFSET      1
#define MARGINING_PORT_CAP_Reserved_15_1_MASK        0xfffe

typedef union {
  struct {
    UINT16                            MARGINING_USES_SOFTWARE:1;
    UINT16                            Reserved_15_1:15;
  } Field;
  UINT16 Value;
} MARGINING_PORT_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MARGINING_PORT_CAP_ADDRESS    0x11100444UL

#define SMN_FUNC0_PCIE1_MARGINING_PORT_CAP_ADDRESS    0x11200444UL

#define SMN_FUNC0_PCIE2_MARGINING_PORT_CAP_ADDRESS    0x11300444UL

#define SMN_FUNC0_PCIE3_MARGINING_PORT_CAP_ADDRESS    0x11400444UL

#define SMN_FUNC1_PCIE0_MARGINING_PORT_CAP_ADDRESS    0x11101444UL

#define SMN_FUNC1_PCIE1_MARGINING_PORT_CAP_ADDRESS    0x11201444UL

#define SMN_FUNC2_PCIE0_MARGINING_PORT_CAP_ADDRESS    0x11102444UL

#define SMN_FUNC2_PCIE1_MARGINING_PORT_CAP_ADDRESS    0x11202444UL

#define SMN_FUNC3_PCIE0_MARGINING_PORT_CAP_ADDRESS    0x11103444UL

#define SMN_FUNC3_PCIE1_MARGINING_PORT_CAP_ADDRESS    0x11203444UL

#define SMN_FUNC4_PCIE0_MARGINING_PORT_CAP_ADDRESS    0x11104444UL

#define SMN_FUNC4_PCIE1_MARGINING_PORT_CAP_ADDRESS    0x11204444UL

#define SMN_FUNC5_PCIE1_MARGINING_PORT_CAP_ADDRESS    0x11205444UL


/***********************************************************
* Register Name : MARGINING_PORT_STATUS
* Register Description :
* Margining Port Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port is ready to accept margining commands.
#define MARGINING_PORT_STATUS_MARGINING_READY_OFFSET      0
#define MARGINING_PORT_STATUS_MARGINING_READY_MASK        0x1

// Bitfield Description : When Margining Uses Software Driver bit is set, indicates if hardware-specific software initialization of the Port has been completed.
#define MARGINING_PORT_STATUS_MARGINING_SOFTWARE_READY_OFFSET      1
#define MARGINING_PORT_STATUS_MARGINING_SOFTWARE_READY_MASK        0x2

// Bitfield Description : 
#define MARGINING_PORT_STATUS_Reserved_15_2_OFFSET      2
#define MARGINING_PORT_STATUS_Reserved_15_2_MASK        0xfffc

typedef union {
  struct {
    UINT16                            MARGINING_READY:1;
    UINT16                            MARGINING_SOFTWARE_READY:1;
    UINT16                            Reserved_15_2:14;
  } Field;
  UINT16 Value;
} MARGINING_PORT_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MARGINING_PORT_STATUS_ADDRESS    0x11100446UL

#define SMN_FUNC0_PCIE1_MARGINING_PORT_STATUS_ADDRESS    0x11200446UL

#define SMN_FUNC0_PCIE2_MARGINING_PORT_STATUS_ADDRESS    0x11300446UL

#define SMN_FUNC0_PCIE3_MARGINING_PORT_STATUS_ADDRESS    0x11400446UL

#define SMN_FUNC1_PCIE0_MARGINING_PORT_STATUS_ADDRESS    0x11101446UL

#define SMN_FUNC1_PCIE1_MARGINING_PORT_STATUS_ADDRESS    0x11201446UL

#define SMN_FUNC2_PCIE0_MARGINING_PORT_STATUS_ADDRESS    0x11102446UL

#define SMN_FUNC2_PCIE1_MARGINING_PORT_STATUS_ADDRESS    0x11202446UL

#define SMN_FUNC3_PCIE0_MARGINING_PORT_STATUS_ADDRESS    0x11103446UL

#define SMN_FUNC3_PCIE1_MARGINING_PORT_STATUS_ADDRESS    0x11203446UL

#define SMN_FUNC4_PCIE0_MARGINING_PORT_STATUS_ADDRESS    0x11104446UL

#define SMN_FUNC4_PCIE1_MARGINING_PORT_STATUS_ADDRESS    0x11204446UL

#define SMN_FUNC5_PCIE1_MARGINING_PORT_STATUS_ADDRESS    0x11205446UL


/***********************************************************
* Register Name : MEM_BASE_LIMIT
* Register Description :
* Defines a non-prefetchable memory address range which is used by the bridge to determine when to forward memory transactions from one interface to the other.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Must be 0.
#define MEM_BASE_LIMIT_MEM_BASE_TYPE_OFFSET      0
#define MEM_BASE_LIMIT_MEM_BASE_TYPE_MASK        0xf

// Bitfield Description : Bits 31-20 for range starting non-prefetchable memory address.
#define MEM_BASE_LIMIT_MEM_BASE_31_20_OFFSET      4
#define MEM_BASE_LIMIT_MEM_BASE_31_20_MASK        0xfff0

// Bitfield Description : Must be 0.
#define MEM_BASE_LIMIT_MEM_LIMIT_TYPE_OFFSET      16
#define MEM_BASE_LIMIT_MEM_LIMIT_TYPE_MASK        0xf0000

// Bitfield Description : Bits 31-20 for range ending non-prefetchable memory address.
#define MEM_BASE_LIMIT_MEM_LIMIT_31_20_OFFSET      20
#define MEM_BASE_LIMIT_MEM_LIMIT_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            MEM_BASE_TYPE:4;
    UINT32                            MEM_BASE_31_20:12;
    UINT32                            MEM_LIMIT_TYPE:4;
    UINT32                            MEM_LIMIT_31_20:12;
  } Field;
  UINT32 Value;
} MEM_BASE_LIMIT_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MEM_BASE_LIMIT_ADDRESS    0x11100020UL

#define SMN_FUNC0_PCIE1_MEM_BASE_LIMIT_ADDRESS    0x11200020UL

#define SMN_FUNC0_PCIE2_MEM_BASE_LIMIT_ADDRESS    0x11300020UL

#define SMN_FUNC0_PCIE3_MEM_BASE_LIMIT_ADDRESS    0x11400020UL

#define SMN_FUNC1_PCIE0_MEM_BASE_LIMIT_ADDRESS    0x11101020UL

#define SMN_FUNC1_PCIE1_MEM_BASE_LIMIT_ADDRESS    0x11201020UL

#define SMN_FUNC2_PCIE0_MEM_BASE_LIMIT_ADDRESS    0x11102020UL

#define SMN_FUNC2_PCIE1_MEM_BASE_LIMIT_ADDRESS    0x11202020UL

#define SMN_FUNC3_PCIE0_MEM_BASE_LIMIT_ADDRESS    0x11103020UL

#define SMN_FUNC3_PCIE1_MEM_BASE_LIMIT_ADDRESS    0x11203020UL

#define SMN_FUNC4_PCIE0_MEM_BASE_LIMIT_ADDRESS    0x11104020UL

#define SMN_FUNC4_PCIE1_MEM_BASE_LIMIT_ADDRESS    0x11204020UL

#define SMN_FUNC5_PCIE1_MEM_BASE_LIMIT_ADDRESS    0x11205020UL


/***********************************************************
* Register Name : MSI_CAP_LIST
* Register Description :
* Message Signaled Interrupt Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the capability.
#define MSI_CAP_LIST_CAP_ID_OFFSET      0
#define MSI_CAP_LIST_CAP_ID_MASK        0xff

// Bitfield Description : Pointer to the next item in the capabilities list, or 00h if no other items exist.
#define MSI_CAP_LIST_NEXT_PTR_OFFSET      8
#define MSI_CAP_LIST_NEXT_PTR_MASK        0xff00

typedef union {
  struct {
    UINT16                            CAP_ID:8;
    UINT16                            NEXT_PTR:8;
  } Field;
  UINT16 Value;
} MSI_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MSI_CAP_LIST_ADDRESS    0x111000a0UL

#define SMN_FUNC0_PCIE1_MSI_CAP_LIST_ADDRESS    0x112000a0UL

#define SMN_FUNC0_PCIE2_MSI_CAP_LIST_ADDRESS    0x113000a0UL

#define SMN_FUNC0_PCIE3_MSI_CAP_LIST_ADDRESS    0x114000a0UL

#define SMN_FUNC1_PCIE0_MSI_CAP_LIST_ADDRESS    0x111010a0UL

#define SMN_FUNC1_PCIE1_MSI_CAP_LIST_ADDRESS    0x112010a0UL

#define SMN_FUNC2_PCIE0_MSI_CAP_LIST_ADDRESS    0x111020a0UL

#define SMN_FUNC2_PCIE1_MSI_CAP_LIST_ADDRESS    0x112020a0UL

#define SMN_FUNC3_PCIE0_MSI_CAP_LIST_ADDRESS    0x111030a0UL

#define SMN_FUNC3_PCIE1_MSI_CAP_LIST_ADDRESS    0x112030a0UL

#define SMN_FUNC4_PCIE0_MSI_CAP_LIST_ADDRESS    0x111040a0UL

#define SMN_FUNC4_PCIE1_MSI_CAP_LIST_ADDRESS    0x112040a0UL

#define SMN_FUNC5_PCIE1_MSI_CAP_LIST_ADDRESS    0x112050a0UL


/***********************************************************
* Register Name : MSI_MAP_CAP
* Register Description :
* MSI Mapping Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the MSI mapping is active.
#define MSI_MAP_CAP_EN_OFFSET      0
#define MSI_MAP_CAP_EN_MASK        0x1

// Bitfield Description : Indicates if the function supports a fixed MSI mapping at 0xFEExxxxx.
#define MSI_MAP_CAP_FIXD_OFFSET      1
#define MSI_MAP_CAP_FIXD_MASK        0x2

// Bitfield Description : 
#define MSI_MAP_CAP_Reserved_10_2_OFFSET      2
#define MSI_MAP_CAP_Reserved_10_2_MASK        0x7fc

// Bitfield Description : Indicates this as the MSI Mapping Capability block.
#define MSI_MAP_CAP_CAP_TYPE_OFFSET      11
#define MSI_MAP_CAP_CAP_TYPE_MASK        0xf800

typedef union {
  struct {
    UINT16                            EN:1;
    UINT16                            FIXD:1;
    UINT16                            Reserved_10_2:9;
    UINT16                            CAP_TYPE:5;
  } Field;
  UINT16 Value;
} MSI_MAP_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MSI_MAP_CAP_ADDRESS    0x111000caUL

#define SMN_FUNC0_PCIE1_MSI_MAP_CAP_ADDRESS    0x112000caUL

#define SMN_FUNC0_PCIE2_MSI_MAP_CAP_ADDRESS    0x113000caUL

#define SMN_FUNC0_PCIE3_MSI_MAP_CAP_ADDRESS    0x114000caUL

#define SMN_FUNC1_PCIE0_MSI_MAP_CAP_ADDRESS    0x111010caUL

#define SMN_FUNC1_PCIE1_MSI_MAP_CAP_ADDRESS    0x112010caUL

#define SMN_FUNC2_PCIE0_MSI_MAP_CAP_ADDRESS    0x111020caUL

#define SMN_FUNC2_PCIE1_MSI_MAP_CAP_ADDRESS    0x112020caUL

#define SMN_FUNC3_PCIE0_MSI_MAP_CAP_ADDRESS    0x111030caUL

#define SMN_FUNC3_PCIE1_MSI_MAP_CAP_ADDRESS    0x112030caUL

#define SMN_FUNC4_PCIE0_MSI_MAP_CAP_ADDRESS    0x111040caUL

#define SMN_FUNC4_PCIE1_MSI_MAP_CAP_ADDRESS    0x112040caUL

#define SMN_FUNC5_PCIE1_MSI_MAP_CAP_ADDRESS    0x112050caUL


/***********************************************************
* Register Name : MSI_MAP_CAP_LIST
* Register Description :
* HyperTransport Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the capability.
#define MSI_MAP_CAP_LIST_CAP_ID_OFFSET      0
#define MSI_MAP_CAP_LIST_CAP_ID_MASK        0xff

// Bitfield Description : Pointer to the next item in the capabilities list, or 00h if no other items exist.
#define MSI_MAP_CAP_LIST_NEXT_PTR_OFFSET      8
#define MSI_MAP_CAP_LIST_NEXT_PTR_MASK        0xff00

typedef union {
  struct {
    UINT16                            CAP_ID:8;
    UINT16                            NEXT_PTR:8;
  } Field;
  UINT16 Value;
} MSI_MAP_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MSI_MAP_CAP_LIST_ADDRESS    0x111000c8UL

#define SMN_FUNC0_PCIE1_MSI_MAP_CAP_LIST_ADDRESS    0x112000c8UL

#define SMN_FUNC0_PCIE2_MSI_MAP_CAP_LIST_ADDRESS    0x113000c8UL

#define SMN_FUNC0_PCIE3_MSI_MAP_CAP_LIST_ADDRESS    0x114000c8UL

#define SMN_FUNC1_PCIE0_MSI_MAP_CAP_LIST_ADDRESS    0x111010c8UL

#define SMN_FUNC1_PCIE1_MSI_MAP_CAP_LIST_ADDRESS    0x112010c8UL

#define SMN_FUNC2_PCIE0_MSI_MAP_CAP_LIST_ADDRESS    0x111020c8UL

#define SMN_FUNC2_PCIE1_MSI_MAP_CAP_LIST_ADDRESS    0x112020c8UL

#define SMN_FUNC3_PCIE0_MSI_MAP_CAP_LIST_ADDRESS    0x111030c8UL

#define SMN_FUNC3_PCIE1_MSI_MAP_CAP_LIST_ADDRESS    0x112030c8UL

#define SMN_FUNC4_PCIE0_MSI_MAP_CAP_LIST_ADDRESS    0x111040c8UL

#define SMN_FUNC4_PCIE1_MSI_MAP_CAP_LIST_ADDRESS    0x112040c8UL

#define SMN_FUNC5_PCIE1_MSI_MAP_CAP_LIST_ADDRESS    0x112050c8UL


/***********************************************************
* Register Name : MSI_MSG_ADDR_HI
* Register Description :
* Message Upper Address register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32 bits for MSI address.
#define MSI_MSG_ADDR_HI_MSI_MSG_ADDR_HI_OFFSET      0
#define MSI_MSG_ADDR_HI_MSI_MSG_ADDR_HI_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MSI_MSG_ADDR_HI:32;
  } Field;
  UINT32 Value;
} MSI_MSG_ADDR_HI_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MSI_MSG_ADDR_HI_ADDRESS    0x11100001UL

#define SMN_FUNC0_PCIE1_MSI_MSG_ADDR_HI_ADDRESS    0x11200001UL

#define SMN_FUNC0_PCIE2_MSI_MSG_ADDR_HI_ADDRESS    0x11300001UL

#define SMN_FUNC0_PCIE3_MSI_MSG_ADDR_HI_ADDRESS    0x11400001UL

#define SMN_FUNC1_PCIE0_MSI_MSG_ADDR_HI_ADDRESS    0x11100001UL

#define SMN_FUNC1_PCIE1_MSI_MSG_ADDR_HI_ADDRESS    0x11200001UL

#define SMN_FUNC2_PCIE0_MSI_MSG_ADDR_HI_ADDRESS    0x11100001UL

#define SMN_FUNC2_PCIE1_MSI_MSG_ADDR_HI_ADDRESS    0x11200001UL

#define SMN_FUNC3_PCIE0_MSI_MSG_ADDR_HI_ADDRESS    0x11100001UL

#define SMN_FUNC3_PCIE1_MSI_MSG_ADDR_HI_ADDRESS    0x11200001UL

#define SMN_FUNC4_PCIE0_MSI_MSG_ADDR_HI_ADDRESS    0x11100001UL

#define SMN_FUNC4_PCIE1_MSI_MSG_ADDR_HI_ADDRESS    0x11200001UL

#define SMN_FUNC5_PCIE1_MSI_MSG_ADDR_HI_ADDRESS    0x11200001UL


/***********************************************************
* Register Name : MSI_MSG_ADDR_LO
* Register Description :
* Message Lower Address register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define MSI_MSG_ADDR_LO_Reserved_1_0_OFFSET      0
#define MSI_MSG_ADDR_LO_Reserved_1_0_MASK        0x3

// Bitfield Description : Bits 31-2 for MSI address.
#define MSI_MSG_ADDR_LO_MSI_MSG_ADDR_LO_OFFSET      2
#define MSI_MSG_ADDR_LO_MSI_MSG_ADDR_LO_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            Reserved_1_0:2;
    UINT32                            MSI_MSG_ADDR_LO:30;
  } Field;
  UINT32 Value;
} MSI_MSG_ADDR_LO_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MSI_MSG_ADDR_LO_ADDRESS    0x111000a4UL

#define SMN_FUNC0_PCIE1_MSI_MSG_ADDR_LO_ADDRESS    0x112000a4UL

#define SMN_FUNC0_PCIE2_MSI_MSG_ADDR_LO_ADDRESS    0x113000a4UL

#define SMN_FUNC0_PCIE3_MSI_MSG_ADDR_LO_ADDRESS    0x114000a4UL

#define SMN_FUNC1_PCIE0_MSI_MSG_ADDR_LO_ADDRESS    0x111010a4UL

#define SMN_FUNC1_PCIE1_MSI_MSG_ADDR_LO_ADDRESS    0x112010a4UL

#define SMN_FUNC2_PCIE0_MSI_MSG_ADDR_LO_ADDRESS    0x111020a4UL

#define SMN_FUNC2_PCIE1_MSI_MSG_ADDR_LO_ADDRESS    0x112020a4UL

#define SMN_FUNC3_PCIE0_MSI_MSG_ADDR_LO_ADDRESS    0x111030a4UL

#define SMN_FUNC3_PCIE1_MSI_MSG_ADDR_LO_ADDRESS    0x112030a4UL

#define SMN_FUNC4_PCIE0_MSI_MSG_ADDR_LO_ADDRESS    0x111040a4UL

#define SMN_FUNC4_PCIE1_MSI_MSG_ADDR_LO_ADDRESS    0x112040a4UL

#define SMN_FUNC5_PCIE1_MSI_MSG_ADDR_LO_ADDRESS    0x112050a4UL


/***********************************************************
* Register Name : MSI_MSG_CNTL
* Register Description :
* Message Signaled Interrupt Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the ability of a function to generate MSI requests.
#define MSI_MSG_CNTL_MSI_EN_OFFSET      0
#define MSI_MSG_CNTL_MSI_EN_MASK        0x1

// Bitfield Description : Indicates the number of MSI vectors that the function can support.
#define MSI_MSG_CNTL_MSI_MULTI_CAP_OFFSET      1
#define MSI_MSG_CNTL_MSI_MULTI_CAP_MASK        0xe

// Bitfield Description : Controls the number of MSI vectors that the function is permitted to use.
#define MSI_MSG_CNTL_MSI_MULTI_EN_OFFSET      4
#define MSI_MSG_CNTL_MSI_MULTI_EN_MASK        0x70

// Bitfield Description : Indicates if the function supports generating 64-bit message addresses.
#define MSI_MSG_CNTL_MSI_64BIT_OFFSET      7
#define MSI_MSG_CNTL_MSI_64BIT_MASK        0x80

// Bitfield Description : Indicates if the function supports MSI per-vector masking.
#define MSI_MSG_CNTL_MSI_PERVECTOR_MASKING_CAP_OFFSET      8
#define MSI_MSG_CNTL_MSI_PERVECTOR_MASKING_CAP_MASK        0x100

// Bitfield Description : Indicates if the function supports generating extended message data.
#define MSI_MSG_CNTL_MSI_EXT_MSG_DATA_CAP_OFFSET      9
#define MSI_MSG_CNTL_MSI_EXT_MSG_DATA_CAP_MASK        0x200

// Bitfield Description : Controls the ability of a function to generate extended message data.
#define MSI_MSG_CNTL_MSI_EXT_MSG_DATA_EN_OFFSET      10
#define MSI_MSG_CNTL_MSI_EXT_MSG_DATA_EN_MASK        0x400

// Bitfield Description : 
#define MSI_MSG_CNTL_Reserved_15_11_OFFSET      11
#define MSI_MSG_CNTL_Reserved_15_11_MASK        0xf800

typedef union {
  struct {
    UINT16                            MSI_EN:1;
    UINT16                            MSI_MULTI_CAP:3;
    UINT16                            MSI_MULTI_EN:3;
    UINT16                            MSI_64BIT:1;
    UINT16                            MSI_PERVECTOR_MASKING_CAP:1;
    UINT16                            MSI_EXT_MSG_DATA_CAP:1;
    UINT16                            MSI_EXT_MSG_DATA_EN:1;
    UINT16                            Reserved_15_11:5;
  } Field;
  UINT16 Value;
} MSI_MSG_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MSI_MSG_CNTL_ADDRESS    0x111000a2UL

#define SMN_FUNC0_PCIE1_MSI_MSG_CNTL_ADDRESS    0x112000a2UL

#define SMN_FUNC0_PCIE2_MSI_MSG_CNTL_ADDRESS    0x113000a2UL

#define SMN_FUNC0_PCIE3_MSI_MSG_CNTL_ADDRESS    0x114000a2UL

#define SMN_FUNC1_PCIE0_MSI_MSG_CNTL_ADDRESS    0x111010a2UL

#define SMN_FUNC1_PCIE1_MSI_MSG_CNTL_ADDRESS    0x112010a2UL

#define SMN_FUNC2_PCIE0_MSI_MSG_CNTL_ADDRESS    0x111020a2UL

#define SMN_FUNC2_PCIE1_MSI_MSG_CNTL_ADDRESS    0x112020a2UL

#define SMN_FUNC3_PCIE0_MSI_MSG_CNTL_ADDRESS    0x111030a2UL

#define SMN_FUNC3_PCIE1_MSI_MSG_CNTL_ADDRESS    0x112030a2UL

#define SMN_FUNC4_PCIE0_MSI_MSG_CNTL_ADDRESS    0x111040a2UL

#define SMN_FUNC4_PCIE1_MSI_MSG_CNTL_ADDRESS    0x112040a2UL

#define SMN_FUNC5_PCIE1_MSI_MSG_CNTL_ADDRESS    0x112050a2UL


/***********************************************************
* Register Name : MSI_MSG_DATA
* Register Description :
* Message Data register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 16 bits for Message Data.
#define MSI_MSG_DATA_MSI_DATA_OFFSET      0
#define MSI_MSG_DATA_MSI_DATA_MASK        0xffff

// Bitfield Description : Upper 16 bits for Message Data.
#define MSI_MSG_DATA_MSI_EXT_DATA_OFFSET      16
#define MSI_MSG_DATA_MSI_EXT_DATA_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            MSI_DATA:16;
    UINT32                            MSI_EXT_DATA:16;
  } Field;
  UINT32 Value;
} MSI_MSG_DATA_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MSI_MSG_DATA_ADDRESS    0x11100000UL

#define SMN_FUNC0_PCIE1_MSI_MSG_DATA_ADDRESS    0x11200000UL

#define SMN_FUNC0_PCIE2_MSI_MSG_DATA_ADDRESS    0x11300000UL

#define SMN_FUNC0_PCIE3_MSI_MSG_DATA_ADDRESS    0x11400000UL

#define SMN_FUNC1_PCIE0_MSI_MSG_DATA_ADDRESS    0x11100000UL

#define SMN_FUNC1_PCIE1_MSI_MSG_DATA_ADDRESS    0x11200000UL

#define SMN_FUNC2_PCIE0_MSI_MSG_DATA_ADDRESS    0x11100000UL

#define SMN_FUNC2_PCIE1_MSI_MSG_DATA_ADDRESS    0x11200000UL

#define SMN_FUNC3_PCIE0_MSI_MSG_DATA_ADDRESS    0x11100000UL

#define SMN_FUNC3_PCIE1_MSI_MSG_DATA_ADDRESS    0x11200000UL

#define SMN_FUNC4_PCIE0_MSI_MSG_DATA_ADDRESS    0x11100000UL

#define SMN_FUNC4_PCIE1_MSI_MSG_DATA_ADDRESS    0x11200000UL

#define SMN_FUNC5_PCIE1_MSI_MSG_DATA_ADDRESS    0x11200000UL


/***********************************************************
* Register Name : MSI_MSG_DATA_64
* Register Description :
* Message Data register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 16 bits for Message Data.
#define MSI_MSG_DATA_64_MSI_DATA_64_OFFSET      0
#define MSI_MSG_DATA_64_MSI_DATA_64_MASK        0xffff

// Bitfield Description : Upper 16 bits for Message Data.
#define MSI_MSG_DATA_64_MSI_EXT_DATA_64_OFFSET      16
#define MSI_MSG_DATA_64_MSI_EXT_DATA_64_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            MSI_DATA_64:16;
    UINT32                            MSI_EXT_DATA_64:16;
  } Field;
  UINT32 Value;
} MSI_MSG_DATA_64_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_MSI_MSG_DATA_64_ADDRESS    0x11100001UL

#define SMN_FUNC0_PCIE1_MSI_MSG_DATA_64_ADDRESS    0x11200001UL

#define SMN_FUNC0_PCIE2_MSI_MSG_DATA_64_ADDRESS    0x11300001UL

#define SMN_FUNC0_PCIE3_MSI_MSG_DATA_64_ADDRESS    0x11400001UL

#define SMN_FUNC1_PCIE0_MSI_MSG_DATA_64_ADDRESS    0x11100001UL

#define SMN_FUNC1_PCIE1_MSI_MSG_DATA_64_ADDRESS    0x11200001UL

#define SMN_FUNC2_PCIE0_MSI_MSG_DATA_64_ADDRESS    0x11100001UL

#define SMN_FUNC2_PCIE1_MSI_MSG_DATA_64_ADDRESS    0x11200001UL

#define SMN_FUNC3_PCIE0_MSI_MSG_DATA_64_ADDRESS    0x11100001UL

#define SMN_FUNC3_PCIE1_MSI_MSG_DATA_64_ADDRESS    0x11200001UL

#define SMN_FUNC4_PCIE0_MSI_MSG_DATA_64_ADDRESS    0x11100001UL

#define SMN_FUNC4_PCIE1_MSI_MSG_DATA_64_ADDRESS    0x11200001UL

#define SMN_FUNC5_PCIE1_MSI_MSG_DATA_64_ADDRESS    0x11200001UL


/***********************************************************
* Register Name : NPEM_CAP
* Register Description :
* The NPEM Capability register identifies Port specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if Native PCIe Enclosure Management is implemented for this Port.
#define NPEM_CAP_NPEM_CAPABLE_OFFSET      0
#define NPEM_CAP_NPEM_CAPABLE_MASK        0x1

// Bitfield Description : Indicates if NPEM Reset mechanism is implemented for this Port.
#define NPEM_CAP_NPEM_RESET_CAPABLE_OFFSET      1
#define NPEM_CAP_NPEM_RESET_CAPABLE_MASK        0x2

// Bitfield Description : Indicates if NPEM OK state indication is implemented for this Port.
#define NPEM_CAP_NPEM_OK_CAPABLE_OFFSET      2
#define NPEM_CAP_NPEM_OK_CAPABLE_MASK        0x4

// Bitfield Description : Indicates if NPEM Locate state indication is implemented for this Port.
#define NPEM_CAP_NPEM_LOCATE_CAPABLE_OFFSET      3
#define NPEM_CAP_NPEM_LOCATE_CAPABLE_MASK        0x8

// Bitfield Description : Indicates if NPEM Fail state indication is implemented for this Port.
#define NPEM_CAP_NPEM_FAIL_CAPABLE_OFFSET      4
#define NPEM_CAP_NPEM_FAIL_CAPABLE_MASK        0x10

// Bitfield Description : Indicates NPEM Rebuild state indication is implemented for this Port.
#define NPEM_CAP_NPEM_REBUILD_CAPABLE_OFFSET      5
#define NPEM_CAP_NPEM_REBUILD_CAPABLE_MASK        0x20

// Bitfield Description : Indicates if NPEM PFA state indication is implemented for this Port.
#define NPEM_CAP_NPEM_PFA_CAPABLE_OFFSET      6
#define NPEM_CAP_NPEM_PFA_CAPABLE_MASK        0x40

// Bitfield Description : Indicates if NPEM Hot Spare state indication is implemented for this Port.
#define NPEM_CAP_NPEM_HOT_SPARE_CAPABLE_OFFSET      7
#define NPEM_CAP_NPEM_HOT_SPARE_CAPABLE_MASK        0x80

// Bitfield Description : Indicates if NPEM In A Critical Array state indication is implemented for this Port.
#define NPEM_CAP_NPEM_CRITICAL_ARRAY_CAPABLE_OFFSET      8
#define NPEM_CAP_NPEM_CRITICAL_ARRAY_CAPABLE_MASK        0x100

// Bitfield Description : Indicates if NPEM In A Failed Array state indication is implemented for this Port.
#define NPEM_CAP_NPEM_FAILED_ARRAY_CAPABLE_OFFSET      9
#define NPEM_CAP_NPEM_FAILED_ARRAY_CAPABLE_MASK        0x200

// Bitfield Description : Indicates if NPEM Invalid Device Type state indication is implemented for this Port.
#define NPEM_CAP_NPEM_INV_DEVICE_TYPE_CAPABLE_OFFSET      10
#define NPEM_CAP_NPEM_INV_DEVICE_TYPE_CAPABLE_MASK        0x400

// Bitfield Description : Indicates if NPEM Disabled state indication is implemented for this Port.
#define NPEM_CAP_NPEM_DISABLED_CAPABLE_OFFSET      11
#define NPEM_CAP_NPEM_DISABLED_CAPABLE_MASK        0x800

// Bitfield Description : 
#define NPEM_CAP_Reserved_23_12_OFFSET      12
#define NPEM_CAP_Reserved_23_12_MASK        0xfff000

// Bitfield Description : Indicates if enclosure-specific capabilities are implemented for this Port. This field is a bit vector that covers all supported capabilities.
#define NPEM_CAP_ENCLOSURE_SPECIFIC_CAPS_OFFSET      24
#define NPEM_CAP_ENCLOSURE_SPECIFIC_CAPS_MASK        0xff000000

typedef union {
  struct {
    UINT32                            NPEM_CAPABLE:1;
    UINT32                            NPEM_RESET_CAPABLE:1;
    UINT32                            NPEM_OK_CAPABLE:1;
    UINT32                            NPEM_LOCATE_CAPABLE:1;
    UINT32                            NPEM_FAIL_CAPABLE:1;
    UINT32                            NPEM_REBUILD_CAPABLE:1;
    UINT32                            NPEM_PFA_CAPABLE:1;
    UINT32                            NPEM_HOT_SPARE_CAPABLE:1;
    UINT32                            NPEM_CRITICAL_ARRAY_CAPABLE:1;
    UINT32                            NPEM_FAILED_ARRAY_CAPABLE:1;
    UINT32                            NPEM_INV_DEVICE_TYPE_CAPABLE:1;
    UINT32                            NPEM_DISABLED_CAPABLE:1;
    UINT32                            Reserved_23_12:12;
    UINT32                            ENCLOSURE_SPECIFIC_CAPS:8;
  } Field;
  UINT32 Value;
} NPEM_CAP_STRUCT;

#define SMN_FUNC0_PCIE0_NPEM_CAP_ADDRESS    0x111004d4UL

#define SMN_FUNC0_PCIE1_NPEM_CAP_ADDRESS    0x112004d4UL

#define SMN_FUNC0_PCIE2_NPEM_CAP_ADDRESS    0x113004d4UL

#define SMN_FUNC0_PCIE3_NPEM_CAP_ADDRESS    0x114004d4UL

#define SMN_FUNC1_PCIE0_NPEM_CAP_ADDRESS    0x111014d4UL

#define SMN_FUNC1_PCIE1_NPEM_CAP_ADDRESS    0x112014d4UL

#define SMN_FUNC2_PCIE0_NPEM_CAP_ADDRESS    0x111024d4UL

#define SMN_FUNC2_PCIE1_NPEM_CAP_ADDRESS    0x112024d4UL

#define SMN_FUNC3_PCIE0_NPEM_CAP_ADDRESS    0x111034d4UL

#define SMN_FUNC3_PCIE1_NPEM_CAP_ADDRESS    0x112034d4UL

#define SMN_FUNC4_PCIE0_NPEM_CAP_ADDRESS    0x111044d4UL

#define SMN_FUNC4_PCIE1_NPEM_CAP_ADDRESS    0x112044d4UL

#define SMN_FUNC5_PCIE1_NPEM_CAP_ADDRESS    0x112054d4UL


/***********************************************************
* Register Name : NPEM_CNTL
* Register Description :
* The NPEM Control register controls port specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : If Native PCIe Enclosure Management is supported, controls if the Port enables NPEM features.
#define NPEM_CNTL_NPEM_ENABLE_OFFSET      0
#define NPEM_CNTL_NPEM_ENABLE_MASK        0x1

// Bitfield Description : If NPEM Reset mechanism is supported, a write of 1 to this bit initiates NPEM Reset to the Port. Reads always return 0.
#define NPEM_CNTL_NPEM_INITIATE_RESET_OFFSET      1
#define NPEM_CNTL_NPEM_INITIATE_RESET_MASK        0x2

// Bitfield Description : If NPEM OK state indication is supported, writes set the NPEM OK Indicator to the written state. Reads return the last written value.
#define NPEM_CNTL_NPEM_OK_CONTROL_OFFSET      2
#define NPEM_CNTL_NPEM_OK_CONTROL_MASK        0x4

// Bitfield Description : If NPEM Locate state indication is supported, writes set the NPEM Locate Indicator to the written state. Reads return the last written value.
#define NPEM_CNTL_NPEM_LOCATE_CONTROL_OFFSET      3
#define NPEM_CNTL_NPEM_LOCATE_CONTROL_MASK        0x8

// Bitfield Description : If NPEM Fail state indication is supported, writes set the NPEM Fail Indicator to the written state. Reads return the last written value.
#define NPEM_CNTL_NPEM_FAIL_CONTROL_OFFSET      4
#define NPEM_CNTL_NPEM_FAIL_CONTROL_MASK        0x10

// Bitfield Description : If NPEM Rebuild state indication is supported, writes set the NPEM Rebuild Indicator to the written state. Reads return the last written value.
#define NPEM_CNTL_NPEM_REBULD_CONTROL_OFFSET      5
#define NPEM_CNTL_NPEM_REBULD_CONTROL_MASK        0x20

// Bitfield Description : If NPEM PFA state indication is supported, writes set the NPEM PFA Indicator to the written state. Reads return the last written value.
#define NPEM_CNTL_NPEM_PFA_CONTROL_OFFSET      6
#define NPEM_CNTL_NPEM_PFA_CONTROL_MASK        0x40

// Bitfield Description : If NPEM Hot Spare state indication is supported, writes set the NPEM Hot Spare Indicator to the written state. Reads return the last written value.
#define NPEM_CNTL_NPEM_HOT_SPARE_CONTROL_OFFSET      7
#define NPEM_CNTL_NPEM_HOT_SPARE_CONTROL_MASK        0x80

// Bitfield Description : If NPEM In A Critical Array state indication is supported, writes set the NPEM In A Critical Array Indicator to the written state. Reads return the last written value.
#define NPEM_CNTL_NPEM_CRITICAL_ARRAY_CONTROL_OFFSET      8
#define NPEM_CNTL_NPEM_CRITICAL_ARRAY_CONTROL_MASK        0x100

// Bitfield Description : If NPEM In A Failed Array state indication is supported, writes set the NPEM In A Failed Array Indicator to the written state. Reads return the last written value.
#define NPEM_CNTL_NPEM_FAILED_ARRAY_CONTROL_OFFSET      9
#define NPEM_CNTL_NPEM_FAILED_ARRAY_CONTROL_MASK        0x200

// Bitfield Description : If NPEM Invalid Device Type state indication is supported, writes set the NPEM Invalid Device Type Indicator to the written state. Reads return the last written value.
#define NPEM_CNTL_NPEM_INV_DEVICE_TYPE_CONTROL_OFFSET      10
#define NPEM_CNTL_NPEM_INV_DEVICE_TYPE_CONTROL_MASK        0x400

// Bitfield Description : If NPEM Disabled state indication is supported, writes set the NPEM Disabled Indicator to the written state. Reads return the last written value.
#define NPEM_CNTL_NPEM_DISABLED_CONTROL_OFFSET      11
#define NPEM_CNTL_NPEM_DISABLED_CONTROL_MASK        0x800

// Bitfield Description : 
#define NPEM_CNTL_Reserved_23_12_OFFSET      12
#define NPEM_CNTL_Reserved_23_12_MASK        0xfff000

// Bitfield Description : If enclosure-specific capabilities are supported, writes set the enclosure-specific control to the written state. Reads return the last written value. This field is a bit vector that covers all supported controls.
#define NPEM_CNTL_ENCLOSURE_SPECIFIC_CNTLS_OFFSET      24
#define NPEM_CNTL_ENCLOSURE_SPECIFIC_CNTLS_MASK        0xff000000

typedef union {
  struct {
    UINT32                            NPEM_ENABLE:1;
    UINT32                            NPEM_INITIATE_RESET:1;
    UINT32                            NPEM_OK_CONTROL:1;
    UINT32                            NPEM_LOCATE_CONTROL:1;
    UINT32                            NPEM_FAIL_CONTROL:1;
    UINT32                            NPEM_REBULD_CONTROL:1;
    UINT32                            NPEM_PFA_CONTROL:1;
    UINT32                            NPEM_HOT_SPARE_CONTROL:1;
    UINT32                            NPEM_CRITICAL_ARRAY_CONTROL:1;
    UINT32                            NPEM_FAILED_ARRAY_CONTROL:1;
    UINT32                            NPEM_INV_DEVICE_TYPE_CONTROL:1;
    UINT32                            NPEM_DISABLED_CONTROL:1;
    UINT32                            Reserved_23_12:12;
    UINT32                            ENCLOSURE_SPECIFIC_CNTLS:8;
  } Field;
  UINT32 Value;
} NPEM_CNTL_STRUCT;

#define SMN_FUNC0_PCIE0_NPEM_CNTL_ADDRESS    0x111004d8UL

#define SMN_FUNC0_PCIE1_NPEM_CNTL_ADDRESS    0x112004d8UL

#define SMN_FUNC0_PCIE2_NPEM_CNTL_ADDRESS    0x113004d8UL

#define SMN_FUNC0_PCIE3_NPEM_CNTL_ADDRESS    0x114004d8UL

#define SMN_FUNC1_PCIE0_NPEM_CNTL_ADDRESS    0x111014d8UL

#define SMN_FUNC1_PCIE1_NPEM_CNTL_ADDRESS    0x112014d8UL

#define SMN_FUNC2_PCIE0_NPEM_CNTL_ADDRESS    0x111024d8UL

#define SMN_FUNC2_PCIE1_NPEM_CNTL_ADDRESS    0x112024d8UL

#define SMN_FUNC3_PCIE0_NPEM_CNTL_ADDRESS    0x111034d8UL

#define SMN_FUNC3_PCIE1_NPEM_CNTL_ADDRESS    0x112034d8UL

#define SMN_FUNC4_PCIE0_NPEM_CNTL_ADDRESS    0x111044d8UL

#define SMN_FUNC4_PCIE1_NPEM_CNTL_ADDRESS    0x112044d8UL

#define SMN_FUNC5_PCIE1_NPEM_CNTL_ADDRESS    0x112054d8UL


/***********************************************************
* Register Name : NPEM_STATUS
* Register Description :
* The NPEM Status register provides information about port specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the NPEM Controller completes an issued command.
#define NPEM_STATUS_NPEM_COMMAND_COMPLETED_OFFSET      0
#define NPEM_STATUS_NPEM_COMMAND_COMPLETED_MASK        0x1

// Bitfield Description : 
#define NPEM_STATUS_Reserved_23_1_OFFSET      1
#define NPEM_STATUS_Reserved_23_1_MASK        0xfffffe

// Bitfield Description : If enclosure-specific capabilities are supported, indicates enclosure-specific status. Reads return the last written value. This field is a bit vector that covers all supported status.
#define NPEM_STATUS_ENCLOSURE_SPECIFIC_STATS_OFFSET      24
#define NPEM_STATUS_ENCLOSURE_SPECIFIC_STATS_MASK        0xff000000

typedef union {
  struct {
    UINT32                            NPEM_COMMAND_COMPLETED:1;
    UINT32                            Reserved_23_1:23;
    UINT32                            ENCLOSURE_SPECIFIC_STATS:8;
  } Field;
  UINT32 Value;
} NPEM_STATUS_STRUCT;

#define SMN_FUNC0_PCIE0_NPEM_STATUS_ADDRESS    0x111004dcUL

#define SMN_FUNC0_PCIE1_NPEM_STATUS_ADDRESS    0x112004dcUL

#define SMN_FUNC0_PCIE2_NPEM_STATUS_ADDRESS    0x113004dcUL

#define SMN_FUNC0_PCIE3_NPEM_STATUS_ADDRESS    0x114004dcUL

#define SMN_FUNC1_PCIE0_NPEM_STATUS_ADDRESS    0x111014dcUL

#define SMN_FUNC1_PCIE1_NPEM_STATUS_ADDRESS    0x112014dcUL

#define SMN_FUNC2_PCIE0_NPEM_STATUS_ADDRESS    0x111024dcUL

#define SMN_FUNC2_PCIE1_NPEM_STATUS_ADDRESS    0x112024dcUL

#define SMN_FUNC3_PCIE0_NPEM_STATUS_ADDRESS    0x111034dcUL

#define SMN_FUNC3_PCIE1_NPEM_STATUS_ADDRESS    0x112034dcUL

#define SMN_FUNC4_PCIE0_NPEM_STATUS_ADDRESS    0x111044dcUL

#define SMN_FUNC4_PCIE1_NPEM_STATUS_ADDRESS    0x112044dcUL

#define SMN_FUNC5_PCIE1_NPEM_STATUS_ADDRESS    0x112054dcUL


/***********************************************************
* Register Name : PCIE_ACS_CAP
* Register Description :
* ACS Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates that the component implements ACS Source Validation.
#define PCIE_ACS_CAP_SOURCE_VALIDATION_OFFSET      0
#define PCIE_ACS_CAP_SOURCE_VALIDATION_MASK        0x1

// Bitfield Description : Indicates that the component implements ACS Translation Blocking.
#define PCIE_ACS_CAP_TRANSLATION_BLOCKING_OFFSET      1
#define PCIE_ACS_CAP_TRANSLATION_BLOCKING_MASK        0x2

// Bitfield Description : Indicates that the component implements ACS P2P Request Redirect.
#define PCIE_ACS_CAP_P2P_REQUEST_REDIRECT_OFFSET      2
#define PCIE_ACS_CAP_P2P_REQUEST_REDIRECT_MASK        0x4

// Bitfield Description : Indicates that the component implements ACS P2P Completion Redirect.
#define PCIE_ACS_CAP_P2P_COMPLETION_REDIRECT_OFFSET      3
#define PCIE_ACS_CAP_P2P_COMPLETION_REDIRECT_MASK        0x8

// Bitfield Description : Indicates that the component implements ACS Upstream Forwarding.
#define PCIE_ACS_CAP_UPSTREAM_FORWARDING_OFFSET      4
#define PCIE_ACS_CAP_UPSTREAM_FORWARDING_MASK        0x10

// Bitfield Description : Indicates that the component implements ACS P2P Egress Control.
#define PCIE_ACS_CAP_P2P_EGRESS_CONTROL_OFFSET      5
#define PCIE_ACS_CAP_P2P_EGRESS_CONTROL_MASK        0x20

// Bitfield Description : Indicates that the component implements ACS Direct Translated P2P.
#define PCIE_ACS_CAP_DIRECT_TRANSLATED_P2P_OFFSET      6
#define PCIE_ACS_CAP_DIRECT_TRANSLATED_P2P_MASK        0x40

// Bitfield Description : Indicates that the component implements ACS Enhanced Capability.
#define PCIE_ACS_CAP_ENHANCED_CAPABILITY_OFFSET      7
#define PCIE_ACS_CAP_ENHANCED_CAPABILITY_MASK        0x80

// Bitfield Description : When ACS P2P Egress Control bit is set, a non-zero value indicates the number of bits in the Egress Control Vector, while a value of 0 indicates 256 bits in the Egress Control Vector.
#define PCIE_ACS_CAP_EGRESS_CONTROL_VECTOR_SIZE_OFFSET      8
#define PCIE_ACS_CAP_EGRESS_CONTROL_VECTOR_SIZE_MASK        0xff00

typedef union {
  struct {
    UINT16                            SOURCE_VALIDATION:1;
    UINT16                            TRANSLATION_BLOCKING:1;
    UINT16                            P2P_REQUEST_REDIRECT:1;
    UINT16                            P2P_COMPLETION_REDIRECT:1;
    UINT16                            UPSTREAM_FORWARDING:1;
    UINT16                            P2P_EGRESS_CONTROL:1;
    UINT16                            DIRECT_TRANSLATED_P2P:1;
    UINT16                            ENHANCED_CAPABILITY:1;
    UINT16                            EGRESS_CONTROL_VECTOR_SIZE:8;
  } Field;
  UINT16 Value;
} PCIE_ACS_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ACS_CAP_ADDRESS    0x111002a4UL

#define SMN_FUNC0_PCIE1_PCIE_ACS_CAP_ADDRESS    0x112002a4UL

#define SMN_FUNC0_PCIE2_PCIE_ACS_CAP_ADDRESS    0x113002a4UL

#define SMN_FUNC0_PCIE3_PCIE_ACS_CAP_ADDRESS    0x114002a4UL

#define SMN_FUNC1_PCIE0_PCIE_ACS_CAP_ADDRESS    0x111012a4UL

#define SMN_FUNC1_PCIE1_PCIE_ACS_CAP_ADDRESS    0x112012a4UL

#define SMN_FUNC2_PCIE0_PCIE_ACS_CAP_ADDRESS    0x111022a4UL

#define SMN_FUNC2_PCIE1_PCIE_ACS_CAP_ADDRESS    0x112022a4UL

#define SMN_FUNC3_PCIE0_PCIE_ACS_CAP_ADDRESS    0x111032a4UL

#define SMN_FUNC3_PCIE1_PCIE_ACS_CAP_ADDRESS    0x112032a4UL

#define SMN_FUNC4_PCIE0_PCIE_ACS_CAP_ADDRESS    0x111042a4UL

#define SMN_FUNC4_PCIE1_PCIE_ACS_CAP_ADDRESS    0x112042a4UL

#define SMN_FUNC5_PCIE1_PCIE_ACS_CAP_ADDRESS    0x112052a4UL


/***********************************************************
* Register Name : PCIE_ACS_CNTL
* Register Description :
* ACS Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the component validates the Bus Number from the Requester ID of Upstream Requests against the secondary or subordinate Bus Numbers.
#define PCIE_ACS_CNTL_SOURCE_VALIDATION_EN_OFFSET      0
#define PCIE_ACS_CNTL_SOURCE_VALIDATION_EN_MASK        0x1

// Bitfield Description : Controls if the component blocks all Upstream Memory Requests whose Address Translation field is not set to the default value.
#define PCIE_ACS_CNTL_TRANSLATION_BLOCKING_EN_OFFSET      1
#define PCIE_ACS_CNTL_TRANSLATION_BLOCKING_EN_MASK        0x2

// Bitfield Description : In conjunction with ACS P2P Egress Control and ACS Direct Translated P2P mechanisms, controls if the component redirects P2P Requests Upstream.
#define PCIE_ACS_CNTL_P2P_REQUEST_REDIRECT_EN_OFFSET      2
#define PCIE_ACS_CNTL_P2P_REQUEST_REDIRECT_EN_MASK        0x4

// Bitfield Description : Controls if the component redirects P2P Completions Upstream; applicable only to Read Completions whose Relaxed Ordering Attribute is clear.
#define PCIE_ACS_CNTL_P2P_COMPLETION_REDIRECT_EN_OFFSET      3
#define PCIE_ACS_CNTL_P2P_COMPLETION_REDIRECT_EN_MASK        0x8

// Bitfield Description : Controls if the component forwards Upstream any Request or Completion TLPs it receives that were redirected Upstream by a component lower in the hierarchy.
#define PCIE_ACS_CNTL_UPSTREAM_FORWARDING_EN_OFFSET      4
#define PCIE_ACS_CNTL_UPSTREAM_FORWARDING_EN_MASK        0x10

// Bitfield Description : In conjunction with the Egress Control Vector plus the ACS P2P Request Redirect and ACS Direct Translated P2P mechanisms, controls when to allow, disallow, or redirect P2P Requests.
#define PCIE_ACS_CNTL_P2P_EGRESS_CONTROL_EN_OFFSET      5
#define PCIE_ACS_CNTL_P2P_EGRESS_CONTROL_EN_MASK        0x20

// Bitfield Description : Controls if the component overrides the ACS P2P Request Redirect and ACS P2P Egress Control mechanisms with P2P Memory Requests whose Address Translation field indicates a Translated address.
#define PCIE_ACS_CNTL_DIRECT_TRANSLATED_P2P_EN_OFFSET      6
#define PCIE_ACS_CNTL_DIRECT_TRANSLATED_P2P_EN_MASK        0x40

// Bitfield Description : Controls if the component handles upstream I/O Requests as ACS Violations.
#define PCIE_ACS_CNTL_IO_REQUEST_BLOCKING_EN_OFFSET      7
#define PCIE_ACS_CNTL_IO_REQUEST_BLOCKING_EN_MASK        0x80

// Bitfield Description : Controls how the component handles upstream Memory Requests that target the component's BAR claimed memory space.
#define PCIE_ACS_CNTL_DSP_MEMORY_TARGET_ACCESS_CNTL_OFFSET      8
#define PCIE_ACS_CNTL_DSP_MEMORY_TARGET_ACCESS_CNTL_MASK        0x300

// Bitfield Description : Controls how the component handles upstream Memory Requests that target the component's Switch Upstream Port BAR claimed memory space.
#define PCIE_ACS_CNTL_USP_MEMORY_TARGET_ACCESS_CNTL_OFFSET      10
#define PCIE_ACS_CNTL_USP_MEMORY_TARGET_ACCESS_CNTL_MASK        0xc00

// Bitfield Description : Controls how the component handles upstream Memory Requests that target the component's Switch Upstream Port memory aperture which is not within any of the component's Switch Downstream Port memory aperture or BAR claimed memory space.
#define PCIE_ACS_CNTL_UNCLAIMED_REQUEST_REDIRECT_CNTL_OFFSET      12
#define PCIE_ACS_CNTL_UNCLAIMED_REQUEST_REDIRECT_CNTL_MASK        0x1000

// Bitfield Description : 
#define PCIE_ACS_CNTL_Reserved_15_13_OFFSET      13
#define PCIE_ACS_CNTL_Reserved_15_13_MASK        0xe000

typedef union {
  struct {
    UINT16                            SOURCE_VALIDATION_EN:1;
    UINT16                            TRANSLATION_BLOCKING_EN:1;
    UINT16                            P2P_REQUEST_REDIRECT_EN:1;
    UINT16                            P2P_COMPLETION_REDIRECT_EN:1;
    UINT16                            UPSTREAM_FORWARDING_EN:1;
    UINT16                            P2P_EGRESS_CONTROL_EN:1;
    UINT16                            DIRECT_TRANSLATED_P2P_EN:1;
    UINT16                            IO_REQUEST_BLOCKING_EN:1;
    UINT16                            DSP_MEMORY_TARGET_ACCESS_CNTL:2;
    UINT16                            USP_MEMORY_TARGET_ACCESS_CNTL:2;
    UINT16                            UNCLAIMED_REQUEST_REDIRECT_CNTL:1;
    UINT16                            Reserved_15_13:3;
  } Field;
  UINT16 Value;
} PCIE_ACS_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ACS_CNTL_ADDRESS    0x111002a6UL

#define SMN_FUNC0_PCIE1_PCIE_ACS_CNTL_ADDRESS    0x112002a6UL

#define SMN_FUNC0_PCIE2_PCIE_ACS_CNTL_ADDRESS    0x113002a6UL

#define SMN_FUNC0_PCIE3_PCIE_ACS_CNTL_ADDRESS    0x114002a6UL

#define SMN_FUNC1_PCIE0_PCIE_ACS_CNTL_ADDRESS    0x111012a6UL

#define SMN_FUNC1_PCIE1_PCIE_ACS_CNTL_ADDRESS    0x112012a6UL

#define SMN_FUNC2_PCIE0_PCIE_ACS_CNTL_ADDRESS    0x111022a6UL

#define SMN_FUNC2_PCIE1_PCIE_ACS_CNTL_ADDRESS    0x112022a6UL

#define SMN_FUNC3_PCIE0_PCIE_ACS_CNTL_ADDRESS    0x111032a6UL

#define SMN_FUNC3_PCIE1_PCIE_ACS_CNTL_ADDRESS    0x112032a6UL

#define SMN_FUNC4_PCIE0_PCIE_ACS_CNTL_ADDRESS    0x111042a6UL

#define SMN_FUNC4_PCIE1_PCIE_ACS_CNTL_ADDRESS    0x112042a6UL

#define SMN_FUNC5_PCIE1_PCIE_ACS_CNTL_ADDRESS    0x112052a6UL


/***********************************************************
* Register Name : PCIE_ACS_ENH_CAP_LIST
* Register Description :
* ACS Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_ACS_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_ACS_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_ACS_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_ACS_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_ACS_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_ACS_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_ACS_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x111002a0UL

#define SMN_FUNC0_PCIE1_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x112002a0UL

#define SMN_FUNC0_PCIE2_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x113002a0UL

#define SMN_FUNC0_PCIE3_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x114002a0UL

#define SMN_FUNC1_PCIE0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x111012a0UL

#define SMN_FUNC1_PCIE1_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x112012a0UL

#define SMN_FUNC2_PCIE0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x111022a0UL

#define SMN_FUNC2_PCIE1_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x112022a0UL

#define SMN_FUNC3_PCIE0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x111032a0UL

#define SMN_FUNC3_PCIE1_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x112032a0UL

#define SMN_FUNC4_PCIE0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x111042a0UL

#define SMN_FUNC4_PCIE1_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x112042a0UL

#define SMN_FUNC5_PCIE1_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x112052a0UL


/***********************************************************
* Register Name : PCIE_ADV_ERR_CAP_CNTL
* Register Description :
* Advanced Error Capabilities and Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the bit position of the first error reported in the Uncorrectable Error Status register. This field is only valid if the indicated bit is also set. This field is sticky.
#define PCIE_ADV_ERR_CAP_CNTL_FIRST_ERR_PTR_OFFSET      0
#define PCIE_ADV_ERR_CAP_CNTL_FIRST_ERR_PTR_MASK        0x1f

// Bitfield Description : Indicates if the function is capable of generating ECRC.
#define PCIE_ADV_ERR_CAP_CNTL_ECRC_GEN_CAP_OFFSET      5
#define PCIE_ADV_ERR_CAP_CNTL_ECRC_GEN_CAP_MASK        0x20

// Bitfield Description : Controls if the function generates ECRC. This bit is sticky.
#define PCIE_ADV_ERR_CAP_CNTL_ECRC_GEN_EN_OFFSET      6
#define PCIE_ADV_ERR_CAP_CNTL_ECRC_GEN_EN_MASK        0x40

// Bitfield Description : Indicates if the function is capable of checking ECRC.
#define PCIE_ADV_ERR_CAP_CNTL_ECRC_CHECK_CAP_OFFSET      7
#define PCIE_ADV_ERR_CAP_CNTL_ECRC_CHECK_CAP_MASK        0x80

// Bitfield Description : Controls if the function checks ECRC. This bit is sticky.
#define PCIE_ADV_ERR_CAP_CNTL_ECRC_CHECK_EN_OFFSET      8
#define PCIE_ADV_ERR_CAP_CNTL_ECRC_CHECK_EN_MASK        0x100

// Bitfield Description : Indicates if the function is capable of recording more than one error header.
#define PCIE_ADV_ERR_CAP_CNTL_MULTI_HDR_RECD_CAP_OFFSET      9
#define PCIE_ADV_ERR_CAP_CNTL_MULTI_HDR_RECD_CAP_MASK        0x200

// Bitfield Description : Controls if the function is permitted to record more than one error header.
#define PCIE_ADV_ERR_CAP_CNTL_MULTI_HDR_RECD_EN_OFFSET      10
#define PCIE_ADV_ERR_CAP_CNTL_MULTI_HDR_RECD_EN_MASK        0x400

// Bitfield Description : When the First Error Pointer is valid, indicates that the TLP Prefix Log register contains valid information. This bit is sticky. If End-End TLP Prefix Supported bit is not set, this bit is Reserved.
#define PCIE_ADV_ERR_CAP_CNTL_TLP_PREFIX_LOG_PRESENT_OFFSET      11
#define PCIE_ADV_ERR_CAP_CNTL_TLP_PREFIX_LOG_PRESENT_MASK        0x800

// Bitfield Description : Indicates if the function is capable of recording the TLP Prefix (if any) and the Header for the TLP corresponding to a detected Completion Timeout error.
#define PCIE_ADV_ERR_CAP_CNTL_COMPLETION_TIMEOUT_LOG_CAPABLE_OFFSET      12
#define PCIE_ADV_ERR_CAP_CNTL_COMPLETION_TIMEOUT_LOG_CAPABLE_MASK        0x1000

// Bitfield Description : 
#define PCIE_ADV_ERR_CAP_CNTL_Reserved_31_13_OFFSET      13
#define PCIE_ADV_ERR_CAP_CNTL_Reserved_31_13_MASK        0xffffe000

typedef union {
  struct {
    UINT32                            FIRST_ERR_PTR:5;
    UINT32                            ECRC_GEN_CAP:1;
    UINT32                            ECRC_GEN_EN:1;
    UINT32                            ECRC_CHECK_CAP:1;
    UINT32                            ECRC_CHECK_EN:1;
    UINT32                            MULTI_HDR_RECD_CAP:1;
    UINT32                            MULTI_HDR_RECD_EN:1;
    UINT32                            TLP_PREFIX_LOG_PRESENT:1;
    UINT32                            COMPLETION_TIMEOUT_LOG_CAPABLE:1;
    UINT32                            Reserved_31_13:19;
  } Field;
  UINT32 Value;
} PCIE_ADV_ERR_CAP_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11100168UL

#define SMN_FUNC0_PCIE1_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11200168UL

#define SMN_FUNC0_PCIE2_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11300168UL

#define SMN_FUNC0_PCIE3_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11400168UL

#define SMN_FUNC1_PCIE0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11101168UL

#define SMN_FUNC1_PCIE1_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11201168UL

#define SMN_FUNC2_PCIE0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11102168UL

#define SMN_FUNC2_PCIE1_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11202168UL

#define SMN_FUNC3_PCIE0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11103168UL

#define SMN_FUNC3_PCIE1_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11203168UL

#define SMN_FUNC4_PCIE0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11104168UL

#define SMN_FUNC4_PCIE1_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11204168UL

#define SMN_FUNC5_PCIE1_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x11205168UL


/***********************************************************
* Register Name : PCIE_ADV_ERR_RPT_ENH_CAP_LIST
* Register Description :
* Advanced Error Reporting Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_ADV_ERR_RPT_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_ADV_ERR_RPT_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_ADV_ERR_RPT_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_ADV_ERR_RPT_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_ADV_ERR_RPT_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_ADV_ERR_RPT_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_ADV_ERR_RPT_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11100150UL

#define SMN_FUNC0_PCIE1_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11200150UL

#define SMN_FUNC0_PCIE2_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11300150UL

#define SMN_FUNC0_PCIE3_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11400150UL

#define SMN_FUNC1_PCIE0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11101150UL

#define SMN_FUNC1_PCIE1_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11201150UL

#define SMN_FUNC2_PCIE0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11102150UL

#define SMN_FUNC2_PCIE1_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11202150UL

#define SMN_FUNC3_PCIE0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11103150UL

#define SMN_FUNC3_PCIE1_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11203150UL

#define SMN_FUNC4_PCIE0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11104150UL

#define SMN_FUNC4_PCIE1_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11204150UL

#define SMN_FUNC5_PCIE1_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x11205150UL


/***********************************************************
* Register Name : PCIE_ARI_CAP
* Register Description :
* ARI Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : For Function 0 only, indicates if the device supports Function Group level arbitration via its MFVC capability.
#define PCIE_ARI_CAP_ARI_MFVC_FUNC_GROUPS_CAP_OFFSET      0
#define PCIE_ARI_CAP_ARI_MFVC_FUNC_GROUPS_CAP_MASK        0x1

// Bitfield Description : For Function 0 only, indicates if the device supports Function Group level granularity for ACS P2P Egress Control.
#define PCIE_ARI_CAP_ARI_ACS_FUNC_GROUPS_CAP_OFFSET      1
#define PCIE_ARI_CAP_ARI_ACS_FUNC_GROUPS_CAP_MASK        0x2

// Bitfield Description : 
#define PCIE_ARI_CAP_Reserved_7_2_OFFSET      2
#define PCIE_ARI_CAP_Reserved_7_2_MASK        0xfc

// Bitfield Description : Indicates the next function number in the device, or 0 if this is the last function.
#define PCIE_ARI_CAP_ARI_NEXT_FUNC_NUM_OFFSET      8
#define PCIE_ARI_CAP_ARI_NEXT_FUNC_NUM_MASK        0xff00

typedef union {
  struct {
    UINT16                            ARI_MFVC_FUNC_GROUPS_CAP:1;
    UINT16                            ARI_ACS_FUNC_GROUPS_CAP:1;
    UINT16                            Reserved_7_2:6;
    UINT16                            ARI_NEXT_FUNC_NUM:8;
  } Field;
  UINT16 Value;
} PCIE_ARI_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ARI_CAP_ADDRESS    0x1110032cUL

#define SMN_FUNC0_PCIE1_PCIE_ARI_CAP_ADDRESS    0x1120032cUL

#define SMN_FUNC0_PCIE2_PCIE_ARI_CAP_ADDRESS    0x1130032cUL

#define SMN_FUNC0_PCIE3_PCIE_ARI_CAP_ADDRESS    0x1140032cUL

#define SMN_FUNC1_PCIE0_PCIE_ARI_CAP_ADDRESS    0x1110132cUL

#define SMN_FUNC1_PCIE1_PCIE_ARI_CAP_ADDRESS    0x1120132cUL

#define SMN_FUNC2_PCIE0_PCIE_ARI_CAP_ADDRESS    0x1110232cUL

#define SMN_FUNC2_PCIE1_PCIE_ARI_CAP_ADDRESS    0x1120232cUL

#define SMN_FUNC3_PCIE0_PCIE_ARI_CAP_ADDRESS    0x1110332cUL

#define SMN_FUNC3_PCIE1_PCIE_ARI_CAP_ADDRESS    0x1120332cUL

#define SMN_FUNC4_PCIE0_PCIE_ARI_CAP_ADDRESS    0x1110432cUL

#define SMN_FUNC4_PCIE1_PCIE_ARI_CAP_ADDRESS    0x1120432cUL

#define SMN_FUNC5_PCIE1_PCIE_ARI_CAP_ADDRESS    0x1120532cUL


/***********************************************************
* Register Name : PCIE_ARI_CNTL
* Register Description :
* ARI Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : For Function 0 only, controls if the device interprets entries in its Function Arbitration table as Function Group numbers.
#define PCIE_ARI_CNTL_ARI_MFVC_FUNC_GROUPS_EN_OFFSET      0
#define PCIE_ARI_CNTL_ARI_MFVC_FUNC_GROUPS_EN_MASK        0x1

// Bitfield Description : For Function 0 only, controls if the device associates bits in its Egress Control Vector with Function Group numbers.
#define PCIE_ARI_CNTL_ARI_ACS_FUNC_GROUPS_EN_OFFSET      1
#define PCIE_ARI_CNTL_ARI_ACS_FUNC_GROUPS_EN_MASK        0x2

// Bitfield Description : 
#define PCIE_ARI_CNTL_Reserved_3_2_OFFSET      2
#define PCIE_ARI_CNTL_Reserved_3_2_MASK        0xc

// Bitfield Description : Controls the Function Group number assigned to this function.
#define PCIE_ARI_CNTL_ARI_FUNCTION_GROUP_OFFSET      4
#define PCIE_ARI_CNTL_ARI_FUNCTION_GROUP_MASK        0x70

// Bitfield Description : 
#define PCIE_ARI_CNTL_Reserved_15_7_OFFSET      7
#define PCIE_ARI_CNTL_Reserved_15_7_MASK        0xff80

typedef union {
  struct {
    UINT16                            ARI_MFVC_FUNC_GROUPS_EN:1;
    UINT16                            ARI_ACS_FUNC_GROUPS_EN:1;
    UINT16                            Reserved_3_2:2;
    UINT16                            ARI_FUNCTION_GROUP:3;
    UINT16                            Reserved_15_7:9;
  } Field;
  UINT16 Value;
} PCIE_ARI_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ARI_CNTL_ADDRESS    0x1110032eUL

#define SMN_FUNC0_PCIE1_PCIE_ARI_CNTL_ADDRESS    0x1120032eUL

#define SMN_FUNC0_PCIE2_PCIE_ARI_CNTL_ADDRESS    0x1130032eUL

#define SMN_FUNC0_PCIE3_PCIE_ARI_CNTL_ADDRESS    0x1140032eUL

#define SMN_FUNC1_PCIE0_PCIE_ARI_CNTL_ADDRESS    0x1110132eUL

#define SMN_FUNC1_PCIE1_PCIE_ARI_CNTL_ADDRESS    0x1120132eUL

#define SMN_FUNC2_PCIE0_PCIE_ARI_CNTL_ADDRESS    0x1110232eUL

#define SMN_FUNC2_PCIE1_PCIE_ARI_CNTL_ADDRESS    0x1120232eUL

#define SMN_FUNC3_PCIE0_PCIE_ARI_CNTL_ADDRESS    0x1110332eUL

#define SMN_FUNC3_PCIE1_PCIE_ARI_CNTL_ADDRESS    0x1120332eUL

#define SMN_FUNC4_PCIE0_PCIE_ARI_CNTL_ADDRESS    0x1110432eUL

#define SMN_FUNC4_PCIE1_PCIE_ARI_CNTL_ADDRESS    0x1120432eUL

#define SMN_FUNC5_PCIE1_PCIE_ARI_CNTL_ADDRESS    0x1120532eUL


/***********************************************************
* Register Name : PCIE_ARI_ENH_CAP_LIST
* Register Description :
* ARI Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_ARI_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_ARI_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_ARI_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_ARI_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_ARI_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_ARI_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_ARI_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11100328UL

#define SMN_FUNC0_PCIE1_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11200328UL

#define SMN_FUNC0_PCIE2_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11300328UL

#define SMN_FUNC0_PCIE3_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11400328UL

#define SMN_FUNC1_PCIE0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11101328UL

#define SMN_FUNC1_PCIE1_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11201328UL

#define SMN_FUNC2_PCIE0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11102328UL

#define SMN_FUNC2_PCIE1_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11202328UL

#define SMN_FUNC3_PCIE0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11103328UL

#define SMN_FUNC3_PCIE1_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11203328UL

#define SMN_FUNC4_PCIE0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11104328UL

#define SMN_FUNC4_PCIE1_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11204328UL

#define SMN_FUNC5_PCIE1_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x11205328UL


/***********************************************************
* Register Name : PCIE_CAP
* Register Description :
* The PCI Express Capabilities register identifies PCI Express device type and associated capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates PCI-SIG defined capability structure version number.
#define PCIE_CAP_VERSION_OFFSET      0
#define PCIE_CAP_VERSION_MASK        0xf

// Bitfield Description : Indicates the type of PCI Express device.
#define PCIE_CAP_DEVICE_TYPE_OFFSET      4
#define PCIE_CAP_DEVICE_TYPE_MASK        0xf0

// Bitfield Description : (Downstream Ports only) Indicates if the Link associated with this Port is connected to a slot.
// For Upstream Ports, this bit is undefined.
#define PCIE_CAP_SLOT_IMPLEMENTED_OFFSET      8
#define PCIE_CAP_SLOT_IMPLEMENTED_MASK        0x100

// Bitfield Description : Indicates the MSI/MSI-X vector used for interrupts generated in association with any of the status bits in the PCI Express capability structure.
#define PCIE_CAP_INT_MESSAGE_NUM_OFFSET      9
#define PCIE_CAP_INT_MESSAGE_NUM_MASK        0x3e00

// Bitfield Description : 
#define PCIE_CAP_Reserved_15_14_OFFSET      14
#define PCIE_CAP_Reserved_15_14_MASK        0xc000

typedef union {
  struct {
    UINT16                            VERSION:4;
    UINT16                            DEVICE_TYPE:4;
    UINT16                            SLOT_IMPLEMENTED:1;
    UINT16                            INT_MESSAGE_NUM:5;
    UINT16                            Reserved_15_14:2;
  } Field;
  UINT16 Value;
} PCIE_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CAP_ADDRESS    0x1110005aUL

#define SMN_FUNC0_PCIE1_PCIE_CAP_ADDRESS    0x1120005aUL

#define SMN_FUNC0_PCIE2_PCIE_CAP_ADDRESS    0x1130005aUL

#define SMN_FUNC0_PCIE3_PCIE_CAP_ADDRESS    0x1140005aUL

#define SMN_FUNC1_PCIE0_PCIE_CAP_ADDRESS    0x1110105aUL

#define SMN_FUNC1_PCIE1_PCIE_CAP_ADDRESS    0x1120105aUL

#define SMN_FUNC2_PCIE0_PCIE_CAP_ADDRESS    0x1110205aUL

#define SMN_FUNC2_PCIE1_PCIE_CAP_ADDRESS    0x1120205aUL

#define SMN_FUNC3_PCIE0_PCIE_CAP_ADDRESS    0x1110305aUL

#define SMN_FUNC3_PCIE1_PCIE_CAP_ADDRESS    0x1120305aUL

#define SMN_FUNC4_PCIE0_PCIE_CAP_ADDRESS    0x1110405aUL

#define SMN_FUNC4_PCIE1_PCIE_CAP_ADDRESS    0x1120405aUL

#define SMN_FUNC5_PCIE1_PCIE_CAP_ADDRESS    0x1120505aUL


/***********************************************************
* Register Name : PCIE_CAP_LIST
* Register Description :
* PCI Express Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the capability.
#define PCIE_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_CAP_LIST_CAP_ID_MASK        0xff

// Bitfield Description : Pointer to the next item in the capabilities list, or 00h if no other items exist.
#define PCIE_CAP_LIST_NEXT_PTR_OFFSET      8
#define PCIE_CAP_LIST_NEXT_PTR_MASK        0xff00

typedef union {
  struct {
    UINT16                            CAP_ID:8;
    UINT16                            NEXT_PTR:8;
  } Field;
  UINT16 Value;
} PCIE_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CAP_LIST_ADDRESS    0x11100058UL

#define SMN_FUNC0_PCIE1_PCIE_CAP_LIST_ADDRESS    0x11200058UL

#define SMN_FUNC0_PCIE2_PCIE_CAP_LIST_ADDRESS    0x11300058UL

#define SMN_FUNC0_PCIE3_PCIE_CAP_LIST_ADDRESS    0x11400058UL

#define SMN_FUNC1_PCIE0_PCIE_CAP_LIST_ADDRESS    0x11101058UL

#define SMN_FUNC1_PCIE1_PCIE_CAP_LIST_ADDRESS    0x11201058UL

#define SMN_FUNC2_PCIE0_PCIE_CAP_LIST_ADDRESS    0x11102058UL

#define SMN_FUNC2_PCIE1_PCIE_CAP_LIST_ADDRESS    0x11202058UL

#define SMN_FUNC3_PCIE0_PCIE_CAP_LIST_ADDRESS    0x11103058UL

#define SMN_FUNC3_PCIE1_PCIE_CAP_LIST_ADDRESS    0x11203058UL

#define SMN_FUNC4_PCIE0_PCIE_CAP_LIST_ADDRESS    0x11104058UL

#define SMN_FUNC4_PCIE1_PCIE_CAP_LIST_ADDRESS    0x11204058UL

#define SMN_FUNC5_PCIE1_PCIE_CAP_LIST_ADDRESS    0x11205058UL


/***********************************************************
* Register Name : PCIE_CCIX_CAP
* Register Description :
* CCIX Transport Capabilities register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the PHY supports CCIX ESM Mode.
#define PCIE_CCIX_CAP_ESM_MODE_SUPPORTED_OFFSET      0
#define PCIE_CCIX_CAP_ESM_MODE_SUPPORTED_MASK        0x1

// Bitfield Description : Indicates the Reach Length capability at ESM Data Rate 1.
#define PCIE_CCIX_CAP_ESM_PHY_REACH_LENGTH_CAPABILITY_OFFSET      1
#define PCIE_CCIX_CAP_ESM_PHY_REACH_LENGTH_CAPABILITY_MASK        0x6

// Bitfield Description : Indicates if the PHY requires re-calibration, when ESM Data Rate0 or ESM Data Rate1 are changed in ESM Control register after initial calibration was done.
#define PCIE_CCIX_CAP_ESM_RECALIBRATION_NEEDED_ON_ESM_DATA_RATE_UPDATE_OFFSET      3
#define PCIE_CCIX_CAP_ESM_RECALIBRATION_NEEDED_ON_ESM_DATA_RATE_UPDATE_MASK        0x8

// Bitfield Description : Indicates the maximum time the PHY needs to complete calibration.
#define PCIE_CCIX_CAP_ESM_CALIBRATION_TIME_OFFSET      4
#define PCIE_CCIX_CAP_ESM_CALIBRATION_TIME_MASK        0x70

// Bitfield Description : 
#define PCIE_CCIX_CAP_Reserved_7_7_OFFSET      7
#define PCIE_CCIX_CAP_Reserved_7_7_MASK        0x80

// Bitfield Description : When Quick Equalization Timeout Select field is non-zero, indicates the minimum equalization timeout in Phase 2/Phase 3 the Port uses for data rates greater than 16.0 GT/s.
#define PCIE_CCIX_CAP_ESM_QUICK_EQUALIZATION_TIMEOUT_OFFSET      8
#define PCIE_CCIX_CAP_ESM_QUICK_EQUALIZATION_TIMEOUT_MASK        0x700

// Bitfield Description : 
#define PCIE_CCIX_CAP_Reserved_15_11_OFFSET      11
#define PCIE_CCIX_CAP_Reserved_15_11_MASK        0xf800

typedef union {
  struct {
    UINT16                            ESM_MODE_SUPPORTED:1;
    UINT16                            ESM_PHY_REACH_LENGTH_CAPABILITY:2;
    UINT16                            ESM_RECALIBRATION_NEEDED_ON_ESM_DATA_RATE_UPDATE:1;
    UINT16                            ESM_CALIBRATION_TIME:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            ESM_QUICK_EQUALIZATION_TIMEOUT:3;
    UINT16                            Reserved_15_11:5;
  } Field;
  UINT16 Value;
} PCIE_CCIX_CAP_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CCIX_CAP_ADDRESS    0x11100492UL

#define SMN_FUNC0_PCIE1_PCIE_CCIX_CAP_ADDRESS    0x11200492UL

#define SMN_FUNC0_PCIE2_PCIE_CCIX_CAP_ADDRESS    0x11300492UL

#define SMN_FUNC0_PCIE3_PCIE_CCIX_CAP_ADDRESS    0x11400492UL

#define SMN_FUNC1_PCIE0_PCIE_CCIX_CAP_ADDRESS    0x11101492UL

#define SMN_FUNC1_PCIE1_PCIE_CCIX_CAP_ADDRESS    0x11201492UL

#define SMN_FUNC2_PCIE0_PCIE_CCIX_CAP_ADDRESS    0x11102492UL

#define SMN_FUNC2_PCIE1_PCIE_CCIX_CAP_ADDRESS    0x11202492UL

#define SMN_FUNC3_PCIE0_PCIE_CCIX_CAP_ADDRESS    0x11103492UL

#define SMN_FUNC3_PCIE1_PCIE_CCIX_CAP_ADDRESS    0x11203492UL

#define SMN_FUNC4_PCIE0_PCIE_CCIX_CAP_ADDRESS    0x11104492UL

#define SMN_FUNC4_PCIE1_PCIE_CCIX_CAP_ADDRESS    0x11204492UL

#define SMN_FUNC5_PCIE1_PCIE_CCIX_CAP_ADDRESS    0x11205492UL


/***********************************************************
* Register Name : PCIE_CCIX_CAP_LIST
* Register Description :
* Designated Vendor-Specific Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_CCIX_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_CCIX_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_CCIX_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_CCIX_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_CCIX_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_CCIX_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_CCIX_CAP_LIST_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CCIX_CAP_LIST_ADDRESS    0x11100488UL

#define SMN_FUNC0_PCIE1_PCIE_CCIX_CAP_LIST_ADDRESS    0x11200488UL

#define SMN_FUNC0_PCIE2_PCIE_CCIX_CAP_LIST_ADDRESS    0x11300488UL

#define SMN_FUNC0_PCIE3_PCIE_CCIX_CAP_LIST_ADDRESS    0x11400488UL

#define SMN_FUNC1_PCIE0_PCIE_CCIX_CAP_LIST_ADDRESS    0x11101488UL

#define SMN_FUNC1_PCIE1_PCIE_CCIX_CAP_LIST_ADDRESS    0x11201488UL

#define SMN_FUNC2_PCIE0_PCIE_CCIX_CAP_LIST_ADDRESS    0x11102488UL

#define SMN_FUNC2_PCIE1_PCIE_CCIX_CAP_LIST_ADDRESS    0x11202488UL

#define SMN_FUNC3_PCIE0_PCIE_CCIX_CAP_LIST_ADDRESS    0x11103488UL

#define SMN_FUNC3_PCIE1_PCIE_CCIX_CAP_LIST_ADDRESS    0x11203488UL

#define SMN_FUNC4_PCIE0_PCIE_CCIX_CAP_LIST_ADDRESS    0x11104488UL

#define SMN_FUNC4_PCIE1_PCIE_CCIX_CAP_LIST_ADDRESS    0x11204488UL

#define SMN_FUNC5_PCIE1_PCIE_CCIX_CAP_LIST_ADDRESS    0x11205488UL


/***********************************************************
* Register Name : PCIE_CCIX_ESM_CNTL
* Register Description :
* ESM Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the ESM data rate used in place of normal 8.0 GT/s. If ESM_COMPLIANCE is 1, this bit is sticky.
#define PCIE_CCIX_ESM_CNTL_ESM_DATA_RATE0_OFFSET      0
#define PCIE_CCIX_ESM_CNTL_ESM_DATA_RATE0_MASK        0x7f

// Bitfield Description : Forces the PHY to perform ESM calibration. Reads always return 0. If ESM_COMPLIANCE is 1, this bit is sticky.
#define PCIE_CCIX_ESM_CNTL_ESM_PERFORM_CALIBRATION_OFFSET      7
#define PCIE_CCIX_ESM_CNTL_ESM_PERFORM_CALIBRATION_MASK        0x80

// Bitfield Description : Controls the ESM data rate used in place of normal 16.0 GT/s. If ESM_COMPLIANCE is 1, this bit is sticky.
#define PCIE_CCIX_ESM_CNTL_ESM_DATA_RATE1_OFFSET      8
#define PCIE_CCIX_ESM_CNTL_ESM_DATA_RATE1_MASK        0x7f00

// Bitfield Description : Writes of 1 triggers the sequence that configures the Port to run in ESM mode. Writes of 0 have no effect. If ESM_COMPLIANCE is 1, this bit is sticky.
#define PCIE_CCIX_ESM_CNTL_ESM_ENABLE_OFFSET      15
#define PCIE_CCIX_ESM_CNTL_ESM_ENABLE_MASK        0x8000

// Bitfield Description : Controls the equalization timeout in Phase 2 for Upstream Port/Downstream Port when the current data rate is greater than 16.0 GT/s. For Upstream Ports with Retimer Present not set, this field has no effect.
#define PCIE_CCIX_ESM_CNTL_ESM_EXTENDED_EQUALIZATION_PHASE2_TIMEOUT_OFFSET      16
#define PCIE_CCIX_ESM_CNTL_ESM_EXTENDED_EQUALIZATION_PHASE2_TIMEOUT_MASK        0x70000

// Bitfield Description : Controls if the LTSSM operates in a mode that supports ESM Electrical Compliance testing. This bit is sticky.
#define PCIE_CCIX_ESM_CNTL_ESM_COMPLIANCE_OFFSET      19
#define PCIE_CCIX_ESM_CNTL_ESM_COMPLIANCE_MASK        0x80000

// Bitfield Description : Controls the equalization timeout in Phase 3 for Downstream Port/Upstream Port when the current data rate is greater than 16.0 GT/s. For Downstream Ports with Retimer Present not set, this field has no effect.
#define PCIE_CCIX_ESM_CNTL_ESM_EXTENDED_EQUALIZATION_PHASE3_TIMEOUT_OFFSET      20
#define PCIE_CCIX_ESM_CNTL_ESM_EXTENDED_EQUALIZATION_PHASE3_TIMEOUT_MASK        0x700000

// Bitfield Description : 
#define PCIE_CCIX_ESM_CNTL_Reserved_23_23_OFFSET      23
#define PCIE_CCIX_ESM_CNTL_Reserved_23_23_MASK        0x800000

// Bitfield Description : Controls the Reach Length Target of the current link. If ESM_COMPLIANCE is 1, this bit is sticky.
#define PCIE_CCIX_ESM_CNTL_LINK_REACH_TARGET_OFFSET      24
#define PCIE_CCIX_ESM_CNTL_LINK_REACH_TARGET_MASK        0x1000000

// Bitfield Description : Controls the register field behavior of ESM_EXTENDED_EQUALIZATION_PHASE2_TIMEOUT field in the Upstream Port and ESM_EXTENDED_EQUALIZATION_PHASE3_TIMEOUT field in the Downstream Port.
#define PCIE_CCIX_ESM_CNTL_RETIMER_PRESENT_OFFSET      25
#define PCIE_CCIX_ESM_CNTL_RETIMER_PRESENT_MASK        0x2000000

// Bitfield Description : If Quick Equalization Timeout field is non-zero, controls the minimum equalization timeout in Phase 2/Phase 3 the Port uses for data rates greater than 16.0 GT/s.
#define PCIE_CCIX_ESM_CNTL_ESM_QUICK_EQUALIZATION_TIMEOUT_SELECT_OFFSET      26
#define PCIE_CCIX_ESM_CNTL_ESM_QUICK_EQUALIZATION_TIMEOUT_SELECT_MASK        0x1c000000

// Bitfield Description : 
#define PCIE_CCIX_ESM_CNTL_Reserved_31_29_OFFSET      29
#define PCIE_CCIX_ESM_CNTL_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            ESM_DATA_RATE0:7;
    UINT32                            ESM_PERFORM_CALIBRATION:1;
    UINT32                            ESM_DATA_RATE1:7;
    UINT32                            ESM_ENABLE:1;
    UINT32                            ESM_EXTENDED_EQUALIZATION_PHASE2_TIMEOUT:3;
    UINT32                            ESM_COMPLIANCE:1;
    UINT32                            ESM_EXTENDED_EQUALIZATION_PHASE3_TIMEOUT:3;
    UINT32                            Reserved_23_23:1;
    UINT32                            LINK_REACH_TARGET:1;
    UINT32                            RETIMER_PRESENT:1;
    UINT32                            ESM_QUICK_EQUALIZATION_TIMEOUT_SELECT:3;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} PCIE_CCIX_ESM_CNTL_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CCIX_ESM_CNTL_ADDRESS    0x111004a0UL

#define SMN_FUNC0_PCIE1_PCIE_CCIX_ESM_CNTL_ADDRESS    0x112004a0UL

#define SMN_FUNC0_PCIE2_PCIE_CCIX_ESM_CNTL_ADDRESS    0x113004a0UL

#define SMN_FUNC0_PCIE3_PCIE_CCIX_ESM_CNTL_ADDRESS    0x114004a0UL

#define SMN_FUNC1_PCIE0_PCIE_CCIX_ESM_CNTL_ADDRESS    0x111014a0UL

#define SMN_FUNC1_PCIE1_PCIE_CCIX_ESM_CNTL_ADDRESS    0x112014a0UL

#define SMN_FUNC2_PCIE0_PCIE_CCIX_ESM_CNTL_ADDRESS    0x111024a0UL

#define SMN_FUNC2_PCIE1_PCIE_CCIX_ESM_CNTL_ADDRESS    0x112024a0UL

#define SMN_FUNC3_PCIE0_PCIE_CCIX_ESM_CNTL_ADDRESS    0x111034a0UL

#define SMN_FUNC3_PCIE1_PCIE_CCIX_ESM_CNTL_ADDRESS    0x112034a0UL

#define SMN_FUNC4_PCIE0_PCIE_CCIX_ESM_CNTL_ADDRESS    0x111044a0UL

#define SMN_FUNC4_PCIE1_PCIE_CCIX_ESM_CNTL_ADDRESS    0x112044a0UL

#define SMN_FUNC5_PCIE1_PCIE_CCIX_ESM_CNTL_ADDRESS    0x112054a0UL


/***********************************************************
* Register Name : PCIE_CCIX_ESM_OPTL_CAP
* Register Description :
* ESM Optional Data Rate Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved.
#define PCIE_CCIX_ESM_OPTL_CAP_Reserved_31_0_OFFSET      0
#define PCIE_CCIX_ESM_OPTL_CAP_Reserved_31_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Reserved_31_0:32;
  } Field;
  UINT32 Value;
} PCIE_CCIX_ESM_OPTL_CAP_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11100498UL

#define SMN_FUNC0_PCIE1_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11200498UL

#define SMN_FUNC0_PCIE2_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11300498UL

#define SMN_FUNC0_PCIE3_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11400498UL

#define SMN_FUNC1_PCIE0_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11101498UL

#define SMN_FUNC1_PCIE1_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11201498UL

#define SMN_FUNC2_PCIE0_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11102498UL

#define SMN_FUNC2_PCIE1_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11202498UL

#define SMN_FUNC3_PCIE0_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11103498UL

#define SMN_FUNC3_PCIE1_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11203498UL

#define SMN_FUNC4_PCIE0_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11104498UL

#define SMN_FUNC4_PCIE1_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11204498UL

#define SMN_FUNC5_PCIE1_PCIE_CCIX_ESM_OPTL_CAP_ADDRESS    0x11205498UL


/***********************************************************
* Register Name : PCIE_CCIX_ESM_REQD_CAP
* Register Description :
* ESM Mandatory Data Rate Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the PHY supports 2.5 GT/S data rate.
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_2P5GT_OFFSET      0
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_2P5GT_MASK        0x1

// Bitfield Description : Indicates if the PHY supports 5.0 GT/S data rate.
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_5GT_OFFSET      1
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_5GT_MASK        0x2

// Bitfield Description : Indicates if the PHY supports 8.0 GT/S data rate.
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_8GT_OFFSET      2
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_8GT_MASK        0x4

// Bitfield Description : 
#define PCIE_CCIX_ESM_REQD_CAP_Reserved_4_3_OFFSET      3
#define PCIE_CCIX_ESM_REQD_CAP_Reserved_4_3_MASK        0x18

// Bitfield Description : Indicates if the PHY supports 16.0 GT/S data rate.
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_16GT_OFFSET      5
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_16GT_MASK        0x20

// Bitfield Description : 
#define PCIE_CCIX_ESM_REQD_CAP_Reserved_8_6_OFFSET      6
#define PCIE_CCIX_ESM_REQD_CAP_Reserved_8_6_MASK        0x1c0

// Bitfield Description : Indicates if the PHY supports 20.0 GT/S data rate.
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_20GT_OFFSET      9
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_20GT_MASK        0x200

// Bitfield Description : 
#define PCIE_CCIX_ESM_REQD_CAP_Reserved_13_10_OFFSET      10
#define PCIE_CCIX_ESM_REQD_CAP_Reserved_13_10_MASK        0x3c00

// Bitfield Description : Indicates if the PHY supports 25.0 GT/S data rate.
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_25GT_OFFSET      14
#define PCIE_CCIX_ESM_REQD_CAP_ESM_SUPPORT_25GT_MASK        0x4000

// Bitfield Description : 
#define PCIE_CCIX_ESM_REQD_CAP_Reserved_31_15_OFFSET      15
#define PCIE_CCIX_ESM_REQD_CAP_Reserved_31_15_MASK        0xffff8000

typedef union {
  struct {
    UINT32                            ESM_SUPPORT_2P5GT:1;
    UINT32                            ESM_SUPPORT_5GT:1;
    UINT32                            ESM_SUPPORT_8GT:1;
    UINT32                            Reserved_4_3:2;
    UINT32                            ESM_SUPPORT_16GT:1;
    UINT32                            Reserved_8_6:3;
    UINT32                            ESM_SUPPORT_20GT:1;
    UINT32                            Reserved_13_10:4;
    UINT32                            ESM_SUPPORT_25GT:1;
    UINT32                            Reserved_31_15:17;
  } Field;
  UINT32 Value;
} PCIE_CCIX_ESM_REQD_CAP_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11100494UL

#define SMN_FUNC0_PCIE1_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11200494UL

#define SMN_FUNC0_PCIE2_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11300494UL

#define SMN_FUNC0_PCIE3_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11400494UL

#define SMN_FUNC1_PCIE0_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11101494UL

#define SMN_FUNC1_PCIE1_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11201494UL

#define SMN_FUNC2_PCIE0_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11102494UL

#define SMN_FUNC2_PCIE1_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11202494UL

#define SMN_FUNC3_PCIE0_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11103494UL

#define SMN_FUNC3_PCIE1_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11203494UL

#define SMN_FUNC4_PCIE0_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11104494UL

#define SMN_FUNC4_PCIE1_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11204494UL

#define SMN_FUNC5_PCIE1_PCIE_CCIX_ESM_REQD_CAP_ADDRESS    0x11205494UL


/***********************************************************
* Register Name : PCIE_CCIX_ESM_STATUS
* Register Description :
* ESM Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the negotiated ESM data rate of the Link.
#define PCIE_CCIX_ESM_STATUS_ESM_CURRENT_DATA_RATE_OFFSET      0
#define PCIE_CCIX_ESM_STATUS_ESM_CURRENT_DATA_RATE_MASK        0x7f

// Bitfield Description : Indicates if PHY calibration is complete.
#define PCIE_CCIX_ESM_STATUS_ESM_CALIBRATION_COMPLETE_OFFSET      7
#define PCIE_CCIX_ESM_STATUS_ESM_CALIBRATION_COMPLETE_MASK        0x80

// Bitfield Description : 
#define PCIE_CCIX_ESM_STATUS_Reserved_31_8_OFFSET      8
#define PCIE_CCIX_ESM_STATUS_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            ESM_CURRENT_DATA_RATE:7;
    UINT32                            ESM_CALIBRATION_COMPLETE:1;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} PCIE_CCIX_ESM_STATUS_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1110049cUL

#define SMN_FUNC0_PCIE1_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1120049cUL

#define SMN_FUNC0_PCIE2_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1130049cUL

#define SMN_FUNC0_PCIE3_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1140049cUL

#define SMN_FUNC1_PCIE0_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1110149cUL

#define SMN_FUNC1_PCIE1_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1120149cUL

#define SMN_FUNC2_PCIE0_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1110249cUL

#define SMN_FUNC2_PCIE1_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1120249cUL

#define SMN_FUNC3_PCIE0_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1110349cUL

#define SMN_FUNC3_PCIE1_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1120349cUL

#define SMN_FUNC4_PCIE0_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1110449cUL

#define SMN_FUNC4_PCIE1_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1120449cUL

#define SMN_FUNC5_PCIE1_PCIE_CCIX_ESM_STATUS_ADDRESS    0x1120549cUL


/***********************************************************
* Register Name : PCIE_CCIX_HEADER_1
* Register Description :
* Designated Vendor-Specific Header 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor-defined ID number. This value is CCIX.
#define PCIE_CCIX_HEADER_1_CCIX_VENDOR_ID_OFFSET      0
#define PCIE_CCIX_HEADER_1_CCIX_VENDOR_ID_MASK        0xffff

// Bitfield Description : Vendor-defined revision number.
#define PCIE_CCIX_HEADER_1_CCIX_CAP_REV_OFFSET      16
#define PCIE_CCIX_HEADER_1_CCIX_CAP_REV_MASK        0xf0000

// Bitfield Description : Number of bytes in the entire DVSEC structure.
#define PCIE_CCIX_HEADER_1_CCIX_CAP_LEN_OFFSET      20
#define PCIE_CCIX_HEADER_1_CCIX_CAP_LEN_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CCIX_VENDOR_ID:16;
    UINT32                            CCIX_CAP_REV:4;
    UINT32                            CCIX_CAP_LEN:12;
  } Field;
  UINT32 Value;
} PCIE_CCIX_HEADER_1_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CCIX_HEADER_1_ADDRESS    0x1110048cUL

#define SMN_FUNC0_PCIE1_PCIE_CCIX_HEADER_1_ADDRESS    0x1120048cUL

#define SMN_FUNC0_PCIE2_PCIE_CCIX_HEADER_1_ADDRESS    0x1130048cUL

#define SMN_FUNC0_PCIE3_PCIE_CCIX_HEADER_1_ADDRESS    0x1140048cUL

#define SMN_FUNC1_PCIE0_PCIE_CCIX_HEADER_1_ADDRESS    0x1110148cUL

#define SMN_FUNC1_PCIE1_PCIE_CCIX_HEADER_1_ADDRESS    0x1120148cUL

#define SMN_FUNC2_PCIE0_PCIE_CCIX_HEADER_1_ADDRESS    0x1110248cUL

#define SMN_FUNC2_PCIE1_PCIE_CCIX_HEADER_1_ADDRESS    0x1120248cUL

#define SMN_FUNC3_PCIE0_PCIE_CCIX_HEADER_1_ADDRESS    0x1110348cUL

#define SMN_FUNC3_PCIE1_PCIE_CCIX_HEADER_1_ADDRESS    0x1120348cUL

#define SMN_FUNC4_PCIE0_PCIE_CCIX_HEADER_1_ADDRESS    0x1110448cUL

#define SMN_FUNC4_PCIE1_PCIE_CCIX_HEADER_1_ADDRESS    0x1120448cUL

#define SMN_FUNC5_PCIE1_PCIE_CCIX_HEADER_1_ADDRESS    0x1120548cUL


/***********************************************************
* Register Name : PCIE_CCIX_HEADER_2
* Register Description :
* Designated Vendor-Specific Header 2 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor-defined ID number that indicates the nature and format of the DVSEC structure. This value is CCIX Transport capability structure.
#define PCIE_CCIX_HEADER_2_CAP_ID_OFFSET      0
#define PCIE_CCIX_HEADER_2_CAP_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            CAP_ID:16;
  } Field;
  UINT16 Value;
} PCIE_CCIX_HEADER_2_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CCIX_HEADER_2_ADDRESS    0x11100490UL

#define SMN_FUNC0_PCIE1_PCIE_CCIX_HEADER_2_ADDRESS    0x11200490UL

#define SMN_FUNC0_PCIE2_PCIE_CCIX_HEADER_2_ADDRESS    0x11300490UL

#define SMN_FUNC0_PCIE3_PCIE_CCIX_HEADER_2_ADDRESS    0x11400490UL

#define SMN_FUNC1_PCIE0_PCIE_CCIX_HEADER_2_ADDRESS    0x11101490UL

#define SMN_FUNC1_PCIE1_PCIE_CCIX_HEADER_2_ADDRESS    0x11201490UL

#define SMN_FUNC2_PCIE0_PCIE_CCIX_HEADER_2_ADDRESS    0x11102490UL

#define SMN_FUNC2_PCIE1_PCIE_CCIX_HEADER_2_ADDRESS    0x11202490UL

#define SMN_FUNC3_PCIE0_PCIE_CCIX_HEADER_2_ADDRESS    0x11103490UL

#define SMN_FUNC3_PCIE1_PCIE_CCIX_HEADER_2_ADDRESS    0x11203490UL

#define SMN_FUNC4_PCIE0_PCIE_CCIX_HEADER_2_ADDRESS    0x11104490UL

#define SMN_FUNC4_PCIE1_PCIE_CCIX_HEADER_2_ADDRESS    0x11204490UL

#define SMN_FUNC5_PCIE1_PCIE_CCIX_HEADER_2_ADDRESS    0x11205490UL


/***********************************************************
* Register Name : PCIE_CCIX_TRANS_CAP
* Register Description :
* Transaction Layer Capabilities register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port supports the Optimized TLP format for CCIX.
#define PCIE_CCIX_TRANS_CAP_CCIX_OPTIMIZED_TLP_FORMAT_SUPPORT_OFFSET      0
#define PCIE_CCIX_TRANS_CAP_CCIX_OPTIMIZED_TLP_FORMAT_SUPPORT_MASK        0x1

// Bitfield Description : 
#define PCIE_CCIX_TRANS_CAP_Reserved_31_1_OFFSET      1
#define PCIE_CCIX_TRANS_CAP_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            CCIX_OPTIMIZED_TLP_FORMAT_SUPPORT:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} PCIE_CCIX_TRANS_CAP_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CCIX_TRANS_CAP_ADDRESS    0x111004c4UL

#define SMN_FUNC0_PCIE1_PCIE_CCIX_TRANS_CAP_ADDRESS    0x112004c4UL

#define SMN_FUNC0_PCIE2_PCIE_CCIX_TRANS_CAP_ADDRESS    0x113004c4UL

#define SMN_FUNC0_PCIE3_PCIE_CCIX_TRANS_CAP_ADDRESS    0x114004c4UL

#define SMN_FUNC1_PCIE0_PCIE_CCIX_TRANS_CAP_ADDRESS    0x111014c4UL

#define SMN_FUNC1_PCIE1_PCIE_CCIX_TRANS_CAP_ADDRESS    0x112014c4UL

#define SMN_FUNC2_PCIE0_PCIE_CCIX_TRANS_CAP_ADDRESS    0x111024c4UL

#define SMN_FUNC2_PCIE1_PCIE_CCIX_TRANS_CAP_ADDRESS    0x112024c4UL

#define SMN_FUNC3_PCIE0_PCIE_CCIX_TRANS_CAP_ADDRESS    0x111034c4UL

#define SMN_FUNC3_PCIE1_PCIE_CCIX_TRANS_CAP_ADDRESS    0x112034c4UL

#define SMN_FUNC4_PCIE0_PCIE_CCIX_TRANS_CAP_ADDRESS    0x111044c4UL

#define SMN_FUNC4_PCIE1_PCIE_CCIX_TRANS_CAP_ADDRESS    0x112044c4UL

#define SMN_FUNC5_PCIE1_PCIE_CCIX_TRANS_CAP_ADDRESS    0x112054c4UL


/***********************************************************
* Register Name : PCIE_CCIX_TRANS_CNTL
* Register Description :
* Transaction Layer Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the Port is permitted to use Optimized TLP format for CCIX.
#define PCIE_CCIX_TRANS_CNTL_CCIX_OPTIMIZED_TLP_FORMAT_ENABLE_OFFSET      0
#define PCIE_CCIX_TRANS_CNTL_CCIX_OPTIMIZED_TLP_FORMAT_ENABLE_MASK        0x1

// Bitfield Description : Controls if the Port is permitted to use PCIe Compatible TLP format for CCIX.
#define PCIE_CCIX_TRANS_CNTL_CCIX_PCIE_COMPATIBLE_TLP_ENABLE_OFFSET      1
#define PCIE_CCIX_TRANS_CNTL_CCIX_PCIE_COMPATIBLE_TLP_ENABLE_MASK        0x2

// Bitfield Description : 
#define PCIE_CCIX_TRANS_CNTL_Reserved_31_2_OFFSET      2
#define PCIE_CCIX_TRANS_CNTL_Reserved_31_2_MASK        0xfffffffc

typedef union {
  struct {
    UINT32                            CCIX_OPTIMIZED_TLP_FORMAT_ENABLE:1;
    UINT32                            CCIX_PCIE_COMPATIBLE_TLP_ENABLE:1;
    UINT32                            Reserved_31_2:30;
  } Field;
  UINT32 Value;
} PCIE_CCIX_TRANS_CNTL_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x111004c8UL

#define SMN_FUNC0_PCIE1_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x112004c8UL

#define SMN_FUNC0_PCIE2_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x113004c8UL

#define SMN_FUNC0_PCIE3_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x114004c8UL

#define SMN_FUNC1_PCIE0_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x111014c8UL

#define SMN_FUNC1_PCIE1_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x112014c8UL

#define SMN_FUNC2_PCIE0_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x111024c8UL

#define SMN_FUNC2_PCIE1_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x112024c8UL

#define SMN_FUNC3_PCIE0_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x111034c8UL

#define SMN_FUNC3_PCIE1_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x112034c8UL

#define SMN_FUNC4_PCIE0_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x111044c8UL

#define SMN_FUNC4_PCIE1_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x112044c8UL

#define SMN_FUNC5_PCIE1_PCIE_CCIX_TRANS_CNTL_ADDRESS    0x112054c8UL


/***********************************************************
* Register Name : PCIE_CORR_ERR_MASK
* Register Description :
* The Correctable Error Mask register controls reporting of individual correctable errors by a function.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the function can report a Receiver Error. This bit is sticky.
#define PCIE_CORR_ERR_MASK_RCV_ERR_MASK_OFFSET      0
#define PCIE_CORR_ERR_MASK_RCV_ERR_MASK_MASK        0x1

// Bitfield Description : 
#define PCIE_CORR_ERR_MASK_Reserved_5_1_OFFSET      1
#define PCIE_CORR_ERR_MASK_Reserved_5_1_MASK        0x3e

// Bitfield Description : Controls if the function can report a Bad TLP Error. This bit is sticky.
#define PCIE_CORR_ERR_MASK_BAD_TLP_MASK_OFFSET      6
#define PCIE_CORR_ERR_MASK_BAD_TLP_MASK_MASK        0x40

// Bitfield Description : Controls if the function can report a Bad DLLP Error. This bit is sticky.
#define PCIE_CORR_ERR_MASK_BAD_DLLP_MASK_OFFSET      7
#define PCIE_CORR_ERR_MASK_BAD_DLLP_MASK_MASK        0x80

// Bitfield Description : Controls if the function can report a REPLAY_NUM Rollover Error. This bit is sticky.
#define PCIE_CORR_ERR_MASK_REPLAY_NUM_ROLLOVER_MASK_OFFSET      8
#define PCIE_CORR_ERR_MASK_REPLAY_NUM_ROLLOVER_MASK_MASK        0x100

// Bitfield Description : 
#define PCIE_CORR_ERR_MASK_Reserved_11_9_OFFSET      9
#define PCIE_CORR_ERR_MASK_Reserved_11_9_MASK        0xe00

// Bitfield Description : Controls if the function can report a Replay Timer Timeout Error. This bit is sticky.
#define PCIE_CORR_ERR_MASK_REPLAY_TIMER_TIMEOUT_MASK_OFFSET      12
#define PCIE_CORR_ERR_MASK_REPLAY_TIMER_TIMEOUT_MASK_MASK        0x1000

// Bitfield Description : Controls if the function can report an Advisory Non-Fatal Error. This bit is sticky.
#define PCIE_CORR_ERR_MASK_ADVISORY_NONFATAL_ERR_MASK_OFFSET      13
#define PCIE_CORR_ERR_MASK_ADVISORY_NONFATAL_ERR_MASK_MASK        0x2000

// Bitfield Description : Controls if the function can report a Corrected Internal Error. This bit is sticky.
#define PCIE_CORR_ERR_MASK_CORR_INT_ERR_MASK_OFFSET      14
#define PCIE_CORR_ERR_MASK_CORR_INT_ERR_MASK_MASK        0x4000

// Bitfield Description : Controls if the function can report a Header Log Overflow Error.
#define PCIE_CORR_ERR_MASK_HDR_LOG_OVFL_MASK_OFFSET      15
#define PCIE_CORR_ERR_MASK_HDR_LOG_OVFL_MASK_MASK        0x8000

// Bitfield Description : 
#define PCIE_CORR_ERR_MASK_Reserved_31_16_OFFSET      16
#define PCIE_CORR_ERR_MASK_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            RCV_ERR_MASK:1;
    UINT32                            Reserved_5_1:5;
    UINT32                            BAD_TLP_MASK:1;
    UINT32                            BAD_DLLP_MASK:1;
    UINT32                            REPLAY_NUM_ROLLOVER_MASK:1;
    UINT32                            Reserved_11_9:3;
    UINT32                            REPLAY_TIMER_TIMEOUT_MASK:1;
    UINT32                            ADVISORY_NONFATAL_ERR_MASK:1;
    UINT32                            CORR_INT_ERR_MASK:1;
    UINT32                            HDR_LOG_OVFL_MASK:1;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} PCIE_CORR_ERR_MASK_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CORR_ERR_MASK_ADDRESS    0x11100164UL

#define SMN_FUNC0_PCIE1_PCIE_CORR_ERR_MASK_ADDRESS    0x11200164UL

#define SMN_FUNC0_PCIE2_PCIE_CORR_ERR_MASK_ADDRESS    0x11300164UL

#define SMN_FUNC0_PCIE3_PCIE_CORR_ERR_MASK_ADDRESS    0x11400164UL

#define SMN_FUNC1_PCIE0_PCIE_CORR_ERR_MASK_ADDRESS    0x11101164UL

#define SMN_FUNC1_PCIE1_PCIE_CORR_ERR_MASK_ADDRESS    0x11201164UL

#define SMN_FUNC2_PCIE0_PCIE_CORR_ERR_MASK_ADDRESS    0x11102164UL

#define SMN_FUNC2_PCIE1_PCIE_CORR_ERR_MASK_ADDRESS    0x11202164UL

#define SMN_FUNC3_PCIE0_PCIE_CORR_ERR_MASK_ADDRESS    0x11103164UL

#define SMN_FUNC3_PCIE1_PCIE_CORR_ERR_MASK_ADDRESS    0x11203164UL

#define SMN_FUNC4_PCIE0_PCIE_CORR_ERR_MASK_ADDRESS    0x11104164UL

#define SMN_FUNC4_PCIE1_PCIE_CORR_ERR_MASK_ADDRESS    0x11204164UL

#define SMN_FUNC5_PCIE1_PCIE_CORR_ERR_MASK_ADDRESS    0x11205164UL


/***********************************************************
* Register Name : PCIE_CORR_ERR_STATUS
* Register Description :
* The Correctable Error Status register reports error status of individual correctable errors in a function.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the function detects a Receiver Error. This bit is sticky.
#define PCIE_CORR_ERR_STATUS_RCV_ERR_STATUS_OFFSET      0
#define PCIE_CORR_ERR_STATUS_RCV_ERR_STATUS_MASK        0x1

// Bitfield Description : 
#define PCIE_CORR_ERR_STATUS_Reserved_5_1_OFFSET      1
#define PCIE_CORR_ERR_STATUS_Reserved_5_1_MASK        0x3e

// Bitfield Description : Indicates if the function detects a Bad TLP Error. This bit is sticky.
#define PCIE_CORR_ERR_STATUS_BAD_TLP_STATUS_OFFSET      6
#define PCIE_CORR_ERR_STATUS_BAD_TLP_STATUS_MASK        0x40

// Bitfield Description : Indicates if the function detects a Bad DLLP Error. This bit is sticky.
#define PCIE_CORR_ERR_STATUS_BAD_DLLP_STATUS_OFFSET      7
#define PCIE_CORR_ERR_STATUS_BAD_DLLP_STATUS_MASK        0x80

// Bitfield Description : Indicates if the function detects a REPLAY_NUM Rollover Error. This bit is sticky.
#define PCIE_CORR_ERR_STATUS_REPLAY_NUM_ROLLOVER_STATUS_OFFSET      8
#define PCIE_CORR_ERR_STATUS_REPLAY_NUM_ROLLOVER_STATUS_MASK        0x100

// Bitfield Description : 
#define PCIE_CORR_ERR_STATUS_Reserved_11_9_OFFSET      9
#define PCIE_CORR_ERR_STATUS_Reserved_11_9_MASK        0xe00

// Bitfield Description : Indicates if the function detects a Replay Timer Timeout Error. This bit is sticky.
#define PCIE_CORR_ERR_STATUS_REPLAY_TIMER_TIMEOUT_STATUS_OFFSET      12
#define PCIE_CORR_ERR_STATUS_REPLAY_TIMER_TIMEOUT_STATUS_MASK        0x1000

// Bitfield Description : Indicates if the function detects an Advisory Non-Fatal Error. This bit is sticky.
#define PCIE_CORR_ERR_STATUS_ADVISORY_NONFATAL_ERR_STATUS_OFFSET      13
#define PCIE_CORR_ERR_STATUS_ADVISORY_NONFATAL_ERR_STATUS_MASK        0x2000

// Bitfield Description : Indicates if the function detects a Corrected Internal Error. This bit is sticky.
#define PCIE_CORR_ERR_STATUS_CORR_INT_ERR_STATUS_OFFSET      14
#define PCIE_CORR_ERR_STATUS_CORR_INT_ERR_STATUS_MASK        0x4000

// Bitfield Description : Indicates if the function detects a Header Log Overflow Error.
#define PCIE_CORR_ERR_STATUS_HDR_LOG_OVFL_STATUS_OFFSET      15
#define PCIE_CORR_ERR_STATUS_HDR_LOG_OVFL_STATUS_MASK        0x8000

// Bitfield Description : 
#define PCIE_CORR_ERR_STATUS_Reserved_31_16_OFFSET      16
#define PCIE_CORR_ERR_STATUS_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            RCV_ERR_STATUS:1;
    UINT32                            Reserved_5_1:5;
    UINT32                            BAD_TLP_STATUS:1;
    UINT32                            BAD_DLLP_STATUS:1;
    UINT32                            REPLAY_NUM_ROLLOVER_STATUS:1;
    UINT32                            Reserved_11_9:3;
    UINT32                            REPLAY_TIMER_TIMEOUT_STATUS:1;
    UINT32                            ADVISORY_NONFATAL_ERR_STATUS:1;
    UINT32                            CORR_INT_ERR_STATUS:1;
    UINT32                            HDR_LOG_OVFL_STATUS:1;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} PCIE_CORR_ERR_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_CORR_ERR_STATUS_ADDRESS    0x11100160UL

#define SMN_FUNC0_PCIE1_PCIE_CORR_ERR_STATUS_ADDRESS    0x11200160UL

#define SMN_FUNC0_PCIE2_PCIE_CORR_ERR_STATUS_ADDRESS    0x11300160UL

#define SMN_FUNC0_PCIE3_PCIE_CORR_ERR_STATUS_ADDRESS    0x11400160UL

#define SMN_FUNC1_PCIE0_PCIE_CORR_ERR_STATUS_ADDRESS    0x11101160UL

#define SMN_FUNC1_PCIE1_PCIE_CORR_ERR_STATUS_ADDRESS    0x11201160UL

#define SMN_FUNC2_PCIE0_PCIE_CORR_ERR_STATUS_ADDRESS    0x11102160UL

#define SMN_FUNC2_PCIE1_PCIE_CORR_ERR_STATUS_ADDRESS    0x11202160UL

#define SMN_FUNC3_PCIE0_PCIE_CORR_ERR_STATUS_ADDRESS    0x11103160UL

#define SMN_FUNC3_PCIE1_PCIE_CORR_ERR_STATUS_ADDRESS    0x11203160UL

#define SMN_FUNC4_PCIE0_PCIE_CORR_ERR_STATUS_ADDRESS    0x11104160UL

#define SMN_FUNC4_PCIE1_PCIE_CORR_ERR_STATUS_ADDRESS    0x11204160UL

#define SMN_FUNC5_PCIE1_PCIE_CORR_ERR_STATUS_ADDRESS    0x11205160UL


/***********************************************************
* Register Name : PCIE_DEV_SERIAL_NUM_DW1
* Register Description :
* PCI Express Device Serial Number (1st DW) register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 32-bits of IEEE defined 64-bit extended unique identifier. (EUI-64).
#define PCIE_DEV_SERIAL_NUM_DW1_SERIAL_NUMBER_LO_OFFSET      0
#define PCIE_DEV_SERIAL_NUM_DW1_SERIAL_NUMBER_LO_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SERIAL_NUMBER_LO:32;
  } Field;
  UINT32 Value;
} PCIE_DEV_SERIAL_NUM_DW1_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11100144UL

#define SMN_FUNC0_PCIE1_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11200144UL

#define SMN_FUNC0_PCIE2_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11300144UL

#define SMN_FUNC0_PCIE3_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11400144UL

#define SMN_FUNC1_PCIE0_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11101144UL

#define SMN_FUNC1_PCIE1_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11201144UL

#define SMN_FUNC2_PCIE0_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11102144UL

#define SMN_FUNC2_PCIE1_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11202144UL

#define SMN_FUNC3_PCIE0_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11103144UL

#define SMN_FUNC3_PCIE1_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11203144UL

#define SMN_FUNC4_PCIE0_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11104144UL

#define SMN_FUNC4_PCIE1_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11204144UL

#define SMN_FUNC5_PCIE1_PCIE_DEV_SERIAL_NUM_DW1_ADDRESS    0x11205144UL


/***********************************************************
* Register Name : PCIE_DEV_SERIAL_NUM_DW2
* Register Description :
* PCI Express Device Serial Number (2nd DW) register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32-bits of IEEE defined 64-bit extended unique identifier. (EUI-64).
#define PCIE_DEV_SERIAL_NUM_DW2_SERIAL_NUMBER_HI_OFFSET      0
#define PCIE_DEV_SERIAL_NUM_DW2_SERIAL_NUMBER_HI_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SERIAL_NUMBER_HI:32;
  } Field;
  UINT32 Value;
} PCIE_DEV_SERIAL_NUM_DW2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11100148UL

#define SMN_FUNC0_PCIE1_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11200148UL

#define SMN_FUNC0_PCIE2_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11300148UL

#define SMN_FUNC0_PCIE3_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11400148UL

#define SMN_FUNC1_PCIE0_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11101148UL

#define SMN_FUNC1_PCIE1_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11201148UL

#define SMN_FUNC2_PCIE0_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11102148UL

#define SMN_FUNC2_PCIE1_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11202148UL

#define SMN_FUNC3_PCIE0_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11103148UL

#define SMN_FUNC3_PCIE1_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11203148UL

#define SMN_FUNC4_PCIE0_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11104148UL

#define SMN_FUNC4_PCIE1_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11204148UL

#define SMN_FUNC5_PCIE1_PCIE_DEV_SERIAL_NUM_DW2_ADDRESS    0x11205148UL


/***********************************************************
* Register Name : PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST
* Register Description :
* Device Serial Number Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11100140UL

#define SMN_FUNC0_PCIE1_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11200140UL

#define SMN_FUNC0_PCIE2_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11300140UL

#define SMN_FUNC0_PCIE3_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11400140UL

#define SMN_FUNC1_PCIE0_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11101140UL

#define SMN_FUNC1_PCIE1_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11201140UL

#define SMN_FUNC2_PCIE0_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11102140UL

#define SMN_FUNC2_PCIE1_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11202140UL

#define SMN_FUNC3_PCIE0_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11103140UL

#define SMN_FUNC3_PCIE1_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11203140UL

#define SMN_FUNC4_PCIE0_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11104140UL

#define SMN_FUNC4_PCIE1_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11204140UL

#define SMN_FUNC5_PCIE1_PCIE_DEV_SERIAL_NUM_ENH_CAP_LIST_ADDRESS    0x11205140UL


/***********************************************************
* Register Name : PCIE_DLF_ENH_CAP_LIST
* Register Description :
* Data Link Feature Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_DLF_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_DLF_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_DLF_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_DLF_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_DLF_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_DLF_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_DLF_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11100400UL

#define SMN_FUNC0_PCIE1_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11200400UL

#define SMN_FUNC0_PCIE2_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11300400UL

#define SMN_FUNC0_PCIE3_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11400400UL

#define SMN_FUNC1_PCIE0_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11101400UL

#define SMN_FUNC1_PCIE1_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11201400UL

#define SMN_FUNC2_PCIE0_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11102400UL

#define SMN_FUNC2_PCIE1_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11202400UL

#define SMN_FUNC3_PCIE0_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11103400UL

#define SMN_FUNC3_PCIE1_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11203400UL

#define SMN_FUNC4_PCIE0_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11104400UL

#define SMN_FUNC4_PCIE1_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11204400UL

#define SMN_FUNC5_PCIE1_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x11205400UL


/***********************************************************
* Register Name : PCIE_DPC_CAP_LIST
* Register Description :
* DPC Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the MSI/MSI-X vector used for interrupts generated in association with any of the status bits in the DPC capability structure.
#define PCIE_DPC_CAP_LIST_DPC_INTR_MSG_NUM_OFFSET      0
#define PCIE_DPC_CAP_LIST_DPC_INTR_MSG_NUM_MASK        0x1f

// Bitfield Description : Indicates if the Port supports a defined set of DPC Extensions that are specific to Root Ports.
#define PCIE_DPC_CAP_LIST_RP_EXTENSIONS_FOR_DPC_OFFSET      5
#define PCIE_DPC_CAP_LIST_RP_EXTENSIONS_FOR_DPC_MASK        0x20

// Bitfield Description : Indicates if the Port supports the ability to block the transmission of a poisoned TLP from its Egress Port.
#define PCIE_DPC_CAP_LIST_POISONED_TLP_EGRESS_BLOCKING_SUPPORTED_OFFSET      6
#define PCIE_DPC_CAP_LIST_POISONED_TLP_EGRESS_BLOCKING_SUPPORTED_MASK        0x40

// Bitfield Description : Indicates if the Port supports the ability for software to trigger DPC.
#define PCIE_DPC_CAP_LIST_DPC_SOFTWARE_TRIGGERING_SUPPORTED_OFFSET      7
#define PCIE_DPC_CAP_LIST_DPC_SOFTWARE_TRIGGERING_SUPPORTED_MASK        0x80

// Bitfield Description : Indicates how many DWORDs are allocated for the RP PIO log registers. When RP Extensions for DPC bit is set, this value ranges from 4 (RP PIO Header Log only) to 9 (RP PIO Header Log, RP PIO ImpSpec Log, 4 DWs of RP PIO TLP Prefix Log).
#define PCIE_DPC_CAP_LIST_RP_PIO_LOG_SIZE_OFFSET      8
#define PCIE_DPC_CAP_LIST_RP_PIO_LOG_SIZE_MASK        0xf00

// Bitfield Description : Indicates if the Port supports the ability to signal with ERR_COR when the Link transitions to the DL_Active state.
#define PCIE_DPC_CAP_LIST_DL_ACTIVE_ERR_COR_SIGNALING_SUPPORTED_OFFSET      12
#define PCIE_DPC_CAP_LIST_DL_ACTIVE_ERR_COR_SIGNALING_SUPPORTED_MASK        0x1000

// Bitfield Description : 
#define PCIE_DPC_CAP_LIST_Reserved_15_13_OFFSET      13
#define PCIE_DPC_CAP_LIST_Reserved_15_13_MASK        0xe000

typedef union {
  struct {
    UINT16                            DPC_INTR_MSG_NUM:5;
    UINT16                            RP_EXTENSIONS_FOR_DPC:1;
    UINT16                            POISONED_TLP_EGRESS_BLOCKING_SUPPORTED:1;
    UINT16                            DPC_SOFTWARE_TRIGGERING_SUPPORTED:1;
    UINT16                            RP_PIO_LOG_SIZE:4;
    UINT16                            DL_ACTIVE_ERR_COR_SIGNALING_SUPPORTED:1;
    UINT16                            Reserved_15_13:3;
  } Field;
  UINT16 Value;
} PCIE_DPC_CAP_LIST_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_DPC_CAP_LIST_ADDRESS    0x11100384UL

#define SMN_FUNC0_PCIE1_PCIE_DPC_CAP_LIST_ADDRESS    0x11200384UL

#define SMN_FUNC0_PCIE2_PCIE_DPC_CAP_LIST_ADDRESS    0x11300384UL

#define SMN_FUNC0_PCIE3_PCIE_DPC_CAP_LIST_ADDRESS    0x11400384UL

#define SMN_FUNC1_PCIE0_PCIE_DPC_CAP_LIST_ADDRESS    0x11101384UL

#define SMN_FUNC1_PCIE1_PCIE_DPC_CAP_LIST_ADDRESS    0x11201384UL

#define SMN_FUNC2_PCIE0_PCIE_DPC_CAP_LIST_ADDRESS    0x11102384UL

#define SMN_FUNC2_PCIE1_PCIE_DPC_CAP_LIST_ADDRESS    0x11202384UL

#define SMN_FUNC3_PCIE0_PCIE_DPC_CAP_LIST_ADDRESS    0x11103384UL

#define SMN_FUNC3_PCIE1_PCIE_DPC_CAP_LIST_ADDRESS    0x11203384UL

#define SMN_FUNC4_PCIE0_PCIE_DPC_CAP_LIST_ADDRESS    0x11104384UL

#define SMN_FUNC4_PCIE1_PCIE_DPC_CAP_LIST_ADDRESS    0x11204384UL

#define SMN_FUNC5_PCIE1_PCIE_DPC_CAP_LIST_ADDRESS    0x11205384UL


/***********************************************************
* Register Name : PCIE_DPC_CNTL
* Register Description :
* DPC Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if DPC is enabled and the conditions that cause DPC to be triggered. Trigger 1 occurs when the Downstream Port detects an unmasked uncorrectable error or when the Downstream Port receives an ERR_FATAL Message. Trigger 2 occurs when the Downstream Port detects an unmasked uncorrectable error or when the Downstream Port receives an ERR_NONFATAL or ERR_FATAL Message.
#define PCIE_DPC_CNTL_DPC_TRIGGER_ENABLE_OFFSET      0
#define PCIE_DPC_CNTL_DPC_TRIGGER_ENABLE_MASK        0x3

// Bitfield Description : Controls the Completion Status for Completions formed during DPC.
#define PCIE_DPC_CNTL_DPC_COMPLETION_CONTROL_OFFSET      2
#define PCIE_DPC_CNTL_DPC_COMPLETION_CONTROL_MASK        0x4

// Bitfield Description : Controls the generation of an interrupt when DPC has been triggered.
#define PCIE_DPC_CNTL_DPC_INTERRUPT_ENABLE_OFFSET      3
#define PCIE_DPC_CNTL_DPC_INTERRUPT_ENABLE_MASK        0x8

// Bitfield Description : Controls if the Downstream Port signals with ERR_COR when DPC has been triggered.
#define PCIE_DPC_CNTL_DPC_ERR_COR_ENABLE_OFFSET      4
#define PCIE_DPC_CNTL_DPC_ERR_COR_ENABLE_MASK        0x10

// Bitfield Description : When Poisoned TLP Egress Blocking Supported bit is set, controls if the Egress Port blocks the transmission of poisoned TLPs.
#define PCIE_DPC_CNTL_POISONED_TLP_EGRESS_BLOCKING_ENABLE_OFFSET      5
#define PCIE_DPC_CNTL_POISONED_TLP_EGRESS_BLOCKING_ENABLE_MASK        0x20

// Bitfield Description : When DPC Software Triggering Supported bit is set, and if DPC Trigger Enable field is 1 or 2, or is being written to 1 or 2 in the same register update that writes this bit, and DPC Trigger Status bit is clear, writes of 1 to this bit trigger DPC; otherwise, writes to this bit have no effect. Reads always return 0.
#define PCIE_DPC_CNTL_DPC_SOFTWARE_TRIGGER_OFFSET      6
#define PCIE_DPC_CNTL_DPC_SOFTWARE_TRIGGER_MASK        0x40

// Bitfield Description : When DL_Active ERR_COR Signaling Supported bit is set, controls if the Downstream Port signals with ERR_COR when the Link transitions to the DL_Active state.
#define PCIE_DPC_CNTL_DL_ACTIVE_ERR_COR_ENABLE_OFFSET      7
#define PCIE_DPC_CNTL_DL_ACTIVE_ERR_COR_ENABLE_MASK        0x80

// Bitfield Description : When ERR_COR Subclass Capable bit is set, controls if the Downstream Port signals with ERR_COR when DPC has been triggered due to an ECS SIG_SFW subclass.
#define PCIE_DPC_CNTL_DPC_SIG_SFW_ENABLE_OFFSET      8
#define PCIE_DPC_CNTL_DPC_SIG_SFW_ENABLE_MASK        0x100

// Bitfield Description : 
#define PCIE_DPC_CNTL_Reserved_15_9_OFFSET      9
#define PCIE_DPC_CNTL_Reserved_15_9_MASK        0xfe00

typedef union {
  struct {
    UINT16                            DPC_TRIGGER_ENABLE:2;
    UINT16                            DPC_COMPLETION_CONTROL:1;
    UINT16                            DPC_INTERRUPT_ENABLE:1;
    UINT16                            DPC_ERR_COR_ENABLE:1;
    UINT16                            POISONED_TLP_EGRESS_BLOCKING_ENABLE:1;
    UINT16                            DPC_SOFTWARE_TRIGGER:1;
    UINT16                            DL_ACTIVE_ERR_COR_ENABLE:1;
    UINT16                            DPC_SIG_SFW_ENABLE:1;
    UINT16                            Reserved_15_9:7;
  } Field;
  UINT16 Value;
} PCIE_DPC_CNTL_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_DPC_CNTL_ADDRESS    0x11100386UL

#define SMN_FUNC0_PCIE1_PCIE_DPC_CNTL_ADDRESS    0x11200386UL

#define SMN_FUNC0_PCIE2_PCIE_DPC_CNTL_ADDRESS    0x11300386UL

#define SMN_FUNC0_PCIE3_PCIE_DPC_CNTL_ADDRESS    0x11400386UL

#define SMN_FUNC1_PCIE0_PCIE_DPC_CNTL_ADDRESS    0x11101386UL

#define SMN_FUNC1_PCIE1_PCIE_DPC_CNTL_ADDRESS    0x11201386UL

#define SMN_FUNC2_PCIE0_PCIE_DPC_CNTL_ADDRESS    0x11102386UL

#define SMN_FUNC2_PCIE1_PCIE_DPC_CNTL_ADDRESS    0x11202386UL

#define SMN_FUNC3_PCIE0_PCIE_DPC_CNTL_ADDRESS    0x11103386UL

#define SMN_FUNC3_PCIE1_PCIE_DPC_CNTL_ADDRESS    0x11203386UL

#define SMN_FUNC4_PCIE0_PCIE_DPC_CNTL_ADDRESS    0x11104386UL

#define SMN_FUNC4_PCIE1_PCIE_DPC_CNTL_ADDRESS    0x11204386UL

#define SMN_FUNC5_PCIE1_PCIE_DPC_CNTL_ADDRESS    0x11205386UL


/***********************************************************
* Register Name : PCIE_DPC_ENH_CAP_LIST
* Register Description :
* Data Port Containment Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_DPC_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_DPC_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_DPC_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_DPC_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_DPC_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_DPC_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_DPC_ENH_CAP_LIST_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11100380UL

#define SMN_FUNC0_PCIE1_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11200380UL

#define SMN_FUNC0_PCIE2_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11300380UL

#define SMN_FUNC0_PCIE3_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11400380UL

#define SMN_FUNC1_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11101380UL

#define SMN_FUNC1_PCIE1_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11201380UL

#define SMN_FUNC2_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11102380UL

#define SMN_FUNC2_PCIE1_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11202380UL

#define SMN_FUNC3_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11103380UL

#define SMN_FUNC3_PCIE1_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11203380UL

#define SMN_FUNC4_PCIE0_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11104380UL

#define SMN_FUNC4_PCIE1_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11204380UL

#define SMN_FUNC5_PCIE1_PCIE_DPC_ENH_CAP_LIST_ADDRESS    0x11205380UL


/***********************************************************
* Register Name : PCIE_DPC_ERROR_SOURCE_ID
* Register Description :
* DPC Error Source ID register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When DPC Trigger Status bit is set and DPC Trigger Reason field is 1 or 2, indicates the Requester ID of the received ERR_FATAL or ERR_NONFATAL Message. This field is sticky.
#define PCIE_DPC_ERROR_SOURCE_ID_DPC_ERROR_SOURCE_ID_OFFSET      0
#define PCIE_DPC_ERROR_SOURCE_ID_DPC_ERROR_SOURCE_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            DPC_ERROR_SOURCE_ID:16;
  } Field;
  UINT16 Value;
} PCIE_DPC_ERROR_SOURCE_ID_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1110038aUL

#define SMN_FUNC0_PCIE1_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1120038aUL

#define SMN_FUNC0_PCIE2_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1130038aUL

#define SMN_FUNC0_PCIE3_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1140038aUL

#define SMN_FUNC1_PCIE0_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1110138aUL

#define SMN_FUNC1_PCIE1_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1120138aUL

#define SMN_FUNC2_PCIE0_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1110238aUL

#define SMN_FUNC2_PCIE1_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1120238aUL

#define SMN_FUNC3_PCIE0_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1110338aUL

#define SMN_FUNC3_PCIE1_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1120338aUL

#define SMN_FUNC4_PCIE0_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1110438aUL

#define SMN_FUNC4_PCIE1_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1120438aUL

#define SMN_FUNC5_PCIE1_PCIE_DPC_ERROR_SOURCE_ID_ADDRESS    0x1120538aUL


/***********************************************************
* Register Name : PCIE_DPC_STATUS
* Register Description :
* DPC Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if DPC has been triggered. While this bit is set, the Port's Link remains in the Disabled state. This bit is sticky.
#define PCIE_DPC_STATUS_DPC_TRIGGER_STATUS_OFFSET      0
#define PCIE_DPC_STATUS_DPC_TRIGGER_STATUS_MASK        0x1

// Bitfield Description : When DPC Trigger Status bit is set, indicates why DPC has been triggered. This field is sticky.
#define PCIE_DPC_STATUS_DPC_TRIGGER_REASON_OFFSET      1
#define PCIE_DPC_STATUS_DPC_TRIGGER_REASON_MASK        0x6

// Bitfield Description : When DPC Interrupt Enable bit is set, indicates if a DPC interrupt is pending internally in the Port. This bit is sticky.
#define PCIE_DPC_STATUS_DPC_INTERRUPT_STATUS_OFFSET      3
#define PCIE_DPC_STATUS_DPC_INTERRUPT_STATUS_MASK        0x8

// Bitfield Description : If RP Extensions for DPC bit is set, when DPC Trigger Status bit is set, indicates if the Root Port is busy with internal activity that must complete before software is permitted to clear the DPC Trigger Status bit.
// If RP Extensions for DPC bit is clear, this bit is Reserved.
#define PCIE_DPC_STATUS_DPC_RP_BUSY_OFFSET      4
#define PCIE_DPC_STATUS_DPC_RP_BUSY_MASK        0x10

// Bitfield Description : When DPC Trigger Status bit is set and DPC Trigger Reason field is 3, indicates why DPC has been triggered. This field is sticky.
#define PCIE_DPC_STATUS_DPC_TRIGGER_REASON_EXTENSION_OFFSET      5
#define PCIE_DPC_STATUS_DPC_TRIGGER_REASON_EXTENSION_MASK        0x60

// Bitfield Description : 
#define PCIE_DPC_STATUS_Reserved_7_7_OFFSET      7
#define PCIE_DPC_STATUS_Reserved_7_7_MASK        0x80

// Bitfield Description : If RP Extensions for DPC bit is set, indicates the bit position of the first error reported in the RP PIO Status register. This field is only valid if the indicated bit is also set. This field is sticky.
// If RP Extensions for DPC bit is clear, this field is Reserved.
#define PCIE_DPC_STATUS_RP_PIO_FIRST_ERROR_POINTER_OFFSET      8
#define PCIE_DPC_STATUS_RP_PIO_FIRST_ERROR_POINTER_MASK        0x1f00

// Bitfield Description : 
#define PCIE_DPC_STATUS_Reserved_15_13_OFFSET      13
#define PCIE_DPC_STATUS_Reserved_15_13_MASK        0xe000

typedef union {
  struct {
    UINT16                            DPC_TRIGGER_STATUS:1;
    UINT16                            DPC_TRIGGER_REASON:2;
    UINT16                            DPC_INTERRUPT_STATUS:1;
    UINT16                            DPC_RP_BUSY:1;
    UINT16                            DPC_TRIGGER_REASON_EXTENSION:2;
    UINT16                            Reserved_7_7:1;
    UINT16                            RP_PIO_FIRST_ERROR_POINTER:5;
    UINT16                            Reserved_15_13:3;
  } Field;
  UINT16 Value;
} PCIE_DPC_STATUS_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_DPC_STATUS_ADDRESS    0x11100388UL

#define SMN_FUNC0_PCIE1_PCIE_DPC_STATUS_ADDRESS    0x11200388UL

#define SMN_FUNC0_PCIE2_PCIE_DPC_STATUS_ADDRESS    0x11300388UL

#define SMN_FUNC0_PCIE3_PCIE_DPC_STATUS_ADDRESS    0x11400388UL

#define SMN_FUNC1_PCIE0_PCIE_DPC_STATUS_ADDRESS    0x11101388UL

#define SMN_FUNC1_PCIE1_PCIE_DPC_STATUS_ADDRESS    0x11201388UL

#define SMN_FUNC2_PCIE0_PCIE_DPC_STATUS_ADDRESS    0x11102388UL

#define SMN_FUNC2_PCIE1_PCIE_DPC_STATUS_ADDRESS    0x11202388UL

#define SMN_FUNC3_PCIE0_PCIE_DPC_STATUS_ADDRESS    0x11103388UL

#define SMN_FUNC3_PCIE1_PCIE_DPC_STATUS_ADDRESS    0x11203388UL

#define SMN_FUNC4_PCIE0_PCIE_DPC_STATUS_ADDRESS    0x11104388UL

#define SMN_FUNC4_PCIE1_PCIE_DPC_STATUS_ADDRESS    0x11204388UL

#define SMN_FUNC5_PCIE1_PCIE_DPC_STATUS_ADDRESS    0x11205388UL


/***********************************************************
* Register Name : PCIE_ERR_SRC_ID
* Register Description :
* The Error Source Identification register identifies the source (Requester ID) of first correctable and uncorrectable (Non-fatal/Fatal) errors reported in the Root Error Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the Requester ID indicated in the received ERR_COR Message when the ERR_COR Received bit is not already set. This field is sticky.
#define PCIE_ERR_SRC_ID_ERR_CORR_SRC_ID_OFFSET      0
#define PCIE_ERR_SRC_ID_ERR_CORR_SRC_ID_MASK        0xffff

// Bitfield Description : Indicates the Requester ID indicated in the received ERR_FATAL or ERR_NONFATAL Message when the ERR_FATAL/NONFATAL Received bit is not already set. This field is sticky.
#define PCIE_ERR_SRC_ID_ERR_FATAL_NONFATAL_SRC_ID_OFFSET      16
#define PCIE_ERR_SRC_ID_ERR_FATAL_NONFATAL_SRC_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            ERR_CORR_SRC_ID:16;
    UINT32                            ERR_FATAL_NONFATAL_SRC_ID:16;
  } Field;
  UINT32 Value;
} PCIE_ERR_SRC_ID_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ERR_SRC_ID_ADDRESS    0x11100184UL

#define SMN_FUNC0_PCIE1_PCIE_ERR_SRC_ID_ADDRESS    0x11200184UL

#define SMN_FUNC0_PCIE2_PCIE_ERR_SRC_ID_ADDRESS    0x11300184UL

#define SMN_FUNC0_PCIE3_PCIE_ERR_SRC_ID_ADDRESS    0x11400184UL

#define SMN_FUNC1_PCIE0_PCIE_ERR_SRC_ID_ADDRESS    0x11101184UL

#define SMN_FUNC1_PCIE1_PCIE_ERR_SRC_ID_ADDRESS    0x11201184UL

#define SMN_FUNC2_PCIE0_PCIE_ERR_SRC_ID_ADDRESS    0x11102184UL

#define SMN_FUNC2_PCIE1_PCIE_ERR_SRC_ID_ADDRESS    0x11202184UL

#define SMN_FUNC3_PCIE0_PCIE_ERR_SRC_ID_ADDRESS    0x11103184UL

#define SMN_FUNC3_PCIE1_PCIE_ERR_SRC_ID_ADDRESS    0x11203184UL

#define SMN_FUNC4_PCIE0_PCIE_ERR_SRC_ID_ADDRESS    0x11104184UL

#define SMN_FUNC4_PCIE1_PCIE_ERR_SRC_ID_ADDRESS    0x11204184UL

#define SMN_FUNC5_PCIE1_PCIE_ERR_SRC_ID_ADDRESS    0x11205184UL


/***********************************************************
* Register Name : PCIE_ESM_CAP_1
* Register Description :
* ESM Capabilities 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port supports ESM Mode at 8.0 GT/s.
#define PCIE_ESM_CAP_1_ESM_8P0G_OFFSET      0
#define PCIE_ESM_CAP_1_ESM_8P0G_MASK        0x1

// Bitfield Description : Indicates if the Port supports ESM Mode at 8.1 GT/s.
#define PCIE_ESM_CAP_1_ESM_8P1G_OFFSET      1
#define PCIE_ESM_CAP_1_ESM_8P1G_MASK        0x2

// Bitfield Description : Indicates if the Port supports ESM Mode at 8.2 GT/s.
#define PCIE_ESM_CAP_1_ESM_8P2G_OFFSET      2
#define PCIE_ESM_CAP_1_ESM_8P2G_MASK        0x4

// Bitfield Description : Indicates if the Port supports ESM Mode at 8.3 GT/s.
#define PCIE_ESM_CAP_1_ESM_8P3G_OFFSET      3
#define PCIE_ESM_CAP_1_ESM_8P3G_MASK        0x8

// Bitfield Description : Indicates if the Port supports ESM Mode at 8.4 GT/s.
#define PCIE_ESM_CAP_1_ESM_8P4G_OFFSET      4
#define PCIE_ESM_CAP_1_ESM_8P4G_MASK        0x10

// Bitfield Description : Indicates if the Port supports ESM Mode at 8.5 GT/s.
#define PCIE_ESM_CAP_1_ESM_8P5G_OFFSET      5
#define PCIE_ESM_CAP_1_ESM_8P5G_MASK        0x20

// Bitfield Description : Indicates if the Port supports ESM Mode at 8.6 GT/s.
#define PCIE_ESM_CAP_1_ESM_8P6G_OFFSET      6
#define PCIE_ESM_CAP_1_ESM_8P6G_MASK        0x40

// Bitfield Description : Indicates if the Port supports ESM Mode at 8.7 GT/s.
#define PCIE_ESM_CAP_1_ESM_8P7G_OFFSET      7
#define PCIE_ESM_CAP_1_ESM_8P7G_MASK        0x80

// Bitfield Description : Indicates if the Port supports ESM Mode at 8.8 GT/s.
#define PCIE_ESM_CAP_1_ESM_8P8G_OFFSET      8
#define PCIE_ESM_CAP_1_ESM_8P8G_MASK        0x100

// Bitfield Description : Indicates if the Port supports ESM Mode at 8.9 GT/s.
#define PCIE_ESM_CAP_1_ESM_8P9G_OFFSET      9
#define PCIE_ESM_CAP_1_ESM_8P9G_MASK        0x200

// Bitfield Description : Indicates if the Port supports ESM Mode at 9.0 GT/s.
#define PCIE_ESM_CAP_1_ESM_9P0G_OFFSET      10
#define PCIE_ESM_CAP_1_ESM_9P0G_MASK        0x400

// Bitfield Description : Indicates if the Port supports ESM Mode at 9.1 GT/s.
#define PCIE_ESM_CAP_1_ESM_9P1G_OFFSET      11
#define PCIE_ESM_CAP_1_ESM_9P1G_MASK        0x800

// Bitfield Description : Indicates if the Port supports ESM Mode at 9.2 GT/s.
#define PCIE_ESM_CAP_1_ESM_9P2G_OFFSET      12
#define PCIE_ESM_CAP_1_ESM_9P2G_MASK        0x1000

// Bitfield Description : Indicates if the Port supports ESM Mode at 9.3 GT/s.
#define PCIE_ESM_CAP_1_ESM_9P3G_OFFSET      13
#define PCIE_ESM_CAP_1_ESM_9P3G_MASK        0x2000

// Bitfield Description : Indicates if the Port supports ESM Mode at 9.4 GT/s.
#define PCIE_ESM_CAP_1_ESM_9P4G_OFFSET      14
#define PCIE_ESM_CAP_1_ESM_9P4G_MASK        0x4000

// Bitfield Description : Indicates if the Port supports ESM Mode at 9.5 GT/s.
#define PCIE_ESM_CAP_1_ESM_9P5G_OFFSET      15
#define PCIE_ESM_CAP_1_ESM_9P5G_MASK        0x8000

// Bitfield Description : Indicates if the Port supports ESM Mode at 9.6 GT/s.
#define PCIE_ESM_CAP_1_ESM_9P6G_OFFSET      16
#define PCIE_ESM_CAP_1_ESM_9P6G_MASK        0x10000

// Bitfield Description : Indicates if the Port supports ESM Mode at 9.7 GT/s.
#define PCIE_ESM_CAP_1_ESM_9P7G_OFFSET      17
#define PCIE_ESM_CAP_1_ESM_9P7G_MASK        0x20000

// Bitfield Description : Indicates if the Port supports ESM Mode at 9.8 GT/s.
#define PCIE_ESM_CAP_1_ESM_9P8G_OFFSET      18
#define PCIE_ESM_CAP_1_ESM_9P8G_MASK        0x40000

// Bitfield Description : Indicates if the Port supports ESM Mode at 9.9 GT/s.
#define PCIE_ESM_CAP_1_ESM_9P9G_OFFSET      19
#define PCIE_ESM_CAP_1_ESM_9P9G_MASK        0x80000

// Bitfield Description : Indicates if the Port supports ESM Mode at 10.0 GT/s.
#define PCIE_ESM_CAP_1_ESM_10P0G_OFFSET      20
#define PCIE_ESM_CAP_1_ESM_10P0G_MASK        0x100000

// Bitfield Description : Indicates if the Port supports ESM Mode at 10.1 GT/s.
#define PCIE_ESM_CAP_1_ESM_10P1G_OFFSET      21
#define PCIE_ESM_CAP_1_ESM_10P1G_MASK        0x200000

// Bitfield Description : Indicates if the Port supports ESM Mode at 10.2 GT/s.
#define PCIE_ESM_CAP_1_ESM_10P2G_OFFSET      22
#define PCIE_ESM_CAP_1_ESM_10P2G_MASK        0x400000

// Bitfield Description : Indicates if the Port supports ESM Mode at 10.3 GT/s.
#define PCIE_ESM_CAP_1_ESM_10P3G_OFFSET      23
#define PCIE_ESM_CAP_1_ESM_10P3G_MASK        0x800000

// Bitfield Description : Indicates if the Port supports ESM Mode at 10.4 GT/s.
#define PCIE_ESM_CAP_1_ESM_10P4G_OFFSET      24
#define PCIE_ESM_CAP_1_ESM_10P4G_MASK        0x1000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 10.5 GT/s.
#define PCIE_ESM_CAP_1_ESM_10P5G_OFFSET      25
#define PCIE_ESM_CAP_1_ESM_10P5G_MASK        0x2000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 10.6 GT/s.
#define PCIE_ESM_CAP_1_ESM_10P6G_OFFSET      26
#define PCIE_ESM_CAP_1_ESM_10P6G_MASK        0x4000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 10.7 GT/s.
#define PCIE_ESM_CAP_1_ESM_10P7G_OFFSET      27
#define PCIE_ESM_CAP_1_ESM_10P7G_MASK        0x8000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 10.8 GT/s.
#define PCIE_ESM_CAP_1_ESM_10P8G_OFFSET      28
#define PCIE_ESM_CAP_1_ESM_10P8G_MASK        0x10000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 10.9 GT/s.
#define PCIE_ESM_CAP_1_ESM_10P9G_OFFSET      29
#define PCIE_ESM_CAP_1_ESM_10P9G_MASK        0x20000000

// Bitfield Description : 
#define PCIE_ESM_CAP_1_Reserved_31_30_OFFSET      30
#define PCIE_ESM_CAP_1_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            ESM_8P0G:1;
    UINT32                            ESM_8P1G:1;
    UINT32                            ESM_8P2G:1;
    UINT32                            ESM_8P3G:1;
    UINT32                            ESM_8P4G:1;
    UINT32                            ESM_8P5G:1;
    UINT32                            ESM_8P6G:1;
    UINT32                            ESM_8P7G:1;
    UINT32                            ESM_8P8G:1;
    UINT32                            ESM_8P9G:1;
    UINT32                            ESM_9P0G:1;
    UINT32                            ESM_9P1G:1;
    UINT32                            ESM_9P2G:1;
    UINT32                            ESM_9P3G:1;
    UINT32                            ESM_9P4G:1;
    UINT32                            ESM_9P5G:1;
    UINT32                            ESM_9P6G:1;
    UINT32                            ESM_9P7G:1;
    UINT32                            ESM_9P8G:1;
    UINT32                            ESM_9P9G:1;
    UINT32                            ESM_10P0G:1;
    UINT32                            ESM_10P1G:1;
    UINT32                            ESM_10P2G:1;
    UINT32                            ESM_10P3G:1;
    UINT32                            ESM_10P4G:1;
    UINT32                            ESM_10P5G:1;
    UINT32                            ESM_10P6G:1;
    UINT32                            ESM_10P7G:1;
    UINT32                            ESM_10P8G:1;
    UINT32                            ESM_10P9G:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} PCIE_ESM_CAP_1_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_CAP_1_ADDRESS    0x111003d4UL

#define SMN_FUNC0_PCIE1_PCIE_ESM_CAP_1_ADDRESS    0x112003d4UL

#define SMN_FUNC0_PCIE2_PCIE_ESM_CAP_1_ADDRESS    0x113003d4UL

#define SMN_FUNC0_PCIE3_PCIE_ESM_CAP_1_ADDRESS    0x114003d4UL

#define SMN_FUNC1_PCIE0_PCIE_ESM_CAP_1_ADDRESS    0x111013d4UL

#define SMN_FUNC1_PCIE1_PCIE_ESM_CAP_1_ADDRESS    0x112013d4UL

#define SMN_FUNC2_PCIE0_PCIE_ESM_CAP_1_ADDRESS    0x111023d4UL

#define SMN_FUNC2_PCIE1_PCIE_ESM_CAP_1_ADDRESS    0x112023d4UL

#define SMN_FUNC3_PCIE0_PCIE_ESM_CAP_1_ADDRESS    0x111033d4UL

#define SMN_FUNC3_PCIE1_PCIE_ESM_CAP_1_ADDRESS    0x112033d4UL

#define SMN_FUNC4_PCIE0_PCIE_ESM_CAP_1_ADDRESS    0x111043d4UL

#define SMN_FUNC4_PCIE1_PCIE_ESM_CAP_1_ADDRESS    0x112043d4UL

#define SMN_FUNC5_PCIE1_PCIE_ESM_CAP_1_ADDRESS    0x112053d4UL


/***********************************************************
* Register Name : PCIE_ESM_CAP_2
* Register Description :
* ESM Capabilities 2 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port supports ESM Mode at 11.0 GT/s.
#define PCIE_ESM_CAP_2_ESM_11P0G_OFFSET      0
#define PCIE_ESM_CAP_2_ESM_11P0G_MASK        0x1

// Bitfield Description : Indicates if the Port supports ESM Mode at 11.1 GT/s.
#define PCIE_ESM_CAP_2_ESM_11P1G_OFFSET      1
#define PCIE_ESM_CAP_2_ESM_11P1G_MASK        0x2

// Bitfield Description : Indicates if the Port supports ESM Mode at 11.2 GT/s.
#define PCIE_ESM_CAP_2_ESM_11P2G_OFFSET      2
#define PCIE_ESM_CAP_2_ESM_11P2G_MASK        0x4

// Bitfield Description : Indicates if the Port supports ESM Mode at 11.3 GT/s.
#define PCIE_ESM_CAP_2_ESM_11P3G_OFFSET      3
#define PCIE_ESM_CAP_2_ESM_11P3G_MASK        0x8

// Bitfield Description : Indicates if the Port supports ESM Mode at 11.4 GT/s.
#define PCIE_ESM_CAP_2_ESM_11P4G_OFFSET      4
#define PCIE_ESM_CAP_2_ESM_11P4G_MASK        0x10

// Bitfield Description : Indicates if the Port supports ESM Mode at 11.5 GT/s.
#define PCIE_ESM_CAP_2_ESM_11P5G_OFFSET      5
#define PCIE_ESM_CAP_2_ESM_11P5G_MASK        0x20

// Bitfield Description : Indicates if the Port supports ESM Mode at 11.6 GT/s.
#define PCIE_ESM_CAP_2_ESM_11P6G_OFFSET      6
#define PCIE_ESM_CAP_2_ESM_11P6G_MASK        0x40

// Bitfield Description : Indicates if the Port supports ESM Mode at 11.7 GT/s.
#define PCIE_ESM_CAP_2_ESM_11P7G_OFFSET      7
#define PCIE_ESM_CAP_2_ESM_11P7G_MASK        0x80

// Bitfield Description : Indicates if the Port supports ESM Mode at 11.8 GT/s.
#define PCIE_ESM_CAP_2_ESM_11P8G_OFFSET      8
#define PCIE_ESM_CAP_2_ESM_11P8G_MASK        0x100

// Bitfield Description : Indicates if the Port supports ESM Mode at 11.9 GT/s.
#define PCIE_ESM_CAP_2_ESM_11P9G_OFFSET      9
#define PCIE_ESM_CAP_2_ESM_11P9G_MASK        0x200

// Bitfield Description : Indicates if the Port supports ESM Mode at 12.0 GT/s.
#define PCIE_ESM_CAP_2_ESM_12P0G_OFFSET      10
#define PCIE_ESM_CAP_2_ESM_12P0G_MASK        0x400

// Bitfield Description : Indicates if the Port supports ESM Mode at 12.1 GT/s.
#define PCIE_ESM_CAP_2_ESM_12P1G_OFFSET      11
#define PCIE_ESM_CAP_2_ESM_12P1G_MASK        0x800

// Bitfield Description : Indicates if the Port supports ESM Mode at 12.2 GT/s.
#define PCIE_ESM_CAP_2_ESM_12P2G_OFFSET      12
#define PCIE_ESM_CAP_2_ESM_12P2G_MASK        0x1000

// Bitfield Description : Indicates if the Port supports ESM Mode at 12.3 GT/s.
#define PCIE_ESM_CAP_2_ESM_12P3G_OFFSET      13
#define PCIE_ESM_CAP_2_ESM_12P3G_MASK        0x2000

// Bitfield Description : Indicates if the Port supports ESM Mode at 12.4 GT/s.
#define PCIE_ESM_CAP_2_ESM_12P4G_OFFSET      14
#define PCIE_ESM_CAP_2_ESM_12P4G_MASK        0x4000

// Bitfield Description : Indicates if the Port supports ESM Mode at 12.5 GT/s.
#define PCIE_ESM_CAP_2_ESM_12P5G_OFFSET      15
#define PCIE_ESM_CAP_2_ESM_12P5G_MASK        0x8000

// Bitfield Description : Indicates if the Port supports ESM Mode at 12.6 GT/s.
#define PCIE_ESM_CAP_2_ESM_12P6G_OFFSET      16
#define PCIE_ESM_CAP_2_ESM_12P6G_MASK        0x10000

// Bitfield Description : Indicates if the Port supports ESM Mode at 12.7 GT/s.
#define PCIE_ESM_CAP_2_ESM_12P7G_OFFSET      17
#define PCIE_ESM_CAP_2_ESM_12P7G_MASK        0x20000

// Bitfield Description : Indicates if the Port supports ESM Mode at 12.8 GT/s.
#define PCIE_ESM_CAP_2_ESM_12P8G_OFFSET      18
#define PCIE_ESM_CAP_2_ESM_12P8G_MASK        0x40000

// Bitfield Description : Indicates if the Port supports ESM Mode at 12.9 GT/s.
#define PCIE_ESM_CAP_2_ESM_12P9G_OFFSET      19
#define PCIE_ESM_CAP_2_ESM_12P9G_MASK        0x80000

// Bitfield Description : Indicates if the Port supports ESM Mode at 13.0 GT/s.
#define PCIE_ESM_CAP_2_ESM_13P0G_OFFSET      20
#define PCIE_ESM_CAP_2_ESM_13P0G_MASK        0x100000

// Bitfield Description : Indicates if the Port supports ESM Mode at 13.1 GT/s.
#define PCIE_ESM_CAP_2_ESM_13P1G_OFFSET      21
#define PCIE_ESM_CAP_2_ESM_13P1G_MASK        0x200000

// Bitfield Description : Indicates if the Port supports ESM Mode at 13.2 GT/s.
#define PCIE_ESM_CAP_2_ESM_13P2G_OFFSET      22
#define PCIE_ESM_CAP_2_ESM_13P2G_MASK        0x400000

// Bitfield Description : Indicates if the Port supports ESM Mode at 13.3 GT/s.
#define PCIE_ESM_CAP_2_ESM_13P3G_OFFSET      23
#define PCIE_ESM_CAP_2_ESM_13P3G_MASK        0x800000

// Bitfield Description : Indicates if the Port supports ESM Mode at 13.4 GT/s.
#define PCIE_ESM_CAP_2_ESM_13P4G_OFFSET      24
#define PCIE_ESM_CAP_2_ESM_13P4G_MASK        0x1000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 13.5 GT/s.
#define PCIE_ESM_CAP_2_ESM_13P5G_OFFSET      25
#define PCIE_ESM_CAP_2_ESM_13P5G_MASK        0x2000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 13.6 GT/s.
#define PCIE_ESM_CAP_2_ESM_13P6G_OFFSET      26
#define PCIE_ESM_CAP_2_ESM_13P6G_MASK        0x4000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 13.7 GT/s.
#define PCIE_ESM_CAP_2_ESM_13P7G_OFFSET      27
#define PCIE_ESM_CAP_2_ESM_13P7G_MASK        0x8000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 13.8 GT/s.
#define PCIE_ESM_CAP_2_ESM_13P8G_OFFSET      28
#define PCIE_ESM_CAP_2_ESM_13P8G_MASK        0x10000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 13.9 GT/s.
#define PCIE_ESM_CAP_2_ESM_13P9G_OFFSET      29
#define PCIE_ESM_CAP_2_ESM_13P9G_MASK        0x20000000

// Bitfield Description : 
#define PCIE_ESM_CAP_2_Reserved_31_30_OFFSET      30
#define PCIE_ESM_CAP_2_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            ESM_11P0G:1;
    UINT32                            ESM_11P1G:1;
    UINT32                            ESM_11P2G:1;
    UINT32                            ESM_11P3G:1;
    UINT32                            ESM_11P4G:1;
    UINT32                            ESM_11P5G:1;
    UINT32                            ESM_11P6G:1;
    UINT32                            ESM_11P7G:1;
    UINT32                            ESM_11P8G:1;
    UINT32                            ESM_11P9G:1;
    UINT32                            ESM_12P0G:1;
    UINT32                            ESM_12P1G:1;
    UINT32                            ESM_12P2G:1;
    UINT32                            ESM_12P3G:1;
    UINT32                            ESM_12P4G:1;
    UINT32                            ESM_12P5G:1;
    UINT32                            ESM_12P6G:1;
    UINT32                            ESM_12P7G:1;
    UINT32                            ESM_12P8G:1;
    UINT32                            ESM_12P9G:1;
    UINT32                            ESM_13P0G:1;
    UINT32                            ESM_13P1G:1;
    UINT32                            ESM_13P2G:1;
    UINT32                            ESM_13P3G:1;
    UINT32                            ESM_13P4G:1;
    UINT32                            ESM_13P5G:1;
    UINT32                            ESM_13P6G:1;
    UINT32                            ESM_13P7G:1;
    UINT32                            ESM_13P8G:1;
    UINT32                            ESM_13P9G:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} PCIE_ESM_CAP_2_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_CAP_2_ADDRESS    0x111003d8UL

#define SMN_FUNC0_PCIE1_PCIE_ESM_CAP_2_ADDRESS    0x112003d8UL

#define SMN_FUNC0_PCIE2_PCIE_ESM_CAP_2_ADDRESS    0x113003d8UL

#define SMN_FUNC0_PCIE3_PCIE_ESM_CAP_2_ADDRESS    0x114003d8UL

#define SMN_FUNC1_PCIE0_PCIE_ESM_CAP_2_ADDRESS    0x111013d8UL

#define SMN_FUNC1_PCIE1_PCIE_ESM_CAP_2_ADDRESS    0x112013d8UL

#define SMN_FUNC2_PCIE0_PCIE_ESM_CAP_2_ADDRESS    0x111023d8UL

#define SMN_FUNC2_PCIE1_PCIE_ESM_CAP_2_ADDRESS    0x112023d8UL

#define SMN_FUNC3_PCIE0_PCIE_ESM_CAP_2_ADDRESS    0x111033d8UL

#define SMN_FUNC3_PCIE1_PCIE_ESM_CAP_2_ADDRESS    0x112033d8UL

#define SMN_FUNC4_PCIE0_PCIE_ESM_CAP_2_ADDRESS    0x111043d8UL

#define SMN_FUNC4_PCIE1_PCIE_ESM_CAP_2_ADDRESS    0x112043d8UL

#define SMN_FUNC5_PCIE1_PCIE_ESM_CAP_2_ADDRESS    0x112053d8UL


/***********************************************************
* Register Name : PCIE_ESM_CAP_3
* Register Description :
* ESM Capabilities 3 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port supports ESM Mode at 14.0 GT/s.
#define PCIE_ESM_CAP_3_ESM_14P0G_OFFSET      0
#define PCIE_ESM_CAP_3_ESM_14P0G_MASK        0x1

// Bitfield Description : Indicates if the Port supports ESM Mode at 14.1 GT/s.
#define PCIE_ESM_CAP_3_ESM_14P1G_OFFSET      1
#define PCIE_ESM_CAP_3_ESM_14P1G_MASK        0x2

// Bitfield Description : Indicates if the Port supports ESM Mode at 14.2 GT/s.
#define PCIE_ESM_CAP_3_ESM_14P2G_OFFSET      2
#define PCIE_ESM_CAP_3_ESM_14P2G_MASK        0x4

// Bitfield Description : Indicates if the Port supports ESM Mode at 14.3 GT/s.
#define PCIE_ESM_CAP_3_ESM_14P3G_OFFSET      3
#define PCIE_ESM_CAP_3_ESM_14P3G_MASK        0x8

// Bitfield Description : Indicates if the Port supports ESM Mode at 14.4 GT/s.
#define PCIE_ESM_CAP_3_ESM_14P4G_OFFSET      4
#define PCIE_ESM_CAP_3_ESM_14P4G_MASK        0x10

// Bitfield Description : Indicates if the Port supports ESM Mode at 14.5 GT/s.
#define PCIE_ESM_CAP_3_ESM_14P5G_OFFSET      5
#define PCIE_ESM_CAP_3_ESM_14P5G_MASK        0x20

// Bitfield Description : Indicates if the Port supports ESM Mode at 14.6 GT/s.
#define PCIE_ESM_CAP_3_ESM_14P6G_OFFSET      6
#define PCIE_ESM_CAP_3_ESM_14P6G_MASK        0x40

// Bitfield Description : Indicates if the Port supports ESM Mode at 14.7 GT/s.
#define PCIE_ESM_CAP_3_ESM_14P7G_OFFSET      7
#define PCIE_ESM_CAP_3_ESM_14P7G_MASK        0x80

// Bitfield Description : Indicates if the Port supports ESM Mode at 14.8 GT/s.
#define PCIE_ESM_CAP_3_ESM_14P8G_OFFSET      8
#define PCIE_ESM_CAP_3_ESM_14P8G_MASK        0x100

// Bitfield Description : Indicates if the Port supports ESM Mode at 14.9 GT/s.
#define PCIE_ESM_CAP_3_ESM_14P9G_OFFSET      9
#define PCIE_ESM_CAP_3_ESM_14P9G_MASK        0x200

// Bitfield Description : Indicates if the Port supports ESM Mode at 15.0 GT/s.
#define PCIE_ESM_CAP_3_ESM_15P0G_OFFSET      10
#define PCIE_ESM_CAP_3_ESM_15P0G_MASK        0x400

// Bitfield Description : Indicates if the Port supports ESM Mode at 15.1 GT/s.
#define PCIE_ESM_CAP_3_ESM_15P1G_OFFSET      11
#define PCIE_ESM_CAP_3_ESM_15P1G_MASK        0x800

// Bitfield Description : Indicates if the Port supports ESM Mode at 15.2 GT/s.
#define PCIE_ESM_CAP_3_ESM_15P2G_OFFSET      12
#define PCIE_ESM_CAP_3_ESM_15P2G_MASK        0x1000

// Bitfield Description : Indicates if the Port supports ESM Mode at 15.3 GT/s.
#define PCIE_ESM_CAP_3_ESM_15P3G_OFFSET      13
#define PCIE_ESM_CAP_3_ESM_15P3G_MASK        0x2000

// Bitfield Description : Indicates if the Port supports ESM Mode at 15.4 GT/s.
#define PCIE_ESM_CAP_3_ESM_15P4G_OFFSET      14
#define PCIE_ESM_CAP_3_ESM_15P4G_MASK        0x4000

// Bitfield Description : Indicates if the Port supports ESM Mode at 15.5 GT/s.
#define PCIE_ESM_CAP_3_ESM_15P5G_OFFSET      15
#define PCIE_ESM_CAP_3_ESM_15P5G_MASK        0x8000

// Bitfield Description : Indicates if the Port supports ESM Mode at 15.6 GT/s.
#define PCIE_ESM_CAP_3_ESM_15P6G_OFFSET      16
#define PCIE_ESM_CAP_3_ESM_15P6G_MASK        0x10000

// Bitfield Description : Indicates if the Port supports ESM Mode at 15.7 GT/s.
#define PCIE_ESM_CAP_3_ESM_15P7G_OFFSET      17
#define PCIE_ESM_CAP_3_ESM_15P7G_MASK        0x20000

// Bitfield Description : Indicates if the Port supports ESM Mode at 15.8 GT/s.
#define PCIE_ESM_CAP_3_ESM_15P8G_OFFSET      18
#define PCIE_ESM_CAP_3_ESM_15P8G_MASK        0x40000

// Bitfield Description : Indicates if the Port supports ESM Mode at 15.9 GT/s.
#define PCIE_ESM_CAP_3_ESM_15P9G_OFFSET      19
#define PCIE_ESM_CAP_3_ESM_15P9G_MASK        0x80000

// Bitfield Description : 
#define PCIE_ESM_CAP_3_Reserved_31_20_OFFSET      20
#define PCIE_ESM_CAP_3_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            ESM_14P0G:1;
    UINT32                            ESM_14P1G:1;
    UINT32                            ESM_14P2G:1;
    UINT32                            ESM_14P3G:1;
    UINT32                            ESM_14P4G:1;
    UINT32                            ESM_14P5G:1;
    UINT32                            ESM_14P6G:1;
    UINT32                            ESM_14P7G:1;
    UINT32                            ESM_14P8G:1;
    UINT32                            ESM_14P9G:1;
    UINT32                            ESM_15P0G:1;
    UINT32                            ESM_15P1G:1;
    UINT32                            ESM_15P2G:1;
    UINT32                            ESM_15P3G:1;
    UINT32                            ESM_15P4G:1;
    UINT32                            ESM_15P5G:1;
    UINT32                            ESM_15P6G:1;
    UINT32                            ESM_15P7G:1;
    UINT32                            ESM_15P8G:1;
    UINT32                            ESM_15P9G:1;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} PCIE_ESM_CAP_3_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_CAP_3_ADDRESS    0x111003dcUL

#define SMN_FUNC0_PCIE1_PCIE_ESM_CAP_3_ADDRESS    0x112003dcUL

#define SMN_FUNC0_PCIE2_PCIE_ESM_CAP_3_ADDRESS    0x113003dcUL

#define SMN_FUNC0_PCIE3_PCIE_ESM_CAP_3_ADDRESS    0x114003dcUL

#define SMN_FUNC1_PCIE0_PCIE_ESM_CAP_3_ADDRESS    0x111013dcUL

#define SMN_FUNC1_PCIE1_PCIE_ESM_CAP_3_ADDRESS    0x112013dcUL

#define SMN_FUNC2_PCIE0_PCIE_ESM_CAP_3_ADDRESS    0x111023dcUL

#define SMN_FUNC2_PCIE1_PCIE_ESM_CAP_3_ADDRESS    0x112023dcUL

#define SMN_FUNC3_PCIE0_PCIE_ESM_CAP_3_ADDRESS    0x111033dcUL

#define SMN_FUNC3_PCIE1_PCIE_ESM_CAP_3_ADDRESS    0x112033dcUL

#define SMN_FUNC4_PCIE0_PCIE_ESM_CAP_3_ADDRESS    0x111043dcUL

#define SMN_FUNC4_PCIE1_PCIE_ESM_CAP_3_ADDRESS    0x112043dcUL

#define SMN_FUNC5_PCIE1_PCIE_ESM_CAP_3_ADDRESS    0x112053dcUL


/***********************************************************
* Register Name : PCIE_ESM_CAP_4
* Register Description :
* ESM Capabilities 4 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port supports ESM Mode at 16.0 GT/s.
#define PCIE_ESM_CAP_4_ESM_16P0G_OFFSET      0
#define PCIE_ESM_CAP_4_ESM_16P0G_MASK        0x1

// Bitfield Description : Indicates if the Port supports ESM Mode at 16.1 GT/s.
#define PCIE_ESM_CAP_4_ESM_16P1G_OFFSET      1
#define PCIE_ESM_CAP_4_ESM_16P1G_MASK        0x2

// Bitfield Description : Indicates if the Port supports ESM Mode at 16.2 GT/s.
#define PCIE_ESM_CAP_4_ESM_16P2G_OFFSET      2
#define PCIE_ESM_CAP_4_ESM_16P2G_MASK        0x4

// Bitfield Description : Indicates if the Port supports ESM Mode at 16.3 GT/s.
#define PCIE_ESM_CAP_4_ESM_16P3G_OFFSET      3
#define PCIE_ESM_CAP_4_ESM_16P3G_MASK        0x8

// Bitfield Description : Indicates if the Port supports ESM Mode at 16.4 GT/s.
#define PCIE_ESM_CAP_4_ESM_16P4G_OFFSET      4
#define PCIE_ESM_CAP_4_ESM_16P4G_MASK        0x10

// Bitfield Description : Indicates if the Port supports ESM Mode at 16.5 GT/s.
#define PCIE_ESM_CAP_4_ESM_16P5G_OFFSET      5
#define PCIE_ESM_CAP_4_ESM_16P5G_MASK        0x20

// Bitfield Description : Indicates if the Port supports ESM Mode at 16.6 GT/s.
#define PCIE_ESM_CAP_4_ESM_16P6G_OFFSET      6
#define PCIE_ESM_CAP_4_ESM_16P6G_MASK        0x40

// Bitfield Description : Indicates if the Port supports ESM Mode at 16.7 GT/s.
#define PCIE_ESM_CAP_4_ESM_16P7G_OFFSET      7
#define PCIE_ESM_CAP_4_ESM_16P7G_MASK        0x80

// Bitfield Description : Indicates if the Port supports ESM Mode at 16.8 GT/s.
#define PCIE_ESM_CAP_4_ESM_16P8G_OFFSET      8
#define PCIE_ESM_CAP_4_ESM_16P8G_MASK        0x100

// Bitfield Description : Indicates if the Port supports ESM Mode at 16.9 GT/s.
#define PCIE_ESM_CAP_4_ESM_16P9G_OFFSET      9
#define PCIE_ESM_CAP_4_ESM_16P9G_MASK        0x200

// Bitfield Description : Indicates if the Port supports ESM Mode at 17.0 GT/s.
#define PCIE_ESM_CAP_4_ESM_17P0G_OFFSET      10
#define PCIE_ESM_CAP_4_ESM_17P0G_MASK        0x400

// Bitfield Description : Indicates if the Port supports ESM Mode at 17.1 GT/s.
#define PCIE_ESM_CAP_4_ESM_17P1G_OFFSET      11
#define PCIE_ESM_CAP_4_ESM_17P1G_MASK        0x800

// Bitfield Description : Indicates if the Port supports ESM Mode at 17.2 GT/s.
#define PCIE_ESM_CAP_4_ESM_17P2G_OFFSET      12
#define PCIE_ESM_CAP_4_ESM_17P2G_MASK        0x1000

// Bitfield Description : Indicates if the Port supports ESM Mode at 17.3 GT/s.
#define PCIE_ESM_CAP_4_ESM_17P3G_OFFSET      13
#define PCIE_ESM_CAP_4_ESM_17P3G_MASK        0x2000

// Bitfield Description : Indicates if the Port supports ESM Mode at 17.4 GT/s.
#define PCIE_ESM_CAP_4_ESM_17P4G_OFFSET      14
#define PCIE_ESM_CAP_4_ESM_17P4G_MASK        0x4000

// Bitfield Description : Indicates if the Port supports ESM Mode at 17.5 GT/s.
#define PCIE_ESM_CAP_4_ESM_17P5G_OFFSET      15
#define PCIE_ESM_CAP_4_ESM_17P5G_MASK        0x8000

// Bitfield Description : Indicates if the Port supports ESM Mode at 17.6 GT/s.
#define PCIE_ESM_CAP_4_ESM_17P6G_OFFSET      16
#define PCIE_ESM_CAP_4_ESM_17P6G_MASK        0x10000

// Bitfield Description : Indicates if the Port supports ESM Mode at 17.7 GT/s.
#define PCIE_ESM_CAP_4_ESM_17P7G_OFFSET      17
#define PCIE_ESM_CAP_4_ESM_17P7G_MASK        0x20000

// Bitfield Description : Indicates if the Port supports ESM Mode at 17.8 GT/s.
#define PCIE_ESM_CAP_4_ESM_17P8G_OFFSET      18
#define PCIE_ESM_CAP_4_ESM_17P8G_MASK        0x40000

// Bitfield Description : Indicates if the Port supports ESM Mode at 17.9 GT/s.
#define PCIE_ESM_CAP_4_ESM_17P9G_OFFSET      19
#define PCIE_ESM_CAP_4_ESM_17P9G_MASK        0x80000

// Bitfield Description : Indicates if the Port supports ESM Mode at 18.0 GT/s.
#define PCIE_ESM_CAP_4_ESM_18P0G_OFFSET      20
#define PCIE_ESM_CAP_4_ESM_18P0G_MASK        0x100000

// Bitfield Description : Indicates if the Port supports ESM Mode at 18.1 GT/s.
#define PCIE_ESM_CAP_4_ESM_18P1G_OFFSET      21
#define PCIE_ESM_CAP_4_ESM_18P1G_MASK        0x200000

// Bitfield Description : Indicates if the Port supports ESM Mode at 18.2 GT/s.
#define PCIE_ESM_CAP_4_ESM_18P2G_OFFSET      22
#define PCIE_ESM_CAP_4_ESM_18P2G_MASK        0x400000

// Bitfield Description : Indicates if the Port supports ESM Mode at 18.3 GT/s.
#define PCIE_ESM_CAP_4_ESM_18P3G_OFFSET      23
#define PCIE_ESM_CAP_4_ESM_18P3G_MASK        0x800000

// Bitfield Description : Indicates if the Port supports ESM Mode at 18.4 GT/s.
#define PCIE_ESM_CAP_4_ESM_18P4G_OFFSET      24
#define PCIE_ESM_CAP_4_ESM_18P4G_MASK        0x1000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 18.5 GT/s.
#define PCIE_ESM_CAP_4_ESM_18P5G_OFFSET      25
#define PCIE_ESM_CAP_4_ESM_18P5G_MASK        0x2000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 18.6 GT/s.
#define PCIE_ESM_CAP_4_ESM_18P6G_OFFSET      26
#define PCIE_ESM_CAP_4_ESM_18P6G_MASK        0x4000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 18.7 GT/s.
#define PCIE_ESM_CAP_4_ESM_18P7G_OFFSET      27
#define PCIE_ESM_CAP_4_ESM_18P7G_MASK        0x8000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 18.8 GT/s.
#define PCIE_ESM_CAP_4_ESM_18P8G_OFFSET      28
#define PCIE_ESM_CAP_4_ESM_18P8G_MASK        0x10000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 18.9 GT/s.
#define PCIE_ESM_CAP_4_ESM_18P9G_OFFSET      29
#define PCIE_ESM_CAP_4_ESM_18P9G_MASK        0x20000000

// Bitfield Description : 
#define PCIE_ESM_CAP_4_Reserved_31_30_OFFSET      30
#define PCIE_ESM_CAP_4_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            ESM_16P0G:1;
    UINT32                            ESM_16P1G:1;
    UINT32                            ESM_16P2G:1;
    UINT32                            ESM_16P3G:1;
    UINT32                            ESM_16P4G:1;
    UINT32                            ESM_16P5G:1;
    UINT32                            ESM_16P6G:1;
    UINT32                            ESM_16P7G:1;
    UINT32                            ESM_16P8G:1;
    UINT32                            ESM_16P9G:1;
    UINT32                            ESM_17P0G:1;
    UINT32                            ESM_17P1G:1;
    UINT32                            ESM_17P2G:1;
    UINT32                            ESM_17P3G:1;
    UINT32                            ESM_17P4G:1;
    UINT32                            ESM_17P5G:1;
    UINT32                            ESM_17P6G:1;
    UINT32                            ESM_17P7G:1;
    UINT32                            ESM_17P8G:1;
    UINT32                            ESM_17P9G:1;
    UINT32                            ESM_18P0G:1;
    UINT32                            ESM_18P1G:1;
    UINT32                            ESM_18P2G:1;
    UINT32                            ESM_18P3G:1;
    UINT32                            ESM_18P4G:1;
    UINT32                            ESM_18P5G:1;
    UINT32                            ESM_18P6G:1;
    UINT32                            ESM_18P7G:1;
    UINT32                            ESM_18P8G:1;
    UINT32                            ESM_18P9G:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} PCIE_ESM_CAP_4_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_CAP_4_ADDRESS    0x111003e0UL

#define SMN_FUNC0_PCIE1_PCIE_ESM_CAP_4_ADDRESS    0x112003e0UL

#define SMN_FUNC0_PCIE2_PCIE_ESM_CAP_4_ADDRESS    0x113003e0UL

#define SMN_FUNC0_PCIE3_PCIE_ESM_CAP_4_ADDRESS    0x114003e0UL

#define SMN_FUNC1_PCIE0_PCIE_ESM_CAP_4_ADDRESS    0x111013e0UL

#define SMN_FUNC1_PCIE1_PCIE_ESM_CAP_4_ADDRESS    0x112013e0UL

#define SMN_FUNC2_PCIE0_PCIE_ESM_CAP_4_ADDRESS    0x111023e0UL

#define SMN_FUNC2_PCIE1_PCIE_ESM_CAP_4_ADDRESS    0x112023e0UL

#define SMN_FUNC3_PCIE0_PCIE_ESM_CAP_4_ADDRESS    0x111033e0UL

#define SMN_FUNC3_PCIE1_PCIE_ESM_CAP_4_ADDRESS    0x112033e0UL

#define SMN_FUNC4_PCIE0_PCIE_ESM_CAP_4_ADDRESS    0x111043e0UL

#define SMN_FUNC4_PCIE1_PCIE_ESM_CAP_4_ADDRESS    0x112043e0UL

#define SMN_FUNC5_PCIE1_PCIE_ESM_CAP_4_ADDRESS    0x112053e0UL


/***********************************************************
* Register Name : PCIE_ESM_CAP_5
* Register Description :
* ESM Capabilities 5 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port supports ESM Mode at 19.0 GT/s.
#define PCIE_ESM_CAP_5_ESM_19P0G_OFFSET      0
#define PCIE_ESM_CAP_5_ESM_19P0G_MASK        0x1

// Bitfield Description : Indicates if the Port supports ESM Mode at 19.1 GT/s.
#define PCIE_ESM_CAP_5_ESM_19P1G_OFFSET      1
#define PCIE_ESM_CAP_5_ESM_19P1G_MASK        0x2

// Bitfield Description : Indicates if the Port supports ESM Mode at 19.2 GT/s.
#define PCIE_ESM_CAP_5_ESM_19P2G_OFFSET      2
#define PCIE_ESM_CAP_5_ESM_19P2G_MASK        0x4

// Bitfield Description : Indicates if the Port supports ESM Mode at 19.3 GT/s.
#define PCIE_ESM_CAP_5_ESM_19P3G_OFFSET      3
#define PCIE_ESM_CAP_5_ESM_19P3G_MASK        0x8

// Bitfield Description : Indicates if the Port supports ESM Mode at 19.4 GT/s.
#define PCIE_ESM_CAP_5_ESM_19P4G_OFFSET      4
#define PCIE_ESM_CAP_5_ESM_19P4G_MASK        0x10

// Bitfield Description : Indicates if the Port supports ESM Mode at 19.5 GT/s.
#define PCIE_ESM_CAP_5_ESM_19P5G_OFFSET      5
#define PCIE_ESM_CAP_5_ESM_19P5G_MASK        0x20

// Bitfield Description : Indicates if the Port supports ESM Mode at 19.6 GT/s.
#define PCIE_ESM_CAP_5_ESM_19P6G_OFFSET      6
#define PCIE_ESM_CAP_5_ESM_19P6G_MASK        0x40

// Bitfield Description : Indicates if the Port supports ESM Mode at 19.7 GT/s.
#define PCIE_ESM_CAP_5_ESM_19P7G_OFFSET      7
#define PCIE_ESM_CAP_5_ESM_19P7G_MASK        0x80

// Bitfield Description : Indicates if the Port supports ESM Mode at 19.8 GT/s.
#define PCIE_ESM_CAP_5_ESM_19P8G_OFFSET      8
#define PCIE_ESM_CAP_5_ESM_19P8G_MASK        0x100

// Bitfield Description : Indicates if the Port supports ESM Mode at 19.9 GT/s.
#define PCIE_ESM_CAP_5_ESM_19P9G_OFFSET      9
#define PCIE_ESM_CAP_5_ESM_19P9G_MASK        0x200

// Bitfield Description : Indicates if the Port supports ESM Mode at 20.0 GT/s.
#define PCIE_ESM_CAP_5_ESM_20P0G_OFFSET      10
#define PCIE_ESM_CAP_5_ESM_20P0G_MASK        0x400

// Bitfield Description : Indicates if the Port supports ESM Mode at 20.1 GT/s.
#define PCIE_ESM_CAP_5_ESM_20P1G_OFFSET      11
#define PCIE_ESM_CAP_5_ESM_20P1G_MASK        0x800

// Bitfield Description : Indicates if the Port supports ESM Mode at 20.2 GT/s.
#define PCIE_ESM_CAP_5_ESM_20P2G_OFFSET      12
#define PCIE_ESM_CAP_5_ESM_20P2G_MASK        0x1000

// Bitfield Description : Indicates if the Port supports ESM Mode at 20.3 GT/s.
#define PCIE_ESM_CAP_5_ESM_20P3G_OFFSET      13
#define PCIE_ESM_CAP_5_ESM_20P3G_MASK        0x2000

// Bitfield Description : Indicates if the Port supports ESM Mode at 20.4 GT/s.
#define PCIE_ESM_CAP_5_ESM_20P4G_OFFSET      14
#define PCIE_ESM_CAP_5_ESM_20P4G_MASK        0x4000

// Bitfield Description : Indicates if the Port supports ESM Mode at 20.5 GT/s.
#define PCIE_ESM_CAP_5_ESM_20P5G_OFFSET      15
#define PCIE_ESM_CAP_5_ESM_20P5G_MASK        0x8000

// Bitfield Description : Indicates if the Port supports ESM Mode at 20.6 GT/s.
#define PCIE_ESM_CAP_5_ESM_20P6G_OFFSET      16
#define PCIE_ESM_CAP_5_ESM_20P6G_MASK        0x10000

// Bitfield Description : Indicates if the Port supports ESM Mode at 20.7 GT/s.
#define PCIE_ESM_CAP_5_ESM_20P7G_OFFSET      17
#define PCIE_ESM_CAP_5_ESM_20P7G_MASK        0x20000

// Bitfield Description : Indicates if the Port supports ESM Mode at 20.8 GT/s.
#define PCIE_ESM_CAP_5_ESM_20P8G_OFFSET      18
#define PCIE_ESM_CAP_5_ESM_20P8G_MASK        0x40000

// Bitfield Description : Indicates if the Port supports ESM Mode at 20.9 GT/s.
#define PCIE_ESM_CAP_5_ESM_20P9G_OFFSET      19
#define PCIE_ESM_CAP_5_ESM_20P9G_MASK        0x80000

// Bitfield Description : Indicates if the Port supports ESM Mode at 21.0 GT/s.
#define PCIE_ESM_CAP_5_ESM_21P0G_OFFSET      20
#define PCIE_ESM_CAP_5_ESM_21P0G_MASK        0x100000

// Bitfield Description : Indicates if the Port supports ESM Mode at 21.1 GT/s.
#define PCIE_ESM_CAP_5_ESM_21P1G_OFFSET      21
#define PCIE_ESM_CAP_5_ESM_21P1G_MASK        0x200000

// Bitfield Description : Indicates if the Port supports ESM Mode at 21.2 GT/s.
#define PCIE_ESM_CAP_5_ESM_21P2G_OFFSET      22
#define PCIE_ESM_CAP_5_ESM_21P2G_MASK        0x400000

// Bitfield Description : Indicates if the Port supports ESM Mode at 21.3 GT/s.
#define PCIE_ESM_CAP_5_ESM_21P3G_OFFSET      23
#define PCIE_ESM_CAP_5_ESM_21P3G_MASK        0x800000

// Bitfield Description : Indicates if the Port supports ESM Mode at 21.4 GT/s.
#define PCIE_ESM_CAP_5_ESM_21P4G_OFFSET      24
#define PCIE_ESM_CAP_5_ESM_21P4G_MASK        0x1000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 21.5 GT/s.
#define PCIE_ESM_CAP_5_ESM_21P5G_OFFSET      25
#define PCIE_ESM_CAP_5_ESM_21P5G_MASK        0x2000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 21.6 GT/s.
#define PCIE_ESM_CAP_5_ESM_21P6G_OFFSET      26
#define PCIE_ESM_CAP_5_ESM_21P6G_MASK        0x4000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 21.7 GT/s.
#define PCIE_ESM_CAP_5_ESM_21P7G_OFFSET      27
#define PCIE_ESM_CAP_5_ESM_21P7G_MASK        0x8000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 21.8 GT/s.
#define PCIE_ESM_CAP_5_ESM_21P8G_OFFSET      28
#define PCIE_ESM_CAP_5_ESM_21P8G_MASK        0x10000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 21.9 GT/s.
#define PCIE_ESM_CAP_5_ESM_21P9G_OFFSET      29
#define PCIE_ESM_CAP_5_ESM_21P9G_MASK        0x20000000

// Bitfield Description : 
#define PCIE_ESM_CAP_5_Reserved_31_30_OFFSET      30
#define PCIE_ESM_CAP_5_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            ESM_19P0G:1;
    UINT32                            ESM_19P1G:1;
    UINT32                            ESM_19P2G:1;
    UINT32                            ESM_19P3G:1;
    UINT32                            ESM_19P4G:1;
    UINT32                            ESM_19P5G:1;
    UINT32                            ESM_19P6G:1;
    UINT32                            ESM_19P7G:1;
    UINT32                            ESM_19P8G:1;
    UINT32                            ESM_19P9G:1;
    UINT32                            ESM_20P0G:1;
    UINT32                            ESM_20P1G:1;
    UINT32                            ESM_20P2G:1;
    UINT32                            ESM_20P3G:1;
    UINT32                            ESM_20P4G:1;
    UINT32                            ESM_20P5G:1;
    UINT32                            ESM_20P6G:1;
    UINT32                            ESM_20P7G:1;
    UINT32                            ESM_20P8G:1;
    UINT32                            ESM_20P9G:1;
    UINT32                            ESM_21P0G:1;
    UINT32                            ESM_21P1G:1;
    UINT32                            ESM_21P2G:1;
    UINT32                            ESM_21P3G:1;
    UINT32                            ESM_21P4G:1;
    UINT32                            ESM_21P5G:1;
    UINT32                            ESM_21P6G:1;
    UINT32                            ESM_21P7G:1;
    UINT32                            ESM_21P8G:1;
    UINT32                            ESM_21P9G:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} PCIE_ESM_CAP_5_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_CAP_5_ADDRESS    0x111003e4UL

#define SMN_FUNC0_PCIE1_PCIE_ESM_CAP_5_ADDRESS    0x112003e4UL

#define SMN_FUNC0_PCIE2_PCIE_ESM_CAP_5_ADDRESS    0x113003e4UL

#define SMN_FUNC0_PCIE3_PCIE_ESM_CAP_5_ADDRESS    0x114003e4UL

#define SMN_FUNC1_PCIE0_PCIE_ESM_CAP_5_ADDRESS    0x111013e4UL

#define SMN_FUNC1_PCIE1_PCIE_ESM_CAP_5_ADDRESS    0x112013e4UL

#define SMN_FUNC2_PCIE0_PCIE_ESM_CAP_5_ADDRESS    0x111023e4UL

#define SMN_FUNC2_PCIE1_PCIE_ESM_CAP_5_ADDRESS    0x112023e4UL

#define SMN_FUNC3_PCIE0_PCIE_ESM_CAP_5_ADDRESS    0x111033e4UL

#define SMN_FUNC3_PCIE1_PCIE_ESM_CAP_5_ADDRESS    0x112033e4UL

#define SMN_FUNC4_PCIE0_PCIE_ESM_CAP_5_ADDRESS    0x111043e4UL

#define SMN_FUNC4_PCIE1_PCIE_ESM_CAP_5_ADDRESS    0x112043e4UL

#define SMN_FUNC5_PCIE1_PCIE_ESM_CAP_5_ADDRESS    0x112053e4UL


/***********************************************************
* Register Name : PCIE_ESM_CAP_6
* Register Description :
* ESM Capabilities 6 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port supports ESM Mode at 22.0 GT/s.
#define PCIE_ESM_CAP_6_ESM_22P0G_OFFSET      0
#define PCIE_ESM_CAP_6_ESM_22P0G_MASK        0x1

// Bitfield Description : Indicates if the Port supports ESM Mode at 22.1 GT/s.
#define PCIE_ESM_CAP_6_ESM_22P1G_OFFSET      1
#define PCIE_ESM_CAP_6_ESM_22P1G_MASK        0x2

// Bitfield Description : Indicates if the Port supports ESM Mode at 22.2 GT/s.
#define PCIE_ESM_CAP_6_ESM_22P2G_OFFSET      2
#define PCIE_ESM_CAP_6_ESM_22P2G_MASK        0x4

// Bitfield Description : Indicates if the Port supports ESM Mode at 22.3 GT/s.
#define PCIE_ESM_CAP_6_ESM_22P3G_OFFSET      3
#define PCIE_ESM_CAP_6_ESM_22P3G_MASK        0x8

// Bitfield Description : Indicates if the Port supports ESM Mode at 22.4 GT/s.
#define PCIE_ESM_CAP_6_ESM_22P4G_OFFSET      4
#define PCIE_ESM_CAP_6_ESM_22P4G_MASK        0x10

// Bitfield Description : Indicates if the Port supports ESM Mode at 22.5 GT/s.
#define PCIE_ESM_CAP_6_ESM_22P5G_OFFSET      5
#define PCIE_ESM_CAP_6_ESM_22P5G_MASK        0x20

// Bitfield Description : Indicates if the Port supports ESM Mode at 22.6 GT/s.
#define PCIE_ESM_CAP_6_ESM_22P6G_OFFSET      6
#define PCIE_ESM_CAP_6_ESM_22P6G_MASK        0x40

// Bitfield Description : Indicates if the Port supports ESM Mode at 22.7 GT/s.
#define PCIE_ESM_CAP_6_ESM_22P7G_OFFSET      7
#define PCIE_ESM_CAP_6_ESM_22P7G_MASK        0x80

// Bitfield Description : Indicates if the Port supports ESM Mode at 22.8 GT/s.
#define PCIE_ESM_CAP_6_ESM_22P8G_OFFSET      8
#define PCIE_ESM_CAP_6_ESM_22P8G_MASK        0x100

// Bitfield Description : Indicates if the Port supports ESM Mode at 22.9 GT/s.
#define PCIE_ESM_CAP_6_ESM_22P9G_OFFSET      9
#define PCIE_ESM_CAP_6_ESM_22P9G_MASK        0x200

// Bitfield Description : Indicates if the Port supports ESM Mode at 23.0 GT/s.
#define PCIE_ESM_CAP_6_ESM_23P0G_OFFSET      10
#define PCIE_ESM_CAP_6_ESM_23P0G_MASK        0x400

// Bitfield Description : Indicates if the Port supports ESM Mode at 23.1 GT/s.
#define PCIE_ESM_CAP_6_ESM_23P1G_OFFSET      11
#define PCIE_ESM_CAP_6_ESM_23P1G_MASK        0x800

// Bitfield Description : Indicates if the Port supports ESM Mode at 23.2 GT/s.
#define PCIE_ESM_CAP_6_ESM_23P2G_OFFSET      12
#define PCIE_ESM_CAP_6_ESM_23P2G_MASK        0x1000

// Bitfield Description : Indicates if the Port supports ESM Mode at 23.3 GT/s.
#define PCIE_ESM_CAP_6_ESM_23P3G_OFFSET      13
#define PCIE_ESM_CAP_6_ESM_23P3G_MASK        0x2000

// Bitfield Description : Indicates if the Port supports ESM Mode at 23.4 GT/s.
#define PCIE_ESM_CAP_6_ESM_23P4G_OFFSET      14
#define PCIE_ESM_CAP_6_ESM_23P4G_MASK        0x4000

// Bitfield Description : Indicates if the Port supports ESM Mode at 23.5 GT/s.
#define PCIE_ESM_CAP_6_ESM_23P5G_OFFSET      15
#define PCIE_ESM_CAP_6_ESM_23P5G_MASK        0x8000

// Bitfield Description : Indicates if the Port supports ESM Mode at 23.6 GT/s.
#define PCIE_ESM_CAP_6_ESM_23P6G_OFFSET      16
#define PCIE_ESM_CAP_6_ESM_23P6G_MASK        0x10000

// Bitfield Description : Indicates if the Port supports ESM Mode at 23.7 GT/s.
#define PCIE_ESM_CAP_6_ESM_23P7G_OFFSET      17
#define PCIE_ESM_CAP_6_ESM_23P7G_MASK        0x20000

// Bitfield Description : Indicates if the Port supports ESM Mode at 23.8 GT/s.
#define PCIE_ESM_CAP_6_ESM_23P8G_OFFSET      18
#define PCIE_ESM_CAP_6_ESM_23P8G_MASK        0x40000

// Bitfield Description : Indicates if the Port supports ESM Mode at 23.9 GT/s.
#define PCIE_ESM_CAP_6_ESM_23P9G_OFFSET      19
#define PCIE_ESM_CAP_6_ESM_23P9G_MASK        0x80000

// Bitfield Description : Indicates if the Port supports ESM Mode at 24.0 GT/s.
#define PCIE_ESM_CAP_6_ESM_24P0G_OFFSET      20
#define PCIE_ESM_CAP_6_ESM_24P0G_MASK        0x100000

// Bitfield Description : Indicates if the Port supports ESM Mode at 24.1 GT/s.
#define PCIE_ESM_CAP_6_ESM_24P1G_OFFSET      21
#define PCIE_ESM_CAP_6_ESM_24P1G_MASK        0x200000

// Bitfield Description : Indicates if the Port supports ESM Mode at 24.2 GT/s.
#define PCIE_ESM_CAP_6_ESM_24P2G_OFFSET      22
#define PCIE_ESM_CAP_6_ESM_24P2G_MASK        0x400000

// Bitfield Description : Indicates if the Port supports ESM Mode at 24.3 GT/s.
#define PCIE_ESM_CAP_6_ESM_24P3G_OFFSET      23
#define PCIE_ESM_CAP_6_ESM_24P3G_MASK        0x800000

// Bitfield Description : Indicates if the Port supports ESM Mode at 24.4 GT/s.
#define PCIE_ESM_CAP_6_ESM_24P4G_OFFSET      24
#define PCIE_ESM_CAP_6_ESM_24P4G_MASK        0x1000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 24.5 GT/s.
#define PCIE_ESM_CAP_6_ESM_24P5G_OFFSET      25
#define PCIE_ESM_CAP_6_ESM_24P5G_MASK        0x2000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 24.6 GT/s.
#define PCIE_ESM_CAP_6_ESM_24P6G_OFFSET      26
#define PCIE_ESM_CAP_6_ESM_24P6G_MASK        0x4000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 24.7 GT/s.
#define PCIE_ESM_CAP_6_ESM_24P7G_OFFSET      27
#define PCIE_ESM_CAP_6_ESM_24P7G_MASK        0x8000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 24.8 GT/s.
#define PCIE_ESM_CAP_6_ESM_24P8G_OFFSET      28
#define PCIE_ESM_CAP_6_ESM_24P8G_MASK        0x10000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 24.9 GT/s.
#define PCIE_ESM_CAP_6_ESM_24P9G_OFFSET      29
#define PCIE_ESM_CAP_6_ESM_24P9G_MASK        0x20000000

// Bitfield Description : 
#define PCIE_ESM_CAP_6_Reserved_31_30_OFFSET      30
#define PCIE_ESM_CAP_6_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            ESM_22P0G:1;
    UINT32                            ESM_22P1G:1;
    UINT32                            ESM_22P2G:1;
    UINT32                            ESM_22P3G:1;
    UINT32                            ESM_22P4G:1;
    UINT32                            ESM_22P5G:1;
    UINT32                            ESM_22P6G:1;
    UINT32                            ESM_22P7G:1;
    UINT32                            ESM_22P8G:1;
    UINT32                            ESM_22P9G:1;
    UINT32                            ESM_23P0G:1;
    UINT32                            ESM_23P1G:1;
    UINT32                            ESM_23P2G:1;
    UINT32                            ESM_23P3G:1;
    UINT32                            ESM_23P4G:1;
    UINT32                            ESM_23P5G:1;
    UINT32                            ESM_23P6G:1;
    UINT32                            ESM_23P7G:1;
    UINT32                            ESM_23P8G:1;
    UINT32                            ESM_23P9G:1;
    UINT32                            ESM_24P0G:1;
    UINT32                            ESM_24P1G:1;
    UINT32                            ESM_24P2G:1;
    UINT32                            ESM_24P3G:1;
    UINT32                            ESM_24P4G:1;
    UINT32                            ESM_24P5G:1;
    UINT32                            ESM_24P6G:1;
    UINT32                            ESM_24P7G:1;
    UINT32                            ESM_24P8G:1;
    UINT32                            ESM_24P9G:1;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} PCIE_ESM_CAP_6_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_CAP_6_ADDRESS    0x111003e8UL

#define SMN_FUNC0_PCIE1_PCIE_ESM_CAP_6_ADDRESS    0x112003e8UL

#define SMN_FUNC0_PCIE2_PCIE_ESM_CAP_6_ADDRESS    0x113003e8UL

#define SMN_FUNC0_PCIE3_PCIE_ESM_CAP_6_ADDRESS    0x114003e8UL

#define SMN_FUNC1_PCIE0_PCIE_ESM_CAP_6_ADDRESS    0x111013e8UL

#define SMN_FUNC1_PCIE1_PCIE_ESM_CAP_6_ADDRESS    0x112013e8UL

#define SMN_FUNC2_PCIE0_PCIE_ESM_CAP_6_ADDRESS    0x111023e8UL

#define SMN_FUNC2_PCIE1_PCIE_ESM_CAP_6_ADDRESS    0x112023e8UL

#define SMN_FUNC3_PCIE0_PCIE_ESM_CAP_6_ADDRESS    0x111033e8UL

#define SMN_FUNC3_PCIE1_PCIE_ESM_CAP_6_ADDRESS    0x112033e8UL

#define SMN_FUNC4_PCIE0_PCIE_ESM_CAP_6_ADDRESS    0x111043e8UL

#define SMN_FUNC4_PCIE1_PCIE_ESM_CAP_6_ADDRESS    0x112043e8UL

#define SMN_FUNC5_PCIE1_PCIE_ESM_CAP_6_ADDRESS    0x112053e8UL


/***********************************************************
* Register Name : PCIE_ESM_CAP_7
* Register Description :
* ESM Capabilities 7 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port supports ESM Mode at 25.0 GT/s.
#define PCIE_ESM_CAP_7_ESM_25P0G_OFFSET      0
#define PCIE_ESM_CAP_7_ESM_25P0G_MASK        0x1

// Bitfield Description : Indicates if the Port supports ESM Mode at 25.1 GT/s.
#define PCIE_ESM_CAP_7_ESM_25P1G_OFFSET      1
#define PCIE_ESM_CAP_7_ESM_25P1G_MASK        0x2

// Bitfield Description : Indicates if the Port supports ESM Mode at 25.2 GT/s.
#define PCIE_ESM_CAP_7_ESM_25P2G_OFFSET      2
#define PCIE_ESM_CAP_7_ESM_25P2G_MASK        0x4

// Bitfield Description : Indicates if the Port supports ESM Mode at 25.3 GT/s.
#define PCIE_ESM_CAP_7_ESM_25P3G_OFFSET      3
#define PCIE_ESM_CAP_7_ESM_25P3G_MASK        0x8

// Bitfield Description : Indicates if the Port supports ESM Mode at 25.4 GT/s.
#define PCIE_ESM_CAP_7_ESM_25P4G_OFFSET      4
#define PCIE_ESM_CAP_7_ESM_25P4G_MASK        0x10

// Bitfield Description : Indicates if the Port supports ESM Mode at 25.5 GT/s.
#define PCIE_ESM_CAP_7_ESM_25P5G_OFFSET      5
#define PCIE_ESM_CAP_7_ESM_25P5G_MASK        0x20

// Bitfield Description : Indicates if the Port supports ESM Mode at 25.6 GT/s.
#define PCIE_ESM_CAP_7_ESM_25P6G_OFFSET      6
#define PCIE_ESM_CAP_7_ESM_25P6G_MASK        0x40

// Bitfield Description : Indicates if the Port supports ESM Mode at 25.7 GT/s.
#define PCIE_ESM_CAP_7_ESM_25P7G_OFFSET      7
#define PCIE_ESM_CAP_7_ESM_25P7G_MASK        0x80

// Bitfield Description : Indicates if the Port supports ESM Mode at 25.8 GT/s.
#define PCIE_ESM_CAP_7_ESM_25P8G_OFFSET      8
#define PCIE_ESM_CAP_7_ESM_25P8G_MASK        0x100

// Bitfield Description : Indicates if the Port supports ESM Mode at 25.9 GT/s.
#define PCIE_ESM_CAP_7_ESM_25P9G_OFFSET      9
#define PCIE_ESM_CAP_7_ESM_25P9G_MASK        0x200

// Bitfield Description : Indicates if the Port supports ESM Mode at 26.0 GT/s.
#define PCIE_ESM_CAP_7_ESM_26P0G_OFFSET      10
#define PCIE_ESM_CAP_7_ESM_26P0G_MASK        0x400

// Bitfield Description : Indicates if the Port supports ESM Mode at 26.1 GT/s.
#define PCIE_ESM_CAP_7_ESM_26P1G_OFFSET      11
#define PCIE_ESM_CAP_7_ESM_26P1G_MASK        0x800

// Bitfield Description : Indicates if the Port supports ESM Mode at 26.2 GT/s.
#define PCIE_ESM_CAP_7_ESM_26P2G_OFFSET      12
#define PCIE_ESM_CAP_7_ESM_26P2G_MASK        0x1000

// Bitfield Description : Indicates if the Port supports ESM Mode at 26.3 GT/s.
#define PCIE_ESM_CAP_7_ESM_26P3G_OFFSET      13
#define PCIE_ESM_CAP_7_ESM_26P3G_MASK        0x2000

// Bitfield Description : Indicates if the Port supports ESM Mode at 26.4 GT/s.
#define PCIE_ESM_CAP_7_ESM_26P4G_OFFSET      14
#define PCIE_ESM_CAP_7_ESM_26P4G_MASK        0x4000

// Bitfield Description : Indicates if the Port supports ESM Mode at 26.5 GT/s.
#define PCIE_ESM_CAP_7_ESM_26P5G_OFFSET      15
#define PCIE_ESM_CAP_7_ESM_26P5G_MASK        0x8000

// Bitfield Description : Indicates if the Port supports ESM Mode at 26.6 GT/s.
#define PCIE_ESM_CAP_7_ESM_26P6G_OFFSET      16
#define PCIE_ESM_CAP_7_ESM_26P6G_MASK        0x10000

// Bitfield Description : Indicates if the Port supports ESM Mode at 26.7 GT/s.
#define PCIE_ESM_CAP_7_ESM_26P7G_OFFSET      17
#define PCIE_ESM_CAP_7_ESM_26P7G_MASK        0x20000

// Bitfield Description : Indicates if the Port supports ESM Mode at 26.8 GT/s.
#define PCIE_ESM_CAP_7_ESM_26P8G_OFFSET      18
#define PCIE_ESM_CAP_7_ESM_26P8G_MASK        0x40000

// Bitfield Description : Indicates if the Port supports ESM Mode at 26.9 GT/s.
#define PCIE_ESM_CAP_7_ESM_26P9G_OFFSET      19
#define PCIE_ESM_CAP_7_ESM_26P9G_MASK        0x80000

// Bitfield Description : Indicates if the Port supports ESM Mode at 27.0 GT/s.
#define PCIE_ESM_CAP_7_ESM_27P0G_OFFSET      20
#define PCIE_ESM_CAP_7_ESM_27P0G_MASK        0x100000

// Bitfield Description : Indicates if the Port supports ESM Mode at 27.1 GT/s.
#define PCIE_ESM_CAP_7_ESM_27P1G_OFFSET      21
#define PCIE_ESM_CAP_7_ESM_27P1G_MASK        0x200000

// Bitfield Description : Indicates if the Port supports ESM Mode at 27.2 GT/s.
#define PCIE_ESM_CAP_7_ESM_27P2G_OFFSET      22
#define PCIE_ESM_CAP_7_ESM_27P2G_MASK        0x400000

// Bitfield Description : Indicates if the Port supports ESM Mode at 27.3 GT/s.
#define PCIE_ESM_CAP_7_ESM_27P3G_OFFSET      23
#define PCIE_ESM_CAP_7_ESM_27P3G_MASK        0x800000

// Bitfield Description : Indicates if the Port supports ESM Mode at 27.4 GT/s.
#define PCIE_ESM_CAP_7_ESM_27P4G_OFFSET      24
#define PCIE_ESM_CAP_7_ESM_27P4G_MASK        0x1000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 27.5 GT/s.
#define PCIE_ESM_CAP_7_ESM_27P5G_OFFSET      25
#define PCIE_ESM_CAP_7_ESM_27P5G_MASK        0x2000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 27.6 GT/s.
#define PCIE_ESM_CAP_7_ESM_27P6G_OFFSET      26
#define PCIE_ESM_CAP_7_ESM_27P6G_MASK        0x4000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 27.7 GT/s.
#define PCIE_ESM_CAP_7_ESM_27P7G_OFFSET      27
#define PCIE_ESM_CAP_7_ESM_27P7G_MASK        0x8000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 27.8 GT/s.
#define PCIE_ESM_CAP_7_ESM_27P8G_OFFSET      28
#define PCIE_ESM_CAP_7_ESM_27P8G_MASK        0x10000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 27.9 GT/s.
#define PCIE_ESM_CAP_7_ESM_27P9G_OFFSET      29
#define PCIE_ESM_CAP_7_ESM_27P9G_MASK        0x20000000

// Bitfield Description : Indicates if the Port supports ESM Mode at 28.0 GT/s.
#define PCIE_ESM_CAP_7_ESM_28P0G_OFFSET      30
#define PCIE_ESM_CAP_7_ESM_28P0G_MASK        0x40000000

// Bitfield Description : 
#define PCIE_ESM_CAP_7_Reserved_31_31_OFFSET      31
#define PCIE_ESM_CAP_7_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            ESM_25P0G:1;
    UINT32                            ESM_25P1G:1;
    UINT32                            ESM_25P2G:1;
    UINT32                            ESM_25P3G:1;
    UINT32                            ESM_25P4G:1;
    UINT32                            ESM_25P5G:1;
    UINT32                            ESM_25P6G:1;
    UINT32                            ESM_25P7G:1;
    UINT32                            ESM_25P8G:1;
    UINT32                            ESM_25P9G:1;
    UINT32                            ESM_26P0G:1;
    UINT32                            ESM_26P1G:1;
    UINT32                            ESM_26P2G:1;
    UINT32                            ESM_26P3G:1;
    UINT32                            ESM_26P4G:1;
    UINT32                            ESM_26P5G:1;
    UINT32                            ESM_26P6G:1;
    UINT32                            ESM_26P7G:1;
    UINT32                            ESM_26P8G:1;
    UINT32                            ESM_26P9G:1;
    UINT32                            ESM_27P0G:1;
    UINT32                            ESM_27P1G:1;
    UINT32                            ESM_27P2G:1;
    UINT32                            ESM_27P3G:1;
    UINT32                            ESM_27P4G:1;
    UINT32                            ESM_27P5G:1;
    UINT32                            ESM_27P6G:1;
    UINT32                            ESM_27P7G:1;
    UINT32                            ESM_27P8G:1;
    UINT32                            ESM_27P9G:1;
    UINT32                            ESM_28P0G:1;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} PCIE_ESM_CAP_7_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_CAP_7_ADDRESS    0x111003ecUL

#define SMN_FUNC0_PCIE1_PCIE_ESM_CAP_7_ADDRESS    0x112003ecUL

#define SMN_FUNC0_PCIE2_PCIE_ESM_CAP_7_ADDRESS    0x113003ecUL

#define SMN_FUNC0_PCIE3_PCIE_ESM_CAP_7_ADDRESS    0x114003ecUL

#define SMN_FUNC1_PCIE0_PCIE_ESM_CAP_7_ADDRESS    0x111013ecUL

#define SMN_FUNC1_PCIE1_PCIE_ESM_CAP_7_ADDRESS    0x112013ecUL

#define SMN_FUNC2_PCIE0_PCIE_ESM_CAP_7_ADDRESS    0x111023ecUL

#define SMN_FUNC2_PCIE1_PCIE_ESM_CAP_7_ADDRESS    0x112023ecUL

#define SMN_FUNC3_PCIE0_PCIE_ESM_CAP_7_ADDRESS    0x111033ecUL

#define SMN_FUNC3_PCIE1_PCIE_ESM_CAP_7_ADDRESS    0x112033ecUL

#define SMN_FUNC4_PCIE0_PCIE_ESM_CAP_7_ADDRESS    0x111043ecUL

#define SMN_FUNC4_PCIE1_PCIE_ESM_CAP_7_ADDRESS    0x112043ecUL

#define SMN_FUNC5_PCIE1_PCIE_ESM_CAP_7_ADDRESS    0x112053ecUL


/***********************************************************
* Register Name : PCIE_ESM_CAP_LIST
* Register Description :
* Designated Vendor-Specific Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_ESM_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_ESM_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_ESM_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_ESM_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_ESM_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_ESM_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_ESM_CAP_LIST_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_CAP_LIST_ADDRESS    0x111003c4UL

#define SMN_FUNC0_PCIE1_PCIE_ESM_CAP_LIST_ADDRESS    0x112003c4UL

#define SMN_FUNC0_PCIE2_PCIE_ESM_CAP_LIST_ADDRESS    0x113003c4UL

#define SMN_FUNC0_PCIE3_PCIE_ESM_CAP_LIST_ADDRESS    0x114003c4UL

#define SMN_FUNC1_PCIE0_PCIE_ESM_CAP_LIST_ADDRESS    0x111013c4UL

#define SMN_FUNC1_PCIE1_PCIE_ESM_CAP_LIST_ADDRESS    0x112013c4UL

#define SMN_FUNC2_PCIE0_PCIE_ESM_CAP_LIST_ADDRESS    0x111023c4UL

#define SMN_FUNC2_PCIE1_PCIE_ESM_CAP_LIST_ADDRESS    0x112023c4UL

#define SMN_FUNC3_PCIE0_PCIE_ESM_CAP_LIST_ADDRESS    0x111033c4UL

#define SMN_FUNC3_PCIE1_PCIE_ESM_CAP_LIST_ADDRESS    0x112033c4UL

#define SMN_FUNC4_PCIE0_PCIE_ESM_CAP_LIST_ADDRESS    0x111043c4UL

#define SMN_FUNC4_PCIE1_PCIE_ESM_CAP_LIST_ADDRESS    0x112043c4UL

#define SMN_FUNC5_PCIE1_PCIE_ESM_CAP_LIST_ADDRESS    0x112053c4UL


/***********************************************************
* Register Name : PCIE_ESM_CTRL
* Register Description :
* ESM Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When ESM Enable bit is set, controls the data rate of the Link when speed changes negotiate to the Gen 3 data rate. The data rate equals (64 + the value in this field) x 100x10e6.
#define PCIE_ESM_CTRL_ESM_GEN_3_DATA_RATE_OFFSET      0
#define PCIE_ESM_CTRL_ESM_GEN_3_DATA_RATE_MASK        0x7f

// Bitfield Description : 
#define PCIE_ESM_CTRL_Reserved_7_7_OFFSET      7
#define PCIE_ESM_CTRL_Reserved_7_7_MASK        0x80

// Bitfield Description : When ESM Enable bit is set, controls the data rate of the Link when speed changes negotiate to the Gen 4 data rate. The data rate equals (128 + the value in this field) x 100x10e6.
#define PCIE_ESM_CTRL_ESM_GEN_4_DATA_RATE_OFFSET      8
#define PCIE_ESM_CTRL_ESM_GEN_4_DATA_RATE_MASK        0x7f00

// Bitfield Description : Writes of 1 triggers the sequence that configures the Port to run in ESM mode. Writes of 0 have no effect, except when the bit is already set, in which case the resulting behavior is undefined.
#define PCIE_ESM_CTRL_ESM_ENABLED_OFFSET      15
#define PCIE_ESM_CTRL_ESM_ENABLED_MASK        0x8000

typedef union {
  struct {
    UINT16                            ESM_GEN_3_DATA_RATE:7;
    UINT16                            Reserved_7_7:1;
    UINT16                            ESM_GEN_4_DATA_RATE:7;
    UINT16                            ESM_ENABLED:1;
  } Field;
  UINT16 Value;
} PCIE_ESM_CTRL_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_CTRL_ADDRESS    0x111003d0UL

#define SMN_FUNC0_PCIE1_PCIE_ESM_CTRL_ADDRESS    0x112003d0UL

#define SMN_FUNC0_PCIE2_PCIE_ESM_CTRL_ADDRESS    0x113003d0UL

#define SMN_FUNC0_PCIE3_PCIE_ESM_CTRL_ADDRESS    0x114003d0UL

#define SMN_FUNC1_PCIE0_PCIE_ESM_CTRL_ADDRESS    0x111013d0UL

#define SMN_FUNC1_PCIE1_PCIE_ESM_CTRL_ADDRESS    0x112013d0UL

#define SMN_FUNC2_PCIE0_PCIE_ESM_CTRL_ADDRESS    0x111023d0UL

#define SMN_FUNC2_PCIE1_PCIE_ESM_CTRL_ADDRESS    0x112023d0UL

#define SMN_FUNC3_PCIE0_PCIE_ESM_CTRL_ADDRESS    0x111033d0UL

#define SMN_FUNC3_PCIE1_PCIE_ESM_CTRL_ADDRESS    0x112033d0UL

#define SMN_FUNC4_PCIE0_PCIE_ESM_CTRL_ADDRESS    0x111043d0UL

#define SMN_FUNC4_PCIE1_PCIE_ESM_CTRL_ADDRESS    0x112043d0UL

#define SMN_FUNC5_PCIE1_PCIE_ESM_CTRL_ADDRESS    0x112053d0UL


/***********************************************************
* Register Name : PCIE_ESM_HEADER_1
* Register Description :
* Designated Vendor-Specific Header 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor-defined ID number. This value is AMD.
#define PCIE_ESM_HEADER_1_ESM_VENDOR_ID_OFFSET      0
#define PCIE_ESM_HEADER_1_ESM_VENDOR_ID_MASK        0xffff

// Bitfield Description : Vendor-defined revision number.
#define PCIE_ESM_HEADER_1_ESM_CAP_REV_OFFSET      16
#define PCIE_ESM_HEADER_1_ESM_CAP_REV_MASK        0xf0000

// Bitfield Description : Number of bytes in the entire DVSEC structure.
#define PCIE_ESM_HEADER_1_ESM_CAP_LEN_OFFSET      20
#define PCIE_ESM_HEADER_1_ESM_CAP_LEN_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            ESM_VENDOR_ID:16;
    UINT32                            ESM_CAP_REV:4;
    UINT32                            ESM_CAP_LEN:12;
  } Field;
  UINT32 Value;
} PCIE_ESM_HEADER_1_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_HEADER_1_ADDRESS    0x111003c8UL

#define SMN_FUNC0_PCIE1_PCIE_ESM_HEADER_1_ADDRESS    0x112003c8UL

#define SMN_FUNC0_PCIE2_PCIE_ESM_HEADER_1_ADDRESS    0x113003c8UL

#define SMN_FUNC0_PCIE3_PCIE_ESM_HEADER_1_ADDRESS    0x114003c8UL

#define SMN_FUNC1_PCIE0_PCIE_ESM_HEADER_1_ADDRESS    0x111013c8UL

#define SMN_FUNC1_PCIE1_PCIE_ESM_HEADER_1_ADDRESS    0x112013c8UL

#define SMN_FUNC2_PCIE0_PCIE_ESM_HEADER_1_ADDRESS    0x111023c8UL

#define SMN_FUNC2_PCIE1_PCIE_ESM_HEADER_1_ADDRESS    0x112023c8UL

#define SMN_FUNC3_PCIE0_PCIE_ESM_HEADER_1_ADDRESS    0x111033c8UL

#define SMN_FUNC3_PCIE1_PCIE_ESM_HEADER_1_ADDRESS    0x112033c8UL

#define SMN_FUNC4_PCIE0_PCIE_ESM_HEADER_1_ADDRESS    0x111043c8UL

#define SMN_FUNC4_PCIE1_PCIE_ESM_HEADER_1_ADDRESS    0x112043c8UL

#define SMN_FUNC5_PCIE1_PCIE_ESM_HEADER_1_ADDRESS    0x112053c8UL


/***********************************************************
* Register Name : PCIE_ESM_HEADER_2
* Register Description :
* Designated Vendor-Specific Header 2 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor-defined ID number that indicates the nature and format of the DVSEC structure. This value is ESM capability structure.
#define PCIE_ESM_HEADER_2_CAP_ID_OFFSET      0
#define PCIE_ESM_HEADER_2_CAP_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            CAP_ID:16;
  } Field;
  UINT16 Value;
} PCIE_ESM_HEADER_2_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_HEADER_2_ADDRESS    0x111003ccUL

#define SMN_FUNC0_PCIE1_PCIE_ESM_HEADER_2_ADDRESS    0x112003ccUL

#define SMN_FUNC0_PCIE2_PCIE_ESM_HEADER_2_ADDRESS    0x113003ccUL

#define SMN_FUNC0_PCIE3_PCIE_ESM_HEADER_2_ADDRESS    0x114003ccUL

#define SMN_FUNC1_PCIE0_PCIE_ESM_HEADER_2_ADDRESS    0x111013ccUL

#define SMN_FUNC1_PCIE1_PCIE_ESM_HEADER_2_ADDRESS    0x112013ccUL

#define SMN_FUNC2_PCIE0_PCIE_ESM_HEADER_2_ADDRESS    0x111023ccUL

#define SMN_FUNC2_PCIE1_PCIE_ESM_HEADER_2_ADDRESS    0x112023ccUL

#define SMN_FUNC3_PCIE0_PCIE_ESM_HEADER_2_ADDRESS    0x111033ccUL

#define SMN_FUNC3_PCIE1_PCIE_ESM_HEADER_2_ADDRESS    0x112033ccUL

#define SMN_FUNC4_PCIE0_PCIE_ESM_HEADER_2_ADDRESS    0x111043ccUL

#define SMN_FUNC4_PCIE1_PCIE_ESM_HEADER_2_ADDRESS    0x112043ccUL

#define SMN_FUNC5_PCIE1_PCIE_ESM_HEADER_2_ADDRESS    0x112053ccUL


/***********************************************************
* Register Name : PCIE_ESM_STATUS
* Register Description :
* ESM Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Min Time in EI Scale value, indicates the minimum time the Port's receivers need to see Electrical Idle during receiver calibration.
#define PCIE_ESM_STATUS_MIN_TIME_IN_EI_VAL_OFFSET      0
#define PCIE_ESM_STATUS_MIN_TIME_IN_EI_VAL_MASK        0x1ff

// Bitfield Description : Indicates the scale used for the Min Time in EI Value.
#define PCIE_ESM_STATUS_MIN_TIME_IN_EI_SCALE_OFFSET      9
#define PCIE_ESM_STATUS_MIN_TIME_IN_EI_SCALE_MASK        0xe00

// Bitfield Description : 
#define PCIE_ESM_STATUS_Reserved_15_12_OFFSET      12
#define PCIE_ESM_STATUS_Reserved_15_12_MASK        0xf000

typedef union {
  struct {
    UINT16                            MIN_TIME_IN_EI_VAL:9;
    UINT16                            MIN_TIME_IN_EI_SCALE:3;
    UINT16                            Reserved_15_12:4;
  } Field;
  UINT16 Value;
} PCIE_ESM_STATUS_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ESM_STATUS_ADDRESS    0x111003ceUL

#define SMN_FUNC0_PCIE1_PCIE_ESM_STATUS_ADDRESS    0x112003ceUL

#define SMN_FUNC0_PCIE2_PCIE_ESM_STATUS_ADDRESS    0x113003ceUL

#define SMN_FUNC0_PCIE3_PCIE_ESM_STATUS_ADDRESS    0x114003ceUL

#define SMN_FUNC1_PCIE0_PCIE_ESM_STATUS_ADDRESS    0x111013ceUL

#define SMN_FUNC1_PCIE1_PCIE_ESM_STATUS_ADDRESS    0x112013ceUL

#define SMN_FUNC2_PCIE0_PCIE_ESM_STATUS_ADDRESS    0x111023ceUL

#define SMN_FUNC2_PCIE1_PCIE_ESM_STATUS_ADDRESS    0x112023ceUL

#define SMN_FUNC3_PCIE0_PCIE_ESM_STATUS_ADDRESS    0x111033ceUL

#define SMN_FUNC3_PCIE1_PCIE_ESM_STATUS_ADDRESS    0x112033ceUL

#define SMN_FUNC4_PCIE0_PCIE_ESM_STATUS_ADDRESS    0x111043ceUL

#define SMN_FUNC4_PCIE1_PCIE_ESM_STATUS_ADDRESS    0x112043ceUL

#define SMN_FUNC5_PCIE1_PCIE_ESM_STATUS_ADDRESS    0x112053ceUL


/***********************************************************
* Register Name : PCIE_HDR_LOG0
* Register Description :
* Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TLP Header 1st DW captures the first DW of the Header for the TLP corresponding to a detected error. This field is sticky.
#define PCIE_HDR_LOG0_TLP_HDR_OFFSET      0
#define PCIE_HDR_LOG0_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_HDR_LOG0_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_HDR_LOG0_ADDRESS    0x1110016cUL

#define SMN_FUNC0_PCIE1_PCIE_HDR_LOG0_ADDRESS    0x1120016cUL

#define SMN_FUNC0_PCIE2_PCIE_HDR_LOG0_ADDRESS    0x1130016cUL

#define SMN_FUNC0_PCIE3_PCIE_HDR_LOG0_ADDRESS    0x1140016cUL

#define SMN_FUNC1_PCIE0_PCIE_HDR_LOG0_ADDRESS    0x1110116cUL

#define SMN_FUNC1_PCIE1_PCIE_HDR_LOG0_ADDRESS    0x1120116cUL

#define SMN_FUNC2_PCIE0_PCIE_HDR_LOG0_ADDRESS    0x1110216cUL

#define SMN_FUNC2_PCIE1_PCIE_HDR_LOG0_ADDRESS    0x1120216cUL

#define SMN_FUNC3_PCIE0_PCIE_HDR_LOG0_ADDRESS    0x1110316cUL

#define SMN_FUNC3_PCIE1_PCIE_HDR_LOG0_ADDRESS    0x1120316cUL

#define SMN_FUNC4_PCIE0_PCIE_HDR_LOG0_ADDRESS    0x1110416cUL

#define SMN_FUNC4_PCIE1_PCIE_HDR_LOG0_ADDRESS    0x1120416cUL

#define SMN_FUNC5_PCIE1_PCIE_HDR_LOG0_ADDRESS    0x1120516cUL


/***********************************************************
* Register Name : PCIE_HDR_LOG1
* Register Description :
* Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TLP Header 2nd DW captures the second DW of the Header for the TLP corresponding to a detected error. This field is sticky.
#define PCIE_HDR_LOG1_TLP_HDR_OFFSET      0
#define PCIE_HDR_LOG1_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_HDR_LOG1_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_HDR_LOG1_ADDRESS    0x11100170UL

#define SMN_FUNC0_PCIE1_PCIE_HDR_LOG1_ADDRESS    0x11200170UL

#define SMN_FUNC0_PCIE2_PCIE_HDR_LOG1_ADDRESS    0x11300170UL

#define SMN_FUNC0_PCIE3_PCIE_HDR_LOG1_ADDRESS    0x11400170UL

#define SMN_FUNC1_PCIE0_PCIE_HDR_LOG1_ADDRESS    0x11101170UL

#define SMN_FUNC1_PCIE1_PCIE_HDR_LOG1_ADDRESS    0x11201170UL

#define SMN_FUNC2_PCIE0_PCIE_HDR_LOG1_ADDRESS    0x11102170UL

#define SMN_FUNC2_PCIE1_PCIE_HDR_LOG1_ADDRESS    0x11202170UL

#define SMN_FUNC3_PCIE0_PCIE_HDR_LOG1_ADDRESS    0x11103170UL

#define SMN_FUNC3_PCIE1_PCIE_HDR_LOG1_ADDRESS    0x11203170UL

#define SMN_FUNC4_PCIE0_PCIE_HDR_LOG1_ADDRESS    0x11104170UL

#define SMN_FUNC4_PCIE1_PCIE_HDR_LOG1_ADDRESS    0x11204170UL

#define SMN_FUNC5_PCIE1_PCIE_HDR_LOG1_ADDRESS    0x11205170UL


/***********************************************************
* Register Name : PCIE_HDR_LOG2
* Register Description :
* Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TLP Header 3rd DW captures the third DW of the Header for the TLP corresponding to a detected error. This field is sticky.
#define PCIE_HDR_LOG2_TLP_HDR_OFFSET      0
#define PCIE_HDR_LOG2_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_HDR_LOG2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_HDR_LOG2_ADDRESS    0x11100174UL

#define SMN_FUNC0_PCIE1_PCIE_HDR_LOG2_ADDRESS    0x11200174UL

#define SMN_FUNC0_PCIE2_PCIE_HDR_LOG2_ADDRESS    0x11300174UL

#define SMN_FUNC0_PCIE3_PCIE_HDR_LOG2_ADDRESS    0x11400174UL

#define SMN_FUNC1_PCIE0_PCIE_HDR_LOG2_ADDRESS    0x11101174UL

#define SMN_FUNC1_PCIE1_PCIE_HDR_LOG2_ADDRESS    0x11201174UL

#define SMN_FUNC2_PCIE0_PCIE_HDR_LOG2_ADDRESS    0x11102174UL

#define SMN_FUNC2_PCIE1_PCIE_HDR_LOG2_ADDRESS    0x11202174UL

#define SMN_FUNC3_PCIE0_PCIE_HDR_LOG2_ADDRESS    0x11103174UL

#define SMN_FUNC3_PCIE1_PCIE_HDR_LOG2_ADDRESS    0x11203174UL

#define SMN_FUNC4_PCIE0_PCIE_HDR_LOG2_ADDRESS    0x11104174UL

#define SMN_FUNC4_PCIE1_PCIE_HDR_LOG2_ADDRESS    0x11204174UL

#define SMN_FUNC5_PCIE1_PCIE_HDR_LOG2_ADDRESS    0x11205174UL


/***********************************************************
* Register Name : PCIE_HDR_LOG3
* Register Description :
* Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TLP Header 4th DW captures the fourth DW (if present) of the Header for the TLP corresponding to a detected error. This field is sticky.
#define PCIE_HDR_LOG3_TLP_HDR_OFFSET      0
#define PCIE_HDR_LOG3_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_HDR_LOG3_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_HDR_LOG3_ADDRESS    0x11100178UL

#define SMN_FUNC0_PCIE1_PCIE_HDR_LOG3_ADDRESS    0x11200178UL

#define SMN_FUNC0_PCIE2_PCIE_HDR_LOG3_ADDRESS    0x11300178UL

#define SMN_FUNC0_PCIE3_PCIE_HDR_LOG3_ADDRESS    0x11400178UL

#define SMN_FUNC1_PCIE0_PCIE_HDR_LOG3_ADDRESS    0x11101178UL

#define SMN_FUNC1_PCIE1_PCIE_HDR_LOG3_ADDRESS    0x11201178UL

#define SMN_FUNC2_PCIE0_PCIE_HDR_LOG3_ADDRESS    0x11102178UL

#define SMN_FUNC2_PCIE1_PCIE_HDR_LOG3_ADDRESS    0x11202178UL

#define SMN_FUNC3_PCIE0_PCIE_HDR_LOG3_ADDRESS    0x11103178UL

#define SMN_FUNC3_PCIE1_PCIE_HDR_LOG3_ADDRESS    0x11203178UL

#define SMN_FUNC4_PCIE0_PCIE_HDR_LOG3_ADDRESS    0x11104178UL

#define SMN_FUNC4_PCIE1_PCIE_HDR_LOG3_ADDRESS    0x11204178UL

#define SMN_FUNC5_PCIE1_PCIE_HDR_LOG3_ADDRESS    0x11205178UL


/***********************************************************
* Register Name : PCIE_L1_PM_SUB_CAP
* Register Description :
* L1 PM Substates Capabilities register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if PCI-PM L1.2 is supported.
#define PCIE_L1_PM_SUB_CAP_PCI_PM_L1_2_SUPPORTED_OFFSET      0
#define PCIE_L1_PM_SUB_CAP_PCI_PM_L1_2_SUPPORTED_MASK        0x1

// Bitfield Description : Indicates if PCI-PM L1.1 is supported.
#define PCIE_L1_PM_SUB_CAP_PCI_PM_L1_1_SUPPORTED_OFFSET      1
#define PCIE_L1_PM_SUB_CAP_PCI_PM_L1_1_SUPPORTED_MASK        0x2

// Bitfield Description : Indicates if ASPM L1.2 is supported.
#define PCIE_L1_PM_SUB_CAP_ASPM_L1_2_SUPPORTED_OFFSET      2
#define PCIE_L1_PM_SUB_CAP_ASPM_L1_2_SUPPORTED_MASK        0x4

// Bitfield Description : Indicates if ASPM L1.1 is supported.
#define PCIE_L1_PM_SUB_CAP_ASPM_L1_1_SUPPORTED_OFFSET      3
#define PCIE_L1_PM_SUB_CAP_ASPM_L1_1_SUPPORTED_MASK        0x8

// Bitfield Description : Indicates if the Port supports L1 PM Substates.
#define PCIE_L1_PM_SUB_CAP_L1_PM_SUB_SUPPORTED_OFFSET      4
#define PCIE_L1_PM_SUB_CAP_L1_PM_SUB_SUPPORTED_MASK        0x10

// Bitfield Description : (Downstream Ports only) Indicates if the Port supports Link Activation.
// For Upstream Ports this field is Reserved.
#define PCIE_L1_PM_SUB_CAP_LINK_ACTIVATION_SUPPORTED_OFFSET      5
#define PCIE_L1_PM_SUB_CAP_LINK_ACTIVATION_SUPPORTED_MASK        0x20

// Bitfield Description : 
#define PCIE_L1_PM_SUB_CAP_Reserved_7_6_OFFSET      6
#define PCIE_L1_PM_SUB_CAP_Reserved_7_6_MASK        0xc0

// Bitfield Description : Indicates time (in us) required for the Port to re-establish common mode.
#define PCIE_L1_PM_SUB_CAP_PORT_CM_RESTORE_TIME_OFFSET      8
#define PCIE_L1_PM_SUB_CAP_PORT_CM_RESTORE_TIME_MASK        0xff00

// Bitfield Description : Indicates the scale used for the Port T_POWER_ON Value.
#define PCIE_L1_PM_SUB_CAP_PORT_T_POWER_ON_SCALE_OFFSET      16
#define PCIE_L1_PM_SUB_CAP_PORT_T_POWER_ON_SCALE_MASK        0x30000

// Bitfield Description : 
#define PCIE_L1_PM_SUB_CAP_Reserved_18_18_OFFSET      18
#define PCIE_L1_PM_SUB_CAP_Reserved_18_18_MASK        0x40000

// Bitfield Description : In combination with the Port T_POWER_ON Scale value, indicates the time (in us) that the Port requires the other side of the Link to wait in L1.2.Exit after sampling CLKREQ# asserted before actively driving the interface.
#define PCIE_L1_PM_SUB_CAP_PORT_T_POWER_ON_VALUE_OFFSET      19
#define PCIE_L1_PM_SUB_CAP_PORT_T_POWER_ON_VALUE_MASK        0xf80000

// Bitfield Description : 
#define PCIE_L1_PM_SUB_CAP_Reserved_31_24_OFFSET      24
#define PCIE_L1_PM_SUB_CAP_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            PCI_PM_L1_2_SUPPORTED:1;
    UINT32                            PCI_PM_L1_1_SUPPORTED:1;
    UINT32                            ASPM_L1_2_SUPPORTED:1;
    UINT32                            ASPM_L1_1_SUPPORTED:1;
    UINT32                            L1_PM_SUB_SUPPORTED:1;
    UINT32                            LINK_ACTIVATION_SUPPORTED:1;
    UINT32                            Reserved_7_6:2;
    UINT32                            PORT_CM_RESTORE_TIME:8;
    UINT32                            PORT_T_POWER_ON_SCALE:2;
    UINT32                            Reserved_18_18:1;
    UINT32                            PORT_T_POWER_ON_VALUE:5;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} PCIE_L1_PM_SUB_CAP_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11100374UL

#define SMN_FUNC0_PCIE1_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11200374UL

#define SMN_FUNC0_PCIE2_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11300374UL

#define SMN_FUNC0_PCIE3_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11400374UL

#define SMN_FUNC1_PCIE0_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11101374UL

#define SMN_FUNC1_PCIE1_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11201374UL

#define SMN_FUNC2_PCIE0_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11102374UL

#define SMN_FUNC2_PCIE1_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11202374UL

#define SMN_FUNC3_PCIE0_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11103374UL

#define SMN_FUNC3_PCIE1_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11203374UL

#define SMN_FUNC4_PCIE0_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11104374UL

#define SMN_FUNC4_PCIE1_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11204374UL

#define SMN_FUNC5_PCIE1_PCIE_L1_PM_SUB_CAP_ADDRESS    0x11205374UL


/***********************************************************
* Register Name : PCIE_L1_PM_SUB_CAP_LIST
* Register Description :
* L1 PM Substates Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_L1_PM_SUB_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_L1_PM_SUB_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_L1_PM_SUB_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_L1_PM_SUB_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_L1_PM_SUB_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_L1_PM_SUB_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_L1_PM_SUB_CAP_LIST_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11100370UL

#define SMN_FUNC0_PCIE1_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11200370UL

#define SMN_FUNC0_PCIE2_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11300370UL

#define SMN_FUNC0_PCIE3_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11400370UL

#define SMN_FUNC1_PCIE0_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11101370UL

#define SMN_FUNC1_PCIE1_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11201370UL

#define SMN_FUNC2_PCIE0_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11102370UL

#define SMN_FUNC2_PCIE1_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11202370UL

#define SMN_FUNC3_PCIE0_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11103370UL

#define SMN_FUNC3_PCIE1_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11203370UL

#define SMN_FUNC4_PCIE0_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11104370UL

#define SMN_FUNC4_PCIE1_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11204370UL

#define SMN_FUNC5_PCIE1_PCIE_L1_PM_SUB_CAP_LIST_ADDRESS    0x11205370UL


/***********************************************************
* Register Name : PCIE_L1_PM_SUB_CNTL
* Register Description :
* L1 PM Substates Control 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if PCI-PM L1.2 is enabled.
#define PCIE_L1_PM_SUB_CNTL_PCI_PM_L1_2_EN_OFFSET      0
#define PCIE_L1_PM_SUB_CNTL_PCI_PM_L1_2_EN_MASK        0x1

// Bitfield Description : Controls if PCI-PM L1.1 is enabled.
#define PCIE_L1_PM_SUB_CNTL_PCI_PM_L1_1_EN_OFFSET      1
#define PCIE_L1_PM_SUB_CNTL_PCI_PM_L1_1_EN_MASK        0x2

// Bitfield Description : Controls if ASPM L1.2 is enabled.
#define PCIE_L1_PM_SUB_CNTL_ASPM_L1_2_EN_OFFSET      2
#define PCIE_L1_PM_SUB_CNTL_ASPM_L1_2_EN_MASK        0x4

// Bitfield Description : Controls if ASPM L1.1 is enabled.
#define PCIE_L1_PM_SUB_CNTL_ASPM_L1_1_EN_OFFSET      3
#define PCIE_L1_PM_SUB_CNTL_ASPM_L1_1_EN_MASK        0x8

// Bitfield Description : (Downstream Ports only) Controls the generation of an interrupt when the link activation process has been completed.
// For Upstream Ports this field is Reserved.
#define PCIE_L1_PM_SUB_CNTL_LINK_ACTIVATION_INTERRUPT_EN_OFFSET      4
#define PCIE_L1_PM_SUB_CNTL_LINK_ACTIVATION_INTERRUPT_EN_MASK        0x10

// Bitfield Description : (Downstream Ports only) Controls if the port must initiate the link activation process when in L1.
// For Upstream Ports this field is Reserved.
#define PCIE_L1_PM_SUB_CNTL_LINK_ACTIVATION_CNTL_OFFSET      5
#define PCIE_L1_PM_SUB_CNTL_LINK_ACTIVATION_CNTL_MASK        0x20

// Bitfield Description : 
#define PCIE_L1_PM_SUB_CNTL_Reserved_7_6_OFFSET      6
#define PCIE_L1_PM_SUB_CNTL_Reserved_7_6_MASK        0xc0

// Bitfield Description : (Downstream Ports only) When any of PCI-PM L1.2 Supported bit or ASPM L1.2 Supported bit is set, and if programmed when ASPM L1.2 Enable bit and PCI-PM L1.2 Enable bit are both clear, controls the value of Tcommonmode (in us) which must be used by the Downstream Port for timing the re-establishment of common mode.
// For Upstream Ports this field is Reserved.
#define PCIE_L1_PM_SUB_CNTL_COMMON_MODE_RESTORE_TIME_OFFSET      8
#define PCIE_L1_PM_SUB_CNTL_COMMON_MODE_RESTORE_TIME_MASK        0xff00

// Bitfield Description : When ASPM L1.2 Supported bit is set, and if programmed when ASPM L1.2 Enable bit is clear, in combination with the LTR L1.2 THRESHOLD Scale value, controls the LTR threshold used to determine if entry into L1 results in L1.1 (if enabled) or L1.2 (if enabled).
#define PCIE_L1_PM_SUB_CNTL_LTR_L1_2_THRESHOLD_VALUE_OFFSET      16
#define PCIE_L1_PM_SUB_CNTL_LTR_L1_2_THRESHOLD_VALUE_MASK        0x3ff0000

// Bitfield Description : 
#define PCIE_L1_PM_SUB_CNTL_Reserved_28_26_OFFSET      26
#define PCIE_L1_PM_SUB_CNTL_Reserved_28_26_MASK        0x1c000000

// Bitfield Description : When ASPM L1.2 Supported bit is set, and if programmed when ASPM L1.2 Enable bit is clear, controls the scale used for the LTR L1.2 THRESHOLD Value.
#define PCIE_L1_PM_SUB_CNTL_LTR_L1_2_THRESHOLD_SCALE_OFFSET      29
#define PCIE_L1_PM_SUB_CNTL_LTR_L1_2_THRESHOLD_SCALE_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            PCI_PM_L1_2_EN:1;
    UINT32                            PCI_PM_L1_1_EN:1;
    UINT32                            ASPM_L1_2_EN:1;
    UINT32                            ASPM_L1_1_EN:1;
    UINT32                            LINK_ACTIVATION_INTERRUPT_EN:1;
    UINT32                            LINK_ACTIVATION_CNTL:1;
    UINT32                            Reserved_7_6:2;
    UINT32                            COMMON_MODE_RESTORE_TIME:8;
    UINT32                            LTR_L1_2_THRESHOLD_VALUE:10;
    UINT32                            Reserved_28_26:3;
    UINT32                            LTR_L1_2_THRESHOLD_SCALE:3;
  } Field;
  UINT32 Value;
} PCIE_L1_PM_SUB_CNTL_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11100378UL

#define SMN_FUNC0_PCIE1_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11200378UL

#define SMN_FUNC0_PCIE2_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11300378UL

#define SMN_FUNC0_PCIE3_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11400378UL

#define SMN_FUNC1_PCIE0_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11101378UL

#define SMN_FUNC1_PCIE1_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11201378UL

#define SMN_FUNC2_PCIE0_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11102378UL

#define SMN_FUNC2_PCIE1_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11202378UL

#define SMN_FUNC3_PCIE0_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11103378UL

#define SMN_FUNC3_PCIE1_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11203378UL

#define SMN_FUNC4_PCIE0_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11104378UL

#define SMN_FUNC4_PCIE1_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11204378UL

#define SMN_FUNC5_PCIE1_PCIE_L1_PM_SUB_CNTL_ADDRESS    0x11205378UL


/***********************************************************
* Register Name : PCIE_L1_PM_SUB_CNTL2
* Register Description :
* L1 PM Substates Control 2 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When any of PCI-PM L1.2 Supported bit or ASPM L1.2 Supported bit is set, and if programmed when ASPM L1.2 Enable bit and PCI-PM L1.2 Enable bit are both clear, controls the scale used for T_POWER_ON Value.
#define PCIE_L1_PM_SUB_CNTL2_T_POWER_ON_SCALE_OFFSET      0
#define PCIE_L1_PM_SUB_CNTL2_T_POWER_ON_SCALE_MASK        0x3

// Bitfield Description : 
#define PCIE_L1_PM_SUB_CNTL2_Reserved_2_2_OFFSET      2
#define PCIE_L1_PM_SUB_CNTL2_Reserved_2_2_MASK        0x4

// Bitfield Description : When any of PCI-PM L1.2 Supported bit or ASPM L1.2 Supported bit is set, and if programmed when ASPM L1.2 Enable bit and PCI-PM L1.2 Enable bit are both clear, in combination with the T_POWER_ON Scale value, controls the minimum amount of time (in us) that the Port must wait in L1.2.Exit after sampling CLKREQ# asserted before actively driving the interface.
#define PCIE_L1_PM_SUB_CNTL2_T_POWER_ON_VALUE_OFFSET      3
#define PCIE_L1_PM_SUB_CNTL2_T_POWER_ON_VALUE_MASK        0xf8

// Bitfield Description : 
#define PCIE_L1_PM_SUB_CNTL2_Reserved_31_8_OFFSET      8
#define PCIE_L1_PM_SUB_CNTL2_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            T_POWER_ON_SCALE:2;
    UINT32                            Reserved_2_2:1;
    UINT32                            T_POWER_ON_VALUE:5;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} PCIE_L1_PM_SUB_CNTL2_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1110037cUL

#define SMN_FUNC0_PCIE1_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1120037cUL

#define SMN_FUNC0_PCIE2_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1130037cUL

#define SMN_FUNC0_PCIE3_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1140037cUL

#define SMN_FUNC1_PCIE0_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1110137cUL

#define SMN_FUNC1_PCIE1_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1120137cUL

#define SMN_FUNC2_PCIE0_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1110237cUL

#define SMN_FUNC2_PCIE1_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1120237cUL

#define SMN_FUNC3_PCIE0_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1110337cUL

#define SMN_FUNC3_PCIE1_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1120337cUL

#define SMN_FUNC4_PCIE0_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1110437cUL

#define SMN_FUNC4_PCIE1_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1120437cUL

#define SMN_FUNC5_PCIE1_PCIE_L1_PM_SUB_CNTL2_ADDRESS    0x1120537cUL


/***********************************************************
* Register Name : PCIE_LANE_EQUALIZATION_CNTL
* Register Description :
* Lane 0 Equalization Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Selects the Downstream Port's transmitter preset for initial operation at 8.0 GT/s. For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_8GT_TX_PRESET_OFFSET      0
#define PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_8GT_TX_PRESET_MASK        0xf

// Bitfield Description : Optionally selects the Downstream Port's receiver preset hint for initial operation at 8.0 GT/s.
// For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_8GT_RX_PRESET_HINT_OFFSET      4
#define PCIE_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_8GT_RX_PRESET_HINT_MASK        0x70

// Bitfield Description : 
#define PCIE_LANE_EQUALIZATION_CNTL_Reserved_7_7_OFFSET      7
#define PCIE_LANE_EQUALIZATION_CNTL_Reserved_7_7_MASK        0x80

// Bitfield Description : Selects the transmitter preset value that the Downstream Port requests the other side to use for initial operation at 8.0 GT/s.
// (Upstream Ports only) captures the transmitter preset value that the Upstream Port received from the other side for initial operation at 8.0 GT/s.
#define PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_8GT_TX_PRESET_OFFSET      8
#define PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_8GT_TX_PRESET_MASK        0xf00

// Bitfield Description : Selects the receiver preset hint value that the Downstream Port requests the other side to optionally use for initial operation at 8.0 GT/s.
// (Upstream Ports only) captures the receiver preset hint value that the Upstream Port received from the other side for initial operation at 8.0 GT/s.
#define PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_8GT_RX_PRESET_HINT_OFFSET      12
#define PCIE_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_8GT_RX_PRESET_HINT_MASK        0x7000

// Bitfield Description : 
#define PCIE_LANE_EQUALIZATION_CNTL_Reserved_15_15_OFFSET      15
#define PCIE_LANE_EQUALIZATION_CNTL_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            DOWNSTREAM_PORT_8GT_TX_PRESET:4;
    UINT16                            DOWNSTREAM_PORT_8GT_RX_PRESET_HINT:3;
    UINT16                            Reserved_7_7:1;
    UINT16                            UPSTREAM_PORT_8GT_TX_PRESET:4;
    UINT16                            UPSTREAM_PORT_8GT_RX_PRESET_HINT:3;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} PCIE_LANE_EQUALIZATION_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110027cUL

#define SMN_FUNC0_PCIE0_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11100290UL

#define SMN_FUNC0_PCIE0_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11100292UL

#define SMN_FUNC0_PCIE0_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11100294UL

#define SMN_FUNC0_PCIE0_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11100296UL

#define SMN_FUNC0_PCIE0_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11100298UL

#define SMN_FUNC0_PCIE0_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110029aUL

#define SMN_FUNC0_PCIE0_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110027eUL

#define SMN_FUNC0_PCIE0_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11100280UL

#define SMN_FUNC0_PCIE0_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11100282UL

#define SMN_FUNC0_PCIE0_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11100284UL

#define SMN_FUNC0_PCIE0_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11100286UL

#define SMN_FUNC0_PCIE0_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11100288UL

#define SMN_FUNC0_PCIE0_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110028aUL

#define SMN_FUNC0_PCIE0_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110028cUL

#define SMN_FUNC0_PCIE0_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110028eUL

#define SMN_FUNC0_PCIE1_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120027cUL

#define SMN_FUNC0_PCIE1_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11200290UL

#define SMN_FUNC0_PCIE1_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11200292UL

#define SMN_FUNC0_PCIE1_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11200294UL

#define SMN_FUNC0_PCIE1_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11200296UL

#define SMN_FUNC0_PCIE1_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11200298UL

#define SMN_FUNC0_PCIE1_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120029aUL

#define SMN_FUNC0_PCIE1_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120027eUL

#define SMN_FUNC0_PCIE1_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11200280UL

#define SMN_FUNC0_PCIE1_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11200282UL

#define SMN_FUNC0_PCIE1_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11200284UL

#define SMN_FUNC0_PCIE1_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11200286UL

#define SMN_FUNC0_PCIE1_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11200288UL

#define SMN_FUNC0_PCIE1_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120028aUL

#define SMN_FUNC0_PCIE1_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120028cUL

#define SMN_FUNC0_PCIE1_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120028eUL

#define SMN_FUNC0_PCIE2_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1130027cUL

#define SMN_FUNC0_PCIE2_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11300290UL

#define SMN_FUNC0_PCIE2_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11300292UL

#define SMN_FUNC0_PCIE2_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11300294UL

#define SMN_FUNC0_PCIE2_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11300296UL

#define SMN_FUNC0_PCIE2_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11300298UL

#define SMN_FUNC0_PCIE2_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1130029aUL

#define SMN_FUNC0_PCIE2_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1130027eUL

#define SMN_FUNC0_PCIE2_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11300280UL

#define SMN_FUNC0_PCIE2_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11300282UL

#define SMN_FUNC0_PCIE2_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11300284UL

#define SMN_FUNC0_PCIE2_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11300286UL

#define SMN_FUNC0_PCIE2_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11300288UL

#define SMN_FUNC0_PCIE2_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1130028aUL

#define SMN_FUNC0_PCIE2_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1130028cUL

#define SMN_FUNC0_PCIE2_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1130028eUL

#define SMN_FUNC0_PCIE3_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1140027cUL

#define SMN_FUNC0_PCIE3_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11400290UL

#define SMN_FUNC0_PCIE3_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11400292UL

#define SMN_FUNC0_PCIE3_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11400294UL

#define SMN_FUNC0_PCIE3_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11400296UL

#define SMN_FUNC0_PCIE3_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11400298UL

#define SMN_FUNC0_PCIE3_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1140029aUL

#define SMN_FUNC0_PCIE3_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1140027eUL

#define SMN_FUNC0_PCIE3_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11400280UL

#define SMN_FUNC0_PCIE3_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11400282UL

#define SMN_FUNC0_PCIE3_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11400284UL

#define SMN_FUNC0_PCIE3_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11400286UL

#define SMN_FUNC0_PCIE3_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11400288UL

#define SMN_FUNC0_PCIE3_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1140028aUL

#define SMN_FUNC0_PCIE3_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1140028cUL

#define SMN_FUNC0_PCIE3_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1140028eUL

#define SMN_FUNC1_PCIE0_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110127cUL

#define SMN_FUNC1_PCIE0_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11101290UL

#define SMN_FUNC1_PCIE0_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11101292UL

#define SMN_FUNC1_PCIE0_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11101294UL

#define SMN_FUNC1_PCIE0_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11101296UL

#define SMN_FUNC1_PCIE0_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11101298UL

#define SMN_FUNC1_PCIE0_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110129aUL

#define SMN_FUNC1_PCIE0_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110127eUL

#define SMN_FUNC1_PCIE0_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11101280UL

#define SMN_FUNC1_PCIE0_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11101282UL

#define SMN_FUNC1_PCIE0_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11101284UL

#define SMN_FUNC1_PCIE0_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11101286UL

#define SMN_FUNC1_PCIE0_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11101288UL

#define SMN_FUNC1_PCIE0_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110128aUL

#define SMN_FUNC1_PCIE0_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110128cUL

#define SMN_FUNC1_PCIE0_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110128eUL

#define SMN_FUNC1_PCIE1_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120127cUL

#define SMN_FUNC1_PCIE1_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11201290UL

#define SMN_FUNC1_PCIE1_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11201292UL

#define SMN_FUNC1_PCIE1_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11201294UL

#define SMN_FUNC1_PCIE1_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11201296UL

#define SMN_FUNC1_PCIE1_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11201298UL

#define SMN_FUNC1_PCIE1_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120129aUL

#define SMN_FUNC1_PCIE1_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120127eUL

#define SMN_FUNC1_PCIE1_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11201280UL

#define SMN_FUNC1_PCIE1_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11201282UL

#define SMN_FUNC1_PCIE1_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11201284UL

#define SMN_FUNC1_PCIE1_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11201286UL

#define SMN_FUNC1_PCIE1_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11201288UL

#define SMN_FUNC1_PCIE1_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120128aUL

#define SMN_FUNC1_PCIE1_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120128cUL

#define SMN_FUNC1_PCIE1_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120128eUL

#define SMN_FUNC2_PCIE0_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110227cUL

#define SMN_FUNC2_PCIE0_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11102290UL

#define SMN_FUNC2_PCIE0_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11102292UL

#define SMN_FUNC2_PCIE0_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11102294UL

#define SMN_FUNC2_PCIE0_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11102296UL

#define SMN_FUNC2_PCIE0_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11102298UL

#define SMN_FUNC2_PCIE0_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110229aUL

#define SMN_FUNC2_PCIE0_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110227eUL

#define SMN_FUNC2_PCIE0_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11102280UL

#define SMN_FUNC2_PCIE0_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11102282UL

#define SMN_FUNC2_PCIE0_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11102284UL

#define SMN_FUNC2_PCIE0_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11102286UL

#define SMN_FUNC2_PCIE0_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11102288UL

#define SMN_FUNC2_PCIE0_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110228aUL

#define SMN_FUNC2_PCIE0_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110228cUL

#define SMN_FUNC2_PCIE0_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110228eUL

#define SMN_FUNC2_PCIE1_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120227cUL

#define SMN_FUNC2_PCIE1_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11202290UL

#define SMN_FUNC2_PCIE1_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11202292UL

#define SMN_FUNC2_PCIE1_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11202294UL

#define SMN_FUNC2_PCIE1_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11202296UL

#define SMN_FUNC2_PCIE1_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11202298UL

#define SMN_FUNC2_PCIE1_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120229aUL

#define SMN_FUNC2_PCIE1_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120227eUL

#define SMN_FUNC2_PCIE1_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11202280UL

#define SMN_FUNC2_PCIE1_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11202282UL

#define SMN_FUNC2_PCIE1_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11202284UL

#define SMN_FUNC2_PCIE1_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11202286UL

#define SMN_FUNC2_PCIE1_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11202288UL

#define SMN_FUNC2_PCIE1_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120228aUL

#define SMN_FUNC2_PCIE1_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120228cUL

#define SMN_FUNC2_PCIE1_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120228eUL

#define SMN_FUNC3_PCIE0_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110327cUL

#define SMN_FUNC3_PCIE0_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11103290UL

#define SMN_FUNC3_PCIE0_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11103292UL

#define SMN_FUNC3_PCIE0_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11103294UL

#define SMN_FUNC3_PCIE0_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11103296UL

#define SMN_FUNC3_PCIE0_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11103298UL

#define SMN_FUNC3_PCIE0_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110329aUL

#define SMN_FUNC3_PCIE0_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110327eUL

#define SMN_FUNC3_PCIE0_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11103280UL

#define SMN_FUNC3_PCIE0_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11103282UL

#define SMN_FUNC3_PCIE0_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11103284UL

#define SMN_FUNC3_PCIE0_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11103286UL

#define SMN_FUNC3_PCIE0_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11103288UL

#define SMN_FUNC3_PCIE0_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110328aUL

#define SMN_FUNC3_PCIE0_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110328cUL

#define SMN_FUNC3_PCIE0_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110328eUL

#define SMN_FUNC3_PCIE1_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120327cUL

#define SMN_FUNC3_PCIE1_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11203290UL

#define SMN_FUNC3_PCIE1_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11203292UL

#define SMN_FUNC3_PCIE1_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11203294UL

#define SMN_FUNC3_PCIE1_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11203296UL

#define SMN_FUNC3_PCIE1_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11203298UL

#define SMN_FUNC3_PCIE1_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120329aUL

#define SMN_FUNC3_PCIE1_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120327eUL

#define SMN_FUNC3_PCIE1_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11203280UL

#define SMN_FUNC3_PCIE1_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11203282UL

#define SMN_FUNC3_PCIE1_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11203284UL

#define SMN_FUNC3_PCIE1_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11203286UL

#define SMN_FUNC3_PCIE1_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11203288UL

#define SMN_FUNC3_PCIE1_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120328aUL

#define SMN_FUNC3_PCIE1_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120328cUL

#define SMN_FUNC3_PCIE1_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120328eUL

#define SMN_FUNC4_PCIE0_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110427cUL

#define SMN_FUNC4_PCIE0_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11104290UL

#define SMN_FUNC4_PCIE0_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11104292UL

#define SMN_FUNC4_PCIE0_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11104294UL

#define SMN_FUNC4_PCIE0_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11104296UL

#define SMN_FUNC4_PCIE0_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11104298UL

#define SMN_FUNC4_PCIE0_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110429aUL

#define SMN_FUNC4_PCIE0_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110427eUL

#define SMN_FUNC4_PCIE0_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11104280UL

#define SMN_FUNC4_PCIE0_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11104282UL

#define SMN_FUNC4_PCIE0_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11104284UL

#define SMN_FUNC4_PCIE0_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11104286UL

#define SMN_FUNC4_PCIE0_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11104288UL

#define SMN_FUNC4_PCIE0_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110428aUL

#define SMN_FUNC4_PCIE0_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110428cUL

#define SMN_FUNC4_PCIE0_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1110428eUL

#define SMN_FUNC4_PCIE1_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120427cUL

#define SMN_FUNC4_PCIE1_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11204290UL

#define SMN_FUNC4_PCIE1_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11204292UL

#define SMN_FUNC4_PCIE1_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11204294UL

#define SMN_FUNC4_PCIE1_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11204296UL

#define SMN_FUNC4_PCIE1_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11204298UL

#define SMN_FUNC4_PCIE1_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120429aUL

#define SMN_FUNC4_PCIE1_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120427eUL

#define SMN_FUNC4_PCIE1_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11204280UL

#define SMN_FUNC4_PCIE1_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11204282UL

#define SMN_FUNC4_PCIE1_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11204284UL

#define SMN_FUNC4_PCIE1_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11204286UL

#define SMN_FUNC4_PCIE1_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11204288UL

#define SMN_FUNC4_PCIE1_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120428aUL

#define SMN_FUNC4_PCIE1_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120428cUL

#define SMN_FUNC4_PCIE1_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120428eUL

#define SMN_FUNC5_PCIE1_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120527cUL

#define SMN_FUNC5_PCIE1_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11205290UL

#define SMN_FUNC5_PCIE1_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11205292UL

#define SMN_FUNC5_PCIE1_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11205294UL

#define SMN_FUNC5_PCIE1_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11205296UL

#define SMN_FUNC5_PCIE1_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11205298UL

#define SMN_FUNC5_PCIE1_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120529aUL

#define SMN_FUNC5_PCIE1_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120527eUL

#define SMN_FUNC5_PCIE1_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11205280UL

#define SMN_FUNC5_PCIE1_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11205282UL

#define SMN_FUNC5_PCIE1_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11205284UL

#define SMN_FUNC5_PCIE1_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11205286UL

#define SMN_FUNC5_PCIE1_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x11205288UL

#define SMN_FUNC5_PCIE1_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120528aUL

#define SMN_FUNC5_PCIE1_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120528cUL

#define SMN_FUNC5_PCIE1_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1120528eUL


/***********************************************************
* Register Name : PCIE_LANE_ERROR_STATUS
* Register Description :
* Lane Error Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the corresponding lane detected a lane based error. This field is a bit vector that covers all supported lanes. Bit 0 corresponds to the default lane 0, and bit ordering is invariant to link width and lane reversal negotiation. This field is sticky.
#define PCIE_LANE_ERROR_STATUS_LANE_ERROR_STATUS_BITS_OFFSET      0
#define PCIE_LANE_ERROR_STATUS_LANE_ERROR_STATUS_BITS_MASK        0xffff

// Bitfield Description : 
#define PCIE_LANE_ERROR_STATUS_Reserved_31_16_OFFSET      16
#define PCIE_LANE_ERROR_STATUS_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            LANE_ERROR_STATUS_BITS:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} PCIE_LANE_ERROR_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11100278UL

#define SMN_FUNC0_PCIE1_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11200278UL

#define SMN_FUNC0_PCIE2_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11300278UL

#define SMN_FUNC0_PCIE3_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11400278UL

#define SMN_FUNC1_PCIE0_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11101278UL

#define SMN_FUNC1_PCIE1_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11201278UL

#define SMN_FUNC2_PCIE0_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11102278UL

#define SMN_FUNC2_PCIE1_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11202278UL

#define SMN_FUNC3_PCIE0_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11103278UL

#define SMN_FUNC3_PCIE1_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11203278UL

#define SMN_FUNC4_PCIE0_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11104278UL

#define SMN_FUNC4_PCIE1_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11204278UL

#define SMN_FUNC5_PCIE1_PCIE_LANE_ERROR_STATUS_ADDRESS    0x11205278UL


/***********************************************************
* Register Name : PCIE_LINK_CNTL3
* Register Description :
* Link Control 3 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : (Downstream Ports only) Forces the Downstream Port to perform link equalization, when a 1 is written to Retrain Link bit with Target Link Speed field set to 8.0 GT/s or higher.
// For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define PCIE_LINK_CNTL3_PERFORM_EQUALIZATION_OFFSET      0
#define PCIE_LINK_CNTL3_PERFORM_EQUALIZATION_MASK        0x1

// Bitfield Description : (Downstream Ports only) Controls if the Downstream Port generates an interrupt to indicate that the Link Equalization Request 8.0 GT/s, or the Link Equalization Request 16.0 GT/s, or the Link Equalization Request 32.0 GT/s bits have been set.
// For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define PCIE_LINK_CNTL3_LINK_EQUALIZATION_REQ_INT_EN_OFFSET      1
#define PCIE_LINK_CNTL3_LINK_EQUALIZATION_REQ_INT_EN_MASK        0x2

// Bitfield Description : 
#define PCIE_LINK_CNTL3_Reserved_8_2_OFFSET      2
#define PCIE_LINK_CNTL3_Reserved_8_2_MASK        0x1fc

// Bitfield Description : Controls if the Port overrides SRIS requirements for SKP ordered set transmission rates by transmitting SKP ordered sets at the SRNS scheduling rate when in L0, for the indicated speed(s).
#define PCIE_LINK_CNTL3_ENABLE_LOWER_SKP_OS_GEN_OFFSET      9
#define PCIE_LINK_CNTL3_ENABLE_LOWER_SKP_OS_GEN_MASK        0xfe00

// Bitfield Description : 
#define PCIE_LINK_CNTL3_Reserved_31_16_OFFSET      16
#define PCIE_LINK_CNTL3_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            PERFORM_EQUALIZATION:1;
    UINT32                            LINK_EQUALIZATION_REQ_INT_EN:1;
    UINT32                            Reserved_8_2:7;
    UINT32                            ENABLE_LOWER_SKP_OS_GEN:7;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} PCIE_LINK_CNTL3_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_LINK_CNTL3_ADDRESS    0x11100274UL

#define SMN_FUNC0_PCIE1_PCIE_LINK_CNTL3_ADDRESS    0x11200274UL

#define SMN_FUNC0_PCIE2_PCIE_LINK_CNTL3_ADDRESS    0x11300274UL

#define SMN_FUNC0_PCIE3_PCIE_LINK_CNTL3_ADDRESS    0x11400274UL

#define SMN_FUNC1_PCIE0_PCIE_LINK_CNTL3_ADDRESS    0x11101274UL

#define SMN_FUNC1_PCIE1_PCIE_LINK_CNTL3_ADDRESS    0x11201274UL

#define SMN_FUNC2_PCIE0_PCIE_LINK_CNTL3_ADDRESS    0x11102274UL

#define SMN_FUNC2_PCIE1_PCIE_LINK_CNTL3_ADDRESS    0x11202274UL

#define SMN_FUNC3_PCIE0_PCIE_LINK_CNTL3_ADDRESS    0x11103274UL

#define SMN_FUNC3_PCIE1_PCIE_LINK_CNTL3_ADDRESS    0x11203274UL

#define SMN_FUNC4_PCIE0_PCIE_LINK_CNTL3_ADDRESS    0x11104274UL

#define SMN_FUNC4_PCIE1_PCIE_LINK_CNTL3_ADDRESS    0x11204274UL

#define SMN_FUNC5_PCIE1_PCIE_LINK_CNTL3_ADDRESS    0x11205274UL


/***********************************************************
* Register Name : PCIE_LTR_CAP
* Register Description :
* Max Snoop Latency and Max No-Snoop Latency registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Max Snoop LatencyScale value, indicates the maximum snoop latency that a device is permitted to request.
#define PCIE_LTR_CAP_LTR_MAX_S_LATENCY_VALUE_OFFSET      0
#define PCIE_LTR_CAP_LTR_MAX_S_LATENCY_VALUE_MASK        0x3ff

// Bitfield Description : Indicates the scale used for the Maximum Snoop LatencyValue.
#define PCIE_LTR_CAP_LTR_MAX_S_LATENCY_SCALE_OFFSET      10
#define PCIE_LTR_CAP_LTR_MAX_S_LATENCY_SCALE_MASK        0x1c00

// Bitfield Description : 
#define PCIE_LTR_CAP_Reserved_15_13_OFFSET      13
#define PCIE_LTR_CAP_Reserved_15_13_MASK        0xe000

// Bitfield Description : In combination with the Max No-Snoop LatencyScale value, indicates the maximum no-snoop latency that a device is permitted to request.
#define PCIE_LTR_CAP_LTR_MAX_NS_LATENCY_VALUE_OFFSET      16
#define PCIE_LTR_CAP_LTR_MAX_NS_LATENCY_VALUE_MASK        0x3ff0000

// Bitfield Description : Indicates the scale used for the Maximum No-Snoop LatencyValue.
#define PCIE_LTR_CAP_LTR_MAX_NS_LATENCY_SCALE_OFFSET      26
#define PCIE_LTR_CAP_LTR_MAX_NS_LATENCY_SCALE_MASK        0x1c000000

// Bitfield Description : 
#define PCIE_LTR_CAP_Reserved_31_29_OFFSET      29
#define PCIE_LTR_CAP_Reserved_31_29_MASK        0xe0000000

typedef union {
  struct {
    UINT32                            LTR_MAX_S_LATENCY_VALUE:10;
    UINT32                            LTR_MAX_S_LATENCY_SCALE:3;
    UINT32                            Reserved_15_13:3;
    UINT32                            LTR_MAX_NS_LATENCY_VALUE:10;
    UINT32                            LTR_MAX_NS_LATENCY_SCALE:3;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} PCIE_LTR_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_LTR_CAP_ADDRESS    0x11100324UL

#define SMN_FUNC0_PCIE1_PCIE_LTR_CAP_ADDRESS    0x11200324UL

#define SMN_FUNC0_PCIE2_PCIE_LTR_CAP_ADDRESS    0x11300324UL

#define SMN_FUNC0_PCIE3_PCIE_LTR_CAP_ADDRESS    0x11400324UL

#define SMN_FUNC1_PCIE0_PCIE_LTR_CAP_ADDRESS    0x11101324UL

#define SMN_FUNC1_PCIE1_PCIE_LTR_CAP_ADDRESS    0x11201324UL

#define SMN_FUNC2_PCIE0_PCIE_LTR_CAP_ADDRESS    0x11102324UL

#define SMN_FUNC2_PCIE1_PCIE_LTR_CAP_ADDRESS    0x11202324UL

#define SMN_FUNC3_PCIE0_PCIE_LTR_CAP_ADDRESS    0x11103324UL

#define SMN_FUNC3_PCIE1_PCIE_LTR_CAP_ADDRESS    0x11203324UL

#define SMN_FUNC4_PCIE0_PCIE_LTR_CAP_ADDRESS    0x11104324UL

#define SMN_FUNC4_PCIE1_PCIE_LTR_CAP_ADDRESS    0x11204324UL

#define SMN_FUNC5_PCIE1_PCIE_LTR_CAP_ADDRESS    0x11205324UL


/***********************************************************
* Register Name : PCIE_LTR_ENH_CAP_LIST
* Register Description :
* LTR Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_LTR_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_LTR_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_LTR_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_LTR_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_LTR_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_LTR_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_LTR_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11100320UL

#define SMN_FUNC0_PCIE1_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11200320UL

#define SMN_FUNC0_PCIE2_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11300320UL

#define SMN_FUNC0_PCIE3_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11400320UL

#define SMN_FUNC1_PCIE0_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11101320UL

#define SMN_FUNC1_PCIE1_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11201320UL

#define SMN_FUNC2_PCIE0_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11102320UL

#define SMN_FUNC2_PCIE1_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11202320UL

#define SMN_FUNC3_PCIE0_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11103320UL

#define SMN_FUNC3_PCIE1_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11203320UL

#define SMN_FUNC4_PCIE0_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11104320UL

#define SMN_FUNC4_PCIE1_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11204320UL

#define SMN_FUNC5_PCIE1_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x11205320UL


/***********************************************************
* Register Name : PCIE_MARGINING_ENH_CAP_LIST
* Register Description :
* Margining Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_MARGINING_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_MARGINING_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_MARGINING_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_MARGINING_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_MARGINING_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_MARGINING_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_MARGINING_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11100440UL

#define SMN_FUNC0_PCIE1_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11200440UL

#define SMN_FUNC0_PCIE2_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11300440UL

#define SMN_FUNC0_PCIE3_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11400440UL

#define SMN_FUNC1_PCIE0_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11101440UL

#define SMN_FUNC1_PCIE1_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11201440UL

#define SMN_FUNC2_PCIE0_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11102440UL

#define SMN_FUNC2_PCIE1_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11202440UL

#define SMN_FUNC3_PCIE0_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11103440UL

#define SMN_FUNC3_PCIE1_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11203440UL

#define SMN_FUNC4_PCIE0_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11104440UL

#define SMN_FUNC4_PCIE1_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11204440UL

#define SMN_FUNC5_PCIE1_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x11205440UL


/***********************************************************
* Register Name : PCIE_MC_ADDR0
* Register Description :
* Multicast Base Address 0 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the location of the LSB of the Multicast Group number within the address. The minimum value is 12.
#define PCIE_MC_ADDR0_MC_INDEX_POS_OFFSET      0
#define PCIE_MC_ADDR0_MC_INDEX_POS_MASK        0x3f

// Bitfield Description : 
#define PCIE_MC_ADDR0_Reserved_11_6_OFFSET      6
#define PCIE_MC_ADDR0_Reserved_11_6_MASK        0xfc0

// Bitfield Description : Controls the lower base address bits 31-12 of the Multicast address range.
#define PCIE_MC_ADDR0_MC_BASE_ADDR_0_OFFSET      12
#define PCIE_MC_ADDR0_MC_BASE_ADDR_0_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            MC_INDEX_POS:6;
    UINT32                            Reserved_11_6:6;
    UINT32                            MC_BASE_ADDR_0:20;
  } Field;
  UINT32 Value;
} PCIE_MC_ADDR0_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_ADDR0_ADDRESS    0x111002f8UL

#define SMN_FUNC0_PCIE1_PCIE_MC_ADDR0_ADDRESS    0x112002f8UL

#define SMN_FUNC0_PCIE2_PCIE_MC_ADDR0_ADDRESS    0x113002f8UL

#define SMN_FUNC0_PCIE3_PCIE_MC_ADDR0_ADDRESS    0x114002f8UL

#define SMN_FUNC1_PCIE0_PCIE_MC_ADDR0_ADDRESS    0x111012f8UL

#define SMN_FUNC1_PCIE1_PCIE_MC_ADDR0_ADDRESS    0x112012f8UL

#define SMN_FUNC2_PCIE0_PCIE_MC_ADDR0_ADDRESS    0x111022f8UL

#define SMN_FUNC2_PCIE1_PCIE_MC_ADDR0_ADDRESS    0x112022f8UL

#define SMN_FUNC3_PCIE0_PCIE_MC_ADDR0_ADDRESS    0x111032f8UL

#define SMN_FUNC3_PCIE1_PCIE_MC_ADDR0_ADDRESS    0x112032f8UL

#define SMN_FUNC4_PCIE0_PCIE_MC_ADDR0_ADDRESS    0x111042f8UL

#define SMN_FUNC4_PCIE1_PCIE_MC_ADDR0_ADDRESS    0x112042f8UL

#define SMN_FUNC5_PCIE1_PCIE_MC_ADDR0_ADDRESS    0x112052f8UL


/***********************************************************
* Register Name : PCIE_MC_ADDR1
* Register Description :
* Multicast Base Address 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the upper base address [63:32] of the Multicast address range.
#define PCIE_MC_ADDR1_MC_BASE_ADDR_1_OFFSET      0
#define PCIE_MC_ADDR1_MC_BASE_ADDR_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MC_BASE_ADDR_1:32;
  } Field;
  UINT32 Value;
} PCIE_MC_ADDR1_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_ADDR1_ADDRESS    0x111002fcUL

#define SMN_FUNC0_PCIE1_PCIE_MC_ADDR1_ADDRESS    0x112002fcUL

#define SMN_FUNC0_PCIE2_PCIE_MC_ADDR1_ADDRESS    0x113002fcUL

#define SMN_FUNC0_PCIE3_PCIE_MC_ADDR1_ADDRESS    0x114002fcUL

#define SMN_FUNC1_PCIE0_PCIE_MC_ADDR1_ADDRESS    0x111012fcUL

#define SMN_FUNC1_PCIE1_PCIE_MC_ADDR1_ADDRESS    0x112012fcUL

#define SMN_FUNC2_PCIE0_PCIE_MC_ADDR1_ADDRESS    0x111022fcUL

#define SMN_FUNC2_PCIE1_PCIE_MC_ADDR1_ADDRESS    0x112022fcUL

#define SMN_FUNC3_PCIE0_PCIE_MC_ADDR1_ADDRESS    0x111032fcUL

#define SMN_FUNC3_PCIE1_PCIE_MC_ADDR1_ADDRESS    0x112032fcUL

#define SMN_FUNC4_PCIE0_PCIE_MC_ADDR1_ADDRESS    0x111042fcUL

#define SMN_FUNC4_PCIE1_PCIE_MC_ADDR1_ADDRESS    0x112042fcUL

#define SMN_FUNC5_PCIE1_PCIE_MC_ADDR1_ADDRESS    0x112052fcUL


/***********************************************************
* Register Name : PCIE_MC_BLOCK_ALL0
* Register Description :
* Multicast Block All 0 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : The lower 32 bits of the vector controlling which associated Multicast Groups the function is blocked from sending TLPs to. This field is half of a bit vector that covers the group range specified by MC_Num_Group.
#define PCIE_MC_BLOCK_ALL0_MC_BLOCK_ALL_0_OFFSET      0
#define PCIE_MC_BLOCK_ALL0_MC_BLOCK_ALL_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MC_BLOCK_ALL_0:32;
  } Field;
  UINT32 Value;
} PCIE_MC_BLOCK_ALL0_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11100308UL

#define SMN_FUNC0_PCIE1_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11200308UL

#define SMN_FUNC0_PCIE2_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11300308UL

#define SMN_FUNC0_PCIE3_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11400308UL

#define SMN_FUNC1_PCIE0_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11101308UL

#define SMN_FUNC1_PCIE1_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11201308UL

#define SMN_FUNC2_PCIE0_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11102308UL

#define SMN_FUNC2_PCIE1_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11202308UL

#define SMN_FUNC3_PCIE0_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11103308UL

#define SMN_FUNC3_PCIE1_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11203308UL

#define SMN_FUNC4_PCIE0_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11104308UL

#define SMN_FUNC4_PCIE1_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11204308UL

#define SMN_FUNC5_PCIE1_PCIE_MC_BLOCK_ALL0_ADDRESS    0x11205308UL


/***********************************************************
* Register Name : PCIE_MC_BLOCK_ALL1
* Register Description :
* Multicast Block All 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : The upper 32 bits of the vector controlling which associated Multicast Groups the function is blocked from sending TLPs to. This field is half of a bit vector that covers the group range specified by MC_Num_Group.
#define PCIE_MC_BLOCK_ALL1_MC_BLOCK_ALL_1_OFFSET      0
#define PCIE_MC_BLOCK_ALL1_MC_BLOCK_ALL_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MC_BLOCK_ALL_1:32;
  } Field;
  UINT32 Value;
} PCIE_MC_BLOCK_ALL1_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1110030cUL

#define SMN_FUNC0_PCIE1_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1120030cUL

#define SMN_FUNC0_PCIE2_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1130030cUL

#define SMN_FUNC0_PCIE3_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1140030cUL

#define SMN_FUNC1_PCIE0_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1110130cUL

#define SMN_FUNC1_PCIE1_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1120130cUL

#define SMN_FUNC2_PCIE0_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1110230cUL

#define SMN_FUNC2_PCIE1_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1120230cUL

#define SMN_FUNC3_PCIE0_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1110330cUL

#define SMN_FUNC3_PCIE1_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1120330cUL

#define SMN_FUNC4_PCIE0_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1110430cUL

#define SMN_FUNC4_PCIE1_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1120430cUL

#define SMN_FUNC5_PCIE1_PCIE_MC_BLOCK_ALL1_ADDRESS    0x1120530cUL


/***********************************************************
* Register Name : PCIE_MC_BLOCK_UNTRANSLATED_0
* Register Description :
* Multicast Block Untranslated 0 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : The lower 32 bits of the vector controlling which associated Multicast Groups the function is blocked from sending TLPs containing untranslated addresses to. This field is half of a bit vector that covers the group range specified by MC_Num_Group.
#define PCIE_MC_BLOCK_UNTRANSLATED_0_MC_BLOCK_UNTRANSLATED_0_OFFSET      0
#define PCIE_MC_BLOCK_UNTRANSLATED_0_MC_BLOCK_UNTRANSLATED_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MC_BLOCK_UNTRANSLATED_0:32;
  } Field;
  UINT32 Value;
} PCIE_MC_BLOCK_UNTRANSLATED_0_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11100310UL

#define SMN_FUNC0_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11200310UL

#define SMN_FUNC0_PCIE2_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11300310UL

#define SMN_FUNC0_PCIE3_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11400310UL

#define SMN_FUNC1_PCIE0_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11101310UL

#define SMN_FUNC1_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11201310UL

#define SMN_FUNC2_PCIE0_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11102310UL

#define SMN_FUNC2_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11202310UL

#define SMN_FUNC3_PCIE0_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11103310UL

#define SMN_FUNC3_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11203310UL

#define SMN_FUNC4_PCIE0_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11104310UL

#define SMN_FUNC4_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11204310UL

#define SMN_FUNC5_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_0_ADDRESS    0x11205310UL


/***********************************************************
* Register Name : PCIE_MC_BLOCK_UNTRANSLATED_1
* Register Description :
* Multicast Block Untranslated 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : The upper 32 bits of the vector controlling which associated Multicast Groups the function is blocked from sending TLPs containing untranslated addresses to. This field is half of a bit vector that covers the group range specified by MC_Num_Group.
#define PCIE_MC_BLOCK_UNTRANSLATED_1_MC_BLOCK_UNTRANSLATED_1_OFFSET      0
#define PCIE_MC_BLOCK_UNTRANSLATED_1_MC_BLOCK_UNTRANSLATED_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MC_BLOCK_UNTRANSLATED_1:32;
  } Field;
  UINT32 Value;
} PCIE_MC_BLOCK_UNTRANSLATED_1_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11100314UL

#define SMN_FUNC0_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11200314UL

#define SMN_FUNC0_PCIE2_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11300314UL

#define SMN_FUNC0_PCIE3_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11400314UL

#define SMN_FUNC1_PCIE0_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11101314UL

#define SMN_FUNC1_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11201314UL

#define SMN_FUNC2_PCIE0_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11102314UL

#define SMN_FUNC2_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11202314UL

#define SMN_FUNC3_PCIE0_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11103314UL

#define SMN_FUNC3_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11203314UL

#define SMN_FUNC4_PCIE0_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11104314UL

#define SMN_FUNC4_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11204314UL

#define SMN_FUNC5_PCIE1_PCIE_MC_BLOCK_UNTRANSLATED_1_ADDRESS    0x11205314UL


/***********************************************************
* Register Name : PCIE_MC_CAP
* Register Description :
* Multicast Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the maximum number (plus one) of Multicast Groups that the component supports.
#define PCIE_MC_CAP_MC_MAX_GROUP_OFFSET      0
#define PCIE_MC_CAP_MC_MAX_GROUP_MASK        0x3f

// Bitfield Description : 
#define PCIE_MC_CAP_Reserved_7_6_OFFSET      6
#define PCIE_MC_CAP_Reserved_7_6_MASK        0xc0

// Bitfield Description : (Endpoints only) Indicates as a log2 value the Multicast window size requested.
// For non-Endpoints this field is Reserved.
#define PCIE_MC_CAP_MC_WIN_SIZE_REQ_OFFSET      8
#define PCIE_MC_CAP_MC_WIN_SIZE_REQ_MASK        0x3f00

// Bitfield Description : 
#define PCIE_MC_CAP_Reserved_14_14_OFFSET      14
#define PCIE_MC_CAP_Reserved_14_14_MASK        0x4000

// Bitfield Description : Indicates if ECRC regeneration is supported by the component.
#define PCIE_MC_CAP_MC_ECRC_REGEN_SUPP_OFFSET      15
#define PCIE_MC_CAP_MC_ECRC_REGEN_SUPP_MASK        0x8000

typedef union {
  struct {
    UINT16                            MC_MAX_GROUP:6;
    UINT16                            Reserved_7_6:2;
    UINT16                            MC_WIN_SIZE_REQ:6;
    UINT16                            Reserved_14_14:1;
    UINT16                            MC_ECRC_REGEN_SUPP:1;
  } Field;
  UINT16 Value;
} PCIE_MC_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_CAP_ADDRESS    0x111002f4UL

#define SMN_FUNC0_PCIE1_PCIE_MC_CAP_ADDRESS    0x112002f4UL

#define SMN_FUNC0_PCIE2_PCIE_MC_CAP_ADDRESS    0x113002f4UL

#define SMN_FUNC0_PCIE3_PCIE_MC_CAP_ADDRESS    0x114002f4UL

#define SMN_FUNC1_PCIE0_PCIE_MC_CAP_ADDRESS    0x111012f4UL

#define SMN_FUNC1_PCIE1_PCIE_MC_CAP_ADDRESS    0x112012f4UL

#define SMN_FUNC2_PCIE0_PCIE_MC_CAP_ADDRESS    0x111022f4UL

#define SMN_FUNC2_PCIE1_PCIE_MC_CAP_ADDRESS    0x112022f4UL

#define SMN_FUNC3_PCIE0_PCIE_MC_CAP_ADDRESS    0x111032f4UL

#define SMN_FUNC3_PCIE1_PCIE_MC_CAP_ADDRESS    0x112032f4UL

#define SMN_FUNC4_PCIE0_PCIE_MC_CAP_ADDRESS    0x111042f4UL

#define SMN_FUNC4_PCIE1_PCIE_MC_CAP_ADDRESS    0x112042f4UL

#define SMN_FUNC5_PCIE1_PCIE_MC_CAP_ADDRESS    0x112052f4UL


/***********************************************************
* Register Name : PCIE_MC_CNTL
* Register Description :
* Multicast Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the number of Multicast Groups (plus one) used by the component.
#define PCIE_MC_CNTL_MC_NUM_GROUP_OFFSET      0
#define PCIE_MC_CNTL_MC_NUM_GROUP_MASK        0x3f

// Bitfield Description : 
#define PCIE_MC_CNTL_Reserved_14_6_OFFSET      6
#define PCIE_MC_CNTL_Reserved_14_6_MASK        0x7fc0

// Bitfield Description : Controls if the Multicast mechanism is enabled for the component.
#define PCIE_MC_CNTL_MC_ENABLE_OFFSET      15
#define PCIE_MC_CNTL_MC_ENABLE_MASK        0x8000

typedef union {
  struct {
    UINT16                            MC_NUM_GROUP:6;
    UINT16                            Reserved_14_6:9;
    UINT16                            MC_ENABLE:1;
  } Field;
  UINT16 Value;
} PCIE_MC_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_CNTL_ADDRESS    0x111002f6UL

#define SMN_FUNC0_PCIE1_PCIE_MC_CNTL_ADDRESS    0x112002f6UL

#define SMN_FUNC0_PCIE2_PCIE_MC_CNTL_ADDRESS    0x113002f6UL

#define SMN_FUNC0_PCIE3_PCIE_MC_CNTL_ADDRESS    0x114002f6UL

#define SMN_FUNC1_PCIE0_PCIE_MC_CNTL_ADDRESS    0x111012f6UL

#define SMN_FUNC1_PCIE1_PCIE_MC_CNTL_ADDRESS    0x112012f6UL

#define SMN_FUNC2_PCIE0_PCIE_MC_CNTL_ADDRESS    0x111022f6UL

#define SMN_FUNC2_PCIE1_PCIE_MC_CNTL_ADDRESS    0x112022f6UL

#define SMN_FUNC3_PCIE0_PCIE_MC_CNTL_ADDRESS    0x111032f6UL

#define SMN_FUNC3_PCIE1_PCIE_MC_CNTL_ADDRESS    0x112032f6UL

#define SMN_FUNC4_PCIE0_PCIE_MC_CNTL_ADDRESS    0x111042f6UL

#define SMN_FUNC4_PCIE1_PCIE_MC_CNTL_ADDRESS    0x112042f6UL

#define SMN_FUNC5_PCIE1_PCIE_MC_CNTL_ADDRESS    0x112052f6UL


/***********************************************************
* Register Name : PCIE_MC_ENH_CAP_LIST
* Register Description :
* Multicast Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_MC_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_MC_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_MC_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_MC_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_MC_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_MC_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_MC_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x111002f0UL

#define SMN_FUNC0_PCIE1_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x112002f0UL

#define SMN_FUNC0_PCIE2_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x113002f0UL

#define SMN_FUNC0_PCIE3_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x114002f0UL

#define SMN_FUNC1_PCIE0_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x111012f0UL

#define SMN_FUNC1_PCIE1_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x112012f0UL

#define SMN_FUNC2_PCIE0_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x111022f0UL

#define SMN_FUNC2_PCIE1_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x112022f0UL

#define SMN_FUNC3_PCIE0_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x111032f0UL

#define SMN_FUNC3_PCIE1_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x112032f0UL

#define SMN_FUNC4_PCIE0_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x111042f0UL

#define SMN_FUNC4_PCIE1_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x112042f0UL

#define SMN_FUNC5_PCIE1_PCIE_MC_ENH_CAP_LIST_ADDRESS    0x112052f0UL


/***********************************************************
* Register Name : PCIE_MC_OVERLAY_BAR0
* Register Description :
* Multicast Overlay BAR 0 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When greater than 5, controls the size in bytes of the overlay aperture as a power of 2. When less than 6, disables the overlay mechanism.
#define PCIE_MC_OVERLAY_BAR0_MC_OVERLAY_SIZE_OFFSET      0
#define PCIE_MC_OVERLAY_BAR0_MC_OVERLAY_SIZE_MASK        0x3f

// Bitfield Description : The lower half of the Multicast Overlay BAR bits 31-6 which controls the base address of the window on which Multicast TLPs passing through this function are overlaid.
#define PCIE_MC_OVERLAY_BAR0_MC_OVERLAY_BAR_0_OFFSET      6
#define PCIE_MC_OVERLAY_BAR0_MC_OVERLAY_BAR_0_MASK        0xffffffc0

typedef union {
  struct {
    UINT32                            MC_OVERLAY_SIZE:6;
    UINT32                            MC_OVERLAY_BAR_0:26;
  } Field;
  UINT32 Value;
} PCIE_MC_OVERLAY_BAR0_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11100318UL

#define SMN_FUNC0_PCIE1_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11200318UL

#define SMN_FUNC0_PCIE2_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11300318UL

#define SMN_FUNC0_PCIE3_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11400318UL

#define SMN_FUNC1_PCIE0_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11101318UL

#define SMN_FUNC1_PCIE1_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11201318UL

#define SMN_FUNC2_PCIE0_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11102318UL

#define SMN_FUNC2_PCIE1_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11202318UL

#define SMN_FUNC3_PCIE0_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11103318UL

#define SMN_FUNC3_PCIE1_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11203318UL

#define SMN_FUNC4_PCIE0_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11104318UL

#define SMN_FUNC4_PCIE1_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11204318UL

#define SMN_FUNC5_PCIE1_PCIE_MC_OVERLAY_BAR0_ADDRESS    0x11205318UL


/***********************************************************
* Register Name : PCIE_MC_OVERLAY_BAR1
* Register Description :
* Multicast Overlay BAR 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : The upper half of the Multicast Overlay BAR [63:32] which controls the base address of the window on which Multicast TLPs passing through this function are overlaid.
#define PCIE_MC_OVERLAY_BAR1_MC_OVERLAY_BAR_1_OFFSET      0
#define PCIE_MC_OVERLAY_BAR1_MC_OVERLAY_BAR_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MC_OVERLAY_BAR_1:32;
  } Field;
  UINT32 Value;
} PCIE_MC_OVERLAY_BAR1_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1110031cUL

#define SMN_FUNC0_PCIE1_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1120031cUL

#define SMN_FUNC0_PCIE2_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1130031cUL

#define SMN_FUNC0_PCIE3_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1140031cUL

#define SMN_FUNC1_PCIE0_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1110131cUL

#define SMN_FUNC1_PCIE1_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1120131cUL

#define SMN_FUNC2_PCIE0_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1110231cUL

#define SMN_FUNC2_PCIE1_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1120231cUL

#define SMN_FUNC3_PCIE0_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1110331cUL

#define SMN_FUNC3_PCIE1_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1120331cUL

#define SMN_FUNC4_PCIE0_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1110431cUL

#define SMN_FUNC4_PCIE1_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1120431cUL

#define SMN_FUNC5_PCIE1_PCIE_MC_OVERLAY_BAR1_ADDRESS    0x1120531cUL


/***********************************************************
* Register Name : PCIE_MC_RCV0
* Register Description :
* Multicast Receive 0 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : The lower 32 bits of the vector controlling which associated Multicast Groups the function (or Port) should accept (or forward) a copy of any Multicast TLP. This field is half of a bit vector that covers the group range specified by MC_Num_Group.
#define PCIE_MC_RCV0_MC_RECEIVE_0_OFFSET      0
#define PCIE_MC_RCV0_MC_RECEIVE_0_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MC_RECEIVE_0:32;
  } Field;
  UINT32 Value;
} PCIE_MC_RCV0_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_RCV0_ADDRESS    0x11100300UL

#define SMN_FUNC0_PCIE1_PCIE_MC_RCV0_ADDRESS    0x11200300UL

#define SMN_FUNC0_PCIE2_PCIE_MC_RCV0_ADDRESS    0x11300300UL

#define SMN_FUNC0_PCIE3_PCIE_MC_RCV0_ADDRESS    0x11400300UL

#define SMN_FUNC1_PCIE0_PCIE_MC_RCV0_ADDRESS    0x11101300UL

#define SMN_FUNC1_PCIE1_PCIE_MC_RCV0_ADDRESS    0x11201300UL

#define SMN_FUNC2_PCIE0_PCIE_MC_RCV0_ADDRESS    0x11102300UL

#define SMN_FUNC2_PCIE1_PCIE_MC_RCV0_ADDRESS    0x11202300UL

#define SMN_FUNC3_PCIE0_PCIE_MC_RCV0_ADDRESS    0x11103300UL

#define SMN_FUNC3_PCIE1_PCIE_MC_RCV0_ADDRESS    0x11203300UL

#define SMN_FUNC4_PCIE0_PCIE_MC_RCV0_ADDRESS    0x11104300UL

#define SMN_FUNC4_PCIE1_PCIE_MC_RCV0_ADDRESS    0x11204300UL

#define SMN_FUNC5_PCIE1_PCIE_MC_RCV0_ADDRESS    0x11205300UL


/***********************************************************
* Register Name : PCIE_MC_RCV1
* Register Description :
* Multicast Receive 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : The upper 32 bits of the vector controlling which associated Multicast Groups the function (or Port) should accept (or forward) a copy of any Multicast TLP. This field is half of a bit vector that covers the group range specified by MC_Num_Group.
#define PCIE_MC_RCV1_MC_RECEIVE_1_OFFSET      0
#define PCIE_MC_RCV1_MC_RECEIVE_1_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MC_RECEIVE_1:32;
  } Field;
  UINT32 Value;
} PCIE_MC_RCV1_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_MC_RCV1_ADDRESS    0x11100304UL

#define SMN_FUNC0_PCIE1_PCIE_MC_RCV1_ADDRESS    0x11200304UL

#define SMN_FUNC0_PCIE2_PCIE_MC_RCV1_ADDRESS    0x11300304UL

#define SMN_FUNC0_PCIE3_PCIE_MC_RCV1_ADDRESS    0x11400304UL

#define SMN_FUNC1_PCIE0_PCIE_MC_RCV1_ADDRESS    0x11101304UL

#define SMN_FUNC1_PCIE1_PCIE_MC_RCV1_ADDRESS    0x11201304UL

#define SMN_FUNC2_PCIE0_PCIE_MC_RCV1_ADDRESS    0x11102304UL

#define SMN_FUNC2_PCIE1_PCIE_MC_RCV1_ADDRESS    0x11202304UL

#define SMN_FUNC3_PCIE0_PCIE_MC_RCV1_ADDRESS    0x11103304UL

#define SMN_FUNC3_PCIE1_PCIE_MC_RCV1_ADDRESS    0x11203304UL

#define SMN_FUNC4_PCIE0_PCIE_MC_RCV1_ADDRESS    0x11104304UL

#define SMN_FUNC4_PCIE1_PCIE_MC_RCV1_ADDRESS    0x11204304UL

#define SMN_FUNC5_PCIE1_PCIE_MC_RCV1_ADDRESS    0x11205304UL


/***********************************************************
* Register Name : PCIE_NPEM_ENH_CAP_LIST
* Register Description :
* Native PCIe Enclosure Management Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_NPEM_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_NPEM_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_NPEM_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_NPEM_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_NPEM_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_NPEM_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_NPEM_ENH_CAP_LIST_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x111004d0UL

#define SMN_FUNC0_PCIE1_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x112004d0UL

#define SMN_FUNC0_PCIE2_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x113004d0UL

#define SMN_FUNC0_PCIE3_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x114004d0UL

#define SMN_FUNC1_PCIE0_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x111014d0UL

#define SMN_FUNC1_PCIE1_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x112014d0UL

#define SMN_FUNC2_PCIE0_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x111024d0UL

#define SMN_FUNC2_PCIE1_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x112024d0UL

#define SMN_FUNC3_PCIE0_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x111034d0UL

#define SMN_FUNC3_PCIE1_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x112034d0UL

#define SMN_FUNC4_PCIE0_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x111044d0UL

#define SMN_FUNC4_PCIE1_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x112044d0UL

#define SMN_FUNC5_PCIE1_PCIE_NPEM_ENH_CAP_LIST_ADDRESS    0x112054d0UL


/***********************************************************
* Register Name : PCIE_PHY_16GT_ENH_CAP_LIST
* Register Description :
* Physical Layer 16.0 GT/s Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_PHY_16GT_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_PHY_16GT_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_PHY_16GT_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_PHY_16GT_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_PHY_16GT_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_PHY_16GT_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_PHY_16GT_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11100410UL

#define SMN_FUNC0_PCIE1_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11200410UL

#define SMN_FUNC0_PCIE2_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11300410UL

#define SMN_FUNC0_PCIE3_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11400410UL

#define SMN_FUNC1_PCIE0_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11101410UL

#define SMN_FUNC1_PCIE1_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11201410UL

#define SMN_FUNC2_PCIE0_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11102410UL

#define SMN_FUNC2_PCIE1_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11202410UL

#define SMN_FUNC3_PCIE0_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11103410UL

#define SMN_FUNC3_PCIE1_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11203410UL

#define SMN_FUNC4_PCIE0_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11104410UL

#define SMN_FUNC4_PCIE1_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11204410UL

#define SMN_FUNC5_PCIE1_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x11205410UL


/***********************************************************
* Register Name : PCIE_PHY_32GT_ENH_CAP_LIST
* Register Description :
* Physical Layer 32.0 GT/s Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_PHY_32GT_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_PHY_32GT_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_PHY_32GT_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_PHY_32GT_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_PHY_32GT_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_PHY_32GT_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_PHY_32GT_ENH_CAP_LIST_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11100500UL

#define SMN_FUNC0_PCIE1_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11200500UL

#define SMN_FUNC0_PCIE2_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11300500UL

#define SMN_FUNC0_PCIE3_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11400500UL

#define SMN_FUNC1_PCIE0_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11101500UL

#define SMN_FUNC1_PCIE1_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11201500UL

#define SMN_FUNC2_PCIE0_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11102500UL

#define SMN_FUNC2_PCIE1_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11202500UL

#define SMN_FUNC3_PCIE0_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11103500UL

#define SMN_FUNC3_PCIE1_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11203500UL

#define SMN_FUNC4_PCIE0_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11104500UL

#define SMN_FUNC4_PCIE1_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11204500UL

#define SMN_FUNC5_PCIE1_PCIE_PHY_32GT_ENH_CAP_LIST_ADDRESS    0x11205500UL


/***********************************************************
* Register Name : PCIE_PORT_VC_CAP_REG1
* Register Description :
* Port VC Capability register 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the number of (extended) Virtual Channels in addition to the default VC supported by the device.
#define PCIE_PORT_VC_CAP_REG1_EXT_VC_COUNT_OFFSET      0
#define PCIE_PORT_VC_CAP_REG1_EXT_VC_COUNT_MASK        0x7

// Bitfield Description : 
#define PCIE_PORT_VC_CAP_REG1_Reserved_3_3_OFFSET      3
#define PCIE_PORT_VC_CAP_REG1_Reserved_3_3_MASK        0x8

// Bitfield Description : Indicates the number of (extended) Virtual Channels in addition to the default VC belonging to the Low Priority Virtual Channel group.
#define PCIE_PORT_VC_CAP_REG1_LOW_PRIORITY_EXT_VC_COUNT_OFFSET      4
#define PCIE_PORT_VC_CAP_REG1_LOW_PRIORITY_EXT_VC_COUNT_MASK        0x70

// Bitfield Description : 
#define PCIE_PORT_VC_CAP_REG1_Reserved_7_7_OFFSET      7
#define PCIE_PORT_VC_CAP_REG1_Reserved_7_7_MASK        0x80

// Bitfield Description : Indicates the reference clock for Virtual Channels that support time-based WRR Port Arbitration, otherwise it must be 0.
#define PCIE_PORT_VC_CAP_REG1_REF_CLK_OFFSET      8
#define PCIE_PORT_VC_CAP_REG1_REF_CLK_MASK        0x300

// Bitfield Description : Indicates the size (in bits) of Port Arbitration table entry in the device, or must be 0 if Port Arbitration table is not supported.
#define PCIE_PORT_VC_CAP_REG1_PORT_ARB_TABLE_ENTRY_SIZE_OFFSET      10
#define PCIE_PORT_VC_CAP_REG1_PORT_ARB_TABLE_ENTRY_SIZE_MASK        0xc00

// Bitfield Description : 
#define PCIE_PORT_VC_CAP_REG1_Reserved_31_12_OFFSET      12
#define PCIE_PORT_VC_CAP_REG1_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            EXT_VC_COUNT:3;
    UINT32                            Reserved_3_3:1;
    UINT32                            LOW_PRIORITY_EXT_VC_COUNT:3;
    UINT32                            Reserved_7_7:1;
    UINT32                            REF_CLK:2;
    UINT32                            PORT_ARB_TABLE_ENTRY_SIZE:2;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} PCIE_PORT_VC_CAP_REG1_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11100114UL

#define SMN_FUNC0_PCIE1_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11200114UL

#define SMN_FUNC0_PCIE2_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11300114UL

#define SMN_FUNC0_PCIE3_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11400114UL

#define SMN_FUNC1_PCIE0_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11101114UL

#define SMN_FUNC1_PCIE1_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11201114UL

#define SMN_FUNC2_PCIE0_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11102114UL

#define SMN_FUNC2_PCIE1_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11202114UL

#define SMN_FUNC3_PCIE0_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11103114UL

#define SMN_FUNC3_PCIE1_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11203114UL

#define SMN_FUNC4_PCIE0_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11104114UL

#define SMN_FUNC4_PCIE1_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11204114UL

#define SMN_FUNC5_PCIE1_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x11205114UL


/***********************************************************
* Register Name : PCIE_PORT_VC_CAP_REG2
* Register Description :
* Port VC Capability register 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the types of VC Arbitration supported by the device for the Low Priority Virtual Channel group, or must be 0 if the Low Priority Virtual Channel group contains only VC0. Bit 0 = Fixed; Bit 1 = WRR-32; Bit 2 = WRR-64; Bit 3 = WRR-128.
#define PCIE_PORT_VC_CAP_REG2_VC_ARB_CAP_OFFSET      0
#define PCIE_PORT_VC_CAP_REG2_VC_ARB_CAP_MASK        0xff

// Bitfield Description : 
#define PCIE_PORT_VC_CAP_REG2_Reserved_23_8_OFFSET      8
#define PCIE_PORT_VC_CAP_REG2_Reserved_23_8_MASK        0xffff00

// Bitfield Description : Indicates the location of the VC Arbitration Table, or must be 0 if the VC Arbitration Table is not present.
#define PCIE_PORT_VC_CAP_REG2_VC_ARB_TABLE_OFFSET_OFFSET      24
#define PCIE_PORT_VC_CAP_REG2_VC_ARB_TABLE_OFFSET_MASK        0xff000000

typedef union {
  struct {
    UINT32                            VC_ARB_CAP:8;
    UINT32                            Reserved_23_8:16;
    UINT32                            VC_ARB_TABLE_OFFSET:8;
  } Field;
  UINT32 Value;
} PCIE_PORT_VC_CAP_REG2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11100118UL

#define SMN_FUNC0_PCIE1_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11200118UL

#define SMN_FUNC0_PCIE2_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11300118UL

#define SMN_FUNC0_PCIE3_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11400118UL

#define SMN_FUNC1_PCIE0_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11101118UL

#define SMN_FUNC1_PCIE1_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11201118UL

#define SMN_FUNC2_PCIE0_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11102118UL

#define SMN_FUNC2_PCIE1_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11202118UL

#define SMN_FUNC3_PCIE0_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11103118UL

#define SMN_FUNC3_PCIE1_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11203118UL

#define SMN_FUNC4_PCIE0_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11104118UL

#define SMN_FUNC4_PCIE1_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11204118UL

#define SMN_FUNC5_PCIE1_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x11205118UL


/***********************************************************
* Register Name : PCIE_PORT_VC_CNTL
* Register Description :
* Port VC Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Updates the VC Arbitration logic from the VC Arbitration Table (if present), when written with 1. Reads always return 0.
#define PCIE_PORT_VC_CNTL_LOAD_VC_ARB_TABLE_OFFSET      0
#define PCIE_PORT_VC_CNTL_LOAD_VC_ARB_TABLE_MASK        0x1

// Bitfield Description : Configures the VC Arbitration by selecting one of the supported VC Arbitration schemes.
#define PCIE_PORT_VC_CNTL_VC_ARB_SELECT_OFFSET      1
#define PCIE_PORT_VC_CNTL_VC_ARB_SELECT_MASK        0xe

// Bitfield Description : 
#define PCIE_PORT_VC_CNTL_Reserved_15_4_OFFSET      4
#define PCIE_PORT_VC_CNTL_Reserved_15_4_MASK        0xfff0

typedef union {
  struct {
    UINT16                            LOAD_VC_ARB_TABLE:1;
    UINT16                            VC_ARB_SELECT:3;
    UINT16                            Reserved_15_4:12;
  } Field;
  UINT16 Value;
} PCIE_PORT_VC_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_PORT_VC_CNTL_ADDRESS    0x1110011cUL

#define SMN_FUNC0_PCIE1_PCIE_PORT_VC_CNTL_ADDRESS    0x1120011cUL

#define SMN_FUNC0_PCIE2_PCIE_PORT_VC_CNTL_ADDRESS    0x1130011cUL

#define SMN_FUNC0_PCIE3_PCIE_PORT_VC_CNTL_ADDRESS    0x1140011cUL

#define SMN_FUNC1_PCIE0_PCIE_PORT_VC_CNTL_ADDRESS    0x1110111cUL

#define SMN_FUNC1_PCIE1_PCIE_PORT_VC_CNTL_ADDRESS    0x1120111cUL

#define SMN_FUNC2_PCIE0_PCIE_PORT_VC_CNTL_ADDRESS    0x1110211cUL

#define SMN_FUNC2_PCIE1_PCIE_PORT_VC_CNTL_ADDRESS    0x1120211cUL

#define SMN_FUNC3_PCIE0_PCIE_PORT_VC_CNTL_ADDRESS    0x1110311cUL

#define SMN_FUNC3_PCIE1_PCIE_PORT_VC_CNTL_ADDRESS    0x1120311cUL

#define SMN_FUNC4_PCIE0_PCIE_PORT_VC_CNTL_ADDRESS    0x1110411cUL

#define SMN_FUNC4_PCIE1_PCIE_PORT_VC_CNTL_ADDRESS    0x1120411cUL

#define SMN_FUNC5_PCIE1_PCIE_PORT_VC_CNTL_ADDRESS    0x1120511cUL


/***********************************************************
* Register Name : PCIE_PORT_VC_STATUS
* Register Description :
* Port VC Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the contents of the VC Arbitration Table are coherent, or must be 0 if the VC Arbitration Table is not present.
#define PCIE_PORT_VC_STATUS_VC_ARB_TABLE_STATUS_OFFSET      0
#define PCIE_PORT_VC_STATUS_VC_ARB_TABLE_STATUS_MASK        0x1

// Bitfield Description : 
#define PCIE_PORT_VC_STATUS_Reserved_15_1_OFFSET      1
#define PCIE_PORT_VC_STATUS_Reserved_15_1_MASK        0xfffe

typedef union {
  struct {
    UINT16                            VC_ARB_TABLE_STATUS:1;
    UINT16                            Reserved_15_1:15;
  } Field;
  UINT16 Value;
} PCIE_PORT_VC_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_PORT_VC_STATUS_ADDRESS    0x1110011eUL

#define SMN_FUNC0_PCIE1_PCIE_PORT_VC_STATUS_ADDRESS    0x1120011eUL

#define SMN_FUNC0_PCIE2_PCIE_PORT_VC_STATUS_ADDRESS    0x1130011eUL

#define SMN_FUNC0_PCIE3_PCIE_PORT_VC_STATUS_ADDRESS    0x1140011eUL

#define SMN_FUNC1_PCIE0_PCIE_PORT_VC_STATUS_ADDRESS    0x1110111eUL

#define SMN_FUNC1_PCIE1_PCIE_PORT_VC_STATUS_ADDRESS    0x1120111eUL

#define SMN_FUNC2_PCIE0_PCIE_PORT_VC_STATUS_ADDRESS    0x1110211eUL

#define SMN_FUNC2_PCIE1_PCIE_PORT_VC_STATUS_ADDRESS    0x1120211eUL

#define SMN_FUNC3_PCIE0_PCIE_PORT_VC_STATUS_ADDRESS    0x1110311eUL

#define SMN_FUNC3_PCIE1_PCIE_PORT_VC_STATUS_ADDRESS    0x1120311eUL

#define SMN_FUNC4_PCIE0_PCIE_PORT_VC_STATUS_ADDRESS    0x1110411eUL

#define SMN_FUNC4_PCIE1_PCIE_PORT_VC_STATUS_ADDRESS    0x1120411eUL

#define SMN_FUNC5_PCIE1_PCIE_PORT_VC_STATUS_ADDRESS    0x1120511eUL


/***********************************************************
* Register Name : PCIE_ROOT_ERR_CMD
* Register Description :
* Root Error Command register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the generation of an interrupt when a correctable error is reported by any of the devices in the hierarchy associated with this Root Port.
#define PCIE_ROOT_ERR_CMD_CORR_ERR_REP_EN_OFFSET      0
#define PCIE_ROOT_ERR_CMD_CORR_ERR_REP_EN_MASK        0x1

// Bitfield Description : Controls the generation of an interrupt when a Non-fatal error is reported by any of the devices in the hierarchy associated with this Root Port.
#define PCIE_ROOT_ERR_CMD_NONFATAL_ERR_REP_EN_OFFSET      1
#define PCIE_ROOT_ERR_CMD_NONFATAL_ERR_REP_EN_MASK        0x2

// Bitfield Description : Controls the generation of an interrupt when a Fatal error is reported by any of the devices in the hierarchy associated with this Root Port.
#define PCIE_ROOT_ERR_CMD_FATAL_ERR_REP_EN_OFFSET      2
#define PCIE_ROOT_ERR_CMD_FATAL_ERR_REP_EN_MASK        0x4

// Bitfield Description : 
#define PCIE_ROOT_ERR_CMD_Reserved_31_3_OFFSET      3
#define PCIE_ROOT_ERR_CMD_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            CORR_ERR_REP_EN:1;
    UINT32                            NONFATAL_ERR_REP_EN:1;
    UINT32                            FATAL_ERR_REP_EN:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} PCIE_ROOT_ERR_CMD_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ROOT_ERR_CMD_ADDRESS    0x1110017cUL

#define SMN_FUNC0_PCIE1_PCIE_ROOT_ERR_CMD_ADDRESS    0x1120017cUL

#define SMN_FUNC0_PCIE2_PCIE_ROOT_ERR_CMD_ADDRESS    0x1130017cUL

#define SMN_FUNC0_PCIE3_PCIE_ROOT_ERR_CMD_ADDRESS    0x1140017cUL

#define SMN_FUNC1_PCIE0_PCIE_ROOT_ERR_CMD_ADDRESS    0x1110117cUL

#define SMN_FUNC1_PCIE1_PCIE_ROOT_ERR_CMD_ADDRESS    0x1120117cUL

#define SMN_FUNC2_PCIE0_PCIE_ROOT_ERR_CMD_ADDRESS    0x1110217cUL

#define SMN_FUNC2_PCIE1_PCIE_ROOT_ERR_CMD_ADDRESS    0x1120217cUL

#define SMN_FUNC3_PCIE0_PCIE_ROOT_ERR_CMD_ADDRESS    0x1110317cUL

#define SMN_FUNC3_PCIE1_PCIE_ROOT_ERR_CMD_ADDRESS    0x1120317cUL

#define SMN_FUNC4_PCIE0_PCIE_ROOT_ERR_CMD_ADDRESS    0x1110417cUL

#define SMN_FUNC4_PCIE1_PCIE_ROOT_ERR_CMD_ADDRESS    0x1120417cUL

#define SMN_FUNC5_PCIE1_PCIE_ROOT_ERR_CMD_ADDRESS    0x1120517cUL


/***********************************************************
* Register Name : PCIE_ROOT_ERR_STATUS
* Register Description :
* Root Error Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if a correctable error Message is received and this bit is not already set. This bit is sticky.
#define PCIE_ROOT_ERR_STATUS_ERR_CORR_RCVD_OFFSET      0
#define PCIE_ROOT_ERR_STATUS_ERR_CORR_RCVD_MASK        0x1

// Bitfield Description : Indicates if a correctable error Message is received and ERR_COR Received is already set. This bit is sticky.
#define PCIE_ROOT_ERR_STATUS_MULT_ERR_CORR_RCVD_OFFSET      1
#define PCIE_ROOT_ERR_STATUS_MULT_ERR_CORR_RCVD_MASK        0x2

// Bitfield Description : Indicates if either a Fatal or a Non-fatal error Message is received and this bit is not already set. This bit is sticky.
#define PCIE_ROOT_ERR_STATUS_ERR_FATAL_NONFATAL_RCVD_OFFSET      2
#define PCIE_ROOT_ERR_STATUS_ERR_FATAL_NONFATAL_RCVD_MASK        0x4

// Bitfield Description : Indicates if either a Fatal or a Non-fatal error is received and ERR_FATAL/NONFATAL Received is already set. This bit is sticky.
#define PCIE_ROOT_ERR_STATUS_MULT_ERR_FATAL_NONFATAL_RCVD_OFFSET      3
#define PCIE_ROOT_ERR_STATUS_MULT_ERR_FATAL_NONFATAL_RCVD_MASK        0x8

// Bitfield Description : Indicates if the first Uncorrectable error Message received is for a Fatal error. This bit is sticky.
#define PCIE_ROOT_ERR_STATUS_FIRST_UNCORRECTABLE_FATAL_OFFSET      4
#define PCIE_ROOT_ERR_STATUS_FIRST_UNCORRECTABLE_FATAL_MASK        0x10

// Bitfield Description : Indicates if one or more Non-Fatal Uncorrectable error Messages have been received. This bit is sticky.
#define PCIE_ROOT_ERR_STATUS_NONFATAL_ERROR_MSG_RCVD_OFFSET      5
#define PCIE_ROOT_ERR_STATUS_NONFATAL_ERROR_MSG_RCVD_MASK        0x20

// Bitfield Description : Indicates if one or more Fatal Uncorrectable error Messages have been received. This bit is sticky.
#define PCIE_ROOT_ERR_STATUS_FATAL_ERROR_MSG_RCVD_OFFSET      6
#define PCIE_ROOT_ERR_STATUS_FATAL_ERROR_MSG_RCVD_MASK        0x40

// Bitfield Description : When ERR_COR Subclass Capable bit is set, indicates the ERR_COR Subclass indicated in the received ERR_COR Message when the ERR_COR Received bit is not already set. This field is sticky.
#define PCIE_ROOT_ERR_STATUS_ERR_COR_SUBCLASS_OFFSET      7
#define PCIE_ROOT_ERR_STATUS_ERR_COR_SUBCLASS_MASK        0x180

// Bitfield Description : 
#define PCIE_ROOT_ERR_STATUS_Reserved_26_9_OFFSET      9
#define PCIE_ROOT_ERR_STATUS_Reserved_26_9_MASK        0x7fffe00

// Bitfield Description : Indicates the MSI/MSI-X vector used for interrupts generated in association with any of the status bits in the AER capability structure.
#define PCIE_ROOT_ERR_STATUS_ADV_ERR_INT_MSG_NUM_OFFSET      27
#define PCIE_ROOT_ERR_STATUS_ADV_ERR_INT_MSG_NUM_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            ERR_CORR_RCVD:1;
    UINT32                            MULT_ERR_CORR_RCVD:1;
    UINT32                            ERR_FATAL_NONFATAL_RCVD:1;
    UINT32                            MULT_ERR_FATAL_NONFATAL_RCVD:1;
    UINT32                            FIRST_UNCORRECTABLE_FATAL:1;
    UINT32                            NONFATAL_ERROR_MSG_RCVD:1;
    UINT32                            FATAL_ERROR_MSG_RCVD:1;
    UINT32                            ERR_COR_SUBCLASS:2;
    UINT32                            Reserved_26_9:18;
    UINT32                            ADV_ERR_INT_MSG_NUM:5;
  } Field;
  UINT32 Value;
} PCIE_ROOT_ERR_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11100180UL

#define SMN_FUNC0_PCIE1_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11200180UL

#define SMN_FUNC0_PCIE2_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11300180UL

#define SMN_FUNC0_PCIE3_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11400180UL

#define SMN_FUNC1_PCIE0_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11101180UL

#define SMN_FUNC1_PCIE1_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11201180UL

#define SMN_FUNC2_PCIE0_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11102180UL

#define SMN_FUNC2_PCIE1_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11202180UL

#define SMN_FUNC3_PCIE0_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11103180UL

#define SMN_FUNC3_PCIE1_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11203180UL

#define SMN_FUNC4_PCIE0_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11104180UL

#define SMN_FUNC4_PCIE1_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11204180UL

#define SMN_FUNC5_PCIE1_PCIE_ROOT_ERR_STATUS_ADDRESS    0x11205180UL


/***********************************************************
* Register Name : PCIE_RP_PIO_EXCEPTION
* Register Description :
* The RP PIO Exception register controls whether an individual RP PIO error is enabled to be reported by a Port as a synchronous processor exception.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the Port can report a synchronous processor exception for a Configuration Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_EXCEPTION_CFG_UR_CPL_OFFSET      0
#define PCIE_RP_PIO_EXCEPTION_CFG_UR_CPL_MASK        0x1

// Bitfield Description : Controls if the Port can report a synchronous processor exception for a Configuration Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_EXCEPTION_CFG_CA_CPL_OFFSET      1
#define PCIE_RP_PIO_EXCEPTION_CFG_CA_CPL_MASK        0x2

// Bitfield Description : Controls if the Port can report a synchronous processor exception for a Configuration Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_EXCEPTION_CFG_CTO_OFFSET      2
#define PCIE_RP_PIO_EXCEPTION_CFG_CTO_MASK        0x4

// Bitfield Description : 
#define PCIE_RP_PIO_EXCEPTION_Reserved_7_3_OFFSET      3
#define PCIE_RP_PIO_EXCEPTION_Reserved_7_3_MASK        0xf8

// Bitfield Description : Controls if the Port can report a synchronous processor exception for an I/O Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_EXCEPTION_IO_UR_CPL_OFFSET      8
#define PCIE_RP_PIO_EXCEPTION_IO_UR_CPL_MASK        0x100

// Bitfield Description : Controls if the Port can report a synchronous processor exception for an I/O Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_EXCEPTION_IO_CA_CPL_OFFSET      9
#define PCIE_RP_PIO_EXCEPTION_IO_CA_CPL_MASK        0x200

// Bitfield Description : Controls if the Port can report a synchronous processor exception for an I/O Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_EXCEPTION_IO_CTO_OFFSET      10
#define PCIE_RP_PIO_EXCEPTION_IO_CTO_MASK        0x400

// Bitfield Description : 
#define PCIE_RP_PIO_EXCEPTION_Reserved_15_11_OFFSET      11
#define PCIE_RP_PIO_EXCEPTION_Reserved_15_11_MASK        0xf800

// Bitfield Description : Controls if the Port can report a synchronous processor exception for a Memory Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_EXCEPTION_MEM_UR_CPL_OFFSET      16
#define PCIE_RP_PIO_EXCEPTION_MEM_UR_CPL_MASK        0x10000

// Bitfield Description : Controls if the Port can report a synchronous processor exception for a Memory Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_EXCEPTION_MEM_CA_CPL_OFFSET      17
#define PCIE_RP_PIO_EXCEPTION_MEM_CA_CPL_MASK        0x20000

// Bitfield Description : Controls if the Port can report a synchronous processor exception for a Memory Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_EXCEPTION_MEM_CTO_OFFSET      18
#define PCIE_RP_PIO_EXCEPTION_MEM_CTO_MASK        0x40000

// Bitfield Description : 
#define PCIE_RP_PIO_EXCEPTION_Reserved_31_19_OFFSET      19
#define PCIE_RP_PIO_EXCEPTION_Reserved_31_19_MASK        0xfff80000

typedef union {
  struct {
    UINT32                            CFG_UR_CPL:1;
    UINT32                            CFG_CA_CPL:1;
    UINT32                            CFG_CTO:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            IO_UR_CPL:1;
    UINT32                            IO_CA_CPL:1;
    UINT32                            IO_CTO:1;
    UINT32                            Reserved_15_11:5;
    UINT32                            MEM_UR_CPL:1;
    UINT32                            MEM_CA_CPL:1;
    UINT32                            MEM_CTO:1;
    UINT32                            Reserved_31_19:13;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_EXCEPTION_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1110039cUL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1120039cUL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1130039cUL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1140039cUL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1110139cUL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1120139cUL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1110239cUL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1120239cUL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1110339cUL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1120339cUL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1110439cUL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1120439cUL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_EXCEPTION_ADDRESS    0x1120539cUL


/***********************************************************
* Register Name : PCIE_RP_PIO_HDR_LOG0
* Register Description :
* RP PIO Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RP PIO Header 1st DW captures the first DW of the Header for the TLP corresponding to a detected RP PIO error. This field is sticky.
#define PCIE_RP_PIO_HDR_LOG0_TLP_HDR_OFFSET      0
#define PCIE_RP_PIO_HDR_LOG0_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_HDR_LOG0_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x111003a0UL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x112003a0UL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x113003a0UL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x114003a0UL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x111013a0UL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x112013a0UL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x111023a0UL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x112023a0UL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x111033a0UL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x112033a0UL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x111043a0UL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x112043a0UL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_HDR_LOG0_ADDRESS    0x112053a0UL


/***********************************************************
* Register Name : PCIE_RP_PIO_HDR_LOG1
* Register Description :
* RP PIO Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RP PIO Header 2nd DW captures the second DW of the Header for the TLP corresponding to a detected RP PIO error. This field is sticky.
#define PCIE_RP_PIO_HDR_LOG1_TLP_HDR_OFFSET      0
#define PCIE_RP_PIO_HDR_LOG1_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_HDR_LOG1_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x111003a4UL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x112003a4UL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x113003a4UL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x114003a4UL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x111013a4UL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x112013a4UL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x111023a4UL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x112023a4UL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x111033a4UL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x112033a4UL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x111043a4UL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x112043a4UL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_HDR_LOG1_ADDRESS    0x112053a4UL


/***********************************************************
* Register Name : PCIE_RP_PIO_HDR_LOG2
* Register Description :
* RP PIO Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RP PIO Header 3rd DW captures the third DW of the Header for the TLP corresponding to a detected RP PIO error. This field is sticky.
#define PCIE_RP_PIO_HDR_LOG2_TLP_HDR_OFFSET      0
#define PCIE_RP_PIO_HDR_LOG2_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_HDR_LOG2_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x111003a8UL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x112003a8UL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x113003a8UL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x114003a8UL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x111013a8UL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x112013a8UL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x111023a8UL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x112023a8UL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x111033a8UL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x112033a8UL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x111043a8UL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x112043a8UL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_HDR_LOG2_ADDRESS    0x112053a8UL


/***********************************************************
* Register Name : PCIE_RP_PIO_HDR_LOG3
* Register Description :
* RP PIO Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RP PIO Header 4th DW captures the fourth DW (if present) of the Header for the TLP corresponding to a detected RP PIO error. This field is sticky.
#define PCIE_RP_PIO_HDR_LOG3_TLP_HDR_OFFSET      0
#define PCIE_RP_PIO_HDR_LOG3_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_HDR_LOG3_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x111003acUL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x112003acUL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x113003acUL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x114003acUL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x111013acUL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x112013acUL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x111023acUL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x112023acUL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x111033acUL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x112033acUL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x111043acUL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x112043acUL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_HDR_LOG3_ADDRESS    0x112053acUL


/***********************************************************
* Register Name : PCIE_RP_PIO_IMPSPEC_LOG
* Register Description :
* RP PIO ImpSpec Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RP PIO ImpSpec log captures implementation-specific information (if any) associated with a detected RP PIO error.
#define PCIE_RP_PIO_IMPSPEC_LOG_TLP_HDR_OFFSET      0
#define PCIE_RP_PIO_IMPSPEC_LOG_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_IMPSPEC_LOG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x111003b0UL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x112003b0UL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x113003b0UL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x114003b0UL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x111013b0UL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x112013b0UL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x111023b0UL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x112023b0UL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x111033b0UL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x112033b0UL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x111043b0UL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x112043b0UL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_IMPSPEC_LOG_ADDRESS    0x112053b0UL


/***********************************************************
* Register Name : PCIE_RP_PIO_MASK
* Register Description :
* The RP PIO Mask register controls reporting of individual RP PIO errors by a Port.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the Port can report a Configuration Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_MASK_CFG_UR_CPL_OFFSET      0
#define PCIE_RP_PIO_MASK_CFG_UR_CPL_MASK        0x1

// Bitfield Description : Controls if the Port can report a Configuration Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_MASK_CFG_CA_CPL_OFFSET      1
#define PCIE_RP_PIO_MASK_CFG_CA_CPL_MASK        0x2

// Bitfield Description : Controls if the Port can report a Configuration Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_MASK_CFG_CTO_OFFSET      2
#define PCIE_RP_PIO_MASK_CFG_CTO_MASK        0x4

// Bitfield Description : 
#define PCIE_RP_PIO_MASK_Reserved_7_3_OFFSET      3
#define PCIE_RP_PIO_MASK_Reserved_7_3_MASK        0xf8

// Bitfield Description : Controls if the Port can report an I/O Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_MASK_IO_UR_CPL_OFFSET      8
#define PCIE_RP_PIO_MASK_IO_UR_CPL_MASK        0x100

// Bitfield Description : Controls if the Port can report an I/O Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_MASK_IO_CA_CPL_OFFSET      9
#define PCIE_RP_PIO_MASK_IO_CA_CPL_MASK        0x200

// Bitfield Description : Controls if the Port can report an I/O Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_MASK_IO_CTO_OFFSET      10
#define PCIE_RP_PIO_MASK_IO_CTO_MASK        0x400

// Bitfield Description : 
#define PCIE_RP_PIO_MASK_Reserved_15_11_OFFSET      11
#define PCIE_RP_PIO_MASK_Reserved_15_11_MASK        0xf800

// Bitfield Description : Controls if the Port can report a Memory Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_MASK_MEM_UR_CPL_OFFSET      16
#define PCIE_RP_PIO_MASK_MEM_UR_CPL_MASK        0x10000

// Bitfield Description : Controls if the Port can report a Memory Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_MASK_MEM_CA_CPL_OFFSET      17
#define PCIE_RP_PIO_MASK_MEM_CA_CPL_MASK        0x20000

// Bitfield Description : Controls if the Port can report a Memory Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_MASK_MEM_CTO_OFFSET      18
#define PCIE_RP_PIO_MASK_MEM_CTO_MASK        0x40000

// Bitfield Description : 
#define PCIE_RP_PIO_MASK_Reserved_31_19_OFFSET      19
#define PCIE_RP_PIO_MASK_Reserved_31_19_MASK        0xfff80000

typedef union {
  struct {
    UINT32                            CFG_UR_CPL:1;
    UINT32                            CFG_CA_CPL:1;
    UINT32                            CFG_CTO:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            IO_UR_CPL:1;
    UINT32                            IO_CA_CPL:1;
    UINT32                            IO_CTO:1;
    UINT32                            Reserved_15_11:5;
    UINT32                            MEM_UR_CPL:1;
    UINT32                            MEM_CA_CPL:1;
    UINT32                            MEM_CTO:1;
    UINT32                            Reserved_31_19:13;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_MASK_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_MASK_ADDRESS    0x11100390UL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_MASK_ADDRESS    0x11200390UL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_MASK_ADDRESS    0x11300390UL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_MASK_ADDRESS    0x11400390UL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_MASK_ADDRESS    0x11101390UL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_MASK_ADDRESS    0x11201390UL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_MASK_ADDRESS    0x11102390UL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_MASK_ADDRESS    0x11202390UL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_MASK_ADDRESS    0x11103390UL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_MASK_ADDRESS    0x11203390UL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_MASK_ADDRESS    0x11104390UL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_MASK_ADDRESS    0x11204390UL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_MASK_ADDRESS    0x11205390UL


/***********************************************************
* Register Name : PCIE_RP_PIO_PREFIX_LOG0
* Register Description :
* RP PIO TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : First RP PIO TLP Prefix Log (if supported) captures the first End-End TLP Prefix (if present) for the TLP corresponding to the detected RP PIO error. This field is sticky.
#define PCIE_RP_PIO_PREFIX_LOG0_TLP_PREFIX_OFFSET      0
#define PCIE_RP_PIO_PREFIX_LOG0_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_PREFIX_LOG0_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x111003b4UL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x112003b4UL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x113003b4UL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x114003b4UL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x111013b4UL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x112013b4UL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x111023b4UL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x112023b4UL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x111033b4UL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x112033b4UL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x111043b4UL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x112043b4UL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_PREFIX_LOG0_ADDRESS    0x112053b4UL


/***********************************************************
* Register Name : PCIE_RP_PIO_PREFIX_LOG1
* Register Description :
* RP PIO TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Second RP PIO TLP Prefix Log (if supported) captures the second End-End TLP Prefix (if present) for the TLP corresponding to the detected RP PIO error.
#define PCIE_RP_PIO_PREFIX_LOG1_TLP_PREFIX_OFFSET      0
#define PCIE_RP_PIO_PREFIX_LOG1_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_PREFIX_LOG1_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x111003b8UL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x112003b8UL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x113003b8UL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x114003b8UL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x111013b8UL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x112013b8UL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x111023b8UL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x112023b8UL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x111033b8UL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x112033b8UL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x111043b8UL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x112043b8UL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_PREFIX_LOG1_ADDRESS    0x112053b8UL


/***********************************************************
* Register Name : PCIE_RP_PIO_PREFIX_LOG2
* Register Description :
* RP PIO TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Third RP PIO TLP Prefix Log (if supported) captures the third End-End TLP Prefix (if present) for the TLP corresponding to the detected RP PIO error.
#define PCIE_RP_PIO_PREFIX_LOG2_TLP_PREFIX_OFFSET      0
#define PCIE_RP_PIO_PREFIX_LOG2_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_PREFIX_LOG2_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x111003bcUL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x112003bcUL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x113003bcUL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x114003bcUL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x111013bcUL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x112013bcUL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x111023bcUL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x112023bcUL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x111033bcUL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x112033bcUL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x111043bcUL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x112043bcUL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_PREFIX_LOG2_ADDRESS    0x112053bcUL


/***********************************************************
* Register Name : PCIE_RP_PIO_PREFIX_LOG3
* Register Description :
* RP PIO TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Fourth RP PIO TLP Prefix Log (if supported) captures the fourth End-End TLP Prefix (if present) for the TLP corresponding to the detected RP PIO error.
#define PCIE_RP_PIO_PREFIX_LOG3_TLP_PREFIX_OFFSET      0
#define PCIE_RP_PIO_PREFIX_LOG3_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_PREFIX_LOG3_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x111003c0UL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x112003c0UL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x113003c0UL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x114003c0UL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x111013c0UL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x112013c0UL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x111023c0UL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x112023c0UL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x111033c0UL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x112033c0UL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x111043c0UL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x112043c0UL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_PREFIX_LOG3_ADDRESS    0x112053c0UL


/***********************************************************
* Register Name : PCIE_RP_PIO_SEVERITY
* Register Description :
* The RP PIO Severity register controls whether an individual RP PIO error is reported by a Port as an Advisory Non-fatal or uncorrectable error.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls how the Port reports a Configuration Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_SEVERITY_CFG_UR_CPL_OFFSET      0
#define PCIE_RP_PIO_SEVERITY_CFG_UR_CPL_MASK        0x1

// Bitfield Description : Controls how the Port reports a Configuration Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_SEVERITY_CFG_CA_CPL_OFFSET      1
#define PCIE_RP_PIO_SEVERITY_CFG_CA_CPL_MASK        0x2

// Bitfield Description : Controls how the Port reports a Configuration Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_SEVERITY_CFG_CTO_OFFSET      2
#define PCIE_RP_PIO_SEVERITY_CFG_CTO_MASK        0x4

// Bitfield Description : 
#define PCIE_RP_PIO_SEVERITY_Reserved_7_3_OFFSET      3
#define PCIE_RP_PIO_SEVERITY_Reserved_7_3_MASK        0xf8

// Bitfield Description : Controls how the Port reports an I/O Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_SEVERITY_IO_UR_CPL_OFFSET      8
#define PCIE_RP_PIO_SEVERITY_IO_UR_CPL_MASK        0x100

// Bitfield Description : Controls how the Port reports an I/O Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_SEVERITY_IO_CA_CPL_OFFSET      9
#define PCIE_RP_PIO_SEVERITY_IO_CA_CPL_MASK        0x200

// Bitfield Description : Controls how the Port reports an I/O Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_SEVERITY_IO_CTO_OFFSET      10
#define PCIE_RP_PIO_SEVERITY_IO_CTO_MASK        0x400

// Bitfield Description : 
#define PCIE_RP_PIO_SEVERITY_Reserved_15_11_OFFSET      11
#define PCIE_RP_PIO_SEVERITY_Reserved_15_11_MASK        0xf800

// Bitfield Description : Controls how the Port reports a Memory Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_SEVERITY_MEM_UR_CPL_OFFSET      16
#define PCIE_RP_PIO_SEVERITY_MEM_UR_CPL_MASK        0x10000

// Bitfield Description : Controls how the Port reports a Memory Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_SEVERITY_MEM_CA_CPL_OFFSET      17
#define PCIE_RP_PIO_SEVERITY_MEM_CA_CPL_MASK        0x20000

// Bitfield Description : Controls how the Port reports a Memory Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_SEVERITY_MEM_CTO_OFFSET      18
#define PCIE_RP_PIO_SEVERITY_MEM_CTO_MASK        0x40000

// Bitfield Description : 
#define PCIE_RP_PIO_SEVERITY_Reserved_31_19_OFFSET      19
#define PCIE_RP_PIO_SEVERITY_Reserved_31_19_MASK        0xfff80000

typedef union {
  struct {
    UINT32                            CFG_UR_CPL:1;
    UINT32                            CFG_CA_CPL:1;
    UINT32                            CFG_CTO:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            IO_UR_CPL:1;
    UINT32                            IO_CA_CPL:1;
    UINT32                            IO_CTO:1;
    UINT32                            Reserved_15_11:5;
    UINT32                            MEM_UR_CPL:1;
    UINT32                            MEM_CA_CPL:1;
    UINT32                            MEM_CTO:1;
    UINT32                            Reserved_31_19:13;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_SEVERITY_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11100394UL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11200394UL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11300394UL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11400394UL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11101394UL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11201394UL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11102394UL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11202394UL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11103394UL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11203394UL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11104394UL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11204394UL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_SEVERITY_ADDRESS    0x11205394UL


/***********************************************************
* Register Name : PCIE_RP_PIO_STATUS
* Register Description :
* The RP PIO Status register reports error status of individual RP PIO errors on a Port.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Port detects a Configuration Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_STATUS_CFG_UR_CPL_OFFSET      0
#define PCIE_RP_PIO_STATUS_CFG_UR_CPL_MASK        0x1

// Bitfield Description : Indicates if the Port detects a Configuration Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_STATUS_CFG_CA_CPL_OFFSET      1
#define PCIE_RP_PIO_STATUS_CFG_CA_CPL_MASK        0x2

// Bitfield Description : Indicates if the Port detects a Configuration Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_STATUS_CFG_CTO_OFFSET      2
#define PCIE_RP_PIO_STATUS_CFG_CTO_MASK        0x4

// Bitfield Description : 
#define PCIE_RP_PIO_STATUS_Reserved_7_3_OFFSET      3
#define PCIE_RP_PIO_STATUS_Reserved_7_3_MASK        0xf8

// Bitfield Description : Indicates if the Port detects an I/O Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_STATUS_IO_UR_CPL_OFFSET      8
#define PCIE_RP_PIO_STATUS_IO_UR_CPL_MASK        0x100

// Bitfield Description : Indicates if the Port detects an I/O Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_STATUS_IO_CA_CPL_OFFSET      9
#define PCIE_RP_PIO_STATUS_IO_CA_CPL_MASK        0x200

// Bitfield Description : Indicates if the Port detects an I/O Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_STATUS_IO_CTO_OFFSET      10
#define PCIE_RP_PIO_STATUS_IO_CTO_MASK        0x400

// Bitfield Description : 
#define PCIE_RP_PIO_STATUS_Reserved_15_11_OFFSET      11
#define PCIE_RP_PIO_STATUS_Reserved_15_11_MASK        0xf800

// Bitfield Description : Indicates if the Port detects a Memory Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_STATUS_MEM_UR_CPL_OFFSET      16
#define PCIE_RP_PIO_STATUS_MEM_UR_CPL_MASK        0x10000

// Bitfield Description : Indicates if the Port detects a Memory Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_STATUS_MEM_CA_CPL_OFFSET      17
#define PCIE_RP_PIO_STATUS_MEM_CA_CPL_MASK        0x20000

// Bitfield Description : Indicates if the Port detects a Memory Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_STATUS_MEM_CTO_OFFSET      18
#define PCIE_RP_PIO_STATUS_MEM_CTO_MASK        0x40000

// Bitfield Description : 
#define PCIE_RP_PIO_STATUS_Reserved_31_19_OFFSET      19
#define PCIE_RP_PIO_STATUS_Reserved_31_19_MASK        0xfff80000

typedef union {
  struct {
    UINT32                            CFG_UR_CPL:1;
    UINT32                            CFG_CA_CPL:1;
    UINT32                            CFG_CTO:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            IO_UR_CPL:1;
    UINT32                            IO_CA_CPL:1;
    UINT32                            IO_CTO:1;
    UINT32                            Reserved_15_11:5;
    UINT32                            MEM_UR_CPL:1;
    UINT32                            MEM_CA_CPL:1;
    UINT32                            MEM_CTO:1;
    UINT32                            Reserved_31_19:13;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_STATUS_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_STATUS_ADDRESS    0x1110038cUL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_STATUS_ADDRESS    0x1120038cUL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_STATUS_ADDRESS    0x1130038cUL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_STATUS_ADDRESS    0x1140038cUL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_STATUS_ADDRESS    0x1110138cUL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_STATUS_ADDRESS    0x1120138cUL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_STATUS_ADDRESS    0x1110238cUL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_STATUS_ADDRESS    0x1120238cUL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_STATUS_ADDRESS    0x1110338cUL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_STATUS_ADDRESS    0x1120338cUL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_STATUS_ADDRESS    0x1110438cUL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_STATUS_ADDRESS    0x1120438cUL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_STATUS_ADDRESS    0x1120538cUL


/***********************************************************
* Register Name : PCIE_RP_PIO_SYSERROR
* Register Description :
* The RP PIO SysError register controls whether an individual unmasked RP PIO error is enabled to be reported by a Port as a system error.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the Port can report a system error for a Configuration Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_SYSERROR_CFG_UR_CPL_OFFSET      0
#define PCIE_RP_PIO_SYSERROR_CFG_UR_CPL_MASK        0x1

// Bitfield Description : Controls if the Port can report a system error for a Configuration Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_SYSERROR_CFG_CA_CPL_OFFSET      1
#define PCIE_RP_PIO_SYSERROR_CFG_CA_CPL_MASK        0x2

// Bitfield Description : Controls if the Port can report a system error for a Configuration Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_SYSERROR_CFG_CTO_OFFSET      2
#define PCIE_RP_PIO_SYSERROR_CFG_CTO_MASK        0x4

// Bitfield Description : 
#define PCIE_RP_PIO_SYSERROR_Reserved_7_3_OFFSET      3
#define PCIE_RP_PIO_SYSERROR_Reserved_7_3_MASK        0xf8

// Bitfield Description : Controls if the Port can report a system error for an I/O Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_SYSERROR_IO_UR_CPL_OFFSET      8
#define PCIE_RP_PIO_SYSERROR_IO_UR_CPL_MASK        0x100

// Bitfield Description : Controls if the Port can report a system error for an I/O Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_SYSERROR_IO_CA_CPL_OFFSET      9
#define PCIE_RP_PIO_SYSERROR_IO_CA_CPL_MASK        0x200

// Bitfield Description : Controls if the Port can report a system error for an I/O Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_SYSERROR_IO_CTO_OFFSET      10
#define PCIE_RP_PIO_SYSERROR_IO_CTO_MASK        0x400

// Bitfield Description : 
#define PCIE_RP_PIO_SYSERROR_Reserved_15_11_OFFSET      11
#define PCIE_RP_PIO_SYSERROR_Reserved_15_11_MASK        0xf800

// Bitfield Description : Controls if the Port can report a system error for a Memory Request received UR Completion. This bit is sticky.
#define PCIE_RP_PIO_SYSERROR_MEM_UR_CPL_OFFSET      16
#define PCIE_RP_PIO_SYSERROR_MEM_UR_CPL_MASK        0x10000

// Bitfield Description : Controls if the Port can report a system error for a Memory Request received CA Completion. This bit is sticky.
#define PCIE_RP_PIO_SYSERROR_MEM_CA_CPL_OFFSET      17
#define PCIE_RP_PIO_SYSERROR_MEM_CA_CPL_MASK        0x20000

// Bitfield Description : Controls if the Port can report a system error for a Memory Request Completion Timeout. This bit is sticky.
#define PCIE_RP_PIO_SYSERROR_MEM_CTO_OFFSET      18
#define PCIE_RP_PIO_SYSERROR_MEM_CTO_MASK        0x40000

// Bitfield Description : 
#define PCIE_RP_PIO_SYSERROR_Reserved_31_19_OFFSET      19
#define PCIE_RP_PIO_SYSERROR_Reserved_31_19_MASK        0xfff80000

typedef union {
  struct {
    UINT32                            CFG_UR_CPL:1;
    UINT32                            CFG_CA_CPL:1;
    UINT32                            CFG_CTO:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            IO_UR_CPL:1;
    UINT32                            IO_CA_CPL:1;
    UINT32                            IO_CTO:1;
    UINT32                            Reserved_15_11:5;
    UINT32                            MEM_UR_CPL:1;
    UINT32                            MEM_CA_CPL:1;
    UINT32                            MEM_CTO:1;
    UINT32                            Reserved_31_19:13;
  } Field;
  UINT32 Value;
} PCIE_RP_PIO_SYSERROR_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11100398UL

#define SMN_FUNC0_PCIE1_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11200398UL

#define SMN_FUNC0_PCIE2_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11300398UL

#define SMN_FUNC0_PCIE3_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11400398UL

#define SMN_FUNC1_PCIE0_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11101398UL

#define SMN_FUNC1_PCIE1_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11201398UL

#define SMN_FUNC2_PCIE0_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11102398UL

#define SMN_FUNC2_PCIE1_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11202398UL

#define SMN_FUNC3_PCIE0_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11103398UL

#define SMN_FUNC3_PCIE1_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11203398UL

#define SMN_FUNC4_PCIE0_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11104398UL

#define SMN_FUNC4_PCIE1_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11204398UL

#define SMN_FUNC5_PCIE1_PCIE_RP_PIO_SYSERROR_ADDRESS    0x11205398UL


/***********************************************************
* Register Name : PCIE_SECONDARY_ENH_CAP_LIST
* Register Description :
* Secondary PCI Express Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_SECONDARY_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_SECONDARY_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_SECONDARY_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_SECONDARY_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_SECONDARY_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_SECONDARY_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_SECONDARY_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11100270UL

#define SMN_FUNC0_PCIE1_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11200270UL

#define SMN_FUNC0_PCIE2_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11300270UL

#define SMN_FUNC0_PCIE3_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11400270UL

#define SMN_FUNC1_PCIE0_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11101270UL

#define SMN_FUNC1_PCIE1_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11201270UL

#define SMN_FUNC2_PCIE0_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11102270UL

#define SMN_FUNC2_PCIE1_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11202270UL

#define SMN_FUNC3_PCIE0_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11103270UL

#define SMN_FUNC3_PCIE1_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11203270UL

#define SMN_FUNC4_PCIE0_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11104270UL

#define SMN_FUNC4_PCIE1_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11204270UL

#define SMN_FUNC5_PCIE1_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x11205270UL


/***********************************************************
* Register Name : PCIE_TLP_PREFIX_LOG0
* Register Description :
* TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : First TLP Prefix Log (if supported) captures the first End-End TLP Prefix (if present) for the TLP corresponding to the detected error. This field is sticky.
#define PCIE_TLP_PREFIX_LOG0_TLP_PREFIX_OFFSET      0
#define PCIE_TLP_PREFIX_LOG0_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_TLP_PREFIX_LOG0_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11100188UL

#define SMN_FUNC0_PCIE1_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11200188UL

#define SMN_FUNC0_PCIE2_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11300188UL

#define SMN_FUNC0_PCIE3_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11400188UL

#define SMN_FUNC1_PCIE0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11101188UL

#define SMN_FUNC1_PCIE1_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11201188UL

#define SMN_FUNC2_PCIE0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11102188UL

#define SMN_FUNC2_PCIE1_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11202188UL

#define SMN_FUNC3_PCIE0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11103188UL

#define SMN_FUNC3_PCIE1_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11203188UL

#define SMN_FUNC4_PCIE0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11104188UL

#define SMN_FUNC4_PCIE1_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11204188UL

#define SMN_FUNC5_PCIE1_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x11205188UL


/***********************************************************
* Register Name : PCIE_TLP_PREFIX_LOG1
* Register Description :
* TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Second TLP Prefix Log (if supported) captures the second End-End TLP Prefix (if present) for the TLP corresponding to the detected error. This field is sticky.
#define PCIE_TLP_PREFIX_LOG1_TLP_PREFIX_OFFSET      0
#define PCIE_TLP_PREFIX_LOG1_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_TLP_PREFIX_LOG1_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1110018cUL

#define SMN_FUNC0_PCIE1_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1120018cUL

#define SMN_FUNC0_PCIE2_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1130018cUL

#define SMN_FUNC0_PCIE3_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1140018cUL

#define SMN_FUNC1_PCIE0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1110118cUL

#define SMN_FUNC1_PCIE1_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1120118cUL

#define SMN_FUNC2_PCIE0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1110218cUL

#define SMN_FUNC2_PCIE1_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1120218cUL

#define SMN_FUNC3_PCIE0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1110318cUL

#define SMN_FUNC3_PCIE1_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1120318cUL

#define SMN_FUNC4_PCIE0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1110418cUL

#define SMN_FUNC4_PCIE1_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1120418cUL

#define SMN_FUNC5_PCIE1_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1120518cUL


/***********************************************************
* Register Name : PCIE_TLP_PREFIX_LOG2
* Register Description :
* TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Third TLP Prefix Log (if supported) captures the third End-End TLP Prefix (if present) for the TLP corresponding to the detected error. This field is sticky.
#define PCIE_TLP_PREFIX_LOG2_TLP_PREFIX_OFFSET      0
#define PCIE_TLP_PREFIX_LOG2_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_TLP_PREFIX_LOG2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11100190UL

#define SMN_FUNC0_PCIE1_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11200190UL

#define SMN_FUNC0_PCIE2_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11300190UL

#define SMN_FUNC0_PCIE3_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11400190UL

#define SMN_FUNC1_PCIE0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11101190UL

#define SMN_FUNC1_PCIE1_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11201190UL

#define SMN_FUNC2_PCIE0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11102190UL

#define SMN_FUNC2_PCIE1_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11202190UL

#define SMN_FUNC3_PCIE0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11103190UL

#define SMN_FUNC3_PCIE1_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11203190UL

#define SMN_FUNC4_PCIE0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11104190UL

#define SMN_FUNC4_PCIE1_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11204190UL

#define SMN_FUNC5_PCIE1_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x11205190UL


/***********************************************************
* Register Name : PCIE_TLP_PREFIX_LOG3
* Register Description :
* TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Fourth TLP Prefix Log (if supported) captures the fourth End-End TLP Prefix (if present) for the TLP corresponding to the detected error. This field is sticky.
#define PCIE_TLP_PREFIX_LOG3_TLP_PREFIX_OFFSET      0
#define PCIE_TLP_PREFIX_LOG3_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_TLP_PREFIX_LOG3_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11100194UL

#define SMN_FUNC0_PCIE1_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11200194UL

#define SMN_FUNC0_PCIE2_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11300194UL

#define SMN_FUNC0_PCIE3_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11400194UL

#define SMN_FUNC1_PCIE0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11101194UL

#define SMN_FUNC1_PCIE1_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11201194UL

#define SMN_FUNC2_PCIE0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11102194UL

#define SMN_FUNC2_PCIE1_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11202194UL

#define SMN_FUNC3_PCIE0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11103194UL

#define SMN_FUNC3_PCIE1_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11203194UL

#define SMN_FUNC4_PCIE0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11104194UL

#define SMN_FUNC4_PCIE1_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11204194UL

#define SMN_FUNC5_PCIE1_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x11205194UL


/***********************************************************
* Register Name : PCIE_UNCORR_ERR_MASK
* Register Description :
* The Uncorrectable Error Mask register controls reporting of individual uncorrectable errors by a function.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define PCIE_UNCORR_ERR_MASK_Reserved_3_0_OFFSET      0
#define PCIE_UNCORR_ERR_MASK_Reserved_3_0_MASK        0xf

// Bitfield Description : Controls if the function can report a Data Link Protocol Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_DLP_ERR_MASK_OFFSET      4
#define PCIE_UNCORR_ERR_MASK_DLP_ERR_MASK_MASK        0x10

// Bitfield Description : (Downstream Ports only) Controls if the function can report a Surprise Down Error.
#define PCIE_UNCORR_ERR_MASK_SURPDN_ERR_MASK_OFFSET      5
#define PCIE_UNCORR_ERR_MASK_SURPDN_ERR_MASK_MASK        0x20

// Bitfield Description : 
#define PCIE_UNCORR_ERR_MASK_Reserved_11_6_OFFSET      6
#define PCIE_UNCORR_ERR_MASK_Reserved_11_6_MASK        0xfc0

// Bitfield Description : Controls if the function can report a Poisoned TLP Received Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_PSN_ERR_MASK_OFFSET      12
#define PCIE_UNCORR_ERR_MASK_PSN_ERR_MASK_MASK        0x1000

// Bitfield Description : Controls if the function can report a Flow Control Protocol Error.
#define PCIE_UNCORR_ERR_MASK_FC_ERR_MASK_OFFSET      13
#define PCIE_UNCORR_ERR_MASK_FC_ERR_MASK_MASK        0x2000

// Bitfield Description : Controls if the function can report a Completion Timeout Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_CPL_TIMEOUT_MASK_OFFSET      14
#define PCIE_UNCORR_ERR_MASK_CPL_TIMEOUT_MASK_MASK        0x4000

// Bitfield Description : Controls if the function can report a Completer Abort Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_CPL_ABORT_ERR_MASK_OFFSET      15
#define PCIE_UNCORR_ERR_MASK_CPL_ABORT_ERR_MASK_MASK        0x8000

// Bitfield Description : Controls if the function can report an Unexpected Completion Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_UNEXP_CPL_MASK_OFFSET      16
#define PCIE_UNCORR_ERR_MASK_UNEXP_CPL_MASK_MASK        0x10000

// Bitfield Description : Controls if the function can report a Receiver Overflow Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_RCV_OVFL_MASK_OFFSET      17
#define PCIE_UNCORR_ERR_MASK_RCV_OVFL_MASK_MASK        0x20000

// Bitfield Description : Controls if the function can report a Malformed TLP Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_MAL_TLP_MASK_OFFSET      18
#define PCIE_UNCORR_ERR_MASK_MAL_TLP_MASK_MASK        0x40000

// Bitfield Description : Controls if the function can report an ECRC Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_ECRC_ERR_MASK_OFFSET      19
#define PCIE_UNCORR_ERR_MASK_ECRC_ERR_MASK_MASK        0x80000

// Bitfield Description : Controls if the function can report an Unsupported Request Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_UNSUPP_REQ_ERR_MASK_OFFSET      20
#define PCIE_UNCORR_ERR_MASK_UNSUPP_REQ_ERR_MASK_MASK        0x100000

// Bitfield Description : Controls if the function can report an ACS Violation Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_ACS_VIOLATION_MASK_OFFSET      21
#define PCIE_UNCORR_ERR_MASK_ACS_VIOLATION_MASK_MASK        0x200000

// Bitfield Description : Controls if the function can report an Uncorrectable Internal Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_UNCORR_INT_ERR_MASK_OFFSET      22
#define PCIE_UNCORR_ERR_MASK_UNCORR_INT_ERR_MASK_MASK        0x400000

// Bitfield Description : Controls if the function can report a MC Blocked TLP Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_MC_BLOCKED_TLP_MASK_OFFSET      23
#define PCIE_UNCORR_ERR_MASK_MC_BLOCKED_TLP_MASK_MASK        0x800000

// Bitfield Description : Controls if the function can report an AtomicOp Egress Blocked Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_ATOMICOP_EGRESS_BLOCKED_MASK_OFFSET      24
#define PCIE_UNCORR_ERR_MASK_ATOMICOP_EGRESS_BLOCKED_MASK_MASK        0x1000000

// Bitfield Description : Controls if the function can report a TLP Prefix Blocked Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_TLP_PREFIX_BLOCKED_ERR_MASK_OFFSET      25
#define PCIE_UNCORR_ERR_MASK_TLP_PREFIX_BLOCKED_ERR_MASK_MASK        0x2000000

// Bitfield Description : Controls if the function can report a Poisoned TLP Egress Blocked Error. This bit is sticky.
#define PCIE_UNCORR_ERR_MASK_POISONED_TLP_EGRESS_BLOCKED_MASK_OFFSET      26
#define PCIE_UNCORR_ERR_MASK_POISONED_TLP_EGRESS_BLOCKED_MASK_MASK        0x4000000

// Bitfield Description : 
#define PCIE_UNCORR_ERR_MASK_Reserved_31_27_OFFSET      27
#define PCIE_UNCORR_ERR_MASK_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            DLP_ERR_MASK:1;
    UINT32                            SURPDN_ERR_MASK:1;
    UINT32                            Reserved_11_6:6;
    UINT32                            PSN_ERR_MASK:1;
    UINT32                            FC_ERR_MASK:1;
    UINT32                            CPL_TIMEOUT_MASK:1;
    UINT32                            CPL_ABORT_ERR_MASK:1;
    UINT32                            UNEXP_CPL_MASK:1;
    UINT32                            RCV_OVFL_MASK:1;
    UINT32                            MAL_TLP_MASK:1;
    UINT32                            ECRC_ERR_MASK:1;
    UINT32                            UNSUPP_REQ_ERR_MASK:1;
    UINT32                            ACS_VIOLATION_MASK:1;
    UINT32                            UNCORR_INT_ERR_MASK:1;
    UINT32                            MC_BLOCKED_TLP_MASK:1;
    UINT32                            ATOMICOP_EGRESS_BLOCKED_MASK:1;
    UINT32                            TLP_PREFIX_BLOCKED_ERR_MASK:1;
    UINT32                            POISONED_TLP_EGRESS_BLOCKED_MASK:1;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} PCIE_UNCORR_ERR_MASK_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11100158UL

#define SMN_FUNC0_PCIE1_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11200158UL

#define SMN_FUNC0_PCIE2_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11300158UL

#define SMN_FUNC0_PCIE3_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11400158UL

#define SMN_FUNC1_PCIE0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11101158UL

#define SMN_FUNC1_PCIE1_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11201158UL

#define SMN_FUNC2_PCIE0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11102158UL

#define SMN_FUNC2_PCIE1_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11202158UL

#define SMN_FUNC3_PCIE0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11103158UL

#define SMN_FUNC3_PCIE1_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11203158UL

#define SMN_FUNC4_PCIE0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11104158UL

#define SMN_FUNC4_PCIE1_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11204158UL

#define SMN_FUNC5_PCIE1_PCIE_UNCORR_ERR_MASK_ADDRESS    0x11205158UL


/***********************************************************
* Register Name : PCIE_UNCORR_ERR_SEVERITY
* Register Description :
* The Uncorrectable Error Severity register controls whether an individual uncorrectable error is reported by a function as a Non-fatal or Fatal error.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define PCIE_UNCORR_ERR_SEVERITY_Reserved_3_0_OFFSET      0
#define PCIE_UNCORR_ERR_SEVERITY_Reserved_3_0_MASK        0xf

// Bitfield Description : Controls how the function reports a Data Link Protocol Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_DLP_ERR_SEVERITY_OFFSET      4
#define PCIE_UNCORR_ERR_SEVERITY_DLP_ERR_SEVERITY_MASK        0x10

// Bitfield Description : (Downstream Ports only) Controls how the function reports a Surprise Down Error.
#define PCIE_UNCORR_ERR_SEVERITY_SURPDN_ERR_SEVERITY_OFFSET      5
#define PCIE_UNCORR_ERR_SEVERITY_SURPDN_ERR_SEVERITY_MASK        0x20

// Bitfield Description : 
#define PCIE_UNCORR_ERR_SEVERITY_Reserved_11_6_OFFSET      6
#define PCIE_UNCORR_ERR_SEVERITY_Reserved_11_6_MASK        0xfc0

// Bitfield Description : Controls how the function reports a Poisoned TLP Received Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_PSN_ERR_SEVERITY_OFFSET      12
#define PCIE_UNCORR_ERR_SEVERITY_PSN_ERR_SEVERITY_MASK        0x1000

// Bitfield Description : Controls how the function reports a Flow Control Protocol Error.
#define PCIE_UNCORR_ERR_SEVERITY_FC_ERR_SEVERITY_OFFSET      13
#define PCIE_UNCORR_ERR_SEVERITY_FC_ERR_SEVERITY_MASK        0x2000

// Bitfield Description : Controls how the function reports a Completion Timeout Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_CPL_TIMEOUT_SEVERITY_OFFSET      14
#define PCIE_UNCORR_ERR_SEVERITY_CPL_TIMEOUT_SEVERITY_MASK        0x4000

// Bitfield Description : Controls how the function reports a Completer Abort Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_CPL_ABORT_ERR_SEVERITY_OFFSET      15
#define PCIE_UNCORR_ERR_SEVERITY_CPL_ABORT_ERR_SEVERITY_MASK        0x8000

// Bitfield Description : Controls how the function reports an Unexpected Completion Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_UNEXP_CPL_SEVERITY_OFFSET      16
#define PCIE_UNCORR_ERR_SEVERITY_UNEXP_CPL_SEVERITY_MASK        0x10000

// Bitfield Description : Controls how the function reports a Receiver Overflow Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_RCV_OVFL_SEVERITY_OFFSET      17
#define PCIE_UNCORR_ERR_SEVERITY_RCV_OVFL_SEVERITY_MASK        0x20000

// Bitfield Description : Controls how the function reports a Malformed TLP Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_MAL_TLP_SEVERITY_OFFSET      18
#define PCIE_UNCORR_ERR_SEVERITY_MAL_TLP_SEVERITY_MASK        0x40000

// Bitfield Description : Controls how the function reports an ECRC Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_ECRC_ERR_SEVERITY_OFFSET      19
#define PCIE_UNCORR_ERR_SEVERITY_ECRC_ERR_SEVERITY_MASK        0x80000

// Bitfield Description : Controls how the function reports an Unsupported Request Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_UNSUPP_REQ_ERR_SEVERITY_OFFSET      20
#define PCIE_UNCORR_ERR_SEVERITY_UNSUPP_REQ_ERR_SEVERITY_MASK        0x100000

// Bitfield Description : Controls how the function reports an ACS Violation Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_ACS_VIOLATION_SEVERITY_OFFSET      21
#define PCIE_UNCORR_ERR_SEVERITY_ACS_VIOLATION_SEVERITY_MASK        0x200000

// Bitfield Description : Controls how the function reports an Uncorrectable Internal Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_UNCORR_INT_ERR_SEVERITY_OFFSET      22
#define PCIE_UNCORR_ERR_SEVERITY_UNCORR_INT_ERR_SEVERITY_MASK        0x400000

// Bitfield Description : Controls how the function reports a MC Blocked TLP Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_MC_BLOCKED_TLP_SEVERITY_OFFSET      23
#define PCIE_UNCORR_ERR_SEVERITY_MC_BLOCKED_TLP_SEVERITY_MASK        0x800000

// Bitfield Description : Controls how the function reports an AtomicOp Egress Blocked Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_ATOMICOP_EGRESS_BLOCKED_SEVERITY_OFFSET      24
#define PCIE_UNCORR_ERR_SEVERITY_ATOMICOP_EGRESS_BLOCKED_SEVERITY_MASK        0x1000000

// Bitfield Description : Controls how the function reports a TLP Prefix Blocked Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_TLP_PREFIX_BLOCKED_ERR_SEVERITY_OFFSET      25
#define PCIE_UNCORR_ERR_SEVERITY_TLP_PREFIX_BLOCKED_ERR_SEVERITY_MASK        0x2000000

// Bitfield Description : Controls how the function reports a Poisoned TLP Egress Blocked Error. This bit is sticky.
#define PCIE_UNCORR_ERR_SEVERITY_POISONED_TLP_EGRESS_BLOCKED_SEVERITY_OFFSET      26
#define PCIE_UNCORR_ERR_SEVERITY_POISONED_TLP_EGRESS_BLOCKED_SEVERITY_MASK        0x4000000

// Bitfield Description : 
#define PCIE_UNCORR_ERR_SEVERITY_Reserved_31_27_OFFSET      27
#define PCIE_UNCORR_ERR_SEVERITY_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            DLP_ERR_SEVERITY:1;
    UINT32                            SURPDN_ERR_SEVERITY:1;
    UINT32                            Reserved_11_6:6;
    UINT32                            PSN_ERR_SEVERITY:1;
    UINT32                            FC_ERR_SEVERITY:1;
    UINT32                            CPL_TIMEOUT_SEVERITY:1;
    UINT32                            CPL_ABORT_ERR_SEVERITY:1;
    UINT32                            UNEXP_CPL_SEVERITY:1;
    UINT32                            RCV_OVFL_SEVERITY:1;
    UINT32                            MAL_TLP_SEVERITY:1;
    UINT32                            ECRC_ERR_SEVERITY:1;
    UINT32                            UNSUPP_REQ_ERR_SEVERITY:1;
    UINT32                            ACS_VIOLATION_SEVERITY:1;
    UINT32                            UNCORR_INT_ERR_SEVERITY:1;
    UINT32                            MC_BLOCKED_TLP_SEVERITY:1;
    UINT32                            ATOMICOP_EGRESS_BLOCKED_SEVERITY:1;
    UINT32                            TLP_PREFIX_BLOCKED_ERR_SEVERITY:1;
    UINT32                            POISONED_TLP_EGRESS_BLOCKED_SEVERITY:1;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} PCIE_UNCORR_ERR_SEVERITY_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1110015cUL

#define SMN_FUNC0_PCIE1_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1120015cUL

#define SMN_FUNC0_PCIE2_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1130015cUL

#define SMN_FUNC0_PCIE3_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1140015cUL

#define SMN_FUNC1_PCIE0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1110115cUL

#define SMN_FUNC1_PCIE1_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1120115cUL

#define SMN_FUNC2_PCIE0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1110215cUL

#define SMN_FUNC2_PCIE1_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1120215cUL

#define SMN_FUNC3_PCIE0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1110315cUL

#define SMN_FUNC3_PCIE1_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1120315cUL

#define SMN_FUNC4_PCIE0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1110415cUL

#define SMN_FUNC4_PCIE1_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1120415cUL

#define SMN_FUNC5_PCIE1_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1120515cUL


/***********************************************************
* Register Name : PCIE_UNCORR_ERR_STATUS
* Register Description :
* The Uncorrectable Error Status register reports error status of individual uncorrectable errors in a function.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define PCIE_UNCORR_ERR_STATUS_Reserved_3_0_OFFSET      0
#define PCIE_UNCORR_ERR_STATUS_Reserved_3_0_MASK        0xf

// Bitfield Description : Indicates if the function detects a Data Link Protocol Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_DLP_ERR_STATUS_OFFSET      4
#define PCIE_UNCORR_ERR_STATUS_DLP_ERR_STATUS_MASK        0x10

// Bitfield Description : (Downstream Ports only) Indicates if the function detects a Surprise Down Error.
#define PCIE_UNCORR_ERR_STATUS_SURPDN_ERR_STATUS_OFFSET      5
#define PCIE_UNCORR_ERR_STATUS_SURPDN_ERR_STATUS_MASK        0x20

// Bitfield Description : 
#define PCIE_UNCORR_ERR_STATUS_Reserved_11_6_OFFSET      6
#define PCIE_UNCORR_ERR_STATUS_Reserved_11_6_MASK        0xfc0

// Bitfield Description : Indicates if the function detects a Poisoned TLP Received Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_PSN_ERR_STATUS_OFFSET      12
#define PCIE_UNCORR_ERR_STATUS_PSN_ERR_STATUS_MASK        0x1000

// Bitfield Description : Indicates if the function detects a Flow Control Protocol Error.
#define PCIE_UNCORR_ERR_STATUS_FC_ERR_STATUS_OFFSET      13
#define PCIE_UNCORR_ERR_STATUS_FC_ERR_STATUS_MASK        0x2000

// Bitfield Description : Indicates if the function detects a Completion Timeout Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_CPL_TIMEOUT_STATUS_OFFSET      14
#define PCIE_UNCORR_ERR_STATUS_CPL_TIMEOUT_STATUS_MASK        0x4000

// Bitfield Description : Indicates if the function detects a Completer Abort Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_CPL_ABORT_ERR_STATUS_OFFSET      15
#define PCIE_UNCORR_ERR_STATUS_CPL_ABORT_ERR_STATUS_MASK        0x8000

// Bitfield Description : Indicates if the function detects an Unexpected Completion Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_UNEXP_CPL_STATUS_OFFSET      16
#define PCIE_UNCORR_ERR_STATUS_UNEXP_CPL_STATUS_MASK        0x10000

// Bitfield Description : Indicates if the function detects a Receiver Overflow Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_RCV_OVFL_STATUS_OFFSET      17
#define PCIE_UNCORR_ERR_STATUS_RCV_OVFL_STATUS_MASK        0x20000

// Bitfield Description : Indicates if the function detects a Malformed TLP Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_MAL_TLP_STATUS_OFFSET      18
#define PCIE_UNCORR_ERR_STATUS_MAL_TLP_STATUS_MASK        0x40000

// Bitfield Description : Indicates if the function detects an ECRC Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_ECRC_ERR_STATUS_OFFSET      19
#define PCIE_UNCORR_ERR_STATUS_ECRC_ERR_STATUS_MASK        0x80000

// Bitfield Description : Indicates if the function detects an Unsupported Request Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_UNSUPP_REQ_ERR_STATUS_OFFSET      20
#define PCIE_UNCORR_ERR_STATUS_UNSUPP_REQ_ERR_STATUS_MASK        0x100000

// Bitfield Description : Indicates if the function detects an ACS Violation Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_ACS_VIOLATION_STATUS_OFFSET      21
#define PCIE_UNCORR_ERR_STATUS_ACS_VIOLATION_STATUS_MASK        0x200000

// Bitfield Description : Indicates if the function detects an Uncorrectable Internal Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_UNCORR_INT_ERR_STATUS_OFFSET      22
#define PCIE_UNCORR_ERR_STATUS_UNCORR_INT_ERR_STATUS_MASK        0x400000

// Bitfield Description : Indicates if the function detects a MC Blocked TLP. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_MC_BLOCKED_TLP_STATUS_OFFSET      23
#define PCIE_UNCORR_ERR_STATUS_MC_BLOCKED_TLP_STATUS_MASK        0x800000

// Bitfield Description : Indicates if the function detects an AtomicOp Egress Blocked Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_ATOMICOP_EGRESS_BLOCKED_STATUS_OFFSET      24
#define PCIE_UNCORR_ERR_STATUS_ATOMICOP_EGRESS_BLOCKED_STATUS_MASK        0x1000000

// Bitfield Description : Indicates if the function detects a TLP Prefix Blocked Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_TLP_PREFIX_BLOCKED_ERR_STATUS_OFFSET      25
#define PCIE_UNCORR_ERR_STATUS_TLP_PREFIX_BLOCKED_ERR_STATUS_MASK        0x2000000

// Bitfield Description : Indicates if the function detects a Poisoned TLP Egress Blocked Error. This bit is sticky.
#define PCIE_UNCORR_ERR_STATUS_POISONED_TLP_EGRESS_BLOCKED_STATUS_OFFSET      26
#define PCIE_UNCORR_ERR_STATUS_POISONED_TLP_EGRESS_BLOCKED_STATUS_MASK        0x4000000

// Bitfield Description : 
#define PCIE_UNCORR_ERR_STATUS_Reserved_31_27_OFFSET      27
#define PCIE_UNCORR_ERR_STATUS_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            DLP_ERR_STATUS:1;
    UINT32                            SURPDN_ERR_STATUS:1;
    UINT32                            Reserved_11_6:6;
    UINT32                            PSN_ERR_STATUS:1;
    UINT32                            FC_ERR_STATUS:1;
    UINT32                            CPL_TIMEOUT_STATUS:1;
    UINT32                            CPL_ABORT_ERR_STATUS:1;
    UINT32                            UNEXP_CPL_STATUS:1;
    UINT32                            RCV_OVFL_STATUS:1;
    UINT32                            MAL_TLP_STATUS:1;
    UINT32                            ECRC_ERR_STATUS:1;
    UINT32                            UNSUPP_REQ_ERR_STATUS:1;
    UINT32                            ACS_VIOLATION_STATUS:1;
    UINT32                            UNCORR_INT_ERR_STATUS:1;
    UINT32                            MC_BLOCKED_TLP_STATUS:1;
    UINT32                            ATOMICOP_EGRESS_BLOCKED_STATUS:1;
    UINT32                            TLP_PREFIX_BLOCKED_ERR_STATUS:1;
    UINT32                            POISONED_TLP_EGRESS_BLOCKED_STATUS:1;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} PCIE_UNCORR_ERR_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11100154UL

#define SMN_FUNC0_PCIE1_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11200154UL

#define SMN_FUNC0_PCIE2_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11300154UL

#define SMN_FUNC0_PCIE3_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11400154UL

#define SMN_FUNC1_PCIE0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11101154UL

#define SMN_FUNC1_PCIE1_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11201154UL

#define SMN_FUNC2_PCIE0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11102154UL

#define SMN_FUNC2_PCIE1_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11202154UL

#define SMN_FUNC3_PCIE0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11103154UL

#define SMN_FUNC3_PCIE1_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11203154UL

#define SMN_FUNC4_PCIE0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11104154UL

#define SMN_FUNC4_PCIE1_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11204154UL

#define SMN_FUNC5_PCIE1_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x11205154UL


/***********************************************************
* Register Name : PCIE_VC0_RESOURCE_CAP
* Register Description :
* VC0 Resource Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the types of Port Arbitration supported by the VC resource. Bit 0 = Fixed; Bit 1 = WRR-32; Bit 2 = WRR-64; Bit 3 = WRR-128; Bit 4 = Timed WRR-128; Bit 5 = WRR-256.
#define PCIE_VC0_RESOURCE_CAP_PORT_ARB_CAP_OFFSET      0
#define PCIE_VC0_RESOURCE_CAP_PORT_ARB_CAP_MASK        0xff

// Bitfield Description : 
#define PCIE_VC0_RESOURCE_CAP_Reserved_14_8_OFFSET      8
#define PCIE_VC0_RESOURCE_CAP_Reserved_14_8_MASK        0x7f00

// Bitfield Description : Controls if the Virtual Channel is permitted to reject transactions for which the No Snoop Attribute is applicable, but is not set.
#define PCIE_VC0_RESOURCE_CAP_REJECT_SNOOP_TRANS_OFFSET      15
#define PCIE_VC0_RESOURCE_CAP_REJECT_SNOOP_TRANS_MASK        0x8000

// Bitfield Description : Indicates the maximum number of time slots (minus one) that the VC resource is capable of supporting, when configured for Timed WRR Port Arbitration.
#define PCIE_VC0_RESOURCE_CAP_MAX_TIME_SLOTS_OFFSET      16
#define PCIE_VC0_RESOURCE_CAP_MAX_TIME_SLOTS_MASK        0x7f0000

// Bitfield Description : 
#define PCIE_VC0_RESOURCE_CAP_Reserved_23_23_OFFSET      23
#define PCIE_VC0_RESOURCE_CAP_Reserved_23_23_MASK        0x800000

// Bitfield Description : Indicates the location of the Port Arbitration Table associated with the VC resource, or must be 0 if the Port Arbitration Table is not present.
#define PCIE_VC0_RESOURCE_CAP_PORT_ARB_TABLE_OFFSET_OFFSET      24
#define PCIE_VC0_RESOURCE_CAP_PORT_ARB_TABLE_OFFSET_MASK        0xff000000

typedef union {
  struct {
    UINT32                            PORT_ARB_CAP:8;
    UINT32                            Reserved_14_8:7;
    UINT32                            REJECT_SNOOP_TRANS:1;
    UINT32                            MAX_TIME_SLOTS:7;
    UINT32                            Reserved_23_23:1;
    UINT32                            PORT_ARB_TABLE_OFFSET:8;
  } Field;
  UINT32 Value;
} PCIE_VC0_RESOURCE_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11100120UL

#define SMN_FUNC0_PCIE1_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11200120UL

#define SMN_FUNC0_PCIE2_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11300120UL

#define SMN_FUNC0_PCIE3_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11400120UL

#define SMN_FUNC1_PCIE0_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11101120UL

#define SMN_FUNC1_PCIE1_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11201120UL

#define SMN_FUNC2_PCIE0_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11102120UL

#define SMN_FUNC2_PCIE1_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11202120UL

#define SMN_FUNC3_PCIE0_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11103120UL

#define SMN_FUNC3_PCIE1_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11203120UL

#define SMN_FUNC4_PCIE0_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11104120UL

#define SMN_FUNC4_PCIE1_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11204120UL

#define SMN_FUNC5_PCIE1_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x11205120UL


/***********************************************************
* Register Name : PCIE_VC0_RESOURCE_CNTL
* Register Description :
* VC0 Resource Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the TCs that are mapped to the VC resource.
#define PCIE_VC0_RESOURCE_CNTL_TC_VC_MAP_TC0_OFFSET      0
#define PCIE_VC0_RESOURCE_CNTL_TC_VC_MAP_TC0_MASK        0x1

// Bitfield Description : Indicates the TCs that are mapped to the VC resource.
#define PCIE_VC0_RESOURCE_CNTL_TC_VC_MAP_TC1_7_OFFSET      1
#define PCIE_VC0_RESOURCE_CNTL_TC_VC_MAP_TC1_7_MASK        0xfe

// Bitfield Description : 
#define PCIE_VC0_RESOURCE_CNTL_Reserved_15_8_OFFSET      8
#define PCIE_VC0_RESOURCE_CNTL_Reserved_15_8_MASK        0xff00

// Bitfield Description : Updates the Port Arbitration logic from the Port Arbitration Table (if present) for the VC resource, when written with 1. Reads always return 0.
#define PCIE_VC0_RESOURCE_CNTL_LOAD_PORT_ARB_TABLE_OFFSET      16
#define PCIE_VC0_RESOURCE_CNTL_LOAD_PORT_ARB_TABLE_MASK        0x10000

// Bitfield Description : Configures the Port Arbitration for the VC resource by selecting one of the supported Port Arbitration schemes.
#define PCIE_VC0_RESOURCE_CNTL_PORT_ARB_SELECT_OFFSET      17
#define PCIE_VC0_RESOURCE_CNTL_PORT_ARB_SELECT_MASK        0xe0000

// Bitfield Description : 
#define PCIE_VC0_RESOURCE_CNTL_Reserved_23_20_OFFSET      20
#define PCIE_VC0_RESOURCE_CNTL_Reserved_23_20_MASK        0xf00000

// Bitfield Description : Assigns a VC ID to the VC resource.
#define PCIE_VC0_RESOURCE_CNTL_VC_ID_OFFSET      24
#define PCIE_VC0_RESOURCE_CNTL_VC_ID_MASK        0x7000000

// Bitfield Description : 
#define PCIE_VC0_RESOURCE_CNTL_Reserved_30_27_OFFSET      27
#define PCIE_VC0_RESOURCE_CNTL_Reserved_30_27_MASK        0x78000000

// Bitfield Description : Controls the enabling of a Virtual Channel.
#define PCIE_VC0_RESOURCE_CNTL_VC_ENABLE_OFFSET      31
#define PCIE_VC0_RESOURCE_CNTL_VC_ENABLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            TC_VC_MAP_TC0:1;
    UINT32                            TC_VC_MAP_TC1_7:7;
    UINT32                            Reserved_15_8:8;
    UINT32                            LOAD_PORT_ARB_TABLE:1;
    UINT32                            PORT_ARB_SELECT:3;
    UINT32                            Reserved_23_20:4;
    UINT32                            VC_ID:3;
    UINT32                            Reserved_30_27:4;
    UINT32                            VC_ENABLE:1;
  } Field;
  UINT32 Value;
} PCIE_VC0_RESOURCE_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11100124UL

#define SMN_FUNC0_PCIE1_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11200124UL

#define SMN_FUNC0_PCIE2_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11300124UL

#define SMN_FUNC0_PCIE3_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11400124UL

#define SMN_FUNC1_PCIE0_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11101124UL

#define SMN_FUNC1_PCIE1_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11201124UL

#define SMN_FUNC2_PCIE0_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11102124UL

#define SMN_FUNC2_PCIE1_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11202124UL

#define SMN_FUNC3_PCIE0_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11103124UL

#define SMN_FUNC3_PCIE1_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11203124UL

#define SMN_FUNC4_PCIE0_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11104124UL

#define SMN_FUNC4_PCIE1_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11204124UL

#define SMN_FUNC5_PCIE1_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x11205124UL


/***********************************************************
* Register Name : PCIE_VC0_RESOURCE_STATUS
* Register Description :
* VC0 Resource Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the contents of the Port Arbitration Table associated with the VC resource are coherent, or must be 0 if the Port Arbitration Table is not present.
#define PCIE_VC0_RESOURCE_STATUS_PORT_ARB_TABLE_STATUS_OFFSET      0
#define PCIE_VC0_RESOURCE_STATUS_PORT_ARB_TABLE_STATUS_MASK        0x1

// Bitfield Description : Indicates whether the Virtual Channel negotiation (initialization or disabling) is in pending state.
#define PCIE_VC0_RESOURCE_STATUS_VC_NEGOTIATION_PENDING_OFFSET      1
#define PCIE_VC0_RESOURCE_STATUS_VC_NEGOTIATION_PENDING_MASK        0x2

// Bitfield Description : 
#define PCIE_VC0_RESOURCE_STATUS_Reserved_15_2_OFFSET      2
#define PCIE_VC0_RESOURCE_STATUS_Reserved_15_2_MASK        0xfffc

typedef union {
  struct {
    UINT16                            PORT_ARB_TABLE_STATUS:1;
    UINT16                            VC_NEGOTIATION_PENDING:1;
    UINT16                            Reserved_15_2:14;
  } Field;
  UINT16 Value;
} PCIE_VC0_RESOURCE_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1110012aUL

#define SMN_FUNC0_PCIE1_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1120012aUL

#define SMN_FUNC0_PCIE2_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1130012aUL

#define SMN_FUNC0_PCIE3_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1140012aUL

#define SMN_FUNC1_PCIE0_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1110112aUL

#define SMN_FUNC1_PCIE1_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1120112aUL

#define SMN_FUNC2_PCIE0_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1110212aUL

#define SMN_FUNC2_PCIE1_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1120212aUL

#define SMN_FUNC3_PCIE0_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1110312aUL

#define SMN_FUNC3_PCIE1_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1120312aUL

#define SMN_FUNC4_PCIE0_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1110412aUL

#define SMN_FUNC4_PCIE1_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1120412aUL

#define SMN_FUNC5_PCIE1_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1120512aUL


/***********************************************************
* Register Name : PCIE_VC1_RESOURCE_CAP
* Register Description :
* VC1 Resource Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the types of Port Arbitration supported by the VC resource. Bit 0 = Fixed; Bit 1 = WRR-32; Bit 2 = WRR-64; Bit 3 = WRR-128; Bit 4 = Timed WRR-128; Bit 5 = WRR-256.
#define PCIE_VC1_RESOURCE_CAP_PORT_ARB_CAP_OFFSET      0
#define PCIE_VC1_RESOURCE_CAP_PORT_ARB_CAP_MASK        0xff

// Bitfield Description : 
#define PCIE_VC1_RESOURCE_CAP_Reserved_14_8_OFFSET      8
#define PCIE_VC1_RESOURCE_CAP_Reserved_14_8_MASK        0x7f00

// Bitfield Description : Controls if the Virtual Channel is permitted to reject transactions for which the No Snoop Attribute is applicable, but is not set.
#define PCIE_VC1_RESOURCE_CAP_REJECT_SNOOP_TRANS_OFFSET      15
#define PCIE_VC1_RESOURCE_CAP_REJECT_SNOOP_TRANS_MASK        0x8000

// Bitfield Description : Indicates the maximum number of time slots (minus one) that the VC resource is capable of supporting, when configured for Timed WRR Port Arbitration.
#define PCIE_VC1_RESOURCE_CAP_MAX_TIME_SLOTS_OFFSET      16
#define PCIE_VC1_RESOURCE_CAP_MAX_TIME_SLOTS_MASK        0x7f0000

// Bitfield Description : 
#define PCIE_VC1_RESOURCE_CAP_Reserved_23_23_OFFSET      23
#define PCIE_VC1_RESOURCE_CAP_Reserved_23_23_MASK        0x800000

// Bitfield Description : Indicates the location of the Port Arbitration Table associated with the VC resource, or must be 0 if the Port Arbitration Table is not present.
#define PCIE_VC1_RESOURCE_CAP_PORT_ARB_TABLE_OFFSET_OFFSET      24
#define PCIE_VC1_RESOURCE_CAP_PORT_ARB_TABLE_OFFSET_MASK        0xff000000

typedef union {
  struct {
    UINT32                            PORT_ARB_CAP:8;
    UINT32                            Reserved_14_8:7;
    UINT32                            REJECT_SNOOP_TRANS:1;
    UINT32                            MAX_TIME_SLOTS:7;
    UINT32                            Reserved_23_23:1;
    UINT32                            PORT_ARB_TABLE_OFFSET:8;
  } Field;
  UINT32 Value;
} PCIE_VC1_RESOURCE_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1110012cUL

#define SMN_FUNC0_PCIE1_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1120012cUL

#define SMN_FUNC0_PCIE2_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1130012cUL

#define SMN_FUNC0_PCIE3_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1140012cUL

#define SMN_FUNC1_PCIE0_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1110112cUL

#define SMN_FUNC1_PCIE1_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1120112cUL

#define SMN_FUNC2_PCIE0_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1110212cUL

#define SMN_FUNC2_PCIE1_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1120212cUL

#define SMN_FUNC3_PCIE0_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1110312cUL

#define SMN_FUNC3_PCIE1_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1120312cUL

#define SMN_FUNC4_PCIE0_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1110412cUL

#define SMN_FUNC4_PCIE1_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1120412cUL

#define SMN_FUNC5_PCIE1_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1120512cUL


/***********************************************************
* Register Name : PCIE_VC1_RESOURCE_CNTL
* Register Description :
* VC1 Resource Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the TCs that are mapped to the VC resource.
#define PCIE_VC1_RESOURCE_CNTL_TC_VC_MAP_TC0_OFFSET      0
#define PCIE_VC1_RESOURCE_CNTL_TC_VC_MAP_TC0_MASK        0x1

// Bitfield Description : Indicates the TCs that are mapped to the VC resource.
#define PCIE_VC1_RESOURCE_CNTL_TC_VC_MAP_TC1_7_OFFSET      1
#define PCIE_VC1_RESOURCE_CNTL_TC_VC_MAP_TC1_7_MASK        0xfe

// Bitfield Description : 
#define PCIE_VC1_RESOURCE_CNTL_Reserved_15_8_OFFSET      8
#define PCIE_VC1_RESOURCE_CNTL_Reserved_15_8_MASK        0xff00

// Bitfield Description : Updates the Port Arbitration logic from the Port Arbitration Table (if present) for the VC resource, when written with 1. Reads always return 0.
#define PCIE_VC1_RESOURCE_CNTL_LOAD_PORT_ARB_TABLE_OFFSET      16
#define PCIE_VC1_RESOURCE_CNTL_LOAD_PORT_ARB_TABLE_MASK        0x10000

// Bitfield Description : Configures the Port Arbitration for the VC resource by selecting one of the supported Port Arbitration schemes.
#define PCIE_VC1_RESOURCE_CNTL_PORT_ARB_SELECT_OFFSET      17
#define PCIE_VC1_RESOURCE_CNTL_PORT_ARB_SELECT_MASK        0xe0000

// Bitfield Description : 
#define PCIE_VC1_RESOURCE_CNTL_Reserved_23_20_OFFSET      20
#define PCIE_VC1_RESOURCE_CNTL_Reserved_23_20_MASK        0xf00000

// Bitfield Description : Assigns a VC ID to the VC resource.
#define PCIE_VC1_RESOURCE_CNTL_VC_ID_OFFSET      24
#define PCIE_VC1_RESOURCE_CNTL_VC_ID_MASK        0x7000000

// Bitfield Description : 
#define PCIE_VC1_RESOURCE_CNTL_Reserved_30_27_OFFSET      27
#define PCIE_VC1_RESOURCE_CNTL_Reserved_30_27_MASK        0x78000000

// Bitfield Description : Controls the enabling of a Virtual Channel.
#define PCIE_VC1_RESOURCE_CNTL_VC_ENABLE_OFFSET      31
#define PCIE_VC1_RESOURCE_CNTL_VC_ENABLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            TC_VC_MAP_TC0:1;
    UINT32                            TC_VC_MAP_TC1_7:7;
    UINT32                            Reserved_15_8:8;
    UINT32                            LOAD_PORT_ARB_TABLE:1;
    UINT32                            PORT_ARB_SELECT:3;
    UINT32                            Reserved_23_20:4;
    UINT32                            VC_ID:3;
    UINT32                            Reserved_30_27:4;
    UINT32                            VC_ENABLE:1;
  } Field;
  UINT32 Value;
} PCIE_VC1_RESOURCE_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11100130UL

#define SMN_FUNC0_PCIE1_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11200130UL

#define SMN_FUNC0_PCIE2_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11300130UL

#define SMN_FUNC0_PCIE3_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11400130UL

#define SMN_FUNC1_PCIE0_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11101130UL

#define SMN_FUNC1_PCIE1_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11201130UL

#define SMN_FUNC2_PCIE0_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11102130UL

#define SMN_FUNC2_PCIE1_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11202130UL

#define SMN_FUNC3_PCIE0_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11103130UL

#define SMN_FUNC3_PCIE1_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11203130UL

#define SMN_FUNC4_PCIE0_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11104130UL

#define SMN_FUNC4_PCIE1_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11204130UL

#define SMN_FUNC5_PCIE1_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x11205130UL


/***********************************************************
* Register Name : PCIE_VC1_RESOURCE_STATUS
* Register Description :
* VC1 Resource Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the contents of the Port Arbitration Table associated with the VC resource are coherent, or must be 0 if the Port Arbitration Table is not present.
#define PCIE_VC1_RESOURCE_STATUS_PORT_ARB_TABLE_STATUS_OFFSET      0
#define PCIE_VC1_RESOURCE_STATUS_PORT_ARB_TABLE_STATUS_MASK        0x1

// Bitfield Description : Indicates whether the Virtual Channel negotiation (initialization or disabling) is in pending state.
#define PCIE_VC1_RESOURCE_STATUS_VC_NEGOTIATION_PENDING_OFFSET      1
#define PCIE_VC1_RESOURCE_STATUS_VC_NEGOTIATION_PENDING_MASK        0x2

// Bitfield Description : 
#define PCIE_VC1_RESOURCE_STATUS_Reserved_15_2_OFFSET      2
#define PCIE_VC1_RESOURCE_STATUS_Reserved_15_2_MASK        0xfffc

typedef union {
  struct {
    UINT16                            PORT_ARB_TABLE_STATUS:1;
    UINT16                            VC_NEGOTIATION_PENDING:1;
    UINT16                            Reserved_15_2:14;
  } Field;
  UINT16 Value;
} PCIE_VC1_RESOURCE_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11100136UL

#define SMN_FUNC0_PCIE1_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11200136UL

#define SMN_FUNC0_PCIE2_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11300136UL

#define SMN_FUNC0_PCIE3_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11400136UL

#define SMN_FUNC1_PCIE0_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11101136UL

#define SMN_FUNC1_PCIE1_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11201136UL

#define SMN_FUNC2_PCIE0_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11102136UL

#define SMN_FUNC2_PCIE1_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11202136UL

#define SMN_FUNC3_PCIE0_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11103136UL

#define SMN_FUNC3_PCIE1_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11203136UL

#define SMN_FUNC4_PCIE0_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11104136UL

#define SMN_FUNC4_PCIE1_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11204136UL

#define SMN_FUNC5_PCIE1_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x11205136UL


/***********************************************************
* Register Name : PCIE_VC_ENH_CAP_LIST
* Register Description :
* Virtual Channel Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_VC_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_VC_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_VC_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_VC_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_VC_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_VC_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_VC_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11100110UL

#define SMN_FUNC0_PCIE1_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11200110UL

#define SMN_FUNC0_PCIE2_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11300110UL

#define SMN_FUNC0_PCIE3_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11400110UL

#define SMN_FUNC1_PCIE0_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11101110UL

#define SMN_FUNC1_PCIE1_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11201110UL

#define SMN_FUNC2_PCIE0_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11102110UL

#define SMN_FUNC2_PCIE1_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11202110UL

#define SMN_FUNC3_PCIE0_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11103110UL

#define SMN_FUNC3_PCIE1_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11203110UL

#define SMN_FUNC4_PCIE0_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11104110UL

#define SMN_FUNC4_PCIE1_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11204110UL

#define SMN_FUNC5_PCIE1_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x11205110UL


/***********************************************************
* Register Name : PCIE_VENDOR_SPECIFIC1
* Register Description :
* Vendor-Specific Scratch register 1
* Visibility : 0x2
************************************************************/

// Bitfield Description : PCIE scratch register.
#define PCIE_VENDOR_SPECIFIC1_SCRATCH_OFFSET      0
#define PCIE_VENDOR_SPECIFIC1_SCRATCH_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SCRATCH:32;
  } Field;
  UINT32 Value;
} PCIE_VENDOR_SPECIFIC1_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11100108UL

#define SMN_FUNC0_PCIE1_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11200108UL

#define SMN_FUNC0_PCIE2_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11300108UL

#define SMN_FUNC0_PCIE3_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11400108UL

#define SMN_FUNC1_PCIE0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11101108UL

#define SMN_FUNC1_PCIE1_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11201108UL

#define SMN_FUNC2_PCIE0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11102108UL

#define SMN_FUNC2_PCIE1_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11202108UL

#define SMN_FUNC3_PCIE0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11103108UL

#define SMN_FUNC3_PCIE1_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11203108UL

#define SMN_FUNC4_PCIE0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11104108UL

#define SMN_FUNC4_PCIE1_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11204108UL

#define SMN_FUNC5_PCIE1_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x11205108UL


/***********************************************************
* Register Name : PCIE_VENDOR_SPECIFIC2
* Register Description :
* Vendor-Specific Scratch register 2
* Visibility : 0x2
************************************************************/

// Bitfield Description : PCIE scratch register.
#define PCIE_VENDOR_SPECIFIC2_SCRATCH_OFFSET      0
#define PCIE_VENDOR_SPECIFIC2_SCRATCH_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SCRATCH:32;
  } Field;
  UINT32 Value;
} PCIE_VENDOR_SPECIFIC2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1110010cUL

#define SMN_FUNC0_PCIE1_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1120010cUL

#define SMN_FUNC0_PCIE2_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1130010cUL

#define SMN_FUNC0_PCIE3_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1140010cUL

#define SMN_FUNC1_PCIE0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1110110cUL

#define SMN_FUNC1_PCIE1_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1120110cUL

#define SMN_FUNC2_PCIE0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1110210cUL

#define SMN_FUNC2_PCIE1_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1120210cUL

#define SMN_FUNC3_PCIE0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1110310cUL

#define SMN_FUNC3_PCIE1_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1120310cUL

#define SMN_FUNC4_PCIE0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1110410cUL

#define SMN_FUNC4_PCIE1_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1120410cUL

#define SMN_FUNC5_PCIE1_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1120510cUL


/***********************************************************
* Register Name : PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST
* Register Description :
* Vendor-Specific Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11100100UL

#define SMN_FUNC0_PCIE1_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11200100UL

#define SMN_FUNC0_PCIE2_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11300100UL

#define SMN_FUNC0_PCIE3_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11400100UL

#define SMN_FUNC1_PCIE0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11101100UL

#define SMN_FUNC1_PCIE1_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11201100UL

#define SMN_FUNC2_PCIE0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11102100UL

#define SMN_FUNC2_PCIE1_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11202100UL

#define SMN_FUNC3_PCIE0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11103100UL

#define SMN_FUNC3_PCIE1_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11203100UL

#define SMN_FUNC4_PCIE0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11104100UL

#define SMN_FUNC4_PCIE1_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11204100UL

#define SMN_FUNC5_PCIE1_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x11205100UL


/***********************************************************
* Register Name : PCIE_VENDOR_SPECIFIC_HDR
* Register Description :
* Vendor Specific Header register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor-defined ID number.
#define PCIE_VENDOR_SPECIFIC_HDR_VSEC_ID_OFFSET      0
#define PCIE_VENDOR_SPECIFIC_HDR_VSEC_ID_MASK        0xffff

// Bitfield Description : Vendor-defined revision number.
#define PCIE_VENDOR_SPECIFIC_HDR_VSEC_REV_OFFSET      16
#define PCIE_VENDOR_SPECIFIC_HDR_VSEC_REV_MASK        0xf0000

// Bitfield Description : Number of bytes in the entire VSEC structure.
#define PCIE_VENDOR_SPECIFIC_HDR_VSEC_LENGTH_OFFSET      20
#define PCIE_VENDOR_SPECIFIC_HDR_VSEC_LENGTH_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            VSEC_ID:16;
    UINT32                            VSEC_REV:4;
    UINT32                            VSEC_LENGTH:12;
  } Field;
  UINT32 Value;
} PCIE_VENDOR_SPECIFIC_HDR_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11100104UL

#define SMN_FUNC0_PCIE1_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11200104UL

#define SMN_FUNC0_PCIE2_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11300104UL

#define SMN_FUNC0_PCIE3_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11400104UL

#define SMN_FUNC1_PCIE0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11101104UL

#define SMN_FUNC1_PCIE1_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11201104UL

#define SMN_FUNC2_PCIE0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11102104UL

#define SMN_FUNC2_PCIE1_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11202104UL

#define SMN_FUNC3_PCIE0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11103104UL

#define SMN_FUNC3_PCIE1_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11203104UL

#define SMN_FUNC4_PCIE0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11104104UL

#define SMN_FUNC4_PCIE1_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11204104UL

#define SMN_FUNC5_PCIE1_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x11205104UL


/***********************************************************
* Register Name : PMI_CAP
* Register Description :
* Power Management Capabilities register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates PCI-SIG defined capability structure version number.
#define PMI_CAP_VERSION_OFFSET      0
#define PMI_CAP_VERSION_MASK        0x7

// Bitfield Description : Does not apply to PCI Express.
#define PMI_CAP_PME_CLOCK_OFFSET      3
#define PMI_CAP_PME_CLOCK_MASK        0x8

// Bitfield Description : Indicates if the function is able to successfully complete valid configuration cycles at any time following return to D0.
#define PMI_CAP_IMMEDIATE_READINESS_ON_RETURN_TO_D0_OFFSET      4
#define PMI_CAP_IMMEDIATE_READINESS_ON_RETURN_TO_D0_MASK        0x10

// Bitfield Description : Indicates if the function requires a Device Specific Initialization sequence, upon transition to D0.
#define PMI_CAP_DEV_SPECIFIC_INIT_OFFSET      5
#define PMI_CAP_DEV_SPECIFIC_INIT_MASK        0x20

// Bitfield Description : Indicates the amount of 3.3Vaux current required to support PME from D3cold.
#define PMI_CAP_AUX_CURRENT_OFFSET      6
#define PMI_CAP_AUX_CURRENT_MASK        0x1c0

// Bitfield Description : Indicates support for the D1 power state.
#define PMI_CAP_D1_SUPPORT_OFFSET      9
#define PMI_CAP_D1_SUPPORT_MASK        0x200

// Bitfield Description : Indicates support for the D2 power state.
#define PMI_CAP_D2_SUPPORT_OFFSET      10
#define PMI_CAP_D2_SUPPORT_MASK        0x400

// Bitfield Description : Indicates the power states from which the function may generate a PME. This field is a bit vector that covers all Dx states. Bit 0 corresponds to D0.
#define PMI_CAP_PME_SUPPORT_OFFSET      11
#define PMI_CAP_PME_SUPPORT_MASK        0xf800

typedef union {
  struct {
    UINT16                            VERSION:3;
    UINT16                            PME_CLOCK:1;
    UINT16                            IMMEDIATE_READINESS_ON_RETURN_TO_D0:1;
    UINT16                            DEV_SPECIFIC_INIT:1;
    UINT16                            AUX_CURRENT:3;
    UINT16                            D1_SUPPORT:1;
    UINT16                            D2_SUPPORT:1;
    UINT16                            PME_SUPPORT:5;
  } Field;
  UINT16 Value;
} PMI_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PMI_CAP_ADDRESS    0x11100052UL

#define SMN_FUNC0_PCIE1_PMI_CAP_ADDRESS    0x11200052UL

#define SMN_FUNC0_PCIE2_PMI_CAP_ADDRESS    0x11300052UL

#define SMN_FUNC0_PCIE3_PMI_CAP_ADDRESS    0x11400052UL

#define SMN_FUNC1_PCIE0_PMI_CAP_ADDRESS    0x11101052UL

#define SMN_FUNC1_PCIE1_PMI_CAP_ADDRESS    0x11201052UL

#define SMN_FUNC2_PCIE0_PMI_CAP_ADDRESS    0x11102052UL

#define SMN_FUNC2_PCIE1_PMI_CAP_ADDRESS    0x11202052UL

#define SMN_FUNC3_PCIE0_PMI_CAP_ADDRESS    0x11103052UL

#define SMN_FUNC3_PCIE1_PMI_CAP_ADDRESS    0x11203052UL

#define SMN_FUNC4_PCIE0_PMI_CAP_ADDRESS    0x11104052UL

#define SMN_FUNC4_PCIE1_PMI_CAP_ADDRESS    0x11204052UL

#define SMN_FUNC5_PCIE1_PMI_CAP_ADDRESS    0x11205052UL


/***********************************************************
* Register Name : PMI_CAP_LIST
* Register Description :
* Power Management Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the capability.
#define PMI_CAP_LIST_CAP_ID_OFFSET      0
#define PMI_CAP_LIST_CAP_ID_MASK        0xff

// Bitfield Description : Pointer to the next item in the capabilities list, or 00h if no other items exist.
#define PMI_CAP_LIST_NEXT_PTR_OFFSET      8
#define PMI_CAP_LIST_NEXT_PTR_MASK        0xff00

typedef union {
  struct {
    UINT16                            CAP_ID:8;
    UINT16                            NEXT_PTR:8;
  } Field;
  UINT16 Value;
} PMI_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PMI_CAP_LIST_ADDRESS    0x11100050UL

#define SMN_FUNC0_PCIE1_PMI_CAP_LIST_ADDRESS    0x11200050UL

#define SMN_FUNC0_PCIE2_PMI_CAP_LIST_ADDRESS    0x11300050UL

#define SMN_FUNC0_PCIE3_PMI_CAP_LIST_ADDRESS    0x11400050UL

#define SMN_FUNC1_PCIE0_PMI_CAP_LIST_ADDRESS    0x11101050UL

#define SMN_FUNC1_PCIE1_PMI_CAP_LIST_ADDRESS    0x11201050UL

#define SMN_FUNC2_PCIE0_PMI_CAP_LIST_ADDRESS    0x11102050UL

#define SMN_FUNC2_PCIE1_PMI_CAP_LIST_ADDRESS    0x11202050UL

#define SMN_FUNC3_PCIE0_PMI_CAP_LIST_ADDRESS    0x11103050UL

#define SMN_FUNC3_PCIE1_PMI_CAP_LIST_ADDRESS    0x11203050UL

#define SMN_FUNC4_PCIE0_PMI_CAP_LIST_ADDRESS    0x11104050UL

#define SMN_FUNC4_PCIE1_PMI_CAP_LIST_ADDRESS    0x11204050UL

#define SMN_FUNC5_PCIE1_PMI_CAP_LIST_ADDRESS    0x11205050UL


/***********************************************************
* Register Name : PMI_STATUS_CNTL
* Register Description :
* Power Management Status/Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Writes set the function to the written power state. Reads return the function's current power state.
#define PMI_STATUS_CNTL_POWER_STATE_OFFSET      0
#define PMI_STATUS_CNTL_POWER_STATE_MASK        0x3

// Bitfield Description : 
#define PMI_STATUS_CNTL_Reserved_2_2_OFFSET      2
#define PMI_STATUS_CNTL_Reserved_2_2_MASK        0x4

// Bitfield Description : Indicates if the function does not reset on a transition from D3hot to D0.
#define PMI_STATUS_CNTL_NO_SOFT_RESET_OFFSET      3
#define PMI_STATUS_CNTL_NO_SOFT_RESET_MASK        0x8

// Bitfield Description : 
#define PMI_STATUS_CNTL_Reserved_7_4_OFFSET      4
#define PMI_STATUS_CNTL_Reserved_7_4_MASK        0xf0

// Bitfield Description : Controls a function's ability to generate a PME. When auxiliary power is consumed, this field is sticky.
#define PMI_STATUS_CNTL_PME_EN_OFFSET      8
#define PMI_STATUS_CNTL_PME_EN_MASK        0x100

// Bitfield Description : Data Select.
#define PMI_STATUS_CNTL_DATA_SELECT_OFFSET      9
#define PMI_STATUS_CNTL_DATA_SELECT_MASK        0x1e00

// Bitfield Description : Data Scale.
#define PMI_STATUS_CNTL_DATA_SCALE_OFFSET      13
#define PMI_STATUS_CNTL_DATA_SCALE_MASK        0x6000

// Bitfield Description : Indicates if a PME is pending internally in the function. When auxiliary power is consumed, this field is sticky.
#define PMI_STATUS_CNTL_PME_STATUS_OFFSET      15
#define PMI_STATUS_CNTL_PME_STATUS_MASK        0x8000

// Bitfield Description : 
#define PMI_STATUS_CNTL_Reserved_21_16_OFFSET      16
#define PMI_STATUS_CNTL_Reserved_21_16_MASK        0x3f0000

// Bitfield Description : Indicates if Secondary Bus Power Control is supported.
#define PMI_STATUS_CNTL_B2_B3_SUPPORT_OFFSET      22
#define PMI_STATUS_CNTL_B2_B3_SUPPORT_MASK        0x400000

// Bitfield Description : Indicates if Secondary Bus Clock Control is supported.
#define PMI_STATUS_CNTL_BUS_PWR_EN_OFFSET      23
#define PMI_STATUS_CNTL_BUS_PWR_EN_MASK        0x800000

// Bitfield Description : Data.
#define PMI_STATUS_CNTL_PMI_DATA_OFFSET      24
#define PMI_STATUS_CNTL_PMI_DATA_MASK        0xff000000

typedef union {
  struct {
    UINT32                            POWER_STATE:2;
    UINT32                            Reserved_2_2:1;
    UINT32                            NO_SOFT_RESET:1;
    UINT32                            Reserved_7_4:4;
    UINT32                            PME_EN:1;
    UINT32                            DATA_SELECT:4;
    UINT32                            DATA_SCALE:2;
    UINT32                            PME_STATUS:1;
    UINT32                            Reserved_21_16:6;
    UINT32                            B2_B3_SUPPORT:1;
    UINT32                            BUS_PWR_EN:1;
    UINT32                            PMI_DATA:8;
  } Field;
  UINT32 Value;
} PMI_STATUS_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PMI_STATUS_CNTL_ADDRESS    0x11100054UL

#define SMN_FUNC0_PCIE1_PMI_STATUS_CNTL_ADDRESS    0x11200054UL

#define SMN_FUNC0_PCIE2_PMI_STATUS_CNTL_ADDRESS    0x11300054UL

#define SMN_FUNC0_PCIE3_PMI_STATUS_CNTL_ADDRESS    0x11400054UL

#define SMN_FUNC1_PCIE0_PMI_STATUS_CNTL_ADDRESS    0x11101054UL

#define SMN_FUNC1_PCIE1_PMI_STATUS_CNTL_ADDRESS    0x11201054UL

#define SMN_FUNC2_PCIE0_PMI_STATUS_CNTL_ADDRESS    0x11102054UL

#define SMN_FUNC2_PCIE1_PMI_STATUS_CNTL_ADDRESS    0x11202054UL

#define SMN_FUNC3_PCIE0_PMI_STATUS_CNTL_ADDRESS    0x11103054UL

#define SMN_FUNC3_PCIE1_PMI_STATUS_CNTL_ADDRESS    0x11203054UL

#define SMN_FUNC4_PCIE0_PMI_STATUS_CNTL_ADDRESS    0x11104054UL

#define SMN_FUNC4_PCIE1_PMI_STATUS_CNTL_ADDRESS    0x11204054UL

#define SMN_FUNC5_PCIE1_PMI_STATUS_CNTL_ADDRESS    0x11205054UL


/***********************************************************
* Register Name : PREF_BASE_LIMIT
* Register Description :
* Defines a prefetchable memory address range which is used by the bridge to determine when to forward memory transactions from one interface to the other. For 64-bit addressing type, combines with Prefetchable Base Upper 32 Bits and Prefetchable Limit Upper 32 Bits registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Prefetchable Memory Base Addressing Type.
#define PREF_BASE_LIMIT_PREF_MEM_BASE_TYPE_OFFSET      0
#define PREF_BASE_LIMIT_PREF_MEM_BASE_TYPE_MASK        0xf

// Bitfield Description : Bits 31-20 for range starting prefetchable memory address.
#define PREF_BASE_LIMIT_PREF_MEM_BASE_31_20_OFFSET      4
#define PREF_BASE_LIMIT_PREF_MEM_BASE_31_20_MASK        0xfff0

// Bitfield Description : Prefetchable Memory Limit Addressing Type.
#define PREF_BASE_LIMIT_PREF_MEM_LIMIT_TYPE_OFFSET      16
#define PREF_BASE_LIMIT_PREF_MEM_LIMIT_TYPE_MASK        0xf0000

// Bitfield Description : Bits 31-20 for range ending prefetchable memory address.
#define PREF_BASE_LIMIT_PREF_MEM_LIMIT_31_20_OFFSET      20
#define PREF_BASE_LIMIT_PREF_MEM_LIMIT_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            PREF_MEM_BASE_TYPE:4;
    UINT32                            PREF_MEM_BASE_31_20:12;
    UINT32                            PREF_MEM_LIMIT_TYPE:4;
    UINT32                            PREF_MEM_LIMIT_31_20:12;
  } Field;
  UINT32 Value;
} PREF_BASE_LIMIT_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PREF_BASE_LIMIT_ADDRESS    0x11100024UL

#define SMN_FUNC0_PCIE1_PREF_BASE_LIMIT_ADDRESS    0x11200024UL

#define SMN_FUNC0_PCIE2_PREF_BASE_LIMIT_ADDRESS    0x11300024UL

#define SMN_FUNC0_PCIE3_PREF_BASE_LIMIT_ADDRESS    0x11400024UL

#define SMN_FUNC1_PCIE0_PREF_BASE_LIMIT_ADDRESS    0x11101024UL

#define SMN_FUNC1_PCIE1_PREF_BASE_LIMIT_ADDRESS    0x11201024UL

#define SMN_FUNC2_PCIE0_PREF_BASE_LIMIT_ADDRESS    0x11102024UL

#define SMN_FUNC2_PCIE1_PREF_BASE_LIMIT_ADDRESS    0x11202024UL

#define SMN_FUNC3_PCIE0_PREF_BASE_LIMIT_ADDRESS    0x11103024UL

#define SMN_FUNC3_PCIE1_PREF_BASE_LIMIT_ADDRESS    0x11203024UL

#define SMN_FUNC4_PCIE0_PREF_BASE_LIMIT_ADDRESS    0x11104024UL

#define SMN_FUNC4_PCIE1_PREF_BASE_LIMIT_ADDRESS    0x11204024UL

#define SMN_FUNC5_PCIE1_PREF_BASE_LIMIT_ADDRESS    0x11205024UL


/***********************************************************
* Register Name : PREF_BASE_UPPER
* Register Description :
* For 64-bit addressing type, combines with Prefetchable Memory Base register to define a prefetchable memory address range starting which is used by the bridge to determine when to forward memory transactions from one interface to the other.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32 bits for range starting 64-bit prefetchable memory address.
#define PREF_BASE_UPPER_PREF_BASE_UPPER_OFFSET      0
#define PREF_BASE_UPPER_PREF_BASE_UPPER_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            PREF_BASE_UPPER:32;
  } Field;
  UINT32 Value;
} PREF_BASE_UPPER_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PREF_BASE_UPPER_ADDRESS    0x11100028UL

#define SMN_FUNC0_PCIE1_PREF_BASE_UPPER_ADDRESS    0x11200028UL

#define SMN_FUNC0_PCIE2_PREF_BASE_UPPER_ADDRESS    0x11300028UL

#define SMN_FUNC0_PCIE3_PREF_BASE_UPPER_ADDRESS    0x11400028UL

#define SMN_FUNC1_PCIE0_PREF_BASE_UPPER_ADDRESS    0x11101028UL

#define SMN_FUNC1_PCIE1_PREF_BASE_UPPER_ADDRESS    0x11201028UL

#define SMN_FUNC2_PCIE0_PREF_BASE_UPPER_ADDRESS    0x11102028UL

#define SMN_FUNC2_PCIE1_PREF_BASE_UPPER_ADDRESS    0x11202028UL

#define SMN_FUNC3_PCIE0_PREF_BASE_UPPER_ADDRESS    0x11103028UL

#define SMN_FUNC3_PCIE1_PREF_BASE_UPPER_ADDRESS    0x11203028UL

#define SMN_FUNC4_PCIE0_PREF_BASE_UPPER_ADDRESS    0x11104028UL

#define SMN_FUNC4_PCIE1_PREF_BASE_UPPER_ADDRESS    0x11204028UL

#define SMN_FUNC5_PCIE1_PREF_BASE_UPPER_ADDRESS    0x11205028UL


/***********************************************************
* Register Name : PREF_LIMIT_UPPER
* Register Description :
* For 64-bit addressing type, combines with Prefetchable Memory Limit register to define a prefetchable memory address range ending which is used by the bridge to determine when to forward memory transactions from one interface to the other.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 32 bits for range ending 64-bit prefetchable memory address.
#define PREF_LIMIT_UPPER_PREF_LIMIT_UPPER_OFFSET      0
#define PREF_LIMIT_UPPER_PREF_LIMIT_UPPER_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            PREF_LIMIT_UPPER:32;
  } Field;
  UINT32 Value;
} PREF_LIMIT_UPPER_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PREF_LIMIT_UPPER_ADDRESS    0x1110002cUL

#define SMN_FUNC0_PCIE1_PREF_LIMIT_UPPER_ADDRESS    0x1120002cUL

#define SMN_FUNC0_PCIE2_PREF_LIMIT_UPPER_ADDRESS    0x1130002cUL

#define SMN_FUNC0_PCIE3_PREF_LIMIT_UPPER_ADDRESS    0x1140002cUL

#define SMN_FUNC1_PCIE0_PREF_LIMIT_UPPER_ADDRESS    0x1110102cUL

#define SMN_FUNC1_PCIE1_PREF_LIMIT_UPPER_ADDRESS    0x1120102cUL

#define SMN_FUNC2_PCIE0_PREF_LIMIT_UPPER_ADDRESS    0x1110202cUL

#define SMN_FUNC2_PCIE1_PREF_LIMIT_UPPER_ADDRESS    0x1120202cUL

#define SMN_FUNC3_PCIE0_PREF_LIMIT_UPPER_ADDRESS    0x1110302cUL

#define SMN_FUNC3_PCIE1_PREF_LIMIT_UPPER_ADDRESS    0x1120302cUL

#define SMN_FUNC4_PCIE0_PREF_LIMIT_UPPER_ADDRESS    0x1110402cUL

#define SMN_FUNC4_PCIE1_PREF_LIMIT_UPPER_ADDRESS    0x1120402cUL

#define SMN_FUNC5_PCIE1_PREF_LIMIT_UPPER_ADDRESS    0x1120502cUL


/***********************************************************
* Register Name : PROG_INTERFACE
* Register Description :
* The Programming Interface register is used with the Base Class Code and the Sub-Class Code registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Identifies the specific register-level programming interface of the function, if one is required.
#define PROG_INTERFACE_PROG_INTERFACE_OFFSET      0
#define PROG_INTERFACE_PROG_INTERFACE_MASK        0xff

typedef union {
  struct {
    UINT8                            PROG_INTERFACE:8;
  } Field;
  UINT8 Value;
} PROG_INTERFACE_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_PROG_INTERFACE_ADDRESS    0x11100009UL

#define SMN_FUNC0_PCIE1_PROG_INTERFACE_ADDRESS    0x11200009UL

#define SMN_FUNC0_PCIE2_PROG_INTERFACE_ADDRESS    0x11300009UL

#define SMN_FUNC0_PCIE3_PROG_INTERFACE_ADDRESS    0x11400009UL

#define SMN_FUNC1_PCIE0_PROG_INTERFACE_ADDRESS    0x11101009UL

#define SMN_FUNC1_PCIE1_PROG_INTERFACE_ADDRESS    0x11201009UL

#define SMN_FUNC2_PCIE0_PROG_INTERFACE_ADDRESS    0x11102009UL

#define SMN_FUNC2_PCIE1_PROG_INTERFACE_ADDRESS    0x11202009UL

#define SMN_FUNC3_PCIE0_PROG_INTERFACE_ADDRESS    0x11103009UL

#define SMN_FUNC3_PCIE1_PROG_INTERFACE_ADDRESS    0x11203009UL

#define SMN_FUNC4_PCIE0_PROG_INTERFACE_ADDRESS    0x11104009UL

#define SMN_FUNC4_PCIE1_PROG_INTERFACE_ADDRESS    0x11204009UL

#define SMN_FUNC5_PCIE1_PROG_INTERFACE_ADDRESS    0x11205009UL


/***********************************************************
* Register Name : READINESS_TIME_REPORTING_1
* Register Description :
* Readiness Time Reporting 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When Valid bit is set, indicates the time required for the function to be configuration ready, following a conventional reset. The lower nine bits indicate the value. The upper three bits indicate the scale used for the value, encoded as: 0 = 1 ns; 1 = 32 ns; 2 = 1024 ns; 3 = 32768 ns; 4 = 1048576 ns; 5 = 33554432 ns.
#define READINESS_TIME_REPORTING_1_RESET_TIME_OFFSET      0
#define READINESS_TIME_REPORTING_1_RESET_TIME_MASK        0xfff

// Bitfield Description : When Valid bit is set, indicates the time required for the function to be configuration ready, following the Downstream Port reporting Data Link Layer Active is set. Encodings are the same as for RESET_TIME.
#define READINESS_TIME_REPORTING_1_DL_UP_TIME_OFFSET      12
#define READINESS_TIME_REPORTING_1_DL_UP_TIME_MASK        0xfff000

// Bitfield Description : 
#define READINESS_TIME_REPORTING_1_Reserved_30_24_OFFSET      24
#define READINESS_TIME_REPORTING_1_Reserved_30_24_MASK        0x7f000000

// Bitfield Description : Indicates if the time values reported in this capability structure are valid.
#define READINESS_TIME_REPORTING_1_VALID_OFFSET      31
#define READINESS_TIME_REPORTING_1_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            RESET_TIME:12;
    UINT32                            DL_UP_TIME:12;
    UINT32                            Reserved_30_24:7;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} READINESS_TIME_REPORTING_1_STRUCT;

#define SMN_FUNC0_PCIE0_READINESS_TIME_REPORTING_1_ADDRESS    0x11100574UL

#define SMN_FUNC0_PCIE1_READINESS_TIME_REPORTING_1_ADDRESS    0x11200574UL

#define SMN_FUNC0_PCIE2_READINESS_TIME_REPORTING_1_ADDRESS    0x11300574UL

#define SMN_FUNC0_PCIE3_READINESS_TIME_REPORTING_1_ADDRESS    0x11400574UL

#define SMN_FUNC1_PCIE0_READINESS_TIME_REPORTING_1_ADDRESS    0x11101574UL

#define SMN_FUNC1_PCIE1_READINESS_TIME_REPORTING_1_ADDRESS    0x11201574UL

#define SMN_FUNC2_PCIE0_READINESS_TIME_REPORTING_1_ADDRESS    0x11102574UL

#define SMN_FUNC2_PCIE1_READINESS_TIME_REPORTING_1_ADDRESS    0x11202574UL

#define SMN_FUNC3_PCIE0_READINESS_TIME_REPORTING_1_ADDRESS    0x11103574UL

#define SMN_FUNC3_PCIE1_READINESS_TIME_REPORTING_1_ADDRESS    0x11203574UL

#define SMN_FUNC4_PCIE0_READINESS_TIME_REPORTING_1_ADDRESS    0x11104574UL

#define SMN_FUNC4_PCIE1_READINESS_TIME_REPORTING_1_ADDRESS    0x11204574UL

#define SMN_FUNC5_PCIE1_READINESS_TIME_REPORTING_1_ADDRESS    0x11205574UL


/***********************************************************
* Register Name : READINESS_TIME_REPORTING_2
* Register Description :
* Readiness Time Reporting 2 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When Valid bit is set and Function Level Reset Capability is set, indicates the time required for the function to be configuration ready, following a FLR. Encodings are the same as for RESET_TIME.
#define READINESS_TIME_REPORTING_2_FLR_TIME_OFFSET      0
#define READINESS_TIME_REPORTING_2_FLR_TIME_MASK        0xfff

// Bitfield Description : When Valid bit is set and Immediate Readiness on Return to D0 bit is clear, indicates the time required for the function to be configuration ready, following a D3hot to D0 transition. Encodings are the same as for RESET_TIME.
#define READINESS_TIME_REPORTING_2_D3HOT_TO_D0_TIME_OFFSET      12
#define READINESS_TIME_REPORTING_2_D3HOT_TO_D0_TIME_MASK        0xfff000

// Bitfield Description : 
#define READINESS_TIME_REPORTING_2_Reserved_31_24_OFFSET      24
#define READINESS_TIME_REPORTING_2_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            FLR_TIME:12;
    UINT32                            D3HOT_TO_D0_TIME:12;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} READINESS_TIME_REPORTING_2_STRUCT;

#define SMN_FUNC0_PCIE0_READINESS_TIME_REPORTING_2_ADDRESS    0x11100578UL

#define SMN_FUNC0_PCIE1_READINESS_TIME_REPORTING_2_ADDRESS    0x11200578UL

#define SMN_FUNC0_PCIE2_READINESS_TIME_REPORTING_2_ADDRESS    0x11300578UL

#define SMN_FUNC0_PCIE3_READINESS_TIME_REPORTING_2_ADDRESS    0x11400578UL

#define SMN_FUNC1_PCIE0_READINESS_TIME_REPORTING_2_ADDRESS    0x11101578UL

#define SMN_FUNC1_PCIE1_READINESS_TIME_REPORTING_2_ADDRESS    0x11201578UL

#define SMN_FUNC2_PCIE0_READINESS_TIME_REPORTING_2_ADDRESS    0x11102578UL

#define SMN_FUNC2_PCIE1_READINESS_TIME_REPORTING_2_ADDRESS    0x11202578UL

#define SMN_FUNC3_PCIE0_READINESS_TIME_REPORTING_2_ADDRESS    0x11103578UL

#define SMN_FUNC3_PCIE1_READINESS_TIME_REPORTING_2_ADDRESS    0x11203578UL

#define SMN_FUNC4_PCIE0_READINESS_TIME_REPORTING_2_ADDRESS    0x11104578UL

#define SMN_FUNC4_PCIE1_READINESS_TIME_REPORTING_2_ADDRESS    0x11204578UL

#define SMN_FUNC5_PCIE1_READINESS_TIME_REPORTING_2_ADDRESS    0x11205578UL


/***********************************************************
* Register Name : READINESS_TIME_REPORTING_ENH_CAP_LIST
* Register Description :
* Readiness Time Reporting Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define READINESS_TIME_REPORTING_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define READINESS_TIME_REPORTING_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define READINESS_TIME_REPORTING_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define READINESS_TIME_REPORTING_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define READINESS_TIME_REPORTING_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define READINESS_TIME_REPORTING_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} READINESS_TIME_REPORTING_ENH_CAP_LIST_STRUCT;

#define SMN_FUNC0_PCIE0_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11100570UL

#define SMN_FUNC0_PCIE1_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11200570UL

#define SMN_FUNC0_PCIE2_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11300570UL

#define SMN_FUNC0_PCIE3_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11400570UL

#define SMN_FUNC1_PCIE0_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11101570UL

#define SMN_FUNC1_PCIE1_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11201570UL

#define SMN_FUNC2_PCIE0_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11102570UL

#define SMN_FUNC2_PCIE1_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11202570UL

#define SMN_FUNC3_PCIE0_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11103570UL

#define SMN_FUNC3_PCIE1_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11203570UL

#define SMN_FUNC4_PCIE0_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11104570UL

#define SMN_FUNC4_PCIE1_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11204570UL

#define SMN_FUNC5_PCIE1_READINESS_TIME_REPORTING_ENH_CAP_LIST_ADDRESS    0x11205570UL


/***********************************************************
* Register Name : RECEIVED_MODIFIED_TS_DATA1
* Register Description :
* The Received Modified TS Data 1 register reports the values received in Modified TS1 or Modified TS2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When MODIFIED_TS_RECEIVED bit is set, contains the Modified TS Usage field from the received Modified TS1 or Modified TS2.
#define RECEIVED_MODIFIED_TS_DATA1_RECEIVED_MODIFIED_TS_USAGE_MODE_OFFSET      0
#define RECEIVED_MODIFIED_TS_DATA1_RECEIVED_MODIFIED_TS_USAGE_MODE_MASK        0x7

// Bitfield Description : When MODIFIED_TS_RECEIVED bit is set, contains the Modified TS Information 1 field from the received Modified TS1 or Modified TS2.
#define RECEIVED_MODIFIED_TS_DATA1_RECEIVED_MODIFIED_TS_INFORMATION_1_OFFSET      3
#define RECEIVED_MODIFIED_TS_DATA1_RECEIVED_MODIFIED_TS_INFORMATION_1_MASK        0xfff8

// Bitfield Description : When MODIFIED_TS_RECEIVED bit is set, contains the Training Set Message Vendor ID field or the Alternate Protocol Vendor ID field from the received Modified TS1 or Modified TS2.
#define RECEIVED_MODIFIED_TS_DATA1_RECEIVED_MODIFIED_TS_VENDOR_ID_OFFSET      16
#define RECEIVED_MODIFIED_TS_DATA1_RECEIVED_MODIFIED_TS_VENDOR_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            RECEIVED_MODIFIED_TS_USAGE_MODE:3;
    UINT32                            RECEIVED_MODIFIED_TS_INFORMATION_1:13;
    UINT32                            RECEIVED_MODIFIED_TS_VENDOR_ID:16;
  } Field;
  UINT32 Value;
} RECEIVED_MODIFIED_TS_DATA1_STRUCT;

#define SMN_FUNC0_PCIE0_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11100510UL

#define SMN_FUNC0_PCIE1_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11200510UL

#define SMN_FUNC0_PCIE2_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11300510UL

#define SMN_FUNC0_PCIE3_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11400510UL

#define SMN_FUNC1_PCIE0_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11101510UL

#define SMN_FUNC1_PCIE1_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11201510UL

#define SMN_FUNC2_PCIE0_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11102510UL

#define SMN_FUNC2_PCIE1_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11202510UL

#define SMN_FUNC3_PCIE0_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11103510UL

#define SMN_FUNC3_PCIE1_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11203510UL

#define SMN_FUNC4_PCIE0_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11104510UL

#define SMN_FUNC4_PCIE1_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11204510UL

#define SMN_FUNC5_PCIE1_RECEIVED_MODIFIED_TS_DATA1_ADDRESS    0x11205510UL


/***********************************************************
* Register Name : RECEIVED_MODIFIED_TS_DATA2
* Register Description :
* The Received Modified TS Data 2 register reports the values received in Modified TS1 or Modified TS2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When MODIFIED_TS_RECEIVED bit is set, contains the Modified TS Information 2 field from the received Modified TS1 or Modified TS2.
#define RECEIVED_MODIFIED_TS_DATA2_RECEIVED_MODIFIED_TS_INFORMATION_2_OFFSET      0
#define RECEIVED_MODIFIED_TS_DATA2_RECEIVED_MODIFIED_TS_INFORMATION_2_MASK        0xffffff

// Bitfield Description : Indicates the status of the Alternate Protocol Negotiation.
#define RECEIVED_MODIFIED_TS_DATA2_RECEIVED_ALTERNATE_PROTOCOL_NEGOTIATION_STATUS_OFFSET      24
#define RECEIVED_MODIFIED_TS_DATA2_RECEIVED_ALTERNATE_PROTOCOL_NEGOTIATION_STATUS_MASK        0x3000000

// Bitfield Description : 
#define RECEIVED_MODIFIED_TS_DATA2_Reserved_31_26_OFFSET      26
#define RECEIVED_MODIFIED_TS_DATA2_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            RECEIVED_MODIFIED_TS_INFORMATION_2:24;
    UINT32                            RECEIVED_ALTERNATE_PROTOCOL_NEGOTIATION_STATUS:2;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} RECEIVED_MODIFIED_TS_DATA2_STRUCT;

#define SMN_FUNC0_PCIE0_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11100514UL

#define SMN_FUNC0_PCIE1_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11200514UL

#define SMN_FUNC0_PCIE2_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11300514UL

#define SMN_FUNC0_PCIE3_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11400514UL

#define SMN_FUNC1_PCIE0_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11101514UL

#define SMN_FUNC1_PCIE1_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11201514UL

#define SMN_FUNC2_PCIE0_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11102514UL

#define SMN_FUNC2_PCIE1_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11202514UL

#define SMN_FUNC3_PCIE0_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11103514UL

#define SMN_FUNC3_PCIE1_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11203514UL

#define SMN_FUNC4_PCIE0_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11104514UL

#define SMN_FUNC4_PCIE1_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11204514UL

#define SMN_FUNC5_PCIE1_RECEIVED_MODIFIED_TS_DATA2_ADDRESS    0x11205514UL


/***********************************************************
* Register Name : REVISION_ID
* Register Description :
* Indicates a device specific revision identifier.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates minor revision identifier.
#define REVISION_ID_MINOR_REV_ID_OFFSET      0
#define REVISION_ID_MINOR_REV_ID_MASK        0xf

// Bitfield Description : Indicates major revision identifier.
#define REVISION_ID_MAJOR_REV_ID_OFFSET      4
#define REVISION_ID_MAJOR_REV_ID_MASK        0xf0

typedef union {
  struct {
    UINT8                            MINOR_REV_ID:4;
    UINT8                            MAJOR_REV_ID:4;
  } Field;
  UINT8 Value;
} REVISION_ID_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_REVISION_ID_ADDRESS    0x11100008UL

#define SMN_FUNC0_PCIE1_REVISION_ID_ADDRESS    0x11200008UL

#define SMN_FUNC0_PCIE2_REVISION_ID_ADDRESS    0x11300008UL

#define SMN_FUNC0_PCIE3_REVISION_ID_ADDRESS    0x11400008UL

#define SMN_FUNC1_PCIE0_REVISION_ID_ADDRESS    0x11101008UL

#define SMN_FUNC1_PCIE1_REVISION_ID_ADDRESS    0x11201008UL

#define SMN_FUNC2_PCIE0_REVISION_ID_ADDRESS    0x11102008UL

#define SMN_FUNC2_PCIE1_REVISION_ID_ADDRESS    0x11202008UL

#define SMN_FUNC3_PCIE0_REVISION_ID_ADDRESS    0x11103008UL

#define SMN_FUNC3_PCIE1_REVISION_ID_ADDRESS    0x11203008UL

#define SMN_FUNC4_PCIE0_REVISION_ID_ADDRESS    0x11104008UL

#define SMN_FUNC4_PCIE1_REVISION_ID_ADDRESS    0x11204008UL

#define SMN_FUNC5_PCIE1_REVISION_ID_ADDRESS    0x11205008UL


/***********************************************************
* Register Name : ROM_BASE_ADDR
* Register Description :
* PCI CFG Expansion ROM BAR.
* Visibility : 0x2
************************************************************/


/***********************************************************
* Register Name : ROOT_CAP
* Register Description :
* The Root Capabilities register identifies Root Port specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Root Port is capable of returning Configuration Request Retry Status Completion Status to software.
#define ROOT_CAP_CRS_SOFTWARE_VISIBILITY_OFFSET      0
#define ROOT_CAP_CRS_SOFTWARE_VISIBILITY_MASK        0x1

// Bitfield Description : 
#define ROOT_CAP_Reserved_15_1_OFFSET      1
#define ROOT_CAP_Reserved_15_1_MASK        0xfffe

typedef union {
  struct {
    UINT16                            CRS_SOFTWARE_VISIBILITY:1;
    UINT16                            Reserved_15_1:15;
  } Field;
  UINT16 Value;
} ROOT_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_ROOT_CAP_ADDRESS    0x11100076UL

#define SMN_FUNC0_PCIE1_ROOT_CAP_ADDRESS    0x11200076UL

#define SMN_FUNC0_PCIE2_ROOT_CAP_ADDRESS    0x11300076UL

#define SMN_FUNC0_PCIE3_ROOT_CAP_ADDRESS    0x11400076UL

#define SMN_FUNC1_PCIE0_ROOT_CAP_ADDRESS    0x11101076UL

#define SMN_FUNC1_PCIE1_ROOT_CAP_ADDRESS    0x11201076UL

#define SMN_FUNC2_PCIE0_ROOT_CAP_ADDRESS    0x11102076UL

#define SMN_FUNC2_PCIE1_ROOT_CAP_ADDRESS    0x11202076UL

#define SMN_FUNC3_PCIE0_ROOT_CAP_ADDRESS    0x11103076UL

#define SMN_FUNC3_PCIE1_ROOT_CAP_ADDRESS    0x11203076UL

#define SMN_FUNC4_PCIE0_ROOT_CAP_ADDRESS    0x11104076UL

#define SMN_FUNC4_PCIE1_ROOT_CAP_ADDRESS    0x11204076UL

#define SMN_FUNC5_PCIE1_ROOT_CAP_ADDRESS    0x11205076UL


/***********************************************************
* Register Name : ROOT_CNTL
* Register Description :
* The Root Control register controls Root Complex specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if a System Error should be generated if a correctable error is reported by any of the devices in the hierarchy associated with this Root Port.
#define ROOT_CNTL_SERR_ON_CORR_ERR_EN_OFFSET      0
#define ROOT_CNTL_SERR_ON_CORR_ERR_EN_MASK        0x1

// Bitfield Description : Controls if a System Error should be generated if a Non-fatal error is reported by any of the devices in the hierarchy associated with this Root Port.
#define ROOT_CNTL_SERR_ON_NONFATAL_ERR_EN_OFFSET      1
#define ROOT_CNTL_SERR_ON_NONFATAL_ERR_EN_MASK        0x2

// Bitfield Description : Controls if a System Error should be generated if a Fatal error is reported by any of the devices in the hierarchy associated with this Root Port.
#define ROOT_CNTL_SERR_ON_FATAL_ERR_EN_OFFSET      2
#define ROOT_CNTL_SERR_ON_FATAL_ERR_EN_MASK        0x4

// Bitfield Description : Controls interrupt generation upon receipt of a PME Message.
#define ROOT_CNTL_PM_INTERRUPT_EN_OFFSET      3
#define ROOT_CNTL_PM_INTERRUPT_EN_MASK        0x8

// Bitfield Description : Controls if the Root Port returns Configuration Request Retry Status Completion Status to software.
#define ROOT_CNTL_CRS_SOFTWARE_VISIBILITY_EN_OFFSET      4
#define ROOT_CNTL_CRS_SOFTWARE_VISIBILITY_EN_MASK        0x10

// Bitfield Description : 
#define ROOT_CNTL_Reserved_15_5_OFFSET      5
#define ROOT_CNTL_Reserved_15_5_MASK        0xffe0

typedef union {
  struct {
    UINT16                            SERR_ON_CORR_ERR_EN:1;
    UINT16                            SERR_ON_NONFATAL_ERR_EN:1;
    UINT16                            SERR_ON_FATAL_ERR_EN:1;
    UINT16                            PM_INTERRUPT_EN:1;
    UINT16                            CRS_SOFTWARE_VISIBILITY_EN:1;
    UINT16                            Reserved_15_5:11;
  } Field;
  UINT16 Value;
} ROOT_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_ROOT_CNTL_ADDRESS    0x11100074UL

#define SMN_FUNC0_PCIE1_ROOT_CNTL_ADDRESS    0x11200074UL

#define SMN_FUNC0_PCIE2_ROOT_CNTL_ADDRESS    0x11300074UL

#define SMN_FUNC0_PCIE3_ROOT_CNTL_ADDRESS    0x11400074UL

#define SMN_FUNC1_PCIE0_ROOT_CNTL_ADDRESS    0x11101074UL

#define SMN_FUNC1_PCIE1_ROOT_CNTL_ADDRESS    0x11201074UL

#define SMN_FUNC2_PCIE0_ROOT_CNTL_ADDRESS    0x11102074UL

#define SMN_FUNC2_PCIE1_ROOT_CNTL_ADDRESS    0x11202074UL

#define SMN_FUNC3_PCIE0_ROOT_CNTL_ADDRESS    0x11103074UL

#define SMN_FUNC3_PCIE1_ROOT_CNTL_ADDRESS    0x11203074UL

#define SMN_FUNC4_PCIE0_ROOT_CNTL_ADDRESS    0x11104074UL

#define SMN_FUNC4_PCIE1_ROOT_CNTL_ADDRESS    0x11204074UL

#define SMN_FUNC5_PCIE1_ROOT_CNTL_ADDRESS    0x11205074UL


/***********************************************************
* Register Name : ROOT_STATUS
* Register Description :
* The Root Status register provides information about device specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When PME Status is set, indicates the Requester ID of the PME Requester.
#define ROOT_STATUS_PME_REQUESTOR_ID_OFFSET      0
#define ROOT_STATUS_PME_REQUESTOR_ID_MASK        0xffff

// Bitfield Description : Indicates if PME was asserted by the PME Requester indicated in the PME Requestor ID field.
#define ROOT_STATUS_PME_STATUS_OFFSET      16
#define ROOT_STATUS_PME_STATUS_MASK        0x10000

// Bitfield Description : Indicates if another PME is pending when the PME Status bit is set. Hardware clears this bit when no more PMEs are pending.
#define ROOT_STATUS_PME_PENDING_OFFSET      17
#define ROOT_STATUS_PME_PENDING_MASK        0x20000

// Bitfield Description : 
#define ROOT_STATUS_Reserved_31_18_OFFSET      18
#define ROOT_STATUS_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            PME_REQUESTOR_ID:16;
    UINT32                            PME_STATUS:1;
    UINT32                            PME_PENDING:1;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} ROOT_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_ROOT_STATUS_ADDRESS    0x11100078UL

#define SMN_FUNC0_PCIE1_ROOT_STATUS_ADDRESS    0x11200078UL

#define SMN_FUNC0_PCIE2_ROOT_STATUS_ADDRESS    0x11300078UL

#define SMN_FUNC0_PCIE3_ROOT_STATUS_ADDRESS    0x11400078UL

#define SMN_FUNC1_PCIE0_ROOT_STATUS_ADDRESS    0x11101078UL

#define SMN_FUNC1_PCIE1_ROOT_STATUS_ADDRESS    0x11201078UL

#define SMN_FUNC2_PCIE0_ROOT_STATUS_ADDRESS    0x11102078UL

#define SMN_FUNC2_PCIE1_ROOT_STATUS_ADDRESS    0x11202078UL

#define SMN_FUNC3_PCIE0_ROOT_STATUS_ADDRESS    0x11103078UL

#define SMN_FUNC3_PCIE1_ROOT_STATUS_ADDRESS    0x11203078UL

#define SMN_FUNC4_PCIE0_ROOT_STATUS_ADDRESS    0x11104078UL

#define SMN_FUNC4_PCIE1_ROOT_STATUS_ADDRESS    0x11204078UL

#define SMN_FUNC5_PCIE1_ROOT_STATUS_ADDRESS    0x11205078UL


/***********************************************************
* Register Name : RTM1_PARITY_MISMATCH_STATUS_16GT
* Register Description :
* 16.0 GT/s First Retimer Data Parity Mismatch Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the corresponding lane detected a data parity mismatch from the first Retimer (if present) on the path. This field is a bit vector that covers all supported lanes. Bit 0 corresponds to the default lane 0, and bit ordering is invariant to link width and lane reversal negotiation. This field is sticky.
#define RTM1_PARITY_MISMATCH_STATUS_16GT_RTM1_PARITY_MISMATCH_STATUS_BITS_OFFSET      0
#define RTM1_PARITY_MISMATCH_STATUS_16GT_RTM1_PARITY_MISMATCH_STATUS_BITS_MASK        0xffff

// Bitfield Description : 
#define RTM1_PARITY_MISMATCH_STATUS_16GT_Reserved_31_16_OFFSET      16
#define RTM1_PARITY_MISMATCH_STATUS_16GT_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            RTM1_PARITY_MISMATCH_STATUS_BITS:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RTM1_PARITY_MISMATCH_STATUS_16GT_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11100424UL

#define SMN_FUNC0_PCIE1_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11200424UL

#define SMN_FUNC0_PCIE2_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11300424UL

#define SMN_FUNC0_PCIE3_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11400424UL

#define SMN_FUNC1_PCIE0_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11101424UL

#define SMN_FUNC1_PCIE1_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11201424UL

#define SMN_FUNC2_PCIE0_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11102424UL

#define SMN_FUNC2_PCIE1_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11202424UL

#define SMN_FUNC3_PCIE0_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11103424UL

#define SMN_FUNC3_PCIE1_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11203424UL

#define SMN_FUNC4_PCIE0_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11104424UL

#define SMN_FUNC4_PCIE1_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11204424UL

#define SMN_FUNC5_PCIE1_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11205424UL


/***********************************************************
* Register Name : RTM2_PARITY_MISMATCH_STATUS_16GT
* Register Description :
* 16.0 GT/s Second Retimer Data Parity Mismatch Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the corresponding lane detected a data parity mismatch from the second Retimer (if present) on the path. This field is a bit vector that covers all supported lanes. Bit 0 corresponds to the default lane 0, and bit ordering is invariant to link width and lane reversal negotiation. This field is sticky.
#define RTM2_PARITY_MISMATCH_STATUS_16GT_RTM2_PARITY_MISMATCH_STATUS_BITS_OFFSET      0
#define RTM2_PARITY_MISMATCH_STATUS_16GT_RTM2_PARITY_MISMATCH_STATUS_BITS_MASK        0xffff

// Bitfield Description : 
#define RTM2_PARITY_MISMATCH_STATUS_16GT_Reserved_31_16_OFFSET      16
#define RTM2_PARITY_MISMATCH_STATUS_16GT_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            RTM2_PARITY_MISMATCH_STATUS_BITS:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} RTM2_PARITY_MISMATCH_STATUS_16GT_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11100428UL

#define SMN_FUNC0_PCIE1_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11200428UL

#define SMN_FUNC0_PCIE2_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11300428UL

#define SMN_FUNC0_PCIE3_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11400428UL

#define SMN_FUNC1_PCIE0_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11101428UL

#define SMN_FUNC1_PCIE1_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11201428UL

#define SMN_FUNC2_PCIE0_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11102428UL

#define SMN_FUNC2_PCIE1_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11202428UL

#define SMN_FUNC3_PCIE0_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11103428UL

#define SMN_FUNC3_PCIE1_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11203428UL

#define SMN_FUNC4_PCIE0_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11104428UL

#define SMN_FUNC4_PCIE1_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11204428UL

#define SMN_FUNC5_PCIE1_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x11205428UL


/***********************************************************
* Register Name : SECONDARY_STATUS
* Register Description :
* Used to record status information for bus related events on the secondary interface.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define SECONDARY_STATUS_Reserved_4_0_OFFSET      0
#define SECONDARY_STATUS_Reserved_4_0_MASK        0x1f

// Bitfield Description : Does not apply to PCI Express.
#define SECONDARY_STATUS_PCI_66_CAP_OFFSET      5
#define SECONDARY_STATUS_PCI_66_CAP_MASK        0x20

// Bitfield Description : 
#define SECONDARY_STATUS_Reserved_6_6_OFFSET      6
#define SECONDARY_STATUS_Reserved_6_6_MASK        0x40

// Bitfield Description : Does not apply to PCI Express.
#define SECONDARY_STATUS_FAST_BACK_CAPABLE_OFFSET      7
#define SECONDARY_STATUS_FAST_BACK_CAPABLE_MASK        0x80

// Bitfield Description : For a Port with its Parity Error Response Enable bit in the Bridge Control register set, indicates if either of the following two conditions occurs on the secondary interface:
// 1) Port receives a poisoned Completion coming upstream
// 2) Port transmits a poisoned Request downstream.
#define SECONDARY_STATUS_MASTER_DATA_PARITY_ERROR_OFFSET      8
#define SECONDARY_STATUS_MASTER_DATA_PARITY_ERROR_MASK        0x100

// Bitfield Description : Does not apply to PCI Express.
#define SECONDARY_STATUS_DEVSEL_TIMING_OFFSET      9
#define SECONDARY_STATUS_DEVSEL_TIMING_MASK        0x600

// Bitfield Description : Indicates if a function completes a Request using Completer Abort Completion Status on the secondary interface.
#define SECONDARY_STATUS_SIGNAL_TARGET_ABORT_OFFSET      11
#define SECONDARY_STATUS_SIGNAL_TARGET_ABORT_MASK        0x800

// Bitfield Description : Indicates if a Requester receives a Completion with Completer Abort Completion Status on the secondary interface.
#define SECONDARY_STATUS_RECEIVED_TARGET_ABORT_OFFSET      12
#define SECONDARY_STATUS_RECEIVED_TARGET_ABORT_MASK        0x1000

// Bitfield Description : Indicates if a Requester receives a Completion with Unsupported Request Completion Status on the secondary interface.
#define SECONDARY_STATUS_RECEIVED_MASTER_ABORT_OFFSET      13
#define SECONDARY_STATUS_RECEIVED_MASTER_ABORT_MASK        0x2000

// Bitfield Description : Indicates if the function receives a Non-fatal or Fatal error Message on the secondary interface.
#define SECONDARY_STATUS_RECEIVED_SYSTEM_ERROR_OFFSET      14
#define SECONDARY_STATUS_RECEIVED_SYSTEM_ERROR_MASK        0x4000

// Bitfield Description : Indicates if a function receives a poisoned TLP on the secondary interface.
#define SECONDARY_STATUS_PARITY_ERROR_DETECTED_OFFSET      15
#define SECONDARY_STATUS_PARITY_ERROR_DETECTED_MASK        0x8000

typedef union {
  struct {
    UINT16                            Reserved_4_0:5;
    UINT16                            PCI_66_CAP:1;
    UINT16                            Reserved_6_6:1;
    UINT16                            FAST_BACK_CAPABLE:1;
    UINT16                            MASTER_DATA_PARITY_ERROR:1;
    UINT16                            DEVSEL_TIMING:2;
    UINT16                            SIGNAL_TARGET_ABORT:1;
    UINT16                            RECEIVED_TARGET_ABORT:1;
    UINT16                            RECEIVED_MASTER_ABORT:1;
    UINT16                            RECEIVED_SYSTEM_ERROR:1;
    UINT16                            PARITY_ERROR_DETECTED:1;
  } Field;
  UINT16 Value;
} SECONDARY_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SECONDARY_STATUS_ADDRESS    0x1110001eUL

#define SMN_FUNC0_PCIE1_SECONDARY_STATUS_ADDRESS    0x1120001eUL

#define SMN_FUNC0_PCIE2_SECONDARY_STATUS_ADDRESS    0x1130001eUL

#define SMN_FUNC0_PCIE3_SECONDARY_STATUS_ADDRESS    0x1140001eUL

#define SMN_FUNC1_PCIE0_SECONDARY_STATUS_ADDRESS    0x1110101eUL

#define SMN_FUNC1_PCIE1_SECONDARY_STATUS_ADDRESS    0x1120101eUL

#define SMN_FUNC2_PCIE0_SECONDARY_STATUS_ADDRESS    0x1110201eUL

#define SMN_FUNC2_PCIE1_SECONDARY_STATUS_ADDRESS    0x1120201eUL

#define SMN_FUNC3_PCIE0_SECONDARY_STATUS_ADDRESS    0x1110301eUL

#define SMN_FUNC3_PCIE1_SECONDARY_STATUS_ADDRESS    0x1120301eUL

#define SMN_FUNC4_PCIE0_SECONDARY_STATUS_ADDRESS    0x1110401eUL

#define SMN_FUNC4_PCIE1_SECONDARY_STATUS_ADDRESS    0x1120401eUL

#define SMN_FUNC5_PCIE1_SECONDARY_STATUS_ADDRESS    0x1120501eUL


/***********************************************************
* Register Name : SLOT_CAP
* Register Description :
* The Slot Capabilities register identifies slot specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if an Attention Button is implemented for this slot.
#define SLOT_CAP_ATTN_BUTTON_PRESENT_OFFSET      0
#define SLOT_CAP_ATTN_BUTTON_PRESENT_MASK        0x1

// Bitfield Description : Indicates if a Power Controller is implemented for this slot.
#define SLOT_CAP_PWR_CONTROLLER_PRESENT_OFFSET      1
#define SLOT_CAP_PWR_CONTROLLER_PRESENT_MASK        0x2

// Bitfield Description : Indicates if a Manually-operated Retention Latch Sensor is implemented for this slot.
#define SLOT_CAP_MRL_SENSOR_PRESENT_OFFSET      2
#define SLOT_CAP_MRL_SENSOR_PRESENT_MASK        0x4

// Bitfield Description : Indicates if an Attention Indicator is implemented for this slot.
#define SLOT_CAP_ATTN_INDICATOR_PRESENT_OFFSET      3
#define SLOT_CAP_ATTN_INDICATOR_PRESENT_MASK        0x8

// Bitfield Description : Indicates if a Power Indicator is implemented for this slot.
#define SLOT_CAP_PWR_INDICATOR_PRESENT_OFFSET      4
#define SLOT_CAP_PWR_INDICATOR_PRESENT_MASK        0x10

// Bitfield Description : Indicates if an adapter present in this slot might be removed from the system without any prior notification.
#define SLOT_CAP_HOTPLUG_SURPRISE_OFFSET      5
#define SLOT_CAP_HOTPLUG_SURPRISE_MASK        0x20

// Bitfield Description : Indicates if this slot is capable of supporting Hot-Plug operations.
#define SLOT_CAP_HOTPLUG_CAPABLE_OFFSET      6
#define SLOT_CAP_HOTPLUG_CAPABLE_MASK        0x40

// Bitfield Description : In combination with the Slot Power Limit Scale value, indicates the upper limit on power supplied by this slot.
#define SLOT_CAP_SLOT_PWR_LIMIT_VALUE_OFFSET      7
#define SLOT_CAP_SLOT_PWR_LIMIT_VALUE_MASK        0x7f80

// Bitfield Description : Indicates the scale used for the Slot Power Limit Value.
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_OFFSET      15
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_MASK        0x18000

// Bitfield Description : Indicates if an Electromechanical Interlock is implemented for this slot.
#define SLOT_CAP_ELECTROMECH_INTERLOCK_PRESENT_OFFSET      17
#define SLOT_CAP_ELECTROMECH_INTERLOCK_PRESENT_MASK        0x20000

// Bitfield Description : Indicates if this slot does not generate software notification when an issued command is completed by the Hot-Plug Controller.
#define SLOT_CAP_NO_COMMAND_COMPLETED_SUPPORTED_OFFSET      18
#define SLOT_CAP_NO_COMMAND_COMPLETED_SUPPORTED_MASK        0x40000

// Bitfield Description : Indicates the physical slot number attached to this Port.
#define SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET      19
#define SLOT_CAP_PHYSICAL_SLOT_NUM_MASK        0xfff80000

typedef union {
  struct {
    UINT32                            ATTN_BUTTON_PRESENT:1;
    UINT32                            PWR_CONTROLLER_PRESENT:1;
    UINT32                            MRL_SENSOR_PRESENT:1;
    UINT32                            ATTN_INDICATOR_PRESENT:1;
    UINT32                            PWR_INDICATOR_PRESENT:1;
    UINT32                            HOTPLUG_SURPRISE:1;
    UINT32                            HOTPLUG_CAPABLE:1;
    UINT32                            SLOT_PWR_LIMIT_VALUE:8;
    UINT32                            SLOT_PWR_LIMIT_SCALE:2;
    UINT32                            ELECTROMECH_INTERLOCK_PRESENT:1;
    UINT32                            NO_COMMAND_COMPLETED_SUPPORTED:1;
    UINT32                            PHYSICAL_SLOT_NUM:13;
  } Field;
  UINT32 Value;
} SLOT_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SLOT_CAP_ADDRESS    0x1110006cUL

#define SMN_FUNC0_PCIE1_SLOT_CAP_ADDRESS    0x1120006cUL

#define SMN_FUNC0_PCIE2_SLOT_CAP_ADDRESS    0x1130006cUL

#define SMN_FUNC0_PCIE3_SLOT_CAP_ADDRESS    0x1140006cUL

#define SMN_FUNC1_PCIE0_SLOT_CAP_ADDRESS    0x1110106cUL

#define SMN_FUNC1_PCIE1_SLOT_CAP_ADDRESS    0x1120106cUL

#define SMN_FUNC2_PCIE0_SLOT_CAP_ADDRESS    0x1110206cUL

#define SMN_FUNC2_PCIE1_SLOT_CAP_ADDRESS    0x1120206cUL

#define SMN_FUNC3_PCIE0_SLOT_CAP_ADDRESS    0x1110306cUL

#define SMN_FUNC3_PCIE1_SLOT_CAP_ADDRESS    0x1120306cUL

#define SMN_FUNC4_PCIE0_SLOT_CAP_ADDRESS    0x1110406cUL

#define SMN_FUNC4_PCIE1_SLOT_CAP_ADDRESS    0x1120406cUL

#define SMN_FUNC5_PCIE1_SLOT_CAP_ADDRESS    0x1120506cUL


/***********************************************************
* Register Name : SLOT_CAP2
* Register Description :
* The Slot Capabilities 2 register identifies slot specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the slot supports disabling the use of in-band presence to determine Presence Detect State and Component Presence.
#define SLOT_CAP2_INBAND_PD_DISABLE_SUPPORTED_OFFSET      0
#define SLOT_CAP2_INBAND_PD_DISABLE_SUPPORTED_MASK        0x1

// Bitfield Description : 
#define SLOT_CAP2_Reserved_31_1_OFFSET      1
#define SLOT_CAP2_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            INBAND_PD_DISABLE_SUPPORTED:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} SLOT_CAP2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SLOT_CAP2_ADDRESS    0x1110008cUL

#define SMN_FUNC0_PCIE1_SLOT_CAP2_ADDRESS    0x1120008cUL

#define SMN_FUNC0_PCIE2_SLOT_CAP2_ADDRESS    0x1130008cUL

#define SMN_FUNC0_PCIE3_SLOT_CAP2_ADDRESS    0x1140008cUL

#define SMN_FUNC1_PCIE0_SLOT_CAP2_ADDRESS    0x1110108cUL

#define SMN_FUNC1_PCIE1_SLOT_CAP2_ADDRESS    0x1120108cUL

#define SMN_FUNC2_PCIE0_SLOT_CAP2_ADDRESS    0x1110208cUL

#define SMN_FUNC2_PCIE1_SLOT_CAP2_ADDRESS    0x1120208cUL

#define SMN_FUNC3_PCIE0_SLOT_CAP2_ADDRESS    0x1110308cUL

#define SMN_FUNC3_PCIE1_SLOT_CAP2_ADDRESS    0x1120308cUL

#define SMN_FUNC4_PCIE0_SLOT_CAP2_ADDRESS    0x1110408cUL

#define SMN_FUNC4_PCIE1_SLOT_CAP2_ADDRESS    0x1120408cUL

#define SMN_FUNC5_PCIE1_SLOT_CAP2_ADDRESS    0x1120508cUL


/***********************************************************
* Register Name : SLOT_CNTL
* Register Description :
* The Slot Control register controls slot specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : If Attention Button is supported, controls if the function generates software notification on an attention button pressed event.
#define SLOT_CNTL_ATTN_BUTTON_PRESSED_EN_OFFSET      0
#define SLOT_CNTL_ATTN_BUTTON_PRESSED_EN_MASK        0x1

// Bitfield Description : If Power Fault Detection is supported, controls if the function generates software notification on a power fault event.
#define SLOT_CNTL_PWR_FAULT_DETECTED_EN_OFFSET      1
#define SLOT_CNTL_PWR_FAULT_DETECTED_EN_MASK        0x2

// Bitfield Description : If MRL Sensor is supported, controls if the function generates software notification on a MRL sensor changed event.
#define SLOT_CNTL_MRL_SENSOR_CHANGED_EN_OFFSET      2
#define SLOT_CNTL_MRL_SENSOR_CHANGED_EN_MASK        0x4

// Bitfield Description : If Hot-Plug Capability is supported, controls if the function generates software notification on a presence detect changed event.
#define SLOT_CNTL_PRESENCE_DETECT_CHANGED_EN_OFFSET      3
#define SLOT_CNTL_PRESENCE_DETECT_CHANGED_EN_MASK        0x8

// Bitfield Description : If Command Completed notification is supported, controls if the function generates software notification when a command is completed by the Hot-Plug Controller.
#define SLOT_CNTL_COMMAND_COMPLETED_INTR_EN_OFFSET      4
#define SLOT_CNTL_COMMAND_COMPLETED_INTR_EN_MASK        0x10

// Bitfield Description : If Hot-Plug Capability is supported, controls if the function generates an interrupt on enabled Hot-Plug events.
#define SLOT_CNTL_HOTPLUG_INTR_EN_OFFSET      5
#define SLOT_CNTL_HOTPLUG_INTR_EN_MASK        0x20

// Bitfield Description : If Attention Indicator is supported, writes set the Attention Indicator to the written state. Reads return the last written value.
#define SLOT_CNTL_ATTN_INDICATOR_CNTL_OFFSET      6
#define SLOT_CNTL_ATTN_INDICATOR_CNTL_MASK        0xc0

// Bitfield Description : If Power Indicator is supported, writes set the Power Indicator to the written state. Reads return the last written value.
#define SLOT_CNTL_PWR_INDICATOR_CNTL_OFFSET      8
#define SLOT_CNTL_PWR_INDICATOR_CNTL_MASK        0x300

// Bitfield Description : If Power Controller is supported, writes set the power state of the slot to the written state. Read returns the last written value.
#define SLOT_CNTL_PWR_CONTROLLER_CNTL_OFFSET      10
#define SLOT_CNTL_PWR_CONTROLLER_CNTL_MASK        0x400

// Bitfield Description : If Electromechanical Interlock is supported, writes of 1 causes the state of the Electromechanical Interlock to toggle. Writes of 0 have no effect. Reads always return 0.
#define SLOT_CNTL_ELECTROMECH_INTERLOCK_CNTL_OFFSET      11
#define SLOT_CNTL_ELECTROMECH_INTERLOCK_CNTL_MASK        0x800

// Bitfield Description : If Data Link Layer Link Active Capability is supported, controls if the function generates software notification when the Data Link Layer Link Active bit is changed.
#define SLOT_CNTL_DL_STATE_CHANGED_EN_OFFSET      12
#define SLOT_CNTL_DL_STATE_CHANGED_EN_MASK        0x1000

// Bitfield Description : (Downstream Ports only) Controls the Port's ability of automatically sending a Set Slot Power Limit Message when a Link transitions from non-DL_Up to DL_Up status.
#define SLOT_CNTL_AUTO_SLOT_PWR_LIMIT_DISABLE_OFFSET      13
#define SLOT_CNTL_AUTO_SLOT_PWR_LIMIT_DISABLE_MASK        0x2000

// Bitfield Description : If In-band Presence Detect Disable Capability is supported, controls the Port's ability to use in-band presence to determine Presence Detect State and Component Presence.
#define SLOT_CNTL_INBAND_PD_DISABLE_OFFSET      14
#define SLOT_CNTL_INBAND_PD_DISABLE_MASK        0x4000

// Bitfield Description : 
#define SLOT_CNTL_Reserved_15_15_OFFSET      15
#define SLOT_CNTL_Reserved_15_15_MASK        0x8000

typedef union {
  struct {
    UINT16                            ATTN_BUTTON_PRESSED_EN:1;
    UINT16                            PWR_FAULT_DETECTED_EN:1;
    UINT16                            MRL_SENSOR_CHANGED_EN:1;
    UINT16                            PRESENCE_DETECT_CHANGED_EN:1;
    UINT16                            COMMAND_COMPLETED_INTR_EN:1;
    UINT16                            HOTPLUG_INTR_EN:1;
    UINT16                            ATTN_INDICATOR_CNTL:2;
    UINT16                            PWR_INDICATOR_CNTL:2;
    UINT16                            PWR_CONTROLLER_CNTL:1;
    UINT16                            ELECTROMECH_INTERLOCK_CNTL:1;
    UINT16                            DL_STATE_CHANGED_EN:1;
    UINT16                            AUTO_SLOT_PWR_LIMIT_DISABLE:1;
    UINT16                            INBAND_PD_DISABLE:1;
    UINT16                            Reserved_15_15:1;
  } Field;
  UINT16 Value;
} SLOT_CNTL_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SLOT_CNTL_ADDRESS    0x11100070UL

#define SMN_FUNC0_PCIE1_SLOT_CNTL_ADDRESS    0x11200070UL

#define SMN_FUNC0_PCIE2_SLOT_CNTL_ADDRESS    0x11300070UL

#define SMN_FUNC0_PCIE3_SLOT_CNTL_ADDRESS    0x11400070UL

#define SMN_FUNC1_PCIE0_SLOT_CNTL_ADDRESS    0x11101070UL

#define SMN_FUNC1_PCIE1_SLOT_CNTL_ADDRESS    0x11201070UL

#define SMN_FUNC2_PCIE0_SLOT_CNTL_ADDRESS    0x11102070UL

#define SMN_FUNC2_PCIE1_SLOT_CNTL_ADDRESS    0x11202070UL

#define SMN_FUNC3_PCIE0_SLOT_CNTL_ADDRESS    0x11103070UL

#define SMN_FUNC3_PCIE1_SLOT_CNTL_ADDRESS    0x11203070UL

#define SMN_FUNC4_PCIE0_SLOT_CNTL_ADDRESS    0x11104070UL

#define SMN_FUNC4_PCIE1_SLOT_CNTL_ADDRESS    0x11204070UL

#define SMN_FUNC5_PCIE1_SLOT_CNTL_ADDRESS    0x11205070UL


/***********************************************************
* Register Name : SLOT_CNTL2
* Register Description :
* The Slot Control 2 register controls slot specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved.
#define SLOT_CNTL2_Reserved_15_0_OFFSET      0
#define SLOT_CNTL2_Reserved_15_0_MASK        0xffff

typedef union {
  struct {
    UINT16                            Reserved_15_0:16;
  } Field;
  UINT16 Value;
} SLOT_CNTL2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SLOT_CNTL2_ADDRESS    0x11100090UL

#define SMN_FUNC0_PCIE1_SLOT_CNTL2_ADDRESS    0x11200090UL

#define SMN_FUNC0_PCIE2_SLOT_CNTL2_ADDRESS    0x11300090UL

#define SMN_FUNC0_PCIE3_SLOT_CNTL2_ADDRESS    0x11400090UL

#define SMN_FUNC1_PCIE0_SLOT_CNTL2_ADDRESS    0x11101090UL

#define SMN_FUNC1_PCIE1_SLOT_CNTL2_ADDRESS    0x11201090UL

#define SMN_FUNC2_PCIE0_SLOT_CNTL2_ADDRESS    0x11102090UL

#define SMN_FUNC2_PCIE1_SLOT_CNTL2_ADDRESS    0x11202090UL

#define SMN_FUNC3_PCIE0_SLOT_CNTL2_ADDRESS    0x11103090UL

#define SMN_FUNC3_PCIE1_SLOT_CNTL2_ADDRESS    0x11203090UL

#define SMN_FUNC4_PCIE0_SLOT_CNTL2_ADDRESS    0x11104090UL

#define SMN_FUNC4_PCIE1_SLOT_CNTL2_ADDRESS    0x11204090UL

#define SMN_FUNC5_PCIE1_SLOT_CNTL2_ADDRESS    0x11205090UL


/***********************************************************
* Register Name : SLOT_STATUS
* Register Description :
* The Slot Status register provides information about slot specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : If Attention Button is supported, indicates if the attention button is pressed.
#define SLOT_STATUS_ATTN_BUTTON_PRESSED_OFFSET      0
#define SLOT_STATUS_ATTN_BUTTON_PRESSED_MASK        0x1

// Bitfield Description : If Power Fault Detection is supported, indicates if the power controller detected a power fault at this slot.
#define SLOT_STATUS_PWR_FAULT_DETECTED_OFFSET      1
#define SLOT_STATUS_PWR_FAULT_DETECTED_MASK        0x2

// Bitfield Description : If MRL Sensor is supported, indicates if a MRL sensor state change is detected.
#define SLOT_STATUS_MRL_SENSOR_CHANGED_OFFSET      2
#define SLOT_STATUS_MRL_SENSOR_CHANGED_MASK        0x4

// Bitfield Description : If Hot-Plug Capability is supported, indicates if the value reported in the Presence Detect State bit is changed.
#define SLOT_STATUS_PRESENCE_DETECT_CHANGED_OFFSET      3
#define SLOT_STATUS_PRESENCE_DETECT_CHANGED_MASK        0x8

// Bitfield Description : If Command Completed notification is supported, indicates if the Hot-Plug Controller completes an issued command.
#define SLOT_STATUS_COMMAND_COMPLETED_OFFSET      4
#define SLOT_STATUS_COMMAND_COMPLETED_MASK        0x10

// Bitfield Description : If MRL Sensor is supported, indicates the status of the MRL sensor.
#define SLOT_STATUS_MRL_SENSOR_STATE_OFFSET      5
#define SLOT_STATUS_MRL_SENSOR_STATE_MASK        0x20

// Bitfield Description : When the optionally supported SFI PD State Mask bit is clear, indicates the presence of an adapter in the slot, based on all enabled presence detect mechanisms.
#define SLOT_STATUS_PRESENCE_DETECT_STATE_OFFSET      6
#define SLOT_STATUS_PRESENCE_DETECT_STATE_MASK        0x40

// Bitfield Description : If Electromechanical Interlock is supported, indicates the status of the Electromechanical Interlock.
#define SLOT_STATUS_ELECTROMECH_INTERLOCK_STATUS_OFFSET      7
#define SLOT_STATUS_ELECTROMECH_INTERLOCK_STATUS_MASK        0x80

// Bitfield Description : If Data Link Layer Link Active Capability is supported, indicates if the value reported in the Data Link Layer Link Active bit of the Link Status register is changed.
#define SLOT_STATUS_DL_STATE_CHANGED_OFFSET      8
#define SLOT_STATUS_DL_STATE_CHANGED_MASK        0x100

// Bitfield Description : 
#define SLOT_STATUS_Reserved_15_9_OFFSET      9
#define SLOT_STATUS_Reserved_15_9_MASK        0xfe00

typedef union {
  struct {
    UINT16                            ATTN_BUTTON_PRESSED:1;
    UINT16                            PWR_FAULT_DETECTED:1;
    UINT16                            MRL_SENSOR_CHANGED:1;
    UINT16                            PRESENCE_DETECT_CHANGED:1;
    UINT16                            COMMAND_COMPLETED:1;
    UINT16                            MRL_SENSOR_STATE:1;
    UINT16                            PRESENCE_DETECT_STATE:1;
    UINT16                            ELECTROMECH_INTERLOCK_STATUS:1;
    UINT16                            DL_STATE_CHANGED:1;
    UINT16                            Reserved_15_9:7;
  } Field;
  UINT16 Value;
} SLOT_STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SLOT_STATUS_ADDRESS    0x11100072UL

#define SMN_FUNC0_PCIE1_SLOT_STATUS_ADDRESS    0x11200072UL

#define SMN_FUNC0_PCIE2_SLOT_STATUS_ADDRESS    0x11300072UL

#define SMN_FUNC0_PCIE3_SLOT_STATUS_ADDRESS    0x11400072UL

#define SMN_FUNC1_PCIE0_SLOT_STATUS_ADDRESS    0x11101072UL

#define SMN_FUNC1_PCIE1_SLOT_STATUS_ADDRESS    0x11201072UL

#define SMN_FUNC2_PCIE0_SLOT_STATUS_ADDRESS    0x11102072UL

#define SMN_FUNC2_PCIE1_SLOT_STATUS_ADDRESS    0x11202072UL

#define SMN_FUNC3_PCIE0_SLOT_STATUS_ADDRESS    0x11103072UL

#define SMN_FUNC3_PCIE1_SLOT_STATUS_ADDRESS    0x11203072UL

#define SMN_FUNC4_PCIE0_SLOT_STATUS_ADDRESS    0x11104072UL

#define SMN_FUNC4_PCIE1_SLOT_STATUS_ADDRESS    0x11204072UL

#define SMN_FUNC5_PCIE1_SLOT_STATUS_ADDRESS    0x11205072UL


/***********************************************************
* Register Name : SLOT_STATUS2
* Register Description :
* The Slot Status 2 register provides information about slot specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved.
#define SLOT_STATUS2_Reserved_15_0_OFFSET      0
#define SLOT_STATUS2_Reserved_15_0_MASK        0xffff

typedef union {
  struct {
    UINT16                            Reserved_15_0:16;
  } Field;
  UINT16 Value;
} SLOT_STATUS2_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SLOT_STATUS2_ADDRESS    0x11100092UL

#define SMN_FUNC0_PCIE1_SLOT_STATUS2_ADDRESS    0x11200092UL

#define SMN_FUNC0_PCIE2_SLOT_STATUS2_ADDRESS    0x11300092UL

#define SMN_FUNC0_PCIE3_SLOT_STATUS2_ADDRESS    0x11400092UL

#define SMN_FUNC1_PCIE0_SLOT_STATUS2_ADDRESS    0x11101092UL

#define SMN_FUNC1_PCIE1_SLOT_STATUS2_ADDRESS    0x11201092UL

#define SMN_FUNC2_PCIE0_SLOT_STATUS2_ADDRESS    0x11102092UL

#define SMN_FUNC2_PCIE1_SLOT_STATUS2_ADDRESS    0x11202092UL

#define SMN_FUNC3_PCIE0_SLOT_STATUS2_ADDRESS    0x11103092UL

#define SMN_FUNC3_PCIE1_SLOT_STATUS2_ADDRESS    0x11203092UL

#define SMN_FUNC4_PCIE0_SLOT_STATUS2_ADDRESS    0x11104092UL

#define SMN_FUNC4_PCIE1_SLOT_STATUS2_ADDRESS    0x11204092UL

#define SMN_FUNC5_PCIE1_SLOT_STATUS2_ADDRESS    0x11205092UL


/***********************************************************
* Register Name : SSID_CAP
* Register Description :
* Subsystem ID Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Subsystem Vendor ID. Specified by the vendor.
#define SSID_CAP_SUBSYSTEM_VENDOR_ID_OFFSET      0
#define SSID_CAP_SUBSYSTEM_VENDOR_ID_MASK        0xffff

// Bitfield Description : Subsystem ID. Specified by the vendor.
#define SSID_CAP_SUBSYSTEM_ID_OFFSET      16
#define SSID_CAP_SUBSYSTEM_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SUBSYSTEM_VENDOR_ID:16;
    UINT32                            SUBSYSTEM_ID:16;
  } Field;
  UINT32 Value;
} SSID_CAP_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SSID_CAP_ADDRESS    0x111000c4UL

#define SMN_FUNC0_PCIE1_SSID_CAP_ADDRESS    0x112000c4UL

#define SMN_FUNC0_PCIE2_SSID_CAP_ADDRESS    0x113000c4UL

#define SMN_FUNC0_PCIE3_SSID_CAP_ADDRESS    0x114000c4UL

#define SMN_FUNC1_PCIE0_SSID_CAP_ADDRESS    0x111010c4UL

#define SMN_FUNC1_PCIE1_SSID_CAP_ADDRESS    0x112010c4UL

#define SMN_FUNC2_PCIE0_SSID_CAP_ADDRESS    0x111020c4UL

#define SMN_FUNC2_PCIE1_SSID_CAP_ADDRESS    0x112020c4UL

#define SMN_FUNC3_PCIE0_SSID_CAP_ADDRESS    0x111030c4UL

#define SMN_FUNC3_PCIE1_SSID_CAP_ADDRESS    0x112030c4UL

#define SMN_FUNC4_PCIE0_SSID_CAP_ADDRESS    0x111040c4UL

#define SMN_FUNC4_PCIE1_SSID_CAP_ADDRESS    0x112040c4UL

#define SMN_FUNC5_PCIE1_SSID_CAP_ADDRESS    0x112050c4UL


/***********************************************************
* Register Name : SSID_CAP_LIST
* Register Description :
* Subsystem ID Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the capability.
#define SSID_CAP_LIST_CAP_ID_OFFSET      0
#define SSID_CAP_LIST_CAP_ID_MASK        0xff

// Bitfield Description : Pointer to the next item in the capabilities list, or 00h if no other items exist.
#define SSID_CAP_LIST_NEXT_PTR_OFFSET      8
#define SSID_CAP_LIST_NEXT_PTR_MASK        0xff00

typedef union {
  struct {
    UINT16                            CAP_ID:8;
    UINT16                            NEXT_PTR:8;
  } Field;
  UINT16 Value;
} SSID_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SSID_CAP_LIST_ADDRESS    0x111000c0UL

#define SMN_FUNC0_PCIE1_SSID_CAP_LIST_ADDRESS    0x112000c0UL

#define SMN_FUNC0_PCIE2_SSID_CAP_LIST_ADDRESS    0x113000c0UL

#define SMN_FUNC0_PCIE3_SSID_CAP_LIST_ADDRESS    0x114000c0UL

#define SMN_FUNC1_PCIE0_SSID_CAP_LIST_ADDRESS    0x111010c0UL

#define SMN_FUNC1_PCIE1_SSID_CAP_LIST_ADDRESS    0x112010c0UL

#define SMN_FUNC2_PCIE0_SSID_CAP_LIST_ADDRESS    0x111020c0UL

#define SMN_FUNC2_PCIE1_SSID_CAP_LIST_ADDRESS    0x112020c0UL

#define SMN_FUNC3_PCIE0_SSID_CAP_LIST_ADDRESS    0x111030c0UL

#define SMN_FUNC3_PCIE1_SSID_CAP_LIST_ADDRESS    0x112030c0UL

#define SMN_FUNC4_PCIE0_SSID_CAP_LIST_ADDRESS    0x111040c0UL

#define SMN_FUNC4_PCIE1_SSID_CAP_LIST_ADDRESS    0x112040c0UL

#define SMN_FUNC5_PCIE1_SSID_CAP_LIST_ADDRESS    0x112050c0UL


/***********************************************************
* Register Name : STATUS
* Register Description :
* Used to record status information for bus related events on the primary interface.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the function is able to successfully complete valid configuration cycles at any time following any type of reset.
#define STATUS_IMMEDIATE_READINESS_OFFSET      0
#define STATUS_IMMEDIATE_READINESS_MASK        0x1

// Bitfield Description : 
#define STATUS_Reserved_2_1_OFFSET      1
#define STATUS_Reserved_2_1_MASK        0x6

// Bitfield Description : Indicates if an INTx interrupt Message is pending internally in the function.
#define STATUS_INT_STATUS_OFFSET      3
#define STATUS_INT_STATUS_MASK        0x8

// Bitfield Description : Indicates the presence of a capability list item.
#define STATUS_CAP_LIST_OFFSET      4
#define STATUS_CAP_LIST_MASK        0x10

// Bitfield Description : Does not apply to PCI Express.
#define STATUS_PCI_66_CAP_OFFSET      5
#define STATUS_PCI_66_CAP_MASK        0x20

// Bitfield Description : 
#define STATUS_Reserved_6_6_OFFSET      6
#define STATUS_Reserved_6_6_MASK        0x40

// Bitfield Description : Does not apply to PCI Express.
#define STATUS_FAST_BACK_CAPABLE_OFFSET      7
#define STATUS_FAST_BACK_CAPABLE_MASK        0x80

// Bitfield Description : For a Requester with its Parity Error Response bit in the Command register set, indicates if either of the following two conditions occurs on the primary interface:
// 1) Requester receives a poisoned Completion
// 2) Requester transmits a poisoned Request.
// For a Port with its Parity Error Response bit in the Command register set, indicates if either of the following two conditions occurs on the primary interface:
// 1) Port receives a poisoned Completion going downstream
// 2) Port transmits a poisoned Request upstream.
#define STATUS_MASTER_DATA_PARITY_ERROR_OFFSET      8
#define STATUS_MASTER_DATA_PARITY_ERROR_MASK        0x100

// Bitfield Description : Does not apply to PCI Express.
#define STATUS_DEVSEL_TIMING_OFFSET      9
#define STATUS_DEVSEL_TIMING_MASK        0x600

// Bitfield Description : Indicates if a function completes a Request using Completer Abort Completion Status on the primary interface.
#define STATUS_SIGNAL_TARGET_ABORT_OFFSET      11
#define STATUS_SIGNAL_TARGET_ABORT_MASK        0x800

// Bitfield Description : Indicates if a Requester receives a Completion with Completer Abort Completion Status on the primary interface.
#define STATUS_RECEIVED_TARGET_ABORT_OFFSET      12
#define STATUS_RECEIVED_TARGET_ABORT_MASK        0x1000

// Bitfield Description : Indicates if a Requester receives a Completion with Unsupported Request Completion Status on the primary interface.
#define STATUS_RECEIVED_MASTER_ABORT_OFFSET      13
#define STATUS_RECEIVED_MASTER_ABORT_MASK        0x2000

// Bitfield Description : For a function with its SERR Enable bit in the Command register set, indicates if the function signals a Non-fatal or Fatal error on the primary interface.
#define STATUS_SIGNALED_SYSTEM_ERROR_OFFSET      14
#define STATUS_SIGNALED_SYSTEM_ERROR_MASK        0x4000

// Bitfield Description : Indicates if a function receives a poisoned TLP on the primary interface.
#define STATUS_PARITY_ERROR_DETECTED_OFFSET      15
#define STATUS_PARITY_ERROR_DETECTED_MASK        0x8000

typedef union {
  struct {
    UINT16                            IMMEDIATE_READINESS:1;
    UINT16                            Reserved_2_1:2;
    UINT16                            INT_STATUS:1;
    UINT16                            CAP_LIST:1;
    UINT16                            PCI_66_CAP:1;
    UINT16                            Reserved_6_6:1;
    UINT16                            FAST_BACK_CAPABLE:1;
    UINT16                            MASTER_DATA_PARITY_ERROR:1;
    UINT16                            DEVSEL_TIMING:2;
    UINT16                            SIGNAL_TARGET_ABORT:1;
    UINT16                            RECEIVED_TARGET_ABORT:1;
    UINT16                            RECEIVED_MASTER_ABORT:1;
    UINT16                            SIGNALED_SYSTEM_ERROR:1;
    UINT16                            PARITY_ERROR_DETECTED:1;
  } Field;
  UINT16 Value;
} STATUS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_STATUS_ADDRESS    0x11100006UL

#define SMN_FUNC0_PCIE1_STATUS_ADDRESS    0x11200006UL

#define SMN_FUNC0_PCIE2_STATUS_ADDRESS    0x11300006UL

#define SMN_FUNC0_PCIE3_STATUS_ADDRESS    0x11400006UL

#define SMN_FUNC1_PCIE0_STATUS_ADDRESS    0x11101006UL

#define SMN_FUNC1_PCIE1_STATUS_ADDRESS    0x11201006UL

#define SMN_FUNC2_PCIE0_STATUS_ADDRESS    0x11102006UL

#define SMN_FUNC2_PCIE1_STATUS_ADDRESS    0x11202006UL

#define SMN_FUNC3_PCIE0_STATUS_ADDRESS    0x11103006UL

#define SMN_FUNC3_PCIE1_STATUS_ADDRESS    0x11203006UL

#define SMN_FUNC4_PCIE0_STATUS_ADDRESS    0x11104006UL

#define SMN_FUNC4_PCIE1_STATUS_ADDRESS    0x11204006UL

#define SMN_FUNC5_PCIE1_STATUS_ADDRESS    0x11205006UL


/***********************************************************
* Register Name : SUB_BUS_NUMBER_LATENCY
* Register Description :
* Primary Bus Number, Secondary Bus Number, Subordinate Bus Number, and Secondary Latency Timer registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Records the bus number of the bus segment to which the primary interface of the bridge is connected.
#define SUB_BUS_NUMBER_LATENCY_PRIMARY_BUS_OFFSET      0
#define SUB_BUS_NUMBER_LATENCY_PRIMARY_BUS_MASK        0xff

// Bitfield Description : Records the bus number of the bus segment to which the secondary interface of the bridge is connected.
#define SUB_BUS_NUMBER_LATENCY_SECONDARY_BUS_OFFSET      8
#define SUB_BUS_NUMBER_LATENCY_SECONDARY_BUS_MASK        0xff00

// Bitfield Description : Records the bus number of the highest numbered bus segment which is behind the bridge.
#define SUB_BUS_NUMBER_LATENCY_SUB_BUS_NUM_OFFSET      16
#define SUB_BUS_NUMBER_LATENCY_SUB_BUS_NUM_MASK        0xff0000

// Bitfield Description : Does not apply to PCI Express.
#define SUB_BUS_NUMBER_LATENCY_SECONDARY_LATENCY_TIMER_OFFSET      24
#define SUB_BUS_NUMBER_LATENCY_SECONDARY_LATENCY_TIMER_MASK        0xff000000

typedef union {
  struct {
    UINT32                            PRIMARY_BUS:8;
    UINT32                            SECONDARY_BUS:8;
    UINT32                            SUB_BUS_NUM:8;
    UINT32                            SECONDARY_LATENCY_TIMER:8;
  } Field;
  UINT32 Value;
} SUB_BUS_NUMBER_LATENCY_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11100018UL

#define SMN_FUNC0_PCIE1_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11200018UL

#define SMN_FUNC0_PCIE2_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11300018UL

#define SMN_FUNC0_PCIE3_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11400018UL

#define SMN_FUNC1_PCIE0_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11101018UL

#define SMN_FUNC1_PCIE1_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11201018UL

#define SMN_FUNC2_PCIE0_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11102018UL

#define SMN_FUNC2_PCIE1_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11202018UL

#define SMN_FUNC3_PCIE0_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11103018UL

#define SMN_FUNC3_PCIE1_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11203018UL

#define SMN_FUNC4_PCIE0_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11104018UL

#define SMN_FUNC4_PCIE1_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11204018UL

#define SMN_FUNC5_PCIE1_SUB_BUS_NUMBER_LATENCY_ADDRESS    0x11205018UL


/***********************************************************
* Register Name : SUB_CLASS
* Register Description :
* The Sub-Class Code register is used with the Base Class Code and the Programming Interface registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Identifies more specifically the operation of the function.
#define SUB_CLASS_SUB_CLASS_OFFSET      0
#define SUB_CLASS_SUB_CLASS_MASK        0xff

typedef union {
  struct {
    UINT8                            SUB_CLASS:8;
  } Field;
  UINT8 Value;
} SUB_CLASS_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_SUB_CLASS_ADDRESS    0x1110000aUL

#define SMN_FUNC0_PCIE1_SUB_CLASS_ADDRESS    0x1120000aUL

#define SMN_FUNC0_PCIE2_SUB_CLASS_ADDRESS    0x1130000aUL

#define SMN_FUNC0_PCIE3_SUB_CLASS_ADDRESS    0x1140000aUL

#define SMN_FUNC1_PCIE0_SUB_CLASS_ADDRESS    0x1110100aUL

#define SMN_FUNC1_PCIE1_SUB_CLASS_ADDRESS    0x1120100aUL

#define SMN_FUNC2_PCIE0_SUB_CLASS_ADDRESS    0x1110200aUL

#define SMN_FUNC2_PCIE1_SUB_CLASS_ADDRESS    0x1120200aUL

#define SMN_FUNC3_PCIE0_SUB_CLASS_ADDRESS    0x1110300aUL

#define SMN_FUNC3_PCIE1_SUB_CLASS_ADDRESS    0x1120300aUL

#define SMN_FUNC4_PCIE0_SUB_CLASS_ADDRESS    0x1110400aUL

#define SMN_FUNC4_PCIE1_SUB_CLASS_ADDRESS    0x1120400aUL

#define SMN_FUNC5_PCIE1_SUB_CLASS_ADDRESS    0x1120500aUL


/***********************************************************
* Register Name : TRANSMITTED_MODIFIED_TS_DATA1
* Register Description :
* The Transmitted Modified TS Data 1 register reports the values transmitted in Modified TS1 or Modified TS2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When MODIFIED_TS_RECEIVED bit is set, contains the Modified TS Usage field from the Modified TS2 transmitted in Configuration.Complete.
#define TRANSMITTED_MODIFIED_TS_DATA1_TRANSMITTED_MODIFIED_TS_USAGE_MODE_OFFSET      0
#define TRANSMITTED_MODIFIED_TS_DATA1_TRANSMITTED_MODIFIED_TS_USAGE_MODE_MASK        0x7

// Bitfield Description : When MODIFIED_TS_RECEIVED bit is set, contains the Modified TS Information 1 field from the Modified TS2 transmitted in Configuration.Complete.
#define TRANSMITTED_MODIFIED_TS_DATA1_TRANSMITTED_MODIFIED_TS_INFORMATION_1_OFFSET      3
#define TRANSMITTED_MODIFIED_TS_DATA1_TRANSMITTED_MODIFIED_TS_INFORMATION_1_MASK        0xfff8

// Bitfield Description : When MODIFIED_TS_RECEIVED bit is set, contains the Training Set Message Vendor ID field or the Alternate Protocol Vendor ID field from the Modified TS2 transmitted in Configuration.Complete.
#define TRANSMITTED_MODIFIED_TS_DATA1_TRANSMITTED_MODIFIED_TS_VENDOR_ID_OFFSET      16
#define TRANSMITTED_MODIFIED_TS_DATA1_TRANSMITTED_MODIFIED_TS_VENDOR_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            TRANSMITTED_MODIFIED_TS_USAGE_MODE:3;
    UINT32                            TRANSMITTED_MODIFIED_TS_INFORMATION_1:13;
    UINT32                            TRANSMITTED_MODIFIED_TS_VENDOR_ID:16;
  } Field;
  UINT32 Value;
} TRANSMITTED_MODIFIED_TS_DATA1_STRUCT;

#define SMN_FUNC0_PCIE0_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11100518UL

#define SMN_FUNC0_PCIE1_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11200518UL

#define SMN_FUNC0_PCIE2_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11300518UL

#define SMN_FUNC0_PCIE3_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11400518UL

#define SMN_FUNC1_PCIE0_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11101518UL

#define SMN_FUNC1_PCIE1_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11201518UL

#define SMN_FUNC2_PCIE0_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11102518UL

#define SMN_FUNC2_PCIE1_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11202518UL

#define SMN_FUNC3_PCIE0_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11103518UL

#define SMN_FUNC3_PCIE1_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11203518UL

#define SMN_FUNC4_PCIE0_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11104518UL

#define SMN_FUNC4_PCIE1_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11204518UL

#define SMN_FUNC5_PCIE1_TRANSMITTED_MODIFIED_TS_DATA1_ADDRESS    0x11205518UL


/***********************************************************
* Register Name : TRANSMITTED_MODIFIED_TS_DATA2
* Register Description :
* The Transmitted Modified TS Data 2 register reports the values transmitted in Modified TS1 or Modified TS2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When MODIFIED_TS_RECEIVED bit is set, contains the Modified TS Information 2 field from the Modified TS2 transmitted in Configuration.Complete.
#define TRANSMITTED_MODIFIED_TS_DATA2_TRANSMITTED_MODIFIED_TS_INFORMATION_2_OFFSET      0
#define TRANSMITTED_MODIFIED_TS_DATA2_TRANSMITTED_MODIFIED_TS_INFORMATION_2_MASK        0xffffff

// Bitfield Description : Indicates the status of the Alternate Protocol Negotiation.
#define TRANSMITTED_MODIFIED_TS_DATA2_TRANSMITTED_ALTERNATE_PROTOCOL_NEGOTIATION_STATUS_OFFSET      24
#define TRANSMITTED_MODIFIED_TS_DATA2_TRANSMITTED_ALTERNATE_PROTOCOL_NEGOTIATION_STATUS_MASK        0x3000000

// Bitfield Description : 
#define TRANSMITTED_MODIFIED_TS_DATA2_Reserved_31_26_OFFSET      26
#define TRANSMITTED_MODIFIED_TS_DATA2_Reserved_31_26_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            TRANSMITTED_MODIFIED_TS_INFORMATION_2:24;
    UINT32                            TRANSMITTED_ALTERNATE_PROTOCOL_NEGOTIATION_STATUS:2;
    UINT32                            Reserved_31_26:6;
  } Field;
  UINT32 Value;
} TRANSMITTED_MODIFIED_TS_DATA2_STRUCT;

#define SMN_FUNC0_PCIE0_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1110051cUL

#define SMN_FUNC0_PCIE1_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1120051cUL

#define SMN_FUNC0_PCIE2_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1130051cUL

#define SMN_FUNC0_PCIE3_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1140051cUL

#define SMN_FUNC1_PCIE0_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1110151cUL

#define SMN_FUNC1_PCIE1_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1120151cUL

#define SMN_FUNC2_PCIE0_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1110251cUL

#define SMN_FUNC2_PCIE1_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1120251cUL

#define SMN_FUNC3_PCIE0_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1110351cUL

#define SMN_FUNC3_PCIE1_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1120351cUL

#define SMN_FUNC4_PCIE0_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1110451cUL

#define SMN_FUNC4_PCIE1_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1120451cUL

#define SMN_FUNC5_PCIE1_TRANSMITTED_MODIFIED_TS_DATA2_ADDRESS    0x1120551cUL


/***********************************************************
* Register Name : VENDOR_CAP_LIST
* Register Description :
* Vendor-defined Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the capability.
#define VENDOR_CAP_LIST_CAP_ID_OFFSET      0
#define VENDOR_CAP_LIST_CAP_ID_MASK        0xff

// Bitfield Description : Pointer to the next item in the capabilities list, or 00h if no other items exist.
#define VENDOR_CAP_LIST_NEXT_PTR_OFFSET      8
#define VENDOR_CAP_LIST_NEXT_PTR_MASK        0xff00

// Bitfield Description : Length in bytes.
#define VENDOR_CAP_LIST_LENGTH_OFFSET      16
#define VENDOR_CAP_LIST_LENGTH_MASK        0xff0000

// Bitfield Description : 
#define VENDOR_CAP_LIST_Reserved_31_24_OFFSET      24
#define VENDOR_CAP_LIST_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            CAP_ID:8;
    UINT32                            NEXT_PTR:8;
    UINT32                            LENGTH:8;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} VENDOR_CAP_LIST_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_VENDOR_CAP_LIST_ADDRESS    0x11100048UL

#define SMN_FUNC0_PCIE1_VENDOR_CAP_LIST_ADDRESS    0x11200048UL

#define SMN_FUNC0_PCIE2_VENDOR_CAP_LIST_ADDRESS    0x11300048UL

#define SMN_FUNC0_PCIE3_VENDOR_CAP_LIST_ADDRESS    0x11400048UL

#define SMN_FUNC1_PCIE0_VENDOR_CAP_LIST_ADDRESS    0x11101048UL

#define SMN_FUNC1_PCIE1_VENDOR_CAP_LIST_ADDRESS    0x11201048UL

#define SMN_FUNC2_PCIE0_VENDOR_CAP_LIST_ADDRESS    0x11102048UL

#define SMN_FUNC2_PCIE1_VENDOR_CAP_LIST_ADDRESS    0x11202048UL

#define SMN_FUNC3_PCIE0_VENDOR_CAP_LIST_ADDRESS    0x11103048UL

#define SMN_FUNC3_PCIE1_VENDOR_CAP_LIST_ADDRESS    0x11203048UL

#define SMN_FUNC4_PCIE0_VENDOR_CAP_LIST_ADDRESS    0x11104048UL

#define SMN_FUNC4_PCIE1_VENDOR_CAP_LIST_ADDRESS    0x11204048UL

#define SMN_FUNC5_PCIE1_VENDOR_CAP_LIST_ADDRESS    0x11205048UL


/***********************************************************
* Register Name : VENDOR_ID
* Register Description :
* Vendor ID register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Identifies the manufacturer of the device.
#define VENDOR_ID_VENDOR_ID_OFFSET      0
#define VENDOR_ID_VENDOR_ID_MASK        0xffff

typedef union {
  struct {
    UINT16                            VENDOR_ID:16;
  } Field;
  UINT16 Value;
} VENDOR_ID_PCIERCCFG_STRUCT;

#define SMN_FUNC0_PCIE0_VENDOR_ID_ADDRESS    0x11100000UL

#define SMN_FUNC0_PCIE1_VENDOR_ID_ADDRESS    0x11200000UL

#define SMN_FUNC0_PCIE2_VENDOR_ID_ADDRESS    0x11300000UL

#define SMN_FUNC0_PCIE3_VENDOR_ID_ADDRESS    0x11400000UL

#define SMN_FUNC1_PCIE0_VENDOR_ID_ADDRESS    0x11101000UL

#define SMN_FUNC1_PCIE1_VENDOR_ID_ADDRESS    0x11201000UL

#define SMN_FUNC2_PCIE0_VENDOR_ID_ADDRESS    0x11102000UL

#define SMN_FUNC2_PCIE1_VENDOR_ID_ADDRESS    0x11202000UL

#define SMN_FUNC3_PCIE0_VENDOR_ID_ADDRESS    0x11103000UL

#define SMN_FUNC3_PCIE1_VENDOR_ID_ADDRESS    0x11203000UL

#define SMN_FUNC4_PCIE0_VENDOR_ID_ADDRESS    0x11104000UL

#define SMN_FUNC4_PCIE1_VENDOR_ID_ADDRESS    0x11204000UL

#define SMN_FUNC5_PCIE1_VENDOR_ID_ADDRESS    0x11205000UL

#endif /* _PCIERCCFG_H_ */
