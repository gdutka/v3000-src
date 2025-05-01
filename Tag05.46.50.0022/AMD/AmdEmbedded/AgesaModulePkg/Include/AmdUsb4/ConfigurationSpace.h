/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/**
 * @file
 *
 *  USB4 Configuration Space
 *  This File contains Structures of  Following Configuration Spaces
 *    1) Router Configuration Space
 *    2) Adapter Configuration Space
 *    3) Path Configuration Space
 *    4) Counter Configuration space
 *
 *  Source: USB4 Spec Chapter 8
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  USB4
 */

#ifndef _AMD_USB4_CONFIGURATION_SPACE_H_
#define _AMD_USB4_CONFIGURATION_SPACE_H_

#define ROUTER_CS_SIZE_DW              27
#define ROUTER_CS_BASE_SIZE_DW         9
#define TMU_RTR_CS_SIZE_DW             26

#define ADAPTER_BASE_CS_SIZE_DW        9
#define TMU_ADP_CS_SIZE_DW             8
#define LANE_ADP_CS_SIZE_DW            3
#define ADP_PORT_CS_SIZE_DW            20
#define ADP_USB3_CS_SIZE_DW            5
#define ADP_DP_IN_CS_SIZE_DW           8
#define ADP_DP_OUT_CS_SIZE_DW          9
#define ADP_PCIE_CS_SIZE_DW            1

#define GETNEXTCAPPTR(a)      ((UINT8) ((a)>>0))
#define GETCAPID(a)           ((UINT8) ((a)>>8))
#define GETVSCID(a)           ((UINT8) ((a)>>16))
#define GETVSCLEN(a)          ((UINT8) ((a)>>24))
#define GETVSECNEXTCAPPTR(a)  ((UINT16)((a)>>32))
#define GETVSECLEN(a)         ((UINT16)((a)>>48))

#pragma  pack (push, 1)

typedef enum {
  BUFFER_ALLOCATION_MAX_USB3            = 0x0001,
  BUFFER_ALLOCATION_MIN_DP_AUX          = 0x0002,
  BUFFER_ALLOCATION_MIN_DP_MAIN         = 0x0003,
  BUFFER_ALLOCATION_MAX_PCIE            = 0x0004,
  BUFFER_ALLOCATION_MAX_HI              = 0x0005
} BUFFER_ALLOCATION_REQUEST_INDEX;

typedef enum {
  QUERY_DP_RESOURCE_AVAILABILITY        = 0x10,
  ALLOCATE_DP_RESOURCE                  = 0x11,
  DEALLOCATE_DP_RESOURCE                = 0x12,
  NVM_WRITE                             = 0x20,
  NVM_AUTHENTICATE_WRITE                = 0x21,
  NVM_READ                              = 0x22,
  NVM_SET_OFFSET                        = 0x23,
  DROM_READ                             = 0x24,
  GET_NVM_SECTOR_SIZE                   = 0x25,
  GET_PCIE_DOWNSTREAM_ENTRY_MAPPING     = 0x30,
  GET_CAPABILITIES                      = 0x31,
  SET_CAPABILITIES                      = 0x32,
  BUFFER_ALLOCATION_REQUEST             = 0x33,
  BLOCK_SIDEBAND_PORT_OPERATIONS        = 0x34,
  UNBLOCK_SIDEBAND_PORT_OPERATIONS      = 0x35,
  GET_CONTAINER_ID                      = 0x36
} ROUTER_OPERATIONS;


/*
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
ROUTER CONFIGURATION SPACE STRUCTURES
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/

//
//  Individual bit fields for  ROUTER_CS_1
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    UpstreamAdapter:6;
    UINT32    MaxAdapter:6;
    UINT32    Depth:3;
    UINT32    Reserved:1;
    UINT32    RevisionNumber:8;
  } Field;
} AMD_USB4_ROUTER_CS_1;

//
//   Individual bit fields for ROUTER_CS_3
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    TopologyIDHigh:24;
    UINT32    Reserved:7;
    UINT32    TopologyIDValid:1;
  } Field;
} AMD_USB4_ROUTER_CS_3;

//
//  Individual bit fields for  ROUTER_CS_4
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NotificationTimeout:8;  // in msec
    UINT32    CMUsb4Version:8;
    UINT32    Reserved:8;
    UINT32    Usb4Version:8;
  } Field;
} AMD_USB4_ROUTER_CS_4;

//
// Individual bit fields for ROUTER_CS_5
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    EnterSleep:1;
    UINT32    EnableWakeOnPCIe:1;
    UINT32    EnableWakeOnUSB3:1;
    UINT32    EnableWakeOnDP:1;
    UINT32    Reserved0:19;
    UINT32    CMTbt3Support:1;
    UINT32    PCIeTunnelingOn:1;
    UINT32    USB3TunnelingOn:1;
    UINT32    InternalHostControllerOn:1;
    UINT32    Reserved1:4;
    UINT32    ConfigurationValid:1;
  } Field;
} AMD_USB4_ROUTER_CS_5;

//
//  Individual bit fields for  ROUTER_CS_6
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    SleepReady:1;
    UINT32    TBT3NotSupported:1;
    UINT32    WakeonPCIeStatus:1;
    UINT32    WakeonUSBStatus:1;
    UINT32    WakeonDPStatus:1;
    UINT32    Reserved0:13;
    UINT32    InternalHostControllerImplemented:1;
    UINT32    Reserved1:5;
    UINT32    RouterReady:1;
    UINT32    ConfigurationReady:1;
    UINT32    Reserved2:6;
  } Field;
} AMD_USB4_ROUTER_CS_6;

//
//  Individual bit fields for  ROUTER_CS_26
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    Opcode:16;
    UINT32    Reserved:8;
    UINT32    Status:6;
    UINT32    OperationNotSupported:1;
    UINT32    OperationValid:1;
  } Field;
} AMD_USB4_ROUTER_CS_26;

//
// ROUTER Configuration Space Structure
//
typedef struct {
  UINT16                   VendorId;
  UINT16                   ProductId;
  AMD_USB4_ROUTER_CS_1     RouterCs1;
  UINT32                   TopologyIDLow;
  AMD_USB4_ROUTER_CS_3     RouterCs3;
  AMD_USB4_ROUTER_CS_4     RouterCs4;
  AMD_USB4_ROUTER_CS_5     RouterCs5;
  AMD_USB4_ROUTER_CS_6     RouterCs6;
  UINT32                   UUIDHigh;
  UINT32                   UUIDLow;
  UINT32                   Data[16];     // ROUTER_CS_9 to ROUTER_CS_24
  UINT32                   MetaData;
  AMD_USB4_ROUTER_CS_26    RouterCs26;
} AMD_USB4_ROUTER_CONFIGURATION_SPACE;

///
/// Capabilities Supported by Router Configuration Space
///
#define  TMU_ROUTER_CONFIGURATION_CAPABILITY_ID    0x03          // Required.
#define  VENDOR_SPECIFIC_CAPABILITY_ID             0x05          // Optional.


///
/// TMU mode
///
#define TMU_MODE_OFF         0
#define TMU_MODE_LOWRES      1
#define TMU_MODE_HIFI_UNI    2
#define TMU_MODE_HIFI_BI     3

#define TMU_TSPacketInterval_OFF       0
#define TMU_TSPacketInterval_LOWRES    1000
#define TMU_TSPacketInterval_HIFI      16

#define TMU_RTR_CS_0          0
#define TMU_RTR_CS_1          1
#define TMU_RTR_CS_2          2
#define TMU_RTR_CS_3          3
#define TMU_RTR_CS_4          4
#define TMU_RTR_CS_5          5
#define TMU_RTR_CS_6          6
#define TMU_RTR_CS_7          7
#define TMU_RTR_CS_8          8
#define TMU_RTR_CS_9          9
#define TMU_RTR_CS_10         10
#define TMU_RTR_CS_11         11
#define TMU_RTR_CS_12         12
#define TMU_RTR_CS_13         13
#define TMU_RTR_CS_14         14
#define TMU_RTR_CS_15         15
#define TMU_RTR_CS_16         16
#define TMU_RTR_CS_17         17
#define TMU_RTR_CS_18         18
#define TMU_RTR_CS_19         19
#define TMU_RTR_CS_20         20
#define TMU_RTR_CS_21         21
#define TMU_RTR_CS_22         22
#define TMU_RTR_CS_23         23
#define TMU_RTR_CS_24         24
#define TMU_RTR_CS_25         25

//
// Individual bit fields for  TMU_RTR_CS_0
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    CapabilityID:8;
    UINT32    FreqMeasurementWindow:11;
    UINT32    TimeDescription:1;
    UINT32    Reserved:2;
    UINT32    UniDirectionalCapability:1;
    UINT32    InterDomainEnable:1;
  } Field;
} AMD_USB4_TMU_RTR_CS_0;

//
// Individual bit fields for TMU_RTR_CS_15
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    FreqAvgConst:6;
    UINT32    DelayAvgConst:6;
    UINT32    OffsetAvgConst:6;
    UINT32    ErrorAvgConst:6;
    UINT32    TsInterDomainInterval:8;
  } Field;
} AMD_USB4_TMU_RTR_CS_15;

//
// TMU Router Configuration Capability Structure
//
typedef struct {
  AMD_USB4_TMU_RTR_CS_0     TmuRtrCs0;
  UINT32                    LocalTimeLow;
  UINT32                    LocalTimeMiddle;
  UINT16                    LocalTimeHigh;
  UINT16                    TSPacketInterval;
  UINT32                    TimeoffsetFromGMLow;
  UINT32                    TimeoffsetFromGMHigh;
  UINT32                    TimeoffsetFromMasterLow;
  UINT32                    TimeoffsetFromMasterHigh;
  UINT32                    FreqoffsetFromGM;
  UINT32                    FreqoffsetFromMaster;
  UINT32                    PropagationDelayLow;
  UINT32                    PropagationDelayHigh;
  UINT32                    ComputationTimeStampLow;
  UINT32                    ComputationTimeStampMiddle;
  UINT16                    ComputationTimeStampHigh;
  UINT16                    Reserved0;
  AMD_USB4_TMU_RTR_CS_15    TmuRtrCs15;
  UINT32                    InterDomainTimeStampLow;
  UINT32                    InterDomainTimeStampMiddle;
  UINT16                    InterDomainTimeStampHigh;
  UINT16                    Reserved1;
  UINT32                    TimeOffsetFromInterDomainGMLow;
  UINT32                    TimeOffsetFromInterDomainGMHigh;
  UINT32                    FreqOffsetFromInterDomainGM;
  UINT32                    PostLocalTimeLow;
  UINT32                    PostLocalTimeHigh;
  UINT32                    PostTimeLow;
  UINT32                    PostTimeHigh;
} AMD_USB4_TMU_ROUTER_CONFIGURATION_CAPABILITY;

//
// Vendor Specific Capability Structure VSC_CS_0
//
typedef struct {
  UINT8    NextCapabilityPointer;
  UINT8    CapabilityID;
  UINT8    VSCID;
  UINT8    VSCLength;
} AMD_USB4_VENDOR_SPECIFIC_CAPABILITY;

//
//  Vendor Specific Extended Capability Structure.
//
typedef struct {
  UINT8    Reserved;
  UINT8    CapabilityID;
  UINT8    VSCID;
  UINT8    VSECHeader;
  UINT16   NextCapabilityPointer;
  UINT16   VsecLength;
} AMD_USB4_VENDOR_SPECIFIC_EXTENDED_CAPABILITY;

/*
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
ADAPTER CONFIGURATION SPACE STRUCTURES
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/

//
// Individual bit fields for  ADP_CS_1
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    MaxCountSets:11;
    UINT32    CounterConfigurationSpaceFlag:1;
    UINT32    Reserved:12;
  } Field;
} AMD_USB4_ADP_CS1;

//
// Individual bit fields for  ADP_CS_3
//
typedef union {
  UINT32     Value;
  struct {
    UINT32    Reserved:20;
    UINT32    AdapterNumber:6;
    UINT32    Reserved1:3;
    UINT32    HECError:1;
    UINT32    FlowControlError:1;
    UINT32    SharedBufferingCapable:1;
  } Field;
} AMD_USB4_ADP_CS3;

//
// Individual bit fields for ADP_CS4
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NonFlowControlledBuffers:10;
    UINT32    Reserved:10;
    UINT32    TotalBuffers:10;
    UINT32    Plugged:1;
    UINT32    Lock:1;
  } Field;
} AMD_USB4_ADP_CS4;

//
// Individual bit fields for ADP_CS5
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    MaxInputHopID:11;
    UINT32    MaxOutputHopID:11;
    UINT32    LinkCreditsAllocated:7;
    UINT32    HECErrorEnable:1;
    UINT32    FlowControlErrorEnable:1;
    UINT32    DisableHotPlugEvents:1;
  } Field;
} AMD_USB4_ADP_CS5;

//
// Adapter Configuration Space Structure
//
typedef struct {
  UINT32              VendorDefined;
  AMD_USB4_ADP_CS1    AdpCs1;
  UINT8               AdapterTypeSubType;
  UINT8               AdapterTypeVersion;
  UINT8               AdapterTypeProtocol;
  UINT8               Reserved;
  AMD_USB4_ADP_CS3    AdpCs3;
  AMD_USB4_ADP_CS4    AdpCs4;
  AMD_USB4_ADP_CS5    AdpCs5;
  UINT32              HECErrors;
  UINT32              InvalidHopIDErrors;
  UINT32              ECCErrors;
} AMD_USB4_ADAPTER_CONFIGURATION_SPACE;

//
//  Type of Adapters
//
typedef enum {
  UnsupportedAdapter      = 0x000000,
  LaneAdapter             = 0x000001,
  HostInterfaceAdapter    = 0x000002,
  DownStreamPCIeAdapter   = 0x100101,
  UpStreamPCIeAdapter     = 0x100102,
  DPOutAdapter            = 0x0E0102,
  DPInAdapter             = 0x0E0101,
  DownStreamUSB3Adapter   = 0x200101,
  UpStreamUSB3Adapter     = 0x200102
} AMD_USB4_ADAPTER_TYPE;

//
//Protocol type supported
//
#define  UNSUPPORTED_ADAPTER_PROTOCOL          0x00
#define  LANE_ADAPTER_PROTOCOL                 0x00
#define  HOST_INTERFACE_ADAPTER_PROTOCOL       0x00
#define  DOWN_STREAM_PCIE_ADAPTER_PROTOCOL     0x10
#define  UP_STREAM_PCIE_ADAPTER_PROTOCOL       0x10
#define  DP_OUT_ADAPTER_PROTOCOL               0x0E
#define  DP_IN_ADAPTER_PROTOCOL                0x0E
#define  DOWN_STREAM_USB3_ADAPTER_PROTOCOL     0X20
#define  UP_STREAM_USB3_ADAPTER_PROTOCOL       0X20

//
//Versions supported
//
#define  UNSUPPORTED_ADAPTER_VERSION           0x00
#define  LANE_ADAPTER_VERSION                  0x00
#define  HOST_INTERFACE_ADPTER_VERSION         0x00
#define  DOWN_STREAM_PCIE_ADAPTER_VERSION      0x01
#define  UP_STREAM_PCIE_ADAPTER_VERSION        0x01
#define  DP_OUT_ADAPTER_VERSION                0x01
#define  DP_IN_ADAPTER_VERSION                 0x01
#define  DOWN_STREAM_USB3_ADAPTER_VERSION      0x01
#define  UP_STREAM_USB3_ADAPTER_VERSION        0x01

//
//Subversion
//
#define  UNSUPPORTED_ADAPTER_SUB_VER           0x00
#define  LANE_ADAPTER_SUB_VER                  0x01
#define  HOST_INTERFACE_ADAPTER_SUB_VER        0x02
#define  DOWN_STREAM_PCIE_ADAPTER_SUB_VER      0x01
#define  UP_STREAM_PCIE_ADAPTER_SUB_VER        0x02
#define  DP_OUT_ADAPTER_SUB_VER                0x02
#define  DP_IN_ADAPTER_SUB_VER                 0x01
#define  DOWN_STREAM_USB3_ADAPTER_SUB_VER      0x01
#define  UP_STREAM_USB3_ADAPTER_SUB_VER        0x02

///
/// Capabilities Supported by Adapter Configuration Space
///
#define   TMU_ADAPTER_CONFIG_CAP_ID            0x03
#define   LANE_ADAPTER_CONFIG_CAP_ID           0x01
#define   USB4_PORT_CAP_ID                     0x06
#define   PCIe_ADAPTER_CONFIG_CAP_ID           0x04
#define   DP_IN_ADAPTER_CONFIG_CAP_ID          0x04
#define   DP_OUT_ADAPTER_CONFIG_CAP_ID         0x04
#define   USB3_ADAPTER_CONFIG_CAP_ID           0x04
#define   VENDOR_SPECIFIC_CAP_ID               0x05


typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    CapabilityID:8;
    UINT32    Reserved:16;
  } Field;
} AMD_USB4_TMU_ADP_CS0;

//
// Individual bit fields for  TMU_ADP_CS3
//
typedef union {
  UINT32    Value;
  struct {
    UINT32    Reserved:29;
    UINT32    EnableUniDirectionalMode:1;
    UINT32    InterDomainMaster:1;
    UINT32    InterDomainSlave:1;
  } Field;
} AMD_USB4_TMU_ADP_CS3;

//
// Individual bit fields for  TMU_ADP_CS6
//
typedef union {
  UINT32    Value;
  struct {
    UINT32    Reserved0:1;
    UINT32    DisableTimeSync:1;
    UINT32    Reserved1:14;
    UINT32    VendorDefined:16;
  } Field;
} AMD_USB4_TMU_ADP_CS6;

//
// Individual bit fields for TMU_ADP_CS7
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    LostTSNOSCounter:10;
    UINT32    LostPacketCounter:10;
    UINT32    BadPacketCounter:10;
    UINT32    Reserved:2;
  } Field;
} AMD_USB4_TMU_ADP_CS7;

//
// TMU Adapter Configuration Capability Structure
//
typedef struct {
  AMD_USB4_TMU_ADP_CS0    TmuAdpCs0;
  UINT32                  TxTimetoWire;
  UINT32                  RxTimetoWire;
  AMD_USB4_TMU_ADP_CS3    TmuAdpCs3;
  UINT16                  RxTSNOSCounter;
  UINT16                  TxTSNOSCounter;
  UINT16                  RxPacketCounter;
  UINT16                  TxPacketCounter;
  AMD_USB4_TMU_ADP_CS6    TmuAdpCs6;
  AMD_USB4_TMU_ADP_CS7    TmuAdpCs7;
} AMD_USB4_TMU_ADAPTER_CONFIGURATION_CAPABILITY;

//
// Individual bit fields  for LANE_ADP_CS0
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    CapabilityID:8;
    UINT32    SupportedLinkSpeeds:4;
    UINT32    SupportedLinkWidths:6;
    UINT32    CL0sSupport:1;
    UINT32    CL1Support:1;
    UINT32    CL2Support:1;
    UINT32    Reserved:3;
  } Field;
} AMD_USB4_LANE_ADP_CS0;

///
/// Supported Link Speed
///
#define   SUPPORTED_LINK_SPEED_GEN2         0x08
#define   SUPPORTED_LINK_SPEED_GEN3         0x0C

///
/// Supported Link Width
///
#define   SUPPORT_LINK_WIDTH_X1             0x01
#define   SUPPORT_LINK_WIDTH_X2             0x02

///
/// Target Link Speed
///
#define   TARGET_LINK_SPEED_GEN2            0x08
#define   TARGET_LINK_SPEED_GEN3            0x0C

///
/// Target Link Width
///
#define   TARGET_LINK_WIDTH_X1              0x01
#define   TARGET_LINK_WIDTH_X2              0x03

///
/// Current Link Speed
///
#define   CURRENT_LINK_SPEED_GEN2           0x08
#define   CURRENT_LINK_SPEED_GEN3           0x04

///
/// Current Link Width
///
#define   CURRENT_LINK_WIDTH_X1             0x01
#define   CURRENT_LINK_WIDTH_X2             0x02

///
/// Adapter State
///
#define   ADAPTER_STATE_DISABLED            0x00
#define   ADAPTER_STATE_TRAINING            0x01
#define   ADAPTER_STATE_CL0                 0x02
#define   ADAPTER_STATE_TX_CL0S             0x03
#define   ADAPTER_STATE_RX_CL0S             0x04
#define   ADAPTER_STATE_CL1                 0x05
#define   ADAPTER_STATE_CL2                 0x06
#define   ADAPTER_STATE_CLd                 0x07

//
// Individual bit fields for LANE_ADP_CS1
//
typedef union {
  UINT32     Value;
  struct {
    UINT32    TargetLinkSpeed:4;
    UINT32    TargetLinkWidth:6;
    UINT32    CL0sEnable:1;
    UINT32    CL1Enable:1;
    UINT32    CL2Enable:1;
    UINT32    Reserved0:1;
    UINT32    LinkDisable:1;
    UINT32    LaneBonding:1;
    UINT32    CurrentLinkSpeed:4;
    UINT32    NegotiatedLinkWidth:6;
    UINT32    AdapterState:4;
    UINT32    PMSecondary:1;
    UINT32    Reserved1:1;
  } Field;
} AMD_USB4_LANE_ADP_CS1;

//
// Individual bit fields for LANE_ADP_CS2
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    LogicalLayerErros:7;
    UINT32    Reserved0:9;
    UINT32    LogicalLayerErrorEnable:7;
    UINT32    Reserved1:9;
  } Field;
} AMD_USB4_LANE_ADP_CS2;

//
// Lane Adapter Configuration Capability Structure
//
typedef struct {
  AMD_USB4_LANE_ADP_CS0     LaneAdpCs0;
  AMD_USB4_LANE_ADP_CS1     LaneAdpCs1;
  AMD_USB4_LANE_ADP_CS2     LaneAdpCs2;
} AMD_USB4_LANE_ADAPTER_CONFIGURATION_CAPABILITY;

//
// Individual bit fields for PORT_CS0
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    CapabilityID:8;
    UINT32    Reserved:16;
  } Field;
} AMD_USB4_PORT_CS0;

//
// Individual bit fields for PORTS_CS1
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    Address:8;
    UINT32    Length:8;
    UINT32    Target:3;
    UINT32    Reserved0:1;
    UINT32    RetimerIndex:4;
    UINT32    WnR:1;
    UINT32    NoResponse:1;
    UINT32    ResultCode:1;
    UINT32    Reserved1:4;
    UINT32    Pending:1;
  } Field;
} AMD_USB4_PORT_CS1;

//
// Individual bit fields for PORTS_CS18
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    CableUSB4Version:8;
    UINT32    BondingEnabled:1;
    UINT32    TBT3CompatibleMode:1;
    UINT32    LinkCLxSupport:1;
    UINT32    RsFecEnabledGen2:1;
    UINT32    RsFecEnabledGen3:1;
    UINT32    RouterDetected:1;
    UINT32    Reserved0:2;
    UINT32    WakeOnConnectStatus:1;
    UINT32    WakeOnDisconnectStatus:1;
    UINT32    WakeOnUSB4WakeStatus:1;
    UINT32    WakeOnInterDomainStatus:1;
    UINT32    Reserved1:12;
  } Field;
} AMD_USB4_PORT_CS18;

//
// Individual bit fields for PORTS_CS19
//
typedef union {
  UINT32     Value;
  struct {
    UINT32    DownStreamPortReset:1;
    UINT32    RequestRsFecGen2:1;
    UINT32    RequestRsFecGen3:1;
    UINT32    USB4PortisConfigured:1;
    UINT32    USB4PortisInterDomain:1;
    UINT32    Reserved0:11;
    UINT32    EnableWakeonConnect:1;
    UINT32    EnableWakeonDisconnect:1;
    UINT32    EnableWakeonUSB4Wake:1;
    UINT32    EnableWakeonInterDomain:1;
    UINT32    Reserved2:12;
  } Field;
} AMD_USB4_PORT_CS19;

//
// USB4 Port Capability Structure
//
typedef struct {
  AMD_USB4_PORT_CS0      PortCs0;
  AMD_USB4_PORT_CS1      PortCs1;
  UINT32                 Data[16];
  AMD_USB4_PORT_CS18     PortCs18;
  AMD_USB4_PORT_CS19     PortCs19;
} AMD_USB4_PORT_CAPABILITY;

//
// Individual bit fields for ADP_USB3_CS_0
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    CapabilityID:8;
    UINT32    Reserved:14 ;
    UINT32    Valid:1;
    UINT32    PathEnable:1;
  } Field;
} AMD_USB4_ADP_USB3_CS_0;

//
// Individual bit fields for ADP_USB3_CS_1
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    ConsumedUpstreamBandwidth:12;
    UINT32    ConsumedDownstreamBandwidth:12;
    UINT32    Reserved:7;
    UINT32    HostControllerAck:1;
  } Field;
} AMD_USB4_ADP_USB3_CS_1;

//
// Individual bit fields for ADP_USB3_CS_2
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    AllocatedUpstreamBandwidth:12;
    UINT32    AllocatedDownstreamBandwidth:12;
    UINT32    Reserved:7;
    UINT32    ConnectionManagerRequest:1;
  } Field;
} AMD_USB4_ADP_USB3_CS_2;

//
// Individual bit fields for ADP_USB3_CS_3
//
typedef union {
  UINT32    Value;
  struct {
    UINT32  Scale:6;
    UINT32  Reserved:26;
  } Field;
} AMD_USB4_ADP_USB3_CS_3;

//
// Individual bit fields for ADP_USB3_CS_4
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    ActualLinkRate:7;
    UINT32    USb3LinkValid:1;
    UINT32    PortLinkState:4;
    UINT32    MaximumSupportedLinkRate:7;
    UINT32    Reserved:13;
  } Field;
} AMD_USB4_ADP_USB3_CS_4;

//
// USB3 Adapter Configuration Capability Structure.
//
typedef struct {
  AMD_USB4_ADP_USB3_CS_0    AdpUSB3_Cs0;
  AMD_USB4_ADP_USB3_CS_1    AdpUSB3_Cs1;
  AMD_USB4_ADP_USB3_CS_2    AdpUSB3_Cs2;
  AMD_USB4_ADP_USB3_CS_3    AdpUSB3_Cs3;
  AMD_USB4_ADP_USB3_CS_4    AdpUSB3_Cs4;
} AMD_USB3_ADAPTER_CONFIGURATION_CAPABILITY;

//
// DP Adapter Configuration Capability.
//
// Individual bit fields for  ADP_DP_CS_0
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    CapabilityID:8;
    UINT32    VideoHopID:11 ;
    UINT32    Reserved:3;
    UINT32    AUXEnable:1;
    UINT32    VideoEnable:1;
  } Field;
} AMD_USB4_ADP_DP_CS_0;

//
// Individual bit fields for ADP_DP_CS_1
//
typedef union {
  UINT32    Value;
  struct {
    UINT32  AUXTxHopID:11;
    UINT32  AUXRxHopID:11;
    UINT32  Reserved:10;
  } Field;
} AMD_USB4_ADP_DP_CS_1;

//
// Individual bit fields for ADP_DP_CS_2
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    Reserved0:3;
    UINT32    SWLinkInit:1;
    UINT32    Reserved1:2;
    UINT32    HPDStatus:1;
    UINT32    Reserved2:25;
  } Field;
} AMD_USB4_ADP_DP_CS_2;

//
// Individual bit fields for ADP_DP_CS_3
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    VendorDefined:9;
    UINT32    HPDOutputClear:1;
    UINT32    HPDOutputSet:1;
    UINT32    VendorDefined1:21;
  } Field;
} AMD_USB4_ADP_DP_CS_3;

//
// Individual bit fields  for  DP_LOCAL_CAP
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    ProtocolAdapterVersion:4;
    UINT32    MaximalDPCDRev:4;
    UINT32    MaximalLinkRate:4;
    UINT32    MaximalLaneCount:3;
    UINT32    MSTCapability:1;
    UINT32    Reserved0:6;
    UINT32    TPS3Capability:1;
    UINT32    Reserved1:1;
    UINT32    TPS4Capability:1;
    UINT32    FECNotSupported:1;
    UINT32    SecondarySplitCapability:1;
    UINT32    LTTPRNotSupported:1;
    UINT32    Reserved2:1;
    UINT32    DSCNotSupported:1;
    UINT32    Reserved3:2;
  } Field;
} AMD_USB4_DP_LOCAL_CAP;

//
// Individual bit fields for DP_REMOTE_CAP
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    ProtocolAdapterVersion:4;
    UINT32    MaximalDPCDRev:4;
    UINT32    MaximalLinkRate:4;
    UINT32    MaximalLaneCount:3;
    UINT32    MSTCapability:1;
    UINT32    Reserved0:6;
    UINT32    TPS3Capability:1;
    UINT32    Reserved1:1;
    UINT32    TPS4Capability:1;
    UINT32    FECNotSupported:1;
    UINT32    SecondarySplitCapability:1;
    UINT32    LTTPRNotSupported:1;
    UINT32    Reserved2:1;
    UINT32    DSCNotSupported:1;
    UINT32    Reserved3:2;
  } Field;
} AMD_USB4_DP_REMOTE_CAP;

//
// Individual bit fields for DP_STATUS
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    LaneCount:3;
    UINT32    Reserved0:5;
    UINT32    LinkRate:4;
    UINT32    Reserved1:5;
    UINT32    Reserved2:15;
  } Field;
} AMD_USB4_DP_STATUS;

//
// Individual bit fields for DP_COMMON_CAP
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    ProtocolAdapterVersion:4;
    UINT32    MaximalDPCDRev:4;
    UINT32    MaximalLinkRate:4;
    UINT32    MaximalLaneCount:3;
    UINT32    MSTCapability:1;
    UINT32    Reserved0:6;
    UINT32    TPS3Capability:1;
    UINT32    Reserved1:1;
    UINT32    TPS4Capability:1;
    UINT32    FECNotSupported:1;
    UINT32    SecondarySplitCapability:1;
    UINT32    LTTPRNotSupported:1;
    UINT32    Reserved2:1;
    UINT32    DSCNotSupported:1;
    UINT32    Reserved3:1;
    UINT32    DPRXCapabilitiesReadDone:1;
  } Field;
} AMD_USB4_DP_COMMON_CAP;

//
// DP IN Adapter Configuration Capability Structure.
//
typedef struct {
  AMD_USB4_ADP_DP_CS_0      AdpDpCs0;
  AMD_USB4_ADP_DP_CS_1      AdpDpCs1;
  AMD_USB4_ADP_DP_CS_2      AdpDpCs2;
  AMD_USB4_ADP_DP_CS_3      AdpDpCs3;
  AMD_USB4_DP_LOCAL_CAP     DpLocalCap;
  AMD_USB4_DP_REMOTE_CAP    DpRemoteCap;
  AMD_USB4_DP_STATUS        DpStatus;
  AMD_USB4_DP_COMMON_CAP    DpCommonCap;
} AMD_USB4_DP_IN_ADAPTER_CONFIGURATION_CAPABILITY;

//
// Individual bit fields for ADP_DP_CS_0
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    CapabilityID:8;
    UINT32    VideoHopID:11;
    UINT32    Reserved:3;
    UINT32    AUXEnable:1;
    UINT32    VideoEnable:1;
  } Field;
} AMD_USB4_ADP_DP_OUT_CS_0;

//
// Individual bit fields for ADP_DP_CS_1
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    AUXTxHopID:11;
    UINT32    AUXRxHopID:11;
    UINT32    Reserved:10;
  } Field;
} AMD_USB4_ADP_DP_OUT_CS_1;

//
// Individual bit fields for ADP_DP_CS_2
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    Reserved0:3;
    UINT32    SWLinkInit:1;
    UINT32    Reserved1:2;
    UINT32    HPDStatus:1;
    UINT32    Reserved2:1;
    UINT32    MaximumAccumulationCycles:16;
    UINT32    Reserved3:8;
  } Field;
} AMD_USB4_ADP_DP_OUT_CS_2;

//
// Individual bit fields for DP_LOCAL_CAP
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    ProtocolAdapterVersion:4;
    UINT32    MaximalDPCDRev:4;
    UINT32    MaximalLinkRate:4;
    UINT32    MaximalLaneCount:3;
    UINT32    MSTCapability:1;
    UINT32    Reserved0:6;
    UINT32    TPS3Capability:1;
    UINT32    Reserved1:1;
    UINT32    TPS4Capability:1;
    UINT32    FECNotSupported:1;
    UINT32    SecondarySplitCapability:1;
    UINT32    LTTPRNotSupported:1;
    UINT32    Reserved2:1;
    UINT32    DSCNotSupported:1;
    UINT32    Reserved3:2;
  } Field;
} AMD_USB4_ADP_DP_OUT_LOCAL_CAP;

//
// Individual bit fields for DP_REMOTE_CAP
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    ProtocolAdapterVersion:4;
    UINT32    MaximalDPCDRev:4;
    UINT32    MaximalLinkRate:4;
    UINT32    MaximalLaneCount:3;
    UINT32    MSTCapability:1;
    UINT32    Reserved0:6;
    UINT32    TPS3Capability:1;
    UINT32    Reserved1:1;
    UINT32    TPS4Capability:1;
    UINT32    FECNotSupported:1;
    UINT32    SecondarySplitCapability:1;
    UINT32    LTTPRNotSupported:1;
    UINT32    Reserved2:1;
    UINT32    DSCNotSupported:1;
    UINT32    Reserved3:2;
  } Field;
} AMD_USB4_ADP_DP_OUT_REMOTE_CAP;

//
// Individual bit fields for DP_STATUS
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    LaneCount:3;
    UINT32    Reserved0:5;
    UINT32    LinkRate:4;
    UINT32    Reserved1:5;
    UINT32    Reserved2:8;
    UINT32    CMHandShake:1;
    UINT32    DPInAdapterUSB4Flag:1;
    UINT32    Reserved3:5;
  } Field;
} AMD_USB4_ADP_DP_OUT_STATUS_CTRL;

//
// Individual bit fields for DP_COMMON_CAP
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    ProtocolAdapterVersion:4;
    UINT32    MaximalDPCDRev:4;
    UINT32    MaximalLinkRate:4;
    UINT32    MaximalLaneCount:3;
    UINT32    MSTCapability:1;
    UINT32    Reserved0:6;
    UINT32    TPS3Capability:1;
    UINT32    Reserved1:1;
    UINT32    TPS4Capability:1;
    UINT32    FECNotSupported:1;
    UINT32    SecondarySplitCapability:1;
    UINT32    LTTPRNotSupported:1;
    UINT32    Reserved2:1;
    UINT32    DSCNotSupported:1;
    UINT32    Reserved3:2;
  } Field;
} AMD_USB4_ADP_DP_OUT_COMMON_CAP;

//
// DP OUT Adapter Configuration Capability Structure.
//
typedef struct {
  AMD_USB4_ADP_DP_OUT_CS_0          AdpDpCs0;
  AMD_USB4_ADP_DP_OUT_CS_1          AdpDpCs1;
  AMD_USB4_ADP_DP_OUT_CS_2          AdpDpCs2;
  UINT32                            VendorDefined;
  AMD_USB4_ADP_DP_OUT_LOCAL_CAP     DpLocalCap;
  AMD_USB4_ADP_DP_OUT_REMOTE_CAP    DpRemoteCap;
  AMD_USB4_ADP_DP_OUT_STATUS_CTRL   DpStatusCtrl;
  AMD_USB4_ADP_DP_OUT_COMMON_CAP    DpCommonCap;
  UINT32                            Reserved;
} AMD_USB4_DP_OUT_ADAPTER_CONFIGURATION_CAPABILITY;

///
/// Capabilities Supported by DP Adapter Configuration Space
///
#define  DP_TMU_ADAPTER_CONFIGURATION_CAPABILITY_ID    0x03
#define  DP_ADAPTER_VENDOR_SPECIFIC_CAPABILITY_ID      0x05

//
// PCI E Adapter Configuration Capability.
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    CapabilityID:8;
    UINT32    Link:1;
    UINT32    TxE1:1;
    UINT32    RxE1:1;
    UINT32    RST:1;
    UINT32    VendorDefined:5;
    UINT32    LTTSM:4;
    UINT32    VendorDefined1:2;
    UINT32    PathEnable:1;
  } Field;
} AMD_USB4_PCIE_ADAPTER_CONFIGURATION_CAPABILITY;

/*
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
PATH CONFIGURATION SPACE STRUCTURES
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/

//
// Individual bit fields for PATH_CS_0
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    VendorDefined1:6;
    UINT32    Reserved1:11;
    UINT32    PathCreditsAllocated:7;
    UINT32    Reserved2:7;
    UINT32    VendorDefined2:1;
  } Field;
} AMD_USB4_PATH_CS_0;

//
// Individual bit fields for PATH_CS_1
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    VendorDefined1:12;
    UINT32    CounterID:11;
    UINT32    CounterEnable:1;
    UINT32    VendorDefined2:8;
  } Field;
} AMD_USB4_PATH_CS_1;

//
// PATH 0 Entry Configuration Space
//
typedef struct {
  AMD_USB4_PATH_CS_0    PathCs0;
  AMD_USB4_PATH_CS_1    PathCs1;
} AMD_USB4_PATH0_ENTRY_CONFIGURATION_SPACE;

//
// Individual bit fields for PATH_CS_0
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    OutputHopID:11;
    UINT32    OutputAdapter:6;
    UINT32    PathCreditsAllocated:7;
    UINT32    Reserved2:7;
    UINT32    Valid:1;
  } Field;
} AMD_USB4_PATHN_CS_0;

//
// Individual bit fields for PATH_CS_1
//
typedef union {
  UINT32      Value;
  struct {
    UINT32    Weight:8;
    UINT32    Priority:3;
    UINT32    Reserved0:1;
    UINT32    CounterID:11;
    UINT32    CounterEnable:1;
    UINT32    IngressFlowControlFlag:1;
    UINT32    EgressFlowControlFlag:1;
    UINT32    IngresSharedBufferingEnableFlag:1;
    UINT32    EgressSharedBufferingEnableFlag:1;
    UINT32    PendingPackets:1;
    UINT32    Reserved1:1;
    UINT32    Reserved2:2;
  } Field;
} AMD_USB4_PATHN_CS_1;

//
// PATH Entry  'n' in Configuration Space at Lane Adapter
//
typedef struct {
  AMD_USB4_PATHN_CS_0      PathNLaneCs0;
  AMD_USB4_PATHN_CS_1      PathNLaneCs1;
} AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_LANE_ADAPTER;

//
// PATH Entry  'n' in Configuration Space at Protocol Adapter
//
typedef struct {
  AMD_USB4_PATHN_CS_0      PathNProtocolCs0;
  AMD_USB4_PATHN_CS_1      PathNProtocolCs1;
} AMD_USB4_PATHN_ENTRY_CONFIGURATION_SPACE_AT_PROTOCOL_ADAPTER;


/*
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
COUNTER  CONFIGURATION SPACE STRUCTURES
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/

//
// Individual bit fields for ADP_PCIE_CS_0
//
typedef struct {
  UINT32    ReceivedPacketLow;
  UINT32    ReceivedPacketsHigh;
  UINT32    DroppedPackets;
} AMD_USB4_COUNTER_CONFIGURATION_SPACE;


/*
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
Interoperability with Thunderbolt 3 (TBT3) Systems
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
*/
typedef union {
  UINT32      Value;
  struct {
    UINT32  TBT3_Compatible1  : 3;
    UINT32  PlugEventDisable  : 4;
    UINT32  TBT3_Compatible2  : 25;
  } Field;
} AMD_USB4_TBT3VSC1_CS_1;

typedef union {
  UINT32      Value;
  struct {
    UINT32  LinkErrorAdpA           : 1;
    UINT32  HecErrorAdpA            : 1;
    UINT32  FlowControlErrorAdpA    : 1;
    UINT32  Rsvd1                   : 1;
    UINT32  LinkErrorAdpB           : 1;
    UINT32  HecErrorAdpB            : 1;
    UINT32  FlowErrorAdpB           : 1;
    UINT32  Rsvd2                   : 1;
    UINT32  LinkErrorAdpC           : 1;
    UINT32  HecErrorAdpC            : 1;
    UINT32  FlowControlErrorAdpC    : 1;
    UINT32  Rsvd3                   : 1;
    UINT32  LinkErrorAdpD           : 1;
    UINT32  HecErrorAdpD            : 1;
    UINT32  FlowControlErrorAdpD    : 1;
    UINT32  Rsvd4                   : 1;
    UINT32  LinkErrorEnAdpA         : 1;
    UINT32  HecErrorEnAdpA          : 1;
    UINT32  FlowControlErrorEnAdpA  : 1;
    UINT32  Rsvd5                   : 1;
    UINT32  LinkErrorEnAdpB         : 1;
    UINT32  HecErrorEnAdpB          : 1;
    UINT32  FlowErrorEnAdpB         : 1;
    UINT32  Rsvd6                   : 1;
    UINT32  LinkErrorEnAdpC         : 1;
    UINT32  HecErrorEnAdpC          : 1;
    UINT32  FlowControlErrorEnAdpC  : 1;
    UINT32  Rsvd7                   : 1;
    UINT32  LinkErrorEnAdpD         : 1;
    UINT32  HecErrorEnAdpD          : 1;
    UINT32  FlowControlErrorEnAdpD  : 1;
    UINT32  Rsvd8                   : 1;
  } Field;
} AMD_USB4_TBT3VSC1_CS_3;

typedef union {
  UINT32    Value;
  struct {
    UINT32  FL_SK               : 1;
    UINT32  FL_CS               : 1;
    UINT32  FL_DI               : 1;
    UINT32  FL_DO               : 1;
    UINT32  BitBangingEnable    : 1;
    UINT32  InvalidFlashMemory  : 1;
    UINT32  TBT3_Compatible     : 26;
  } Field;
} AMD_USB4_TBT3VSC1_CS_4;


typedef struct {
  AMD_USB4_VENDOR_SPECIFIC_CAPABILITY Header;
  AMD_USB4_TBT3VSC1_CS_1              Vsec1Cs1;
  UINT32                              TBT3_Compatible1;
  AMD_USB4_TBT3VSC1_CS_3              Vsec1Cs3;
  AMD_USB4_TBT3VSC1_CS_4              Vsec1Cs4;
  UINT32                              TBT3_Compatible2[7];
  UINT32                              DROMBaseAddress;
} AMD_USB4_TBT3_VENDOR_SPECIFIC_1_CAP;

typedef struct {
  AMD_USB4_VENDOR_SPECIFIC_CAPABILITY Header;
} AMD_USB4_TBT3_VENDOR_SPECIFIC_3_CAP;

typedef struct {
  AMD_USB4_VENDOR_SPECIFIC_CAPABILITY Header;
} AMD_USB4_TBT3_VENDOR_SPECIFIC_4_CAP;

typedef struct {
  AMD_USB4_VENDOR_SPECIFIC_CAPABILITY Header;
} AMD_USB4_TBT3_VENDOR_SPECIFIC_6_CAP;

typedef struct {
  AMD_USB4_VENDOR_SPECIFIC_CAPABILITY Header;
} AMD_USB4_DP_TBT3_VENDOR_SPECIFIC_6_CAP;

typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    CapabilityID:8;
    UINT32    Rsvd:16;
  } Field;
} AMD_USB4_TBT3_TMU_DP_CS_0;

typedef union {
  UINT32      Value;
  struct {
    UINT32    Rsvd1:16;
    UINT32    TmuStable:8;
    UINT32    Rsvd2:8;
  } Field;
} AMD_USB4_TBT3_TMU_DP_CS_13;

typedef struct {
  AMD_USB4_TBT3_TMU_DP_CS_0          Tbt3TmuDpCs0;
  AMD_USB4_TBT3_TMU_DP_CS_13         Tbt3TmuDpCs13;
} AMD_USB4_TBT3_DP_TMU_ADAPTER_CONFIGURATION_CAPABILITY;

typedef union {
  UINT32      Value;
  struct {
    UINT32    NextCapabilityPointer:8;
    UINT32    CapabilityID:8;
    UINT32    VscId:8;
    UINT32    VscLength:8;
  } Field;
} AMD_USB4_TBT3_ADP_DP_VSC_0_CS_0;

typedef union {
  UINT32      Value;
  struct {
    UINT32    Valid:1;
    UINT32    Cmd:1;
    UINT32    Rsvd:30;
  } Field;
} AMD_USB4_TBT3_ADP_DP_IP_CMD;

typedef union {
  UINT32      Value;
  struct {
    UINT32    Address:16;
    UINT32    Rsvd:16;
  } Field;
} AMD_USB4_TBT3_ADP_DP_IP_ADDR;

typedef struct {
  AMD_USB4_VENDOR_SPECIFIC_CAPABILITY    Header;
  AMD_USB4_TBT3_ADP_DP_IP_CMD            IpCmd;
  AMD_USB4_TBT3_ADP_DP_IP_ADDR           IpAddr;
  UINT32                                 WriteData;
  UINT32                                 ReadData;
} AMD_USB4_TBT3_DP_ADAPTER_VENDOR_SPECIFIC_CAP_1;

#pragma  pack (pop)

#endif
