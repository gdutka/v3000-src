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

#include <Library/PciCapabilityIdTableLib.h>

EFI_GUID                   mPciCapabilityIdTableStringPackGuid = PCI_CAPABILITYID_TABLE_STRING_PACKAGE_GUID;
EFI_HII_HANDLE             mCapabilityIdStringPackHandle = NULL;

extern PCI_CAP_TABLE       mPciCapbilityTable[];
extern PCIE_EXT_CAP_TABLE  mPcieExtendedCapbilityTable[];
extern UINT8               PciCapabilityIdTableLibStrings[];


/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
PciCapabilityIdStringInit (
  VOID
) {
  mCapabilityIdStringPackHandle = HiiAddPackages (
                                    &mPciCapabilityIdTableStringPackGuid,
                                    NULL,
                                    PciCapabilityIdTableLibStrings,
                                    NULL
                                    );
  if (mCapabilityIdStringPackHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

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
  EFI_STATUS  Status;
  UINT16      Index = 0;

  if (NameString == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *NameString = NULL;

  //
  // Search Capability ID for Capability name
  //
  while (mPciCapbilityTable[Index].CapabilityId != 0xFF) {
    if (mPciCapbilityTable[Index].CapabilityId == CapabiltyId) {
      *NameString = HiiGetString (mCapabilityIdStringPackHandle, mPciCapbilityTable[Index].CapabilityStr, Languages);
    }
    Index++;
  }

  Status = (*NameString != NULL) ? EFI_SUCCESS : EFI_NOT_FOUND;

  return Status;
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
  IN OUT CHAR16 **NameString,
  IN     CHAR8  *Languages  OPTIONAL
  )
{
  EFI_STATUS  Status;
  UINT16      Index = 0;

  if (NameString == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *NameString = NULL;

  //
  // Search Extend Capability ID for Capability name
  //
  while (mPcieExtendedCapbilityTable[Index].ExtCapabilityId != 0xFFFF) {
    if (mPcieExtendedCapbilityTable[Index].ExtCapabilityId == ExtCapabiltyId) {
      *NameString = HiiGetString (mCapabilityIdStringPackHandle, mPcieExtendedCapbilityTable[Index].ExtCapabilityStr, Languages);
    }
    Index++;
  }

  Status = (*NameString != NULL) ? EFI_SUCCESS : EFI_NOT_FOUND;

  return Status;
}