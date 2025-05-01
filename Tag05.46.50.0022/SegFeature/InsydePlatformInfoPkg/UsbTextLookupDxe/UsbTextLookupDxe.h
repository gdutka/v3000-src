/** @file


;******************************************************************************
;* Copyright 2021 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef USB_TEXT_LOOKUP_DXE_H_
#define USB_TEXT_LOOKUP_DXE_H_

#include <Uefi.h>

#include <Protocol/H2OUsbTextLookup.h>

#include <Library/DebugLib.h>

#include "H2OUsbClassCodeString.h"

//
// Defined in AutoGen.c
//
extern UINT8 UsbTextLookupDxeStrings[];

#define H2O_USB_TEXT_LOOKUP_PRIVATE_SIGNATURE  SIGNATURE_32 ('U', 'T', 'X', 'L')

typedef struct {
  UINT32                                      Signature;
  H2O_USB_TEXT_LOOKUP_PROTOCOL                Protocol;
  CONST H2O_USB_CLASS_CODE_DESCRIPTION_ENTRY  *UsbClassDescTable;
  CONST H2O_USB_SUBCLASS_RANGE_TABLE          *UsbSubClassRangeTable;
  EFI_HII_HANDLE                              HiiHandle;
} H2O_USB_TEXT_LOOKUP_PRIVATE_DATA;

#define H2O_USB_TEXT_LOOKUP_PRIVATE_FROM_PROTOCOL(a) \
    CR(a, H2O_USB_TEXT_LOOKUP_PRIVATE_DATA, Protocol, H2O_USB_TEXT_LOOKUP_PRIVATE_SIGNATURE)

#endif // USB_TEXT_LOOKUP_DXE_H_
