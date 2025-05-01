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

#include <Library/PciVendorIdTableLib.h>

/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
PciVendorIdStringInit (
  VOID
) {
  return EFI_SUCCESS;
}

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
  IN     UINT16  VendorId,
  IN OUT CHAR16 **NameString,
  IN     CHAR8  *Languages  OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

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
  IN     UINT16  VendorId,
  IN OUT CHAR16 **NameString,
  IN     CHAR8  *Languages  OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}