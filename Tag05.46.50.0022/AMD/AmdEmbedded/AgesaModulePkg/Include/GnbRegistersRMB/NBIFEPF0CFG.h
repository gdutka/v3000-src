/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _NBIFEPF0CFG_H_
#define _NBIFEPF0CFG_H_


/***********************************************************
* Register Name : ADAPTER_ID
* Register Description :
* Subsystem Vendor ID and Subsystem ID registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Subsystem Vendor ID.
#define ADAPTER_ID_SUBSYSTEM_VENDOR_ID_OFFSET      0
#define ADAPTER_ID_SUBSYSTEM_VENDOR_ID_MASK        0xffff

// Bitfield Description : Subsystem ID.
#define ADAPTER_ID_SUBSYSTEM_ID_OFFSET      16
#define ADAPTER_ID_SUBSYSTEM_ID_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SUBSYSTEM_VENDOR_ID:16;
    UINT32                            SUBSYSTEM_ID:16;
  } Field;
  UINT32 Value;
} ADAPTER_ID_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_ADAPTER_ID_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_ADAPTER_ID_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2c)

#define SMN_DEV1_FUNC0_NBIF0_ADAPTER_ID_ADDRESS    0x1014802cUL

#define SMN_DEV2_FUNC0_NBIF0_ADAPTER_ID_ADDRESS    0x1015002cUL


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
} ADAPTER_ID_W_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_ADAPTER_ID_W_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_ADAPTER_ID_W_OFFSET       0x4c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x4c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_ADAPTER_ID_W_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_ADAPTER_ID_W_OFFSET       0x4c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x4c)

#define SMN_DEV1_FUNC0_NBIF0_ADAPTER_ID_W_ADDRESS    0x1014804cUL

#define SMN_DEV2_FUNC0_NBIF0_ADAPTER_ID_W_ADDRESS    0x1015004cUL


/***********************************************************
* Register Name : BASE_ADDR_1
* Register Description :
* PCI CFG BAR 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit 0 is read-only and used to determine whether the register maps into Memory or I/O space. Base Address registers that map into I/O Space are always 32bits wide with bit 0 hardwired to 1b. Bit 1 is Reserved and must return 0b on reads and the other bits are used to map the Function into I/O Space. Base Address registers that map into Memory Space can be 32 bits or 64 bits wide with bit 0 hardwired to 0b. For Memory Base Address registers, bits 2 and 1 have an encoded meaning. 00 = Base register is 32 bits wide and can be mapped anywhere in the 32 address bit Memory Space. 10 = Base register is 64 bits wide and can be mapped anywhere in the 64 address bit Memory Space. 01/11 = Reserved. For Memory Base Address registers, Bit 3 indicate whether the data is prefetchable. For Memory Base Address registers, Bit 3 to Bit 0 is RO, Bit N to Bit 4 is RO-ZERO. For I/O Base Address registers, Bit 1 to Bit 0 is RO, Bit N to Bit 2 is RO-ZERO. N is determined by corresponding aperture size.
#define BASE_ADDR_1_BASE_ADDR_OFFSET      0
#define BASE_ADDR_1_BASE_ADDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BASE_ADDR:32;
  } Field;
  UINT32 Value;
} BASE_ADDR_1_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_1_OFFSET       0x10
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x10)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_1_OFFSET       0x10
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x10)

#define SMN_DEV1_FUNC0_NBIF0_BASE_ADDR_1_ADDRESS    0x10148010UL

#define SMN_DEV2_FUNC0_NBIF0_BASE_ADDR_1_ADDRESS    0x10150010UL


/***********************************************************
* Register Name : BASE_ADDR_2
* Register Description :
* PCI CFG BAR 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit 0 is read-only and used to determine whether the register maps into Memory or I/O space. Base Address registers that map into I/O Space are always 32bits wide with bit 0 hardwired to 1b. Bit 1 is Reserved and must return 0b on reads and the other bits are used to map the Function into I/O Space. Base Address registers that map into Memory Space can be 32 bits or 64 bits wide with bit 0 hardwired to 0b. For Memory Base Address registers, bits 2 and 1 have an encoded meaning. 00 = Base register is 32 bits wide and can be mapped anywhere in the 32 address bit Memory Space. 10 = Base register is 64 bits wide and can be mapped anywhere in the 64 address bit Memory Space. 01/11 = Reserved. For Memory Base Address registers, Bit 3 indicate whether the data is prefetchable. For Memory Base Address registers, Bit 3 to Bit 0 is RO, Bit N to Bit 4 is RO-ZERO. For I/O Base Address registers, Bit 1 to Bit 0 is RO, Bit N to Bit 2 is RO-ZERO. N is determined by corresponding aperture size.
#define BASE_ADDR_2_BASE_ADDR_OFFSET      0
#define BASE_ADDR_2_BASE_ADDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BASE_ADDR:32;
  } Field;
  UINT32 Value;
} BASE_ADDR_2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_2_OFFSET       0x14
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x14)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_2_OFFSET       0x14
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x14)

#define SMN_DEV1_FUNC0_NBIF0_BASE_ADDR_2_ADDRESS    0x10148014UL

#define SMN_DEV2_FUNC0_NBIF0_BASE_ADDR_2_ADDRESS    0x10150014UL


/***********************************************************
* Register Name : BASE_ADDR_3
* Register Description :
* PCI CFG BAR 3.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit 0 is read-only and used to determine whether the register maps into Memory or I/O space. Base Address registers that map into I/O Space are always 32bits wide with bit 0 hardwired to 1b. Bit 1 is Reserved and must return 0b on reads and the other bits are used to map the Function into I/O Space. Base Address registers that map into Memory Space can be 32 bits or 64 bits wide with bit 0 hardwired to 0b. For Memory Base Address registers, bits 2 and 1 have an encoded meaning. 00 = Base register is 32 bits wide and can be mapped anywhere in the 32 address bit Memory Space. 10 = Base register is 64 bits wide and can be mapped anywhere in the 64 address bit Memory Space. 01/11 = Reserved. For Memory Base Address registers, Bit 3 indicate whether the data is prefetchable. For Memory Base Address registers, Bit 3 to Bit 0 is RO, Bit N to Bit 4 is RO-ZERO. For I/O Base Address registers, Bit 1 to Bit 0 is RO, Bit N to Bit 2 is RO-ZERO. N is determined by corresponding aperture size.
#define BASE_ADDR_3_BASE_ADDR_OFFSET      0
#define BASE_ADDR_3_BASE_ADDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BASE_ADDR:32;
  } Field;
  UINT32 Value;
} BASE_ADDR_3_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_3_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_3_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_3_OFFSET       0x18
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x18)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_3_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_3_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_3_OFFSET       0x18
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x18)

#define SMN_DEV1_FUNC0_NBIF0_BASE_ADDR_3_ADDRESS    0x10148018UL

#define SMN_DEV2_FUNC0_NBIF0_BASE_ADDR_3_ADDRESS    0x10150018UL


/***********************************************************
* Register Name : BASE_ADDR_4
* Register Description :
* PCI CFG BAR 4.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit 0 is read-only and used to determine whether the register maps into Memory or I/O space. Base Address registers that map into I/O Space are always 32bits wide with bit 0 hardwired to 1b. Bit 1 is Reserved and must return 0b on reads and the other bits are used to map the Function into I/O Space. Base Address registers that map into Memory Space can be 32 bits or 64 bits wide with bit 0 hardwired to 0b. For Memory Base Address registers, bits 2 and 1 have an encoded meaning. 00 = Base register is 32 bits wide and can be mapped anywhere in the 32 address bit Memory Space. 10 = Base register is 64 bits wide and can be mapped anywhere in the 64 address bit Memory Space. 01/11 = Reserved. For Memory Base Address registers, Bit 3 indicate whether the data is prefetchable. For Memory Base Address registers, Bit 3 to Bit 0 is RO, Bit N to Bit 4 is RO-ZERO. For I/O Base Address registers, Bit 1 to Bit 0 is RO, Bit N to Bit 2 is RO-ZERO. N is determined by corresponding aperture size.
#define BASE_ADDR_4_BASE_ADDR_OFFSET      0
#define BASE_ADDR_4_BASE_ADDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BASE_ADDR:32;
  } Field;
  UINT32 Value;
} BASE_ADDR_4_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_4_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_4_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_4_OFFSET       0x1c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x1c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_4_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_4_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_4_OFFSET       0x1c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x1c)

#define SMN_DEV1_FUNC0_NBIF0_BASE_ADDR_4_ADDRESS    0x1014801cUL

#define SMN_DEV2_FUNC0_NBIF0_BASE_ADDR_4_ADDRESS    0x1015001cUL


/***********************************************************
* Register Name : BASE_ADDR_5
* Register Description :
* PCI CFG BAR 5.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit 0 is read-only and used to determine whether the register maps into Memory or I/O space. Base Address registers that map into I/O Space are always 32bits wide with bit 0 hardwired to 1b. Bit 1 is Reserved and must return 0b on reads and the other bits are used to map the Function into I/O Space. Base Address registers that map into Memory Space can be 32 bits or 64 bits wide with bit 0 hardwired to 0b. For Memory Base Address registers, bits 2 and 1 have an encoded meaning. 00 = Base register is 32 bits wide and can be mapped anywhere in the 32 address bit Memory Space. 10 = Base register is 64 bits wide and can be mapped anywhere in the 64 address bit Memory Space. 01/11 = Reserved. For Memory Base Address registers, Bit 3 indicate whether the data is prefetchable. For Memory Base Address registers, Bit 3 to Bit 0 is RO, Bit N to Bit 4 is RO-ZERO. For I/O Base Address registers, Bit 1 to Bit 0 is RO, Bit N to Bit 2 is RO-ZERO. N is determined by corresponding aperture size.
#define BASE_ADDR_5_BASE_ADDR_OFFSET      0
#define BASE_ADDR_5_BASE_ADDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BASE_ADDR:32;
  } Field;
  UINT32 Value;
} BASE_ADDR_5_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_5_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_5_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_5_OFFSET       0x20
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x20)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_5_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_5_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_5_OFFSET       0x20
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x20)

#define SMN_DEV1_FUNC0_NBIF0_BASE_ADDR_5_ADDRESS    0x10148020UL

#define SMN_DEV2_FUNC0_NBIF0_BASE_ADDR_5_ADDRESS    0x10150020UL


/***********************************************************
* Register Name : BASE_ADDR_6
* Register Description :
* PCI CFG BAR 6.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit 0 is read-only and used to determine whether the register maps into Memory or I/O space. Base Address registers that map into I/O Space are always 32bits wide with bit 0 hardwired to 1b. Bit 1 is Reserved and must return 0b on reads and the other bits are used to map the Function into I/O Space. Base Address registers that map into Memory Space can be 32 bits or 64 bits wide with bit 0 hardwired to 0b. For Memory Base Address registers, bits 2 and 1 have an encoded meaning. 00 = Base register is 32 bits wide and can be mapped anywhere in the 32 address bit Memory Space. 10 = Base register is 64 bits wide and can be mapped anywhere in the 64 address bit Memory Space. 01/11 = Reserved. For Memory Base Address registers, Bit 3 indicate whether the data is prefetchable. For Memory Base Address registers, Bit 3 to Bit 0 is RO, Bit N to Bit 4 is RO-ZERO. For I/O Base Address registers, Bit 1 to Bit 0 is RO, Bit N to Bit 2 is RO-ZERO. N is determined by corresponding aperture size.
#define BASE_ADDR_6_BASE_ADDR_OFFSET      0
#define BASE_ADDR_6_BASE_ADDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            BASE_ADDR:32;
  } Field;
  UINT32 Value;
} BASE_ADDR_6_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_6_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_6_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_6_OFFSET       0x24
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_ADDR_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x24)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_6_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_6_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_6_OFFSET       0x24
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_ADDR_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x24)

#define SMN_DEV1_FUNC0_NBIF0_BASE_ADDR_6_ADDRESS    0x10148024UL

#define SMN_DEV2_FUNC0_NBIF0_BASE_ADDR_6_ADDRESS    0x10150024UL


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
} BASE_CLASS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_CLASS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_CLASS_OFFSET       0xb
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BASE_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xb)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_CLASS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_CLASS_OFFSET       0xb
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BASE_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xb)

#define SMN_DEV1_FUNC0_NBIF0_BASE_CLASS_ADDRESS    0x1014800bUL

#define SMN_DEV2_FUNC0_NBIF0_BASE_CLASS_ADDRESS    0x1015000bUL


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
} BIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BIST_OFFSET       0xf
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_BIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xf)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BIST_OFFSET       0xf
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_BIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xf)

#define SMN_DEV1_FUNC0_NBIF0_BIST_ADDRESS    0x1014800fUL

#define SMN_DEV2_FUNC0_NBIF0_BIST_ADDRESS    0x1015000fUL


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
} CACHE_LINE_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CACHE_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CACHE_LINE_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CACHE_LINE_OFFSET       0xc
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CACHE_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CACHE_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CACHE_LINE_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CACHE_LINE_OFFSET       0xc
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CACHE_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc)

#define SMN_DEV1_FUNC0_NBIF0_CACHE_LINE_ADDRESS    0x1014800cUL

#define SMN_DEV2_FUNC0_NBIF0_CACHE_LINE_ADDRESS    0x1015000cUL


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
} CAP_PTR_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CAP_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CAP_PTR_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CAP_PTR_OFFSET       0x34
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CAP_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x34)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CAP_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CAP_PTR_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CAP_PTR_OFFSET       0x34
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CAP_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x34)

#define SMN_DEV1_FUNC0_NBIF0_CAP_PTR_ADDRESS    0x10148034UL

#define SMN_DEV2_FUNC0_NBIF0_CAP_PTR_ADDRESS    0x10150034UL


/***********************************************************
* Register Name : CARDBUS_CIS_PTR
* Register Description :
* Cardbus CIS Pointer register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Does not apply to PCI Express.
#define CARDBUS_CIS_PTR_CARDBUS_CIS_PTR_OFFSET      0
#define CARDBUS_CIS_PTR_CARDBUS_CIS_PTR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CARDBUS_CIS_PTR:32;
  } Field;
  UINT32 Value;
} CARDBUS_CIS_PTR_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CARDBUS_CIS_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CARDBUS_CIS_PTR_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CARDBUS_CIS_PTR_OFFSET       0x28
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_CARDBUS_CIS_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x28)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CARDBUS_CIS_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CARDBUS_CIS_PTR_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CARDBUS_CIS_PTR_OFFSET       0x28
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_CARDBUS_CIS_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x28)

#define SMN_DEV1_FUNC0_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10148028UL

#define SMN_DEV2_FUNC0_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10150028UL


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
} COMMAND_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_COMMAND_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_COMMAND_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x4)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_COMMAND_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_COMMAND_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x4)

#define SMN_DEV1_FUNC0_NBIF0_COMMAND_ADDRESS    0x10148004UL

#define SMN_DEV2_FUNC0_NBIF0_COMMAND_ADDRESS    0x10150004UL


/***********************************************************
* Register Name : DATA_LINK_FEATURE_CAP
* Register Description :
* Data Link Feature Capabilities register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates which Data Link Features the Port supports. This field is a bit vector that covers all supported features: Bit 0 is Scaled Flow Control Supported. If Data Link Feature Exchange Enable bit is set, this field's value is sent in the Port's transmitted Data Link Feature DLLP.
#define DATA_LINK_FEATURE_CAP_LOCAL_DLF_SUPPORTED_OFFSET      0
#define DATA_LINK_FEATURE_CAP_LOCAL_DLF_SUPPORTED_MASK        0x7fffff

// Bitfield Description : 
#define DATA_LINK_FEATURE_CAP_Reserved_30_23_OFFSET      23
#define DATA_LINK_FEATURE_CAP_Reserved_30_23_MASK        0x7f800000

// Bitfield Description : Indicates if the Port will perform the Data Link Feature Exchange during Data Link Layer activation.
#define DATA_LINK_FEATURE_CAP_DLF_EXCHANGE_ENABLE_OFFSET      31
#define DATA_LINK_FEATURE_CAP_DLF_EXCHANGE_ENABLE_MASK        0x80000000

typedef union {
  struct {
    UINT32                            LOCAL_DLF_SUPPORTED:23;
    UINT32                            Reserved_30_23:8;
    UINT32                            DLF_EXCHANGE_ENABLE:1;
  } Field;
  UINT32 Value;
} DATA_LINK_FEATURE_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DATA_LINK_FEATURE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DATA_LINK_FEATURE_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DATA_LINK_FEATURE_CAP_OFFSET       0x404
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DATA_LINK_FEATURE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x404)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DATA_LINK_FEATURE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DATA_LINK_FEATURE_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DATA_LINK_FEATURE_CAP_OFFSET       0x404
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DATA_LINK_FEATURE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x404)

#define SMN_DEV1_FUNC0_NBIF0_DATA_LINK_FEATURE_CAP_ADDRESS    0x10148404UL

#define SMN_DEV2_FUNC0_NBIF0_DATA_LINK_FEATURE_CAP_ADDRESS    0x10150404UL


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
} DATA_LINK_FEATURE_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DATA_LINK_FEATURE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DATA_LINK_FEATURE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DATA_LINK_FEATURE_STATUS_OFFSET       0x408
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DATA_LINK_FEATURE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x408)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DATA_LINK_FEATURE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DATA_LINK_FEATURE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DATA_LINK_FEATURE_STATUS_OFFSET       0x408
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DATA_LINK_FEATURE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x408)

#define SMN_DEV1_FUNC0_NBIF0_DATA_LINK_FEATURE_STATUS_ADDRESS    0x10148408UL

#define SMN_DEV2_FUNC0_NBIF0_DATA_LINK_FEATURE_STATUS_ADDRESS    0x10150408UL


/***********************************************************
* Register Name : DBESL_DBESLD
* Register Description :
* Default Best Effort Service Latency and Default Best Effort Service Latency Deep registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : If non-zero, indicates the Default Best Effort Service Latency value.
#define DBESL_DBESLD_DBESL_OFFSET      0
#define DBESL_DBESLD_DBESL_MASK        0xf

// Bitfield Description : If non-zero, indicates the Default Best Effort Service Latency Deep value.
#define DBESL_DBESLD_DBESLD_OFFSET      4
#define DBESL_DBESLD_DBESLD_MASK        0xf0

typedef union {
  struct {
    UINT8                            DBESL:4;
    UINT8                            DBESLD:4;
  } Field;
  UINT8 Value;
} DBESL_DBESLD_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DBESL_DBESLD_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DBESL_DBESLD_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DBESL_DBESLD_OFFSET       0x62
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DBESL_DBESLD_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x62)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DBESL_DBESLD_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DBESL_DBESLD_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DBESL_DBESLD_OFFSET       0x62
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DBESL_DBESLD_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x62)

#define SMN_DEV1_FUNC0_NBIF0_DBESL_DBESLD_ADDRESS    0x10148062UL

#define SMN_DEV2_FUNC0_NBIF0_DBESL_DBESLD_ADDRESS    0x10150062UL


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

// Bitfield Description : When 10-Bit Tag Requseter Supported bit is clear, indicates the maximum supported size of the Tag field as a Requester.
#define DEVICE_CAP_EXTENDED_TAG_OFFSET      5
#define DEVICE_CAP_EXTENDED_TAG_MASK        0x20

// Bitfield Description : (Endpoints only) Indicates the maximum acceptable total latency that Endpoint can withstand due to the transition from L0s state to L0 state. For non-Endpoints this field is Reserved.
#define DEVICE_CAP_L0S_ACCEPTABLE_LATENCY_OFFSET      6
#define DEVICE_CAP_L0S_ACCEPTABLE_LATENCY_MASK        0x1c0

// Bitfield Description : (Endpoints only) Indicates the maximum acceptable latency that an Endpoint can withstand due to the transition from L1 state to L0 state. For non-Endpoints this field is Reserved.
#define DEVICE_CAP_L1_ACCEPTABLE_LATENCY_OFFSET      9
#define DEVICE_CAP_L1_ACCEPTABLE_LATENCY_MASK        0xe00

// Bitfield Description : 
#define DEVICE_CAP_Reserved_14_12_OFFSET      12
#define DEVICE_CAP_Reserved_14_12_MASK        0x7000

// Bitfield Description : Indicates the function implements Role-Based Error Reporting functionality.
#define DEVICE_CAP_ROLE_BASED_ERR_REPORTING_OFFSET      15
#define DEVICE_CAP_ROLE_BASED_ERR_REPORTING_MASK        0x8000

// Bitfield Description : 
#define DEVICE_CAP_Reserved_17_16_OFFSET      16
#define DEVICE_CAP_Reserved_17_16_MASK        0x30000

// Bitfield Description : (Upstream Ports only) In combination with the Slot Power Limit Scale value, indicates the upper limit on power supplied by slot.
#define DEVICE_CAP_CAPTURED_SLOT_POWER_LIMIT_OFFSET      18
#define DEVICE_CAP_CAPTURED_SLOT_POWER_LIMIT_MASK        0x3fc0000

// Bitfield Description : (Upstream Ports only) Indicates the scale used for the Slot Power Limit Value.
#define DEVICE_CAP_CAPTURED_SLOT_POWER_SCALE_OFFSET      26
#define DEVICE_CAP_CAPTURED_SLOT_POWER_SCALE_MASK        0xc000000

// Bitfield Description : Indicates if the function supports Function Level Resets.
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
    UINT32                            Reserved_17_16:2;
    UINT32                            CAPTURED_SLOT_POWER_LIMIT:8;
    UINT32                            CAPTURED_SLOT_POWER_SCALE:2;
    UINT32                            FLR_CAPABLE:1;
    UINT32                            Reserved_31_29:3;
  } Field;
  UINT32 Value;
} DEVICE_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CAP_OFFSET       0x68
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x68)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CAP_OFFSET       0x68
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x68)

#define SMN_DEV1_FUNC0_NBIF0_DEVICE_CAP_ADDRESS    0x10148068UL

#define SMN_DEV2_FUNC0_NBIF0_DEVICE_CAP_ADDRESS    0x10150068UL


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

// Bitfield Description : Indicates if a Port supports ARI forwarding.
#define DEVICE_CAP2_ARI_FORWARDING_SUPPORTED_OFFSET      5
#define DEVICE_CAP2_ARI_FORWARDING_SUPPORTED_MASK        0x20

// Bitfield Description : Indicates if a Port supports AtomicOp routing.
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

// Bitfield Description : Indicates if a routing element supports never allowing posted requests with Relaxed Ordering attribute bit set, to pass each other.
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

// Bitfield Description : Indicateds support level of the optional Emergency Power Reduction State feature. A function can enter Emergency Power Reduction State autonomously, or based on one of two mechanisms defined by the associated Form Factor Specification. Functions that are in the Emergency Power Reduction State consume less power. The Emergency Power Reduction mechanism permits a chassis to request add-in cards to rapidly enter Emergency Power Reduction State without involving system software.
#define DEVICE_CAP2_EMER_POWER_REDUCTION_SUPPORTED_OFFSET      24
#define DEVICE_CAP2_EMER_POWER_REDUCTION_SUPPORTED_MASK        0x3000000

// Bitfield Description : If set, the function requires complete or partial initialization upon exit from the Emergency Power Reduction State. If clear, the function requires no software intervention to return to normal operation upon exit from the Emergency Power Reduction State.
#define DEVICE_CAP2_EMER_POWER_REDUCTION_INIT_REQ_OFFSET      26
#define DEVICE_CAP2_EMER_POWER_REDUCTION_INIT_REQ_MASK        0x4000000

// Bitfield Description : 
#define DEVICE_CAP2_Reserved_30_27_OFFSET      27
#define DEVICE_CAP2_Reserved_30_27_MASK        0x78000000

// Bitfield Description : Indicates if the function supports Function Readiness Status Capability
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
} DEVICE_CAP2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CAP2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CAP2_OFFSET       0x88
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x88)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CAP2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CAP2_OFFSET       0x88
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x88)

#define SMN_DEV1_FUNC0_NBIF0_DEVICE_CAP2_ADDRESS    0x10148088UL

#define SMN_DEV2_FUNC0_NBIF0_DEVICE_CAP2_ADDRESS    0x10150088UL


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

// Bitfield Description : Controls if a function is permitted to use an 8-bit Tag field as a Requester. When disabled the function is restricted to use a 5-bit Tag field.
#define DEVICE_CNTL_EXTENDED_TAG_EN_OFFSET      8
#define DEVICE_CNTL_EXTENDED_TAG_EN_MASK        0x100

// Bitfield Description : Controls if a function is permitted to use unclaimed function numbers as Phantom Functions to extend the number of outstanding transaction identifiers.
#define DEVICE_CNTL_PHANTOM_FUNC_EN_OFFSET      9
#define DEVICE_CNTL_PHANTOM_FUNC_EN_MASK        0x200

// Bitfield Description : Controls if a function is permitted to draw AUX power independent of PME AUX power. This bit is sticky.
#define DEVICE_CNTL_AUX_POWER_PM_EN_OFFSET      10
#define DEVICE_CNTL_AUX_POWER_PM_EN_MASK        0x400

// Bitfield Description : Controls if the function is permitted to set the No Snoop bit in the Requester Attributes of transactions it initiates that do not require hardware enforced cache coherency.
#define DEVICE_CNTL_NO_SNOOP_EN_OFFSET      11
#define DEVICE_CNTL_NO_SNOOP_EN_MASK        0x800

// Bitfield Description : Controls the maximum Read Request size for the function as a Requester.
#define DEVICE_CNTL_MAX_READ_REQUEST_SIZE_OFFSET      12
#define DEVICE_CNTL_MAX_READ_REQUEST_SIZE_MASK        0x7000

// Bitfield Description : (Endpoints only) A write of 1 to this bit initiates Function Level Reset to the function. Reads always return 0.
// (Bridges only) Controls if a PCI-E to PCI/PCI-X Bridge is permitted to return Configuration Retry Status.
// For non-Bridges and non-Endpoints this bit is Reserved.
#define DEVICE_CNTL_INITIATE_FLR_OFFSET      15
#define DEVICE_CNTL_INITIATE_FLR_MASK        0x8000

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
    UINT16                            INITIATE_FLR:1;
  } Field;
  UINT16 Value;
} DEVICE_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CNTL_OFFSET       0x6c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x6c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CNTL_OFFSET       0x6c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x6c)

#define SMN_DEV1_FUNC0_NBIF0_DEVICE_CNTL_ADDRESS    0x1014806cUL

#define SMN_DEV2_FUNC0_NBIF0_DEVICE_CNTL_ADDRESS    0x1015006cUL


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

// Bitfield Description : Controls the ability of a Port to allow Configuration requests for Device Numbers greater than 0, when converting from a Type 1 Configuration cycle to a Type 0 Configuration cycle.
#define DEVICE_CNTL2_ARI_FORWARDING_EN_OFFSET      5
#define DEVICE_CNTL2_ARI_FORWARDING_EN_MASK        0x20

// Bitfield Description : Controls the ability of a function to initiate AtomicOp requests.
#define DEVICE_CNTL2_ATOMICOP_REQUEST_EN_OFFSET      6
#define DEVICE_CNTL2_ATOMICOP_REQUEST_EN_MASK        0x40

// Bitfield Description : Blocking atomics that target going out of this Egress Port.
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

// Bitfield Description : If set, all functions in the component that support Emergency Power Reduction State must enter the Emergency Power Reduction State. If clear these functions must exit the Emergency Power Reduction State if no other reasons exist to preclude exiting this state.
#define DEVICE_CNTL2_EMER_POWER_REDUCTION_REQUEST_OFFSET      11
#define DEVICE_CNTL2_EMER_POWER_REDUCTION_REQUEST_MASK        0x800

// Bitfield Description : Controls if a function is permitted to use a 10-bit Tag field as a Requester. When disabled the function uses the setting in the Extended Tag Field Enable bit.
#define DEVICE_CNTL2_TEN_BIT_TAG_REQUESTER_ENABLE_OFFSET      12
#define DEVICE_CNTL2_TEN_BIT_TAG_REQUESTER_ENABLE_MASK        0x1000

// Bitfield Description : Controls if the function is permitted to use Optimized Buffer Flush/Fill signaling.
#define DEVICE_CNTL2_OBFF_EN_OFFSET      13
#define DEVICE_CNTL2_OBFF_EN_MASK        0x6000

// Bitfield Description : For Root Ports and Switch Ports, controls the ability of a Port to block TLPs containing End-End TLP Prefixes that target going out of this Egress Port. For all other functions, this bit is Reserved.
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
} DEVICE_CNTL2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CNTL2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CNTL2_OFFSET       0x8c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x8c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CNTL2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CNTL2_OFFSET       0x8c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x8c)

#define SMN_DEV1_FUNC0_NBIF0_DEVICE_CNTL2_ADDRESS    0x1014808cUL

#define SMN_DEV2_FUNC0_NBIF0_DEVICE_CNTL2_ADDRESS    0x1015008cUL


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
} DEVICE_ID_NBIFEPF0CFG_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_ID_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_ID_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2)

#define SMN_DEV1_FUNC0_NBIF0_DEVICE_ID_ADDRESS    0x10148002UL

#define SMN_DEV2_FUNC0_NBIF0_DEVICE_ID_ADDRESS    0x10150002UL


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

// Bitfield Description : This bit is set when the function is in the Emergency Power Reduction Status. Whenever any condition is present that would cause the Emergency Power Reduction State to be entered, the function remains in the Emergency Power Reduction State and writes to this bit have no effect.
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
} DEVICE_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_STATUS_OFFSET       0x6e
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x6e)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_STATUS_OFFSET       0x6e
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x6e)

#define SMN_DEV1_FUNC0_NBIF0_DEVICE_STATUS_ADDRESS    0x1014806eUL

#define SMN_DEV2_FUNC0_NBIF0_DEVICE_STATUS_ADDRESS    0x1015006eUL


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
} DEVICE_STATUS2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_STATUS2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_STATUS2_OFFSET       0x8e
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_DEVICE_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x8e)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_STATUS2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_STATUS2_OFFSET       0x8e
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_DEVICE_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x8e)

#define SMN_DEV1_FUNC0_NBIF0_DEVICE_STATUS2_ADDRESS    0x1014808eUL

#define SMN_DEV2_FUNC0_NBIF0_DEVICE_STATUS2_ADDRESS    0x1015008eUL


/***********************************************************
* Register Name : FLADJ
* Register Description :
* Frame Length Adjustment register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : If NO_FL_CAP bit is clear, controls the Frame Length Timing Value. A value of 0 indicates 59488 high speed bit times, while each value increments by 16 high speed bit times. This field is sticky.
#define FLADJ_FLADJ_OFFSET      0
#define FLADJ_FLADJ_MASK        0x3f

// Bitfield Description : 
#define FLADJ_NFC_OFFSET      6
#define FLADJ_NFC_MASK        0x40

// Bitfield Description : 
#define FLADJ_Reserved_7_7_OFFSET      7
#define FLADJ_Reserved_7_7_MASK        0x80

typedef union {
  struct {
    UINT8                            FLADJ:6;
    UINT8                            NFC:1;
    UINT8                            Reserved_7_7:1;
  } Field;
  UINT8 Value;
} FLADJ_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_FLADJ_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_FLADJ_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_FLADJ_OFFSET       0x61
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_FLADJ_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x61)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_FLADJ_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_FLADJ_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_FLADJ_OFFSET       0x61
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_FLADJ_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x61)

#define SMN_DEV1_FUNC0_NBIF0_FLADJ_ADDRESS    0x10148061UL

#define SMN_DEV2_FUNC0_NBIF0_FLADJ_ADDRESS    0x10150061UL


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
} HEADER_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_HEADER_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_HEADER_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_HEADER_OFFSET       0xe
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xe)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_HEADER_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_HEADER_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_HEADER_OFFSET       0xe
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xe)

#define SMN_DEV1_FUNC0_NBIF0_HEADER_ADDRESS    0x1014800eUL

#define SMN_DEV2_FUNC0_NBIF0_HEADER_ADDRESS    0x1015000eUL


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
} INTERRUPT_LINE_NBIFEPF0CFG_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_INTERRUPT_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_INTERRUPT_LINE_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_INTERRUPT_LINE_OFFSET       0x3c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_INTERRUPT_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x3c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_INTERRUPT_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_INTERRUPT_LINE_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_INTERRUPT_LINE_OFFSET       0x3c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_INTERRUPT_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x3c)

#define SMN_DEV1_FUNC0_NBIF0_INTERRUPT_LINE_ADDRESS    0x1014803cUL

#define SMN_DEV2_FUNC0_NBIF0_INTERRUPT_LINE_ADDRESS    0x1015003cUL


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
} INTERRUPT_PIN_NBIFEPF0CFG_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_INTERRUPT_PIN_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_INTERRUPT_PIN_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_INTERRUPT_PIN_OFFSET       0x3d
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_INTERRUPT_PIN_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x3d)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_INTERRUPT_PIN_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_INTERRUPT_PIN_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_INTERRUPT_PIN_OFFSET       0x3d
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_INTERRUPT_PIN_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x3d)

#define SMN_DEV1_FUNC0_NBIF0_INTERRUPT_PIN_ADDRESS    0x1014803dUL

#define SMN_DEV2_FUNC0_NBIF0_INTERRUPT_PIN_ADDRESS    0x1015003dUL


/***********************************************************
* Register Name : LANE_EQUALIZATION_CNTL_16GT
* Register Description :
* 16.0 GT/s Lane 0 Equalization Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define LANE_EQUALIZATION_CNTL_16GT_LANE_DSP_16GT_TX_PRESET_OFFSET      0
#define LANE_EQUALIZATION_CNTL_16GT_LANE_DSP_16GT_TX_PRESET_MASK        0xf

// Bitfield Description : (Upstream Ports only) captures the transmitter preset value that the Upstream Port received from the other side for initial operation at 16.0 GT/s.
#define LANE_EQUALIZATION_CNTL_16GT_LANE_USP_16GT_TX_PRESET_OFFSET      4
#define LANE_EQUALIZATION_CNTL_16GT_LANE_USP_16GT_TX_PRESET_MASK        0xf0

typedef union {
  struct {
    UINT8                            LANE_DSP_16GT_TX_PRESET:4;
    UINT8                            LANE_USP_16GT_TX_PRESET:4;
  } Field;
  UINT8 Value;
} LANE_EQUALIZATION_CNTL_16GT_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x430
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x430)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x431)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43a)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43b)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43c)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43d)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43e)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43f)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x432)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x433)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x434)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x435)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x436)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x437)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x438)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x439)

#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x430
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x430)

#define SMN_DEV1_FUNC0_NBIF0_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10148430UL

#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x43a
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43a)

#define SMN_DEV1_FUNC0_NBIF0_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1014843aUL

#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x43b
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43b)

#define SMN_DEV1_FUNC0_NBIF0_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1014843bUL

#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x43c
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43c)

#define SMN_DEV1_FUNC0_NBIF0_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1014843cUL

#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x43d
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43d)

#define SMN_DEV1_FUNC0_NBIF0_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1014843dUL

#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x43e
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43e)

#define SMN_DEV1_FUNC0_NBIF0_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1014843eUL

#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x43f
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x43f)

#define SMN_DEV1_FUNC0_NBIF0_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1014843fUL

#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x431
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x431)

#define SMN_DEV1_FUNC0_NBIF0_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10148431UL

#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x432
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x432)

#define SMN_DEV1_FUNC0_NBIF0_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10148432UL

#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x433
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x433)

#define SMN_DEV1_FUNC0_NBIF0_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10148433UL

#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x434
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x434)

#define SMN_DEV1_FUNC0_NBIF0_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10148434UL

#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x435
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x435)

#define SMN_DEV1_FUNC0_NBIF0_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10148435UL

#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x436
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x436)

#define SMN_DEV1_FUNC0_NBIF0_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10148436UL

#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x437
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x437)

#define SMN_DEV1_FUNC0_NBIF0_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10148437UL

#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x438
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x438)

#define SMN_DEV1_FUNC0_NBIF0_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10148438UL

#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_LANE_EQUALIZATION_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_LANE_EQUALIZATION_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_LANE_EQUALIZATION_CNTL_16GT_OFFSET       0x439
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x439)

#define SMN_DEV1_FUNC0_NBIF0_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10148439UL

#define SMN_DEV2_FUNC0_NBIF0_N0_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10150430UL

#define SMN_DEV2_FUNC0_NBIF0_N10_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1015043aUL

#define SMN_DEV2_FUNC0_NBIF0_N11_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1015043bUL

#define SMN_DEV2_FUNC0_NBIF0_N12_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1015043cUL

#define SMN_DEV2_FUNC0_NBIF0_N13_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1015043dUL

#define SMN_DEV2_FUNC0_NBIF0_N14_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1015043eUL

#define SMN_DEV2_FUNC0_NBIF0_N15_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x1015043fUL

#define SMN_DEV2_FUNC0_NBIF0_N1_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10150431UL

#define SMN_DEV2_FUNC0_NBIF0_N2_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10150432UL

#define SMN_DEV2_FUNC0_NBIF0_N3_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10150433UL

#define SMN_DEV2_FUNC0_NBIF0_N4_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10150434UL

#define SMN_DEV2_FUNC0_NBIF0_N5_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10150435UL

#define SMN_DEV2_FUNC0_NBIF0_N6_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10150436UL

#define SMN_DEV2_FUNC0_NBIF0_N7_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10150437UL

#define SMN_DEV2_FUNC0_NBIF0_N8_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10150438UL

#define SMN_DEV2_FUNC0_NBIF0_N9_LANE_EQUALIZATION_CNTL_16GT_ADDRESS    0x10150439UL


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
} LATENCY_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LATENCY_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LATENCY_OFFSET       0xd
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LATENCY_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LATENCY_OFFSET       0xd
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd)

#define SMN_DEV1_FUNC0_NBIF0_LATENCY_ADDRESS    0x1014800dUL

#define SMN_DEV2_FUNC0_NBIF0_LATENCY_ADDRESS    0x1015000dUL


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

// Bitfield Description : Indicates if the Port tolerates removal of REFCLK via the CLKREQ# mechanism when the Link is in L1 or L2/L3 Ready.
#define LINK_CAP_CLOCK_POWER_MANAGEMENT_OFFSET      18
#define LINK_CAP_CLOCK_POWER_MANAGEMENT_MASK        0x40000

// Bitfield Description : Indicates if the Port supports the detecting and reporting of a Surprise Down error condition.
#define LINK_CAP_SURPRISE_DOWN_ERR_REPORTING_OFFSET      19
#define LINK_CAP_SURPRISE_DOWN_ERR_REPORTING_MASK        0x80000

// Bitfield Description : Indicates if the Port supports the reporting of DL_Active state of the Data Link Control and Management State Machine.
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
} LINK_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP_OFFSET       0x70
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x70)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP_OFFSET       0x70
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x70)

#define SMN_DEV1_FUNC0_NBIF0_LINK_CAP_ADDRESS    0x10148070UL

#define SMN_DEV2_FUNC0_NBIF0_LINK_CAP_ADDRESS    0x10150070UL


/***********************************************************
* Register Name : LINK_CAP2
* Register Description :
* The Link Capabilities 2 register identifies Link specific capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define LINK_CAP2_Reserved_0_0_OFFSET      0
#define LINK_CAP2_Reserved_0_0_MASK        0x1

// Bitfield Description : Identifies what link speeds are supported. Bit 1 = 2.5 GT/s, Bit 2 = 5.0 Gt/s, Bit 3 = 8.0 GT/s, Bit 4 = 16.0 GT/s, Bit 5 = 32.0 GT/s.
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

// Bitfield Description : Indicates if the function supports Device Readiness Status Capability.
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
} LINK_CAP2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP2_OFFSET       0x90
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x90)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP2_OFFSET       0x90
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x90)

#define SMN_DEV1_FUNC0_NBIF0_LINK_CAP2_ADDRESS    0x10148090UL

#define SMN_DEV2_FUNC0_NBIF0_LINK_CAP2_ADDRESS    0x10150090UL


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
} LINK_CAP_16GT_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP_16GT_OFFSET       0x414
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CAP_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x414)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP_16GT_OFFSET       0x414
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CAP_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x414)

#define SMN_DEV1_FUNC0_NBIF0_LINK_CAP_16GT_ADDRESS    0x10148414UL

#define SMN_DEV2_FUNC0_NBIF0_LINK_CAP_16GT_ADDRESS    0x10150414UL


/***********************************************************
* Register Name : LINK_CNTL
* Register Description :
* The Link Control register controls Link specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls the level of ASPM enabled on the Link.
#define LINK_CNTL_PM_CONTROL_OFFSET      0
#define LINK_CNTL_PM_CONTROL_MASK        0x3

// Bitfield Description : Controls the interpretation of the Propagation Delay[31:0] field of the PTM Message.
// For devices that don't support PTM, this bit is Reserved.
#define LINK_CNTL_PTM_PROP_DELAY_ADAPT_INTER_B_OFFSET      2
#define LINK_CNTL_PTM_PROP_DELAY_ADAPT_INTER_B_MASK        0x4

// Bitfield Description : Indicates the RCB value for the Root Port sourcing the topology associated with the Link.
#define LINK_CNTL_READ_CPL_BOUNDARY_OFFSET      3
#define LINK_CNTL_READ_CPL_BOUNDARY_MASK        0x8

// Bitfield Description : (Downstream Ports only) Disables the Link when set to 1, by directing the Physical Layer LTSSM to the Disabled state.
// For Upstream Ports, this bit is Reserved.
#define LINK_CNTL_LINK_DIS_OFFSET      4
#define LINK_CNTL_LINK_DIS_MASK        0x10

// Bitfield Description : (Upstream Ports only) Reserved.
#define LINK_CNTL_RETRAIN_LINK_OFFSET      5
#define LINK_CNTL_RETRAIN_LINK_MASK        0x20

// Bitfield Description : Indicates if this component and the component at the opposite end of this Link are operating with a distributed common reference clock.
#define LINK_CNTL_COMMON_CLOCK_CFG_OFFSET      6
#define LINK_CNTL_COMMON_CLOCK_CFG_MASK        0x40

// Bitfield Description : Forces the transmission of additional ordered sets, exiting the L0s or Recovery states.
#define LINK_CNTL_EXTENDED_SYNC_OFFSET      7
#define LINK_CNTL_EXTENDED_SYNC_MASK        0x80

// Bitfield Description : Controls if the function is permitted to use CLKREQ# signal to power manage the Link clock.
#define LINK_CNTL_CLOCK_POWER_MANAGEMENT_EN_OFFSET      8
#define LINK_CNTL_CLOCK_POWER_MANAGEMENT_EN_MASK        0x100

// Bitfield Description : Controls the Port's ability to autonomously direct changes in link width.
#define LINK_CNTL_HW_AUTONOMOUS_WIDTH_DISABLE_OFFSET      9
#define LINK_CNTL_HW_AUTONOMOUS_WIDTH_DISABLE_MASK        0x200

// Bitfield Description : (Upstream Ports only) Reserved.
#define LINK_CNTL_LINK_BW_MANAGEMENT_INT_EN_OFFSET      10
#define LINK_CNTL_LINK_BW_MANAGEMENT_INT_EN_MASK        0x400

// Bitfield Description : (Upstream Ports only) Reserved.
#define LINK_CNTL_LINK_AUTONOMOUS_BW_INT_EN_OFFSET      11
#define LINK_CNTL_LINK_AUTONOMOUS_BW_INT_EN_MASK        0x800

// Bitfield Description : 
#define LINK_CNTL_Reserved_13_12_OFFSET      12
#define LINK_CNTL_Reserved_13_12_MASK        0x3000

// Bitfield Description : (Upstream Ports only) Reserved.
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
} LINK_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL_OFFSET       0x74
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x74)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL_OFFSET       0x74
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x74)

#define SMN_DEV1_FUNC0_NBIF0_LINK_CNTL_ADDRESS    0x10148074UL

#define SMN_DEV2_FUNC0_NBIF0_LINK_CNTL_ADDRESS    0x10150074UL


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
// At 8.0 GT/s only, when using Enter Compliance bit, this field controls the Port's transmitter Preset setting in compliance mode. This field is sticky.
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
} LINK_CNTL2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL2_OFFSET       0x94
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x94)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL2_OFFSET       0x94
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x94)

#define SMN_DEV1_FUNC0_NBIF0_LINK_CNTL2_ADDRESS    0x10148094UL

#define SMN_DEV2_FUNC0_NBIF0_LINK_CNTL2_ADDRESS    0x10150094UL


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
} LINK_CNTL_16GT_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL_16GT_OFFSET       0x418
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x418)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL_16GT_OFFSET       0x418
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_CNTL_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x418)

#define SMN_DEV1_FUNC0_NBIF0_LINK_CNTL_16GT_ADDRESS    0x10148418UL

#define SMN_DEV2_FUNC0_NBIF0_LINK_CNTL_16GT_ADDRESS    0x10150418UL


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

// Bitfield Description : If Data Link Layer Link Active Capability is supported, indicates the status of the Data Link Control and Management State Machine.
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
} LINK_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS_OFFSET       0x76
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x76)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS_OFFSET       0x76
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x76)

#define SMN_DEV1_FUNC0_NBIF0_LINK_STATUS_ADDRESS    0x10148076UL

#define SMN_DEV2_FUNC0_NBIF0_LINK_STATUS_ADDRESS    0x10150076UL


/***********************************************************
* Register Name : LINK_STATUS2
* Register Description :
* The Link Status 2 register provides information about Link specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : At 5.0 GT/s only, indicates the Port's transmitter de-emphasis level.
#define LINK_STATUS2_CUR_DEEMPHASIS_LEVEL_OFFSET      0
#define LINK_STATUS2_CUR_DEEMPHASIS_LEVEL_MASK        0x1

// Bitfield Description : Indicates if the Transmitter Equalization procedure has completed. This bit is sticky.
#define LINK_STATUS2_EQUALIZATION_COMPLETE_8GT_OFFSET      1
#define LINK_STATUS2_EQUALIZATION_COMPLETE_8GT_MASK        0x2

// Bitfield Description : Indicates if Phase 1 of the Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS2_EQUALIZATION_PHASE1_SUCCESS_8GT_OFFSET      2
#define LINK_STATUS2_EQUALIZATION_PHASE1_SUCCESS_8GT_MASK        0x4

// Bitfield Description : Indicates if Phase 2 of the Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS2_EQUALIZATION_PHASE2_SUCCESS_8GT_OFFSET      3
#define LINK_STATUS2_EQUALIZATION_PHASE2_SUCCESS_8GT_MASK        0x8

// Bitfield Description : Indicates if Phase 3 of the Transmitter Equalization procedure has successfully completed. This bit is sticky.
#define LINK_STATUS2_EQUALIZATION_PHASE3_SUCCESS_8GT_OFFSET      4
#define LINK_STATUS2_EQUALIZATION_PHASE3_SUCCESS_8GT_MASK        0x10

// Bitfield Description : Indicates if hardware requests link equalization to be performed. This bit is sticky.
#define LINK_STATUS2_LINK_EQUALIZATION_REQUEST_8GT_OFFSET      5
#define LINK_STATUS2_LINK_EQUALIZATION_REQUEST_8GT_MASK        0x20

// Bitfield Description : When Retimer Presenct Detect Supported bit is set, indicates if the Port detected a Retimer in the most recent link negotiation. This bit is sticky.
#define LINK_STATUS2_RTM1_PRESENCE_DET_OFFSET      6
#define LINK_STATUS2_RTM1_PRESENCE_DET_MASK        0x40

// Bitfield Description : When Two Retimer Presenct Detect Supported bit is set, indicates if the Port detected two Retimers in the most recent link negotiation. This bit is sticky.
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

// Bitfield Description : (Downstream Ports only) When DRS Supported bit is set, indicates if the Port has received a DRS Message.
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
} LINK_STATUS2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS2_OFFSET       0x96
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x96)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS2_OFFSET       0x96
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x96)

#define SMN_DEV1_FUNC0_NBIF0_LINK_STATUS2_ADDRESS    0x10148096UL

#define SMN_DEV2_FUNC0_NBIF0_LINK_STATUS2_ADDRESS    0x10150096UL


/***********************************************************
* Register Name : LINK_STATUS_16GT
* Register Description :
* The 16.0 GT/s Status register provides information about Physical Layer 16.0 GT/s specific parameters.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the Transmitter Equalization procedure at 16.0 GT/s has completed. This bit is sticky.
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
} LINK_STATUS_16GT_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS_16GT_OFFSET       0x41c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LINK_STATUS_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x41c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS_16GT_OFFSET       0x41c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LINK_STATUS_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x41c)

#define SMN_DEV1_FUNC0_NBIF0_LINK_STATUS_16GT_ADDRESS    0x1014841cUL

#define SMN_DEV2_FUNC0_NBIF0_LINK_STATUS_16GT_ADDRESS    0x1015041cUL


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
} LOCAL_PARITY_MISMATCH_STATUS_16GT_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LOCAL_PARITY_MISMATCH_STATUS_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LOCAL_PARITY_MISMATCH_STATUS_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LOCAL_PARITY_MISMATCH_STATUS_16GT_OFFSET       0x420
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x420)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LOCAL_PARITY_MISMATCH_STATUS_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LOCAL_PARITY_MISMATCH_STATUS_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LOCAL_PARITY_MISMATCH_STATUS_16GT_OFFSET       0x420
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x420)

#define SMN_DEV1_FUNC0_NBIF0_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x10148420UL

#define SMN_DEV2_FUNC0_NBIF0_LOCAL_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x10150420UL


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
} MARGINING_LANE_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x458
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x458)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x45c)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x480)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x484)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x488)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x48c)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x490)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x494)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x460)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x464)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x468)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x46c)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x470)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x474)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x478)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x47c)

#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x458
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x458)

#define SMN_DEV1_FUNC0_NBIF0_N0_MARGINING_LANE_CNTL_ADDRESS    0x10148458UL

#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x480
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x480)

#define SMN_DEV1_FUNC0_NBIF0_N10_MARGINING_LANE_CNTL_ADDRESS    0x10148480UL

#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x484
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x484)

#define SMN_DEV1_FUNC0_NBIF0_N11_MARGINING_LANE_CNTL_ADDRESS    0x10148484UL

#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x488
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x488)

#define SMN_DEV1_FUNC0_NBIF0_N12_MARGINING_LANE_CNTL_ADDRESS    0x10148488UL

#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x48c
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x48c)

#define SMN_DEV1_FUNC0_NBIF0_N13_MARGINING_LANE_CNTL_ADDRESS    0x1014848cUL

#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x490
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x490)

#define SMN_DEV1_FUNC0_NBIF0_N14_MARGINING_LANE_CNTL_ADDRESS    0x10148490UL

#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x494
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x494)

#define SMN_DEV1_FUNC0_NBIF0_N15_MARGINING_LANE_CNTL_ADDRESS    0x10148494UL

#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x45c
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x45c)

#define SMN_DEV1_FUNC0_NBIF0_N1_MARGINING_LANE_CNTL_ADDRESS    0x1014845cUL

#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x460
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x460)

#define SMN_DEV1_FUNC0_NBIF0_N2_MARGINING_LANE_CNTL_ADDRESS    0x10148460UL

#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x464
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x464)

#define SMN_DEV1_FUNC0_NBIF0_N3_MARGINING_LANE_CNTL_ADDRESS    0x10148464UL

#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x468
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x468)

#define SMN_DEV1_FUNC0_NBIF0_N4_MARGINING_LANE_CNTL_ADDRESS    0x10148468UL

#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x46c
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x46c)

#define SMN_DEV1_FUNC0_NBIF0_N5_MARGINING_LANE_CNTL_ADDRESS    0x1014846cUL

#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x470
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x470)

#define SMN_DEV1_FUNC0_NBIF0_N6_MARGINING_LANE_CNTL_ADDRESS    0x10148470UL

#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x474
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x474)

#define SMN_DEV1_FUNC0_NBIF0_N7_MARGINING_LANE_CNTL_ADDRESS    0x10148474UL

#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x478
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x478)

#define SMN_DEV1_FUNC0_NBIF0_N8_MARGINING_LANE_CNTL_ADDRESS    0x10148478UL

#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_MARGINING_LANE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_MARGINING_LANE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_MARGINING_LANE_CNTL_OFFSET       0x47c
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_MARGINING_LANE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x47c)

#define SMN_DEV1_FUNC0_NBIF0_N9_MARGINING_LANE_CNTL_ADDRESS    0x1014847cUL

#define SMN_DEV2_FUNC0_NBIF0_N0_MARGINING_LANE_CNTL_ADDRESS    0x10150458UL

#define SMN_DEV2_FUNC0_NBIF0_N10_MARGINING_LANE_CNTL_ADDRESS    0x10150480UL

#define SMN_DEV2_FUNC0_NBIF0_N11_MARGINING_LANE_CNTL_ADDRESS    0x10150484UL

#define SMN_DEV2_FUNC0_NBIF0_N12_MARGINING_LANE_CNTL_ADDRESS    0x10150488UL

#define SMN_DEV2_FUNC0_NBIF0_N13_MARGINING_LANE_CNTL_ADDRESS    0x1015048cUL

#define SMN_DEV2_FUNC0_NBIF0_N14_MARGINING_LANE_CNTL_ADDRESS    0x10150490UL

#define SMN_DEV2_FUNC0_NBIF0_N15_MARGINING_LANE_CNTL_ADDRESS    0x10150494UL

#define SMN_DEV2_FUNC0_NBIF0_N1_MARGINING_LANE_CNTL_ADDRESS    0x1015045cUL

#define SMN_DEV2_FUNC0_NBIF0_N2_MARGINING_LANE_CNTL_ADDRESS    0x10150460UL

#define SMN_DEV2_FUNC0_NBIF0_N3_MARGINING_LANE_CNTL_ADDRESS    0x10150464UL

#define SMN_DEV2_FUNC0_NBIF0_N4_MARGINING_LANE_CNTL_ADDRESS    0x10150468UL

#define SMN_DEV2_FUNC0_NBIF0_N5_MARGINING_LANE_CNTL_ADDRESS    0x1015046cUL

#define SMN_DEV2_FUNC0_NBIF0_N6_MARGINING_LANE_CNTL_ADDRESS    0x10150470UL

#define SMN_DEV2_FUNC0_NBIF0_N7_MARGINING_LANE_CNTL_ADDRESS    0x10150474UL

#define SMN_DEV2_FUNC0_NBIF0_N8_MARGINING_LANE_CNTL_ADDRESS    0x10150478UL

#define SMN_DEV2_FUNC0_NBIF0_N9_MARGINING_LANE_CNTL_ADDRESS    0x1015047cUL


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
} MARGINING_LANE_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x45a
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x45a)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x45e)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x482)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x486)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x48a)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x48e)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x492)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x496)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x462)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x466)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x46a)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x46e)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x472)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x476)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x47a)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x47e)

#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x45a
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x45a)

#define SMN_DEV1_FUNC0_NBIF0_N0_MARGINING_LANE_STATUS_ADDRESS    0x1014845aUL

#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x482
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x482)

#define SMN_DEV1_FUNC0_NBIF0_N10_MARGINING_LANE_STATUS_ADDRESS    0x10148482UL

#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x486
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x486)

#define SMN_DEV1_FUNC0_NBIF0_N11_MARGINING_LANE_STATUS_ADDRESS    0x10148486UL

#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x48a
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x48a)

#define SMN_DEV1_FUNC0_NBIF0_N12_MARGINING_LANE_STATUS_ADDRESS    0x1014848aUL

#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x48e
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x48e)

#define SMN_DEV1_FUNC0_NBIF0_N13_MARGINING_LANE_STATUS_ADDRESS    0x1014848eUL

#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x492
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x492)

#define SMN_DEV1_FUNC0_NBIF0_N14_MARGINING_LANE_STATUS_ADDRESS    0x10148492UL

#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x496
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x496)

#define SMN_DEV1_FUNC0_NBIF0_N15_MARGINING_LANE_STATUS_ADDRESS    0x10148496UL

#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x45e
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x45e)

#define SMN_DEV1_FUNC0_NBIF0_N1_MARGINING_LANE_STATUS_ADDRESS    0x1014845eUL

#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x462
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x462)

#define SMN_DEV1_FUNC0_NBIF0_N2_MARGINING_LANE_STATUS_ADDRESS    0x10148462UL

#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x466
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x466)

#define SMN_DEV1_FUNC0_NBIF0_N3_MARGINING_LANE_STATUS_ADDRESS    0x10148466UL

#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x46a
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x46a)

#define SMN_DEV1_FUNC0_NBIF0_N4_MARGINING_LANE_STATUS_ADDRESS    0x1014846aUL

#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x46e
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x46e)

#define SMN_DEV1_FUNC0_NBIF0_N5_MARGINING_LANE_STATUS_ADDRESS    0x1014846eUL

#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x472
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x472)

#define SMN_DEV1_FUNC0_NBIF0_N6_MARGINING_LANE_STATUS_ADDRESS    0x10148472UL

#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x476
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x476)

#define SMN_DEV1_FUNC0_NBIF0_N7_MARGINING_LANE_STATUS_ADDRESS    0x10148476UL

#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x47a
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x47a)

#define SMN_DEV1_FUNC0_NBIF0_N8_MARGINING_LANE_STATUS_ADDRESS    0x1014847aUL

#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_MARGINING_LANE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_MARGINING_LANE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_MARGINING_LANE_STATUS_OFFSET       0x47e
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_MARGINING_LANE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x47e)

#define SMN_DEV1_FUNC0_NBIF0_N9_MARGINING_LANE_STATUS_ADDRESS    0x1014847eUL

#define SMN_DEV2_FUNC0_NBIF0_N0_MARGINING_LANE_STATUS_ADDRESS    0x1015045aUL

#define SMN_DEV2_FUNC0_NBIF0_N10_MARGINING_LANE_STATUS_ADDRESS    0x10150482UL

#define SMN_DEV2_FUNC0_NBIF0_N11_MARGINING_LANE_STATUS_ADDRESS    0x10150486UL

#define SMN_DEV2_FUNC0_NBIF0_N12_MARGINING_LANE_STATUS_ADDRESS    0x1015048aUL

#define SMN_DEV2_FUNC0_NBIF0_N13_MARGINING_LANE_STATUS_ADDRESS    0x1015048eUL

#define SMN_DEV2_FUNC0_NBIF0_N14_MARGINING_LANE_STATUS_ADDRESS    0x10150492UL

#define SMN_DEV2_FUNC0_NBIF0_N15_MARGINING_LANE_STATUS_ADDRESS    0x10150496UL

#define SMN_DEV2_FUNC0_NBIF0_N1_MARGINING_LANE_STATUS_ADDRESS    0x1015045eUL

#define SMN_DEV2_FUNC0_NBIF0_N2_MARGINING_LANE_STATUS_ADDRESS    0x10150462UL

#define SMN_DEV2_FUNC0_NBIF0_N3_MARGINING_LANE_STATUS_ADDRESS    0x10150466UL

#define SMN_DEV2_FUNC0_NBIF0_N4_MARGINING_LANE_STATUS_ADDRESS    0x1015046aUL

#define SMN_DEV2_FUNC0_NBIF0_N5_MARGINING_LANE_STATUS_ADDRESS    0x1015046eUL

#define SMN_DEV2_FUNC0_NBIF0_N6_MARGINING_LANE_STATUS_ADDRESS    0x10150472UL

#define SMN_DEV2_FUNC0_NBIF0_N7_MARGINING_LANE_STATUS_ADDRESS    0x10150476UL

#define SMN_DEV2_FUNC0_NBIF0_N8_MARGINING_LANE_STATUS_ADDRESS    0x1015047aUL

#define SMN_DEV2_FUNC0_NBIF0_N9_MARGINING_LANE_STATUS_ADDRESS    0x1015047eUL


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
} MARGINING_PORT_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_PORT_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_PORT_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_PORT_CAP_OFFSET       0x454
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_PORT_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x454)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MARGINING_PORT_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MARGINING_PORT_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MARGINING_PORT_CAP_OFFSET       0x454
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MARGINING_PORT_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x454)

#define SMN_DEV1_FUNC0_NBIF0_MARGINING_PORT_CAP_ADDRESS    0x10148454UL

#define SMN_DEV2_FUNC0_NBIF0_MARGINING_PORT_CAP_ADDRESS    0x10150454UL


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
} MARGINING_PORT_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_PORT_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_PORT_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_PORT_STATUS_OFFSET       0x456
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MARGINING_PORT_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x456)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MARGINING_PORT_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MARGINING_PORT_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MARGINING_PORT_STATUS_OFFSET       0x456
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MARGINING_PORT_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x456)

#define SMN_DEV1_FUNC0_NBIF0_MARGINING_PORT_STATUS_ADDRESS    0x10148456UL

#define SMN_DEV2_FUNC0_NBIF0_MARGINING_PORT_STATUS_ADDRESS    0x10150456UL


/***********************************************************
* Register Name : MAX_LATENCY
* Register Description :
* Max_Lat register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Does not apply to PCI Express.
#define MAX_LATENCY_MAX_LAT_OFFSET      0
#define MAX_LATENCY_MAX_LAT_MASK        0xff

typedef union {
  struct {
    UINT8                            MAX_LAT:8;
  } Field;
  UINT8 Value;
} MAX_LATENCY_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MAX_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MAX_LATENCY_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MAX_LATENCY_OFFSET       0x3f
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MAX_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x3f)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MAX_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MAX_LATENCY_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MAX_LATENCY_OFFSET       0x3f
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MAX_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x3f)

#define SMN_DEV1_FUNC0_NBIF0_MAX_LATENCY_ADDRESS    0x1014803fUL

#define SMN_DEV2_FUNC0_NBIF0_MAX_LATENCY_ADDRESS    0x1015003fUL


/***********************************************************
* Register Name : MIN_GRANT
* Register Description :
* Min_Gnt register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Does not apply to PCI Express.
#define MIN_GRANT_MIN_GNT_OFFSET      0
#define MIN_GRANT_MIN_GNT_MASK        0xff

typedef union {
  struct {
    UINT8                            MIN_GNT:8;
  } Field;
  UINT8 Value;
} MIN_GRANT_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MIN_GRANT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MIN_GRANT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MIN_GRANT_OFFSET       0x3e
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MIN_GRANT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x3e)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MIN_GRANT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MIN_GRANT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MIN_GRANT_OFFSET       0x3e
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MIN_GRANT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x3e)

#define SMN_DEV1_FUNC0_NBIF0_MIN_GRANT_ADDRESS    0x1014803eUL

#define SMN_DEV2_FUNC0_NBIF0_MIN_GRANT_ADDRESS    0x1015003eUL


/***********************************************************
* Register Name : MSIX_CAP_LIST
* Register Description :
* MSI-X Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the capability.
#define MSIX_CAP_LIST_CAP_ID_OFFSET      0
#define MSIX_CAP_LIST_CAP_ID_MASK        0xff

// Bitfield Description : Pointer to the next item in the capabilities list, or 00h if no other items exist.
#define MSIX_CAP_LIST_NEXT_PTR_OFFSET      8
#define MSIX_CAP_LIST_NEXT_PTR_MASK        0xff00

typedef union {
  struct {
    UINT16                            CAP_ID:8;
    UINT16                            NEXT_PTR:8;
  } Field;
  UINT16 Value;
} MSIX_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_CAP_LIST_OFFSET       0xc0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc0)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_CAP_LIST_OFFSET       0xc0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc0)

#define SMN_DEV1_FUNC0_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101480c0UL

#define SMN_DEV2_FUNC0_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101500c0UL


/***********************************************************
* Register Name : MSIX_MSG_CNTL
* Register Description :
* MSI-X Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the MSI-X Table Size (minus one).
#define MSIX_MSG_CNTL_MSIX_TABLE_SIZE_OFFSET      0
#define MSIX_MSG_CNTL_MSIX_TABLE_SIZE_MASK        0x7ff

// Bitfield Description : 
#define MSIX_MSG_CNTL_Reserved_13_11_OFFSET      11
#define MSIX_MSG_CNTL_Reserved_13_11_MASK        0x3800

// Bitfield Description : Controls if all of the vectors associated with the function are masked, regardless of their per-vector Mask bit values.
#define MSIX_MSG_CNTL_MSIX_FUNC_MASK_OFFSET      14
#define MSIX_MSG_CNTL_MSIX_FUNC_MASK_MASK        0x4000

// Bitfield Description : When MSI_EN bit is clear, controls the ability of a function to generate MSI-X requests.
#define MSIX_MSG_CNTL_MSIX_EN_OFFSET      15
#define MSIX_MSG_CNTL_MSIX_EN_MASK        0x8000

typedef union {
  struct {
    UINT16                            MSIX_TABLE_SIZE:11;
    UINT16                            Reserved_13_11:3;
    UINT16                            MSIX_FUNC_MASK:1;
    UINT16                            MSIX_EN:1;
  } Field;
  UINT16 Value;
} MSIX_MSG_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_MSG_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_MSG_CNTL_OFFSET       0xc2
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc2)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_MSG_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_MSG_CNTL_OFFSET       0xc2
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc2)

#define SMN_DEV1_FUNC0_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101480c2UL

#define SMN_DEV2_FUNC0_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101500c2UL


/***********************************************************
* Register Name : MSIX_PBA
* Register Description :
* MSI-X PBA Offset and PBA BIR register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the BAR whose allocated address range contains the MSI-X PBA.
#define MSIX_PBA_MSIX_PBA_BIR_OFFSET      0
#define MSIX_PBA_MSIX_PBA_BIR_MASK        0x7

// Bitfield Description : Indicates the offset from the address value contained in the BAR defined by MSIX_PBA_BIR field, to point to the start of the MSI-X PBA. The offset value must be padded with 3 bits of zero, to form a QWORD-aligned offset.
#define MSIX_PBA_MSIX_PBA_OFFSET_OFFSET      3
#define MSIX_PBA_MSIX_PBA_OFFSET_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            MSIX_PBA_BIR:3;
    UINT32                            MSIX_PBA_OFFSET:29;
  } Field;
  UINT32 Value;
} MSIX_PBA_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_PBA_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_PBA_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_PBA_OFFSET       0xc8
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_PBA_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc8)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_PBA_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_PBA_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_PBA_OFFSET       0xc8
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_PBA_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc8)

#define SMN_DEV1_FUNC0_NBIF0_MSIX_PBA_ADDRESS    0x101480c8UL

#define SMN_DEV2_FUNC0_NBIF0_MSIX_PBA_ADDRESS    0x101500c8UL


/***********************************************************
* Register Name : MSIX_TABLE
* Register Description :
* MSI-X Table Offset and Table BIR register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the BAR whose allocated address range contains the MSI-X Table.
#define MSIX_TABLE_MSIX_TABLE_BIR_OFFSET      0
#define MSIX_TABLE_MSIX_TABLE_BIR_MASK        0x7

// Bitfield Description : Indicates the offset from the address value contained in the BAR defined by MSIX_TABLE_BIR field, to point to the start of the MSI-X Table. The offset value must be padded with 3 bits of zero, to form a QWORD-aligned offset.
#define MSIX_TABLE_MSIX_TABLE_OFFSET_OFFSET      3
#define MSIX_TABLE_MSIX_TABLE_OFFSET_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            MSIX_TABLE_BIR:3;
    UINT32                            MSIX_TABLE_OFFSET:29;
  } Field;
  UINT32 Value;
} MSIX_TABLE_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_TABLE_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_TABLE_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_TABLE_OFFSET       0xc4
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSIX_TABLE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc4)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_TABLE_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_TABLE_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_TABLE_OFFSET       0xc4
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSIX_TABLE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xc4)

#define SMN_DEV1_FUNC0_NBIF0_MSIX_TABLE_ADDRESS    0x101480c4UL

#define SMN_DEV2_FUNC0_NBIF0_MSIX_TABLE_ADDRESS    0x101500c4UL


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
} MSI_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_CAP_LIST_OFFSET       0xa0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa0)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_CAP_LIST_OFFSET       0xa0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa0)

#define SMN_DEV1_FUNC0_NBIF0_MSI_CAP_LIST_ADDRESS    0x101480a0UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_CAP_LIST_ADDRESS    0x101500a0UL


/***********************************************************
* Register Name : MSI_EXT_MSG_DATA
* Register Description :
* Extended Message Data register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 16 bits for Message Data.
#define MSI_EXT_MSG_DATA_MSI_EXT_DATA_OFFSET      0
#define MSI_EXT_MSG_DATA_MSI_EXT_DATA_MASK        0xffff

typedef union {
  struct {
    UINT16                            MSI_EXT_DATA:16;
  } Field;
  UINT16 Value;
} MSI_EXT_MSG_DATA_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_EXT_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_EXT_MSG_DATA_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_EXT_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_EXT_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_EXT_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_EXT_MSG_DATA_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_EXT_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_EXT_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define SMN_DEV1_FUNC0_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL


/***********************************************************
* Register Name : MSI_EXT_MSG_DATA_64
* Register Description :
* Extended Message Data register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Upper 16 bits for Message Data.
#define MSI_EXT_MSG_DATA_64_MSI_EXT_DATA_64_OFFSET      0
#define MSI_EXT_MSG_DATA_64_MSI_EXT_DATA_64_MASK        0xffff

typedef union {
  struct {
    UINT16                            MSI_EXT_DATA_64:16;
  } Field;
  UINT16 Value;
} MSI_EXT_MSG_DATA_64_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_EXT_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_EXT_MSG_DATA_64_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_EXT_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x1)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_EXT_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_EXT_MSG_DATA_64_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_EXT_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x1)

#define SMN_DEV1_FUNC0_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL


/***********************************************************
* Register Name : MSI_MASK
* Register Description :
* MSI Mask bits register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the function can generate a specific MSI vector. This field is a bit vector that covers all supported vectors.
#define MSI_MASK_MSI_MASK_OFFSET      0
#define MSI_MASK_MSI_MASK_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MSI_MASK:32;
  } Field;
  UINT32 Value;
} MSI_MASK_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MASK_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MASK_OFFSET       0x0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MASK_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MASK_OFFSET       0x0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define SMN_DEV1_FUNC0_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_MASK_ADDRESS    0x10100000UL


/***********************************************************
* Register Name : MSI_MASK_64
* Register Description :
* MSI Mask bits register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the function can generate a specific MSI vector. This field is a bit vector that covers all supported vectors.
#define MSI_MASK_64_MSI_MASK_64_OFFSET      0
#define MSI_MASK_64_MSI_MASK_64_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MSI_MASK_64:32;
  } Field;
  UINT32 Value;
} MSI_MASK_64_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MASK_64_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MASK_64_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MASK_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MASK_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x1)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MASK_64_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MASK_64_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MASK_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MASK_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x1)

#define SMN_DEV1_FUNC0_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL


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
} MSI_MSG_ADDR_HI_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_ADDR_HI_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_ADDR_HI_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_ADDR_HI_OFFSET       0x1
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_ADDR_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x1)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_ADDR_HI_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_ADDR_HI_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_ADDR_HI_OFFSET       0x1
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_ADDR_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x1)

#define SMN_DEV1_FUNC0_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL


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
} MSI_MSG_ADDR_LO_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_ADDR_LO_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_ADDR_LO_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_ADDR_LO_OFFSET       0xa4
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_ADDR_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa4)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_ADDR_LO_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_ADDR_LO_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_ADDR_LO_OFFSET       0xa4
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_ADDR_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa4)

#define SMN_DEV1_FUNC0_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101480a4UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101500a4UL


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
} MSI_MSG_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_CNTL_OFFSET       0xa2
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa2)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_CNTL_OFFSET       0xa2
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa2)

#define SMN_DEV1_FUNC0_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101480a2UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101500a2UL


/***********************************************************
* Register Name : MSI_MSG_DATA
* Register Description :
* Message Data register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 16 bits for Message Data.
#define MSI_MSG_DATA_MSI_DATA_OFFSET      0
#define MSI_MSG_DATA_MSI_DATA_MASK        0xffff

typedef union {
  struct {
    UINT16                            MSI_DATA:16;
  } Field;
  UINT16 Value;
} MSI_MSG_DATA_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_DATA_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_DATA_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define SMN_DEV1_FUNC0_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL


/***********************************************************
* Register Name : MSI_MSG_DATA_64
* Register Description :
* Message Data register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Lower 16 bits for Message Data.
#define MSI_MSG_DATA_64_MSI_DATA_64_OFFSET      0
#define MSI_MSG_DATA_64_MSI_DATA_64_MASK        0xffff

typedef union {
  struct {
    UINT16                            MSI_DATA_64:16;
  } Field;
  UINT16 Value;
} MSI_MSG_DATA_64_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_DATA_64_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x1)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_DATA_64_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x1)

#define SMN_DEV1_FUNC0_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL


/***********************************************************
* Register Name : MSI_PENDING
* Register Description :
* MSI Pending bits register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if a specific MSI vector is pending internally in the function. This field is a bit vector that covers all supported vectors.
#define MSI_PENDING_MSI_PENDING_OFFSET      0
#define MSI_PENDING_MSI_PENDING_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MSI_PENDING:32;
  } Field;
  UINT32 Value;
} MSI_PENDING_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_PENDING_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_PENDING_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_PENDING_OFFSET       0x0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_PENDING_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_PENDING_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_PENDING_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_PENDING_OFFSET       0x0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_PENDING_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define SMN_DEV1_FUNC0_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL


/***********************************************************
* Register Name : MSI_PENDING_64
* Register Description :
* MSI Pending bits register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if a specific MSI vector is pending internally in the function. This field is a bit vector that covers all supported vectors.
#define MSI_PENDING_64_MSI_PENDING_64_OFFSET      0
#define MSI_PENDING_64_MSI_PENDING_64_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            MSI_PENDING_64:32;
  } Field;
  UINT32 Value;
} MSI_PENDING_64_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_PENDING_64_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_PENDING_64_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_PENDING_64_OFFSET       0xb4
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_MSI_PENDING_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xb4)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_PENDING_64_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_PENDING_64_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_PENDING_64_OFFSET       0xb4
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_MSI_PENDING_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xb4)

#define SMN_DEV1_FUNC0_NBIF0_MSI_PENDING_64_ADDRESS    0x101480b4UL

#define SMN_DEV2_FUNC0_NBIF0_MSI_PENDING_64_ADDRESS    0x101500b4UL


/***********************************************************
* Register Name : PCIE_ACS_CAP
* Register Description :
* ACS Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates that the component implements ACS Source Validation.
#define NBIF_ACS_CAP_SOURCE_VALIDATION_OFFSET      0
#define NBIF_ACS_CAP_SOURCE_VALIDATION_MASK        0x1

// Bitfield Description : Indicates that the component implements ACS Translation Blocking.
#define NBIF_ACS_CAP_TRANSLATION_BLOCKING_OFFSET      1
#define NBIF_ACS_CAP_TRANSLATION_BLOCKING_MASK        0x2

// Bitfield Description : Indicates that the component implements ACS P2P Request Redirect.
#define NBIF_ACS_CAP_P2P_REQUEST_REDIRECT_OFFSET      2
#define NBIF_ACS_CAP_P2P_REQUEST_REDIRECT_MASK        0x4

// Bitfield Description : Indicates that the component implements ACS P2P Completion Redirect.
#define NBIF_ACS_CAP_P2P_COMPLETION_REDIRECT_OFFSET      3
#define NBIF_ACS_CAP_P2P_COMPLETION_REDIRECT_MASK        0x8

// Bitfield Description : Indicates that the component implements ACS Upstream Forwarding.
#define NBIF_ACS_CAP_UPSTREAM_FORWARDING_OFFSET      4
#define NBIF_ACS_CAP_UPSTREAM_FORWARDING_MASK        0x10

// Bitfield Description : Indicates that the component implements ACS P2P Egress Control.
#define NBIF_ACS_CAP_P2P_EGRESS_CONTROL_OFFSET      5
#define NBIF_ACS_CAP_P2P_EGRESS_CONTROL_MASK        0x20

// Bitfield Description : Indicates that the component implements ACS Direct Translated P2P.
#define NBIF_ACS_CAP_DIRECT_TRANSLATED_P2P_OFFSET      6
#define NBIF_ACS_CAP_DIRECT_TRANSLATED_P2P_MASK        0x40

// Bitfield Description : 
#define NBIF_ACS_CAP_Reserved_7_7_OFFSET      7
#define NBIF_ACS_CAP_Reserved_7_7_MASK        0x80

// Bitfield Description : When ACS P2P Egress Control bit is set, a non-zero value indicates the number of bits in the Egress Control Vector, while a value of 0 indicates 256 bits in the Egress Control Vector.
#define NBIF_ACS_CAP_EGRESS_CONTROL_VECTOR_SIZE_OFFSET      8
#define NBIF_ACS_CAP_EGRESS_CONTROL_VECTOR_SIZE_MASK        0xff00

typedef union {
  struct {
    UINT16                            SOURCE_VALIDATION:1;
    UINT16                            TRANSLATION_BLOCKING:1;
    UINT16                            P2P_REQUEST_REDIRECT:1;
    UINT16                            P2P_COMPLETION_REDIRECT:1;
    UINT16                            UPSTREAM_FORWARDING:1;
    UINT16                            P2P_EGRESS_CONTROL:1;
    UINT16                            DIRECT_TRANSLATED_P2P:1;
    UINT16                            Reserved_7_7:1;
    UINT16                            EGRESS_CONTROL_VECTOR_SIZE:8;
  } Field;
  UINT16 Value;
} PCIE_ACS_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_CAP_OFFSET       0x2a4
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2a4)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_CAP_OFFSET       0x2a4
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2a4)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101482a4UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101502a4UL


/***********************************************************
* Register Name : PCIE_ACS_CNTL
* Register Description :
* ACS Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if the component validates the Bus Number from the Requester ID of Upstream Requests against the secondary or subordinate Bus Numbers.
#define NBIF_ACS_CNTL_SOURCE_VALIDATION_EN_OFFSET      0
#define NBIF_ACS_CNTL_SOURCE_VALIDATION_EN_MASK        0x1

// Bitfield Description : Controls if the component blocks all Upstream Memory Requests whose Address Translation field is not set to the default value.
#define NBIF_ACS_CNTL_TRANSLATION_BLOCKING_EN_OFFSET      1
#define NBIF_ACS_CNTL_TRANSLATION_BLOCKING_EN_MASK        0x2

// Bitfield Description : In conjunction with ACS P2P Egress Control and ACS Direct Translated P2P mechanisms, controls if the component redirects P2P Requests Upstream.
#define NBIF_ACS_CNTL_P2P_REQUEST_REDIRECT_EN_OFFSET      2
#define NBIF_ACS_CNTL_P2P_REQUEST_REDIRECT_EN_MASK        0x4

// Bitfield Description : Controls if the component redirects P2P Completions Upstream; applicable only to Read Completions whose Relaxed Ordering Attribute is clear.
#define NBIF_ACS_CNTL_P2P_COMPLETION_REDIRECT_EN_OFFSET      3
#define NBIF_ACS_CNTL_P2P_COMPLETION_REDIRECT_EN_MASK        0x8

// Bitfield Description : Controls if the component forwards Upstream any Request or Completion TLPs it receives that were redirected Upstream by a component lower in the hierarchy.
#define NBIF_ACS_CNTL_UPSTREAM_FORWARDING_EN_OFFSET      4
#define NBIF_ACS_CNTL_UPSTREAM_FORWARDING_EN_MASK        0x10

// Bitfield Description : In conjunction with the Egress Control Vector plus the ACS P2P Request Redirect and ACS Direct Translated P2P mechanisms, controls when to allow, disallow, or redirect P2P Requests.
#define NBIF_ACS_CNTL_P2P_EGRESS_CONTROL_EN_OFFSET      5
#define NBIF_ACS_CNTL_P2P_EGRESS_CONTROL_EN_MASK        0x20

// Bitfield Description : Controls if the component overrides the ACS P2P Request Redirect and ACS P2P Egress Control mechanisms with P2P Memory Requests whose Address Translation field indicates a Translated address.
#define NBIF_ACS_CNTL_DIRECT_TRANSLATED_P2P_EN_OFFSET      6
#define NBIF_ACS_CNTL_DIRECT_TRANSLATED_P2P_EN_MASK        0x40

// Bitfield Description : 
#define NBIF_ACS_CNTL_Reserved_15_7_OFFSET      7
#define NBIF_ACS_CNTL_Reserved_15_7_MASK        0xff80

typedef union {
  struct {
    UINT16                            SOURCE_VALIDATION_EN:1;
    UINT16                            TRANSLATION_BLOCKING_EN:1;
    UINT16                            P2P_REQUEST_REDIRECT_EN:1;
    UINT16                            P2P_COMPLETION_REDIRECT_EN:1;
    UINT16                            UPSTREAM_FORWARDING_EN:1;
    UINT16                            P2P_EGRESS_CONTROL_EN:1;
    UINT16                            DIRECT_TRANSLATED_P2P_EN:1;
    UINT16                            Reserved_15_7:9;
  } Field;
  UINT16 Value;
} PCIE_ACS_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_CNTL_OFFSET       0x2a6
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2a6)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_CNTL_OFFSET       0x2a6
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2a6)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101482a6UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101502a6UL


/***********************************************************
* Register Name : PCIE_ACS_ENH_CAP_LIST
* Register Description :
* ACS Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_ACS_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_ACS_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_ACS_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_ACS_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_ACS_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_ACS_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_ACS_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_ENH_CAP_LIST_OFFSET       0x2a0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2a0)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_ENH_CAP_LIST_OFFSET       0x2a0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2a0)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101482a0UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101502a0UL


/***********************************************************
* Register Name : PCIE_ADV_ERR_CAP_CNTL
* Register Description :
* Advanced Error Capabilities and Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the bit position of the first error reported in the Uncorrectable Error Status register. This field is only valid if the indicated bit is also set. This field is sticky.
#define NBIF_ADV_ERR_CAP_CNTL_FIRST_ERR_PTR_OFFSET      0
#define NBIF_ADV_ERR_CAP_CNTL_FIRST_ERR_PTR_MASK        0x1f

// Bitfield Description : Indicates if the function is capable of generating ECRC.
#define NBIF_ADV_ERR_CAP_CNTL_ECRC_GEN_CAP_OFFSET      5
#define NBIF_ADV_ERR_CAP_CNTL_ECRC_GEN_CAP_MASK        0x20

// Bitfield Description : Controls if the function generates ECRC. This bit is sticky.
#define NBIF_ADV_ERR_CAP_CNTL_ECRC_GEN_EN_OFFSET      6
#define NBIF_ADV_ERR_CAP_CNTL_ECRC_GEN_EN_MASK        0x40

// Bitfield Description : Indicates if the function is capable of checking ECRC.
#define NBIF_ADV_ERR_CAP_CNTL_ECRC_CHECK_CAP_OFFSET      7
#define NBIF_ADV_ERR_CAP_CNTL_ECRC_CHECK_CAP_MASK        0x80

// Bitfield Description : Controls if the function checks ECRC. This bit is sticky.
#define NBIF_ADV_ERR_CAP_CNTL_ECRC_CHECK_EN_OFFSET      8
#define NBIF_ADV_ERR_CAP_CNTL_ECRC_CHECK_EN_MASK        0x100

// Bitfield Description : Indicates if the function is capable of recording more than one error header.
#define NBIF_ADV_ERR_CAP_CNTL_MULTI_HDR_RECD_CAP_OFFSET      9
#define NBIF_ADV_ERR_CAP_CNTL_MULTI_HDR_RECD_CAP_MASK        0x200

// Bitfield Description : Controls if the function is permitted to record more than one error header.
#define NBIF_ADV_ERR_CAP_CNTL_MULTI_HDR_RECD_EN_OFFSET      10
#define NBIF_ADV_ERR_CAP_CNTL_MULTI_HDR_RECD_EN_MASK        0x400

// Bitfield Description : When the First Error Pointer is valid, indicates that the TLP Prefix Log register contains valid information. This bit is sticky. If End-End TLP Prefix Supported bit is not set, this bit is Reserved.
#define NBIF_ADV_ERR_CAP_CNTL_TLP_PREFIX_LOG_PRESENT_OFFSET      11
#define NBIF_ADV_ERR_CAP_CNTL_TLP_PREFIX_LOG_PRESENT_MASK        0x800

// Bitfield Description : Indicates if the function is capable of recording the TLP Prefix (if any) and the Header for the TLP corresponding to a detected Completion Timeout error.
#define NBIF_ADV_ERR_CAP_CNTL_COMPLETION_TIMEOUT_LOG_CAPABLE_OFFSET      12
#define NBIF_ADV_ERR_CAP_CNTL_COMPLETION_TIMEOUT_LOG_CAPABLE_MASK        0x1000

// Bitfield Description : 
#define NBIF_ADV_ERR_CAP_CNTL_Reserved_31_13_OFFSET      13
#define NBIF_ADV_ERR_CAP_CNTL_Reserved_31_13_MASK        0xffffe000

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
} PCIE_ADV_ERR_CAP_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_OFFSET       0x168
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x168)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ADV_ERR_CAP_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ADV_ERR_CAP_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ADV_ERR_CAP_CNTL_OFFSET       0x168
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x168)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10148168UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10150168UL


/***********************************************************
* Register Name : PCIE_ADV_ERR_RPT_ENH_CAP_LIST
* Register Description :
* Advanced Error Reporting Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_ADV_ERR_RPT_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_ADV_ERR_RPT_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_ADV_ERR_RPT_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_ADV_ERR_RPT_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_ADV_ERR_RPT_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_ADV_ERR_RPT_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_ADV_ERR_RPT_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_OFFSET       0x150
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x150)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_OFFSET       0x150
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x150)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10148150UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10150150UL


/***********************************************************
* Register Name : PCIE_ARI_CAP
* Register Description :
* ARI Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : For Function 0 only, indicates if the device supports Function Group level arbitration via its MFVC capability.
#define NBIF_ARI_CAP_ARI_MFVC_FUNC_GROUPS_CAP_OFFSET      0
#define NBIF_ARI_CAP_ARI_MFVC_FUNC_GROUPS_CAP_MASK        0x1

// Bitfield Description : For Function 0 only, indicates if the device supports Function Group level granularity for ACS P2P Egress Control.
#define NBIF_ARI_CAP_ARI_ACS_FUNC_GROUPS_CAP_OFFSET      1
#define NBIF_ARI_CAP_ARI_ACS_FUNC_GROUPS_CAP_MASK        0x2

// Bitfield Description : 
#define NBIF_ARI_CAP_Reserved_7_2_OFFSET      2
#define NBIF_ARI_CAP_Reserved_7_2_MASK        0xfc

// Bitfield Description : Indicates the next function number in the device, or 0 if this is the last function.
#define NBIF_ARI_CAP_ARI_NEXT_FUNC_NUM_OFFSET      8
#define NBIF_ARI_CAP_ARI_NEXT_FUNC_NUM_MASK        0xff00

typedef union {
  struct {
    UINT16                            ARI_MFVC_FUNC_GROUPS_CAP:1;
    UINT16                            ARI_ACS_FUNC_GROUPS_CAP:1;
    UINT16                            Reserved_7_2:6;
    UINT16                            ARI_NEXT_FUNC_NUM:8;
  } Field;
  UINT16 Value;
} PCIE_ARI_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_CAP_OFFSET       0x32c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x32c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_CAP_OFFSET       0x32c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x32c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1014832cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1015032cUL


/***********************************************************
* Register Name : PCIE_ARI_CNTL
* Register Description :
* ARI Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : For Function 0 only, controls if the device interprets entries in its Function Arbitration table as Function Group numbers.
#define NBIF_ARI_CNTL_ARI_MFVC_FUNC_GROUPS_EN_OFFSET      0
#define NBIF_ARI_CNTL_ARI_MFVC_FUNC_GROUPS_EN_MASK        0x1

// Bitfield Description : For Function 0 only, controls if the device associates bits in its Egress Control Vector with Function Group numbers.
#define NBIF_ARI_CNTL_ARI_ACS_FUNC_GROUPS_EN_OFFSET      1
#define NBIF_ARI_CNTL_ARI_ACS_FUNC_GROUPS_EN_MASK        0x2

// Bitfield Description : 
#define NBIF_ARI_CNTL_Reserved_3_2_OFFSET      2
#define NBIF_ARI_CNTL_Reserved_3_2_MASK        0xc

// Bitfield Description : Controls the Function Group number assigned to this function.
#define NBIF_ARI_CNTL_ARI_FUNCTION_GROUP_OFFSET      4
#define NBIF_ARI_CNTL_ARI_FUNCTION_GROUP_MASK        0x70

// Bitfield Description : 
#define NBIF_ARI_CNTL_Reserved_15_7_OFFSET      7
#define NBIF_ARI_CNTL_Reserved_15_7_MASK        0xff80

typedef union {
  struct {
    UINT16                            ARI_MFVC_FUNC_GROUPS_EN:1;
    UINT16                            ARI_ACS_FUNC_GROUPS_EN:1;
    UINT16                            Reserved_3_2:2;
    UINT16                            ARI_FUNCTION_GROUP:3;
    UINT16                            Reserved_15_7:9;
  } Field;
  UINT16 Value;
} PCIE_ARI_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_CNTL_OFFSET       0x32e
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x32e)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_CNTL_OFFSET       0x32e
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x32e)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1014832eUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1015032eUL


/***********************************************************
* Register Name : PCIE_ARI_ENH_CAP_LIST
* Register Description :
* ARI Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_ARI_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_ARI_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_ARI_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_ARI_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_ARI_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_ARI_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_ARI_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_ENH_CAP_LIST_OFFSET       0x328
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x328)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_ENH_CAP_LIST_OFFSET       0x328
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x328)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10148328UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10150328UL


/***********************************************************
* Register Name : PCIE_BAR1_CAP
* Register Description :
* Resizable BAR1 Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NBIF_BAR1_CAP_Reserved_3_0_OFFSET      0
#define NBIF_BAR1_CAP_Reserved_3_0_MASK        0xf

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 1 MB to 128 TB.
#define NBIF_BAR1_CAP_BAR_SIZE_SUPPORTED_OFFSET      4
#define NBIF_BAR1_CAP_BAR_SIZE_SUPPORTED_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            BAR_SIZE_SUPPORTED:28;
  } Field;
  UINT32 Value;
} PCIE_BAR1_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR1_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR1_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR1_CAP_OFFSET       0x204
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR1_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x204)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR1_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR1_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR1_CAP_OFFSET       0x204
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR1_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x204)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10148204UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10150204UL


/***********************************************************
* Register Name : PCIE_BAR1_CNTL
* Register Description :
* Resizable BAR1 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the BAR controlled by this register.
#define NBIF_BAR1_CNTL_BAR_INDEX_OFFSET      0
#define NBIF_BAR1_CNTL_BAR_INDEX_MASK        0x7

// Bitfield Description : 
#define NBIF_BAR1_CNTL_Reserved_4_3_OFFSET      3
#define NBIF_BAR1_CNTL_Reserved_4_3_MASK        0x18

// Bitfield Description : Indicates the total number of resizable BARs in the capability structure for the function.
#define NBIF_BAR1_CNTL_BAR_TOTAL_NUM_OFFSET      5
#define NBIF_BAR1_CNTL_BAR_TOTAL_NUM_MASK        0xe0

// Bitfield Description : Controls the size of the address space that the BAR resource is requesting. The encoded values range from 0 (1 MB) to 43 (8 EB), with each incremented value indicating double the previous size.
#define NBIF_BAR1_CNTL_BAR_SIZE_OFFSET      8
#define NBIF_BAR1_CNTL_BAR_SIZE_MASK        0x3f00

// Bitfield Description : 
#define NBIF_BAR1_CNTL_Reserved_15_14_OFFSET      14
#define NBIF_BAR1_CNTL_Reserved_15_14_MASK        0xc000

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 256 TB to 8 EB.
#define NBIF_BAR1_CNTL_BAR_SIZE_SUPPORTED_UPPER_OFFSET      16
#define NBIF_BAR1_CNTL_BAR_SIZE_SUPPORTED_UPPER_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            BAR_INDEX:3;
    UINT32                            Reserved_4_3:2;
    UINT32                            BAR_TOTAL_NUM:3;
    UINT32                            BAR_SIZE:6;
    UINT32                            Reserved_15_14:2;
    UINT32                            BAR_SIZE_SUPPORTED_UPPER:16;
  } Field;
  UINT32 Value;
} PCIE_BAR1_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR1_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR1_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR1_CNTL_OFFSET       0x208
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR1_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x208)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR1_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR1_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR1_CNTL_OFFSET       0x208
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR1_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x208)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10148208UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10150208UL


/***********************************************************
* Register Name : PCIE_BAR2_CAP
* Register Description :
* Resizable BAR2 Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NBIF_BAR2_CAP_Reserved_3_0_OFFSET      0
#define NBIF_BAR2_CAP_Reserved_3_0_MASK        0xf

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 1 MB to 128 TB.
#define NBIF_BAR2_CAP_BAR_SIZE_SUPPORTED_OFFSET      4
#define NBIF_BAR2_CAP_BAR_SIZE_SUPPORTED_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            BAR_SIZE_SUPPORTED:28;
  } Field;
  UINT32 Value;
} PCIE_BAR2_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR2_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR2_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR2_CAP_OFFSET       0x20c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR2_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x20c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR2_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR2_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR2_CAP_OFFSET       0x20c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR2_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x20c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1014820cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1015020cUL


/***********************************************************
* Register Name : PCIE_BAR2_CNTL
* Register Description :
* Resizable BAR2 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the BAR controlled by this register. It has the same encoding as shown for Resizable BAR1 Control register.
#define NBIF_BAR2_CNTL_BAR_INDEX_OFFSET      0
#define NBIF_BAR2_CNTL_BAR_INDEX_MASK        0x7

// Bitfield Description : 
#define NBIF_BAR2_CNTL_Reserved_4_3_OFFSET      3
#define NBIF_BAR2_CNTL_Reserved_4_3_MASK        0x18

// Bitfield Description : Reserved.
#define NBIF_BAR2_CNTL_BAR_TOTAL_NUM_OFFSET      5
#define NBIF_BAR2_CNTL_BAR_TOTAL_NUM_MASK        0xe0

// Bitfield Description : Controls the size of the address space that the BAR resource is requesting. The encoded values range from 0 (1 MB) to 43 (8 EB), with each incremented value indicating double the previous size.
#define NBIF_BAR2_CNTL_BAR_SIZE_OFFSET      8
#define NBIF_BAR2_CNTL_BAR_SIZE_MASK        0x3f00

// Bitfield Description : 
#define NBIF_BAR2_CNTL_Reserved_15_14_OFFSET      14
#define NBIF_BAR2_CNTL_Reserved_15_14_MASK        0xc000

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 256 TB to 8 EB.
#define NBIF_BAR2_CNTL_BAR_SIZE_SUPPORTED_UPPER_OFFSET      16
#define NBIF_BAR2_CNTL_BAR_SIZE_SUPPORTED_UPPER_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            BAR_INDEX:3;
    UINT32                            Reserved_4_3:2;
    UINT32                            BAR_TOTAL_NUM:3;
    UINT32                            BAR_SIZE:6;
    UINT32                            Reserved_15_14:2;
    UINT32                            BAR_SIZE_SUPPORTED_UPPER:16;
  } Field;
  UINT32 Value;
} PCIE_BAR2_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR2_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR2_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR2_CNTL_OFFSET       0x210
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR2_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x210)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR2_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR2_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR2_CNTL_OFFSET       0x210
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR2_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x210)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10148210UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10150210UL


/***********************************************************
* Register Name : PCIE_BAR3_CAP
* Register Description :
* Resizable BAR3 Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NBIF_BAR3_CAP_Reserved_3_0_OFFSET      0
#define NBIF_BAR3_CAP_Reserved_3_0_MASK        0xf

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 1 MB to 128 TB.
#define NBIF_BAR3_CAP_BAR_SIZE_SUPPORTED_OFFSET      4
#define NBIF_BAR3_CAP_BAR_SIZE_SUPPORTED_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            BAR_SIZE_SUPPORTED:28;
  } Field;
  UINT32 Value;
} PCIE_BAR3_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR3_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR3_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR3_CAP_OFFSET       0x214
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR3_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x214)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR3_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR3_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR3_CAP_OFFSET       0x214
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR3_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x214)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10148214UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10150214UL


/***********************************************************
* Register Name : PCIE_BAR3_CNTL
* Register Description :
* Resizable BAR3 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the BAR controlled by this register. It has the same encoding as shown for Resizable BAR1 Control register.
#define NBIF_BAR3_CNTL_BAR_INDEX_OFFSET      0
#define NBIF_BAR3_CNTL_BAR_INDEX_MASK        0x7

// Bitfield Description : 
#define NBIF_BAR3_CNTL_Reserved_4_3_OFFSET      3
#define NBIF_BAR3_CNTL_Reserved_4_3_MASK        0x18

// Bitfield Description : Reserved.
#define NBIF_BAR3_CNTL_BAR_TOTAL_NUM_OFFSET      5
#define NBIF_BAR3_CNTL_BAR_TOTAL_NUM_MASK        0xe0

// Bitfield Description : Controls the size of the address space that the BAR resource is requesting. The encoded values range from 0 (1 MB) to 43 (8 EB), with each incremented value indicating double the previous size.
#define NBIF_BAR3_CNTL_BAR_SIZE_OFFSET      8
#define NBIF_BAR3_CNTL_BAR_SIZE_MASK        0x3f00

// Bitfield Description : 
#define NBIF_BAR3_CNTL_Reserved_15_14_OFFSET      14
#define NBIF_BAR3_CNTL_Reserved_15_14_MASK        0xc000

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 256 TB to 8 EB.
#define NBIF_BAR3_CNTL_BAR_SIZE_SUPPORTED_UPPER_OFFSET      16
#define NBIF_BAR3_CNTL_BAR_SIZE_SUPPORTED_UPPER_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            BAR_INDEX:3;
    UINT32                            Reserved_4_3:2;
    UINT32                            BAR_TOTAL_NUM:3;
    UINT32                            BAR_SIZE:6;
    UINT32                            Reserved_15_14:2;
    UINT32                            BAR_SIZE_SUPPORTED_UPPER:16;
  } Field;
  UINT32 Value;
} PCIE_BAR3_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR3_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR3_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR3_CNTL_OFFSET       0x218
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR3_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x218)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR3_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR3_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR3_CNTL_OFFSET       0x218
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR3_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x218)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10148218UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10150218UL


/***********************************************************
* Register Name : PCIE_BAR4_CAP
* Register Description :
* Resizable BAR4 Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NBIF_BAR4_CAP_Reserved_3_0_OFFSET      0
#define NBIF_BAR4_CAP_Reserved_3_0_MASK        0xf

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 1 MB to 128 TB.
#define NBIF_BAR4_CAP_BAR_SIZE_SUPPORTED_OFFSET      4
#define NBIF_BAR4_CAP_BAR_SIZE_SUPPORTED_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            BAR_SIZE_SUPPORTED:28;
  } Field;
  UINT32 Value;
} PCIE_BAR4_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR4_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR4_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR4_CAP_OFFSET       0x21c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR4_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x21c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR4_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR4_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR4_CAP_OFFSET       0x21c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR4_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x21c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1014821cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1015021cUL


/***********************************************************
* Register Name : PCIE_BAR4_CNTL
* Register Description :
* Resizable BAR4 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the BAR controlled by this register. It has the same encoding as shown for Resizable BAR1 Control register.
#define NBIF_BAR4_CNTL_BAR_INDEX_OFFSET      0
#define NBIF_BAR4_CNTL_BAR_INDEX_MASK        0x7

// Bitfield Description : 
#define NBIF_BAR4_CNTL_Reserved_4_3_OFFSET      3
#define NBIF_BAR4_CNTL_Reserved_4_3_MASK        0x18

// Bitfield Description : Reserved.
#define NBIF_BAR4_CNTL_BAR_TOTAL_NUM_OFFSET      5
#define NBIF_BAR4_CNTL_BAR_TOTAL_NUM_MASK        0xe0

// Bitfield Description : Controls the size of the address space that the BAR resource is requesting. The encoded values range from 0 (1 MB) to 43 (8 EB), with each incremented value indicating double the previous size.
#define NBIF_BAR4_CNTL_BAR_SIZE_OFFSET      8
#define NBIF_BAR4_CNTL_BAR_SIZE_MASK        0x3f00

// Bitfield Description : 
#define NBIF_BAR4_CNTL_Reserved_15_14_OFFSET      14
#define NBIF_BAR4_CNTL_Reserved_15_14_MASK        0xc000

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 256 TB to 8 EB.
#define NBIF_BAR4_CNTL_BAR_SIZE_SUPPORTED_UPPER_OFFSET      16
#define NBIF_BAR4_CNTL_BAR_SIZE_SUPPORTED_UPPER_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            BAR_INDEX:3;
    UINT32                            Reserved_4_3:2;
    UINT32                            BAR_TOTAL_NUM:3;
    UINT32                            BAR_SIZE:6;
    UINT32                            Reserved_15_14:2;
    UINT32                            BAR_SIZE_SUPPORTED_UPPER:16;
  } Field;
  UINT32 Value;
} PCIE_BAR4_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR4_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR4_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR4_CNTL_OFFSET       0x220
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR4_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x220)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR4_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR4_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR4_CNTL_OFFSET       0x220
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR4_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x220)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10148220UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10150220UL


/***********************************************************
* Register Name : PCIE_BAR5_CAP
* Register Description :
* Resizable BAR5 Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NBIF_BAR5_CAP_Reserved_3_0_OFFSET      0
#define NBIF_BAR5_CAP_Reserved_3_0_MASK        0xf

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 1 MB to 128 TB.
#define NBIF_BAR5_CAP_BAR_SIZE_SUPPORTED_OFFSET      4
#define NBIF_BAR5_CAP_BAR_SIZE_SUPPORTED_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            BAR_SIZE_SUPPORTED:28;
  } Field;
  UINT32 Value;
} PCIE_BAR5_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR5_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR5_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR5_CAP_OFFSET       0x224
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR5_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x224)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR5_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR5_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR5_CAP_OFFSET       0x224
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR5_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x224)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10148224UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10150224UL


/***********************************************************
* Register Name : PCIE_BAR5_CNTL
* Register Description :
* Resizable BAR5 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the BAR controlled by this register. It has the same encoding as shown for Resizable BAR1 Control register.
#define NBIF_BAR5_CNTL_BAR_INDEX_OFFSET      0
#define NBIF_BAR5_CNTL_BAR_INDEX_MASK        0x7

// Bitfield Description : 
#define NBIF_BAR5_CNTL_Reserved_4_3_OFFSET      3
#define NBIF_BAR5_CNTL_Reserved_4_3_MASK        0x18

// Bitfield Description : Reserved.
#define NBIF_BAR5_CNTL_BAR_TOTAL_NUM_OFFSET      5
#define NBIF_BAR5_CNTL_BAR_TOTAL_NUM_MASK        0xe0

// Bitfield Description : Controls the size of the address space that the BAR resource is requesting. The encoded values range from 0 (1 MB) to 43 (8 EB), with each incremented value indicating double the previous size.
#define NBIF_BAR5_CNTL_BAR_SIZE_OFFSET      8
#define NBIF_BAR5_CNTL_BAR_SIZE_MASK        0x3f00

// Bitfield Description : 
#define NBIF_BAR5_CNTL_Reserved_15_14_OFFSET      14
#define NBIF_BAR5_CNTL_Reserved_15_14_MASK        0xc000

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 256 TB to 8 EB.
#define NBIF_BAR5_CNTL_BAR_SIZE_SUPPORTED_UPPER_OFFSET      16
#define NBIF_BAR5_CNTL_BAR_SIZE_SUPPORTED_UPPER_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            BAR_INDEX:3;
    UINT32                            Reserved_4_3:2;
    UINT32                            BAR_TOTAL_NUM:3;
    UINT32                            BAR_SIZE:6;
    UINT32                            Reserved_15_14:2;
    UINT32                            BAR_SIZE_SUPPORTED_UPPER:16;
  } Field;
  UINT32 Value;
} PCIE_BAR5_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR5_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR5_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR5_CNTL_OFFSET       0x228
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR5_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x228)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR5_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR5_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR5_CNTL_OFFSET       0x228
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR5_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x228)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10148228UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10150228UL


/***********************************************************
* Register Name : PCIE_BAR6_CAP
* Register Description :
* Resizable BAR6 Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NBIF_BAR6_CAP_Reserved_3_0_OFFSET      0
#define NBIF_BAR6_CAP_Reserved_3_0_MASK        0xf

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 1 MB to 128 TB.
#define NBIF_BAR6_CAP_BAR_SIZE_SUPPORTED_OFFSET      4
#define NBIF_BAR6_CAP_BAR_SIZE_SUPPORTED_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            Reserved_3_0:4;
    UINT32                            BAR_SIZE_SUPPORTED:28;
  } Field;
  UINT32 Value;
} PCIE_BAR6_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR6_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR6_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR6_CAP_OFFSET       0x22c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR6_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x22c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR6_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR6_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR6_CAP_OFFSET       0x22c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR6_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x22c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1014822cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1015022cUL


/***********************************************************
* Register Name : PCIE_BAR6_CNTL
* Register Description :
* Resizable BAR6 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the BAR controlled by this register. It has the same encoding as shown for Resizable BAR1 Control register.
#define NBIF_BAR6_CNTL_BAR_INDEX_OFFSET      0
#define NBIF_BAR6_CNTL_BAR_INDEX_MASK        0x7

// Bitfield Description : 
#define NBIF_BAR6_CNTL_Reserved_4_3_OFFSET      3
#define NBIF_BAR6_CNTL_Reserved_4_3_MASK        0x18

// Bitfield Description : Reserved.
#define NBIF_BAR6_CNTL_BAR_TOTAL_NUM_OFFSET      5
#define NBIF_BAR6_CNTL_BAR_TOTAL_NUM_MASK        0xe0

// Bitfield Description : Controls the size of the address space that the BAR resource is requesting. The encoded values range from 0 (1 MB) to 43 (8 EB), with each incremented value indicating double the previous size.
#define NBIF_BAR6_CNTL_BAR_SIZE_OFFSET      8
#define NBIF_BAR6_CNTL_BAR_SIZE_MASK        0x3f00

// Bitfield Description : 
#define NBIF_BAR6_CNTL_Reserved_15_14_OFFSET      14
#define NBIF_BAR6_CNTL_Reserved_15_14_MASK        0xc000

// Bitfield Description : Indicates that the function supports operating with the specified BAR sizes. This field is a bit vector that covers the size range of 256 TB to 8 EB.
#define NBIF_BAR6_CNTL_BAR_SIZE_SUPPORTED_UPPER_OFFSET      16
#define NBIF_BAR6_CNTL_BAR_SIZE_SUPPORTED_UPPER_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            BAR_INDEX:3;
    UINT32                            Reserved_4_3:2;
    UINT32                            BAR_TOTAL_NUM:3;
    UINT32                            BAR_SIZE:6;
    UINT32                            Reserved_15_14:2;
    UINT32                            BAR_SIZE_SUPPORTED_UPPER:16;
  } Field;
  UINT32 Value;
} PCIE_BAR6_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR6_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR6_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR6_CNTL_OFFSET       0x230
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR6_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x230)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR6_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR6_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR6_CNTL_OFFSET       0x230
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR6_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x230)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10148230UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10150230UL


/***********************************************************
* Register Name : PCIE_BAR_ENH_CAP_LIST
* Register Description :
* Resizable BAR Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_BAR_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_BAR_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_BAR_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_BAR_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_BAR_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_BAR_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_BAR_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR_ENH_CAP_LIST_OFFSET       0x200
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x200)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR_ENH_CAP_LIST_OFFSET       0x200
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x200)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10148200UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10150200UL


/***********************************************************
* Register Name : PCIE_CAP
* Register Description :
* The PCI Express Capabilities register identifies PCI Express device type and associated capabilities.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates PCI-SIG defined capability structure version number.
#define NBIF_CAP_VERSION_OFFSET      0
#define NBIF_CAP_VERSION_MASK        0xf

// Bitfield Description : Indicates the type of PCI Express device.
#define NBIF_CAP_DEVICE_TYPE_OFFSET      4
#define NBIF_CAP_DEVICE_TYPE_MASK        0xf0

// Bitfield Description : For Downstream Ports, indicates if the Link associated with this Port is connected to a slot. For Upstream Ports, this bit is undefined.
#define NBIF_CAP_SLOT_IMPLEMENTED_OFFSET      8
#define NBIF_CAP_SLOT_IMPLEMENTED_MASK        0x100

// Bitfield Description : Interrupt Message Number.
#define NBIF_CAP_INT_MESSAGE_NUM_OFFSET      9
#define NBIF_CAP_INT_MESSAGE_NUM_MASK        0x3e00

// Bitfield Description : 
#define NBIF_CAP_Reserved_15_14_OFFSET      14
#define NBIF_CAP_Reserved_15_14_MASK        0xc000

typedef union {
  struct {
    UINT16                            VERSION:4;
    UINT16                            DEVICE_TYPE:4;
    UINT16                            SLOT_IMPLEMENTED:1;
    UINT16                            INT_MESSAGE_NUM:5;
    UINT16                            Reserved_15_14:2;
  } Field;
  UINT16 Value;
} PCIE_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CAP_OFFSET       0x66
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x66)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CAP_OFFSET       0x66
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x66)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_CAP_ADDRESS    0x10148066UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_CAP_ADDRESS    0x10150066UL


/***********************************************************
* Register Name : PCIE_CAP_LIST
* Register Description :
* PCI Express Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the capability.
#define NBIF_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_CAP_LIST_CAP_ID_MASK        0xff

// Bitfield Description : Pointer to the next item in the capabilities list, or 00h if no other items exist.
#define NBIF_CAP_LIST_NEXT_PTR_OFFSET      8
#define NBIF_CAP_LIST_NEXT_PTR_MASK        0xff00

typedef union {
  struct {
    UINT16                            CAP_ID:8;
    UINT16                            NEXT_PTR:8;
  } Field;
  UINT16 Value;
} PCIE_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CAP_LIST_OFFSET       0x64
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x64)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CAP_LIST_OFFSET       0x64
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x64)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10148064UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10150064UL


/***********************************************************
* Register Name : PCIE_CORR_ERR_MASK
* Register Description :
* The Correctable Error Mask register controls reporting of individual correctable errors by a function.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Receiver Error Mask.
#define NBIF_CORR_ERR_MASK_RCV_ERR_MASK_OFFSET      0
#define NBIF_CORR_ERR_MASK_RCV_ERR_MASK_MASK        0x1

// Bitfield Description : 
#define NBIF_CORR_ERR_MASK_Reserved_5_1_OFFSET      1
#define NBIF_CORR_ERR_MASK_Reserved_5_1_MASK        0x3e

// Bitfield Description : Bad TLP Mask.
#define NBIF_CORR_ERR_MASK_BAD_TLP_MASK_OFFSET      6
#define NBIF_CORR_ERR_MASK_BAD_TLP_MASK_MASK        0x40

// Bitfield Description : Bad DLLP Mask.
#define NBIF_CORR_ERR_MASK_BAD_DLLP_MASK_OFFSET      7
#define NBIF_CORR_ERR_MASK_BAD_DLLP_MASK_MASK        0x80

// Bitfield Description : REPLAY_NUM Rollover Mask.
#define NBIF_CORR_ERR_MASK_REPLAY_NUM_ROLLOVER_MASK_OFFSET      8
#define NBIF_CORR_ERR_MASK_REPLAY_NUM_ROLLOVER_MASK_MASK        0x100

// Bitfield Description : 
#define NBIF_CORR_ERR_MASK_Reserved_11_9_OFFSET      9
#define NBIF_CORR_ERR_MASK_Reserved_11_9_MASK        0xe00

// Bitfield Description : Replay Timer Timeout Mask.
#define NBIF_CORR_ERR_MASK_REPLAY_TIMER_TIMEOUT_MASK_OFFSET      12
#define NBIF_CORR_ERR_MASK_REPLAY_TIMER_TIMEOUT_MASK_MASK        0x1000

// Bitfield Description : Advisory Non-Fatal Mask.
#define NBIF_CORR_ERR_MASK_ADVISORY_NONFATAL_ERR_MASK_OFFSET      13
#define NBIF_CORR_ERR_MASK_ADVISORY_NONFATAL_ERR_MASK_MASK        0x2000

// Bitfield Description : Controls if the function can report a Corrected Internal Error. This bit is sticky.
#define NBIF_CORR_ERR_MASK_CORR_INT_ERR_MASK_OFFSET      14
#define NBIF_CORR_ERR_MASK_CORR_INT_ERR_MASK_MASK        0x4000

// Bitfield Description : Controls if the function can report a Header Log Overflow Error.
#define NBIF_CORR_ERR_MASK_HDR_LOG_OVFL_MASK_OFFSET      15
#define NBIF_CORR_ERR_MASK_HDR_LOG_OVFL_MASK_MASK        0x8000

// Bitfield Description : 
#define NBIF_CORR_ERR_MASK_Reserved_31_16_OFFSET      16
#define NBIF_CORR_ERR_MASK_Reserved_31_16_MASK        0xffff0000

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
} PCIE_CORR_ERR_MASK_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CORR_ERR_MASK_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CORR_ERR_MASK_OFFSET       0x164
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x164)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CORR_ERR_MASK_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CORR_ERR_MASK_OFFSET       0x164
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x164)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10148164UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10150164UL


/***********************************************************
* Register Name : PCIE_CORR_ERR_STATUS
* Register Description :
* The Correctable Error Status register reports error status of individual correctable errors in a function.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the function detects a Receiver Error. This bit is sticky.
#define NBIF_CORR_ERR_STATUS_RCV_ERR_STATUS_OFFSET      0
#define NBIF_CORR_ERR_STATUS_RCV_ERR_STATUS_MASK        0x1

// Bitfield Description : 
#define NBIF_CORR_ERR_STATUS_Reserved_5_1_OFFSET      1
#define NBIF_CORR_ERR_STATUS_Reserved_5_1_MASK        0x3e

// Bitfield Description : Bad TLP Status.
#define NBIF_CORR_ERR_STATUS_BAD_TLP_STATUS_OFFSET      6
#define NBIF_CORR_ERR_STATUS_BAD_TLP_STATUS_MASK        0x40

// Bitfield Description : Bad DLLP Status.
#define NBIF_CORR_ERR_STATUS_BAD_DLLP_STATUS_OFFSET      7
#define NBIF_CORR_ERR_STATUS_BAD_DLLP_STATUS_MASK        0x80

// Bitfield Description : REPLAY_NUM Rollover Status.
#define NBIF_CORR_ERR_STATUS_REPLAY_NUM_ROLLOVER_STATUS_OFFSET      8
#define NBIF_CORR_ERR_STATUS_REPLAY_NUM_ROLLOVER_STATUS_MASK        0x100

// Bitfield Description : 
#define NBIF_CORR_ERR_STATUS_Reserved_11_9_OFFSET      9
#define NBIF_CORR_ERR_STATUS_Reserved_11_9_MASK        0xe00

// Bitfield Description : Replay Timer Timeout Status.
#define NBIF_CORR_ERR_STATUS_REPLAY_TIMER_TIMEOUT_STATUS_OFFSET      12
#define NBIF_CORR_ERR_STATUS_REPLAY_TIMER_TIMEOUT_STATUS_MASK        0x1000

// Bitfield Description : Advisory Non-Fatal Status.
#define NBIF_CORR_ERR_STATUS_ADVISORY_NONFATAL_ERR_STATUS_OFFSET      13
#define NBIF_CORR_ERR_STATUS_ADVISORY_NONFATAL_ERR_STATUS_MASK        0x2000

// Bitfield Description : Corrected Internal Error Status.
#define NBIF_CORR_ERR_STATUS_CORR_INT_ERR_STATUS_OFFSET      14
#define NBIF_CORR_ERR_STATUS_CORR_INT_ERR_STATUS_MASK        0x4000

// Bitfield Description : Indicates if the function detects a Header Log Overflow Error.
#define NBIF_CORR_ERR_STATUS_HDR_LOG_OVFL_STATUS_OFFSET      15
#define NBIF_CORR_ERR_STATUS_HDR_LOG_OVFL_STATUS_MASK        0x8000

// Bitfield Description : 
#define NBIF_CORR_ERR_STATUS_Reserved_31_16_OFFSET      16
#define NBIF_CORR_ERR_STATUS_Reserved_31_16_MASK        0xffff0000

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
} PCIE_CORR_ERR_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CORR_ERR_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CORR_ERR_STATUS_OFFSET       0x160
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x160)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CORR_ERR_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CORR_ERR_STATUS_OFFSET       0x160
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x160)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10148160UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10150160UL


/***********************************************************
* Register Name : PCIE_DLF_ENH_CAP_LIST
* Register Description :
* Data Link Feature Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_DLF_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_DLF_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_DLF_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_DLF_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_DLF_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_DLF_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_DLF_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DLF_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DLF_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DLF_ENH_CAP_LIST_OFFSET       0x400
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DLF_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x400)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DLF_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DLF_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DLF_ENH_CAP_LIST_OFFSET       0x400
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DLF_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x400)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x10148400UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DLF_ENH_CAP_LIST_ADDRESS    0x10150400UL


/***********************************************************
* Register Name : PCIE_DPA_CAP
* Register Description :
* DPA Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the maximum substate number (plus one) supported by the function.
#define NBIF_DPA_CAP_SUBSTATE_MAX_OFFSET      0
#define NBIF_DPA_CAP_SUBSTATE_MAX_MASK        0x1f

// Bitfield Description : 
#define NBIF_DPA_CAP_Reserved_7_5_OFFSET      5
#define NBIF_DPA_CAP_Reserved_7_5_MASK        0xe0

// Bitfield Description : Indicates the unit used for the Transition Latency Value 0/1 value.
#define NBIF_DPA_CAP_TRANS_LAT_UNIT_OFFSET      8
#define NBIF_DPA_CAP_TRANS_LAT_UNIT_MASK        0x300

// Bitfield Description : 
#define NBIF_DPA_CAP_Reserved_11_10_OFFSET      10
#define NBIF_DPA_CAP_Reserved_11_10_MASK        0xc00

// Bitfield Description : Indicates the scale used for the Substate Power Allocation value.
#define NBIF_DPA_CAP_PWR_ALLOC_SCALE_OFFSET      12
#define NBIF_DPA_CAP_PWR_ALLOC_SCALE_MASK        0x3000

// Bitfield Description : 
#define NBIF_DPA_CAP_Reserved_15_14_OFFSET      14
#define NBIF_DPA_CAP_Reserved_15_14_MASK        0xc000

// Bitfield Description : In combination with the Transition Latency Unit value, indicates the transition latency value 0. The Transition Latency Indicator Bits field can use this value to indicate the maximum transition latency for a substate.
#define NBIF_DPA_CAP_TRANS_LAT_VAL_0_OFFSET      16
#define NBIF_DPA_CAP_TRANS_LAT_VAL_0_MASK        0xff0000

// Bitfield Description : In combination with the Transition Latency Unit value, indicates the transition latency value 1. The Transition Latency Indicator Bits field can use this value to indicate the maximum transition latency for a substate.
#define NBIF_DPA_CAP_TRANS_LAT_VAL_1_OFFSET      24
#define NBIF_DPA_CAP_TRANS_LAT_VAL_1_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SUBSTATE_MAX:5;
    UINT32                            Reserved_7_5:3;
    UINT32                            TRANS_LAT_UNIT:2;
    UINT32                            Reserved_11_10:2;
    UINT32                            PWR_ALLOC_SCALE:2;
    UINT32                            Reserved_15_14:2;
    UINT32                            TRANS_LAT_VAL_0:8;
    UINT32                            TRANS_LAT_VAL_1:8;
  } Field;
  UINT32 Value;
} PCIE_DPA_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_CAP_OFFSET       0x254
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x254)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_CAP_OFFSET       0x254
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x254)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10148254UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10150254UL


/***********************************************************
* Register Name : PCIE_DPA_CNTL
* Register Description :
* DPA Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When Substate Control Enabled bit is set, controls the function's current substate.
#define NBIF_DPA_CNTL_SUBSTATE_CNTL_OFFSET      0
#define NBIF_DPA_CNTL_SUBSTATE_CNTL_MASK        0x1f

// Bitfield Description : 
#define NBIF_DPA_CNTL_Reserved_15_5_OFFSET      5
#define NBIF_DPA_CNTL_Reserved_15_5_MASK        0xffe0

typedef union {
  struct {
    UINT16                            SUBSTATE_CNTL:5;
    UINT16                            Reserved_15_5:11;
  } Field;
  UINT16 Value;
} PCIE_DPA_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_CNTL_OFFSET       0x25e
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x25e)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_CNTL_OFFSET       0x25e
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x25e)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1014825eUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1015025eUL


/***********************************************************
* Register Name : PCIE_DPA_ENH_CAP_LIST
* Register Description :
* DPA Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_DPA_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_DPA_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_DPA_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_DPA_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_DPA_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_DPA_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_DPA_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_ENH_CAP_LIST_OFFSET       0x250
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x250)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_ENH_CAP_LIST_OFFSET       0x250
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x250)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10148250UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10150250UL


/***********************************************************
* Register Name : PCIE_DPA_LATENCY_INDICATOR
* Register Description :
* DPA Latency Indicator register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates which Transition Latency Value 0/1 applies for a substate. This field is a bit vector that covers all supported substates.
#define NBIF_DPA_LATENCY_INDICATOR_TRANS_LAT_INDICATOR_BITS_OFFSET      0
#define NBIF_DPA_LATENCY_INDICATOR_TRANS_LAT_INDICATOR_BITS_MASK        0xff

// Bitfield Description : 
#define NBIF_DPA_LATENCY_INDICATOR_Reserved_31_8_OFFSET      8
#define NBIF_DPA_LATENCY_INDICATOR_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            TRANS_LAT_INDICATOR_BITS:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} PCIE_DPA_LATENCY_INDICATOR_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_OFFSET       0x258
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x258)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_LATENCY_INDICATOR_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_LATENCY_INDICATOR_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_LATENCY_INDICATOR_OFFSET       0x258
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x258)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10148258UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10150258UL


/***********************************************************
* Register Name : PCIE_DPA_STATUS
* Register Description :
* DPA Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the function's current substate.
#define NBIF_DPA_STATUS_SUBSTATE_STATUS_OFFSET      0
#define NBIF_DPA_STATUS_SUBSTATE_STATUS_MASK        0x1f

// Bitfield Description : 
#define NBIF_DPA_STATUS_Reserved_7_5_OFFSET      5
#define NBIF_DPA_STATUS_Reserved_7_5_MASK        0xe0

// Bitfield Description : Enables the current substate to be controlled by the Substate Control field.
#define NBIF_DPA_STATUS_SUBSTATE_CNTL_ENABLED_OFFSET      8
#define NBIF_DPA_STATUS_SUBSTATE_CNTL_ENABLED_MASK        0x100

// Bitfield Description : 
#define NBIF_DPA_STATUS_Reserved_15_9_OFFSET      9
#define NBIF_DPA_STATUS_Reserved_15_9_MASK        0xfe00

typedef union {
  struct {
    UINT16                            SUBSTATE_STATUS:5;
    UINT16                            Reserved_7_5:3;
    UINT16                            SUBSTATE_CNTL_ENABLED:1;
    UINT16                            Reserved_15_9:7;
  } Field;
  UINT16 Value;
} PCIE_DPA_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_STATUS_OFFSET       0x25c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x25c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_STATUS_OFFSET       0x25c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x25c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1014825cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1015025cUL


/***********************************************************
* Register Name : PCIE_DPA_SUBSTATE_PWR_ALLOC_0
* Register Description :
* DPA Power Allocation Array 0 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Power Allocation Scale value, indicates the power allocation in Watts for substate 0.
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_0_SUBSTATE_PWR_ALLOC_OFFSET      0
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_0_SUBSTATE_PWR_ALLOC_MASK        0xff

typedef union {
  struct {
    UINT8                            SUBSTATE_PWR_ALLOC:8;
  } Field;
  UINT8 Value;
} PCIE_DPA_SUBSTATE_PWR_ALLOC_0_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_OFFSET       0x260
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x260)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_OFFSET       0x260
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x260)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10148260UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10150260UL


/***********************************************************
* Register Name : PCIE_DPA_SUBSTATE_PWR_ALLOC_1
* Register Description :
* DPA Power Allocation Array 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Power Allocation Scale value, indicates the power allocation in Watts for substate 1.
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_1_SUBSTATE_PWR_ALLOC_OFFSET      0
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_1_SUBSTATE_PWR_ALLOC_MASK        0xff

typedef union {
  struct {
    UINT8                            SUBSTATE_PWR_ALLOC:8;
  } Field;
  UINT8 Value;
} PCIE_DPA_SUBSTATE_PWR_ALLOC_1_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_OFFSET       0x261
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x261)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_OFFSET       0x261
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x261)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10148261UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10150261UL


/***********************************************************
* Register Name : PCIE_DPA_SUBSTATE_PWR_ALLOC_2
* Register Description :
* DPA Power Allocation Array 2 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Power Allocation Scale value, indicates the power allocation in Watts for substate 2.
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_2_SUBSTATE_PWR_ALLOC_OFFSET      0
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_2_SUBSTATE_PWR_ALLOC_MASK        0xff

typedef union {
  struct {
    UINT8                            SUBSTATE_PWR_ALLOC:8;
  } Field;
  UINT8 Value;
} PCIE_DPA_SUBSTATE_PWR_ALLOC_2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_OFFSET       0x262
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x262)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_OFFSET       0x262
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x262)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10148262UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10150262UL


/***********************************************************
* Register Name : PCIE_DPA_SUBSTATE_PWR_ALLOC_3
* Register Description :
* DPA Power Allocation Array 3 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Power Allocation Scale value, indicates the power allocation in Watts for substate 3.
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_3_SUBSTATE_PWR_ALLOC_OFFSET      0
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_3_SUBSTATE_PWR_ALLOC_MASK        0xff

typedef union {
  struct {
    UINT8                            SUBSTATE_PWR_ALLOC:8;
  } Field;
  UINT8 Value;
} PCIE_DPA_SUBSTATE_PWR_ALLOC_3_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_OFFSET       0x263
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x263)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_OFFSET       0x263
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x263)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10148263UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10150263UL


/***********************************************************
* Register Name : PCIE_DPA_SUBSTATE_PWR_ALLOC_4
* Register Description :
* DPA Power Allocation Array 4 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Power Allocation Scale value, indicates the power allocation in Watts for substate 4.
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_4_SUBSTATE_PWR_ALLOC_OFFSET      0
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_4_SUBSTATE_PWR_ALLOC_MASK        0xff

typedef union {
  struct {
    UINT8                            SUBSTATE_PWR_ALLOC:8;
  } Field;
  UINT8 Value;
} PCIE_DPA_SUBSTATE_PWR_ALLOC_4_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_OFFSET       0x264
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x264)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_OFFSET       0x264
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x264)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10148264UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10150264UL


/***********************************************************
* Register Name : PCIE_DPA_SUBSTATE_PWR_ALLOC_5
* Register Description :
* DPA Power Allocation Array 5 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Power Allocation Scale value, indicates the power allocation in Watts for substate 5.
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_5_SUBSTATE_PWR_ALLOC_OFFSET      0
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_5_SUBSTATE_PWR_ALLOC_MASK        0xff

typedef union {
  struct {
    UINT8                            SUBSTATE_PWR_ALLOC:8;
  } Field;
  UINT8 Value;
} PCIE_DPA_SUBSTATE_PWR_ALLOC_5_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_OFFSET       0x265
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x265)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_OFFSET       0x265
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x265)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10148265UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10150265UL


/***********************************************************
* Register Name : PCIE_DPA_SUBSTATE_PWR_ALLOC_6
* Register Description :
* DPA Power Allocation Array 6 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Power Allocation Scale value, indicates the power allocation in Watts for substate 6.
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_6_SUBSTATE_PWR_ALLOC_OFFSET      0
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_6_SUBSTATE_PWR_ALLOC_MASK        0xff

typedef union {
  struct {
    UINT8                            SUBSTATE_PWR_ALLOC:8;
  } Field;
  UINT8 Value;
} PCIE_DPA_SUBSTATE_PWR_ALLOC_6_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_OFFSET       0x266
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x266)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_OFFSET       0x266
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x266)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10148266UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10150266UL


/***********************************************************
* Register Name : PCIE_DPA_SUBSTATE_PWR_ALLOC_7
* Register Description :
* DPA Power Allocation Array 7 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Power Allocation Scale value, indicates the power allocation in Watts for substate 7.
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_7_SUBSTATE_PWR_ALLOC_OFFSET      0
#define NBIF_DPA_SUBSTATE_PWR_ALLOC_7_SUBSTATE_PWR_ALLOC_MASK        0xff

typedef union {
  struct {
    UINT8                            SUBSTATE_PWR_ALLOC:8;
  } Field;
  UINT8 Value;
} PCIE_DPA_SUBSTATE_PWR_ALLOC_7_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_OFFSET       0x267
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x267)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_OFFSET       0x267
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x267)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10148267UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10150267UL


/***********************************************************
* Register Name : PCIE_HDR_LOG0
* Register Description :
* Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TLP Header 1st DW.
#define NBIF_HDR_LOG0_TLP_HDR_OFFSET      0
#define NBIF_HDR_LOG0_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_HDR_LOG0_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG0_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG0_OFFSET       0x16c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x16c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG0_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG0_OFFSET       0x16c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x16c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1014816cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1015016cUL


/***********************************************************
* Register Name : PCIE_HDR_LOG1
* Register Description :
* Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TLP Header 2nd DW.
#define NBIF_HDR_LOG1_TLP_HDR_OFFSET      0
#define NBIF_HDR_LOG1_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_HDR_LOG1_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG1_OFFSET       0x170
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x170)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG1_OFFSET       0x170
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x170)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10148170UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10150170UL


/***********************************************************
* Register Name : PCIE_HDR_LOG2
* Register Description :
* Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TLP Header 3rd DW.
#define NBIF_HDR_LOG2_TLP_HDR_OFFSET      0
#define NBIF_HDR_LOG2_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_HDR_LOG2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG2_OFFSET       0x174
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x174)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG2_OFFSET       0x174
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x174)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10148174UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10150174UL


/***********************************************************
* Register Name : PCIE_HDR_LOG3
* Register Description :
* Header Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : TLP Header 4th DW.
#define NBIF_HDR_LOG3_TLP_HDR_OFFSET      0
#define NBIF_HDR_LOG3_TLP_HDR_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_HDR:32;
  } Field;
  UINT32 Value;
} PCIE_HDR_LOG3_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG3_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG3_OFFSET       0x178
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_HDR_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x178)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG3_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG3_OFFSET       0x178
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_HDR_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x178)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10148178UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10150178UL


/***********************************************************
* Register Name : PCIE_LANE_EQUALIZATION_CNTL
* Register Description :
* Lane 0 Equalization Control Register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define NBIF_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_8GT_TX_PRESET_OFFSET      0
#define NBIF_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_8GT_TX_PRESET_MASK        0xf

// Bitfield Description : For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define NBIF_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_8GT_RX_PRESET_HINT_OFFSET      4
#define NBIF_LANE_EQUALIZATION_CNTL_DOWNSTREAM_PORT_8GT_RX_PRESET_HINT_MASK        0x70

// Bitfield Description : 
#define NBIF_LANE_EQUALIZATION_CNTL_Reserved_7_7_OFFSET      7
#define NBIF_LANE_EQUALIZATION_CNTL_Reserved_7_7_MASK        0x80

// Bitfield Description : (Upstream Ports only) captures the transmitter preset value that the Upstream Port received from the other side for initial operation at 8.0 GT/s.
#define NBIF_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_8GT_TX_PRESET_OFFSET      8
#define NBIF_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_8GT_TX_PRESET_MASK        0xf00

// Bitfield Description : (Upstream Ports only) captures the receiver preset hint value that the Upstream Port received from the other side for initial operation at 8.0 GT/s.
#define NBIF_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_8GT_RX_PRESET_HINT_OFFSET      12
#define NBIF_LANE_EQUALIZATION_CNTL_UPSTREAM_PORT_8GT_RX_PRESET_HINT_MASK        0x7000

// Bitfield Description : 
#define NBIF_LANE_EQUALIZATION_CNTL_Reserved_15_15_OFFSET      15
#define NBIF_LANE_EQUALIZATION_CNTL_Reserved_15_15_MASK        0x8000

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
} PCIE_LANE_EQUALIZATION_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x27c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x27c)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x27e)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x290)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x292)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x294)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x296)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x298)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x29a)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x280)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x282)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x284)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x286)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x288)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x28a)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x28c)

// FIXME DUPLICATE #define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x28e)

#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x27c
#define PCICFG_NBIO_FUNC0_DEV1_N0_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x27c)

#define SMN_DEV1_FUNC0_NBIF0_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1014827cUL

#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x290
#define PCICFG_NBIO_FUNC0_DEV1_N10_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x290)

#define SMN_DEV1_FUNC0_NBIF0_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10148290UL

#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x292
#define PCICFG_NBIO_FUNC0_DEV1_N11_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x292)

#define SMN_DEV1_FUNC0_NBIF0_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10148292UL

#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x294
#define PCICFG_NBIO_FUNC0_DEV1_N12_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x294)

#define SMN_DEV1_FUNC0_NBIF0_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10148294UL

#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x296
#define PCICFG_NBIO_FUNC0_DEV1_N13_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x296)

#define SMN_DEV1_FUNC0_NBIF0_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10148296UL

#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x298
#define PCICFG_NBIO_FUNC0_DEV1_N14_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x298)

#define SMN_DEV1_FUNC0_NBIF0_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10148298UL

#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x29a
#define PCICFG_NBIO_FUNC0_DEV1_N15_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x29a)

#define SMN_DEV1_FUNC0_NBIF0_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1014829aUL

#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x27e
#define PCICFG_NBIO_FUNC0_DEV1_N1_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x27e)

#define SMN_DEV1_FUNC0_NBIF0_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1014827eUL

#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x280
#define PCICFG_NBIO_FUNC0_DEV1_N2_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x280)

#define SMN_DEV1_FUNC0_NBIF0_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10148280UL

#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x282
#define PCICFG_NBIO_FUNC0_DEV1_N3_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x282)

#define SMN_DEV1_FUNC0_NBIF0_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10148282UL

#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x284
#define PCICFG_NBIO_FUNC0_DEV1_N4_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x284)

#define SMN_DEV1_FUNC0_NBIF0_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10148284UL

#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x286
#define PCICFG_NBIO_FUNC0_DEV1_N5_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x286)

#define SMN_DEV1_FUNC0_NBIF0_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10148286UL

#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x288
#define PCICFG_NBIO_FUNC0_DEV1_N6_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x288)

#define SMN_DEV1_FUNC0_NBIF0_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10148288UL

#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x28a
#define PCICFG_NBIO_FUNC0_DEV1_N7_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x28a)

#define SMN_DEV1_FUNC0_NBIF0_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1014828aUL

#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x28c
#define PCICFG_NBIO_FUNC0_DEV1_N8_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x28c)

#define SMN_DEV1_FUNC0_NBIF0_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1014828cUL

#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_OFFSET       0x28e
#define PCICFG_NBIO_FUNC0_DEV1_N9_NBIF0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x28e)

#define SMN_DEV1_FUNC0_NBIF0_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1014828eUL

#define SMN_DEV2_FUNC0_NBIF0_N0_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1015027cUL

#define SMN_DEV2_FUNC0_NBIF0_N10_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10150290UL

#define SMN_DEV2_FUNC0_NBIF0_N11_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10150292UL

#define SMN_DEV2_FUNC0_NBIF0_N12_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10150294UL

#define SMN_DEV2_FUNC0_NBIF0_N13_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10150296UL

#define SMN_DEV2_FUNC0_NBIF0_N14_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10150298UL

#define SMN_DEV2_FUNC0_NBIF0_N15_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1015029aUL

#define SMN_DEV2_FUNC0_NBIF0_N1_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1015027eUL

#define SMN_DEV2_FUNC0_NBIF0_N2_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10150280UL

#define SMN_DEV2_FUNC0_NBIF0_N3_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10150282UL

#define SMN_DEV2_FUNC0_NBIF0_N4_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10150284UL

#define SMN_DEV2_FUNC0_NBIF0_N5_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10150286UL

#define SMN_DEV2_FUNC0_NBIF0_N6_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x10150288UL

#define SMN_DEV2_FUNC0_NBIF0_N7_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1015028aUL

#define SMN_DEV2_FUNC0_NBIF0_N8_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1015028cUL

#define SMN_DEV2_FUNC0_NBIF0_N9_PCIE_LANE_EQUALIZATION_CNTL_ADDRESS    0x1015028eUL


/***********************************************************
* Register Name : PCIE_LANE_ERROR_STATUS
* Register Description :
* Lane Error Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the corresponding lane detected a lane based error. This field is a bit vector that covers all supported lanes. Bit 0 corresponds to the default lane 0, and bit ordering is invariant to link width and lane reversal negotiation. This field is sticky.
#define NBIF_LANE_ERROR_STATUS_LANE_ERROR_STATUS_BITS_OFFSET      0
#define NBIF_LANE_ERROR_STATUS_LANE_ERROR_STATUS_BITS_MASK        0xffff

// Bitfield Description : 
#define NBIF_LANE_ERROR_STATUS_Reserved_31_16_OFFSET      16
#define NBIF_LANE_ERROR_STATUS_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            LANE_ERROR_STATUS_BITS:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} PCIE_LANE_ERROR_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_ERROR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_ERROR_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_ERROR_STATUS_OFFSET       0x278
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LANE_ERROR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x278)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LANE_ERROR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LANE_ERROR_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LANE_ERROR_STATUS_OFFSET       0x278
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LANE_ERROR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x278)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_LANE_ERROR_STATUS_ADDRESS    0x10148278UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_LANE_ERROR_STATUS_ADDRESS    0x10150278UL


/***********************************************************
* Register Name : PCIE_LINK_CNTL3
* Register Description :
* Link Control 3 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define NBIF_LINK_CNTL3_PERFORM_EQUALIZATION_OFFSET      0
#define NBIF_LINK_CNTL3_PERFORM_EQUALIZATION_MASK        0x1

// Bitfield Description : For Upstream Ports that don't support crosslinks, this bit is Reserved.
#define NBIF_LINK_CNTL3_LINK_EQUALIZATION_REQ_INT_EN_OFFSET      1
#define NBIF_LINK_CNTL3_LINK_EQUALIZATION_REQ_INT_EN_MASK        0x2

// Bitfield Description : 
#define NBIF_LINK_CNTL3_Reserved_8_2_OFFSET      2
#define NBIF_LINK_CNTL3_Reserved_8_2_MASK        0x1fc

// Bitfield Description : Controls if the Port supports both SRIS and software control of the SKP ordered set transmission scheduling rate for the indicated speed(s).
#define NBIF_LINK_CNTL3_ENABLE_LOWER_SKP_OS_GEN_OFFSET      9
#define NBIF_LINK_CNTL3_ENABLE_LOWER_SKP_OS_GEN_MASK        0xfe00

// Bitfield Description : 
#define NBIF_LINK_CNTL3_Reserved_31_16_OFFSET      16
#define NBIF_LINK_CNTL3_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            PERFORM_EQUALIZATION:1;
    UINT32                            LINK_EQUALIZATION_REQ_INT_EN:1;
    UINT32                            Reserved_8_2:7;
    UINT32                            ENABLE_LOWER_SKP_OS_GEN:7;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} PCIE_LINK_CNTL3_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LINK_CNTL3_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LINK_CNTL3_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LINK_CNTL3_OFFSET       0x274
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LINK_CNTL3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x274)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LINK_CNTL3_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LINK_CNTL3_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LINK_CNTL3_OFFSET       0x274
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LINK_CNTL3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x274)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_LINK_CNTL3_ADDRESS    0x10148274UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_LINK_CNTL3_ADDRESS    0x10150274UL


/***********************************************************
* Register Name : PCIE_LTR_CAP
* Register Description :
* Max Snoop Latency and Max No-Snoop Latency registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Max Snoop LatencyScale value, indicates the maximum snoop latency that a device is permitted to request.
#define NBIF_LTR_CAP_LTR_MAX_S_LATENCY_VALUE_OFFSET      0
#define NBIF_LTR_CAP_LTR_MAX_S_LATENCY_VALUE_MASK        0x3ff

// Bitfield Description : Indicates the scale used for the Maximum Snoop LatencyValue.
#define NBIF_LTR_CAP_LTR_MAX_S_LATENCY_SCALE_OFFSET      10
#define NBIF_LTR_CAP_LTR_MAX_S_LATENCY_SCALE_MASK        0x1c00

// Bitfield Description : 
#define NBIF_LTR_CAP_Reserved_15_13_OFFSET      13
#define NBIF_LTR_CAP_Reserved_15_13_MASK        0xe000

// Bitfield Description : Along with the Max Non-Snoop LatencyScale field, this register specifies the maximum Non-Snoop latency that a device is permitted to request. Software should set this to the platform's maximum supported latency or less.
#define NBIF_LTR_CAP_LTR_MAX_NS_LATENCY_VALUE_OFFSET      16
#define NBIF_LTR_CAP_LTR_MAX_NS_LATENCY_VALUE_MASK        0x3ff0000

// Bitfield Description : Indicates the scale used for the Maximum No-Snoop LatencyValue.
#define NBIF_LTR_CAP_LTR_MAX_NS_LATENCY_SCALE_OFFSET      26
#define NBIF_LTR_CAP_LTR_MAX_NS_LATENCY_SCALE_MASK        0x1c000000

// Bitfield Description : 
#define NBIF_LTR_CAP_Reserved_31_29_OFFSET      29
#define NBIF_LTR_CAP_Reserved_31_29_MASK        0xe0000000

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
} PCIE_LTR_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LTR_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LTR_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LTR_CAP_OFFSET       0x324
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LTR_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x324)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LTR_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LTR_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LTR_CAP_OFFSET       0x324
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LTR_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x324)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_LTR_CAP_ADDRESS    0x10148324UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_LTR_CAP_ADDRESS    0x10150324UL


/***********************************************************
* Register Name : PCIE_LTR_ENH_CAP_LIST
* Register Description :
* LTR Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_LTR_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_LTR_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_LTR_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_LTR_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_LTR_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_LTR_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_LTR_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LTR_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LTR_ENH_CAP_LIST_OFFSET       0x320
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_LTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x320)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LTR_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LTR_ENH_CAP_LIST_OFFSET       0x320
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_LTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x320)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x10148320UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_LTR_ENH_CAP_LIST_ADDRESS    0x10150320UL


/***********************************************************
* Register Name : PCIE_MARGINING_ENH_CAP_LIST
* Register Description :
* Margining Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_MARGINING_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_MARGINING_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_MARGINING_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_MARGINING_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_MARGINING_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_MARGINING_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_MARGINING_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_MARGINING_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_MARGINING_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_MARGINING_ENH_CAP_LIST_OFFSET       0x450
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x450)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_MARGINING_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_MARGINING_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_MARGINING_ENH_CAP_LIST_OFFSET       0x450
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x450)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x10148450UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_MARGINING_ENH_CAP_LIST_ADDRESS    0x10150450UL


/***********************************************************
* Register Name : PCIE_PASID_CAP
* Register Description :
* PASID Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NBIF_PASID_CAP_Reserved_0_0_OFFSET      0
#define NBIF_PASID_CAP_Reserved_0_0_MASK        0x1

// Bitfield Description : Indicates if the function supports sending requests that have the Execute Requested bit set.
#define NBIF_PASID_CAP_PASID_EXE_PERMISSION_SUPPORTED_OFFSET      1
#define NBIF_PASID_CAP_PASID_EXE_PERMISSION_SUPPORTED_MASK        0x2

// Bitfield Description : Indicates if the function supports sending requests that have the Privileged Mode Requested bit set.
#define NBIF_PASID_CAP_PASID_PRIV_MODE_SUPPORTED_OFFSET      2
#define NBIF_PASID_CAP_PASID_PRIV_MODE_SUPPORTED_MASK        0x4

// Bitfield Description : 
#define NBIF_PASID_CAP_Reserved_7_3_OFFSET      3
#define NBIF_PASID_CAP_Reserved_7_3_MASK        0xf8

// Bitfield Description : Indicates the width of the PASID field supported by the function. This value ranges from 0 (PASID 0) to 20 (20-bit PASID).
#define NBIF_PASID_CAP_MAX_PASID_WIDTH_OFFSET      8
#define NBIF_PASID_CAP_MAX_PASID_WIDTH_MASK        0x1f00

// Bitfield Description : 
#define NBIF_PASID_CAP_Reserved_15_13_OFFSET      13
#define NBIF_PASID_CAP_Reserved_15_13_MASK        0xe000

typedef union {
  struct {
    UINT16                            Reserved_0_0:1;
    UINT16                            PASID_EXE_PERMISSION_SUPPORTED:1;
    UINT16                            PASID_PRIV_MODE_SUPPORTED:1;
    UINT16                            Reserved_7_3:5;
    UINT16                            MAX_PASID_WIDTH:5;
    UINT16                            Reserved_15_13:3;
  } Field;
  UINT16 Value;
} PCIE_PASID_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_CAP_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2d4)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_CAP_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2d4)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101482d4UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101502d4UL


/***********************************************************
* Register Name : PCIE_PASID_CNTL
* Register Description :
* PASID Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : If programmed when Enable bit is clear in the ATS Control register, controls if the function is permitted to send requests that contain a PASID value.
#define NBIF_PASID_CNTL_PASID_ENABLE_OFFSET      0
#define NBIF_PASID_CNTL_PASID_ENABLE_MASK        0x1

// Bitfield Description : If programmed when Enable bit is clear in the ATS Control register, controls if the function is permitted to send requests that have the Execute Requested bit set.
#define NBIF_PASID_CNTL_PASID_EXE_PERMISSION_ENABLE_OFFSET      1
#define NBIF_PASID_CNTL_PASID_EXE_PERMISSION_ENABLE_MASK        0x2

// Bitfield Description : If programmed when Enable bit is clear in the ATS Control register, controls if the function is permitted to send requests that have the Privileged Mode Requested bit set.
#define NBIF_PASID_CNTL_PASID_PRIV_MODE_SUPPORTED_ENABLE_OFFSET      2
#define NBIF_PASID_CNTL_PASID_PRIV_MODE_SUPPORTED_ENABLE_MASK        0x4

// Bitfield Description : 
#define NBIF_PASID_CNTL_Reserved_15_3_OFFSET      3
#define NBIF_PASID_CNTL_Reserved_15_3_MASK        0xfff8

typedef union {
  struct {
    UINT16                            PASID_ENABLE:1;
    UINT16                            PASID_EXE_PERMISSION_ENABLE:1;
    UINT16                            PASID_PRIV_MODE_SUPPORTED_ENABLE:1;
    UINT16                            Reserved_15_3:13;
  } Field;
  UINT16 Value;
} PCIE_PASID_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_CNTL_OFFSET       0x2d6
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2d6)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_CNTL_OFFSET       0x2d6
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2d6)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101482d6UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101502d6UL


/***********************************************************
* Register Name : PCIE_PASID_ENH_CAP_LIST
* Register Description :
* PASID Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_PASID_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_PASID_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_PASID_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_PASID_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_PASID_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_PASID_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_PASID_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_ENH_CAP_LIST_OFFSET       0x2d0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2d0)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_ENH_CAP_LIST_OFFSET       0x2d0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x2d0)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101482d0UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101502d0UL


/***********************************************************
* Register Name : PCIE_PHY_16GT_ENH_CAP_LIST
* Register Description :
* Physical Layer 16.0 GT/s Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_PHY_16GT_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_PHY_16GT_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_PHY_16GT_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_PHY_16GT_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_PHY_16GT_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_PHY_16GT_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_PHY_16GT_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PHY_16GT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PHY_16GT_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PHY_16GT_ENH_CAP_LIST_OFFSET       0x410
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x410)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PHY_16GT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PHY_16GT_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PHY_16GT_ENH_CAP_LIST_OFFSET       0x410
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x410)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x10148410UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PHY_16GT_ENH_CAP_LIST_ADDRESS    0x10150410UL


/***********************************************************
* Register Name : PCIE_PORT_VC_CAP_REG1
* Register Description :
* Port VC Capability register 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the number of (extended) Virtual Channels in addition to the default VC supported by the device.
#define NBIF_PORT_VC_CAP_REG1_EXT_VC_COUNT_OFFSET      0
#define NBIF_PORT_VC_CAP_REG1_EXT_VC_COUNT_MASK        0x7

// Bitfield Description : 
#define NBIF_PORT_VC_CAP_REG1_Reserved_3_3_OFFSET      3
#define NBIF_PORT_VC_CAP_REG1_Reserved_3_3_MASK        0x8

// Bitfield Description : Indicates the number of (extended) Virtual Channels in addition to the default VC belonging to the Low Priority Virtual Channel group.
#define NBIF_PORT_VC_CAP_REG1_LOW_PRIORITY_EXT_VC_COUNT_OFFSET      4
#define NBIF_PORT_VC_CAP_REG1_LOW_PRIORITY_EXT_VC_COUNT_MASK        0x70

// Bitfield Description : 
#define NBIF_PORT_VC_CAP_REG1_Reserved_7_7_OFFSET      7
#define NBIF_PORT_VC_CAP_REG1_Reserved_7_7_MASK        0x80

// Bitfield Description : Indicates the reference clock for Virtual Channels that support time-based WRR Port Arbitration, otherwise it must be 0.
#define NBIF_PORT_VC_CAP_REG1_REF_CLK_OFFSET      8
#define NBIF_PORT_VC_CAP_REG1_REF_CLK_MASK        0x300

// Bitfield Description : Indicates the size (in bits) of Port Arbitration table entry in the device, or must be 0 if Port Arbitration table is not supported.
#define NBIF_PORT_VC_CAP_REG1_PORT_ARB_TABLE_ENTRY_SIZE_OFFSET      10
#define NBIF_PORT_VC_CAP_REG1_PORT_ARB_TABLE_ENTRY_SIZE_MASK        0xc00

// Bitfield Description : 
#define NBIF_PORT_VC_CAP_REG1_Reserved_31_12_OFFSET      12
#define NBIF_PORT_VC_CAP_REG1_Reserved_31_12_MASK        0xfffff000

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
} PCIE_PORT_VC_CAP_REG1_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CAP_REG1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CAP_REG1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CAP_REG1_OFFSET       0x114
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CAP_REG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x114)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CAP_REG1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CAP_REG1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CAP_REG1_OFFSET       0x114
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CAP_REG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x114)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x10148114UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PORT_VC_CAP_REG1_ADDRESS    0x10150114UL


/***********************************************************
* Register Name : PCIE_PORT_VC_CAP_REG2
* Register Description :
* Port VC Capability register 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the types of VC Arbitration supported by the device for the Low Priority Virtual Channel group, or must be 0 if the Low Priority Virtual Channel group contains only VC0. Bit 0 = Fixed; Bit 1 = WRR-32; Bit 2 = WRR-64; Bit 3 = WRR-128.
#define NBIF_PORT_VC_CAP_REG2_VC_ARB_CAP_OFFSET      0
#define NBIF_PORT_VC_CAP_REG2_VC_ARB_CAP_MASK        0xff

// Bitfield Description : 
#define NBIF_PORT_VC_CAP_REG2_Reserved_23_8_OFFSET      8
#define NBIF_PORT_VC_CAP_REG2_Reserved_23_8_MASK        0xffff00

// Bitfield Description : Indicates the location of the VC Arbitration Table, or must be 0 if the VC Arbitration Table is not present.
#define NBIF_PORT_VC_CAP_REG2_VC_ARB_TABLE_OFFSET_OFFSET      24
#define NBIF_PORT_VC_CAP_REG2_VC_ARB_TABLE_OFFSET_MASK        0xff000000

typedef union {
  struct {
    UINT32                            VC_ARB_CAP:8;
    UINT32                            Reserved_23_8:16;
    UINT32                            VC_ARB_TABLE_OFFSET:8;
  } Field;
  UINT32 Value;
} PCIE_PORT_VC_CAP_REG2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CAP_REG2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CAP_REG2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CAP_REG2_OFFSET       0x118
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CAP_REG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x118)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CAP_REG2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CAP_REG2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CAP_REG2_OFFSET       0x118
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CAP_REG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x118)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x10148118UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PORT_VC_CAP_REG2_ADDRESS    0x10150118UL


/***********************************************************
* Register Name : PCIE_PORT_VC_CNTL
* Register Description :
* Port VC Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Updates the VC Arbitration logic from the VC Arbitration Table (if present), when written with 1. Reads always return 0.
#define NBIF_PORT_VC_CNTL_LOAD_VC_ARB_TABLE_OFFSET      0
#define NBIF_PORT_VC_CNTL_LOAD_VC_ARB_TABLE_MASK        0x1

// Bitfield Description : Configures the VC Arbitration by selecting one of the supported VC Arbitration schemes.
#define NBIF_PORT_VC_CNTL_VC_ARB_SELECT_OFFSET      1
#define NBIF_PORT_VC_CNTL_VC_ARB_SELECT_MASK        0xe

// Bitfield Description : 
#define NBIF_PORT_VC_CNTL_Reserved_15_4_OFFSET      4
#define NBIF_PORT_VC_CNTL_Reserved_15_4_MASK        0xfff0

typedef union {
  struct {
    UINT16                            LOAD_VC_ARB_TABLE:1;
    UINT16                            VC_ARB_SELECT:3;
    UINT16                            Reserved_15_4:12;
  } Field;
  UINT16 Value;
} PCIE_PORT_VC_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CNTL_OFFSET       0x11c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x11c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CNTL_OFFSET       0x11c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x11c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PORT_VC_CNTL_ADDRESS    0x1014811cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PORT_VC_CNTL_ADDRESS    0x1015011cUL


/***********************************************************
* Register Name : PCIE_PORT_VC_STATUS
* Register Description :
* Port VC Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the contents of the VC Arbitration Table are coherent, or must be 0 if the VC Arbitration Table is not present.
#define NBIF_PORT_VC_STATUS_VC_ARB_TABLE_STATUS_OFFSET      0
#define NBIF_PORT_VC_STATUS_VC_ARB_TABLE_STATUS_MASK        0x1

// Bitfield Description : 
#define NBIF_PORT_VC_STATUS_Reserved_15_1_OFFSET      1
#define NBIF_PORT_VC_STATUS_Reserved_15_1_MASK        0xfffe

typedef union {
  struct {
    UINT16                            VC_ARB_TABLE_STATUS:1;
    UINT16                            Reserved_15_1:15;
  } Field;
  UINT16 Value;
} PCIE_PORT_VC_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_STATUS_OFFSET       0x11e
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PORT_VC_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x11e)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_STATUS_OFFSET       0x11e
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PORT_VC_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x11e)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PORT_VC_STATUS_ADDRESS    0x1014811eUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PORT_VC_STATUS_ADDRESS    0x1015011eUL


/***********************************************************
* Register Name : PCIE_PWR_BUDGET_CAP
* Register Description :
* Power Budget Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the power budget for the device is included within the system power budget.
#define NBIF_PWR_BUDGET_CAP_SYSTEM_ALLOCATED_OFFSET      0
#define NBIF_PWR_BUDGET_CAP_SYSTEM_ALLOCATED_MASK        0x1

// Bitfield Description : 
#define NBIF_PWR_BUDGET_CAP_Reserved_7_1_OFFSET      1
#define NBIF_PWR_BUDGET_CAP_Reserved_7_1_MASK        0xfe

typedef union {
  struct {
    UINT8                            SYSTEM_ALLOCATED:1;
    UINT8                            Reserved_7_1:7;
  } Field;
  UINT8 Value;
} PCIE_PWR_BUDGET_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_CAP_OFFSET       0x24c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x24c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_CAP_OFFSET       0x24c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x24c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1014824cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1015024cUL


/***********************************************************
* Register Name : PCIE_PWR_BUDGET_DATA
* Register Description :
* Power Budgeting Data register. Value is only valid when the entire DW returns a non-zero value.
* Visibility : 0x2
************************************************************/

// Bitfield Description : In combination with the Data Scale value, indicates in Watts the base power value in the given operating condition.
#define NBIF_PWR_BUDGET_DATA_BASE_POWER_OFFSET      0
#define NBIF_PWR_BUDGET_DATA_BASE_POWER_MASK        0xff

// Bitfield Description : Indicates the scale used for the Base Power value.
#define NBIF_PWR_BUDGET_DATA_DATA_SCALE_OFFSET      8
#define NBIF_PWR_BUDGET_DATA_DATA_SCALE_MASK        0x300

// Bitfield Description : Indicates the power management sub-state of the operating condition being described.
#define NBIF_PWR_BUDGET_DATA_PM_SUB_STATE_OFFSET      10
#define NBIF_PWR_BUDGET_DATA_PM_SUB_STATE_MASK        0x1c00

// Bitfield Description : Indicates the power management state of the operating condition being described.
#define NBIF_PWR_BUDGET_DATA_PM_STATE_OFFSET      13
#define NBIF_PWR_BUDGET_DATA_PM_STATE_MASK        0x6000

// Bitfield Description : Indicates the type of the operating condition being described.
#define NBIF_PWR_BUDGET_DATA_TYPE_OFFSET      15
#define NBIF_PWR_BUDGET_DATA_TYPE_MASK        0x38000

// Bitfield Description : Indicates the thermal load or power rail of the operating condition being described.
#define NBIF_PWR_BUDGET_DATA_POWER_RAIL_OFFSET      18
#define NBIF_PWR_BUDGET_DATA_POWER_RAIL_MASK        0x1c0000

// Bitfield Description : 
#define NBIF_PWR_BUDGET_DATA_Reserved_31_21_OFFSET      21
#define NBIF_PWR_BUDGET_DATA_Reserved_31_21_MASK        0xffe00000

typedef union {
  struct {
    UINT32                            BASE_POWER:8;
    UINT32                            DATA_SCALE:2;
    UINT32                            PM_SUB_STATE:3;
    UINT32                            PM_STATE:2;
    UINT32                            TYPE:3;
    UINT32                            POWER_RAIL:3;
    UINT32                            Reserved_31_21:11;
  } Field;
  UINT32 Value;
} PCIE_PWR_BUDGET_DATA_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_OFFSET       0x248
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x248)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_DATA_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_DATA_OFFSET       0x248
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x248)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10148248UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10150248UL


/***********************************************************
* Register Name : PCIE_PWR_BUDGET_DATA_SELECT
* Register Description :
* Power Budgeting Data Select register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Selects the Power Budgeting Data (DW) reported through the Data register.
#define NBIF_PWR_BUDGET_DATA_SELECT_DATA_SELECT_OFFSET      0
#define NBIF_PWR_BUDGET_DATA_SELECT_DATA_SELECT_MASK        0xff

typedef union {
  struct {
    UINT8                            DATA_SELECT:8;
  } Field;
  UINT8 Value;
} PCIE_PWR_BUDGET_DATA_SELECT_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_OFFSET       0x244
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x244)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_OFFSET       0x244
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x244)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10148244UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10150244UL


/***********************************************************
* Register Name : PCIE_PWR_BUDGET_ENH_CAP_LIST
* Register Description :
* Power Budgeting Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_PWR_BUDGET_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_PWR_BUDGET_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_PWR_BUDGET_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_PWR_BUDGET_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_PWR_BUDGET_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_PWR_BUDGET_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_PWR_BUDGET_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_OFFSET       0x240
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x240)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_OFFSET       0x240
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x240)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10148240UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10150240UL


/***********************************************************
* Register Name : PCIE_RTR_ENH_CAP_LIST
* Register Description :
* Readiness Time Reporting Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_RTR_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_RTR_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_RTR_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_RTR_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_RTR_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_RTR_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_RTR_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_RTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_RTR_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_RTR_ENH_CAP_LIST_OFFSET       0x570
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x570)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_RTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_RTR_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_RTR_ENH_CAP_LIST_OFFSET       0x570
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x570)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10148570UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10150570UL


/***********************************************************
* Register Name : PCIE_SECONDARY_ENH_CAP_LIST
* Register Description :
* Secondary PCI Express Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_SECONDARY_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_SECONDARY_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_SECONDARY_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_SECONDARY_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_SECONDARY_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_SECONDARY_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_SECONDARY_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_SECONDARY_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_SECONDARY_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_SECONDARY_ENH_CAP_LIST_OFFSET       0x270
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x270)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_SECONDARY_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_SECONDARY_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_SECONDARY_ENH_CAP_LIST_OFFSET       0x270
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x270)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x10148270UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_SECONDARY_ENH_CAP_LIST_ADDRESS    0x10150270UL


/***********************************************************
* Register Name : PCIE_TLP_PREFIX_LOG0
* Register Description :
* TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : First TLP Prefix Log.
#define NBIF_TLP_PREFIX_LOG0_TLP_PREFIX_OFFSET      0
#define NBIF_TLP_PREFIX_LOG0_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_TLP_PREFIX_LOG0_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG0_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG0_OFFSET       0x188
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x188)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG0_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG0_OFFSET       0x188
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x188)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10148188UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10150188UL


/***********************************************************
* Register Name : PCIE_TLP_PREFIX_LOG1
* Register Description :
* TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Second TLP Prefix Log.
#define NBIF_TLP_PREFIX_LOG1_TLP_PREFIX_OFFSET      0
#define NBIF_TLP_PREFIX_LOG1_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_TLP_PREFIX_LOG1_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG1_OFFSET       0x18c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x18c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG1_OFFSET       0x18c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x18c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1014818cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1015018cUL


/***********************************************************
* Register Name : PCIE_TLP_PREFIX_LOG2
* Register Description :
* TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Third TLP Prefix Log.
#define NBIF_TLP_PREFIX_LOG2_TLP_PREFIX_OFFSET      0
#define NBIF_TLP_PREFIX_LOG2_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_TLP_PREFIX_LOG2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG2_OFFSET       0x190
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x190)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG2_OFFSET       0x190
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x190)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10148190UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10150190UL


/***********************************************************
* Register Name : PCIE_TLP_PREFIX_LOG3
* Register Description :
* TLP Prefix Log register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Fourth TLP Prefix Log.
#define NBIF_TLP_PREFIX_LOG3_TLP_PREFIX_OFFSET      0
#define NBIF_TLP_PREFIX_LOG3_TLP_PREFIX_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            TLP_PREFIX:32;
  } Field;
  UINT32 Value;
} PCIE_TLP_PREFIX_LOG3_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG3_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG3_OFFSET       0x194
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x194)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG3_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG3_OFFSET       0x194
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x194)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10148194UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10150194UL


/***********************************************************
* Register Name : PCIE_UNCORR_ERR_MASK
* Register Description :
* The Uncorrectable Error Mask register controls reporting of individual uncorrectable errors by a function.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NBIF_UNCORR_ERR_MASK_Reserved_3_0_OFFSET      0
#define NBIF_UNCORR_ERR_MASK_Reserved_3_0_MASK        0xf

// Bitfield Description : Data Link Protocol Error Mask.
#define NBIF_UNCORR_ERR_MASK_DLP_ERR_MASK_OFFSET      4
#define NBIF_UNCORR_ERR_MASK_DLP_ERR_MASK_MASK        0x10

// Bitfield Description : Controls if the function can report a Surprise Down Error.
#define NBIF_UNCORR_ERR_MASK_SURPDN_ERR_MASK_OFFSET      5
#define NBIF_UNCORR_ERR_MASK_SURPDN_ERR_MASK_MASK        0x20

// Bitfield Description : 
#define NBIF_UNCORR_ERR_MASK_Reserved_11_6_OFFSET      6
#define NBIF_UNCORR_ERR_MASK_Reserved_11_6_MASK        0xfc0

// Bitfield Description : Poisoned TLP Mask.
#define NBIF_UNCORR_ERR_MASK_PSN_ERR_MASK_OFFSET      12
#define NBIF_UNCORR_ERR_MASK_PSN_ERR_MASK_MASK        0x1000

// Bitfield Description : Controls if the function can report a Flow Control Protocol Error.
#define NBIF_UNCORR_ERR_MASK_FC_ERR_MASK_OFFSET      13
#define NBIF_UNCORR_ERR_MASK_FC_ERR_MASK_MASK        0x2000

// Bitfield Description : Completion Timeout Mask.
#define NBIF_UNCORR_ERR_MASK_CPL_TIMEOUT_MASK_OFFSET      14
#define NBIF_UNCORR_ERR_MASK_CPL_TIMEOUT_MASK_MASK        0x4000

// Bitfield Description : Completer Abort Mask.
#define NBIF_UNCORR_ERR_MASK_CPL_ABORT_ERR_MASK_OFFSET      15
#define NBIF_UNCORR_ERR_MASK_CPL_ABORT_ERR_MASK_MASK        0x8000

// Bitfield Description : Unexpected Completion Mask.
#define NBIF_UNCORR_ERR_MASK_UNEXP_CPL_MASK_OFFSET      16
#define NBIF_UNCORR_ERR_MASK_UNEXP_CPL_MASK_MASK        0x10000

// Bitfield Description : Controls if the function can report a Receiver Overflow Error.
#define NBIF_UNCORR_ERR_MASK_RCV_OVFL_MASK_OFFSET      17
#define NBIF_UNCORR_ERR_MASK_RCV_OVFL_MASK_MASK        0x20000

// Bitfield Description : Malformed TLP Mask.
#define NBIF_UNCORR_ERR_MASK_MAL_TLP_MASK_OFFSET      18
#define NBIF_UNCORR_ERR_MASK_MAL_TLP_MASK_MASK        0x40000

// Bitfield Description : ECRC Error Mask.
#define NBIF_UNCORR_ERR_MASK_ECRC_ERR_MASK_OFFSET      19
#define NBIF_UNCORR_ERR_MASK_ECRC_ERR_MASK_MASK        0x80000

// Bitfield Description : Unsupported Request Error Mask.
#define NBIF_UNCORR_ERR_MASK_UNSUPP_REQ_ERR_MASK_OFFSET      20
#define NBIF_UNCORR_ERR_MASK_UNSUPP_REQ_ERR_MASK_MASK        0x100000

// Bitfield Description : ACS Violation Mask.
#define NBIF_UNCORR_ERR_MASK_ACS_VIOLATION_MASK_OFFSET      21
#define NBIF_UNCORR_ERR_MASK_ACS_VIOLATION_MASK_MASK        0x200000

// Bitfield Description : Controls if the function can report an Uncorrectable Internal Error. This bit is sticky.
#define NBIF_UNCORR_ERR_MASK_UNCORR_INT_ERR_MASK_OFFSET      22
#define NBIF_UNCORR_ERR_MASK_UNCORR_INT_ERR_MASK_MASK        0x400000

// Bitfield Description : Controls if the function can report a MC Blocked TLP Error. This bit is sticky.
#define NBIF_UNCORR_ERR_MASK_MC_BLOCKED_TLP_MASK_OFFSET      23
#define NBIF_UNCORR_ERR_MASK_MC_BLOCKED_TLP_MASK_MASK        0x800000

// Bitfield Description : Controls if the function can report an AtomicOp Egress Blocked Error.
#define NBIF_UNCORR_ERR_MASK_ATOMICOP_EGRESS_BLOCKED_MASK_OFFSET      24
#define NBIF_UNCORR_ERR_MASK_ATOMICOP_EGRESS_BLOCKED_MASK_MASK        0x1000000

// Bitfield Description : Controls if the function can report a TLP Prefix Blocked Error.
#define NBIF_UNCORR_ERR_MASK_TLP_PREFIX_BLOCKED_ERR_MASK_OFFSET      25
#define NBIF_UNCORR_ERR_MASK_TLP_PREFIX_BLOCKED_ERR_MASK_MASK        0x2000000

// Bitfield Description : Controls if the function can report a Poisoned TLP Egress Blocked Error.
#define NBIF_UNCORR_ERR_MASK_POISONED_TLP_EGRESS_BLOCKED_MASK_OFFSET      26
#define NBIF_UNCORR_ERR_MASK_POISONED_TLP_EGRESS_BLOCKED_MASK_MASK        0x4000000

// Bitfield Description : 
#define NBIF_UNCORR_ERR_MASK_Reserved_31_27_OFFSET      27
#define NBIF_UNCORR_ERR_MASK_Reserved_31_27_MASK        0xf8000000

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
} PCIE_UNCORR_ERR_MASK_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_MASK_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_MASK_OFFSET       0x158
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x158)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_MASK_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_MASK_OFFSET       0x158
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x158)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10148158UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10150158UL


/***********************************************************
* Register Name : PCIE_UNCORR_ERR_SEVERITY
* Register Description :
* The Uncorrectable Error Severity register controls whether an individual uncorrectable error is reported by a function as a Non-fatal or Fatal error.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NBIF_UNCORR_ERR_SEVERITY_Reserved_3_0_OFFSET      0
#define NBIF_UNCORR_ERR_SEVERITY_Reserved_3_0_MASK        0xf

// Bitfield Description : Data Link Protocol Error Severity.
#define NBIF_UNCORR_ERR_SEVERITY_DLP_ERR_SEVERITY_OFFSET      4
#define NBIF_UNCORR_ERR_SEVERITY_DLP_ERR_SEVERITY_MASK        0x10

// Bitfield Description : Controls how the function reports a Surprise Down Error.
#define NBIF_UNCORR_ERR_SEVERITY_SURPDN_ERR_SEVERITY_OFFSET      5
#define NBIF_UNCORR_ERR_SEVERITY_SURPDN_ERR_SEVERITY_MASK        0x20

// Bitfield Description : 
#define NBIF_UNCORR_ERR_SEVERITY_Reserved_11_6_OFFSET      6
#define NBIF_UNCORR_ERR_SEVERITY_Reserved_11_6_MASK        0xfc0

// Bitfield Description : Poisoned TLP Severity.
#define NBIF_UNCORR_ERR_SEVERITY_PSN_ERR_SEVERITY_OFFSET      12
#define NBIF_UNCORR_ERR_SEVERITY_PSN_ERR_SEVERITY_MASK        0x1000

// Bitfield Description : Controls how the function reports a Flow Control Protocol Error.
#define NBIF_UNCORR_ERR_SEVERITY_FC_ERR_SEVERITY_OFFSET      13
#define NBIF_UNCORR_ERR_SEVERITY_FC_ERR_SEVERITY_MASK        0x2000

// Bitfield Description : Completion Timeout Error Severity.
#define NBIF_UNCORR_ERR_SEVERITY_CPL_TIMEOUT_SEVERITY_OFFSET      14
#define NBIF_UNCORR_ERR_SEVERITY_CPL_TIMEOUT_SEVERITY_MASK        0x4000

// Bitfield Description : Completer Abort Error Severity.
#define NBIF_UNCORR_ERR_SEVERITY_CPL_ABORT_ERR_SEVERITY_OFFSET      15
#define NBIF_UNCORR_ERR_SEVERITY_CPL_ABORT_ERR_SEVERITY_MASK        0x8000

// Bitfield Description : Unexpected Completion Error Severity.
#define NBIF_UNCORR_ERR_SEVERITY_UNEXP_CPL_SEVERITY_OFFSET      16
#define NBIF_UNCORR_ERR_SEVERITY_UNEXP_CPL_SEVERITY_MASK        0x10000

// Bitfield Description : Controls how the function reports a Receiver Overflow Error.
#define NBIF_UNCORR_ERR_SEVERITY_RCV_OVFL_SEVERITY_OFFSET      17
#define NBIF_UNCORR_ERR_SEVERITY_RCV_OVFL_SEVERITY_MASK        0x20000

// Bitfield Description : Malformed TLP Severity.
#define NBIF_UNCORR_ERR_SEVERITY_MAL_TLP_SEVERITY_OFFSET      18
#define NBIF_UNCORR_ERR_SEVERITY_MAL_TLP_SEVERITY_MASK        0x40000

// Bitfield Description : ECRC Error Severity.
#define NBIF_UNCORR_ERR_SEVERITY_ECRC_ERR_SEVERITY_OFFSET      19
#define NBIF_UNCORR_ERR_SEVERITY_ECRC_ERR_SEVERITY_MASK        0x80000

// Bitfield Description : Unsupported Request Error Severity.
#define NBIF_UNCORR_ERR_SEVERITY_UNSUPP_REQ_ERR_SEVERITY_OFFSET      20
#define NBIF_UNCORR_ERR_SEVERITY_UNSUPP_REQ_ERR_SEVERITY_MASK        0x100000

// Bitfield Description : ACS Violation Severity.
#define NBIF_UNCORR_ERR_SEVERITY_ACS_VIOLATION_SEVERITY_OFFSET      21
#define NBIF_UNCORR_ERR_SEVERITY_ACS_VIOLATION_SEVERITY_MASK        0x200000

// Bitfield Description : Controls how the function reports an Uncorrectable Internal Error. This bit is sticky.
#define NBIF_UNCORR_ERR_SEVERITY_UNCORR_INT_ERR_SEVERITY_OFFSET      22
#define NBIF_UNCORR_ERR_SEVERITY_UNCORR_INT_ERR_SEVERITY_MASK        0x400000

// Bitfield Description : Controls how the function reports a MC Blocked TLP Error. This bit is sticky.
#define NBIF_UNCORR_ERR_SEVERITY_MC_BLOCKED_TLP_SEVERITY_OFFSET      23
#define NBIF_UNCORR_ERR_SEVERITY_MC_BLOCKED_TLP_SEVERITY_MASK        0x800000

// Bitfield Description : Controls how the function reports an AtomicOp Egress Blocked Error.
#define NBIF_UNCORR_ERR_SEVERITY_ATOMICOP_EGRESS_BLOCKED_SEVERITY_OFFSET      24
#define NBIF_UNCORR_ERR_SEVERITY_ATOMICOP_EGRESS_BLOCKED_SEVERITY_MASK        0x1000000

// Bitfield Description : Controls how the function reports a TLP Prefix Blocked Error.
#define NBIF_UNCORR_ERR_SEVERITY_TLP_PREFIX_BLOCKED_ERR_SEVERITY_OFFSET      25
#define NBIF_UNCORR_ERR_SEVERITY_TLP_PREFIX_BLOCKED_ERR_SEVERITY_MASK        0x2000000

// Bitfield Description : Controls how the function reports a Poisoned TLP Egress Blocked Error.
#define NBIF_UNCORR_ERR_SEVERITY_POISONED_TLP_EGRESS_BLOCKED_SEVERITY_OFFSET      26
#define NBIF_UNCORR_ERR_SEVERITY_POISONED_TLP_EGRESS_BLOCKED_SEVERITY_MASK        0x4000000

// Bitfield Description : 
#define NBIF_UNCORR_ERR_SEVERITY_Reserved_31_27_OFFSET      27
#define NBIF_UNCORR_ERR_SEVERITY_Reserved_31_27_MASK        0xf8000000

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
} PCIE_UNCORR_ERR_SEVERITY_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_OFFSET       0x15c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x15c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_SEVERITY_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_SEVERITY_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_SEVERITY_OFFSET       0x15c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x15c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1014815cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1015015cUL


/***********************************************************
* Register Name : PCIE_UNCORR_ERR_STATUS
* Register Description :
* The Uncorrectable Error Status register reports error status of individual uncorrectable errors in a function.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define NBIF_UNCORR_ERR_STATUS_Reserved_3_0_OFFSET      0
#define NBIF_UNCORR_ERR_STATUS_Reserved_3_0_MASK        0xf

// Bitfield Description : Data Link Protocol Error Status.
#define NBIF_UNCORR_ERR_STATUS_DLP_ERR_STATUS_OFFSET      4
#define NBIF_UNCORR_ERR_STATUS_DLP_ERR_STATUS_MASK        0x10

// Bitfield Description : Indicates if the function detects a Surprise Down Error.
#define NBIF_UNCORR_ERR_STATUS_SURPDN_ERR_STATUS_OFFSET      5
#define NBIF_UNCORR_ERR_STATUS_SURPDN_ERR_STATUS_MASK        0x20

// Bitfield Description : 
#define NBIF_UNCORR_ERR_STATUS_Reserved_11_6_OFFSET      6
#define NBIF_UNCORR_ERR_STATUS_Reserved_11_6_MASK        0xfc0

// Bitfield Description : Poisoned TLP Status.
#define NBIF_UNCORR_ERR_STATUS_PSN_ERR_STATUS_OFFSET      12
#define NBIF_UNCORR_ERR_STATUS_PSN_ERR_STATUS_MASK        0x1000

// Bitfield Description : Indicates if the function detects a Flow Control Protocol Error.
#define NBIF_UNCORR_ERR_STATUS_FC_ERR_STATUS_OFFSET      13
#define NBIF_UNCORR_ERR_STATUS_FC_ERR_STATUS_MASK        0x2000

// Bitfield Description : Completion Timeout Status.
#define NBIF_UNCORR_ERR_STATUS_CPL_TIMEOUT_STATUS_OFFSET      14
#define NBIF_UNCORR_ERR_STATUS_CPL_TIMEOUT_STATUS_MASK        0x4000

// Bitfield Description : Completer Abort Status.
#define NBIF_UNCORR_ERR_STATUS_CPL_ABORT_ERR_STATUS_OFFSET      15
#define NBIF_UNCORR_ERR_STATUS_CPL_ABORT_ERR_STATUS_MASK        0x8000

// Bitfield Description : Unexpected Completion Status.
#define NBIF_UNCORR_ERR_STATUS_UNEXP_CPL_STATUS_OFFSET      16
#define NBIF_UNCORR_ERR_STATUS_UNEXP_CPL_STATUS_MASK        0x10000

// Bitfield Description : Indicates if the function detects a Receiver Overflow Error.
#define NBIF_UNCORR_ERR_STATUS_RCV_OVFL_STATUS_OFFSET      17
#define NBIF_UNCORR_ERR_STATUS_RCV_OVFL_STATUS_MASK        0x20000

// Bitfield Description : Malformed TLP Status.
#define NBIF_UNCORR_ERR_STATUS_MAL_TLP_STATUS_OFFSET      18
#define NBIF_UNCORR_ERR_STATUS_MAL_TLP_STATUS_MASK        0x40000

// Bitfield Description : ECRC Error Status.
#define NBIF_UNCORR_ERR_STATUS_ECRC_ERR_STATUS_OFFSET      19
#define NBIF_UNCORR_ERR_STATUS_ECRC_ERR_STATUS_MASK        0x80000

// Bitfield Description : Unsupported Request Error Status.
#define NBIF_UNCORR_ERR_STATUS_UNSUPP_REQ_ERR_STATUS_OFFSET      20
#define NBIF_UNCORR_ERR_STATUS_UNSUPP_REQ_ERR_STATUS_MASK        0x100000

// Bitfield Description : ACS Violation Error Status.
#define NBIF_UNCORR_ERR_STATUS_ACS_VIOLATION_STATUS_OFFSET      21
#define NBIF_UNCORR_ERR_STATUS_ACS_VIOLATION_STATUS_MASK        0x200000

// Bitfield Description : Uncorrectable Internal Error Status.
#define NBIF_UNCORR_ERR_STATUS_UNCORR_INT_ERR_STATUS_OFFSET      22
#define NBIF_UNCORR_ERR_STATUS_UNCORR_INT_ERR_STATUS_MASK        0x400000

// Bitfield Description : MC Blocked TLP Status.
#define NBIF_UNCORR_ERR_STATUS_MC_BLOCKED_TLP_STATUS_OFFSET      23
#define NBIF_UNCORR_ERR_STATUS_MC_BLOCKED_TLP_STATUS_MASK        0x800000

// Bitfield Description : AtomicOp Egress Blocked Status.
#define NBIF_UNCORR_ERR_STATUS_ATOMICOP_EGRESS_BLOCKED_STATUS_OFFSET      24
#define NBIF_UNCORR_ERR_STATUS_ATOMICOP_EGRESS_BLOCKED_STATUS_MASK        0x1000000

// Bitfield Description : Indicates if the function detects a TLP Prefix Blocked Error.
#define NBIF_UNCORR_ERR_STATUS_TLP_PREFIX_BLOCKED_ERR_STATUS_OFFSET      25
#define NBIF_UNCORR_ERR_STATUS_TLP_PREFIX_BLOCKED_ERR_STATUS_MASK        0x2000000

// Bitfield Description : Indicates if the function detects a Poisoned TLP Egress Blocked Error.
#define NBIF_UNCORR_ERR_STATUS_POISONED_TLP_EGRESS_BLOCKED_STATUS_OFFSET      26
#define NBIF_UNCORR_ERR_STATUS_POISONED_TLP_EGRESS_BLOCKED_STATUS_MASK        0x4000000

// Bitfield Description : 
#define NBIF_UNCORR_ERR_STATUS_Reserved_31_27_OFFSET      27
#define NBIF_UNCORR_ERR_STATUS_Reserved_31_27_MASK        0xf8000000

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
} PCIE_UNCORR_ERR_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_STATUS_OFFSET       0x154
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x154)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_STATUS_OFFSET       0x154
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x154)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10148154UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10150154UL


/***********************************************************
* Register Name : PCIE_VC0_RESOURCE_CAP
* Register Description :
* VC0 Resource Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the types of Port Arbitration supported by the VC resource. Bit 0 = Fixed; Bit 1 = WRR-32; Bit 2 = WRR-64; Bit 3 = WRR-128; Bit 4 = Timed WRR-128; Bit 5 = WRR-256.
#define NBIF_VC0_RESOURCE_CAP_PORT_ARB_CAP_OFFSET      0
#define NBIF_VC0_RESOURCE_CAP_PORT_ARB_CAP_MASK        0xff

// Bitfield Description : 
#define NBIF_VC0_RESOURCE_CAP_Reserved_14_8_OFFSET      8
#define NBIF_VC0_RESOURCE_CAP_Reserved_14_8_MASK        0x7f00

// Bitfield Description : Controls if the Virtual Channel is permitted to reject transactions for which the No Snoop Attribute is applicable, but is not set.
#define NBIF_VC0_RESOURCE_CAP_REJECT_SNOOP_TRANS_OFFSET      15
#define NBIF_VC0_RESOURCE_CAP_REJECT_SNOOP_TRANS_MASK        0x8000

// Bitfield Description : Indicates the maximum number of time slots (minus one) that the VC resource is capable of supporting, when configured for Timed WRR Port Arbitration.
#define NBIF_VC0_RESOURCE_CAP_MAX_TIME_SLOTS_OFFSET      16
#define NBIF_VC0_RESOURCE_CAP_MAX_TIME_SLOTS_MASK        0x7f0000

// Bitfield Description : 
#define NBIF_VC0_RESOURCE_CAP_Reserved_23_23_OFFSET      23
#define NBIF_VC0_RESOURCE_CAP_Reserved_23_23_MASK        0x800000

// Bitfield Description : Indicates the location of the Port Arbitration Table associated with the VC resource, or must be 0 if the Port Arbitration Table is not present.
#define NBIF_VC0_RESOURCE_CAP_PORT_ARB_TABLE_OFFSET_OFFSET      24
#define NBIF_VC0_RESOURCE_CAP_PORT_ARB_TABLE_OFFSET_MASK        0xff000000

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
} PCIE_VC0_RESOURCE_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_CAP_OFFSET       0x120
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x120)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_CAP_OFFSET       0x120
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x120)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x10148120UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VC0_RESOURCE_CAP_ADDRESS    0x10150120UL


/***********************************************************
* Register Name : PCIE_VC0_RESOURCE_CNTL
* Register Description :
* VC0 Resource Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the TCs that are mapped to the VC resource.
#define NBIF_VC0_RESOURCE_CNTL_TC_VC_MAP_TC0_OFFSET      0
#define NBIF_VC0_RESOURCE_CNTL_TC_VC_MAP_TC0_MASK        0x1

// Bitfield Description : Indicates the TCs that are mapped to the VC resource.
#define NBIF_VC0_RESOURCE_CNTL_TC_VC_MAP_TC1_7_OFFSET      1
#define NBIF_VC0_RESOURCE_CNTL_TC_VC_MAP_TC1_7_MASK        0xfe

// Bitfield Description : 
#define NBIF_VC0_RESOURCE_CNTL_Reserved_15_8_OFFSET      8
#define NBIF_VC0_RESOURCE_CNTL_Reserved_15_8_MASK        0xff00

// Bitfield Description : Updates the Port Arbitration logic from the Port Arbitration Table (if present) for the VC resource, when written with 1. Reads always return 0.
#define NBIF_VC0_RESOURCE_CNTL_LOAD_PORT_ARB_TABLE_OFFSET      16
#define NBIF_VC0_RESOURCE_CNTL_LOAD_PORT_ARB_TABLE_MASK        0x10000

// Bitfield Description : Configures the Port Arbitration for the VC resource by selecting one of the supported Port Arbitration schemes.
#define NBIF_VC0_RESOURCE_CNTL_PORT_ARB_SELECT_OFFSET      17
#define NBIF_VC0_RESOURCE_CNTL_PORT_ARB_SELECT_MASK        0xe0000

// Bitfield Description : 
#define NBIF_VC0_RESOURCE_CNTL_Reserved_23_20_OFFSET      20
#define NBIF_VC0_RESOURCE_CNTL_Reserved_23_20_MASK        0xf00000

// Bitfield Description : Assigns a VC ID to the VC resource.
#define NBIF_VC0_RESOURCE_CNTL_VC_ID_OFFSET      24
#define NBIF_VC0_RESOURCE_CNTL_VC_ID_MASK        0x7000000

// Bitfield Description : 
#define NBIF_VC0_RESOURCE_CNTL_Reserved_30_27_OFFSET      27
#define NBIF_VC0_RESOURCE_CNTL_Reserved_30_27_MASK        0x78000000

// Bitfield Description : Controls the enabling of a Virtual Channel.
#define NBIF_VC0_RESOURCE_CNTL_VC_ENABLE_OFFSET      31
#define NBIF_VC0_RESOURCE_CNTL_VC_ENABLE_MASK        0x80000000

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
} PCIE_VC0_RESOURCE_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_CNTL_OFFSET       0x124
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x124)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_CNTL_OFFSET       0x124
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x124)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x10148124UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VC0_RESOURCE_CNTL_ADDRESS    0x10150124UL


/***********************************************************
* Register Name : PCIE_VC0_RESOURCE_STATUS
* Register Description :
* VC0 Resource Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the contents of the Port Arbitration Table associated with the VC resource are coherent, or must be 0 if the Port Arbitration Table is not present.
#define NBIF_VC0_RESOURCE_STATUS_PORT_ARB_TABLE_STATUS_OFFSET      0
#define NBIF_VC0_RESOURCE_STATUS_PORT_ARB_TABLE_STATUS_MASK        0x1

// Bitfield Description : Indicates whether the Virtual Channel negotiation (initialization or disabling) is in pending state.
#define NBIF_VC0_RESOURCE_STATUS_VC_NEGOTIATION_PENDING_OFFSET      1
#define NBIF_VC0_RESOURCE_STATUS_VC_NEGOTIATION_PENDING_MASK        0x2

// Bitfield Description : 
#define NBIF_VC0_RESOURCE_STATUS_Reserved_15_2_OFFSET      2
#define NBIF_VC0_RESOURCE_STATUS_Reserved_15_2_MASK        0xfffc

typedef union {
  struct {
    UINT16                            PORT_ARB_TABLE_STATUS:1;
    UINT16                            VC_NEGOTIATION_PENDING:1;
    UINT16                            Reserved_15_2:14;
  } Field;
  UINT16 Value;
} PCIE_VC0_RESOURCE_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_STATUS_OFFSET       0x12a
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC0_RESOURCE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x12a)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_STATUS_OFFSET       0x12a
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC0_RESOURCE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x12a)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1014812aUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VC0_RESOURCE_STATUS_ADDRESS    0x1015012aUL


/***********************************************************
* Register Name : PCIE_VC1_RESOURCE_CAP
* Register Description :
* VC1 Resource Capability register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the types of Port Arbitration supported by the VC resource. Bit 0 = Fixed; Bit 1 = WRR-32; Bit 2 = WRR-64; Bit 3 = WRR-128; Bit 4 = Timed WRR-128; Bit 5 = WRR-256.
#define NBIF_VC1_RESOURCE_CAP_PORT_ARB_CAP_OFFSET      0
#define NBIF_VC1_RESOURCE_CAP_PORT_ARB_CAP_MASK        0xff

// Bitfield Description : 
#define NBIF_VC1_RESOURCE_CAP_Reserved_14_8_OFFSET      8
#define NBIF_VC1_RESOURCE_CAP_Reserved_14_8_MASK        0x7f00

// Bitfield Description : Controls if the Virtual Channel is permitted to reject transactions for which the No Snoop Attribute is applicable, but is not set.
#define NBIF_VC1_RESOURCE_CAP_REJECT_SNOOP_TRANS_OFFSET      15
#define NBIF_VC1_RESOURCE_CAP_REJECT_SNOOP_TRANS_MASK        0x8000

// Bitfield Description : Indicates the maximum number of time slots (minus one) that the VC resource is capable of supporting, when configured for Timed WRR Port Arbitration.
#define NBIF_VC1_RESOURCE_CAP_MAX_TIME_SLOTS_OFFSET      16
#define NBIF_VC1_RESOURCE_CAP_MAX_TIME_SLOTS_MASK        0x3f0000

// Bitfield Description : 
#define NBIF_VC1_RESOURCE_CAP_Reserved_23_22_OFFSET      22
#define NBIF_VC1_RESOURCE_CAP_Reserved_23_22_MASK        0xc00000

// Bitfield Description : Indicates the location of the Port Arbitration Table associated with the VC resource, or must be 0 if the Port Arbitration Table is not present.
#define NBIF_VC1_RESOURCE_CAP_PORT_ARB_TABLE_OFFSET_OFFSET      24
#define NBIF_VC1_RESOURCE_CAP_PORT_ARB_TABLE_OFFSET_MASK        0xff000000

typedef union {
  struct {
    UINT32                            PORT_ARB_CAP:8;
    UINT32                            Reserved_14_8:7;
    UINT32                            REJECT_SNOOP_TRANS:1;
    UINT32                            MAX_TIME_SLOTS:6;
    UINT32                            Reserved_23_22:2;
    UINT32                            PORT_ARB_TABLE_OFFSET:8;
  } Field;
  UINT32 Value;
} PCIE_VC1_RESOURCE_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_CAP_OFFSET       0x12c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x12c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_CAP_OFFSET       0x12c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x12c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1014812cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VC1_RESOURCE_CAP_ADDRESS    0x1015012cUL


/***********************************************************
* Register Name : PCIE_VC1_RESOURCE_CNTL
* Register Description :
* VC1 Resource Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the TCs that are mapped to the VC resource.
#define NBIF_VC1_RESOURCE_CNTL_TC_VC_MAP_TC0_OFFSET      0
#define NBIF_VC1_RESOURCE_CNTL_TC_VC_MAP_TC0_MASK        0x1

// Bitfield Description : Indicates the TCs that are mapped to the VC resource.
#define NBIF_VC1_RESOURCE_CNTL_TC_VC_MAP_TC1_7_OFFSET      1
#define NBIF_VC1_RESOURCE_CNTL_TC_VC_MAP_TC1_7_MASK        0xfe

// Bitfield Description : 
#define NBIF_VC1_RESOURCE_CNTL_Reserved_15_8_OFFSET      8
#define NBIF_VC1_RESOURCE_CNTL_Reserved_15_8_MASK        0xff00

// Bitfield Description : Updates the Port Arbitration logic from the Port Arbitration Table (if present) for the VC resource, when written with 1. Reads always return 0.
#define NBIF_VC1_RESOURCE_CNTL_LOAD_PORT_ARB_TABLE_OFFSET      16
#define NBIF_VC1_RESOURCE_CNTL_LOAD_PORT_ARB_TABLE_MASK        0x10000

// Bitfield Description : Configures the Port Arbitration for the VC resource by selecting one of the supported Port Arbitration schemes.
#define NBIF_VC1_RESOURCE_CNTL_PORT_ARB_SELECT_OFFSET      17
#define NBIF_VC1_RESOURCE_CNTL_PORT_ARB_SELECT_MASK        0xe0000

// Bitfield Description : 
#define NBIF_VC1_RESOURCE_CNTL_Reserved_23_20_OFFSET      20
#define NBIF_VC1_RESOURCE_CNTL_Reserved_23_20_MASK        0xf00000

// Bitfield Description : Assigns a VC ID to the VC resource.
#define NBIF_VC1_RESOURCE_CNTL_VC_ID_OFFSET      24
#define NBIF_VC1_RESOURCE_CNTL_VC_ID_MASK        0x7000000

// Bitfield Description : 
#define NBIF_VC1_RESOURCE_CNTL_Reserved_30_27_OFFSET      27
#define NBIF_VC1_RESOURCE_CNTL_Reserved_30_27_MASK        0x78000000

// Bitfield Description : Controls the enabling of a Virtual Channel.
#define NBIF_VC1_RESOURCE_CNTL_VC_ENABLE_OFFSET      31
#define NBIF_VC1_RESOURCE_CNTL_VC_ENABLE_MASK        0x80000000

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
} PCIE_VC1_RESOURCE_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_CNTL_OFFSET       0x130
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x130)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_CNTL_OFFSET       0x130
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x130)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x10148130UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VC1_RESOURCE_CNTL_ADDRESS    0x10150130UL


/***********************************************************
* Register Name : PCIE_VC1_RESOURCE_STATUS
* Register Description :
* VC1 Resource Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the contents of the Port Arbitration Table associated with the VC resource are coherent, or must be 0 if the Port Arbitration Table is not present.
#define NBIF_VC1_RESOURCE_STATUS_PORT_ARB_TABLE_STATUS_OFFSET      0
#define NBIF_VC1_RESOURCE_STATUS_PORT_ARB_TABLE_STATUS_MASK        0x1

// Bitfield Description : Indicates whether the Virtual Channel negotiation (initialization or disabling) is in pending state.
#define NBIF_VC1_RESOURCE_STATUS_VC_NEGOTIATION_PENDING_OFFSET      1
#define NBIF_VC1_RESOURCE_STATUS_VC_NEGOTIATION_PENDING_MASK        0x2

// Bitfield Description : 
#define NBIF_VC1_RESOURCE_STATUS_Reserved_15_2_OFFSET      2
#define NBIF_VC1_RESOURCE_STATUS_Reserved_15_2_MASK        0xfffc

typedef union {
  struct {
    UINT16                            PORT_ARB_TABLE_STATUS:1;
    UINT16                            VC_NEGOTIATION_PENDING:1;
    UINT16                            Reserved_15_2:14;
  } Field;
  UINT16 Value;
} PCIE_VC1_RESOURCE_STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_STATUS_OFFSET       0x136
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC1_RESOURCE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x136)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_STATUS_OFFSET       0x136
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC1_RESOURCE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x136)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x10148136UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VC1_RESOURCE_STATUS_ADDRESS    0x10150136UL


/***********************************************************
* Register Name : PCIE_VC_ENH_CAP_LIST
* Register Description :
* Virtual Channel Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_VC_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_VC_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_VC_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_VC_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_VC_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_VC_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_VC_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC_ENH_CAP_LIST_OFFSET       0x110
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x110)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC_ENH_CAP_LIST_OFFSET       0x110
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x110)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x10148110UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VC_ENH_CAP_LIST_ADDRESS    0x10150110UL


/***********************************************************
* Register Name : PCIE_VENDOR_SPECIFIC1
* Register Description :
* Vendor-Specific Scratch register 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : PCIE scratch register.
#define NBIF_VENDOR_SPECIFIC1_SCRATCH_OFFSET      0
#define NBIF_VENDOR_SPECIFIC1_SCRATCH_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SCRATCH:32;
  } Field;
  UINT32 Value;
} PCIE_VENDOR_SPECIFIC1_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC1_OFFSET       0x108
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x108)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC1_OFFSET       0x108
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x108)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10148108UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10150108UL


/***********************************************************
* Register Name : PCIE_VENDOR_SPECIFIC2
* Register Description :
* Vendor-Specific Scratch register 2.
* Visibility : 0x2
************************************************************/

// Bitfield Description : PCIE scratch register.
#define NBIF_VENDOR_SPECIFIC2_SCRATCH_OFFSET      0
#define NBIF_VENDOR_SPECIFIC2_SCRATCH_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            SCRATCH:32;
  } Field;
  UINT32 Value;
} PCIE_VENDOR_SPECIFIC2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC2_OFFSET       0x10c
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x10c)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC2_OFFSET       0x10c
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x10c)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1014810cUL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1015010cUL


/***********************************************************
* Register Name : PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST
* Register Description :
* Vendor-Specific Extended Capability header.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the extended capability.
#define NBIF_VENDOR_SPECIFIC_ENH_CAP_LIST_CAP_ID_OFFSET      0
#define NBIF_VENDOR_SPECIFIC_ENH_CAP_LIST_CAP_ID_MASK        0xffff

// Bitfield Description : A PCI-SIG defined version number that indicates the version of the capability structure.
#define NBIF_VENDOR_SPECIFIC_ENH_CAP_LIST_CAP_VER_OFFSET      16
#define NBIF_VENDOR_SPECIFIC_ENH_CAP_LIST_CAP_VER_MASK        0xf0000

// Bitfield Description : Contains the offset to the next PCI Express capability structure or 000h if no other items exist.
#define NBIF_VENDOR_SPECIFIC_ENH_CAP_LIST_NEXT_PTR_OFFSET      20
#define NBIF_VENDOR_SPECIFIC_ENH_CAP_LIST_NEXT_PTR_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            CAP_ID:16;
    UINT32                            CAP_VER:4;
    UINT32                            NEXT_PTR:12;
  } Field;
  UINT32 Value;
} PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_OFFSET       0x100
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x100)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_OFFSET       0x100
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x100)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10148100UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10150100UL


/***********************************************************
* Register Name : PCIE_VENDOR_SPECIFIC_HDR
* Register Description :
* Vendor Specific Header register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Vendor-defined ID number.
#define NBIF_VENDOR_SPECIFIC_HDR_VSEC_ID_OFFSET      0
#define NBIF_VENDOR_SPECIFIC_HDR_VSEC_ID_MASK        0xffff

// Bitfield Description : Vendor-defined revision number.
#define NBIF_VENDOR_SPECIFIC_HDR_VSEC_REV_OFFSET      16
#define NBIF_VENDOR_SPECIFIC_HDR_VSEC_REV_MASK        0xf0000

// Bitfield Description : Number of bytes in the entire VSEC structure.
#define NBIF_VENDOR_SPECIFIC_HDR_VSEC_LENGTH_OFFSET      20
#define NBIF_VENDOR_SPECIFIC_HDR_VSEC_LENGTH_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            VSEC_ID:16;
    UINT32                            VSEC_REV:4;
    UINT32                            VSEC_LENGTH:12;
  } Field;
  UINT32 Value;
} PCIE_VENDOR_SPECIFIC_HDR_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_OFFSET       0x104
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x104)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_OFFSET       0x104
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x104)

#define SMN_DEV1_FUNC0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10148104UL

#define SMN_DEV2_FUNC0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10150104UL


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
} PMI_CAP_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_CAP_OFFSET       0x52
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x52)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_CAP_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_CAP_OFFSET       0x52
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x52)

#define SMN_DEV1_FUNC0_NBIF0_PMI_CAP_ADDRESS    0x10148052UL

#define SMN_DEV2_FUNC0_NBIF0_PMI_CAP_ADDRESS    0x10150052UL


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
} PMI_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_CAP_LIST_OFFSET       0x50
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x50)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_CAP_LIST_OFFSET       0x50
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x50)

#define SMN_DEV1_FUNC0_NBIF0_PMI_CAP_LIST_ADDRESS    0x10148050UL

#define SMN_DEV2_FUNC0_NBIF0_PMI_CAP_LIST_ADDRESS    0x10150050UL


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

// Bitfield Description : Controls a function's ability to generate a PME.
#define PMI_STATUS_CNTL_PME_EN_OFFSET      8
#define PMI_STATUS_CNTL_PME_EN_MASK        0x100

// Bitfield Description : Data Select.
#define PMI_STATUS_CNTL_DATA_SELECT_OFFSET      9
#define PMI_STATUS_CNTL_DATA_SELECT_MASK        0x1e00

// Bitfield Description : Data Scale.
#define PMI_STATUS_CNTL_DATA_SCALE_OFFSET      13
#define PMI_STATUS_CNTL_DATA_SCALE_MASK        0x6000

// Bitfield Description : Indicates if a PME is pending internally in the function.
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
} PMI_STATUS_CNTL_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_STATUS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_STATUS_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_STATUS_CNTL_OFFSET       0x54
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PMI_STATUS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x54)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_STATUS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_STATUS_CNTL_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_STATUS_CNTL_OFFSET       0x54
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PMI_STATUS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x54)

#define SMN_DEV1_FUNC0_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10148054UL

#define SMN_DEV2_FUNC0_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10150054UL


/***********************************************************
* Register Name : PROG_INTERFACE
* Register Description :
* The Programming Interface register is used with the Base Class Code and the Sub-Class Code registers.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define PROG_INTERFACE_PROG_INTERFACE_OFFSET      0
#define PROG_INTERFACE_PROG_INTERFACE_MASK        0xff

typedef union {
  struct {
    UINT8                            PROG_INTERFACE:8;
  } Field;
  UINT8 Value;
} PROG_INTERFACE_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PROG_INTERFACE_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PROG_INTERFACE_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PROG_INTERFACE_OFFSET       0x9
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_PROG_INTERFACE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x9)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PROG_INTERFACE_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PROG_INTERFACE_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PROG_INTERFACE_OFFSET       0x9
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_PROG_INTERFACE_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x9)

#define SMN_DEV1_FUNC0_NBIF0_PROG_INTERFACE_ADDRESS    0x10148009UL

#define SMN_DEV2_FUNC0_NBIF0_PROG_INTERFACE_ADDRESS    0x10150009UL


/***********************************************************
* Register Name : REVISION_ID
* Register Description :
* Indicates a device specific revision identifier.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Major revision ID.
#define REVISION_ID_MINOR_REV_ID_OFFSET      0
#define REVISION_ID_MINOR_REV_ID_MASK        0xf

// Bitfield Description : Minor revision ID.
#define REVISION_ID_MAJOR_REV_ID_OFFSET      4
#define REVISION_ID_MAJOR_REV_ID_MASK        0xf0

typedef union {
  struct {
    UINT8                            MINOR_REV_ID:4;
    UINT8                            MAJOR_REV_ID:4;
  } Field;
  UINT8 Value;
} REVISION_ID_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_REVISION_ID_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x8)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_REVISION_ID_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x8)

#define SMN_DEV1_FUNC0_NBIF0_REVISION_ID_ADDRESS    0x10148008UL

#define SMN_DEV2_FUNC0_NBIF0_REVISION_ID_ADDRESS    0x10150008UL


/***********************************************************
* Register Name : ROM_BASE_ADDR
* Register Description :
* PCI CFG Expansion ROM BAR.
* Visibility : 0x2
************************************************************/


/***********************************************************
* Register Name : RTM1_PARITY_MISMATCH_STATUS_16GT
* Register Description :
* 16.0 GT/s First Retimer Data Parity Mismatch Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the corresponding lane detected a data parity mismatch from the first retimer (if present) on the path. This field is a bit vector that covers all supported lanes. Bit 0 corresponds to the default lane 0, and bit ordering is invariant to link width and lane reversal negotiation. This field is sticky.
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
} RTM1_PARITY_MISMATCH_STATUS_16GT_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTM1_PARITY_MISMATCH_STATUS_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTM1_PARITY_MISMATCH_STATUS_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTM1_PARITY_MISMATCH_STATUS_16GT_OFFSET       0x424
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x424)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTM1_PARITY_MISMATCH_STATUS_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTM1_PARITY_MISMATCH_STATUS_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTM1_PARITY_MISMATCH_STATUS_16GT_OFFSET       0x424
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x424)

#define SMN_DEV1_FUNC0_NBIF0_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x10148424UL

#define SMN_DEV2_FUNC0_NBIF0_RTM1_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x10150424UL


/***********************************************************
* Register Name : RTM2_PARITY_MISMATCH_STATUS_16GT
* Register Description :
* 16.0 GT/s Second Retimer Data Parity Mismatch Status register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates if the corresponding lane detected a data parity mismatch from the second retimer (if present) on the path. This field is a bit vector that covers all supported lanes. Bit 0 corresponds to the default lane 0, and bit ordering is invariant to link width and lane reversal negotiation. This field is sticky.
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
} RTM2_PARITY_MISMATCH_STATUS_16GT_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTM2_PARITY_MISMATCH_STATUS_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTM2_PARITY_MISMATCH_STATUS_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTM2_PARITY_MISMATCH_STATUS_16GT_OFFSET       0x428
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x428)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTM2_PARITY_MISMATCH_STATUS_16GT_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTM2_PARITY_MISMATCH_STATUS_16GT_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTM2_PARITY_MISMATCH_STATUS_16GT_OFFSET       0x428
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x428)

#define SMN_DEV1_FUNC0_NBIF0_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x10148428UL

#define SMN_DEV2_FUNC0_NBIF0_RTM2_PARITY_MISMATCH_STATUS_16GT_ADDRESS    0x10150428UL


/***********************************************************
* Register Name : RTR_DATA1
* Register Description :
* Readiness Time Reporting 1 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When Valid bit is set, indicates the time required for the function to be configuration ready, following a conventional reset. The lower nine bits indicate the value. The upper three bits indicate the scale used for the value, encoded as: 0 = 1 ns; 1 = 32 ns; 2 = 1024 ns; 3 = 32768 ns; 4 = 1048576 ns; 5 = 33554432 ns.
#define RTR_DATA1_RESET_TIME_OFFSET      0
#define RTR_DATA1_RESET_TIME_MASK        0xfff

// Bitfield Description : When Valid bit is set, indicates the time required for the function to be configuration ready, following the Downstream Port reporting Data Link Layer Active is set. Encodings are the same as for RESET_TIME.
#define RTR_DATA1_DLUP_TIME_OFFSET      12
#define RTR_DATA1_DLUP_TIME_MASK        0xfff000

// Bitfield Description : 
#define RTR_DATA1_Reserved_30_24_OFFSET      24
#define RTR_DATA1_Reserved_30_24_MASK        0x7f000000

// Bitfield Description : Indicates if the time values reported in this capability structure are valid.
#define RTR_DATA1_VALID_OFFSET      31
#define RTR_DATA1_VALID_MASK        0x80000000

typedef union {
  struct {
    UINT32                            RESET_TIME:12;
    UINT32                            DLUP_TIME:12;
    UINT32                            Reserved_30_24:7;
    UINT32                            VALID:1;
  } Field;
  UINT32 Value;
} RTR_DATA1_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTR_DATA1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTR_DATA1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTR_DATA1_OFFSET       0x574
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTR_DATA1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x574)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTR_DATA1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTR_DATA1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTR_DATA1_OFFSET       0x574
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTR_DATA1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x574)

#define SMN_DEV1_FUNC0_NBIF0_RTR_DATA1_ADDRESS    0x10148574UL

#define SMN_DEV2_FUNC0_NBIF0_RTR_DATA1_ADDRESS    0x10150574UL


/***********************************************************
* Register Name : RTR_DATA2
* Register Description :
* Readiness Time Reporting 2 register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : When Valid bit is set and Function Level Reset Capability is set, indicates the time required for the function to be configuration ready, following a FLR. Encodings are the same as for RESET_TIME.
#define RTR_DATA2_FLR_TIME_OFFSET      0
#define RTR_DATA2_FLR_TIME_MASK        0xfff

// Bitfield Description : When Valid bit is set and Immediate Readiness on Return to D0 bit is clear, indicates the time required for the function to be configuration ready, following a D3hot to D0 transition. Encodings are the same as for RESET_TIME.
#define RTR_DATA2_D3HOTD0_TIME_OFFSET      12
#define RTR_DATA2_D3HOTD0_TIME_MASK        0xfff000

// Bitfield Description : 
#define RTR_DATA2_Reserved_31_24_OFFSET      24
#define RTR_DATA2_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            FLR_TIME:12;
    UINT32                            D3HOTD0_TIME:12;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} RTR_DATA2_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTR_DATA2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTR_DATA2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTR_DATA2_OFFSET       0x578
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_RTR_DATA2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x578)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTR_DATA2_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTR_DATA2_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTR_DATA2_OFFSET       0x578
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_RTR_DATA2_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x578)

#define SMN_DEV1_FUNC0_NBIF0_RTR_DATA2_ADDRESS    0x10148578UL

#define SMN_DEV2_FUNC0_NBIF0_RTR_DATA2_ADDRESS    0x10150578UL


/***********************************************************
* Register Name : SATA_CAP_0
* Register Description :
* Serial ATA Capability register 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : A PCI-SIG defined ID number that indicates the nature and format of the capability.
#define SATA_CAP_0_CAP_ID_OFFSET      0
#define SATA_CAP_0_CAP_ID_MASK        0xff

// Bitfield Description : Pointer to the next item in the capabilities list, or 00h if no other items exist.
#define SATA_CAP_0_NEXT_PTR_OFFSET      8
#define SATA_CAP_0_NEXT_PTR_MASK        0xff00

// Bitfield Description : Indicates minor revision number of this capability structure.
#define SATA_CAP_0_SATA_CAP_MINOR_REV_OFFSET      16
#define SATA_CAP_0_SATA_CAP_MINOR_REV_MASK        0xf0000

// Bitfield Description : Indicates major revision number of this capability structure.
#define SATA_CAP_0_SATA_CAP_MAJOR_REV_OFFSET      20
#define SATA_CAP_0_SATA_CAP_MAJOR_REV_MASK        0xf00000

// Bitfield Description : Reserved.
#define SATA_CAP_0_SATA_CAP_RESERVED1_OFFSET      24
#define SATA_CAP_0_SATA_CAP_RESERVED1_MASK        0xff000000

typedef union {
  struct {
    UINT32                            CAP_ID:8;
    UINT32                            NEXT_PTR:8;
    UINT32                            SATA_CAP_MINOR_REV:4;
    UINT32                            SATA_CAP_MAJOR_REV:4;
    UINT32                            SATA_CAP_RESERVED1:8;
  } Field;
  UINT32 Value;
} SATA_CAP_0_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_CAP_0_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_CAP_0_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_CAP_0_OFFSET       0xd0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_CAP_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd0)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_CAP_0_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_CAP_0_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_CAP_0_OFFSET       0xd0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_CAP_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd0)

#define SMN_DEV1_FUNC0_NBIF0_SATA_CAP_0_ADDRESS    0x101480d0UL

#define SMN_DEV2_FUNC0_NBIF0_SATA_CAP_0_ADDRESS    0x101500d0UL


/***********************************************************
* Register Name : SATA_CAP_1
* Register Description :
* Serial ATA Capability register 1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the BAR whose allocated address range contains the Index-Data pair.
#define SATA_CAP_1_SATA_CAP_BAR_LOC_OFFSET      0
#define SATA_CAP_1_SATA_CAP_BAR_LOC_MASK        0xf

// Bitfield Description : When SATA_CAP_BAR_LOC field is 0 to 5, indicates the offset from the address value contained in the BAR defined by SATA_CAP_BAR_LOC field, to point to the start of the Index-Data pair. The offset value must be padded with 2 bits of zero, to form a DWORD-aligned offset.
#define SATA_CAP_1_SATA_CAP_BAR_OFFSET_OFFSET      4
#define SATA_CAP_1_SATA_CAP_BAR_OFFSET_MASK        0xfffff0

// Bitfield Description : Reserved.
#define SATA_CAP_1_SATA_CAP_RESERVED2_OFFSET      24
#define SATA_CAP_1_SATA_CAP_RESERVED2_MASK        0xff000000

typedef union {
  struct {
    UINT32                            SATA_CAP_BAR_LOC:4;
    UINT32                            SATA_CAP_BAR_OFFSET:20;
    UINT32                            SATA_CAP_RESERVED2:8;
  } Field;
  UINT32 Value;
} SATA_CAP_1_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_CAP_1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_CAP_1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_CAP_1_OFFSET       0xd4
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_CAP_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd4)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_CAP_1_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_CAP_1_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_CAP_1_OFFSET       0xd4
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_CAP_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd4)

#define SMN_DEV1_FUNC0_NBIF0_SATA_CAP_1_ADDRESS    0x101480d4UL

#define SMN_DEV2_FUNC0_NBIF0_SATA_CAP_1_ADDRESS    0x101500d4UL


/***********************************************************
* Register Name : SATA_IDP_DATA
* Register Description :
* IDP Data register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : For reads, returns the value in the memory mapped AHCI register pointed to by the IDP_INDEX field. For writes, this value is written to the memory mapped AHCI register pointed to by the IDP_INDEX field.
#define SATA_IDP_DATA_IDP_DATA_OFFSET      0
#define SATA_IDP_DATA_IDP_DATA_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            IDP_DATA:32;
  } Field;
  UINT32 Value;
} SATA_IDP_DATA_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_IDP_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_IDP_DATA_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_IDP_DATA_OFFSET       0xdc
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_IDP_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xdc)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_IDP_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_IDP_DATA_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_IDP_DATA_OFFSET       0xdc
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_IDP_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xdc)

#define SMN_DEV1_FUNC0_NBIF0_SATA_IDP_DATA_ADDRESS    0x101480dcUL

#define SMN_DEV2_FUNC0_NBIF0_SATA_IDP_DATA_ADDRESS    0x101500dcUL


/***********************************************************
* Register Name : SATA_IDP_INDEX
* Register Description :
* IDP Index register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Reserved.
#define SATA_IDP_INDEX_IDP_RESERVED1_OFFSET      0
#define SATA_IDP_INDEX_IDP_RESERVED1_MASK        0x3

// Bitfield Description : Selects the offset of the memory mapped AHCI register accessed using the IDP_DATA field. The offset value must be padded with 2 bits of zero, to form a DWORD-aligned offset.
#define SATA_IDP_INDEX_IDP_INDEX_OFFSET      2
#define SATA_IDP_INDEX_IDP_INDEX_MASK        0xffc

// Bitfield Description : Reserved.
#define SATA_IDP_INDEX_IDP_RESERVED2_OFFSET      12
#define SATA_IDP_INDEX_IDP_RESERVED2_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            IDP_RESERVED1:2;
    UINT32                            IDP_INDEX:10;
    UINT32                            IDP_RESERVED2:20;
  } Field;
  UINT32 Value;
} SATA_IDP_INDEX_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_IDP_INDEX_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_IDP_INDEX_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_IDP_INDEX_OFFSET       0xd8
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SATA_IDP_INDEX_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd8)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_IDP_INDEX_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_IDP_INDEX_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_IDP_INDEX_OFFSET       0xd8
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SATA_IDP_INDEX_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xd8)

#define SMN_DEV1_FUNC0_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101480d8UL

#define SMN_DEV2_FUNC0_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101500d8UL


/***********************************************************
* Register Name : SBRN
* Register Description :
* Serial Bus Release Number register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Indicates the Serial Bus Specification Release Number.
#define SBRN_SBRN_OFFSET      0
#define SBRN_SBRN_MASK        0xff

typedef union {
  struct {
    UINT8                            SBRN:8;
  } Field;
  UINT8 Value;
} SBRN_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SBRN_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SBRN_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SBRN_OFFSET       0x60
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SBRN_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x60)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SBRN_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SBRN_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SBRN_OFFSET       0x60
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SBRN_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x60)

#define SMN_DEV1_FUNC0_NBIF0_SBRN_ADDRESS    0x10148060UL

#define SMN_DEV2_FUNC0_NBIF0_SBRN_ADDRESS    0x10150060UL


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

// Bitfield Description : Indicates the presence of a capability list item. This bit must be 1 for PCI Express.
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
} STATUS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_STATUS_OFFSET       0x6
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x6)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_STATUS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_STATUS_OFFSET       0x6
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x6)

#define SMN_DEV1_FUNC0_NBIF0_STATUS_ADDRESS    0x10148006UL

#define SMN_DEV2_FUNC0_NBIF0_STATUS_ADDRESS    0x10150006UL


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
} SUB_CLASS_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SUB_CLASS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SUB_CLASS_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0xa)

#define SMN_DEV1_FUNC0_NBIF0_SUB_CLASS_ADDRESS    0x1014800aUL

#define SMN_DEV2_FUNC0_NBIF0_SUB_CLASS_ADDRESS    0x1015000aUL


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
} VENDOR_CAP_LIST_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_VENDOR_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_VENDOR_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_VENDOR_CAP_LIST_OFFSET       0x48
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_VENDOR_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x48)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_VENDOR_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_VENDOR_CAP_LIST_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_VENDOR_CAP_LIST_OFFSET       0x48
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_VENDOR_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x48)

#define SMN_DEV1_FUNC0_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10148048UL

#define SMN_DEV2_FUNC0_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10150048UL


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
} VENDOR_ID_NBIFEPF0CFG_STRUCT;

#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_VENDOR_ID_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_FUNC0_DEV2_NBIF0_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_VENDOR_ID_FUNC         0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_FUNC0_DEV1_NBIF0_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 0, 0x0)

#define SMN_DEV1_FUNC0_NBIF0_VENDOR_ID_ADDRESS    0x10148000UL

#define SMN_DEV2_FUNC0_NBIF0_VENDOR_ID_ADDRESS    0x10150000UL

#endif /* _NBIFEPF0CFG_H_ */
