/** @file

;******************************************************************************
;* Copyright (c) 2012 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _UI_FUNCS_H
#define _UI_FUNCS_H

#include <Protocol/H2OWindow.h>

extern UINT32                   mDbgNo;
extern UINT32                   mSetPositionLevel;
extern BOOLEAN                  mShowSetPositionDbg;
extern BOOLEAN                  mShowUpdateRegionDbg;


typedef struct _UI_CONTROL_CLASS UI_CONTROL_CLASS;
typedef struct _UI_LENGTH        UI_LENGTH;

typedef
UI_CONTROL_CLASS *
(EFIAPI *UI_GET_CLASS) (
  VOID
  );

VOID
EFIAPI
RegisterClassTable (
  UI_GET_CLASS                  *GetClassTable
  );

EFI_IMAGE_INPUT *
GetImageByString (
  CHAR16  *Name
  );

EFI_IMAGE_INPUT *
GetVfcfImage (
  IN EFI_IMAGE_ID               ImageId
  );

COLORREF
EFIAPI
GetColorValue (
  CHAR16 *Value
  );

VOID
AdjustImageToMenuColor (
  IN OUT UI_CONTROL             *Control
  );

CHAR16 *
StrDuplicate (
  IN CONST CHAR16  *Src
  );

BOOLEAN
EFIAPI
UiSetAttribute (
  VOID       *This,
  CHAR16     *Name,
  CHAR16     *Value
  );

BOOLEAN
EFIAPI
UiSetAttributeEx (
  VOID                          *This,
  CHAR16                        *Name,
  IN  CONST CHAR16              *Format,
  ...
  );

UI_CONTROL *
UiFindChildByName (
  VOID                          *This,
  CHAR16                        *Name
  );

VOID
ShowSetPositionDbgInfo (
  UI_CONTROL                    *Control,
  CONST RECT                    *Pos
  );

VOID
ShowUpdateRegionDbgInfo (
  UI_CONTROL                    *Control,
  CONST RECT                    *Pos
  );

VOID
UiNeedUpdatePos (
  VOID                          *This
  );

INT32
UiLengthToPixelEx (
  IN UI_LENGTH                     *Length,
  IN INT32                         Available,
  IN INT32                         FontSize,
  IN BOOLEAN                       IncludeRemainderPixel
  );

INT32
UiLengthToPixel (
  IN UI_LENGTH                     *Length,
  IN INT32                         Available,
  IN INT32                         FontSize
  );

POINT
CalculateControlDisplayXY (
  IN UI_CONTROL                 *Control,
  IN SIZE                       *ControlSize,
  IN SIZE                       *AvailableSize
  );

INT32
CalculateControlDisplayWidth (
  IN UI_CONTROL                 *Control,
  IN SIZE                       AvailableSize
  );

INT32
CalculateControlDisplayHeight (
  IN UI_CONTROL                 *Control,
  IN SIZE                       AvailableSize
  );

SIZE
CalculateControlDisplaySize (
  IN UI_CONTROL                 *Control,
  IN SIZE                       AvailableSize
  );

UINT32
GetControlBkImageStyle (
  IN UI_CONTROL                    *Control
  );

UINT32
GetControlFontSize (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlPaddingLeft (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlPaddingRight (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlPaddingTop (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlPaddingBottom (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlPaddingWidth (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlPaddingHeight (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlBorderLeft (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlBorderRight (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlBorderTop (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlBorderBottom (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlBorderWidth (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlBorderHeight (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlMarginLeft (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlMarginRight (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlMarginTop (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlMarginBottom (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlMarginWidth (
  IN UI_CONTROL                    *Control
  );

INT32
GetControlMarginHeight (
  IN UI_CONTROL                    *Control
  );

UINT8
ConvertToIfrNumericFlags (
  IN BOOLEAN                       IsHexValue,
  IN BOOLEAN                       IsSignedValue,
  IN UINTN                         ValueSize
  );

//
// XML parser
//
typedef
VOID
(EFIAPI *SAX_PARSER_CONTENT_CALLBACK) (
  VOID         *Data,
  CHAR16       *Str
  );

typedef
VOID
(EFIAPI *SAX_PARSER_START_CALLBACK) (
  VOID         *Data,
  CHAR16       *Element,
  CHAR16       **AttrNames,
  CHAR16       **AttrValues
  );


typedef
VOID
(EFIAPI *SAX_PARSER_END_CALLBACK) (
  VOID         *Data,
  CHAR16       *Element
  );

BOOLEAN
EFIAPI
SaxParser (
  CHAR16                      *XMLBuffer,
  SAX_PARSER_START_CALLBACK   StartCb,
  SAX_PARSER_CONTENT_CALLBACK ContentCb,
  SAX_PARSER_END_CALLBACK     EndCb,
  VOID                        *Data
  );

UI_CONTROL *
EFIAPI
XmlCreateControl (
  CHAR16     *XMLBuffer,
  UI_CONTROL *Parent
  );

#endif
