/** @file
  H2O PCI Lookup table Protocol

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


#ifndef _H2O_PCI_LOOKUP_TABLE_H_
#define _H2O_PCI_LOOKUP_TABLE_H_

//
// H2O_PCI_LOOKUP_TABLE_PROTOCOL_GUID Protocol GUID
//
#define H2O_PCI_LOOKUP_TABLE_PROTOCOL_GUID                                         \
  {                                                                                \
    0x4DDD6123, 0x8F4B, 0x4C22, { 0xA9, 0xDE, 0x39, 0x0B, 0x0C, 0xC9, 0x30, 0x86 } \
  }

typedef struct _H2O_PCI_LOOKUP_TABLE_PROTOCOL H2O_PCI_LOOKUP_TABLE_PROTOCOL;


/**
  With giving Vendor ID, this function will return a short vendor name of the Vendor ID,
  if find the Vendor Id but the Vendor name isn's exist. Call GetVerdorIdFullName to get full name.

  @param[in]      VendorId        - Vendor ID for the PCI device.
  @param[in]      DeviceId        - Device ID for the PCI device.
  @param[in, out] **VendorName    - Vendor name for the Vendor ID.
  @param[in, out] **DeviceName    - Device name for the Device ID.
  @param[in]      *Languages      - String Language.


  @retval EFI_SUCCESS             - Find the String Success.
  @retval EFI_INVALID_PARAMETER   - Invalid PCI Vendor Id.
  @retval EFI_UNSUPPORTED         - All String pointer are NULL pointer or Feature closed.
  @retval EFI_NOT_FOUND           - Vendor Id not found or Device Id valued but device name not found
**/
typedef
EFI_STATUS
(EFIAPI *PCI_ID2STR)(
  IN     UINT16    VendorId,
  IN     UINT16    DeviceId,     OPTIONAL
  IN OUT CHAR16    **VendorName, OPTIONAL
  IN OUT CHAR16    **DeviceName, OPTIONAL
  IN     CHAR8     *Languages    OPTIONAL
);

/**
  Generates printable Unicode strings that represent PCI device class,
  subclass and programmed I/F based on a value passed to the function.

  @param[in]      ClassCode     Value representing the PCI "Class Code" register read from a
                                PCI device. The encodings are:
                                bits 23:16 - Base Class Code
                                bits 15:8  - Sub-Class Code
                                bits  7:0  - Programming Interface
  @param[in, out] ClassStrings  Pointer of PCI_CLASS_STRINGS structure, which contains
                                printable class strings corresponding to ClassCode. The
                                caller must not modify the strings that are pointed by
                                the fields in ClassStrings.
  @param[in]      *Languages    String Language.
**/
typedef
VOID
(EFIAPI *GET_CLASS_STR)(
  IN      UINT32    ClassCode,
  IN OUT  CHAR16    **BaseClass, OPTIONAL
  IN OUT  CHAR16    **SubClass,  OPTIONAL
  IN OUT  CHAR16    **PifClass,   OPTIONAL
  IN      CHAR8     *Languages   OPTIONAL
);

/**
  With giving Linkspeed value, this function will return a String of the Linkspeed.

  @param[in]      LinkSpeed        - Linkspeed for the PCI device.
  @param[in, out] **LinkSpeedStr   - String for the Linkspeed.
  @param[in, out] **PcieRevision   - PCIe Revision String
  @param[in]      *Languages       - Hii Supported Language

  @retval EFI_SUCCESS              - Find the Linkspeed Str.
  @retval EFI_NOT_FOUND            - Undefined Linkspeed value.
**/
typedef
EFI_STATUS
(EFIAPI *GET_LINK_SPEED_STR)(
  IN     UINT32     LinkSpeed,
  IN     CHAR16     **LinkSpeedStr,
  IN OUT CHAR16     **PcieRevision,
  IN     CHAR8      *Languages   OPTIONAL
);

/**
  With giving header type value, this function will return a String of the header type.

  @param[in]      HeaderType        - Header for the PCI device.
  @param[in, out] **HeaderTypeStr   - String for header type.
  @param[in, out] **MultiFuncStr    - String for Multi-Function.
  @param[in]      *Languages        - Hii Supported Language.

  @retval EFI_SUCCESS               - Find the header type str.
  @retval EFI_INVALID_PARAMETER     - HeaderTypeInfo is a NULL pointer.
**/
typedef
EFI_STATUS
(EFIAPI *GET_HEADER_TYPE_STR)(
  IN     UINT8    HeaderType,
  IN OUT CHAR16   **HeaderTypeStr,
  IN OUT CHAR16   **MultiFuncStr,
  IN     CHAR8    *Languages   OPTIONAL
);

/**
  With giving Capability ID, this function will return a String of the Capability name.

  @param[in]  CapabilityId        - Capability ID for the PCI device.
  @param[out] **CapabilityStr     - String for Capability.
  @param[in]  *Languages          - Hii Supported Language.

  @retval EFI_SUCCESS             - Find the Capability str.
  @retval EFI_INVALID_PARAMETER   - CapabilityStr is a NULL pointer.
**/
typedef
EFI_STATUS
(EFIAPI *GET_CAP_STR)(
  IN     UINT8    CapabilityId,
  IN OUT CHAR16   **CapabilityStr,
  IN     CHAR8    *Languages   OPTIONAL
);

/**
  With giving Capability ID, this function will return a String of the Capability name.

  @param[in]  ExtCapabilityId     - Capability ID for the PCI device.
  @param[out] **CapabilityStr     - String for Capability.
  @param[in]  *Languages          - Hii Supported Language.

  @retval EFI_SUCCESS             - Find the Capability str.
  @retval EFI_INVALID_PARAMETER   - CapabilityStr is a NULL pointer.
**/
typedef
EFI_STATUS
(EFIAPI *GET_EXT_CAP_STR)(
  IN     UINT16   ExtCapabilityId,
  IN OUT CHAR16   **CapabilityStr,
  IN     CHAR8    *Languages   OPTIONAL
);



typedef struct _H2O_PCI_LOOKUP_TABLE_PROTOCOL {
  PCI_ID2STR                  PCIeId2StrFull;  //for full vendor name
  PCI_ID2STR                  PCIeId2Str;      //for short vendor name
  GET_CLASS_STR               PciGetClassStrings;
  GET_LINK_SPEED_STR          GetVendorLinkSpeedStr;
  GET_HEADER_TYPE_STR         GetHeaderTypeStr;
  GET_CAP_STR                 GetCapabilityStr;
  GET_EXT_CAP_STR             GetExtCapabilityStr;
} H2O_PCI_LOOKUP_TABLE_PROTOCOL;

extern EFI_GUID gH2OPciLookupTableProtocolGuid;

#endif

