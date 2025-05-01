/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _SYSHUBMM_H_
#define _SYSHUBMM_H_


/***********************************************************
* Register Name : GDCSHUB_RAS_CENTRAL_STATUS
* Register Description :
* GDCSHUB RAS CENTRAL STATUS register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : local EgStall_det.
#define GDCSHUB_RAS_CENTRAL_STATUS_GDCSHUB_L2C_EgStall_det_OFFSET      0
#define GDCSHUB_RAS_CENTRAL_STATUS_GDCSHUB_L2C_EgStall_det_MASK        0x1

// Bitfield Description : local ErrEvent_det.
#define GDCSHUB_RAS_CENTRAL_STATUS_GDCSHUB_L2C_ErrEvent_det_OFFSET      1
#define GDCSHUB_RAS_CENTRAL_STATUS_GDCSHUB_L2C_ErrEvent_det_MASK        0x2

// Bitfield Description : propagated EgStall_det.
#define GDCSHUB_RAS_CENTRAL_STATUS_GDCSHUB_C2L_EgStall_det_OFFSET      2
#define GDCSHUB_RAS_CENTRAL_STATUS_GDCSHUB_C2L_EgStall_det_MASK        0x4

// Bitfield Description : propagated ErrEvent_det.
#define GDCSHUB_RAS_CENTRAL_STATUS_GDCSHUB_C2L_ErrEvent_det_OFFSET      3
#define GDCSHUB_RAS_CENTRAL_STATUS_GDCSHUB_C2L_ErrEvent_det_MASK        0x8

// Bitfield Description : 
#define GDCSHUB_RAS_CENTRAL_STATUS_Reserved_31_4_OFFSET      4
#define GDCSHUB_RAS_CENTRAL_STATUS_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            GDCSHUB_L2C_EgStall_det:1;
    UINT32                            GDCSHUB_L2C_ErrEvent_det:1;
    UINT32                            GDCSHUB_C2L_EgStall_det:1;
    UINT32                            GDCSHUB_C2L_ErrEvent_det:1;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} GDCSHUB_RAS_CENTRAL_STATUS_STRUCT;

#define SMN_NBIF0_GDCSHUB_RAS_CENTRAL_STATUS_ADDRESS    0x1418840UL


/***********************************************************
* Register Name : GDCSHUB_RAS_LEAF0_STATUS
* Register Description :
* GDCSHUB RAS LEAF0 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf0 status for SDP error event receive recording.
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_ERR_EVENT_RECV_OFFSET      0
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_ERR_EVENT_RECV_MASK        0x1

// Bitfield Description : RAS Leaf0 status for self-detected poison data recording.
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_POISON_ERR_DET_OFFSET      1
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_POISON_ERR_DET_MASK        0x2

// Bitfield Description : RAS Leaf0 status for self-detected parity error recording.
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_PARITY_ERR_DET_OFFSET      2
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_PARITY_ERR_DET_MASK        0x4

// Bitfield Description : 
#define GDCSHUB_RAS_LEAF0_STATUS_Reserved_7_3_OFFSET      3
#define GDCSHUB_RAS_LEAF0_STATUS_Reserved_7_3_MASK        0xf8

// Bitfield Description : RAS Leaf0 has sent errEvent from self-generated error.
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_ERR_EVENT_GENN_STAT_OFFSET      8
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_ERR_EVENT_GENN_STAT_MASK        0x100

// Bitfield Description : RAS Leaf0 has stalled egress port from self-generated error.
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_EGRESS_STALLED_GENN_STAT_OFFSET      9
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_EGRESS_STALLED_GENN_STAT_MASK        0x200

// Bitfield Description : RAS Leaf0 sent error event from propagated errEvent.
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_ERR_EVENT_PROP_STAT_OFFSET      10
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_ERR_EVENT_PROP_STAT_MASK        0x400

// Bitfield Description : RAS Leaf0 stalled egress port from propagated stall.
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_EGRESS_STALLED_PROP_STAT_OFFSET      11
#define GDCSHUB_RAS_LEAF0_STATUS_GDCSHUB_RAS_LEAF0_STATUS_EGRESS_STALLED_PROP_STAT_MASK        0x800

// Bitfield Description : 
#define GDCSHUB_RAS_LEAF0_STATUS_Reserved_31_12_OFFSET      12
#define GDCSHUB_RAS_LEAF0_STATUS_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            GDCSHUB_RAS_LEAF0_STATUS_ERR_EVENT_RECV:1;
    UINT32                            GDCSHUB_RAS_LEAF0_STATUS_POISON_ERR_DET:1;
    UINT32                            GDCSHUB_RAS_LEAF0_STATUS_PARITY_ERR_DET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            GDCSHUB_RAS_LEAF0_STATUS_ERR_EVENT_GENN_STAT:1;
    UINT32                            GDCSHUB_RAS_LEAF0_STATUS_EGRESS_STALLED_GENN_STAT:1;
    UINT32                            GDCSHUB_RAS_LEAF0_STATUS_ERR_EVENT_PROP_STAT:1;
    UINT32                            GDCSHUB_RAS_LEAF0_STATUS_EGRESS_STALLED_PROP_STAT:1;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} GDCSHUB_RAS_LEAF0_STATUS_STRUCT;

#define SMN_NBIF0_GDCSHUB_RAS_LEAF0_STATUS_ADDRESS    0x14188c0UL


/***********************************************************
* Register Name : GDCSHUB_RAS_LEAF1_STATUS
* Register Description :
* GDCSHUB RAS LEAF1 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf1 status for SDP error event receive recording.
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_ERR_EVENT_RECV_OFFSET      0
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_ERR_EVENT_RECV_MASK        0x1

// Bitfield Description : RAS Leaf1 status for self-detected poison data recording.
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_POISON_ERR_DET_OFFSET      1
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_POISON_ERR_DET_MASK        0x2

// Bitfield Description : RAS Leaf1 status for self-detected parity error recording.
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_PARITY_ERR_DET_OFFSET      2
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_PARITY_ERR_DET_MASK        0x4

// Bitfield Description : 
#define GDCSHUB_RAS_LEAF1_STATUS_Reserved_7_3_OFFSET      3
#define GDCSHUB_RAS_LEAF1_STATUS_Reserved_7_3_MASK        0xf8

// Bitfield Description : RAS Leaf1 has sent errEvent from self-generated error.
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_ERR_EVENT_GENN_STAT_OFFSET      8
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_ERR_EVENT_GENN_STAT_MASK        0x100

// Bitfield Description : RAS Leaf1 has stalled egress port from self-generated error.
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_EGRESS_STALLED_GENN_STAT_OFFSET      9
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_EGRESS_STALLED_GENN_STAT_MASK        0x200

// Bitfield Description : RAS Leaf1 sent error event from propagated errEvent.
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_ERR_EVENT_PROP_STAT_OFFSET      10
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_ERR_EVENT_PROP_STAT_MASK        0x400

// Bitfield Description : RAS Leaf1 stalled egress port from propagated stall.
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_EGRESS_STALLED_PROP_STAT_OFFSET      11
#define GDCSHUB_RAS_LEAF1_STATUS_GDCSHUB_RAS_LEAF1_STATUS_EGRESS_STALLED_PROP_STAT_MASK        0x800

// Bitfield Description : 
#define GDCSHUB_RAS_LEAF1_STATUS_Reserved_31_12_OFFSET      12
#define GDCSHUB_RAS_LEAF1_STATUS_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            GDCSHUB_RAS_LEAF1_STATUS_ERR_EVENT_RECV:1;
    UINT32                            GDCSHUB_RAS_LEAF1_STATUS_POISON_ERR_DET:1;
    UINT32                            GDCSHUB_RAS_LEAF1_STATUS_PARITY_ERR_DET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            GDCSHUB_RAS_LEAF1_STATUS_ERR_EVENT_GENN_STAT:1;
    UINT32                            GDCSHUB_RAS_LEAF1_STATUS_EGRESS_STALLED_GENN_STAT:1;
    UINT32                            GDCSHUB_RAS_LEAF1_STATUS_ERR_EVENT_PROP_STAT:1;
    UINT32                            GDCSHUB_RAS_LEAF1_STATUS_EGRESS_STALLED_PROP_STAT:1;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} GDCSHUB_RAS_LEAF1_STATUS_STRUCT;

#define SMN_NBIF0_GDCSHUB_RAS_LEAF1_STATUS_ADDRESS    0x14188c4UL


/***********************************************************
* Register Name : GDCSOC_RAS_CENTRAL_STATUS
* Register Description :
* GDCSOC RAS CENTRAL STATUS register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : local EgStall_det.
#define GDCSOC_RAS_CENTRAL_STATUS_GDCSOC_L2C_EgStall_det_OFFSET      0
#define GDCSOC_RAS_CENTRAL_STATUS_GDCSOC_L2C_EgStall_det_MASK        0x1

// Bitfield Description : local ErrEvent_det.
#define GDCSOC_RAS_CENTRAL_STATUS_GDCSOC_L2C_ErrEvent_det_OFFSET      1
#define GDCSOC_RAS_CENTRAL_STATUS_GDCSOC_L2C_ErrEvent_det_MASK        0x2

// Bitfield Description : propagated EgStall_det.
#define GDCSOC_RAS_CENTRAL_STATUS_GDCSOC_C2L_EgStall_det_OFFSET      2
#define GDCSOC_RAS_CENTRAL_STATUS_GDCSOC_C2L_EgStall_det_MASK        0x4

// Bitfield Description : propagated ErrEvent_det.
#define GDCSOC_RAS_CENTRAL_STATUS_GDCSOC_C2L_ErrEvent_det_OFFSET      3
#define GDCSOC_RAS_CENTRAL_STATUS_GDCSOC_C2L_ErrEvent_det_MASK        0x8

// Bitfield Description : 
#define GDCSOC_RAS_CENTRAL_STATUS_Reserved_31_4_OFFSET      4
#define GDCSOC_RAS_CENTRAL_STATUS_Reserved_31_4_MASK        0xfffffff0

typedef union {
  struct {
    UINT32                            GDCSOC_L2C_EgStall_det:1;
    UINT32                            GDCSOC_L2C_ErrEvent_det:1;
    UINT32                            GDCSOC_C2L_EgStall_det:1;
    UINT32                            GDCSOC_C2L_ErrEvent_det:1;
    UINT32                            Reserved_31_4:28;
  } Field;
  UINT32 Value;
} GDCSOC_RAS_CENTRAL_STATUS_STRUCT;

#define SMN_NBIF0_GDCSOC_RAS_CENTRAL_STATUS_ADDRESS    0x1418040UL


/***********************************************************
* Register Name : GDCSOC_RAS_LEAF0_CTRL
* Register Description :
* GDCSOC RAS LEAF0 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf0 control bit for port err event detection.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_ERR_EVENT_DET_EN_OFFSET      0
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_ERR_EVENT_DET_EN_MASK        0x1

// Bitfield Description : RAS Leaf0 control for sending error event when dectecting poison data.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_POISON_ERREVENT_EN_OFFSET      1
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_POISON_ERREVENT_EN_MASK        0x2

// Bitfield Description : RAS Leaf0 control for stalling its egress port when dectecting poison data.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_POISON_STALL_EN_OFFSET      2
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_POISON_STALL_EN_MASK        0x4

// Bitfield Description : RAS Leaf0 control for sending error event when dectecting parity error.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_PARITY_ERREVENT_EN_OFFSET      3
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_PARITY_ERREVENT_EN_MASK        0x8

// Bitfield Description : RAS Leaf0 control for stalling its egress port when dectecting parity error.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_PARITY_STALL_EN_OFFSET      4
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_PARITY_STALL_EN_MASK        0x10

// Bitfield Description : RAS Leaf0 control for sending error event when dectecting parity error.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_RCVERREVENT_ERREVENT_EN_OFFSET      5
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_RCVERREVENT_ERREVENT_EN_MASK        0x20

// Bitfield Description : RAS Leaf0 control for stalling its egress port when dectecting parity error.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_RCVERREVENT_STALL_EN_OFFSET      6
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_RCVERREVENT_STALL_EN_MASK        0x40

// Bitfield Description : 
#define GDCSOC_RAS_LEAF0_CTRL_Reserved_7_7_OFFSET      7
#define GDCSOC_RAS_LEAF0_CTRL_Reserved_7_7_MASK        0x80

// Bitfield Description : RAS Leaf0 control to propagate errEvent.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_ERR_EVENT_GEN_EN_OFFSET      8
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_ERR_EVENT_GEN_EN_MASK        0x100

// Bitfield Description : RAS Leaf0 control  to propagate egress stall.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_EGRESS_STALL_GEN_EN_OFFSET      9
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_EGRESS_STALL_GEN_EN_MASK        0x200

// Bitfield Description : RAS Leaf0 control to propagate errEvent.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_ERR_EVENT_PROP_EN_OFFSET      10
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_ERR_EVENT_PROP_EN_MASK        0x400

// Bitfield Description : RAS Leaf0 control  to propagate egress stall.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_EGRESS_STALL_PROP_EN_OFFSET      11
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_EGRESS_STALL_PROP_EN_MASK        0x800

// Bitfield Description : RAS Leaf0 control for pollute data to poison.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_POISON_DBUG_EN_OFFSET      12
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_POISON_DBUG_EN_MASK        0x1000

// Bitfield Description : RAS Leaf0 control for pollute data to parity.
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_PARITY_DBUG_EN_OFFSET      13
#define GDCSOC_RAS_LEAF0_CTRL_GDCSOC_RAS_LEAF0_CTRL_PARITY_DBUG_EN_MASK        0x2000

// Bitfield Description : 
#define GDCSOC_RAS_LEAF0_CTRL_Reserved_31_14_OFFSET      14
#define GDCSOC_RAS_LEAF0_CTRL_Reserved_31_14_MASK        0xffffc000

typedef union {
  struct {
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_ERR_EVENT_DET_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_POISON_ERREVENT_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_POISON_STALL_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_PARITY_ERREVENT_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_PARITY_STALL_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_RCVERREVENT_ERREVENT_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_RCVERREVENT_STALL_EN:1;
    UINT32                            Reserved_7_7:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_ERR_EVENT_GEN_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_EGRESS_STALL_GEN_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_ERR_EVENT_PROP_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_EGRESS_STALL_PROP_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_POISON_DBUG_EN:1;
    UINT32                            GDCSOC_RAS_LEAF0_CTRL_PARITY_DBUG_EN:1;
    UINT32                            Reserved_31_14:18;
  } Field;
  UINT32 Value;
} GDCSOC_RAS_LEAF0_CTRL_STRUCT;

#define SMN_NBIF0_GDCSOC_RAS_LEAF0_CTRL_ADDRESS    0x1418080UL


/***********************************************************
* Register Name : GDCSOC_RAS_LEAF0_STATUS
* Register Description :
* GDCSOC RAS LEAF0 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf0 status for SDP error event receive recording.
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_ERR_EVENT_RECV_OFFSET      0
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_ERR_EVENT_RECV_MASK        0x1

// Bitfield Description : RAS Leaf0 status for self-detected poison data recording.
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_POISON_ERR_DET_OFFSET      1
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_POISON_ERR_DET_MASK        0x2

// Bitfield Description : RAS Leaf0 status for self-detected parity error recording.
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_PARITY_ERR_DET_OFFSET      2
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_PARITY_ERR_DET_MASK        0x4

// Bitfield Description : 
#define GDCSOC_RAS_LEAF0_STATUS_Reserved_7_3_OFFSET      3
#define GDCSOC_RAS_LEAF0_STATUS_Reserved_7_3_MASK        0xf8

// Bitfield Description : RAS Leaf0 has sent errEvent from self-generated error.
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_ERR_EVENT_GENN_STAT_OFFSET      8
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_ERR_EVENT_GENN_STAT_MASK        0x100

// Bitfield Description : RAS Leaf0 has stalled egress port from self-generated error.
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_EGRESS_STALLED_GENN_STAT_OFFSET      9
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_EGRESS_STALLED_GENN_STAT_MASK        0x200

// Bitfield Description : RAS Leaf0 sent error event from propagated errEvent.
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_ERR_EVENT_PROP_STAT_OFFSET      10
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_ERR_EVENT_PROP_STAT_MASK        0x400

// Bitfield Description : RAS Leaf0 stalled egress port from propagated stall.
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_EGRESS_STALLED_PROP_STAT_OFFSET      11
#define GDCSOC_RAS_LEAF0_STATUS_GDCSOC_RAS_LEAF0_STATUS_EGRESS_STALLED_PROP_STAT_MASK        0x800

// Bitfield Description : 
#define GDCSOC_RAS_LEAF0_STATUS_Reserved_31_12_OFFSET      12
#define GDCSOC_RAS_LEAF0_STATUS_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            GDCSOC_RAS_LEAF0_STATUS_ERR_EVENT_RECV:1;
    UINT32                            GDCSOC_RAS_LEAF0_STATUS_POISON_ERR_DET:1;
    UINT32                            GDCSOC_RAS_LEAF0_STATUS_PARITY_ERR_DET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            GDCSOC_RAS_LEAF0_STATUS_ERR_EVENT_GENN_STAT:1;
    UINT32                            GDCSOC_RAS_LEAF0_STATUS_EGRESS_STALLED_GENN_STAT:1;
    UINT32                            GDCSOC_RAS_LEAF0_STATUS_ERR_EVENT_PROP_STAT:1;
    UINT32                            GDCSOC_RAS_LEAF0_STATUS_EGRESS_STALLED_PROP_STAT:1;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} GDCSOC_RAS_LEAF0_STATUS_STRUCT;

#define SMN_NBIF0_GDCSOC_RAS_LEAF0_STATUS_ADDRESS    0x14180c0UL


/***********************************************************
* Register Name : GDCSOC_RAS_LEAF1_CTRL
* Register Description :
* GDCSOC RAS LEAF1 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf1 control bit for port err event detection.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_ERR_EVENT_DET_EN_OFFSET      0
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_ERR_EVENT_DET_EN_MASK        0x1

// Bitfield Description : RAS Leaf1 control for sending error event when dectecting poison data.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_POISON_ERREVENT_EN_OFFSET      1
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_POISON_ERREVENT_EN_MASK        0x2

// Bitfield Description : RAS Leaf1 control for stalling its egress port when dectecting poison data.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_POISON_STALL_EN_OFFSET      2
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_POISON_STALL_EN_MASK        0x4

// Bitfield Description : RAS Leaf1 control for sending error event when dectecting parity error.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_PARITY_ERREVENT_EN_OFFSET      3
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_PARITY_ERREVENT_EN_MASK        0x8

// Bitfield Description : RAS Leaf1 control for stalling its egress port when dectecting parity error.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_PARITY_STALL_EN_OFFSET      4
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_PARITY_STALL_EN_MASK        0x10

// Bitfield Description : RAS Leaf1 control for sending error event when dectecting parity error.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_RCVERREVENT_ERREVENT_EN_OFFSET      5
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_RCVERREVENT_ERREVENT_EN_MASK        0x20

// Bitfield Description : RAS Leaf1 control for stalling its egress port when dectecting parity error.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_RCVERREVENT_STALL_EN_OFFSET      6
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_RCVERREVENT_STALL_EN_MASK        0x40

// Bitfield Description : 
#define GDCSOC_RAS_LEAF1_CTRL_Reserved_7_7_OFFSET      7
#define GDCSOC_RAS_LEAF1_CTRL_Reserved_7_7_MASK        0x80

// Bitfield Description : RAS Leaf1 control to propagate errEvent.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_ERR_EVENT_GEN_EN_OFFSET      8
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_ERR_EVENT_GEN_EN_MASK        0x100

// Bitfield Description : RAS Leaf1 control  to propagate egress stall.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_EGRESS_STALL_GEN_EN_OFFSET      9
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_EGRESS_STALL_GEN_EN_MASK        0x200

// Bitfield Description : RAS Leaf1 control to propagate errEvent.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_ERR_EVENT_PROP_EN_OFFSET      10
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_ERR_EVENT_PROP_EN_MASK        0x400

// Bitfield Description : RAS Leaf1 control  to propagate egress stall.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_EGRESS_STALL_PROP_EN_OFFSET      11
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_EGRESS_STALL_PROP_EN_MASK        0x800

// Bitfield Description : RAS Leaf1 control for pollute data to poison.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_POISON_DBUG_EN_OFFSET      12
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_POISON_DBUG_EN_MASK        0x1000

// Bitfield Description : RAS Leaf1 control for pollute data to parity.
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_PARITY_DBUG_EN_OFFSET      13
#define GDCSOC_RAS_LEAF1_CTRL_GDCSOC_RAS_LEAF1_CTRL_PARITY_DBUG_EN_MASK        0x2000

// Bitfield Description : 
#define GDCSOC_RAS_LEAF1_CTRL_Reserved_31_14_OFFSET      14
#define GDCSOC_RAS_LEAF1_CTRL_Reserved_31_14_MASK        0xffffc000

typedef union {
  struct {
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_ERR_EVENT_DET_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_POISON_ERREVENT_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_POISON_STALL_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_PARITY_ERREVENT_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_PARITY_STALL_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_RCVERREVENT_ERREVENT_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_RCVERREVENT_STALL_EN:1;
    UINT32                            Reserved_7_7:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_ERR_EVENT_GEN_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_EGRESS_STALL_GEN_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_ERR_EVENT_PROP_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_EGRESS_STALL_PROP_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_POISON_DBUG_EN:1;
    UINT32                            GDCSOC_RAS_LEAF1_CTRL_PARITY_DBUG_EN:1;
    UINT32                            Reserved_31_14:18;
  } Field;
  UINT32 Value;
} GDCSOC_RAS_LEAF1_CTRL_STRUCT;

#define SMN_NBIF0_GDCSOC_RAS_LEAF1_CTRL_ADDRESS    0x1418084UL


/***********************************************************
* Register Name : GDCSOC_RAS_LEAF1_STATUS
* Register Description :
* GDCSOC RAS LEAF1 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf1 status for SDP error event receive recording.
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_ERR_EVENT_RECV_OFFSET      0
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_ERR_EVENT_RECV_MASK        0x1

// Bitfield Description : RAS Leaf1 status for self-detected poison data recording.
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_POISON_ERR_DET_OFFSET      1
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_POISON_ERR_DET_MASK        0x2

// Bitfield Description : RAS Leaf1 status for self-detected parity error recording.
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_PARITY_ERR_DET_OFFSET      2
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_PARITY_ERR_DET_MASK        0x4

// Bitfield Description : 
#define GDCSOC_RAS_LEAF1_STATUS_Reserved_7_3_OFFSET      3
#define GDCSOC_RAS_LEAF1_STATUS_Reserved_7_3_MASK        0xf8

// Bitfield Description : RAS Leaf1 has sent errEvent from self-generated error.
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_ERR_EVENT_GENN_STAT_OFFSET      8
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_ERR_EVENT_GENN_STAT_MASK        0x100

// Bitfield Description : RAS Leaf1 has stalled egress port from self-generated error.
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_EGRESS_STALLED_GENN_STAT_OFFSET      9
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_EGRESS_STALLED_GENN_STAT_MASK        0x200

// Bitfield Description : RAS Leaf1 sent error event from propagated errEvent.
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_ERR_EVENT_PROP_STAT_OFFSET      10
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_ERR_EVENT_PROP_STAT_MASK        0x400

// Bitfield Description : RAS Leaf1 stalled egress port from propagated stall.
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_EGRESS_STALLED_PROP_STAT_OFFSET      11
#define GDCSOC_RAS_LEAF1_STATUS_GDCSOC_RAS_LEAF1_STATUS_EGRESS_STALLED_PROP_STAT_MASK        0x800

// Bitfield Description : 
#define GDCSOC_RAS_LEAF1_STATUS_Reserved_31_12_OFFSET      12
#define GDCSOC_RAS_LEAF1_STATUS_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            GDCSOC_RAS_LEAF1_STATUS_ERR_EVENT_RECV:1;
    UINT32                            GDCSOC_RAS_LEAF1_STATUS_POISON_ERR_DET:1;
    UINT32                            GDCSOC_RAS_LEAF1_STATUS_PARITY_ERR_DET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            GDCSOC_RAS_LEAF1_STATUS_ERR_EVENT_GENN_STAT:1;
    UINT32                            GDCSOC_RAS_LEAF1_STATUS_EGRESS_STALLED_GENN_STAT:1;
    UINT32                            GDCSOC_RAS_LEAF1_STATUS_ERR_EVENT_PROP_STAT:1;
    UINT32                            GDCSOC_RAS_LEAF1_STATUS_EGRESS_STALLED_PROP_STAT:1;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} GDCSOC_RAS_LEAF1_STATUS_STRUCT;

#define SMN_NBIF0_GDCSOC_RAS_LEAF1_STATUS_ADDRESS    0x14180c4UL

#endif /* _SYSHUBMM_H_ */
