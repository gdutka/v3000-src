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


#ifndef _PCI_CLASSCODE_TABLE_H_
#define _PCI_CLASSCODE_TABLE_H_

#include <Uefi.h>
#include <Library/HiiLib.h>

//
// ConstPrintable strings for Pci class code
//
typedef struct {
  CONST CHAR16  *BaseClass; // Pointer to the PCI base class string
  CONST CHAR16  *SubClass;  // Pointer to the PCI sub class string
  CONST CHAR16  *PifClass;  // Pointer to the PCI programming interface string
} PCI_CLASS_STRINGS;

#define PCI_CLASSCODE_TABLE_STRING_PACKAGE_GUID \
  { 0x6bf47e2b, 0xd8f1, 0x49a3, { 0xad, 0x9a, 0x47, 0x96, 0xd3, 0x1b, 0x2c, 0x17 } }

/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
PciClassCodeStringInit (
  VOID
);

//
// a structure holding a single entry, which also points to its lower level
// class
//
typedef struct PCI_CLASS_ENTRY_TAG {
  UINT8                       Code;             // Class, subclass or I/F code
  EFI_STRING_ID               DescToken;         // Description Token
  struct PCI_CLASS_ENTRY_TAG  *LowerLevelClass; // Subclass or I/F if any
} PCI_CLASS_ENTRY;

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
);


#endif

