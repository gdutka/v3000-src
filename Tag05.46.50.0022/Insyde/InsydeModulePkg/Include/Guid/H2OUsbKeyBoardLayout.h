/** @file
 Guid used to trigger event to do USB Keyboard connection.

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

#ifndef __H2O_USB_KEYBOARD_LAYOUT_GUID_H__
#define __H2O_USB_KEYBOARD_LAYOUT_GUID_H__

//
// GUID for USB keyboard HII package list.
//
#define H2O_USB_KEYBOARD_LAYOUT_PACKAGE_GUID \
  { \
    0xdbb355c8, 0xf931, 0x11e4, { 0x9e, 0x84, 0x1f, 0x05, 0x1d, 0x5d, 0x46, 0xb0 } \
  }

//
// GUID for USB US keyboard layout
//
#define H2O_USB_KEYBOARD_US_LAYOUT_KEY_GUID \
  { \
    0x1f00fe5c, 0xf932, 0x11e4, { 0x97, 0xe7, 0x44, 0x05, 0x1d, 0x5d, 0x46, 0xb0 } \
  }

//
// GUID for USB UK keyboard layout
//
#define H2O_USB_KEYBOARD_UK_LAYOUT_KEY_GUID \
  { \
    0xb015127f, 0x5301, 0x43e7, { 0xbf, 0xa1, 0x54, 0x7e, 0xc5, 0xde, 0xa0, 0xf3 } \
  }

//
// GUID for USB French keyboard layout
//
#define H2O_USB_KEYBOARD_FRENCH_LAYOUT_KEY_GUID \
  { \
    0x43499238, 0xf932, 0x11e4, { 0x88, 0xfb, 0x61, 0x05, 0x1d, 0x5d, 0x46, 0xb0 } \
  }

//
// GUID for USB Japanese keyboard layout
//
#define H2O_USB_KEYBOARD_JAPANESE_LAYOUT_KEY_GUID \
  { \
    0xadb43429, 0xec3e, 0x42d3, {0xb3, 0xbc, 0x12, 0x35, 0xb1, 0x93, 0x3e, 0x75} \
  }

//
// GUID for USB German keyboard layout
//
#define H2O_USB_KEYBOARD_GERMAN_LAYOUT_KEY_GUID \
  { \
    0x240f190b, 0x9515, 0x414a, { 0xb2, 0x4a, 0x0d, 0x2a, 0xca, 0x03, 0x5b, 0x70 } \
  }

extern EFI_GUID  gH2OUsbKeyboardLayoutPackageGuid;
extern EFI_GUID  gH2OUsbKeyboardUSLayoutKeyGuid;
extern EFI_GUID  gH2OUsbKeyboardUKLayoutKeyGuid;
extern EFI_GUID  gH2OUsbKeyboardFrenchLayoutKeyGuid;
extern EFI_GUID  gH2OUsbKeyboardJapaneseLayoutKeyGuid;
extern EFI_GUID  gH2OUsbKeyboardGermanLayoutKeyGuid;

#endif
