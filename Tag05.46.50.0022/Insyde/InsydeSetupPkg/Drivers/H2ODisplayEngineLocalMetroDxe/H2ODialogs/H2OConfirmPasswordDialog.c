/** @file
  UI ordered list control

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

#include "MetroDialog.h"

STATIC H2O_CONFIRM_PASSWORD_DIALOG_CLASS    *mH2OConfirmPasswordDialogClass = NULL;
#define CURRENT_CLASS                        mH2OConfirmPasswordDialogClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mConfirmPasswordDialogChilds = L""
  L"<VerticalLayout name='DialogWithoutSendForm'>"
    L"<Control/>"
    L"<Control background-color='@menucolor' name='parent' height='wrap_content'>"
      L"<VerticalLayout padding='20,30,20,30' background-color='@menucolor' height='wrap_content'>"
        L"<VerticalLayout name='TitleLayout' height='wrap_content'>"
          L"<Label name='DialogTitle' text-align='center' height='40' visibility='false' font-size='19' textcolor='0xFFFFFFFF'/>"
          L"<Label width='match_parent' textcolor='0xFFFFFFFF' font-size='19' name='DialogText' height='wrap_content'/>"
          L"<Control height='15'/>"
          L"<Control height='10' background-image='@DialogSeparator' background-color='0x0' background-image-style='center'/>"
          L"<Control height='15'/>"
        L"</VerticalLayout>"
        L"<VerticalLayout padding='0,0,10,0' min-height='51' name='DialogPasswordInput' height='wrap_content'>"
          L"<Label name='ConfirmNewPasswordLabel' height='wrap_content' padding='0,0,2,0' width='match_parent' font-size='18' textcolor='0xFFFFFFFF'/>"
          L"<Control padding='2,2,2,2' background-color='0xFFCCCCCC' height='wrap_content'>"
            L"<UiEdit name='ConfirmPasswordInput' focusbkcolor='0xFFFFFFFF' tabstop='true' height='20' padding='7,3,0,3' taborder='1' background-color='@menulightcolor' password='true'/>"
          L"</Control>"
        L"</VerticalLayout>"
      L"</VerticalLayout>"
      L"<Texture name='FormHalo' float='true' height='-1' width='-1' background-image='@FormHalo' scale9grid='23,26,22,31'/>"
    L"</Control>"
    L"<Control/>"
  L"</VerticalLayout>";
#endif

STATIC
EFI_STATUS
SendConfirmPassword (
  IN H2O_CONFIRM_PASSWORD_DIALOG *This
  )
{
  UI_CONTROL                     *Control;
  CHAR16                         *PasswordStr;
  EFI_HII_VALUE                  HiiValue;

  Control = UiFindChildByName (This, L"ConfirmPasswordInput");
  PasswordStr = ((UI_LABEL *) Control)->Text;

  HiiValue.BufferLen = (UINT16) StrSize (PasswordStr);
  HiiValue.Buffer    = AllocatePool (HiiValue.BufferLen);
  ASSERT (HiiValue.Buffer != NULL);
  if (HiiValue.Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (HiiValue.Buffer, PasswordStr, HiiValue.BufferLen);
  SendChangeQNotify (0, 0, &HiiValue);

  return EFI_SUCCESS;
}

LRESULT
H2OConfirmPasswordProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_CONFIRM_PASSWORD_DIALOG   *This;
  UI_CONTROL                    *Control;
  CHAR16                        Str[20];

  Control = GetUiControl (Wnd);
  This    = (H2O_CONFIRM_PASSWORD_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_CONFIRM_PASSWORD_DIALOG *) AllocateZeroPool (sizeof (H2O_CONFIRM_PASSWORD_DIALOG));
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
        XmlCreateControl (mCommonDialogWithHelpChilds, Control);
      } else {
        XmlCreateControl (mConfirmPasswordDialogChilds, Control);
      }
    }
    break;

  case UI_NOTIFY_WINDOWINIT:
    if (gFB->CurrentP == NULL) {
      Control = UiFindChildByName (This, L"TitleLayout");
      UiSetAttribute (Control, L"visibility", L"false");
    }

    if (mFbDialog->TitleString != NULL) {
      Control = UiFindChildByName (This, L"DialogText");
      UiSetAttribute (Control, L"text", mFbDialog->TitleString);
      Control = UiFindChildByName (This, L"ConfirmNewPasswordLabel");
      UiSetAttribute (Control, L"text", mFbDialog->TitleString);
    }

    if (mFbDialog->BodyInputCount != 0) {
      UnicodeSPrint (Str, sizeof (Str), L"%d", (mFbDialog->ConfirmHiiValue.BufferLen / sizeof (CHAR16) - 1));
      Control = UiFindChildByName (This, L"ConfirmPasswordInput");
      UiSetAttribute (Control, L"maxlength", Str);
    }

    if (gFB->CurrentP != NULL) {
      mTitleVisible = TRUE;
      SetTimer (Wnd, 0, 1, DialogCallback);
    }

    Control = UiFindChildByName (This, L"ConfirmPasswordInput");
    SetFocus (Control->Wnd);
    break;

  case UI_NOTIFY_CLICK:
  case UI_NOTIFY_CARRIAGE_RETURN:
    //
    // click ok button or ConfirmPasswordInput passowrd input
    //
    SendConfirmPassword (This);
    return 0;
    break;

  case WM_DESTROY:
    if (gFB->CurrentP != NULL) {
      KillTimer (Wnd, 0);
    }
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

  case UI_NOTIFY_BLUR:
  case UI_NOTIFY_EDITFOCUS:
  case UI_NOTIFY_CHANGE:
    Control    = (UI_CONTROL *) GetWindowLongPtr (GetFocus (), 0);
    if (Control != NULL &&
        StrCmp (Control->Name, L"ConfirmPasswordInput") == 0) {
      if (Msg == UI_NOTIFY_BLUR) {
        UnicodeSPrint (Str, sizeof (Str), L"");
      } else {
        UnicodeSPrint (Str, sizeof (Str), L"%d/%d", StrLen (((UI_LABEL *)Control)->Text), ((UI_EDIT *)Control)->MaxLength);
      }
      Control = UiFindChildByName (This, L"PasswordStrLen");
      if (Control != NULL) {
        UiSetAttribute (Control, L"text", Str);
      }
    }
    return 0;

  default:
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }

  return 1;
}

H2O_CONFIRM_PASSWORD_DIALOG_CLASS *
EFIAPI
GetH2OConfirmPasswordDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OConfirmPasswordDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = H2OConfirmPasswordProc;

  return CURRENT_CLASS;
}

