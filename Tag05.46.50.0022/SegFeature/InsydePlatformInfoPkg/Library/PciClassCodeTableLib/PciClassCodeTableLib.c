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

#include <Library/PciClassCodeTableLib.h>

EFI_GUID                 mPciClassCodeTableStringPackGuid = PCI_CLASSCODE_TABLE_STRING_PACKAGE_GUID;
EFI_HII_HANDLE           mClassCodeStringPackHandle       = NULL;

extern PCI_CLASS_ENTRY   gClassStringList[];
extern UINT8             PciClassCodeTableLibStrings[];

/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
PciClassCodeStringInit (
  VOID
) {
  mClassCodeStringPackHandle = HiiAddPackages (
                                 &mPciClassCodeTableStringPackGuid,
                                 NULL,
                                 PciClassCodeTableLibStrings,
                                 NULL
                                 );
  if (mClassCodeStringPackHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

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
  UINTN           Index;
  UINT8           Code;
  PCI_CLASS_ENTRY *CurrentClass;

  //
  // Assume no strings found
  //
  if (BaseClass != NULL) {
    *BaseClass = NULL;
  }
  if (SubClass != NULL) {
    *SubClass  = NULL;
  }
  if (PifClass != NULL) {
    *PifClass  = NULL;
  }

  CurrentClass = gClassStringList;
  Code = (UINT8)(ClassCode >> 16);
  Index = 0;

  //
  // Go through all entries of the base class, until the entry with a matching
  // base class code is found. If reaches an entry with a null description
  // text, the last entry is met, which means no text for the base class was
  // found, so no more action is needed.
  //
  while (Code != CurrentClass[Index].Code) {
    if (CurrentClass[Index].DescToken == STRING_TOKEN (STR_NULL_STRING)) {
      return;
    }
    Index++;
  }

  //
  // A base class was found. Assign description, and check if this class has
  // sub-class defined. If sub-class defined, no more action is needed,
  // otherwise, continue to find description for the sub-class code.
  //
  if ((BaseClass != NULL) && (CurrentClass[Index].DescToken != STRING_TOKEN (STR_NULL_STRING))) {
    *BaseClass = HiiGetString (mClassCodeStringPackHandle, CurrentClass[Index].DescToken, Languages);
  }
  if (CurrentClass[Index].LowerLevelClass == NULL) {
    return;
  }

  //
  // find Subclass entry
  //
  CurrentClass  = CurrentClass[Index].LowerLevelClass;
  Code          = (UINT8)(ClassCode >> 8);
  Index         = 0;

  //
  // Go through all entries of the sub-class, until the entry with a matching
  // sub-class code is found. If reaches an entry with a null description
  // text, the last entry is met, which means no text for the sub-class was
  // found, so no more action is needed.
  //
  while (Code != CurrentClass[Index].Code) {
    if (CurrentClass[Index].DescToken == STRING_TOKEN (STR_NULL_STRING)) {
      return;
    }
    Index++;
  }
  //
  // A class was found for the sub-class code. Assign description, and check if
  // this sub-class has programming interface defined. If no, no more action is
  // needed, otherwise, continue to find description for the programming
  // interface.
  //
  if ((SubClass != NULL) && (CurrentClass[Index].DescToken != STRING_TOKEN (STR_NULL_STRING))) {
    *SubClass = HiiGetString (mClassCodeStringPackHandle, CurrentClass[Index].DescToken, Languages);
  }
  if (CurrentClass[Index].LowerLevelClass == NULL) {
    return;
  }
  //
  // Find programming interface entry
  //
  CurrentClass  = CurrentClass[Index].LowerLevelClass;
  Code          = (UINT8)ClassCode;
  Index         = 0;

  //
  // Go through all entries of the I/F entries, until the entry with a
  // matching I/F code is found. If reaches an entry with a null description
  // text, the last entry is met, which means no text was found, so no more
  // action is needed.
  //
  while (Code != CurrentClass[Index].Code) {
    if (CurrentClass[Index].DescToken == STRING_TOKEN (STR_NULL_STRING)) {
      return;
    }
    Index++;
  }
  //
  // A class was found for the I/F code. Assign description, done!
  //
  if ((PifClass != NULL) && (CurrentClass[Index].DescToken != STRING_TOKEN (STR_NULL_STRING))) {
    *PifClass = HiiGetString (mClassCodeStringPackHandle, CurrentClass[Index].DescToken, Languages);
  }

  return;
}