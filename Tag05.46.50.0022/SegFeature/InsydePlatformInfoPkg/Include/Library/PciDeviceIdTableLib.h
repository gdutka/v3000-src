/** @file
  PCI Device Id Lookup table

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
#ifndef _PCI_DEVICEID_TABLE_H_
#define _PCI_DEVICEID_TABLE_H_

#include <Uefi.h>
#include <Library/HiiLib.h>

typedef struct {
  UINT16          DeviceId;
  EFI_STRING_ID   DeviceName;
} PCI_DEVICE_STR;

typedef struct {
  UINT16             VendorId;
  UINT16             ListSize;
  PCI_DEVICE_STR     *DeviceList;
} PCI_VENDOR_DEVICE_TABLE;

#define PCI_DEVICEID_TABLE_STRING_PACKAGE_GUID \
  { 0x50b73fc8, 0xbff4, 0x4a89, { 0x9d, 0x24, 0xe9, 0x56, 0xb3, 0x6e, 0xa0, 0x0b } }

/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
PciDeviceIdStringInit (
  VOID
);

/**
  With giving Vendor ID and Device ID, this function will return a Device name of the Device ID

  @param[in]      VendorId      - Vendor ID for the PCI device.
  @param[in]      DeviceId      - Device ID for the PCI device.
  @param[in, out] **NameString  - Device name for the Device ID.
  @param[in]      Languages     - Support the hii languages.

  @retval EFI_SUCCESS           - Find the Device name.
  @retval EFI_INVALID_PARAMETER - NameString is a NULL pointer.
  @retval EFI_NOT_FOUND         - Can not find the Device name.
**/
EFI_STATUS
LibGetDeviceIdName (
  IN      UINT16  VendorId,
  IN      UINT16  DeviceId,
  IN OUT  CHAR16  **NameString,
  IN      CHAR8   *Language
);

#endif

