/** @file
  PCI Lookup table Dxe

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


#ifndef _PCI_LOOKUP_H_
#define _PCI_LOOKUP_H_

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>

#include <Protocol/H2OPciLookupTable.h>

#define HEADER_TYPE_UNDEFINE 3

//
// Const Printable String for Linkspeed
//
typedef struct {
  UINT32         LinkSpeed;
  EFI_STRING_ID  LinkSpeedStr;
  EFI_STRING_ID  PcieRevision;
} PCIE_LINKSPEED_INFO;

//
// Prototype of String init
//
EFI_STATUS
typedef
(EFIAPI *PCI_STRING_INIT_FUNCTION) (
  VOID
);

EFI_STATUS
EFIAPI
PciCommonStringInit (
  VOID
);

/**
  With giving Vendor ID, this function will return a short vendor name of the Vendor ID,
  if find the Vendor Id but the Vendor name isn's exist. Call GetVerdorIdFullName to get full name.

  @param[in]      VendorId      - Vendor ID for the PCI device.
  @param[in]      DeviceId      - Device ID for the PCI device.
  @param[in, out] **VendorName  - Vendor name for the Vendor ID.
  @param[in, out] **DeviceName  - Device name for the Device ID.
  @param[in]      *Languages    - String Language.

  @retval EFI_SUCCESS             - Find the String Success.
  @retval EFI_INVALID_PARAMETER   - Invalid PCI Vendor Id.
  @retval EFI_UNSUPPORTED         - All String pointer are NULL pointer or Feature closed.
  @retval EFI_NOT_FOUND           - Vendor Id not found or Device Id valued but device name not found
**/
EFI_STATUS
PCIeId2Str (
  IN     UINT16    VendorId,
  IN     UINT16    DeviceId,    OPTIONAL
  IN OUT CHAR16    **VendorName,
  IN OUT CHAR16    **DeviceName,  OPTIONAL
  IN      CHAR8    *Languages   OPTIONAL
);

/**
  With giving Vendor ID, this function will return a short vendor name of the Vendor ID,
  if find the Vendor Id but the Vendor name isn's exist. Call GetVerdorIdFullName to get full name.

  @param[in]      VendorId      - Vendor ID for the PCI device.
  @param[in]      DeviceId      - Device ID for the PCI device.
  @param[in, out] **VendorName  - Vendor name for the Vendor ID.
  @param[in, out] **DeviceName  - Device name for the Device ID.
  @param[in]      *Languages    - String Language.

  @retval EFI_SUCCESS             - Find the String Success.
  @retval EFI_INVALID_PARAMETER   - Invalid PCI Vendor Id.
  @retval EFI_UNSUPPORTED         - All String pointer are NULL pointer or Feature closed.
  @retval EFI_NOT_FOUND           - Vendor Id not found or Device Id valued but device name not found
**/
EFI_STATUS
PCIeId2StrFull (
  IN     UINT16    VendorId,
  IN     UINT16    DeviceId,    OPTIONAL
  IN OUT CHAR16    **VendorName,
  IN OUT CHAR16    **DeviceName,  OPTIONAL
  IN      CHAR8    *Languages   OPTIONAL
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
EFI_STATUS
GetLinkSpeedStr (
  IN     UINT32     LinkSpeed,
  IN     CHAR16     **LinkSpeedStr,
  IN OUT CHAR16     **PcieRevision,
  IN      CHAR8     *Languages   OPTIONAL
);


/**
  With giving header type value, this function will return a String of the header type.

  @param[in]      HeaderType        - Header for the PCI device.
  @param[in, out] **HeaderTypeStr   - String for header type.
  @param[in, out] **MultiFuncStr    - String for Multi-Function.
  @param[in]      *Languages        - Hii Supported Language.

  @retval EFI_SUCCESS             - Find the header type str.
  @retval EFI_INVALID_PARAMETER   - HeaderTypeInfo is a NULL pointer.
**/
EFI_STATUS
GetHeaderTypeStr (
  IN     UINT8    HeaderType,
  IN OUT CHAR16   **HeaderTypeStr,
  IN OUT CHAR16   **MultiFuncStr,
  IN     CHAR8    *Languages   OPTIONAL
);

#endif

