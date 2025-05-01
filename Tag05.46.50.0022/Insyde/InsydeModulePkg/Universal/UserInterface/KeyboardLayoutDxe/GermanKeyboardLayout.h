/** @file
 Header file of German Keyboard layout.

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

#ifndef _GERMAN_KEYBOARD_LAYOUT_H_
#define _GERMAN_KEYBOARD_LAYOUT_H_

#include "KeyboardLayoutDxe.h"

#define H2O_KEYBOARD_LAYOUT_GERMAN_KEY_COUNT             124
#define H2O_KEYBOARD_LAYOUT_GERMAN_LANGUAGE_STR_LEN      (sizeof("de-DE") - 1)
#define H2O_KEYBOARD_LAYOUT_GERMAN_DESCRIPTION_STR       L"German Keyboard"
#define H2O_KEYBOARD_LAYOUT_GERMAN_DESCRIPTION_STR_LEN   (sizeof(H2O_KEYBOARD_LAYOUT_GERMAN_DESCRIPTION_STR) / sizeof(CHAR16))

#pragma pack (1)
typedef struct {
  //
  // This 4-bytes total array length is required by PreparePackageList()
  //
  UINT32                 Length;

  //
  // Keyboard Layout package definition
  //
  EFI_HII_PACKAGE_HEADER PackageHeader;
  UINT16                 LayoutCount;

  //
  // EFI_HII_KEYBOARD_LAYOUT
  //
  UINT16                 LayoutLength;
  EFI_GUID               Guid;
  UINT32                 LayoutDescriptorStringOffset;
  UINT8                  DescriptorCount;
  EFI_KEY_DESCRIPTOR     KeyDescriptor[H2O_KEYBOARD_LAYOUT_GERMAN_KEY_COUNT];
  UINT16                 DescriptionCount;
  CHAR16                 Language[H2O_KEYBOARD_LAYOUT_GERMAN_LANGUAGE_STR_LEN];
  CHAR16                 Space;
  CHAR16                 DescriptionString[H2O_KEYBOARD_LAYOUT_GERMAN_DESCRIPTION_STR_LEN];
} H2O_KEYBOARD_LAYOUT_GERMAN_PACK_BIN;
#pragma pack()

extern H2O_KEYBOARD_LAYOUT_GERMAN_PACK_BIN  mH2OKeyboardLayoutGermanBin;
#endif
