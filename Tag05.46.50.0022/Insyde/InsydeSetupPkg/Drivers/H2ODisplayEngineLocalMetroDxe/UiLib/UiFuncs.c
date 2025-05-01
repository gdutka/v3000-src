/** @file
  UI Common Controls

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "UiControls.h"
#include "UiManager.h"
#include "UiRender.h"
#include "H2ODisplayEngineLocalMetro.h"
#include "MetroUi.h"

UINT32                          mDbgNo               = 0;
UINT32                          mSetPositionLevel    = 0;
BOOLEAN                         mShowSetPositionDbg  = FALSE;
BOOLEAN                         mShowUpdateRegionDbg = FALSE;
STATIC CHAR16                   mPaddingChar[201] = {0};

#define MAX_PARENT  128

typedef struct {
  UI_CONTROL *Parents[MAX_PARENT];
  UI_CONTROL *FirstControl;
  CHAR16     *ParentsName[MAX_PARENT];
  UINTN      Count;
  UI_MANAGER *Manager;
  HINSTANCE  Instance;
} XML_UI_DATA;


VOID
SetUnicodeMem (
  IN VOID   *Buffer,
  IN UINTN  Size,
  IN CHAR16 Value
  )
/*++

Routine Description:

  Set Buffer to Value for Size bytes.

Arguments:

  Buffer  - Memory to set.

  Size    - Number of bytes to set

  Value   - Value of the set operation.

Returns:

  None

--*/
{
  CHAR16  *Ptr;

  Ptr = Buffer;
  while (Size--) {
    *(Ptr++) = Value;
  }
}


VOID
ShowSetPositionDbgInfo (
  UI_CONTROL                    *Control,
  CONST RECT                    *Pos
  )
{
  if (!mShowSetPositionDbg) {
    return ;
  }

  if (mSetPositionLevel == 1) {
    SetUnicodeMem (mPaddingChar, 200, '-');
  }

  if (mSetPositionLevel > 50) {
    ASSERT (FALSE);
  }

  mPaddingChar[(mSetPositionLevel << 1) + 1] = '\0';
  DEBUG ((
    EFI_D_INFO,
    "%06d:SetPosition %4d %4d %4d %4d:%s(%s):%s:%d\n",
    mDbgNo++,
    Pos->left,
    Pos->top,
    Pos->right - Pos->left,
    Pos->bottom - Pos->top,
    mPaddingChar,
    Control->Class->ClassName,
    ((Control->Name[0] != '\0') ? Control->Name : L""),
    mSetPositionLevel
    ));
  mPaddingChar[(mSetPositionLevel << 1) + 1] = '-';
}

VOID
ShowUpdateRegionDbgInfo (
  UI_CONTROL                    *Control,
  CONST RECT                    *Pos
  )
{
  if (!mShowUpdateRegionDbg) {
    return;
  }

  DEBUG ((
    EFI_D_INFO, "%06d:UpdateRegion %4d %4d %4d %4d:(%s)\n",
    mDbgNo++,
    Pos->left,
    Pos->top,
    Pos->right - Pos->left,
    Pos->bottom - Pos->top,
    Control->Class->ClassName
    ));
}

BOOLEAN
EFIAPI
UiSetAttribute (
  VOID                          *This,
  CHAR16                        *Name,
  CHAR16                        *Value
  )
{
  ASSERT (This != NULL);
  if (This == NULL) {
    return FALSE;
  }
  return CONTROL_CLASS(This)->SetAttribute ((UI_CONTROL *)This, Name, Value);
}

BOOLEAN
EFIAPI
UiSetAttributeEx (
  VOID                          *This,
  CHAR16                        *Name,
  IN  CONST CHAR16              *Format,
  ...
  )
{
  VA_LIST                       Marker;
  CHAR16                        *ValueStr;
  BOOLEAN                       Result;

  ZeroMem (&Marker, sizeof (Marker));
  VA_START (Marker, Format);
  ValueStr = CatVSPrint (NULL, Format, Marker);
  VA_END (Marker);

  Result = CONTROL_CLASS(This)->SetAttribute ((UI_CONTROL *)This, Name, ValueStr);

  FreePool (ValueStr);

  return Result;
}

UI_CONTROL *
UiFindChildByName (
  VOID                          *This,
  CHAR16                        *Name
  )
{
  return CONTROL_CLASS(This)->FindChildByName ((UI_CONTROL *)This, Name, UI_SEARCH_TYPE_ALL);
}

VOID
EFIAPI
UiApplyAttributeList (
  UI_CONTROL *Control,
  CHAR16     *StrList
  )
{
  CHAR16                        Name[256];
  CHAR16                        Value[1024];
  UINTN                         Index;
  CHAR16                        QuoteChar;

  while(*StrList != L'\0') {
    Index = 0;

    //
    // skip space or tab
    //
    while ((*StrList == L' ') || (*StrList == L'\t')) {
      StrList++;
    }

    while( *StrList != L'\0' && *StrList != L'=') {
      Name[Index++] = *StrList++;
    }
    Name[Index] = '\0';

    ASSERT(*StrList == L'=');
    if(*StrList++ != L'=') {
      return ;
    }

    ASSERT(*StrList == L'\"' || *StrList == L'\'');
    if (*StrList != L'\"' && *StrList != L'\'') {
      return ;
    }

    QuoteChar = *StrList;
    StrList++;
    Index = 0;
    while(*StrList != L'\0' && *StrList != QuoteChar) {
      if (*StrList == '\\' && *(StrList + 1) == QuoteChar) {
        Value[Index++] = *StrList++;
      }
      Value[Index++] = *StrList++;
    }
    Value[Index] = '\0';

    ASSERT(*StrList == L'\"' || *StrList == L'\'');
    if (*StrList != L'\"' && *StrList != L'\'') {
      return ;
    }

    UiSetAttribute (Control, Name, Value);

    StrList++;
    if(*StrList != L' ' && *StrList != '\t') {
      return ;
    }
    StrList++;
  }
}

UI_CONTROL *
EFIAPI
GetUiControl (
  HWND Hwnd
  )
{

  return (UI_CONTROL *) GetWindowLongPtr (Hwnd, 0);
}

UI_CONTROL *
EFIAPI
CreateControl (
  CHAR16     *ControlName,
  UI_CONTROL *Parent
  )
{
  HWND                          Wnd;
  UI_MANAGER                    *Manager;

  Manager = NULL;
  if (Parent != NULL) {
    Manager = Parent->Manager;
    ASSERT (Manager != NULL);
  }

  Wnd = CreateWindowEx (
          WS_EX_NOACTIVATE,
          ControlName,
          NULL,
          ((Parent != NULL) ? WS_CHILD : 0) | WS_VISIBLE,
          0, 0, 0, 0,
          (Parent != NULL) ? Parent->Wnd : NULL,
          NULL,
          NULL,
          Manager
          );

  ASSERT (Wnd != NULL);

  if (Wnd == NULL) {
    return NULL;
  }

  return GetUiControl (Wnd);
}

UI_CONTROL *
EFIAPI
CreateAddControl (
  CHAR16     *ControlName,
  UI_CONTROL *Parent
  )
{
  UI_CONTROL *CreatedControl;

  CreatedControl = CreateControl (ControlName, Parent);
  if (CreatedControl != NULL) {
    CONTROL_CLASS(Parent)->AddChild (Parent, CreatedControl);
  }

  return CreatedControl;
}

INT32
UiLengthToPixelEx (
  IN UI_LENGTH                     *Length,
  IN INT32                         Available,
  IN INT32                         FontSize,
  IN BOOLEAN                       IncludeRemainderPixel
  )
{
  switch (Length->Type) {

  case UI_LENGTH_TYPE_PIXEL:
    return Length->Value;

  case UI_LENGTH_TYPE_PERCENTAGE:
    if (IncludeRemainderPixel) {
      return (Length->Value * Available) / 100 + ((((Length->Value * Available) % 100) > 0) ? 1 : 0);
    } else {
      return (Length->Value * Available) / 100;
    }

  case UI_LENGTH_TYPE_EM:
    return (Length->Value * FontSize);

  default:
    return 0;
  }
}

INT32
UiLengthToPixel (
  IN UI_LENGTH                     *Length,
  IN INT32                         Available,
  IN INT32                         FontSize
  )
{
  return UiLengthToPixelEx (Length, Available, FontSize, FALSE);
}

POINT
CalculateControlDisplayXY (
  IN UI_CONTROL                 *Control,
  IN SIZE                       *ControlSize,
  IN SIZE                       *AvailableSize
  )
{
  POINT                         Point;

  Point.x = 0;
  Point.y = 0;

  //
  // Control display location is determined by Left, Top, Right, Bottom. Top and Left have higher priority.
  //
  switch (Control->Left.Type) {

  case UI_LENGTH_TYPE_PIXEL:
  case UI_LENGTH_TYPE_PERCENTAGE:
  case UI_LENGTH_TYPE_EM:
    Point.x = UiLengthToPixelEx (&Control->Left, AvailableSize->cx, GetControlFontSize (Control), FALSE);
    break;

  default:
    switch (Control->Right.Type) {

    case UI_LENGTH_TYPE_PIXEL:
    case UI_LENGTH_TYPE_PERCENTAGE:
    case UI_LENGTH_TYPE_EM:
      Point.x = AvailableSize->cx -
                ControlSize->cx -
                UiLengthToPixelEx (&Control->Right, AvailableSize->cx, GetControlFontSize (Control), TRUE);
      break;

    default:
      break;
    }
  }

  switch (Control->Top.Type) {

  case UI_LENGTH_TYPE_PIXEL:
  case UI_LENGTH_TYPE_PERCENTAGE:
  case UI_LENGTH_TYPE_EM:
    Point.y = UiLengthToPixelEx (&Control->Top, AvailableSize->cy, GetControlFontSize (Control), FALSE);
    break;

  default:
    switch (Control->Bottom.Type) {

    case UI_LENGTH_TYPE_PIXEL:
    case UI_LENGTH_TYPE_PERCENTAGE:
    case UI_LENGTH_TYPE_EM:
      Point.y = AvailableSize->cy -
                ControlSize->cy -
                UiLengthToPixelEx (&Control->Bottom, AvailableSize->cy, GetControlFontSize (Control), TRUE);
      break;
    default:
      break;
    }
  }

  return Point;
}

INT32
CalculateControlDisplayWidth (
  IN UI_CONTROL                 *Control,
  IN SIZE                       AvailableSize
  )
{
  INT32                         Width;

  //
  // Control display width is determined by Width or (Left,Right). Width has higher priority.
  //
  switch (Control->Width.Type) {

  case UI_LENGTH_TYPE_MATCH_PARENT:
    Width = MATCH_PARENT;
    break;

  case UI_LENGTH_TYPE_WRAP_CONTENT:
    Width = WRAP_CONTENT;
    break;

  case UI_LENGTH_TYPE_PIXEL:
  case UI_LENGTH_TYPE_PERCENTAGE:
  case UI_LENGTH_TYPE_EM:
    Width = UiLengthToPixel (&Control->Width, AvailableSize.cx, GetControlFontSize (Control));
    if (Width > 0 && Control->BoxSizing == UI_BOX_SIZING_CONTENT_BOX) {
      Width += GetControlPaddingWidth (Control) + GetControlBorderWidth (Control);
    }
    break;

  default:
    if (IS_UI_LENGTH_VALUE_ASSIGNED(Control->Left ) &&
        IS_UI_LENGTH_VALUE_ASSIGNED(Control->Right)) {
      Width = AvailableSize.cx -
              UiLengthToPixelEx (&Control->Left , AvailableSize.cx, GetControlFontSize (Control), FALSE) -
              UiLengthToPixelEx (&Control->Right, AvailableSize.cx, GetControlFontSize (Control), TRUE );
      Width = MAX (Width, 0);
    } else {
      Width = 0;
    }
    break;
  }

  return Width;
}

INT32
CalculateControlDisplayHeight (
  IN UI_CONTROL                 *Control,
  IN SIZE                       AvailableSize
  )
{
  INT32                         Height;

  //
  // Control display height is determined by Height or (Top,Bottom). Height has higher priority.
  //
  switch (Control->Height.Type) {

  case UI_LENGTH_TYPE_MATCH_PARENT:
    Height = MATCH_PARENT;
    break;

  case UI_LENGTH_TYPE_WRAP_CONTENT:
    Height = WRAP_CONTENT;
    break;

  case UI_LENGTH_TYPE_PIXEL:
  case UI_LENGTH_TYPE_PERCENTAGE:
  case UI_LENGTH_TYPE_EM:
    Height = UiLengthToPixel (&Control->Height, AvailableSize.cy, GetControlFontSize (Control));
    if (Height > 0 && Control->BoxSizing == UI_BOX_SIZING_CONTENT_BOX) {
      Height += GetControlPaddingHeight (Control) + GetControlBorderHeight (Control);
    }
    break;

  default:
    if (IS_UI_LENGTH_VALUE_ASSIGNED(Control->Top   ) &&
        IS_UI_LENGTH_VALUE_ASSIGNED(Control->Bottom)) {
      Height = AvailableSize.cy -
               UiLengthToPixelEx (&Control->Top   , AvailableSize.cy, GetControlFontSize (Control), FALSE) -
               UiLengthToPixelEx (&Control->Bottom, AvailableSize.cy, GetControlFontSize (Control), TRUE );
      Height = MAX (Height, 0);
    } else {
      Height = 0;
    }
    break;
  }

  return Height;
}

SIZE
CalculateControlDisplaySize (
  IN UI_CONTROL                 *Control,
  IN SIZE                       AvailableSize
  )
{
  SIZE                          Size;

  Size.cx = CalculateControlDisplayWidth (Control, AvailableSize);
  Size.cy = CalculateControlDisplayHeight (Control, AvailableSize);

  return Size;
}

UINT32
GetControlBkImageStyle (
  IN UI_CONTROL                    *Control
  )
{
  return ((Control->Filter & DT_GRAY) == DT_GRAY) ? (Control->BkImageStyle | DT_GRAY) : Control->BkImageStyle;
}

UINT32
GetControlFontSize (
  IN UI_CONTROL                    *Control
  )
{
  if (Control->FontSize != 0) {
    return Control->FontSize;
  }

  if (Control->Manager != NULL) {
    return Control->Manager->GetDefaultFontSize (Control->Manager);
  }

  return 0;
}

INT32
GetControlPaddingLeft (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Padding.Left, Control->Available.cx, GetControlFontSize (Control));
}

INT32
GetControlPaddingRight (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Padding.Right, Control->Available.cx, GetControlFontSize (Control));
}

INT32
GetControlPaddingTop (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Padding.Top, Control->Available.cy, GetControlFontSize (Control));
}

INT32
GetControlPaddingBottom (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Padding.Bottom, Control->Available.cy, GetControlFontSize (Control));
}

INT32
GetControlPaddingWidth (
  IN UI_CONTROL                    *Control
  )
{
  return (GetControlPaddingLeft (Control) + GetControlPaddingRight (Control));
}

INT32
GetControlPaddingHeight (
  IN UI_CONTROL                    *Control
  )
{
  return (GetControlPaddingTop (Control) + GetControlPaddingBottom (Control));
}

INT32
GetControlBorderLeft (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Border.Left, Control->Available.cx, GetControlFontSize (Control));
}

INT32
GetControlBorderRight (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Border.Right, Control->Available.cx, GetControlFontSize (Control));
}

INT32
GetControlBorderTop (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Border.Top, Control->Available.cy, GetControlFontSize (Control));
}

INT32
GetControlBorderBottom (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Border.Bottom, Control->Available.cy, GetControlFontSize (Control));
}

INT32
GetControlBorderWidth (
  IN UI_CONTROL                    *Control
  )
{
  return (GetControlBorderLeft (Control) + GetControlBorderRight (Control));
}

INT32
GetControlBorderHeight (
  IN UI_CONTROL                    *Control
  )
{
  return (GetControlBorderTop (Control) + GetControlBorderBottom (Control));
}

INT32
GetControlMarginLeft (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Margin.Left, Control->Available.cx, GetControlFontSize (Control));
}

INT32
GetControlMarginRight (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Margin.Right, Control->Available.cx, GetControlFontSize (Control));
}

INT32
GetControlMarginTop (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Margin.Top, Control->Available.cy, GetControlFontSize (Control));
}

INT32
GetControlMarginBottom (
  IN UI_CONTROL                    *Control
  )
{
  return UiLengthToPixel (&Control->Margin.Bottom, Control->Available.cy, GetControlFontSize (Control));
}

INT32
GetControlMarginWidth (
  IN UI_CONTROL                    *Control
  )
{
  return (GetControlMarginLeft (Control) + GetControlMarginRight (Control));
}

INT32
GetControlMarginHeight (
  IN UI_CONTROL                    *Control
  )
{
  return (GetControlMarginTop (Control) + GetControlMarginBottom (Control));
}

STATIC
COLORREF
GetMenuColor (
  VOID
  )
{
  HSV_VALUE                        Hsv;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL    Pixel;

  if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
    return 0xFF89E52C;
  }

  GetCurrentMenuHsv (&Hsv);
  if (Hsv.Hue == 0 && Hsv.Saturation == 0 && Hsv.Value == 0) {
    //
    // Use default color if current menu image doesn't exist.
    //
    return 0xFF834EAD;
  }
  HSV2RGB (&Hsv, &Pixel);

  return (0xFF000000 | (Pixel.Red << 16) | (Pixel.Green << 8) | Pixel.Blue);
}

STATIC
COLORREF
GetMenuLightColor (
  VOID
  )
{
  HSV_VALUE                        Hsv;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL    Pixel;

  if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
    return 0xFF89E52C;
  }

  GetCurrentMenuHsv (&Hsv);
  if (Hsv.Hue == 0 && Hsv.Saturation == 0 && Hsv.Value == 0) {
    //
    // Use default light color if current menu image doesn't exist.
    //
    return 0xFF9D72C0;
  }
  Hsv.Hue = Hsv.Hue <= 5 ? Hsv.Hue + 360 - 5 : Hsv.Hue - 5;
  Hsv.Saturation = Hsv.Saturation <= 3 ? 0 : Hsv.Saturation - 3;
  Hsv.Value = Hsv.Value >= 73 ? 100 : Hsv.Value + 27;
  HSV2RGB (&Hsv, &Pixel);
  return (0xFF000000 | (Pixel.Red << 16) | (Pixel.Green << 8) | Pixel.Blue);
}

COLORREF
EFIAPI
GetColorValue (
  CHAR16 *Value
  )
{
  EFI_STATUS Status;
  COLORREF   Color;

  while (*Value > L'\0' && *Value <= L' ') Value++;
  if (*Value == '#') Value++;
  if (*Value == L'@') {
    Value++;
    if (StrCmp (Value, L"menucolor") == 0) {
      return GetMenuColor ();
    } else if (StrCmp (Value, L"menulightcolor") == 0) {
      return GetMenuLightColor ();
    }
    ASSERT (FALSE);
  }

  Color = (COLORREF) StrToUInt (Value, 16, &Status);

  return Color;
}

VOID
AdjustImageToMenuColor (
  IN OUT UI_CONTROL             *Control
  )
{
  HSV_VALUE                     SrcHsv;
  HSV_VALUE                     DstHsv;
  UINT32                        Color;
  INT16                         HueDiff;
  INT8                          SaturationDiff;
  INT8                          ValueDiff;
  CHAR16                        Str[20];

  GetCurrentMenuHsv (&SrcHsv);

  Color = (UINT32)GetColorValue (L"@menucolor");
  Color |= 0xFF000000;
  RGB2HSV((EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)&Color, &DstHsv);

  if (DstHsv.Hue == 0 && DstHsv.Saturation == 0 && DstHsv.Value == 0) {
    return ;
  }
  HueDiff        = (INT16) (DstHsv.Hue - SrcHsv.Hue);
  SaturationDiff = (INT8)  (DstHsv.Saturation - SrcHsv.Saturation);
  ValueDiff      = (INT8)  (DstHsv.Value - SrcHsv.Value);
  UnicodeSPrint (Str, sizeof (Str), L"%d,%d,%d",  HueDiff, SaturationDiff, ValueDiff);
  UiSetAttribute (Control, L"hsvadjust", Str);
}

/**
 Convert to IFR numeric question flags value.

 @param[in] IsHexValue     Flag to determine that numeric question is hex or not
 @param[in] IsSignedValue  Flag to determine that numeric question is signed value or not
 @param[in] ValueSize      Numeric question value size in bytes

 @return Numeric question flags value
**/
UINT8
ConvertToIfrNumericFlags (
  IN BOOLEAN                       IsHexValue,
  IN BOOLEAN                       IsSignedValue,
  IN UINTN                         ValueSize
  )
{
  UINT8                            IfrNumericFlags;

  if (IsHexValue) {
    IfrNumericFlags = EFI_IFR_DISPLAY_UINT_HEX;
  } else {
    IfrNumericFlags = IsSignedValue ? EFI_IFR_DISPLAY_INT_DEC : EFI_IFR_DISPLAY_UINT_DEC;
  }

  switch (ValueSize) {

  case 1:
    IfrNumericFlags |= EFI_IFR_NUMERIC_SIZE_1;
    break;

  case 2:
    IfrNumericFlags |= EFI_IFR_NUMERIC_SIZE_2;
    break;

  case 4:
    IfrNumericFlags |= EFI_IFR_NUMERIC_SIZE_4;
    break;

  case 8:
  default:
    IfrNumericFlags |= EFI_IFR_NUMERIC_SIZE_8;
    break;
  }

  return IfrNumericFlags;
}

UI_CONTROL *
EFIAPI
ContainerCreateControl (
  UI_CONTROL *Container,
  CHAR16     *ControlName
  )
{
  UI_CONTROL *Control;

  Control = CreateControl (ControlName,  (UI_CONTROL *) Container);
  CONTROL_CLASS(Container)->AddChild (Container, Control);

  return Control;
}

VOID
EFIAPI
RegisterClassTable (
  UI_GET_CLASS                  *GetClassTable
  )
{
  UINT8                         Buf[sizeof (WNDCLASS) + sizeof (UI_CONTROL_CLASS *)];
  WNDCLASS                      *WndClass;
  UI_CONTROL_CLASS              *ControlClass;
  UI_GET_CLASS                  *GetClass;

  WndClass                      = (WNDCLASS *) Buf;
  WndClass->style               = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_GLOBALCLASS;
  WndClass->lpfnWndProc         = (WNDPROC) NULL;
  WndClass->cbClsExtra          = sizeof (UI_CONTROL_CLASS *);
  WndClass->cbWndExtra          = sizeof (UI_CONTROL *);
  WndClass->hInstance           = NULL;
  WndClass->hIcon               = NULL;
  WndClass->hCursor             = 0;
  WndClass->hbrBackground       = GetStockObject(NULL_BRUSH);
  WndClass->lpszMenuName        = NULL;
  WndClass->lpszClassName       = NULL;


  GetClass = GetClassTable;

  while ((*GetClass) != NULL) {
    ControlClass = (*GetClass)();
    ASSERT (ControlClass != NULL);
    if (ControlClass == NULL) {
      continue;
    }

    WndClass->lpfnWndProc       = ControlClass->WndProc;
    WndClass->lpszClassName     = ControlClass->ClassName;

    *(UI_CONTROL_CLASS **)(Buf + sizeof (WNDCLASS))   = ControlClass;
    RegisterClass(WndClass);

    GetClass++;
  }
}

CHAR16 *
StrDuplicate (
  IN CONST CHAR16  *Src
  )
{
  return AllocateCopyPool (StrSize (Src), Src);
}

EFI_STATUS
EFIAPI
InitUiClass (
  UI_CONTROL_CLASS              **Class,
  UINT32                        ClassSize,
  CHAR16                        *ClassName,
  UI_CONTROL_CLASS              *ParentClass
  )
{
  ASSERT (ParentClass != NULL);
  if (ParentClass == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Class = AllocateZeroPool (ClassSize);
  ASSERT (*Class != NULL);
  if (*Class == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (*Class, ParentClass, ParentClass->ClassSize);

  (*Class)->Parent     = ParentClass;
  (*Class)->ClassSize  = ClassSize;
  (*Class)->ClassName  = StrDuplicate (ClassName);

  return EFI_SUCCESS;
}

VOID
UiNeedUpdatePos (
  VOID                          *This
  )
{
  UI_CONTROL                    *Control;
  UI_CONTROL                    *Parent;

  Control = (UI_CONTROL *)This;

  Control->NeedUpdateChildPos = TRUE;
  CONTROL_CLASS (Control)->Invalidate (Control);

  Parent = CONTROL_CLASS(Control)->GetParent(Control);
  if (Parent != NULL) {
    CONTROL_CLASS (Control)->Invalidate (Control);
  }
  while (Parent != NULL) {
    Parent->NeedUpdateChildPos = TRUE;
    Parent = CONTROL_CLASS (Parent)->GetParent(Parent);
  }
}

VOID
XmlCreateControlStartCallback (
  VOID          *Data,
  CHAR16        *ElementName,
  CHAR16        **AttrNames,
  CHAR16        **AttrValues
  )
{
  HWND          ControlWnd;
  UI_CONTROL    *ParentControl;
  UI_CONTROL    *Control;
  XML_UI_DATA   *XmlUiData;

  XmlUiData    = (XML_UI_DATA *) Data;
  ParentControl = XmlUiData->Parents[XmlUiData->Count - 1];

  ControlWnd = CreateWindowEx (
                 WS_EX_NOACTIVATE,
                 ElementName,
                 L"",
                 WS_CHILD | WS_VISIBLE,
                 0,
                 0,
                 0,
                 0,
                 ParentControl->Wnd,
                 0,
                 XmlUiData->Instance,
                 XmlUiData->Manager
                 );
  ASSERT (ControlWnd != NULL);
  if (ControlWnd == NULL) {
    return ;
  }

  Control = (UI_CONTROL *) GetWindowLongPtr (ControlWnd, 0);
  if (XmlUiData->FirstControl == NULL) {
    XmlUiData->FirstControl = Control;
  }

  CONTROL_CLASS(ParentControl)->AddChild (ParentControl, Control);

  XmlUiData->Parents[XmlUiData->Count]     = Control;
  XmlUiData->ParentsName[XmlUiData->Count] = ElementName;
  ++XmlUiData->Count;

  while (*AttrNames != NULL) {
    CONTROL_CLASS(Control)->SetAttribute (Control, *AttrNames, *AttrValues);
    AttrNames++;
    AttrValues++;
  }

}

VOID
XmlCreateControlEndCallback (
  VOID        *Data,
  CHAR16      *ElementName
  )
{
  XML_UI_DATA *XmlUiData;

  XmlUiData = (XML_UI_DATA *) Data;

  ASSERT (XmlUiData->Count > 1);
  if (XmlUiData->Count <= 1) {
    return ;
  }

  if (StrCmp (ElementName, XmlUiData->ParentsName[XmlUiData->Count - 1]) == 0) {
    XmlUiData->ParentsName[XmlUiData->Count - 1] = NULL;
    --XmlUiData->Count;
  }
}

UI_CONTROL *
EFIAPI
XmlCreateControl (
  CHAR16     *XMLBuffer,
  UI_CONTROL *Parent
  )
{
  XML_UI_DATA XmlUiData;
  CHAR16      *Buf;

  ASSERT (Parent != NULL);

  ZeroMem (&XmlUiData, sizeof (XmlUiData));

  XmlUiData.Count        = 1;
  XmlUiData.Parents[0]   = Parent;
  XmlUiData.Manager      = Parent->Manager;
  XmlUiData.Instance     = Parent->Manager->Instance;
  XmlUiData.FirstControl = NULL;

  Buf = StrDuplicate (XMLBuffer);
  if (Buf != NULL) {
    SaxParser (
      Buf,
      XmlCreateControlStartCallback,
      NULL,
      XmlCreateControlEndCallback,
      &XmlUiData
      );

    FreePool (Buf);
  }

  return XmlUiData.FirstControl;
}

