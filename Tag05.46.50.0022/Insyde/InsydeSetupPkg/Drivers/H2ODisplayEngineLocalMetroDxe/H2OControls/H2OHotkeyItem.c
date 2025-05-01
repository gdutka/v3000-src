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

STATIC H2O_HOTKEY_ITEM_CLASS    *mHotkeyItemClass = NULL;
#define CURRENT_CLASS           mHotkeyItemClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
//CHAR16 *mHotkeyItemChilds = L""
//  L"<VerticalLayout padding='2,2,2,2' name='ItemLayout' height='-1'>"
//    L"<HorizontalLayout name='MarkLayout' height='50'>"
//      L"<Label name='MarkLayoutLeft'/>"
//      L"<HorizontalLayout name='HotkeyLayout' height='48'/>"
//      L"<Label name='MarkLayoutRight'/>"
//    L"</HorizontalLayout>"
//    L"<Label name='HelpText' text-align='center' height='23' padding='4,0,4,0' font-size='15' textcolor='0xFF999999'/>"
//    L"<Label name='HelpTextBottom'/>"
//  L"</VerticalLayout>";
CHAR16 *mHotkeyItemChilds = NULL;
#endif

VOID
UpdateHotkeyItem (
  IN OUT H2O_HOTKEY_ITEM                *This
  )
{
  HOT_KEY_INFO                         *HotkeyInfo;
  UI_CONTROL                           *HotkeyLayout;
  UI_CONTROL                           *HotkeyChild;
  UI_CONTROL                           *HotkeyImage;
  UI_CONTROL                           *HotkeyMarkText;
  UI_CONTROL                           *HelpText;
  CHAR16                               Str[20];


  HotkeyInfo = This->HotkeyInfo[This->NumberOfHotkey - 1];
  ASSERT (HotkeyInfo != NULL);
  if (HotkeyInfo == NULL) {
    return;
  }

  //
  // Create hotkey-image and hotkey-mark control in HotkeyLayout
  //
  HotkeyLayout = UiFindChildByName (This, L"HotkeyLayout");
  UnicodeSPrint (Str, sizeof (Str), L"%d", This->NumberOfHotkey * 47);
  UiSetAttribute (HotkeyLayout, L"width", Str);

  HotkeyChild = CreateControl (L"Control", HotkeyLayout);
  CONTROL_CLASS(HotkeyLayout)->AddChild (HotkeyLayout, HotkeyChild);
  UiApplyAttributeList (HotkeyChild, L"name='HotkeyChild' width='47'");

  HotkeyImage = CreateControl (L"Button", HotkeyChild);
  CONTROL_CLASS(HotkeyChild)->AddChild (HotkeyChild, HotkeyImage);
  UiApplyAttributeList (HotkeyImage, L"name='HotkeyImage' width='47' background-color='0x0'");
  SetWindowLongPtr (HotkeyImage->Wnd, GWL_EXSTYLE, GetWindowLongPtr (HotkeyImage->Wnd, GWL_EXSTYLE) | WS_EX_NOACTIVATE);

  HotkeyMarkText = CreateControl (L"Button", HotkeyChild);
  CONTROL_CLASS(HotkeyChild)->AddChild (HotkeyChild, HotkeyMarkText);
  UiApplyAttributeList (HotkeyMarkText, L"name='HotkeyMarkText' width='47' text-align='center' font-size='15' textcolor='0xFFFFFFFF' float='true'");
  SetWindowLongPtr (HotkeyMarkText->Wnd, GWL_EXSTYLE, GetWindowLongPtr (HotkeyMarkText->Wnd, GWL_EXSTYLE) | WS_EX_NOACTIVATE);

  //
  // Set attribute by HotkeyInfo
  //
  SetWindowLongPtr (HotkeyImage->Wnd, GWLP_USERDATA, (INTN) HotkeyInfo);
  SetWindowLongPtr (HotkeyMarkText->Wnd, GWLP_USERDATA, (INTN) HotkeyInfo);

  if (HotkeyInfo->ImageBuffer != NULL) {
    UnicodeSPrint (Str, sizeof (Str), L"0x%p", HotkeyInfo->ImageBuffer);
    UiSetAttribute (HotkeyImage, L"background-image", Str);
    UiSetAttribute (HotkeyMarkText, L"visibility", L"false");
  }

  if (HotkeyInfo->Mark != NULL) {
    UiSetAttribute (HotkeyMarkText, L"text", HotkeyInfo->Mark);
  }

  if (HotkeyInfo->String != NULL) {
    HelpText = UiFindChildByName (This, L"HelpText");
    UiSetAttribute (HelpText, L"text", HotkeyInfo->String);
  }

  return;
}

VOID
H2OHotkeyItemUpdateVfcfOrder (
  IN UI_CONTROL                        *Order,
  IN HOT_KEY_INFO                      *HotkeyInfo
  )
{
  UINTN                                Index;
  UI_CONTROL                           *Child;
  CHAR16                               Str[20];
  UI_CONTROL                           *Image;
  UINTN                                NameLen;
  SIZE                                 ButtonSize;
  UI_CONTROL                           *NewImageControl;
  CHAR16                               ButtonName[] = L"HotkeyMarkText";

  if (Order == NULL || HotkeyInfo == NULL) {
    return;
  }
  if (HotkeyInfo->ImageBuffer == NULL && HotkeyInfo->String == NULL && HotkeyInfo->Mark == NULL) {
    return;
  }

  NewImageControl = NULL;

  for (Index = 0; Index < Order->ItemCount; Index++) {
    if (Order->Items[Index]->Name == NULL) {
      continue;
    }

    NameLen = StrLen(Order->Items[Index]->Name);

    if (STR_MATCH(Order->Items[Index]->Name, NameLen, L"prompt")) {
      if (HotkeyInfo->String == NULL) {
        continue;
      }

      UiSetAttribute (Order->Items[Index], L"text", HotkeyInfo->String);
    } else if (STR_MATCH(Order->Items[Index]->Name, NameLen, L"text2")) {
      if (HotkeyInfo->Mark == NULL) {
        continue;
      }

      UiSetAttribute (Order->Items[Index], L"text", HotkeyInfo->Mark);
    } else if (STR_MATCH(Order->Items[Index]->Name, NameLen, L"image")) {
      if (HotkeyInfo->ImageBuffer == NULL) {
        continue;
      }

      Image = Order->Items[Index];

      ZeroMem (&ButtonSize, sizeof(ButtonSize));
      Child = CONTROL_CLASS(Image)->FindChildByName (Image, ButtonName, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Child == NULL) {
        ButtonSize = CalculateControlDisplaySize (Image, ButtonSize);
        if (ButtonSize.cx <= 0) {
          ButtonSize.cx = HotkeyInfo->ImageBuffer->Width;
        }
        if (ButtonSize.cy <= 0) {
          ButtonSize.cy = HotkeyInfo->ImageBuffer->Height;
        }
      } else {
        //
        // There are multiple hotkey in this image control.
        //
        ButtonSize = CalculateControlDisplaySize (Child, ButtonSize);
        NewImageControl = CreateControl (L"Control", Order);
        Image = NewImageControl;
      }
      UiApplyAttributeList (Image, L"width='wrap_content' height='wrap_content'");

      Child = CreateAddControl (L"Button", Image);
      UiSetAttribute (Child, L"name", ButtonName);

      UnicodeSPrint (Str, sizeof (Str), L"%dpx", ButtonSize.cx);
      UiSetAttribute (Child, L"width", Str);
      UnicodeSPrint (Str, sizeof (Str), L"%dpx", ButtonSize.cy);
      UiSetAttribute (Child, L"height", Str);
      UnicodeSPrint (Str, sizeof (Str), L"0x%p", HotkeyInfo->ImageBuffer);
      UiSetAttribute (Child, L"background-image", Str);

      SetWindowLongPtr (Child->Wnd, GWL_EXSTYLE, GetWindowLongPtr (Child->Wnd, GWL_EXSTYLE) | WS_EX_NOACTIVATE);
      SetWindowLongPtr (Child->Wnd, GWLP_USERDATA, (INTN) HotkeyInfo);
    } else if (STR_MATCH(Order->Items[Index]->Name, NameLen, L"container")) {
      Child = CONTROL_CLASS(Order->Items[Index])->FindChildByName (Order->Items[Index], VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Child == NULL) {
        continue;
      }

      H2OHotkeyItemUpdateVfcfOrder (Child, HotkeyInfo);
    }
  }

  if (NewImageControl != NULL) {
    CONTROL_CLASS(Order)->AddChild (Order, NewImageControl);
  }
}

BOOLEAN
EFIAPI
H2OHotkeyItemSetAttribute (
  IN OUT UI_CONTROL                    *Control,
  IN     CHAR16                        *Name,
  IN     CHAR16                        *Value
  )
{
  H2O_HOTKEY_ITEM                       *This;
  EFI_STATUS                           Status;

  This = (H2O_HOTKEY_ITEM *) Control;

  if (StrCmp (Name, L"hotkey") == 0) {
    This->HotkeyInfo[This->NumberOfHotkey] = (HOT_KEY_INFO *)(UINTN) StrToUInt (Value, 16, &Status);
    This->NumberOfHotkey ++;
    if (mHotkeyItemChilds != NULL) {
      UpdateHotkeyItem (This);
    }
  } else {
    return PARENT_CLASS_SET_ATTRIBUTE (CURRENT_CLASS, Control, Name, Value);
  }

  CONTROL_CLASS_INVALIDATE (This);

  return TRUE;
}


LRESULT
EFIAPI
H2OHotkeyItemProc (
  IN     HWND                          Hwnd,
  IN     UINT32                        Msg,
  IN     WPARAM                        WParam,
  IN     LPARAM                        LParam
  )
{
  H2O_HOTKEY_ITEM                       *This;
  UI_CONTROL                           *Control;

  This = (H2O_HOTKEY_ITEM *) GetWindowLongPtr (Hwnd, 0);
  if (This == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  Control = (UI_CONTROL *)This;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_HOTKEY_ITEM *) AllocateZeroPool (sizeof (H2O_HOTKEY_ITEM));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    if (mHotkeyItemChilds != NULL) {
      XmlCreateControl (mHotkeyItemChilds, Control);
    }
    break;

  case WM_NCHITTEST:
    return HTTRANSPARENT;
    break;

  case WM_DESTROY:
    KillTimer (Hwnd, 0);
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

H2O_HOTKEY_ITEM_CLASS *
EFIAPI
GetHotkeyItemClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"HotkeyItem", (UI_CONTROL_CLASS *) GetControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = H2OHotkeyItemProc;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetAttribute = H2OHotkeyItemSetAttribute;

  return CURRENT_CLASS;
}


