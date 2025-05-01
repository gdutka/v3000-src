/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _PCIEPORT_H_
#define _PCIEPORT_H_


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
} PCIE_ERR_CNTL_STRUCT;

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

#define SMN_FUNC0_PCIE2_PCIE_ERR_CNTL_ADDRESS    0x113401a8UL

#define SMN_FUNC0_PCIE3_PCIE_ERR_CNTL_ADDRESS    0x114401a8UL

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


/***********************************************************
* Register Name : PCIE_TX_ERR_CTRL
* Register Description :
* TX Error Control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : Controls if LCRC error is generated for the next transmitted TLP.
#define PCIE_TX_ERR_CTRL_TX_GENERATE_LCRC_ERR_OFFSET      0
#define PCIE_TX_ERR_CTRL_TX_GENERATE_LCRC_ERR_MASK        0x1

// Bitfield Description : Controls if ECRC error is generated for the next transmitted TLP.
#define PCIE_TX_ERR_CTRL_TX_GENERATE_ECRC_ERR_OFFSET      1
#define PCIE_TX_ERR_CTRL_TX_GENERATE_ECRC_ERR_MASK        0x2

// Bitfield Description : A write of 1 to this register causes the next transmitted Posted or Completion with Data to be poisoned.
#define PCIE_TX_ERR_CTRL_TX_GENERATE_POIS_TLP_OFFSET      2
#define PCIE_TX_ERR_CTRL_TX_GENERATE_POIS_TLP_MASK        0x4

// Bitfield Description : 
#define PCIE_TX_ERR_CTRL_Reserved_31_3_OFFSET      3
#define PCIE_TX_ERR_CTRL_Reserved_31_3_MASK        0xfffffff8

typedef union {
  struct {
    UINT32                            TX_GENERATE_LCRC_ERR:1;
    UINT32                            TX_GENERATE_ECRC_ERR:1;
    UINT32                            TX_GENERATE_POIS_TLP:1;
    UINT32                            Reserved_31_3:29;
  } Field;
  UINT32 Value;
} PCIE_TX_ERR_CTRL_STRUCT;

#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_ERR_CTRL_DEVICE       2
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_ERR_CTRL_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_ERR_CTRL_OFFSET       0x670
#define PCICFG_NBIO_FUNC1_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 2, 1, 0x670)

#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_ERR_CTRL_DEVICE       2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_ERR_CTRL_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_ERR_CTRL_OFFSET       0x670
#define PCICFG_NBIO_FUNC2_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 2, 2, 0x670)

#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_ERR_CTRL_DEVICE       2
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_ERR_CTRL_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_ERR_CTRL_OFFSET       0x670
#define PCICFG_NBIO_FUNC3_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 2, 3, 0x670)

#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_ERR_CTRL_DEVICE       2
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_ERR_CTRL_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_ERR_CTRL_OFFSET       0x670
#define PCICFG_NBIO_FUNC4_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 2, 4, 0x670)

#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_ERR_CTRL_DEVICE       2
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_ERR_CTRL_FUNC         5
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_ERR_CTRL_OFFSET       0x670
#define PCICFG_NBIO_FUNC5_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 2, 5, 0x670)

#define SMN_FUNC0_PCIE0_PCIE_TX_ERR_CTRL_ADDRESS    0x11140670UL

#define SMN_FUNC0_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS    0x11240670UL

#define SMN_FUNC0_PCIE2_PCIE_TX_ERR_CTRL_ADDRESS    0x11340670UL

#define SMN_FUNC0_PCIE3_PCIE_TX_ERR_CTRL_ADDRESS    0x11440670UL

#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_TX_ERR_CTRL_DEVICE       1
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_TX_ERR_CTRL_FUNC         1
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_TX_ERR_CTRL_OFFSET       0x670
#define PCICFG_NBIO_FUNC1_PCIE0_PCIE_TX_ERR_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 1, 1, 0x670)

#define SMN_FUNC1_PCIE0_PCIE_TX_ERR_CTRL_ADDRESS    0x11141670UL

#define SMN_FUNC1_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS    0x11241670UL

#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_TX_ERR_CTRL_DEVICE       1
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_TX_ERR_CTRL_FUNC         2
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_TX_ERR_CTRL_OFFSET       0x670
#define PCICFG_NBIO_FUNC2_PCIE0_PCIE_TX_ERR_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 1, 2, 0x670)

#define SMN_FUNC2_PCIE0_PCIE_TX_ERR_CTRL_ADDRESS    0x11142670UL

#define SMN_FUNC2_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS    0x11242670UL

#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_TX_ERR_CTRL_DEVICE       1
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_TX_ERR_CTRL_FUNC         3
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_TX_ERR_CTRL_OFFSET       0x670
#define PCICFG_NBIO_FUNC3_PCIE0_PCIE_TX_ERR_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 1, 3, 0x670)

#define SMN_FUNC3_PCIE0_PCIE_TX_ERR_CTRL_ADDRESS    0x11143670UL

#define SMN_FUNC3_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS    0x11243670UL

#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_TX_ERR_CTRL_DEVICE       1
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_TX_ERR_CTRL_FUNC         4
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_TX_ERR_CTRL_OFFSET       0x670
#define PCICFG_NBIO_FUNC4_PCIE0_PCIE_TX_ERR_CTRL_ADDRESS      MAKE_SBDFO(0, 0, 1, 4, 0x670)

#define SMN_FUNC4_PCIE0_PCIE_TX_ERR_CTRL_ADDRESS    0x11144670UL

#define SMN_FUNC4_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS    0x11244670UL

#define SMN_FUNC5_PCIE1_PCIE_TX_ERR_CTRL_ADDRESS    0x11245670UL

#endif /* _PCIEPORT_H_ */
