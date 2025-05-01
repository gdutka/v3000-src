/** @file
  PCI Class Code Lookup table

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
PciClassCodeStringInit (
  VOID
) {
  return EFI_SUCCESS;
}


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
VOID
LibPciGetClassStrings (
  IN      UINT32    ClassCode,
  IN OUT  CHAR16    **BaseClass, OPTIONAL
  IN OUT  CHAR16    **SubClass,  OPTIONAL
  IN OUT  CHAR16    **PifClass,  OPTIONAL
  IN      CHAR8     *Languages   OPTIONAL
  )
{
  //
  // Assume no strings found
  //
  *BaseClass = NULL;
  *SubClass  = NULL;
  *PifClass  = NULL;

  return;
}