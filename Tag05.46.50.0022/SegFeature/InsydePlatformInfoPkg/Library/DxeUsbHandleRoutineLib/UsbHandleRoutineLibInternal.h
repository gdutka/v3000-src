/** @file
  DXE USB handle auxiliary routines.

;******************************************************************************
;* Copyright 2022 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef USB_HANDLE_ROUTINE_LIB_INTERNAL_H_
#define USB_HANDLE_ROUTINE_LIB_INTERNAL_H_

#include <Uefi.h>

#define USB_DESC_TYPE_DEVICE_QUALIFIER  6

#define USB_DESC_MAX_LENGTH             MAX_UINT8

//
// Definitions copied from "MdeModulePkg/Bus/Usb/UsbBusDxe/UsbHub.h"
//
#define USB_HUB_CLASS_CODE              0x09
#define USB_HUB_SUBCLASS_CODE           0x00
#define USB_DESC_TYPE_HUB               0x29
#define USB_DESC_TYPE_HUB_SUPER_SPEED   0x2A

//
// Common header of USB descriptors.
// Definition copied from "MdeModulePkg/Bus/Usb/UsbBusDxe/UsbDesc.h"
// (Too bad that "MdePkg/Include/IndustryStandard/Usb.h" didn't define this.)
//
#pragma pack (1)
typedef struct {
  UINT8  Len;
  UINT8  Type;
} USB_DESC_HEAD;
#pragma pack ()

#endif // USB_HANDLE_ROUTINE_LIB_INTERNAL_H_
