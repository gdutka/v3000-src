/** @file


;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>              // MdeModulePkg
#include <Library/BinarySearchExLib.h>   // InsydePlatformInfoPkg

#include "H2OUsbClassCodeString.h"
#include "UsbTextLookupDxe.h"

#define H2O_USB_CC_DESC(Class, Member, StringId) \
  {\
    (Class), \
    (UINT16) (OFFSET_OF (H2O_USB_SUBCLASS_RANGE_TABLE, Member) / \
               sizeof (H2O_USB_SUBCLASS_RANGE_PRIVATE)), \
    (StringId) \
  }

CONST H2O_USB_CLASS_CODE_DESCRIPTION_ENTRY  mUsbClassDescTable[] = {
  //
  // Must be sorted by (base) class code.
  //
  H2O_USB_CC_DESC (0x00, Class00h, STRING_TOKEN (STR_NOT_APPLICABLE)),
  H2O_USB_CC_DESC (0x01, Class01h, STRING_TOKEN (STR_USB_CLASS_01H_AUDIO)),
  H2O_USB_CC_DESC (0x02, Class02h, STRING_TOKEN (STR_USB_CLASS_02H_COMMUNICATIONS)),
  H2O_USB_CC_DESC (0x03, Class03h, STRING_TOKEN (STR_USB_CLASS_03H_HID)),
  H2O_USB_CC_DESC (0x06, Class06h, STRING_TOKEN (STR_USB_CLASS_06H_IMAGE)),
  H2O_USB_CC_DESC (0x07, Class07h, STRING_TOKEN (STR_USB_CLASS_07H_PRINTER)),
  H2O_USB_CC_DESC (0x08, Class08h, STRING_TOKEN (STR_USB_CLASS_08H_MASS_STORAGE)),
  H2O_USB_CC_DESC (0x09, Class09h, STRING_TOKEN (STR_USB_CLASS_09H_HUB)),
  H2O_USB_CC_DESC (0x0A, Class0Ah, STRING_TOKEN (STR_USB_CLASS_0AH_CDC_DATA)),
  H2O_USB_CC_DESC (0x0B, Class0Bh, STRING_TOKEN (STR_USB_CLASS_0BH_SMART_CARD)),
  H2O_USB_CC_DESC (0x0D, IntScNa,  STRING_TOKEN (STR_USB_CLASS_0DH_CONTENT_SECURITY)),
  H2O_USB_CC_DESC (0x0E, Class0Eh, STRING_TOKEN (STR_USB_CLASS_0EH_VIDEO)),
  H2O_USB_CC_DESC (0x0F, ScNa,     STRING_TOKEN (STR_USB_CLASS_0FH_PHDC)),
  H2O_USB_CC_DESC (0x10, Class10h, STRING_TOKEN (STR_USB_CLASS_10H_AUDIO_VIDEO)),
  H2O_USB_CC_DESC (0x11, ScNa,     STRING_TOKEN (STR_USB_CLASS_11H_BILLBOARD)),
  H2O_USB_CC_DESC (0x12, IntScNa,  STRING_TOKEN (STR_USB_CLASS_12H_TYPE_C_BRIDGE)),
  H2O_USB_CC_DESC (0x3C, IntScNa,  STRING_TOKEN (STR_USB_CLASS_3CH_USB_I3C_DEVICE)),
  H2O_USB_CC_DESC (0xDC, ClassDCh, STRING_TOKEN (STR_USB_CLASS_DCH_DIAGNOSTIC)),
  H2O_USB_CC_DESC (0xE0, ClassE0h, STRING_TOKEN (STR_USB_CLASS_E0H_WIRELESS_CONTROLLER)),
  H2O_USB_CC_DESC (0xEF, ClassEFh, STRING_TOKEN (STR_USB_CLASS_EFH_MISCELLANEOUS)),
  H2O_USB_CC_DESC (0xFE, ClassFEh, STRING_TOKEN (STR_USB_CLASS_FEH_APPLICATION)),
  H2O_USB_CC_DESC (0xFF, NoScDef,  STRING_TOKEN (STR_VENDOR_SPECIFIC))
};

#undef H2O_USB_CC_DESC

CONST H2O_USB_SUBCLASS_RANGE_TABLE  mUsbSubClassRangeTable = {
  //
  // {{{Type,End,  D,I,F}, SubClass, Protocol, CodeCount}, StringId}
  //
  { // (No subclass definition / all vendor specific)
    {{{0,1,  0,0,0}, 0x00,    0,   0}, STRING_TOKEN (0x0000)}
  },
  { // (Subclass not applicable)
    {{{0,0,  1,1,0}, 0x00,    0,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{2,1,  1,1,0}, 0x00, 0x00,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)}
  },
  { // (Interface subclass not applicable)
    {{{0,0,  0,1,0}, 0x00,    0,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{2,1,  0,1,0}, 0x00, 0x00,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)}
  },
  { // Class 00h
    {{{0,1,  1,0,0}, 0x00,    0,   1}, STRING_TOKEN (STR_USB_DEV_SUBCLASS_00_00H)}
  },
  { // STR_USB_CLASS_01H_AUDIO
    {{{0,0,  0,1,1}, 0x00,    0,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{0,0,  0,0,1}, 0x01,    0,   1}, STRING_TOKEN (STR_USB_FUNC_SUBCLASS_01_01H)},
    {{{0,0,  0,0,1}, 0x20,    0,   7}, STRING_TOKEN (STR_USB_FUNC_SUBCLASS_01_20H_TO_26H)},
    {{{0,0,  0,1,0}, 0x01,    0,   3}, STRING_TOKEN (STR_USB_INT_SUBCLASS_01_01H_TO_03H)},
    {{{3,0,  0,1,1},    0, 0x00,   1}, STRING_TOKEN (STR_USB_PROTOCOL_01_00H)},
    {{{3,0,  0,1,1},    0, 0x20,   1}, STRING_TOKEN (STR_USB_PROTOCOL_01_20H)},
    {{{3,1,  0,1,1},    0, 0x30,   1}, STRING_TOKEN (STR_USB_PROTOCOL_01_30H)}
  },
  { // STR_USB_CLASS_02H_COMMUNICATIONS
    {{{0,0,  1,0,0}, 0x00,    0,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{2,0,  1,0,0}, 0x00, 0x00,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{0,0,  0,1,0}, 0x01,    0,  14}, STRING_TOKEN (STR_USB_INT_SUBCLASS_02_01H_TO_0EH)},
    {{{2,0,  0,1,0}, 0x0C, 0x07,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_02_0C_07H)},
    {{{0,0,  0,1,0}, 0x80,    0, 127}, STRING_TOKEN (STR_VENDOR_SPECIFIC_RANGE)},
    {{{3,0,  0,1,0},    0, 0x00,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{3,0,  0,1,0},    0, 0x01,   6}, STRING_TOKEN (STR_USB_INT_PROTOCOL_02_01H_TO_06H)},
    {{{3,0,  0,1,0},    0, 0xFE,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_02_FEH)},
    {{{3,1,  0,1,0},    0, 0xFF,   1}, STRING_TOKEN (STR_VENDOR_SPECIFIC)}
  },
  { // STR_USB_CLASS_03H_HID
    {{{0,0,  0,1,0}, 0x00,    0,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{0,0,  0,1,0}, 0x01,    0,   1}, STRING_TOKEN (STR_USB_INT_SUBCLASS_03_01H)},
    {{{2,0,  0,1,0}, 0x01, 0x01,   2}, STRING_TOKEN (STR_USB_INT_PROTOCOL_03_01_01H_TO_02H)},
    {{{3,1,  0,1,0},    0, 0x00,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)}
  },
  { // STR_USB_CLASS_06H_IMAGE
    {{{0,0,  0,1,0}, 0x01,    0,   1}, STRING_TOKEN (STR_USB_INT_SUBCLASS_06_01H)},
    {{{2,1,  0,1,0}, 0x01, 0x01,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_06_01_01H)}
  },
  { // STR_USB_CLASS_07H_PRINTER
    {{{0,0,  0,1,0}, 0x01,    0,   1}, STRING_TOKEN (STR_USB_INT_SUBCLASS_07_01H)},
    {{{2,0,  0,1,0}, 0x01, 0x01,   4}, STRING_TOKEN (STR_USB_INT_PROTOCOL_07_01_01H_TO_04H)},
    {{{2,1,  0,1,0}, 0x01, 0xFF,   1}, STRING_TOKEN (STR_VENDOR_SPECIFIC)}
  },
  { // STR_USB_CLASS_08H_MASS_STORAGE
    {{{0,0,  0,1,0}, 0x01,    0,   8}, STRING_TOKEN (STR_USB_INT_SUBCLASS_08_01H_TO_08H)},
    {{{0,0,  0,1,0}, 0xFF,    0,   1}, STRING_TOKEN (STR_VENDOR_SPECIFIC)},
    {{{2,0,  0,1,0}, 0x04, 0x00,   2}, STRING_TOKEN (STR_USB_INT_PROTOCOL_08_04_00H_TO_01H)},
    {{{3,0,  0,1,0},    0, 0x50,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_08_50H)},
    {{{3,0,  0,1,0},    0, 0x62,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_08_62H)},
    {{{3,1,  0,1,0},    0, 0xFF,   1}, STRING_TOKEN (STR_VENDOR_SPECIFIC)}
  },
  { // STR_USB_CLASS_09H_HUB
    {{{0,0,  1,1,0}, 0x00,    0,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{2,0,  1,0,0}, 0x00, 0x00,   1}, STRING_TOKEN (STR_USB_DEV_PROTOCOL_09_00_00H)},
    {{{2,0,  0,1,0}, 0x00, 0x00,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{2,0,  1,1,0}, 0x00, 0x01,   2}, STRING_TOKEN (STR_USB_PROTOCOL_09_00_01H_TO_02H)},
    {{{2,1,  1,0,0}, 0x00, 0x03,   1}, STRING_TOKEN (STR_USB_DEV_PROTOCOL_09_00_03H)}
  },
  { // STR_USB_CLASS_0AH_CDC_DATA
    {{{0,0,  0,1,0}, 0x00,    0,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{2,0,  0,1,0}, 0x00, 0x00,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{2,0,  0,1,0}, 0x00, 0x01,   2}, STRING_TOKEN (STR_USB_INT_PROTOCOL_0A_00_01_TO_02H)},
    {{{2,0,  0,1,0}, 0x00, 0x30,   3}, STRING_TOKEN (STR_USB_INT_PROTOCOL_0A_00_30_TO_32H)},
    {{{2,0,  0,1,0}, 0x00, 0x50,   3}, STRING_TOKEN (STR_USB_INT_PROTOCOL_0A_00_50_TO_52H)},
    {{{2,0,  0,1,0}, 0x00, 0x90,   4}, STRING_TOKEN (STR_USB_INT_PROTOCOL_0A_00_90_TO_93H)},
    {{{2,1,  0,1,0}, 0x00, 0xFE,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_0A_00_FEH)}
  },
  { // STR_USB_CLASS_0BH_SMART_CARD
    {{{0,0,  0,1,0}, 0x00,    0,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{2,1,  0,1,0}, 0x00, 0x00,   3}, STRING_TOKEN (STR_USB_INT_PROTOCOL_0B_00_00H_TO_02H)}
  },
  { // STR_USB_CLASS_0EH_VIDEO
    {{{0,0,  0,0,1}, 0x03,    0,   1}, STRING_TOKEN (STR_USB_FUNC_SUBCLASS_0E_03H)},
    {{{2,0,  0,0,1}, 0x00, 0x00,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{0,0,  0,1,0}, 0x01,    0,   2}, STRING_TOKEN (STR_USB_INT_SUBCLASS_0E_01H_TO_02H)},
    {{{3,1,  0,1,0},    0, 0x00,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_0E_01H)}
  },
  { // STR_USB_CLASS_10H_AUDIO_VIDEO
    {{{0,0,  0,1,1}, 0x00,    0,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{2,0,  0,0,1}, 0x00, 0x00,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{0,0,  0,1,0}, 0x01,    0,   3}, STRING_TOKEN (STR_USB_INT_SUBCLASS_10_01H_TO_03H)},
    {{{3,1,  0,1,0},    0, 0x10,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_10_10H)}
  },
  { // STR_USB_CLASS_DCH_DIAGNOSTIC
    {{{0,0,  1,0,0}, 0x01,    0,   1}, STRING_TOKEN (STR_USB_DEV_SUBCLASS_DC_01H)},
    {{{2,0,  1,0,0}, 0x01, 0x01,   1}, STRING_TOKEN (STR_USB_DEV_PROTOCOL_DC_01_01H)},
    {{{0,0,  0,1,1}, 0x02,    0,   7}, STRING_TOKEN (STR_USB_SUBCLASS_DC_02H_TO_08H)},
    {{{2,0,  0,1,1}, 0x02, 0x01,   1}, STRING_TOKEN (STR_USB_PROTOCOL_DC_02_01H_AND_05_01H)},
    {{{2,1,  0,1,1}, 0x05, 0x01,   1}, STRING_TOKEN (STR_USB_PROTOCOL_DC_02_01H_AND_05_01H)}
  },
  { // STR_USB_CLASS_E0H_WIRELESS_CONTROLLER
    {{{0,0,  1,1,0}, 0x01,    0,   1}, STRING_TOKEN (STR_USB_SUBCLASS_E0_01H)},
    {{{2,0,  1,1,0}, 0x01, 0x01,   1}, STRING_TOKEN (STR_USB_PROTOCOL_E0_01_01H)},
    {{{2,0,  0,1,0}, 0x01, 0x02,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_E0_01_02H)},
    {{{2,0,  1,1,0}, 0x01, 0x04,   1}, STRING_TOKEN (STR_USB_PROTOCOL_E0_01_04H)},
    {{{0,0,  0,1,0}, 0x02,    0,   1}, STRING_TOKEN (STR_USB_INT_SUBCLASS_E0_02H)},
    {{{2,0,  0,1,0}, 0x02, 0x01,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_E0_02_01H)},
    {{{2,0,  0,1,1}, 0x02, 0x02,   1}, STRING_TOKEN (STR_USB_PROTOCOL_E0_02_02H)},
    {{{2,1,  0,1,0}, 0x02, 0x03,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_E0_02_03H)}
  },
  { // STR_USB_CLASS_EFH_MISCELLANEOUS
    {{{0,0,  1,0,0}, 0x02,    0,   1}, STRING_TOKEN (STR_USB_DEV_SUBCLASS_EF_02H)},
    {{{2,0,  1,0,0}, 0x02, 0x01,   2}, STRING_TOKEN (STR_USB_DEV_PROTOCOL_EF_02_01H_TO_02H)},
    {{{0,0,  0,1,1}, 0x07,    0,   1}, STRING_TOKEN (STR_USB_SUBCLASS_EF_07H)},
    {{{2,0,  0,1,1}, 0x07, 0x01,   1}, STRING_TOKEN (STR_USB_PROTOCOL_EF_07_01H)},
    {{{2,1,  0,1,0}, 0x07, 0x02,   1}, STRING_TOKEN (STR_USB_INT_PROTOCOL_EF_07_02H)}
  },
  { // STR_USB_CLASS_FEH_APPLICATION
    {{{0,0,  0,1,0}, 0x01,    0,   2}, STRING_TOKEN (STR_USB_INT_SUBCLASS_FE_01H_TO_02H)},
    {{{2,0,  0,1,0}, 0x01, 0x01,   2}, STRING_TOKEN (STR_USB_INT_PROTOCOL_FE_01_01H_TO_02H)},
    {{{2,0,  0,1,0}, 0x02, 0x00,   1}, STRING_TOKEN (STR_NOT_APPLICABLE)},
    {{{2,1,  0,1,0}, 0x03, 0x00,   2}, STRING_TOKEN (STR_USB_INT_PROTOCOL_FE_03_00H_TO_01H)}
  }
};

STATIC
INTN
CompareUsbClassCodeInEntry (
  IN CONST VOID  *Key,
  IN CONST VOID  *Element,
  IN VOID        *Context OPTIONAL
  )
{
  UINT8                                       ClassCode;
  CONST H2O_USB_CLASS_CODE_DESCRIPTION_ENTRY  *ClassDescEntry;

  ClassCode = *(CONST UINT8 *) Key;
  ClassDescEntry = (CONST H2O_USB_CLASS_CODE_DESCRIPTION_ENTRY *) Element;
  (VOID) Context;

  return (INTN) ClassCode - (INTN) ClassDescEntry->Class;
}

STATIC
CONST H2O_USB_CLASS_CODE_DESCRIPTION_ENTRY *
FindUsbClassCodeDescriptionEntry (
  IN H2O_USB_TEXT_LOOKUP_PRIVATE_DATA  *PrivateData,
  IN UINT8                             ClassCode,
  OUT UINT8                            *NextClassCode OPTIONAL
  )
{
  UINTN          UsbClassDescTableSize;
  RETURN_STATUS  Status;
  UINTN          Index;

  if (PrivateData == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  if (PrivateData->UsbClassDescTable != mUsbClassDescTable) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The UsbClassDescTable is not equal to mUsbClassDescTable\n",
      __FUNCTION__
      ));
    return NULL;
  }

  UsbClassDescTableSize = ARRAY_SIZE (mUsbClassDescTable);

  Status = BinarySearchEx (
             (UINT8 *) &ClassCode,
             PrivateData->UsbClassDescTable,
             UsbClassDescTableSize,
             sizeof (*mUsbClassDescTable),
             CompareUsbClassCodeInEntry,
             NULL, // Context
             &Index
             );
  if (NextClassCode != NULL) {
    if (RETURN_ERROR (Status)) {
      *NextClassCode = PrivateData->UsbClassDescTable[Index].Class;
    } else if (Index < UsbClassDescTableSize - 1) {
      *NextClassCode = PrivateData->UsbClassDescTable[Index + 1].Class;
    } else {
      *NextClassCode = 0x00;
    }
  }
  if (RETURN_ERROR (Status)) {
    return NULL;
  }
  return &PrivateData->UsbClassDescTable[Index];
}

STATIC
CONST H2O_USB_SUBCLASS_RANGE_PRIVATE *
InternalGetNextUsbSubClassRange (
  IN CONST H2O_USB_SUBCLASS_RANGE_PRIVATE  *SubClassRange
  )
{
  if (SubClassRange->Range.Flags.IsLastEntryOfClass) {
    return NULL;
  } else {
    return SubClassRange + 1;
  }
}

STATIC
VOID
FindUsbSubClassRanges (
  IN CONST H2O_USB_SUBCLASS_RANGE_PRIVATE   *StartingRangeEntry,
  IN H2O_USB_CLASS_CODE_SEARCH_KEY          SearchKey,
  OUT CONST H2O_USB_SUBCLASS_RANGE_PRIVATE  **SubClassRange OPTIONAL,
  OUT CONST H2O_USB_SUBCLASS_RANGE_PRIVATE  **ProtocolRange OPTIONAL
  )
{
  UINT8                                 DescriptorType;
  BOOLEAN                               NeedsSubClassRange;
  BOOLEAN                               NeedsProtocolRange;
  CONST H2O_USB_SUBCLASS_RANGE_PRIVATE  *RangeEntry;
  BOOLEAN                               IsSubClassInRange;

  ASSERT (StartingRangeEntry != NULL);
  ASSERT (SubClassRange == NULL || (*SubClassRange) == NULL);
  ASSERT (ProtocolRange == NULL || (*ProtocolRange) == NULL);

  if (SearchKey.Codes.DescriptorType == USB_DESC_TYPE_DEVICE_QUALIFIER) {
    SearchKey.Codes.DescriptorType = USB_DESC_TYPE_DEVICE;
  }
  DescriptorType     = SearchKey.Codes.DescriptorType;
  NeedsSubClassRange = (SubClassRange != NULL);
  NeedsProtocolRange = (ProtocolRange != NULL);
  for (RangeEntry = StartingRangeEntry;
      RangeEntry != NULL && (NeedsSubClassRange || NeedsProtocolRange);
      RangeEntry = InternalGetNextUsbSubClassRange (RangeEntry))
  {
    if (DescriptorType == USB_DESC_TYPE_DEVICE &&
        (!RangeEntry->Range.Flags.AppliesToDeviceClass))
    {
      continue;
    }
    if (DescriptorType == USB_DESC_TYPE_INTERFACE &&
        (!RangeEntry->Range.Flags.AppliesToInterfaceClass))
    {
      continue;
    }
    if (DescriptorType == USB_DESC_TYPE_INTERFACE_ASSOCIATION &&
        (!RangeEntry->Range.Flags.AppliesToFunctionClass))
    {
      continue;
    }
    IsSubClassInRange =
      (RangeEntry->Range.Flags.RangeType & BIT0) != 0 ||
      (RangeEntry->Range.SubClass <= SearchKey.Codes.SubClass &&
      RangeEntry->Range.SubClass + RangeEntry->Range.CodeCount >
      SearchKey.Codes.SubClass);
    if (!IsSubClassInRange) {
      continue;
    }
    if (NeedsSubClassRange &&
        RangeEntry->Range.Flags.RangeType == H2OUsbSubClassRange)
    {
      *SubClassRange = RangeEntry;
      NeedsSubClassRange = FALSE;
      continue;
    }
    if (NeedsProtocolRange &&
        (RangeEntry->Range.Flags.RangeType & BIT1) != 0 &&
        (RangeEntry->Range.Protocol <= SearchKey.Codes.Protocol &&
        RangeEntry->Range.Protocol + RangeEntry->Range.CodeCount >
        SearchKey.Codes.Protocol))
    {
      *ProtocolRange = RangeEntry;
      NeedsProtocolRange = FALSE;
    }
  }
}

STATIC
EFI_STRING
InternalCreateUsbClassCodeString (
  IN EFI_HII_HANDLE  HiiHandle,
  IN EFI_STRING_ID   StringId,
  IN CONST CHAR8     *Language OPTIONAL,
  IN UINT8           CodeOffset
  )
{
  EFI_STRING    HiiString;
  CONST CHAR16  *CodeString;
  UINTN         ShortStringLength;
  UINTN         CharIndex;
  UINTN         CodeStringLength;
  UINTN         AllocateSize;
  UINTN         DigitsCount;
  CHAR16        *NewString;

  HiiString = HiiGetString (HiiHandle, StringId, Language);
  if (HiiString == NULL) {
    return NULL;
  }

  CodeString        = (CHAR16 *) HiiString;
  ShortStringLength = 0;
  CharIndex         = 0;
  while (CodeString[CharIndex] != L'\0') {
    if (CodeString[CharIndex] == L'|') {
      if (CodeOffset <= 0) {
        break;
      } else if (CodeString[CharIndex + 1] != L'\0') {
        CodeOffset--;
        CodeString += CharIndex + 1;
        ShortStringLength = 0;
        CharIndex         = 0;
      }
    } else if (CodeString[CharIndex] == L';' && ShortStringLength <= 0) {
      ShortStringLength = CharIndex;
      if (CharIndex <= 0) {
        CodeString++;
      } else {
        CharIndex++;
      }
    } else {
      CharIndex++;
    }
  }
  CodeStringLength = CharIndex;
  AllocateSize = (CharIndex + 1) * sizeof (CHAR16);
  if (ShortStringLength > 0) {
    CodeStringLength -= (ShortStringLength + 1);
    AllocateSize += sizeof (CHAR16); // Converting from L"|" to L"\r\n"
  }
  DigitsCount = 0;
  if (CodeOffset > 0) {
    DigitsCount = ((CodeOffset < 10) ? 1 : ((CodeOffset < 100) ? 2 : 3));
    AllocateSize += (sizeof (" (+)") - 1 + DigitsCount) * sizeof (CHAR16);
    if (ShortStringLength > 0) {
      AllocateSize += (sizeof (" +") - 1 + DigitsCount) * sizeof (CHAR16);
    }
  }

  NewString = AllocatePool (AllocateSize);
  if (NewString != NULL) {
    CharIndex = 0;
    if (ShortStringLength > 0) {
      CopyMem (
        (NewString + CharIndex),
        CodeString,
        (ShortStringLength * sizeof (CHAR16))
        );
      CharIndex += ShortStringLength;
      CodeString += ShortStringLength + 1;
      if (CodeOffset > 0) {
        CharIndex += UnicodeSPrintAsciiFormat (
                       (NewString + CharIndex),
                       (AllocateSize - (CharIndex * sizeof (CHAR16))),
                       " +%d",
                       (unsigned int) CodeOffset
                       );
      }
      NewString[CharIndex++] = L'\r';
      NewString[CharIndex++] = L'\n';
    }
    CopyMem (
      (NewString + CharIndex),
      CodeString,
      (CodeStringLength * sizeof (CHAR16))
      );
    CharIndex += CodeStringLength;
    if (CodeOffset > 0) {
      CharIndex += UnicodeSPrintAsciiFormat (
                     (NewString + CharIndex),
                     (AllocateSize - (CharIndex * sizeof (CHAR16))),
                     " (+%d)",
                     (unsigned int) CodeOffset
                     );
    }
    NewString[CharIndex++] = L'\0';
    ASSERT (CharIndex * sizeof (CHAR16) == AllocateSize);
  }

  FreePool (HiiString);
  return (EFI_STRING) NewString;
}

/**
  Looks up a group of USB class, subclass and protocol code strings.

  Searches in the database of this Lookup Protocol for strings describing the
  USB class, subclass and protocol codes.

  @implements H2O_GET_USB_CLASS_CODE_STRING

  @param[in]  This                A pointer to the H2O_USB_TEXT_LOOKUP_PROTOCOL
                                  instance.
  @param[in]  SearchKey           A search key including the USB descriptor
                                  type, class, subclass and protocol codes.
  @param[in]  Language            The language of the string to retrieve. If
                                  this parameter is NULL, then the current
                                  platform language is used. The format of
                                  Language must follow the language format
                                  assumed in the HII Database.
  @param[out] ClassNameString     On output, points to an allocated string
                                  describing the USB class code. The caller is
                                  responsible for freeing this buffer using
                                  FreePool().
  @param[out] SubClassNameString  On output, points to an allocated string
                                  describing the USB subclass code. The caller
                                  is responsible for freeing this buffer using
                                  FreePool(). If no string is found for the
                                  subclass code, this points to NULL instead.
  @param[out] ProtocolNameString  On output, points to an allocated string
                                  describing the USB protocol code. The caller
                                  is responsible for freeing this buffer using
                                  FreePool(). If no string is found for the
                                  protocol code, this points to NULL instead.

  @retval  EFI_SUCCESS           The strings are returned successfully.
  @retval  EFI_NOT_FOUND         The specified Class in SearchKey is not found
                                 in the database of this Lookup Protocol.
  @retval  EFI_OUT_OF_RESOURCES  No enough memory to allocate the strings. No
                                 strings will be returned in this error case.

**/
EFI_STATUS
EFIAPI
GetUsbClassCodeString (
  IN H2O_USB_TEXT_LOOKUP_PROTOCOL   *This,
  IN H2O_USB_CLASS_CODE_SEARCH_KEY  SearchKey,
  IN CONST CHAR8                    *Language OPTIONAL,
  OUT EFI_STRING                    *ClassNameString OPTIONAL,
  OUT EFI_STRING                    *SubClassNameString OPTIONAL,
  OUT EFI_STRING                    *ProtocolNameString OPTIONAL
  )
{
  H2O_USB_TEXT_LOOKUP_PRIVATE_DATA            *PrivateData;
  CONST H2O_USB_CLASS_CODE_DESCRIPTION_ENTRY  *UsbClassDescEntry;
  BOOLEAN                                     ReturnStatusOnly;
  CONST H2O_USB_SUBCLASS_RANGE_PRIVATE        *StartingRangeEntry;
  CONST H2O_USB_SUBCLASS_RANGE_PRIVATE        *SubClassRange;
  CONST H2O_USB_SUBCLASS_RANGE_PRIVATE        *ProtocolRange;
  EFI_STATUS                                  Status;
  EFI_STRING                                  NewClassString;
  EFI_STRING                                  NewSubClassString;
  EFI_STRING                                  NewProtocolString;

  if (This == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_USB_TEXT_LOOKUP_PRIVATE_FROM_PROTOCOL (This);

  if (ClassNameString != NULL) {
    *ClassNameString = NULL;
  }
  if (SubClassNameString != NULL) {
    *SubClassNameString = NULL;
  }
  if (ProtocolNameString != NULL) {
    *ProtocolNameString = NULL;
  }

  UsbClassDescEntry = FindUsbClassCodeDescriptionEntry (
                        PrivateData,
                        SearchKey.Codes.Class,
                        NULL // NextClassCode
                        );
  if (UsbClassDescEntry == NULL) {
    return EFI_NOT_FOUND;
  }

  ReturnStatusOnly = (ClassNameString == NULL && SubClassNameString == NULL &&
                     ProtocolNameString == NULL);

  StartingRangeEntry = ((CONST H2O_USB_SUBCLASS_RANGE_PRIVATE *)
                         (PrivateData->UsbSubClassRangeTable)) +
                         UsbClassDescEntry->SubClassRangeTableOffset;
  SubClassRange = NULL;
  ProtocolRange = NULL;
  FindUsbSubClassRanges (
    StartingRangeEntry,
    SearchKey,
    ((SubClassNameString != NULL || ReturnStatusOnly) ? &SubClassRange : NULL),
    ((ProtocolNameString != NULL || ReturnStatusOnly) ? &ProtocolRange : NULL)
    );

  if (ReturnStatusOnly) {
    if (SubClassRange != NULL && ProtocolRange != NULL) {
      return EFI_SUCCESS;
    } else {
      return EFI_NOT_FOUND;
    }
  }

  Status            = EFI_NOT_FOUND;
  NewClassString    = NULL;
  NewSubClassString = NULL;
  NewProtocolString = NULL;
  if (ClassNameString != NULL) {
    NewClassString = InternalCreateUsbClassCodeString (
                       PrivateData->HiiHandle,
                       UsbClassDescEntry->NameStringId,
                       Language,
                       0 // CodeOffset
                       );
    if (NewClassString == NULL) {
      goto ClassStringFail;
    }
    Status = EFI_SUCCESS;
  }
  if (SubClassNameString != NULL && SubClassRange != NULL &&
      SubClassRange->NameStringId != STRING_TOKEN (0x0000))
  {
    NewSubClassString = InternalCreateUsbClassCodeString (
                          PrivateData->HiiHandle,
                          SubClassRange->NameStringId,
                          Language,
                          (SearchKey.Codes.SubClass -
                            SubClassRange->Range.SubClass)
                          );
    if (NewSubClassString == NULL) {
      goto SubClassStringFail;
    }
    Status = EFI_SUCCESS;
  }
  if (ProtocolNameString != NULL && ProtocolRange != NULL &&
      ProtocolRange->NameStringId != STRING_TOKEN (0x0000))
  {
    NewProtocolString = InternalCreateUsbClassCodeString (
                          PrivateData->HiiHandle,
                          ProtocolRange->NameStringId,
                          Language,
                          (SearchKey.Codes.Protocol -
                            ProtocolRange->Range.Protocol)
                          );
    if (NewProtocolString == NULL) {
      goto ProtocolStringFail;
    }
    Status = EFI_SUCCESS;
  }
  if (ClassNameString != NULL) {
    *ClassNameString = NewClassString;
  }
  if (SubClassNameString != NULL) {
    *SubClassNameString = NewSubClassString;
  }
  if (ProtocolNameString != NULL) {
    *ProtocolNameString = NewProtocolString;
  }
  return Status;

ProtocolStringFail:
  if (NewSubClassString != NULL) {
    FreePool (NewSubClassString);
  }
SubClassStringFail:
  if (NewClassString != NULL) {
    FreePool (NewClassString);
  }
ClassStringFail:
  return EFI_OUT_OF_RESOURCES;
}

/**
  Retrieves next range of known USB class, subclass or protocol codes.

  Returns an internal structure that specifies a range of USB subclass codes or
  Protocol codes. This function can be used to list all USB class codes
  collected in the string database of this Lookup Protocol.

  If SubClassRange is a pointer to NULL, then the address of the first range of
  subclasses of the given ClassCode is returned in SubClassRange, and the next
  class code is returned in NextClassCode, and EFI_SUCCESS is returned.

  If SubClassRange points to an address that was returned on a previous call to
  GetNextUsbSubClassRange(), then ClassCode and NextClassCode are ignored, and
  the address of the next range of subclasses is returned in SubClassRange, and
  EFI_SUCCESS is returned.

  If SubClassRange points to an address that is the last range of subclasses of
  a particular class code, then ClassCode and NextClassCode are ignored, and a
  pointer to NULL is returned in SubClassRange, and EFI_SUCCESS is returned.

  If SubClassRange is a pointer that does not point to NULL nor to an address
  returned on a previous call to GetNextUsbSubClassRange(), then ASSERT().

  @implements H2O_GET_NEXT_USB_SUBCLASS_RANGE

  @param[in]     This           A pointer to the H2O_USB_TEXT_LOOKUP_PROTOCOL
                                instance.
  @param[in]     ClassCode      The USB class code to look up.
  @param[in,out] SubClassRange  On input, a pointer to the previous
                                SubClassRange buffer returned by
                                GetNextUsbSubClassRange(). On output, a pointer
                                to the next range of subclasses, or NULL if
                                SubClassRange is the last range of a particular
                                class code. Passing in a pointer to NULL, will
                                return the first range of subclasses of the
                                given ClassCode.
  @param[out]    NextClassCode  Returns the next USB class code known by this
                                Lookup Protocol. This field is valid only when
                                SubClassRange is a pointer to NULL.
                                If ClassCode is 0xFF, then 0x00 is returned.

  @retval  EFI_SUCCESS            The next SubClassRange or NextClassCode is
                                  retrieved successfully.
  @retval  EFI_INVALID_PARAMETER  Both SubClassRange and NextClassCode are
                                  NULL.
  @retval  EFI_NOT_FOUND          SubClassRange is NULL, and ClassCode is not
                                  found in the database. NextClassCode will
                                  point to the next valid class code in the
                                  database in this case.

**/
EFI_STATUS
EFIAPI
GetNextUsbSubClassRange (
  IN H2O_USB_TEXT_LOOKUP_PROTOCOL      *This,
  IN UINT8                             ClassCode,
  IN OUT CONST H2O_USB_SUBCLASS_RANGE  **SubClassRange OPTIONAL,
  OUT UINT8                            *NextClassCode OPTIONAL
  )
{
  H2O_USB_TEXT_LOOKUP_PRIVATE_DATA            *PrivateData;
  INTN                                        PtrDiff;
  BOOLEAN                                     SubClassRangePointerIsValid;
  CONST H2O_USB_SUBCLASS_RANGE_PRIVATE        *SubClassRangePrivate;
  CONST H2O_USB_CLASS_CODE_DESCRIPTION_ENTRY  *UsbClassDescEntry;

  if (This == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_USB_TEXT_LOOKUP_PRIVATE_FROM_PROTOCOL (This);

  if (SubClassRange == NULL && NextClassCode == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (SubClassRange != NULL) {
    PtrDiff = 0;
    SubClassRangePointerIsValid = TRUE;
#if !defined(MDEPKG_NDEBUG)
    if (DebugAssertEnabled ()) {
      PtrDiff = ((CONST UINT8 *) *SubClassRange) -
                  ((CONST UINT8 *) PrivateData->UsbSubClassRangeTable);
      SubClassRangePointerIsValid =
        (PtrDiff > 0 && PtrDiff < sizeof (mUsbSubClassRangeTable) &&
        PtrDiff % sizeof (H2O_USB_SUBCLASS_RANGE_PRIVATE) == 0);
      //
      // Throw assertion error to caller when (*SubClassRange) looks uninitialised.
      //
      ASSERT ((*SubClassRange) == NULL || SubClassRangePointerIsValid);
    }
#endif
    if ((*SubClassRange) != NULL && SubClassRangePointerIsValid) {
      ASSERT (OFFSET_OF (H2O_USB_SUBCLASS_RANGE_PRIVATE, Range) == 0);
      SubClassRangePrivate = InternalGetNextUsbSubClassRange (
                               ((CONST H2O_USB_SUBCLASS_RANGE_PRIVATE *)
                                 (*SubClassRange))
                               );
      *SubClassRange = &SubClassRangePrivate->Range;
      return EFI_SUCCESS;
    }
  }

  UsbClassDescEntry = FindUsbClassCodeDescriptionEntry (
                        PrivateData,
                        ClassCode,
                        NextClassCode
                        );
  if (UsbClassDescEntry == NULL) {
    return EFI_NOT_FOUND;
  }
  if (SubClassRange != NULL) {
    *SubClassRange =
      &((PrivateData->UsbSubClassRangeTable->NoScDef +
      UsbClassDescEntry->SubClassRangeTableOffset)->Range);
    if ((*SubClassRange)->CodeCount <= 0) {
      //
      // Don't output a dummy entry. Output a null pointer instead.
      //
      *SubClassRange = NULL;
    }
  }
  return EFI_SUCCESS;
}
