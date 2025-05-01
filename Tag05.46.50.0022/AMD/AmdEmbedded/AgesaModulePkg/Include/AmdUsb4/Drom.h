/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _USB4_DROM_H_
#define _USB4_DROM_H_

#define MAX_NAME_SIZE                                250
#define MAX_ASCII_NAME_SIZE                          252

#pragma  pack (push, 1)
//
//  Individual bit fields for  DROM Header Part
//  Header size =  16 bytes
//
typedef struct {
  UINT8     Reserved1;         // 0
  UINT32    Reserved2;         // 0
  UINT32    Reserved3;         // 0
  UINT32    Crc32;             // 32bit crc value which protects USB4 drom starting from version
  UINT8     Version;           // usb4 DROM spec version
  UINT16    Length:12;         // Total size of DROM structure in bytes starting from Version Field
  UINT16    Reserved4:4;       // 0
} DROM_HEADER;

//
//  Individual bit fields for  TBT3 Identification Section
//  Header size =  9 bytes
//
typedef struct {
  UINT8     Crc8;              // 8 bit Crc Value
  UINT64    UUID;              // Universal Uniq ID as defined in USB4 Spec.
                               // Bit definitions : Router ID [3:0], Component ID [47:4], Silicon Vendor ID [63:48]
} TBT3_IDENTIFICATION_SECTION;

//
//  Individual bit fields for  TBT3 Header Section
//  Header size =  13 bytes
//
typedef struct {
  UINT32     Crc32;              // 32 bit Crc Value
  UINT8      Version;            // DROM SPecific Version
  UINT16     Length:12;          // Total size of DROM structure in bytes starting from Version Field
  UINT16     Reserved:4;         // 0
  UINT16     TBT3VendorID;       // A 16 Bit Number  That Identifies the TBT3 vendor of this Product
  UINT16     TBT3ModelID;        // A 16 Bit Number  That Identifies the TBT3 Model of this Product
  UINT8      TBT3ModelRevision;  // A 8 bit Number That Identifies the Model Revision Number
  UINT8      TBT3NvmRevision;    // A 8 bit Number That Identifies the Nvm Revision
} TBT3_HEADER_SECTION;

//
// Individual bit fields for Unused Adapter part
// Unused adapter struct size =  2 bytes
//
typedef struct {
  UINT8   EntryLength;           // Size of this structure entry, shall be set to 2 ( 2 bytes)
  UINT8   AdapterNumber:6;       // AdapterNumber in Configuration space of router
  UINT8   AdapterDisabled:1;     // 1 indicates adapter disabled / 0 indicates  adapter enabled
  UINT8   AdapterEntry:1;        // 1 ( adapter entry)
} UNUSED_ADAPTER_ENTRY;

//
// Individual bit fields for DP Adapter part
// DP adapter struct size =  5 bytes
//
typedef struct {
  UINT8   EntryLength;              // Size of this structure entry,
  UINT8   AdapterNumber:6;          // AdapterNumber in Configuration space of router
  UINT8   AdapterDisabled:1;        // 1 indicates adapter disabled / 0 indicates  adapter enabled
  UINT8   AdapterEntry:1;           // 1 ( adapter entry)
  UINT16  Reserved;                 // 0
  UINT8   PreferedLaneAdapter:6;    // This is used for setting up path to DP
  UINT8   PreferedValid:1;          // This indicates adapter has lane adapter
  UINT8   Reserved1:1;              // 0
} DP_ADAPTER_ENTRY;

//
// Individual bit fields for  TBT3 Lane Adapter Entry
// Lane adapter struct size =  8 bytes
//
typedef struct {
  // Offset 0
  UINT8   EntryLength;              // Size of this structure entry
  // Offset 1
  UINT8   AdapterNumber:6;          // AdapterNumber in Configuration space of router
  UINT8   AdapterDisabled:1;        // 1 indicates adapter disabled / 0 indicates  adapter enabled
  UINT8   AdapterEntry:1;           // 1 ( adapter entry)
  // Offset 2
  UINT8   Reserved1:4;              // 0
  UINT8   Lane1Adapter:1;           // 1 Bit value that identifies lane0 or lane1
  UINT8   Reserved2:2;              // 0
  UINT8   DualLaneLinkCapable:1;    // 0:Not Capable 1: Capable
  // Offset 3
  UINT8   SecAdapNum:6;             // which Identifies Second Lane Adapter
  UINT8   Reserved3:2;              // 0
  // Offset 4
  UINT8   Reserved4;                // 0
  // Offset 5
  UINT8   Reserved5;                // 0
  // Offset 6
  UINT8   Reserved6;                // 0
  // Offset 7
  UINT8   Reserved7;                // 0
} TBT3_LANE_ADAPTER_ENTRY;

//
// Individual bit fields for TBT3 PCIe UpStream Adapter Entry
// Size :11 Bytes
//
typedef struct {
  // Offset 0
  UINT8   EntryLength;          // Size of this structure entry
  // Offset 1
  UINT8   AdapterNumber:6;      // AdapterNumber in Configuration space of router
  UINT8   AdapterDisabled:1;    // 1 indicates adapter disabled / 0 indicates  adapter enabled
  UINT8   AdapterEntry:1;       // 1 ( adapter entry)
  // Offset 2
  UINT8   FunctionNumber:3;     //Pci e Device Function Number
  UINT8   DevHi:2;              //
  UINT8   DevLo:3;              //
  // Offset 3
  UINT16  Reserved1;            // 0
  // Offset 5
  UINT16  Reserved2;            // 0
  // Offset 7
  UINT16  Reserved3;            // 0
  // Offset 9
  UINT16  Reserved4;            // 0
} TBT3_PCIE_UPSTREAM_ADAPTER_ENTRY;

//
// Individual bit fields for TBT3 PCIe DownStream Adapter Entry
// Size :3 Bytes
//
typedef struct {
  // Offset 0
  UINT8   EntryLength;          // Size of this structure entry
  // Offset 1
  UINT8   AdapterNumber:6;      // AdapterNumber in Configuration space of router
  UINT8   AdapterDisabled:1;    // 1 indicates adapter disabled / 0 indicates  adapter enabled
  UINT8   AdapterEntry:1;       // 1 ( adapter entry)
  // Offset 2
  UINT8   FunctionNumber:3;     //Pci e Device Function Number
  UINT8   DevHi:2;              //
  UINT8   DevLo:3;              //
} TBT3_PCIE_DOWNSTREAM_ADAPTER_ENTRY;

//
// Generic Entries
// Individual bit fields for Generic entry
// ASCII Vendor Name structure
//
typedef struct {
  UINT8   EntryLength;                            // Size of this structure entry
  UINT8   Type:6;                                 // 0x1 for ASCII vendor name
  UINT8   Reserved:1;                             // 0.
  UINT8   AdapterEntry:1;                         // 0 ( Generic entry)
  UINT8   ACSCIIVendorName[MAX_ASCII_NAME_SIZE];  // The Vendor name in ASCII.
} ASCII_VENDOR_NAME_ENTRY;

//
//  Individual bit fields for Generic entry ASCII Model Name structure
//  struct size =  N+1 Bytes
//
typedef struct {
  UINT8   EntryLength;                            // Size of this structure entry
  UINT8   Type:6;                                 // 0x2 for ASCII Model name
  UINT8   Reserved:1;                             // 0.
  UINT8   AdapterEntry:1;                         // 0 ( Generic entry)
  UINT8   ACSCIIModelName [MAX_ASCII_NAME_SIZE];  // The Model name in ASCII.
} ASCII_MODEL_NAME_ENTRY;

//
//  Individual bit fields for Generic entry TMU  Minimum Request mode structure
//  struct size =  3 bytes
//
typedef struct {
  UINT8   EntryLength;           // Size of this structure entry, shall be set to 3 bytes
  UINT8   Type:6;                // 0x8 for TMU minimun request mode
  UINT8   Reserved:1;            // 0.
  UINT8   AdapterEntry:1;        // 0 ( Generic entry)
  UINT8   TMUMode:2;             // TMU mode 0->off, 1->Uni directional , 2->Bi directional, 3->reserved
  UINT8   TMURefreshRate:2;      // TMU refresh rate 0->Reserved, 1->hifi, 2->Low res, 3->Reserved
  UINT8   Reserved1:4;           // 0
} TMU_MINIMUM_REQUESTMODE_ENTRY;

//
//  Individual bit fields for Generic entry Product Descriptor structure
//  struct size =  15 bytes
//
typedef struct {
  UINT8   EntryLength;           // Size of this structure entry
  UINT8   Type:6;                // 0x9 for Product descriptor
  UINT8   Reserved:1;            // 0.
  UINT8   AdapterEntry:1;        // 0 ( Generic entry)
  UINT16  bcsUSBSpec;            // Usb sepcification version Number
  UINT16  idVendor;              // Product Vendor ID(PVI).Value Assigned by USB-IF.
  UINT16  idProduct;             // Product ID (PI).Value Assigned by Vendor.
  UINT16  bcdProductFWRevision;  // Product Firmware revision. Value Assigned by Vendor
  UINT32  TID;                   // TEST ID.Value Assigned by USB-IF.
  UINT8   ProductHWRevision;     // Product Hardware Revision. Value Assigned by Vendor
} PRODUCT_DESCRIPTOR_ENTRY;

//
//  Individual bit fields for Generic entry  Serial Number structure
//
typedef struct {
  UINT8   EntryLength;                  // Size of this structure entry
  UINT8   Type:6;                       // 0xA for Serial Number
  UINT8   Reserved:1;                   // set to 0.
  UINT8   AdapterEntry:1;               // 0 ( Generic entry)
  UINT16  wLANGID;                      // A 16-bit language ID (LANGID) defined by the USB-IF.
  UINT8   SerialNumber[MAX_NAME_SIZE];  // Serial number
} SERIAL_NUMBER_ENTRY;

//
//  Individual bit fields for Generic entry  USB Port Mapping  structure
//  struct size = 3 bytes- Usb3 port, Pdport, AdapterNumber
//
typedef union{
  UINT8 Value;
  struct{
    UINT8  Usb3PortNumber:4;      // This Field Indicates Down stream USb3 Port Number
    UINT8  Reserved0:4;           // Shall be Set to 0
  } Field;
} AMD_USB4_USB3_PORT_NUMBER;

typedef union{
  UINT8 Value;
  struct{
    UINT8  PDPortNumber:5;        // This Field Conatins The Port Number of USB PD controller
    UINT8  Reserved1:2;           // Shall  be set to 0
    UINT8  USbTypeC:1;            // This Field set to 1 when usb3 port is connect to type c
  } Field;
} AMD_USB4_PD_PORT_NUMBER;

typedef union{
  UINT8 Value;
  struct{
    UINT8  USB3AdapterNumber:6;   // This Field Contains the Adapter number of the
                                  // USB3 adapter that is connected to Downstream USB3 port
    UINT8  Reserved2:1;           // Shall be set to 0
    UINT8  TunnelingSupport:1;    // This Field set to 1 when USB3 port is connect to USB3 adapter Usb3_Adapter_Number;
  } Field;
} AMD_USB4_USB3_ADAPTER_NUMBER;

typedef struct{
  AMD_USB4_USB3_PORT_NUMBER      Usb3PortNumber;
  AMD_USB4_PD_PORT_NUMBER        PdPortNumber;
  AMD_USB4_USB3_ADAPTER_NUMBER   Usb3AdapterNumber;
} AMD_USB4_PORT_MAP_ENTRY;

/*
typedef struct {
  UINT8                    EntryLength;           // Size of this structure
  UINT8                    Type:6;                // 0xB for USB PortMap
  UINT8                    Reserved:1;            // 0.
  UINT8                    AdapterEntry:1;        // 0 ( Generic entry)
  AMD_USB4_PORT_MAP_ENTRY  Usb3PortMap[MAX_USB3_PORT_MAPPINGS];
} AMD_USB4_USB3_PORT_MAPPING;
*/

//
//  Individual bit fields for Generic entry  UTF16 Vendor name  structure
//
typedef struct {
  UINT8   EntryLength;                // Size of this structure entry
  UINT8   Type:6;                     // 0xC for UTF16 vendor name
  UINT8   Reserved:1;                 // 0.
  UINT8   AdapterEntry:1;             // 0 ( Generic entry)
  UINT16  wLANGID;                    // A 16-bit language ID (LANGID) defined by the USB-IF.
  UINT8   VendorName[MAX_NAME_SIZE];  // Product Vendor Name
} UTF16_VENDOR_NAME_ENTRY;

//
//  Individual bit fields for Generic entry  UTF16 Model name  structure
//
typedef struct {
  UINT8   EntryLength;               // Size of this structure entry
  UINT8   Type:6;                    // 0xD for UTF16 Model Name
  UINT8   Reserved:1;                // 0.
  UINT8   AdapterEntry:1;            // 0 ( Generic entry)
  UINT16  wLANGID;                   // A 16-bit language ID (LANGID) defined by the USB-IF.
  UINT8   ModelName[MAX_NAME_SIZE];  // Product Model Name
} UTF16_MODEL_NAME_ENTRY;

/*
//
// USB4 DROM Structure as per Example B.1 in Usb4 DROM rev 1.0
//
typedef struct{

  DROM_HEADER                         Header;
  TBT3_IDENTIFICATION_SECTION         TBT3Identification;                            // TBT3
  TBT3_HEADER_SECTION                 TBT3Header;                                    // TBT3
  TBT3_LANE_ADAPTER_ENTRY             TBT3LaneAdapter[MAX_TBT3_ADAPTERS];            // TBT3
  TBT3_PCIE_UPSTREAM_ADAPTER_ENTRY    TBT3PCIeUpStreamAdapter[MAX_TBT3_ADAPTERS];    // TBT3
  TBT3_PCIE_DOWNSTREAM_ADAPTER_ENTRY  TBT3PCIeDownStreamAdapter[MAX_TBT3_ADAPTERS];  // TBT3
  UNUSED_ADAPTER_ENTRY                UnusedAdapter[MAX_UNUSED_ADAPTERS];
  DP_ADAPTER_ENTRY                    DpAdapter[MAX_DP_ADAPTERS];
  ASCII_VENDOR_NAME_ENTRY             VendorName;
  ASCII_MODEL_NAME_ENTRY              ModelName;
  TMU_MINIMUM_REQUESTMODE_ENTRY       TMU;
  PRODUCT_DESCRIPTOR_ENTRY            ProductDescriptor;
  SERIAL_NUMBER_ENTRY                 SerialNumber;
  AMD_USB4_USB3_PORT_MAPPING          UsbPortsMapping;
} USB4_DROM;
*/

#pragma  pack (pop)

#endif
