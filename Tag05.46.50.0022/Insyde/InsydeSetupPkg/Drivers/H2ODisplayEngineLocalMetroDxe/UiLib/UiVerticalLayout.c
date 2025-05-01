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

STATIC UI_VERTICAL_LAYOUT_CLASS   *mVerticalLayoutClass = NULL;
#define CURRENT_CLASS             mVerticalLayoutClass


SIZE
UiVerticalEstimateSize (
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
  INTN                          MaxWidth;
  INTN                          Height;
  INT32                         PreviousItemMarginBottom;

  if ((Control->Width.Type != UI_LENGTH_TYPE_WRAP_CONTENT) &&
      (Control->Height.Type != UI_LENGTH_TYPE_WRAP_CONTENT)) {
    return CalculateControlDisplaySize (Control, AvailableSize);
  }

  Count = Control->ItemCount;

  ItemAvailableSize.cx = AvailableSize.cx - GetControlPaddingWidth (Control) - GetControlBorderWidth (Control);
  ItemAvailableSize.cy = AvailableSize.cy - GetControlPaddingHeight (Control) - GetControlBorderHeight (Control);

  if (IS_CONTROL_WIDTH_VALUE_ASSIGNED (Control)) {
    ItemAvailableSize.cx = CalculateControlDisplayWidth (Control, AvailableSize) - GetControlPaddingWidth (Control) - GetControlBorderWidth (Control);
  }

  MaxWidth = 0;
  Height = 0;
  PreviousItemMarginBottom = 0;
  for (Index = 0; Index < Count; ++Index) {

    Item = Control->Items[Index];
    if ((GetWindowLongPtr (Item->Wnd, GWL_STYLE) & WS_VISIBLE) == 0) {
      continue;
    }
    if (Item->Float || Item->Position == UI_POSITION_ABSOLUTE) {
      continue;
    }

    ItemSize = CONTROL_CLASS (Item)->EstimateSize (Item, ItemAvailableSize);

    if (ItemSize.cx > 0) {
      MaxWidth = MAX (MaxWidth, ItemSize.cx + GetControlMarginWidth (Item));
    }
    if (ItemSize.cy >= 0) {
      Height += ItemSize.cy;
      if (Index != (Count - 1)) {
        Height += Control->ChildPadding;
      }
      Height += MAX (GetControlMarginTop (Item), PreviousItemMarginBottom);
      PreviousItemMarginBottom = GetControlMarginBottom (Item);
    }
  }
  Height += PreviousItemMarginBottom;

  if (MaxWidth > 0) {
    MaxWidth += (GetControlPaddingWidth (Control)  + GetControlBorderWidth (Control));
  }
  if (Height > 0) {
    Height   += (GetControlPaddingHeight (Control) + GetControlBorderHeight (Control));
  }

  MaxWidth = MAX (MaxWidth, Control->MinSize.cx);
  Height   = MAX (Height, Control->MinSize.cy);

  Size = CalculateControlDisplaySize (Control, AvailableSize);
  if (Size.cx == WRAP_CONTENT) {
    Size.cx = (GDICOORD)MaxWidth;
  }

  if (Size.cy == WRAP_CONTENT) {
    Size.cy = (GDICOORD)Height;
  }

  return Size;
}



VOID
UiVerticalLayoutSetPosition (
  UI_CONTROL                   *Control,
  CONST RECT                   *Pos
  )
{
  UI_VERTICAL_LAYOUT *This;
  UI_CONTROL   *Item;
  UINTN  Index;
  RECT   Rc;
  RECT   ItemRc;
  SIZE   Size;
  UINT32       AjustableCount;
  UINT32       Fixed;
  UINT32       EstimateNum;
  UINT32       Remain;
  UINT32       Top;
  UINT32       RemainFixed;
  UINT32       Expand;
  SIZE   Available;
  SIZE   ItemSize;
  SIZE    ItemAvailable;
  INT32   PreviousItemMarginBottom;
  POINT   Point;
  BOOLEAN EnableScroll;

  Size.cx  = 0;
  Size.cy  = 0;
  Expand   = 0;

  This = (UI_VERTICAL_LAYOUT *) Control;

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
  PreviousItemMarginBottom = 0;
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
    if (Size.cy == 0) {
      AjustableCount++;
    }

    if (Size.cy == MATCH_PARENT) {
      continue;
    }

    Fixed += Size.cy;
    Fixed += MAX (GetControlMarginTop (Item), PreviousItemMarginBottom);
    PreviousItemMarginBottom = GetControlMarginBottom (Item);
    EstimateNum ++;
  }
  Fixed += PreviousItemMarginBottom;
  if (EstimateNum > 0) {
    Fixed += (EstimateNum - 1) * Control->ChildPadding;
  }

  if (AjustableCount > 0) {
    Expand = MAX (0, (INT32)(Available.cy - Fixed) / (INT32)AjustableCount);
  }

  Top = Rc.top;

  EnableScroll = TRUE;
  if (Control->VScrollBar != NULL) {
    if (Fixed > (UINT32) Available.cy) {
      EnableScroll = TRUE;
      Top -= Control->ScrollPos.y;
      Control->VScrollBar->Range = Fixed - Available.cy;
    } else {
      Control->ScrollPos.y = 0;
      Control->VScrollBar->ScrollPos = 0;
      Control->VScrollBar->Range = 0;
    }
  }

  Remain = 0;
  RemainFixed = Fixed;
  PreviousItemMarginBottom = 0;
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
        Size.cx = ItemAvailable.cx;
      }

      if (Size.cy == 0 || Size.cy == MATCH_PARENT) {
        Size.cy = ItemAvailable.cy;
      }

      Point = CalculateControlDisplayXY (Item, &Size, &ItemAvailable);

      SetRect (
        &ItemRc,
        Point.x + GetControlMarginLeft (Item),
        Point.y + GetControlMarginTop (Item),
        Point.x + GetControlMarginLeft (Item) + Size.cx,
        Point.y + GetControlMarginTop (Item)  + Size.cy
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

    if (Size.cy == 0) {
      ItemSize.cy = Expand;
    } else if (Size.cy == MATCH_PARENT) {
      ItemSize.cy = Rc.bottom - Top;
    } else {
      ItemSize.cy = Size.cy;
      RemainFixed -= (Size.cy + MAX (GetControlMarginTop (Item), PreviousItemMarginBottom));
    }

    if ((Size.cx == 0) || (Size.cx == MATCH_PARENT)) {
      ItemSize.cx = ItemAvailable.cx;
    } else {
      ItemSize.cx = Size.cx;
    }

    if (Size.cy != MATCH_PARENT) {
      Top += MAX (GetControlMarginTop (Item), PreviousItemMarginBottom);
      PreviousItemMarginBottom = GetControlMarginBottom (Item);
    }

    ItemRc.left   = Rc.left + GetControlMarginLeft (Item);
    ItemRc.top    = Top;
    ItemRc.right  = ItemRc.left + ItemSize.cx;
    ItemRc.bottom = ItemRc.top  + ItemSize.cy;
    mSetPositionLevel++;
    CONTROL_CLASS (Item)->SetPosition (Item, &ItemRc);
    mSetPositionLevel--;
    Top = ItemRc.bottom + Control->ChildPadding;
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
    if (EnableScroll) {
      UiSetAttribute (Control->VScrollBar, L"visibility", L"true");

      CopyRect (&Rc, Pos);
      OffsetRect (&Rc, -Rc.left, -Rc.top);
      Rc.right -= Control->VScrollBar->Control.Width.Value;

      ItemRc.left   = Rc.right;
      ItemRc.top    = 0;
      ItemRc.right  = ItemRc.left + Control->VScrollBar->Control.Width.Value;
      ItemRc.bottom = Rc.bottom;
      CONTROL_CLASS (Control->VScrollBar)->SetPosition ((UI_CONTROL *)Control->VScrollBar, &ItemRc);
    } else {
      UiSetAttribute (Control->VScrollBar, L"visibility", L"false");
    }
  }
}


LRESULT
EFIAPI
UiVerticalLayoutProc (
  HWND   Wnd,
  UINT32 Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  UI_VERTICAL_LAYOUT            *This;
  UI_CONTROL                    *Control;

  This = (UI_VERTICAL_LAYOUT *) GetWindowLongPtr (Wnd, 0);
  if (This == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  Control = (UI_CONTROL *)This;

  switch (Msg) {

  case WM_CREATE:
    This = (UI_VERTICAL_LAYOUT *) AllocateZeroPool (sizeof (UI_VERTICAL_LAYOUT));
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

UI_VERTICAL_LAYOUT_CLASS  *
EFIAPI
GetVerticalLayoutClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"VerticalLayout", (UI_CONTROL_CLASS *)GetControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = UiVerticalLayoutProc;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetPosition  = UiVerticalLayoutSetPosition;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->EstimateSize = UiVerticalEstimateSize;
  return CURRENT_CLASS;
}

