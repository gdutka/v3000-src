/** @file

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

#include "MetroDialog.h"

STATIC H2O_MSG_POPUP_DIALOG_CLASS   *mH2OMsgPopupDialogClass = NULL;
#define CURRENT_CLASS               mH2OMsgPopupDialogClass

STATIC
LRESULT
H2OMsgPopUpDialogProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_MSG_POPUP_DIALOG          *This;
  UI_CONTROL                    *Control;
  UI_CONTROL                    *ListControl;
  UI_CONTROL                    *FocusControl;
  CHAR16                        *BodyString;
  CHAR16                        ButtonWidthStr[20];
  UINTN                         Index;
  INTN                          Result;
  EFI_STATUS                    Status;

  Control = GetUiControl (Wnd);
  This    = (H2O_MSG_POPUP_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_MSG_POPUP_DIALOG *) AllocateZeroPool (sizeof (H2O_MSG_POPUP_DIALOG));
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
      if (mIsSendForm) {
        XmlCreateControl (mCommonDialogChilds, Control);
      } else {
        XmlCreateControl (mDialogWithoutSendFormChilds, Control);
      }
    }
    break;

  case UI_NOTIFY_WINDOWINIT:
    if (mFbDialog->TitleString != NULL) {
      Control = UiFindChildByName (This, L"DialogTitle");
      UiSetAttribute (Control, L"text", mFbDialog->TitleString);
      UiSetAttribute (Control, L"visibility", L"true");
    }

    if (mFbDialog->BodyStringArray != NULL) {
      Control = UiFindChildByName (This, L"DialogText");
      BodyString = CatStringArray (mFbDialog->BodyStringCount, (CONST CHAR16 **) mFbDialog->BodyStringArray);
      if (BodyString != NULL) {
        UiSetAttribute (Control, L"text", BodyString);
        FreePool (BodyString);
      }
    }

    ListControl = UiFindChildByName (This, L"DialogButtonList");
    UiSetAttribute (ListControl, L"visibility", L"true");

    Control = CreateControl (L"Control", ListControl);
    CONTROL_CLASS(ListControl)->AddChild (ListControl, Control);

    UnicodeSPrint (ButtonWidthStr, sizeof (ButtonWidthStr), L"%d", GetButtonWidthByStr ());
    FocusControl = NULL;
    for (Index = 0; Index < mFbDialog->ButtonCount; Index++) {
      if (mFbDialog->ButtonStringArray[Index] == NULL) {
        continue;
      }

      Control = CreateControl (L"Button", ListControl);
      SetWindowLongPtr (Control->Wnd, GWLP_USERDATA, (INTN) Index);

      UiApplyAttributeList (Control, L"name='Button' height='30' font-size='19' textcolor='0xFFFFFFFF' text-align='center' text-align='singleline' background-color='0xFFCCCCCC' focusbkcolor='@menulightcolor'");
      if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
        UiApplyAttributeList (Control, L"background-color='0x0' focustextcolor='0xFF404040'");
      }
      UiSetAttribute (Control, L"text",  mFbDialog->ButtonStringArray[Index]);
      UiSetAttribute (Control, L"width", ButtonWidthStr);

      Status = CompareHiiValue (&mFbDialog->ButtonHiiValueArray[Index], &mFbDialog->ConfirmHiiValue, &Result);
      if (!EFI_ERROR(Status) && Result == 0) {
        FocusControl = Control;
      }
      CONTROL_CLASS(ListControl)->AddChild (ListControl, Control);
    }
    if (FocusControl != NULL) {
      SetFocus (FocusControl->Wnd);
    }

    Control = CreateControl (L"Control", ListControl);
    CONTROL_CLASS(ListControl)->AddChild (ListControl, Control);
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

H2O_MSG_POPUP_DIALOG_CLASS *
EFIAPI
GetH2OMsgPopUpDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OMsgPopUpDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OMsgPopUpDialogProc;

  return CURRENT_CLASS;
}

