/** @file
  H2O USB Text Lookup Protocol interface definitions

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

#ifndef H2O_USB_TEXT_LOOKUP_H_
#define H2O_USB_TEXT_LOOKUP_H_

#include <Uefi.h>

#include <IndustryStandard/Usb.h>

#define H2O_USB_TEXT_LOOKUP_PROTOCOL_GUID \
  { \
    0x3d003d61, 0xf366, 0x4419, { 0xa1, 0xc4, 0xc5, 0x65, 0x33, 0xcd, 0x6e, 0xe4 } \
  }

typedef struct _H2O_USB_TEXT_LOOKUP_PROTOCOL  H2O_USB_TEXT_LOOKUP_PROTOCOL;

typedef union {
  struct {
    UINT8  DescriptorType;
    UINT8  Class;
    UINT8  SubClass;
    UINT8  Protocol;
  } Codes;
  UINT32  Uint32;
} H2O_USB_CLASS_CODE_SEARCH_KEY;

/**
  Looks up a group of USB class, subclass and protocol code strings.

  Searches in the database of this Lookup Protocol for strings describing the
  USB class, subclass and protocol codes.

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
typedef
EFI_STATUS
(EFIAPI *H2O_GET_USB_CLASS_CODE_STRING) (
  IN H2O_USB_TEXT_LOOKUP_PROTOCOL   *This,
  IN H2O_USB_CLASS_CODE_SEARCH_KEY  SearchKey,
  IN CONST CHAR8                    *Language OPTIONAL,
  OUT EFI_STRING                    *ClassNameString OPTIONAL,
  OUT EFI_STRING                    *SubClassNameString OPTIONAL,
  OUT EFI_STRING                    *ProtocolNameString OPTIONAL
  );

typedef enum {
  H2OUsbSubClassRange         = 0,
  H2OReservedUsbSubClassRange = 1,
  H2OUsbSubClassProtocolRange = 2,
  H2OUsbClassProtocolRange    = 3
} H2O_USB_SUBCLASS_RANGE_TYPE;

typedef struct {
  struct {
    UINT8  RangeType:2;
    UINT8  IsLastEntryOfClass:1;
    UINT8  AppliesToDeviceClass:1;
    UINT8  AppliesToInterfaceClass:1;
    UINT8  AppliesToFunctionClass:1;
	UINT8  Reserved:2;
  } Flags;
  UINT8    SubClass;
  UINT8    Protocol;
  UINT8    CodeCount;
} H2O_USB_SUBCLASS_RANGE;

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
typedef
EFI_STATUS
(EFIAPI *H2O_GET_NEXT_USB_SUBCLASS_RANGE) (
  IN H2O_USB_TEXT_LOOKUP_PROTOCOL      *This,
  IN UINT8                             ClassCode,
  IN OUT CONST H2O_USB_SUBCLASS_RANGE  **SubClassRange OPTIONAL,
  OUT UINT8                            *NextClassCode OPTIONAL
  );

struct _H2O_USB_TEXT_LOOKUP_PROTOCOL {
  struct {
    UINT8  ClassTextLookupSupported:1;
    UINT8  Reserved:7;
  } Flags;
  UINT8  Reserved[3];
  H2O_GET_USB_CLASS_CODE_STRING    GetUsbClassCodeString;
  H2O_GET_NEXT_USB_SUBCLASS_RANGE  GetNextUsbSubClassRange;
};

extern EFI_GUID gH2OUsbTextLookupProtocolGuid;

#endif // H2O_USB_TEXT_LOOKUP_H_
