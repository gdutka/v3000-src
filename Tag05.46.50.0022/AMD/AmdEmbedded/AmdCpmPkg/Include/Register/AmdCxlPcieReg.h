/** @file

Cxl PCIE Registers.

**/
/******************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/
#ifndef __AMD_CXL_PCIE_REG_H__
#define __AMD_CXL_PCIE_REG_H__

#undef ATI_VENDOR_ID
#undef AMD_VENDOR_ID

#define CXL_VENDOR_ID                 0x1E98
#define CXL_1_0_VENDOR_ID             0x8086
#define ATI_VENDOR_ID                 0x1002
#define XILINX_VENDOR_ID              0x10EE
#define AMD_VENDOR_ID                 0x1022
#define INVALID_VENDOR_ID             0xFFFF

// PCIe Virtual Channels
#define CXL_DVSEC_ID                  0x0007
#define VC_EXTENDED_CAP_ID1           0x0002
#define VC_EXTENDED_CAP_ID2           0x0009
#define DVSEC_EXTENDED_CAP_ID         0x0023

// Relative to VC ExtCap
#define PORT_VC_CAP1_OFFSET           0x0004
#define PORT_VC_CAP2_OFFSET           0x0008
#define PORT_VC_CONTROL_OFFSET        0x000C
#define PORT_VC_STATUS_OFFSET         0x000E
#define VC_RESOURCE_CAP_OFFSET        0x0010
#define VC_RESOURCE_CNTL_OFFSET       0x0014
#define VC_RESOURCE_CNTL_CHN_SIZE     0x000C
#define VC_RESOURCE_STAT_OFFSET       0x0018

// CXL Component IDs
#define CXL_GENERAL_ID               0x0000
#define CXL_TRANSPORT_DVSEC_ID       0x0001
#define CXL_PROTOCOL_DVSEC_ID        0x0002
#define CXL_COMMON_STRUCT_ID         0x0003
#define CXL_PORT_STRUCT_ID           0x0004
#define CXL_LINK_STRUCT_ID           0x0005
#define CXL_HA_STRUCT_ID             0x0006
#define CXL_RA_STRUCT_ID             0x0008
#define CXL_SA_STRUCT_ID             0x000A
#define CXL_ACCELERATOR_ID           0x000C
#define CXL_MAX_COMPONENT_ID         0x000C

// Relative to PCIe ExtCap: CXL DVSEC
#define DVSEC_HEADER1_OFFSET          0x04
#define DVSEC_HEADER2_OFFSET          0x08

// Relative to PCIe ExtCap: CXL Protocol Layer
#define DVSEC_CXL_PL_STATUS_OFFSET   0x0C
#define DVSEC_CXL_PL_CONTROL_OFFSET  0x10

// Relative to PCIe ExtCap: CXL Transport Layer
#define DVSEC_CXL_TL_CAP_OFFSET      0x3C
#define DVSEC_CXL_TL_CONTROL_OFFSET  0x40

// Relative to Common Status ExtCap
#define CXL_COMMON_STAT1_OFFSET            0x04
#define CXL_COMMON_STAT2_OFFSET            0x08
#define CXL_COMMON_STAT3_OFFSET            0x0C
#define CXL_DEV_ERROR_LOG_OFFSET           0x10
#define CXL_IDM_TABLE_OFFSET               0x14
#define CXL_RSAM_TABLE_OFFSET              0x18
#define CXL_HSAM_TABLE_OFFSET              0x1C

// ComnCapStat1.MultiPortDevCap
#define CXL_MULTI_PORT_CAP_BIT0            0x1
#define CXL_PRIMARY_PORT_CAP_BIT1          0x2

// IDM Entry
#define CXL_IDM_ENTRY_SIZE                 0x04

typedef enum {
  CXL_IDM_LOCAL_DESTINATION = 0,
  CXL_IDM_PORT_DESTINATION = 1
} CXL_IDM_DEST_TYPE;

// Relative to Common Control ExtCap
#define CXL_COMMON_CNTL1_OFFSET            0x04
#define CXL_COMMON_CNTL2_OFFSET            0x08
#define CXL_COMMON_ERR_CNTL_OFFSET         0x0C

// Relative to Port Status ExtCap
#define CXL_PORT_STAT1_OFFSET              0x04
#define CXL_PORT_STAT2_OFFSET              0x08
#define CXL_PORT_STAT3_OFFSET              0x0C
#define CXL_PORT_ERR_LOG_OFFSET            0x10

// Relative to Port Control ExtCap
#define CXL_PORT_CONTROL_OFFSET            0x04
#define CXL_PORT_ERR_CNT_STAT0_OFFSET      0x08
#define CXL_PORT_ERR_CNT_STAT1_OFFSET      0x0C
#define CXL_PORT_TRANS_ID_OFFSET           0x10
#define CXL_PORT_PSAM_ENTRY_OFFSET         0x14

// Relative to Link Status ExtCap
#define CXL_LINK_STAT1_OFFSET              0x04
#define CXL_LINK_STAT2_OFFSET              0x08
#define CXL_LINK_STAT3_OFFSET              0x0C
#define CXL_LINK_STAT4_OFFSET              0x10
#define CXL_LINK_ERR_LOG_OFFSET            0x14

// Relative to Link Control ExtCap
// Entries = Link#0...(PortCapStat1.NumLinkCap - 1).
#define CXL_LINK0_ATTRIB_CNTL_OFFSET       0x04

// Link Attribute Entry
#define CXL_LINK_ATTRIB_ENTRY_SIZE         0x18
#define CXL_LINK_ATTRIB_OPTONAL_SIZE       0x08

// Relative to CXL_LINKn_ATTRIB_CNTL_OFFSET
#define CXL_LINK_MAX_CREDIT_CNTL_OFFSET    0x04
#define CXL_LINK_MIN_CREDIT_CNTL_OFFSET    0x08
#define CXL_LINK_MISC_CREDIT_CNTL_OFFSET   0x0C
#define CXL_LINK_ERR_CNT_STAT0_OFFSET      0x10
#define CXL_LINK_ERR_CNT_STAT1_OFFSET      0x14

// Link TransportID Entry
#define CXL_LINK_TRANS_ID_ENTRY_SIZE       0x04

// Relative to HomeAgent Status ExtCap
#define CXL_HA_STATUS_OFFSET               0x04
#define CXL_HA_ERR_LOG_OFFSET              0x08
#define CXL_HA_MEM_POOL_CAP1_OFFSET        0x0C
#define CXL_HA_MEM_POOL_CAP2_OFFSET        0x10

// Relative to HomeAgent Control ExtCap
#define CXL_HA_CONTROL_OFFSET              0x04
#define CXL_HA_CNT_PRS_VECT0_OFFSET        0x08
#define CXL_HA_CNT_PRS_VECT1_OFFSET        0x0C
#define CXL_HA_ERR_CNT_STAT0_OFFSET        0x10
#define CXL_HA_ERR_CNT_STAT1_OFFSET        0x14
#define CXL_HAID_TABLE_ENTRY_0_OFFSET      0x18

// Relative to ReqestAgent Status ExtCap
#define CXL_RA_STATUS_OFFSET               0x04
#define CXL_RA_ERR_LOG_OFFSET              0x08

// Relative to ReqestAgent Control ExtCap
#define CXL_RA_CONTROL_OFFSET              0x04
#define CXL_RA_ERR_CNT_STAT0_OFFSET        0x08
#define CXL_RA_ERR_CNT_STAT1_OFFSET        0x0C

// Relative to SlaveAgent Status ExtCap
#define CXL_SA_STATUS_OFFSET               0x04
#define CXL_SA_ERR_LOG_OFFSET              0x08
#define CXL_SA_MEM_POOL_CAP1_OFFSET        0x0C
#define CXL_SA_MEM_POOL_CAP2_OFFSET        0x10

// Memory Pool Entry
#define CXL_MEM_POOL_ENTRY_SIZE            0x08

// Relative to SlaveAgent Control ExtCap
#define CXL_SA_CONTROL_OFFSET              0x04
#define CXL_SA_ERR_CNT_STAT0_OFFSET        0x08
#define CXL_SA_ERR_CNT_STAT1_OFFSET        0x0C
#define CXL_SA_BAT_ENTRY_CNTL0_OFFSET      0x10
#define CXL_SA_BAT_ENTRY_CNTL1_OFFSET      0x14

// BAT Entry
#define CXL_BAT_ENTRY_SIZE                 0x08

// Relative to Common Status[SAMTableOffset]
#define CXL_SAM_ENTRY_ATTRIB_OFFSET        0x00
#define CXL_SAM_ENTRY_ADDR0_OFFSET         0x04
#define CXL_SAM_ENTRY_ADDR1_OFFSET         0x08

#pragma pack (push, 1)

typedef union {
  struct {
    UINT16  CapId : 8;
    UINT16  NextCapPtr : 8;
  } AsBits;

  UINT16  AsUint16;
} PCI_CAP_HEADER;

typedef union {
  struct {
    UINT32  CapId : 16;
    UINT32  CapVersion : 4;
    UINT32  NextCapPtr : 12;
  } AsBits;

  UINT32  AsUint32;
} PCIE_EXTENDED_CAP_HEADER;

typedef union {
  struct {
    UINT32  ExtVcCount : 3;
    UINT32  RsvdP1 : 1;
    UINT32  LowPriorityExtVcCount : 3;
    UINT32  RsvdP2 : 1;
    UINT32  RefCock : 2;
    UINT32  PortArbTableSize : 2;
    UINT32  RsvdP3 : 20;
  } AsBits;

  UINT32  AsUint32;
} PORT_VC_CAPABILITY1;

typedef union {
  struct {
    UINT32  VcArbCap : 8;
    UINT32  RsvdP : 16;
    UINT32  VcArbTableOffset : 8;
  } AsBits;

  UINT32  AsUint32;
} PORT_VC_CAPABILITY2;

typedef union {
  struct {
    UINT32  TcVcMap : 8;
    UINT32  RsvdP1 : 8;
    UINT32  PortArbTable : 1;
    UINT32  PortArbSelect : 3;
    UINT32  RsvdP2 : 4;
    UINT32  VcId : 3;
    UINT32  RsvdP3 : 4;
    UINT32  VcEnable : 1;
  } AsBits;

  UINT32  AsUint32;
} VC_RESOURCE_CONTROL;

typedef union {
  struct {
    UINT32  VendorId : 16;
    UINT32  Revision : 4;
    UINT32  Length : 12;
  } AsBits;

  UINT32  AsUint32;
} PCIE_DVSEC_HEADER1;

typedef union {
  struct {
    UINT32  DvsecId : 16;
    UINT32  RsvdZ : 16;
  } AsBits;

  UINT32  AsUint32;
} PCIE_DVSEC_HEADER2;

typedef union {
  struct {
    UINT32  OptimizedTlpFormatSupport : 1;
    UINT32  RsvdZ1 : 7;
    UINT32  CxlVcResourceCapIndex : 3;
    UINT32  RsvdZ2 : 21;
  } AsBits;

  UINT32  AsUint32;
} CXL_TRANSPORT_DVSEC_CAP;

typedef union {
  struct {
    UINT32  Size : 12;
    UINT32  RsvdZ : 8;
    UINT32  Offset : 12;
  } AsBits;

  UINT32  AsUint32;
} CXL_PROTOCOL_DVSEC_HEADER;

typedef union {
  struct {
    UINT32  MultiPortDevCap : 3;
    UINT32  RsvdZ : 19;
    UINT32  ComnVersionCap : 2;
    UINT32  DevIDStat : 8;
  } AsBits;

  UINT32  AsUint32;
} CXL_COMMON_STAT1;

typedef union {
  struct {
    UINT32  DevDiscRdyStat : 1;
    UINT32  PartialCacheStatesCap : 1;
    UINT32  PortAggCap : 1;
    UINT32  CachelineSizeCap : 1;
    UINT32  AddrWidthCap : 3;
    UINT32  MultiHopPortAggCap : 1;
    UINT32  SoftwareServicePortalCap : 1;
    UINT32  SAMAlignCap : 1;
    UINT32  RsvdZ1 : 9;
    UINT32  DevRdyTimeValue : 9;
    UINT32  DevRdyTimeScale : 3;
    UINT32  RsvdZ2 : 1;
  } AsBits;

  UINT32  AsUint32;
} CXL_COMMON_STAT2;

typedef union {
  struct {
    UINT32  DevEnable : 1;
    UINT32  PrimaryPortEnable : 1;
    UINT32  MeshTopologyEnable : 1;
    UINT32  RsvdP1 : 1;
    UINT32  PortAggEnable : 1;
    UINT32  IDMTblVal : 1;
    UINT32  RSAMTblVal : 1;
    UINT32  HSAMTblVal : 1;
    UINT32  SoftwareServicesPortalEnable : 1;
    UINT32  RsvdP2 : 7;
    UINT32  ErrAgentID : 6;
    UINT32  RsvdP3 : 2;
    UINT32  DevIDCntl : 8;
  } AsBits;

  UINT32  AsUint32;
} CXL_COMMON_CNTL1;

typedef union {
  struct {
    UINT32  RsvdP1 : 1;
    UINT32  PartialCacheStatesEnable : 1;
    UINT32  RsvdP2 : 1;
    UINT32  CachelineSizeEnable : 1;
    UINT32  AddrWidthEnable : 3;
    UINT32  RsvdP3 : 25;
  } AsBits;

  UINT32  AsUint32;
} CXL_COMMON_CNTL2;

typedef union {
  struct {
    UINT32  Enable : 1;
    UINT32  RsvdP : 31;
  } AsBits;

  UINT32  AsUint32;
} CXL_COMMON_ERR_CNTL;

typedef union {
  struct {
    UINT32  DiscRdyStat : 1;
    UINT32  PktHdrTypeCap : 1;
    UINT32  RsvdZ1 : 1;
    UINT32  PortQACK : 1;
    UINT32  PortHWQACKCap : 1;
    UINT32  PortToPortFwdingCap : 1;
    UINT32  RsvdZ2 : 1;
    UINT32  NumLinkCap : 6;
    UINT32  NumPSAMEntryCap : 6;
    UINT32  RsvdZ3 : 9;
    UINT32  PortID : 4;
  } AsBits;

  UINT32  AsUint32;
} CXL_PORT_STAT1;

typedef union {
  struct {
    UINT32  PortAggVctr : 16;
    UINT32  RsvdZ : 16;
  } AsBits;

  UINT32  AsUint32;
} CXL_PORT_STAT2;

typedef union {
  struct {
    UINT32  PortFwdingVctr : 16;
    UINT32  RsvdZ : 16;
  } AsBits;

  UINT32  AsUint32;
} CXL_PORT_STAT3;

typedef union {
  struct {
    UINT32  PortEnable : 1;
    UINT32  PktHdrTypeEnable : 1;
    UINT32  RsvdP1 : 5;
    UINT32  NumLinksEnable : 6;
    UINT32  NumPSAMEntryEnable : 6;
    UINT32  RsvdP2 : 13;
  } AsBits;

  UINT32  AsUint32;
} CXL_PORT_CONTROL;

typedef union {
  struct {
    UINT32  SrcTransID : 16;
    UINT32  RsvdP : 16;
  } AsBits;

  UINT32  AsUint32;
} CXL_PORT_TRANS_ID;

typedef union {
  struct {
    UINT32  DiscRdyStat : 1;
    UINT32  LinkCreditType : 1;
    UINT32  MsgPackingCap : 1;
    UINT32  RsvdZ1 : 3;
    UINT32  NoCompAckCap : 1;
    UINT32  MaxPktSizeCap : 3;
    UINT32  RsvdZ2 : 22;
  } AsBits;

  UINT32  AsUint32;
} CXL_LINK_STAT1;

typedef union {
  struct {
    UINT32  MaxMemReqSendCap : 10;
    UINT32  MaxSnpReqSendCap : 10;
    UINT32  MaxDatReqSendCap : 10;
    UINT32  RsvdZ : 2;
  } AsBits;

  UINT32  AsUint32;
} CXL_LINK_STAT2;

typedef union {
  struct {
    UINT32  MaxMemReqRcvCap : 10;
    UINT32  MaxSnpReqRcvCap : 10;
    UINT32  MaxDatReqRcvCap : 10;
    UINT32  RsvdZ : 2;
  } AsBits;

  UINT32  AsUint32;
} CXL_LINK_STAT3;

typedef union {
  struct {
    UINT32  MaxMiscReqSendCap : 10;
    UINT32  MaxMiscReqRcvCap : 10;
    UINT32  RsvdZ : 12;
  } AsBits;

  UINT32  AsUint32;
} CXL_LINK_STAT4;

typedef union {
  struct {
    UINT32  LinkEnable : 1;
    UINT32  LinkCreditSendEnable : 1;
    UINT32  MsgPackingEnable : 1;
    UINT32  RsvdP1 : 3;
    UINT32  NoCompAckEnbale : 1;
    UINT32  MaxPktSizeEnable : 3;
    UINT32  LinkEntryAddrType : 1;
    UINT32  RsvdP2 : 21;
  } AsBits;

  UINT32  AsUint32;
} CXL_LINK_ATTRIB_CNTL;

typedef union {
  struct {
    UINT32  MaxMemReqCreditEnable : 10;
    UINT32  MaxSnoopReqCreditEnable : 10;
    UINT32  MaxDataReqCreditEnable : 10;
    UINT32  RsvdP : 2;
  } AsBits;

  UINT32  AsUint32;
} CXL_LINK_MAX_CREDIT_CNTL;

typedef union {
  struct {
    UINT32  MinMemReqCreditEnable : 10;
    UINT32  MinSnoopReqCreditEnable : 10;
    UINT32  MinDataReqCreditEnable : 10;
    UINT32  RsvdP : 2;
  } AsBits;

  UINT32  AsUint32;
} CXL_LINK_MIN_CREDIT_CNTL;

typedef union {
  struct {
    UINT32  MaxMiscCreditEnable : 10;
    UINT32  MinMiscCreditEnable : 10;
    UINT32  RsvdZ : 12;
  } AsBits;

  UINT32  AsUint32;
} CXL_LINK_MISC_CREDIT_CNTL;

typedef union {
  struct {
    UINT32  DstTransID : 16;
    UINT32  RsvdP : 16;
  } AsBits;

  UINT32  AsUint32;
} CXL_LINK_TRANS_ID;

typedef union {
  struct {
    UINT32  DiscRdyStat : 1;
    UINT32  RsvdZ1 : 2;
    UINT32  RAQACK : 1;
    UINT32  RAHWQACKCap : 1;
    UINT32  RsvdZ2 : 13;
    UINT32  RAPreciseSnpRespCap : 1;
    UINT32  CacheFlushTimeValue : 9;
    UINT32  CacheFlushTimeScale : 3;
    UINT32  RACacheFlushStat : 1;
  } AsBits;

  UINT32  AsUint32;
} CXL_RA_STATUS;

typedef union {
  struct {
    UINT32  RAEnable : 1;
    UINT32  RASnpRspEnable : 1;
    UINT32  RsvdP1 : 1;
    UINT32  RAQREQ : 1;
    UINT32  RsvdP2 : 10;
    UINT32  RACacheFlushEnable : 1;
    UINT32  RACacheEnable : 1;
    UINT32  RsvdP3 : 1;
    UINT32  RAEvictHintCntl : 1;
    UINT32  RAWriteEvictFullHintCntl : 1;
    UINT32  RsvdP4 : 7;
    UINT32  RAID : 6;
  } AsBits;

  UINT32  AsUint32;
} CXL_RA_CONTROL;

typedef union {
  struct {
    UINT32  DiscRdyStat : 1;
    UINT32  RsvdZ1 : 3;
    UINT32  MemPoolCap : 6;
    UINT32  RsvdZ2 : 9;
    UINT32  MemPoolRdyTimeValue : 9;
    UINT32  MemPoolRdyTimeScale : 3;
    UINT32  MemPoolRdyStat : 1;
  } AsBits;

  UINT32  AsUint32;
} CXL_SA_STATUS;

typedef union {
  struct {
    UINT32  SAEnable : 1;
    UINT32  RsvdP1 : 3;
    UINT32  SBATDepthEnable : 6;
    UINT32  RsvdP2 : 16;
    UINT32  SAID : 6;
  } AsBits;

  UINT32  AsUint32;
} CXL_SA_CONTROL;

typedef union {
  struct {
    UINT32  ErrorStatus : 2;
    UINT32  LogDisable : 2;
    UINT32  RsvdP : 28;
  } AsBits;

  UINT32  AsUint32;
} CXL_ERR_CNT_STAT0;

typedef union {
  struct {
    UINT32  SevLogMask : 6;
    UINT32  RsvdP1 : 2;
    UINT32  SevReportMask : 6;
    UINT32  RsvdP2 : 2;
    UINT32  PerTypeMask : 10;
    UINT32  RsvdP3 : 6;
  } AsBits;

  UINT32  AsUint32;
} CXL_ERR_CNT_STAT1;

#if 1   // TO-DO
// Current FPGA implementation
typedef union {
  struct {
    UINT32  ErrLogOffset : 12;
    UINT32  RsvdZ : 20;
  } AsBits;

  UINT32  AsUint32;
} CXL_ERROR_LOG;
#else
// Per CXL spec
typedef union {
  struct {
    UINT32  RsvdZ : 20;
    UINT32  ErrLogOffset : 12;
  } AsBits;

  UINT32  AsUint32;
} CXL_ERROR_LOG;
#endif  // TO-DO

typedef union {
  struct {
    UINT32  RsvdZ : 20;
    UINT32  IDMTblOffset : 12;
  } AsBits;

  UINT32  AsUint32;
} CXL_IDM_TABLE;

typedef union {
  struct {
    UINT32  AgentIDnValid : 1;
    UINT32  AgentIDnDestType : 1;
    UINT32  RsvdP1 : 2;
    UINT32  AgentIDnPortID : 4;
    UINT32  RsvdP2 : 2;
    UINT32  NumAggPorts : 4;
    UINT32  RsvdP3 : 1;
    UINT32  AgentIDnLinkID : 6;
    UINT32  RsvdP4 : 11;
  } AsBits;

  UINT32  AsUint32;
} CXL_IDM_ENTRY;

typedef union {
  struct {
    UINT32  SAMMaxTblSize : 12;
    UINT32  RsvdZ : 8;
    UINT32  SAMTblOffset : 12;
  } AsBits;

  UINT32  AsUint32;
} CXL_SAM_TABLE;

typedef union {
  struct {
    UINT32  SAMEntryValid : 1;
    UINT32  DestType : 1;
    UINT32  RsvdP1 : 2;
    UINT32  PortID : 4;
    UINT32  RsvdP2 : 2;
    UINT32  NumAggPorts : 4;
    UINT32  RsvdP3 : 18;
  } AsBits;

  UINT32  AsUint32;
} CXL_SAM_ENTRY_ATTRIB;

typedef union {
  struct {
    UINT32  StartAddrHi : 32;
  } AsBits;

  UINT32  AsUint32;
} CXL_SAM_ENTRY_ADDR0;

typedef union {
  struct {
    UINT32  EndAddrHi : 32;
  } AsBits;

  UINT32  AsUint32;
} CXL_SAM_ENTRY_ADDR1;

typedef union {
  struct {
    UINT32  DiscRdyStat : 1;
    UINT32  GenMemTypeCap : 3;
    UINT32  SpMemTypeCap : 3;
    UINT32  AddrCap : 1;
    UINT32  MemAttr : 3;
    UINT32  ExtMemAttr : 3;
    UINT32  RsvdZ : 2;
    UINT32  PoolSizeLo : 16;
  } AsBits;

  UINT32  AsUint32;
} CXL_MEM_POOL_CAP1;

typedef union {
  struct {
    UINT32  PoolSizeHi : 32;
  } AsBits;

  UINT32  AsUint32;
} CXL_MEM_POOL_CAP2;

typedef union {
  struct {
    UINT32  EntryValid : 1;
    UINT32  RsvdP : 31;
  } AsBits;

  UINT32  AsUint32;
} CXL_BAT_ENTRY_CNTL0;

typedef union {
  struct {
    UINT32  BaseAddrHi : 32;
  } AsBits;

  UINT32  AsUint32;
} CXL_BAT_ENTRY_CNTL1;

typedef union {
  struct {
    UINT32  EntryValid : 1;
    UINT32  RsvdP1 : 8;
    UINT32  LinkNumber : 6;
    UINT32  AddrType : 1;
    UINT32  RsvdP2 : 16;
  } AsBits;

  UINT32  AsUint32;
} CXL_PSAM_ENTRY_CNTL;

typedef union {
  struct {
    UINT32  StartAddrHi : 32;
  } AsBits;

  UINT32  AsUint32;
} CXL_PSAM_ENTRY_START_ADDR;

typedef union {
  struct {
    UINT32  EndAddrHi : 32;
  } AsBits;

  UINT32  AsUint32;
} CXL_PSAM_ENTRY_END_ADDR;

typedef union {
  struct {
    UINT32  AgentIdHA1 : 6;
    UINT32  : 2;
    UINT32  AgentIdHA2 : 6;
    UINT32  : 2;
    UINT32  AgentIdHA3 : 6;
    UINT32  : 2;
    UINT32  AgentIdHA4 : 6;
    UINT32  : 2;
  } AsBits;

  UINT32  AsUint32;
} CXL_HAIDTBL_ENTRY_0;

#pragma pack (pop)

#endif
