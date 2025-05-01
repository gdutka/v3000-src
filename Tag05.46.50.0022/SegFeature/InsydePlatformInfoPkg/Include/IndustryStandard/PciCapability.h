/** @file
  PCI Capability Interpretation standard

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/


#ifndef _PCI_CAPABILITY_H_
#define _PCI_CAPABILITY_H_

#include <IndustryStandard/Pci.h>

#pragma pack(1)

//
// Already defined in Pci22.h
//
// #define EFI_PCI_CAPABILITY_ID_MSI     0x05
//
// typedef struct {
//   EFI_PCI_CAPABILITY_HDR  Hdr;
//   UINT16                  MsgCtrlReg;
//   UINT32                  MsgAddrReg;
//   UINT16                  MsgDataReg;
// } EFI_PCI_CAPABILITY_MSI32;

// ///
// /// Message Capability Structure for 64-bit Message Address
// /// Section 6.8.1, PCI Local Bus Specification, 2.2
// ///
// typedef struct {
//   EFI_PCI_CAPABILITY_HDR  Hdr;
//   UINT16                  MsgCtrlReg;
//   UINT32                  MsgAddrRegLsdw;
//   UINT32                  MsgAddrRegMsdw;
//   UINT16                  MsgDataReg;
// } EFI_PCI_CAPABILITY_MSI64;

typedef union {
  struct {
    UINT16  MsiEnable : 1;
    UINT16  MultiMsgCap : 3;
    UINT16  MultiMsgEn : 3;
    UINT16  Addr64BitCap : 1;
    UINT16  PVMCap : 1;
    UINT16  ExtDataCap : 1;
    UINT16  ExtDataEn : 1;
    UINT16  Rsv : 5;
  } Bits;
  UINT16  Uint16;
} EFI_PCI_CAPABILITY_MSI_MESSAGE_CONTROL_REG;

typedef struct {
  EFI_PCI_CAPABILITY_HDR                              Hdr;
  EFI_PCI_CAPABILITY_MSI_MESSAGE_CONTROL_REG          MsgCtrlReg;
} EFI_PCI_CAPABILITY_MSI_COMMON_HEADER;

typedef struct {
  EFI_PCI_CAPABILITY_MSI64 BaseCap;
  UINT16                   MsgExtDataReg;
  UINT32                   Mask;
  UINT32                   Pending;
} EFI_PCI_CAPABILITY_MSI64_EXT;

typedef struct {
  EFI_PCI_CAPABILITY_MSI32 BaseCap;
  UINT16                   MsgExtDataReg;
  UINT32                   Mask;
  UINT32                   Pending;
} EFI_PCI_CAPABILITY_MSI32_EXT;

#define H2O_PCI_CAPABILITY_ID_MSIX     0x11

typedef struct {
  EFI_PCI_CAPABILITY_HDR  Hdr;
  UINT16                  MessageControl;
  UINT32                  BirOffset;
  UINT32                  PbaOffset;
} H2O_PCI_CAPABILITY_MSIX;

#define H2O_PCI_CAPABILITY_ID_ENHANCED_ALLOCATION     0x14

typedef union {
  struct {
    UINT32     EntrySize         : 3;
    UINT32     Rsvd0             : 1;
    UINT32     Bei               : 4;
    UINT32     PrimaryProperty   : 8;
    UINT32     SecondaryProperty : 8;
    UINT32     Rsvd1             : 6;
    UINT32     Writable          : 1;
    UINT32     Enable            : 1;
  } Bits;
  UINT32 Uint32;
} ENHANCED_ALLOCATION_ENTRIES_HEADER;

typedef struct {
  ENHANCED_ALLOCATION_ENTRIES_HEADER  EntryHeader;
  UINT32                              Base;
  UINT32                              MaxOffset;
// UINT32                             Base64;
// UINT32                             MaxOffset64;
} ENHANCED_ALLCOTION_ENTRIES;

typedef struct {
  EFI_PCI_CAPABILITY_HDR        Hdr;
  UINT8                         NumEntries; // 6 Bits
  UINT8                         Rsvd;
  //
  //
  //
  UINT8                         DynamicArea;  // Parameterfor offset Calculate
// UINT32                       SecondDw;   // Type 1 PCI config only
// ENHANCED_ALLCOTION_ENTRIES   Entry[2];   // dynamic number by NumEntries
} H2O_PCI_CAPABILITY_ENHANCED_ALLOCATION;

#define H2O_PCI_CAPABILITY_ENHANCED_ALLOCATION_ENTRIES_64BITLENGTH(a) (((a&BIT1)>>1) == 1)

#define H2O_PCI_CAPABILITY_ID_FPB     0x15

typedef struct {
  EFI_PCI_CAPABILITY_HDR  Hdr;
  UINT16                  Capability;
  UINT32                  RidVectorControl1;
  UINT32                  RidVectorControl2;
  UINT32                  MemLowVectorControl;
  UINT32                  MemHighVectorControl1;
  UINT32                  MemHighVectorControl2;
  UINT32                  AccessControl;
  UINT32                  AccessData;
} H2O_PCI_CAPABILITY_FPB;

#define EFI_PCI_CAPABILITY_ID_AF     0x13

typedef struct {
  EFI_PCI_CAPABILITY_HDR  Hdr;
  UINT8                   Length;
  UINT8                   AfCapability;
  UINT8                   AfControl;
  UINT8                   AfStatus;
} H2O_PCI_CAPABILITY_AF;

#define H2O_PCI_CAPABILITY_ID_SSVID     0x0D

typedef struct {
  EFI_PCI_CAPABILITY_HDR  Hdr;
  UINT16                  Rsvd;
  UINT16                  SSVID;
  UINT16                  SSID;
} H2O_PCI_CAPABILITY_SSVID;

//
// Alredy defined in Pci23.h
//
// #define EFI_PCI_CAPABILITY_ID_VENDOR  0x09

typedef struct {
  EFI_PCI_CAPABILITY_HDR  Hdr;
  UINT8                   Length;
  UINT8                   VsData[1];
} H2O_PCI_CAPABILITY_VS;

//
// PCIe Extended Capability
//

//
// Already defined in PciExpress21.h
//
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_VIRTUAL_CHANNEL_ID    0x0002
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_VIRTUAL_CHANNEL_MFVC  0x0009
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_VIRTUAL_CHANNEL_VER1  0x1

typedef union {
  struct {
  UINT32                   ExtendedVcCount:3;
  UINT32                   PortVcCapability1:29;
  } Bits;
  UINT32 Uint32;
} PCI_EXPRESS_REG_VC_CAP_NUMBER;

typedef union {
  struct {
    UINT32                 Capability:24;
    UINT32                 ArbTableOffset:8;
  } Bits;
  UINT32 Uint32;
} PCI_EXPRESS_REG_VC_ARBOFFSET_CAP;

typedef struct {
  PCI_EXPRESS_REG_VC_ARBOFFSET_CAP          ResourceCapability;
  UINT32                                    VcResourceControl;
  UINT16                                    Reserved;
  UINT16                                    VcResourceStatus;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_VIRTUAL_CHANNEL_VC_EXT;

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER              Header;
  PCI_EXPRESS_REG_VC_CAP_NUMBER                         PortVcCapability1;
  PCI_EXPRESS_REG_VC_ARBOFFSET_CAP                      PortVcCapability2;
  UINT16                                                PortVcControl;
  UINT16                                                PortVcStatus;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_VIRTUAL_CHANNEL_VC  Capability[1];
} PCI_EXPRESS_EXTENDED_CAPABILITIES_VIRTUAL_CHANNEL_CAPABILITY_EXT;

//
// Already defined in PciExpress21.h
//
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_POWER_BUDGETING_ID   0x0004
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_POWER_BUDGETING_VER1 0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT32                                    DataSelect;
  UINT32                                    Data;
  UINT32                                    PowerBudgetCapability;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_POWER_BUDGETING_EXT;

//
// Already defined in PciExpress21.h
//
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_LINK_DECLARATION_ID   0x0005
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_LINK_DECLARATION_VER1 0x1

typedef union {
  struct {
    UINT32    ExtendId   : 16;
    UINT32    CapVersion : 4;
    UINT32    PortNumber : 12;
  } Bits;
  UINT32 Uint32;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_LINKDEC_ELEMENT_SELFDSC;

//
// Already defined in PciExpress21.h
//
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_ACS_EXTENDED_ID   0x000D
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_ACS_EXTENDED_VER1 0x1

typedef union {
  struct {
    UINT16    SrcValid        : 1;
    UINT16    TransBlk        : 1;
    UINT16    P2PReqRediriect : 1;
    UINT16    P2PReqComplete  : 1;
    UINT16    UpFoward        : 1;
    UINT16    P2PEgressCtl    : 1;
    UINT16    DirectTransP2P  : 1;
    UINT16    EnhancedCap     : 1;
    UINT16    CtlVectorSize   : 8;
  } Bits;
  UINT16 Uint16;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_ACS_CAP;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_DVSEC_ID        0x0023
#define PCI_EXPRESS_EXTENDED_CAPABILITY_DVSEC_VER1      0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT32                                    DvsecHeader1;
  UINT16                                    DvsecHeader2;
  UINT8                                     DvsecData[1];
} PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_DVSEC_GET_LENGTH(DVSEC) (UINT16)(((DVSEC->DvsecHeader1)&0xfff00000)>>20)


#define PCI_EXPRESS_EXTENDED_CAPABILITY_DATA_LINK_ID    0x0025
#define PCI_EXPRESS_EXTENDED_CAPABILITY_DATA_LINK_VER1  0x1

typedef union {
  struct {
    UINT32       LocalSupported  : 23;
    UINT32       Rsvd            : 8;
    UINT32       ExchangedEnable : 1;
  } Bits;
  UINT32       Uint32;
} DATA_LINK_FEATURE_CAPABILITIES_REGISTER;

typedef union {
struct {
UINT32       RemoteSupported  : 23;
UINT32       Rsvd             : 8;
UINT32       RemoteValid      : 1;
} Bits;
UINT32       Uint32;
} DATA_LINK_FEATURE_STATUS_REGISTER;

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  DATA_LINK_FEATURE_CAPABILITIES_REGISTER   Capability;
  DATA_LINK_FEATURE_CAPABILITIES_REGISTER   Status;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_DATA_LINK;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_LANE_MARGINING_ID    0x0027
#define PCI_EXPRESS_EXTENDED_CAPABILITY_LANE_MARGINING_VER1  0x1


typedef struct {
  UINT16     LaneControl;
  UINT16     LaneStaus;
} MARGINING_REGISTER;

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT16                                    PortCapability;
  UINT16                                    PortStatus;
  MARGINING_REGISTER                        MarginingRegister[1];
} PCI_EXPRESS_EXTENDED_CAPABILITIES_LANE_MARGINING;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_PASID_ID    0x001B
#define PCI_EXPRESS_EXTENDED_CAPABILITY_PASID_VER1  0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT16                                    CapabilityReg;
  UINT16                                    ControlReg;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_PASID;


#define PCI_EXPRESS_EXTENDED_CAPABILITY_FRS_QUEUE_ID    0x0021
#define PCI_EXPRESS_EXTENDED_CAPABILITY_FRS_QUEUE_VER1  0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT32                                    CapabilityReg;
  UINT16                                    StatusReg;
  UINT16                                    ControlReg;
  UINT32                                    MessageQueueReg;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_FRS_QUEUE;


#define PCI_EXPRESS_EXTENDED_CAPABILITY_LNR_ID    0x001C
#define PCI_EXPRESS_EXTENDED_CAPABILITY_LNR_VER1  0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT16                                    CapabilityReg;
  UINT16                                    ControlReg;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_LNR;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_DPC_ID    0x001D
#define PCI_EXPRESS_EXTENDED_CAPABILITY_DPC_VER1  0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT16                                    DpcCapabilityReg;
  UINT16                                    DpcControlReg;
  UINT16                                    DpcStatusReg;
  UINT16                                    ErrorSourceId;
  UINT32                                    PioStatus;
  UINT32                                    PioMaskId;
  UINT32                                    PioSeverity;
  UINT32                                    PioSysError;
  UINT32                                    PioException;
  UINT32                                    PioHeaderLog[4];    // Optional
  UINT32                                    PioImpspecLog;      // Optional
  UINT32                                    PioTlpPrefixLog[1]; // Optional
} PCI_EXPRESS_EXTENDED_CAPABILITIES_DPC;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_DPC_HEADER_LOG_SIZE 4  // unit UINT32
#define PCI_EXPRESS_EXTENDED_CAPABILITY_DPC_RP_EXTENTION_SUPPORTED(DPC) (UINT8)(((DPC->DpcCapabilityReg)&BIT5)>>5)
#define PCI_EXPRESS_EXTENDED_CAPABILITY_DPC_GET_PIOLOGSIZE(DPC) (UINT8)(((DPC->DpcCapabilityReg)&0x00000f00)>>8)

#define PCI_EXPRESS_EXTENDED_CAPABILITY_PTM_ID    0x001F
#define PCI_EXPRESS_EXTENDED_CAPABILITY_PTM_VER1  0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT32                                    CapabilityReg;
  UINT32                                    ControlReg;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_PTM;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_RTR_ID    0x0022
#define PCI_EXPRESS_EXTENDED_CAPABILITY_RTR_VER1  0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT32                                    ReportingReg1;
  UINT32                                    ReportingReg2;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_RTR;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_HIERARCHY_ID_ID    0x0028
#define PCI_EXPRESS_EXTENDED_CAPABILITY_HIERARCHY_ID_VER1  0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT32                                    Status;
  UINT32                                    Data;
  UINT32                                    HierarchyIdGuid[5];
} PCI_EXPRESS_EXTENDED_CAPABILITIES_HIERARCHY_ID;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_NPEM_ID    0x0029
#define PCI_EXPRESS_EXTENDED_CAPABILITY_NPEM_VER1  0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT32                                    CapabilityReg;
  UINT32                                    ControlReg;
  UINT32                                    StatusReg;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_NPEM;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_ALTERNATE_PROTOCOL_ID    0x002B
#define PCI_EXPRESS_EXTENDED_CAPABILITY_ALTERNATE_PROTOCOL_VER1  0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT32                                    CapabilityReg;
  UINT32                                    ControlReg;
  UINT32                                    Data1;
  UINT32                                    Data2;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_ALTERNATE_PROTOCOL;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_SFI_ID    0x002C
#define PCI_EXPRESS_EXTENDED_CAPABILITY_SFI_VER1  0x1

typedef struct {
  PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
  UINT16                                    CapabilityReg;
  UINT16                                    ControlReg;
  UINT16                                    StatusReg;
  UINT16                                    Rsvd;
  UINT32                                    CamAddress;
  UINT32                                    CamData;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_SFI;


//
// Already defined in PciExpress21.h
//
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_LINK_DECLARATION_ID   0x0005
// #define PCI_EXPRESS_EXTENDED_CAPABILITY_LINK_DECLARATION_VER1 0x1

// typedef struct {
//   PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER  Header;
//   UINT32                                    ElementSelfDescription;
//   UINT32                                    Reserved;
//   UINT32                                    LinkEntry[1];
// } PCI_EXPRESS_EXTENDED_CAPABILITIES_LINK_DECLARATION;

// #define PCI_EXPRESS_EXTENDED_CAPABILITY_LINK_DECLARATION_GET_LINK_COUNT(LINK_DECLARATION) (UINT8)(((LINK_DECLARATION->ElementSelfDescription)&0x0000ff00)>>8)

typedef struct {
  UINT32     LinkDescription;
  UINT32     Reserved;
  UINT32     LinkAddress1;
  UINT32     LinkAddress2;
} PCI_EXPRESS_EXTENDED_CAPABILITIES_LINK_DECLARATION_ENTRIES;

#define PCI_EXPRESS_EXTENDED_CAPABILITY_LINK_DECLARATION_ENTRIES_GET_TYPE(LINK_ENTRY) (UINT8)(((LINK_ENTRY->LinkDescription)&BIT1)>>1)

//
// Temp section for 5.3 kernal since PciExpress50 is only contain in H20 Kernal after 5.4
//

#ifndef _PCIEXPRESS50_H_
#define _PCIEXPRESS50_H_

  /// The Physical Layer PCI Express Extended Capability definitions.
  ///
  /// Based on section 7.7.6 of PCI Express Base Specification 5.0.
  ///@{
  #define PCI_EXPRESS_EXTENDED_CAPABILITY_PHYSICAL_LAYER_32_0_ID    0x002A
  #define PCI_EXPRESS_EXTENDED_CAPABILITY_PHYSICAL_LAYER_32_0_VER1  0x1

  // Register offsets from Physical Layer PCI-E Ext Cap Header
  #define PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_CAPABILITIES_OFFSET                         0x04
  #define PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_CONTROL_OFFSET                              0x08
  #define PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_STATUS_OFFSET                               0x0C
  #define PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_RCVD_MODIFIED_TS_DATA1_OFFSET               0x10
  #define PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_RCVD_MODIFIED_TS_DATA2_OFFSET               0x14
  #define PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_TRANS_MODIFIED_TS_DATA1_OFFSET              0x18
  #define PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_TRANS_MODIFIED_TS_DATA2_OFFSET              0x1C
  #define PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_LANE_EQUALIZATION_CONTROL_OFFSET            0x20

  typedef union {
    struct {
      UINT32 EqualizationByPassToHighestRateSupport                  : 1; // bit 0
      UINT32 NoEqualizationNeededSupport                             : 1; // bit 1
      UINT32 Reserved1                                               : 6; // Reserved bit 2:7
      UINT32 ModifiedTSUsageMode0Support                             : 1; // bit 8
      UINT32 ModifiedTSUsageMode1Support                             : 1; // bit 9
      UINT32 ModifiedTSUsageMode2Support                             : 1; // bit 10
      UINT32 ModifiedTSReservedUsageModes                            : 5; // bit 11:15
      UINT32 Reserved2                                               : 16; // Reserved bit 16:31
    } Bits;
    UINT32   Uint32;
  } PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_CAPABILITIES;

  typedef union {
    struct {
      UINT32 EqualizationByPassToHighestRateDisable                  : 1; // bit 0
      UINT32 NoEqualizationNeededDisable                             : 1; // bit 1
      UINT32 Reserved1                                               : 6; // Reserved bit 2:7
      UINT32 ModifiedTSUsageModeSelected                             : 3; // bit 8:10
      UINT32 Reserved2                                               : 21; // Reserved bit 11:31
    } Bits;
    UINT32   Uint32;
  } PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_CONTROL;

  typedef union {
    struct {
      UINT32 EqualizationComplete      : 1; // bit 0
      UINT32 EqualizationPhase1Success : 1; // bit 1
      UINT32 EqualizationPhase2Success : 1; // bit 2
      UINT32 EqualizationPhase3Success : 1; // bit 3
      UINT32 LinkEqualizationRequest   : 1; // bit 4
      UINT32 ModifiedTSRcvd            : 1; // bit 5
      UINT32 RcvdEnhancedLinkControl   : 2; // bit 6:7
      UINT32 TransmitterPrecodingOn    : 1; // bit 8
      UINT32 TransmitterPrecodeRequest : 1; // bit 9
      UINT32 NoEqualizationNeededRcvd  : 1; // bit 10
      UINT32 Reserved                  : 21; // Reserved bit 11:31
    } Bits;
    UINT32   Uint32;
  } PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_STATUS;

  typedef union {
    struct {
      UINT32 RcvdModifiedTSUsageMode   : 3; // bit 0:2
      UINT32 RcvdModifiedTSUsageInfo1  : 13; // bit 3:15
      UINT32 RcvdModifiedTSVendorId    : 16; // bit 16:31
    } Bits;
    UINT32   Uint32;
  } PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_RCVD_MODIFIED_TS_DATA1;

  typedef union {
    struct {
      UINT32 RcvdModifiedTSUsageInfo2     : 24; // bit 0:23
      UINT32 AltProtocolNegotiationStatus : 2; // bit 24:25
      UINT32 Reserved                     : 6; // Reserved bit 26:31
    } Bits;
    UINT32   Uint32;
  } PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_RCVD_MODIFIED_TS_DATA2;

  typedef union {
    struct {
      UINT32 TransModifiedTSUsageMode   : 3; // bit 0:2
      UINT32 TransModifiedTSUsageInfo1  : 13; // bit 3:15
      UINT32 TransModifiedTSVendorId    : 16; // bit 16:31
    } Bits;
    UINT32   Uint32;
  } PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_TRANS_MODIFIED_TS_DATA1;

  typedef union {
    struct {
      UINT32 TransModifiedTSUsageInfo2    : 24; // bit 0:23
      UINT32 AltProtocolNegotiationStatus : 2; // bit 24:25
      UINT32 Reserved                     : 6; // Reserved bit 26:31
    } Bits;
    UINT32   Uint32;
  } PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_TRANS_MODIFIED_TS_DATA2;

  typedef union {
    struct {
      UINT8 DownstreamPortTransmitterPreset : 4; //bit 0..3
      UINT8 UpstreamPortTransmitterPreset   : 4; //bit 4..7
    } Bits;
    UINT8   Uint8;
  } PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_LANE_EQUALIZATION_CONTROL;

  typedef struct {
    PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER                      Header;
    PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_CAPABILITIES              Capablities;
    PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_CONTROL                   Control;
    PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_STATUS                    Status;
    PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_RCVD_MODIFIED_TS_DATA1    RcvdModifiedTs1Data;
    PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_RCVD_MODIFIED_TS_DATA2    RcvdModifiedTs2Data;
    PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_TRANS_MODIFIED_TS_DATA1   TransModifiedTs1Data;
    PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_TRANS_MODIFIED_TS_DATA2   TransModifiedTs2Data;
    PCI_EXPRESS_REG_PHYSICAL_LAYER_32_0_LANE_EQUALIZATION_CONTROL LaneEqualizationControl[1];
  } PCI_EXPRESS_EXTENDED_CAPABILITIES_PHYSICAL_LAYER_32_0;
  ///@}
#endif

#pragma pack()

#endif
