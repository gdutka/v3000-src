/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _ATHUB_XPB_H_
#define _ATHUB_XPB_H_


/***********************************************************
* Register Name : XPB_CLG_CFG0
* Register Description :
* Each Client Group (CLG) is assigned to use specific resources (ie: wcb #, P2P BAR#, Host P2P Flush Counter #, Side P2P Flush Counter #), and with which type of load balancing to use when accessing the neighboring GPU's memenory. Reg(0-19) applies to: (all_others, cp, extra(0-1), cb(0-3), db(0-3), sx/tc(0-3), drmdma0(even,odd), drmdma1(even,odd)). <from XPB_defs.vh>
* Visibility : 0x2
************************************************************/

// Bitfield Description : indicates which Write Combining Buffer to use
#define XPB_CLG_CFG0_WCB_NUM_OFFSET      0
#define XPB_CLG_CFG0_WCB_NUM_MASK        0xf

// Bitfield Description : Indicates which type of load balancing (LB) to apply (This SoC doesn't suport LB).
#define XPB_CLG_CFG0_LB_TYPE_OFFSET      4
#define XPB_CLG_CFG0_LB_TYPE_MASK        0x70

// Bitfield Description : indicates which Host P2P BAR # to use to compress requests across the hostport
#define XPB_CLG_CFG0_P2P_BAR_OFFSET      7
#define XPB_CLG_CFG0_P2P_BAR_MASK        0x380

// Bitfield Description : indicates which Host P2P Flush Counter # to use for flushing/tracking requests across hostport, valid range is 0~7.
#define XPB_CLG_CFG0_HOST_FLUSH_OFFSET      10
#define XPB_CLG_CFG0_HOST_FLUSH_MASK        0x3c00

// Bitfield Description : Iindicates which Side P2P Flush Counter # to use for flushing/tracking requests across sideport (this SoC doesn't suport sideport).
#define XPB_CLG_CFG0_SIDE_FLUSH_OFFSET      14
#define XPB_CLG_CFG0_SIDE_FLUSH_MASK        0x3c000

// Bitfield Description : 
#define XPB_CLG_CFG0_Reserved_31_18_OFFSET      18
#define XPB_CLG_CFG0_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            WCB_NUM:4;
    UINT32                            LB_TYPE:3;
    UINT32                            P2P_BAR:3;
    UINT32                            HOST_FLUSH:4;
    UINT32                            SIDE_FLUSH:4;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} XPB_CLG_CFG0_STRUCT;

#define SMN_XPB_CLG_CFG0_ADDRESS    0x3210UL


/***********************************************************
* Register Name : XPB_CLG_CFG1
* Register Description :
* Each Client Group (CLG) is assigned to use specific resources (ie: wcb #, P2P BAR#, Host P2P Flush Counter #, Side P2P Flush Counter #), and with which type of load balancing to use when accessing the neighboring GPU's memenory. Reg(0-19) applies to: (all_others, cp, extra(0-1), cb(0-3), db(0-3), sx/tc(0-3), drmdma0(even,odd), drmdma1(even,odd)). <from XPB_defs.vh>
* Visibility : 0x2
************************************************************/

// Bitfield Description : indicates which Write Combining Buffer to use
#define XPB_CLG_CFG1_WCB_NUM_OFFSET      0
#define XPB_CLG_CFG1_WCB_NUM_MASK        0xf

// Bitfield Description : Indicates which type of load balancing (LB) to apply (This SoC doesn't suport LB).
#define XPB_CLG_CFG1_LB_TYPE_OFFSET      4
#define XPB_CLG_CFG1_LB_TYPE_MASK        0x70

// Bitfield Description : indicates which Host P2P BAR # to use to compress requests across the hostport
#define XPB_CLG_CFG1_P2P_BAR_OFFSET      7
#define XPB_CLG_CFG1_P2P_BAR_MASK        0x380

// Bitfield Description : indicates which Host P2P Flush Counter # to use for flushing/tracking requests across hostport, valid range is 0~7.
#define XPB_CLG_CFG1_HOST_FLUSH_OFFSET      10
#define XPB_CLG_CFG1_HOST_FLUSH_MASK        0x3c00

// Bitfield Description : Iindicates which Side P2P Flush Counter # to use for flushing/tracking requests across sideport (this SoC doesn't suport sideport).
#define XPB_CLG_CFG1_SIDE_FLUSH_OFFSET      14
#define XPB_CLG_CFG1_SIDE_FLUSH_MASK        0x3c000

// Bitfield Description : 
#define XPB_CLG_CFG1_Reserved_31_18_OFFSET      18
#define XPB_CLG_CFG1_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            WCB_NUM:4;
    UINT32                            LB_TYPE:3;
    UINT32                            P2P_BAR:3;
    UINT32                            HOST_FLUSH:4;
    UINT32                            SIDE_FLUSH:4;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} XPB_CLG_CFG1_STRUCT;

#define SMN_XPB_CLG_CFG1_ADDRESS    0x3214UL


/***********************************************************
* Register Name : XPB_CLG_CFG2
* Register Description :
* Each Client Group (CLG) is assigned to use specific resources (ie: wcb #, P2P BAR#, Host P2P Flush Counter #, Side P2P Flush Counter #), and with which type of load balancing to use when accessing the neighboring GPU's memenory. Reg(0-19) applies to: (all_others, cp, extra(0-1), cb(0-3), db(0-3), sx/tc(0-3), drmdma0(even,odd), drmdma1(even,odd)). <from XPB_defs.vh>
* Visibility : 0x2
************************************************************/

// Bitfield Description : indicates which Write Combining Buffer to use
#define XPB_CLG_CFG2_WCB_NUM_OFFSET      0
#define XPB_CLG_CFG2_WCB_NUM_MASK        0xf

// Bitfield Description : Indicates which type of load balancing (LB) to apply (This SoC doesn't suport LB).
#define XPB_CLG_CFG2_LB_TYPE_OFFSET      4
#define XPB_CLG_CFG2_LB_TYPE_MASK        0x70

// Bitfield Description : indicates which Host P2P BAR # to use to compress requests across the hostport
#define XPB_CLG_CFG2_P2P_BAR_OFFSET      7
#define XPB_CLG_CFG2_P2P_BAR_MASK        0x380

// Bitfield Description : indicates which Host P2P Flush Counter # to use for flushing/tracking requests across hostport, valid range is 0~7.
#define XPB_CLG_CFG2_HOST_FLUSH_OFFSET      10
#define XPB_CLG_CFG2_HOST_FLUSH_MASK        0x3c00

// Bitfield Description : Iindicates which Side P2P Flush Counter # to use for flushing/tracking requests across sideport (this SoC doesn't suport sideport).
#define XPB_CLG_CFG2_SIDE_FLUSH_OFFSET      14
#define XPB_CLG_CFG2_SIDE_FLUSH_MASK        0x3c000

// Bitfield Description : 
#define XPB_CLG_CFG2_Reserved_31_18_OFFSET      18
#define XPB_CLG_CFG2_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            WCB_NUM:4;
    UINT32                            LB_TYPE:3;
    UINT32                            P2P_BAR:3;
    UINT32                            HOST_FLUSH:4;
    UINT32                            SIDE_FLUSH:4;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} XPB_CLG_CFG2_STRUCT;

#define SMN_XPB_CLG_CFG2_ADDRESS    0x3218UL


/***********************************************************
* Register Name : XPB_CLG_CFG3
* Register Description :
* Each Client Group (CLG) is assigned to use specific resources (ie: wcb #, P2P BAR#, Host P2P Flush Counter #, Side P2P Flush Counter #), and with which type of load balancing to use when accessing the neighboring GPU's memenory. Reg(0-19) applies to: (all_others, cp, extra(0-1), cb(0-3), db(0-3), sx/tc(0-3), drmdma0(even,odd), drmdma1(even,odd)). <from XPB_defs.vh>
* Visibility : 0x2
************************************************************/

// Bitfield Description : indicates which Write Combining Buffer to use
#define XPB_CLG_CFG3_WCB_NUM_OFFSET      0
#define XPB_CLG_CFG3_WCB_NUM_MASK        0xf

// Bitfield Description : Indicates which type of load balancing (LB) to apply (This SoC doesn't suport LB).
#define XPB_CLG_CFG3_LB_TYPE_OFFSET      4
#define XPB_CLG_CFG3_LB_TYPE_MASK        0x70

// Bitfield Description : indicates which Host P2P BAR # to use to compress requests across the hostport
#define XPB_CLG_CFG3_P2P_BAR_OFFSET      7
#define XPB_CLG_CFG3_P2P_BAR_MASK        0x380

// Bitfield Description : indicates which Host P2P Flush Counter # to use for flushing/tracking requests across hostport, valid range is 0~7.
#define XPB_CLG_CFG3_HOST_FLUSH_OFFSET      10
#define XPB_CLG_CFG3_HOST_FLUSH_MASK        0x3c00

// Bitfield Description : Iindicates which Side P2P Flush Counter # to use for flushing/tracking requests across sideport (this SoC doesn't suport sideport).
#define XPB_CLG_CFG3_SIDE_FLUSH_OFFSET      14
#define XPB_CLG_CFG3_SIDE_FLUSH_MASK        0x3c000

// Bitfield Description : 
#define XPB_CLG_CFG3_Reserved_31_18_OFFSET      18
#define XPB_CLG_CFG3_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            WCB_NUM:4;
    UINT32                            LB_TYPE:3;
    UINT32                            P2P_BAR:3;
    UINT32                            HOST_FLUSH:4;
    UINT32                            SIDE_FLUSH:4;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} XPB_CLG_CFG3_STRUCT;

#define SMN_XPB_CLG_CFG3_ADDRESS    0x321cUL


/***********************************************************
* Register Name : XPB_CLG_CFG4
* Register Description :
* Each Client Group (CLG) is assigned to use specific resources (ie: wcb #, P2P BAR#, Host P2P Flush Counter #, Side P2P Flush Counter #), and with which type of load balancing to use when accessing the neighboring GPU's memenory. Reg(0-19) applies to: (all_others, cp, extra(0-1), cb(0-3), db(0-3), sx/tc(0-3), drmdma0(even,odd), drmdma1(even,odd)). <from XPB_defs.vh>
* Visibility : 0x2
************************************************************/

// Bitfield Description : indicates which Write Combining Buffer to use
#define XPB_CLG_CFG4_WCB_NUM_OFFSET      0
#define XPB_CLG_CFG4_WCB_NUM_MASK        0xf

// Bitfield Description : Indicates which type of load balancing (LB) to apply (This SoC doesn't suport LB).
#define XPB_CLG_CFG4_LB_TYPE_OFFSET      4
#define XPB_CLG_CFG4_LB_TYPE_MASK        0x70

// Bitfield Description : indicates which Host P2P BAR # to use to compress requests across the hostport
#define XPB_CLG_CFG4_P2P_BAR_OFFSET      7
#define XPB_CLG_CFG4_P2P_BAR_MASK        0x380

// Bitfield Description : indicates which Host P2P Flush Counter # to use for flushing/tracking requests across hostport, valid range is 0~7.
#define XPB_CLG_CFG4_HOST_FLUSH_OFFSET      10
#define XPB_CLG_CFG4_HOST_FLUSH_MASK        0x3c00

// Bitfield Description : Iindicates which Side P2P Flush Counter # to use for flushing/tracking requests across sideport (this SoC doesn't suport sideport).
#define XPB_CLG_CFG4_SIDE_FLUSH_OFFSET      14
#define XPB_CLG_CFG4_SIDE_FLUSH_MASK        0x3c000

// Bitfield Description : 
#define XPB_CLG_CFG4_Reserved_31_18_OFFSET      18
#define XPB_CLG_CFG4_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            WCB_NUM:4;
    UINT32                            LB_TYPE:3;
    UINT32                            P2P_BAR:3;
    UINT32                            HOST_FLUSH:4;
    UINT32                            SIDE_FLUSH:4;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} XPB_CLG_CFG4_STRUCT;

#define SMN_XPB_CLG_CFG4_ADDRESS    0x3220UL


/***********************************************************
* Register Name : XPB_CLG_CFG5
* Register Description :
* Each Client Group (CLG) is assigned to use specific resources (ie: wcb #, P2P BAR#, Host P2P Flush Counter #, Side P2P Flush Counter #), and with which type of load balancing to use when accessing the neighboring GPU's memenory. Reg(0-19) applies to: (all_others, cp, extra(0-1), cb(0-3), db(0-3), sx/tc(0-3), drmdma0(even,odd), drmdma1(even,odd)). <from XPB_defs.vh>
* Visibility : 0x2
************************************************************/

// Bitfield Description : indicates which Write Combining Buffer to use
#define XPB_CLG_CFG5_WCB_NUM_OFFSET      0
#define XPB_CLG_CFG5_WCB_NUM_MASK        0xf

// Bitfield Description : Indicates which type of load balancing (LB) to apply (This SoC doesn't suport LB).
#define XPB_CLG_CFG5_LB_TYPE_OFFSET      4
#define XPB_CLG_CFG5_LB_TYPE_MASK        0x70

// Bitfield Description : indicates which Host P2P BAR # to use to compress requests across the hostport
#define XPB_CLG_CFG5_P2P_BAR_OFFSET      7
#define XPB_CLG_CFG5_P2P_BAR_MASK        0x380

// Bitfield Description : indicates which Host P2P Flush Counter # to use for flushing/tracking requests across hostport, valid range is 0~7.
#define XPB_CLG_CFG5_HOST_FLUSH_OFFSET      10
#define XPB_CLG_CFG5_HOST_FLUSH_MASK        0x3c00

// Bitfield Description : Iindicates which Side P2P Flush Counter # to use for flushing/tracking requests across sideport (this SoC doesn't suport sideport).
#define XPB_CLG_CFG5_SIDE_FLUSH_OFFSET      14
#define XPB_CLG_CFG5_SIDE_FLUSH_MASK        0x3c000

// Bitfield Description : 
#define XPB_CLG_CFG5_Reserved_31_18_OFFSET      18
#define XPB_CLG_CFG5_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            WCB_NUM:4;
    UINT32                            LB_TYPE:3;
    UINT32                            P2P_BAR:3;
    UINT32                            HOST_FLUSH:4;
    UINT32                            SIDE_FLUSH:4;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} XPB_CLG_CFG5_STRUCT;

#define SMN_XPB_CLG_CFG5_ADDRESS    0x3224UL


/***********************************************************
* Register Name : XPB_CLG_CFG6
* Register Description :
* Each Client Group (CLG) is assigned to use specific resources (ie: wcb #, P2P BAR#, Host P2P Flush Counter #, Side P2P Flush Counter #), and with which type of load balancing to use when accessing the neighboring GPU's memenory. Reg(0-19) applies to: (all_others, cp, extra(0-1), cb(0-3), db(0-3), sx/tc(0-3), drmdma0(even,odd), drmdma1(even,odd)). <from XPB_defs.vh>
* Visibility : 0x2
************************************************************/

// Bitfield Description : indicates which Write Combining Buffer to use
#define XPB_CLG_CFG6_WCB_NUM_OFFSET      0
#define XPB_CLG_CFG6_WCB_NUM_MASK        0xf

// Bitfield Description : Indicates which type of load balancing (LB) to apply (This SoC doesn't suport LB).
#define XPB_CLG_CFG6_LB_TYPE_OFFSET      4
#define XPB_CLG_CFG6_LB_TYPE_MASK        0x70

// Bitfield Description : indicates which Host P2P BAR # to use to compress requests across the hostport
#define XPB_CLG_CFG6_P2P_BAR_OFFSET      7
#define XPB_CLG_CFG6_P2P_BAR_MASK        0x380

// Bitfield Description : indicates which Host P2P Flush Counter # to use for flushing/tracking requests across hostport, valid range is 0~7.
#define XPB_CLG_CFG6_HOST_FLUSH_OFFSET      10
#define XPB_CLG_CFG6_HOST_FLUSH_MASK        0x3c00

// Bitfield Description : Iindicates which Side P2P Flush Counter # to use for flushing/tracking requests across sideport (this SoC doesn't suport sideport).
#define XPB_CLG_CFG6_SIDE_FLUSH_OFFSET      14
#define XPB_CLG_CFG6_SIDE_FLUSH_MASK        0x3c000

// Bitfield Description : 
#define XPB_CLG_CFG6_Reserved_31_18_OFFSET      18
#define XPB_CLG_CFG6_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            WCB_NUM:4;
    UINT32                            LB_TYPE:3;
    UINT32                            P2P_BAR:3;
    UINT32                            HOST_FLUSH:4;
    UINT32                            SIDE_FLUSH:4;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} XPB_CLG_CFG6_STRUCT;

#define SMN_XPB_CLG_CFG6_ADDRESS    0x3228UL


/***********************************************************
* Register Name : XPB_CLG_CFG7
* Register Description :
* Each Client Group (CLG) is assigned to use specific resources (ie: wcb #, P2P BAR#, Host P2P Flush Counter #, Side P2P Flush Counter #), and with which type of load balancing to use when accessing the neighboring GPU's memenory. Reg(0-19) applies to: (all_others, cp, extra(0-1), cb(0-3), db(0-3), sx/tc(0-3), drmdma0(even,odd), drmdma1(even,odd)). <from XPB_defs.vh>
* Visibility : 0x2
************************************************************/

// Bitfield Description : indicates which Write Combining Buffer to use
#define XPB_CLG_CFG7_WCB_NUM_OFFSET      0
#define XPB_CLG_CFG7_WCB_NUM_MASK        0xf

// Bitfield Description : Indicates which type of load balancing (LB) to apply (This SoC doesn't suport LB).
#define XPB_CLG_CFG7_LB_TYPE_OFFSET      4
#define XPB_CLG_CFG7_LB_TYPE_MASK        0x70

// Bitfield Description : indicates which Host P2P BAR # to use to compress requests across the hostport
#define XPB_CLG_CFG7_P2P_BAR_OFFSET      7
#define XPB_CLG_CFG7_P2P_BAR_MASK        0x380

// Bitfield Description : indicates which Host P2P Flush Counter # to use for flushing/tracking requests across hostport, valid range is 0~7.
#define XPB_CLG_CFG7_HOST_FLUSH_OFFSET      10
#define XPB_CLG_CFG7_HOST_FLUSH_MASK        0x3c00

// Bitfield Description : Iindicates which Side P2P Flush Counter # to use for flushing/tracking requests across sideport (this SoC doesn't suport sideport).
#define XPB_CLG_CFG7_SIDE_FLUSH_OFFSET      14
#define XPB_CLG_CFG7_SIDE_FLUSH_MASK        0x3c000

// Bitfield Description : 
#define XPB_CLG_CFG7_Reserved_31_18_OFFSET      18
#define XPB_CLG_CFG7_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            WCB_NUM:4;
    UINT32                            LB_TYPE:3;
    UINT32                            P2P_BAR:3;
    UINT32                            HOST_FLUSH:4;
    UINT32                            SIDE_FLUSH:4;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} XPB_CLG_CFG7_STRUCT;

#define SMN_XPB_CLG_CFG7_ADDRESS    0x322cUL


/***********************************************************
* Register Name : XPB_CLG_EXTRA
* Register Description :
* This reg can be programmed with 2 additional UnitIDs to map to Client Groups, you could also specify the clg that you want to match it to.
* Visibility : 0x2
************************************************************/

// Bitfield Description : If incoming request UnitIDs high bits+low bits equals this value (after MSK0 bit masking) & if VLD0 is set, then client grp is set to clg0_num default set to (currently) unused UnitIDs
#define XPB_CLG_EXTRA_CMP0_HIGH_OFFSET      0
#define XPB_CLG_EXTRA_CMP0_HIGH_MASK        0x3f

// Bitfield Description : If incoming request UnitIDs high bits+low bits equals this value (after MSK0 bit masking) & if VLD0 is set, then client grp is set to clg0_num default set to (currently) unused UnitIDs
#define XPB_CLG_EXTRA_CMP0_LOW_OFFSET      6
#define XPB_CLG_EXTRA_CMP0_LOW_MASK        0x7c0

// Bitfield Description : Must be set for CMP0 to match.
#define XPB_CLG_EXTRA_VLD0_OFFSET      11
#define XPB_CLG_EXTRA_VLD0_MASK        0x800

// Bitfield Description : it tells the destination clg_num for unitid cmp0.
#define XPB_CLG_EXTRA_CLG0_NUM_OFFSET      12
#define XPB_CLG_EXTRA_CLG0_NUM_MASK        0x7000

// Bitfield Description : If incoming request UnitIDs high bits+low bits equals this value (after MSK1 bit masking) & if VLD1 is set, then client grp is set to clg1_num default set to (currently) unused UnitIDs
#define XPB_CLG_EXTRA_CMP1_HIGH_OFFSET      15
#define XPB_CLG_EXTRA_CMP1_HIGH_MASK        0x1f8000

// Bitfield Description : If incoming request UnitIDs high bits+low bits equals this value (after MSK1 bit masking) & if VLD1 is set, then client grp is set to clg1_num default set to (currently) unused UnitIDs
#define XPB_CLG_EXTRA_CMP1_LOW_OFFSET      21
#define XPB_CLG_EXTRA_CMP1_LOW_MASK        0x3e00000

// Bitfield Description : Must be set for CMP1 to match.
#define XPB_CLG_EXTRA_VLD1_OFFSET      26
#define XPB_CLG_EXTRA_VLD1_MASK        0x4000000

// Bitfield Description : Must be set for CMP1 to match.
#define XPB_CLG_EXTRA_CLG1_NUM_OFFSET      27
#define XPB_CLG_EXTRA_CLG1_NUM_MASK        0x38000000

// Bitfield Description : 
#define XPB_CLG_EXTRA_Reserved_31_30_OFFSET      30
#define XPB_CLG_EXTRA_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CMP0_HIGH:6;
    UINT32                            CMP0_LOW:5;
    UINT32                            VLD0:1;
    UINT32                            CLG0_NUM:3;
    UINT32                            CMP1_HIGH:6;
    UINT32                            CMP1_LOW:5;
    UINT32                            VLD1:1;
    UINT32                            CLG1_NUM:3;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} XPB_CLG_EXTRA_STRUCT;

#define SMN_XPB_CLG_EXTRA_ADDRESS    0x3230UL


/***********************************************************
* Register Name : XPB_CLG_EXTRA_MSK
* Register Description :
* This reg gives the msk feature for the unitid cmp0 and cmp1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit-wise mask of CMP0 high bits of UnitIDS matching.
#define XPB_CLG_EXTRA_MSK_MSK0_HIGH_OFFSET      0
#define XPB_CLG_EXTRA_MSK_MSK0_HIGH_MASK        0x3f

// Bitfield Description : Bit-wise mask of CMP0 low bits of UnitIDS matching.
#define XPB_CLG_EXTRA_MSK_MSK0_LOW_OFFSET      6
#define XPB_CLG_EXTRA_MSK_MSK0_LOW_MASK        0x7c0

// Bitfield Description : Bit-wise mask of CMP1 hight bits of UnitIDS matching.
#define XPB_CLG_EXTRA_MSK_MSK1_HIGH_OFFSET      11
#define XPB_CLG_EXTRA_MSK_MSK1_HIGH_MASK        0x1f800

// Bitfield Description : Bit-wise mask of CMP1 low bits of UnitIDS matching.
#define XPB_CLG_EXTRA_MSK_MSK1_LOW_OFFSET      17
#define XPB_CLG_EXTRA_MSK_MSK1_LOW_MASK        0x3e0000

// Bitfield Description : 
#define XPB_CLG_EXTRA_MSK_Reserved_31_22_OFFSET      22
#define XPB_CLG_EXTRA_MSK_Reserved_31_22_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            MSK0_HIGH:6;
    UINT32                            MSK0_LOW:5;
    UINT32                            MSK1_HIGH:6;
    UINT32                            MSK1_LOW:5;
    UINT32                            Reserved_31_22:10;
  } Field;
  UINT32 Value;
} XPB_CLG_EXTRA_MSK_STRUCT;

#define SMN_XPB_CLG_EXTRA_MSK_ADDRESS    0x3234UL


/***********************************************************
* Register Name : XPB_CLG_EXTRA_MSK_RD
* Register Description :
* This reg gives the msk feature for the unitid cmp0 and cmp1.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Bit-wise mask of CMP0 high bits of UnitIDS matching.
#define XPB_CLG_EXTRA_MSK_RD_MSK0_HIGH_OFFSET      0
#define XPB_CLG_EXTRA_MSK_RD_MSK0_HIGH_MASK        0x3f

// Bitfield Description : Bit-wise mask of CMP0 low bits of UnitIDS matching.
#define XPB_CLG_EXTRA_MSK_RD_MSK0_LOW_OFFSET      6
#define XPB_CLG_EXTRA_MSK_RD_MSK0_LOW_MASK        0x7c0

// Bitfield Description : Bit-wise mask of CMP1 hight bits of UnitIDS matching.
#define XPB_CLG_EXTRA_MSK_RD_MSK1_HIGH_OFFSET      11
#define XPB_CLG_EXTRA_MSK_RD_MSK1_HIGH_MASK        0x1f800

// Bitfield Description : Bit-wise mask of CMP1 low bits of UnitIDS matching.
#define XPB_CLG_EXTRA_MSK_RD_MSK1_LOW_OFFSET      17
#define XPB_CLG_EXTRA_MSK_RD_MSK1_LOW_MASK        0x3e0000

// Bitfield Description : 
#define XPB_CLG_EXTRA_MSK_RD_Reserved_31_22_OFFSET      22
#define XPB_CLG_EXTRA_MSK_RD_Reserved_31_22_MASK        0xffc00000

typedef union {
  struct {
    UINT32                            MSK0_HIGH:6;
    UINT32                            MSK0_LOW:5;
    UINT32                            MSK1_HIGH:6;
    UINT32                            MSK1_LOW:5;
    UINT32                            Reserved_31_22:10;
  } Field;
  UINT32 Value;
} XPB_CLG_EXTRA_MSK_RD_STRUCT;

#define SMN_XPB_CLG_EXTRA_MSK_RD_ADDRESS    0x32e0UL


/***********************************************************
* Register Name : XPB_CLG_EXTRA_RD
* Register Description :
* This reg can be programmed with 2 additional UnitIDs to map to Client Groups, you could also specify the clg that you want to match it to.
* Visibility : 0x2
************************************************************/

// Bitfield Description : If incoming request UnitIDs high bits+low bits equals this value (after MSK0 bit masking) & if VLD0 is set, then client grp is set to clg0_num default set to (currently) unused UnitIDs
#define XPB_CLG_EXTRA_RD_CMP0_HIGH_OFFSET      0
#define XPB_CLG_EXTRA_RD_CMP0_HIGH_MASK        0x3f

// Bitfield Description : If incoming request UnitIDs high bits+low bits equals this value (after MSK0 bit masking) & if VLD0 is set, then client grp is set to clg0_num default set to (currently) unused UnitIDs
#define XPB_CLG_EXTRA_RD_CMP0_LOW_OFFSET      6
#define XPB_CLG_EXTRA_RD_CMP0_LOW_MASK        0x7c0

// Bitfield Description : Must be set for CMP0 to match.
#define XPB_CLG_EXTRA_RD_VLD0_OFFSET      11
#define XPB_CLG_EXTRA_RD_VLD0_MASK        0x800

// Bitfield Description : it tells the destination clg_num for unitid cmp0.
#define XPB_CLG_EXTRA_RD_CLG0_NUM_OFFSET      12
#define XPB_CLG_EXTRA_RD_CLG0_NUM_MASK        0x7000

// Bitfield Description : If incoming request UnitIDs high bits+low bits equals this value (after MSK1 bit masking) & if VLD1 is set, then client grp is set to clg1_num default set to (currently) unused UnitIDs
#define XPB_CLG_EXTRA_RD_CMP1_HIGH_OFFSET      15
#define XPB_CLG_EXTRA_RD_CMP1_HIGH_MASK        0x1f8000

// Bitfield Description : If incoming request UnitIDs high bits+low bits equals this value (after MSK1 bit masking) & if VLD1 is set, then client grp is set to clg1_num default set to (currently) unused UnitIDs
#define XPB_CLG_EXTRA_RD_CMP1_LOW_OFFSET      21
#define XPB_CLG_EXTRA_RD_CMP1_LOW_MASK        0x3e00000

// Bitfield Description : Must be set for CMP1 to match.
#define XPB_CLG_EXTRA_RD_VLD1_OFFSET      26
#define XPB_CLG_EXTRA_RD_VLD1_MASK        0x4000000

// Bitfield Description : Must be set for CMP1 to match.
#define XPB_CLG_EXTRA_RD_CLG1_NUM_OFFSET      27
#define XPB_CLG_EXTRA_RD_CLG1_NUM_MASK        0x38000000

// Bitfield Description : 
#define XPB_CLG_EXTRA_RD_Reserved_31_30_OFFSET      30
#define XPB_CLG_EXTRA_RD_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            CMP0_HIGH:6;
    UINT32                            CMP0_LOW:5;
    UINT32                            VLD0:1;
    UINT32                            CLG0_NUM:3;
    UINT32                            CMP1_HIGH:6;
    UINT32                            CMP1_LOW:5;
    UINT32                            VLD1:1;
    UINT32                            CLG1_NUM:3;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} XPB_CLG_EXTRA_RD_STRUCT;

#define SMN_XPB_CLG_EXTRA_RD_ADDRESS    0x32dcUL


/***********************************************************
* Register Name : XPB_CLG_GFX_MATCH
* Register Description :
* This register tells the source sdp port type, it's gfx's sdp request
* Visibility : 0x2
************************************************************/

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_GFX_MATCH_FARBIRC0_ID_OFFSET      0
#define XPB_CLG_GFX_MATCH_FARBIRC0_ID_MASK        0x3f

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_GFX_MATCH_FARBIRC1_ID_OFFSET      6
#define XPB_CLG_GFX_MATCH_FARBIRC1_ID_MASK        0xfc0

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_GFX_MATCH_FARBIRC2_ID_OFFSET      12
#define XPB_CLG_GFX_MATCH_FARBIRC2_ID_MASK        0x3f000

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_GFX_MATCH_FARBIRC3_ID_OFFSET      18
#define XPB_CLG_GFX_MATCH_FARBIRC3_ID_MASK        0xfc0000

// Bitfield Description : if the farbirc0_id is valid.
#define XPB_CLG_GFX_MATCH_FARBIRC0_VLD_OFFSET      24
#define XPB_CLG_GFX_MATCH_FARBIRC0_VLD_MASK        0x1000000

// Bitfield Description : if the farbirc0_id is valid.
#define XPB_CLG_GFX_MATCH_FARBIRC1_VLD_OFFSET      25
#define XPB_CLG_GFX_MATCH_FARBIRC1_VLD_MASK        0x2000000

// Bitfield Description : if the farbirc0_id is valid.
#define XPB_CLG_GFX_MATCH_FARBIRC2_VLD_OFFSET      26
#define XPB_CLG_GFX_MATCH_FARBIRC2_VLD_MASK        0x4000000

// Bitfield Description : if the farbirc0_id is valid.
#define XPB_CLG_GFX_MATCH_FARBIRC3_VLD_OFFSET      27
#define XPB_CLG_GFX_MATCH_FARBIRC3_VLD_MASK        0x8000000

// Bitfield Description : 
#define XPB_CLG_GFX_MATCH_Reserved_31_28_OFFSET      28
#define XPB_CLG_GFX_MATCH_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            FARBIRC0_ID:6;
    UINT32                            FARBIRC1_ID:6;
    UINT32                            FARBIRC2_ID:6;
    UINT32                            FARBIRC3_ID:6;
    UINT32                            FARBIRC0_VLD:1;
    UINT32                            FARBIRC1_VLD:1;
    UINT32                            FARBIRC2_VLD:1;
    UINT32                            FARBIRC3_VLD:1;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} XPB_CLG_GFX_MATCH_STRUCT;

#define SMN_XPB_CLG_GFX_MATCH_ADDRESS    0x32e4UL


/***********************************************************
* Register Name : XPB_CLG_GFX_MATCH_MSK
* Register Description :
* This register used to mask the GFX slice#, for example, it need be 0xf on navi10.
* Visibility : 0x2
************************************************************/

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_GFX_MATCH_MSK_FARBIRC0_ID_MSK_OFFSET      0
#define XPB_CLG_GFX_MATCH_MSK_FARBIRC0_ID_MSK_MASK        0x3f

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_GFX_MATCH_MSK_FARBIRC1_ID_MSK_OFFSET      6
#define XPB_CLG_GFX_MATCH_MSK_FARBIRC1_ID_MSK_MASK        0xfc0

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_GFX_MATCH_MSK_FARBIRC2_ID_MSK_OFFSET      12
#define XPB_CLG_GFX_MATCH_MSK_FARBIRC2_ID_MSK_MASK        0x3f000

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_GFX_MATCH_MSK_FARBIRC3_ID_MSK_OFFSET      18
#define XPB_CLG_GFX_MATCH_MSK_FARBIRC3_ID_MSK_MASK        0xfc0000

// Bitfield Description : 
#define XPB_CLG_GFX_MATCH_MSK_Reserved_31_24_OFFSET      24
#define XPB_CLG_GFX_MATCH_MSK_Reserved_31_24_MASK        0xff000000

typedef union {
  struct {
    UINT32                            FARBIRC0_ID_MSK:6;
    UINT32                            FARBIRC1_ID_MSK:6;
    UINT32                            FARBIRC2_ID_MSK:6;
    UINT32                            FARBIRC3_ID_MSK:6;
    UINT32                            Reserved_31_24:8;
  } Field;
  UINT32 Value;
} XPB_CLG_GFX_MATCH_MSK_STRUCT;

#define SMN_XPB_CLG_GFX_MATCH_MSK_ADDRESS    0x32e8UL


/***********************************************************
* Register Name : XPB_CLG_GFX_UNITID_MAPPING0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING0_UNITID_LOW_OFFSET      0
#define XPB_CLG_GFX_UNITID_MAPPING0_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING0_UNITID_VLD_OFFSET      5
#define XPB_CLG_GFX_UNITID_MAPPING0_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING0_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GFX_UNITID_MAPPING0_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING0_Reserved_31_9_OFFSET      9
#define XPB_CLG_GFX_UNITID_MAPPING0_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GFX_UNITID_MAPPING0_STRUCT;

#define SMN_XPB_CLG_GFX_UNITID_MAPPING0_ADDRESS    0x32fcUL


/***********************************************************
* Register Name : XPB_CLG_GFX_UNITID_MAPPING1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING1_UNITID_LOW_OFFSET      0
#define XPB_CLG_GFX_UNITID_MAPPING1_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING1_UNITID_VLD_OFFSET      5
#define XPB_CLG_GFX_UNITID_MAPPING1_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING1_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GFX_UNITID_MAPPING1_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING1_Reserved_31_9_OFFSET      9
#define XPB_CLG_GFX_UNITID_MAPPING1_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GFX_UNITID_MAPPING1_STRUCT;

#define SMN_XPB_CLG_GFX_UNITID_MAPPING1_ADDRESS    0x3300UL


/***********************************************************
* Register Name : XPB_CLG_GFX_UNITID_MAPPING2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING2_UNITID_LOW_OFFSET      0
#define XPB_CLG_GFX_UNITID_MAPPING2_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING2_UNITID_VLD_OFFSET      5
#define XPB_CLG_GFX_UNITID_MAPPING2_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING2_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GFX_UNITID_MAPPING2_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING2_Reserved_31_9_OFFSET      9
#define XPB_CLG_GFX_UNITID_MAPPING2_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GFX_UNITID_MAPPING2_STRUCT;

#define SMN_XPB_CLG_GFX_UNITID_MAPPING2_ADDRESS    0x3304UL


/***********************************************************
* Register Name : XPB_CLG_GFX_UNITID_MAPPING3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING3_UNITID_LOW_OFFSET      0
#define XPB_CLG_GFX_UNITID_MAPPING3_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING3_UNITID_VLD_OFFSET      5
#define XPB_CLG_GFX_UNITID_MAPPING3_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING3_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GFX_UNITID_MAPPING3_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING3_Reserved_31_9_OFFSET      9
#define XPB_CLG_GFX_UNITID_MAPPING3_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GFX_UNITID_MAPPING3_STRUCT;

#define SMN_XPB_CLG_GFX_UNITID_MAPPING3_ADDRESS    0x3308UL


/***********************************************************
* Register Name : XPB_CLG_GFX_UNITID_MAPPING4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING4_UNITID_LOW_OFFSET      0
#define XPB_CLG_GFX_UNITID_MAPPING4_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING4_UNITID_VLD_OFFSET      5
#define XPB_CLG_GFX_UNITID_MAPPING4_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING4_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GFX_UNITID_MAPPING4_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING4_Reserved_31_9_OFFSET      9
#define XPB_CLG_GFX_UNITID_MAPPING4_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GFX_UNITID_MAPPING4_STRUCT;

#define SMN_XPB_CLG_GFX_UNITID_MAPPING4_ADDRESS    0x330cUL


/***********************************************************
* Register Name : XPB_CLG_GFX_UNITID_MAPPING5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING5_UNITID_LOW_OFFSET      0
#define XPB_CLG_GFX_UNITID_MAPPING5_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING5_UNITID_VLD_OFFSET      5
#define XPB_CLG_GFX_UNITID_MAPPING5_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING5_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GFX_UNITID_MAPPING5_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING5_Reserved_31_9_OFFSET      9
#define XPB_CLG_GFX_UNITID_MAPPING5_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GFX_UNITID_MAPPING5_STRUCT;

#define SMN_XPB_CLG_GFX_UNITID_MAPPING5_ADDRESS    0x3310UL


/***********************************************************
* Register Name : XPB_CLG_GFX_UNITID_MAPPING6
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING6_UNITID_LOW_OFFSET      0
#define XPB_CLG_GFX_UNITID_MAPPING6_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING6_UNITID_VLD_OFFSET      5
#define XPB_CLG_GFX_UNITID_MAPPING6_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING6_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GFX_UNITID_MAPPING6_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING6_Reserved_31_9_OFFSET      9
#define XPB_CLG_GFX_UNITID_MAPPING6_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GFX_UNITID_MAPPING6_STRUCT;

#define SMN_XPB_CLG_GFX_UNITID_MAPPING6_ADDRESS    0x3314UL


/***********************************************************
* Register Name : XPB_CLG_GFX_UNITID_MAPPING7
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING7_UNITID_LOW_OFFSET      0
#define XPB_CLG_GFX_UNITID_MAPPING7_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING7_UNITID_VLD_OFFSET      5
#define XPB_CLG_GFX_UNITID_MAPPING7_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING7_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GFX_UNITID_MAPPING7_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GFX_UNITID_MAPPING7_Reserved_31_9_OFFSET      9
#define XPB_CLG_GFX_UNITID_MAPPING7_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GFX_UNITID_MAPPING7_STRUCT;

#define SMN_XPB_CLG_GFX_UNITID_MAPPING7_ADDRESS    0x3318UL


/***********************************************************
* Register Name : XPB_CLG_GUS_MATCH
* Register Description :
* This register tells the source sdp port type, it's GUS's sdp request
* Visibility : 0x2
************************************************************/

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_GUS_MATCH_FARBIRC0_ID_OFFSET      0
#define XPB_CLG_GUS_MATCH_FARBIRC0_ID_MASK        0x3f

// Bitfield Description : if the farbirc0_id is valid.
#define XPB_CLG_GUS_MATCH_FARBIRC0_VLD_OFFSET      6
#define XPB_CLG_GUS_MATCH_FARBIRC0_VLD_MASK        0x40

// Bitfield Description : 
#define XPB_CLG_GUS_MATCH_Reserved_31_7_OFFSET      7
#define XPB_CLG_GUS_MATCH_Reserved_31_7_MASK        0xffffff80

typedef union {
  struct {
    UINT32                            FARBIRC0_ID:6;
    UINT32                            FARBIRC0_VLD:1;
    UINT32                            Reserved_31_7:25;
  } Field;
  UINT32 Value;
} XPB_CLG_GUS_MATCH_STRUCT;

#define SMN_XPB_CLG_GUS_MATCH_ADDRESS    0x32f4UL


/***********************************************************
* Register Name : XPB_CLG_GUS_MATCH_MSK
* Register Description :
* This register tells the source sdp port type, it's GUS's sdp request
* Visibility : 0x2
************************************************************/

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_GUS_MATCH_MSK_FARBIRC0_ID_MSK_OFFSET      0
#define XPB_CLG_GUS_MATCH_MSK_FARBIRC0_ID_MSK_MASK        0x3f

// Bitfield Description : 
#define XPB_CLG_GUS_MATCH_MSK_Reserved_31_6_OFFSET      6
#define XPB_CLG_GUS_MATCH_MSK_Reserved_31_6_MASK        0xffffffc0

typedef union {
  struct {
    UINT32                            FARBIRC0_ID_MSK:6;
    UINT32                            Reserved_31_6:26;
  } Field;
  UINT32 Value;
} XPB_CLG_GUS_MATCH_MSK_STRUCT;

#define SMN_XPB_CLG_GUS_MATCH_MSK_ADDRESS    0x32f8UL


/***********************************************************
* Register Name : XPB_CLG_GUS_UNITID_MAPPING0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING0_UNITID_LOW_OFFSET      0
#define XPB_CLG_GUS_UNITID_MAPPING0_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING0_UNITID_VLD_OFFSET      5
#define XPB_CLG_GUS_UNITID_MAPPING0_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING0_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GUS_UNITID_MAPPING0_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING0_Reserved_31_9_OFFSET      9
#define XPB_CLG_GUS_UNITID_MAPPING0_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GUS_UNITID_MAPPING0_STRUCT;

#define SMN_XPB_CLG_GUS_UNITID_MAPPING0_ADDRESS    0x332cUL


/***********************************************************
* Register Name : XPB_CLG_GUS_UNITID_MAPPING1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING1_UNITID_LOW_OFFSET      0
#define XPB_CLG_GUS_UNITID_MAPPING1_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING1_UNITID_VLD_OFFSET      5
#define XPB_CLG_GUS_UNITID_MAPPING1_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING1_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GUS_UNITID_MAPPING1_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING1_Reserved_31_9_OFFSET      9
#define XPB_CLG_GUS_UNITID_MAPPING1_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GUS_UNITID_MAPPING1_STRUCT;

#define SMN_XPB_CLG_GUS_UNITID_MAPPING1_ADDRESS    0x3330UL


/***********************************************************
* Register Name : XPB_CLG_GUS_UNITID_MAPPING2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING2_UNITID_LOW_OFFSET      0
#define XPB_CLG_GUS_UNITID_MAPPING2_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING2_UNITID_VLD_OFFSET      5
#define XPB_CLG_GUS_UNITID_MAPPING2_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING2_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GUS_UNITID_MAPPING2_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING2_Reserved_31_9_OFFSET      9
#define XPB_CLG_GUS_UNITID_MAPPING2_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GUS_UNITID_MAPPING2_STRUCT;

#define SMN_XPB_CLG_GUS_UNITID_MAPPING2_ADDRESS    0x3334UL


/***********************************************************
* Register Name : XPB_CLG_GUS_UNITID_MAPPING3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING3_UNITID_LOW_OFFSET      0
#define XPB_CLG_GUS_UNITID_MAPPING3_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING3_UNITID_VLD_OFFSET      5
#define XPB_CLG_GUS_UNITID_MAPPING3_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING3_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GUS_UNITID_MAPPING3_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING3_Reserved_31_9_OFFSET      9
#define XPB_CLG_GUS_UNITID_MAPPING3_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GUS_UNITID_MAPPING3_STRUCT;

#define SMN_XPB_CLG_GUS_UNITID_MAPPING3_ADDRESS    0x3338UL


/***********************************************************
* Register Name : XPB_CLG_GUS_UNITID_MAPPING4
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING4_UNITID_LOW_OFFSET      0
#define XPB_CLG_GUS_UNITID_MAPPING4_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING4_UNITID_VLD_OFFSET      5
#define XPB_CLG_GUS_UNITID_MAPPING4_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING4_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GUS_UNITID_MAPPING4_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING4_Reserved_31_9_OFFSET      9
#define XPB_CLG_GUS_UNITID_MAPPING4_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GUS_UNITID_MAPPING4_STRUCT;

#define SMN_XPB_CLG_GUS_UNITID_MAPPING4_ADDRESS    0x333cUL


/***********************************************************
* Register Name : XPB_CLG_GUS_UNITID_MAPPING5
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING5_UNITID_LOW_OFFSET      0
#define XPB_CLG_GUS_UNITID_MAPPING5_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING5_UNITID_VLD_OFFSET      5
#define XPB_CLG_GUS_UNITID_MAPPING5_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING5_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GUS_UNITID_MAPPING5_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING5_Reserved_31_9_OFFSET      9
#define XPB_CLG_GUS_UNITID_MAPPING5_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GUS_UNITID_MAPPING5_STRUCT;

#define SMN_XPB_CLG_GUS_UNITID_MAPPING5_ADDRESS    0x3340UL


/***********************************************************
* Register Name : XPB_CLG_GUS_UNITID_MAPPING6
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING6_UNITID_LOW_OFFSET      0
#define XPB_CLG_GUS_UNITID_MAPPING6_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING6_UNITID_VLD_OFFSET      5
#define XPB_CLG_GUS_UNITID_MAPPING6_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING6_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GUS_UNITID_MAPPING6_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING6_Reserved_31_9_OFFSET      9
#define XPB_CLG_GUS_UNITID_MAPPING6_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GUS_UNITID_MAPPING6_STRUCT;

#define SMN_XPB_CLG_GUS_UNITID_MAPPING6_ADDRESS    0x3344UL


/***********************************************************
* Register Name : XPB_CLG_GUS_UNITID_MAPPING7
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING7_UNITID_LOW_OFFSET      0
#define XPB_CLG_GUS_UNITID_MAPPING7_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING7_UNITID_VLD_OFFSET      5
#define XPB_CLG_GUS_UNITID_MAPPING7_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING7_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_GUS_UNITID_MAPPING7_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_GUS_UNITID_MAPPING7_Reserved_31_9_OFFSET      9
#define XPB_CLG_GUS_UNITID_MAPPING7_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_GUS_UNITID_MAPPING7_STRUCT;

#define SMN_XPB_CLG_GUS_UNITID_MAPPING7_ADDRESS    0x3348UL


/***********************************************************
* Register Name : XPB_CLG_MM_MATCH
* Register Description :
* This register tells the source sdp port type, it's mmhub's sdp request
* Visibility : 0x2
************************************************************/

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_MM_MATCH_FARBIRC0_ID_OFFSET      0
#define XPB_CLG_MM_MATCH_FARBIRC0_ID_MASK        0x3f

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_MM_MATCH_FARBIRC1_ID_OFFSET      6
#define XPB_CLG_MM_MATCH_FARBIRC1_ID_MASK        0xfc0

// Bitfield Description : if the farbirc0_id is valid.
#define XPB_CLG_MM_MATCH_FARBIRC0_VLD_OFFSET      12
#define XPB_CLG_MM_MATCH_FARBIRC0_VLD_MASK        0x1000

// Bitfield Description : if the farbirc1_id is valid.
#define XPB_CLG_MM_MATCH_FARBIRC1_VLD_OFFSET      13
#define XPB_CLG_MM_MATCH_FARBIRC1_VLD_MASK        0x2000

// Bitfield Description : 
#define XPB_CLG_MM_MATCH_Reserved_31_14_OFFSET      14
#define XPB_CLG_MM_MATCH_Reserved_31_14_MASK        0xffffc000

typedef union {
  struct {
    UINT32                            FARBIRC0_ID:6;
    UINT32                            FARBIRC1_ID:6;
    UINT32                            FARBIRC0_VLD:1;
    UINT32                            FARBIRC1_VLD:1;
    UINT32                            Reserved_31_14:18;
  } Field;
  UINT32 Value;
} XPB_CLG_MM_MATCH_STRUCT;

#define SMN_XPB_CLG_MM_MATCH_ADDRESS    0x32ecUL


/***********************************************************
* Register Name : XPB_CLG_MM_MATCH_MSK
* Register Description :
* This register tells the source sdp port type, it's mmhub's sdp request
* Visibility : 0x2
************************************************************/

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_MM_MATCH_MSK_FARBIRC0_ID_MSK_OFFSET      0
#define XPB_CLG_MM_MATCH_MSK_FARBIRC0_ID_MSK_MASK        0x3f

// Bitfield Description : high 6 bits for the unitid.
#define XPB_CLG_MM_MATCH_MSK_FARBIRC1_ID_MSK_OFFSET      6
#define XPB_CLG_MM_MATCH_MSK_FARBIRC1_ID_MSK_MASK        0xfc0

// Bitfield Description : 
#define XPB_CLG_MM_MATCH_MSK_Reserved_31_12_OFFSET      12
#define XPB_CLG_MM_MATCH_MSK_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            FARBIRC0_ID_MSK:6;
    UINT32                            FARBIRC1_ID_MSK:6;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} XPB_CLG_MM_MATCH_MSK_STRUCT;

#define SMN_XPB_CLG_MM_MATCH_MSK_ADDRESS    0x32f0UL


/***********************************************************
* Register Name : XPB_CLG_MM_UNITID_MAPPING0
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING0_UNITID_LOW_OFFSET      0
#define XPB_CLG_MM_UNITID_MAPPING0_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING0_UNITID_VLD_OFFSET      5
#define XPB_CLG_MM_UNITID_MAPPING0_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING0_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_MM_UNITID_MAPPING0_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING0_Reserved_31_9_OFFSET      9
#define XPB_CLG_MM_UNITID_MAPPING0_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_MM_UNITID_MAPPING0_STRUCT;

#define SMN_XPB_CLG_MM_UNITID_MAPPING0_ADDRESS    0x331cUL


/***********************************************************
* Register Name : XPB_CLG_MM_UNITID_MAPPING1
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING1_UNITID_LOW_OFFSET      0
#define XPB_CLG_MM_UNITID_MAPPING1_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING1_UNITID_VLD_OFFSET      5
#define XPB_CLG_MM_UNITID_MAPPING1_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING1_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_MM_UNITID_MAPPING1_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING1_Reserved_31_9_OFFSET      9
#define XPB_CLG_MM_UNITID_MAPPING1_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_MM_UNITID_MAPPING1_STRUCT;

#define SMN_XPB_CLG_MM_UNITID_MAPPING1_ADDRESS    0x3320UL


/***********************************************************
* Register Name : XPB_CLG_MM_UNITID_MAPPING2
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING2_UNITID_LOW_OFFSET      0
#define XPB_CLG_MM_UNITID_MAPPING2_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING2_UNITID_VLD_OFFSET      5
#define XPB_CLG_MM_UNITID_MAPPING2_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING2_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_MM_UNITID_MAPPING2_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING2_Reserved_31_9_OFFSET      9
#define XPB_CLG_MM_UNITID_MAPPING2_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_MM_UNITID_MAPPING2_STRUCT;

#define SMN_XPB_CLG_MM_UNITID_MAPPING2_ADDRESS    0x3324UL


/***********************************************************
* Register Name : XPB_CLG_MM_UNITID_MAPPING3
* Register Description :
* 
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING3_UNITID_LOW_OFFSET      0
#define XPB_CLG_MM_UNITID_MAPPING3_UNITID_LOW_MASK        0x1f

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING3_UNITID_VLD_OFFSET      5
#define XPB_CLG_MM_UNITID_MAPPING3_UNITID_VLD_MASK        0x20

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING3_DEST_CLG_NUM_OFFSET      6
#define XPB_CLG_MM_UNITID_MAPPING3_DEST_CLG_NUM_MASK        0x1c0

// Bitfield Description : 
#define XPB_CLG_MM_UNITID_MAPPING3_Reserved_31_9_OFFSET      9
#define XPB_CLG_MM_UNITID_MAPPING3_Reserved_31_9_MASK        0xfffffe00

typedef union {
  struct {
    UINT32                            UNITID_LOW:5;
    UINT32                            UNITID_VLD:1;
    UINT32                            DEST_CLG_NUM:3;
    UINT32                            Reserved_31_9:23;
  } Field;
  UINT32 Value;
} XPB_CLG_MM_UNITID_MAPPING3_STRUCT;

#define SMN_XPB_CLG_MM_UNITID_MAPPING3_ADDRESS    0x3328UL


/***********************************************************
* Register Name : XPB_CLK_GAT
* Register Description :
* RESERVED for debug: control for xpb domain clock gate#IMPR? Not used and we should remove it later?
* Visibility : 0x2
************************************************************/

// Bitfield Description : delay in clock cycles (+1) to turn on the clock after requested when LS disable; delay in clock cycles (+1) to turn off LS after requested when LS enable
#define XPB_CLK_GAT_ONDLY_OFFSET      0
#define XPB_CLK_GAT_ONDLY_MASK        0x3f

// Bitfield Description : delay in clock cycles to turn off clock after busy goes low
#define XPB_CLK_GAT_OFFDLY_OFFSET      6
#define XPB_CLK_GAT_OFFDLY_MASK        0xfc0

// Bitfield Description : delay in clock cycles (+1) to turn on rdy after the clock starts
#define XPB_CLK_GAT_RDYDLY_OFFSET      12
#define XPB_CLK_GAT_RDYDLY_MASK        0x3f000

// Bitfield Description : 
#define XPB_CLK_GAT_ENABLE_OFFSET      18
#define XPB_CLK_GAT_ENABLE_MASK        0x40000

// Bitfield Description : 
#define XPB_CLK_GAT_MEM_LS_ENABLE_OFFSET      19
#define XPB_CLK_GAT_MEM_LS_ENABLE_MASK        0x80000

// Bitfield Description : 
#define XPB_CLK_GAT_Reserved_31_20_OFFSET      20
#define XPB_CLK_GAT_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            ONDLY:6;
    UINT32                            OFFDLY:6;
    UINT32                            RDYDLY:6;
    UINT32                            ENABLE:1;
    UINT32                            MEM_LS_ENABLE:1;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} XPB_CLK_GAT_STRUCT;

#define SMN_XPB_CLK_GAT_ADDRESS    0x32b0UL


/***********************************************************
* Register Name : XPB_HST_CFG
* Register Description :
* This configures misc options for xpb
* Visibility : 0x2
************************************************************/

// Bitfield Description : use which wr cmd for p2p bar update write.
#define XPB_HST_CFG_BAR_UP_WR_CMD_OFFSET      0
#define XPB_HST_CFG_BAR_UP_WR_CMD_MASK        0x1

// Bitfield Description : 
#define XPB_HST_CFG_Reserved_31_1_OFFSET      1
#define XPB_HST_CFG_Reserved_31_1_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            BAR_UP_WR_CMD:1;
    UINT32                            Reserved_31_1:31;
  } Field;
  UINT32 Value;
} XPB_HST_CFG_STRUCT;

#define SMN_XPB_HST_CFG_ADDRESS    0x3240UL


/***********************************************************
* Register Name : XPB_INTF_CFG
* Register Description :
* configurable thresholds for XPB interfaces
* Visibility : 0x2
************************************************************/

// Bitfield Description : max # of credits available to use on intf for sending wrreq to rpb
#define XPB_INTF_CFG_RPB_WRREQ_CRD_OFFSET      0
#define XPB_INTF_CFG_RPB_WRREQ_CRD_MASK        0xff

// Bitfield Description : max # of outstanding asks that can be sent to MC for wrret intf
#define XPB_INTF_CFG_MC_WRRET_ASK_OFFSET      8
#define XPB_INTF_CFG_MC_WRRET_ASK_MASK        0xff00

// Bitfield Description : 1 less than max # of credits available to use on intf for sending req to xsp (eg: value of 7 means 8 credits)
#define XPB_INTF_CFG_XSP_REQ_CRD_OFFSET      16
#define XPB_INTF_CFG_XSP_REQ_CRD_MASK        0x7f0000

// Bitfield Description : Override enable - If P2P_WR_CHAIN_BREAK==1, then all of the value about chain will be set to 0.
#define XPB_INTF_CFG_P2P_WR_CHAIN_BREAK_OFFSET      23
#define XPB_INTF_CFG_P2P_WR_CHAIN_BREAK_MASK        0x800000

// Bitfield Description : 
#define XPB_INTF_CFG_Reserved_26_24_OFFSET      24
#define XPB_INTF_CFG_Reserved_26_24_MASK        0x7000000

// Bitfield Description : This bit is used to select what is used for 'snoop' bit in requests sent to XSP.
#define XPB_INTF_CFG_XSP_SNOOP_SEL_OFFSET      27
#define XPB_INTF_CFG_XSP_SNOOP_SEL_MASK        0x18000000

// Bitfield Description : This bit is used if XSP_SNOOP_SEL is set to use it. See XSP_SNOOP_SEL description.
#define XPB_INTF_CFG_XSP_SNOOP_VAL_OFFSET      29
#define XPB_INTF_CFG_XSP_SNOOP_VAL_MASK        0x20000000

// Bitfield Description : This bit is used to select what is used for 'ordering' bit in requests sent to XSP.
#define XPB_INTF_CFG_XSP_ORDERING_SEL_OFFSET      30
#define XPB_INTF_CFG_XSP_ORDERING_SEL_MASK        0x40000000

// Bitfield Description : This bit is used if XSP_ORDERING_SEL is set to use it. See XSP_ORDERING_SEL description.
#define XPB_INTF_CFG_XSP_ORDERING_VAL_OFFSET      31
#define XPB_INTF_CFG_XSP_ORDERING_VAL_MASK        0x80000000

typedef union {
  struct {
    UINT32                            RPB_WRREQ_CRD:8;
    UINT32                            MC_WRRET_ASK:8;
    UINT32                            XSP_REQ_CRD:7;
    UINT32                            P2P_WR_CHAIN_BREAK:1;
    UINT32                            Reserved_26_24:3;
    UINT32                            XSP_SNOOP_SEL:2;
    UINT32                            XSP_SNOOP_VAL:1;
    UINT32                            XSP_ORDERING_SEL:1;
    UINT32                            XSP_ORDERING_VAL:1;
  } Field;
  UINT32 Value;
} XPB_INTF_CFG_STRUCT;

#define SMN_XPB_INTF_CFG_ADDRESS    0x32b4UL


/***********************************************************
* Register Name : XPB_INTF_CFG2
* Register Description :
* configurable thresholds for XPB interfaces
* Visibility : 0x2
************************************************************/

// Bitfield Description : max # of credits available to use on intf for sending rdreq to rpb
#define XPB_INTF_CFG2_RPB_RDREQ_CRD_OFFSET      0
#define XPB_INTF_CFG2_RPB_RDREQ_CRD_MASK        0xff

// Bitfield Description : 
#define XPB_INTF_CFG2_Reserved_31_8_OFFSET      8
#define XPB_INTF_CFG2_Reserved_31_8_MASK        0xffffff00

typedef union {
  struct {
    UINT32                            RPB_RDREQ_CRD:8;
    UINT32                            Reserved_31_8:24;
  } Field;
  UINT32 Value;
} XPB_INTF_CFG2_STRUCT;

#define SMN_XPB_INTF_CFG2_ADDRESS    0x32d8UL


/***********************************************************
* Register Name : XPB_INTF_STS
* Register Description :
* RESERVED for debug: current credit status for XPB interfaces
* Visibility : 0x2
************************************************************/

// Bitfield Description : # of credits in use on intf for sending wrreq to rpb
#define XPB_INTF_STS_RPB_WRREQ_CRD_OFFSET      0
#define XPB_INTF_STS_RPB_WRREQ_CRD_MASK        0xff

// Bitfield Description : # of credits in use on intf for sending req to xsp
#define XPB_INTF_STS_XSP_REQ_CRD_OFFSET      8
#define XPB_INTF_STS_XSP_REQ_CRD_MASK        0x7f00

// Bitfield Description : This displays whether this buffer is full
#define XPB_INTF_STS_HOP_DATA_BUF_FULL_OFFSET      15
#define XPB_INTF_STS_HOP_DATA_BUF_FULL_MASK        0x8000

// Bitfield Description : This displays whether this buffer is full
#define XPB_INTF_STS_HOP_ATTR_BUF_FULL_OFFSET      16
#define XPB_INTF_STS_HOP_ATTR_BUF_FULL_MASK        0x10000

// Bitfield Description : This displays whether this buffer is full
#define XPB_INTF_STS_CNS_BUF_FULL_OFFSET      17
#define XPB_INTF_STS_CNS_BUF_FULL_MASK        0x20000

// Bitfield Description : This displays whether this module is busy (!empty)
#define XPB_INTF_STS_CNS_BUF_BUSY_OFFSET      18
#define XPB_INTF_STS_CNS_BUF_BUSY_MASK        0x40000

// Bitfield Description : # of credits in use on intf for sending wrreq to rpb
#define XPB_INTF_STS_RPB_RDREQ_CRD_OFFSET      19
#define XPB_INTF_STS_RPB_RDREQ_CRD_MASK        0x7f80000

// Bitfield Description : 
#define XPB_INTF_STS_Reserved_31_27_OFFSET      27
#define XPB_INTF_STS_Reserved_31_27_MASK        0xf8000000

typedef union {
  struct {
    UINT32                            RPB_WRREQ_CRD:8;
    UINT32                            XSP_REQ_CRD:7;
    UINT32                            HOP_DATA_BUF_FULL:1;
    UINT32                            HOP_ATTR_BUF_FULL:1;
    UINT32                            CNS_BUF_FULL:1;
    UINT32                            CNS_BUF_BUSY:1;
    UINT32                            RPB_RDREQ_CRD:8;
    UINT32                            Reserved_31_27:5;
  } Field;
  UINT32 Value;
} XPB_INTF_STS_STRUCT;

#define SMN_XPB_INTF_STS_ADDRESS    0x32b8UL


/***********************************************************
* Register Name : XPB_LB_ADDR
* Register Description :
* This controls the type of address-based load balancing is done. Each address threshold comparison consists of 2 steps: 1st a bitwise AND-mask is applied to the address. Then the resulting bits are compared to a threshold value. Post-masked addresses that are under the threshold go to hostport or sideport as indicated by the type of load balancing being used. Otherwise, the request goes to the other port. (Of course, this logic only applies to requests that are eligible to be sent to the sideport.) ADDR_CMP0 & ADDR_CMP1 addresses cover slightly different ranges depending on what's useful. Large address thresholds are intended to be helpful in load balancing cases of address-striding that might otherwise always end up on the same port. Also, care should be taken to not set the threshold too low if that prevents write combining from creating larger, more efficient write requests.iyuan?
* Visibility : 0x2
************************************************************/

// Bitfield Description : Set these bits to indicate the value to compare against the masked address. Range of granularity of ADDR_CMP0 switching thresholds: 64B-32k (addr(15:6)).
#define XPB_LB_ADDR_CMP0_OFFSET      0
#define XPB_LB_ADDR_CMP0_MASK        0x3ff

// Bitfield Description : Set these bits to indicate which address(15:6) bits are masked for ADDR_CMP0 compare
#define XPB_LB_ADDR_MASK0_OFFSET      10
#define XPB_LB_ADDR_MASK0_MASK        0xffc00

// Bitfield Description : Set these bits to indicate the value to compare against the masked address. Range of granularity of ADDR_CMP1 switching thresholds: 32B-1k (addr(10:5)).
#define XPB_LB_ADDR_CMP1_OFFSET      20
#define XPB_LB_ADDR_CMP1_MASK        0x3f00000

// Bitfield Description : Set these bits to indicate which address(10:5) bits are masked for ADDR_CMP1 compare
#define XPB_LB_ADDR_MASK1_OFFSET      26
#define XPB_LB_ADDR_MASK1_MASK        0xfc000000

typedef union {
  struct {
    UINT32                            CMP0:10;
    UINT32                            MASK0:10;
    UINT32                            CMP1:6;
    UINT32                            MASK1:6;
  } Field;
  UINT32 Value;
} XPB_LB_ADDR_STRUCT;

#define SMN_XPB_LB_ADDR_ADDRESS    0x3238UL


/***********************************************************
* Register Name : XPB_MAP_INVERT_FLUSH_NUM_LSB
* Register Description :
* RESERVED for debug: This is for debug visibility to MAP's invert_flush_num_lsb register (bit per flush number) iyuan?
* Visibility : 0x2
************************************************************/

// Bitfield Description : one of these bits get written to each time that HDP_XDP_D2H_FLUSH gets written to (via XPB snooped path). See HDP_XDP_D2H_FLUSH reg desc for more details.
#define XPB_MAP_INVERT_FLUSH_NUM_LSB_ALTER_FLUSH_NUM_OFFSET      0
#define XPB_MAP_INVERT_FLUSH_NUM_LSB_ALTER_FLUSH_NUM_MASK        0xffff

// Bitfield Description : 
#define XPB_MAP_INVERT_FLUSH_NUM_LSB_Reserved_31_16_OFFSET      16
#define XPB_MAP_INVERT_FLUSH_NUM_LSB_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            ALTER_FLUSH_NUM:16;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} XPB_MAP_INVERT_FLUSH_NUM_LSB_STRUCT;

#define SMN_XPB_MAP_INVERT_FLUSH_NUM_LSB_ADDRESS    0x32c4UL


/***********************************************************
* Register Name : XPB_MISC_CFG
* Register Description :
* RESERVED for potential ECO: Placeholder for some TBD hardware options (mostly for debug).
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_MISC_CFG_FIELDNAME0_OFFSET      0
#define XPB_MISC_CFG_FIELDNAME0_MASK        0xff

// Bitfield Description : 
#define XPB_MISC_CFG_FIELDNAME1_OFFSET      8
#define XPB_MISC_CFG_FIELDNAME1_MASK        0xff00

// Bitfield Description : 
#define XPB_MISC_CFG_FIELDNAME2_OFFSET      16
#define XPB_MISC_CFG_FIELDNAME2_MASK        0xff0000

// Bitfield Description : 
#define XPB_MISC_CFG_FIELDNAME3_OFFSET      24
#define XPB_MISC_CFG_FIELDNAME3_MASK        0x7f000000

// Bitfield Description : 
#define XPB_MISC_CFG_TRIGGERNAME_OFFSET      31
#define XPB_MISC_CFG_TRIGGERNAME_MASK        0x80000000

typedef union {
  struct {
    UINT32                            FIELDNAME0:8;
    UINT32                            FIELDNAME1:8;
    UINT32                            FIELDNAME2:8;
    UINT32                            FIELDNAME3:7;
    UINT32                            TRIGGERNAME:1;
  } Field;
  UINT32 Value;
} XPB_MISC_CFG_STRUCT;

#define SMN_XPB_MISC_CFG_ADDRESS    0x32d4UL


/***********************************************************
* Register Name : XPB_P2P_BAR0
* Register Description :
* RESERVED for debug: This is for debug read-only visibility of each P2P BAR that XPB uses for compressing requests across hostport. See XPB_P2P_BAR_SETUP & XPB_P2P_BAR_DEBUG reg descriptions for field descriptions.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_P2P_BAR0_HOST_FLUSH_OFFSET      0
#define XPB_P2P_BAR0_HOST_FLUSH_MASK        0xf

// Bitfield Description : 
#define XPB_P2P_BAR0_REG_SYS_BAR_OFFSET      4
#define XPB_P2P_BAR0_REG_SYS_BAR_MASK        0xf0

// Bitfield Description : 
#define XPB_P2P_BAR0_MEM_SYS_BAR_OFFSET      8
#define XPB_P2P_BAR0_MEM_SYS_BAR_MASK        0xf00

// Bitfield Description : 
#define XPB_P2P_BAR0_VALID_OFFSET      12
#define XPB_P2P_BAR0_VALID_MASK        0x1000

// Bitfield Description : 
#define XPB_P2P_BAR0_SEND_DIS_OFFSET      13
#define XPB_P2P_BAR0_SEND_DIS_MASK        0x2000

// Bitfield Description : 
#define XPB_P2P_BAR0_COMPRESS_DIS_OFFSET      14
#define XPB_P2P_BAR0_COMPRESS_DIS_MASK        0x4000

// Bitfield Description : 
#define XPB_P2P_BAR0_Reserved_15_15_OFFSET      15
#define XPB_P2P_BAR0_Reserved_15_15_MASK        0x8000

// Bitfield Description : 
#define XPB_P2P_BAR0_ADDRESS_OFFSET      16
#define XPB_P2P_BAR0_ADDRESS_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            HOST_FLUSH:4;
    UINT32                            REG_SYS_BAR:4;
    UINT32                            MEM_SYS_BAR:4;
    UINT32                            VALID:1;
    UINT32                            SEND_DIS:1;
    UINT32                            COMPRESS_DIS:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            ADDRESS:16;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR0_STRUCT;

#define SMN_XPB_P2P_BAR0_ADDRESS    0x3248UL


/***********************************************************
* Register Name : XPB_P2P_BAR1
* Register Description :
* RESERVED for debug: This is for debug read-only visibility of each P2P BAR that XPB uses for compressing requests across hostport. See XPB_P2P_BAR_SETUP & XPB_P2P_BAR_DEBUG reg descriptions for field descriptions.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_P2P_BAR1_HOST_FLUSH_OFFSET      0
#define XPB_P2P_BAR1_HOST_FLUSH_MASK        0xf

// Bitfield Description : 
#define XPB_P2P_BAR1_REG_SYS_BAR_OFFSET      4
#define XPB_P2P_BAR1_REG_SYS_BAR_MASK        0xf0

// Bitfield Description : 
#define XPB_P2P_BAR1_MEM_SYS_BAR_OFFSET      8
#define XPB_P2P_BAR1_MEM_SYS_BAR_MASK        0xf00

// Bitfield Description : 
#define XPB_P2P_BAR1_VALID_OFFSET      12
#define XPB_P2P_BAR1_VALID_MASK        0x1000

// Bitfield Description : 
#define XPB_P2P_BAR1_SEND_DIS_OFFSET      13
#define XPB_P2P_BAR1_SEND_DIS_MASK        0x2000

// Bitfield Description : 
#define XPB_P2P_BAR1_COMPRESS_DIS_OFFSET      14
#define XPB_P2P_BAR1_COMPRESS_DIS_MASK        0x4000

// Bitfield Description : 
#define XPB_P2P_BAR1_Reserved_15_15_OFFSET      15
#define XPB_P2P_BAR1_Reserved_15_15_MASK        0x8000

// Bitfield Description : 
#define XPB_P2P_BAR1_ADDRESS_OFFSET      16
#define XPB_P2P_BAR1_ADDRESS_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            HOST_FLUSH:4;
    UINT32                            REG_SYS_BAR:4;
    UINT32                            MEM_SYS_BAR:4;
    UINT32                            VALID:1;
    UINT32                            SEND_DIS:1;
    UINT32                            COMPRESS_DIS:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            ADDRESS:16;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR1_STRUCT;

#define SMN_XPB_P2P_BAR1_ADDRESS    0x324cUL


/***********************************************************
* Register Name : XPB_P2P_BAR2
* Register Description :
* RESERVED for debug: This is for debug read-only visibility of each P2P BAR that XPB uses for compressing requests across hostport. See XPB_P2P_BAR_SETUP & XPB_P2P_BAR_DEBUG reg descriptions for field descriptions.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_P2P_BAR2_HOST_FLUSH_OFFSET      0
#define XPB_P2P_BAR2_HOST_FLUSH_MASK        0xf

// Bitfield Description : 
#define XPB_P2P_BAR2_REG_SYS_BAR_OFFSET      4
#define XPB_P2P_BAR2_REG_SYS_BAR_MASK        0xf0

// Bitfield Description : 
#define XPB_P2P_BAR2_MEM_SYS_BAR_OFFSET      8
#define XPB_P2P_BAR2_MEM_SYS_BAR_MASK        0xf00

// Bitfield Description : 
#define XPB_P2P_BAR2_VALID_OFFSET      12
#define XPB_P2P_BAR2_VALID_MASK        0x1000

// Bitfield Description : 
#define XPB_P2P_BAR2_SEND_DIS_OFFSET      13
#define XPB_P2P_BAR2_SEND_DIS_MASK        0x2000

// Bitfield Description : 
#define XPB_P2P_BAR2_COMPRESS_DIS_OFFSET      14
#define XPB_P2P_BAR2_COMPRESS_DIS_MASK        0x4000

// Bitfield Description : 
#define XPB_P2P_BAR2_Reserved_15_15_OFFSET      15
#define XPB_P2P_BAR2_Reserved_15_15_MASK        0x8000

// Bitfield Description : 
#define XPB_P2P_BAR2_ADDRESS_OFFSET      16
#define XPB_P2P_BAR2_ADDRESS_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            HOST_FLUSH:4;
    UINT32                            REG_SYS_BAR:4;
    UINT32                            MEM_SYS_BAR:4;
    UINT32                            VALID:1;
    UINT32                            SEND_DIS:1;
    UINT32                            COMPRESS_DIS:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            ADDRESS:16;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR2_STRUCT;

#define SMN_XPB_P2P_BAR2_ADDRESS    0x3250UL


/***********************************************************
* Register Name : XPB_P2P_BAR3
* Register Description :
* RESERVED for debug: This is for debug read-only visibility of each P2P BAR that XPB uses for compressing requests across hostport. See XPB_P2P_BAR_SETUP & XPB_P2P_BAR_DEBUG reg descriptions for field descriptions.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_P2P_BAR3_HOST_FLUSH_OFFSET      0
#define XPB_P2P_BAR3_HOST_FLUSH_MASK        0xf

// Bitfield Description : 
#define XPB_P2P_BAR3_REG_SYS_BAR_OFFSET      4
#define XPB_P2P_BAR3_REG_SYS_BAR_MASK        0xf0

// Bitfield Description : 
#define XPB_P2P_BAR3_MEM_SYS_BAR_OFFSET      8
#define XPB_P2P_BAR3_MEM_SYS_BAR_MASK        0xf00

// Bitfield Description : 
#define XPB_P2P_BAR3_VALID_OFFSET      12
#define XPB_P2P_BAR3_VALID_MASK        0x1000

// Bitfield Description : 
#define XPB_P2P_BAR3_SEND_DIS_OFFSET      13
#define XPB_P2P_BAR3_SEND_DIS_MASK        0x2000

// Bitfield Description : 
#define XPB_P2P_BAR3_COMPRESS_DIS_OFFSET      14
#define XPB_P2P_BAR3_COMPRESS_DIS_MASK        0x4000

// Bitfield Description : 
#define XPB_P2P_BAR3_Reserved_15_15_OFFSET      15
#define XPB_P2P_BAR3_Reserved_15_15_MASK        0x8000

// Bitfield Description : 
#define XPB_P2P_BAR3_ADDRESS_OFFSET      16
#define XPB_P2P_BAR3_ADDRESS_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            HOST_FLUSH:4;
    UINT32                            REG_SYS_BAR:4;
    UINT32                            MEM_SYS_BAR:4;
    UINT32                            VALID:1;
    UINT32                            SEND_DIS:1;
    UINT32                            COMPRESS_DIS:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            ADDRESS:16;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR3_STRUCT;

#define SMN_XPB_P2P_BAR3_ADDRESS    0x3254UL


/***********************************************************
* Register Name : XPB_P2P_BAR4
* Register Description :
* RESERVED for debug: This is for debug read-only visibility of each P2P BAR that XPB uses for compressing requests across hostport. See XPB_P2P_BAR_SETUP & XPB_P2P_BAR_DEBUG reg descriptions for field descriptions.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_P2P_BAR4_HOST_FLUSH_OFFSET      0
#define XPB_P2P_BAR4_HOST_FLUSH_MASK        0xf

// Bitfield Description : 
#define XPB_P2P_BAR4_REG_SYS_BAR_OFFSET      4
#define XPB_P2P_BAR4_REG_SYS_BAR_MASK        0xf0

// Bitfield Description : 
#define XPB_P2P_BAR4_MEM_SYS_BAR_OFFSET      8
#define XPB_P2P_BAR4_MEM_SYS_BAR_MASK        0xf00

// Bitfield Description : 
#define XPB_P2P_BAR4_VALID_OFFSET      12
#define XPB_P2P_BAR4_VALID_MASK        0x1000

// Bitfield Description : 
#define XPB_P2P_BAR4_SEND_DIS_OFFSET      13
#define XPB_P2P_BAR4_SEND_DIS_MASK        0x2000

// Bitfield Description : 
#define XPB_P2P_BAR4_COMPRESS_DIS_OFFSET      14
#define XPB_P2P_BAR4_COMPRESS_DIS_MASK        0x4000

// Bitfield Description : 
#define XPB_P2P_BAR4_Reserved_15_15_OFFSET      15
#define XPB_P2P_BAR4_Reserved_15_15_MASK        0x8000

// Bitfield Description : 
#define XPB_P2P_BAR4_ADDRESS_OFFSET      16
#define XPB_P2P_BAR4_ADDRESS_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            HOST_FLUSH:4;
    UINT32                            REG_SYS_BAR:4;
    UINT32                            MEM_SYS_BAR:4;
    UINT32                            VALID:1;
    UINT32                            SEND_DIS:1;
    UINT32                            COMPRESS_DIS:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            ADDRESS:16;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR4_STRUCT;

#define SMN_XPB_P2P_BAR4_ADDRESS    0x3258UL


/***********************************************************
* Register Name : XPB_P2P_BAR5
* Register Description :
* RESERVED for debug: This is for debug read-only visibility of each P2P BAR that XPB uses for compressing requests across hostport. See XPB_P2P_BAR_SETUP & XPB_P2P_BAR_DEBUG reg descriptions for field descriptions.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_P2P_BAR5_HOST_FLUSH_OFFSET      0
#define XPB_P2P_BAR5_HOST_FLUSH_MASK        0xf

// Bitfield Description : 
#define XPB_P2P_BAR5_REG_SYS_BAR_OFFSET      4
#define XPB_P2P_BAR5_REG_SYS_BAR_MASK        0xf0

// Bitfield Description : 
#define XPB_P2P_BAR5_MEM_SYS_BAR_OFFSET      8
#define XPB_P2P_BAR5_MEM_SYS_BAR_MASK        0xf00

// Bitfield Description : 
#define XPB_P2P_BAR5_VALID_OFFSET      12
#define XPB_P2P_BAR5_VALID_MASK        0x1000

// Bitfield Description : 
#define XPB_P2P_BAR5_SEND_DIS_OFFSET      13
#define XPB_P2P_BAR5_SEND_DIS_MASK        0x2000

// Bitfield Description : 
#define XPB_P2P_BAR5_COMPRESS_DIS_OFFSET      14
#define XPB_P2P_BAR5_COMPRESS_DIS_MASK        0x4000

// Bitfield Description : 
#define XPB_P2P_BAR5_Reserved_15_15_OFFSET      15
#define XPB_P2P_BAR5_Reserved_15_15_MASK        0x8000

// Bitfield Description : 
#define XPB_P2P_BAR5_ADDRESS_OFFSET      16
#define XPB_P2P_BAR5_ADDRESS_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            HOST_FLUSH:4;
    UINT32                            REG_SYS_BAR:4;
    UINT32                            MEM_SYS_BAR:4;
    UINT32                            VALID:1;
    UINT32                            SEND_DIS:1;
    UINT32                            COMPRESS_DIS:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            ADDRESS:16;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR5_STRUCT;

#define SMN_XPB_P2P_BAR5_ADDRESS    0x325cUL


/***********************************************************
* Register Name : XPB_P2P_BAR6
* Register Description :
* RESERVED for debug: This is for debug read-only visibility of each P2P BAR that XPB uses for compressing requests across hostport. See XPB_P2P_BAR_SETUP & XPB_P2P_BAR_DEBUG reg descriptions for field descriptions.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_P2P_BAR6_HOST_FLUSH_OFFSET      0
#define XPB_P2P_BAR6_HOST_FLUSH_MASK        0xf

// Bitfield Description : 
#define XPB_P2P_BAR6_REG_SYS_BAR_OFFSET      4
#define XPB_P2P_BAR6_REG_SYS_BAR_MASK        0xf0

// Bitfield Description : 
#define XPB_P2P_BAR6_MEM_SYS_BAR_OFFSET      8
#define XPB_P2P_BAR6_MEM_SYS_BAR_MASK        0xf00

// Bitfield Description : 
#define XPB_P2P_BAR6_VALID_OFFSET      12
#define XPB_P2P_BAR6_VALID_MASK        0x1000

// Bitfield Description : 
#define XPB_P2P_BAR6_SEND_DIS_OFFSET      13
#define XPB_P2P_BAR6_SEND_DIS_MASK        0x2000

// Bitfield Description : 
#define XPB_P2P_BAR6_COMPRESS_DIS_OFFSET      14
#define XPB_P2P_BAR6_COMPRESS_DIS_MASK        0x4000

// Bitfield Description : 
#define XPB_P2P_BAR6_Reserved_15_15_OFFSET      15
#define XPB_P2P_BAR6_Reserved_15_15_MASK        0x8000

// Bitfield Description : 
#define XPB_P2P_BAR6_ADDRESS_OFFSET      16
#define XPB_P2P_BAR6_ADDRESS_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            HOST_FLUSH:4;
    UINT32                            REG_SYS_BAR:4;
    UINT32                            MEM_SYS_BAR:4;
    UINT32                            VALID:1;
    UINT32                            SEND_DIS:1;
    UINT32                            COMPRESS_DIS:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            ADDRESS:16;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR6_STRUCT;

#define SMN_XPB_P2P_BAR6_ADDRESS    0x3260UL


/***********************************************************
* Register Name : XPB_P2P_BAR7
* Register Description :
* RESERVED for debug: This is for debug read-only visibility of each P2P BAR that XPB uses for compressing requests across hostport. See XPB_P2P_BAR_SETUP & XPB_P2P_BAR_DEBUG reg descriptions for field descriptions.
* Visibility : 0x2
************************************************************/

// Bitfield Description : 
#define XPB_P2P_BAR7_HOST_FLUSH_OFFSET      0
#define XPB_P2P_BAR7_HOST_FLUSH_MASK        0xf

// Bitfield Description : 
#define XPB_P2P_BAR7_REG_SYS_BAR_OFFSET      4
#define XPB_P2P_BAR7_REG_SYS_BAR_MASK        0xf0

// Bitfield Description : 
#define XPB_P2P_BAR7_MEM_SYS_BAR_OFFSET      8
#define XPB_P2P_BAR7_MEM_SYS_BAR_MASK        0xf00

// Bitfield Description : 
#define XPB_P2P_BAR7_VALID_OFFSET      12
#define XPB_P2P_BAR7_VALID_MASK        0x1000

// Bitfield Description : 
#define XPB_P2P_BAR7_SEND_DIS_OFFSET      13
#define XPB_P2P_BAR7_SEND_DIS_MASK        0x2000

// Bitfield Description : 
#define XPB_P2P_BAR7_COMPRESS_DIS_OFFSET      14
#define XPB_P2P_BAR7_COMPRESS_DIS_MASK        0x4000

// Bitfield Description : 
#define XPB_P2P_BAR7_Reserved_15_15_OFFSET      15
#define XPB_P2P_BAR7_Reserved_15_15_MASK        0x8000

// Bitfield Description : 
#define XPB_P2P_BAR7_ADDRESS_OFFSET      16
#define XPB_P2P_BAR7_ADDRESS_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            HOST_FLUSH:4;
    UINT32                            REG_SYS_BAR:4;
    UINT32                            MEM_SYS_BAR:4;
    UINT32                            VALID:1;
    UINT32                            SEND_DIS:1;
    UINT32                            COMPRESS_DIS:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            ADDRESS:16;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR7_STRUCT;

#define SMN_XPB_P2P_BAR7_ADDRESS    0x3264UL


/***********************************************************
* Register Name : XPB_P2P_BAR_CFG
* Register Description :
* This reg specifies to what amount the address should be compressed, and how many bits (lsbs) of the P2P BAR# should be sent (using the next most significant bits above the compressed address bits).
* Visibility : 0x2
************************************************************/

// Bitfield Description : this controls how many address bits the request needs to be compressed down to (normally: peer's PCIE FB aperture size/#bits taken for sending P2P BAR num)
#define XPB_P2P_BAR_CFG_ADDR_SIZE_OFFSET      0
#define XPB_P2P_BAR_CFG_ADDR_SIZE_MASK        0xf

// Bitfield Description : This & HDP_XDP_P2P_BAR_CFG.P2P_BAR_CFG_BAR_FROM should be programmed the same. this controls how many bits (lsbs) of the P2P BAR# should be sent (using the next most significant bits above the compressed address bits)
#define XPB_P2P_BAR_CFG_SEND_BAR_OFFSET      4
#define XPB_P2P_BAR_CFG_SEND_BAR_MASK        0x30

// Bitfield Description : This bit is used for the P2P_BAR update commands sent from XPB to XDP
#define XPB_P2P_BAR_CFG_SNOOP_OFFSET      6
#define XPB_P2P_BAR_CFG_SNOOP_MASK        0x40

// Bitfield Description : This chicken bit is used to disable sending P2P_BAR updates from XPB to XDP (for SW managed compression). P2P BAR contents are still used to compress request if disabled. (RESERVED, untested), we have direct case for these three bits!
#define XPB_P2P_BAR_CFG_SEND_DIS_OFFSET      7
#define XPB_P2P_BAR_CFG_SEND_DIS_MASK        0x80

// Bitfield Description : This chicken bit is used to disable P2P compression modification of address. (RESERVED, untested)
#define XPB_P2P_BAR_CFG_COMPRESS_DIS_OFFSET      8
#define XPB_P2P_BAR_CFG_COMPRESS_DIS_MASK        0x100

// Bitfield Description : This chicken bit is used to disable HW to modify P2P_BAR automatically when request doesn't match. (RESERVED, untested)
#define XPB_P2P_BAR_CFG_UPDATE_DIS_OFFSET      9
#define XPB_P2P_BAR_CFG_UPDATE_DIS_MASK        0x200

// Bitfield Description : If set to 1, the PEER_SYS_BAR used for the p2p_bar update reg write is (DEST_MAP.DEST_SEL^4'b0001). Otherwise (if 0), the p2p_bar needs to be initialized by SW writing XPB_P2P_BAR_SETUP.REG_SYS_BAR.
#define XPB_P2P_BAR_CFG_REGBAR_FROM_SYSBAR_OFFSET      10
#define XPB_P2P_BAR_CFG_REGBAR_FROM_SYSBAR_MASK        0x400

// Bitfield Description : enables p2p read pathway in xpb
#define XPB_P2P_BAR_CFG_RD_EN_OFFSET      11
#define XPB_P2P_BAR_CFG_RD_EN_MASK        0x800

// Bitfield Description : sets value of atc translated signal for xpb bar requests when CC_ATC_FUSE.SECURE_TRANSLATION is disabled
#define XPB_P2P_BAR_CFG_ATC_TRANSLATED_OFFSET      12
#define XPB_P2P_BAR_CFG_ATC_TRANSLATED_MASK        0x1000

// Bitfield Description : 
#define XPB_P2P_BAR_CFG_Reserved_31_13_OFFSET      13
#define XPB_P2P_BAR_CFG_Reserved_31_13_MASK        0xffffe000

typedef union {
  struct {
    UINT32                            ADDR_SIZE:4;
    UINT32                            SEND_BAR:2;
    UINT32                            SNOOP:1;
    UINT32                            SEND_DIS:1;
    UINT32                            COMPRESS_DIS:1;
    UINT32                            UPDATE_DIS:1;
    UINT32                            REGBAR_FROM_SYSBAR:1;
    UINT32                            RD_EN:1;
    UINT32                            ATC_TRANSLATED:1;
    UINT32                            Reserved_31_13:19;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR_CFG_STRUCT;

#define SMN_XPB_P2P_BAR_CFG_ADDRESS    0x3244UL


/***********************************************************
* Register Name : XPB_P2P_BAR_DEBUG
* Register Description :
* RESERVED for debug: This is for debug or workaround access to each P2P BAR that XPB uses for compressing requests across hostport. These fields are normally automatically updated by HW during P2P compression. Note that medium-grain clock gating must be overridden (forced on) when updating this reg, to ensure HW captures the changes.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Select which P2P_BAR reg(s) to update. Each bit that's 1 enables HOST_FLUSH & MEM_SYS_BAR to be updated in each of the corresponding P2P_BAR reg.
#define XPB_P2P_BAR_DEBUG_SEL_OFFSET      0
#define XPB_P2P_BAR_DEBUG_SEL_MASK        0xff

// Bitfield Description : Host P2P Flush Counter# associated with this P2P BAR.
#define XPB_P2P_BAR_DEBUG_HOST_FLUSH_OFFSET      8
#define XPB_P2P_BAR_DEBUG_HOST_FLUSH_MASK        0xf00

// Bitfield Description : which SYS BAR is (MEM) associated with this P2P BAR (for BAR update check).
#define XPB_P2P_BAR_DEBUG_MEM_SYS_BAR_OFFSET      12
#define XPB_P2P_BAR_DEBUG_MEM_SYS_BAR_MASK        0xf000

// Bitfield Description : 
#define XPB_P2P_BAR_DEBUG_Reserved_31_16_OFFSET      16
#define XPB_P2P_BAR_DEBUG_Reserved_31_16_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SEL:8;
    UINT32                            HOST_FLUSH:4;
    UINT32                            MEM_SYS_BAR:4;
    UINT32                            Reserved_31_16:16;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR_DEBUG_STRUCT;

#define SMN_XPB_P2P_BAR_DEBUG_ADDRESS    0x326cUL


/***********************************************************
* Register Name : XPB_P2P_BAR_DELTA_ABOVE
* Register Description :
* When a P2P BAR update is needed because the req address misses above the P2P aperture (ie: req_addr >= P2P BAR+aperture_size), the P2P BAR address is updated to the new request address MINUS the value in this register (ie: req_addr - DELTA_ABOVE). That can help avoid thrashing the P2P BAR in the cases where address isn't incrementing.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable: Each bit of this field applies to the respective P2P BAR (1 bit per). This way, requests that are just incrementing bursts can get a DELTA_ABOVE of 0, minimizing # of BAR updates needed.
#define XPB_P2P_BAR_DELTA_ABOVE_EN_OFFSET      0
#define XPB_P2P_BAR_DELTA_ABOVE_EN_MASK        0xff

// Bitfield Description : This delta_address(35:16) can be subtracted from the address that will be used for the P2P BAR update. This should always be set to something LESS than 1/2 of XPB_P2P_BAR_CFG.ADDR_SIZE.
#define XPB_P2P_BAR_DELTA_ABOVE_DELTA_OFFSET      8
#define XPB_P2P_BAR_DELTA_ABOVE_DELTA_MASK        0xfffff00

// Bitfield Description : 
#define XPB_P2P_BAR_DELTA_ABOVE_Reserved_31_28_OFFSET      28
#define XPB_P2P_BAR_DELTA_ABOVE_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            EN:8;
    UINT32                            DELTA:20;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR_DELTA_ABOVE_STRUCT;

#define SMN_XPB_P2P_BAR_DELTA_ABOVE_ADDRESS    0x3270UL


/***********************************************************
* Register Name : XPB_P2P_BAR_DELTA_BELOW
* Register Description :
* When a P2P BAR update is needed because the req address misses below the P2P aperture (ie: req_addr < P2P BAR), the P2P BAR address is updated to the new request address MINUS the value in this register (ie: req_addr - DELTA_BELOW). That can help avoid thrashing the P2P BAR in the cases where address isn't incrementing.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Enable: Each bit of this field applies to the respective P2P BAR (1 bit per). This way, requests that are just incrementing bursts can get a DELTA_BELOW of 0, minimizing # of BAR updates needed.
#define XPB_P2P_BAR_DELTA_BELOW_EN_OFFSET      0
#define XPB_P2P_BAR_DELTA_BELOW_EN_MASK        0xff

// Bitfield Description : This delta_address(35:16) can be subtracted from the address that will be used for the P2P BAR update. This should always be set to something LESS than 1/2 of XPB_P2P_BAR_CFG.ADDR_SIZE.
#define XPB_P2P_BAR_DELTA_BELOW_DELTA_OFFSET      8
#define XPB_P2P_BAR_DELTA_BELOW_DELTA_MASK        0xfffff00

// Bitfield Description : 
#define XPB_P2P_BAR_DELTA_BELOW_Reserved_31_28_OFFSET      28
#define XPB_P2P_BAR_DELTA_BELOW_Reserved_31_28_MASK        0xf0000000

typedef union {
  struct {
    UINT32                            EN:8;
    UINT32                            DELTA:20;
    UINT32                            Reserved_31_28:4;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR_DELTA_BELOW_STRUCT;

#define SMN_XPB_P2P_BAR_DELTA_BELOW_ADDRESS    0x3274UL


/***********************************************************
* Register Name : XPB_P2P_BAR_SETUP
* Register Description :
* RESERVED for debug: This is for workaround (& alternate init config) access to each P2P BAR that XPB uses for compressing requests across hostport. Note that medium-grain clock gating must be overridden (forced on) when updating this reg, to ensure HW captures the changes. iyuan?
* Visibility : 0x2
************************************************************/

// Bitfield Description : Select which P2P_BAR reg(s) to update. Each bit that's 1 enables ADDRESS & VALID to be updated in each of the corresponding P2P_BAR reg.
#define XPB_P2P_BAR_SETUP_SEL_OFFSET      0
#define XPB_P2P_BAR_SETUP_SEL_MASK        0xff

// Bitfield Description : indicates which SYS BAR has the REG BAR to use & send a P2P BAR update command to peer. SW sets this & HW does not change it. If SW writes to this register while HW is doing automatic P2P_BAR updates, SW should set VALID=0 so that HW will update other fields.
#define XPB_P2P_BAR_SETUP_REG_SYS_BAR_OFFSET      8
#define XPB_P2P_BAR_SETUP_REG_SYS_BAR_MASK        0xf00

// Bitfield Description : Whether this P2P BAR is valid. (if not, next request trying to use it will trigger update of it).
#define XPB_P2P_BAR_SETUP_VALID_OFFSET      12
#define XPB_P2P_BAR_SETUP_VALID_MASK        0x1000

// Bitfield Description : Set to disable HW sending P2P_BAR updates. (RESERVED, untested)
#define XPB_P2P_BAR_SETUP_SEND_DIS_OFFSET      13
#define XPB_P2P_BAR_SETUP_SEND_DIS_MASK        0x2000

// Bitfield Description : Set to disable p2p compression of req address. (RESERVED, untested)
#define XPB_P2P_BAR_SETUP_COMPRESS_DIS_OFFSET      14
#define XPB_P2P_BAR_SETUP_COMPRESS_DIS_MASK        0x4000

// Bitfield Description : 
#define XPB_P2P_BAR_SETUP_Reserved_15_15_OFFSET      15
#define XPB_P2P_BAR_SETUP_Reserved_15_15_MASK        0x8000

// Bitfield Description : Address bits (35:20) of P2P BAR.
#define XPB_P2P_BAR_SETUP_ADDRESS_OFFSET      16
#define XPB_P2P_BAR_SETUP_ADDRESS_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            SEL:8;
    UINT32                            REG_SYS_BAR:4;
    UINT32                            VALID:1;
    UINT32                            SEND_DIS:1;
    UINT32                            COMPRESS_DIS:1;
    UINT32                            Reserved_15_15:1;
    UINT32                            ADDRESS:16;
  } Field;
  UINT32 Value;
} XPB_P2P_BAR_SETUP_STRUCT;

#define SMN_XPB_P2P_BAR_SETUP_ADDRESS    0x3268UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR0
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this).
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR0_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR0_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR0_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR0_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR0_STRUCT;

#define SMN_XPB_PEER_SYS_BAR0_ADDRESS    0x3278UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR1
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this).
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR1_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR1_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR1_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR1_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR1_STRUCT;

#define SMN_XPB_PEER_SYS_BAR1_ADDRESS    0x327cUL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR10
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this). Note that XMDA_PEER_SYS_BAR0 corresponds to the legacy PEER_SYS_BAR10 and so on for register progamming purposes.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR10_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR10_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR10_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR10_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR10_STRUCT;

#define SMN_XPB_PEER_SYS_BAR10_ADDRESS    0x32a0UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR11
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this). Note that XMDA_PEER_SYS_BAR0 corresponds to the legacy PEER_SYS_BAR10 and so on for register progamming purposes.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR11_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR11_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR11_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR11_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR11_STRUCT;

#define SMN_XPB_PEER_SYS_BAR11_ADDRESS    0x32a4UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR12
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this). Note that XMDA_PEER_SYS_BAR0 corresponds to the legacy PEER_SYS_BAR10 and so on for register progamming purposes.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR12_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR12_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR12_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR12_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR12_STRUCT;

#define SMN_XPB_PEER_SYS_BAR12_ADDRESS    0x32a8UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR13
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this). Note that XMDA_PEER_SYS_BAR0 corresponds to the legacy PEER_SYS_BAR10 and so on for register progamming purposes.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR13_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR13_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR13_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR13_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR13_STRUCT;

#define SMN_XPB_PEER_SYS_BAR13_ADDRESS    0x32acUL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR2
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this).
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR2_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR2_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR2_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR2_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR2_STRUCT;

#define SMN_XPB_PEER_SYS_BAR2_ADDRESS    0x3280UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR3
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this).
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR3_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR3_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR3_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR3_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR3_STRUCT;

#define SMN_XPB_PEER_SYS_BAR3_ADDRESS    0x3284UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR4
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this).
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR4_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR4_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR4_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR4_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR4_STRUCT;

#define SMN_XPB_PEER_SYS_BAR4_ADDRESS    0x3288UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR5
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this).
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR5_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR5_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR5_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR5_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR5_STRUCT;

#define SMN_XPB_PEER_SYS_BAR5_ADDRESS    0x328cUL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR6
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this).
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR6_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR6_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR6_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR6_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR6_STRUCT;

#define SMN_XPB_PEER_SYS_BAR6_ADDRESS    0x3290UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR7
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this).
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR7_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR7_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR7_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR7_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR7_STRUCT;

#define SMN_XPB_PEER_SYS_BAR7_ADDRESS    0x3294UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR8
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this).
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR8_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR8_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR8_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR8_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR8_STRUCT;

#define SMN_XPB_PEER_SYS_BAR8_ADDRESS    0x3298UL


/***********************************************************
* Register Name : XPB_PEER_SYS_BAR9
* Register Description :
* This is configured with FB or REG system fabric BARs to be used in accessing peer GPUs. (ie: copied from other GPUs PCIE BARs from their PCIE config). Assumed to be pseudo-static (need to stop traffic & invalidate P2P BARs when changing this).
* Visibility : 0x2
************************************************************/

// Bitfield Description : Added for SW debug: Error condition if this address is used while Valid==0.
#define XPB_PEER_SYS_BAR9_VALID_OFFSET      0
#define XPB_PEER_SYS_BAR9_VALID_MASK        0x1

// Bitfield Description : Base Address(47:17): This includes support for 48bit addressing, down to 128kB (for REG BARs).
#define XPB_PEER_SYS_BAR9_ADDR_OFFSET      1
#define XPB_PEER_SYS_BAR9_ADDR_MASK        0xfffffffe

typedef union {
  struct {
    UINT32                            VALID:1;
    UINT32                            ADDR:31;
  } Field;
  UINT32 Value;
} XPB_PEER_SYS_BAR9_STRUCT;

#define SMN_XPB_PEER_SYS_BAR9_ADDRESS    0x329cUL


/***********************************************************
* Register Name : XPB_PERF_KNOBS
* Register Description :
* RESERVED for DEBUG & PERF: Various knobs for debug & perf verif
* Visibility : 0x2
************************************************************/

// Bitfield Description : PERF: Programmable depth for artificially limitting HW to use a reduced fifo depth. If non-0, this value controls how much of the fifo HW will use.
#define XPB_PERF_KNOBS_CNS_FIFO_DEPTH_OFFSET      0
#define XPB_PERF_KNOBS_CNS_FIFO_DEPTH_MASK        0x3f

// Bitfield Description : PERF: Programmable depth for artificially limitting HW to use a reduced fifo depth. If non-0, this value controls how much of the fifo HW will use.
#define XPB_PERF_KNOBS_WCB_HST_FIFO_DEPTH_OFFSET      6
#define XPB_PERF_KNOBS_WCB_HST_FIFO_DEPTH_MASK        0xfc0

// Bitfield Description : PERF: Programmable depth for artificially limitting HW to use a reduced fifo depth. If non-0, this value controls how much of the fifo HW will use.
#define XPB_PERF_KNOBS_WCB_SID_FIFO_DEPTH_OFFSET      12
#define XPB_PERF_KNOBS_WCB_SID_FIFO_DEPTH_MASK        0x3f000

// Bitfield Description : 
#define XPB_PERF_KNOBS_Reserved_31_18_OFFSET      18
#define XPB_PERF_KNOBS_Reserved_31_18_MASK        0xfffc0000

typedef union {
  struct {
    UINT32                            CNS_FIFO_DEPTH:6;
    UINT32                            WCB_HST_FIFO_DEPTH:6;
    UINT32                            WCB_SID_FIFO_DEPTH:6;
    UINT32                            Reserved_31_18:14;
  } Field;
  UINT32 Value;
} XPB_PERF_KNOBS_STRUCT;

#define SMN_XPB_PERF_KNOBS_ADDRESS    0x32c8UL


/***********************************************************
* Register Name : XPB_PIPE_STS
* Register Description :
* RESERVED for debug: This displays status of buffers, to aid in debug, perf, & detecting when all requests are empty.
* Visibility : 0x2
************************************************************/

// Bitfield Description : This displays 1 if ANY pending WCB have valid data in them. (OR of all pending WCB valid bits). See XPB_WCB_STS.PBUF_VLD for individual bits.
#define XPB_PIPE_STS_WCB_ANY_PBUF_OFFSET      0
#define XPB_PIPE_STS_WCB_ANY_PBUF_MASK        0x1

// Bitfield Description : This displays num entries stored in this buffer. (Same as XPB_WCB_STS's field of same name.)
#define XPB_PIPE_STS_WCB_HST_DATA_BUF_CNT_OFFSET      1
#define XPB_PIPE_STS_WCB_HST_DATA_BUF_CNT_MASK        0xfe

// Bitfield Description : This displays num entries stored in this buffer. (Same as XPB_WCB_STS's field of same name.)
#define XPB_PIPE_STS_WCB_SID_DATA_BUF_CNT_OFFSET      8
#define XPB_PIPE_STS_WCB_SID_DATA_BUF_CNT_MASK        0x7f00

// Bitfield Description : This displays whether this buffer is full
#define XPB_PIPE_STS_WCB_HST_RD_PTR_BUF_FULL_OFFSET      15
#define XPB_PIPE_STS_WCB_HST_RD_PTR_BUF_FULL_MASK        0x8000

// Bitfield Description : This displays whether this buffer is full
#define XPB_PIPE_STS_WCB_SID_RD_PTR_BUF_FULL_OFFSET      16
#define XPB_PIPE_STS_WCB_SID_RD_PTR_BUF_FULL_MASK        0x10000

// Bitfield Description : This displays whether this buffer is full
#define XPB_PIPE_STS_WCB_HST_REQ_FIFO_FULL_OFFSET      17
#define XPB_PIPE_STS_WCB_HST_REQ_FIFO_FULL_MASK        0x20000

// Bitfield Description : This displays whether this buffer is full
#define XPB_PIPE_STS_WCB_SID_REQ_FIFO_FULL_OFFSET      18
#define XPB_PIPE_STS_WCB_SID_REQ_FIFO_FULL_MASK        0x40000

// Bitfield Description : This displays whether this buffer is full
#define XPB_PIPE_STS_WCB_HST_REQ_OBUF_FULL_OFFSET      19
#define XPB_PIPE_STS_WCB_HST_REQ_OBUF_FULL_MASK        0x80000

// Bitfield Description : This displays whether this buffer is full
#define XPB_PIPE_STS_WCB_SID_REQ_OBUF_FULL_OFFSET      20
#define XPB_PIPE_STS_WCB_SID_REQ_OBUF_FULL_MASK        0x100000

// Bitfield Description : This displays whether this buffer is full
#define XPB_PIPE_STS_WCB_HST_DATA_OBUF_FULL_OFFSET      21
#define XPB_PIPE_STS_WCB_HST_DATA_OBUF_FULL_MASK        0x200000

// Bitfield Description : This displays whether this buffer is full
#define XPB_PIPE_STS_WCB_SID_DATA_OBUF_FULL_OFFSET      22
#define XPB_PIPE_STS_WCB_SID_DATA_OBUF_FULL_MASK        0x400000

// Bitfield Description : This displays whether this buffer is full
#define XPB_PIPE_STS_RET_BUF_FULL_OFFSET      23
#define XPB_PIPE_STS_RET_BUF_FULL_MASK        0x800000

// Bitfield Description : This displays busy status reported by each block on XPB CLK: {P2PR_busy, RTR_MAP_req_busy, MAP_busy, WCB_busy, SID_busy, HST_busy, HOP_busy, RET_busy}
#define XPB_PIPE_STS_XPB_CLK_BUSY_BITS_OFFSET      24
#define XPB_PIPE_STS_XPB_CLK_BUSY_BITS_MASK        0xff000000

typedef union {
  struct {
    UINT32                            WCB_ANY_PBUF:1;
    UINT32                            WCB_HST_DATA_BUF_CNT:7;
    UINT32                            WCB_SID_DATA_BUF_CNT:7;
    UINT32                            WCB_HST_RD_PTR_BUF_FULL:1;
    UINT32                            WCB_SID_RD_PTR_BUF_FULL:1;
    UINT32                            WCB_HST_REQ_FIFO_FULL:1;
    UINT32                            WCB_SID_REQ_FIFO_FULL:1;
    UINT32                            WCB_HST_REQ_OBUF_FULL:1;
    UINT32                            WCB_SID_REQ_OBUF_FULL:1;
    UINT32                            WCB_HST_DATA_OBUF_FULL:1;
    UINT32                            WCB_SID_DATA_OBUF_FULL:1;
    UINT32                            RET_BUF_FULL:1;
    UINT32                            XPB_CLK_BUSY_BITS:8;
  } Field;
  UINT32 Value;
} XPB_PIPE_STS_STRUCT;

#define SMN_XPB_PIPE_STS_ADDRESS    0x32bcUL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP0
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP0_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP0_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB iyuan?, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP0_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP0_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.iyuan?
#define XPB_RTR_DEST_MAP0_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP0_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, tested and is used to disable this APTR)
#define XPB_RTR_DEST_MAP0_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP0_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SOC).
#define XPB_RTR_DEST_MAP0_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP0_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP0_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP0_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP0_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP0_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP0_STRUCT;

#define SMN_XPB_RTR_DEST_MAP0_ADDRESS    0x31d8UL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP1
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP1_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP1_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP1_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP1_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP1_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP1_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP1_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP1_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP1_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP1_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP1_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP1_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP1_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP1_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP1_STRUCT;

#define SMN_XPB_RTR_DEST_MAP1_ADDRESS    0x31dcUL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP10
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP10_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP10_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP10_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP10_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP10_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP10_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP10_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP10_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP10_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP10_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP10_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP10_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP10_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP10_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP10_STRUCT;

#define SMN_XPB_RTR_DEST_MAP10_ADDRESS    0x3200UL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP11
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP11_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP11_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP11_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP11_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP11_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP11_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP11_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP11_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP11_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP11_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP11_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP11_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP11_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP11_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP11_STRUCT;

#define SMN_XPB_RTR_DEST_MAP11_ADDRESS    0x3204UL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP12
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP12_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP12_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP12_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP12_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP12_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP12_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP12_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP12_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP12_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP12_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP12_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP12_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP12_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP12_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP12_STRUCT;

#define SMN_XPB_RTR_DEST_MAP12_ADDRESS    0x3208UL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP13
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP13_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP13_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP13_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP13_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP13_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP13_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP13_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP13_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP13_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP13_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP13_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP13_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP13_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP13_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP13_STRUCT;

#define SMN_XPB_RTR_DEST_MAP13_ADDRESS    0x320cUL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP2
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP2_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP2_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP2_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP2_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP2_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP2_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP2_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP2_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP2_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP2_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP2_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP2_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP2_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP2_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP2_STRUCT;

#define SMN_XPB_RTR_DEST_MAP2_ADDRESS    0x31e0UL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP3
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP3_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP3_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP3_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP3_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP3_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP3_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP3_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP3_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP3_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP3_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP3_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP3_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP3_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP3_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP3_STRUCT;

#define SMN_XPB_RTR_DEST_MAP3_ADDRESS    0x31e4UL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP4
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP4_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP4_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP4_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP4_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP4_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP4_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP4_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP4_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP4_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP4_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP4_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP4_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP4_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP4_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP4_STRUCT;

#define SMN_XPB_RTR_DEST_MAP4_ADDRESS    0x31e8UL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP5
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP5_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP5_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP5_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP5_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP5_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP5_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP5_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP5_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP5_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP5_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP5_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP5_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP5_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP5_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP5_STRUCT;

#define SMN_XPB_RTR_DEST_MAP5_ADDRESS    0x31ecUL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP6
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP6_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP6_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP6_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP6_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP6_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP6_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP6_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP6_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP6_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP6_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP6_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP6_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP6_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP6_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP6_STRUCT;

#define SMN_XPB_RTR_DEST_MAP6_ADDRESS    0x31f0UL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP7
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP7_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP7_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP7_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP7_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP7_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP7_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP7_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP7_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP7_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP7_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP7_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP7_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP7_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP7_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP7_STRUCT;

#define SMN_XPB_RTR_DEST_MAP7_ADDRESS    0x31f4UL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP8
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP8_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP8_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP8_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP8_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP8_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP8_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP8_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP8_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP8_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP8_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP8_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP8_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP8_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP8_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP8_STRUCT;

#define SMN_XPB_RTR_DEST_MAP8_ADDRESS    0x31f8UL


/***********************************************************
* Register Name : XPB_RTR_DEST_MAP9
* Register Description :
* This register maps requests coming into XPB to destination GPU REG or MEM requests, including an offset into the dest GPU if needed. Each of these regs applies to the requests that match the respective SRC_APRTR reg. (ie: if a request matches SRC_APRTR2, then DEST_MAP2 reg info is applied to that request.) After the XPB incoming request's offset within the SRC_APRTR has been determined, this register provides an additional address offset that is added to allow any 36b address to be reached. This register also identifies: which DEST# to send it to, whether the request is targeting a peer's register or memory, and whether these requests are eligible to be sent via sideport (to neighbor GPU).
* Visibility : 0x2
************************************************************/

// Bitfield Description : nMR = Not-Mem, Reg
#define XPB_RTR_DEST_MAP9_NMR_OFFSET      0
#define XPB_RTR_DEST_MAP9_NMR_MASK        0x1

// Bitfield Description : This address(39:21) offset is added to the requested address offset. address(39:36) are dropped on requests sent to XPB, so upper bits only intended for DEST_SEL_RPB==1 requests sent to RPB.
#define XPB_RTR_DEST_MAP9_DEST_OFFSET_OFFSET      1
#define XPB_RTR_DEST_MAP9_DEST_OFFSET_MASK        0xffffe

// Bitfield Description : indicates which destination (PEER_SYS_BAR) these requests should be sent to, so that the proper system BAR can be added to the request before sending out to the system bus (via hostport). note that if XPB_P2P_BAR_CFG.REGBAR_FROM_SYSBAR is set, then (DEST_SEL ^ 4'b0001) is used to select which PEER_SYS_BAR to use for the hostport p2p_bar update reg writes.
#define XPB_RTR_DEST_MAP9_DEST_SEL_OFFSET      20
#define XPB_RTR_DEST_MAP9_DEST_SEL_MASK        0xf00000

// Bitfield Description : Set this to indicate that matches to this SRC_APRTR (& DEST_MAP) will be sent to RPB instead of XPB, with SRC_APRTR & DEST_MAP address mappings applied. (RESERVED: implemented, but untested)
#define XPB_RTR_DEST_MAP9_DEST_SEL_RPB_OFFSET      24
#define XPB_RTR_DEST_MAP9_DEST_SEL_RPB_MASK        0x1000000

// Bitfield Description : This indicates if these requests are allowed to use sideport. Note: sideport usage also requires dest to be neighboring GPU & depends on client grp load balancing. Note: sideport flush requests should use a dest with SIDE_OK==1 (& NMR==1). XSP knows that all other REG requests must go to hostport, even if SIDE_OK==0 (this field is already retired from the SoC).
#define XPB_RTR_DEST_MAP9_SIDE_OK_OFFSET      25
#define XPB_RTR_DEST_MAP9_SIDE_OK_MASK        0x2000000

// Bitfield Description : Valid size range of 4-25 (& 0=disabled). Indicates aperture size used for this 40b-address comparison. Valid size range: 4 (32KB) to 25 (64GB).
#define XPB_RTR_DEST_MAP9_APRTR_SIZE_OFFSET      26
#define XPB_RTR_DEST_MAP9_APRTR_SIZE_MASK        0x7c000000

// Bitfield Description : 
#define XPB_RTR_DEST_MAP9_Reserved_31_31_OFFSET      31
#define XPB_RTR_DEST_MAP9_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            NMR:1;
    UINT32                            DEST_OFFSET:19;
    UINT32                            DEST_SEL:4;
    UINT32                            DEST_SEL_RPB:1;
    UINT32                            SIDE_OK:1;
    UINT32                            APRTR_SIZE:5;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_DEST_MAP9_STRUCT;

#define SMN_XPB_RTR_DEST_MAP9_ADDRESS    0x31fcUL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR0
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR0_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR0_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR0_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR0_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR0_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR0_ADDRESS    0x31a0UL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR1
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR1_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR1_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR1_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR1_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR1_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR1_ADDRESS    0x31a4UL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR10
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR10_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR10_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR10_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR10_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR10_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR10_ADDRESS    0x31c8UL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR11
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR11_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR11_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR11_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR11_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR11_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR11_ADDRESS    0x31ccUL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR12
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR12_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR12_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR12_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR12_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR12_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR12_ADDRESS    0x31d0UL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR13
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR13_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR13_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR13_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR13_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR13_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR13_ADDRESS    0x31d4UL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR2
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR2_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR2_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR2_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR2_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR2_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR2_ADDRESS    0x31a8UL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR3
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR3_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR3_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR3_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR3_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR3_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR3_ADDRESS    0x31acUL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR4
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR4_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR4_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR4_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR4_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR4_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR4_ADDRESS    0x31b0UL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR5
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR5_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR5_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR5_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR5_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR5_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR5_ADDRESS    0x31b4UL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR6
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR6_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR6_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR6_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR6_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR6_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR6_ADDRESS    0x31b8UL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR7
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR7_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR7_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR7_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR7_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR7_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR7_ADDRESS    0x31bcUL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR8
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR8_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR8_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR8_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR8_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR8_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR8_ADDRESS    0x31c0UL


/***********************************************************
* Register Name : XPB_RTR_SRC_APRTR9
* Register Description :
* The DF incoming 40b address space can have up to 14 (source) address ranges that can be mapped to destination address ranges (destination in a peer GPU). These registers specify 1 source address range per reg. The address range must be size aligned. The address range in this register is compared to incoming DF write requests to determine whether req is addressing a peer GPU (P2P) or system memory. P2P requests (for reg or fb) will match one of these addresses. The apertures need to of 2^n in size, and size-aligned.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Address(47:17) at the base of this aperture. (Aperture size specified in corresponding XPB_RTR_DEST_MAP* reg.)
#define XPB_RTR_SRC_APRTR9_BASE_ADDR_OFFSET      0
#define XPB_RTR_SRC_APRTR9_BASE_ADDR_MASK        0x7fffffff

// Bitfield Description : 
#define XPB_RTR_SRC_APRTR9_Reserved_31_31_OFFSET      31
#define XPB_RTR_SRC_APRTR9_Reserved_31_31_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BASE_ADDR:31;
    UINT32                            Reserved_31_31:1;
  } Field;
  UINT32 Value;
} XPB_RTR_SRC_APRTR9_STRUCT;

#define SMN_XPB_RTR_SRC_APRTR9_ADDRESS    0x31c4UL


/***********************************************************
* Register Name : XPB_STICKY
* Register Description :
* RESERVED for debug: This is for debug or workaround access to misc XPB info. eg: Error bits
* Visibility : 0x2
************************************************************/

// Bitfield Description : pending implementation
#define XPB_STICKY_BITS_OFFSET      0
#define XPB_STICKY_BITS_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Reserved_BITS:32;
  } Field;
  UINT32 Value;
} XPB_STICKY_STRUCT;

#define SMN_XPB_STICKY_ADDRESS    0x32ccUL


/***********************************************************
* Register Name : XPB_STICKY_W1C
* Register Description :
* RESERVED for debug: This reg is written to to clear bit(s) in XPB_STICKY reg. Value of 1 forces the respective bit in XPB_STICKY to be 0.
* Visibility : 0x2
************************************************************/

// Bitfield Description : see XPB_STICKY.BITS description
#define XPB_STICKY_W1C_BITS_OFFSET      0
#define XPB_STICKY_W1C_BITS_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            Reserved_BITS:32;
  } Field;
  UINT32 Value;
} XPB_STICKY_W1C_STRUCT;

#define SMN_XPB_STICKY_W1C_ADDRESS    0x32d0UL


/***********************************************************
* Register Name : XPB_SUB_CTRL
* Register Description :
* RESERVED for debug: RESERVED: stall bits for stalling internal sub-module interfaces, and sub-module resets. (RESERVED for debug since behavior is unverified & most bits likely have restrictions on how/when they can be safely used.)
* Visibility : 0x2
************************************************************/

// Bitfield Description : This bit enables a shortcut path through XPB. RESERVED, intended for initial integration & maybe to try to ensure XPB can be removed from SIP (for derivatives).
#define XPB_SUB_CTRL_WRREQ_BYPASS_XPB_OFFSET      0
#define XPB_SUB_CTRL_WRREQ_BYPASS_XPB_MASK        0x1

// Bitfield Description : set to stall this internal interface
#define XPB_SUB_CTRL_STALL_CNS_RTR_REQ_OFFSET      1
#define XPB_SUB_CTRL_STALL_CNS_RTR_REQ_MASK        0x2

// Bitfield Description : set to stall this internal interface
#define XPB_SUB_CTRL_STALL_RTR_RPB_WRREQ_OFFSET      2
#define XPB_SUB_CTRL_STALL_RTR_RPB_WRREQ_MASK        0x4

// Bitfield Description : set to stall this internal interface
#define XPB_SUB_CTRL_STALL_RTR_MAP_REQ_OFFSET      3
#define XPB_SUB_CTRL_STALL_RTR_MAP_REQ_MASK        0x8

// Bitfield Description : set to stall this internal interface
#define XPB_SUB_CTRL_STALL_MAP_WCB_REQ_OFFSET      4
#define XPB_SUB_CTRL_STALL_MAP_WCB_REQ_MASK        0x10

// Bitfield Description : set to stall this internal interface
#define XPB_SUB_CTRL_STALL_WCB_SID_REQ_OFFSET      5
#define XPB_SUB_CTRL_STALL_WCB_SID_REQ_MASK        0x20

// Bitfield Description : set to stall this internal interface
#define XPB_SUB_CTRL_STALL_MC_XSP_REQ_SEND_OFFSET      6
#define XPB_SUB_CTRL_STALL_MC_XSP_REQ_SEND_MASK        0x40

// Bitfield Description : set to stall this internal interface
#define XPB_SUB_CTRL_STALL_WCB_HST_REQ_OFFSET      7
#define XPB_SUB_CTRL_STALL_WCB_HST_REQ_MASK        0x80

// Bitfield Description : set to stall this internal interface
#define XPB_SUB_CTRL_STALL_HST_HOP_REQ_OFFSET      8
#define XPB_SUB_CTRL_STALL_HST_HOP_REQ_MASK        0x100

// Bitfield Description : set to stall this internal interface
#define XPB_SUB_CTRL_STALL_XPB_RPB_REQ_ATTR_OFFSET      9
#define XPB_SUB_CTRL_STALL_XPB_RPB_REQ_ATTR_MASK        0x200

// Bitfield Description : set to assert reset for this sub-module. must write a 2nd time to deassert (not self clearing).
#define XPB_SUB_CTRL_RESET_CNS_OFFSET      10
#define XPB_SUB_CTRL_RESET_CNS_MASK        0x400

// Bitfield Description : set to assert reset for this sub-module. must write a 2nd time to deassert (not self clearing).
#define XPB_SUB_CTRL_RESET_RTR_OFFSET      11
#define XPB_SUB_CTRL_RESET_RTR_MASK        0x800

// Bitfield Description : set to assert reset for this sub-module. must write a 2nd time to deassert (not self clearing).
#define XPB_SUB_CTRL_RESET_RET_OFFSET      12
#define XPB_SUB_CTRL_RESET_RET_MASK        0x1000

// Bitfield Description : set to assert reset for this sub-module. must write a 2nd time to deassert (not self clearing).
#define XPB_SUB_CTRL_RESET_MAP_OFFSET      13
#define XPB_SUB_CTRL_RESET_MAP_MASK        0x2000

// Bitfield Description : set to assert reset for this sub-module. must write a 2nd time to deassert (not self clearing).
#define XPB_SUB_CTRL_RESET_WCB_OFFSET      14
#define XPB_SUB_CTRL_RESET_WCB_MASK        0x4000

// Bitfield Description : set to assert reset for this sub-module. must write a 2nd time to deassert (not self clearing).
#define XPB_SUB_CTRL_RESET_HST_OFFSET      15
#define XPB_SUB_CTRL_RESET_HST_MASK        0x8000

// Bitfield Description : set to assert reset for this sub-module. must write a 2nd time to deassert (not self clearing).
#define XPB_SUB_CTRL_RESET_HOP_OFFSET      16
#define XPB_SUB_CTRL_RESET_HOP_MASK        0x10000

// Bitfield Description : set to assert reset for this sub-module. must write a 2nd time to deassert (not self clearing).
#define XPB_SUB_CTRL_RESET_SID_OFFSET      17
#define XPB_SUB_CTRL_RESET_SID_MASK        0x20000

// Bitfield Description : set to assert reset for this sub-module. must write a 2nd time to deassert (not self clearing).
#define XPB_SUB_CTRL_RESET_SRB_OFFSET      18
#define XPB_SUB_CTRL_RESET_SRB_MASK        0x40000

// Bitfield Description : set to assert reset for this sub-module. must write a 2nd time to deassert (not self clearing).
#define XPB_SUB_CTRL_RESET_CGR_OFFSET      19
#define XPB_SUB_CTRL_RESET_CGR_MASK        0x80000

// Bitfield Description : 
#define XPB_SUB_CTRL_Reserved_31_20_OFFSET      20
#define XPB_SUB_CTRL_Reserved_31_20_MASK        0xfff00000

typedef union {
  struct {
    UINT32                            WRREQ_BYPASS_XPB:1;
    UINT32                            STALL_CNS_RTR_REQ:1;
    UINT32                            STALL_RTR_RPB_WRREQ:1;
    UINT32                            STALL_RTR_MAP_REQ:1;
    UINT32                            STALL_MAP_WCB_REQ:1;
    UINT32                            STALL_WCB_SID_REQ:1;
    UINT32                            STALL_MC_XSP_REQ_SEND:1;
    UINT32                            STALL_WCB_HST_REQ:1;
    UINT32                            STALL_HST_HOP_REQ:1;
    UINT32                            STALL_XPB_RPB_REQ_ATTR:1;
    UINT32                            RESET_CNS:1;
    UINT32                            RESET_RTR:1;
    UINT32                            RESET_RET:1;
    UINT32                            RESET_MAP:1;
    UINT32                            RESET_WCB:1;
    UINT32                            RESET_HST:1;
    UINT32                            RESET_HOP:1;
    UINT32                            RESET_SID:1;
    UINT32                            RESET_SRB:1;
    UINT32                            RESET_CGR:1;
    UINT32                            Reserved_31_20:12;
  } Field;
  UINT32 Value;
} XPB_SUB_CTRL_STRUCT;

#define SMN_XPB_SUB_CTRL_ADDRESS    0x32c0UL


/***********************************************************
* Register Name : XPB_WCB_STS
* Register Description :
* RESERVED for debug: This displays Write Combining Buffer status
* Visibility : 0x2
************************************************************/

// Bitfield Description : This displays STatuS of which active WCB have VaLiD data in them.
#define XPB_WCB_STS_PBUF_VLD_OFFSET      0
#define XPB_WCB_STS_PBUF_VLD_MASK        0xffff

// Bitfield Description : This displays num entries stored in this buffer. (Same as XPB_PIPE_STS's field of same name.)
#define XPB_WCB_STS_WCB_HST_DATA_BUF_CNT_OFFSET      16
#define XPB_WCB_STS_WCB_HST_DATA_BUF_CNT_MASK        0x7f0000

// Bitfield Description : This displays num entries stored in this buffer. (Same as XPB_PIPE_STS's field of same name.)
#define XPB_WCB_STS_WCB_SID_DATA_BUF_CNT_OFFSET      23
#define XPB_WCB_STS_WCB_SID_DATA_BUF_CNT_MASK        0x3f800000

// Bitfield Description : 
#define XPB_WCB_STS_Reserved_31_30_OFFSET      30
#define XPB_WCB_STS_Reserved_31_30_MASK        0xc0000000

typedef union {
  struct {
    UINT32                            PBUF_VLD:16;
    UINT32                            WCB_HST_DATA_BUF_CNT:7;
    UINT32                            WCB_SID_DATA_BUF_CNT:7;
    UINT32                            Reserved_31_30:2;
  } Field;
  UINT32 Value;
} XPB_WCB_STS_STRUCT;

#define SMN_XPB_WCB_STS_ADDRESS    0x323cUL

#endif /* _ATHUB_XPB_H_ */
