/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific PCIe definitions
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 312288 $   @e \$Date: 2015-02-04 00:39:01 -0600 (Wed, 04 Feb 2015) $
 *
 */
#ifndef _AMDNBIOPCIEAER_H_
#define _AMDNBIOPCIEAER_H_
#include <GnbPcieAer.h>

#define NUMBER_OF_PORTS          11

VOID
NbioPcieAerFeatureEnable (
  IN       GNB_HANDLE                 *GnbHandle,
  IN       NBIO_PCIe_PORT_AER_CONFIG  *PortAerConfig
  );

VOID
NbioPcieAerInit (
  IN       GNB_HANDLE  *GnbHandle
  );

VOID
AerSettingsOnAllPcieDevices (
  VOID
  );

///
/// For the DPCenable PCD
///
#define PCD_DPC_DIS        0
#define PCD_DPC_EN         1

///
/// Different possible values for IntGenSel
///
#define AER_REPORT_MCA     0
#define AER_REPORT_NMI     1
#define AER_REPORT_SCI     2
#define AER_REPORT_SMI     3

///
/// Recognized values for the pcd
/// PcdAmdPcieReportMechanism
///
#define AER_PCD_MCA        0
#define AER_PCD_OS_FIRST   1
#define AER_PCD_FW_FIRST   2

///
/// These defines are used for walking the NBIF
///
#define NBIF_INSTANCE_NUMBER_ADDRESS_MULTIPLICAND  0x20   ///< Each NBIF port spaced this much apart
#define NBIF_INSTANCES                             4      ///< Accounts for NBIFxPortx

//
// MCA_CTL_MASK_NBIO Register Bitfields:
//

/// Bitfield Description : ECC or Parity error. An SRAM ECC or parity error was detected.
#define MCA_CTL_MASK_NBIO_EccParityError_OFFSET                0
#define MCA_CTL_MASK_NBIO_EccParityError_WIDTH                 1
#define MCA_CTL_MASK_NBIO_EccParityError_MASK                  0x1

/// Bitfield Description : PCIE error. A PCIe error was logged in a PCIe root port.
#define MCA_CTL_MASK_NBIO_PCIE_Sideband_OFFSET                 1
#define MCA_CTL_MASK_NBIO_PCIE_Sideband_WIDTH                  1
#define MCA_CTL_MASK_NBIO_PCIE_Sideband_MASK                   0x2

/// Bitfield Description : SDP ErrEvent error. A system fatal error event from data fabric was detected.
#define MCA_CTL_MASK_NBIO_ErrEvent_OFFSET                      2
#define MCA_CTL_MASK_NBIO_ErrEvent_WIDTH                       1
#define MCA_CTL_MASK_NBIO_ErrEvent_MASK                        0x4

/// Bitfield Description : SDP Egress Poison Error. Poison was propagated to an egress port.
#define MCA_CTL_MASK_NBIO_Egress_Poison_OFFSET                 3
#define MCA_CTL_MASK_NBIO_Egress_Poison_WIDTH                  1
#define MCA_CTL_MASK_NBIO_Egress_Poison_MASK                   0x8

/// Bitfield Description : IOHC Internal Poison Error. Poison data was sent to an internal IOHC client.
#define MCA_CTL_MASK_NBIO_IOHC_Internal_Poison_OFFSET          4
#define MCA_CTL_MASK_NBIO_IOHC_Internal_Poison_WIDTH           1
#define MCA_CTL_MASK_NBIO_IOHC_Internal_Poison_MASK            0x10

/// Bitfield Description :
#define MCA_CTL_MASK_NBIO_Reserved_63_5_OFFSET                 5
#define MCA_CTL_MASK_NBIO_Reserved_63_5_WIDTH                  59
#define MCA_CTL_MASK_NBIO_Reserved_63_5_MASK                   0xffffffffffffffe0L

typedef union {
  struct {
    UINT32                                          EccParityError:1 ; ///<
    UINT32                                           PCIE_Sideband:1 ; ///<
    UINT32                                                ErrEvent:1 ; ///<
    UINT32                                           Egress_Poison:1 ; ///<
    UINT32                                    IOHC_Internal_Poison:1 ; ///<
    UINT64                                           Reserved_63_5:59; ///<
  } Field;                                                             ///<
  UINT64 Value;                                                        ///<
} MCA_CTL_MASK_NBIO_STRUCT;

//
// MCG_CAP Register Bitfields:
//

/// Bitfield Description : Indicates the number of error reporting banks visible to the core. This value may differ from core to core.
#define MCG_CAP_Count_OFFSET                                   0
#define MCG_CAP_Count_WIDTH                                    8
#define MCG_CAP_Count_MASK                                     0xff

/// Bitfield Description : See <xref type="header" file="svd_mca_func" title="Machine Check Architecture">.
#define MCG_CAP_McgCtlP_OFFSET                                 8
#define MCG_CAP_McgCtlP_WIDTH                                  1
#define MCG_CAP_McgCtlP_MASK                                   0x100

/// Bitfield Description :
#define MCG_CAP_Reserved_63_9_OFFSET                           9
#define MCG_CAP_Reserved_63_9_WIDTH                            55
#define MCG_CAP_Reserved_63_9_MASK                             0xfffffffffffffe00L

typedef union {
  struct {
    UINT32                                                   Count:8 ; ///<
    UINT32                                                 McgCtlP:1 ; ///<
    UINT64                                           Reserved_63_9:55; ///<
  } Field;                                                             ///<
  UINT64 Value;                                                        ///<
} MCG_CAP_STRUCT;

#define MSR_CCD0LTHREE0CORE0THREAD0_MCG_CAP_ADDRESS            0x179UL

//
// MCA_IPID_LS Register Bitfields:
//

/// Bitfield Description : The instance ID of this IP. This is initialized to a unique ID per instance of this register.
#define MCA_IPID_LS_InstanceId_OFFSET                          0
#define MCA_IPID_LS_InstanceId_WIDTH                           32
#define MCA_IPID_LS_InstanceId_MASK                            0xffffffffL

/// Bitfield Description : The Hardware ID of the IP associated with this MCA bank.
#define MCA_IPID_LS_HardwareID_OFFSET                          32
#define MCA_IPID_LS_HardwareID_WIDTH                           12
#define MCA_IPID_LS_HardwareID_MASK                            0xfff00000000L

/// Bitfield Description :
#define MCA_IPID_LS_Reserved_47_44_OFFSET                      44
#define MCA_IPID_LS_Reserved_47_44_WIDTH                       4
#define MCA_IPID_LS_Reserved_47_44_MASK                        0xf00000000000L

/// Bitfield Description : The McaType of the MCA bank within this IP.
#define MCA_IPID_LS_McaType_OFFSET                             48
#define MCA_IPID_LS_McaType_WIDTH                              16
#define MCA_IPID_LS_McaType_MASK                               0xffff000000000000L

typedef union {
  struct {
    UINT32                                              InstanceId:32; ///<
    UINT32                                              HardwareID:12; ///<
    UINT32                                          Reserved_47_44:4 ; ///<
    UINT32                                                 McaType:16; ///<
  } Field;                                                             ///<
  UINT64 Value;                                                        ///<
} MCA_IPID_LS_STRUCT;

#define MSR_CCD0LTHREE0CORE00MSR_MCA_IPID_LS_ADDRESS           0xc0002005UL

//
// MCA_CTL_MASK_LS Register Bitfields:
//

/// Bitfield Description : Load queue parity error.
#define MCA_CTL_MASK_LS_LDQ_OFFSET                             0
#define MCA_CTL_MASK_LS_LDQ_WIDTH                              1
#define MCA_CTL_MASK_LS_LDQ_MASK                               0x1

/// Bitfield Description : Store queue parity error.
#define MCA_CTL_MASK_LS_STQ_OFFSET                             1
#define MCA_CTL_MASK_LS_STQ_WIDTH                              1
#define MCA_CTL_MASK_LS_STQ_MASK                               0x2

/// Bitfield Description : Miss address buffer payload parity error.
#define MCA_CTL_MASK_LS_MAB_OFFSET                             2
#define MCA_CTL_MASK_LS_MAB_WIDTH                              1
#define MCA_CTL_MASK_LS_MAB_MASK                               0x4

/// Bitfield Description : Level 1 TLB parity error.
#define MCA_CTL_MASK_LS_L1DTLB_OFFSET                          3
#define MCA_CTL_MASK_LS_L1DTLB_WIDTH                           1
#define MCA_CTL_MASK_LS_L1DTLB_MASK                            0x8

/// Bitfield Description : DC Tag error type 5.
#define MCA_CTL_MASK_LS_DcTagErr5_OFFSET                       4
#define MCA_CTL_MASK_LS_DcTagErr5_WIDTH                        1
#define MCA_CTL_MASK_LS_DcTagErr5_MASK                         0x10

/// Bitfield Description : DC Tag error type 6.
#define MCA_CTL_MASK_LS_DcTagErr6_OFFSET                       5
#define MCA_CTL_MASK_LS_DcTagErr6_WIDTH                        1
#define MCA_CTL_MASK_LS_DcTagErr6_MASK                         0x20

/// Bitfield Description : DC Tag error type 1.
#define MCA_CTL_MASK_LS_DcTagErr1_OFFSET                       6
#define MCA_CTL_MASK_LS_DcTagErr1_WIDTH                        1
#define MCA_CTL_MASK_LS_DcTagErr1_MASK                         0x40

/// Bitfield Description : Internal error type 1.
#define MCA_CTL_MASK_LS_IntErrTyp1_OFFSET                      7
#define MCA_CTL_MASK_LS_IntErrTyp1_WIDTH                       1
#define MCA_CTL_MASK_LS_IntErrTyp1_MASK                        0x80

/// Bitfield Description : Internal error type 2.
#define MCA_CTL_MASK_LS_IntErrTyp2_OFFSET                      8
#define MCA_CTL_MASK_LS_IntErrTyp2_WIDTH                       1
#define MCA_CTL_MASK_LS_IntErrTyp2_MASK                        0x100

/// Bitfield Description : System Read Data Error Thread 0. An error in a read of a line from the data fabric. Possible reasons include master abort and target abort.
#define MCA_CTL_MASK_LS_SystemReadDataErrorT0_OFFSET           9
#define MCA_CTL_MASK_LS_SystemReadDataErrorT0_WIDTH            1
#define MCA_CTL_MASK_LS_SystemReadDataErrorT0_MASK             0x200

/// Bitfield Description : System Read Data Error Thread 1. An error in a read of a line from the data fabric. Possible reasons include master abort and target abort.
#define MCA_CTL_MASK_LS_SystemReadDataErrorT1_OFFSET           10
#define MCA_CTL_MASK_LS_SystemReadDataErrorT1_WIDTH            1
#define MCA_CTL_MASK_LS_SystemReadDataErrorT1_MASK             0x400

/// Bitfield Description : DC Tag error type 2.
#define MCA_CTL_MASK_LS_DcTagErr2_OFFSET                       11
#define MCA_CTL_MASK_LS_DcTagErr2_WIDTH                        1
#define MCA_CTL_MASK_LS_DcTagErr2_MASK                         0x800

/// Bitfield Description : DC Data error type 1 and poison consumption.  MCA_STATUS[Poison] is set on poison consumption from L2/L3.
#define MCA_CTL_MASK_LS_DcDataErr1_OFFSET                      12
#define MCA_CTL_MASK_LS_DcDataErr1_WIDTH                       1
#define MCA_CTL_MASK_LS_DcDataErr1_MASK                        0x1000

/// Bitfield Description : DC Data error type 2.
#define MCA_CTL_MASK_LS_DcDataErr2_OFFSET                      13
#define MCA_CTL_MASK_LS_DcDataErr2_WIDTH                       1
#define MCA_CTL_MASK_LS_DcDataErr2_MASK                        0x2000

/// Bitfield Description : DC Data error type 3.
#define MCA_CTL_MASK_LS_DcDataErr3_OFFSET                      14
#define MCA_CTL_MASK_LS_DcDataErr3_WIDTH                       1
#define MCA_CTL_MASK_LS_DcDataErr3_MASK                        0x4000

/// Bitfield Description : DC Tag error type 4.
#define MCA_CTL_MASK_LS_DcTagErr4_OFFSET                       15
#define MCA_CTL_MASK_LS_DcTagErr4_WIDTH                        1
#define MCA_CTL_MASK_LS_DcTagErr4_MASK                         0x8000

/// Bitfield Description : Level 2 TLB parity error. MCA_ADDR_LS logs a virtual address.
#define MCA_CTL_MASK_LS_L2DTLB_OFFSET                          16
#define MCA_CTL_MASK_LS_L2DTLB_WIDTH                           1
#define MCA_CTL_MASK_LS_L2DTLB_MASK                            0x10000

/// Bitfield Description : PDC parity error. MCA_ADDR_LS logs a virtual address.
#define MCA_CTL_MASK_LS_PDC_OFFSET                             17
#define MCA_CTL_MASK_LS_PDC_WIDTH                              1
#define MCA_CTL_MASK_LS_PDC_MASK                               0x20000

/// Bitfield Description : DC Tag error type 3.
#define MCA_CTL_MASK_LS_DcTagErr3_OFFSET                       18
#define MCA_CTL_MASK_LS_DcTagErr3_WIDTH                        1
#define MCA_CTL_MASK_LS_DcTagErr3_MASK                         0x40000

/// Bitfield Description : DC Tag error type 5.
#define MCA_CTL_MASK_LS_DcTagErr7_OFFSET                       19
#define MCA_CTL_MASK_LS_DcTagErr7_WIDTH                        1
#define MCA_CTL_MASK_LS_DcTagErr7_MASK                         0x80000

/// Bitfield Description : L2 Fill Data error.
#define MCA_CTL_MASK_LS_L2DataErr_OFFSET                       20
#define MCA_CTL_MASK_LS_L2DataErr_WIDTH                        1
#define MCA_CTL_MASK_LS_L2DataErr_MASK                         0x100000

/// Bitfield Description :
#define MCA_CTL_MASK_LS_Reserved_63_21_OFFSET                  21
#define MCA_CTL_MASK_LS_Reserved_63_21_WIDTH                   43
#define MCA_CTL_MASK_LS_Reserved_63_21_MASK                    0xffffffffffe00000L

typedef union {
  struct {
    UINT32                                                     LDQ:1 ; ///<
    UINT32                                                     STQ:1 ; ///<
    UINT32                                                     MAB:1 ; ///<
    UINT32                                                  L1DTLB:1 ; ///<
    UINT32                                               DcTagErr5:1 ; ///<
    UINT32                                               DcTagErr6:1 ; ///<
    UINT32                                               DcTagErr1:1 ; ///<
    UINT32                                              IntErrTyp1:1 ; ///<
    UINT32                                              IntErrTyp2:1 ; ///<
    UINT32                                   SystemReadDataErrorT0:1 ; ///<
    UINT32                                   SystemReadDataErrorT1:1 ; ///<
    UINT32                                               DcTagErr2:1 ; ///<
    UINT32                                              DcDataErr1:1 ; ///<
    UINT32                                              DcDataErr2:1 ; ///<
    UINT32                                              DcDataErr3:1 ; ///<
    UINT32                                               DcTagErr4:1 ; ///<
    UINT32                                                  L2DTLB:1 ; ///<
    UINT32                                                     PDC:1 ; ///<
    UINT32                                               DcTagErr3:1 ; ///<
    UINT32                                               DcTagErr7:1 ; ///<
    UINT32                                               L2DataErr:1 ; ///<
    UINT64                                          Reserved_63_21:43; ///<
  } Field;                                                             ///<
  UINT64 Value;                                                        ///<
} MCA_CTL_MASK_LS_STRUCT;


//
// Register Name MCA_CTL_MASK_LS
//
#define MSR_CCD0LTHREE0CORE00MSR_MCA_CTL_MASK_LS_ADDRESS       0xc0010400UL

// Used for finding MCA_CTL_MASK
#define MCA_IPID_NBIO_HardwareId 0x18
#define MCA_IPID_NBIO_McaType 0x0

#define PCD_RASCONTROL_DISABLED     0  // Enabled FALSE
#define PCD_RASCONTROL_MCA          1  // Enabled TRUE
#define PCD_RASCONTROL_LEGACY       2  // Enabled TRUE

#define PCD_AERREPORTING_MCA        0
#define PCD_AERREPORTING_OS_FIRST   1
#define PCD_AERREPORTING_FFIRST     2

#endif /* _AMDNBIOPCIEAER_H_*/


