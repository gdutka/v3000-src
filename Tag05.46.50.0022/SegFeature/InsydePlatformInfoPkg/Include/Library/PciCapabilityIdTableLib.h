/** @file
  PCI Capability Lookup table

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


#ifndef _PCI_CAPABILITYID_TABLE_H_
#define _PCI_CAPABILITYID_TABLE_H_

#include <Uefi.h>
#include <Library/HiiLib.h>

typedef struct{
  UINT8          CapabilityId;
  EFI_STRING_ID  CapabilityStr;
} PCI_CAP_TABLE;

typedef struct{
  UINT16         ExtCapabilityId;
  EFI_STRING_ID  ExtCapabilityStr;
} PCIE_EXT_CAP_TABLE;

#define PCI_CAPABILITYID_TABLE_STRING_PACKAGE_GUID \
  { 0x7e5345ec, 0x3baa, 0x4715, { 0xa0, 0x01, 0x33, 0xa6, 0x03, 0x15, 0x4d, 0x49 } }

/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
PciCapabilityIdStringInit (
  VOID
);

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
  IN      UINT8   CapabilityId,
  IN OUT  CHAR16  **NameString,
  IN      CHAR8   *Language
);

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
  IN     UINT16  CapabiltyId,
  IN OUT CHAR16 **NameString,
  IN     CHAR8  *Languages  OPTIONAL
);

#endif

