/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _NBIFEPFNCFG_H_
#define _NBIFEPFNCFG_H_


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
} ADAPTER_ID_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_ADAPTER_ID_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_ADAPTER_ID_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2c)

#define SMN_DEV0_FUNC2_NBIF0_ADAPTER_ID_ADDRESS    0x1014202cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_ADAPTER_ID_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x2c)

#define SMN_DEV0_FUNC3_NBIF0_ADAPTER_ID_ADDRESS    0x1014302cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_ADAPTER_ID_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x2c)

#define SMN_DEV0_FUNC4_NBIF0_ADAPTER_ID_ADDRESS    0x1014402cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_ADAPTER_ID_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x2c)

#define SMN_DEV0_FUNC5_NBIF0_ADAPTER_ID_ADDRESS    0x1014502cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_ADAPTER_ID_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x2c)

#define SMN_DEV0_FUNC6_NBIF0_ADAPTER_ID_ADDRESS    0x1014602cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_ADAPTER_ID_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_ADAPTER_ID_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_ADAPTER_ID_OFFSET       0x2c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_ADAPTER_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x2c)

#define SMN_DEV0_FUNC7_NBIF0_ADAPTER_ID_ADDRESS    0x1014702cUL

#define SMN_DEV1_FUNC1_NBIF0_ADAPTER_ID_ADDRESS    0x1014902cUL

#define SMN_DEV1_FUNC2_NBIF0_ADAPTER_ID_ADDRESS    0x1014a02cUL

#define SMN_DEV1_FUNC3_NBIF0_ADAPTER_ID_ADDRESS    0x1014b02cUL

#define SMN_DEV2_FUNC1_NBIF0_ADAPTER_ID_ADDRESS    0x1015102cUL

#define SMN_DEV2_FUNC2_NBIF0_ADAPTER_ID_ADDRESS    0x1015202cUL

#define SMN_DEV2_FUNC3_NBIF0_ADAPTER_ID_ADDRESS    0x1015302cUL

#define SMN_DEV2_FUNC4_NBIF0_ADAPTER_ID_ADDRESS    0x1015402cUL

#define SMN_DEV2_FUNC5_NBIF0_ADAPTER_ID_ADDRESS    0x1015502cUL

#define SMN_DEV2_FUNC6_NBIF0_ADAPTER_ID_ADDRESS    0x1015602cUL


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
} ADAPTER_ID_W_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_ADAPTER_ID_W_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_ADAPTER_ID_W_OFFSET       0x4c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x4c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_ADAPTER_ID_W_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_ADAPTER_ID_W_OFFSET       0x4c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x4c)

#define SMN_DEV0_FUNC2_NBIF0_ADAPTER_ID_W_ADDRESS    0x1014204cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_ADAPTER_ID_W_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_ADAPTER_ID_W_OFFSET       0x4c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x4c)

#define SMN_DEV0_FUNC3_NBIF0_ADAPTER_ID_W_ADDRESS    0x1014304cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_ADAPTER_ID_W_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_ADAPTER_ID_W_OFFSET       0x4c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x4c)

#define SMN_DEV0_FUNC4_NBIF0_ADAPTER_ID_W_ADDRESS    0x1014404cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_ADAPTER_ID_W_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_ADAPTER_ID_W_OFFSET       0x4c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x4c)

#define SMN_DEV0_FUNC5_NBIF0_ADAPTER_ID_W_ADDRESS    0x1014504cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_ADAPTER_ID_W_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_ADAPTER_ID_W_OFFSET       0x4c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x4c)

#define SMN_DEV0_FUNC6_NBIF0_ADAPTER_ID_W_ADDRESS    0x1014604cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_ADAPTER_ID_W_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_ADAPTER_ID_W_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_ADAPTER_ID_W_OFFSET       0x4c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_ADAPTER_ID_W_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x4c)

#define SMN_DEV0_FUNC7_NBIF0_ADAPTER_ID_W_ADDRESS    0x1014704cUL

#define SMN_DEV1_FUNC1_NBIF0_ADAPTER_ID_W_ADDRESS    0x1014904cUL

#define SMN_DEV1_FUNC2_NBIF0_ADAPTER_ID_W_ADDRESS    0x1014a04cUL

#define SMN_DEV1_FUNC3_NBIF0_ADAPTER_ID_W_ADDRESS    0x1014b04cUL

#define SMN_DEV2_FUNC1_NBIF0_ADAPTER_ID_W_ADDRESS    0x1015104cUL

#define SMN_DEV2_FUNC2_NBIF0_ADAPTER_ID_W_ADDRESS    0x1015204cUL

#define SMN_DEV2_FUNC3_NBIF0_ADAPTER_ID_W_ADDRESS    0x1015304cUL

#define SMN_DEV2_FUNC4_NBIF0_ADAPTER_ID_W_ADDRESS    0x1015404cUL

#define SMN_DEV2_FUNC5_NBIF0_ADAPTER_ID_W_ADDRESS    0x1015504cUL

#define SMN_DEV2_FUNC6_NBIF0_ADAPTER_ID_W_ADDRESS    0x1015604cUL


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
} BASE_ADDR_1_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_1_OFFSET       0x10
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x10)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_1_OFFSET       0x10
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x10)

#define SMN_DEV0_FUNC2_NBIF0_BASE_ADDR_1_ADDRESS    0x10142010UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_1_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_1_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_1_OFFSET       0x10
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x10)

#define SMN_DEV0_FUNC3_NBIF0_BASE_ADDR_1_ADDRESS    0x10143010UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_1_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_1_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_1_OFFSET       0x10
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x10)

#define SMN_DEV0_FUNC4_NBIF0_BASE_ADDR_1_ADDRESS    0x10144010UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_1_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_1_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_1_OFFSET       0x10
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x10)

#define SMN_DEV0_FUNC5_NBIF0_BASE_ADDR_1_ADDRESS    0x10145010UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_1_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_1_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_1_OFFSET       0x10
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x10)

#define SMN_DEV0_FUNC6_NBIF0_BASE_ADDR_1_ADDRESS    0x10146010UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_1_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_1_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_1_OFFSET       0x10
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x10)

#define SMN_DEV0_FUNC7_NBIF0_BASE_ADDR_1_ADDRESS    0x10147010UL

#define SMN_DEV1_FUNC1_NBIF0_BASE_ADDR_1_ADDRESS    0x10149010UL

#define SMN_DEV1_FUNC2_NBIF0_BASE_ADDR_1_ADDRESS    0x1014a010UL

#define SMN_DEV1_FUNC3_NBIF0_BASE_ADDR_1_ADDRESS    0x1014b010UL

#define SMN_DEV2_FUNC1_NBIF0_BASE_ADDR_1_ADDRESS    0x10151010UL

#define SMN_DEV2_FUNC2_NBIF0_BASE_ADDR_1_ADDRESS    0x10152010UL

#define SMN_DEV2_FUNC3_NBIF0_BASE_ADDR_1_ADDRESS    0x10153010UL

#define SMN_DEV2_FUNC4_NBIF0_BASE_ADDR_1_ADDRESS    0x10154010UL

#define SMN_DEV2_FUNC5_NBIF0_BASE_ADDR_1_ADDRESS    0x10155010UL

#define SMN_DEV2_FUNC6_NBIF0_BASE_ADDR_1_ADDRESS    0x10156010UL


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
} BASE_ADDR_2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_2_OFFSET       0x14
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x14)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_2_OFFSET       0x14
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x14)

#define SMN_DEV0_FUNC2_NBIF0_BASE_ADDR_2_ADDRESS    0x10142014UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_2_OFFSET       0x14
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x14)

#define SMN_DEV0_FUNC3_NBIF0_BASE_ADDR_2_ADDRESS    0x10143014UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_2_OFFSET       0x14
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x14)

#define SMN_DEV0_FUNC4_NBIF0_BASE_ADDR_2_ADDRESS    0x10144014UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_2_OFFSET       0x14
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x14)

#define SMN_DEV0_FUNC5_NBIF0_BASE_ADDR_2_ADDRESS    0x10145014UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_2_OFFSET       0x14
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x14)

#define SMN_DEV0_FUNC6_NBIF0_BASE_ADDR_2_ADDRESS    0x10146014UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_2_OFFSET       0x14
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x14)

#define SMN_DEV0_FUNC7_NBIF0_BASE_ADDR_2_ADDRESS    0x10147014UL

#define SMN_DEV1_FUNC1_NBIF0_BASE_ADDR_2_ADDRESS    0x10149014UL

#define SMN_DEV1_FUNC2_NBIF0_BASE_ADDR_2_ADDRESS    0x1014a014UL

#define SMN_DEV1_FUNC3_NBIF0_BASE_ADDR_2_ADDRESS    0x1014b014UL

#define SMN_DEV2_FUNC1_NBIF0_BASE_ADDR_2_ADDRESS    0x10151014UL

#define SMN_DEV2_FUNC2_NBIF0_BASE_ADDR_2_ADDRESS    0x10152014UL

#define SMN_DEV2_FUNC3_NBIF0_BASE_ADDR_2_ADDRESS    0x10153014UL

#define SMN_DEV2_FUNC4_NBIF0_BASE_ADDR_2_ADDRESS    0x10154014UL

#define SMN_DEV2_FUNC5_NBIF0_BASE_ADDR_2_ADDRESS    0x10155014UL

#define SMN_DEV2_FUNC6_NBIF0_BASE_ADDR_2_ADDRESS    0x10156014UL


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
} BASE_ADDR_3_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_3_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_3_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_3_OFFSET       0x18
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x18)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_3_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_3_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_3_OFFSET       0x18
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x18)

#define SMN_DEV0_FUNC2_NBIF0_BASE_ADDR_3_ADDRESS    0x10142018UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_3_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_3_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_3_OFFSET       0x18
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x18)

#define SMN_DEV0_FUNC3_NBIF0_BASE_ADDR_3_ADDRESS    0x10143018UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_3_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_3_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_3_OFFSET       0x18
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x18)

#define SMN_DEV0_FUNC4_NBIF0_BASE_ADDR_3_ADDRESS    0x10144018UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_3_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_3_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_3_OFFSET       0x18
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x18)

#define SMN_DEV0_FUNC5_NBIF0_BASE_ADDR_3_ADDRESS    0x10145018UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_3_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_3_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_3_OFFSET       0x18
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x18)

#define SMN_DEV0_FUNC6_NBIF0_BASE_ADDR_3_ADDRESS    0x10146018UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_3_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_3_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_3_OFFSET       0x18
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x18)

#define SMN_DEV0_FUNC7_NBIF0_BASE_ADDR_3_ADDRESS    0x10147018UL

#define SMN_DEV1_FUNC1_NBIF0_BASE_ADDR_3_ADDRESS    0x10149018UL

#define SMN_DEV1_FUNC2_NBIF0_BASE_ADDR_3_ADDRESS    0x1014a018UL

#define SMN_DEV1_FUNC3_NBIF0_BASE_ADDR_3_ADDRESS    0x1014b018UL

#define SMN_DEV2_FUNC1_NBIF0_BASE_ADDR_3_ADDRESS    0x10151018UL

#define SMN_DEV2_FUNC2_NBIF0_BASE_ADDR_3_ADDRESS    0x10152018UL

#define SMN_DEV2_FUNC3_NBIF0_BASE_ADDR_3_ADDRESS    0x10153018UL

#define SMN_DEV2_FUNC4_NBIF0_BASE_ADDR_3_ADDRESS    0x10154018UL

#define SMN_DEV2_FUNC5_NBIF0_BASE_ADDR_3_ADDRESS    0x10155018UL

#define SMN_DEV2_FUNC6_NBIF0_BASE_ADDR_3_ADDRESS    0x10156018UL


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
} BASE_ADDR_4_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_4_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_4_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_4_OFFSET       0x1c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x1c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_4_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_4_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_4_OFFSET       0x1c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x1c)

#define SMN_DEV0_FUNC2_NBIF0_BASE_ADDR_4_ADDRESS    0x1014201cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_4_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_4_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_4_OFFSET       0x1c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x1c)

#define SMN_DEV0_FUNC3_NBIF0_BASE_ADDR_4_ADDRESS    0x1014301cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_4_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_4_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_4_OFFSET       0x1c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x1c)

#define SMN_DEV0_FUNC4_NBIF0_BASE_ADDR_4_ADDRESS    0x1014401cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_4_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_4_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_4_OFFSET       0x1c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x1c)

#define SMN_DEV0_FUNC5_NBIF0_BASE_ADDR_4_ADDRESS    0x1014501cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_4_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_4_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_4_OFFSET       0x1c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x1c)

#define SMN_DEV0_FUNC6_NBIF0_BASE_ADDR_4_ADDRESS    0x1014601cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_4_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_4_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_4_OFFSET       0x1c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x1c)

#define SMN_DEV0_FUNC7_NBIF0_BASE_ADDR_4_ADDRESS    0x1014701cUL

#define SMN_DEV1_FUNC1_NBIF0_BASE_ADDR_4_ADDRESS    0x1014901cUL

#define SMN_DEV1_FUNC2_NBIF0_BASE_ADDR_4_ADDRESS    0x1014a01cUL

#define SMN_DEV1_FUNC3_NBIF0_BASE_ADDR_4_ADDRESS    0x1014b01cUL

#define SMN_DEV2_FUNC1_NBIF0_BASE_ADDR_4_ADDRESS    0x1015101cUL

#define SMN_DEV2_FUNC2_NBIF0_BASE_ADDR_4_ADDRESS    0x1015201cUL

#define SMN_DEV2_FUNC3_NBIF0_BASE_ADDR_4_ADDRESS    0x1015301cUL

#define SMN_DEV2_FUNC4_NBIF0_BASE_ADDR_4_ADDRESS    0x1015401cUL

#define SMN_DEV2_FUNC5_NBIF0_BASE_ADDR_4_ADDRESS    0x1015501cUL

#define SMN_DEV2_FUNC6_NBIF0_BASE_ADDR_4_ADDRESS    0x1015601cUL


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
} BASE_ADDR_5_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_5_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_5_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_5_OFFSET       0x20
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x20)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_5_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_5_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_5_OFFSET       0x20
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x20)

#define SMN_DEV0_FUNC2_NBIF0_BASE_ADDR_5_ADDRESS    0x10142020UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_5_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_5_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_5_OFFSET       0x20
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x20)

#define SMN_DEV0_FUNC3_NBIF0_BASE_ADDR_5_ADDRESS    0x10143020UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_5_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_5_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_5_OFFSET       0x20
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x20)

#define SMN_DEV0_FUNC4_NBIF0_BASE_ADDR_5_ADDRESS    0x10144020UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_5_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_5_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_5_OFFSET       0x20
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x20)

#define SMN_DEV0_FUNC5_NBIF0_BASE_ADDR_5_ADDRESS    0x10145020UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_5_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_5_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_5_OFFSET       0x20
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x20)

#define SMN_DEV0_FUNC6_NBIF0_BASE_ADDR_5_ADDRESS    0x10146020UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_5_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_5_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_5_OFFSET       0x20
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x20)

#define SMN_DEV0_FUNC7_NBIF0_BASE_ADDR_5_ADDRESS    0x10147020UL

#define SMN_DEV1_FUNC1_NBIF0_BASE_ADDR_5_ADDRESS    0x10149020UL

#define SMN_DEV1_FUNC2_NBIF0_BASE_ADDR_5_ADDRESS    0x1014a020UL

#define SMN_DEV1_FUNC3_NBIF0_BASE_ADDR_5_ADDRESS    0x1014b020UL

#define SMN_DEV2_FUNC1_NBIF0_BASE_ADDR_5_ADDRESS    0x10151020UL

#define SMN_DEV2_FUNC2_NBIF0_BASE_ADDR_5_ADDRESS    0x10152020UL

#define SMN_DEV2_FUNC3_NBIF0_BASE_ADDR_5_ADDRESS    0x10153020UL

#define SMN_DEV2_FUNC4_NBIF0_BASE_ADDR_5_ADDRESS    0x10154020UL

#define SMN_DEV2_FUNC5_NBIF0_BASE_ADDR_5_ADDRESS    0x10155020UL

#define SMN_DEV2_FUNC6_NBIF0_BASE_ADDR_5_ADDRESS    0x10156020UL


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
} BASE_ADDR_6_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_6_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_6_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_6_OFFSET       0x24
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_ADDR_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x24)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_6_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_6_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_6_OFFSET       0x24
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_ADDR_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x24)

#define SMN_DEV0_FUNC2_NBIF0_BASE_ADDR_6_ADDRESS    0x10142024UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_6_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_6_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_6_OFFSET       0x24
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_ADDR_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x24)

#define SMN_DEV0_FUNC3_NBIF0_BASE_ADDR_6_ADDRESS    0x10143024UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_6_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_6_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_6_OFFSET       0x24
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_ADDR_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x24)

#define SMN_DEV0_FUNC4_NBIF0_BASE_ADDR_6_ADDRESS    0x10144024UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_6_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_6_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_6_OFFSET       0x24
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_ADDR_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x24)

#define SMN_DEV0_FUNC5_NBIF0_BASE_ADDR_6_ADDRESS    0x10145024UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_6_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_6_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_6_OFFSET       0x24
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_ADDR_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x24)

#define SMN_DEV0_FUNC6_NBIF0_BASE_ADDR_6_ADDRESS    0x10146024UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_6_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_6_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_6_OFFSET       0x24
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_ADDR_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x24)

#define SMN_DEV0_FUNC7_NBIF0_BASE_ADDR_6_ADDRESS    0x10147024UL

#define SMN_DEV1_FUNC1_NBIF0_BASE_ADDR_6_ADDRESS    0x10149024UL

#define SMN_DEV1_FUNC2_NBIF0_BASE_ADDR_6_ADDRESS    0x1014a024UL

#define SMN_DEV1_FUNC3_NBIF0_BASE_ADDR_6_ADDRESS    0x1014b024UL

#define SMN_DEV2_FUNC1_NBIF0_BASE_ADDR_6_ADDRESS    0x10151024UL

#define SMN_DEV2_FUNC2_NBIF0_BASE_ADDR_6_ADDRESS    0x10152024UL

#define SMN_DEV2_FUNC3_NBIF0_BASE_ADDR_6_ADDRESS    0x10153024UL

#define SMN_DEV2_FUNC4_NBIF0_BASE_ADDR_6_ADDRESS    0x10154024UL

#define SMN_DEV2_FUNC5_NBIF0_BASE_ADDR_6_ADDRESS    0x10155024UL

#define SMN_DEV2_FUNC6_NBIF0_BASE_ADDR_6_ADDRESS    0x10156024UL


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
} BASE_CLASS_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_CLASS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_CLASS_OFFSET       0xb
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BASE_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xb)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_CLASS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_CLASS_OFFSET       0xb
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BASE_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xb)

#define SMN_DEV0_FUNC2_NBIF0_BASE_CLASS_ADDRESS    0x1014200bUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_CLASS_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_CLASS_OFFSET       0xb
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BASE_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xb)

#define SMN_DEV0_FUNC3_NBIF0_BASE_CLASS_ADDRESS    0x1014300bUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_CLASS_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_CLASS_OFFSET       0xb
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BASE_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xb)

#define SMN_DEV0_FUNC4_NBIF0_BASE_CLASS_ADDRESS    0x1014400bUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_CLASS_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_CLASS_OFFSET       0xb
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BASE_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xb)

#define SMN_DEV0_FUNC5_NBIF0_BASE_CLASS_ADDRESS    0x1014500bUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_CLASS_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_CLASS_OFFSET       0xb
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BASE_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xb)

#define SMN_DEV0_FUNC6_NBIF0_BASE_CLASS_ADDRESS    0x1014600bUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_CLASS_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_CLASS_OFFSET       0xb
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BASE_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xb)

#define SMN_DEV0_FUNC7_NBIF0_BASE_CLASS_ADDRESS    0x1014700bUL

#define SMN_DEV1_FUNC1_NBIF0_BASE_CLASS_ADDRESS    0x1014900bUL

#define SMN_DEV1_FUNC2_NBIF0_BASE_CLASS_ADDRESS    0x1014a00bUL

#define SMN_DEV1_FUNC3_NBIF0_BASE_CLASS_ADDRESS    0x1014b00bUL

#define SMN_DEV2_FUNC1_NBIF0_BASE_CLASS_ADDRESS    0x1015100bUL

#define SMN_DEV2_FUNC2_NBIF0_BASE_CLASS_ADDRESS    0x1015200bUL

#define SMN_DEV2_FUNC3_NBIF0_BASE_CLASS_ADDRESS    0x1015300bUL

#define SMN_DEV2_FUNC4_NBIF0_BASE_CLASS_ADDRESS    0x1015400bUL

#define SMN_DEV2_FUNC5_NBIF0_BASE_CLASS_ADDRESS    0x1015500bUL

#define SMN_DEV2_FUNC6_NBIF0_BASE_CLASS_ADDRESS    0x1015600bUL


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
} BIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BIST_OFFSET       0xf
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_BIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xf)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BIST_OFFSET       0xf
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_BIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xf)

#define SMN_DEV0_FUNC2_NBIF0_BIST_ADDRESS    0x1014200fUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BIST_OFFSET       0xf
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_BIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xf)

#define SMN_DEV0_FUNC3_NBIF0_BIST_ADDRESS    0x1014300fUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BIST_OFFSET       0xf
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_BIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xf)

#define SMN_DEV0_FUNC4_NBIF0_BIST_ADDRESS    0x1014400fUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BIST_OFFSET       0xf
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_BIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xf)

#define SMN_DEV0_FUNC5_NBIF0_BIST_ADDRESS    0x1014500fUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BIST_OFFSET       0xf
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_BIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xf)

#define SMN_DEV0_FUNC6_NBIF0_BIST_ADDRESS    0x1014600fUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BIST_OFFSET       0xf
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_BIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xf)

#define SMN_DEV0_FUNC7_NBIF0_BIST_ADDRESS    0x1014700fUL

#define SMN_DEV1_FUNC1_NBIF0_BIST_ADDRESS    0x1014900fUL

#define SMN_DEV1_FUNC2_NBIF0_BIST_ADDRESS    0x1014a00fUL

#define SMN_DEV1_FUNC3_NBIF0_BIST_ADDRESS    0x1014b00fUL

#define SMN_DEV2_FUNC1_NBIF0_BIST_ADDRESS    0x1015100fUL

#define SMN_DEV2_FUNC2_NBIF0_BIST_ADDRESS    0x1015200fUL

#define SMN_DEV2_FUNC3_NBIF0_BIST_ADDRESS    0x1015300fUL

#define SMN_DEV2_FUNC4_NBIF0_BIST_ADDRESS    0x1015400fUL

#define SMN_DEV2_FUNC5_NBIF0_BIST_ADDRESS    0x1015500fUL

#define SMN_DEV2_FUNC6_NBIF0_BIST_ADDRESS    0x1015600fUL


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
} CACHE_LINE_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CACHE_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CACHE_LINE_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CACHE_LINE_OFFSET       0xc
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CACHE_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xc)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CACHE_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CACHE_LINE_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CACHE_LINE_OFFSET       0xc
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CACHE_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xc)

#define SMN_DEV0_FUNC2_NBIF0_CACHE_LINE_ADDRESS    0x1014200cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CACHE_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CACHE_LINE_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CACHE_LINE_OFFSET       0xc
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CACHE_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xc)

#define SMN_DEV0_FUNC3_NBIF0_CACHE_LINE_ADDRESS    0x1014300cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CACHE_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CACHE_LINE_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CACHE_LINE_OFFSET       0xc
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CACHE_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xc)

#define SMN_DEV0_FUNC4_NBIF0_CACHE_LINE_ADDRESS    0x1014400cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CACHE_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CACHE_LINE_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CACHE_LINE_OFFSET       0xc
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CACHE_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xc)

#define SMN_DEV0_FUNC5_NBIF0_CACHE_LINE_ADDRESS    0x1014500cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CACHE_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CACHE_LINE_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CACHE_LINE_OFFSET       0xc
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CACHE_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xc)

#define SMN_DEV0_FUNC6_NBIF0_CACHE_LINE_ADDRESS    0x1014600cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CACHE_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CACHE_LINE_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CACHE_LINE_OFFSET       0xc
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CACHE_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xc)

#define SMN_DEV0_FUNC7_NBIF0_CACHE_LINE_ADDRESS    0x1014700cUL

#define SMN_DEV1_FUNC1_NBIF0_CACHE_LINE_ADDRESS    0x1014900cUL

#define SMN_DEV1_FUNC2_NBIF0_CACHE_LINE_ADDRESS    0x1014a00cUL

#define SMN_DEV1_FUNC3_NBIF0_CACHE_LINE_ADDRESS    0x1014b00cUL

#define SMN_DEV2_FUNC1_NBIF0_CACHE_LINE_ADDRESS    0x1015100cUL

#define SMN_DEV2_FUNC2_NBIF0_CACHE_LINE_ADDRESS    0x1015200cUL

#define SMN_DEV2_FUNC3_NBIF0_CACHE_LINE_ADDRESS    0x1015300cUL

#define SMN_DEV2_FUNC4_NBIF0_CACHE_LINE_ADDRESS    0x1015400cUL

#define SMN_DEV2_FUNC5_NBIF0_CACHE_LINE_ADDRESS    0x1015500cUL

#define SMN_DEV2_FUNC6_NBIF0_CACHE_LINE_ADDRESS    0x1015600cUL


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
} CAP_PTR_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CAP_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CAP_PTR_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CAP_PTR_OFFSET       0x34
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CAP_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x34)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CAP_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CAP_PTR_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CAP_PTR_OFFSET       0x34
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CAP_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x34)

#define SMN_DEV0_FUNC2_NBIF0_CAP_PTR_ADDRESS    0x10142034UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CAP_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CAP_PTR_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CAP_PTR_OFFSET       0x34
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CAP_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x34)

#define SMN_DEV0_FUNC3_NBIF0_CAP_PTR_ADDRESS    0x10143034UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CAP_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CAP_PTR_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CAP_PTR_OFFSET       0x34
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CAP_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x34)

#define SMN_DEV0_FUNC4_NBIF0_CAP_PTR_ADDRESS    0x10144034UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CAP_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CAP_PTR_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CAP_PTR_OFFSET       0x34
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CAP_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x34)

#define SMN_DEV0_FUNC5_NBIF0_CAP_PTR_ADDRESS    0x10145034UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CAP_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CAP_PTR_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CAP_PTR_OFFSET       0x34
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CAP_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x34)

#define SMN_DEV0_FUNC6_NBIF0_CAP_PTR_ADDRESS    0x10146034UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CAP_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CAP_PTR_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CAP_PTR_OFFSET       0x34
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CAP_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x34)

#define SMN_DEV0_FUNC7_NBIF0_CAP_PTR_ADDRESS    0x10147034UL

#define SMN_DEV1_FUNC1_NBIF0_CAP_PTR_ADDRESS    0x10149034UL

#define SMN_DEV1_FUNC2_NBIF0_CAP_PTR_ADDRESS    0x1014a034UL

#define SMN_DEV1_FUNC3_NBIF0_CAP_PTR_ADDRESS    0x1014b034UL

#define SMN_DEV2_FUNC1_NBIF0_CAP_PTR_ADDRESS    0x10151034UL

#define SMN_DEV2_FUNC2_NBIF0_CAP_PTR_ADDRESS    0x10152034UL

#define SMN_DEV2_FUNC3_NBIF0_CAP_PTR_ADDRESS    0x10153034UL

#define SMN_DEV2_FUNC4_NBIF0_CAP_PTR_ADDRESS    0x10154034UL

#define SMN_DEV2_FUNC5_NBIF0_CAP_PTR_ADDRESS    0x10155034UL

#define SMN_DEV2_FUNC6_NBIF0_CAP_PTR_ADDRESS    0x10156034UL


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
} CARDBUS_CIS_PTR_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CARDBUS_CIS_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CARDBUS_CIS_PTR_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CARDBUS_CIS_PTR_OFFSET       0x28
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_CARDBUS_CIS_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x28)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CARDBUS_CIS_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CARDBUS_CIS_PTR_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CARDBUS_CIS_PTR_OFFSET       0x28
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_CARDBUS_CIS_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x28)

#define SMN_DEV0_FUNC2_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10142028UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CARDBUS_CIS_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CARDBUS_CIS_PTR_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CARDBUS_CIS_PTR_OFFSET       0x28
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_CARDBUS_CIS_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x28)

#define SMN_DEV0_FUNC3_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10143028UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CARDBUS_CIS_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CARDBUS_CIS_PTR_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CARDBUS_CIS_PTR_OFFSET       0x28
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_CARDBUS_CIS_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x28)

#define SMN_DEV0_FUNC4_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10144028UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CARDBUS_CIS_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CARDBUS_CIS_PTR_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CARDBUS_CIS_PTR_OFFSET       0x28
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_CARDBUS_CIS_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x28)

#define SMN_DEV0_FUNC5_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10145028UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CARDBUS_CIS_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CARDBUS_CIS_PTR_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CARDBUS_CIS_PTR_OFFSET       0x28
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_CARDBUS_CIS_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x28)

#define SMN_DEV0_FUNC6_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10146028UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CARDBUS_CIS_PTR_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CARDBUS_CIS_PTR_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CARDBUS_CIS_PTR_OFFSET       0x28
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_CARDBUS_CIS_PTR_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x28)

#define SMN_DEV0_FUNC7_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10147028UL

#define SMN_DEV1_FUNC1_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10149028UL

#define SMN_DEV1_FUNC2_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x1014a028UL

#define SMN_DEV1_FUNC3_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x1014b028UL

#define SMN_DEV2_FUNC1_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10151028UL

#define SMN_DEV2_FUNC2_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10152028UL

#define SMN_DEV2_FUNC3_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10153028UL

#define SMN_DEV2_FUNC4_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10154028UL

#define SMN_DEV2_FUNC5_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10155028UL

#define SMN_DEV2_FUNC6_NBIF0_CARDBUS_CIS_PTR_ADDRESS    0x10156028UL


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
} COMMAND_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_COMMAND_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_COMMAND_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x4)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_COMMAND_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_COMMAND_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x4)

#define SMN_DEV0_FUNC2_NBIF0_COMMAND_ADDRESS    0x10142004UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_COMMAND_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_COMMAND_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x4)

#define SMN_DEV0_FUNC3_NBIF0_COMMAND_ADDRESS    0x10143004UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_COMMAND_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_COMMAND_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x4)

#define SMN_DEV0_FUNC4_NBIF0_COMMAND_ADDRESS    0x10144004UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_COMMAND_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_COMMAND_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x4)

#define SMN_DEV0_FUNC5_NBIF0_COMMAND_ADDRESS    0x10145004UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_COMMAND_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_COMMAND_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x4)

#define SMN_DEV0_FUNC6_NBIF0_COMMAND_ADDRESS    0x10146004UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_COMMAND_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_COMMAND_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_COMMAND_OFFSET       0x4
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_COMMAND_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x4)

#define SMN_DEV0_FUNC7_NBIF0_COMMAND_ADDRESS    0x10147004UL

#define SMN_DEV1_FUNC1_NBIF0_COMMAND_ADDRESS    0x10149004UL

#define SMN_DEV1_FUNC2_NBIF0_COMMAND_ADDRESS    0x1014a004UL

#define SMN_DEV1_FUNC3_NBIF0_COMMAND_ADDRESS    0x1014b004UL

#define SMN_DEV2_FUNC1_NBIF0_COMMAND_ADDRESS    0x10151004UL

#define SMN_DEV2_FUNC2_NBIF0_COMMAND_ADDRESS    0x10152004UL

#define SMN_DEV2_FUNC3_NBIF0_COMMAND_ADDRESS    0x10153004UL

#define SMN_DEV2_FUNC4_NBIF0_COMMAND_ADDRESS    0x10154004UL

#define SMN_DEV2_FUNC5_NBIF0_COMMAND_ADDRESS    0x10155004UL

#define SMN_DEV2_FUNC6_NBIF0_COMMAND_ADDRESS    0x10156004UL


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
} DBESL_DBESLD_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DBESL_DBESLD_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DBESL_DBESLD_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DBESL_DBESLD_OFFSET       0x62
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DBESL_DBESLD_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x62)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DBESL_DBESLD_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DBESL_DBESLD_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DBESL_DBESLD_OFFSET       0x62
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DBESL_DBESLD_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x62)

#define SMN_DEV0_FUNC2_NBIF0_DBESL_DBESLD_ADDRESS    0x10142062UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DBESL_DBESLD_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DBESL_DBESLD_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DBESL_DBESLD_OFFSET       0x62
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DBESL_DBESLD_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x62)

#define SMN_DEV0_FUNC3_NBIF0_DBESL_DBESLD_ADDRESS    0x10143062UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DBESL_DBESLD_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DBESL_DBESLD_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DBESL_DBESLD_OFFSET       0x62
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DBESL_DBESLD_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x62)

#define SMN_DEV0_FUNC4_NBIF0_DBESL_DBESLD_ADDRESS    0x10144062UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DBESL_DBESLD_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DBESL_DBESLD_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DBESL_DBESLD_OFFSET       0x62
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DBESL_DBESLD_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x62)

#define SMN_DEV0_FUNC5_NBIF0_DBESL_DBESLD_ADDRESS    0x10145062UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DBESL_DBESLD_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DBESL_DBESLD_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DBESL_DBESLD_OFFSET       0x62
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DBESL_DBESLD_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x62)

#define SMN_DEV0_FUNC6_NBIF0_DBESL_DBESLD_ADDRESS    0x10146062UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DBESL_DBESLD_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DBESL_DBESLD_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DBESL_DBESLD_OFFSET       0x62
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DBESL_DBESLD_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x62)

#define SMN_DEV0_FUNC7_NBIF0_DBESL_DBESLD_ADDRESS    0x10147062UL

#define SMN_DEV1_FUNC1_NBIF0_DBESL_DBESLD_ADDRESS    0x10149062UL

#define SMN_DEV1_FUNC2_NBIF0_DBESL_DBESLD_ADDRESS    0x1014a062UL

#define SMN_DEV1_FUNC3_NBIF0_DBESL_DBESLD_ADDRESS    0x1014b062UL

#define SMN_DEV2_FUNC1_NBIF0_DBESL_DBESLD_ADDRESS    0x10151062UL

#define SMN_DEV2_FUNC2_NBIF0_DBESL_DBESLD_ADDRESS    0x10152062UL

#define SMN_DEV2_FUNC3_NBIF0_DBESL_DBESLD_ADDRESS    0x10153062UL

#define SMN_DEV2_FUNC4_NBIF0_DBESL_DBESLD_ADDRESS    0x10154062UL

#define SMN_DEV2_FUNC5_NBIF0_DBESL_DBESLD_ADDRESS    0x10155062UL

#define SMN_DEV2_FUNC6_NBIF0_DBESL_DBESLD_ADDRESS    0x10156062UL


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
} DEVICE_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CAP_OFFSET       0x68
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x68)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CAP_OFFSET       0x68
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x68)

#define SMN_DEV0_FUNC2_NBIF0_DEVICE_CAP_ADDRESS    0x10142068UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CAP_OFFSET       0x68
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x68)

#define SMN_DEV0_FUNC3_NBIF0_DEVICE_CAP_ADDRESS    0x10143068UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CAP_OFFSET       0x68
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x68)

#define SMN_DEV0_FUNC4_NBIF0_DEVICE_CAP_ADDRESS    0x10144068UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CAP_OFFSET       0x68
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x68)

#define SMN_DEV0_FUNC5_NBIF0_DEVICE_CAP_ADDRESS    0x10145068UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CAP_OFFSET       0x68
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x68)

#define SMN_DEV0_FUNC6_NBIF0_DEVICE_CAP_ADDRESS    0x10146068UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CAP_OFFSET       0x68
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x68)

#define SMN_DEV0_FUNC7_NBIF0_DEVICE_CAP_ADDRESS    0x10147068UL

#define SMN_DEV1_FUNC1_NBIF0_DEVICE_CAP_ADDRESS    0x10149068UL

#define SMN_DEV1_FUNC2_NBIF0_DEVICE_CAP_ADDRESS    0x1014a068UL

#define SMN_DEV1_FUNC3_NBIF0_DEVICE_CAP_ADDRESS    0x1014b068UL

#define SMN_DEV2_FUNC1_NBIF0_DEVICE_CAP_ADDRESS    0x10151068UL

#define SMN_DEV2_FUNC2_NBIF0_DEVICE_CAP_ADDRESS    0x10152068UL

#define SMN_DEV2_FUNC3_NBIF0_DEVICE_CAP_ADDRESS    0x10153068UL

#define SMN_DEV2_FUNC4_NBIF0_DEVICE_CAP_ADDRESS    0x10154068UL

#define SMN_DEV2_FUNC5_NBIF0_DEVICE_CAP_ADDRESS    0x10155068UL

#define SMN_DEV2_FUNC6_NBIF0_DEVICE_CAP_ADDRESS    0x10156068UL


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
} DEVICE_CAP2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CAP2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CAP2_OFFSET       0x88
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x88)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CAP2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CAP2_OFFSET       0x88
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x88)

#define SMN_DEV0_FUNC2_NBIF0_DEVICE_CAP2_ADDRESS    0x10142088UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CAP2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CAP2_OFFSET       0x88
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x88)

#define SMN_DEV0_FUNC3_NBIF0_DEVICE_CAP2_ADDRESS    0x10143088UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CAP2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CAP2_OFFSET       0x88
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x88)

#define SMN_DEV0_FUNC4_NBIF0_DEVICE_CAP2_ADDRESS    0x10144088UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CAP2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CAP2_OFFSET       0x88
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x88)

#define SMN_DEV0_FUNC5_NBIF0_DEVICE_CAP2_ADDRESS    0x10145088UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CAP2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CAP2_OFFSET       0x88
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x88)

#define SMN_DEV0_FUNC6_NBIF0_DEVICE_CAP2_ADDRESS    0x10146088UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CAP2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CAP2_OFFSET       0x88
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x88)

#define SMN_DEV0_FUNC7_NBIF0_DEVICE_CAP2_ADDRESS    0x10147088UL

#define SMN_DEV1_FUNC1_NBIF0_DEVICE_CAP2_ADDRESS    0x10149088UL

#define SMN_DEV1_FUNC2_NBIF0_DEVICE_CAP2_ADDRESS    0x1014a088UL

#define SMN_DEV1_FUNC3_NBIF0_DEVICE_CAP2_ADDRESS    0x1014b088UL

#define SMN_DEV2_FUNC1_NBIF0_DEVICE_CAP2_ADDRESS    0x10151088UL

#define SMN_DEV2_FUNC2_NBIF0_DEVICE_CAP2_ADDRESS    0x10152088UL

#define SMN_DEV2_FUNC3_NBIF0_DEVICE_CAP2_ADDRESS    0x10153088UL

#define SMN_DEV2_FUNC4_NBIF0_DEVICE_CAP2_ADDRESS    0x10154088UL

#define SMN_DEV2_FUNC5_NBIF0_DEVICE_CAP2_ADDRESS    0x10155088UL

#define SMN_DEV2_FUNC6_NBIF0_DEVICE_CAP2_ADDRESS    0x10156088UL


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
} DEVICE_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CNTL_OFFSET       0x6c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x6c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CNTL_OFFSET       0x6c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x6c)

#define SMN_DEV0_FUNC2_NBIF0_DEVICE_CNTL_ADDRESS    0x1014206cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CNTL_OFFSET       0x6c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x6c)

#define SMN_DEV0_FUNC3_NBIF0_DEVICE_CNTL_ADDRESS    0x1014306cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CNTL_OFFSET       0x6c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x6c)

#define SMN_DEV0_FUNC4_NBIF0_DEVICE_CNTL_ADDRESS    0x1014406cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CNTL_OFFSET       0x6c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x6c)

#define SMN_DEV0_FUNC5_NBIF0_DEVICE_CNTL_ADDRESS    0x1014506cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CNTL_OFFSET       0x6c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x6c)

#define SMN_DEV0_FUNC6_NBIF0_DEVICE_CNTL_ADDRESS    0x1014606cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CNTL_OFFSET       0x6c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x6c)

#define SMN_DEV0_FUNC7_NBIF0_DEVICE_CNTL_ADDRESS    0x1014706cUL

#define SMN_DEV1_FUNC1_NBIF0_DEVICE_CNTL_ADDRESS    0x1014906cUL

#define SMN_DEV1_FUNC2_NBIF0_DEVICE_CNTL_ADDRESS    0x1014a06cUL

#define SMN_DEV1_FUNC3_NBIF0_DEVICE_CNTL_ADDRESS    0x1014b06cUL

#define SMN_DEV2_FUNC1_NBIF0_DEVICE_CNTL_ADDRESS    0x1015106cUL

#define SMN_DEV2_FUNC2_NBIF0_DEVICE_CNTL_ADDRESS    0x1015206cUL

#define SMN_DEV2_FUNC3_NBIF0_DEVICE_CNTL_ADDRESS    0x1015306cUL

#define SMN_DEV2_FUNC4_NBIF0_DEVICE_CNTL_ADDRESS    0x1015406cUL

#define SMN_DEV2_FUNC5_NBIF0_DEVICE_CNTL_ADDRESS    0x1015506cUL

#define SMN_DEV2_FUNC6_NBIF0_DEVICE_CNTL_ADDRESS    0x1015606cUL


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
} DEVICE_CNTL2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CNTL2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CNTL2_OFFSET       0x8c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x8c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CNTL2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CNTL2_OFFSET       0x8c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x8c)

#define SMN_DEV0_FUNC2_NBIF0_DEVICE_CNTL2_ADDRESS    0x1014208cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CNTL2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CNTL2_OFFSET       0x8c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x8c)

#define SMN_DEV0_FUNC3_NBIF0_DEVICE_CNTL2_ADDRESS    0x1014308cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CNTL2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CNTL2_OFFSET       0x8c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x8c)

#define SMN_DEV0_FUNC4_NBIF0_DEVICE_CNTL2_ADDRESS    0x1014408cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CNTL2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CNTL2_OFFSET       0x8c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x8c)

#define SMN_DEV0_FUNC5_NBIF0_DEVICE_CNTL2_ADDRESS    0x1014508cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CNTL2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CNTL2_OFFSET       0x8c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x8c)

#define SMN_DEV0_FUNC6_NBIF0_DEVICE_CNTL2_ADDRESS    0x1014608cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CNTL2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CNTL2_OFFSET       0x8c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x8c)

#define SMN_DEV0_FUNC7_NBIF0_DEVICE_CNTL2_ADDRESS    0x1014708cUL

#define SMN_DEV1_FUNC1_NBIF0_DEVICE_CNTL2_ADDRESS    0x1014908cUL

#define SMN_DEV1_FUNC2_NBIF0_DEVICE_CNTL2_ADDRESS    0x1014a08cUL

#define SMN_DEV1_FUNC3_NBIF0_DEVICE_CNTL2_ADDRESS    0x1014b08cUL

#define SMN_DEV2_FUNC1_NBIF0_DEVICE_CNTL2_ADDRESS    0x1015108cUL

#define SMN_DEV2_FUNC2_NBIF0_DEVICE_CNTL2_ADDRESS    0x1015208cUL

#define SMN_DEV2_FUNC3_NBIF0_DEVICE_CNTL2_ADDRESS    0x1015308cUL

#define SMN_DEV2_FUNC4_NBIF0_DEVICE_CNTL2_ADDRESS    0x1015408cUL

#define SMN_DEV2_FUNC5_NBIF0_DEVICE_CNTL2_ADDRESS    0x1015508cUL

#define SMN_DEV2_FUNC6_NBIF0_DEVICE_CNTL2_ADDRESS    0x1015608cUL


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
} DEVICE_ID_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_ID_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_ID_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2)

#define SMN_DEV0_FUNC2_NBIF0_DEVICE_ID_ADDRESS    0x10142002UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_ID_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x2)

#define SMN_DEV0_FUNC3_NBIF0_DEVICE_ID_ADDRESS    0x10143002UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_ID_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x2)

#define SMN_DEV0_FUNC4_NBIF0_DEVICE_ID_ADDRESS    0x10144002UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_ID_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x2)

#define SMN_DEV0_FUNC5_NBIF0_DEVICE_ID_ADDRESS    0x10145002UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_ID_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x2)

#define SMN_DEV0_FUNC6_NBIF0_DEVICE_ID_ADDRESS    0x10146002UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_ID_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_ID_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_ID_OFFSET       0x2
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x2)

#define SMN_DEV0_FUNC7_NBIF0_DEVICE_ID_ADDRESS    0x10147002UL

#define SMN_DEV1_FUNC1_NBIF0_DEVICE_ID_ADDRESS    0x10149002UL

#define SMN_DEV1_FUNC2_NBIF0_DEVICE_ID_ADDRESS    0x1014a002UL

#define SMN_DEV1_FUNC3_NBIF0_DEVICE_ID_ADDRESS    0x1014b002UL

#define SMN_DEV2_FUNC1_NBIF0_DEVICE_ID_ADDRESS    0x10151002UL

#define SMN_DEV2_FUNC2_NBIF0_DEVICE_ID_ADDRESS    0x10152002UL

#define SMN_DEV2_FUNC3_NBIF0_DEVICE_ID_ADDRESS    0x10153002UL

#define SMN_DEV2_FUNC4_NBIF0_DEVICE_ID_ADDRESS    0x10154002UL

#define SMN_DEV2_FUNC5_NBIF0_DEVICE_ID_ADDRESS    0x10155002UL

#define SMN_DEV2_FUNC6_NBIF0_DEVICE_ID_ADDRESS    0x10156002UL


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
} DEVICE_STATUS_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_STATUS_OFFSET       0x6e
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x6e)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_STATUS_OFFSET       0x6e
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x6e)

#define SMN_DEV0_FUNC2_NBIF0_DEVICE_STATUS_ADDRESS    0x1014206eUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_STATUS_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_STATUS_OFFSET       0x6e
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x6e)

#define SMN_DEV0_FUNC3_NBIF0_DEVICE_STATUS_ADDRESS    0x1014306eUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_STATUS_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_STATUS_OFFSET       0x6e
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x6e)

#define SMN_DEV0_FUNC4_NBIF0_DEVICE_STATUS_ADDRESS    0x1014406eUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_STATUS_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_STATUS_OFFSET       0x6e
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x6e)

#define SMN_DEV0_FUNC5_NBIF0_DEVICE_STATUS_ADDRESS    0x1014506eUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_STATUS_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_STATUS_OFFSET       0x6e
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x6e)

#define SMN_DEV0_FUNC6_NBIF0_DEVICE_STATUS_ADDRESS    0x1014606eUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_STATUS_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_STATUS_OFFSET       0x6e
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x6e)

#define SMN_DEV0_FUNC7_NBIF0_DEVICE_STATUS_ADDRESS    0x1014706eUL

#define SMN_DEV1_FUNC1_NBIF0_DEVICE_STATUS_ADDRESS    0x1014906eUL

#define SMN_DEV1_FUNC2_NBIF0_DEVICE_STATUS_ADDRESS    0x1014a06eUL

#define SMN_DEV1_FUNC3_NBIF0_DEVICE_STATUS_ADDRESS    0x1014b06eUL

#define SMN_DEV2_FUNC1_NBIF0_DEVICE_STATUS_ADDRESS    0x1015106eUL

#define SMN_DEV2_FUNC2_NBIF0_DEVICE_STATUS_ADDRESS    0x1015206eUL

#define SMN_DEV2_FUNC3_NBIF0_DEVICE_STATUS_ADDRESS    0x1015306eUL

#define SMN_DEV2_FUNC4_NBIF0_DEVICE_STATUS_ADDRESS    0x1015406eUL

#define SMN_DEV2_FUNC5_NBIF0_DEVICE_STATUS_ADDRESS    0x1015506eUL

#define SMN_DEV2_FUNC6_NBIF0_DEVICE_STATUS_ADDRESS    0x1015606eUL


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
} DEVICE_STATUS2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_STATUS2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_STATUS2_OFFSET       0x8e
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_DEVICE_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x8e)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_STATUS2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_STATUS2_OFFSET       0x8e
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_DEVICE_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x8e)

#define SMN_DEV0_FUNC2_NBIF0_DEVICE_STATUS2_ADDRESS    0x1014208eUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_STATUS2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_STATUS2_OFFSET       0x8e
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_DEVICE_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x8e)

#define SMN_DEV0_FUNC3_NBIF0_DEVICE_STATUS2_ADDRESS    0x1014308eUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_STATUS2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_STATUS2_OFFSET       0x8e
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_DEVICE_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x8e)

#define SMN_DEV0_FUNC4_NBIF0_DEVICE_STATUS2_ADDRESS    0x1014408eUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_STATUS2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_STATUS2_OFFSET       0x8e
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_DEVICE_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x8e)

#define SMN_DEV0_FUNC5_NBIF0_DEVICE_STATUS2_ADDRESS    0x1014508eUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_STATUS2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_STATUS2_OFFSET       0x8e
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_DEVICE_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x8e)

#define SMN_DEV0_FUNC6_NBIF0_DEVICE_STATUS2_ADDRESS    0x1014608eUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_STATUS2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_STATUS2_OFFSET       0x8e
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_DEVICE_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x8e)

#define SMN_DEV0_FUNC7_NBIF0_DEVICE_STATUS2_ADDRESS    0x1014708eUL

#define SMN_DEV1_FUNC1_NBIF0_DEVICE_STATUS2_ADDRESS    0x1014908eUL

#define SMN_DEV1_FUNC2_NBIF0_DEVICE_STATUS2_ADDRESS    0x1014a08eUL

#define SMN_DEV1_FUNC3_NBIF0_DEVICE_STATUS2_ADDRESS    0x1014b08eUL

#define SMN_DEV2_FUNC1_NBIF0_DEVICE_STATUS2_ADDRESS    0x1015108eUL

#define SMN_DEV2_FUNC2_NBIF0_DEVICE_STATUS2_ADDRESS    0x1015208eUL

#define SMN_DEV2_FUNC3_NBIF0_DEVICE_STATUS2_ADDRESS    0x1015308eUL

#define SMN_DEV2_FUNC4_NBIF0_DEVICE_STATUS2_ADDRESS    0x1015408eUL

#define SMN_DEV2_FUNC5_NBIF0_DEVICE_STATUS2_ADDRESS    0x1015508eUL

#define SMN_DEV2_FUNC6_NBIF0_DEVICE_STATUS2_ADDRESS    0x1015608eUL


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
} FLADJ_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_FLADJ_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_FLADJ_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_FLADJ_OFFSET       0x61
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_FLADJ_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x61)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_FLADJ_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_FLADJ_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_FLADJ_OFFSET       0x61
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_FLADJ_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x61)

#define SMN_DEV0_FUNC2_NBIF0_FLADJ_ADDRESS    0x10142061UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_FLADJ_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_FLADJ_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_FLADJ_OFFSET       0x61
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_FLADJ_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x61)

#define SMN_DEV0_FUNC3_NBIF0_FLADJ_ADDRESS    0x10143061UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_FLADJ_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_FLADJ_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_FLADJ_OFFSET       0x61
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_FLADJ_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x61)

#define SMN_DEV0_FUNC4_NBIF0_FLADJ_ADDRESS    0x10144061UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_FLADJ_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_FLADJ_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_FLADJ_OFFSET       0x61
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_FLADJ_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x61)

#define SMN_DEV0_FUNC5_NBIF0_FLADJ_ADDRESS    0x10145061UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_FLADJ_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_FLADJ_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_FLADJ_OFFSET       0x61
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_FLADJ_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x61)

#define SMN_DEV0_FUNC6_NBIF0_FLADJ_ADDRESS    0x10146061UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_FLADJ_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_FLADJ_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_FLADJ_OFFSET       0x61
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_FLADJ_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x61)

#define SMN_DEV0_FUNC7_NBIF0_FLADJ_ADDRESS    0x10147061UL

#define SMN_DEV1_FUNC1_NBIF0_FLADJ_ADDRESS    0x10149061UL

#define SMN_DEV1_FUNC2_NBIF0_FLADJ_ADDRESS    0x1014a061UL

#define SMN_DEV1_FUNC3_NBIF0_FLADJ_ADDRESS    0x1014b061UL

#define SMN_DEV2_FUNC1_NBIF0_FLADJ_ADDRESS    0x10151061UL

#define SMN_DEV2_FUNC2_NBIF0_FLADJ_ADDRESS    0x10152061UL

#define SMN_DEV2_FUNC3_NBIF0_FLADJ_ADDRESS    0x10153061UL

#define SMN_DEV2_FUNC4_NBIF0_FLADJ_ADDRESS    0x10154061UL

#define SMN_DEV2_FUNC5_NBIF0_FLADJ_ADDRESS    0x10155061UL

#define SMN_DEV2_FUNC6_NBIF0_FLADJ_ADDRESS    0x10156061UL


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
} HEADER_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_HEADER_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_HEADER_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_HEADER_OFFSET       0xe
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xe)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_HEADER_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_HEADER_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_HEADER_OFFSET       0xe
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xe)

#define SMN_DEV0_FUNC2_NBIF0_HEADER_ADDRESS    0x1014200eUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_HEADER_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_HEADER_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_HEADER_OFFSET       0xe
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xe)

#define SMN_DEV0_FUNC3_NBIF0_HEADER_ADDRESS    0x1014300eUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_HEADER_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_HEADER_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_HEADER_OFFSET       0xe
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xe)

#define SMN_DEV0_FUNC4_NBIF0_HEADER_ADDRESS    0x1014400eUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_HEADER_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_HEADER_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_HEADER_OFFSET       0xe
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xe)

#define SMN_DEV0_FUNC5_NBIF0_HEADER_ADDRESS    0x1014500eUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_HEADER_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_HEADER_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_HEADER_OFFSET       0xe
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xe)

#define SMN_DEV0_FUNC6_NBIF0_HEADER_ADDRESS    0x1014600eUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_HEADER_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_HEADER_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_HEADER_OFFSET       0xe
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_HEADER_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xe)

#define SMN_DEV0_FUNC7_NBIF0_HEADER_ADDRESS    0x1014700eUL

#define SMN_DEV1_FUNC1_NBIF0_HEADER_ADDRESS    0x1014900eUL

#define SMN_DEV1_FUNC2_NBIF0_HEADER_ADDRESS    0x1014a00eUL

#define SMN_DEV1_FUNC3_NBIF0_HEADER_ADDRESS    0x1014b00eUL

#define SMN_DEV2_FUNC1_NBIF0_HEADER_ADDRESS    0x1015100eUL

#define SMN_DEV2_FUNC2_NBIF0_HEADER_ADDRESS    0x1015200eUL

#define SMN_DEV2_FUNC3_NBIF0_HEADER_ADDRESS    0x1015300eUL

#define SMN_DEV2_FUNC4_NBIF0_HEADER_ADDRESS    0x1015400eUL

#define SMN_DEV2_FUNC5_NBIF0_HEADER_ADDRESS    0x1015500eUL

#define SMN_DEV2_FUNC6_NBIF0_HEADER_ADDRESS    0x1015600eUL


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
} INTERRUPT_LINE_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_INTERRUPT_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_INTERRUPT_LINE_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_INTERRUPT_LINE_OFFSET       0x3c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_INTERRUPT_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x3c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_INTERRUPT_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_INTERRUPT_LINE_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_INTERRUPT_LINE_OFFSET       0x3c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_INTERRUPT_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x3c)

#define SMN_DEV0_FUNC2_NBIF0_INTERRUPT_LINE_ADDRESS    0x1014203cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_INTERRUPT_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_INTERRUPT_LINE_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_INTERRUPT_LINE_OFFSET       0x3c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_INTERRUPT_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x3c)

#define SMN_DEV0_FUNC3_NBIF0_INTERRUPT_LINE_ADDRESS    0x1014303cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_INTERRUPT_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_INTERRUPT_LINE_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_INTERRUPT_LINE_OFFSET       0x3c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_INTERRUPT_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x3c)

#define SMN_DEV0_FUNC4_NBIF0_INTERRUPT_LINE_ADDRESS    0x1014403cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_INTERRUPT_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_INTERRUPT_LINE_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_INTERRUPT_LINE_OFFSET       0x3c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_INTERRUPT_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x3c)

#define SMN_DEV0_FUNC5_NBIF0_INTERRUPT_LINE_ADDRESS    0x1014503cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_INTERRUPT_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_INTERRUPT_LINE_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_INTERRUPT_LINE_OFFSET       0x3c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_INTERRUPT_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x3c)

#define SMN_DEV0_FUNC6_NBIF0_INTERRUPT_LINE_ADDRESS    0x1014603cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_INTERRUPT_LINE_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_INTERRUPT_LINE_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_INTERRUPT_LINE_OFFSET       0x3c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_INTERRUPT_LINE_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x3c)

#define SMN_DEV0_FUNC7_NBIF0_INTERRUPT_LINE_ADDRESS    0x1014703cUL

#define SMN_DEV1_FUNC1_NBIF0_INTERRUPT_LINE_ADDRESS    0x1014903cUL

#define SMN_DEV1_FUNC2_NBIF0_INTERRUPT_LINE_ADDRESS    0x1014a03cUL

#define SMN_DEV1_FUNC3_NBIF0_INTERRUPT_LINE_ADDRESS    0x1014b03cUL

#define SMN_DEV2_FUNC1_NBIF0_INTERRUPT_LINE_ADDRESS    0x1015103cUL

#define SMN_DEV2_FUNC2_NBIF0_INTERRUPT_LINE_ADDRESS    0x1015203cUL

#define SMN_DEV2_FUNC3_NBIF0_INTERRUPT_LINE_ADDRESS    0x1015303cUL

#define SMN_DEV2_FUNC4_NBIF0_INTERRUPT_LINE_ADDRESS    0x1015403cUL

#define SMN_DEV2_FUNC5_NBIF0_INTERRUPT_LINE_ADDRESS    0x1015503cUL

#define SMN_DEV2_FUNC6_NBIF0_INTERRUPT_LINE_ADDRESS    0x1015603cUL


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
} INTERRUPT_PIN_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_INTERRUPT_PIN_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_INTERRUPT_PIN_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_INTERRUPT_PIN_OFFSET       0x3d
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_INTERRUPT_PIN_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x3d)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_INTERRUPT_PIN_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_INTERRUPT_PIN_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_INTERRUPT_PIN_OFFSET       0x3d
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_INTERRUPT_PIN_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x3d)

#define SMN_DEV0_FUNC2_NBIF0_INTERRUPT_PIN_ADDRESS    0x1014203dUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_INTERRUPT_PIN_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_INTERRUPT_PIN_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_INTERRUPT_PIN_OFFSET       0x3d
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_INTERRUPT_PIN_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x3d)

#define SMN_DEV0_FUNC3_NBIF0_INTERRUPT_PIN_ADDRESS    0x1014303dUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_INTERRUPT_PIN_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_INTERRUPT_PIN_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_INTERRUPT_PIN_OFFSET       0x3d
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_INTERRUPT_PIN_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x3d)

#define SMN_DEV0_FUNC4_NBIF0_INTERRUPT_PIN_ADDRESS    0x1014403dUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_INTERRUPT_PIN_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_INTERRUPT_PIN_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_INTERRUPT_PIN_OFFSET       0x3d
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_INTERRUPT_PIN_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x3d)

#define SMN_DEV0_FUNC5_NBIF0_INTERRUPT_PIN_ADDRESS    0x1014503dUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_INTERRUPT_PIN_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_INTERRUPT_PIN_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_INTERRUPT_PIN_OFFSET       0x3d
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_INTERRUPT_PIN_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x3d)

#define SMN_DEV0_FUNC6_NBIF0_INTERRUPT_PIN_ADDRESS    0x1014603dUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_INTERRUPT_PIN_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_INTERRUPT_PIN_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_INTERRUPT_PIN_OFFSET       0x3d
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_INTERRUPT_PIN_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x3d)

#define SMN_DEV0_FUNC7_NBIF0_INTERRUPT_PIN_ADDRESS    0x1014703dUL

#define SMN_DEV1_FUNC1_NBIF0_INTERRUPT_PIN_ADDRESS    0x1014903dUL

#define SMN_DEV1_FUNC2_NBIF0_INTERRUPT_PIN_ADDRESS    0x1014a03dUL

#define SMN_DEV1_FUNC3_NBIF0_INTERRUPT_PIN_ADDRESS    0x1014b03dUL

#define SMN_DEV2_FUNC1_NBIF0_INTERRUPT_PIN_ADDRESS    0x1015103dUL

#define SMN_DEV2_FUNC2_NBIF0_INTERRUPT_PIN_ADDRESS    0x1015203dUL

#define SMN_DEV2_FUNC3_NBIF0_INTERRUPT_PIN_ADDRESS    0x1015303dUL

#define SMN_DEV2_FUNC4_NBIF0_INTERRUPT_PIN_ADDRESS    0x1015403dUL

#define SMN_DEV2_FUNC5_NBIF0_INTERRUPT_PIN_ADDRESS    0x1015503dUL

#define SMN_DEV2_FUNC6_NBIF0_INTERRUPT_PIN_ADDRESS    0x1015603dUL


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
} LATENCY_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LATENCY_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LATENCY_OFFSET       0xd
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LATENCY_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LATENCY_OFFSET       0xd
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd)

#define SMN_DEV0_FUNC2_NBIF0_LATENCY_ADDRESS    0x1014200dUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LATENCY_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LATENCY_OFFSET       0xd
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xd)

#define SMN_DEV0_FUNC3_NBIF0_LATENCY_ADDRESS    0x1014300dUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LATENCY_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LATENCY_OFFSET       0xd
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xd)

#define SMN_DEV0_FUNC4_NBIF0_LATENCY_ADDRESS    0x1014400dUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LATENCY_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LATENCY_OFFSET       0xd
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xd)

#define SMN_DEV0_FUNC5_NBIF0_LATENCY_ADDRESS    0x1014500dUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LATENCY_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LATENCY_OFFSET       0xd
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xd)

#define SMN_DEV0_FUNC6_NBIF0_LATENCY_ADDRESS    0x1014600dUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LATENCY_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LATENCY_OFFSET       0xd
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xd)

#define SMN_DEV0_FUNC7_NBIF0_LATENCY_ADDRESS    0x1014700dUL

#define SMN_DEV1_FUNC1_NBIF0_LATENCY_ADDRESS    0x1014900dUL

#define SMN_DEV1_FUNC2_NBIF0_LATENCY_ADDRESS    0x1014a00dUL

#define SMN_DEV1_FUNC3_NBIF0_LATENCY_ADDRESS    0x1014b00dUL

#define SMN_DEV2_FUNC1_NBIF0_LATENCY_ADDRESS    0x1015100dUL

#define SMN_DEV2_FUNC2_NBIF0_LATENCY_ADDRESS    0x1015200dUL

#define SMN_DEV2_FUNC3_NBIF0_LATENCY_ADDRESS    0x1015300dUL

#define SMN_DEV2_FUNC4_NBIF0_LATENCY_ADDRESS    0x1015400dUL

#define SMN_DEV2_FUNC5_NBIF0_LATENCY_ADDRESS    0x1015500dUL

#define SMN_DEV2_FUNC6_NBIF0_LATENCY_ADDRESS    0x1015600dUL


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
} LINK_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CAP_OFFSET       0x70
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x70)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CAP_OFFSET       0x70
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x70)

#define SMN_DEV0_FUNC2_NBIF0_LINK_CAP_ADDRESS    0x10142070UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CAP_OFFSET       0x70
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x70)

#define SMN_DEV0_FUNC3_NBIF0_LINK_CAP_ADDRESS    0x10143070UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CAP_OFFSET       0x70
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x70)

#define SMN_DEV0_FUNC4_NBIF0_LINK_CAP_ADDRESS    0x10144070UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CAP_OFFSET       0x70
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x70)

#define SMN_DEV0_FUNC5_NBIF0_LINK_CAP_ADDRESS    0x10145070UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CAP_OFFSET       0x70
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x70)

#define SMN_DEV0_FUNC6_NBIF0_LINK_CAP_ADDRESS    0x10146070UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CAP_OFFSET       0x70
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x70)

#define SMN_DEV0_FUNC7_NBIF0_LINK_CAP_ADDRESS    0x10147070UL

#define SMN_DEV1_FUNC1_NBIF0_LINK_CAP_ADDRESS    0x10149070UL

#define SMN_DEV1_FUNC2_NBIF0_LINK_CAP_ADDRESS    0x1014a070UL

#define SMN_DEV1_FUNC3_NBIF0_LINK_CAP_ADDRESS    0x1014b070UL

#define SMN_DEV2_FUNC1_NBIF0_LINK_CAP_ADDRESS    0x10151070UL

#define SMN_DEV2_FUNC2_NBIF0_LINK_CAP_ADDRESS    0x10152070UL

#define SMN_DEV2_FUNC3_NBIF0_LINK_CAP_ADDRESS    0x10153070UL

#define SMN_DEV2_FUNC4_NBIF0_LINK_CAP_ADDRESS    0x10154070UL

#define SMN_DEV2_FUNC5_NBIF0_LINK_CAP_ADDRESS    0x10155070UL

#define SMN_DEV2_FUNC6_NBIF0_LINK_CAP_ADDRESS    0x10156070UL


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
} LINK_CAP2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CAP2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CAP2_OFFSET       0x90
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x90)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CAP2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CAP2_OFFSET       0x90
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x90)

#define SMN_DEV0_FUNC2_NBIF0_LINK_CAP2_ADDRESS    0x10142090UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CAP2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CAP2_OFFSET       0x90
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x90)

#define SMN_DEV0_FUNC3_NBIF0_LINK_CAP2_ADDRESS    0x10143090UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CAP2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CAP2_OFFSET       0x90
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x90)

#define SMN_DEV0_FUNC4_NBIF0_LINK_CAP2_ADDRESS    0x10144090UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CAP2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CAP2_OFFSET       0x90
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x90)

#define SMN_DEV0_FUNC5_NBIF0_LINK_CAP2_ADDRESS    0x10145090UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CAP2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CAP2_OFFSET       0x90
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x90)

#define SMN_DEV0_FUNC6_NBIF0_LINK_CAP2_ADDRESS    0x10146090UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CAP2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CAP2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CAP2_OFFSET       0x90
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CAP2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x90)

#define SMN_DEV0_FUNC7_NBIF0_LINK_CAP2_ADDRESS    0x10147090UL

#define SMN_DEV1_FUNC1_NBIF0_LINK_CAP2_ADDRESS    0x10149090UL

#define SMN_DEV1_FUNC2_NBIF0_LINK_CAP2_ADDRESS    0x1014a090UL

#define SMN_DEV1_FUNC3_NBIF0_LINK_CAP2_ADDRESS    0x1014b090UL

#define SMN_DEV2_FUNC1_NBIF0_LINK_CAP2_ADDRESS    0x10151090UL

#define SMN_DEV2_FUNC2_NBIF0_LINK_CAP2_ADDRESS    0x10152090UL

#define SMN_DEV2_FUNC3_NBIF0_LINK_CAP2_ADDRESS    0x10153090UL

#define SMN_DEV2_FUNC4_NBIF0_LINK_CAP2_ADDRESS    0x10154090UL

#define SMN_DEV2_FUNC5_NBIF0_LINK_CAP2_ADDRESS    0x10155090UL

#define SMN_DEV2_FUNC6_NBIF0_LINK_CAP2_ADDRESS    0x10156090UL


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
} LINK_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CNTL_OFFSET       0x74
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x74)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CNTL_OFFSET       0x74
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x74)

#define SMN_DEV0_FUNC2_NBIF0_LINK_CNTL_ADDRESS    0x10142074UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CNTL_OFFSET       0x74
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x74)

#define SMN_DEV0_FUNC3_NBIF0_LINK_CNTL_ADDRESS    0x10143074UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CNTL_OFFSET       0x74
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x74)

#define SMN_DEV0_FUNC4_NBIF0_LINK_CNTL_ADDRESS    0x10144074UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CNTL_OFFSET       0x74
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x74)

#define SMN_DEV0_FUNC5_NBIF0_LINK_CNTL_ADDRESS    0x10145074UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CNTL_OFFSET       0x74
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x74)

#define SMN_DEV0_FUNC6_NBIF0_LINK_CNTL_ADDRESS    0x10146074UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CNTL_OFFSET       0x74
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x74)

#define SMN_DEV0_FUNC7_NBIF0_LINK_CNTL_ADDRESS    0x10147074UL

#define SMN_DEV1_FUNC1_NBIF0_LINK_CNTL_ADDRESS    0x10149074UL

#define SMN_DEV1_FUNC2_NBIF0_LINK_CNTL_ADDRESS    0x1014a074UL

#define SMN_DEV1_FUNC3_NBIF0_LINK_CNTL_ADDRESS    0x1014b074UL

#define SMN_DEV2_FUNC1_NBIF0_LINK_CNTL_ADDRESS    0x10151074UL

#define SMN_DEV2_FUNC2_NBIF0_LINK_CNTL_ADDRESS    0x10152074UL

#define SMN_DEV2_FUNC3_NBIF0_LINK_CNTL_ADDRESS    0x10153074UL

#define SMN_DEV2_FUNC4_NBIF0_LINK_CNTL_ADDRESS    0x10154074UL

#define SMN_DEV2_FUNC5_NBIF0_LINK_CNTL_ADDRESS    0x10155074UL

#define SMN_DEV2_FUNC6_NBIF0_LINK_CNTL_ADDRESS    0x10156074UL


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
} LINK_CNTL2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CNTL2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CNTL2_OFFSET       0x94
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x94)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CNTL2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CNTL2_OFFSET       0x94
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x94)

#define SMN_DEV0_FUNC2_NBIF0_LINK_CNTL2_ADDRESS    0x10142094UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CNTL2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CNTL2_OFFSET       0x94
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x94)

#define SMN_DEV0_FUNC3_NBIF0_LINK_CNTL2_ADDRESS    0x10143094UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CNTL2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CNTL2_OFFSET       0x94
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x94)

#define SMN_DEV0_FUNC4_NBIF0_LINK_CNTL2_ADDRESS    0x10144094UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CNTL2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CNTL2_OFFSET       0x94
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x94)

#define SMN_DEV0_FUNC5_NBIF0_LINK_CNTL2_ADDRESS    0x10145094UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CNTL2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CNTL2_OFFSET       0x94
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x94)

#define SMN_DEV0_FUNC6_NBIF0_LINK_CNTL2_ADDRESS    0x10146094UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CNTL2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CNTL2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CNTL2_OFFSET       0x94
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_CNTL2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x94)

#define SMN_DEV0_FUNC7_NBIF0_LINK_CNTL2_ADDRESS    0x10147094UL

#define SMN_DEV1_FUNC1_NBIF0_LINK_CNTL2_ADDRESS    0x10149094UL

#define SMN_DEV1_FUNC2_NBIF0_LINK_CNTL2_ADDRESS    0x1014a094UL

#define SMN_DEV1_FUNC3_NBIF0_LINK_CNTL2_ADDRESS    0x1014b094UL

#define SMN_DEV2_FUNC1_NBIF0_LINK_CNTL2_ADDRESS    0x10151094UL

#define SMN_DEV2_FUNC2_NBIF0_LINK_CNTL2_ADDRESS    0x10152094UL

#define SMN_DEV2_FUNC3_NBIF0_LINK_CNTL2_ADDRESS    0x10153094UL

#define SMN_DEV2_FUNC4_NBIF0_LINK_CNTL2_ADDRESS    0x10154094UL

#define SMN_DEV2_FUNC5_NBIF0_LINK_CNTL2_ADDRESS    0x10155094UL

#define SMN_DEV2_FUNC6_NBIF0_LINK_CNTL2_ADDRESS    0x10156094UL


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
} LINK_STATUS_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_STATUS_OFFSET       0x76
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x76)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_STATUS_OFFSET       0x76
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x76)

#define SMN_DEV0_FUNC2_NBIF0_LINK_STATUS_ADDRESS    0x10142076UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_STATUS_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_STATUS_OFFSET       0x76
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x76)

#define SMN_DEV0_FUNC3_NBIF0_LINK_STATUS_ADDRESS    0x10143076UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_STATUS_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_STATUS_OFFSET       0x76
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x76)

#define SMN_DEV0_FUNC4_NBIF0_LINK_STATUS_ADDRESS    0x10144076UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_STATUS_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_STATUS_OFFSET       0x76
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x76)

#define SMN_DEV0_FUNC5_NBIF0_LINK_STATUS_ADDRESS    0x10145076UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_STATUS_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_STATUS_OFFSET       0x76
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x76)

#define SMN_DEV0_FUNC6_NBIF0_LINK_STATUS_ADDRESS    0x10146076UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_STATUS_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_STATUS_OFFSET       0x76
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x76)

#define SMN_DEV0_FUNC7_NBIF0_LINK_STATUS_ADDRESS    0x10147076UL

#define SMN_DEV1_FUNC1_NBIF0_LINK_STATUS_ADDRESS    0x10149076UL

#define SMN_DEV1_FUNC2_NBIF0_LINK_STATUS_ADDRESS    0x1014a076UL

#define SMN_DEV1_FUNC3_NBIF0_LINK_STATUS_ADDRESS    0x1014b076UL

#define SMN_DEV2_FUNC1_NBIF0_LINK_STATUS_ADDRESS    0x10151076UL

#define SMN_DEV2_FUNC2_NBIF0_LINK_STATUS_ADDRESS    0x10152076UL

#define SMN_DEV2_FUNC3_NBIF0_LINK_STATUS_ADDRESS    0x10153076UL

#define SMN_DEV2_FUNC4_NBIF0_LINK_STATUS_ADDRESS    0x10154076UL

#define SMN_DEV2_FUNC5_NBIF0_LINK_STATUS_ADDRESS    0x10155076UL

#define SMN_DEV2_FUNC6_NBIF0_LINK_STATUS_ADDRESS    0x10156076UL


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
} LINK_STATUS2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_STATUS2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_STATUS2_OFFSET       0x96
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_LINK_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x96)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_STATUS2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_STATUS2_OFFSET       0x96
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_LINK_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x96)

#define SMN_DEV0_FUNC2_NBIF0_LINK_STATUS2_ADDRESS    0x10142096UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_STATUS2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_STATUS2_OFFSET       0x96
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_LINK_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x96)

#define SMN_DEV0_FUNC3_NBIF0_LINK_STATUS2_ADDRESS    0x10143096UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_STATUS2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_STATUS2_OFFSET       0x96
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_LINK_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x96)

#define SMN_DEV0_FUNC4_NBIF0_LINK_STATUS2_ADDRESS    0x10144096UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_STATUS2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_STATUS2_OFFSET       0x96
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_LINK_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x96)

#define SMN_DEV0_FUNC5_NBIF0_LINK_STATUS2_ADDRESS    0x10145096UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_STATUS2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_STATUS2_OFFSET       0x96
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_LINK_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x96)

#define SMN_DEV0_FUNC6_NBIF0_LINK_STATUS2_ADDRESS    0x10146096UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_STATUS2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_STATUS2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_STATUS2_OFFSET       0x96
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_LINK_STATUS2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x96)

#define SMN_DEV0_FUNC7_NBIF0_LINK_STATUS2_ADDRESS    0x10147096UL

#define SMN_DEV1_FUNC1_NBIF0_LINK_STATUS2_ADDRESS    0x10149096UL

#define SMN_DEV1_FUNC2_NBIF0_LINK_STATUS2_ADDRESS    0x1014a096UL

#define SMN_DEV1_FUNC3_NBIF0_LINK_STATUS2_ADDRESS    0x1014b096UL

#define SMN_DEV2_FUNC1_NBIF0_LINK_STATUS2_ADDRESS    0x10151096UL

#define SMN_DEV2_FUNC2_NBIF0_LINK_STATUS2_ADDRESS    0x10152096UL

#define SMN_DEV2_FUNC3_NBIF0_LINK_STATUS2_ADDRESS    0x10153096UL

#define SMN_DEV2_FUNC4_NBIF0_LINK_STATUS2_ADDRESS    0x10154096UL

#define SMN_DEV2_FUNC5_NBIF0_LINK_STATUS2_ADDRESS    0x10155096UL

#define SMN_DEV2_FUNC6_NBIF0_LINK_STATUS2_ADDRESS    0x10156096UL


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
} MAX_LATENCY_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MAX_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MAX_LATENCY_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MAX_LATENCY_OFFSET       0x3f
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MAX_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x3f)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MAX_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MAX_LATENCY_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MAX_LATENCY_OFFSET       0x3f
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MAX_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x3f)

#define SMN_DEV0_FUNC2_NBIF0_MAX_LATENCY_ADDRESS    0x1014203fUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MAX_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MAX_LATENCY_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MAX_LATENCY_OFFSET       0x3f
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MAX_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x3f)

#define SMN_DEV0_FUNC3_NBIF0_MAX_LATENCY_ADDRESS    0x1014303fUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MAX_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MAX_LATENCY_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MAX_LATENCY_OFFSET       0x3f
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MAX_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x3f)

#define SMN_DEV0_FUNC4_NBIF0_MAX_LATENCY_ADDRESS    0x1014403fUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MAX_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MAX_LATENCY_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MAX_LATENCY_OFFSET       0x3f
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MAX_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x3f)

#define SMN_DEV0_FUNC5_NBIF0_MAX_LATENCY_ADDRESS    0x1014503fUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MAX_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MAX_LATENCY_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MAX_LATENCY_OFFSET       0x3f
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MAX_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x3f)

#define SMN_DEV0_FUNC6_NBIF0_MAX_LATENCY_ADDRESS    0x1014603fUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MAX_LATENCY_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MAX_LATENCY_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MAX_LATENCY_OFFSET       0x3f
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MAX_LATENCY_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x3f)

#define SMN_DEV0_FUNC7_NBIF0_MAX_LATENCY_ADDRESS    0x1014703fUL

#define SMN_DEV1_FUNC1_NBIF0_MAX_LATENCY_ADDRESS    0x1014903fUL

#define SMN_DEV1_FUNC2_NBIF0_MAX_LATENCY_ADDRESS    0x1014a03fUL

#define SMN_DEV1_FUNC3_NBIF0_MAX_LATENCY_ADDRESS    0x1014b03fUL

#define SMN_DEV2_FUNC1_NBIF0_MAX_LATENCY_ADDRESS    0x1015103fUL

#define SMN_DEV2_FUNC2_NBIF0_MAX_LATENCY_ADDRESS    0x1015203fUL

#define SMN_DEV2_FUNC3_NBIF0_MAX_LATENCY_ADDRESS    0x1015303fUL

#define SMN_DEV2_FUNC4_NBIF0_MAX_LATENCY_ADDRESS    0x1015403fUL

#define SMN_DEV2_FUNC5_NBIF0_MAX_LATENCY_ADDRESS    0x1015503fUL

#define SMN_DEV2_FUNC6_NBIF0_MAX_LATENCY_ADDRESS    0x1015603fUL


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
} MIN_GRANT_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MIN_GRANT_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MIN_GRANT_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MIN_GRANT_OFFSET       0x3e
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MIN_GRANT_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x3e)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MIN_GRANT_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MIN_GRANT_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MIN_GRANT_OFFSET       0x3e
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MIN_GRANT_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x3e)

#define SMN_DEV0_FUNC2_NBIF0_MIN_GRANT_ADDRESS    0x1014203eUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MIN_GRANT_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MIN_GRANT_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MIN_GRANT_OFFSET       0x3e
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MIN_GRANT_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x3e)

#define SMN_DEV0_FUNC3_NBIF0_MIN_GRANT_ADDRESS    0x1014303eUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MIN_GRANT_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MIN_GRANT_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MIN_GRANT_OFFSET       0x3e
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MIN_GRANT_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x3e)

#define SMN_DEV0_FUNC4_NBIF0_MIN_GRANT_ADDRESS    0x1014403eUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MIN_GRANT_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MIN_GRANT_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MIN_GRANT_OFFSET       0x3e
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MIN_GRANT_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x3e)

#define SMN_DEV0_FUNC5_NBIF0_MIN_GRANT_ADDRESS    0x1014503eUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MIN_GRANT_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MIN_GRANT_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MIN_GRANT_OFFSET       0x3e
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MIN_GRANT_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x3e)

#define SMN_DEV0_FUNC6_NBIF0_MIN_GRANT_ADDRESS    0x1014603eUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MIN_GRANT_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MIN_GRANT_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MIN_GRANT_OFFSET       0x3e
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MIN_GRANT_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x3e)

#define SMN_DEV0_FUNC7_NBIF0_MIN_GRANT_ADDRESS    0x1014703eUL

#define SMN_DEV1_FUNC1_NBIF0_MIN_GRANT_ADDRESS    0x1014903eUL

#define SMN_DEV1_FUNC2_NBIF0_MIN_GRANT_ADDRESS    0x1014a03eUL

#define SMN_DEV1_FUNC3_NBIF0_MIN_GRANT_ADDRESS    0x1014b03eUL

#define SMN_DEV2_FUNC1_NBIF0_MIN_GRANT_ADDRESS    0x1015103eUL

#define SMN_DEV2_FUNC2_NBIF0_MIN_GRANT_ADDRESS    0x1015203eUL

#define SMN_DEV2_FUNC3_NBIF0_MIN_GRANT_ADDRESS    0x1015303eUL

#define SMN_DEV2_FUNC4_NBIF0_MIN_GRANT_ADDRESS    0x1015403eUL

#define SMN_DEV2_FUNC5_NBIF0_MIN_GRANT_ADDRESS    0x1015503eUL

#define SMN_DEV2_FUNC6_NBIF0_MIN_GRANT_ADDRESS    0x1015603eUL


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
} MSIX_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_CAP_LIST_OFFSET       0xc0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xc0)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_CAP_LIST_OFFSET       0xc0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xc0)

#define SMN_DEV0_FUNC2_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101420c0UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_CAP_LIST_OFFSET       0xc0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xc0)

#define SMN_DEV0_FUNC3_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101430c0UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_CAP_LIST_OFFSET       0xc0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xc0)

#define SMN_DEV0_FUNC4_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101440c0UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_CAP_LIST_OFFSET       0xc0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xc0)

#define SMN_DEV0_FUNC5_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101450c0UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_CAP_LIST_OFFSET       0xc0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xc0)

#define SMN_DEV0_FUNC6_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101460c0UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_CAP_LIST_OFFSET       0xc0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xc0)

#define SMN_DEV0_FUNC7_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101470c0UL

#define SMN_DEV1_FUNC1_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101490c0UL

#define SMN_DEV1_FUNC2_NBIF0_MSIX_CAP_LIST_ADDRESS    0x1014a0c0UL

#define SMN_DEV1_FUNC3_NBIF0_MSIX_CAP_LIST_ADDRESS    0x1014b0c0UL

#define SMN_DEV2_FUNC1_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101510c0UL

#define SMN_DEV2_FUNC2_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101520c0UL

#define SMN_DEV2_FUNC3_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101530c0UL

#define SMN_DEV2_FUNC4_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101540c0UL

#define SMN_DEV2_FUNC5_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101550c0UL

#define SMN_DEV2_FUNC6_NBIF0_MSIX_CAP_LIST_ADDRESS    0x101560c0UL


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
} MSIX_MSG_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_MSG_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_MSG_CNTL_OFFSET       0xc2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xc2)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_MSG_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_MSG_CNTL_OFFSET       0xc2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xc2)

#define SMN_DEV0_FUNC2_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101420c2UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_MSG_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_MSG_CNTL_OFFSET       0xc2
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xc2)

#define SMN_DEV0_FUNC3_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101430c2UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_MSG_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_MSG_CNTL_OFFSET       0xc2
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xc2)

#define SMN_DEV0_FUNC4_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101440c2UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_MSG_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_MSG_CNTL_OFFSET       0xc2
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xc2)

#define SMN_DEV0_FUNC5_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101450c2UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_MSG_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_MSG_CNTL_OFFSET       0xc2
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xc2)

#define SMN_DEV0_FUNC6_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101460c2UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_MSG_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_MSG_CNTL_OFFSET       0xc2
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xc2)

#define SMN_DEV0_FUNC7_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101470c2UL

#define SMN_DEV1_FUNC1_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101490c2UL

#define SMN_DEV1_FUNC2_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x1014a0c2UL

#define SMN_DEV1_FUNC3_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x1014b0c2UL

#define SMN_DEV2_FUNC1_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101510c2UL

#define SMN_DEV2_FUNC2_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101520c2UL

#define SMN_DEV2_FUNC3_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101530c2UL

#define SMN_DEV2_FUNC4_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101540c2UL

#define SMN_DEV2_FUNC5_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101550c2UL

#define SMN_DEV2_FUNC6_NBIF0_MSIX_MSG_CNTL_ADDRESS    0x101560c2UL


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
} MSIX_PBA_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_PBA_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_PBA_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_PBA_OFFSET       0xc8
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_PBA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xc8)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_PBA_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_PBA_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_PBA_OFFSET       0xc8
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_PBA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xc8)

#define SMN_DEV0_FUNC2_NBIF0_MSIX_PBA_ADDRESS    0x101420c8UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_PBA_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_PBA_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_PBA_OFFSET       0xc8
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_PBA_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xc8)

#define SMN_DEV0_FUNC3_NBIF0_MSIX_PBA_ADDRESS    0x101430c8UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_PBA_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_PBA_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_PBA_OFFSET       0xc8
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_PBA_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xc8)

#define SMN_DEV0_FUNC4_NBIF0_MSIX_PBA_ADDRESS    0x101440c8UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_PBA_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_PBA_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_PBA_OFFSET       0xc8
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_PBA_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xc8)

#define SMN_DEV0_FUNC5_NBIF0_MSIX_PBA_ADDRESS    0x101450c8UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_PBA_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_PBA_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_PBA_OFFSET       0xc8
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_PBA_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xc8)

#define SMN_DEV0_FUNC6_NBIF0_MSIX_PBA_ADDRESS    0x101460c8UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_PBA_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_PBA_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_PBA_OFFSET       0xc8
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_PBA_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xc8)

#define SMN_DEV0_FUNC7_NBIF0_MSIX_PBA_ADDRESS    0x101470c8UL

#define SMN_DEV1_FUNC1_NBIF0_MSIX_PBA_ADDRESS    0x101490c8UL

#define SMN_DEV1_FUNC2_NBIF0_MSIX_PBA_ADDRESS    0x1014a0c8UL

#define SMN_DEV1_FUNC3_NBIF0_MSIX_PBA_ADDRESS    0x1014b0c8UL

#define SMN_DEV2_FUNC1_NBIF0_MSIX_PBA_ADDRESS    0x101510c8UL

#define SMN_DEV2_FUNC2_NBIF0_MSIX_PBA_ADDRESS    0x101520c8UL

#define SMN_DEV2_FUNC3_NBIF0_MSIX_PBA_ADDRESS    0x101530c8UL

#define SMN_DEV2_FUNC4_NBIF0_MSIX_PBA_ADDRESS    0x101540c8UL

#define SMN_DEV2_FUNC5_NBIF0_MSIX_PBA_ADDRESS    0x101550c8UL

#define SMN_DEV2_FUNC6_NBIF0_MSIX_PBA_ADDRESS    0x101560c8UL


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
} MSIX_TABLE_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_TABLE_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_TABLE_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_TABLE_OFFSET       0xc4
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSIX_TABLE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xc4)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_TABLE_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_TABLE_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_TABLE_OFFSET       0xc4
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSIX_TABLE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xc4)

#define SMN_DEV0_FUNC2_NBIF0_MSIX_TABLE_ADDRESS    0x101420c4UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_TABLE_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_TABLE_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_TABLE_OFFSET       0xc4
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSIX_TABLE_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xc4)

#define SMN_DEV0_FUNC3_NBIF0_MSIX_TABLE_ADDRESS    0x101430c4UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_TABLE_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_TABLE_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_TABLE_OFFSET       0xc4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSIX_TABLE_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xc4)

#define SMN_DEV0_FUNC4_NBIF0_MSIX_TABLE_ADDRESS    0x101440c4UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_TABLE_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_TABLE_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_TABLE_OFFSET       0xc4
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSIX_TABLE_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xc4)

#define SMN_DEV0_FUNC5_NBIF0_MSIX_TABLE_ADDRESS    0x101450c4UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_TABLE_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_TABLE_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_TABLE_OFFSET       0xc4
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSIX_TABLE_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xc4)

#define SMN_DEV0_FUNC6_NBIF0_MSIX_TABLE_ADDRESS    0x101460c4UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_TABLE_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_TABLE_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_TABLE_OFFSET       0xc4
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSIX_TABLE_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xc4)

#define SMN_DEV0_FUNC7_NBIF0_MSIX_TABLE_ADDRESS    0x101470c4UL

#define SMN_DEV1_FUNC1_NBIF0_MSIX_TABLE_ADDRESS    0x101490c4UL

#define SMN_DEV1_FUNC2_NBIF0_MSIX_TABLE_ADDRESS    0x1014a0c4UL

#define SMN_DEV1_FUNC3_NBIF0_MSIX_TABLE_ADDRESS    0x1014b0c4UL

#define SMN_DEV2_FUNC1_NBIF0_MSIX_TABLE_ADDRESS    0x101510c4UL

#define SMN_DEV2_FUNC2_NBIF0_MSIX_TABLE_ADDRESS    0x101520c4UL

#define SMN_DEV2_FUNC3_NBIF0_MSIX_TABLE_ADDRESS    0x101530c4UL

#define SMN_DEV2_FUNC4_NBIF0_MSIX_TABLE_ADDRESS    0x101540c4UL

#define SMN_DEV2_FUNC5_NBIF0_MSIX_TABLE_ADDRESS    0x101550c4UL

#define SMN_DEV2_FUNC6_NBIF0_MSIX_TABLE_ADDRESS    0x101560c4UL


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
} MSI_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_CAP_LIST_OFFSET       0xa0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xa0)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_CAP_LIST_OFFSET       0xa0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xa0)

#define SMN_DEV0_FUNC2_NBIF0_MSI_CAP_LIST_ADDRESS    0x101420a0UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_CAP_LIST_OFFSET       0xa0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xa0)

#define SMN_DEV0_FUNC3_NBIF0_MSI_CAP_LIST_ADDRESS    0x101430a0UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_CAP_LIST_OFFSET       0xa0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xa0)

#define SMN_DEV0_FUNC4_NBIF0_MSI_CAP_LIST_ADDRESS    0x101440a0UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_CAP_LIST_OFFSET       0xa0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xa0)

#define SMN_DEV0_FUNC5_NBIF0_MSI_CAP_LIST_ADDRESS    0x101450a0UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_CAP_LIST_OFFSET       0xa0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xa0)

#define SMN_DEV0_FUNC6_NBIF0_MSI_CAP_LIST_ADDRESS    0x101460a0UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_CAP_LIST_OFFSET       0xa0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xa0)

#define SMN_DEV0_FUNC7_NBIF0_MSI_CAP_LIST_ADDRESS    0x101470a0UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_CAP_LIST_ADDRESS    0x101490a0UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_CAP_LIST_ADDRESS    0x1014a0a0UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_CAP_LIST_ADDRESS    0x1014b0a0UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_CAP_LIST_ADDRESS    0x101510a0UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_CAP_LIST_ADDRESS    0x101520a0UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_CAP_LIST_ADDRESS    0x101530a0UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_CAP_LIST_ADDRESS    0x101540a0UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_CAP_LIST_ADDRESS    0x101550a0UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_CAP_LIST_ADDRESS    0x101560a0UL


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
} MSI_EXT_MSG_DATA_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_EXT_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_EXT_MSG_DATA_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_EXT_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_EXT_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_EXT_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_EXT_MSG_DATA_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_EXT_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_EXT_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define SMN_DEV0_FUNC2_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_EXT_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_EXT_MSG_DATA_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_EXT_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_EXT_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x0)

#define SMN_DEV0_FUNC3_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_EXT_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_EXT_MSG_DATA_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_EXT_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_EXT_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x0)

#define SMN_DEV0_FUNC4_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_EXT_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_EXT_MSG_DATA_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_EXT_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_EXT_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x0)

#define SMN_DEV0_FUNC5_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_EXT_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_EXT_MSG_DATA_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_EXT_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_EXT_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x0)

#define SMN_DEV0_FUNC6_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_EXT_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_EXT_MSG_DATA_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_EXT_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_EXT_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x0)

#define SMN_DEV0_FUNC7_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_EXT_MSG_DATA_ADDRESS    0x10100000UL


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
} MSI_EXT_MSG_DATA_64_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_EXT_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_EXT_MSG_DATA_64_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_EXT_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x1)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x1)

#define SMN_DEV0_FUNC2_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x1)

#define SMN_DEV0_FUNC3_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x1)

#define SMN_DEV0_FUNC4_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x1)

#define SMN_DEV0_FUNC5_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x1)

#define SMN_DEV0_FUNC6_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x1)

#define SMN_DEV0_FUNC7_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_EXT_MSG_DATA_64_ADDRESS    0x10100001UL


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
} MSI_MASK_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MASK_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MASK_OFFSET       0x0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MASK_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MASK_OFFSET       0x0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define SMN_DEV0_FUNC2_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MASK_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MASK_OFFSET       0x0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x0)

#define SMN_DEV0_FUNC3_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MASK_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MASK_OFFSET       0x0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x0)

#define SMN_DEV0_FUNC4_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MASK_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MASK_OFFSET       0x0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x0)

#define SMN_DEV0_FUNC5_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MASK_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MASK_OFFSET       0x0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x0)

#define SMN_DEV0_FUNC6_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MASK_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MASK_OFFSET       0x0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x0)

#define SMN_DEV0_FUNC7_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_MASK_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_MASK_ADDRESS    0x10100000UL


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
} MSI_MASK_64_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MASK_64_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MASK_64_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MASK_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MASK_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x1)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MASK_64_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MASK_64_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MASK_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MASK_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x1)

#define SMN_DEV0_FUNC2_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MASK_64_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MASK_64_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MASK_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MASK_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x1)

#define SMN_DEV0_FUNC3_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MASK_64_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MASK_64_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MASK_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MASK_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x1)

#define SMN_DEV0_FUNC4_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MASK_64_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MASK_64_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MASK_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MASK_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x1)

#define SMN_DEV0_FUNC5_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MASK_64_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MASK_64_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MASK_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MASK_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x1)

#define SMN_DEV0_FUNC6_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MASK_64_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MASK_64_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MASK_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MASK_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x1)

#define SMN_DEV0_FUNC7_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_MASK_64_ADDRESS    0x10100001UL


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
} MSI_MSG_ADDR_HI_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_ADDR_HI_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_ADDR_HI_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_ADDR_HI_OFFSET       0x1
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_ADDR_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x1)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_ADDR_HI_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_ADDR_HI_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_ADDR_HI_OFFSET       0x1
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_ADDR_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x1)

#define SMN_DEV0_FUNC2_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_ADDR_HI_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_ADDR_HI_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_ADDR_HI_OFFSET       0x1
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_ADDR_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x1)

#define SMN_DEV0_FUNC3_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_ADDR_HI_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_ADDR_HI_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_ADDR_HI_OFFSET       0x1
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_ADDR_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x1)

#define SMN_DEV0_FUNC4_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_ADDR_HI_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_ADDR_HI_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_ADDR_HI_OFFSET       0x1
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_ADDR_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x1)

#define SMN_DEV0_FUNC5_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_ADDR_HI_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_ADDR_HI_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_ADDR_HI_OFFSET       0x1
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_ADDR_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x1)

#define SMN_DEV0_FUNC6_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_ADDR_HI_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_ADDR_HI_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_ADDR_HI_OFFSET       0x1
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_ADDR_HI_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x1)

#define SMN_DEV0_FUNC7_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_MSG_ADDR_HI_ADDRESS    0x10100001UL


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
} MSI_MSG_ADDR_LO_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_ADDR_LO_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_ADDR_LO_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_ADDR_LO_OFFSET       0xa4
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_ADDR_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xa4)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_ADDR_LO_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_ADDR_LO_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_ADDR_LO_OFFSET       0xa4
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_ADDR_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xa4)

#define SMN_DEV0_FUNC2_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101420a4UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_ADDR_LO_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_ADDR_LO_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_ADDR_LO_OFFSET       0xa4
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_ADDR_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xa4)

#define SMN_DEV0_FUNC3_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101430a4UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_ADDR_LO_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_ADDR_LO_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_ADDR_LO_OFFSET       0xa4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_ADDR_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xa4)

#define SMN_DEV0_FUNC4_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101440a4UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_ADDR_LO_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_ADDR_LO_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_ADDR_LO_OFFSET       0xa4
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_ADDR_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xa4)

#define SMN_DEV0_FUNC5_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101450a4UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_ADDR_LO_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_ADDR_LO_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_ADDR_LO_OFFSET       0xa4
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_ADDR_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xa4)

#define SMN_DEV0_FUNC6_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101460a4UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_ADDR_LO_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_ADDR_LO_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_ADDR_LO_OFFSET       0xa4
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_ADDR_LO_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xa4)

#define SMN_DEV0_FUNC7_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101470a4UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101490a4UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x1014a0a4UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x1014b0a4UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101510a4UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101520a4UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101530a4UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101540a4UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101550a4UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_MSG_ADDR_LO_ADDRESS    0x101560a4UL


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
} MSI_MSG_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_CNTL_OFFSET       0xa2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xa2)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_CNTL_OFFSET       0xa2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xa2)

#define SMN_DEV0_FUNC2_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101420a2UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_CNTL_OFFSET       0xa2
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xa2)

#define SMN_DEV0_FUNC3_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101430a2UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_CNTL_OFFSET       0xa2
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xa2)

#define SMN_DEV0_FUNC4_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101440a2UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_CNTL_OFFSET       0xa2
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xa2)

#define SMN_DEV0_FUNC5_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101450a2UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_CNTL_OFFSET       0xa2
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xa2)

#define SMN_DEV0_FUNC6_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101460a2UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_CNTL_OFFSET       0xa2
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xa2)

#define SMN_DEV0_FUNC7_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101470a2UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101490a2UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_MSG_CNTL_ADDRESS    0x1014a0a2UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_MSG_CNTL_ADDRESS    0x1014b0a2UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101510a2UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101520a2UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101530a2UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101540a2UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101550a2UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_MSG_CNTL_ADDRESS    0x101560a2UL


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
} MSI_MSG_DATA_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_DATA_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_DATA_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define SMN_DEV0_FUNC2_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_DATA_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x0)

#define SMN_DEV0_FUNC3_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_DATA_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x0)

#define SMN_DEV0_FUNC4_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_DATA_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x0)

#define SMN_DEV0_FUNC5_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_DATA_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x0)

#define SMN_DEV0_FUNC6_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_DATA_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_DATA_OFFSET       0x0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x0)

#define SMN_DEV0_FUNC7_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_MSG_DATA_ADDRESS    0x10100000UL


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
} MSI_MSG_DATA_64_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_DATA_64_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x1)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_DATA_64_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x1)

#define SMN_DEV0_FUNC2_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_DATA_64_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x1)

#define SMN_DEV0_FUNC3_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_DATA_64_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x1)

#define SMN_DEV0_FUNC4_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_DATA_64_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x1)

#define SMN_DEV0_FUNC5_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_DATA_64_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x1)

#define SMN_DEV0_FUNC6_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_DATA_64_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_DATA_64_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_DATA_64_OFFSET       0x1
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_MSG_DATA_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x1)

#define SMN_DEV0_FUNC7_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_MSG_DATA_64_ADDRESS    0x10100001UL


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
} MSI_PENDING_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_PENDING_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_PENDING_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_PENDING_OFFSET       0x0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_PENDING_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_PENDING_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_PENDING_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_PENDING_OFFSET       0x0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_PENDING_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define SMN_DEV0_FUNC2_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_PENDING_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_PENDING_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_PENDING_OFFSET       0x0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_PENDING_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x0)

#define SMN_DEV0_FUNC3_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_PENDING_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_PENDING_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_PENDING_OFFSET       0x0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_PENDING_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x0)

#define SMN_DEV0_FUNC4_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_PENDING_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_PENDING_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_PENDING_OFFSET       0x0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_PENDING_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x0)

#define SMN_DEV0_FUNC5_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_PENDING_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_PENDING_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_PENDING_OFFSET       0x0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_PENDING_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x0)

#define SMN_DEV0_FUNC6_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_PENDING_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_PENDING_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_PENDING_OFFSET       0x0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_PENDING_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x0)

#define SMN_DEV0_FUNC7_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_PENDING_ADDRESS    0x10100000UL


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
} MSI_PENDING_64_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_PENDING_64_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_PENDING_64_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_PENDING_64_OFFSET       0xb4
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_MSI_PENDING_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xb4)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_PENDING_64_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_PENDING_64_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_PENDING_64_OFFSET       0xb4
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_MSI_PENDING_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xb4)

#define SMN_DEV0_FUNC2_NBIF0_MSI_PENDING_64_ADDRESS    0x101420b4UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_PENDING_64_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_PENDING_64_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_PENDING_64_OFFSET       0xb4
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_MSI_PENDING_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xb4)

#define SMN_DEV0_FUNC3_NBIF0_MSI_PENDING_64_ADDRESS    0x101430b4UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_PENDING_64_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_PENDING_64_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_PENDING_64_OFFSET       0xb4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_MSI_PENDING_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xb4)

#define SMN_DEV0_FUNC4_NBIF0_MSI_PENDING_64_ADDRESS    0x101440b4UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_PENDING_64_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_PENDING_64_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_PENDING_64_OFFSET       0xb4
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_MSI_PENDING_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xb4)

#define SMN_DEV0_FUNC5_NBIF0_MSI_PENDING_64_ADDRESS    0x101450b4UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_PENDING_64_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_PENDING_64_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_PENDING_64_OFFSET       0xb4
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_MSI_PENDING_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xb4)

#define SMN_DEV0_FUNC6_NBIF0_MSI_PENDING_64_ADDRESS    0x101460b4UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_PENDING_64_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_PENDING_64_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_PENDING_64_OFFSET       0xb4
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_MSI_PENDING_64_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xb4)

#define SMN_DEV0_FUNC7_NBIF0_MSI_PENDING_64_ADDRESS    0x101470b4UL

#define SMN_DEV1_FUNC1_NBIF0_MSI_PENDING_64_ADDRESS    0x101490b4UL

#define SMN_DEV1_FUNC2_NBIF0_MSI_PENDING_64_ADDRESS    0x1014a0b4UL

#define SMN_DEV1_FUNC3_NBIF0_MSI_PENDING_64_ADDRESS    0x1014b0b4UL

#define SMN_DEV2_FUNC1_NBIF0_MSI_PENDING_64_ADDRESS    0x101510b4UL

#define SMN_DEV2_FUNC2_NBIF0_MSI_PENDING_64_ADDRESS    0x101520b4UL

#define SMN_DEV2_FUNC3_NBIF0_MSI_PENDING_64_ADDRESS    0x101530b4UL

#define SMN_DEV2_FUNC4_NBIF0_MSI_PENDING_64_ADDRESS    0x101540b4UL

#define SMN_DEV2_FUNC5_NBIF0_MSI_PENDING_64_ADDRESS    0x101550b4UL

#define SMN_DEV2_FUNC6_NBIF0_MSI_PENDING_64_ADDRESS    0x101560b4UL


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
} PCIE_ACS_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_CAP_OFFSET       0x2a4
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2a4)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_CAP_OFFSET       0x2a4
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2a4)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101422a4UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_CAP_OFFSET       0x2a4
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x2a4)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101432a4UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_CAP_OFFSET       0x2a4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x2a4)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101442a4UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_CAP_OFFSET       0x2a4
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x2a4)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101452a4UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_CAP_OFFSET       0x2a4
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x2a4)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101462a4UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_CAP_OFFSET       0x2a4
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x2a4)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101472a4UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101492a4UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_ACS_CAP_ADDRESS    0x1014a2a4UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_ACS_CAP_ADDRESS    0x1014b2a4UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101512a4UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101522a4UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101532a4UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101542a4UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101552a4UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_ACS_CAP_ADDRESS    0x101562a4UL


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
} PCIE_ACS_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_CNTL_OFFSET       0x2a6
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2a6)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_CNTL_OFFSET       0x2a6
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2a6)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101422a6UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_CNTL_OFFSET       0x2a6
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x2a6)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101432a6UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_CNTL_OFFSET       0x2a6
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x2a6)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101442a6UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_CNTL_OFFSET       0x2a6
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x2a6)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101452a6UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_CNTL_OFFSET       0x2a6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x2a6)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101462a6UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_CNTL_OFFSET       0x2a6
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x2a6)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101472a6UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101492a6UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x1014a2a6UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x1014b2a6UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101512a6UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101522a6UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101532a6UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101542a6UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101552a6UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_ACS_CNTL_ADDRESS    0x101562a6UL


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
} PCIE_ACS_ENH_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_ENH_CAP_LIST_OFFSET       0x2a0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2a0)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_OFFSET       0x2a0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2a0)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101422a0UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_OFFSET       0x2a0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x2a0)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101432a0UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_OFFSET       0x2a0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x2a0)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101442a0UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_OFFSET       0x2a0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x2a0)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101452a0UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_OFFSET       0x2a0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x2a0)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101462a0UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_OFFSET       0x2a0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x2a0)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101472a0UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101492a0UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x1014a2a0UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x1014b2a0UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101512a0UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101522a0UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101532a0UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101542a0UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101552a0UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_ACS_ENH_CAP_LIST_ADDRESS    0x101562a0UL


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
} PCIE_ADV_ERR_CAP_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_OFFSET       0x168
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x168)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_OFFSET       0x168
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x168)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10142168UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_OFFSET       0x168
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x168)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10143168UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_OFFSET       0x168
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x168)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10144168UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_OFFSET       0x168
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x168)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10145168UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_OFFSET       0x168
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x168)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10146168UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_OFFSET       0x168
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x168)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10147168UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10149168UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x1014a168UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x1014b168UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10151168UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10152168UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10153168UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10154168UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10155168UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_ADV_ERR_CAP_CNTL_ADDRESS    0x10156168UL


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
} PCIE_ADV_ERR_RPT_ENH_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_OFFSET       0x150
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x150)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_OFFSET       0x150
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x150)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10142150UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_OFFSET       0x150
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x150)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10143150UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_OFFSET       0x150
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x150)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10144150UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_OFFSET       0x150
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x150)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10145150UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_OFFSET       0x150
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x150)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10146150UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_OFFSET       0x150
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x150)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10147150UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10149150UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x1014a150UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x1014b150UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10151150UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10152150UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10153150UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10154150UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10155150UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_ADV_ERR_RPT_ENH_CAP_LIST_ADDRESS    0x10156150UL


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
} PCIE_ARI_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_CAP_OFFSET       0x32c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x32c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_CAP_OFFSET       0x32c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x32c)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1014232cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_CAP_OFFSET       0x32c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x32c)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1014332cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_CAP_OFFSET       0x32c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x32c)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1014432cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_CAP_OFFSET       0x32c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x32c)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1014532cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_CAP_OFFSET       0x32c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x32c)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1014632cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_CAP_OFFSET       0x32c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x32c)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1014732cUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1014932cUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1014a32cUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1014b32cUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1015132cUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1015232cUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1015332cUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1015432cUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1015532cUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_ARI_CAP_ADDRESS    0x1015632cUL


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
} PCIE_ARI_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_CNTL_OFFSET       0x32e
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x32e)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_CNTL_OFFSET       0x32e
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x32e)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1014232eUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_CNTL_OFFSET       0x32e
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x32e)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1014332eUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_CNTL_OFFSET       0x32e
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x32e)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1014432eUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_CNTL_OFFSET       0x32e
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x32e)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1014532eUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_CNTL_OFFSET       0x32e
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x32e)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1014632eUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_CNTL_OFFSET       0x32e
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x32e)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1014732eUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1014932eUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1014a32eUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1014b32eUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1015132eUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1015232eUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1015332eUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1015432eUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1015532eUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_ARI_CNTL_ADDRESS    0x1015632eUL


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
} PCIE_ARI_ENH_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_ENH_CAP_LIST_OFFSET       0x328
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x328)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_OFFSET       0x328
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x328)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10142328UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_OFFSET       0x328
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x328)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10143328UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_OFFSET       0x328
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x328)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10144328UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_OFFSET       0x328
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x328)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10145328UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_OFFSET       0x328
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x328)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10146328UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_OFFSET       0x328
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x328)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10147328UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10149328UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x1014a328UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x1014b328UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10151328UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10152328UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10153328UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10154328UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10155328UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_ARI_ENH_CAP_LIST_ADDRESS    0x10156328UL


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
} PCIE_BAR1_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR1_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR1_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR1_CAP_OFFSET       0x204
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR1_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x204)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR1_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR1_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR1_CAP_OFFSET       0x204
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR1_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x204)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10142204UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR1_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR1_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR1_CAP_OFFSET       0x204
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR1_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x204)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10143204UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR1_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR1_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR1_CAP_OFFSET       0x204
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR1_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x204)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10144204UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR1_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR1_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR1_CAP_OFFSET       0x204
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR1_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x204)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10145204UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR1_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR1_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR1_CAP_OFFSET       0x204
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR1_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x204)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10146204UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR1_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR1_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR1_CAP_OFFSET       0x204
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR1_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x204)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10147204UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10149204UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x1014a204UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x1014b204UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10151204UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10152204UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10153204UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10154204UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10155204UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR1_CAP_ADDRESS    0x10156204UL


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
} PCIE_BAR1_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR1_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR1_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR1_CNTL_OFFSET       0x208
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR1_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x208)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR1_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR1_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR1_CNTL_OFFSET       0x208
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR1_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x208)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10142208UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR1_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR1_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR1_CNTL_OFFSET       0x208
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR1_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x208)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10143208UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR1_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR1_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR1_CNTL_OFFSET       0x208
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR1_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x208)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10144208UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR1_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR1_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR1_CNTL_OFFSET       0x208
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR1_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x208)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10145208UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR1_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR1_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR1_CNTL_OFFSET       0x208
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR1_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x208)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10146208UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR1_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR1_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR1_CNTL_OFFSET       0x208
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR1_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x208)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10147208UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10149208UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x1014a208UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x1014b208UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10151208UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10152208UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10153208UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10154208UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10155208UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR1_CNTL_ADDRESS    0x10156208UL


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
} PCIE_BAR2_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR2_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR2_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR2_CAP_OFFSET       0x20c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR2_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x20c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR2_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR2_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR2_CAP_OFFSET       0x20c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR2_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x20c)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1014220cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR2_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR2_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR2_CAP_OFFSET       0x20c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR2_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x20c)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1014320cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR2_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR2_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR2_CAP_OFFSET       0x20c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR2_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x20c)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1014420cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR2_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR2_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR2_CAP_OFFSET       0x20c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR2_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x20c)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1014520cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR2_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR2_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR2_CAP_OFFSET       0x20c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR2_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x20c)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1014620cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR2_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR2_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR2_CAP_OFFSET       0x20c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR2_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x20c)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1014720cUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1014920cUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1014a20cUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1014b20cUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1015120cUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1015220cUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1015320cUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1015420cUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1015520cUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR2_CAP_ADDRESS    0x1015620cUL


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
} PCIE_BAR2_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR2_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR2_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR2_CNTL_OFFSET       0x210
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR2_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x210)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR2_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR2_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR2_CNTL_OFFSET       0x210
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR2_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x210)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10142210UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR2_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR2_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR2_CNTL_OFFSET       0x210
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR2_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x210)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10143210UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR2_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR2_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR2_CNTL_OFFSET       0x210
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR2_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x210)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10144210UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR2_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR2_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR2_CNTL_OFFSET       0x210
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR2_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x210)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10145210UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR2_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR2_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR2_CNTL_OFFSET       0x210
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR2_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x210)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10146210UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR2_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR2_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR2_CNTL_OFFSET       0x210
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR2_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x210)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10147210UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10149210UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x1014a210UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x1014b210UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10151210UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10152210UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10153210UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10154210UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10155210UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR2_CNTL_ADDRESS    0x10156210UL


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
} PCIE_BAR3_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR3_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR3_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR3_CAP_OFFSET       0x214
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR3_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x214)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR3_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR3_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR3_CAP_OFFSET       0x214
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR3_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x214)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10142214UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR3_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR3_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR3_CAP_OFFSET       0x214
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR3_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x214)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10143214UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR3_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR3_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR3_CAP_OFFSET       0x214
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR3_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x214)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10144214UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR3_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR3_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR3_CAP_OFFSET       0x214
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR3_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x214)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10145214UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR3_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR3_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR3_CAP_OFFSET       0x214
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR3_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x214)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10146214UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR3_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR3_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR3_CAP_OFFSET       0x214
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR3_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x214)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10147214UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10149214UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x1014a214UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x1014b214UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10151214UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10152214UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10153214UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10154214UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10155214UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR3_CAP_ADDRESS    0x10156214UL


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
} PCIE_BAR3_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR3_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR3_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR3_CNTL_OFFSET       0x218
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR3_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x218)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR3_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR3_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR3_CNTL_OFFSET       0x218
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR3_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x218)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10142218UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR3_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR3_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR3_CNTL_OFFSET       0x218
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR3_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x218)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10143218UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR3_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR3_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR3_CNTL_OFFSET       0x218
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR3_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x218)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10144218UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR3_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR3_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR3_CNTL_OFFSET       0x218
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR3_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x218)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10145218UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR3_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR3_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR3_CNTL_OFFSET       0x218
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR3_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x218)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10146218UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR3_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR3_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR3_CNTL_OFFSET       0x218
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR3_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x218)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10147218UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10149218UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x1014a218UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x1014b218UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10151218UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10152218UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10153218UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10154218UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10155218UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR3_CNTL_ADDRESS    0x10156218UL


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
} PCIE_BAR4_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR4_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR4_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR4_CAP_OFFSET       0x21c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR4_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x21c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR4_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR4_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR4_CAP_OFFSET       0x21c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR4_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x21c)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1014221cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR4_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR4_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR4_CAP_OFFSET       0x21c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR4_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x21c)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1014321cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR4_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR4_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR4_CAP_OFFSET       0x21c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR4_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x21c)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1014421cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR4_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR4_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR4_CAP_OFFSET       0x21c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR4_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x21c)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1014521cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR4_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR4_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR4_CAP_OFFSET       0x21c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR4_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x21c)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1014621cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR4_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR4_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR4_CAP_OFFSET       0x21c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR4_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x21c)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1014721cUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1014921cUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1014a21cUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1014b21cUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1015121cUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1015221cUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1015321cUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1015421cUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1015521cUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR4_CAP_ADDRESS    0x1015621cUL


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
} PCIE_BAR4_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR4_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR4_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR4_CNTL_OFFSET       0x220
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR4_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x220)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR4_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR4_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR4_CNTL_OFFSET       0x220
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR4_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x220)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10142220UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR4_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR4_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR4_CNTL_OFFSET       0x220
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR4_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x220)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10143220UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR4_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR4_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR4_CNTL_OFFSET       0x220
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR4_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x220)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10144220UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR4_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR4_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR4_CNTL_OFFSET       0x220
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR4_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x220)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10145220UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR4_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR4_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR4_CNTL_OFFSET       0x220
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR4_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x220)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10146220UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR4_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR4_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR4_CNTL_OFFSET       0x220
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR4_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x220)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10147220UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10149220UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x1014a220UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x1014b220UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10151220UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10152220UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10153220UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10154220UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10155220UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR4_CNTL_ADDRESS    0x10156220UL


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
} PCIE_BAR5_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR5_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR5_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR5_CAP_OFFSET       0x224
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR5_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x224)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR5_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR5_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR5_CAP_OFFSET       0x224
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR5_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x224)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10142224UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR5_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR5_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR5_CAP_OFFSET       0x224
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR5_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x224)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10143224UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR5_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR5_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR5_CAP_OFFSET       0x224
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR5_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x224)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10144224UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR5_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR5_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR5_CAP_OFFSET       0x224
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR5_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x224)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10145224UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR5_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR5_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR5_CAP_OFFSET       0x224
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR5_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x224)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10146224UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR5_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR5_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR5_CAP_OFFSET       0x224
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR5_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x224)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10147224UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10149224UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x1014a224UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x1014b224UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10151224UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10152224UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10153224UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10154224UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10155224UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR5_CAP_ADDRESS    0x10156224UL


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
} PCIE_BAR5_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR5_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR5_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR5_CNTL_OFFSET       0x228
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR5_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x228)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR5_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR5_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR5_CNTL_OFFSET       0x228
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR5_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x228)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10142228UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR5_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR5_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR5_CNTL_OFFSET       0x228
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR5_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x228)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10143228UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR5_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR5_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR5_CNTL_OFFSET       0x228
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR5_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x228)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10144228UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR5_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR5_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR5_CNTL_OFFSET       0x228
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR5_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x228)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10145228UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR5_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR5_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR5_CNTL_OFFSET       0x228
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR5_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x228)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10146228UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR5_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR5_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR5_CNTL_OFFSET       0x228
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR5_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x228)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10147228UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10149228UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x1014a228UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x1014b228UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10151228UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10152228UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10153228UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10154228UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10155228UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR5_CNTL_ADDRESS    0x10156228UL


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
} PCIE_BAR6_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR6_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR6_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR6_CAP_OFFSET       0x22c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR6_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x22c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR6_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR6_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR6_CAP_OFFSET       0x22c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR6_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x22c)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1014222cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR6_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR6_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR6_CAP_OFFSET       0x22c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR6_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x22c)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1014322cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR6_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR6_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR6_CAP_OFFSET       0x22c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR6_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x22c)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1014422cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR6_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR6_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR6_CAP_OFFSET       0x22c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR6_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x22c)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1014522cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR6_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR6_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR6_CAP_OFFSET       0x22c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR6_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x22c)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1014622cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR6_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR6_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR6_CAP_OFFSET       0x22c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR6_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x22c)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1014722cUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1014922cUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1014a22cUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1014b22cUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1015122cUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1015222cUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1015322cUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1015422cUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1015522cUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR6_CAP_ADDRESS    0x1015622cUL


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
} PCIE_BAR6_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR6_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR6_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR6_CNTL_OFFSET       0x230
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR6_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x230)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR6_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR6_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR6_CNTL_OFFSET       0x230
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR6_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x230)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10142230UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR6_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR6_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR6_CNTL_OFFSET       0x230
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR6_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x230)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10143230UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR6_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR6_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR6_CNTL_OFFSET       0x230
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR6_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x230)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10144230UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR6_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR6_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR6_CNTL_OFFSET       0x230
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR6_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x230)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10145230UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR6_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR6_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR6_CNTL_OFFSET       0x230
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR6_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x230)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10146230UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR6_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR6_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR6_CNTL_OFFSET       0x230
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR6_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x230)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10147230UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10149230UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x1014a230UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x1014b230UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10151230UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10152230UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10153230UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10154230UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10155230UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR6_CNTL_ADDRESS    0x10156230UL


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
} PCIE_BAR_ENH_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR_ENH_CAP_LIST_OFFSET       0x200
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x200)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_OFFSET       0x200
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x200)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10142200UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_OFFSET       0x200
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x200)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10143200UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_OFFSET       0x200
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x200)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10144200UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_OFFSET       0x200
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x200)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10145200UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_OFFSET       0x200
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x200)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10146200UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_OFFSET       0x200
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x200)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10147200UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10149200UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x1014a200UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x1014b200UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10151200UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10152200UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10153200UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10154200UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10155200UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_BAR_ENH_CAP_LIST_ADDRESS    0x10156200UL


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
} PCIE_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CAP_OFFSET       0x66
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x66)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CAP_OFFSET       0x66
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x66)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_CAP_ADDRESS    0x10142066UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CAP_OFFSET       0x66
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x66)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_CAP_ADDRESS    0x10143066UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CAP_OFFSET       0x66
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x66)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_CAP_ADDRESS    0x10144066UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CAP_OFFSET       0x66
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x66)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_CAP_ADDRESS    0x10145066UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CAP_OFFSET       0x66
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x66)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_CAP_ADDRESS    0x10146066UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CAP_OFFSET       0x66
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x66)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_CAP_ADDRESS    0x10147066UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_CAP_ADDRESS    0x10149066UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_CAP_ADDRESS    0x1014a066UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_CAP_ADDRESS    0x1014b066UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_CAP_ADDRESS    0x10151066UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_CAP_ADDRESS    0x10152066UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_CAP_ADDRESS    0x10153066UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_CAP_ADDRESS    0x10154066UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_CAP_ADDRESS    0x10155066UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_CAP_ADDRESS    0x10156066UL


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
} PCIE_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CAP_LIST_OFFSET       0x64
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x64)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CAP_LIST_OFFSET       0x64
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x64)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10142064UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CAP_LIST_OFFSET       0x64
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x64)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10143064UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CAP_LIST_OFFSET       0x64
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x64)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10144064UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CAP_LIST_OFFSET       0x64
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x64)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10145064UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CAP_LIST_OFFSET       0x64
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x64)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10146064UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CAP_LIST_OFFSET       0x64
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x64)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10147064UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10149064UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_CAP_LIST_ADDRESS    0x1014a064UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_CAP_LIST_ADDRESS    0x1014b064UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10151064UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10152064UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10153064UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10154064UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10155064UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_CAP_LIST_ADDRESS    0x10156064UL


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
} PCIE_CORR_ERR_MASK_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CORR_ERR_MASK_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CORR_ERR_MASK_OFFSET       0x164
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x164)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CORR_ERR_MASK_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CORR_ERR_MASK_OFFSET       0x164
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x164)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10142164UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CORR_ERR_MASK_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CORR_ERR_MASK_OFFSET       0x164
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x164)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10143164UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CORR_ERR_MASK_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CORR_ERR_MASK_OFFSET       0x164
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x164)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10144164UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CORR_ERR_MASK_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CORR_ERR_MASK_OFFSET       0x164
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x164)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10145164UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CORR_ERR_MASK_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CORR_ERR_MASK_OFFSET       0x164
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x164)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10146164UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CORR_ERR_MASK_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CORR_ERR_MASK_OFFSET       0x164
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x164)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10147164UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10149164UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x1014a164UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x1014b164UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10151164UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10152164UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10153164UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10154164UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10155164UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_CORR_ERR_MASK_ADDRESS    0x10156164UL


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
} PCIE_CORR_ERR_STATUS_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CORR_ERR_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CORR_ERR_STATUS_OFFSET       0x160
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x160)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_OFFSET       0x160
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x160)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10142160UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_OFFSET       0x160
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x160)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10143160UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_OFFSET       0x160
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x160)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10144160UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_OFFSET       0x160
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x160)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10145160UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_OFFSET       0x160
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x160)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10146160UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_OFFSET       0x160
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x160)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10147160UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10149160UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x1014a160UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x1014b160UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10151160UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10152160UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10153160UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10154160UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10155160UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_CORR_ERR_STATUS_ADDRESS    0x10156160UL


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
} PCIE_DPA_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_CAP_OFFSET       0x254
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x254)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_CAP_OFFSET       0x254
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x254)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10142254UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_CAP_OFFSET       0x254
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x254)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10143254UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_CAP_OFFSET       0x254
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x254)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10144254UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_CAP_OFFSET       0x254
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x254)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10145254UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_CAP_OFFSET       0x254
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x254)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10146254UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_CAP_OFFSET       0x254
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x254)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10147254UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10149254UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_CAP_ADDRESS    0x1014a254UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_CAP_ADDRESS    0x1014b254UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10151254UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10152254UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10153254UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10154254UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10155254UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_CAP_ADDRESS    0x10156254UL


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
} PCIE_DPA_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_CNTL_OFFSET       0x25e
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x25e)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_CNTL_OFFSET       0x25e
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x25e)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1014225eUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_CNTL_OFFSET       0x25e
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x25e)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1014325eUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_CNTL_OFFSET       0x25e
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x25e)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1014425eUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_CNTL_OFFSET       0x25e
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x25e)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1014525eUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_CNTL_OFFSET       0x25e
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x25e)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1014625eUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_CNTL_OFFSET       0x25e
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x25e)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1014725eUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1014925eUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1014a25eUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1014b25eUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1015125eUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1015225eUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1015325eUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1015425eUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1015525eUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_CNTL_ADDRESS    0x1015625eUL


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
} PCIE_DPA_ENH_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_ENH_CAP_LIST_OFFSET       0x250
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x250)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_OFFSET       0x250
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x250)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10142250UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_OFFSET       0x250
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x250)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10143250UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_OFFSET       0x250
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x250)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10144250UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_OFFSET       0x250
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x250)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10145250UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_OFFSET       0x250
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x250)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10146250UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_OFFSET       0x250
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x250)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10147250UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10149250UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x1014a250UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x1014b250UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10151250UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10152250UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10153250UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10154250UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10155250UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_ENH_CAP_LIST_ADDRESS    0x10156250UL


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
} PCIE_DPA_LATENCY_INDICATOR_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_OFFSET       0x258
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x258)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_OFFSET       0x258
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x258)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10142258UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_OFFSET       0x258
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x258)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10143258UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_OFFSET       0x258
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x258)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10144258UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_OFFSET       0x258
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x258)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10145258UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_OFFSET       0x258
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x258)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10146258UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_OFFSET       0x258
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x258)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10147258UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10149258UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x1014a258UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x1014b258UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10151258UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10152258UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10153258UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10154258UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10155258UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_LATENCY_INDICATOR_ADDRESS    0x10156258UL


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
} PCIE_DPA_STATUS_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_STATUS_OFFSET       0x25c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x25c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_STATUS_OFFSET       0x25c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x25c)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1014225cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_STATUS_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_STATUS_OFFSET       0x25c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x25c)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1014325cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_STATUS_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_STATUS_OFFSET       0x25c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x25c)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1014425cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_STATUS_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_STATUS_OFFSET       0x25c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x25c)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1014525cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_STATUS_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_STATUS_OFFSET       0x25c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x25c)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1014625cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_STATUS_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_STATUS_OFFSET       0x25c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x25c)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1014725cUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1014925cUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1014a25cUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1014b25cUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1015125cUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1015225cUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1015325cUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1015425cUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1015525cUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_STATUS_ADDRESS    0x1015625cUL


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
} PCIE_DPA_SUBSTATE_PWR_ALLOC_0_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_OFFSET       0x260
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x260)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_OFFSET       0x260
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x260)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10142260UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_OFFSET       0x260
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x260)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10143260UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_OFFSET       0x260
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x260)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10144260UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_OFFSET       0x260
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x260)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10145260UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_OFFSET       0x260
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x260)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10146260UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_OFFSET       0x260
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x260)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10147260UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10149260UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x1014a260UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x1014b260UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10151260UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10152260UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10153260UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10154260UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10155260UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_0_ADDRESS    0x10156260UL


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
} PCIE_DPA_SUBSTATE_PWR_ALLOC_1_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_OFFSET       0x261
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x261)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_OFFSET       0x261
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x261)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10142261UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_OFFSET       0x261
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x261)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10143261UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_OFFSET       0x261
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x261)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10144261UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_OFFSET       0x261
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x261)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10145261UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_OFFSET       0x261
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x261)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10146261UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_OFFSET       0x261
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x261)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10147261UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10149261UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x1014a261UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x1014b261UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10151261UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10152261UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10153261UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10154261UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10155261UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_1_ADDRESS    0x10156261UL


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
} PCIE_DPA_SUBSTATE_PWR_ALLOC_2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_OFFSET       0x262
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x262)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_OFFSET       0x262
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x262)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10142262UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_OFFSET       0x262
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x262)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10143262UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_OFFSET       0x262
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x262)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10144262UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_OFFSET       0x262
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x262)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10145262UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_OFFSET       0x262
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x262)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10146262UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_OFFSET       0x262
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x262)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10147262UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10149262UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x1014a262UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x1014b262UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10151262UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10152262UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10153262UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10154262UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10155262UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_2_ADDRESS    0x10156262UL


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
} PCIE_DPA_SUBSTATE_PWR_ALLOC_3_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_OFFSET       0x263
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x263)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_OFFSET       0x263
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x263)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10142263UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_OFFSET       0x263
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x263)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10143263UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_OFFSET       0x263
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x263)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10144263UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_OFFSET       0x263
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x263)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10145263UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_OFFSET       0x263
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x263)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10146263UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_OFFSET       0x263
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x263)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10147263UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10149263UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x1014a263UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x1014b263UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10151263UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10152263UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10153263UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10154263UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10155263UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_3_ADDRESS    0x10156263UL


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
} PCIE_DPA_SUBSTATE_PWR_ALLOC_4_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_OFFSET       0x264
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x264)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_OFFSET       0x264
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x264)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10142264UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_OFFSET       0x264
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x264)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10143264UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_OFFSET       0x264
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x264)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10144264UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_OFFSET       0x264
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x264)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10145264UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_OFFSET       0x264
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x264)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10146264UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_OFFSET       0x264
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x264)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10147264UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10149264UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x1014a264UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x1014b264UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10151264UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10152264UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10153264UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10154264UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10155264UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_4_ADDRESS    0x10156264UL


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
} PCIE_DPA_SUBSTATE_PWR_ALLOC_5_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_OFFSET       0x265
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x265)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_OFFSET       0x265
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x265)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10142265UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_OFFSET       0x265
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x265)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10143265UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_OFFSET       0x265
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x265)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10144265UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_OFFSET       0x265
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x265)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10145265UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_OFFSET       0x265
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x265)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10146265UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_OFFSET       0x265
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x265)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10147265UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10149265UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x1014a265UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x1014b265UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10151265UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10152265UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10153265UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10154265UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10155265UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_5_ADDRESS    0x10156265UL


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
} PCIE_DPA_SUBSTATE_PWR_ALLOC_6_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_OFFSET       0x266
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x266)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_OFFSET       0x266
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x266)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10142266UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_OFFSET       0x266
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x266)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10143266UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_OFFSET       0x266
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x266)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10144266UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_OFFSET       0x266
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x266)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10145266UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_OFFSET       0x266
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x266)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10146266UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_OFFSET       0x266
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x266)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10147266UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10149266UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x1014a266UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x1014b266UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10151266UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10152266UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10153266UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10154266UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10155266UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_6_ADDRESS    0x10156266UL


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
} PCIE_DPA_SUBSTATE_PWR_ALLOC_7_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_OFFSET       0x267
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x267)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_OFFSET       0x267
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x267)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10142267UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_OFFSET       0x267
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x267)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10143267UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_OFFSET       0x267
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x267)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10144267UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_OFFSET       0x267
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x267)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10145267UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_OFFSET       0x267
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x267)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10146267UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_OFFSET       0x267
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x267)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10147267UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10149267UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x1014a267UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x1014b267UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10151267UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10152267UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10153267UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10154267UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10155267UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_DPA_SUBSTATE_PWR_ALLOC_7_ADDRESS    0x10156267UL


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
} PCIE_HDR_LOG0_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG0_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG0_OFFSET       0x16c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x16c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG0_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG0_OFFSET       0x16c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x16c)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1014216cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG0_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG0_OFFSET       0x16c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x16c)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1014316cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG0_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG0_OFFSET       0x16c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x16c)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1014416cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG0_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG0_OFFSET       0x16c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x16c)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1014516cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG0_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG0_OFFSET       0x16c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x16c)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1014616cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG0_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG0_OFFSET       0x16c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x16c)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1014716cUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1014916cUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1014a16cUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1014b16cUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1015116cUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1015216cUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1015316cUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1015416cUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1015516cUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_HDR_LOG0_ADDRESS    0x1015616cUL


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
} PCIE_HDR_LOG1_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG1_OFFSET       0x170
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x170)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG1_OFFSET       0x170
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x170)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10142170UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG1_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG1_OFFSET       0x170
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x170)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10143170UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG1_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG1_OFFSET       0x170
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x170)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10144170UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG1_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG1_OFFSET       0x170
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x170)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10145170UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG1_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG1_OFFSET       0x170
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x170)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10146170UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG1_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG1_OFFSET       0x170
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x170)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10147170UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10149170UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x1014a170UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x1014b170UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10151170UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10152170UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10153170UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10154170UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10155170UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_HDR_LOG1_ADDRESS    0x10156170UL


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
} PCIE_HDR_LOG2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG2_OFFSET       0x174
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x174)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG2_OFFSET       0x174
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x174)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10142174UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG2_OFFSET       0x174
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x174)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10143174UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG2_OFFSET       0x174
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x174)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10144174UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG2_OFFSET       0x174
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x174)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10145174UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG2_OFFSET       0x174
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x174)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10146174UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG2_OFFSET       0x174
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x174)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10147174UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10149174UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x1014a174UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x1014b174UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10151174UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10152174UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10153174UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10154174UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10155174UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_HDR_LOG2_ADDRESS    0x10156174UL


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
} PCIE_HDR_LOG3_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG3_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG3_OFFSET       0x178
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_HDR_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x178)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG3_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG3_OFFSET       0x178
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_HDR_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x178)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10142178UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG3_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG3_OFFSET       0x178
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_HDR_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x178)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10143178UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG3_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG3_OFFSET       0x178
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_HDR_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x178)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10144178UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG3_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG3_OFFSET       0x178
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_HDR_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x178)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10145178UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG3_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG3_OFFSET       0x178
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_HDR_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x178)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10146178UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG3_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG3_OFFSET       0x178
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_HDR_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x178)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10147178UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10149178UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x1014a178UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x1014b178UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10151178UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10152178UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10153178UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10154178UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10155178UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_HDR_LOG3_ADDRESS    0x10156178UL


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
} PCIE_PASID_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_CAP_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2d4)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_CAP_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2d4)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101422d4UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_CAP_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x2d4)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101432d4UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_CAP_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x2d4)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101442d4UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_CAP_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x2d4)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101452d4UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_CAP_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x2d4)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101462d4UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_CAP_OFFSET       0x2d4
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x2d4)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101472d4UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101492d4UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_PASID_CAP_ADDRESS    0x1014a2d4UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_PASID_CAP_ADDRESS    0x1014b2d4UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101512d4UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101522d4UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101532d4UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101542d4UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101552d4UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_PASID_CAP_ADDRESS    0x101562d4UL


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
} PCIE_PASID_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_CNTL_OFFSET       0x2d6
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2d6)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_CNTL_OFFSET       0x2d6
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2d6)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101422d6UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_CNTL_OFFSET       0x2d6
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x2d6)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101432d6UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_CNTL_OFFSET       0x2d6
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x2d6)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101442d6UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_CNTL_OFFSET       0x2d6
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x2d6)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101452d6UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_CNTL_OFFSET       0x2d6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x2d6)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101462d6UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_CNTL_OFFSET       0x2d6
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x2d6)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101472d6UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101492d6UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x1014a2d6UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x1014b2d6UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101512d6UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101522d6UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101532d6UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101542d6UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101552d6UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_PASID_CNTL_ADDRESS    0x101562d6UL


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
} PCIE_PASID_ENH_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_ENH_CAP_LIST_OFFSET       0x2d0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2d0)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_OFFSET       0x2d0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x2d0)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101422d0UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_OFFSET       0x2d0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x2d0)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101432d0UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_OFFSET       0x2d0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x2d0)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101442d0UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_OFFSET       0x2d0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x2d0)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101452d0UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_OFFSET       0x2d0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x2d0)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101462d0UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_OFFSET       0x2d0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x2d0)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101472d0UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101492d0UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x1014a2d0UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x1014b2d0UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101512d0UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101522d0UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101532d0UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101542d0UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101552d0UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_PASID_ENH_CAP_LIST_ADDRESS    0x101562d0UL


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
} PCIE_PWR_BUDGET_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_CAP_OFFSET       0x24c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x24c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_OFFSET       0x24c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x24c)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1014224cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_OFFSET       0x24c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x24c)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1014324cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_OFFSET       0x24c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x24c)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1014424cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_OFFSET       0x24c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x24c)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1014524cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_OFFSET       0x24c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x24c)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1014624cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_OFFSET       0x24c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x24c)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1014724cUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1014924cUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1014a24cUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1014b24cUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1015124cUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1015224cUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1015324cUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1015424cUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1015524cUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_PWR_BUDGET_CAP_ADDRESS    0x1015624cUL


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
} PCIE_PWR_BUDGET_DATA_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_OFFSET       0x248
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x248)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_OFFSET       0x248
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x248)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10142248UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_OFFSET       0x248
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x248)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10143248UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_OFFSET       0x248
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x248)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10144248UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_OFFSET       0x248
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x248)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10145248UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_OFFSET       0x248
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x248)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10146248UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_OFFSET       0x248
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x248)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10147248UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10149248UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x1014a248UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x1014b248UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10151248UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10152248UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10153248UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10154248UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10155248UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_PWR_BUDGET_DATA_ADDRESS    0x10156248UL


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
} PCIE_PWR_BUDGET_DATA_SELECT_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_OFFSET       0x244
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x244)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_OFFSET       0x244
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x244)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10142244UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_OFFSET       0x244
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x244)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10143244UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_OFFSET       0x244
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x244)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10144244UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_OFFSET       0x244
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x244)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10145244UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_OFFSET       0x244
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x244)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10146244UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_OFFSET       0x244
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x244)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10147244UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10149244UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x1014a244UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x1014b244UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10151244UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10152244UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10153244UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10154244UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10155244UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_PWR_BUDGET_DATA_SELECT_ADDRESS    0x10156244UL


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
} PCIE_PWR_BUDGET_ENH_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_OFFSET       0x240
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x240)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_OFFSET       0x240
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x240)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10142240UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_OFFSET       0x240
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x240)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10143240UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_OFFSET       0x240
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x240)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10144240UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_OFFSET       0x240
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x240)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10145240UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_OFFSET       0x240
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x240)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10146240UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_OFFSET       0x240
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x240)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10147240UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10149240UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x1014a240UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x1014b240UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10151240UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10152240UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10153240UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10154240UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10155240UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_PWR_BUDGET_ENH_CAP_LIST_ADDRESS    0x10156240UL


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
} PCIE_RTR_ENH_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_RTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_RTR_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_RTR_ENH_CAP_LIST_OFFSET       0x570
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x570)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_OFFSET       0x570
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x570)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10142570UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_OFFSET       0x570
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x570)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10143570UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_OFFSET       0x570
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x570)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10144570UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_OFFSET       0x570
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x570)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10145570UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_OFFSET       0x570
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x570)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10146570UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_OFFSET       0x570
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x570)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10147570UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10149570UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x1014a570UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x1014b570UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10151570UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10152570UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10153570UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10154570UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10155570UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_RTR_ENH_CAP_LIST_ADDRESS    0x10156570UL


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
} PCIE_TLP_PREFIX_LOG0_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG0_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG0_OFFSET       0x188
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x188)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_OFFSET       0x188
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x188)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10142188UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_OFFSET       0x188
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x188)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10143188UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_OFFSET       0x188
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x188)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10144188UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_OFFSET       0x188
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x188)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10145188UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_OFFSET       0x188
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x188)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10146188UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_OFFSET       0x188
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x188)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10147188UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10149188UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x1014a188UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x1014b188UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10151188UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10152188UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10153188UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10154188UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10155188UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_TLP_PREFIX_LOG0_ADDRESS    0x10156188UL


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
} PCIE_TLP_PREFIX_LOG1_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG1_OFFSET       0x18c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x18c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_OFFSET       0x18c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x18c)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1014218cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_OFFSET       0x18c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x18c)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1014318cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_OFFSET       0x18c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x18c)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1014418cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_OFFSET       0x18c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x18c)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1014518cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_OFFSET       0x18c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x18c)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1014618cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_OFFSET       0x18c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x18c)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1014718cUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1014918cUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1014a18cUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1014b18cUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1015118cUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1015218cUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1015318cUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1015418cUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1015518cUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_TLP_PREFIX_LOG1_ADDRESS    0x1015618cUL


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
} PCIE_TLP_PREFIX_LOG2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG2_OFFSET       0x190
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x190)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_OFFSET       0x190
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x190)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10142190UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_OFFSET       0x190
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x190)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10143190UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_OFFSET       0x190
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x190)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10144190UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_OFFSET       0x190
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x190)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10145190UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_OFFSET       0x190
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x190)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10146190UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_OFFSET       0x190
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x190)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10147190UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10149190UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x1014a190UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x1014b190UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10151190UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10152190UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10153190UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10154190UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10155190UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_TLP_PREFIX_LOG2_ADDRESS    0x10156190UL


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
} PCIE_TLP_PREFIX_LOG3_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG3_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG3_OFFSET       0x194
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x194)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_OFFSET       0x194
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x194)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10142194UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_OFFSET       0x194
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x194)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10143194UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_OFFSET       0x194
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x194)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10144194UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_OFFSET       0x194
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x194)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10145194UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_OFFSET       0x194
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x194)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10146194UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_OFFSET       0x194
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x194)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10147194UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10149194UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x1014a194UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x1014b194UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10151194UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10152194UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10153194UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10154194UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10155194UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_TLP_PREFIX_LOG3_ADDRESS    0x10156194UL


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
} PCIE_UNCORR_ERR_MASK_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_MASK_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_MASK_OFFSET       0x158
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x158)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_OFFSET       0x158
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x158)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10142158UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_OFFSET       0x158
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x158)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10143158UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_OFFSET       0x158
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x158)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10144158UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_OFFSET       0x158
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x158)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10145158UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_OFFSET       0x158
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x158)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10146158UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_OFFSET       0x158
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x158)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10147158UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10149158UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x1014a158UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x1014b158UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10151158UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10152158UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10153158UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10154158UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10155158UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_UNCORR_ERR_MASK_ADDRESS    0x10156158UL


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
} PCIE_UNCORR_ERR_SEVERITY_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_OFFSET       0x15c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x15c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_OFFSET       0x15c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x15c)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1014215cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_OFFSET       0x15c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x15c)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1014315cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_OFFSET       0x15c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x15c)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1014415cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_OFFSET       0x15c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x15c)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1014515cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_OFFSET       0x15c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x15c)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1014615cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_OFFSET       0x15c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x15c)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1014715cUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1014915cUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1014a15cUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1014b15cUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1015115cUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1015215cUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1015315cUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1015415cUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1015515cUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS    0x1015615cUL


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
} PCIE_UNCORR_ERR_STATUS_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_STATUS_OFFSET       0x154
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x154)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_OFFSET       0x154
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x154)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10142154UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_OFFSET       0x154
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x154)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10143154UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_OFFSET       0x154
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x154)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10144154UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_OFFSET       0x154
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x154)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10145154UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_OFFSET       0x154
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x154)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10146154UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_OFFSET       0x154
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x154)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10147154UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10149154UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x1014a154UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x1014b154UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10151154UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10152154UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10153154UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10154154UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10155154UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_UNCORR_ERR_STATUS_ADDRESS    0x10156154UL


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
} PCIE_VENDOR_SPECIFIC1_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC1_OFFSET       0x108
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x108)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_OFFSET       0x108
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x108)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10142108UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_OFFSET       0x108
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x108)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10143108UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_OFFSET       0x108
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x108)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10144108UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_OFFSET       0x108
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x108)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10145108UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_OFFSET       0x108
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x108)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10146108UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_OFFSET       0x108
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x108)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10147108UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10149108UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x1014a108UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x1014b108UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10151108UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10152108UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10153108UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10154108UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10155108UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_VENDOR_SPECIFIC1_ADDRESS    0x10156108UL


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
} PCIE_VENDOR_SPECIFIC2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC2_OFFSET       0x10c
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x10c)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_OFFSET       0x10c
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x10c)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1014210cUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_OFFSET       0x10c
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x10c)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1014310cUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_OFFSET       0x10c
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x10c)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1014410cUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_OFFSET       0x10c
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x10c)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1014510cUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_OFFSET       0x10c
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x10c)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1014610cUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_OFFSET       0x10c
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x10c)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1014710cUL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1014910cUL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1014a10cUL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1014b10cUL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1015110cUL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1015210cUL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1015310cUL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1015410cUL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1015510cUL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_VENDOR_SPECIFIC2_ADDRESS    0x1015610cUL


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
} PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_OFFSET       0x100
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x100)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_OFFSET       0x100
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x100)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10142100UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_OFFSET       0x100
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x100)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10143100UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_OFFSET       0x100
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x100)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10144100UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_OFFSET       0x100
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x100)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10145100UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_OFFSET       0x100
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x100)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10146100UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_OFFSET       0x100
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x100)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10147100UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10149100UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x1014a100UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x1014b100UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10151100UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10152100UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10153100UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10154100UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10155100UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_VENDOR_SPECIFIC_ENH_CAP_LIST_ADDRESS    0x10156100UL


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
} PCIE_VENDOR_SPECIFIC_HDR_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_OFFSET       0x104
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x104)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_OFFSET       0x104
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x104)

#define SMN_DEV0_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10142104UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_OFFSET       0x104
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x104)

#define SMN_DEV0_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10143104UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_OFFSET       0x104
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x104)

#define SMN_DEV0_FUNC4_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10144104UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_OFFSET       0x104
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x104)

#define SMN_DEV0_FUNC5_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10145104UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_OFFSET       0x104
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x104)

#define SMN_DEV0_FUNC6_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10146104UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_OFFSET       0x104
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x104)

#define SMN_DEV0_FUNC7_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10147104UL

#define SMN_DEV1_FUNC1_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10149104UL

#define SMN_DEV1_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x1014a104UL

#define SMN_DEV1_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x1014b104UL

#define SMN_DEV2_FUNC1_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10151104UL

#define SMN_DEV2_FUNC2_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10152104UL

#define SMN_DEV2_FUNC3_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10153104UL

#define SMN_DEV2_FUNC4_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10154104UL

#define SMN_DEV2_FUNC5_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10155104UL

#define SMN_DEV2_FUNC6_NBIF0_PCIE_VENDOR_SPECIFIC_HDR_ADDRESS    0x10156104UL


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
} PMI_CAP_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_CAP_OFFSET       0x52
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x52)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_CAP_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_CAP_OFFSET       0x52
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x52)

#define SMN_DEV0_FUNC2_NBIF0_PMI_CAP_ADDRESS    0x10142052UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_CAP_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_CAP_OFFSET       0x52
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x52)

#define SMN_DEV0_FUNC3_NBIF0_PMI_CAP_ADDRESS    0x10143052UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_CAP_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_CAP_OFFSET       0x52
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x52)

#define SMN_DEV0_FUNC4_NBIF0_PMI_CAP_ADDRESS    0x10144052UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_CAP_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_CAP_OFFSET       0x52
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x52)

#define SMN_DEV0_FUNC5_NBIF0_PMI_CAP_ADDRESS    0x10145052UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_CAP_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_CAP_OFFSET       0x52
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x52)

#define SMN_DEV0_FUNC6_NBIF0_PMI_CAP_ADDRESS    0x10146052UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_CAP_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_CAP_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_CAP_OFFSET       0x52
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_CAP_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x52)

#define SMN_DEV0_FUNC7_NBIF0_PMI_CAP_ADDRESS    0x10147052UL

#define SMN_DEV1_FUNC1_NBIF0_PMI_CAP_ADDRESS    0x10149052UL

#define SMN_DEV1_FUNC2_NBIF0_PMI_CAP_ADDRESS    0x1014a052UL

#define SMN_DEV1_FUNC3_NBIF0_PMI_CAP_ADDRESS    0x1014b052UL

#define SMN_DEV2_FUNC1_NBIF0_PMI_CAP_ADDRESS    0x10151052UL

#define SMN_DEV2_FUNC2_NBIF0_PMI_CAP_ADDRESS    0x10152052UL

#define SMN_DEV2_FUNC3_NBIF0_PMI_CAP_ADDRESS    0x10153052UL

#define SMN_DEV2_FUNC4_NBIF0_PMI_CAP_ADDRESS    0x10154052UL

#define SMN_DEV2_FUNC5_NBIF0_PMI_CAP_ADDRESS    0x10155052UL

#define SMN_DEV2_FUNC6_NBIF0_PMI_CAP_ADDRESS    0x10156052UL


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
} PMI_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_CAP_LIST_OFFSET       0x50
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x50)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_CAP_LIST_OFFSET       0x50
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x50)

#define SMN_DEV0_FUNC2_NBIF0_PMI_CAP_LIST_ADDRESS    0x10142050UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_CAP_LIST_OFFSET       0x50
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x50)

#define SMN_DEV0_FUNC3_NBIF0_PMI_CAP_LIST_ADDRESS    0x10143050UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_CAP_LIST_OFFSET       0x50
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x50)

#define SMN_DEV0_FUNC4_NBIF0_PMI_CAP_LIST_ADDRESS    0x10144050UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_CAP_LIST_OFFSET       0x50
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x50)

#define SMN_DEV0_FUNC5_NBIF0_PMI_CAP_LIST_ADDRESS    0x10145050UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_CAP_LIST_OFFSET       0x50
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x50)

#define SMN_DEV0_FUNC6_NBIF0_PMI_CAP_LIST_ADDRESS    0x10146050UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_CAP_LIST_OFFSET       0x50
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x50)

#define SMN_DEV0_FUNC7_NBIF0_PMI_CAP_LIST_ADDRESS    0x10147050UL

#define SMN_DEV1_FUNC1_NBIF0_PMI_CAP_LIST_ADDRESS    0x10149050UL

#define SMN_DEV1_FUNC2_NBIF0_PMI_CAP_LIST_ADDRESS    0x1014a050UL

#define SMN_DEV1_FUNC3_NBIF0_PMI_CAP_LIST_ADDRESS    0x1014b050UL

#define SMN_DEV2_FUNC1_NBIF0_PMI_CAP_LIST_ADDRESS    0x10151050UL

#define SMN_DEV2_FUNC2_NBIF0_PMI_CAP_LIST_ADDRESS    0x10152050UL

#define SMN_DEV2_FUNC3_NBIF0_PMI_CAP_LIST_ADDRESS    0x10153050UL

#define SMN_DEV2_FUNC4_NBIF0_PMI_CAP_LIST_ADDRESS    0x10154050UL

#define SMN_DEV2_FUNC5_NBIF0_PMI_CAP_LIST_ADDRESS    0x10155050UL

#define SMN_DEV2_FUNC6_NBIF0_PMI_CAP_LIST_ADDRESS    0x10156050UL


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
} PMI_STATUS_CNTL_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_STATUS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_STATUS_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_STATUS_CNTL_OFFSET       0x54
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PMI_STATUS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x54)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_STATUS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_STATUS_CNTL_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_STATUS_CNTL_OFFSET       0x54
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PMI_STATUS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x54)

#define SMN_DEV0_FUNC2_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10142054UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_STATUS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_STATUS_CNTL_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_STATUS_CNTL_OFFSET       0x54
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PMI_STATUS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x54)

#define SMN_DEV0_FUNC3_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10143054UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_STATUS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_STATUS_CNTL_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_STATUS_CNTL_OFFSET       0x54
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PMI_STATUS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x54)

#define SMN_DEV0_FUNC4_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10144054UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_STATUS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_STATUS_CNTL_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_STATUS_CNTL_OFFSET       0x54
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PMI_STATUS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x54)

#define SMN_DEV0_FUNC5_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10145054UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_STATUS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_STATUS_CNTL_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_STATUS_CNTL_OFFSET       0x54
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PMI_STATUS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x54)

#define SMN_DEV0_FUNC6_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10146054UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_STATUS_CNTL_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_STATUS_CNTL_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_STATUS_CNTL_OFFSET       0x54
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PMI_STATUS_CNTL_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x54)

#define SMN_DEV0_FUNC7_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10147054UL

#define SMN_DEV1_FUNC1_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10149054UL

#define SMN_DEV1_FUNC2_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x1014a054UL

#define SMN_DEV1_FUNC3_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x1014b054UL

#define SMN_DEV2_FUNC1_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10151054UL

#define SMN_DEV2_FUNC2_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10152054UL

#define SMN_DEV2_FUNC3_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10153054UL

#define SMN_DEV2_FUNC4_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10154054UL

#define SMN_DEV2_FUNC5_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10155054UL

#define SMN_DEV2_FUNC6_NBIF0_PMI_STATUS_CNTL_ADDRESS    0x10156054UL


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
} PROG_INTERFACE_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PROG_INTERFACE_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PROG_INTERFACE_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PROG_INTERFACE_OFFSET       0x9
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_PROG_INTERFACE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x9)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PROG_INTERFACE_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PROG_INTERFACE_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PROG_INTERFACE_OFFSET       0x9
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_PROG_INTERFACE_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x9)

#define SMN_DEV0_FUNC2_NBIF0_PROG_INTERFACE_ADDRESS    0x10142009UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PROG_INTERFACE_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PROG_INTERFACE_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PROG_INTERFACE_OFFSET       0x9
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_PROG_INTERFACE_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x9)

#define SMN_DEV0_FUNC3_NBIF0_PROG_INTERFACE_ADDRESS    0x10143009UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PROG_INTERFACE_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PROG_INTERFACE_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PROG_INTERFACE_OFFSET       0x9
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_PROG_INTERFACE_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x9)

#define SMN_DEV0_FUNC4_NBIF0_PROG_INTERFACE_ADDRESS    0x10144009UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PROG_INTERFACE_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PROG_INTERFACE_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PROG_INTERFACE_OFFSET       0x9
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_PROG_INTERFACE_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x9)

#define SMN_DEV0_FUNC5_NBIF0_PROG_INTERFACE_ADDRESS    0x10145009UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PROG_INTERFACE_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PROG_INTERFACE_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PROG_INTERFACE_OFFSET       0x9
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_PROG_INTERFACE_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x9)

#define SMN_DEV0_FUNC6_NBIF0_PROG_INTERFACE_ADDRESS    0x10146009UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PROG_INTERFACE_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PROG_INTERFACE_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PROG_INTERFACE_OFFSET       0x9
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_PROG_INTERFACE_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x9)

#define SMN_DEV0_FUNC7_NBIF0_PROG_INTERFACE_ADDRESS    0x10147009UL

#define SMN_DEV1_FUNC1_NBIF0_PROG_INTERFACE_ADDRESS    0x10149009UL

#define SMN_DEV1_FUNC2_NBIF0_PROG_INTERFACE_ADDRESS    0x1014a009UL

#define SMN_DEV1_FUNC3_NBIF0_PROG_INTERFACE_ADDRESS    0x1014b009UL

#define SMN_DEV2_FUNC1_NBIF0_PROG_INTERFACE_ADDRESS    0x10151009UL

#define SMN_DEV2_FUNC2_NBIF0_PROG_INTERFACE_ADDRESS    0x10152009UL

#define SMN_DEV2_FUNC3_NBIF0_PROG_INTERFACE_ADDRESS    0x10153009UL

#define SMN_DEV2_FUNC4_NBIF0_PROG_INTERFACE_ADDRESS    0x10154009UL

#define SMN_DEV2_FUNC5_NBIF0_PROG_INTERFACE_ADDRESS    0x10155009UL

#define SMN_DEV2_FUNC6_NBIF0_PROG_INTERFACE_ADDRESS    0x10156009UL


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
} REVISION_ID_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_REVISION_ID_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x8)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_REVISION_ID_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x8)

#define SMN_DEV0_FUNC2_NBIF0_REVISION_ID_ADDRESS    0x10142008UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_REVISION_ID_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x8)

#define SMN_DEV0_FUNC3_NBIF0_REVISION_ID_ADDRESS    0x10143008UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_REVISION_ID_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x8)

#define SMN_DEV0_FUNC4_NBIF0_REVISION_ID_ADDRESS    0x10144008UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_REVISION_ID_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x8)

#define SMN_DEV0_FUNC5_NBIF0_REVISION_ID_ADDRESS    0x10145008UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_REVISION_ID_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x8)

#define SMN_DEV0_FUNC6_NBIF0_REVISION_ID_ADDRESS    0x10146008UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_REVISION_ID_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_REVISION_ID_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_REVISION_ID_OFFSET       0x8
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_REVISION_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x8)

#define SMN_DEV0_FUNC7_NBIF0_REVISION_ID_ADDRESS    0x10147008UL

#define SMN_DEV1_FUNC1_NBIF0_REVISION_ID_ADDRESS    0x10149008UL

#define SMN_DEV1_FUNC2_NBIF0_REVISION_ID_ADDRESS    0x1014a008UL

#define SMN_DEV1_FUNC3_NBIF0_REVISION_ID_ADDRESS    0x1014b008UL

#define SMN_DEV2_FUNC1_NBIF0_REVISION_ID_ADDRESS    0x10151008UL

#define SMN_DEV2_FUNC2_NBIF0_REVISION_ID_ADDRESS    0x10152008UL

#define SMN_DEV2_FUNC3_NBIF0_REVISION_ID_ADDRESS    0x10153008UL

#define SMN_DEV2_FUNC4_NBIF0_REVISION_ID_ADDRESS    0x10154008UL

#define SMN_DEV2_FUNC5_NBIF0_REVISION_ID_ADDRESS    0x10155008UL

#define SMN_DEV2_FUNC6_NBIF0_REVISION_ID_ADDRESS    0x10156008UL


/***********************************************************
* Register Name : ROM_BASE_ADDR
* Register Description :
* PCI CFG Expansion ROM BAR.
* Visibility : 0x2
************************************************************/


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
} RTR_DATA1_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_RTR_DATA1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_RTR_DATA1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_RTR_DATA1_OFFSET       0x574
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_RTR_DATA1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x574)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_RTR_DATA1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_RTR_DATA1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_RTR_DATA1_OFFSET       0x574
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_RTR_DATA1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x574)

#define SMN_DEV0_FUNC2_NBIF0_RTR_DATA1_ADDRESS    0x10142574UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_RTR_DATA1_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_RTR_DATA1_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_RTR_DATA1_OFFSET       0x574
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_RTR_DATA1_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x574)

#define SMN_DEV0_FUNC3_NBIF0_RTR_DATA1_ADDRESS    0x10143574UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_RTR_DATA1_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_RTR_DATA1_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_RTR_DATA1_OFFSET       0x574
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_RTR_DATA1_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x574)

#define SMN_DEV0_FUNC4_NBIF0_RTR_DATA1_ADDRESS    0x10144574UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_RTR_DATA1_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_RTR_DATA1_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_RTR_DATA1_OFFSET       0x574
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_RTR_DATA1_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x574)

#define SMN_DEV0_FUNC5_NBIF0_RTR_DATA1_ADDRESS    0x10145574UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_RTR_DATA1_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_RTR_DATA1_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_RTR_DATA1_OFFSET       0x574
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_RTR_DATA1_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x574)

#define SMN_DEV0_FUNC6_NBIF0_RTR_DATA1_ADDRESS    0x10146574UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_RTR_DATA1_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_RTR_DATA1_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_RTR_DATA1_OFFSET       0x574
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_RTR_DATA1_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x574)

#define SMN_DEV0_FUNC7_NBIF0_RTR_DATA1_ADDRESS    0x10147574UL

#define SMN_DEV1_FUNC1_NBIF0_RTR_DATA1_ADDRESS    0x10149574UL

#define SMN_DEV1_FUNC2_NBIF0_RTR_DATA1_ADDRESS    0x1014a574UL

#define SMN_DEV1_FUNC3_NBIF0_RTR_DATA1_ADDRESS    0x1014b574UL

#define SMN_DEV2_FUNC1_NBIF0_RTR_DATA1_ADDRESS    0x10151574UL

#define SMN_DEV2_FUNC2_NBIF0_RTR_DATA1_ADDRESS    0x10152574UL

#define SMN_DEV2_FUNC3_NBIF0_RTR_DATA1_ADDRESS    0x10153574UL

#define SMN_DEV2_FUNC4_NBIF0_RTR_DATA1_ADDRESS    0x10154574UL

#define SMN_DEV2_FUNC5_NBIF0_RTR_DATA1_ADDRESS    0x10155574UL

#define SMN_DEV2_FUNC6_NBIF0_RTR_DATA1_ADDRESS    0x10156574UL


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
} RTR_DATA2_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_RTR_DATA2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_RTR_DATA2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_RTR_DATA2_OFFSET       0x578
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_RTR_DATA2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x578)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_RTR_DATA2_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_RTR_DATA2_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_RTR_DATA2_OFFSET       0x578
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_RTR_DATA2_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x578)

#define SMN_DEV0_FUNC2_NBIF0_RTR_DATA2_ADDRESS    0x10142578UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_RTR_DATA2_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_RTR_DATA2_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_RTR_DATA2_OFFSET       0x578
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_RTR_DATA2_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x578)

#define SMN_DEV0_FUNC3_NBIF0_RTR_DATA2_ADDRESS    0x10143578UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_RTR_DATA2_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_RTR_DATA2_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_RTR_DATA2_OFFSET       0x578
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_RTR_DATA2_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x578)

#define SMN_DEV0_FUNC4_NBIF0_RTR_DATA2_ADDRESS    0x10144578UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_RTR_DATA2_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_RTR_DATA2_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_RTR_DATA2_OFFSET       0x578
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_RTR_DATA2_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x578)

#define SMN_DEV0_FUNC5_NBIF0_RTR_DATA2_ADDRESS    0x10145578UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_RTR_DATA2_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_RTR_DATA2_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_RTR_DATA2_OFFSET       0x578
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_RTR_DATA2_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x578)

#define SMN_DEV0_FUNC6_NBIF0_RTR_DATA2_ADDRESS    0x10146578UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_RTR_DATA2_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_RTR_DATA2_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_RTR_DATA2_OFFSET       0x578
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_RTR_DATA2_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x578)

#define SMN_DEV0_FUNC7_NBIF0_RTR_DATA2_ADDRESS    0x10147578UL

#define SMN_DEV1_FUNC1_NBIF0_RTR_DATA2_ADDRESS    0x10149578UL

#define SMN_DEV1_FUNC2_NBIF0_RTR_DATA2_ADDRESS    0x1014a578UL

#define SMN_DEV1_FUNC3_NBIF0_RTR_DATA2_ADDRESS    0x1014b578UL

#define SMN_DEV2_FUNC1_NBIF0_RTR_DATA2_ADDRESS    0x10151578UL

#define SMN_DEV2_FUNC2_NBIF0_RTR_DATA2_ADDRESS    0x10152578UL

#define SMN_DEV2_FUNC3_NBIF0_RTR_DATA2_ADDRESS    0x10153578UL

#define SMN_DEV2_FUNC4_NBIF0_RTR_DATA2_ADDRESS    0x10154578UL

#define SMN_DEV2_FUNC5_NBIF0_RTR_DATA2_ADDRESS    0x10155578UL

#define SMN_DEV2_FUNC6_NBIF0_RTR_DATA2_ADDRESS    0x10156578UL


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
} SATA_CAP_0_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_CAP_0_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_CAP_0_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_CAP_0_OFFSET       0xd0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_CAP_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd0)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_CAP_0_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_CAP_0_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_CAP_0_OFFSET       0xd0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_CAP_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd0)

#define SMN_DEV0_FUNC2_NBIF0_SATA_CAP_0_ADDRESS    0x101420d0UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_CAP_0_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_CAP_0_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_CAP_0_OFFSET       0xd0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_CAP_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xd0)

#define SMN_DEV0_FUNC3_NBIF0_SATA_CAP_0_ADDRESS    0x101430d0UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_CAP_0_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_CAP_0_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_CAP_0_OFFSET       0xd0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_CAP_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xd0)

#define SMN_DEV0_FUNC4_NBIF0_SATA_CAP_0_ADDRESS    0x101440d0UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_CAP_0_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_CAP_0_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_CAP_0_OFFSET       0xd0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_CAP_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xd0)

#define SMN_DEV0_FUNC5_NBIF0_SATA_CAP_0_ADDRESS    0x101450d0UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_CAP_0_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_CAP_0_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_CAP_0_OFFSET       0xd0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_CAP_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xd0)

#define SMN_DEV0_FUNC6_NBIF0_SATA_CAP_0_ADDRESS    0x101460d0UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_CAP_0_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_CAP_0_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_CAP_0_OFFSET       0xd0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_CAP_0_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xd0)

#define SMN_DEV0_FUNC7_NBIF0_SATA_CAP_0_ADDRESS    0x101470d0UL

#define SMN_DEV1_FUNC1_NBIF0_SATA_CAP_0_ADDRESS    0x101490d0UL

#define SMN_DEV1_FUNC2_NBIF0_SATA_CAP_0_ADDRESS    0x1014a0d0UL

#define SMN_DEV1_FUNC3_NBIF0_SATA_CAP_0_ADDRESS    0x1014b0d0UL

#define SMN_DEV2_FUNC1_NBIF0_SATA_CAP_0_ADDRESS    0x101510d0UL

#define SMN_DEV2_FUNC2_NBIF0_SATA_CAP_0_ADDRESS    0x101520d0UL

#define SMN_DEV2_FUNC3_NBIF0_SATA_CAP_0_ADDRESS    0x101530d0UL

#define SMN_DEV2_FUNC4_NBIF0_SATA_CAP_0_ADDRESS    0x101540d0UL

#define SMN_DEV2_FUNC5_NBIF0_SATA_CAP_0_ADDRESS    0x101550d0UL

#define SMN_DEV2_FUNC6_NBIF0_SATA_CAP_0_ADDRESS    0x101560d0UL


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
} SATA_CAP_1_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_CAP_1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_CAP_1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_CAP_1_OFFSET       0xd4
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_CAP_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd4)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_CAP_1_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_CAP_1_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_CAP_1_OFFSET       0xd4
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_CAP_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd4)

#define SMN_DEV0_FUNC2_NBIF0_SATA_CAP_1_ADDRESS    0x101420d4UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_CAP_1_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_CAP_1_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_CAP_1_OFFSET       0xd4
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_CAP_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xd4)

#define SMN_DEV0_FUNC3_NBIF0_SATA_CAP_1_ADDRESS    0x101430d4UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_CAP_1_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_CAP_1_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_CAP_1_OFFSET       0xd4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_CAP_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xd4)

#define SMN_DEV0_FUNC4_NBIF0_SATA_CAP_1_ADDRESS    0x101440d4UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_CAP_1_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_CAP_1_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_CAP_1_OFFSET       0xd4
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_CAP_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xd4)

#define SMN_DEV0_FUNC5_NBIF0_SATA_CAP_1_ADDRESS    0x101450d4UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_CAP_1_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_CAP_1_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_CAP_1_OFFSET       0xd4
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_CAP_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xd4)

#define SMN_DEV0_FUNC6_NBIF0_SATA_CAP_1_ADDRESS    0x101460d4UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_CAP_1_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_CAP_1_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_CAP_1_OFFSET       0xd4
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_CAP_1_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xd4)

#define SMN_DEV0_FUNC7_NBIF0_SATA_CAP_1_ADDRESS    0x101470d4UL

#define SMN_DEV1_FUNC1_NBIF0_SATA_CAP_1_ADDRESS    0x101490d4UL

#define SMN_DEV1_FUNC2_NBIF0_SATA_CAP_1_ADDRESS    0x1014a0d4UL

#define SMN_DEV1_FUNC3_NBIF0_SATA_CAP_1_ADDRESS    0x1014b0d4UL

#define SMN_DEV2_FUNC1_NBIF0_SATA_CAP_1_ADDRESS    0x101510d4UL

#define SMN_DEV2_FUNC2_NBIF0_SATA_CAP_1_ADDRESS    0x101520d4UL

#define SMN_DEV2_FUNC3_NBIF0_SATA_CAP_1_ADDRESS    0x101530d4UL

#define SMN_DEV2_FUNC4_NBIF0_SATA_CAP_1_ADDRESS    0x101540d4UL

#define SMN_DEV2_FUNC5_NBIF0_SATA_CAP_1_ADDRESS    0x101550d4UL

#define SMN_DEV2_FUNC6_NBIF0_SATA_CAP_1_ADDRESS    0x101560d4UL


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
} SATA_IDP_DATA_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_IDP_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_IDP_DATA_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_IDP_DATA_OFFSET       0xdc
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_IDP_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xdc)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_IDP_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_IDP_DATA_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_IDP_DATA_OFFSET       0xdc
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_IDP_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xdc)

#define SMN_DEV0_FUNC2_NBIF0_SATA_IDP_DATA_ADDRESS    0x101420dcUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_IDP_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_IDP_DATA_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_IDP_DATA_OFFSET       0xdc
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_IDP_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xdc)

#define SMN_DEV0_FUNC3_NBIF0_SATA_IDP_DATA_ADDRESS    0x101430dcUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_IDP_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_IDP_DATA_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_IDP_DATA_OFFSET       0xdc
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_IDP_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xdc)

#define SMN_DEV0_FUNC4_NBIF0_SATA_IDP_DATA_ADDRESS    0x101440dcUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_IDP_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_IDP_DATA_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_IDP_DATA_OFFSET       0xdc
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_IDP_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xdc)

#define SMN_DEV0_FUNC5_NBIF0_SATA_IDP_DATA_ADDRESS    0x101450dcUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_IDP_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_IDP_DATA_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_IDP_DATA_OFFSET       0xdc
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_IDP_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xdc)

#define SMN_DEV0_FUNC6_NBIF0_SATA_IDP_DATA_ADDRESS    0x101460dcUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_IDP_DATA_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_IDP_DATA_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_IDP_DATA_OFFSET       0xdc
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_IDP_DATA_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xdc)

#define SMN_DEV0_FUNC7_NBIF0_SATA_IDP_DATA_ADDRESS    0x101470dcUL

#define SMN_DEV1_FUNC1_NBIF0_SATA_IDP_DATA_ADDRESS    0x101490dcUL

#define SMN_DEV1_FUNC2_NBIF0_SATA_IDP_DATA_ADDRESS    0x1014a0dcUL

#define SMN_DEV1_FUNC3_NBIF0_SATA_IDP_DATA_ADDRESS    0x1014b0dcUL

#define SMN_DEV2_FUNC1_NBIF0_SATA_IDP_DATA_ADDRESS    0x101510dcUL

#define SMN_DEV2_FUNC2_NBIF0_SATA_IDP_DATA_ADDRESS    0x101520dcUL

#define SMN_DEV2_FUNC3_NBIF0_SATA_IDP_DATA_ADDRESS    0x101530dcUL

#define SMN_DEV2_FUNC4_NBIF0_SATA_IDP_DATA_ADDRESS    0x101540dcUL

#define SMN_DEV2_FUNC5_NBIF0_SATA_IDP_DATA_ADDRESS    0x101550dcUL

#define SMN_DEV2_FUNC6_NBIF0_SATA_IDP_DATA_ADDRESS    0x101560dcUL


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
} SATA_IDP_INDEX_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_IDP_INDEX_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_IDP_INDEX_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_IDP_INDEX_OFFSET       0xd8
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SATA_IDP_INDEX_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd8)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_IDP_INDEX_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_IDP_INDEX_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_IDP_INDEX_OFFSET       0xd8
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SATA_IDP_INDEX_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xd8)

#define SMN_DEV0_FUNC2_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101420d8UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_IDP_INDEX_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_IDP_INDEX_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_IDP_INDEX_OFFSET       0xd8
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SATA_IDP_INDEX_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xd8)

#define SMN_DEV0_FUNC3_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101430d8UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_IDP_INDEX_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_IDP_INDEX_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_IDP_INDEX_OFFSET       0xd8
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SATA_IDP_INDEX_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xd8)

#define SMN_DEV0_FUNC4_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101440d8UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_IDP_INDEX_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_IDP_INDEX_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_IDP_INDEX_OFFSET       0xd8
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SATA_IDP_INDEX_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xd8)

#define SMN_DEV0_FUNC5_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101450d8UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_IDP_INDEX_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_IDP_INDEX_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_IDP_INDEX_OFFSET       0xd8
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SATA_IDP_INDEX_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xd8)

#define SMN_DEV0_FUNC6_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101460d8UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_IDP_INDEX_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_IDP_INDEX_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_IDP_INDEX_OFFSET       0xd8
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SATA_IDP_INDEX_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xd8)

#define SMN_DEV0_FUNC7_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101470d8UL

#define SMN_DEV1_FUNC1_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101490d8UL

#define SMN_DEV1_FUNC2_NBIF0_SATA_IDP_INDEX_ADDRESS    0x1014a0d8UL

#define SMN_DEV1_FUNC3_NBIF0_SATA_IDP_INDEX_ADDRESS    0x1014b0d8UL

#define SMN_DEV2_FUNC1_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101510d8UL

#define SMN_DEV2_FUNC2_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101520d8UL

#define SMN_DEV2_FUNC3_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101530d8UL

#define SMN_DEV2_FUNC4_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101540d8UL

#define SMN_DEV2_FUNC5_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101550d8UL

#define SMN_DEV2_FUNC6_NBIF0_SATA_IDP_INDEX_ADDRESS    0x101560d8UL


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
} SBRN_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SBRN_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SBRN_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SBRN_OFFSET       0x60
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SBRN_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x60)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SBRN_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SBRN_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SBRN_OFFSET       0x60
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SBRN_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x60)

#define SMN_DEV0_FUNC2_NBIF0_SBRN_ADDRESS    0x10142060UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SBRN_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SBRN_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SBRN_OFFSET       0x60
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SBRN_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x60)

#define SMN_DEV0_FUNC3_NBIF0_SBRN_ADDRESS    0x10143060UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SBRN_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SBRN_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SBRN_OFFSET       0x60
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SBRN_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x60)

#define SMN_DEV0_FUNC4_NBIF0_SBRN_ADDRESS    0x10144060UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SBRN_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SBRN_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SBRN_OFFSET       0x60
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SBRN_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x60)

#define SMN_DEV0_FUNC5_NBIF0_SBRN_ADDRESS    0x10145060UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SBRN_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SBRN_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SBRN_OFFSET       0x60
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SBRN_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x60)

#define SMN_DEV0_FUNC6_NBIF0_SBRN_ADDRESS    0x10146060UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SBRN_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SBRN_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SBRN_OFFSET       0x60
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SBRN_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x60)

#define SMN_DEV0_FUNC7_NBIF0_SBRN_ADDRESS    0x10147060UL

#define SMN_DEV1_FUNC1_NBIF0_SBRN_ADDRESS    0x10149060UL

#define SMN_DEV1_FUNC2_NBIF0_SBRN_ADDRESS    0x1014a060UL

#define SMN_DEV1_FUNC3_NBIF0_SBRN_ADDRESS    0x1014b060UL

#define SMN_DEV2_FUNC1_NBIF0_SBRN_ADDRESS    0x10151060UL

#define SMN_DEV2_FUNC2_NBIF0_SBRN_ADDRESS    0x10152060UL

#define SMN_DEV2_FUNC3_NBIF0_SBRN_ADDRESS    0x10153060UL

#define SMN_DEV2_FUNC4_NBIF0_SBRN_ADDRESS    0x10154060UL

#define SMN_DEV2_FUNC5_NBIF0_SBRN_ADDRESS    0x10155060UL

#define SMN_DEV2_FUNC6_NBIF0_SBRN_ADDRESS    0x10156060UL


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
} STATUS_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_STATUS_OFFSET       0x6
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x6)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_STATUS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_STATUS_OFFSET       0x6
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x6)

#define SMN_DEV0_FUNC2_NBIF0_STATUS_ADDRESS    0x10142006UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_STATUS_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_STATUS_OFFSET       0x6
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x6)

#define SMN_DEV0_FUNC3_NBIF0_STATUS_ADDRESS    0x10143006UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_STATUS_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_STATUS_OFFSET       0x6
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x6)

#define SMN_DEV0_FUNC4_NBIF0_STATUS_ADDRESS    0x10144006UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_STATUS_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_STATUS_OFFSET       0x6
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x6)

#define SMN_DEV0_FUNC5_NBIF0_STATUS_ADDRESS    0x10145006UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_STATUS_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_STATUS_OFFSET       0x6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x6)

#define SMN_DEV0_FUNC6_NBIF0_STATUS_ADDRESS    0x10146006UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_STATUS_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_STATUS_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_STATUS_OFFSET       0x6
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_STATUS_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x6)

#define SMN_DEV0_FUNC7_NBIF0_STATUS_ADDRESS    0x10147006UL

#define SMN_DEV1_FUNC1_NBIF0_STATUS_ADDRESS    0x10149006UL

#define SMN_DEV1_FUNC2_NBIF0_STATUS_ADDRESS    0x1014a006UL

#define SMN_DEV1_FUNC3_NBIF0_STATUS_ADDRESS    0x1014b006UL

#define SMN_DEV2_FUNC1_NBIF0_STATUS_ADDRESS    0x10151006UL

#define SMN_DEV2_FUNC2_NBIF0_STATUS_ADDRESS    0x10152006UL

#define SMN_DEV2_FUNC3_NBIF0_STATUS_ADDRESS    0x10153006UL

#define SMN_DEV2_FUNC4_NBIF0_STATUS_ADDRESS    0x10154006UL

#define SMN_DEV2_FUNC5_NBIF0_STATUS_ADDRESS    0x10155006UL

#define SMN_DEV2_FUNC6_NBIF0_STATUS_ADDRESS    0x10156006UL


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
} SUB_CLASS_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SUB_CLASS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xa)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SUB_CLASS_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0xa)

#define SMN_DEV0_FUNC2_NBIF0_SUB_CLASS_ADDRESS    0x1014200aUL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SUB_CLASS_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0xa)

#define SMN_DEV0_FUNC3_NBIF0_SUB_CLASS_ADDRESS    0x1014300aUL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SUB_CLASS_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0xa)

#define SMN_DEV0_FUNC4_NBIF0_SUB_CLASS_ADDRESS    0x1014400aUL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SUB_CLASS_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0xa)

#define SMN_DEV0_FUNC5_NBIF0_SUB_CLASS_ADDRESS    0x1014500aUL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SUB_CLASS_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0xa)

#define SMN_DEV0_FUNC6_NBIF0_SUB_CLASS_ADDRESS    0x1014600aUL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SUB_CLASS_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SUB_CLASS_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SUB_CLASS_OFFSET       0xa
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_SUB_CLASS_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0xa)

#define SMN_DEV0_FUNC7_NBIF0_SUB_CLASS_ADDRESS    0x1014700aUL

#define SMN_DEV1_FUNC1_NBIF0_SUB_CLASS_ADDRESS    0x1014900aUL

#define SMN_DEV1_FUNC2_NBIF0_SUB_CLASS_ADDRESS    0x1014a00aUL

#define SMN_DEV1_FUNC3_NBIF0_SUB_CLASS_ADDRESS    0x1014b00aUL

#define SMN_DEV2_FUNC1_NBIF0_SUB_CLASS_ADDRESS    0x1015100aUL

#define SMN_DEV2_FUNC2_NBIF0_SUB_CLASS_ADDRESS    0x1015200aUL

#define SMN_DEV2_FUNC3_NBIF0_SUB_CLASS_ADDRESS    0x1015300aUL

#define SMN_DEV2_FUNC4_NBIF0_SUB_CLASS_ADDRESS    0x1015400aUL

#define SMN_DEV2_FUNC5_NBIF0_SUB_CLASS_ADDRESS    0x1015500aUL

#define SMN_DEV2_FUNC6_NBIF0_SUB_CLASS_ADDRESS    0x1015600aUL


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
} VENDOR_CAP_LIST_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_VENDOR_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_VENDOR_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_VENDOR_CAP_LIST_OFFSET       0x48
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_VENDOR_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x48)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_VENDOR_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_VENDOR_CAP_LIST_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_VENDOR_CAP_LIST_OFFSET       0x48
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_VENDOR_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x48)

#define SMN_DEV0_FUNC2_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10142048UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_VENDOR_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_VENDOR_CAP_LIST_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_VENDOR_CAP_LIST_OFFSET       0x48
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_VENDOR_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x48)

#define SMN_DEV0_FUNC3_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10143048UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_VENDOR_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_VENDOR_CAP_LIST_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_VENDOR_CAP_LIST_OFFSET       0x48
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_VENDOR_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x48)

#define SMN_DEV0_FUNC4_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10144048UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_VENDOR_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_VENDOR_CAP_LIST_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_VENDOR_CAP_LIST_OFFSET       0x48
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_VENDOR_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x48)

#define SMN_DEV0_FUNC5_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10145048UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_VENDOR_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_VENDOR_CAP_LIST_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_VENDOR_CAP_LIST_OFFSET       0x48
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_VENDOR_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x48)

#define SMN_DEV0_FUNC6_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10146048UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_VENDOR_CAP_LIST_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_VENDOR_CAP_LIST_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_VENDOR_CAP_LIST_OFFSET       0x48
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_VENDOR_CAP_LIST_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x48)

#define SMN_DEV0_FUNC7_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10147048UL

#define SMN_DEV1_FUNC1_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10149048UL

#define SMN_DEV1_FUNC2_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x1014a048UL

#define SMN_DEV1_FUNC3_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x1014b048UL

#define SMN_DEV2_FUNC1_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10151048UL

#define SMN_DEV2_FUNC2_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10152048UL

#define SMN_DEV2_FUNC3_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10153048UL

#define SMN_DEV2_FUNC4_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10154048UL

#define SMN_DEV2_FUNC5_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10155048UL

#define SMN_DEV2_FUNC6_NBIF0_VENDOR_CAP_LIST_ADDRESS    0x10156048UL


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
} VENDOR_ID_NBIFEPFNCFG_STRUCT;

#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_VENDOR_ID_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_FUNC2_DEV2_NBIF0_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_VENDOR_ID_FUNC         2
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_FUNC2_DEV0_NBIF0_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 2, 0x0)

#define SMN_DEV0_FUNC2_NBIF0_VENDOR_ID_ADDRESS    0x10142000UL

#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_VENDOR_ID_FUNC         3
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_FUNC3_DEV0_NBIF0_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 3, 0x0)

#define SMN_DEV0_FUNC3_NBIF0_VENDOR_ID_ADDRESS    0x10143000UL

#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_VENDOR_ID_FUNC         4
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_FUNC4_DEV0_NBIF0_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 4, 0x0)

#define SMN_DEV0_FUNC4_NBIF0_VENDOR_ID_ADDRESS    0x10144000UL

#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_VENDOR_ID_FUNC         5
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_FUNC5_DEV0_NBIF0_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 5, 0x0)

#define SMN_DEV0_FUNC5_NBIF0_VENDOR_ID_ADDRESS    0x10145000UL

#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_VENDOR_ID_FUNC         6
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_FUNC6_DEV0_NBIF0_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 6, 0x0)

#define SMN_DEV0_FUNC6_NBIF0_VENDOR_ID_ADDRESS    0x10146000UL

#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_VENDOR_ID_DEVICE       0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_VENDOR_ID_FUNC         7
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_VENDOR_ID_OFFSET       0x0
#define PCICFG_NBIO_FUNC7_DEV0_NBIF0_VENDOR_ID_ADDRESS      MAKE_SBDFO(0, 0, 0, 7, 0x0)

#define SMN_DEV0_FUNC7_NBIF0_VENDOR_ID_ADDRESS    0x10147000UL

#define SMN_DEV1_FUNC1_NBIF0_VENDOR_ID_ADDRESS    0x10149000UL

#define SMN_DEV1_FUNC2_NBIF0_VENDOR_ID_ADDRESS    0x1014a000UL

#define SMN_DEV1_FUNC3_NBIF0_VENDOR_ID_ADDRESS    0x1014b000UL

#define SMN_DEV2_FUNC1_NBIF0_VENDOR_ID_ADDRESS    0x10151000UL

#define SMN_DEV2_FUNC2_NBIF0_VENDOR_ID_ADDRESS    0x10152000UL

#define SMN_DEV2_FUNC3_NBIF0_VENDOR_ID_ADDRESS    0x10153000UL

#define SMN_DEV2_FUNC4_NBIF0_VENDOR_ID_ADDRESS    0x10154000UL

#define SMN_DEV2_FUNC5_NBIF0_VENDOR_ID_ADDRESS    0x10155000UL

#define SMN_DEV2_FUNC6_NBIF0_VENDOR_ID_ADDRESS    0x10156000UL

#endif /* _NBIFEPFNCFG_H_ */
