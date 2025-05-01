/** @file
  UI Common Controls

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

#include "UiControls.h"

STATIC UI_HORIZONTAL_LAYOUT_CLASS   *mHorizontalLayoutClass = NULL;
#define CURRENT_CLASS               mHorizontalLayoutClass

SIZE
UiHorizontalEstimateSize (
  UI_CONTROL                    *Control,
  SIZE                          AvailableSize
  )
{
  SIZE                          Size;
  SIZE                          ItemSize;
  SIZE                          ItemAvailableSize;
  UINTN                         Index;
  UI_CONTROL                    *Item;
  UINTN                         Count;
  INTN                          MaxHeight;
  INTN                          Width;
  INT32                         PreviousItemMarginRight;

  if (Control->Width.Type != UI_LENGTH_TYPE_WRAP_CONTENT &&
      Control->Height.Type != UI_LENGTH_TYPE_WRAP_CONTENT) {
    return CalculateControlDisplaySize (Control, AvailableSize);
  }

  Count = Control->ItemCount;

  ItemAvailableSize.cx = AvailableSize.cx - GetControlPaddingWidth (Control) - GetControlBorderWidth (Control);
  ItemAvailableSize.cy = AvailableSize.cy - GetControlPaddingHeight (Control) - GetControlBorderHeight (Control);

  if (IS_CONTROL_HEIGHT_VALUE_ASSIGNED (Control)) {
    ItemAvailableSize.cy = CalculateControlDisplayHeight (Control, AvailableSize) - GetControlPaddingHeight (Control) - GetControlBorderHeight (Control);
  }

  MaxHeight = 0;
  Width = 0;
  PreviousItemMarginRight = 0;
  for (Index = 0; Index < Count; ++Index) {

    Item = Control->Items[Index];
    if ((GetWindowLongPtr (Item->Wnd, GWL_STYLE) & WS_VISIBLE) == 0) {
      continue;
    }
    if (Item->Float || Item->Position == UI_POSITION_ABSOLUTE) {
      continue;
    }

    ItemSize = CONTROL_CLASS (Item)->EstimateSize (Item, ItemAvailableSize);

    if (ItemSize.cy > 0) {
      MaxHeight = MAX (MaxHeight, ItemSize.cy + GetControlMarginHeight (Item));
    }
    if (ItemSize.cx >= 0) {
      Width += ItemSize.cx;
      if (Index != (Count - 1)) {
        Width += Control->ChildPadding;
      }
      Width += MAX (GetControlMarginLeft (Item), PreviousItemMarginRight);
      PreviousItemMarginRight = GetControlMarginRight (Item);
    }
  }
  Width += PreviousItemMarginRight;

  if (Width > 0) {
    Width += (GetControlPaddingWidth (Control)  + GetControlBorderWidth (Control));
  }
  if (MaxHeight > 0) {
    MaxHeight += (GetControlPaddingHeight (Control) + GetControlBorderHeight (Control));
  }

  Width     = MAX (Width, Control->MinSize.cx);
  MaxHeight = MAX (MaxHeight, Control->MinSize.cy);

  Size = CalculateControlDisplaySize (Control, AvailableSize);
  if (Size.cy == WRAP_CONTENT) {
    Size.cy = (GDICOORD)MaxHeight;
  }

  if (Size.cx == WRAP_CONTENT) {
    Size.cx = (GDICOORD)Width;
  }

  return Size;
}


VOID
UiHorizontalLayoutSetPosition (
  UI_CONTROL                   *Control,
  CONST RECT                   *Pos
  )
{
  UI_HORIZONTAL_LAYOUT *This;
  UI_CONTROL   *Item;
  UINTN  Index;
  RECT   Rc;
  RECT   ItemRc;
  SIZE   Size;
  UINT32       AjustableCount;
  UINT32       Fixed;
  UINT32       EstimateNum;
  UINT32       Remain;
  UINT32       Left;
  UINT32       RemainFixed;
  UINT32       Expand;
  SIZE   Available;
  SIZE   ItemSize;
  SIZE   ItemAvailable;
  INT32  PreviousItemMarginRight;
  POINT  Point;


  Size.cx  = 0;
  Size.cy  = 0;
  Expand   = 0;

  This = (UI_HORIZONTAL_LAYOUT *) Control;

  ShowSetPositionDbgInfo (Control, Pos);
  GetWindowRect(Control->Wnd, &Rc);
  MapWindowPoints(HWND_DESKTOP, GetParent(Control->Wnd), (LPPOINT) &Rc, 2);
  if (!EqualRect (&Rc, Pos)) {
    ShowSetPositionDbgInfo (Control, Pos);
    MoveWindow (Control->Wnd, Pos->left, Pos->top, Pos->right - Pos->left, Pos->bottom - Pos->top, TRUE);
    CONTROL_CLASS_INVALIDATE (This);
    Control->NeedUpdateChildPos = TRUE;
  }
  if (Control->ItemCount == 0) {
    return ;
  }

  if (!Control->NeedUpdateChildPos) {
    return ;
  }
  Control->NeedUpdateChildPos = FALSE;


  CopyRect (&Rc, Pos);
  OffsetRect (&Rc, -Rc.left, -Rc.top);

  Rc.left   += (GetControlPaddingLeft (Control)   + GetControlBorderLeft (Control)  );
  Rc.top    += (GetControlPaddingTop (Control)    + GetControlBorderTop (Control)   );
  Rc.right  -= (GetControlPaddingRight (Control)  + GetControlBorderRight (Control) );
  Rc.bottom -= (GetControlPaddingBottom (Control) + GetControlBorderBottom (Control));

  if (Control->VScrollBar != NULL ) {
    Rc.right -= Control->VScrollBar->Control.Width.Value;
  }

  if (Control->HScrollBar != NULL) {
    Rc.bottom -= Control->HScrollBar->Control.Height.Value;
  }

  if (Rc.left > Rc.right) {
    Rc.left = Rc.right;
  }
  if (Rc.top > Rc.bottom) {
    Rc.top = Rc.bottom;
  }

  Available.cx = Rc.right - Rc.left;
  Available.cy = Rc.bottom - Rc.top;

  AjustableCount = 0;
  Fixed          = 0;
  EstimateNum    = 0;

  PreviousItemMarginRight = 0;
  for (Index = 0; Index < Control->ItemCount; Index++) {

    Item = Control->Items[Index];
    if ((GetWindowLongPtr (Item->Wnd, GWL_STYLE) & WS_VISIBLE) == 0) {
      continue;
    }

    if (Item->Float || Item->Position == UI_POSITION_ABSOLUTE) {
      continue;
    }

    CopyMem (&Item->Available, &Available, sizeof (SIZE));
    CopyMem (&ItemAvailable  , &Available, sizeof (SIZE));
    ItemAvailable.cx -= GetControlMarginWidth (Item);
    ItemAvailable.cy -= GetControlMarginHeight (Item);
    Size = CONTROL_CLASS (Item)->EstimateSize (Item, ItemAvailable);
    if (Size.cx == 0) {
      AjustableCount++;
    }

    if (Size.cx == MATCH_PARENT) {
      continue;
    }

    Fixed += Size.cx;
    Fixed += MAX (GetControlMarginLeft (Item), PreviousItemMarginRight);
    PreviousItemMarginRight = GetControlMarginRight (Item);
    EstimateNum ++;
  }
  Fixed += PreviousItemMarginRight;
  if (EstimateNum > 0) {
    Fixed += (EstimateNum - 1) * Control->ChildPadding;
  }
  if (AjustableCount > 0) {
    Expand = MAX (0, (INT32)(Available.cx - Fixed) / (INT32)AjustableCount);
  }

  Left = Rc.left;
  Left -= Control->ScrollPos.y;
  Remain = 0;
  RemainFixed = Fixed;
  PreviousItemMarginRight = 0;
  for (Index = 0; Index < Control->ItemCount; Index++) {

    Item = Control->Items[Index];

    if ((GetWindowLongPtr (Item->Wnd, GWL_STYLE) & WS_VISIBLE) == 0) {
      continue;
    }

    if (Item->Float || Item->Position == UI_POSITION_ABSOLUTE) {
      CopyMem (&Item->Available, &Available, sizeof (SIZE));
      CopyMem (&ItemAvailable  , &Available, sizeof (SIZE));
      ItemAvailable.cx -= GetControlMarginWidth (Item);
      ItemAvailable.cy -= GetControlMarginHeight (Item);
      Size = CONTROL_CLASS (Item)->EstimateSize (Item, ItemAvailable);

      if (Size.cx == 0 || Size.cx == MATCH_PARENT) {
        Size.cx = Available.cx;
      }

      if (Size.cy == 0 || Size.cy == MATCH_PARENT) {
        Size.cy = Available.cy;
      }

      Point = CalculateControlDisplayXY (Item, &Size, &ItemAvailable);
      SetRect (
        &ItemRc,
        Point.x + GetControlMarginLeft (Item),
        Point.y + GetControlMarginTop (Item),
        Point.x + GetControlMarginLeft (Item) + Size.cx,
        Point.y + GetControlMarginTop (Item) + Size.cy
        );
      mSetPositionLevel++;
      CONTROL_CLASS (Item)->SetPosition (Item, &ItemRc);
      mSetPositionLevel--;
      continue;
    }

    CopyMem (&Item->Available, &Available, sizeof (SIZE));
    CopyMem (&ItemAvailable  , &Available, sizeof (SIZE));
    ItemAvailable.cx -= GetControlMarginWidth (Item);
    ItemAvailable.cy -= GetControlMarginHeight (Item);
    Size = CONTROL_CLASS (Item)->EstimateSize (Item, ItemAvailable);

    if (Size.cx == 0) {
      ItemSize.cx = Expand;
    } else if (Size.cx == MATCH_PARENT) {
      ItemSize.cx = RemainFixed;
      RemainFixed = 0;
    } else {
      ItemSize.cx = Size.cx;
      RemainFixed -= (Size.cx + MAX (GetControlMarginLeft (Item), PreviousItemMarginRight));
    }


    if ((Size.cy == 0) || (Size.cy == MATCH_PARENT)) {
      ItemSize.cy = ItemAvailable.cy;
    } else {
      ItemSize.cy = Size.cy;
    }

    if (Size.cx != MATCH_PARENT) {
      Left += MAX (GetControlMarginLeft (Item), PreviousItemMarginRight);
      PreviousItemMarginRight = GetControlMarginRight (Item);
    }

    ItemRc.left   = Left;
    ItemRc.top    = Rc.top + GetControlMarginTop (Item);
    ItemRc.right  = ItemRc.left + ItemSize.cx;
    ItemRc.bottom = ItemRc.top  + ItemSize.cy;

    mSetPositionLevel++;
    CONTROL_CLASS (Item)->SetPosition (Item, &ItemRc);
    mSetPositionLevel--;
    Left = ItemRc.right + Control->ChildPadding;

  }


  //
  // process scroll bar
  //
  if (Control->HScrollBar != NULL) {
    SetWindowPos (
      Control->HScrollBar->Control.Wnd,
      HWND_TOP,
      0, Rc.bottom, Rc.right - Rc.left, Control->HScrollBar->Control.Height.Value, 0);
  }

  if (Control->VScrollBar != NULL) {
    SetWindowPos (
      Control->VScrollBar->Control.Wnd,
      HWND_TOP,
      Rc.right, 0, Control->VScrollBar->Control.Width.Value, Rc.bottom, 0);
  }
}

LRESULT
EFIAPI
UiHorizontalLayoutProc (
  HWND   Wnd,
  UINT32 Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  UI_HORIZONTAL_LAYOUT          *This;
  UI_CONTROL                    *Control;

  This = (UI_HORIZONTAL_LAYOUT *) GetWindowLongPtr (Wnd, 0);
  if (This == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  Control = (UI_CONTROL *)This;

  switch (Msg) {

  case WM_CREATE:
    This = (UI_HORIZONTAL_LAYOUT *) AllocateZeroPool (sizeof (UI_HORIZONTAL_LAYOUT));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Wnd, 0);
      SetWindowLongPtr (Wnd, 0, (INTN)This);
      SendMessage (Wnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

    break;

  case WM_NCHITTEST:
    if (Control->VScrollBar != NULL) {
      return HTCLIENT;
    }
    return HTTRANSPARENT;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }

  return 0;

}


UI_HORIZONTAL_LAYOUT_CLASS *
EFIAPI
GetHorizontalLayoutClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"HorizontalLayout", (UI_CONTROL_CLASS *)GetControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = UiHorizontalLayoutProc;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetPosition  = UiHorizontalLayoutSetPosition;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->EstimateSize = UiHorizontalEstimateSize;
  return CURRENT_CLASS;
}

