/** @file
  UI Common Controls

;******************************************************************************
;* Copyright (c) 2014 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "H2OControls.h"
#include "H2ODisplayEngineLocalMetro.h"
#include "MetroUi.h"

STATIC H2O_SETUP_MENU_ITEM_CLASS  *mSetupMenuItemClass = NULL;
#define CURRENT_CLASS            mSetupMenuItemClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
//CHAR16 *mSetupMenuItemChilds = L""
//  L"<VerticalLayout background-color='0xFF000000'>"
//    L"<Texture name='bkimage' background-image-style='stretch' float='true' height='match_parent' width='match_parent' background-image='@MenuBackgroundNormal'/>"
//    L"<VerticalLayout padding='30, 30, 30, 30' width='match_parent' float='true' height='match_parent'>"
//      L"<Texture name='image' background-image-style='stretch|light'/>"
//    L"</VerticalLayout>"
//    L"<VerticalLayout/>"
//    L"<Label name='text' text-align='center' height='33' padding='4,0,8,0' font-size='20' textcolor='0xFFDDDDDD'/>"
//  L"</VerticalLayout>";
CHAR16 *mSetupMenuItemChilds = NULL;
#endif

VOID
H2OSetupMenuItemUpdateVfcfOrder (
  IN UI_CONTROL                        *Order,
  IN SETUP_MENU_INFO                   *SetupMenuInfo
  )
{
  UINTN                                Index;
  UI_CONTROL                           *Child;
  CHAR16                               Str[20];
  SETUP_MENU_INFO                      CurrentSetupMenuInfo;
  EFI_STATUS                           Status;
  UINTN                                NameLen;

  if (Order == NULL || SetupMenuInfo == NULL) {
    return;
  }
  if (SetupMenuInfo->PageImage == NULL && SetupMenuInfo->PageTitle == NULL) {
    return;
  }

  for (Index = 0; Index < Order->ItemCount; Index++) {
    Child = Order->Items[Index];
    if (Child->Name == NULL) {
      continue;
    }

    NameLen = StrLen(Child->Name);

    if (STR_MATCH(Child->Name, NameLen, L"formset-image")) {
      if (SetupMenuInfo->PageImage == NULL) {
        continue;
      }

      UnicodeSPrint (Str, sizeof (Str), L"0x%p",  SetupMenuInfo->PageImage);
      UiSetAttribute (Child, L"background-image", Str);
      Status = GetSetupMenuInfoByPage (gFB->CurrentP, &CurrentSetupMenuInfo);
      if (EFI_ERROR (Status) || SetupMenuInfo->PageId != CurrentSetupMenuInfo.PageId) {
        UiSetAttribute (Child, L"background-image-style", L"light");
      } else if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
        AdjustImageToMenuColor (Child);
      }
    } else if (STR_MATCH(Child->Name, NameLen, L"formset-title")) {
      if (SetupMenuInfo->PageTitle == NULL) {
        continue;
      }

      UiSetAttribute (Child, L"text", SetupMenuInfo->PageTitle);
    } else if (STR_MATCH(Child->Name, NameLen, L"container")) {
      Child = CONTROL_CLASS(Child)->FindChildByName (Child, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Child == NULL) {
        continue;
      }

      H2OSetupMenuItemUpdateVfcfOrder (Child, SetupMenuInfo);
    }
  }
}

VOID
EFIAPI
H2OSetupMenuItemSetPosition (
  UI_CONTROL                    *Control,
  CONST RECT                    *Pos
  )
{
  PARENT_CONTROL_CLASS(CURRENT_CLASS)->SetPosition (Control,Pos);
}

VOID
EFIAPI
H2OSetupMenuItemSetState (
  UI_CONTROL                    *Control,
  UI_STATE                      SetState,
  UI_STATE                      ClearState
  )
{
  UI_CONTROL                    *Child;

  PARENT_CONTROL_CLASS(CURRENT_CLASS)->SetState (Control, SetState, ClearState);

  Child = UiFindChildByName (Control, L"HotBkg");
  if (Child == NULL) {
    return;
  }

  if ((SetState & UISTATE_SELECTED) == UISTATE_SELECTED) {
    UiSetAttribute (Child, L"visibility", L"true");
  } else if ((ClearState & UISTATE_SELECTED) == UISTATE_SELECTED) {
    UiSetAttribute (Child, L"visibility", L"false");
  }
}

BOOLEAN
EFIAPI
H2OSetupMenuItemSetAttribute (
  UI_CONTROL *Control,
  CHAR16     *Name,
  CHAR16     *Value
  )
{
  H2O_SETUP_MENU_ITEM            *This;
  EFI_STATUS                    Status;
  UI_CONTROL                    *Child;
  SETUP_MENU_INFO               *SetupMenuInfo;
  SETUP_MENU_INFO               CurrentSetupMenuInfo;
  CHAR16                        Str[20];

  This = (H2O_SETUP_MENU_ITEM *) Control;

  if (StrCmp (Name, L"setupmenuinfo") == 0) {
    SetupMenuInfo = (SETUP_MENU_INFO *)(UINTN) StrToUInt (Value, 16, &Status);
    This->SetupMenuInfo = SetupMenuInfo;

    if (mSetupMenuItemChilds != NULL) {
      if (SetupMenuInfo->PageImage != NULL) {
        Child = UiFindChildByName (This, L"image");
        UnicodeSPrint (Str, sizeof (Str), L"0x%p",  SetupMenuInfo->PageImage);
        UiSetAttribute (Child, L"background-image", Str);
        UiSetAttribute (Child, L"background-image-style", L"stretch");
        Status = GetSetupMenuInfoByPage (gFB->CurrentP, &CurrentSetupMenuInfo);
        if (EFI_ERROR (Status) || SetupMenuInfo->PageId != CurrentSetupMenuInfo.PageId) {
          UiSetAttribute (Child, L"background-image-style", L"stretch|light");
        } else if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
          AdjustImageToMenuColor (Child);
        }
      }

      Child = UiFindChildByName (This, L"text");
      UiSetAttribute (Child, L"text", SetupMenuInfo->PageTitle);
    }
  } else {
    return PARENT_CLASS_SET_ATTRIBUTE (CURRENT_CLASS, Control, Name, Value);
  }

  CONTROL_CLASS_INVALIDATE (This);

  return TRUE;
}

LRESULT
EFIAPI
H2OSetupMenuItemProc (
  HWND   Wnd,
  UINT32 Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  H2O_SETUP_MENU_ITEM            *This;
  UI_CONTROL                     *Control;

  This = (H2O_SETUP_MENU_ITEM *) GetWindowLongPtr (Wnd, 0);
  if (This == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  Control = (UI_CONTROL *)This;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_SETUP_MENU_ITEM *) AllocateZeroPool (sizeof (H2O_SETUP_MENU_ITEM));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Wnd, 0);
      SetWindowLongPtr (Wnd, 0, (INTN)This);
      SendMessage (Wnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, UI_NOTIFY_CREATE, WParam, LParam);
    if (mSetupMenuItemChilds != NULL) {
      XmlCreateControl (mSetupMenuItemChilds, Control);
    }
    break;

  case UI_NOTIFY_PAINT:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

    if (WParam == PAINT_ALL &&
        (CONTROL_CLASS_GET_STATE (This) & (UISTATE_SELECTED | UISTATE_FOCUSED)) == (UISTATE_SELECTED | UISTATE_FOCUSED)) {
      COLORREF                  Color;
      UI_MANAGER                *Manager;
      RECT                      Rc;
      HDC                       Hdc;
      UI_LIST_VIEW              *ListView;
      
      //
      // Horizontal Setup Menu don't need display focus red box
      //
      ListView = (UI_LIST_VIEW *) UiFindChildByName ((UI_CONTROL *)GetWindowLongPtr(gWnd, 0), L"H2OSetupMenuPanel");
      if (ListView != NULL && ListView->Layout == UiListViewHorizontalLayout) {
        break;
      }

      if (PcdGet32(PcdH2OLmdeMultiLayout) == 0) {
        Color = 0xFFFF0000;
      } else {
        Color = 0xFFFFFFFF;
      }
      Manager = Control->Manager;
      Hdc     = Manager->PaintDC;

      Manager->GetControlRect (Manager, Control, &Rc);

      SelectObject (Hdc, GetStockObject (PS_NULL));
      SelectObject (Hdc, GetStockObject (DC_PEN));
      SetDCPenColor (Hdc, Color);
      Rectangle (Hdc, Rc.left, Rc.top, Rc.right, Rc.bottom);
   }
   break;

  case WM_NCHITTEST:
    return HTTRANSPARENT;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }

  return 0;
}

H2O_SETUP_MENU_ITEM_CLASS *
EFIAPI
GetSetupMenuItemClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"SetupMenuItem", (UI_CONTROL_CLASS *)GetControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = H2OSetupMenuItemProc;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetAttribute = H2OSetupMenuItemSetAttribute;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetPosition  = H2OSetupMenuItemSetPosition;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetState     = H2OSetupMenuItemSetState;

  return CURRENT_CLASS;
}

