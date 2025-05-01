
#ifndef _ACPI_SDEV_TABLE_H_
#define _ACPI_SDEV_TABLE_H_

#define SECURE_BIO_CAM_SSDT_GUID \
  { 0xa2b0063d, 0x9326, 0x46f0, {0x83, 0x65, 0x25, 0x48, 0x48, 0x96, 0xf5, 0x1b} }

#pragma pack (1)

typedef struct {
  UINT8    PciDevice;
  UINT8    PciFunction;
} ACPI_SDEV_PCIE_EP_OBJECT;

typedef struct {
  UINT8    Type;                     // 
  UINT16   Length;                   // offset 1
  UINT8    Revision;                 // offset 3
  UINT8    Attributes;               // offset 4
  UINT8    RootPortNumber;           // offset 5
  UINT16   VID;                      // offset 6
  UINT16   PID;                      // offset 8
  UINT16   UsbRevision;              // offset 10
  UINT8    InterfaceNumber;          // offset 12
  UINT8    Class;                    // offset 13
  UINT8    Subclass;                 // offset 14
  UINT8    Protocol;                 // offset 15
  UINT16   AcpiPathStringOffset;     // offset 16
  UINT16   AcpiPathStringLength;     // offset 18
  UINT8    FwHash[32];               // offset 20
//CHAR8    *AcpiPathName             // offset 52   
} ACPI_SDEV_XHCI_SECURE_DEVICE_DESCRIPTOR;

typedef struct {
  UINT8    Type;
  UINT16   Length;
  UINT8    ControllerCapability[2];
  UINT8    AlternateFunctionNumber;
  UINT8    Reserved[4];  
} ACPI_SDEV_XHCI_VENDOR_SPECIFIC_INFO;
#pragma pack ()

#endif //_ACPI_SDEV_TABLE_H_