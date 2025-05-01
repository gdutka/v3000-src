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

STATIC UI_EDIT_CLASS           *mEditClass = NULL;
#define CURRENT_CLASS           mEditClass

#define CARET_WIDTH             2
#define CARET_TIMER_ID          2
#define CARET_TIMER_INTERVAL    500

CHAR16 *mEditChilds = L""
  L"<Control float='true' name='Caret' visibility='false'>"
   L"<Control name='CaretImage' background-color='0xFF000000'/>"
  L"</Control>";

VOID
UiEditCreate (
  UI_EDIT *This
  )
{
  UI_CONTROL *Control;
  SIZE       Size;

  Control = (UI_CONTROL *) This;

  GetTextExtentPoint32 (Control->Manager->PaintDC, L"X", 1, &Size);
  This->CharHeight  = Size.cy;
  This->EditPos     = 0;
  This->StartPos    = 0;
  This->IsPainted   = FALSE;
  This->IsCaretShown= FALSE;
  This->MinValue    = 0;
  This->MaxValue    = 0;
  This->Step        = 0;
  This->ValueType   = NOT_VALUE;
  This->ValueSize   = 8;
}

VOID
UiEditSetPasswordMode (
  UI_EDIT *This,
  BOOLEAN PasswordMode
  )
{
  if (This->IsPasswordMode != PasswordMode) {
    This->IsPasswordMode = PasswordMode;
    CONTROL_CLASS(This)->Invalidate ((UI_CONTROL *)This);
  }
}

VOID
UiEditSetReadOnly (
  UI_EDIT *This,
  BOOLEAN ReadOnly
  )
{
  if (This->IsReadOnly != ReadOnly) {
    This->IsReadOnly = ReadOnly;
    CONTROL_CLASS(This)->Invalidate ((UI_CONTROL *)This);
  }
}


SIZE
EFIAPI
UiEditEstimateSize (
  UI_CONTROL *Control,
  SIZE       AvailableSize
  )
{

  RECT              TextRc;
  UI_EDIT           *This;
  SIZE              Size;
  UI_LABEL          *Label;
  UINT32            Style;


  This = (UI_EDIT *) Control;
  Label = (UI_LABEL *) Control;

  if (IS_CONTROL_HEIGHT_DEFINED(Control)) {
    AvailableSize.cx = MIN (AvailableSize.cx, CalculateControlDisplayWidth (Control, AvailableSize));
  }

  if (Label->Text == NULL || *Label->Text == '\0') {
    Size.cx = CalculateControlDisplayWidth (Control, AvailableSize);
    Size.cy = GetControlFontSize (Control);
    return Size;
  }

  SetRect (&TextRc, 0, 0, AvailableSize.cx, 9999);
//TextRc.left  += This->TextPadding.left;
//TextRc.right -= This->TextPadding.right;

  Style = DT_WORDBREAK | DT_CALCRECT;
  if (Label->ShowHtmlText) {
    Style |= DT_HTML_TEXT;
  }

  UiPaintText (
    Control->Manager->PaintDC,
    GetControlFontSize (Control),
    &TextRc,
    Label->Text,
    INVALID_COLOR,
    INVALID_COLOR,
    Style
    );

  Size.cx = TextRc.right - TextRc.left; // + This->TextPadding.left + This->TextPadding.right;
  Size.cy = TextRc.bottom - TextRc.top; // + This->TextPadding.top + This->TextPadding.bottom;

  if (IS_CONTROL_HEIGHT_DEFINED (Control)) {
    Size.cy = CalculateControlDisplayHeight (Control, AvailableSize);
  }

  if (IS_CONTROL_WIDTH_DEFINED (Control)) {
    Size.cx = CalculateControlDisplayWidth (Control, AvailableSize);
  }

  return Size;
}



BOOLEAN
EFIAPI
UiEditSetAttribute (
  UI_CONTROL  *Control,
  CHAR16      *Name,
  CHAR16      *Value
  )
{
  UI_EDIT     *This;
  EFI_STATUS  Status;

  This = (UI_EDIT *) Control;

  if (StrCmp (Name, L"valuetype") == 0) {
    if (StrCmp (Value, L"dec") == 0) {
      This->ValueType = DEC_VALUE;
    } else if (StrCmp (Value, L"hex") == 0) {
      This->ValueType = HEX_VALUE;
    } else if (StrCmp (Value, L"signeddec") == 0) {
      This->ValueType = SIGNED_DEC_VALUE;
    } else {
      This->ValueType = NOT_VALUE;
    }
  } else if (StrCmp (Name, L"password") == 0) {
    UiEditSetPasswordMode (This, (StrCmp (Value, L"true") == 0));
  } else if (StrCmp (Name, L"readonly") == 0) {
    UiEditSetReadOnly (This, (StrCmp (Value, L"true") == 0));
  } else if (StrCmp (Name, L"maxlength") == 0) {
    This->MaxLength = (UINT32) StrToUInt (Value, 10, &Status);
  } else if (StrCmp (Name, L"maxvalue") == 0) {
    This->MaxValue = StrToUInt (Value, 10, &Status);
  } else if (StrCmp (Name, L"minvalue") == 0) {
    This->MinValue = StrToUInt (Value, 10, &Status);
  } else if (StrCmp (Name, L"valuesize") == 0) {
    This->ValueSize = (UINT8)StrToUInt (Value, 10, &Status);
  } else {
    return PARENT_CLASS_SET_ATTRIBUTE (CURRENT_CLASS, Control, Name, Value);
  }

  return TRUE;
}

/**
 Create an allocated buffer for display string. If password mode is on, all characters will be replaced by '*'.

 @param[in] This          Pointer to UI_EDIT instance

 @return A pointer to the allocated buffer or NULL if allocation fails.
**/
STATIC
CHAR16 *
UiEditCreateDisplayStr (
  IN UI_EDIT                *This
  )
{
  UI_LABEL                  *Label;
  CHAR16                    *DisplayStr;
  UINTN                     StringLen;
  UINTN                     Index;

  Label = (UI_LABEL *) This;

  StringLen  = StrLen (Label->Text);
  DisplayStr = AllocateCopyPool ((StringLen + 1) * sizeof(CHAR16), Label->Text);
  if (DisplayStr == NULL) {
    return NULL;
  }
  if (This->IsPasswordMode) {
    for (Index = 0; Index < StringLen; Index++) {
      DisplayStr[Index] = L'*';
    }
  }
  return DisplayStr;
}

/**
 Get the available display region for the text of UI_EDIT

 @param[in]  This           Pointer to UI_EDIT instance
 @param[out] Rect           Pointer to available display region
**/
STATIC
VOID
UiEditGetTextRect (
  IN  UI_EDIT               *This,
  OUT RECT                  *Rect
  )
{
  UI_CONTROL                *Control;

  Control     = (UI_CONTROL *) This;

  Control->Manager->GetControlRect (Control->Manager, Control, Rect);
  Rect->left   += (GetControlPaddingLeft (Control)   + GetControlBorderLeft (Control)  );
  Rect->top    += (GetControlPaddingTop (Control)    + GetControlBorderTop (Control)   );
  Rect->right  -= (GetControlPaddingRight (Control)  + GetControlBorderRight (Control) );
  Rect->bottom -= (GetControlPaddingBottom (Control) + GetControlBorderBottom (Control));
}

/**
 Based on input start position, calculate the start position of next line.

 @param[in] This          Pointer to UI_EDIT instance
 @param[in] StartPos      Start position of current line

 @return The start position of next line or -1 if not found or failed to create display string.
**/
STATIC
UINT32
UiEditGetNextStartPos (
  IN UI_EDIT                *This,
  IN UINT32                 StartPos
  )
{
  UI_CONTROL                *Control;
  UI_LABEL                  *Label;
  RECT                      Rect;
  RECT                      CalculatedRect;
  UINT32                    TextStyle;
  UINT32                    Index;
  CHAR16                    *DisplayString;
  UINTN                     DisplayStringLen;
  CHAR16                    Char;
  INT32                     Height;
  UINT32                    NewStartPos;

  Control     = (UI_CONTROL *) This;
  Label       = (UI_LABEL *) Control;
  NewStartPos = (UINT32) -1;

  DisplayString = UiEditCreateDisplayStr (This);
  if (DisplayString == NULL) {
    return NewStartPos;
  }
  DisplayStringLen = StrLen (DisplayString);

  Control->Manager->PaintDC->font->FontSize = GetControlFontSize (Control);
  TextStyle = Label->TextStyle | DT_CALCRECT | DT_WORDBREAK;
  if (Label->ShowHtmlText) {
    TextStyle |= DT_HTML_TEXT;
  }

  UiEditGetTextRect (This, &Rect);
  OffsetRect (&Rect, -Rect.left, -Rect.top);
  Rect.bottom = MAX_INT32;

  for (Index = StartPos + 1, Height = 0; Index <= DisplayStringLen; Index++) {
    CopyRect (&CalculatedRect, &Rect);

    Char = DisplayString[Index];
    DisplayString[Index] = L'\0';
    UiPaintText (
      Control->Manager->PaintDC,
      GetControlFontSize (Control),
      &CalculatedRect,
      DisplayString + StartPos,
      Control->TextColor,
      INVALID_COLOR,
      TextStyle
      );
    DisplayString[Index] = Char;

    if (Index == StartPos + 1) {
      Height = CalculatedRect.bottom;
      continue;
    }
    if (CalculatedRect.bottom > Height) {
      NewStartPos = Index - 1;
      break;
    }
  }

  FreePool (DisplayString);
  return NewStartPos;
}

/**
 Based on input start position, calculate the start position of previous line.

 @param[in] This          Pointer to UI_EDIT instance
 @param[in] StartPos      Start position of current line

 @return The start position of previous line or -1 if not found or failed to create display string.
**/
STATIC
UINT32
UiEditGetPreviousStartPos (
  IN UI_EDIT                *This,
  IN UINT32                 StartPos
  )
{
  UI_CONTROL                *Control;
  UI_LABEL                  *Label;
  RECT                      Rect;
  RECT                      CalculatedRect;
  UINT32                    TextStyle;
  UINT32                    Index;
  CHAR16                    *DisplayString;
  UINTN                     DisplayStringLen;
  INT32                     Height;
  UINT32                    NewStartPos;

  Control = (UI_CONTROL *) This;
  Label   = (UI_LABEL *) Control;
  NewStartPos = (UINT32) -1;

  if (StartPos == 0) {
    return NewStartPos;
  }

  DisplayString = UiEditCreateDisplayStr (This);
  if (DisplayString == NULL) {
    return NewStartPos;
  }
  DisplayStringLen = StrLen (DisplayString);

  Control->Manager->PaintDC->font->FontSize = GetControlFontSize (Control);
  TextStyle = Label->TextStyle | DT_CALCRECT | DT_WORDBREAK;
  if (Label->ShowHtmlText) {
    TextStyle |= DT_HTML_TEXT;
  }

  UiEditGetTextRect (This, &Rect);
  OffsetRect (&Rect, -Rect.left, -Rect.top);
  Rect.bottom = MAX_INT32;

  for (Index = StartPos, Height = 0; Index > 0; Index--) {
    CopyRect (&CalculatedRect, &Rect);

    DisplayString[Index] = L'\0';
    UiPaintText (
      Control->Manager->PaintDC,
      GetControlFontSize (Control),
      &CalculatedRect,
      DisplayString,
      Control->TextColor,
      INVALID_COLOR,
      TextStyle
      );

    if (Index == StartPos) {
      Height = CalculatedRect.bottom;
      continue;
    }
    if (CalculatedRect.bottom != Height) {
      break;
    }
  }
  NewStartPos = Index;

  FreePool (DisplayString);
  return NewStartPos;
}

/**
 Based on current edit position, recalcluate the start position of multi-line edit box.

 @param[in] This                Pointer to UI_EDIT instance

 @retval  EFI_SUCCESS           Recalcluate the start position of edit box successfully.
 @retval  EFI_OUT_OF_RESOURCES  Failed to create display string.
**/
STATIC
EFI_STATUS
UiEditReCalculateStartPosMultiLine (
  IN UI_EDIT                *This
  )
{
  UI_CONTROL                *Control;
  UI_LABEL                  *Label;
  CHAR16                    *DisplayString;
  UINTN                     DisplayStringLen;
  RECT                      Rect;
  RECT                      CalculatedRect;
  UINT32                    TextStyle;
  CHAR16                    Char;
  INT32                     AvailableHeight;
  UINT32                    StartPos;

  Control = (UI_CONTROL *) This;
  Label   = (UI_LABEL *) Control;

  DisplayString = UiEditCreateDisplayStr (This);
  if (DisplayString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  DisplayStringLen = StrLen (DisplayString);

  Control->Manager->PaintDC->font->FontSize = GetControlFontSize (Control);
  TextStyle = Label->TextStyle | DT_CALCRECT | DT_WORDBREAK;
  if (Label->ShowHtmlText) {
    TextStyle |= DT_HTML_TEXT;
  }

  UiEditGetTextRect (This, &Rect);
  OffsetRect (&Rect, -Rect.left, -Rect.top);
  AvailableHeight = Rect.bottom;
  Rect.bottom = MAX_INT32;

  if (This->StartPos > This->EditPos) {
    for (StartPos = UiEditGetPreviousStartPos (This, This->StartPos);
         StartPos != (UINT32)-1;
         StartPos = UiEditGetPreviousStartPos (This, StartPos)) {
      if (StartPos <= This->EditPos) {
        This->StartPos = StartPos;
        break;
      }
    }
  } else {
    //
    // Make sure edit position in the box
    //
    Char                         = DisplayString[This->EditPos];
    DisplayString[This->EditPos] = L'\0';
    for (StartPos = This->StartPos;
         StartPos < DisplayStringLen;
         StartPos = UiEditGetNextStartPos (This, StartPos)) {
      CopyRect (&CalculatedRect, &Rect);
      UiPaintText (
        Control->Manager->PaintDC,
        GetControlFontSize (Control),
        &CalculatedRect,
        DisplayString + StartPos,
        Control->TextColor,
        INVALID_COLOR,
        TextStyle
        );
      if (CalculatedRect.bottom <= AvailableHeight) {
        This->StartPos = StartPos;
        break;
      }
    }
    DisplayString[This->EditPos] = Char;

    //
    // If there is room in the end, scroll up StartPos until box is full.
    //
    for (StartPos = UiEditGetPreviousStartPos (This, This->StartPos);
         StartPos < DisplayStringLen;
         StartPos = UiEditGetPreviousStartPos (This, StartPos)) {
      CopyRect (&CalculatedRect, &Rect);
      UiPaintText (
        Control->Manager->PaintDC,
        GetControlFontSize (Control),
        &CalculatedRect,
        DisplayString + StartPos,
        Control->TextColor,
        INVALID_COLOR,
        TextStyle
        );
      if (CalculatedRect.bottom > AvailableHeight) {
        break;
      }
      This->StartPos = StartPos;
    }
  }

  FreePool (DisplayString);
  return EFI_SUCCESS;
}

/**
 Based on current edit position, recalcluate the start position of single-line edit box.

 @param[in] This                Pointer to UI_EDIT instance

 @retval  EFI_SUCCESS           Recalcluate the start position of edit box successfully.
 @retval  EFI_OUT_OF_RESOURCES  Failed to create display string.
**/
STATIC
EFI_STATUS
UiEditReCalculateStartPosSingleLine (
  IN UI_EDIT                *This
  )
{
  UI_CONTROL                *Control;
  UI_LABEL                  *Label;
  INT32                     AvailableWidth;
  RECT                      Rect;
  RECT                      CalculatedRect;
  UINT32                    TextStyle;
  UINT32                    Index;
  CHAR16                    *DisplayString;

  Control = (UI_CONTROL *) This;
  Label   = (UI_LABEL *) Control;

  DisplayString = UiEditCreateDisplayStr (This);
  if (DisplayString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Control->Manager->PaintDC->font->FontSize = GetControlFontSize (Control);
  TextStyle = Label->TextStyle | DT_CALCRECT;
  if (Label->ShowHtmlText) {
    TextStyle |= DT_HTML_TEXT;
  }

  UiEditGetTextRect (This, &Rect);
  OffsetRect (&Rect, -Rect.left, -Rect.top);
  AvailableWidth = Rect.right;
  Rect.right = MAX_INT32;

  DisplayString[This->EditPos] = L'\0';
  for (Index = This->StartPos; Index < This->EditPos; Index++) {
    CopyRect (&CalculatedRect, &Rect);
    UiPaintText (
      Control->Manager->PaintDC,
      GetControlFontSize (Control),
      &CalculatedRect,
      DisplayString + Index,
      Control->TextColor,
      INVALID_COLOR,
      TextStyle
      );
    if (CalculatedRect.right + CARET_WIDTH <= AvailableWidth) {
      break;
    }
  }
  This->StartPos = Index;

  FreePool (DisplayString);
  return EFI_SUCCESS;
}

/**
 Recalcluate the start position of edit box.

 @param[in] This                Pointer to UI_EDIT instance

 @retval  EFI_SUCCESS           Recalcluate the start position of edit box successfully.
 @retval  EFI_NOT_READY         Paint device context is not ready.
**/
STATIC
EFI_STATUS
UiEditReCalculateStartPos (
  IN UI_EDIT                    *This
  )
{
  if (((UI_CONTROL *)This)->Manager->PaintDC == NULL) {
    return EFI_NOT_READY;
  }

  if (StrLen (((UI_LABEL *)This)->Text) == 0) {
    This->StartPos = 0;
    return EFI_SUCCESS;
  }

  if ((((UI_LABEL *)This)->TextStyle & DT_SINGLELINE) == 0) {
    return UiEditReCalculateStartPosMultiLine (This);
  } else {
    return UiEditReCalculateStartPosSingleLine (This);
  }
}

/**
 Set caret UI attributes.

 @param[in] This                Pointer to UI_EDIT instance
**/
VOID
UiEditSetCaretAttr (
  IN UI_EDIT                    *This
  )
{
  UI_CONTROL                    *Control;
  UI_LABEL                      *Label;
  UI_CONTROL                    *Child;
  CHAR16                        Str[20];

  Control = (UI_CONTROL *) This;
  Label   = (UI_LABEL *) Control;

  Child = UiFindChildByName (Control, L"CaretImage");
  if (Child == NULL) {
    return;
  }
  UnicodeSPrint (Str, sizeof (Str), L"%d", CARET_WIDTH);
  UiSetAttribute (Child, L"width", Str);

  UnicodeSPrint (Str, sizeof (Str), L"%d", GetControlFontSize (Control));
  UiSetAttribute (Child, L"height", Str);

  if (Label->FocusTextColor != 0) {
    UnicodeSPrint (Str, sizeof (Str), L"0x%08x",  Label->FocusTextColor);
    UiSetAttribute (Child, L"background-color", Str);
  }
}

/**
 Set caret visibility attribute.

 @param[in] This                Pointer to UI_EDIT instance
 @param[in] Visibility          Visibility flag of caret
**/
STATIC
VOID
UiEditSetCaretVisibility (
  IN UI_EDIT                *This,
  IN BOOLEAN                Visibility
  )
{
  UI_CONTROL                *Child;

  Child = UiFindChildByName (This, L"Caret");
  if (Child == NULL) {
    return;
  }
  UiSetAttribute (Child, L"visibility", Visibility ? L"true" : L"false");
  CONTROL_CLASS(Child)->Invalidate (Child);

  This->IsCaretShown = Visibility;
}

/**
 Based on current edit position, recalcluate the caret position.

 @param[in] This                Pointer to UI_EDIT instance

 @retval  EFI_SUCCESS           Recalcluate the caret position successfully.
 @retval  EFI_NOT_READY         Paint device context is not ready.
 @retval  EFI_OUT_OF_RESOURCES  Failed to create display string.
**/
STATIC
EFI_STATUS
UiEditReCalculateCaretPos (
  IN UI_EDIT                *This
  )
{
  UI_CONTROL                *Control;
  UI_LABEL                  *Label;
  RECT                      Rect;
  RECT                      CalculatedRect;
  UINT32                    TextStyle;
  CHAR16                    *DisplayString;
  UI_CONTROL                *Child;
  CHAR16                    Str[20];
  INT32                     X;
  INT32                     Y;

  Control = (UI_CONTROL *) This;
  Label   = (UI_LABEL *) Control;

  if (Control->Manager->PaintDC == NULL) {
    return EFI_NOT_READY;
  }

  X = 0;
  Y = 0;
  if (This->EditPos == 0 || This->EditPos <= This->StartPos) {
    goto Done;
  }

  DisplayString = UiEditCreateDisplayStr (This);
  if (DisplayString == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Control->Manager->PaintDC->font->FontSize = GetControlFontSize (Control);
  TextStyle = Label->TextStyle | DT_CALCRECT;
  if (Label->ShowHtmlText) {
    TextStyle |= DT_HTML_TEXT;
  }
  if ((Label->TextStyle & DT_SINGLELINE) == 0) {
    TextStyle |= DT_WORDBREAK;
  }

  UiEditGetTextRect (This, &Rect);
  OffsetRect (&Rect, -Rect.left, -Rect.top);
  if ((Label->TextStyle & DT_SINGLELINE) == 0) {
    Rect.bottom = MAX_INT32;
  } else {
    Rect.right = MAX_INT32;
  }

  DisplayString[This->EditPos] = L'\0';
  CopyRect (&CalculatedRect, &Rect);
  UiPaintText (
    Control->Manager->PaintDC,
    GetControlFontSize (Control),
    &CalculatedRect,
    DisplayString + This->StartPos,
    Control->TextColor,
    INVALID_COLOR,
    TextStyle
    );

  X = CalculatedRect.right;
  if (X + CARET_WIDTH > Rect.right) {
    X = Rect.right - CARET_WIDTH;
  }
  if ((Label->TextStyle & DT_SINGLELINE) == 0) {
    Y = CalculatedRect.bottom;

    SetRect (&CalculatedRect, 0, 0, MAX_INT32, MAX_INT32);
    UiPaintText (
      Control->Manager->PaintDC,
      GetControlFontSize (Control),
      &CalculatedRect,
      L"X",
      Control->TextColor,
      INVALID_COLOR,
      TextStyle
      );
    Y -= CalculatedRect.bottom;
  } else {
    Y = 0;
  }

  FreePool (DisplayString);

Done:
  Child = UiFindChildByName (This, L"Caret");
  if (Child != NULL) {
    UnicodeSPrint (Str, sizeof (Str), L"%d,0,0,%d", Y, X);
    UiSetAttribute (Child, L"padding", Str);
  }

  //
  // Always change to show caret after set position is updated.
  //
  UiEditSetCaretVisibility (This, TRUE);
  KillTimer (Control->Wnd, CARET_TIMER_ID);
  SetTimer (Control->Wnd, CARET_TIMER_ID, CARET_TIMER_INTERVAL, NULL);
  return EFI_SUCCESS;
}

VOID
UiEditKeyDown (
  HWND Hwnd,
  WPARAM WParam,
  LPARAM LParam
  )
{

  UI_CONTROL *Control;
  UI_EDIT    *Edit;
  BOOLEAN    ReDraw;
  UINTN      LastEditPos;
  SIZE       Size;
  UI_MANAGER *Manager;
  UI_LABEL   *Label;

  Control = GetUiControl (Hwnd);
  Edit    = (UI_EDIT *) Control;
  Label   = (UI_LABEL *) Control;
  LastEditPos = Edit->EditPos;
  Manager = Control->Manager;


  ReDraw  = FALSE;

  switch (WParam) {

  case VK_LEFT:
    if (Edit->EditPos > 0) {
      Edit->EditPos--;

      if ((Label->TextStyle & DT_SINGLELINE) == 0) {
        UiEditReCalculateStartPos (Edit);
      } else {
        if (Edit->EditPos == Edit->StartPos &&
            Edit->EditPos != 0) {
          Edit->StartPos--;
        } else if (Edit->EditPos < Edit->StartPos) {
          Edit->StartPos = 0;
        }
      }
    }
    break;

  case VK_RIGHT:
    if (Edit->EditPos < StrLen (Label->Text)) {
      Edit->EditPos++;

      UiEditReCalculateStartPos (Edit);
    }
    break;

  case VK_HOME:
    Edit->EditPos = 0;
    Edit->StartPos = 0;
    break;

  case VK_END:
    Edit->EditPos = (UINT32) StrLen (Label->Text);

    UiEditReCalculateStartPos (Edit);
    break;


  case VK_BACK:
    if (Edit->EditPos > 0) {
      Edit->EditPos--;
      memmove (Label->Text + Edit->EditPos,
         Label->Text + Edit->EditPos + 1,
         (StrLen (Label->Text) - Edit->EditPos + 1) * sizeof (CHAR16));
      ReDraw = TRUE;

      if ((Label->TextStyle & DT_SINGLELINE) == 0) {
        UiEditReCalculateStartPos (Edit);
      } else {
        if (Edit->StartPos != 0) {
          Edit->StartPos--;
        }
      }
    }
    break;

  case VK_DELETE:
    if (Edit->EditPos < StrLen (Label->Text)) {
      memmove (Label->Text + Edit->EditPos,
         Label->Text + Edit->EditPos + 1,
         (StrLen (Label->Text) - Edit->EditPos + 1) * sizeof (CHAR16));
      ReDraw = TRUE;
    }
    break;
  }

  if (LastEditPos != Edit->EditPos || ReDraw) {
    UiEditReCalculateCaretPos (Edit);
    if (Edit->EditPos != 0) {
      GetTextExtentPoint32 (Manager->PaintDC, Label->Text, (UINT32)Edit->EditPos, &Size);
    }
    CONTROL_CLASS(Edit)->Invalidate (Control);
    if (ReDraw) {
      SendMessage (Control->Manager->MainWnd, UI_NOTIFY_CHANGE, (WPARAM)Control, 0);
    }
  }
}

VOID
UiEditCharPress (
  HWND     Hwnd,
  WPARAM   WParam,
  LPARAM   LParam
  )
{
  CHAR16     Char;
  CHAR16     *OldStr;
  CHAR16     *NewStr;
  UI_EDIT    *This;
  SIZE       Size;
  UI_CONTROL *Control;
  UI_LABEL   *Label;
  UINT8      NumericFlags;


  This    = (UI_EDIT *) GetUiControl (Hwnd);
  Control = (UI_CONTROL *)This;
  Label   = (UI_LABEL *) This;

  ASSERT (Label->Text != NULL);
  if (Label->Text == NULL) {
    return ;
  }

  if (This->IsReadOnly) {
    return ;
  }

  Char = LOWORD (WParam);

  switch (Char) {
    case 0x00:  /* NULL */
    case 0x07:  /* BEL */
    case 0x08:  /* BS */
    case 0x09:  /* HT */
    case 0x0A:  /* LF */
    case 0x0B:  /* VT */
    case 0x0C:  /* FF */
    case 0x1B:  /* Escape */
      return ;
    case 0x0D:  /* enter */
      if (mSetupMouse != NULL) {
        mSetupMouse->CloseKeyboard (mSetupMouse);
      }
      return ;
  }

  if (This->MaxLength != 0 && StrLen (Label->Text) >= This->MaxLength) {
    if (FeaturePcdGet (PcdH2OBdsCpFormBrowserInputPasswordTooLongSupported)) {
      if (This->IsPasswordMode) {
        H2O_BDS_CP_FORM_BROWSER_INPUT_PASSWORD_TOO_LONG_DATA  FormBrowserInputPaswwordTooLong;

        FormBrowserInputPaswwordTooLong.Size    = sizeof (H2O_BDS_CP_FORM_BROWSER_INPUT_PASSWORD_TOO_LONG_DATA);
        FormBrowserInputPaswwordTooLong.Status  = H2O_CP_TASK_NORMAL;

        DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBdsCpFormBrowserInputPasswordTooLongGuid));
        H2OCpTrigger (&gH2OBdsCpFormBrowserInputPasswordTooLongGuid, &FormBrowserInputPaswwordTooLong);
        DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", FormBrowserInputPaswwordTooLong.Status));
      }
    }
    return;
  }

  //
  // insert data
  //
  OldStr = Label->Text;
  NewStr = AllocatePool (StrSize (OldStr) + sizeof (CHAR16));
  if (NewStr == NULL) {
    return;
  }
  if (This->EditPos != 0) {
    CopyMem (NewStr, OldStr, This->EditPos * sizeof (CHAR16));
  }

  NewStr[This->EditPos] = Char;

  CopyMem (NewStr + This->EditPos + 1,
        OldStr + This->EditPos,
        (StrLen (OldStr) - This->EditPos + 1) * sizeof (CHAR16));


  if (This->ValueType == DEC_VALUE || This->ValueType == HEX_VALUE || This->ValueType == SIGNED_DEC_VALUE) {
    NumericFlags = ConvertToIfrNumericFlags (This->ValueType == HEX_VALUE, This->ValueType == SIGNED_DEC_VALUE, This->ValueSize);
    if (!IfrNumericIsEditStrValid (NumericFlags, NewStr, This->MinValue, This->MaxValue)) {
      FreePool (NewStr);
      return ;
    }
  }

    FreePool (OldStr);
  Label->Text = NewStr;
  This->EditPos++;

  UiEditReCalculateStartPos (This);
  UiEditReCalculateCaretPos (This);
  GetTextExtentPoint32 (Control->Manager->PaintDC, Label->Text, This->EditPos, &Size);
  CONTROL_CLASS(This)->Invalidate (Control);
  SendMessage (Control->Manager->MainWnd, UI_NOTIFY_CHANGE, (WPARAM)Control, 0);
  return ;
}

LRESULT
EFIAPI
UiEditProc (
  HWND   Hwnd,
  UINT32 Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  UI_EDIT      *This;
  UI_CONTROL   *Control;
  UI_MANAGER   *Manager;
//POINT        Point;
  RECT         Rc;
  SIZE         Size;
  LRESULT      Ret;
  CHAR16       *StrBuf;
  CHAR16       *TmpPtr;
  KEYBOARD_ATTRIBUTES           KeyboardAttributes;
  UI_LABEL                      *Label;


  Control   = (UI_CONTROL *) GetWindowLongPtr (Hwnd, 0);
  if (Control == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  This    = (UI_EDIT *) Control;
  Label    = (UI_LABEL *) Control;
  Manager = NULL;
  if (Control != NULL) {
    Manager = Control->Manager;
  }

  switch (Msg) {

  case WM_CREATE:
    This = (UI_EDIT *) AllocateZeroPool (sizeof (UI_EDIT));
    if (This != NULL) {
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    UiEditCreate (This);
    Control->Class              = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
    Control->BkColor     = 0xFFD0D0D0;
    Control->BorderColor = 0xFF808080;
    SetWindowLongPtr (
      Control->Wnd,
      GWL_EXSTYLE, GetWindowLongPtr (Control->Wnd, GWL_EXSTYLE) & ~WS_EX_NOACTIVATE
      );
    XmlCreateControl (mEditChilds, Control);
    break;

  case WM_SETFOCUS:
    if ((CONTROL_CLASS_GET_STATE (Control) & UISTATE_CAPTURED) != 0) {
      return 0;
    }
    CONTROL_CLASS_SET_STATE (Control, UISTATE_CAPTURED, 0);

    if (This->IsReadOnly) {
      return 0;
    }

    This->EditPos = (UINT32) StrLen (Label->Text);
    This->StartPos = 0;
    UiEditReCalculateStartPos (This);

    UiEditReCalculateCaretPos (This);
    UiEditSetCaretAttr (This);
    UiEditSetCaretVisibility (This, TRUE);
    KillTimer (Control->Wnd, CARET_TIMER_ID);
    SetTimer (Control->Wnd, CARET_TIMER_ID, CARET_TIMER_INTERVAL, NULL);

    GetTextExtentPoint32 (Control->Manager->PaintDC, Label->Text, This->EditPos, &Size);
    SendMessage (Manager->MainWnd, UI_NOTIFY_EDITFOCUS, (WPARAM)Control, 0);

    Control->BorderColor = 0xFF800000;
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);

  case WM_KILLFOCUS:
    CONTROL_CLASS_SET_STATE (Control, 0, UISTATE_CAPTURED);

    This->StartPos = 0;
    UiEditSetCaretVisibility (This, FALSE);
    KillTimer (Control->Wnd, CARET_TIMER_ID);

    SendMessage (Manager->MainWnd, UI_NOTIFY_BLUR, (WPARAM)Control, 0);
    Control->BorderColor = 0xFF808080;
    if (mSetupMouse != NULL) {
      mSetupMouse->CloseKeyboard (mSetupMouse);
    }
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);

  case WM_CHAR:
    UiEditCharPress (Hwnd, WParam, LParam);
    break;

  case WM_KEYDOWN:
    if (WParam == VK_RETURN) {
      SendMessage (Manager->MainWnd, UI_NOTIFY_CARRIAGE_RETURN, (WPARAM) Control, 0);
    } else {
      UiEditKeyDown (Hwnd, WParam, LParam);
    }
    break;

  case WM_LBUTTONDOWN:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
    GetWindowRect (Hwnd, &Rc);
    if ((GetWindowLongPtr (Hwnd, GWL_STYLE) & WS_DISABLED) == 0) {
      if (mSetupMouse != NULL) {
        mSetupMouse->GetKeyboardAttributes (mSetupMouse, &KeyboardAttributes);
        if (!KeyboardAttributes.IsStart) {
          mSetupMouse->StartKeyboard (mSetupMouse, Rc.left, Rc.bottom + 20);
        }
      }
    }
    break;

  case WM_DESTROY:
    if (mSetupMouse != NULL) {
      mSetupMouse->GetKeyboardAttributes (mSetupMouse, &KeyboardAttributes);
      if (KeyboardAttributes.IsStart) {
        mSetupMouse->CloseKeyboard (mSetupMouse);
      }
    }
    KillTimer (Control->Wnd, CARET_TIMER_ID);
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);

  case WM_NCHITTEST:
    return HTCLIENT;
    break;

  case UI_NOTIFY_PAINT:
    if (WParam == PAINT_TEXT) {
      //
      // Because control position is not determined in SETFOCUS event, calculate StartPos and CaretPos in first paint.
      //
      if (!This->IsPainted) {
        This->IsPainted = TRUE;
        if ((CONTROL_CLASS_GET_STATE (Control) & UISTATE_CAPTURED) != 0) {
          UiEditReCalculateStartPos (This);
          UiEditReCalculateCaretPos (This);
          UiEditSetCaretAttr (This);
        }
      }

      TmpPtr = Label->Text;
      StrBuf = UiEditCreateDisplayStr (This);
      if (StrBuf != NULL) {
        Label->Text = StrBuf + This->StartPos;
      }
      Ret =  PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
      Label->Text = TmpPtr;
      if (StrBuf != NULL) {
        FreePool (StrBuf);
      }
      return Ret;
    }
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);

  case WM_TIMER:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
    if (WParam == CARET_TIMER_ID) {
      UiEditSetCaretVisibility (This, !This->IsCaretShown);
    }
    break;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

UI_EDIT_CLASS *
EFIAPI
GetEditClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"UiEdit", (UI_CONTROL_CLASS *)GetLabelClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = UiEditProc;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetAttribute = UiEditSetAttribute;
  //((UI_CONTROL_CLASS *)CURRENT_CLASS)->EstimateSize = UiEditEstimateSize;

  return CURRENT_CLASS;
}

