/** @file
  UI Date Roller Controls

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

extern H2O_FORM_BROWSER_D        *mFbDialog;

STATIC H2O_PASSWORD_INPUT_CLASS  *mPasswordInputClass = NULL;
#define CURRENT_CLASS            mPasswordInputClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mPasswordInputChilds = L""
  L"<VerticalLayout width='match_parent' height='match_parent'>"
    L"<HorizontalLayout width='300' background-color='0xFFF2F2F2' name='OldPassword' height='41'>"
      L"<Label width='119' textcolor='0xFF4D4D4D' font-size='16' name='OldPasswordLabel' height='41'/>"
      L"<VerticalLayout padding='6,8,6,0' width='181' height='41'>"
        L"<Control/>"
        L"<Control padding='1,1,1,1' width='173' background-color='0xFF999999' height='29'>"
          L"<UiEdit name='OldPasswordText' focusbkcolor='@menulightcolor' tabstop='true' height='27' padding='7,3,0,3' width='171' taborder='1' background-color='0xFFF2F2F2' password='true'/>"
        L"</Control>"
        L"<Control/>"
      L"</VerticalLayout>"
    L"</HorizontalLayout>"
    L"<Control name='OldPasswordSeperator' height='2'/>"
    L"<HorizontalLayout width='300' background-color='0xFFF2F2F2' name='NewPassword' height='41'>"
      L"<Label width='119' textcolor='0xFF4D4D4D' font-size='16' name='NewPasswordLabel' height='41'/>"
      L"<VerticalLayout padding='6,8,6,0' width='181' height='41'>"
        L"<Control/>"
        L"<Control padding='1,1,1,1' width='173' background-color='0xFF999999' height='29'>"
          L"<UiEdit name='NewPasswordText' focusbkcolor='@menulightcolor' tabstop='true' height='27' padding='7,3,0,3' width='171' taborder='2' background-color='0xFFF2F2F2' password='true'/>"
        L"</Control>"
        L"<Control/>"
      L"</VerticalLayout>"
    L"</HorizontalLayout>"
    L"<Control name='NewPasswordSeperator' height='2'/>"
    L"<HorizontalLayout width='300' background-color='0xFFF2F2F2' name='ConfirmNewPassword' height='41'>"
      L"<Label width='119' textcolor='0xFF4D4D4D' font-size='16' name='ConfirmNewPasswordLabel' height='41'/>"
      L"<VerticalLayout padding='6,8,6,0' width='181' height='41'>"
        L"<Control/>"
        L"<Control padding='1,1,1,1' width='173' background-color='0xFF999999' height='29'>"
          L"<UiEdit name='ConfirmNewPasswordText' focusbkcolor='@menulightcolor' tabstop='true' height='27' padding='7,3,0,3' width='171' taborder='3' background-color='0xFFF2F2F2' password='true'/>"
        L"</Control>"
        L"<Control/>"
      L"</VerticalLayout>"
    L"</HorizontalLayout>"
  L"</VerticalLayout>"
  ;
#endif

STATIC
EFI_STATUS
H2OPasswordInputOnSendChangeQ (
  IN H2O_STATEMENT_CONTROL      *This
  )
{
  UI_CONTROL                    *Control;
  UI_CONTROL                    *OldPasswordControl;
  UI_CONTROL                    *NewPasswordControl;
  UI_CONTROL                    *ConfirmNewPasswordControl;
  CHAR16                        *OldPasswordStr;
  CHAR16                        *NewPasswordStr;
  CHAR16                        *ConfirmNewPasswordStr;
  UINT32                        OldPasswordStrSize;
  UINT32                        NewPasswordStrSize;
  UINT32                        ConfirmNewPasswordStrSize;
  UINT32                        BodyInputCount;
  EFI_HII_VALUE                 HiiValue;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Control = (UI_CONTROL *)This;

  OldPasswordControl        = UiFindChildByName (Control, L"OldPasswordText");
  OldPasswordStr            = ((UI_LABEL *) OldPasswordControl)->Text;
  NewPasswordControl        = UiFindChildByName (Control, L"NewPasswordText");
  NewPasswordStr            = ((UI_LABEL *) NewPasswordControl)->Text;
  ConfirmNewPasswordControl = UiFindChildByName (Control, L"ConfirmNewPasswordText");
  ConfirmNewPasswordStr     = ((UI_LABEL *) ConfirmNewPasswordControl)->Text;

  ZeroMem (&HiiValue, sizeof (HiiValue));
  HiiValue.Type = EFI_IFR_TYPE_STRING;

  BodyInputCount = mFbDialog->BodyInputCount;
  switch (BodyInputCount) {

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
    return EFI_UNSUPPORTED;
  }

  This->ValueChanged = TRUE;
  CopyMem (&This->HiiValue, &HiiValue, sizeof (EFI_HII_VALUE));

  H2OStatementControlOnSendChangeQ (This);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CreatePasswordInputControl (
  IN OUT UI_CONTROL              *Control
  )
{
  H2O_STATEMENT_CONTROL          *StatementControl;
  H2O_FORM_BROWSER_S             *Statement;
  UI_CONTROL                     *Child;
  UINT32                         BodyInputCount;
  CHAR16                         Str[20];

  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  StatementControl = (H2O_STATEMENT_CONTROL *) Control;

  Statement = StatementControl->Statement;
  if (Statement == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // set maximum password length
  //
  UnicodeSPrint (Str, sizeof (Str), L"%d",  (mFbDialog->ConfirmHiiValue.BufferLen / sizeof (CHAR16) - 1));
  Child = UiFindChildByName (Control, L"OldPasswordText");
  UiSetAttribute (Child, L"maxlength", Str);
  Child = UiFindChildByName (Control, L"NewPasswordText");
  UiSetAttribute (Child, L"maxlength", Str);
  Child = UiFindChildByName (Control, L"ConfirmNewPasswordText");
  UiSetAttribute (Child, L"maxlength", Str);

  //
  // Disable Enter old password if password doesn't exist.
  //
  BodyInputCount = mFbDialog->BodyInputCount;
  switch (BodyInputCount) {

  case 3:
    Child = UiFindChildByName (Control, L"OldPasswordLabel");
    UiSetAttribute (Child, L"text", mFbDialog->BodyStringArray[0]);
    Child = UiFindChildByName (Control, L"NewPasswordLabel");
    UiSetAttribute (Child, L"text", mFbDialog->BodyStringArray[1]);
    Child = UiFindChildByName (Control, L"ConfirmNewPasswordLabel");
    UiSetAttribute (Child, L"text", mFbDialog->BodyStringArray[2]);

    Child = UiFindChildByName (Control, L"OldPasswordText");
    SetFocus (Child->Wnd);
    break;

  case 2:
    Child = UiFindChildByName (Control, L"OldPassword");
    UiSetAttribute (Child, L"visibility", L"false");
    Child = UiFindChildByName (Control, L"OldPasswordSeperator");
    UiSetAttribute (Child, L"visibility", L"false");

    Child = UiFindChildByName (Control, L"NewPasswordLabel");
    UiSetAttribute (Child, L"text", mFbDialog->BodyStringArray[0]);
    Child = UiFindChildByName (Control, L"ConfirmNewPasswordLabel");
    UiSetAttribute (Child, L"text", mFbDialog->BodyStringArray[1]);

    Child = UiFindChildByName (Control, L"NewPasswordText");
    SetFocus (Child->Wnd);
    break;

  case 1:
    Child = UiFindChildByName (Control, L"OldPassword");
    UiSetAttribute (Child, L"visibility", L"false");
    Child = UiFindChildByName (Control, L"OldPasswordSeperator");
    UiSetAttribute (Child, L"visibility", L"false");

    Child = UiFindChildByName (Control, L"NewPassword");
    UiSetAttribute (Child, L"visibility", L"false");
    Child = UiFindChildByName (Control, L"NewPasswordSeperator");
    UiSetAttribute (Child, L"visibility", L"false");

    Child = UiFindChildByName (Control, L"ConfirmNewPasswordLabel");
    UiSetAttribute (Child, L"text", mFbDialog->BodyStringArray[0]);

    Child = UiFindChildByName (Control, L"ConfirmNewPasswordText");
    SetFocus (Child->Wnd);
    break;

  default:
    ASSERT(FALSE);
    break;
  }

  CONTROL_CLASS_INVALIDATE (Control);

  return EFI_SUCCESS;
}

LRESULT
EFIAPI
H2OPasswordInputProc (
  IN HWND                        Hwnd,
  IN UINT32                      Msg,
  IN WPARAM                      WParam,
  IN LPARAM                      LParam
  )
{
  H2O_PASSWORD_INPUT             *This;
  UI_CONTROL                     *Control;
  UI_CONTROL                     *Child;
  UI_CONTROL                     *FocusedControl;

  Control = GetUiControl (Hwnd);
  if (Control == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  This = (H2O_PASSWORD_INPUT *) Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_PASSWORD_INPUT *) AllocateZeroPool (sizeof (H2O_PASSWORD_INPUT));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    XmlCreateControl (mPasswordInputChilds, Control);
    break;

  case UI_NOTIFY_WINDOWINIT:
    if (Control == NULL) {
      break;
    }
    CreatePasswordInputControl (Control);
    ((H2O_STATEMENT_CONTROL *)Control)->OnSendChangeQ = H2OPasswordInputOnSendChangeQ;
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
    if (Control == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    FocusedControl = GetUiControl (GetFocus ());
    if (StrCmp (FocusedControl->Name, L"OldPasswordText") == 0) {
      Child = UiFindChildByName (Control, L"NewPasswordText");
      SetFocus (Child->Wnd);
    } else if (StrCmp (FocusedControl->Name, L"NewPasswordText") == 0) {
      Child = UiFindChildByName (Control, L"ConfirmNewPasswordText");
      SetFocus (Child->Wnd);
    } else if (StrCmp (FocusedControl->Name, L"ConfirmNewPasswordText") == 0) {
      FocusedControl = (UI_CONTROL *)GetNextStatementControlProc ((H2O_STATEMENT_CONTROL *)Control);
      if (FocusedControl != NULL) {
        SetFocus (FocusedControl->Wnd);
      }
    }
    break;

  case WM_SETFOCUS:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
    Child = UiFindChildByName (Control, L"OldPasswordText");
    if (Child != NULL) {
      SetFocus (Child->Wnd);
    }
    break;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

H2O_PASSWORD_INPUT_CLASS *
EFIAPI
GetH2OPasswordInputClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"PasswordInput", (UI_CONTROL_CLASS *)GetH2OStatementControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OPasswordInputProc;

  return CURRENT_CLASS;
}

