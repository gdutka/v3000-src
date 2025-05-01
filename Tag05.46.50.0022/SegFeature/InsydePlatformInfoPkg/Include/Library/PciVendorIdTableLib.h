/** @file
  PCI Vendor Id Lookup table

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


#ifndef _PCI_VENDORID_TABLE_H_
#define _PCI_VENDORID_TABLE_H_

#include <Uefi.h>
#include <Library/HiiLib.h>


typedef struct{
  UINT16         VendorId;
  EFI_STRING_ID  ShortName;
  EFI_STRING_ID  LongName;
} PCI_VENDOR_TABLE;

#define PCI_VENDORID_TABLE_STRING_PACKAGE_GUID \
  { 0x95ba84f5, 0xad27, 0x4a09, { 0xb0, 0x93, 0x4a, 0x13, 0x65, 0x4b, 0x62, 0x42 } }

/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
PciVendorIdStringInit (
  VOID
);

/**
  With giving Vendor ID, this function will return a Vendor name of the Vendor ID

  @param[in]  VendorId            - Vendor ID for the PCI device.
  @param[out] **NameString        - Vendor name for the Vendor ID.
  @param[in]  Languages           - Support the hii languages.

  @retval EFI_SUCCESS             - Find the Vendor name.
  @retval EFI_INVALID_PARAMETER   - NameString is a NULL pointer.
  @retval EFI_NOT_FOUND           - Can not find the Vendor name.
**/
EFI_STATUS
LibGetVendorIdShortName (
  IN      UINT16  VendorId,
  IN OUT  CHAR16  **NameString,
  IN      CHAR8   *Language
);

/**
  With giving Vendor ID, this function will return a Vendor name(Long) of the Vendor ID

  @param[in]  VendorId            - Vendor ID for the PCI device.
  @param[out] **NameString        - Vendor name for the Vendor ID.
  @param[in]  Languages           - Support the hii languages.

  @retval EFI_SUCCESS             - Find the Vendor name.
  @retval EFI_INVALID_PARAMETER   - NameString is a NULL pointer.
  @retval EFI_NOT_FOUND           - Can not find the Vendor name.
**/
EFI_STATUS
LibGetVendorIdLongName (
  IN      UINT16  VendorId,
  IN OUT  CHAR16  **NameString,
  IN      CHAR8   *Language
);

#endif

