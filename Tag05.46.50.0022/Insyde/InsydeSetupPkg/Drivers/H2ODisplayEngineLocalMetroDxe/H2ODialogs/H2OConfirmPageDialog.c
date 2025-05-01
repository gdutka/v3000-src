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

STATIC H2O_CONFIRM_PAGE_DIALOG_CLASS    *mH2OConfirmPageDialogClass = NULL;
#define CURRENT_CLASS                    mH2OConfirmPageDialogClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mConfirmPageDialogChilds = L""
  L"<VerticalLayout padding='20,20,0,20' background-color='@menucolor' name='ConfirmPageDialog'>"
    L"<VerticalLayout name='TitleLayout' height='70'>"
      L"<Label padding='0,0,30,0' textcolor='0xFFFFFFFF' font-size='29' name='DialogTitle'/>"
    L"</VerticalLayout>"
    L"<VerticalLayout padding='0,0,20,0' vscrollbar='false'>"
      L"<Label textcolor='0xFFFFFFFF' font-size='19' name='ConfirmPageDialogText'/>"
    L"</VerticalLayout>"
    L"<HorizontalLayout height='30'>"
      L"<Label text-align='center' textcolor='0xFFFFFFFF' font-size='20' name='ConfirmPageConfirmString'/>"
    L"</HorizontalLayout>"
    L"<HorizontalLayout height='50'>"
      L"<HorizontalLayout padding='10,0,0,0' child-padding='2' name='DialogButtonList'/>"
    L"</HorizontalLayout>"
  L"</VerticalLayout>";
#endif

LRESULT
H2OConfirmPageDialogProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_CONFIRM_PAGE_DIALOG       *This;
  UI_CONTROL                    *Control;
  EFI_STATUS                    Status;
  UI_CONTROL                    *DialogControl;
  UI_CONTROL                    *FocusControl;
  UINT32                        Index;
  CHAR16                        ButtonWidthStr[20];
  INTN                          Result;

  Control = GetUiControl (Wnd);
  This    = (H2O_CONFIRM_PAGE_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_CONFIRM_PAGE_DIALOG *) AllocateZeroPool (sizeof (H2O_CONFIRM_PAGE_DIALOG));
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
      XmlCreateControl (mConfirmPageDialogChilds, Control);
    }
    break;

  case UI_NOTIFY_WINDOWINIT:
    if (mFbDialog->TitleString != NULL) {
      Control = UiFindChildByName (This, L"DialogTitle");
      UiSetAttribute (Control, L"text", mFbDialog->TitleString);
    }

    Control = UiFindChildByName (This, L"ConfirmPageConfirmString");
    if (mFbDialog->BodyStringCount >= 1 && mFbDialog->BodyStringArray[0] != NULL) {
      UiSetAttribute (Control, L"text", mFbDialog->BodyStringArray[0]);
    }

    Control = UiFindChildByName (This, L"ConfirmPageDialogText");
    if (mFbDialog->BodyStringCount >= 2 && mFbDialog->BodyStringArray[1] != NULL) {
      UiSetAttribute (Control, L"text", mFbDialog->BodyStringArray[1]);
    }

    if (mFbDialog->ButtonCount == 0) {
      return EFI_SUCCESS;
    }

    DialogControl = UiFindChildByName (This, L"DialogButtonList");

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
    break;

  case UI_NOTIFY_CLICK:
    Control = (UI_CONTROL *) WParam;
    Index = (UINT32) GetWindowLongPtr (Control->Wnd, GWLP_USERDATA);
    SendChangeQNotify (0, 0, &mFbDialog->ButtonHiiValueArray[Index]);
    break;

  default:
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }
  return 1;
}

H2O_CONFIRM_PAGE_DIALOG_CLASS *
EFIAPI
GetH2OConfirmPageDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OConfirmPageDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = H2OConfirmPageDialogProc;

  return CURRENT_CLASS;
}

