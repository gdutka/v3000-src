/** @file
  UI Common Controls

;******************************************************************************
;* Copyright (c) 2012 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "UiControls.h"

HBITMAP                      mDummyBitmap = NULL; //Dummy module variable used for klocwork misjudgement

STATIC
UI_CONTROL **
GetOrderedItemsByZIndex (
  IN UI_CONTROL                 *This
  )
{
  UINTN                         Index;
  UINTN                         CheckIndex;
  INT32                         ParentZIndex;
  INT32                         ZIndex;
  UI_CONTROL                    *Control;
  UI_CONTROL                    **OrderedItems;

  if (This == NULL || This->Items == NULL || This->ItemCount < 2) {
    return NULL;
  }

  //
  // If all items are the same z-index value, no need to allocate memory to return ordered items.
  //
  for (Index = 1; Index < This->ItemCount; Index++) {
    if (This->Items[Index]->ZIndex.Type  != This->Items[0]->ZIndex.Type ||
        This->Items[Index]->ZIndex.Value != This->Items[0]->ZIndex.Value) {
      break;
    }
  }
  if (Index == This->ItemCount) {
    return NULL;
  }

  OrderedItems = AllocateCopyPool (This->ItemCount * sizeof(UI_CONTROL *), This->Items);
  if (OrderedItems == NULL) {
    return NULL;
  }

  //
  // Get parent z-index value if need.
  //
  ParentZIndex = 0;
  for (Index = 0; Index < This->ItemCount; Index++) {
    if (This->Items[Index]->ZIndex.Type != UI_Z_INDEX_TYPE_AUTO) {
      continue;
    }

    for (Control = This; Control != NULL; Control = CONTROL_CLASS_GET_PARENT(Control)) {
      if (Control->ZIndex.Type == UI_Z_INDEX_TYPE_NUMBER) {
        ParentZIndex = Control->ZIndex.Value;
        break;
      }
    }
    break;
  }

  for (Index = 0; Index < This->ItemCount - 1; Index++) {
    Control = OrderedItems[Index];
    ZIndex  = (Control->ZIndex.Type == UI_Z_INDEX_TYPE_AUTO) ? ParentZIndex : Control->ZIndex.Value;

    for (CheckIndex = Index + 1; CheckIndex < This->ItemCount; CheckIndex++) {
      if ((OrderedItems[CheckIndex]->ZIndex.Type == UI_Z_INDEX_TYPE_NUMBER && OrderedItems[CheckIndex]->ZIndex.Value < ZIndex) ||
          (OrderedItems[CheckIndex]->ZIndex.Type == UI_Z_INDEX_TYPE_AUTO   && ParentZIndex                           < ZIndex)) {
        OrderedItems[Index]      = OrderedItems[CheckIndex];
        OrderedItems[CheckIndex] = Control;
        Control = OrderedItems[Index];
        ZIndex = (Control->ZIndex.Type == UI_Z_INDEX_TYPE_AUTO) ? ParentZIndex : Control->ZIndex.Value;
      }
    }
  }

  return OrderedItems;
}

STATIC
VOID
StrToUiLength (
  IN OUT CHAR16                 **ValueStr,
  OUT    UI_LENGTH              *UiLength
  )
{
  UiLength->Type  = UI_LENGTH_TYPE_PIXEL;
  UiLength->Value = wcstol (*ValueStr, ValueStr, 10);

  if ((*ValueStr)[0] == '%') {
    (*ValueStr) += 1;
    UiLength->Type = UI_LENGTH_TYPE_PERCENTAGE;
  } else if (StrnCmp (*ValueStr, L"px", 2) == 0) {
    (*ValueStr) += 2;
    UiLength->Type = UI_LENGTH_TYPE_PIXEL;
  } else if (StrnCmp (*ValueStr, L"em", 2) == 0) {
    (*ValueStr) += 2;
    UiLength->Type = UI_LENGTH_TYPE_EM;
  }

  if ((*ValueStr)[0] == ',' || (*ValueStr)[0] == '|') {
    (*ValueStr) += 1;
  }
}

VOID
UiControlOnUiCreate (
  UI_CONTROL                    *Control,
  HWND                          Wnd,
  UI_MANAGER                    *Manager
  )
{
  UI_CONTROL                    *This;

  This = (UI_CONTROL *) Control;

  This->Wnd             = Wnd;
  if (Manager != NULL) {
    This->Manager       = Manager;
  }

  This = (UI_CONTROL *) Control;

  //
  // original container
  //
  This->ItemCount = 0;
  This->Items     = NULL;
  This->ChildPadding = 0;
  This->HScrollBar = NULL;
  This->VScrollBar   = NULL;
  This->ScrollPos.x = 0;
  This->ScrollPos.y = 0;
  This->TouchSlop   = 4;
  This->IsBeginDraged   = FALSE;
  This->Dragged         = FALSE;
  This->BkImageStyle    = 0;
  This->Filter          = 0;
  This->TextColor       = 0;
  This->UserData        = NULL;
  This->Name            = L"";
  This->BkColor         = 0x00000000;
  This->FocusBkColor    = 0x00000000;
  This->BorderColor     = 0;
  This->Float           = FALSE;
  This->GradientVertial = TRUE;
  This->NeedUpdateChildPos   = TRUE;
  This->TabOrder        = (UINT32)(-2);
  This->Position        = UI_POSITION_RELATIVE;
  This->ZIndex.Type     = UI_Z_INDEX_TYPE_AUTO;
  This->ZIndex.Value    = 0;
  This->BoxSizing       = UI_BOX_SIZING_CONTENT_BOX;
  SetRectEmpty (&This->Scale9Grid);

  //
  // no focus
  //
  SetWindowLongPtr (
    This->Wnd,
    GWL_EXSTYLE,
    GetWindowLongPtr (This->Wnd, GWL_EXSTYLE) | WS_EX_NOACTIVATE
    );

  SetWindowLongPtr (
    This->Wnd,
    GWL_EXSTYLE,
    GetWindowLongPtr (This->Wnd, GWL_EXSTYLE) | WS_EX_CONTROLPARENT
    );


}


VOID
UiControlOnDestroy (
  UI_CONTROL                    *Control
  )
{
  UI_CONTROL                    *This;


  This = (UI_CONTROL *) Control;

  /*
  if (This->Wnd != This->Manager->MainWnd &&
      This->Wnd != This->Manager->Root) {
    Parent = ((UI_CONTAINER *)GetUiControl(GetParent (This->Wnd)));
    Parent->Remove (Parent, This);
  } */

  ASSERT (This);
  ASSERT (This->Name);

  if (StrCmp (This->Name, L"") != 0) {
    StringMapRemove (&This->Manager->NameList, This->Name);
    FreePool (This->Name);
  }

  if (This->UserData != NULL) {
    FreePool (This->UserData);
  }

  if (This->BkImage != NULL) {
    FreePool (This->BkImage);
  }
  if (This->BkImageBuf.Bitmap != NULL) {
    FreePool (This->BkImageBuf.Bitmap);
  }

  if (This->HScrollBar != NULL) {
    DestroyWindow (((UI_CONTROL *)(This->HScrollBar))->Wnd);
  }

  if (This->VScrollBar != NULL) {
    DestroyWindow (((UI_CONTROL *)(This->VScrollBar))->Wnd);
  }

  CONTROL_CLASS (This)->RemoveAllChild (This);

  FreePool (This);
}


VOID
UiControlSetPosition (
  UI_CONTROL                    *Control,
  CONST RECT                    *Pos
  )
{
  RECT                          Rc;
  UINTN                         Index;
  UI_CONTROL                    *Item;
  RECT                          ItemRc;
  UI_CONTROL                    *This;
  SIZE                          Size;
  SIZE                          Available;
  POINT                         LeftTop;

  This = (UI_CONTROL *) Control;

  ASSERT (Pos->left <= Pos->right);
  ASSERT (Pos->top <= Pos->bottom);

  ShowSetPositionDbgInfo (Control, Pos);
  GetWindowRect(Control->Wnd, &Rc);
  MapWindowPoints(HWND_DESKTOP, GetParent(Control->Wnd), (LPPOINT) &Rc, 2);
  if (!EqualRect (&Rc, Pos)) {
    MoveWindow (Control->Wnd, Pos->left, Pos->top, Pos->right - Pos->left, Pos->bottom - Pos->top, TRUE);
    CONTROL_CLASS_INVALIDATE (This);
    This->NeedUpdateChildPos = TRUE;
  }

  if (This->ItemCount == 0) {
    return ;
  }

  if (!This->NeedUpdateChildPos) {
    return ;
  }
  This->NeedUpdateChildPos = FALSE;


  CopyRect (&Rc, Pos);
  OffsetRect (&Rc, -Rc.left, -Rc.top);

  Rc.left   += (GetControlPaddingLeft (This)   + GetControlBorderLeft (This)  );
  Rc.top    += (GetControlPaddingTop (This)    + GetControlBorderTop (This)   );
  Rc.right  -= (GetControlPaddingRight (This)  + GetControlBorderRight (This) );
  Rc.bottom -= (GetControlPaddingBottom (This) + GetControlBorderBottom (This));

  if (Rc.left > Rc.right) {
    Rc.left = Rc.right;
  }
  if (Rc.top > Rc.bottom) {
    Rc.top = Rc.bottom;
  }

  Available.cx = Rc.right - Rc.left;
  Available.cy = Rc.bottom - Rc.top;
  for (Index = 0; Index < This->ItemCount; Index++) {
    Item = This->Items[Index];

    if ((GetWindowLongPtr (Item->Wnd, GWL_STYLE) & WS_VISIBLE) == 0) {
      continue;
    }
    Item->Available = Available;
    Size = CONTROL_CLASS (Item)->EstimateSize (Item, Available);

    if (Size.cx == 0 || Size.cx == MATCH_PARENT) {
      Size.cx = Rc.right - Rc.left;
    }

    if (Size.cy == 0 || Size.cy == MATCH_PARENT) {
      Size.cy = Rc.bottom - Rc.top;
    }

    LeftTop = CalculateControlDisplayXY (Item, &Size, &Available);
    LeftTop.x += Rc.left;
    LeftTop.y += Rc.top;

    SetRect (
      &ItemRc,
      LeftTop.x,
      LeftTop.y,
      LeftTop.x + Size.cx,
      LeftTop.y + Size.cy
      );
    mSetPositionLevel++;
    CONTROL_CLASS (Item)->SetPosition (Item, &ItemRc);
    mSetPositionLevel--;
  }
}


VOID
UiControlOnUiPaint (
  UI_CONTROL                    *Control,
  WPARAM                        WParam,
  LPARAM                        LParam
  )
{
  COLORREF                      Color;
  RECT                          Rc;
  HDC                           Hdc;
  UI_MANAGER                    *Manager;
  HWND                          Wnd;
  RECT                          ClipRc;
  HRGN                          OldRgn;
  HRGN                          NewRgn;
  UINTN                         Index;
  UI_CONTROL                    *This;
  UI_CONTROL                    *Item;
  RECT                          TmpRc;
  RECT                          ItemRc;
  UI_CONTROL                    **OrderedItems;
  SIZE                          Size;

  This = (UI_CONTROL *) Control;

  Manager = Control->Manager;
  Hdc = Control->Manager->PaintDC;
  Manager->GetControlRect (Manager, Control, &Rc);
  Wnd = Control->Wnd;

  switch (WParam) {

  case PAINT_ALL:
    CONTROL_CLASS_SET_STATE(Control, 0, UISTATE_INVALIDATED);

    Manager->GetControlRect (Manager, This, &Rc);
    if (!IntersectRect (&TmpRc, &Rc, (RECT *)LParam)) {
      return ;
    }

    SendMessage (Wnd, UI_NOTIFY_PAINT,  PAINT_BKCOLOR, LParam);
    SendMessage (Wnd, UI_NOTIFY_PAINT,  PAINT_BKIMAGE, LParam);
    SendMessage (Wnd, UI_NOTIFY_PAINT,  PAINT_STATUSIMAGE, LParam);
    SendMessage (Wnd, UI_NOTIFY_PAINT,  PAINT_TEXT, LParam);
    SendMessage (Wnd, UI_NOTIFY_PAINT,  PAINT_BORDER, LParam);

    GetClipBox (Manager->PaintDC, &ClipRc);
    OldRgn = CreateRectRgnIndirect (&ClipRc);
    NewRgn = CreateRectRgnIndirect (&TmpRc);
    ExtSelectClipRgn (Manager->PaintDC, NewRgn, RGN_AND);

    OrderedItems = GetOrderedItemsByZIndex (This);
    if (OrderedItems == NULL) {
      OrderedItems = This->Items;
    }
    for (Index = 0; Index < This->ItemCount; Index++) {
      Item = OrderedItems[Index];

      if (!IsWindowVisible (Item->Wnd)) {
        continue;
      }


      Manager->GetControlRect (Manager, Item, &ItemRc);
      if (IntersectRect (&TmpRc, &ItemRc, (RECT *)LParam)) {
        SendMessage (Item->Wnd, UI_NOTIFY_PAINT, PAINT_ALL, (LPARAM)&TmpRc);
      }
    }
    if (OrderedItems != NULL && OrderedItems != This->Items) {
      FreePool (OrderedItems);
    }

    if (This->HScrollBar != NULL) {
      Manager->GetControlRect (Manager, (UI_CONTROL *)This->HScrollBar, &ItemRc);
      if (IntersectRect (&TmpRc, &ItemRc, (RECT *)LParam)) {
        SendMessage (This->HScrollBar->Control.Wnd, UI_NOTIFY_PAINT, PAINT_ALL, (LPARAM)&TmpRc);
      }
    }

    if (This->VScrollBar != NULL) {
      Manager->GetControlRect (Manager, (UI_CONTROL *)This->VScrollBar, &ItemRc);
      if (IntersectRect (&TmpRc, &ItemRc, (RECT *)LParam)) {
        SendMessage (This->VScrollBar->Control.Wnd, UI_NOTIFY_PAINT, PAINT_ALL, (LPARAM)&TmpRc);
      }
    }

    SelectClipRgn(Manager->PaintDC, OldRgn);
    if (OldRgn != NULL) {
      DeleteObject(OldRgn);
    }
    if (NewRgn != NULL) {
      DeleteObject(NewRgn);
    }
    break;

  case PAINT_BKIMAGE:
    if (Control->BkImage != 0) {
      ASSERT (Rc.right > Rc.left && Rc.bottom > Rc.top);
      if (Control->Border.Left.Value   > 0) {Rc.left   += GetControlBorderLeft (This);  }
      if (Control->Border.Top.Value    > 0) {Rc.top    += GetControlBorderTop (This);   }
      if (Control->Border.Right.Value  > 0) {Rc.right  -= GetControlBorderRight (This); }
      if (Control->Border.Bottom.Value > 0) {Rc.bottom -= GetControlBorderBottom (This);}

      if (IS_UI_LENGTH_VALUE_ASSIGNED(This->BkImageSize.Width)) {
        Size.cx = UiLengthToPixel (&This->BkImageSize.Width , Rc.right - Rc.left, GetControlFontSize (This));

        if (IS_UI_LENGTH_VALUE_ASSIGNED(This->BkImageSize.Height)) {
          Size.cy = UiLengthToPixel (&This->BkImageSize.Height , Rc.bottom - Rc.top, GetControlFontSize (This));
        } else {
          Size.cy = ((Rc.bottom - Rc.top) * Size.cx) / (Rc.right - Rc.left);
        }
      } else {
        if (IS_UI_LENGTH_VALUE_ASSIGNED(This->BkImageSize.Height)) {
          Size.cy = UiLengthToPixel (&This->BkImageSize.Height , Rc.bottom - Rc.top, GetControlFontSize (This));
          Size.cx = ((Rc.right - Rc.left) * Size.cy) / (Rc.bottom - Rc.top);
        } else {
          Size.cy = (Rc.bottom - Rc.top);
          Size.cx = (Rc.right - Rc.left);
        }
      }

      Rc.right  = Rc.left + Size.cx;
      Rc.bottom = Rc.top + Size.cy;
      UiShowBitmap (
        Control->Wnd,
        Manager->PaintDC,
        &Rc,
        &Control->Scale9Grid,
        Control->BkImageBuf.Bitmap != NULL ? &Control->BkImageBuf : GetImageByString (Control->BkImage),
        GetControlBkImageStyle (Control),
        &Control->HsvDiff
        );
    }
    break;

  case PAINT_BKCOLOR:
    if ((CONTROL_CLASS_GET_STATE(Control) & UISTATE_FOCUSED) != 0 && Control->FocusBkColor != 0) {
      Color = Control->FocusBkColor;
    } else {
      Color = Control->BkColor;
    }

    if (Color != 0) {
      if (Control->Border.Left.Value   > 0) {Rc.left   += GetControlBorderLeft (This);  }
      if (Control->Border.Top.Value    > 0) {Rc.top    += GetControlBorderTop (This);   }
      if (Control->Border.Right.Value  > 0) {Rc.right  -= GetControlBorderRight (This); }
      if (Control->Border.Bottom.Value > 0) {Rc.bottom -= GetControlBorderBottom (This);}

      if (Color >= 0xFF000000) {
        UiFastFillRect (Hdc, &Rc, Color);
      } else {
        HDC      hdcMem;
        HBITMAP  hbmp, hbmpOrg, hbmpDel;
        RECT     rc;

        GetClientRect (Control->Wnd, &rc);
        rc.right  -= (GetControlBorderWidth (Control));
        rc.bottom -= (GetControlBorderHeight (Control));
        /* redirect painting to offscreen dc*/
        hdcMem  = CreateCompatibleDC(Hdc);
        if (hdcMem != NULL) {
          mDummyBitmap = CreateCompatibleBitmap(hdcMem, rc.right, rc.bottom);
          hbmp = mDummyBitmap;
          hbmpOrg = SelectObject(hdcMem, hbmp);

          UiFastFillRect (hdcMem, &rc, Color);

          /* alpha blend blit offscreen map with physical screen*/
          BitBlt(Hdc, Rc.left, Rc.top, rc.right - rc.left, rc.bottom - rc.top, hdcMem, 0, 0, MWROP_SRC_OVER);  /* FIXME specify constant alpha somewhere!*/
          hbmpDel = SelectObject(hdcMem, hbmpOrg);
          if (hbmpDel != NULL) {
            DeleteObject(hbmpDel);
          }
          DeleteDC(hdcMem);
        }

      }
    }
    break;

  case PAINT_STATUSIMAGE:
    break;

  case PAINT_TEXT:
    break;

  case PAINT_BORDER:
if (0) {
    Color = Control->BorderColor;
    if (CONTROL_CLASS_GET_STATE(Control) & UISTATE_FOCUSED) {
      Color = 0xFFFF0000;
    }
    if (Color != 0) {
      SelectObject (Hdc, GetStockObject (PS_NULL));
      SelectObject (Hdc, GetStockObject (DC_PEN));
      SetDCPenColor (Hdc, Color);
      Rectangle (Hdc, Rc.left, Rc.top, Rc.right, Rc.bottom);
    }
    if (CONTROL_CLASS_GET_STATE(Control) & UISTATE_FOCUSED) {
      Rectangle (Hdc, Rc.left + 1, Rc.top + 1, Rc.right - 1, Rc.bottom - 1);
    }
}
    Color = Control->BorderColor;
    if (GetControlBorderLeft (Control) > 0) {
      CopyRect (&TmpRc, &Rc);
      TmpRc.right = TmpRc.left + GetControlBorderLeft (Control);
      UiFastFillRect (Hdc, &TmpRc, Color);
    }
    if (GetControlBorderRight (Control) > 0) {
      CopyRect (&TmpRc, &Rc);
      TmpRc.left = TmpRc.right - GetControlBorderRight (Control);
      UiFastFillRect (Hdc, &TmpRc, Color);
    }
    if (GetControlBorderTop (Control) > 0) {
      CopyRect (&TmpRc, &Rc);
      TmpRc.bottom = TmpRc.top + GetControlBorderTop (Control);
      UiFastFillRect (Hdc, &TmpRc, Color);
    }
    if (GetControlBorderBottom (Control) > 0) {
      CopyRect (&TmpRc, &Rc);
      TmpRc.top = TmpRc.bottom - GetControlBorderBottom (Control);
      UiFastFillRect (Hdc, &TmpRc, Color);
    }
    break;

  default:
    ASSERT (FALSE);
    break;

  }
}

UI_STATE
EFIAPI
UiControlGetState (
  UI_CONTROL                    *Control
  )
{
  return Control->StateFlags;
}

VOID
EFIAPI
UiControlSetState (
  UI_CONTROL                    *Control,
  UI_STATE                      SetState,
  UI_STATE                      ClearState
  )
{
  UI_STATE                      OldState;

  if (Control->OnSetState) {
    Control->OnSetState (Control, SetState, ClearState);
  }
  OldState = Control->StateFlags;

  Control->StateFlags |= SetState;
  Control->StateFlags &= ~(ClearState);

  if ((Control->StateFlags != OldState) &&
      !(ClearState & UISTATE_INVALIDATED) &&
      !(Control->StateFlags & UISTATE_INVALIDATED)) {
    CONTROL_CLASS_INVALIDATE (Control);
  }
}

BOOLEAN
UiControlSetAttribute (
  UI_CONTROL                    *Control,
  CHAR16                        *Name,
  CHAR16                        *Value
  )
{
  EFI_STATUS                    Status;
  CHAR16                        *PtrValue;
  UI_CONTROL                    *ScrollBar;
  UI_CONTROL                    *This;

  This = (UI_CONTROL *) Control;
  PtrValue = Value;

  if (StrCmp (Name, L"name") == 0) {
    ASSERT (This->Name);
    if (StrCmp(This->Name, L"") != 0) {
      StringMapRemove (&This->Manager->NameList, Value);
      FreePool (This->Name);
    }
    This->Name = AllocateCopyPool (StrSize(Value), Value);
    StringMapAdd (&This->Manager->NameList, Value, This);
  } else if (StrCmp (Name, L"width") == 0) {
    if (isalpha (Value[0]) || StrCmp (Value, L"-1") == 0 || StrCmp (Value, L"-2") == 0) {
      if (StrCmp (Value, L"match_parent") == 0 || StrCmp (Value, L"-1") == 0) {
        This->Width.Type  = UI_LENGTH_TYPE_MATCH_PARENT;
        This->Width.Value = 0;
      } else if (StrCmp (Value, L"wrap_content") == 0 || StrCmp (Value, L"-2") == 0) {
        This->Width.Type  = UI_LENGTH_TYPE_WRAP_CONTENT;
        This->Width.Value = 0;
      } else {
        DEBUG ((EFI_D_ERROR, "width can only use match_parent, wrap_content or const value"));
        ASSERT (FALSE);
      }
    } else {
      StrToUiLength (&PtrValue, &This->Width);
    }
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"height") == 0) {
    if (isalpha (Value[0]) || StrCmp (Value, L"-1") == 0 || StrCmp (Value, L"-2") == 0) {
      if (StrCmp (Value, L"match_parent") == 0 || StrCmp (Value, L"-1") == 0) {
        This->Height.Type  = UI_LENGTH_TYPE_MATCH_PARENT;
        This->Height.Value = 0;
      } else if (StrCmp (Value, L"wrap_content") == 0 || StrCmp (Value, L"-2") == 0) {
        This->Height.Type  = UI_LENGTH_TYPE_WRAP_CONTENT;
        This->Height.Value = 0;
      } else {
        DEBUG ((EFI_D_ERROR, "width can only use match_parent, wrap_content or const value"));
        ASSERT (FALSE);
      }
    } else {
      StrToUiLength (&PtrValue, &This->Height);
    }
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"background-image") == 0) {
    if (This->BkImage != NULL) {
      FreePool (This->BkImage);
    }
    if (This->BkImageBuf.Bitmap != NULL) {
      FreePool (This->BkImageBuf.Bitmap);
    }

    This->BkImage = AllocateCopyPool (StrSize(Value), Value);
    if (This->BkImage == NULL) {
      DEBUG ((EFI_D_ERROR, "Allocate This->BkImage fail."));
      return TRUE;
    }

    if (Value[0] != '@') {
      EFI_IMAGE_INPUT             *Image;

      Image = GetImageByString (This->BkImage);
      if (Image != NULL && Image->Width != 0 && Image->Height != 0) {
        CopyMem (&This->BkImageBuf, Image, sizeof(EFI_IMAGE_INPUT));
        This->BkImageBuf.Bitmap = AllocateCopyPool (Image->Width * Image->Height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), Image->Bitmap);
      }
    }
  } else if (StrCmp (Name, L"background-size") == 0) {
    if (isalpha (PtrValue[0])) {
      if (StrnCmp (PtrValue, L"auto", 4) == 0) {
        This->BkImageSize.Width.Value = 0;
        This->BkImageSize.Width.Type  = UI_LENGTH_TYPE_UNDEFINED;
        PtrValue += 4;
      } else {
        DEBUG ((EFI_D_ERROR, "background-size can only use auto or const value"));
        ASSERT (FALSE);
      }
      if (*PtrValue == ',') {
        PtrValue++;
      }
    } else {
      StrToUiLength (&PtrValue, &This->BkImageSize.Width);
    }

    if (*PtrValue == 0 || isalpha (PtrValue[0])) {
      if (*PtrValue == 0 || StrnCmp (PtrValue, L"auto", 4) == 0) {
        This->BkImageSize.Height.Value = 0;
        This->BkImageSize.Height.Type  = UI_LENGTH_TYPE_UNDEFINED;
      } else {
        DEBUG ((EFI_D_ERROR, "background-size can only use auto or const value"));
        ASSERT (FALSE);
      }
    } else {
      StrToUiLength (&PtrValue, &This->BkImageSize.Height);
    }
  } else if (StrCmp (Name, L"background-color") == 0) {
    This->BkColor     = GetColorValue (Value);
  } else if (StrCmp (Name, L"focusbkcolor") == 0) {
    This->FocusBkColor = GetColorValue (Value);
  } else if (StrCmp (Name, L"border-color") == 0) {
    This->BorderColor = GetColorValue (Value);
  } else if (StrCmp (Name, L"visibility") == 0) {
    if (StrCmp (Value, L"true") == 0) {
      ShowWindow (This->Wnd, SW_SHOW);
    } else {
      ShowWindow (This->Wnd, SW_HIDE);
    }
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"font-size") == 0) {
    Control->FontSize = (INT32)StrToUInt (Value, 10, &Status);
  } else if (StrCmp (Name, L"textcolor") == 0) {
    Control->TextColor = GetColorValue (Value);
  } else if (StrnCmp (Name, L"padding", 7) == 0) {
    Name += 7;
    if (*Name == 0) {
      StrToUiLength (&PtrValue, &This->Padding.Top);
      StrToUiLength (&PtrValue, &This->Padding.Right);
      StrToUiLength (&PtrValue, &This->Padding.Bottom);
      StrToUiLength (&PtrValue, &This->Padding.Left);
    } else if (StrCmp (Name, L"-top") == 0) {
      StrToUiLength (&PtrValue, &This->Padding.Top);
    } else if (StrCmp (Name, L"-right") == 0) {
      StrToUiLength (&PtrValue, &This->Padding.Right);
    } else if (StrCmp (Name, L"-bottom") == 0) {
      StrToUiLength (&PtrValue, &This->Padding.Bottom);
    } else if (StrCmp (Name, L"-left") == 0) {
      StrToUiLength (&PtrValue, &This->Padding.Left);
    } else {
      ASSERT (FALSE);
    }
    UiNeedUpdatePos (Control);
  } else if (StrnCmp (Name, L"border", 6) == 0) {
    Name += 6;
    if (StrCmp (Name, L"-width") == 0) {
      StrToUiLength (&PtrValue, &This->Border.Top);
      StrToUiLength (&PtrValue, &This->Border.Right);
      StrToUiLength (&PtrValue, &This->Border.Bottom);
      StrToUiLength (&PtrValue, &This->Border.Left);
    } else if (StrCmp (Name, L"-top") == 0) {
      StrToUiLength (&PtrValue, &This->Border.Top);
    } else if (StrCmp (Name, L"-right") == 0) {
      StrToUiLength (&PtrValue, &This->Border.Right);
    } else if (StrCmp (Name, L"-bottom") == 0) {
      StrToUiLength (&PtrValue, &This->Border.Bottom);
    } else if (StrCmp (Name, L"-left") == 0) {
      StrToUiLength (&PtrValue, &This->Border.Left);
    } else {
      ASSERT (FALSE);
    }
    UiNeedUpdatePos (Control);
  } else if (StrnCmp (Name, L"margin", 6) == 0) {
    Name += 6;
    if (*Name == 0) {
      StrToUiLength (&PtrValue, &This->Margin.Top);
      StrToUiLength (&PtrValue, &This->Margin.Right);
      StrToUiLength (&PtrValue, &This->Margin.Bottom);
      StrToUiLength (&PtrValue, &This->Margin.Left);
    } else if (StrCmp (Name, L"-top") == 0) {
      StrToUiLength (&PtrValue, &This->Margin.Top);
    } else if (StrCmp (Name, L"-right") == 0) {
      StrToUiLength (&PtrValue, &This->Margin.Right);
    } else if (StrCmp (Name, L"-bottom") == 0) {
      StrToUiLength (&PtrValue, &This->Margin.Bottom);
    } else if (StrCmp (Name, L"-left") == 0) {
      StrToUiLength (&PtrValue, &This->Margin.Left);
    } else {
      ASSERT (FALSE);
    }
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"left") == 0) {
    StrToUiLength (&PtrValue, &Control->Left);
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"right") == 0) {
    StrToUiLength (&PtrValue, &Control->Right);
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"top") == 0) {
    StrToUiLength (&PtrValue, &Control->Top);
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"bottom") == 0) {
    StrToUiLength (&PtrValue, &Control->Bottom);
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"scale9grid") == 0) {
    This->Scale9Grid.left   = wcstol (Value, &PtrValue, 10);
    This->Scale9Grid.top    = wcstol (PtrValue + 1, &PtrValue, 10);
    This->Scale9Grid.right  = wcstol (PtrValue + 1, &PtrValue, 10);
    This->Scale9Grid.bottom = wcstol (PtrValue + 1, &PtrValue, 10);
  } else if (StrCmp (Name, L"float") == 0) {
    if (StrCmp (Value, L"true") == 0) {
      This->Float = TRUE;
    } else {
      This->Float = FALSE;
    }
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"gradient_vertical") == 0) {
    if (StrCmp (Value, L"true") == 0) {
      This->GradientVertial = TRUE;
    } else {
      This->GradientVertial = FALSE;
    }
  } else if (StrCmp (Name, L"user_data") == 0) {
    if (This->UserData != NULL) {
      FreePool (This->UserData);
    }
    This->UserData = AllocateCopyPool (StrSize (Value), Value);
  } else if (StrCmp (Name, L"background-image-style") == 0) {

    This->BkImageStyle = 0;
    if (StrStr (Value, L"center") != NULL) {
      This->BkImageStyle &= ~ (DT_STRETCH);
      This->BkImageStyle |= DT_CENTER | DT_VCENTER;
    }
    if (StrStr (Value, L"stretch") != NULL) {
      SetRectEmpty (&Control->Scale9Grid);
      Control->BkImageStyle &= ~ (DT_CENTER | DT_VCENTER);
      Control->BkImageStyle |= DT_STRETCH;
    }
    if (StrStr (Value, L"gray") != NULL) {
      Control->BkImageStyle |= DT_GRAY;
    }
    if (StrStr (Value, L"light") != NULL) {
      Control->BkImageStyle |= DT_LIGHT;
    }
    if (StrCmp (Value, L"") == 0) {
      This->BkImageStyle = 0;
    }
  } else if (StrCmp (Name, L"filter") == 0) {
    if (StrCmp (Value, L"gray") == 0) {
      Control->Filter |= DT_GRAY;
    } else if (StrCmp (Value, L"none") == 0) {
      Control->Filter = 0;
    } else {
      DEBUG ((EFI_D_ERROR, "filter can only use gray or none"));
      ASSERT (FALSE);
    }
  } else if (StrCmp (Name, L"child-padding") == 0) {
    This->ChildPadding = (UINT32)StrToUInt (Value, 10, &Status);
  } else if (StrCmp (Name, L"vscrollbarpos") == 0) {
    if (This->VScrollBar != NULL) {
       SCROLLBAR_CLASS(This->VScrollBar)->SetScrollPos (This->VScrollBar, (INT32)StrToUInt (Value, 10, &Status));
    }
  } else if (StrCmp (Name, L"vscrollbar") == 0) {

    if (StrCmp (Value, L"true") == 0 &&
      This->VScrollBar == NULL) {
      ScrollBar = CreateControl (L"SCROLLBAR", This);
      ScrollBar->Class->SetAttribute (ScrollBar, L"hor", L"false");
      This->VScrollBar = (UI_SCROLLBAR *) ScrollBar;
      ((UI_SCROLLBAR *)ScrollBar)->Owner = This;
    }
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"hscrollbar") == 0) {

    if ((StrCmp (Value, L"true") == 0) && (This->HScrollBar == NULL)) {

      ScrollBar = CreateControl (L"SCROLLBAR", This);
      ScrollBar->Class->SetAttribute (ScrollBar, L"hor", L"true");
      This->HScrollBar = (UI_SCROLLBAR *) ScrollBar;
    }
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"hsvadjust") == 0) {
    This->HsvDiff.HueDiff         = (INT16) wcstol (Value, &PtrValue, 10);
    This->HsvDiff.SaturationDiff  = (INT8) wcstol (PtrValue + 1, &PtrValue, 10);
    This->HsvDiff.ValueDiff       = (INT8) wcstol (PtrValue + 1, &PtrValue, 10);
  } else if (StrCmp (Name, L"min-height") == 0) {
    This->MinSize.cy  =  wcstol (Value, &PtrValue, 10);
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"max-width") == 0) {
    This->MaxSize.cx  =  wcstol (Value, &PtrValue, 10);
    UiNeedUpdatePos (Control);
  } else if (StrCmp (Name, L"taborder") == 0) {
    This->TabOrder = wcstol (Value, &PtrValue, 10);
  } else if (StrCmp (Name, L"tabstop") == 0) {
    if (StrCmp (Value, L"true") == 0) {
      SetWindowLongPtr (This->Wnd, GWL_STYLE, GetWindowLongPtr (This->Wnd, GWL_STYLE) | WS_TABSTOP);
    } else {
      SetWindowLongPtr (This->Wnd, GWL_STYLE, GetWindowLongPtr (This->Wnd, GWL_STYLE) & (~WS_TABSTOP));
    }
  } else if (StrCmp (Name, L"position") == 0) {
    if (StrCmp (Value, L"relative") == 0) {
      This->Position = UI_POSITION_RELATIVE;
    } else if (StrCmp (Value, L"absolute") == 0) {
      This->Position = UI_POSITION_ABSOLUTE;
    }
  } else if (StrCmp (Name, L"z-index") == 0) {
    if (isalpha (Value[0])) {
      if (StrCmp (Value, L"auto") == 0) {
        This->ZIndex.Type = UI_Z_INDEX_TYPE_AUTO;
      } else {
        DEBUG ((EFI_D_ERROR, "z-index can only use auto or const value"));
        ASSERT (FALSE);
      }
    } else {
      This->ZIndex.Value = wcstol (Value, &PtrValue, 10);
      This->ZIndex.Type  = UI_Z_INDEX_TYPE_NUMBER;
    }
  } else if (StrCmp (Name, L"box-sizing") == 0) {
    if (StrCmp (Value, L"border-box") == 0) {
      This->BoxSizing = UI_BOX_SIZING_BORDER_BOX;
    } else if (StrCmp (Value, L"content-box") == 0) {
      This->BoxSizing = UI_BOX_SIZING_CONTENT_BOX;
    } else {
      ASSERT (FALSE);
    }
  } else {
    DEBUG ((EFI_D_ERROR, "Unsupported attribute name/name: %s %s", Name, Value));
    ASSERT (0);
    return FALSE;
  }

  CONTROL_CLASS_INVALIDATE (This);

  return TRUE;
}

BOOLEAN
EFIAPI
UiControlAddChild (
  UI_CONTROL                    *Control,
  UI_CONTROL                    *Child
  )
{
  UI_CONTROL                    *This;

  ASSERT (Control != NULL);

  This = (UI_CONTROL *) Control;
  This->ItemCount++;
  This->Items = ReallocatePool (
                  sizeof (UI_CONTROL *) * (This->ItemCount - 1),
                  sizeof (UI_CONTROL *) * This->ItemCount,
                  This->Items
                  );
  if (This->Items == NULL) {
    return FALSE;
  }
  This->Items[This->ItemCount - 1] = Child;

  UiNeedUpdatePos (Child);

  return TRUE;
}

BOOLEAN
EFIAPI
UiControlRemoveChild (
  UI_CONTROL                    *Control,
  UI_CONTROL                    *Child
  )
{

  UINTN                         Index;
  UINTN                         Index2;
  UI_CONTROL                    *This;

  ASSERT (Control != NULL);
  This = (UI_CONTROL *) Control;

  for (Index = 0;  Index < This->ItemCount; Index++) {
    if (This->Items[Index] == Child) {
      for (Index2 = Index; Index < This->ItemCount - 1; Index2++) {
        This->Items[Index2] = This->Items[Index2 + 1];
      }
      This->ItemCount--;
      DestroyWindow (Child->Wnd);
      break;
    }
  }

  UiNeedUpdatePos (This);

  return TRUE;
}



BOOLEAN
EFIAPI
UiControlRemoveAllChild (
  UI_CONTROL                    *Control
  )
{
  UINTN Index;

  if (Control->ItemCount == 0) {
    return TRUE;
  }

  for (Index = 0; Index < Control->ItemCount; Index++) {
    DestroyWindow (Control->Items[Index]->Wnd);
  }

  FreePool (Control->Items);
  Control->Items     = NULL;
  Control->ItemCount = 0;

  UiNeedUpdatePos (Control);
  return TRUE;
}

UI_CONTROL *
EFIAPI
UiControlFindChildByName (
  UI_CONTROL                    *Control,
  CHAR16                        *Name,
  UI_SEARCH_TYPE                SearchType
  )
{
  UINTN                         Index;
  UI_CONTROL                    *This;
  UI_CONTROL                    *Child;

  This = (UI_CONTROL *) Control;

  for (Index = 0; Index < This->ItemCount; Index++) {
    if (StrCmp (This->Items[Index]->Name, Name) == 0) {
      return This->Items[Index];
    }
    if (SearchType == UI_SEARCH_TYPE_ONLY_CHILD) {
      continue;
    }

    Child = CONTROL_CLASS (This->Items[Index])->FindChildByName (This->Items[Index], Name, SearchType);
    if (Child != NULL) {
      return Child;
    }
  }

  return NULL;
}

BOOLEAN
UiControlActivate (
  UI_CONTROL *This
  )
{

  UINT32 Style;

  Style = (UINT32) GetWindowLongPtr (This->Wnd, GWL_STYLE);
  if ((Style & WS_DISABLED) != 0) {
    return FALSE;
  }

  if ((Style & WS_VISIBLE) == 0) {
    return FALSE;
  }

  return TRUE;
}

SIZE
UiControlEstimateSize (
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
  INTN                          MaxHeight;

  if (((Control->Width.Type  != UI_LENGTH_TYPE_WRAP_CONTENT) && (Control->Width.Type  != UI_LENGTH_TYPE_MATCH_PARENT)) &&
      ((Control->Height.Type != UI_LENGTH_TYPE_WRAP_CONTENT) && (Control->Height.Type != UI_LENGTH_TYPE_MATCH_PARENT))) {
    return CalculateControlDisplaySize (Control, AvailableSize);
  }

  Count = Control->ItemCount;

  ItemAvailableSize.cx = AvailableSize.cx - GetControlPaddingWidth (Control) - GetControlBorderWidth (Control);
  ItemAvailableSize.cy = AvailableSize.cy - GetControlPaddingHeight (Control) - GetControlBorderHeight (Control);

  MaxWidth = 0;
  MaxHeight = 0;
  for (Index = 0; Index < Count; ++Index) {

    Item = Control->Items[Index];
    if ((GetWindowLongPtr (Item->Wnd, GWL_STYLE) & WS_VISIBLE) == 0) {
      continue;
    }
    if (Item->Float) {
      continue;
    }

    ItemSize = CONTROL_CLASS (Item)->EstimateSize (Item, ItemAvailableSize);

    MaxWidth = MAX (MaxWidth, ItemSize.cx);
    MaxHeight = MAX (MaxHeight, ItemSize.cy);
  }
  if (MaxWidth > 0) {
    MaxWidth += (GetControlPaddingWidth (Control)  + GetControlBorderWidth (Control));
  }
  if (MaxHeight > 0) {
    MaxHeight += (GetControlPaddingHeight (Control) + GetControlBorderHeight (Control));
  }

  MaxWidth = MAX (MaxWidth, Control->MinSize.cx);
  MaxHeight = MAX (MaxHeight, Control->MinSize.cy);

  Size = CalculateControlDisplaySize (Control, AvailableSize);
  if (Size.cx == WRAP_CONTENT) {
    Size.cx = (GDICOORD)MaxWidth;
  }

  if (Size.cy == WRAP_CONTENT) {
    Size.cy = (GDICOORD)MaxHeight;
  }

  return Size;
}

VOID
UiControlInvalidate (
  UI_CONTROL *Control
  )
{
  RECT Rc;
  RECT InvalidateRc;
  RECT TmpRc;
  RECT ParentRc;
  HWND Parent;

  if (CONTROL_CLASS_GET_STATE(Control) & UISTATE_INVALIDATED) {
    //
    // check update region already in invalidate region
    //
    if (GetUpdateRect (Control->Manager->MainWnd, &InvalidateRc, FALSE)) {
      GetWindowRect (Control->Wnd, &Rc);
      MapWindowPoints(NULL, Control->Manager->MainWnd, (POINT *)&Rc, 2);
      if ((InvalidateRc.left <= Rc.left) && (InvalidateRc.right >= Rc.right) &&
          (InvalidateRc.top <= Rc.top)   && (InvalidateRc.bottom >= Rc.bottom)){
        return ;

      }
    }
  }

  CONTROL_CLASS_SET_STATE(Control, UISTATE_INVALIDATED, 0);
  GetWindowRect (Control->Wnd, &Rc);

  Parent = Control->Wnd;
  CopyRect (&InvalidateRc, &Rc);
  while ((Parent = Parent->parent) != NULL) {
    CopyRect (&TmpRc, &InvalidateRc);
    GetWindowRect (Parent, &ParentRc);

    if (!IntersectRect (&InvalidateRc, &TmpRc, &ParentRc)) {
      return ;
    }

    if (Parent == Control->Manager->Root) {
      break;
    }
  }

  MapWindowPoints(NULL, Control->Manager->MainWnd, (POINT *)&InvalidateRc, 2);
  InvalidateRect (Control->Manager->MainWnd, &InvalidateRc, FALSE);
}


UI_CONTROL*
UiControlFindNextFocus (
  UI_CONTROL *Control,
  UI_CONTROL *Focused,
  UINTN      Direction
  )
{
  UI_CONTROL   *Root;
  UINTN        Index;
  UI_CONTROL   *Item;
  RECT         ItemRc;
  RECT         FocusRc;
  UI_CONTROL   *NextFocus;
  UINT32       Distance;
  UINT32       BestDistance;


  ASSERT (Direction == VK_LEFT || Direction == VK_RIGHT || Direction == VK_UP || Direction == VK_DOWN);

  if (!(GetWindowLongPtr (Control->Wnd, GWL_STYLE) & WS_CHILD)) {
    return NULL;
  }

  Root = GetUiControl (GetParent (Control->Wnd));
  if (Root == NULL) {
    return NULL;
  }

  GetWindowRect (Focused->Wnd, &FocusRc);

  //
  // search next focus
  //
  NextFocus = NULL;
  BestDistance = (UINT32)-1;
  for (Index = 0; Index < Root->ItemCount; Index++) {
    Item = Root->Items[Index];
    if (Item == Focused) {
      continue;
    }
    if (!IsWindowVisible (Item->Wnd)) {
      continue;
    }
    if (GetWindowLongPtr (Item->Wnd, GWL_EXSTYLE) & WS_EX_NOACTIVATE) {
      continue;
    }
    GetWindowRect (Item->Wnd, &ItemRc);

    if (Direction == VK_LEFT) {
      if (ItemRc.left >= FocusRc.left || ((ItemRc.bottom <= FocusRc.top || ItemRc.top >= FocusRc.bottom) && ItemRc.right >= FocusRc.left)) {
        continue;
      }
    } else if (Direction == VK_UP) {
      if (ItemRc.top >= FocusRc.top || ((ItemRc.right <= FocusRc.left || ItemRc.left >= FocusRc.right) && ItemRc.bottom >= FocusRc.top)) {
        continue;
      }
    } else if (Direction == VK_RIGHT) {
      if (ItemRc.left <= FocusRc.left || ((ItemRc.bottom <= FocusRc.top || ItemRc.top >= FocusRc.bottom) && ItemRc.left <= FocusRc.right)) {
        continue;
      }
    } else if (Direction == VK_DOWN) {
      if (ItemRc.top <= FocusRc.top || ((ItemRc.right <= FocusRc.left || ItemRc.left >= FocusRc.right) && ItemRc.top <= FocusRc.bottom)) {
        continue;
      }
    }
    Distance = (FocusRc.left - ItemRc.left) * (FocusRc.left - ItemRc.left) +
               (FocusRc.top - ItemRc.top) * (FocusRc.top - ItemRc.top);
    if (Distance < BestDistance) {
      NextFocus    = Item;
      BestDistance = Distance;
    }
  }

  return NextFocus;

}

UI_CONTROL*
UiControlGetParent (
  UI_CONTROL *Control
  )
{
  HWND       Hwnd;
  UI_CONTROL *Parent;

  Hwnd = GetParent (Control->Wnd);
  ASSERT (Hwnd != NULL);

  Parent = GetUiControl (Hwnd);
  ASSERT (Parent != NULL);

  return Parent;
}

BOOLEAN
UiControlShouldDelayChildPressedState (
  UI_CONTROL                    *Control
  )
{
  if (Control->VScrollBar != NULL || Control->HScrollBar != NULL) {
    return TRUE;
  }
  return FALSE;
}


BOOLEAN
UiControlIsInScrollControl (
  UI_CONTROL *This
  )
{
  UI_CONTROL *Parent;

  Parent = CONTROL_CLASS (This)->GetParent (This);

  while (Parent != NULL) {
    if (CONTROL_CLASS (Parent)->ShouldDelayChildPressedState (Parent)) {
      return TRUE;
    }
    Parent = CONTROL_CLASS (Parent)->GetParent (Parent);
  }

  return FALSE;
}


LRESULT
UiControlProc (
  HWND   Wnd,
  UINT32 Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  UI_CONTROL                    *This;
  UI_CONTROL                    *ParentControl;
  UI_CONTROL                    *NextFocus;
  HWND                           ParentWnd;

  RECT                          Rc;
  UI_MANAGER                    *Manager;

  POINT                         Point;
  POINT                         LastPoint;
  INT32                         DeltaX;
  INT32                         DeltaY;
  INT32                         ScrollPos;

  This = NULL;
  Manager = NULL;

  if (Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    This = (UI_CONTROL *) GetWindowLongPtr (Wnd, 0);
    if (This == NULL) {
      ASSERT (FALSE);
      return 0;
    }
  }

  if (This != NULL) {
    Manager = This->Manager;
  }

  switch (Msg) {

  case WM_CREATE:
    if (This == NULL) {
      This     = (UI_CONTROL *) AllocateZeroPool (sizeof (UI_CONTROL));
    } else {
      ASSERT (FALSE);
    }
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Wnd, 0);
      SetWindowLongPtr (Wnd, 0, (INTN)This);
      SendMessage (Wnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    if (LParam == 0) {
      Manager = NULL;
    } else {
      Manager = (UI_MANAGER *)(((CREATESTRUCT *)(LParam))->lpCreateParams);
    }
    UiControlOnUiCreate (This, Wnd, Manager);
    break;

  case UI_NOTIFY_ACTIVATE:
    return UiControlActivate (This);

  case WM_DESTROY:
    UiControlOnDestroy (This);
    break;

  case WM_WINDOWPOSCHANGED:
    break;

  case WM_PAINT:
//    ValidateRect (Wnd, NULL);
    break;

  case WM_NCHITTEST:
    return HTTRANSPARENT;

  case UI_NOTIFY_PAINT:
    UiControlOnUiPaint (This, WParam, LParam);
    break;

  case WM_KEYDOWN:
    if (WParam == VK_RETURN) {
      if (!IsWindowVisible (Wnd)) {
        break;
      }

      if (!(This->StyleFlags & UISTYLE_CLICKABLE)) {
        break;
      }

      CONTROL_CLASS_SET_STATE (This, UISTATE_PRESSED, 0);
    }

  //
  // we didn't send WM_KEYUP, so continue process WM_KEYUP
  //
  //
  case WM_KEYUP:
    if (WParam == VK_LEFT || WParam == VK_RIGHT
        || WParam == VK_UP || WParam == VK_DOWN) {

      NextFocus = This->Class->FindNextFocus (This, This, WParam);
      if (NextFocus != NULL) {
        SetFocus (NextFocus->Wnd);
      }
      break;
    }


    if (WParam == VK_RETURN) {
      if (!IsWindowVisible (Wnd)) {
        break;
      }

      if (!(This->StyleFlags & UISTYLE_CLICKABLE)) {
        break;
      }

      if (!(CONTROL_CLASS_GET_STATE (This) & UISTATE_PRESSED)) {
        break;
      }

      CONTROL_CLASS_SET_STATE(This, 0, UISTATE_PRESSED);
      SendMessage (Wnd, UI_NOTIFY_ACTIVATE, 0, 0);
    }
    break;

  case WM_LBUTTONDOWN:
    if ((This->HScrollBar != NULL) || (This->VScrollBar != NULL)) {
      //
      // orignal Container
      //
      This->IsBeginDraged = TRUE;
      POINTSTOPOINT(Point, LParam);
      ClientToScreen (Wnd, &Point);
      This->LastMotionX = Point.x;
      This->LastMotionY = Point.y;
      SetCapture (Wnd);
      if ((GetWindowLongPtr (Wnd, GWL_STYLE) & WS_DISABLED) == 0) {
        CONTROL_CLASS_SET_STATE(This, UISTATE_PRESSED | UISTATE_CAPTURED, 0);
      }

    } else {
      POINTSTOPOINT(Point, LParam);
      ClientToScreen (Wnd, &Point);
      if ((GetWindowLongPtr (Wnd, GWL_STYLE) & WS_DISABLED) == 0) {
        SetCapture (Wnd);
        CONTROL_CLASS_SET_STATE (This, UISTATE_PRESSED | UISTATE_CAPTURED, 0);
      }

      if (UiControlIsInScrollControl (This)) {
        CONTROL_CLASS_SET_STATE(This, UISTATE_PREPRESSED, 0);
        This->LastX = Point.x;
        This->LastY = Point.y;
      }
    }
    break;

  case WM_LBUTTONDBLCLK:
    return SendMessage (Wnd, WM_LBUTTONDOWN, WParam, LParam);

  case WM_NCMOUSEMOVE:

    if (This->IsBeginDraged) {
      POINTSTOPOINT(Point, LParam);
      DeltaX = This->LastMotionX - Point.x;
      DeltaY = This->LastMotionY - Point.y;

      if (abs(DeltaY) < This->TouchSlop) {
        break;
      }

      This->LastMotionX = Point.x;
      This->LastMotionY = Point.y;
      //
      // now only process y
      //


      if (DeltaY < 0) {
        if (This->ScrollPos.y <= 0) {
          break;
        }
      } else if (DeltaY > 0) {
  //     DeltaY =
      }

      if (This->VScrollBar != NULL) {
        ScrollPos = (This->ScrollPos.y + DeltaY) / This->VScrollBar->LineSize;
        SCROLLBAR_CLASS(This->VScrollBar)->SetScrollPos (This->VScrollBar, ScrollPos);
        This->Dragged = TRUE;
        //SCROLLBAR_CLASS(This->VScrollBar)->SetScrollPos (This->VScrollBar, This->ScrollPos.y + DeltaY);
      }
    } else if (CONTROL_CLASS_GET_STATE (This) & UISTATE_PREPRESSED) {
      POINTSTOPOINT(Point, LParam);
      GetWindowRect (Wnd, &Rc);
      if (!PtInRect (&Rc, Point) || abs(This->LastY - Point.y) > 10) {
        //
        // change capture to parent
        //
        CONTROL_CLASS_SET_STATE (This, 0, UISTATE_PRESSED | UISTATE_CAPTURED | UISTATE_PREPRESSED);
        ParentControl = This->Class->GetParent (This);
        ASSERT (ParentControl != NULL);

        ParentWnd = ParentControl->Wnd;
        SetCapture (ParentWnd);
        LastPoint.x = This->LastX;
        LastPoint.y = This->LastY;
        ScreenToClient (ParentWnd, &LastPoint);
        SendMessage (ParentWnd, WM_LBUTTONDOWN, 0, MAKELONG(LastPoint.x, LastPoint.y));
        SendMessage (ParentWnd, WM_NCMOUSEMOVE, 0, MAKELONG(Point.x, Point.y));
      }
    }
    break;

  case WM_NCLBUTTONUP:
    break;

  case WM_MOUSELEAVE:
  case WM_LBUTTONUP:
    This->Dragged = FALSE;
    if (This->IsBeginDraged) {
      ReleaseCapture ();
      This->IsBeginDraged = FALSE;
    } else if (GetCapture () == Wnd) {
      CONTROL_CLASS_SET_STATE (This, 0, UISTATE_PRESSED | UISTATE_CAPTURED | UISTATE_PREPRESSED);
      ReleaseCapture ();
    }
    break;

  case WM_SETFOCUS:
    CONTROL_CLASS_SET_STATE (This, UISTATE_FOCUSED, 0);
    break;

  case WM_KILLFOCUS:
    CONTROL_CLASS_SET_STATE (This, 0, UISTATE_FOCUSED);
    break;

  case WM_ERASEBKGND:
    return 1;

  default:
    return DefWindowProc (Wnd, Msg, WParam, LParam);
  }

  return 0;

}


UI_CONTROL_CLASS *
GetControlClass (
  VOID
  )
{
  STATIC UI_CONTROL_CLASS       *Class = NULL;

  if (Class != NULL) {
    return Class;
  }

  Class                  = AllocateZeroPool (sizeof (UI_CONTROL_CLASS));
  if (Class == NULL) {
    return NULL;
  }

  Class->Parent          = NULL;
  Class->ClassSize       = sizeof (*Class);
  Class->ClassName       = StrDuplicate (L"Control");

  Class->FindChildByName = UiControlFindChildByName;
  Class->AddChild        = UiControlAddChild;
  Class->RemoveChild     = UiControlRemoveChild;
  Class->RemoveAllChild  = UiControlRemoveAllChild;

  Class->WndProc         = UiControlProc;
  Class->SetState        = UiControlSetState;
  Class->GetState        = UiControlGetState;
  Class->SetAttribute    = UiControlSetAttribute;
  Class->SetPosition     = UiControlSetPosition;
  Class->EstimateSize    = UiControlEstimateSize;
  Class->Invalidate      = UiControlInvalidate;
  Class->FindNextFocus   = UiControlFindNextFocus;
  Class->GetParent       = UiControlGetParent;
  Class->ShouldDelayChildPressedState = UiControlShouldDelayChildPressedState;


  return Class;
}


