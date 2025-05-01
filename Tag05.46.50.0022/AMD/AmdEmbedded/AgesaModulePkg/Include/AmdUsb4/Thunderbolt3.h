/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _USB4_TBT3_COMPATIBLE_H_
#define _USB4_TBT3_COMPATIBLE_H_

#define VSC_ID_1                             1
#define VSC_ID_3                             3
#define VSC_ID_4                             4
#define VESC_ID_6                            6

//
// Tbt3 Router VSC & VSEC
//
#define ROUTER_VSC_1_CS_1                    1
#define   Disable_USB_Plug_Events BIT2
#define ROUTER_VSC_1_CS_12                   12
#define ROUTER_VSC_1_PCIe_WR_Data            27
#define ROUTER_VSC_1_PCIe_CMD                28
#define ROUTER_VSC_1_PCIe_RD_DATA            29

// VSEC 6
#define VSEC_6_COMMON_CAP_STRUCT             2
#define   COMMON_REGION_LENGTH    0x14
#define   PORT_REGION_LENGTH      0x100

#define VSEC_6_PORT_CS_38                    38     // PORT_MODE
#define   PORT_DPR                BIT6              //   Downstream Port Reset

#define VSEC_6_PORT_CS_42                    42     // VSEC_6_CS_42
#define   USB_PLUGGED             BIT31             //   This field indicates whether or not a
                                                    //   Legacy USB device is plugged to the port:
                                                    //     0b - USB device is not plugged
                                                    //     1b - USB device is plugged

#define VSEC_6_PORT_CS_43                    43     // ID_WAKE
#define   EWI                     BIT5              //   Enable Wake on Inter-Domain

#define VSEC_6_PORT_CS_61                    61     // ID_SLEEP
#define   IDD                     BIT5              //   Inter-Domain Disconnect on Sleep

#define VSEC_6_PORT_CS_141                   141    // PORT_ATTR
#define   BE                      BIT12             //   Bonding Enabled
#define   RTD3_SUPPORT_INDICATION BIT24             //   This field indicates whether or not the
                                                    //   TBT3 Router supports RTD3 flows:
                                                    //     0b - Not supported
                                                    //     1b - Supported

#define VSEC_6_PORT_CS_150                   150    // LC_SX_CTRL
#define   L0C                     BIT16             //   Lane 0 Configured
#define   L0D                     BIT17             //   Lane 0 is Inter-Domain
#define   L1C                     BIT20             //   Lane 1 Configured
#define   L1D                     BIT21             //   Lane 1 is Inter-Domain
#define   CM_COMPAT1              BIT27
#define   CM_COMPAT2              BIT28
#define   SLI                     BIT29             //   Start Link Initialization
#define   PORT_IS_UPSTREAM        BIT30             //   A Connection Manager uses this bit to designate this
                                                    //   port as the Upstream Facing Port:
                                                    //     0b - Port is not the Upstream Facing Port
                                                    //     1b - Port is the Upstream Facing Port
                                                    //   A Connection Manager shall not set this bit to 1b in
                                                    //   more than one port on a Router.
#define   SLP                     BIT31             //   Enter Sleep

#define VSEC_6_PORT_CS_151                   151    // LINK_ATTR
#define   RE2                     BIT10             //   RS-FEC Enabled (Gen 2) (RE2)
#define   RE3                     BIT11             //   RS-FEC Enabled (Gen 3) (RE3)
#define   TCM                     BIT17             //   TBT3-Compatible Mode (TCM)
#define   CPS                     BIT18             //   CLx Protocol Support (CPS)

#define VSEC_6_PORT_CS_157                   157    // VSEC_6_CS_157
#define   ROUTER_DETECTED_OFFSET  24                //   Router Detected
#define   ROUTER_DETECTED_VALUE   0x31              //   If the value in this field is 31h, it is the same as when the PORT_CS_18.RD bit is 1b.
                                                    //   On disconnect the value in this field will change and will not be equal to 31h.

#define VSEC_6_PORT_CS_173                   173    // LINK_REQ
#define   RS2                     BIT0              //   Request RS-FEC Gen 2 (RS2)
#define   RRS3                    BIT1              //   Request RS-FEC Gen 3 (RRS3)
#define   XHC_CONNECT             BIT31             //   xHC Connect
                                                    //   A Connection Manager uses this bit to enable the internal xHC:
                                                    //     0b - xHC is disabled
                                                    //     1b - xHC is enabled

//
// Tbt3 DP Adapter TMU Capability
//
#define DP_TMU_DP_CS_0                       0
#define DP_TMU_DP_CS_13                      13

//
// Tbt3 DP Adapter VSC 1 Capability
//
#define DP_IP_CMD                            1
#define DP_IP_ADDR                           2
#define DP_IP_WDATA                          3
#define DP_IP_RDATA                          4

//
// Tbt3 Host Interface Adapter VSC 1 Capability
//
#define HI_VSC_1_CS_1                        1     /// > Data[0]
#define HI_VSC_1_CS_2                        2     /// > Data[1]
#define HI_VSC_1_CS_3                        3     /// > Data[2]
#define HI_VSC_1_CS_4                        4     /// > Data[3]
#define HI_VSC_1_CS_5                        5     /// > Data[4]
#define HI_VSC_1_CS_6                        6     /// > Data[5]
#define HI_VSC_1_CS_7                        7     /// > Data[6]
#define HI_VSC_1_CS_8                        8     /// > Data[7]
#define HI_VSC_1_CS_9                        9     /// > Data[8]
#define HI_VSC_1_CS_10                       10    /// > Data[9]
#define HI_VSC_1_CS_11                       11    /// > Data[10]
#define HI_VSC_1_CS_12                       12    /// > Data[11]
#define HI_VSC_1_CS_13                       13    /// > Data[12]
#define HI_VSC_1_CS_14                       14    /// > Data[13]
#define HI_VSC_1_CS_15                       15    /// > Data[14]
#define HI_VSC_1_CS_16                       16    /// > Data[15]
#define HI_VSC_1_CS_17                       17    /// > Mail In
#define   MAILIN_OPERATION_REQUEST           BIT0
#define HI_VSC_1_CS_18                       18    /// > Mail Out

//
// Mail In
//
#define MAIL_IN_NVM_READ_CMD                 2
#define MAIL_IN_NVM_READ_CMD_TIMEOUT         100000   /// > 100ms

#define DP_IP_REGISTER_ACCESS_TIMEOUT        50000    /// > 50ms

#define DP_IP_INIT_CMHS_ADDRESS              0xD282

typedef union {
  UINT32    Value;
  struct {
    UINT32  OperationRequest  : 1;
    UINT32  Rsvd              : 1;
    UINT32  StartDWAddress    : 22;
    UINT32  NumberOfDWRead    : 4;
    UINT32  CommandCode       : 4;
  } MailIn;
  struct {
    UINT32  Status          : 4;
    UINT32  StatusIsForCmd  : 4;
    UINT32  Rsvd0           : 21;
    UINT32  StatusResponse  : 1;
    UINT32  Rsvd1           : 2;
  } MailOut;
} AMD_TBT3_NVM_READ;

#endif