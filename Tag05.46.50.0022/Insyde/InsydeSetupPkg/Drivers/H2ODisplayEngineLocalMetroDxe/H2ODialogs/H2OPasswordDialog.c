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

STATIC H2O_PASSWORD_DIALOG_CLASS    *mH2OPasswordDialogClass = NULL;
#define CURRENT_CLASS                mH2OPasswordDialogClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mPasswordDialogChilds = L""
  L"<VerticalLayout>"
    L"<HorizontalLayout width='300' background-color='0xFFF2F2F2' name='OldPassword' height='41'>"
      L"<Label width='119' textcolor='0xFF4D4D4D' font-size='16' name='OldPasswordLabel' height='41'/>"
      L"<VerticalLayout padding='6,8,6,0' width='173' height='29'>"
        L"<Control/>"
        L"<Control padding='1,1,1,1' width='171' background-color='0xFF999999' height='27'>"
          L"<UiEdit name='OldPasswordText' focusbkcolor='@menulightcolor' tabstop='true' height='20' padding='7,3,0,3' width='165' taborder='1' background-color='0xFFF2F2F2' password='true'/>"
        L"</Control>"
        L"<Control/>"
      L"</VerticalLayout>"
    L"</HorizontalLayout>"
    L"<Control name='OldPasswordSeperator' height='2'/>"
    L"<HorizontalLayout width='300' background-color='0xFFF2F2F2' name='NewPassword' height='41'>"
      L"<Label width='119' textcolor='0xFF4D4D4D' font-size='16' name='NewPasswordLabel' height='41'/>"
      L"<VerticalLayout padding='6,8,6,0' width='173' height='29'>"
        L"<Control/>"
        L"<Control padding='1,1,1,1' width='171' background-color='0xFF999999' height='27'>"
          L"<UiEdit name='NewPasswordText' focusbkcolor='@menulightcolor' tabstop='true' height='20' padding='7,3,0,3' width='165' taborder='2' background-color='0xFFF2F2F2' password='true'/>"
        L"</Control>"
        L"<Control/>"
      L"</VerticalLayout>"
    L"</HorizontalLayout>"
    L"<Control name='NewPasswordSeperator' height='2'/>"
    L"<HorizontalLayout width='300' background-color='0xFFF2F2F2' name='ConfirmNewPassword' height='41'>"
      L"<Label width='119' textcolor='0xFF4D4D4D' font-size='16' name='ConfirmNewPasswordLabel' height='41'/>"
      L"<VerticalLayout padding='6,8,6,0' width='173' height='29'>"
        L"<Control/>"
        L"<Control padding='1,1,1,1' width='171' background-color='0xFF999999' height='27'>"
          L"<UiEdit name='ConfirmNewPasswordText' focusbkcolor='@menulightcolor' tabstop='true' height='20' padding='7,3,0,3' width='165' taborder='3' background-color='0xFFF2F2F2' password='true'/>"
        L"</Control>"
        L"<Control/>"
      L"</VerticalLayout>"
    L"</HorizontalLayout>"
    L"<Control name='ConfirmNewPasswordSeperator' height='2'/>"
    L"<Control height='10'/>"
    L"<HorizontalLayout child-padding='2' width='300' height='30'>"
      L"<Control/>"
      L"<Button name='Ok' focusbkcolor='@menulightcolor' text='Enter' text-align='singleline|center' height='30' width='55' taborder='4' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF' tabstop='true'/>"
      L"<Button name='Cancel' focusbkcolor='@menulightcolor' text='Cancel' text-align='singleline|center' height='30' width='55' taborder='5' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF' tabstop='true'/>"
    L"</HorizontalLayout>"
  L"</VerticalLayout>";
#endif

STATIC
EFI_STATUS
SendPasswordChange (
  IN HWND                       Wnd,
  IN UINT32                     PasswordCount
  )
{
  UI_DIALOG                     *Dialog;
  UI_CONTROL                    *OldPasswordControl;
  UI_CONTROL                    *NewPasswordControl;
  UI_CONTROL                    *ConfirmNewPasswordControl;
  CHAR16                        *OldPasswordStr;
  CHAR16                        *NewPasswordStr;
  CHAR16                        *ConfirmNewPasswordStr;
  UINT32                        OldPasswordStrSize;
  UINT32                        NewPasswordStrSize;
  UINT32                        ConfirmNewPasswordStrSize;
  EFI_HII_VALUE                 HiiValue;

  Dialog = (UI_DIALOG *) GetWindowLongPtr (Wnd, 0);

  OldPasswordControl        = UiFindChildByName (Dialog, L"OldPasswordText");
  OldPasswordStr            = ((UI_LABEL *) OldPasswordControl)->Text;
  NewPasswordControl        = UiFindChildByName (Dialog, L"NewPasswordText");
  NewPasswordStr            = ((UI_LABEL *) NewPasswordControl)->Text;
  ConfirmNewPasswordControl = UiFindChildByName (Dialog, L"ConfirmNewPasswordText");
  ConfirmNewPasswordStr     = ((UI_LABEL *) ConfirmNewPasswordControl)->Text;

  ZeroMem (&HiiValue, sizeof (HiiValue));
  HiiValue.Type = EFI_IFR_TYPE_STRING;

  switch (PasswordCount) {

  case 3:
    OldPasswordStrSize        = (UINT32) StrSize (OldPasswordStr);
    NewPasswordStrSize        = (UINT32) StrSize (NewPasswordStr);
    ConfirmNewPasswordStrSize = (UINT32) StrSize (ConfirmNewPasswordStr);

    HiiValue.BufferLen = (UINT16) (OldPasswordStrSize + NewPasswordStrSize + ConfirmNewPasswordStrSize);
    HiiValue.Buffer    = AllocatePool (HiiValue.BufferLen);
    ASSERT (HiiValue.Buffer != NULL);
    if (HiiValue.Buffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    CopyMem (&HiiValue.Buffer[0]                                      , OldPasswordStr       , OldPasswordStrSize);
    CopyMem (&HiiValue.Buffer[OldPasswordStrSize]                     , NewPasswordStr       , NewPasswordStrSize);
    CopyMem (&HiiValue.Buffer[OldPasswordStrSize + NewPasswordStrSize], ConfirmNewPasswordStr, ConfirmNewPasswordStrSize);
    break;

  case 2:
    NewPasswordStrSize        = (UINT32) StrSize (NewPasswordStr);
    ConfirmNewPasswordStrSize = (UINT32) StrSize (ConfirmNewPasswordStr);

    HiiValue.BufferLen = (UINT16) (NewPasswordStrSize + ConfirmNewPasswordStrSize);
    HiiValue.Buffer    = AllocatePool (HiiValue.BufferLen);
    ASSERT (HiiValue.Buffer != NULL);
    if (HiiValue.Buffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    CopyMem (&HiiValue.Buffer[0]                 , NewPasswordStr       , NewPasswordStrSize);
    CopyMem (&HiiValue.Buffer[NewPasswordStrSize], ConfirmNewPasswordStr, ConfirmNewPasswordStrSize);
    break;

  case 1:
    ConfirmNewPasswordStrSize = (UINT32) StrSize (ConfirmNewPasswordStr);

    HiiValue.BufferLen = (UINT16) (ConfirmNewPasswordStrSize);
    HiiValue.Buffer    = AllocatePool (HiiValue.BufferLen);
    ASSERT (HiiValue.Buffer != NULL);
    if (HiiValue.Buffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    CopyMem (&HiiValue.Buffer[0], ConfirmNewPasswordStr, ConfirmNewPasswordStrSize);
    break;

  default:
    ASSERT (FALSE);
    break;
  }

  SendChangeQNotify (0, 0, &HiiValue);

  return EFI_SUCCESS;
}

STATIC
LRESULT
H2OPasswordDialogProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_PASSWORD_DIALOG           *This;
  UI_CONTROL                    *Control;
  EFI_IMAGE_INPUT               *FormsetImage;
  CHAR16                        Str[20];
  STATIC UINT32                 BodyInputCount;
  HWND                          FocusedWnd;

  if (!mIsSendForm) {
    return H2OConfirmPasswordProc (Wnd, Msg, WParam, LParam);
  }

  Control = GetUiControl (Wnd);
  This    = (H2O_PASSWORD_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_PASSWORD_DIALOG *) AllocateZeroPool (sizeof (H2O_PASSWORD_DIALOG));
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

    if (gFB->CurrentQ != NULL && gFB->CurrentQ->Help != NULL) {
      Control = UiFindChildByName (This, L"DialogText");
      UiSetAttribute (Control, L"text", gFB->CurrentQ->Help);
    }

    Control = UiFindChildByName (This, L"DialogTextScrollView");
    if (Control != NULL) {
      Control->OnSetState = H2OCommonDialogWithHelpOnSetState;
    }

    Control = UiFindChildByName (This, L"Content");
    XmlCreateControl (mPasswordDialogChilds, Control);

    //
    // set maximum password length
    //
    UnicodeSPrint (Str, sizeof (Str), L"%d",  (mFbDialog->ConfirmHiiValue.BufferLen / sizeof (CHAR16) - 1));
    Control = UiFindChildByName (This, L"OldPasswordText");
    UiSetAttribute (Control, L"maxlength", Str);
    Control = UiFindChildByName (This, L"NewPasswordText");
    UiSetAttribute (Control, L"maxlength", Str);
    Control = UiFindChildByName (This, L"ConfirmNewPasswordText");
    UiSetAttribute (Control, L"maxlength", Str);

    //
    // Disable Enter old password if password doesn't exist.
    //
    BodyInputCount = mFbDialog->BodyInputCount;
    switch (BodyInputCount) {

    case 3:
      Control = UiFindChildByName (This, L"OldPasswordLabel");
      UiSetAttribute (Control, L"text", mFbDialog->BodyStringArray[0]);
      Control = UiFindChildByName (This, L"NewPasswordLabel");
      UiSetAttribute (Control, L"text", mFbDialog->BodyStringArray[1]);
      Control = UiFindChildByName (This, L"ConfirmNewPasswordLabel");
      UiSetAttribute (Control, L"text", mFbDialog->BodyStringArray[2]);

      Control = UiFindChildByName (This, L"OldPasswordText");
      SetFocus (Control->Wnd);
      break;

    case 2:
      Control = UiFindChildByName (This, L"OldPassword");
      UiSetAttribute (Control, L"visibility", L"false");
      Control = UiFindChildByName (This, L"OldPasswordSeperator");
      UiSetAttribute (Control, L"visibility", L"false");

      Control = UiFindChildByName (This, L"NewPasswordLabel");
      UiSetAttribute (Control, L"text", mFbDialog->BodyStringArray[0]);
      Control = UiFindChildByName (This, L"ConfirmNewPasswordLabel");
      UiSetAttribute (Control, L"text", mFbDialog->BodyStringArray[1]);

      Control = UiFindChildByName (This, L"NewPasswordText");
      SetFocus (Control->Wnd);
      break;

    case 1:
      Control = UiFindChildByName (This, L"OldPassword");
      UiSetAttribute (Control, L"visibility", L"false");
      Control = UiFindChildByName (This, L"OldPasswordSeperator");
      UiSetAttribute (Control, L"visibility", L"false");

      Control = UiFindChildByName (This, L"NewPassword");
      UiSetAttribute (Control, L"visibility", L"false");
      Control = UiFindChildByName (This, L"NewPasswordSeperator");
      UiSetAttribute (Control, L"visibility", L"false");

      Control = UiFindChildByName (This, L"ConfirmNewPasswordLabel");
      UiSetAttribute (Control, L"text", mFbDialog->BodyStringArray[0]);

      Control = UiFindChildByName (This, L"ConfirmNewPasswordText");
      SetFocus (Control->Wnd);
      break;

    default:
      ASSERT(FALSE);
      break;
    }

    Control = UiFindChildByName (This, L"DialogTitle");
    if (mFbDialog->TitleString != NULL) {
      UiSetAttribute (Control, L"text", mFbDialog->TitleString);
    }
    FormsetImage = GetCurrentFormSetImage ();

    if (FormsetImage != NULL) {
      Control = UiFindChildByName (This, L"DialogImage");
      UnicodeSPrint (Str, sizeof (Str), L"0x%p", FormsetImage);
      UiSetAttribute (Control, L"background-image", Str);
      UiSetAttribute (Control, L"visibility", L"true");
    }

    //
    // Add text for buttons
    //
    if (mFbDialog->ButtonCount == 2) {
      Control = UiFindChildByName (This, L"Ok");
      UiSetAttribute (Control, L"text", mFbDialog->ButtonStringArray[0]);
      Control = UiFindChildByName (This, L"Cancel");
      UiSetAttribute (Control, L"text", mFbDialog->ButtonStringArray[1]);
    }

    mTitleVisible = TRUE;
    SetTimer (Wnd, 0, 1, DialogCallback);
    break;

  case UI_NOTIFY_CLICK:
    FocusedWnd = GetFocus ();
    Control  = (UI_CONTROL *) GetWindowLongPtr (FocusedWnd, 0);
    if (StrCmp (Control->Name, L"Cancel") == 0) {
      SendShutDNotify ();
    } else {
      SendPasswordChange (Wnd, BodyInputCount);
    }
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
    FocusedWnd = GetFocus ();
    Control    = (UI_CONTROL *) GetWindowLongPtr (FocusedWnd, 0);
    if (StrCmp (Control->Name, L"OldPasswordText") == 0) {
      Control = UiFindChildByName (This, L"NewPasswordText");
      SetFocus (Control->Wnd);
    } else if (StrCmp (Control->Name, L"NewPasswordText") == 0) {
      Control = UiFindChildByName (This, L"ConfirmNewPasswordText");
      SetFocus (Control->Wnd);
    } else if (StrCmp (Control->Name, L"ConfirmNewPasswordText") == 0) {
      Control = UiFindChildByName (This, L"Ok");
      SetFocus (Control->Wnd);
    } else if (StrCmp (Control->Name, L"Ok") == 0) {
      SendPasswordChange (Wnd, BodyInputCount);
    } else if (StrCmp (Control->Name, L"Cancel") == 0) {
      SendShutDNotify ();
    }
    return 0;

  case UI_NOTIFY_BLUR:
  case UI_NOTIFY_EDITFOCUS:
  case UI_NOTIFY_CHANGE:
    FocusedWnd = GetFocus ();
    Control    = (UI_CONTROL *) GetWindowLongPtr (FocusedWnd, 0);
    if (Control != NULL &&
        (StrCmp (Control->Name, L"OldPasswordText") == 0 ||
         StrCmp (Control->Name, L"NewPasswordText") == 0 ||
         StrCmp (Control->Name, L"ConfirmNewPasswordText") == 0)) {
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

  case WM_HOTKEY:
    if (HIWORD(LParam) == VK_ESCAPE) {
      SendShutDNotify ();
      return 0;
    }
    return 1;

  case WM_DESTROY:
    KillTimer (Wnd, 0);
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

  default:
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }
  return 1;
}

H2O_PASSWORD_DIALOG_CLASS *
EFIAPI
GetH2OPasswordDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OPasswordDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OPasswordDialogProc;

  return CURRENT_CLASS;
}

