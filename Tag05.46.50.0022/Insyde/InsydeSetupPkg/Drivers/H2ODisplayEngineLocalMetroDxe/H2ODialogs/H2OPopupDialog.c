/** @file
  UI ordered list control

;******************************************************************************
;* Copyright (c) 2014 - 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "MetroDialog.h"

STATIC H2O_POPUP_DIALOG_CLASS       *mH2OPopupDialogClass = NULL;
#define CURRENT_CLASS               mH2OPopupDialogClass

STATIC
EFI_STATUS
PopupDialogInit (
  IN UI_DIALOG                             *Dialog
  )
{
  EFI_STATUS                               Status;
  UI_CONTROL                               *Control;
  UI_CONTROL                               *DialogControl;
  UI_CONTROL                               *FocusControl;
  UINT32                                   Index;
  CHAR16                                   ButtonWidthStr[20];
  CHAR16                                   *BodyString;
  INTN                                     Result;

  if (mFbDialog->TitleString != NULL) {
    Control = UiFindChildByName (Dialog, L"DialogTitle");
    UiSetAttribute (Control, L"text", mFbDialog->TitleString);
  }

  if (mFbDialog->BodyStringArray != NULL) {
    Control = UiFindChildByName (Dialog, L"DialogText");
    BodyString = CatStringArray (mFbDialog->BodyStringCount, (CONST CHAR16 **) mFbDialog->BodyStringArray);
    if (BodyString != NULL) {
      UiSetAttribute (Control, L"text", BodyString);
      FreePool (BodyString);
    }
  }

  if (mFbDialog->ButtonCount == 0) {
    return EFI_SUCCESS;
  }

  DialogControl = UiFindChildByName (Dialog, L"DialogButtonList");

  Control = CreateControl (L"Control", DialogControl);
  CONTROL_CLASS(DialogControl)->AddChild (DialogControl, Control);

  UnicodeSPrint (ButtonWidthStr, sizeof (ButtonWidthStr), L"%d", GetButtonWidthByStr ());
  FocusControl = NULL;
  for (Index = 0; Index < mFbDialog->ButtonCount; Index++) {
    if (mFbDialog->ButtonStringArray[Index] == NULL) {
      continue;
    }

    Control = CreateControl (L"Button", DialogControl);
    UiSetAttribute (Control, L"text",  mFbDialog->ButtonStringArray[Index]);
    SetWindowLongPtr (Control->Wnd, GWLP_USERDATA, (INTN) Index);

    UiApplyAttributeList (Control, L"name='Button' height='30' font-size='19' textcolor='0xFFFFFFFF' text-align='center' text-align='singleline' background-color='0xFFCCCCCC' focusbkcolor='@menulightcolor'");
    if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
      UiApplyAttributeList (Control, L"background-color='0x0' focustextcolor='0xFF404040'");
    }
    UiSetAttribute (Control, L"width", ButtonWidthStr);

    Status = CompareHiiValue (&mFbDialog->ButtonHiiValueArray[Index], &mFbDialog->ConfirmHiiValue, &Result);
    if (!EFI_ERROR(Status) && Result == 0) {
      FocusControl = Control;
    }
    CONTROL_CLASS(DialogControl)->AddChild (DialogControl, Control);
  }
  if (FocusControl != NULL) {
    SetFocus (FocusControl->Wnd);
  }

  Control = CreateControl (L"Control", DialogControl);
  CONTROL_CLASS(DialogControl)->AddChild (DialogControl, Control);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
SetDialogText (
  IN H2O_POPUP_DIALOG                      *This
  )
{
  EFI_STATUS                               Status;
  UINTN                                    Index;
  UI_CONTROL                               *Control;
  UI_CONTROL                               *ListControl;
  UI_CONTROL                               *FocusControl;
  CHAR16                                   *BodyString;
  INTN                                     Result;

  if ((mFbDialog->DialogType & H2O_FORM_BROWSER_D_TYPE_QUESTIONS) != 0) {
    return EFI_UNSUPPORTED;
  }

  FocusControl = NULL;

  if ((mFbDialog->DialogType & H2O_FORM_BROWSER_D_TYPE_SELECTION) == H2O_FORM_BROWSER_D_TYPE_SELECTION) {
    if (mFbDialog->BodyStringArray != NULL) {
      Control = UiFindChildByName (This, L"DialogText");
      BodyString = CatStringArray (mFbDialog->BodyStringCount, (CONST CHAR16 **) mFbDialog->BodyStringArray);
      if (BodyString != NULL) {
        UiSetAttribute (Control, L"text", BodyString);
        if (GetSystemMetrics(SM_CYSCREEN) < 600) {
          UiSetAttribute (Control, L"font-size", L"16");
        }
        FreePool (BodyString);
      }
    }

    ListControl = UiFindChildByName (This, L"DialogButtonList");
    UiSetAttribute (ListControl, L"visibility", L"true");

    Control = CreateControl (L"Control", ListControl);
    CONTROL_CLASS(ListControl)->AddChild (ListControl, Control);

    for (Index = 0; Index < mFbDialog->ButtonCount; Index++) {
      if (mFbDialog->ButtonStringArray[Index] == NULL) {
        continue;
      }

      Control = CreateControl (L"Button", ListControl);
      UiSetAttribute (Control, L"text",  mFbDialog->ButtonStringArray[Index]);
      SetWindowLongPtr (Control->Wnd, GWLP_USERDATA, (INTN) Index);
      UiApplyAttributeList (Control, L"name='Button' height='30' width='75' font-size='19' textcolor='0xFFFFFFFF' text-align='center' text-align='singleline' background-color='0xFFCCCCCC' focusbkcolor='@menulightcolor'");
      if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
        UiApplyAttributeList (Control, L"background-color='0x0' focustextcolor='0xFF404040'");
      }
      CONTROL_CLASS(ListControl)->AddChild (ListControl, Control);

      Status = CompareHiiValue (&mFbDialog->ButtonHiiValueArray[Index], &mFbDialog->ConfirmHiiValue, &Result);
      if (!EFI_ERROR(Status) && Result == 0) {
        FocusControl = Control;
      }
    }

    Control = CreateControl (L"Control", ListControl);
    CONTROL_CLASS(ListControl)->AddChild (ListControl, Control);

    if (FocusControl != NULL) {
      SetFocus (FocusControl->Wnd);
    }
  }

  return EFI_SUCCESS;
}

STATIC
LRESULT
H2OPopupDialogWithoutSendFormProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_POPUP_DIALOG              *This;
  UI_CONTROL                    *Control;
  UINTN                         Index;

  Control = GetUiControl (Wnd);
  This    = (H2O_POPUP_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_POPUP_DIALOG *) AllocateZeroPool (sizeof (H2O_POPUP_DIALOG));
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
      XmlCreateControl (mDialogWithoutSendFormChilds, Control);
    }
    break;

  case UI_NOTIFY_WINDOWINIT:
    SetDialogText (This);
    break;

  case UI_NOTIFY_CLICK:
    Control = (UI_CONTROL *)WParam;

    if ((mFbDialog->DialogType & H2O_FORM_BROWSER_D_TYPE_SELECTION) == H2O_FORM_BROWSER_D_TYPE_SELECTION) {
      Index = (UINTN) GetWindowLongPtr (Control->Wnd, GWLP_USERDATA);
      SendChangeQNotify (0, 0, &mFbDialog->ButtonHiiValueArray[Index]);
    }
    break;

  case WM_DESTROY:
    FreeDialogEvent (&mFbDialog);
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

  default:
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }

  return 1;
}

STATIC
LRESULT
H2OPopupDialogProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_POPUP_DIALOG              *This;
  UI_CONTROL                    *Control;
  UINTN                         Index;

  if (!mIsSendForm) {
    return H2OPopupDialogWithoutSendFormProc (Wnd, Msg, WParam, LParam);
  }

  Control = GetUiControl (Wnd);
  This    = (H2O_POPUP_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_POPUP_DIALOG *) AllocateZeroPool (sizeof (H2O_POPUP_DIALOG));
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
      XmlCreateControl (mCommonDialogChilds, Control);
    }
    break;

  case UI_NOTIFY_WINDOWINIT:
    PopupDialogInit ((UI_DIALOG *)This);
    break;

  case UI_NOTIFY_CLICK:
    Control = (UI_CONTROL *) WParam;
    Index = (UINTN) GetWindowLongPtr (Control->Wnd, GWLP_USERDATA);
    SendChangeQNotify (0, 0, &mFbDialog->ButtonHiiValueArray[Index]);
    break;

  case WM_HOTKEY:
    if (HIWORD(LParam) == VK_ESCAPE) {
      SendShutDNotify ();
      return 0;
    }
    return 1;

  case WM_DESTROY:
    FreeDialogEvent (&mFbDialog);
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

  default:
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }

  return 1;
}

H2O_POPUP_DIALOG_CLASS *
EFIAPI
GetH2OPopupDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OPopupDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OPopupDialogProc;

  return CURRENT_CLASS;
}

