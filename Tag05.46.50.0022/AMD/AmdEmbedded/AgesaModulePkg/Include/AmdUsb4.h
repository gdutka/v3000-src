/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_USB4_H_
#define _AMD_USB4_H_

#include <AmdUsb4/Drom.h>

#define AMD_USB4_PRE_OS_CM_PEI_SUPPORT              0

#define AMD_USB4_DEBUG                              1

#if AMD_USB4_PRE_OS_CM_PEI_SUPPORT
#undef AMD_USB4_DEBUG
#define AMD_USB4_DEBUG                              0
#endif

#define AMD_USB4_DEBUG_TMU_ENABLE                   1

#define AMD_USB4_DEBUG_PRE_OS_CM_DISABLE            0 // Skip Pre-OS CM
#define AMD_USB4_DEBUG_BP_BEFORE_CM                 0 // Break point after MMIO allocation and before CM.
                                                      // Check CMOS[0x88] value, 0x5A stop.
                                                      // WARNING: Should not enable it in offical release.
#define AMD_USB4_DEBUG_LANE_HOTPLUG_DISABLE         0 // Lane Adapter Hotplug Disable
#define AMD_USB4_DEBUG_DP_HOTPLUG_DISABLE           0 // DP Adapter Hotplug Disable
#define AMD_USB4_DEBUG_LANE_ADAPTER_SKIP            0 // Skip Lane Adapter
#define AMD_USB4_DEBUG_USB3_ADAPTER_SKIP            0 // Skip Usb3 Adapter
#define AMD_USB4_DEBUG_PCIE_ADAPTER_SKIP            0 // Skip Pcie Adapter
#define AMD_USB4_DEBUG_DPIN_ADAPTER_SKIP            0 // Skip DP In Adapter
#define AMD_USB4_DEBUG_DPOUT_ADAPTER_SKIP           0 // Skip DP Out Adapter
#define AMD_USB4_DEBUG_WAIT_OV_INFINITE_TIMEOUT     0 // Router Operation check OV bit infinite timeout
#define AMD_USB4_DEBUG_EXPOSE_PREOS_CM_PROTOCOL     0 // Expose Pre-OS CM Protocol and CV test workaround
#define AMD_USB4_DEBUG_RESET_HOST_INTERFACE         0 // Reset Host Interface before Pre-OS CM.
#define AMD_USB4_DEBUG_CLEAR_RING_BUFFER            0 // Clear Ring Data buffer
#define AMD_USB4_DEBUG_POSTCODE                     0
#define   USB4_DEBUG_PC_HOTPLUG                     0x08CCCCC0
#define   USB4_DEBUG_PC_READREQUEST                 0x08DDDDD0
#define   USB4_DEBUG_PC_READRESPONSE                0x08DDDDD1
#define   USB4_DEBUG_PC_WRITEREQUEST                0x08EEEEE0
#define   USB4_DEBUG_PC_WRITERESPONSE               0x08EEEEE1

#define TP_HOST_ROUTER_INIT                         0x08100000
#define   TP_HOST_ROUTER_INIT_ROUTER                0x08110000
#define   TP_HOST_ROUTER_INIT_DROM                  0x08120000
#define   TP_HOST_ROUTER_INIT_ADAPTER               0x08130000
#define   TP_HOST_ROUTER_INIT_LINK_ADAPTER          0x08140000
#define   TP_HOST_ROUTER_INIT_TURNON_TUNNEL         0x08150000
#define   TP_HOST_ROUTER_INIT_TMU                   0x08160000

#define TP_DEVICE_ROUTER_INIT                       0x08200000
#define   TP_DEVICE_ROUTER_INIT_DETECT_ROUTER       0x08210000
#define   TP_DEVICE_ROUTER_INIT_ROUTER              0x08220000
#define   TP_DEVICE_ROUTER_INIT_ADAPTER             0x08230000
#define   TP_DEVICE_ROUTER_INIT_DROM                0x08240000
#define   TP_DEVICE_ROUTER_INIT_LINK_BONDING        0x08250000
#define   TP_DEVICE_ROUTER_INIT_LINK_ADAPTER        0x08260000
#define   TP_DEVICE_ROUTER_INIT_TURNON_TUNNEL       0x08270000
#define   TP_DEVICE_ROUTER_INIT_TMU                 0x08280000

#define TP_WAIT_DPOUT                               0x08300000
#define TP_DP_TUNNELING                             0x08400000
#define TP_USB_TUNNELING                            0x08500000
#define TP_PCI_TUNNELING                            0x08600000


#define AMD_USB4_HOST_AMOUNT                        2

#define AMD_USB4_CM_VERSION                        0x00000001ul

#define AMD_USB4_MAX_HOST_NUM                      8
#define AMD_USB4_MAX_HOST_USB3                     8
#define AMD_USB4_MAX_HOST_PCIE                     8
#define AMD_USB4_MAX_HOST_DP                       8
#define AMD_USB4_TEMP_BUS_BASE                     0x15

#define AMD_USB4_HOST_MMIO_SIZE                    0x00100000                    // Need to add size detection


#define MAX_RING_DESCRIPTOR_NUM                    4096                          // Max Ring descriptor number
#define MAX_RING_NUMBER                            1

#define RING_TX_DESCRIPTOR_NUM                     MAX_RING_DESCRIPTOR_NUM
#define RING_RX_DESCRIPTOR_NUM                     MAX_RING_DESCRIPTOR_NUM

#define RING_TX_SIZE                               (RING_TX_DESCRIPTOR_NUM<<4)   // Transmit Descriptor size is 16 bytes
#define RING_RX_SIZE                               (RING_RX_DESCRIPTOR_NUM<<4)   // Receive Descriptor size is 16 bytes

#define MAX_CONTROL_PACKET_SIZE                    (64 << 2)                     // 64 * 4 bytes
#define RING_TX_BUFFER_SIZE                        (RING_TX_DESCRIPTOR_NUM * MAX_CONTROL_PACKET_SIZE)
#define RING_RX_BUFFER_SIZE                        (RING_RX_DESCRIPTOR_NUM * MAX_CONTROL_PACKET_SIZE)

#define MAX_NOTIFICATION_QUEUE_SIZE                 64
#define MAX_HP_QUEUE_SIZE                           64

#define TX_RING0_SIZE                               8
#define RX_RING0_SIZE                               8
#define TX_RING0_BUFFER_SIZE                        TX_RING0_SIZE * MAX_CONTROL_PACKET_SIZE
#define RX_RING0_BUFFER_SIZE                        RX_RING0_SIZE * MAX_CONTROL_PACKET_SIZE
#define MAX_DFP                                     8

#if AMD_USB4_PRE_OS_CM_PEI_SUPPORT
#define MAX_CM_DATA_ARRAY_AMOUNT                    128
#else
#define MAX_CM_DATA_ARRAY_AMOUNT                    4096
#endif

#define MAX_CM_DROM_BUFFER_SIZE                     512

#define MAX_ADAPTER_AMOUNT_IN_ROUTER                64

#define USB3_DOWNSTREAM_PATH_HOPID                  9
#define USB3_UPSTREAM_PATH_HOPID                    9
#define PCIE_DOWNSTREAM_PATH_HOPID                  10
#define PCIE_UPSTREAM_PATH_HOPID                    10
#define DP_MAIN_PATH_HOPID                          11
#define DP_IN_AUX_PATH_HOPID                        12
#define DP_OUT_AUX_PATH_HOPID                       11

#define MIN_PCIE_PATH_CREDITS                       6

// USB4 CM guide Table 5-4: USB3 Path Attributes
#define USB3_TO_LANE_PRIORITY                   3
#define USB3_TO_LANE_WEIGHT                     2
#define USB3_TO_LANE_EFC                        1
#define USB3_TO_LANE_ESE                        0
#define LANE_TO_USB3_PRIORITY                   3
#define LANE_TO_USB3_WEIGHT                     1
#define LANE_TO_USB3_IFC                        1
#define LANE_TO_USB3_EFC                        0
#define LANE_TO_USB3_ISE                        0
#define LANE_TO_USB3_ESE                        0

// USB4 CM guide Table 5-1: PCIe Path Attributes
#define PCIE_TO_LANE_PRIORITY                   3
#define PCIE_TO_LANE_WEIGHT                     1
#define PCIE_TO_LANE_EFC                        1
#define PCIE_TO_LANE_ESE                        0
#define LANE_TO_PCIE_PRIORITY                   3
#define LANE_TO_PCIE_WEIGHT                     1
#define LANE_TO_PCIE_IFC                        1
#define LANE_TO_PCIE_EFC                        0
#define LANE_TO_PCIE_ISE                        0
#define LANE_TO_PCIE_ESE                        0

// USB4 CM guide Table 5-3: DP Path Attributes
#define DP_AUX_DP_ADAPTER_TO_LANE_INPUT_HOPID   8
#define DP_AUX_DP_ADAPTER_TO_LANE_PRIORITY      2
#define DP_AUX_DP_ADAPTER_TO_LANE_WEIGHT        1
#define DP_AUX_DP_ADAPTER_TO_LANE_IFC           0
#define DP_AUX_DP_ADAPTER_TO_LANE_EFC           1
#define DP_AUX_DP_ADAPTER_TO_LANE_ISE           0
#define DP_AUX_DP_ADAPTER_TO_LANE_ESE           0

#define DP_AUX_LANE_TO_LANE_PRIORITY            2
#define DP_AUX_LANE_TO_LANE_WEIGHT              1
#define DP_AUX_LANE_TO_LANE_IFC                 1
#define DP_AUX_LANE_TO_LANE_EFC                 1
#define DP_AUX_LANE_TO_LANE_ISE                 0
#define DP_AUX_LANE_TO_LANE_ESE                 0

#define DP_AUX_LANE_TO_DP_ADAPTER_OUTPUT_HOPID  8
#define DP_AUX_LANE_TO_DP_ADAPTER_PRIORITY      2
#define DP_AUX_LANE_TO_DP_ADAPTER_WEIGHT        1
#define DP_AUX_LANE_TO_DP_ADAPTER_IFC           1
#define DP_AUX_LANE_TO_DP_ADAPTER_EFC           0
#define DP_AUX_LANE_TO_DP_ADAPTER_ISE           0
#define DP_AUX_LANE_TO_DP_ADAPTER_ESE           0

#define DP_MAIN_DP_ADAPTER_TO_LANE_INPUT_HOPID  9
#define DP_MAIN_DP_ADAPTER_TO_LANE_PRIORITY     1
#define DP_MAIN_DP_ADAPTER_TO_LANE_WEIGHT       1
#define DP_MAIN_DP_ADAPTER_TO_LANE_IFC          0
#define DP_MAIN_DP_ADAPTER_TO_LANE_EFC          0
#define DP_MAIN_DP_ADAPTER_TO_LANE_ISE          0
#define DP_MAIN_DP_ADAPTER_TO_LANE_ESE          0

#define DP_MAIN_LANE_TO_LANE_PRIORITY           1
#define DP_MAIN_LANE_TO_LANE_WEIGHT             1
#define DP_MAIN_LANE_TO_LANE_IFC                0
#define DP_MAIN_LANE_TO_LANE_EFC                0
#define DP_MAIN_LANE_TO_LANE_ISE                0
#define DP_MAIN_LANE_TO_LANE_ESE                0

#define DP_MAIN_LANE_TO_DP_ADAPTER_OUTPUT_HOPID 9
#define DP_MAIN_LANE_TO_DP_ADAPTER_PRIORITY     1
#define DP_MAIN_LANE_TO_DP_ADAPTER_WEIGHT       1
#define DP_MAIN_LANE_TO_DP_ADAPTER_IFC          0
#define DP_MAIN_LANE_TO_DP_ADAPTER_EFC          0
#define DP_MAIN_LANE_TO_DP_ADAPTER_ISE          0
#define DP_MAIN_LANE_TO_DP_ADAPTER_ESE          0

#define USB4_RAW_BANDWIDTH_SCALING              1000
#define USB4_RAW_BANDWIDTH_10GBPS               (10 * USB4_RAW_BANDWIDTH_SCALING)
#define USB4_RAW_BANDWIDTH_20GBPS               (20 * USB4_RAW_BANDWIDTH_SCALING)
#define USB4_RAW_BANDWIDTH_40GBPS               (40 * USB4_RAW_BANDWIDTH_SCALING)

#define GUARD_BAND_BANDWIDTH_NUMERATOR          9
#define GUARD_BAND_BANDWIDTH_DENOMINATOR        10
#define DP_LINK_8B_10B_ENCODING_NUMERATOR       8
#define DP_LINK_8B_10B_ENCODING_DENOMINATOR     10

#define MAX_ADAPTER_AMOUNT_IN_ROUTER            64
#define MAX_ROUTER_DEPTH                        5

// DROM
#define MAX_UNUSED_ADAPTERS                          9
#define MAX_DP_ADAPTERS                              8
#define MAX_TBT3_ADAPTERS                            8
#define MAX_USB3_PORT_MAPPINGS                       8

// VSC
#define MAX_VSC_COUNT                           6

// DP Out HPD Timeout
#define DPOUT_HPD_TIMEOUT                       100000  // 100ms


// DP Out HPD Timeout
#define TRAININGTIMEOUT_USB4                    1500000  // 1.5s
#define TRAININGTIMEOUT_TBT3                    2500000  // 2.5s




#pragma pack (push, 1)

//
// Structure for Notification and Hotplug Queue
//
typedef struct {
  UINT32  AddressLow;
  UINT32  AddressHigh;
  UINT32  NotificationDW3;  //AdapterNumber and EventCode
} AMD_USB4_NOTIFICATION_INFO_ENTRY;

typedef struct {
  UINT32  TopologyIDLow;
  UINT32  TopologyIDHigh  : 24;
  UINT32  AdapterNumber   : 6;
  UINT32  UPG             : 1;
  UINT32  Valid           : 1;  /// < 1 - Entry Valid; 0 - Entry Invalid
  UINT32  PMTimer;
} AMD_USB4_HOTPLUG_INFO_ENTRY;

//
// PCI Device Function Byte.
//
typedef struct {
  UINT8    FunctionNum:3;
  UINT8    DeviceNum:5;
} AMD_USB4_PCI_DEV_FUN_BYTE;

//
// PCI B/D/F location
//
typedef struct {
  UINT8                        Bus;
  AMD_USB4_PCI_DEV_FUN_BYTE    DevFun;
} AMD_USB4_PCI_BDF;

//
// USB4 HOST Location
//
typedef struct {
  UINT8                         HostPciePortBus;         /// PCI bus of upstream bridge
  AMD_USB4_PCI_DEV_FUN_BYTE     HostPciePortDevFun;      /// Dev/Fun of upstream bridge
  UINT16                        HostPciePort04h;         /// Command of upstream bridge
  UINT32                        HostPciePort20h;         /// Memory range of upstream bridge
  UINT8                         HostBus;                 /// assigned PCI bus for USB4 Host
  AMD_USB4_PCI_DEV_FUN_BYTE     HostDevFun;              /// Dev/Fun of USB4 Host
  BOOLEAN                       HostBusAssigned;         /// Host Bus already assigned
} AMD_USB4_HOST_PCI_ADDRESS;

//
// USB4 HOST Bus Info
//
typedef struct {
  UINT8    HostBus;         //PCI bus of USB4 Host
  UINT8    HostCount;       //how many USB4 Host on this bus
} AMD_USB4_HOST_BUS_INFO;

//
// Data Structure Definition
//
typedef enum {
  CELL_TYPE_HOST_ROUTER       = 0xE0,
  CELL_TYPE_DEVICE_ROUTER     = 0xE1,
  CELL_TYPE_DP_IN_ADAPTER     = 0xF0,
  CELL_TYPE_DP_OUT_ADAPTER    = 0xF1,
  CELL_TYPE_USB3_UP_ADAPTER   = 0xF2,
  CELL_TYPE_USB3_DOWN_ADAPTER = 0xF3,
  CELL_TYPE_PCIE_UP_ADAPTER   = 0xF4,
  CELL_TYPE_PCIE_DOWN_ADAPTER = 0xF5,
  CELL_TYPE_LANE_ADAPTER      = 0xF6,
  CELL_TYPE_HI_ADAPTER        = 0xF7,
  CELL_TYPE_CONTROL_ADAPTER   = 0xF8,
  CELL_TYPE_UNUSED_ADAPTER    = 0xFF
} AMD_USB4_CM_CELL_TYPE;

typedef enum {
  LINK_RATE_10_GBPS  = 0x0,     // Gen-2 - Single Lane
  LINK_RATE_20_GBPS  = 0x1      // Gen-2 - Dual Lane
} AMD_USB4_USB3_LINK_RATE;

typedef enum {
  ADAPTER_STATE_RESET   = 0x0,
  ADAPTER_STATE_PRESENT = 0x1,
  ADAPTER_STATE_PLUGGED = 0x2,
  ADAPTER_STATE_PAIRED  = 0x3
} AMD_USB4_DP_ADAPTER_STATE;

typedef enum {
  LINK_RATE_1_62_GHZ = 0x0,
  LINK_RATE_2_7_GHZ  = 0x1,
  LINK_RATE_5_4_GHZ  = 0x2,
  LINK_RATE_8_1_GHZ  = 0x3
} AMD_USB4_DP_LINK_RATE;

typedef enum {
  LANE_COUNT_1 = 0x0,
  LANE_COUNT_2 = 0x1,
  LANE_COUNT_4 = 0x2
} AMD_USB4_DP_LANE_COUNT;

typedef enum {
  TS_PACKET_INTERVAL_DISABLE  = 0,
  TS_PACKET_INTERVAL_HIFI     = 16,
  TS_PACKET_INTERVAL_LOWRES   = 1000
} AMD_USB4_TS_PACKET_INTERVAL_MODE;

typedef struct _AMD_USB4_CM_CELL_HEADER AMD_USB4_CM_CELL_HEADER;
typedef union _AMD_USB4_CM_CELL AMD_USB4_CM_CELL;
typedef struct _AMD_USB4_CM_DP_ADAPTER AMD_USB4_CM_DP_ADAPTER;

typedef struct _AMD_USB4_CM_CELL_HEADER {
  AMD_USB4_CM_CELL_TYPE   CellType;   // 0xE0 - Host Router
                                      // 0xE1 - Device Router
                                      // 0xF0 - DP IN Adapter
                                      // 0xF1 - DP OUT Adapter
                                      // 0xF2 - Usb3 UP Adapter
                                      // 0xF3 - Usb3 Down Adapter
                                      // 0xF4 - PCIe Up Adapter
                                      // 0xF5 - PCIe Down Adapter
                                      // 0xF6 - Lane Adapter
                                      // 0xF7 - Host Interface Adapter
                                      // 0xF8 - Control Adapter
                                      // 0xFF - Unused Adapter
  UINT64                  TopologyID;
  UINT8                   AdapterNum; // For Router, it is 0x00
  AMD_USB4_CM_CELL_HEADER *Parent;    // Pointer Parent Router or Upstream Router's Lane 0 Adapter
  UINT8                   Reserved[3];
} AMD_USB4_CM_CELL_HEADER;

typedef struct _AMD_USB4_ROUTER_TMU {
  UINT32  TSPacketInterval    : 16;   // TMU_RTR_CS_3.TSPacketInterval
  UINT32  UCAP                : 1;    // TMU_RTR_CS_0.Uni-Directional Capability (UCAP)
  UINT32  UniDirectionalMode  : 1;    // Upstream Port mode. 1 - Uni-Directional Mode; 0 - Bi-Directional Mode
  UINT32  Reserved            : 14;
} AMD_USB4_ROUTER_TMU;


//
// ============= DROM Start =============
//
typedef union {
  UINT8       Value;
  struct {
    UINT8     AdapterNumber   : 6;
    UINT8     AdapterDisable  : 1;
    UINT8     AdapterEntry    : 1;
  } Field;
} AMD_USB4_DROM_UNUSED_ADP_ENTRY;

typedef union {
  UINT16      Value;
  struct {
    UINT8     AdapterNumber   : 6;
    UINT8     AdapterDisable  : 1;
    UINT8     AdapterEntry    : 1;
    UINT8     PA              : 6;  // Preferred Lane Adapter (PA)
                                    // Identifies the Lane Adapter which is preferred when setting up a Path to this DP Adapter. This field
                                    // is valid when the PV Valid is set to 1b, otherwise this field is ignored.
    UINT8     PV              : 1;  // A bit that signifies that this adapter has a preferred Lane Adapter
                                    // as specified in the Preferred Lane Adapter field.
  } Field;
} AMD_USB4_DROM_DP_ADP_ENTRY;

typedef union {
  UINT16      Value;
  struct {
    UINT8     AdapterNumber   : 6;
    UINT8     AdapterDisable  : 1;
    UINT8     AdapterEntry    : 1;
    UINT8     L1A             : 1;  // Lane 1 Adapter (L1A)  ( 0 - Lane0; 1 - Lane1 )
    UINT8     DLC             : 1;  // Dual-Lane Link Capable (DLC) ( 0 - Not Capable; 1 - Capable )
    UINT8     SecAdapterNum   : 6;  // The second Lane Adapter in the same USB4 Port.
  } Field;
} AMD_USB4_DROM_TBT3_LANE_ADP_ENTRY;

typedef union {
  UINT16      Value;
  struct {
    UINT8     AdapterNumber   : 6;
    UINT8     AdapterDisable  : 1;
    UINT8     AdapterEntry    : 1;
    UINT8     Fn              : 3;  // Function Number
    UINT8     Dev             : 5;  // Device Number
  } Field;
} AMD_USB4_DROM_TBT3_PCIE_ADP_ENTRY;

typedef union {
  UINT8       Value;
  struct {
    UINT8     TMUMode         : 2;  // A 2-bit value containing the preferred TMU Mode
                                    // 0 - Off; 1 - Unidirectional; 2 - Bidirectional; 3 - Reserved
    UINT8     TMURefreshRate  : 2;  // A 2-bit value containing the preferred TMU refresh rate
                                    // 0 - Reserved; 1 - HiFi; 2 - LowRes; 3 - Reserved
  } Field;
} AMD_USB4_DROM_TMU_ENTRY;

typedef union {
  UINT32      Value;
  struct {
    UINT32    UsbPortNumber           : 4;
    UINT32    PdPortNumber            : 5;
    UINT32    TC                      : 1;
    UINT32    Usb3DownStreamAdpNumber : 6;
    UINT32    TS                      : 1;
  } Field;
} AMD_USB4_DROM_USB_PORT_MAPPING_ENTRY;

//
// ============= DROM End =============
//


//
// USB4 ROUTER DATA
//
typedef struct _AMD_USB4_CM_ROUTER{
  AMD_USB4_CM_CELL_HEADER            Header;
  UINT16                             VendorId;           //Vendor ID
  UINT16                             ProductId;          //Product ID
  UINT64                             UUID;               //UUID
  UINT8                              Depth;              //Depth
  UINT8                              MaxAdapter;         //Max Adapter Number
  UINT8                              UpstreamAdapter;    //Upstream Adapter or Host I/F Adapter
  UINT8                              CapabilityOffset;   //Pointer to Capability
  UINT8                              Revision;           //Revision
  UINT8                              Usb4Version;        //USB4 Version
  UINT8                              Enumerated:1;       //Router Enumerated
  UINT8                              PcieSupport:1;      //PCIe Tunneling Support
  UINT8                              Usb3Support:1;      //USB3 Tunneling Support
  UINT8                              PcieTunnelOn:1;     //PCIe Tunneling Enabled
  UINT8                              Usb3TunnelOn:1;     //USB3 Tunneling Enabled
  UINT8                              TBT3Support:1;      //TBT3 support
  UINT8                              IsHost:1;           //Host Router or Device Router
  UINT8                              InternalHost:1;     //Internal Host Controller
  UINT8                              InternalHostOn;     //Internal Host Controller On
  AMD_USB4_CM_CELL                   *pAdapter;          //Base for Adapter Data
  UINT8                              DFPAmount;          //DFP port number
  UINT8                              DpInAmount;         //DP IN Adapter number
  UINT8                              DpOutAmount;        //DP OUT Adapter number
  UINT8                              Usb3UpAmount;       //USB3 Up Adapter number
  UINT8                              Usb3DownAmount;     //USB3 Down Adapter number
  UINT8                              PcieUpAmount;       //PCIe Up Adapter number
  UINT8                              PcieDownAmount;     //PCIe Down Adapter number
  AMD_USB4_CM_CELL                   *pAdapterUsb3Up;
  AMD_USB4_CM_CELL                   *pAdapterUsb3Down;
  AMD_USB4_CM_CELL                   *pAdapterPcieUp;
  AMD_USB4_CM_CELL                   *pAdapterPcieDown;
  AMD_USB4_CM_CELL                   *pAdapterDpIn;
  AMD_USB4_CM_CELL                   *pAdapterDpOut;
  AMD_USB4_CM_CELL                   *pAdapterUFP;
  AMD_USB4_CM_CELL                   *pAdapterDFP;
  AMD_USB4_CM_CELL                   *pAdapterHostInterface;
  //DRom_Data
  UINT32                             DromVersion;
  AMD_USB4_DROM_UNUSED_ADP_ENTRY     UnusedAdapEntry[MAX_UNUSED_ADAPTERS];
  UINT8                              NumOfUnusedAdapEntries;
  AMD_USB4_DROM_DP_ADP_ENTRY         DpAdapEntry[MAX_DP_ADAPTERS];
  UINT8                              NumOfDpAdapEntries;
  AMD_USB4_DROM_TBT3_LANE_ADP_ENTRY  Tbt3LaneAdapEntry[MAX_TBT3_ADAPTERS];
  UINT8                              NumOfTbt3LaneAdapEntries;
  AMD_USB4_DROM_TBT3_PCIE_ADP_ENTRY  Tbt3UpStreamAdapEntry[MAX_TBT3_ADAPTERS];
  UINT8                              NumOfTbt3UpStreamAdapEntries;
  AMD_USB4_DROM_TBT3_PCIE_ADP_ENTRY  Tbt3DownStreamAdapEntry[MAX_TBT3_ADAPTERS];
  UINT8                              NumOfTbt3DownStreamAdapEntries;
  AMD_USB4_DROM_TMU_ENTRY            DromTMU;
  AMD_USB4_DROM_USB_PORT_MAPPING_ENTRY  Usb3PortMap[MAX_USB3_PORT_MAPPINGS];
  UINT8                              NumOfUsb3PortMapEntries;
  // DP
  UINT16                             BufferAllocationMaxUSB3;
  UINT16                             BufferAllocationMinDPAux;
  UINT16                             BufferAllocationMinDPMain;
  UINT16                             BufferAllocationMaxPcie;
  UINT16                             BufferAllocationMaxHI;
  // TMU
  UINT8                              TMUCapability;                   // TMU Capability offset
  AMD_USB4_ROUTER_TMU                TMUInfo;
  // Vendor Specific Capability
  UINT8                              VSCapability[MAX_VSC_COUNT];     // Vendor Specific Capability
  UINT16                             VSECapability;                   // Vendor Specific Extended Capability
  UINT64                             PtoUtoTimeStamp;                 // Time Stamp of PTO/UTO enable
  // TBT3
  UINT32                             Tbt3DROMBase;                    // Tbt3 DROM base (in bytes). Zero means no DROM base.
} AMD_USB4_CM_ROUTER;

//
// USB4 ADAPTER HEADER
//
typedef struct _AMD_USB4_CM_ADAPTER_HEADER{
  UINT16                      MaxInputHopId;      //Max Input HopID
  UINT16                      MaxOutputHopId;     //Max Output HopID
  AMD_USB4_CM_CELL            *pSiblingAdapter;
  AMD_USB4_CM_CELL            *pLinkPartner;
  UINT32                       Reserved;
} AMD_USB4_CM_ADAPTER_HEADER;

typedef struct {
  UINTN  OverFlow;
  UINTN  EndValue;
  UINTN  OldTimerValue;
  UINT8  IsInfinite;
} USB4_TIMEOUT;

//
// USB4 Lane Adapter
//
typedef struct _AMD_USB4_CM_LANE_ADAPTER {
  AMD_USB4_CM_CELL_HEADER            Header;
  AMD_USB4_CM_ADAPTER_HEADER         AdpHeader;          //Adapter Header
  UINT32                             TotalBuffer:10;     //Total Buffer
  UINT32                             NFCBuffer:10;       //Non Flow Control Buffer
  UINT32                             Plugged:1;          //Plugged
  UINT32                             Lock:1;             //Lock
  UINT32                             HE:1;               //HEC Error
  UINT32                             FCE:1;              //Flow Control Error
  UINT32                             SBC:1;              //Shared Buffer Capable
  UINT32                             LinkCredit:7;       //Link Credit Allocated
  UINT32                             HEE:1;              //HEC Error Enable
  UINT32                             FCEE:1;             //Flow Control Error Enable
  UINT32                             DHP:1;              //Disable Hot Plug Event
  UINT32                             Usb3TunnelOn:1;
  UINT32                             PcieTunnelOn:1;
  UINT32                             DpMainTunnel1On:1;
  UINT32                             DpAuxInTunnel1On:1;
  UINT32                             DpAuxOutTunnel1On:1;
  UINT32                             DpMainTunnel2On:1;
  UINT32                             DpAuxInTunnel2On:1;
  UINT32                             DpAuxOutTunnel2On:1;
  UINT32                             Reserved0:21;       //

  //TMU Capability
  UINT8                              TMUCapability;      //TMU Capability offset
  //Lane Capability
  UINT8                              LaneCapability;            //Lane Capability offset
  UINT8                              Reserved1[2];
  UINT32                             LaneAdp_SupportSpeed:4;    //Supported Link Speed
  UINT32                             LaneAdp_SupportWidth:6;    //Supported Link Width
  UINT32                             LaneAdp_TargetSpeed:4;     //Target Link Speed
  UINT32                             LaneAdp_TargetWidth:6;     //Target Link Width
  UINT32                             LaneAdp_CurrentSpeed:4;    //Current Link Speed
  UINT32                             LaneAdp_CurrentWidth:6;    //Current Link Width
  UINT32                             LaneAdp_LinkDisable:1;     //Link Disable
  UINT32                             LaneAdp_LinkBonding:1;     //Link Bonding
  UINT32                             LaneAdp_AdapterState:4;    //Adapter State
  UINT32                             LaneAdp_Reserve0:28;       //Reserved for future usage
  //USB4 Port Capability
  UINT32                             Usb4Capability;            //USB4 port Capability offset
  //UINT8                            Reserved2[3];
  UINT32                             Usb4Port_CableVersion:8;   //USB4 Cable Version
  UINT32                             Usb4Port_BE:1;             //Bonding Enable
  UINT32                             Usb4Port_TCM:1;            //TBT3 Compatible Mode
  UINT32                             Usb4Port_LCL:1;            //Link CLx support
  UINT32                             Usb4Port_RE2:1;            //RS-FEC Enable Gen2
  UINT32                             Usb4Port_RE3:1;            //RS-FEC Enable Gen3
  UINT32                             Usb4Port_RD:1;             //Router Detected
  UINT32                             Usb4Port_Reserve0:18;
  UINT32                             Usb4Port_Reserve1;

  //Flow Control
  UINT32                             RemainingBuffers:10;       //Remaining Buffers
  UINT32                             ControlPathCredits:7;      //Control Path Credits
  UINT32                             HiCredits:10;              //Host Interface Credits
  UINT32                             Reserved3:5;               //Reserved for future usage
  UINT32                             TotalDpCredits:10;         //Total DisplayPort Credits
  UINT32                             DpMainPath1Credits:10;     //DisplayPort Main Path 1 Credits
  UINT32                             DpMainPath2Credits:10;     //DisplayPort Main Path 2 Credits
  UINT32                             DpAuxPath1Credits:7;       //DisplayPort Aux Path 1 Credits
  UINT32                             DpAuxPath2Credits:7;       //DisplayPort Aux Path 2 Credits
  UINT32                             Reserved4:5;               //Reserved for future usage
  UINT32                             PcieCredits:7;             //PCIe Path Credits
  UINT32                             Usb3Credits:7;             //USB3 Path Credits
  UINT32                             Reserved5:1;               //Reserved for future usage

  //Bandwidth
  UINT32                             RawBandwidth;                   // Raw USB4 Link Bandwidth
  UINT32                             AvailableBandwidth;             // Available USB4 Link Bandwidth
  UINT32                             TotalDownstreamDpPathBandwidth; // Sum of downstream DP Tunneling Main-Link Path
  UINT32                             DownstreamDpPath1Bandwidth;     // Downstream DP Tunneling Main-Link Path 1
  UINT32                             DownstreamDpPath2Bandwidth;     // Downstream DP Tunneling Main-Link Path 2
  UINT32                             TotalUpstreamDpPathBandwidth;   // Sum of upstream DP Tunneling Main-Link Path
  UINT32                             UpstreamDpPath1Bandwidth;       // Upstream DP Tunneling Main-Link Path 1
  UINT32                             UpstreamDpPath2Bandwidth;       // Upstream DP Tunneling Main-Link Path 2
  UINT32                             DownstreamUsb3PathBandwidth;    // Sum of downstream USB3 Path
  UINT32                             UpstreamUsb3PathBandwidth;      // Sum of upstream USB3 Path going

  AMD_USB4_CM_ROUTER                 *pConnectedRouter;         //Connected Router
  USB4_TIMEOUT                       PMTimer;                   // Record ACPI timer when do DFP reset
} AMD_USB4_CM_LANE_ADAPTER;

typedef struct _AMD_USB4_CM_USB3_ADAPTER {
  AMD_USB4_CM_CELL_HEADER     Header;
  AMD_USB4_CM_ADAPTER_HEADER  AdpHeader;                  // Adapter Header
  UINT32                      USB3Capability        :8;
  UINT32                      PathValid             :1;   // Is Path CS Valid
  UINT32                      ActualLinkRate        :7;
  UINT32                      USB3LinkValid         :1;
  UINT32                      PortLinkState         :4;
  UINT32                      MaxSupportedLinkRate  :7;
  UINT32                      TunnelOn              :1;   // Adapter Tunneling on
  UINT32                      Reserved              :3;
} AMD_USB4_CM_USB3_ADAPTER;

typedef struct _AMD_USB4_CM_PCIE_ADAPTER {
  AMD_USB4_CM_CELL_HEADER     Header;
  AMD_USB4_CM_ADAPTER_HEADER  AdpHeader;                  // Adapter Header
  UINT32                      PcieCapability        :8;
  UINT32                      PathValid             :1;   // Is Path CS Valid
  UINT32                      PcieAdp_Link          :1;   // Link
  UINT32                      PcieAdp_TXEI          :1;   // TX EI
  UINT32                      PcieAdp_RXEI          :1;   // RX EI
  UINT32                      PcieAdp_RST           :1;   // RST
  UINT32                      PcieAdp_LTSSM         :4;   // LTSSM
  UINT32                      TunnelOn              :1;   // Adapter Tunneling on
  UINT32                      Reserved              :14;
} AMD_USB4_CM_PCIE_ADAPTER;

typedef struct _AMD_USB4_CM_DP_ADAPTER {
  AMD_USB4_CM_CELL_HEADER     Header;
  AMD_USB4_CM_ADAPTER_HEADER  AdpHeader;          //Adapter Header
  UINT32                      CapabilityOffset             :8;
  UINT32                      ProtocolAdapterVersion       :4;
  UINT32                      MaximalDPCDRev               :4;
  UINT32                      MaximalLinkRate              :4;
  UINT32                      MaximalLaneCount             :3;
  UINT32                      MSTCapability                :1;
  UINT32                      TPS3Capability               :1;
  UINT32                      TPS4Capability               :1;
  UINT32                      FECNotSupported              :1;
  UINT32                      SecondarySplitCapability     :1;
  UINT32                      LTTPRNotSupported            :1;
  UINT32                      DSCNotSupported              :1;
  UINT32                      AdapterState                 :2;  // 0x0 - Reset; 0x1: Present; 0x2 Plugged; 0x3: Paired
  UINT32                      MainTunnelOn                 :1;
  UINT32                      AuxInTunnelOn                :1;
  UINT32                      AuxOutTunnelOn               :1;
  UINT32                      DpLinkNumber                 :2;  // 0x0 - None; 0x1: First; 0x2 Second; 0x3: Reserved
  UINT32                      HPDStatus                    :1;
  UINT32                      TBT3DpTmuCapabilityOffset    :8;  // DP TMU Adapter Configuration Capability
  UINT32                      TBT3DpVSCCapabilityOffset    :8;  // DP Adapter Vendor Specific Capability
  UINT32                      MaximalLinkRateCommon        :4;
  UINT32                      MaximalLaneCountCommon       :3;
  UINT32                      Reserved                     :3;
  AMD_USB4_CM_DP_ADAPTER      *pPairedDpAdapter;
  LIST_ENTRY                  Link;
} AMD_USB4_CM_DP_ADAPTER;

typedef struct {
  AMD_USB4_CM_CELL_HEADER     Header;
  AMD_USB4_CM_ADAPTER_HEADER  AdpHeader;          //Adapter Header
  UINT32                      CapabilityOffset       :8;
  UINT32                      VSC1Offset             :8;
  UINT32                      Rsvd                  :16;
} AMD_USB4_CM_HI_ADAPTER;

typedef union _AMD_USB4_CM_CELL {
  AMD_USB4_CM_CELL_HEADER   Header;
  AMD_USB4_CM_ROUTER        Router;
  AMD_USB4_CM_DP_ADAPTER    DPAdapter;
  AMD_USB4_CM_PCIE_ADAPTER  PCIEAdapter;
  AMD_USB4_CM_USB3_ADAPTER  USB3Adapter;
  AMD_USB4_CM_LANE_ADAPTER  LaneAdapter;
  AMD_USB4_CM_HI_ADAPTER    HIAdapter;
} AMD_USB4_CM_CELL;

typedef struct _AMD_USB4_CM {
  UINT64  HIMmioBase;                                // Host Interface MMIO base address
  UINT64  RingTxBase[MAX_RING_NUMBER];               // Ring buffer of Transmit Rings' base address in host memory
  UINT64  RingRxBase[MAX_RING_NUMBER];               // Ring buffer of Receive Rings' base address in host memory
  UINT64  RingTxDataBufferBase[MAX_RING_NUMBER];     // Data buffer of Transmit Rings' base address in host memory
  UINT64  RingRxDataBufferBase[MAX_RING_NUMBER];     // Data buffer of Receive Rings' base address in host memory
  UINT64  RingDevTxBase[MAX_RING_NUMBER];            // Ring buffer of Transmit Rings' base address in host memory
  UINT64  RingDevRxBase[MAX_RING_NUMBER];            // Ring buffer of Receive Rings' base address in host memory
  UINT64  RingDevTxDataBufferBase[MAX_RING_NUMBER];  // Data buffer of Transmit Rings' base address in host memory
  UINT64  RingDevRxDataBufferBase[MAX_RING_NUMBER];  // Data buffer of Receive Rings' base address in host memory

  UINT16  RingTxProducerIndex[MAX_RING_NUMBER];   // Local copy of Transmit Rings' Producer index
  UINT16  RingTxConsumerIndex[MAX_RING_NUMBER];   // Local copy of Transmit Rings' Consumer index
  UINT16  RingRxProducerIndex[MAX_RING_NUMBER];   // Local copy of Receive Rings' Producer index
  UINT16  RingRxConsumerIndex[MAX_RING_NUMBER];   // Local copy of Receive Rings' Consumer index

  // Connected USB3/PCIe/DP
  AMD_USB4_PCI_BDF                 Usb3Controller[AMD_USB4_MAX_HOST_USB3];
  AMD_USB4_PCI_BDF                 PcieController[AMD_USB4_MAX_HOST_PCIE];
  AMD_USB4_PCI_BDF                 DpController[AMD_USB4_MAX_HOST_DP];

  // Pointer to pre-allocated 64 double words buffer.
  UINT32  *PtrControlPacketTxBuffer;
  UINT32  *PtrControlPacketRxBuffer;
  UINT32  *PtrControlPacketClearBuffer;

  // Pointer to DROM buffer.
  UINT8   *PtrDROMBuffer;

  // Queue for pending control packets
  AMD_USB4_NOTIFICATION_INFO_ENTRY  NotificationQueue[MAX_RING_NUMBER][MAX_NOTIFICATION_QUEUE_SIZE];
  AMD_USB4_HOTPLUG_INFO_ENTRY       HotPlugQueue[MAX_RING_NUMBER][MAX_HP_QUEUE_SIZE];

  // Lists for DP Adapters in Plugged State
  LIST_ENTRY   DpInPluggedList;
  LIST_ENTRY   DpOutPluggedList;
  UINT32       NumPairedAdapters;

  // Connection Manager Data Structure
  // Init code need set CmDataIndex to 0 and allocate memory size of MAX_CM_DATA_ARRAY_AMOUNT * sizeof(AMD_USB4_CM_CELL) for PtrCmData
  AMD_USB4_CM_CELL  *PtrCmData;   // Pointer to the first element of array.
  UINT32            CmDataIndex;  // Index of the array. point out how many elements of the array has been used.

  UINT8        CmTmuMode;         // TMU mode
  AMD_USB4_HOST_PCI_ADDRESS        HostRouterPciAddress;
  UINT8        RequestRsFecGen2;  // Request RS-FEC Gen 2
  UINT8        RequestRsFecGen3;  // Request RS-FEC Gen 3
  UINT8        LinkSpeed;         // USB4 port link speed

  VOID         *pUsb4PciIo;
  VOID         *pHostAddress;
  VOID         *pRingMapping;

  USB4_TIMEOUT  HPDWaitTimeout;
  UINT32        Misc;             // Bit[0]     - FCHUSB4PDSLVI2C::PD_LL_port0_data [0]: USB4 Mode;
                                  // Bit[1]     - FCHUSB4PDSLVI2C::PD_LL_port0_data [1]: TBT3 Alt Mode;
  UINT32        TBT3WaitCL0;
  UINT32        USB4WaitCL0;
} AMD_USB4_CM;

//
// AMD USB4 Pirvate Data Structure
//
typedef struct {
  UINT32                        AmdUsb4Version;                           // Driver version
  UINT32                        Usb4HostNum;                              // USB Host Number
  UINT32                        TempPcieBus;                              // Temporary PCIe bus for USB4 Host
  AMD_USB4_HOST_PCI_ADDRESS     *Usb4HostList;                            // PCI bus of upstream bridge
  UINT64                        Usb4HostMmioBase[AMD_USB4_MAX_HOST_NUM];  // MMIO base for each Host
  AMD_USB4_CM                   *pCM[AMD_USB4_MAX_HOST_NUM];              // Connection Manager
} AMD_USB4_HOST_DATA;


//
// DROM - Start
//
typedef union {
  UINT32      Value;
  struct {
    UINT32  Rsvd1     : 2;
    UINT32  Address   : 13; // DW address in DROM relative to DROM first address.
                            // Address = 0 targets the first DW in DROM.
    UINT32  ReadSize  : 5;  // Number of Doublewords that shall be read starting from the Address field value.
                            // The Read Size field shall be greater than 0 and less than or equal to 16.
    UINT32  Rsvd2     : 12;
  } Field;
} AMD_DROM_METADATA;

typedef enum {
  ROUTER_OPERATION_COMPLETED_SUCCESSFULLY = 0,
  ROUTER_OPERATION_FAILED_TO_EXECUTE
} AMD_DROM_RETURN_STATUS;
//
// DROM - End
//

typedef struct {
  AMD_USB4_CM   **ppCM;
  UINT32        Usb4HostNum;
} AMD_USB4_PROTOCOL;

#pragma pack (pop)


EFI_STATUS
AmdUsb4SetHostUpstreamPcieBus (
  IN OUT AMD_USB4_HOST_DATA         *pAmdUsb4Data,
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *Usb4Host
);

EFI_STATUS
AmdUsb4ResetHostUpstreamPcieBus (
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *Usb4Host
);

EFI_STATUS
AmdUsb4HostAllocateMMIO (
  IN     AMD_USB4_HOST_DATA  *pAmdUsbData,
  IN     UINT8               HostBus,
  IN     UINT8               Usb4HostCount,
     OUT UINT64              *Usb4MmioBase64
);

EFI_STATUS
AmdUsb4HostBridgeEnableMMIO (
  IN     AMD_USB4_HOST_DATA         *pAmdUsbData,
  IN     AMD_USB4_HOST_PCI_ADDRESS  Usb4Host,
  IN     UINT32                     Usb4HostMmioBase,
  IN     UINT32                     MmioSize
);

EFI_STATUS
AmdUsb4HostEnableMMIO (
  IN     AMD_USB4_HOST_DATA         *pAmdUsbData,
  IN     AMD_USB4_HOST_PCI_ADDRESS  Usb4Host,
  IN     UINT32                     Usb4HostMmioBase
);

BOOLEAN
AmdUsb4CheckHost (
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  Usb4Host
);

EFI_STATUS
AmdUsb4GetUsb4Hosts (
  IN     AMD_USB4_HOST_DATA         *pAmdUsbData,
  IN OUT UINT32                     *Usb4HostNum,
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *Usb4Host
);

EFI_STATUS
AmdUsb4InitHostMmio (
  IN     AMD_USB4_HOST_DATA         *pAmdUsbData,
  IN OUT UINT32                     *Usb4HostNum,
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *Usb4Host
);

#endif
