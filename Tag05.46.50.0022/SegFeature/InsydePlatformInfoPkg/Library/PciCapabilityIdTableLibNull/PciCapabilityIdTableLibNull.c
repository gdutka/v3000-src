/** @file
  PCI Capability Lookup table

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/PciCapabilityIdTableLib.h>

/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
PciCapabilityIdStringInit (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  With giving Capabilty ID , this function will return a String of capability name

  @param[in]  CapabiltyId        - PCI Capabilty Id.
  @param[out] **NameString       - Capability Id Name String .
  @param[in]  Languages          - Support the hii languages.

  @retval EFI_SUCCESS            - Capabilty name found.
  @retval EFI_INVALID_PARAMETER  - NameString is a NULL pointer.
  @retval EFI_NOT_FOUND          - Capability name not found.
**/
EFI_STATUS
LibGetCapabilityStr (
  IN     UINT8  CapabiltyId,
  IN OUT CHAR16 **NameString,
  IN     CHAR8  *Languages  OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**
  With giving Extend Capabilty ID , this function will return a String of capability name

  @param[in]  ExtCapabiltyId        - PCIe Extend Capabilty Id.
  @param[out] **NameString          - Extend Capability Id Name String .
  @param[in]  Languages             - Support the hii languages.

  @retval EFI_SUCCESS               - Extend Capabilty name found.
  @retval EFI_INVALID_PARAMETER     - NameString is a NULL pointer.
  @retval EFI_NOT_FOUND             - Extend Capability name not found.
**/
EFI_STATUS
LibGetExtCapabilityStr (
  IN     UINT16  ExtCapabiltyId,
  IN OUT CHAR16  **NameString,
  IN     CHAR8   *Languages  OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}