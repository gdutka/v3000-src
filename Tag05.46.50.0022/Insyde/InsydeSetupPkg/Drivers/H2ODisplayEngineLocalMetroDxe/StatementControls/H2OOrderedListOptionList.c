/** @file
  UI Option List Controls

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "H2ODisplayEngineLocalMetro.h"
#include "MetroUi.h"
#include "StatementControls.h"

STATIC H2O_ORDERED_LIST_OPTION_LIST_CLASS      *mOrderedListOptionListClass = NULL;
#define CURRENT_CLASS             mOrderedListOptionListClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mOrderedListOptionListChilds = L""
  L"<ListView moveitemsupport='true' name='OrderedListOptionList' width='match_parent' height='match_parent' vscrollbar='true' tabstop='true' child-padding='2' taborder='1'/>";

CHAR16 *mOrderedListOptionListOptionChilds = L""
  L"<HorizontalLayout width='-1' name='Option' height='40' background-color='0xFFE6E6E6'>"
    L"<Texture width='40' name='Icon' height='40'/>"
    L"<Label padding='7,0,7,0' text-align='center' textcolor='0xFF404040' font-size='14' name='Words'/>"
    L"<Texture height='40' width='40' background-image='@DialogSortIcon' name='SortIcon' background-image-style='stretch|gray'/>"
  L"</HorizontalLayout>";
#endif

STATIC
UINT16
GetSizeByHiiValueType (
  IN UINT8                      Type
  )
{
  UINT8                         Width;

  //
  // Size of HII value type is referenced from IfrParse of form browser
  //
  Width = 1;
  switch (Type) {

  case EFI_IFR_TYPE_NUM_SIZE_8:
    Width = 1;
    break;

  case EFI_IFR_TYPE_NUM_SIZE_16:
    Width = 2;
    break;

  case EFI_IFR_TYPE_NUM_SIZE_32:
    Width = 4;
    break;

  case EFI_IFR_TYPE_NUM_SIZE_64:
    Width = 8;
    break;

  default:
    break;
  }

  return Width;
}

STATIC
VOID
H2OOrderedListOptionListOptionOnSetState (
  UI_CONTROL                    *Control,
  UI_STATE                      SetState,
  UI_STATE                      ClearState
  )
{
  UI_CONTROL                    *ChildControl;

  if (StrCmp (Control->Name, L"Option") == 0) {
    ChildControl = Control;
  } else {
    ChildControl = UiFindChildByName (Control, L"Option");
  }
  if (ChildControl == NULL) {
    return;
  }

  if (SetState & UISTATE_SELECTED) {
    UiApplyAttributeList (ChildControl, L"background-color='@menulightcolor'");
  }

  if (ClearState & UISTATE_SELECTED) {
    UiApplyAttributeList (ChildControl, L"background-color='0xFFE6E6E6'");
  }
}

STATIC
EFI_STATUS
H2OOrderedListOptionListOnSendChangeQ (
  IN H2O_STATEMENT_CONTROL      *This
  )
{
  UI_CONTROL                    *OrderedListOptionList;
  EFI_HII_VALUE                 HiiValue;
  UINT8                         HiiValueType;
  UINT32                        Index;
  UI_CONTROL                    *ChildControl;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OrderedListOptionList = UiFindChildByName ((UI_CONTROL *)This, L"OrderedListOptionList");
  if (OrderedListOptionList == NULL) {
    return EFI_NOT_FOUND;
  }

  HiiValueType = This->Statement->Options[0].HiiValue.Type;

  ZeroMem (&HiiValue, sizeof (EFI_HII_VALUE));
  HiiValue.Type      = EFI_IFR_TYPE_BUFFER;
  HiiValue.BufferLen = (UINT16) (GetSizeByHiiValueType (HiiValueType) * OrderedListOptionList->ItemCount);
  HiiValue.Buffer    = AllocateZeroPool (HiiValue.BufferLen);
  if (HiiValue.Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < OrderedListOptionList->ItemCount; Index++) {
    ChildControl = OrderedListOptionList->Items[Index];
    SetHiiBufferValue (HiiValue.Buffer, HiiValueType, Index, (UINT64) GetWindowLongPtr (ChildControl->Wnd, GWLP_USERDATA));
  }

  This->ValueChanged = TRUE;
  CopyMem (&This->HiiValue, &HiiValue, sizeof (EFI_HII_VALUE));

  H2OStatementControlOnSendChangeQ (This);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CreateOrderedListOptionListControl (
  IN OUT UI_CONTROL              *Control
  )
{
  H2O_ORDERED_LIST_OPTION_LIST   *This;
  H2O_FORM_BROWSER_S             *Statement;
  UI_CONTROL                     *OrderedListOptionList;
  UINT8                          *ValueBuffer;
  UINT8                          ContainerCount;
  UINTN                          NumberOfOptions;
  H2O_FORM_BROWSER_O             *OptionArray;
  UINT64                         ContainerHiiValue;
  UINT32                         ContainerIndex;
  UINT32                         OptionIndex;
  H2O_FORM_BROWSER_O             *Option;
  UI_CONTROL                     *CreatedControl;
  UI_CONTROL                     *ChildControl;
  CHAR16                         Str[20];
  BOOLEAN                        Found;

  This = (H2O_ORDERED_LIST_OPTION_LIST *) Control;

  Statement = This->StatementControl.Statement;
  if (Statement == NULL) {
    return EFI_NOT_FOUND;
  }

  if (Statement == NULL ||
    Statement->NumberOfOptions == 0 ||
    Statement->Options == NULL) {
    return EFI_NOT_FOUND;
  }

  OrderedListOptionList = UiFindChildByName (Control, L"OrderedListOptionList");
  if (OrderedListOptionList == NULL) {
    return EFI_NOT_FOUND;
  }

  CONTROL_CLASS (This)->RemoveAllChild (OrderedListOptionList);

  if (OrderedListOptionList->VScrollBar != NULL) {
    UiSetAttribute (OrderedListOptionList->VScrollBar, L"displayscrollbar", L"false");
    CONTROL_CLASS_INVALIDATE (OrderedListOptionList);
  }

  Found           = FALSE;
  ValueBuffer     = Statement->HiiValue.Buffer;
  ContainerCount  = Statement->ContainerCount;
  NumberOfOptions = Statement->NumberOfOptions;
  OptionArray     = Statement->Options;

  if (ContainerCount == 0 || NumberOfOptions == 0 || ValueBuffer == NULL || OptionArray == NULL) {
    return EFI_NOT_FOUND;
  }

  for (ContainerIndex = 0; ContainerIndex < ContainerCount; ContainerIndex ++) {
    ContainerHiiValue = GetHiiBufferValue (ValueBuffer, OptionArray[0].HiiValue.Type, ContainerIndex);

    for (OptionIndex = 0; OptionIndex < NumberOfOptions; OptionIndex ++) {
      Option = &OptionArray[OptionIndex];
      if (Option->HiiValue.Value.u64 != ContainerHiiValue) {
        continue;
      }

      CreatedControl = XmlCreateControl (mOrderedListOptionListOptionChilds, OrderedListOptionList);
      if (CreatedControl == NULL) {
        continue;
      }
      CreatedControl->OnSetState = H2OOrderedListOptionListOptionOnSetState;
      SetWindowLongPtr (CreatedControl->Wnd, GWLP_USERDATA, (INTN) Option->HiiValue.Value.u64);

      //
      // Get Option button and set its text and bkimage
      //
      ChildControl = UiFindChildByName (CreatedControl, L"Words");
      UiSetAttribute (ChildControl, L"text", Option->Text);

      if (Option->Image != NULL) {
        UnicodeSPrint (Str, sizeof (Str), L"0x%p", Option->Image);
        ChildControl = UiFindChildByName (CreatedControl, L"Icon");
        UiSetAttribute (ChildControl, L"background-image", Str);
      }

      Found = TRUE;
      break;
    }
  }

  if (!Found) {
    return EFI_NOT_FOUND;
  }

  LIST_VIEW_CLASS (OrderedListOptionList)->SetSelection ((UI_LIST_VIEW *) OrderedListOptionList, (INT32)0, TRUE);

  SetFocus (OrderedListOptionList->Wnd);

  CONTROL_CLASS_INVALIDATE (This);

  return EFI_SUCCESS;
}

LRESULT
EFIAPI
H2OOrderedListOptionListProc (
  IN HWND                        Hwnd,
  IN UINT32                      Msg,
  IN WPARAM                      WParam,
  IN LPARAM                      LParam
  )
{
  H2O_ORDERED_LIST_OPTION_LIST   *This;
  UI_CONTROL                     *Control;
  UI_CONTROL                     *Child;
  UI_CONTROL                     *OrderedListOptionList;
  UI_CONTROL                     *FocusedControl;
  INT32                          SelectedIndex;
  INT32                          NextIndex;

  Control = GetUiControl (Hwnd);
  if (Control == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  This    = (H2O_ORDERED_LIST_OPTION_LIST *) Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_ORDERED_LIST_OPTION_LIST *) AllocateZeroPool (sizeof (H2O_ORDERED_LIST_OPTION_LIST));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    XmlCreateControl (mOrderedListOptionListChilds, Control);
    break;

  case UI_NOTIFY_WINDOWINIT:
    CreateOrderedListOptionListControl (Control);
    ((H2O_STATEMENT_CONTROL *)Control)->OnSendChangeQ = H2OOrderedListOptionListOnSendChangeQ;

    UnregisterHotKey (Hwnd, 2);
    UnregisterHotKey (Hwnd, 3);
    UnregisterHotKey (Hwnd, 4);
    UnregisterHotKey (Hwnd, 5);
    MwRegisterHotKey (Hwnd, 2,  0, VK_F5);
    MwRegisterHotKey (Hwnd, 3,  0, VK_F6);
    MwRegisterHotKey (Hwnd, 4,  0, VK_OEM_PLUS);
    MwRegisterHotKey (Hwnd, 5,  0, VK_OEM_MINUS);
    break;

  case WM_HOTKEY:
    switch (HIWORD (LParam)) {

    case VK_F5:
    case VK_F6:
    case VK_OEM_PLUS:
    case VK_OEM_MINUS:
      OrderedListOptionList = UiFindChildByName (Control, L"OrderedListOptionList");
      if (OrderedListOptionList == NULL) {
        break;
      }
      SelectedIndex = LIST_VIEW_CLASS (OrderedListOptionList)->GetSelection ((UI_LIST_VIEW *) OrderedListOptionList);
      NextIndex     = LIST_VIEW_CLASS (OrderedListOptionList)->FindNextSelection (
                        (UI_LIST_VIEW *) OrderedListOptionList,
                        SelectedIndex,
                        (HIWORD(LParam) == VK_F6 || HIWORD(LParam) == VK_OEM_PLUS) ? VK_UP : VK_DOWN
                        );
      if (SelectedIndex != -1 && NextIndex != -1) {
        LIST_VIEW_CLASS (OrderedListOptionList)->SwitchItems ((UI_LIST_VIEW *) OrderedListOptionList, SelectedIndex, NextIndex);
        LIST_VIEW_CLASS (OrderedListOptionList)->SetSelection ((UI_LIST_VIEW *) OrderedListOptionList, NextIndex, TRUE);
      }
      break;
    }
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);

  case UI_NOTIFY_CARRIAGE_RETURN:
    if (Control == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    FocusedControl = GetUiControl (GetFocus ());
    if (StrCmp (FocusedControl->Name, L"OrderedListOptionList") == 0) {
      FocusedControl = (UI_CONTROL *)GetNextStatementControlProc ((H2O_STATEMENT_CONTROL *)Control);
      if (FocusedControl != NULL) {
        SetFocus (FocusedControl->Wnd);
      }
    }
    break;

  case WM_SETFOCUS:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
    Child = UiFindChildByName (Control, L"OrderedListOptionList");
    if (Child != NULL) {
      SetFocus (Child->Wnd);
    }
    break;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

H2O_ORDERED_LIST_OPTION_LIST_CLASS *
EFIAPI
GetH2OOrderedListOptionListClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"OrderedListOptionList", (UI_CONTROL_CLASS *)GetH2OStatementControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OOrderedListOptionListProc;

  return CURRENT_CLASS;
}

