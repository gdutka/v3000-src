/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _NBIFMM_H_
#define _NBIFMM_H_


/***********************************************************
* Register Name : BIFL_RAS_CENTRAL_CNTL
* Register Description :
* BIFL RAS CENTRAL Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description :
#define BIFL_RAS_CENTRAL_CNTL_Reserved_28_0_OFFSET      0
#define BIFL_RAS_CENTRAL_CNTL_Reserved_28_0_MASK        0x1fffffff

// Bitfield Description : 0, send sdp err_event to all nBIF's SDP orig port and stall all nBIF's egress port when recieved ras err_event from RAS controller; 1, disable this feature.
#define BIFL_RAS_CENTRAL_CNTL_BIFL_RAS_CONTL_ERREVENT_DIS_OFFSET      29
#define BIFL_RAS_CENTRAL_CNTL_BIFL_RAS_CONTL_ERREVENT_DIS_MASK        0x20000000

// Bitfield Description : 0, send intrrupt upstream to GFX driver when recieved ras_intr from RAS controller; 1, disable this feature.
#define BIFL_RAS_CENTRAL_CNTL_BIFL_RAS_CONTL_INTR_DIS_OFFSET      30
#define BIFL_RAS_CENTRAL_CNTL_BIFL_RAS_CONTL_INTR_DIS_MASK        0x40000000

// Bitfield Description : 0,send egress stall when receive LinkDis; 1, disable send.
#define BIFL_RAS_CENTRAL_CNTL_BIFL_LINKDIS_TRIG_EGRESS_STALL_DIS_OFFSET      31
#define BIFL_RAS_CENTRAL_CNTL_BIFL_LINKDIS_TRIG_EGRESS_STALL_DIS_MASK        0x80000000

typedef union {
  struct {
    UINT32                            Reserved_28_0:29;
    UINT32                            BIFL_RAS_CONTL_ERREVENT_DIS:1;
    UINT32                            BIFL_RAS_CONTL_INTR_DIS:1;
    UINT32                            BIFL_LINKDIS_TRIG_EGRESS_STALL_DIS:1;
  } Field;
  UINT32 Value;
} BIFL_RAS_CENTRAL_CNTL_STRUCT;

#define SMN_NBIF0_BIFL_RAS_CENTRAL_CNTL_ADDRESS    0x10139000UL


/***********************************************************
* Register Name : BIFL_RAS_CENTRAL_STATUS
* Register Description :
* BIF RAS CENTRAL STATUS register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : local EgStall_det.
#define BIFL_RAS_CENTRAL_STATUS_BIFL_L2C_EgStall_det_OFFSET      0
#define BIFL_RAS_CENTRAL_STATUS_BIFL_L2C_EgStall_det_MASK        0x1

// Bitfield Description : local ErrEvent_det.
#define BIFL_RAS_CENTRAL_STATUS_BIFL_L2C_ErrEvent_det_OFFSET      1
#define BIFL_RAS_CENTRAL_STATUS_BIFL_L2C_ErrEvent_det_MASK        0x2

// Bitfield Description : propagated EgStall_det.
#define BIFL_RAS_CENTRAL_STATUS_BIFL_C2L_EgStall_det_OFFSET      2
#define BIFL_RAS_CENTRAL_STATUS_BIFL_C2L_EgStall_det_MASK        0x4

// Bitfield Description : propagated ErrEvent_det.
#define BIFL_RAS_CENTRAL_STATUS_BIFL_C2L_ErrEvent_det_OFFSET      3
#define BIFL_RAS_CENTRAL_STATUS_BIFL_C2L_ErrEvent_det_MASK        0x8

// Bitfield Description :
#define BIFL_RAS_CENTRAL_STATUS_Reserved_28_4_OFFSET      4
#define BIFL_RAS_CENTRAL_STATUS_Reserved_28_4_MASK        0x1ffffff0

// Bitfield Description : recieved ras err_event from RAS controller.
#define BIFL_RAS_CENTRAL_STATUS_BIFL_RasContller_ErrEvent_Recv_OFFSET      29
#define BIFL_RAS_CENTRAL_STATUS_BIFL_RasContller_ErrEvent_Recv_MASK        0x20000000

// Bitfield Description : recieved ras_intr from RAS controller.
#define BIFL_RAS_CENTRAL_STATUS_BIFL_RasContller_Intr_Recv_OFFSET      30
#define BIFL_RAS_CENTRAL_STATUS_BIFL_RasContller_Intr_Recv_MASK        0x40000000

// Bitfield Description : received LinkDis.
#define BIFL_RAS_CENTRAL_STATUS_BIFL_LinkDis_Recv_OFFSET      31
#define BIFL_RAS_CENTRAL_STATUS_BIFL_LinkDis_Recv_MASK        0x80000000

typedef union {
  struct {
    UINT32                            BIFL_L2C_EgStall_det:1;
    UINT32                            BIFL_L2C_ErrEvent_det:1;
    UINT32                            BIFL_C2L_EgStall_det:1;
    UINT32                            BIFL_C2L_ErrEvent_det:1;
    UINT32                            Reserved_28_4:25;
    UINT32                            BIFL_RasContller_ErrEvent_Recv:1;
    UINT32                            BIFL_RasContller_Intr_Recv:1;
    UINT32                            BIFL_LinkDis_Recv:1;
  } Field;
  UINT32 Value;
} BIFL_RAS_CENTRAL_STATUS_STRUCT;

#define SMN_NBIF0_BIFL_RAS_CENTRAL_STATUS_ADDRESS    0x10139040UL


/***********************************************************
* Register Name : BIFL_RAS_LEAF0_CTRL
* Register Description :
* BIFL RAS LEAF0 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf0 control bit for port err event detection.
#define BIFL_RAS_LEAF0_CTRL_ERR_EVENT_DET_EN_OFFSET      0
#define BIFL_RAS_LEAF0_CTRL_ERR_EVENT_DET_EN_MASK        0x1

// Bitfield Description : RAS Leaf0 control for sending error event when dectecting poison data.
#define BIFL_RAS_LEAF0_CTRL_POISON_ERREVENT_EN_OFFSET      1
#define BIFL_RAS_LEAF0_CTRL_POISON_ERREVENT_EN_MASK        0x2

// Bitfield Description : RAS Leaf0 control for stalling its egress port when dectecting poison data.
#define BIFL_RAS_LEAF0_CTRL_POISON_STALL_EN_OFFSET      2
#define BIFL_RAS_LEAF0_CTRL_POISON_STALL_EN_MASK        0x4

// Bitfield Description : RAS Leaf0 control for sending error event when dectecting parity error.
#define BIFL_RAS_LEAF0_CTRL_PARITY_ERREVENT_EN_OFFSET      3
#define BIFL_RAS_LEAF0_CTRL_PARITY_ERREVENT_EN_MASK        0x8

// Bitfield Description : RAS Leaf0 control for stalling its egress port when dectecting parity error.
#define BIFL_RAS_LEAF0_CTRL_PARITY_STALL_EN_OFFSET      4
#define BIFL_RAS_LEAF0_CTRL_PARITY_STALL_EN_MASK        0x10

// Bitfield Description : RAS Leaf0 control for sending error event when recieve a errevnet.
#define BIFL_RAS_LEAF0_CTRL_RCVERREVENT_ERREVENT_EN_OFFSET      5
#define BIFL_RAS_LEAF0_CTRL_RCVERREVENT_ERREVENT_EN_MASK        0x20

// Bitfield Description : RAS Leaf0 control for stalling its egress port when recieve a errevnet.
#define BIFL_RAS_LEAF0_CTRL_RCVERREVENT_STALL_EN_OFFSET      6
#define BIFL_RAS_LEAF0_CTRL_RCVERREVENT_STALL_EN_MASK        0x40

// Bitfield Description :
#define BIFL_RAS_LEAF0_CTRL_Reserved_7_7_OFFSET      7
#define BIFL_RAS_LEAF0_CTRL_Reserved_7_7_MASK        0x80

// Bitfield Description : RAS Leaf0 control to propagate errEvent.
#define BIFL_RAS_LEAF0_CTRL_ERR_EVENT_GEN_EN_OFFSET      8
#define BIFL_RAS_LEAF0_CTRL_ERR_EVENT_GEN_EN_MASK        0x100

// Bitfield Description : RAS Leaf0 control  to propagate egress stall.
#define BIFL_RAS_LEAF0_CTRL_EGRESS_STALL_GEN_EN_OFFSET      9
#define BIFL_RAS_LEAF0_CTRL_EGRESS_STALL_GEN_EN_MASK        0x200

// Bitfield Description : RAS Leaf0 control to propagate errEvent.
#define BIFL_RAS_LEAF0_CTRL_ERR_EVENT_PROP_EN_OFFSET      10
#define BIFL_RAS_LEAF0_CTRL_ERR_EVENT_PROP_EN_MASK        0x400

// Bitfield Description : RAS Leaf0 control  to propagate egress stall.
#define BIFL_RAS_LEAF0_CTRL_EGRESS_STALL_PROP_EN_OFFSET      11
#define BIFL_RAS_LEAF0_CTRL_EGRESS_STALL_PROP_EN_MASK        0x800

// Bitfield Description : RAS Leaf0 control for pollute data to poison.
#define BIFL_RAS_LEAF0_CTRL_POISON_DBUG_EN_OFFSET      12
#define BIFL_RAS_LEAF0_CTRL_POISON_DBUG_EN_MASK        0x1000

// Bitfield Description : RAS Leaf0 control for pollute data to parity.
#define BIFL_RAS_LEAF0_CTRL_PARITY_DBUG_EN_OFFSET      13
#define BIFL_RAS_LEAF0_CTRL_PARITY_DBUG_EN_MASK        0x2000

// Bitfield Description :
#define BIFL_RAS_LEAF0_CTRL_Reserved_15_14_OFFSET      14
#define BIFL_RAS_LEAF0_CTRL_Reserved_15_14_MASK        0xc000

// Bitfield Description : Only for athub.
#define BIFL_RAS_LEAF0_CTRL_ERR_EVENT_RAS_INTR_EN_OFFSET      16
#define BIFL_RAS_LEAF0_CTRL_ERR_EVENT_RAS_INTR_EN_MASK        0x10000

// Bitfield Description :
#define BIFL_RAS_LEAF0_CTRL_Reserved_31_17_OFFSET      17
#define BIFL_RAS_LEAF0_CTRL_Reserved_31_17_MASK        0xfffe0000

typedef union {
  struct {
    UINT32                            ERR_EVENT_DET_EN:1;
    UINT32                            POISON_ERREVENT_EN:1;
    UINT32                            POISON_STALL_EN:1;
    UINT32                            PARITY_ERREVENT_EN:1;
    UINT32                            PARITY_STALL_EN:1;
    UINT32                            RCVERREVENT_ERREVENT_EN:1;
    UINT32                            RCVERREVENT_STALL_EN:1;
    UINT32                            Reserved_7_7:1;
    UINT32                            ERR_EVENT_GEN_EN:1;
    UINT32                            EGRESS_STALL_GEN_EN:1;
    UINT32                            ERR_EVENT_PROP_EN:1;
    UINT32                            EGRESS_STALL_PROP_EN:1;
    UINT32                            POISON_DBUG_EN:1;
    UINT32                            PARITY_DBUG_EN:1;
    UINT32                            Reserved_15_14:2;
    UINT32                            ERR_EVENT_RAS_INTR_EN:1;
    UINT32                            Reserved_31_17:15;
  } Field;
  UINT32 Value;
} BIFL_RAS_LEAF0_CTRL_STRUCT;

#define SMN_NBIF0_BIFL_RAS_LEAF0_CTRL_ADDRESS    0x10139080UL


/***********************************************************
* Register Name : BIFL_RAS_LEAF0_STATUS
* Register Description :
* BIFL RAS LEAF0 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf0 status for SDP error event receive recording.
#define BIFL_RAS_LEAF0_STATUS_ERR_EVENT_RECV_OFFSET      0
#define BIFL_RAS_LEAF0_STATUS_ERR_EVENT_RECV_MASK        0x1

// Bitfield Description : RAS Leaf0 status for self-detected poison data recording.
#define BIFL_RAS_LEAF0_STATUS_POISON_ERR_DET_OFFSET      1
#define BIFL_RAS_LEAF0_STATUS_POISON_ERR_DET_MASK        0x2

// Bitfield Description : RAS Leaf0 status for self-detected parity error recording.
#define BIFL_RAS_LEAF0_STATUS_PARITY_ERR_DET_OFFSET      2
#define BIFL_RAS_LEAF0_STATUS_PARITY_ERR_DET_MASK        0x4

// Bitfield Description :
#define BIFL_RAS_LEAF0_STATUS_Reserved_7_3_OFFSET      3
#define BIFL_RAS_LEAF0_STATUS_Reserved_7_3_MASK        0xf8

// Bitfield Description : RAS Leaf0 has sent errEvent from self-generated error.
#define BIFL_RAS_LEAF0_STATUS_ERR_EVENT_GENN_STAT_OFFSET      8
#define BIFL_RAS_LEAF0_STATUS_ERR_EVENT_GENN_STAT_MASK        0x100

// Bitfield Description : RAS Leaf0 has stalled egress port from self-generated error.
#define BIFL_RAS_LEAF0_STATUS_EGRESS_STALLED_GENN_STAT_OFFSET      9
#define BIFL_RAS_LEAF0_STATUS_EGRESS_STALLED_GENN_STAT_MASK        0x200

// Bitfield Description : RAS Leaf0 sent error event from propagated errEvent.
#define BIFL_RAS_LEAF0_STATUS_ERR_EVENT_PROP_STAT_OFFSET      10
#define BIFL_RAS_LEAF0_STATUS_ERR_EVENT_PROP_STAT_MASK        0x400

// Bitfield Description : RAS Leaf0 stalled egress port from propagated stall.
#define BIFL_RAS_LEAF0_STATUS_EGRESS_STALLED_PROP_STAT_OFFSET      11
#define BIFL_RAS_LEAF0_STATUS_EGRESS_STALLED_PROP_STAT_MASK        0x800

// Bitfield Description :
#define BIFL_RAS_LEAF0_STATUS_Reserved_31_12_OFFSET      12
#define BIFL_RAS_LEAF0_STATUS_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            ERR_EVENT_RECV:1;
    UINT32                            POISON_ERR_DET:1;
    UINT32                            PARITY_ERR_DET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            ERR_EVENT_GENN_STAT:1;
    UINT32                            EGRESS_STALLED_GENN_STAT:1;
    UINT32                            ERR_EVENT_PROP_STAT:1;
    UINT32                            EGRESS_STALLED_PROP_STAT:1;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} BIFL_RAS_LEAF0_STATUS_STRUCT;

#define SMN_NBIF0_BIFL_RAS_LEAF0_STATUS_ADDRESS    0x101390c0UL


/***********************************************************
* Register Name : BIFL_RAS_LEAF1_CTRL
* Register Description :
* BIFL RAS LEAF1 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf1 control bit for port err event detection.
#define BIFL_RAS_LEAF1_CTRL_ERR_EVENT_DET_EN_OFFSET      0
#define BIFL_RAS_LEAF1_CTRL_ERR_EVENT_DET_EN_MASK        0x1

// Bitfield Description : RAS Leaf1 control for sending error event when dectecting poison data.
#define BIFL_RAS_LEAF1_CTRL_POISON_ERREVENT_EN_OFFSET      1
#define BIFL_RAS_LEAF1_CTRL_POISON_ERREVENT_EN_MASK        0x2

// Bitfield Description : RAS Leaf1 control for stalling its egress port when dectecting poison data.
#define BIFL_RAS_LEAF1_CTRL_POISON_STALL_EN_OFFSET      2
#define BIFL_RAS_LEAF1_CTRL_POISON_STALL_EN_MASK        0x4

// Bitfield Description : RAS Leaf1 control for sending error event when dectecting parity error.
#define BIFL_RAS_LEAF1_CTRL_PARITY_ERREVENT_EN_OFFSET      3
#define BIFL_RAS_LEAF1_CTRL_PARITY_ERREVENT_EN_MASK        0x8

// Bitfield Description : RAS Leaf1 control for stalling its egress port when dectecting parity error.
#define BIFL_RAS_LEAF1_CTRL_PARITY_STALL_EN_OFFSET      4
#define BIFL_RAS_LEAF1_CTRL_PARITY_STALL_EN_MASK        0x10

// Bitfield Description : RAS Leaf1 control for sending error event when recieve a errevnet.
#define BIFL_RAS_LEAF1_CTRL_RCVERREVENT_ERREVENT_EN_OFFSET      5
#define BIFL_RAS_LEAF1_CTRL_RCVERREVENT_ERREVENT_EN_MASK        0x20

// Bitfield Description : RAS Leaf1 control for stalling its egress port when recieve a errevnet.
#define BIFL_RAS_LEAF1_CTRL_RCVERREVENT_STALL_EN_OFFSET      6
#define BIFL_RAS_LEAF1_CTRL_RCVERREVENT_STALL_EN_MASK        0x40

// Bitfield Description :
#define BIFL_RAS_LEAF1_CTRL_Reserved_7_7_OFFSET      7
#define BIFL_RAS_LEAF1_CTRL_Reserved_7_7_MASK        0x80

// Bitfield Description : RAS Leaf1 control to propagate errEvent.
#define BIFL_RAS_LEAF1_CTRL_ERR_EVENT_GEN_EN_OFFSET      8
#define BIFL_RAS_LEAF1_CTRL_ERR_EVENT_GEN_EN_MASK        0x100

// Bitfield Description : RAS Leaf1 control  to propagate egress stall.
#define BIFL_RAS_LEAF1_CTRL_EGRESS_STALL_GEN_EN_OFFSET      9
#define BIFL_RAS_LEAF1_CTRL_EGRESS_STALL_GEN_EN_MASK        0x200

// Bitfield Description : RAS Leaf1 control to propagate errEvent.
#define BIFL_RAS_LEAF1_CTRL_ERR_EVENT_PROP_EN_OFFSET      10
#define BIFL_RAS_LEAF1_CTRL_ERR_EVENT_PROP_EN_MASK        0x400

// Bitfield Description : RAS Leaf1 control  to propagate egress stall.
#define BIFL_RAS_LEAF1_CTRL_EGRESS_STALL_PROP_EN_OFFSET      11
#define BIFL_RAS_LEAF1_CTRL_EGRESS_STALL_PROP_EN_MASK        0x800

// Bitfield Description : RAS Leaf1 control for pollute data to poison.
#define BIFL_RAS_LEAF1_CTRL_POISON_DBUG_EN_OFFSET      12
#define BIFL_RAS_LEAF1_CTRL_POISON_DBUG_EN_MASK        0x1000

// Bitfield Description : RAS Leaf1 control for pollute data to parity.
#define BIFL_RAS_LEAF1_CTRL_PARITY_DBUG_EN_OFFSET      13
#define BIFL_RAS_LEAF1_CTRL_PARITY_DBUG_EN_MASK        0x2000

// Bitfield Description :
#define BIFL_RAS_LEAF1_CTRL_Reserved_15_14_OFFSET      14
#define BIFL_RAS_LEAF1_CTRL_Reserved_15_14_MASK        0xc000

// Bitfield Description : Only for athub.
#define BIFL_RAS_LEAF1_CTRL_ERR_EVENT_RAS_INTR_EN_OFFSET      16
#define BIFL_RAS_LEAF1_CTRL_ERR_EVENT_RAS_INTR_EN_MASK        0x10000

// Bitfield Description :
#define BIFL_RAS_LEAF1_CTRL_Reserved_31_17_OFFSET      17
#define BIFL_RAS_LEAF1_CTRL_Reserved_31_17_MASK        0xfffe0000

typedef union {
  struct {
    UINT32                            ERR_EVENT_DET_EN:1;
    UINT32                            POISON_ERREVENT_EN:1;
    UINT32                            POISON_STALL_EN:1;
    UINT32                            PARITY_ERREVENT_EN:1;
    UINT32                            PARITY_STALL_EN:1;
    UINT32                            RCVERREVENT_ERREVENT_EN:1;
    UINT32                            RCVERREVENT_STALL_EN:1;
    UINT32                            Reserved_7_7:1;
    UINT32                            ERR_EVENT_GEN_EN:1;
    UINT32                            EGRESS_STALL_GEN_EN:1;
    UINT32                            ERR_EVENT_PROP_EN:1;
    UINT32                            EGRESS_STALL_PROP_EN:1;
    UINT32                            POISON_DBUG_EN:1;
    UINT32                            PARITY_DBUG_EN:1;
    UINT32                            Reserved_15_14:2;
    UINT32                            ERR_EVENT_RAS_INTR_EN:1;
    UINT32                            Reserved_31_17:15;
  } Field;
  UINT32 Value;
} BIFL_RAS_LEAF1_CTRL_STRUCT;

#define SMN_NBIF0_BIFL_RAS_LEAF1_CTRL_ADDRESS    0x10139084UL


/***********************************************************
* Register Name : BIFL_RAS_LEAF1_STATUS
* Register Description :
* BIFL RAS LEAF1 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf1 status for SDP error event receive recording.
#define BIFL_RAS_LEAF1_STATUS_ERR_EVENT_RECV_OFFSET      0
#define BIFL_RAS_LEAF1_STATUS_ERR_EVENT_RECV_MASK        0x1

// Bitfield Description : RAS Leaf1 status for self-detected poison data recording.
#define BIFL_RAS_LEAF1_STATUS_POISON_ERR_DET_OFFSET      1
#define BIFL_RAS_LEAF1_STATUS_POISON_ERR_DET_MASK        0x2

// Bitfield Description : RAS Leaf1 status for self-detected parity error recording.
#define BIFL_RAS_LEAF1_STATUS_PARITY_ERR_DET_OFFSET      2
#define BIFL_RAS_LEAF1_STATUS_PARITY_ERR_DET_MASK        0x4

// Bitfield Description :
#define BIFL_RAS_LEAF1_STATUS_Reserved_7_3_OFFSET      3
#define BIFL_RAS_LEAF1_STATUS_Reserved_7_3_MASK        0xf8

// Bitfield Description : RAS Leaf1 has sent errEvent from self-generated error.
#define BIFL_RAS_LEAF1_STATUS_ERR_EVENT_GENN_STAT_OFFSET      8
#define BIFL_RAS_LEAF1_STATUS_ERR_EVENT_GENN_STAT_MASK        0x100

// Bitfield Description : RAS Leaf1 has stalled egress port from self-generated error.
#define BIFL_RAS_LEAF1_STATUS_EGRESS_STALLED_GENN_STAT_OFFSET      9
#define BIFL_RAS_LEAF1_STATUS_EGRESS_STALLED_GENN_STAT_MASK        0x200

// Bitfield Description : RAS Leaf1 sent error event from propagated errEvent.
#define BIFL_RAS_LEAF1_STATUS_ERR_EVENT_PROP_STAT_OFFSET      10
#define BIFL_RAS_LEAF1_STATUS_ERR_EVENT_PROP_STAT_MASK        0x400

// Bitfield Description : RAS Leaf1 stalled egress port from propagated stall.
#define BIFL_RAS_LEAF1_STATUS_EGRESS_STALLED_PROP_STAT_OFFSET      11
#define BIFL_RAS_LEAF1_STATUS_EGRESS_STALLED_PROP_STAT_MASK        0x800

// Bitfield Description :
#define BIFL_RAS_LEAF1_STATUS_Reserved_31_12_OFFSET      12
#define BIFL_RAS_LEAF1_STATUS_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            ERR_EVENT_RECV:1;
    UINT32                            POISON_ERR_DET:1;
    UINT32                            PARITY_ERR_DET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            ERR_EVENT_GENN_STAT:1;
    UINT32                            EGRESS_STALLED_GENN_STAT:1;
    UINT32                            ERR_EVENT_PROP_STAT:1;
    UINT32                            EGRESS_STALLED_PROP_STAT:1;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} BIFL_RAS_LEAF1_STATUS_STRUCT;

#define SMN_NBIF0_BIFL_RAS_LEAF1_STATUS_ADDRESS    0x101390c4UL


/***********************************************************
* Register Name : BIFL_RAS_LEAF2_CTRL
* Register Description :
* BIFL RAS LEAF2 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf2 control bit for port err event detection.
#define BIFL_RAS_LEAF2_CTRL_ERR_EVENT_DET_EN_OFFSET      0
#define BIFL_RAS_LEAF2_CTRL_ERR_EVENT_DET_EN_MASK        0x1

// Bitfield Description : RAS Leaf2 control for sending error event when dectecting poison data.
#define BIFL_RAS_LEAF2_CTRL_POISON_ERREVENT_EN_OFFSET      1
#define BIFL_RAS_LEAF2_CTRL_POISON_ERREVENT_EN_MASK        0x2

// Bitfield Description : RAS Leaf2 control for stalling its egress port when dectecting poison data.
#define BIFL_RAS_LEAF2_CTRL_POISON_STALL_EN_OFFSET      2
#define BIFL_RAS_LEAF2_CTRL_POISON_STALL_EN_MASK        0x4

// Bitfield Description : RAS Leaf2 control for sending error event when dectecting parity error.
#define BIFL_RAS_LEAF2_CTRL_PARITY_ERREVENT_EN_OFFSET      3
#define BIFL_RAS_LEAF2_CTRL_PARITY_ERREVENT_EN_MASK        0x8

// Bitfield Description : RAS Leaf2 control for stalling its egress port when dectecting parity error.
#define BIFL_RAS_LEAF2_CTRL_PARITY_STALL_EN_OFFSET      4
#define BIFL_RAS_LEAF2_CTRL_PARITY_STALL_EN_MASK        0x10

// Bitfield Description : RAS Leaf2 control for sending error event when recieve a errevnet.
#define BIFL_RAS_LEAF2_CTRL_RCVERREVENT_ERREVENT_EN_OFFSET      5
#define BIFL_RAS_LEAF2_CTRL_RCVERREVENT_ERREVENT_EN_MASK        0x20

// Bitfield Description : RAS Leaf2 control for stalling its egress port when recieve a errevnet.
#define BIFL_RAS_LEAF2_CTRL_RCVERREVENT_STALL_EN_OFFSET      6
#define BIFL_RAS_LEAF2_CTRL_RCVERREVENT_STALL_EN_MASK        0x40

// Bitfield Description :
#define BIFL_RAS_LEAF2_CTRL_Reserved_7_7_OFFSET      7
#define BIFL_RAS_LEAF2_CTRL_Reserved_7_7_MASK        0x80

// Bitfield Description : RAS Leaf2 control to propagate errEvent.
#define BIFL_RAS_LEAF2_CTRL_ERR_EVENT_GEN_EN_OFFSET      8
#define BIFL_RAS_LEAF2_CTRL_ERR_EVENT_GEN_EN_MASK        0x100

// Bitfield Description : RAS Leaf2 control  to propagate egress stall.
#define BIFL_RAS_LEAF2_CTRL_EGRESS_STALL_GEN_EN_OFFSET      9
#define BIFL_RAS_LEAF2_CTRL_EGRESS_STALL_GEN_EN_MASK        0x200

// Bitfield Description : RAS Leaf2 control to propagate errEvent.
#define BIFL_RAS_LEAF2_CTRL_ERR_EVENT_PROP_EN_OFFSET      10
#define BIFL_RAS_LEAF2_CTRL_ERR_EVENT_PROP_EN_MASK        0x400

// Bitfield Description : RAS Leaf2 control  to propagate egress stall.
#define BIFL_RAS_LEAF2_CTRL_EGRESS_STALL_PROP_EN_OFFSET      11
#define BIFL_RAS_LEAF2_CTRL_EGRESS_STALL_PROP_EN_MASK        0x800

// Bitfield Description : RAS Leaf2 control for pollute data to poison.
#define BIFL_RAS_LEAF2_CTRL_POISON_DBUG_EN_OFFSET      12
#define BIFL_RAS_LEAF2_CTRL_POISON_DBUG_EN_MASK        0x1000

// Bitfield Description : RAS Leaf2 control for pollute data to parity.
#define BIFL_RAS_LEAF2_CTRL_PARITY_DBUG_EN_OFFSET      13
#define BIFL_RAS_LEAF2_CTRL_PARITY_DBUG_EN_MASK        0x2000

// Bitfield Description :
#define BIFL_RAS_LEAF2_CTRL_Reserved_15_14_OFFSET      14
#define BIFL_RAS_LEAF2_CTRL_Reserved_15_14_MASK        0xc000

// Bitfield Description : Only for athub.
#define BIFL_RAS_LEAF2_CTRL_ERR_EVENT_RAS_INTR_EN_OFFSET      16
#define BIFL_RAS_LEAF2_CTRL_ERR_EVENT_RAS_INTR_EN_MASK        0x10000

// Bitfield Description :
#define BIFL_RAS_LEAF2_CTRL_Reserved_31_17_OFFSET      17
#define BIFL_RAS_LEAF2_CTRL_Reserved_31_17_MASK        0xfffe0000

typedef union {
  struct {
    UINT32                            ERR_EVENT_DET_EN:1;
    UINT32                            POISON_ERREVENT_EN:1;
    UINT32                            POISON_STALL_EN:1;
    UINT32                            PARITY_ERREVENT_EN:1;
    UINT32                            PARITY_STALL_EN:1;
    UINT32                            RCVERREVENT_ERREVENT_EN:1;
    UINT32                            RCVERREVENT_STALL_EN:1;
    UINT32                            Reserved_7_7:1;
    UINT32                            ERR_EVENT_GEN_EN:1;
    UINT32                            EGRESS_STALL_GEN_EN:1;
    UINT32                            ERR_EVENT_PROP_EN:1;
    UINT32                            EGRESS_STALL_PROP_EN:1;
    UINT32                            POISON_DBUG_EN:1;
    UINT32                            PARITY_DBUG_EN:1;
    UINT32                            Reserved_15_14:2;
    UINT32                            ERR_EVENT_RAS_INTR_EN:1;
    UINT32                            Reserved_31_17:15;
  } Field;
  UINT32 Value;
} BIFL_RAS_LEAF2_CTRL_STRUCT;

#define SMN_NBIF0_BIFL_RAS_LEAF2_CTRL_ADDRESS    0x10139088UL


/***********************************************************
* Register Name : BIFL_RAS_LEAF2_STATUS
* Register Description :
* BIFL RAS LEAF2 Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : RAS Leaf2 status for SDP error event receive recording.
#define BIFL_RAS_LEAF2_STATUS_ERR_EVENT_RECV_OFFSET      0
#define BIFL_RAS_LEAF2_STATUS_ERR_EVENT_RECV_MASK        0x1

// Bitfield Description : RAS Leaf2 status for self-detected poison data recording.
#define BIFL_RAS_LEAF2_STATUS_POISON_ERR_DET_OFFSET      1
#define BIFL_RAS_LEAF2_STATUS_POISON_ERR_DET_MASK        0x2

// Bitfield Description : RAS Leaf2 status for self-detected parity error recording.
#define BIFL_RAS_LEAF2_STATUS_PARITY_ERR_DET_OFFSET      2
#define BIFL_RAS_LEAF2_STATUS_PARITY_ERR_DET_MASK        0x4

// Bitfield Description :
#define BIFL_RAS_LEAF2_STATUS_Reserved_7_3_OFFSET      3
#define BIFL_RAS_LEAF2_STATUS_Reserved_7_3_MASK        0xf8

// Bitfield Description : RAS Leaf2 has sent errEvent from self-generated error.
#define BIFL_RAS_LEAF2_STATUS_ERR_EVENT_GENN_STAT_OFFSET      8
#define BIFL_RAS_LEAF2_STATUS_ERR_EVENT_GENN_STAT_MASK        0x100

// Bitfield Description : RAS Leaf2 has stalled egress port from self-generated error.
#define BIFL_RAS_LEAF2_STATUS_EGRESS_STALLED_GENN_STAT_OFFSET      9
#define BIFL_RAS_LEAF2_STATUS_EGRESS_STALLED_GENN_STAT_MASK        0x200

// Bitfield Description : RAS Leaf2 sent error event from propagated errEvent.
#define BIFL_RAS_LEAF2_STATUS_ERR_EVENT_PROP_STAT_OFFSET      10
#define BIFL_RAS_LEAF2_STATUS_ERR_EVENT_PROP_STAT_MASK        0x400

// Bitfield Description : RAS Leaf2 stalled egress port from propagated stall.
#define BIFL_RAS_LEAF2_STATUS_EGRESS_STALLED_PROP_STAT_OFFSET      11
#define BIFL_RAS_LEAF2_STATUS_EGRESS_STALLED_PROP_STAT_MASK        0x800

// Bitfield Description :
#define BIFL_RAS_LEAF2_STATUS_Reserved_31_12_OFFSET      12
#define BIFL_RAS_LEAF2_STATUS_Reserved_31_12_MASK        0xfffff000

typedef union {
  struct {
    UINT32                            ERR_EVENT_RECV:1;
    UINT32                            POISON_ERR_DET:1;
    UINT32                            PARITY_ERR_DET:1;
    UINT32                            Reserved_7_3:5;
    UINT32                            ERR_EVENT_GENN_STAT:1;
    UINT32                            EGRESS_STALLED_GENN_STAT:1;
    UINT32                            ERR_EVENT_PROP_STAT:1;
    UINT32                            EGRESS_STALLED_PROP_STAT:1;
    UINT32                            Reserved_31_12:20;
  } Field;
  UINT32 Value;
} BIFL_RAS_LEAF2_STATUS_STRUCT;

#define SMN_NBIF0_BIFL_RAS_LEAF2_STATUS_ADDRESS    0x101390c8UL

/***********************************************************
* Register Name : RCC_DEV2_EPF5_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set DEVICE_ID in EP function.
#define RCC_DEV2_EPF5_STRAP0_STRAP_DEVICE_ID_DEV2_F5_OFFSET      0
#define RCC_DEV2_EPF5_STRAP0_STRAP_DEVICE_ID_DEV2_F5_MASK        0xffff

// Bitfield Description : Set MAJOR_REV_ID in EP function.
#define RCC_DEV2_EPF5_STRAP0_STRAP_MAJOR_REV_ID_DEV2_F5_OFFSET      16
#define RCC_DEV2_EPF5_STRAP0_STRAP_MAJOR_REV_ID_DEV2_F5_MASK        0xf0000

// Bitfield Description : Set MINOR_REV_ID in EP function.
#define RCC_DEV2_EPF5_STRAP0_STRAP_MINOR_REV_ID_DEV2_F5_OFFSET      20
#define RCC_DEV2_EPF5_STRAP0_STRAP_MINOR_REV_ID_DEV2_F5_MASK        0xf00000

// Bitfield Description :
#define RCC_DEV2_EPF5_STRAP0_Reserved_27_24_OFFSET      24
#define RCC_DEV2_EPF5_STRAP0_Reserved_27_24_MASK        0xf000000

// Bitfield Description : EP Function Enable.
#define RCC_DEV2_EPF5_STRAP0_STRAP_FUNC_EN_DEV2_F5_OFFSET      28
#define RCC_DEV2_EPF5_STRAP0_STRAP_FUNC_EN_DEV2_F5_MASK        0x10000000

// Bitfield Description : Enable non-legacy mode of operation ||1:   native PCIE function ||0:  legacy function (32BAR VGA IO RdLk).
#define RCC_DEV2_EPF5_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F5_OFFSET      29
#define RCC_DEV2_EPF5_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F5_MASK        0x20000000

// Bitfield Description : Enable D1 Support in EP function.
#define RCC_DEV2_EPF5_STRAP0_STRAP_D1_SUPPORT_DEV2_F5_OFFSET      30
#define RCC_DEV2_EPF5_STRAP0_STRAP_D1_SUPPORT_DEV2_F5_MASK        0x40000000

// Bitfield Description : Enable D2 Support in EP function.
#define RCC_DEV2_EPF5_STRAP0_STRAP_D2_SUPPORT_DEV2_F5_OFFSET      31
#define RCC_DEV2_EPF5_STRAP0_STRAP_D2_SUPPORT_DEV2_F5_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_DEVICE_ID_DEV2_F5:16;
    UINT32                            STRAP_MAJOR_REV_ID_DEV2_F5:4;
    UINT32                            STRAP_MINOR_REV_ID_DEV2_F5:4;
    UINT32                            Reserved_27_24:4;
    UINT32                            STRAP_FUNC_EN_DEV2_F5:1;
    UINT32                            STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F5:1;
    UINT32                            STRAP_D1_SUPPORT_DEV2_F5:1;
    UINT32                            STRAP_D2_SUPPORT_DEV2_F5:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_EPF5_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF5_STRAP0_ADDRESS    0x10136a00UL

/***********************************************************
* Register Name : RCC_DEV2_EPF5_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Poisoned Error log as Advisory NonFatal Error type in EP function.
#define RCC_DEV2_EPF5_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F5_OFFSET      0
#define RCC_DEV2_EPF5_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F5_MASK        0x1

// Bitfield Description : Enable PWR Budget cap in EP function.
#define RCC_DEV2_EPF5_STRAP3_STRAP_PWR_EN_DEV2_F5_OFFSET      1
#define RCC_DEV2_EPF5_STRAP3_STRAP_PWR_EN_DEV2_F5_MASK        0x2

// Bitfield Description : Set SubSystemID in EP function.
#define RCC_DEV2_EPF5_STRAP3_STRAP_SUBSYS_ID_DEV2_F5_OFFSET      2
#define RCC_DEV2_EPF5_STRAP3_STRAP_SUBSYS_ID_DEV2_F5_MASK        0x3fffc

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV2_EPF5_STRAP3_STRAP_MSI_EN_DEV2_F5_OFFSET      18
#define RCC_DEV2_EPF5_STRAP3_STRAP_MSI_EN_DEV2_F5_MASK        0x40000

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV2_EPF5_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV2_F5_OFFSET      19
#define RCC_DEV2_EPF5_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV2_F5_MASK        0x80000

// Bitfield Description : Enable MSIX cap in EP function.
#define RCC_DEV2_EPF5_STRAP3_STRAP_MSIX_EN_DEV2_F5_OFFSET      20
#define RCC_DEV2_EPF5_STRAP3_STRAP_MSIX_EN_DEV2_F5_MASK        0x100000

// Bitfield Description :
#define RCC_DEV2_EPF5_STRAP3_Reserved_23_21_OFFSET      21
#define RCC_DEV2_EPF5_STRAP3_Reserved_23_21_MASK        0xe00000

// Bitfield Description : Set PMC.DSI in EP function.
#define RCC_DEV2_EPF5_STRAP3_STRAP_PMC_DSI_DEV2_F5_OFFSET      24
#define RCC_DEV2_EPF5_STRAP3_STRAP_PMC_DSI_DEV2_F5_MASK        0x1000000

// Bitfield Description :
#define RCC_DEV2_EPF5_STRAP3_Reserved_25_25_OFFSET      25
#define RCC_DEV2_EPF5_STRAP3_Reserved_25_25_MASK        0x2000000

// Bitfield Description : Enable all msi event support regardless of MSI_MULTI_CAP.
#define RCC_DEV2_EPF5_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F5_OFFSET      26
#define RCC_DEV2_EPF5_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F5_MASK        0x4000000

// Bitfield Description : 0:Disable SMN error response status for EP function being masked to SC. 1:Enable SMN error response status for EP function being masked to SC.
#define RCC_DEV2_EPF5_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F5_OFFSET      27
#define RCC_DEV2_EPF5_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F5_MASK        0x8000000

// Bitfield Description :
#define RCC_DEV2_EPF5_STRAP3_Reserved_28_28_OFFSET      28
#define RCC_DEV2_EPF5_STRAP3_Reserved_28_28_MASK        0x10000000

// Bitfield Description : strap for CLKREQ# enable.
#define RCC_DEV2_EPF5_STRAP3_STRAP_CLK_PM_EN_DEV2_F5_OFFSET      29
#define RCC_DEV2_EPF5_STRAP3_STRAP_CLK_PM_EN_DEV2_F5_MASK        0x20000000

// Bitfield Description : Enable communication with SMUMP for Dx state change.
#define RCC_DEV2_EPF5_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F5_OFFSET      30
#define RCC_DEV2_EPF5_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F5_MASK        0x40000000

// Bitfield Description : Enable Readiness Time Reporting .
#define RCC_DEV2_EPF5_STRAP3_STRAP_RTR_EN_DEV2_F5_OFFSET      31
#define RCC_DEV2_EPF5_STRAP3_STRAP_RTR_EN_DEV2_F5_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F5:1;
    UINT32                            STRAP_PWR_EN_DEV2_F5:1;
    UINT32                            STRAP_SUBSYS_ID_DEV2_F5:16;
    UINT32                            STRAP_MSI_EN_DEV2_F5:1;
    UINT32                            STRAP_MSI_CLR_PENDING_EN_DEV2_F5:1;
    UINT32                            STRAP_MSIX_EN_DEV2_F5:1;
    UINT32                            Reserved_23_21:3;
    UINT32                            STRAP_PMC_DSI_DEV2_F5:1;
    UINT32                            Reserved_25_25:1;
    UINT32                            STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F5:1;
    UINT32                            STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F5:1;
    UINT32                            Reserved_28_28:1;
    UINT32                            STRAP_CLK_PM_EN_DEV2_F5:1;
    UINT32                            STRAP_TRUE_PM_STATUS_EN_DEV2_F5:1;
    UINT32                            STRAP_RTR_EN_DEV2_F5:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_EPF5_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF5_STRAP3_ADDRESS    0x10136a0cUL

/***********************************************************
* Register Name : RCC_DEV2_EPF6_STRAP0
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Set DEVICE_ID in EP function.
#define RCC_DEV2_EPF6_STRAP0_STRAP_DEVICE_ID_DEV2_F6_OFFSET      0
#define RCC_DEV2_EPF6_STRAP0_STRAP_DEVICE_ID_DEV2_F6_MASK        0xffff

// Bitfield Description : Set MAJOR_REV_ID in EP function.
#define RCC_DEV2_EPF6_STRAP0_STRAP_MAJOR_REV_ID_DEV2_F6_OFFSET      16
#define RCC_DEV2_EPF6_STRAP0_STRAP_MAJOR_REV_ID_DEV2_F6_MASK        0xf0000

// Bitfield Description : Set MINOR_REV_ID in EP function.
#define RCC_DEV2_EPF6_STRAP0_STRAP_MINOR_REV_ID_DEV2_F6_OFFSET      20
#define RCC_DEV2_EPF6_STRAP0_STRAP_MINOR_REV_ID_DEV2_F6_MASK        0xf00000

// Bitfield Description :
#define RCC_DEV2_EPF6_STRAP0_Reserved_27_24_OFFSET      24
#define RCC_DEV2_EPF6_STRAP0_Reserved_27_24_MASK        0xf000000

// Bitfield Description : EP Function Enable.
#define RCC_DEV2_EPF6_STRAP0_STRAP_FUNC_EN_DEV2_F6_OFFSET      28
#define RCC_DEV2_EPF6_STRAP0_STRAP_FUNC_EN_DEV2_F6_MASK        0x10000000

// Bitfield Description : Enable non-legacy mode of operation ||1:   native PCIE function ||0:  legacy function (32BAR VGA IO RdLk).
#define RCC_DEV2_EPF6_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F6_OFFSET      29
#define RCC_DEV2_EPF6_STRAP0_STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F6_MASK        0x20000000

// Bitfield Description : Enable D1 Support in EP function.
#define RCC_DEV2_EPF6_STRAP0_STRAP_D1_SUPPORT_DEV2_F6_OFFSET      30
#define RCC_DEV2_EPF6_STRAP0_STRAP_D1_SUPPORT_DEV2_F6_MASK        0x40000000

// Bitfield Description : Enable D2 Support in EP function.
#define RCC_DEV2_EPF6_STRAP0_STRAP_D2_SUPPORT_DEV2_F6_OFFSET      31
#define RCC_DEV2_EPF6_STRAP0_STRAP_D2_SUPPORT_DEV2_F6_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_DEVICE_ID_DEV2_F6:16;
    UINT32                            STRAP_MAJOR_REV_ID_DEV2_F6:4;
    UINT32                            STRAP_MINOR_REV_ID_DEV2_F6:4;
    UINT32                            Reserved_27_24:4;
    UINT32                            STRAP_FUNC_EN_DEV2_F6:1;
    UINT32                            STRAP_LEGACY_DEVICE_TYPE_EN_DEV2_F6:1;
    UINT32                            STRAP_D1_SUPPORT_DEV2_F6:1;
    UINT32                            STRAP_D2_SUPPORT_DEV2_F6:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_EPF6_STRAP0_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF6_STRAP0_ADDRESS    0x10136c00UL

/***********************************************************
* Register Name : RCC_DEV2_EPF6_STRAP3
* Register Description :
*
* Visibility : 0x4
************************************************************/

// Bitfield Description : Enable Poisoned Error log as Advisory NonFatal Error type in EP function.
#define RCC_DEV2_EPF6_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F6_OFFSET      0
#define RCC_DEV2_EPF6_STRAP3_STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F6_MASK        0x1

// Bitfield Description : Enable PWR Budget cap in EP function.
#define RCC_DEV2_EPF6_STRAP3_STRAP_PWR_EN_DEV2_F6_OFFSET      1
#define RCC_DEV2_EPF6_STRAP3_STRAP_PWR_EN_DEV2_F6_MASK        0x2

// Bitfield Description : Set SubSystemID in EP function.
#define RCC_DEV2_EPF6_STRAP3_STRAP_SUBSYS_ID_DEV2_F6_OFFSET      2
#define RCC_DEV2_EPF6_STRAP3_STRAP_SUBSYS_ID_DEV2_F6_MASK        0x3fffc

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV2_EPF6_STRAP3_STRAP_MSI_EN_DEV2_F6_OFFSET      18
#define RCC_DEV2_EPF6_STRAP3_STRAP_MSI_EN_DEV2_F6_MASK        0x40000

// Bitfield Description : Enable MSI cap in EP function.
#define RCC_DEV2_EPF6_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV2_F6_OFFSET      19
#define RCC_DEV2_EPF6_STRAP3_STRAP_MSI_CLR_PENDING_EN_DEV2_F6_MASK        0x80000

// Bitfield Description : Enable MSIX cap in EP function.
#define RCC_DEV2_EPF6_STRAP3_STRAP_MSIX_EN_DEV2_F6_OFFSET      20
#define RCC_DEV2_EPF6_STRAP3_STRAP_MSIX_EN_DEV2_F6_MASK        0x100000

// Bitfield Description :
#define RCC_DEV2_EPF6_STRAP3_Reserved_23_21_OFFSET      21
#define RCC_DEV2_EPF6_STRAP3_Reserved_23_21_MASK        0xe00000

// Bitfield Description : Set PMC.DSI in EP function.
#define RCC_DEV2_EPF6_STRAP3_STRAP_PMC_DSI_DEV2_F6_OFFSET      24
#define RCC_DEV2_EPF6_STRAP3_STRAP_PMC_DSI_DEV2_F6_MASK        0x1000000

// Bitfield Description :
#define RCC_DEV2_EPF6_STRAP3_Reserved_25_25_OFFSET      25
#define RCC_DEV2_EPF6_STRAP3_Reserved_25_25_MASK        0x2000000

// Bitfield Description : Enable all msi event support regardless of MSI_MULTI_CAP.
#define RCC_DEV2_EPF6_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F6_OFFSET      26
#define RCC_DEV2_EPF6_STRAP3_STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F6_MASK        0x4000000

// Bitfield Description : 0:Disable SMN error response status for EP function being masked to SC. 1:Enable SMN error response status for EP function being masked to SC.
#define RCC_DEV2_EPF6_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F6_OFFSET      27
#define RCC_DEV2_EPF6_STRAP3_STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F6_MASK        0x8000000

// Bitfield Description :
#define RCC_DEV2_EPF6_STRAP3_Reserved_28_28_OFFSET      28
#define RCC_DEV2_EPF6_STRAP3_Reserved_28_28_MASK        0x10000000

// Bitfield Description : strap for CLKREQ# enable.
#define RCC_DEV2_EPF6_STRAP3_STRAP_CLK_PM_EN_DEV2_F6_OFFSET      29
#define RCC_DEV2_EPF6_STRAP3_STRAP_CLK_PM_EN_DEV2_F6_MASK        0x20000000

// Bitfield Description : Enable communication with SMUMP for Dx state change.
#define RCC_DEV2_EPF6_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F6_OFFSET      30
#define RCC_DEV2_EPF6_STRAP3_STRAP_TRUE_PM_STATUS_EN_DEV2_F6_MASK        0x40000000

// Bitfield Description : Enable Readiness Time Reporting .
#define RCC_DEV2_EPF6_STRAP3_STRAP_RTR_EN_DEV2_F6_OFFSET      31
#define RCC_DEV2_EPF6_STRAP3_STRAP_RTR_EN_DEV2_F6_MASK        0x80000000

typedef union {
  struct {
    UINT32                            STRAP_POISONED_ADVISORY_NONFATAL_DEV2_F6:1;
    UINT32                            STRAP_PWR_EN_DEV2_F6:1;
    UINT32                            STRAP_SUBSYS_ID_DEV2_F6:16;
    UINT32                            STRAP_MSI_EN_DEV2_F6:1;
    UINT32                            STRAP_MSI_CLR_PENDING_EN_DEV2_F6:1;
    UINT32                            STRAP_MSIX_EN_DEV2_F6:1;
    UINT32                            Reserved_23_21:3;
    UINT32                            STRAP_PMC_DSI_DEV2_F6:1;
    UINT32                            Reserved_25_25:1;
    UINT32                            STRAP_ALL_MSI_EVENT_SUPPORT_EN_DEV2_F6:1;
    UINT32                            STRAP_SMN_ERR_STATUS_MASK_EN_EP_DEV2_F6:1;
    UINT32                            Reserved_28_28:1;
    UINT32                            STRAP_CLK_PM_EN_DEV2_F6:1;
    UINT32                            STRAP_TRUE_PM_STATUS_EN_DEV2_F6:1;
    UINT32                            STRAP_RTR_EN_DEV2_F6:1;
  } Field;
  UINT32 Value;
} RCC_DEV2_EPF6_STRAP3_STRUCT;

#define SMN_NBIF0INTERNAL_RCC_DEV2_EPF6_STRAP3_ADDRESS    0x10136c0cUL

/***********************************************************
* Register Name : NBIF_SHUB_TODET_CTRL
* Register Description :
* NBIF-SHUB timeout response control.
* Visibility : 0x2
************************************************************/
// Bitfield Description : 1: NBIF_SHUB timeout response enable. 0: NBIF_SHUB timeout response disable.
#define NBIF_SHUB_TODET_CTRL_NBIF_SHUB_TODET_EN_OFFSET      0
#define NBIF_SHUB_TODET_CTRL_NBIF_SHUB_TODET_EN_MASK        0x1
// Bitfield Description : 1: NBIF_SHUB timeout response AER log enable. 0: NBIF_SHUB timeout response AER log disable.
#define NBIF_SHUB_TODET_CTRL_NBIF_SHUB_TODET_AER_LOG_EN_OFFSET      1
#define NBIF_SHUB_TODET_CTRL_NBIF_SHUB_TODET_AER_LOG_EN_MASK        0x2
// Bitfield Description :
#define NBIF_SHUB_TODET_CTRL_Reserved_7_2_OFFSET      2
#define NBIF_SHUB_TODET_CTRL_Reserved_7_2_MASK        0xfc
// Bitfield Description : Unit of timer of NBIF-SHUB timeout detection.
#define NBIF_SHUB_TODET_CTRL_NBIF_SHUB_TODET_TIMER_UNIT_OFFSET      8
#define NBIF_SHUB_TODET_CTRL_NBIF_SHUB_TODET_TIMER_UNIT_MASK        0x700
// Bitfield Description :
#define NBIF_SHUB_TODET_CTRL_Reserved_15_11_OFFSET      11
#define NBIF_SHUB_TODET_CTRL_Reserved_15_11_MASK        0xf800
// Bitfield Description : Number of timer unit for timeout value(2.6s by default).
#define NBIF_SHUB_TODET_CTRL_NBIF_SHUB_TIMEOUT_COUNT_OFFSET      16
#define NBIF_SHUB_TODET_CTRL_NBIF_SHUB_TIMEOUT_COUNT_MASK        0xffff0000

typedef union {
  struct {
    UINT32                            NBIF_SHUB_TODET_EN:1;
    UINT32                            NBIF_SHUB_TODET_AER_LOG_EN:1;
    UINT32                            Reserved_7_2:6;
    UINT32                            NBIF_SHUB_TODET_TIMER_UNIT:3;
    UINT32                            Reserved_15_11:5;
    UINT32                            NBIF_SHUB_TIMEOUT_COUNT:16;
  } Field;
  UINT32 Value;
} NBIF_SHUB_TODET_CTRL_STRUCT;
#define SMN_NBIF_SHUB_TODET_CTRL_ADDRESS    0x1013a260UL

/***********************************************************
* Register Name : NBIF_SHUB_TODET_SYNCFLOOD_CTRL
* Register Description :
* NBIF-SHUB timeout response control for per-client.
* Visibility : 0x2
************************************************************/
// Bitfield Description : Enable sync-flood triggering on timeout for each client.
#define NBIF_SHUB_TODET_SYNCFLOOD_CTRL_NBIF_SHUB_TODET_SYNCFLOOD_EN_OFFSET      0
#define NBIF_SHUB_TODET_SYNCFLOOD_CTRL_NBIF_SHUB_TODET_SYNCFLOOD_EN_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NBIF_SHUB_TODET_SYNCFLOOD_EN:32;
  } Field;
  UINT32 Value;
} NBIF_SHUB_TODET_SYNCFLOOD_CTRL_STRUCT;
#define SMN_NBIF_SHUB_TODET_SYNCFLOOD_CTRL_ADDRESS    0x1013a26cUL

/***********************************************************
* Register Name : NBIF_SHUB_TODET_CLIENT_CTRL
* Register Description :
* NBIF-SHUB timeout response control for per-client.
* Visibility : 0x2
************************************************************/
// Bitfield Description : Enable SLVERR return on timeout for each client.
#define NBIF_SHUB_TODET_CLIENT_CTRL_NBIF_SHUB_TODET_SLVERR_EN_OFFSET      0
#define NBIF_SHUB_TODET_CLIENT_CTRL_NBIF_SHUB_TODET_SLVERR_EN_MASK        0xffffffff

typedef union {
  struct {
    UINT32                            NBIF_SHUB_TODET_SLVERR_EN:32;
  } Field;
  UINT32 Value;
} NBIF_SHUB_TODET_CLIENT_CTRL_STRUCT;
#define SMN_NBIF_SHUB_TODET_CLIENT_CTRL_ADDRESS    0x1013a264UL

#endif /* _NBIFMM_H_ */
