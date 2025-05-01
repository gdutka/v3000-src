/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _MP_MP0CRU_H_
#define _MP_MP0CRU_H_


/***********************************************************
* Register Name : MP0_C2PMSG_0
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_0_CONTENT_OFFSET      0
#define MP0_C2PMSG_0_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_0_STRUCT;

#define SMN_MP0_C2PMSG_0_ADDRESS    0x3810500UL


/***********************************************************
* Register Name : MP0_C2PMSG_1
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_1_CONTENT_OFFSET      0
#define MP0_C2PMSG_1_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_1_STRUCT;

#define SMN_MP0_C2PMSG_1_ADDRESS    0x3810504UL


/***********************************************************
* Register Name : MP0_C2PMSG_10
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_10_CONTENT_OFFSET      0
#define MP0_C2PMSG_10_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_10_STRUCT;

#define SMN_MP0_C2PMSG_10_ADDRESS    0x3810528UL


/***********************************************************
* Register Name : MP0_C2PMSG_100
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_100_CONTENT_OFFSET      0
#define MP0_C2PMSG_100_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_100_STRUCT;

#define SMN_MP0_C2PMSG_100_ADDRESS    0x3810a90UL


/***********************************************************
* Register Name : MP0_C2PMSG_101
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_101_CONTENT_OFFSET      0
#define MP0_C2PMSG_101_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_101_STRUCT;

#define SMN_MP0_C2PMSG_101_ADDRESS    0x3810a94UL


/***********************************************************
* Register Name : MP0_C2PMSG_102
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_102_CONTENT_OFFSET      0
#define MP0_C2PMSG_102_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_102_STRUCT;

#define SMN_MP0_C2PMSG_102_ADDRESS    0x3810a98UL


/***********************************************************
* Register Name : MP0_C2PMSG_103
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_103_CONTENT_OFFSET      0
#define MP0_C2PMSG_103_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_103_STRUCT;

#define SMN_MP0_C2PMSG_103_ADDRESS    0x3810a9cUL


/***********************************************************
* Register Name : MP0_C2PMSG_104
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_104_CONTENT_OFFSET      0
#define MP0_C2PMSG_104_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_104_STRUCT;

#define SMN_MP0_C2PMSG_104_ADDRESS    0x3810aa0UL


/***********************************************************
* Register Name : MP0_C2PMSG_105
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_105_CONTENT_OFFSET      0
#define MP0_C2PMSG_105_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_105_STRUCT;

#define SMN_MP0_C2PMSG_105_ADDRESS    0x3810aa4UL


/***********************************************************
* Register Name : MP0_C2PMSG_106
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_106_CONTENT_OFFSET      0
#define MP0_C2PMSG_106_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_106_STRUCT;

#define SMN_MP0_C2PMSG_106_ADDRESS    0x3810aa8UL


/***********************************************************
* Register Name : MP0_C2PMSG_107
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_107_CONTENT_OFFSET      0
#define MP0_C2PMSG_107_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_107_STRUCT;

#define SMN_MP0_C2PMSG_107_ADDRESS    0x3810aacUL


/***********************************************************
* Register Name : MP0_C2PMSG_108
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_108_CONTENT_OFFSET      0
#define MP0_C2PMSG_108_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_108_STRUCT;

#define SMN_MP0_C2PMSG_108_ADDRESS    0x3810ab0UL


/***********************************************************
* Register Name : MP0_C2PMSG_109
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_109_CONTENT_OFFSET      0
#define MP0_C2PMSG_109_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_109_STRUCT;

#define SMN_MP0_C2PMSG_109_ADDRESS    0x3810ab4UL


/***********************************************************
* Register Name : MP0_C2PMSG_11
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_11_CONTENT_OFFSET      0
#define MP0_C2PMSG_11_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_11_STRUCT;

#define SMN_MP0_C2PMSG_11_ADDRESS    0x381052cUL


/***********************************************************
* Register Name : MP0_C2PMSG_110
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_110_CONTENT_OFFSET      0
#define MP0_C2PMSG_110_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_110_STRUCT;

#define SMN_MP0_C2PMSG_110_ADDRESS    0x3810ab8UL


/***********************************************************
* Register Name : MP0_C2PMSG_111
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_111_CONTENT_OFFSET      0
#define MP0_C2PMSG_111_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_111_STRUCT;

#define SMN_MP0_C2PMSG_111_ADDRESS    0x3810abcUL


/***********************************************************
* Register Name : MP0_C2PMSG_112
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_112_CONTENT_OFFSET      0
#define MP0_C2PMSG_112_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_112_STRUCT;

#define SMN_MP0_C2PMSG_112_ADDRESS    0x3810ac0UL


/***********************************************************
* Register Name : MP0_C2PMSG_113
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_113_CONTENT_OFFSET      0
#define MP0_C2PMSG_113_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_113_STRUCT;

#define SMN_MP0_C2PMSG_113_ADDRESS    0x3810ac4UL


/***********************************************************
* Register Name : MP0_C2PMSG_114
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_114_CONTENT_OFFSET      0
#define MP0_C2PMSG_114_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_114_STRUCT;

#define SMN_MP0_C2PMSG_114_ADDRESS    0x3810ac8UL


/***********************************************************
* Register Name : MP0_C2PMSG_115
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_115_CONTENT_OFFSET      0
#define MP0_C2PMSG_115_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_115_STRUCT;

#define SMN_MP0_C2PMSG_115_ADDRESS    0x3810accUL


/***********************************************************
* Register Name : MP0_C2PMSG_116
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_116_CONTENT_OFFSET      0
#define MP0_C2PMSG_116_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_116_STRUCT;

#define SMN_MP0_C2PMSG_116_ADDRESS    0x3810ad0UL


/***********************************************************
* Register Name : MP0_C2PMSG_117
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_117_CONTENT_OFFSET      0
#define MP0_C2PMSG_117_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_117_STRUCT;

#define SMN_MP0_C2PMSG_117_ADDRESS    0x3810ad4UL


/***********************************************************
* Register Name : MP0_C2PMSG_118
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_118_CONTENT_OFFSET      0
#define MP0_C2PMSG_118_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_118_STRUCT;

#define SMN_MP0_C2PMSG_118_ADDRESS    0x3810ad8UL


/***********************************************************
* Register Name : MP0_C2PMSG_119
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_119_CONTENT_OFFSET      0
#define MP0_C2PMSG_119_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_119_STRUCT;

#define SMN_MP0_C2PMSG_119_ADDRESS    0x3810adcUL


/***********************************************************
* Register Name : MP0_C2PMSG_12
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_12_CONTENT_OFFSET      0
#define MP0_C2PMSG_12_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_12_STRUCT;

#define SMN_MP0_C2PMSG_12_ADDRESS    0x3810530UL


/***********************************************************
* Register Name : MP0_C2PMSG_120
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_120_CONTENT_OFFSET      0
#define MP0_C2PMSG_120_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_120_STRUCT;

#define SMN_MP0_C2PMSG_120_ADDRESS    0x3810ae0UL


/***********************************************************
* Register Name : MP0_C2PMSG_121
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_121_CONTENT_OFFSET      0
#define MP0_C2PMSG_121_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_121_STRUCT;

#define SMN_MP0_C2PMSG_121_ADDRESS    0x3810ae4UL


/***********************************************************
* Register Name : MP0_C2PMSG_122
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_122_CONTENT_OFFSET      0
#define MP0_C2PMSG_122_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_122_STRUCT;

#define SMN_MP0_C2PMSG_122_ADDRESS    0x3810ae8UL


/***********************************************************
* Register Name : MP0_C2PMSG_123
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_123_CONTENT_OFFSET      0
#define MP0_C2PMSG_123_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_123_STRUCT;

#define SMN_MP0_C2PMSG_123_ADDRESS    0x3810aecUL


/***********************************************************
* Register Name : MP0_C2PMSG_124
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_124_CONTENT_OFFSET      0
#define MP0_C2PMSG_124_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_124_STRUCT;

#define SMN_MP0_C2PMSG_124_ADDRESS    0x3810af0UL


/***********************************************************
* Register Name : MP0_C2PMSG_125
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_125_CONTENT_OFFSET      0
#define MP0_C2PMSG_125_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_125_STRUCT;

#define SMN_MP0_C2PMSG_125_ADDRESS    0x3810af4UL


/***********************************************************
* Register Name : MP0_C2PMSG_126
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_126_CONTENT_OFFSET      0
#define MP0_C2PMSG_126_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_126_STRUCT;

#define SMN_MP0_C2PMSG_126_ADDRESS    0x3810af8UL


/***********************************************************
* Register Name : MP0_C2PMSG_127
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_7
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_127_CONTENT_OFFSET      0
#define MP0_C2PMSG_127_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_127_STRUCT;

#define SMN_MP0_C2PMSG_127_ADDRESS    0x3810afcUL


/***********************************************************
* Register Name : MP0_C2PMSG_13
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_13_CONTENT_OFFSET      0
#define MP0_C2PMSG_13_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_13_STRUCT;

#define SMN_MP0_C2PMSG_13_ADDRESS    0x3810534UL


/***********************************************************
* Register Name : MP0_C2PMSG_14
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_14_CONTENT_OFFSET      0
#define MP0_C2PMSG_14_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_14_STRUCT;

#define SMN_MP0_C2PMSG_14_ADDRESS    0x3810538UL


/***********************************************************
* Register Name : MP0_C2PMSG_15
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_15_CONTENT_OFFSET      0
#define MP0_C2PMSG_15_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_15_STRUCT;

#define SMN_MP0_C2PMSG_15_ADDRESS    0x381053cUL


/***********************************************************
* Register Name : MP0_C2PMSG_16
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_16_CONTENT_OFFSET      0
#define MP0_C2PMSG_16_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_16_STRUCT;

#define SMN_MP0_C2PMSG_16_ADDRESS    0x3810540UL


/***********************************************************
* Register Name : MP0_C2PMSG_17
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_17_CONTENT_OFFSET      0
#define MP0_C2PMSG_17_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_17_STRUCT;

#define SMN_MP0_C2PMSG_17_ADDRESS    0x3810544UL


/***********************************************************
* Register Name : MP0_C2PMSG_18
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_18_CONTENT_OFFSET      0
#define MP0_C2PMSG_18_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_18_STRUCT;

#define SMN_MP0_C2PMSG_18_ADDRESS    0x3810548UL


/***********************************************************
* Register Name : MP0_C2PMSG_19
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_19_CONTENT_OFFSET      0
#define MP0_C2PMSG_19_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_19_STRUCT;

#define SMN_MP0_C2PMSG_19_ADDRESS    0x381054cUL


/***********************************************************
* Register Name : MP0_C2PMSG_2
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_2_CONTENT_OFFSET      0
#define MP0_C2PMSG_2_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_2_STRUCT;

#define SMN_MP0_C2PMSG_2_ADDRESS    0x3810508UL


/***********************************************************
* Register Name : MP0_C2PMSG_20
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_20_CONTENT_OFFSET      0
#define MP0_C2PMSG_20_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_20_STRUCT;

#define SMN_MP0_C2PMSG_20_ADDRESS    0x3810550UL


/***********************************************************
* Register Name : MP0_C2PMSG_21
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_21_CONTENT_OFFSET      0
#define MP0_C2PMSG_21_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_21_STRUCT;

#define SMN_MP0_C2PMSG_21_ADDRESS    0x3810554UL


/***********************************************************
* Register Name : MP0_C2PMSG_22
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_22_CONTENT_OFFSET      0
#define MP0_C2PMSG_22_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_22_STRUCT;

#define SMN_MP0_C2PMSG_22_ADDRESS    0x3810558UL


/***********************************************************
* Register Name : MP0_C2PMSG_23
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_23_CONTENT_OFFSET      0
#define MP0_C2PMSG_23_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_23_STRUCT;

#define SMN_MP0_C2PMSG_23_ADDRESS    0x381055cUL


/***********************************************************
* Register Name : MP0_C2PMSG_24
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_24_CONTENT_OFFSET      0
#define MP0_C2PMSG_24_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_24_STRUCT;

#define SMN_MP0_C2PMSG_24_ADDRESS    0x3810560UL


/***********************************************************
* Register Name : MP0_C2PMSG_25
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_25_CONTENT_OFFSET      0
#define MP0_C2PMSG_25_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_25_STRUCT;

#define SMN_MP0_C2PMSG_25_ADDRESS    0x3810564UL


/***********************************************************
* Register Name : MP0_C2PMSG_26
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_26_CONTENT_OFFSET      0
#define MP0_C2PMSG_26_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_26_STRUCT;

#define SMN_MP0_C2PMSG_26_ADDRESS    0x3810568UL


/***********************************************************
* Register Name : MP0_C2PMSG_27
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_27_CONTENT_OFFSET      0
#define MP0_C2PMSG_27_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_27_STRUCT;

#define SMN_MP0_C2PMSG_27_ADDRESS    0x381056cUL


/***********************************************************
* Register Name : MP0_C2PMSG_28
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_28_CONTENT_OFFSET      0
#define MP0_C2PMSG_28_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_28_STRUCT;

#define SMN_MP0_C2PMSG_28_ADDRESS    0x3810570UL


/***********************************************************
* Register Name : MP0_C2PMSG_29
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_29_CONTENT_OFFSET      0
#define MP0_C2PMSG_29_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_29_STRUCT;

#define SMN_MP0_C2PMSG_29_ADDRESS    0x3810574UL


/***********************************************************
* Register Name : MP0_C2PMSG_3
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_3_CONTENT_OFFSET      0
#define MP0_C2PMSG_3_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_3_STRUCT;

#define SMN_MP0_C2PMSG_3_ADDRESS    0x381050cUL


/***********************************************************
* Register Name : MP0_C2PMSG_30
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_30_CONTENT_OFFSET      0
#define MP0_C2PMSG_30_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_30_STRUCT;

#define SMN_MP0_C2PMSG_30_ADDRESS    0x3810578UL


/***********************************************************
* Register Name : MP0_C2PMSG_31
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_1
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_31_CONTENT_OFFSET      0
#define MP0_C2PMSG_31_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_31_STRUCT;

#define SMN_MP0_C2PMSG_31_ADDRESS    0x381057cUL


/***********************************************************
* Register Name : MP0_C2PMSG_32
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_32_CONTENT_OFFSET      0
#define MP0_C2PMSG_32_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_32_STRUCT;

#define SMN_MP0_C2PMSG_32_ADDRESS    0x3810980UL


/***********************************************************
* Register Name : MP0_C2PMSG_33
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_33_CONTENT_OFFSET      0
#define MP0_C2PMSG_33_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_33_STRUCT;

#define SMN_MP0_C2PMSG_33_ADDRESS    0x3810984UL


/***********************************************************
* Register Name : MP0_C2PMSG_34
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_34_CONTENT_OFFSET      0
#define MP0_C2PMSG_34_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_34_STRUCT;

#define SMN_MP0_C2PMSG_34_ADDRESS    0x3810988UL


/***********************************************************
* Register Name : MP0_C2PMSG_35
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_35_CONTENT_OFFSET      0
#define MP0_C2PMSG_35_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_35_STRUCT;

#define SMN_MP0_C2PMSG_35_ADDRESS    0x381098cUL


/***********************************************************
* Register Name : MP0_C2PMSG_36
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_36_CONTENT_OFFSET      0
#define MP0_C2PMSG_36_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_36_STRUCT;

#define SMN_MP0_C2PMSG_36_ADDRESS    0x3810990UL


/***********************************************************
* Register Name : MP0_C2PMSG_37
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_37_CONTENT_OFFSET      0
#define MP0_C2PMSG_37_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_37_STRUCT;

#define SMN_MP0_C2PMSG_37_ADDRESS    0x3810994UL


/***********************************************************
* Register Name : MP0_C2PMSG_38
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_38_CONTENT_OFFSET      0
#define MP0_C2PMSG_38_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_38_STRUCT;

#define SMN_MP0_C2PMSG_38_ADDRESS    0x3810998UL


/***********************************************************
* Register Name : MP0_C2PMSG_39
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_39_CONTENT_OFFSET      0
#define MP0_C2PMSG_39_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_39_STRUCT;

#define SMN_MP0_C2PMSG_39_ADDRESS    0x381099cUL


/***********************************************************
* Register Name : MP0_C2PMSG_4
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_4_CONTENT_OFFSET      0
#define MP0_C2PMSG_4_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_4_STRUCT;

#define SMN_MP0_C2PMSG_4_ADDRESS    0x3810510UL


/***********************************************************
* Register Name : MP0_C2PMSG_40
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_40_CONTENT_OFFSET      0
#define MP0_C2PMSG_40_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_40_STRUCT;

#define SMN_MP0_C2PMSG_40_ADDRESS    0x38109a0UL


/***********************************************************
* Register Name : MP0_C2PMSG_41
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_41_CONTENT_OFFSET      0
#define MP0_C2PMSG_41_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_41_STRUCT;

#define SMN_MP0_C2PMSG_41_ADDRESS    0x38109a4UL


/***********************************************************
* Register Name : MP0_C2PMSG_42
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_42_CONTENT_OFFSET      0
#define MP0_C2PMSG_42_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_42_STRUCT;

#define SMN_MP0_C2PMSG_42_ADDRESS    0x38109a8UL


/***********************************************************
* Register Name : MP0_C2PMSG_43
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_43_CONTENT_OFFSET      0
#define MP0_C2PMSG_43_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_43_STRUCT;

#define SMN_MP0_C2PMSG_43_ADDRESS    0x38109acUL


/***********************************************************
* Register Name : MP0_C2PMSG_44
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_44_CONTENT_OFFSET      0
#define MP0_C2PMSG_44_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_44_STRUCT;

#define SMN_MP0_C2PMSG_44_ADDRESS    0x38109b0UL


/***********************************************************
* Register Name : MP0_C2PMSG_45
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_45_CONTENT_OFFSET      0
#define MP0_C2PMSG_45_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_45_STRUCT;

#define SMN_MP0_C2PMSG_45_ADDRESS    0x38109b4UL


/***********************************************************
* Register Name : MP0_C2PMSG_46
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_46_CONTENT_OFFSET      0
#define MP0_C2PMSG_46_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_46_STRUCT;

#define SMN_MP0_C2PMSG_46_ADDRESS    0x38109b8UL


/***********************************************************
* Register Name : MP0_C2PMSG_47
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_2
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_47_CONTENT_OFFSET      0
#define MP0_C2PMSG_47_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_47_STRUCT;

#define SMN_MP0_C2PMSG_47_ADDRESS    0x38109bcUL


/***********************************************************
* Register Name : MP0_C2PMSG_48
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_48_CONTENT_OFFSET      0
#define MP0_C2PMSG_48_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_48_STRUCT;

#define SMN_MP0_C2PMSG_48_ADDRESS    0x38109c0UL


/***********************************************************
* Register Name : MP0_C2PMSG_49
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_49_CONTENT_OFFSET      0
#define MP0_C2PMSG_49_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_49_STRUCT;

#define SMN_MP0_C2PMSG_49_ADDRESS    0x38109c4UL


/***********************************************************
* Register Name : MP0_C2PMSG_5
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_5_CONTENT_OFFSET      0
#define MP0_C2PMSG_5_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_5_STRUCT;

#define SMN_MP0_C2PMSG_5_ADDRESS    0x3810514UL


/***********************************************************
* Register Name : MP0_C2PMSG_50
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_50_CONTENT_OFFSET      0
#define MP0_C2PMSG_50_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_50_STRUCT;

#define SMN_MP0_C2PMSG_50_ADDRESS    0x38109c8UL


/***********************************************************
* Register Name : MP0_C2PMSG_51
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_51_CONTENT_OFFSET      0
#define MP0_C2PMSG_51_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_51_STRUCT;

#define SMN_MP0_C2PMSG_51_ADDRESS    0x38109ccUL


/***********************************************************
* Register Name : MP0_C2PMSG_52
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_52_CONTENT_OFFSET      0
#define MP0_C2PMSG_52_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_52_STRUCT;

#define SMN_MP0_C2PMSG_52_ADDRESS    0x38109d0UL


/***********************************************************
* Register Name : MP0_C2PMSG_53
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_53_CONTENT_OFFSET      0
#define MP0_C2PMSG_53_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_53_STRUCT;

#define SMN_MP0_C2PMSG_53_ADDRESS    0x38109d4UL


/***********************************************************
* Register Name : MP0_C2PMSG_54
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_54_CONTENT_OFFSET      0
#define MP0_C2PMSG_54_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_54_STRUCT;

#define SMN_MP0_C2PMSG_54_ADDRESS    0x38109d8UL


/***********************************************************
* Register Name : MP0_C2PMSG_55
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_55_CONTENT_OFFSET      0
#define MP0_C2PMSG_55_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_55_STRUCT;

#define SMN_MP0_C2PMSG_55_ADDRESS    0x38109dcUL


/***********************************************************
* Register Name : MP0_C2PMSG_56
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_56_CONTENT_OFFSET      0
#define MP0_C2PMSG_56_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_56_STRUCT;

#define SMN_MP0_C2PMSG_56_ADDRESS    0x38109e0UL


/***********************************************************
* Register Name : MP0_C2PMSG_57
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_57_CONTENT_OFFSET      0
#define MP0_C2PMSG_57_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_57_STRUCT;

#define SMN_MP0_C2PMSG_57_ADDRESS    0x38109e4UL


/***********************************************************
* Register Name : MP0_C2PMSG_58
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_58_CONTENT_OFFSET      0
#define MP0_C2PMSG_58_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_58_STRUCT;

#define SMN_MP0_C2PMSG_58_ADDRESS    0x38109e8UL


/***********************************************************
* Register Name : MP0_C2PMSG_59
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_59_CONTENT_OFFSET      0
#define MP0_C2PMSG_59_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_59_STRUCT;

#define SMN_MP0_C2PMSG_59_ADDRESS    0x38109ecUL


/***********************************************************
* Register Name : MP0_C2PMSG_6
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_6_CONTENT_OFFSET      0
#define MP0_C2PMSG_6_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_6_STRUCT;

#define SMN_MP0_C2PMSG_6_ADDRESS    0x3810518UL


/***********************************************************
* Register Name : MP0_C2PMSG_60
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_60_CONTENT_OFFSET      0
#define MP0_C2PMSG_60_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_60_STRUCT;

#define SMN_MP0_C2PMSG_60_ADDRESS    0x38109f0UL


/***********************************************************
* Register Name : MP0_C2PMSG_61
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_61_CONTENT_OFFSET      0
#define MP0_C2PMSG_61_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_61_STRUCT;

#define SMN_MP0_C2PMSG_61_ADDRESS    0x38109f4UL


/***********************************************************
* Register Name : MP0_C2PMSG_62
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_62_CONTENT_OFFSET      0
#define MP0_C2PMSG_62_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_62_STRUCT;

#define SMN_MP0_C2PMSG_62_ADDRESS    0x38109f8UL


/***********************************************************
* Register Name : MP0_C2PMSG_63
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_3
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_63_CONTENT_OFFSET      0
#define MP0_C2PMSG_63_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_63_STRUCT;

#define SMN_MP0_C2PMSG_63_ADDRESS    0x38109fcUL


/***********************************************************
* Register Name : MP0_C2PMSG_64
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_64_CONTENT_OFFSET      0
#define MP0_C2PMSG_64_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_64_STRUCT;

#define SMN_MP0_C2PMSG_64_ADDRESS    0x3810a00UL


/***********************************************************
* Register Name : MP0_C2PMSG_65
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_65_CONTENT_OFFSET      0
#define MP0_C2PMSG_65_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_65_STRUCT;

#define SMN_MP0_C2PMSG_65_ADDRESS    0x3810a04UL


/***********************************************************
* Register Name : MP0_C2PMSG_66
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_66_CONTENT_OFFSET      0
#define MP0_C2PMSG_66_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_66_STRUCT;

#define SMN_MP0_C2PMSG_66_ADDRESS    0x3810a08UL


/***********************************************************
* Register Name : MP0_C2PMSG_67
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_67_CONTENT_OFFSET      0
#define MP0_C2PMSG_67_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_67_STRUCT;

#define SMN_MP0_C2PMSG_67_ADDRESS    0x3810a0cUL


/***********************************************************
* Register Name : MP0_C2PMSG_68
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_68_CONTENT_OFFSET      0
#define MP0_C2PMSG_68_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_68_STRUCT;

#define SMN_MP0_C2PMSG_68_ADDRESS    0x3810a10UL


/***********************************************************
* Register Name : MP0_C2PMSG_69
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_69_CONTENT_OFFSET      0
#define MP0_C2PMSG_69_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_69_STRUCT;

#define SMN_MP0_C2PMSG_69_ADDRESS    0x3810a14UL


/***********************************************************
* Register Name : MP0_C2PMSG_7
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_7_CONTENT_OFFSET      0
#define MP0_C2PMSG_7_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_7_STRUCT;

#define SMN_MP0_C2PMSG_7_ADDRESS    0x381051cUL


/***********************************************************
* Register Name : MP0_C2PMSG_70
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_70_CONTENT_OFFSET      0
#define MP0_C2PMSG_70_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_70_STRUCT;

#define SMN_MP0_C2PMSG_70_ADDRESS    0x3810a18UL


/***********************************************************
* Register Name : MP0_C2PMSG_71
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_71_CONTENT_OFFSET      0
#define MP0_C2PMSG_71_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_71_STRUCT;

#define SMN_MP0_C2PMSG_71_ADDRESS    0x3810a1cUL


/***********************************************************
* Register Name : MP0_C2PMSG_72
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_72_CONTENT_OFFSET      0
#define MP0_C2PMSG_72_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_72_STRUCT;

#define SMN_MP0_C2PMSG_72_ADDRESS    0x3810a20UL


/***********************************************************
* Register Name : MP0_C2PMSG_73
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_73_CONTENT_OFFSET      0
#define MP0_C2PMSG_73_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_73_STRUCT;

#define SMN_MP0_C2PMSG_73_ADDRESS    0x3810a24UL


/***********************************************************
* Register Name : MP0_C2PMSG_74
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_74_CONTENT_OFFSET      0
#define MP0_C2PMSG_74_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_74_STRUCT;

#define SMN_MP0_C2PMSG_74_ADDRESS    0x3810a28UL


/***********************************************************
* Register Name : MP0_C2PMSG_75
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_75_CONTENT_OFFSET      0
#define MP0_C2PMSG_75_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_75_STRUCT;

#define SMN_MP0_C2PMSG_75_ADDRESS    0x3810a2cUL


/***********************************************************
* Register Name : MP0_C2PMSG_76
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_76_CONTENT_OFFSET      0
#define MP0_C2PMSG_76_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_76_STRUCT;

#define SMN_MP0_C2PMSG_76_ADDRESS    0x3810a30UL


/***********************************************************
* Register Name : MP0_C2PMSG_77
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_77_CONTENT_OFFSET      0
#define MP0_C2PMSG_77_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_77_STRUCT;

#define SMN_MP0_C2PMSG_77_ADDRESS    0x3810a34UL


/***********************************************************
* Register Name : MP0_C2PMSG_78
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_78_CONTENT_OFFSET      0
#define MP0_C2PMSG_78_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_78_STRUCT;

#define SMN_MP0_C2PMSG_78_ADDRESS    0x3810a38UL


/***********************************************************
* Register Name : MP0_C2PMSG_79
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_4
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_79_CONTENT_OFFSET      0
#define MP0_C2PMSG_79_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_79_STRUCT;

#define SMN_MP0_C2PMSG_79_ADDRESS    0x3810a3cUL


/***********************************************************
* Register Name : MP0_C2PMSG_8
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_8_CONTENT_OFFSET      0
#define MP0_C2PMSG_8_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_8_STRUCT;

#define SMN_MP0_C2PMSG_8_ADDRESS    0x3810520UL


/***********************************************************
* Register Name : MP0_C2PMSG_80
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_80_CONTENT_OFFSET      0
#define MP0_C2PMSG_80_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_80_STRUCT;

#define SMN_MP0_C2PMSG_80_ADDRESS    0x3810a40UL


/***********************************************************
* Register Name : MP0_C2PMSG_81
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_81_CONTENT_OFFSET      0
#define MP0_C2PMSG_81_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_81_STRUCT;

#define SMN_MP0_C2PMSG_81_ADDRESS    0x3810a44UL


/***********************************************************
* Register Name : MP0_C2PMSG_82
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_82_CONTENT_OFFSET      0
#define MP0_C2PMSG_82_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_82_STRUCT;

#define SMN_MP0_C2PMSG_82_ADDRESS    0x3810a48UL


/***********************************************************
* Register Name : MP0_C2PMSG_83
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_83_CONTENT_OFFSET      0
#define MP0_C2PMSG_83_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_83_STRUCT;

#define SMN_MP0_C2PMSG_83_ADDRESS    0x3810a4cUL


/***********************************************************
* Register Name : MP0_C2PMSG_84
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_84_CONTENT_OFFSET      0
#define MP0_C2PMSG_84_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_84_STRUCT;

#define SMN_MP0_C2PMSG_84_ADDRESS    0x3810a50UL


/***********************************************************
* Register Name : MP0_C2PMSG_85
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_85_CONTENT_OFFSET      0
#define MP0_C2PMSG_85_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_85_STRUCT;

#define SMN_MP0_C2PMSG_85_ADDRESS    0x3810a54UL


/***********************************************************
* Register Name : MP0_C2PMSG_86
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_86_CONTENT_OFFSET      0
#define MP0_C2PMSG_86_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_86_STRUCT;

#define SMN_MP0_C2PMSG_86_ADDRESS    0x3810a58UL


/***********************************************************
* Register Name : MP0_C2PMSG_87
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_87_CONTENT_OFFSET      0
#define MP0_C2PMSG_87_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_87_STRUCT;

#define SMN_MP0_C2PMSG_87_ADDRESS    0x3810a5cUL


/***********************************************************
* Register Name : MP0_C2PMSG_88
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_88_CONTENT_OFFSET      0
#define MP0_C2PMSG_88_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_88_STRUCT;

#define SMN_MP0_C2PMSG_88_ADDRESS    0x3810a60UL


/***********************************************************
* Register Name : MP0_C2PMSG_89
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_89_CONTENT_OFFSET      0
#define MP0_C2PMSG_89_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_89_STRUCT;

#define SMN_MP0_C2PMSG_89_ADDRESS    0x3810a64UL


/***********************************************************
* Register Name : MP0_C2PMSG_9
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_0
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_C2PMSG_9_CONTENT_OFFSET      0
#define MP0_C2PMSG_9_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_9_STRUCT;

#define SMN_MP0_C2PMSG_9_ADDRESS    0x3810524UL


/***********************************************************
* Register Name : MP0_C2PMSG_90
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_90_CONTENT_OFFSET      0
#define MP0_C2PMSG_90_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_90_STRUCT;

#define SMN_MP0_C2PMSG_90_ADDRESS    0x3810a68UL


/***********************************************************
* Register Name : MP0_C2PMSG_91
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_91_CONTENT_OFFSET      0
#define MP0_C2PMSG_91_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_91_STRUCT;

#define SMN_MP0_C2PMSG_91_ADDRESS    0x3810a6cUL


/***********************************************************
* Register Name : MP0_C2PMSG_92
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_92_CONTENT_OFFSET      0
#define MP0_C2PMSG_92_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_92_STRUCT;

#define SMN_MP0_C2PMSG_92_ADDRESS    0x3810a70UL


/***********************************************************
* Register Name : MP0_C2PMSG_93
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_93_CONTENT_OFFSET      0
#define MP0_C2PMSG_93_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_93_STRUCT;

#define SMN_MP0_C2PMSG_93_ADDRESS    0x3810a74UL


/***********************************************************
* Register Name : MP0_C2PMSG_94
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_94_CONTENT_OFFSET      0
#define MP0_C2PMSG_94_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_94_STRUCT;

#define SMN_MP0_C2PMSG_94_ADDRESS    0x3810a78UL


/***********************************************************
* Register Name : MP0_C2PMSG_95
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_5
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_95_CONTENT_OFFSET      0
#define MP0_C2PMSG_95_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_95_STRUCT;

#define SMN_MP0_C2PMSG_95_ADDRESS    0x3810a7cUL


/***********************************************************
* Register Name : MP0_C2PMSG_96
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_96_CONTENT_OFFSET      0
#define MP0_C2PMSG_96_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_96_STRUCT;

#define SMN_MP0_C2PMSG_96_ADDRESS    0x3810a80UL


/***********************************************************
* Register Name : MP0_C2PMSG_97
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_97_CONTENT_OFFSET      0
#define MP0_C2PMSG_97_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_97_STRUCT;

#define SMN_MP0_C2PMSG_97_ADDRESS    0x3810a84UL


/***********************************************************
* Register Name : MP0_C2PMSG_98
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_98_CONTENT_OFFSET      0
#define MP0_C2PMSG_98_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_98_STRUCT;

#define SMN_MP0_C2PMSG_98_ADDRESS    0x3810a88UL


/***********************************************************
* Register Name : MP0_C2PMSG_99
* Register Description :
* Inbound mailbox register (external->MP0)
External access control by C2PMSG_ATTR_6
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : CPU to MP0 message (32-bit register)
#define MP0_C2PMSG_99_CONTENT_OFFSET      0
#define MP0_C2PMSG_99_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_C2PMSG_99_STRUCT;

#define SMN_MP0_C2PMSG_99_ADDRESS    0x3810a8cUL


/***********************************************************
* Register Name : MP0_P2CMSG_0
* Register Description :
* Outbound message register (MP0 -> external)
External access control by P2CMSG_ATTR
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_P2CMSG_0_CONTENT_OFFSET      0
#define MP0_P2CMSG_0_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_P2CMSG_0_STRUCT;

#define SMN_MP0_P2CMSG_0_ADDRESS    0x3810680UL


/***********************************************************
* Register Name : MP0_P2CMSG_1
* Register Description :
* Outbound message register (MP0 -> external)
External access control by P2CMSG_ATTR
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_P2CMSG_1_CONTENT_OFFSET      0
#define MP0_P2CMSG_1_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_P2CMSG_1_STRUCT;

#define SMN_MP0_P2CMSG_1_ADDRESS    0x3810684UL


/***********************************************************
* Register Name : MP0_P2CMSG_2
* Register Description :
* Outbound message register (MP0 -> external)
External access control by P2CMSG_ATTR
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_P2CMSG_2_CONTENT_OFFSET      0
#define MP0_P2CMSG_2_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_P2CMSG_2_STRUCT;

#define SMN_MP0_P2CMSG_2_ADDRESS    0x3810688UL


/***********************************************************
* Register Name : MP0_P2CMSG_3
* Register Description :
* Outbound message register (MP0 -> external)
External access control by P2CMSG_ATTR
Reset by Cpl_VDDCR_<dom>_Resetn (0-> RESET, 1 -> out of RESET), where <dom>=VDD_SOC, VDD_S5, etc. as applicable
* Visibility : 0x2
************************************************************/

// Bitfield Description : message content
#define MP0_P2CMSG_3_CONTENT_OFFSET      0
#define MP0_P2CMSG_3_CONTENT_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            CONTENT:32;
  } Field;
  UINT32 Value;
} MP0_P2CMSG_3_STRUCT;

#define SMN_MP0_P2CMSG_3_ADDRESS    0x381068cUL

#endif /* _MP_MP0CRU_H_ */
