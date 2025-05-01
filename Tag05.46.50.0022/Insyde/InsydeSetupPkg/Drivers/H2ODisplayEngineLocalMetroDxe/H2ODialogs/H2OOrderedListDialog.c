/** @file
  UI ordered list control

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

#include "MetroDialog.h"

STATIC H2O_ORDERD_LIST_DIALOG_CLASS    *mH2OOrderdListDialogClass = NULL;
#define CURRENT_CLASS                   mH2OOrderdListDialogClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mOrderedListDialogChilds = L""
  L"<VerticalLayout name='OrderedListDialog'>"
    L"<ListView moveitemsupport='true' name='OrderedListList' vscrollbar='true' tabstop='true' child-padding='2' taborder='1'/>"
    L"<Control height='12'/>"
    L"<HorizontalLayout padding='0,0,30,0' child-padding='2' width='300' height='25'>"
      L"<Control/>"
      L"<Button name='Ok' focusbkcolor='@menulightcolor' text='YES' text-align='singleline|center' height='30' width='55' taborder='2' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF' tabstop='true'/>"
      L"<Button name='Cancel' focusbkcolor='@menulightcolor' text='NO' text-align='singleline|center' height='30' width='55' taborder='3' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF' tabstop='true'/>"
    L"</HorizontalLayout>"
  L"</VerticalLayout>";

CHAR16 *mOrderedListOptionChilds = L""
  L"<HorizontalLayout width='-1' background-color='0xFFE6E6E6' name='Option' height='40'>"
    L"<Texture width='40' name='Icon' height='40'/>"
    L"<Label padding='7,0,7,0' text-align='center' textcolor='0xFF404040' font-size='14' name='Words'/>"
    L"<Texture height='40' width='40' background-image='@DialogSortIcon' name='SortIcon' background-image-style='stretch|gray'/>"
  L"</HorizontalLayout>";
#endif

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
H2OOrderedListOptionOnSetState (
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
H2OOrderedListCreateOptions (
  IN UI_CONTROL                 *Layout
  )
{
  UINT8                         *ValueBuffer;
  UINT8                         ContainerCount;
  UINTN                         NumberOfOptions;
  H2O_FORM_BROWSER_O            *OptionArray;
  UINT64                        ContainerHiiValue;
  UINT32                        ContainerIndex;
  UINT32                        OptionIndex;
  H2O_FORM_BROWSER_O            *Option;
  UI_CONTROL                    *CreatedControl;
  UI_CONTROL                    *ChildControl;
  CHAR16                        Str[20];
  BOOLEAN                       Found;

  Found           = FALSE;
  ValueBuffer     = gFB->CurrentQ->HiiValue.Buffer;
  ContainerCount  = gFB->CurrentQ->ContainerCount;
  NumberOfOptions = gFB->CurrentQ->NumberOfOptions;
  OptionArray     = gFB->CurrentQ->Options;

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

      CreatedControl             = XmlCreateControl (mOrderedListOptionChilds, Layout);
      if (CreatedControl == NULL) {
        continue;
      }
      CreatedControl->OnSetState = H2OOrderedListOptionOnSetState;
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

  LIST_VIEW_CLASS (Layout)->SetSelection ((UI_LIST_VIEW *) Layout, (INT32)0, TRUE);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
H2OOrderedListGetHiiValue (
  IN  UI_CONTROL                *Layout,
  OUT EFI_HII_VALUE             *HiiValue
  )
{
  UINT8                         HiiValueType;
  UINT32                        Index;
  UI_CONTROL                    *ChildControl;

  if (Layout == NULL || Layout->ItemCount == 0 || HiiValue == NULL) {
    return EFI_ABORTED;
  }

  HiiValueType = gFB->CurrentQ->Options[0].HiiValue.Type;

  ZeroMem (HiiValue, sizeof (EFI_HII_VALUE));
  HiiValue->Type      = EFI_IFR_TYPE_BUFFER;
  HiiValue->BufferLen = (UINT16) (GetSizeByHiiValueType (HiiValueType) * Layout->ItemCount);
  HiiValue->Buffer    = AllocateZeroPool (HiiValue->BufferLen);
  if (HiiValue->Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < Layout->ItemCount; Index++) {
    ChildControl = Layout->Items[Index];
    SetHiiBufferValue (HiiValue->Buffer, HiiValueType, Index, (UINT64) GetWindowLongPtr (ChildControl->Wnd, GWLP_USERDATA));
  }

  return EFI_SUCCESS;
}

STATIC
LRESULT
H2OOrderedListInputProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_ORDERD_LIST_DIALOG        *This;
  UI_CONTROL                    *Control;
  UI_CONTROL                    *Layout;
  UI_CONTROL                    *ChildControl;
  CHAR16                        Str[20];
  EFI_IMAGE_INPUT               *FormsetImage;
  EFI_HII_VALUE                 HiiValue;
  EFI_STATUS                    Status;
  INT32                         SelectedIndex;
  INT32                         NextIndex;

  Control = GetUiControl (Wnd);
  This    = (H2O_ORDERD_LIST_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_ORDERD_LIST_DIALOG *) AllocateZeroPool (sizeof (H2O_ORDERD_LIST_DIALOG));
    if (This == NULL) {
      break;
    }
    CopyMem (This, (VOID *)((CREATESTRUCT *)(LParam))->lpCreateParams, sizeof (UI_DIALOG));
    CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Wnd, 0);
    SetWindowLongPtr (Wnd, 0, (INTN)This);
    SetWindowLongPtr (Wnd, GWL_STYLE, GetWindowLongPtr (Wnd, GWL_STYLE) & (~WS_CHILD));
    SendMessage (Wnd, UI_NOTIFY_CREATE, WParam, LParam);
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
    if (Control != NULL) {
      XmlCreateControl (mCommonDialogWithHelpChilds, Control);
    }
    break;

  case UI_NOTIFY_WINDOWINIT:
    if (mFbDialog->TitleString != NULL) {
      ChildControl = UiFindChildByName (This, L"DialogTitle");
      UiSetAttribute (ChildControl, L"text", mFbDialog->TitleString);
    }

    if (gFB->CurrentQ != NULL && gFB->CurrentQ->Help != NULL) {
      ChildControl = UiFindChildByName (This, L"DialogText");
      UiSetAttribute (ChildControl, L"text", gFB->CurrentQ->Help);
    }

    Control = UiFindChildByName (This, L"Content");
    if (Control != NULL &&  gFB->CurrentQ != NULL) {
      XmlCreateControl (mOrderedListDialogChilds, Control);
    }

    Control = UiFindChildByName (This, L"DialogTextScrollView");
    if (Control != NULL) {
      Control->OnSetState = H2OCommonDialogWithHelpOnSetState;
    }

    FormsetImage = GetCurrentFormSetImage ();
    if (FormsetImage != NULL) {
      ChildControl = UiFindChildByName (This, L"DialogImage");
      UnicodeSPrint (Str, sizeof (Str), L"0x%p", FormsetImage);
      UiSetAttribute (ChildControl, L"background-image", Str);
      UiSetAttribute (ChildControl, L"visibility", L"true");
    }

    Layout = UiFindChildByName (This, L"OrderedListList");
    if (Layout->VScrollBar != NULL) {
      UiSetAttribute (Layout->VScrollBar, L"displayscrollbar", L"false");
      CONTROL_CLASS_INVALIDATE (Layout);
    }

    Status = H2OOrderedListCreateOptions (Layout);
    if (!EFI_ERROR (Status)) {
      ChildControl = Layout;
    } else {
      ChildControl = UiFindChildByName (This, L"Ok");
    }
    SetFocus (ChildControl->Wnd);

    UnregisterHotKey (Wnd, 2);
    UnregisterHotKey (Wnd, 3);
    UnregisterHotKey (Wnd, 4);
    UnregisterHotKey (Wnd, 5);
    MwRegisterHotKey (Wnd, 2,  0, VK_F5);
    MwRegisterHotKey (Wnd, 3,  0, VK_F6);
    MwRegisterHotKey (Wnd, 4,  0, VK_OEM_PLUS);
    MwRegisterHotKey (Wnd, 5,  0, VK_OEM_MINUS);
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
  case UI_NOTIFY_CLICK:
    ChildControl = (UI_CONTROL *) WParam;
    if (StrCmp (ChildControl->Name, L"OrderedListList") == 0) {
      ChildControl = UiFindChildByName (This, L"Ok");
      SetFocus (ChildControl->Wnd);
    } else if (StrCmp (ChildControl->Name, L"Cancel") == 0) {
      SendShutDNotify ();
    } else if (StrCmp (ChildControl->Name, L"Ok") == 0) {
      Layout = UiFindChildByName (This, L"OrderedListList");
      H2OOrderedListGetHiiValue (Layout, &HiiValue);
      SendChangeQNotify (0, 0, &HiiValue);
    }
    break;

  case WM_HOTKEY:
    switch (HIWORD(LParam)) {

    case VK_ESCAPE:
      SendShutDNotify ();
      return 0;

    case VK_F5:
    case VK_F6:
    case VK_OEM_PLUS:
    case VK_OEM_MINUS:
      Layout        = UiFindChildByName (This, L"OrderedListList");
      SelectedIndex = LIST_VIEW_CLASS (Layout)->GetSelection ((UI_LIST_VIEW *) Layout);
      NextIndex     = LIST_VIEW_CLASS (Layout)->FindNextSelection (
                                                  (UI_LIST_VIEW *) Layout,
                                                  SelectedIndex,
                                                  (HIWORD(LParam) == VK_F6 || HIWORD(LParam) == VK_OEM_PLUS) ? VK_UP : VK_DOWN
                                                  );
      if (SelectedIndex != -1 && NextIndex != -1) {
        LIST_VIEW_CLASS (Layout)->SwitchItems ((UI_LIST_VIEW *) Layout, SelectedIndex, NextIndex);
        LIST_VIEW_CLASS (Layout)->SetSelection ((UI_LIST_VIEW *) Layout, NextIndex, TRUE);
      }
      return 0;

    default:
      break;
    }
    return 1;

  case WM_DESTROY:
    UnregisterHotKey (Wnd, 2);
    UnregisterHotKey (Wnd, 3);
    UnregisterHotKey (Wnd, 4);
    UnregisterHotKey (Wnd, 5);
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

  default:
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }

  return 1;
}

H2O_ORDERD_LIST_DIALOG_CLASS *
EFIAPI
GetH2OOrderedListDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OOrderedListDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OOrderedListInputProc;

  return CURRENT_CLASS;
}

