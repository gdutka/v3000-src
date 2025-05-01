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

#include <Library/PciDeviceIdTableLib.h>

/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
PciDeviceIdStringInit (
  VOID
) {
  return EFI_SUCCESS;
}

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
  IN     UINT16  VendorId,
  IN     UINT16  DeviceId,
  IN OUT CHAR16 **NameString,
  IN     CHAR8  *Languages  OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}