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

STATIC H2O_HELP_DIALOG_CLASS    *mH2OHelpDialogClass = NULL;
#define CURRENT_CLASS            mH2OHelpDialogClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mHelpDialogChilds = L""
  L"<VerticalLayout padding='20,20,0,20' background-color='@menucolor' name='HelpDialog'>"
    L"<VerticalLayout padding='0,0,20,0' vscrollbar='false'>"
      L"<Label textcolor='0xFFFFFFFF' font-size='19' name='DialogText'/>"
    L"</VerticalLayout>"
    L"<HorizontalLayout height='50'>"
      L"<Control/>"
      L"<Button name='DialogButton' focusbkcolor='@menulightcolor' text='OK' text-align='singleline|center' height='30' width='55' font-size='27' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF'/>"
      L"<Control/>"
    L"</HorizontalLayout>"
  L"</VerticalLayout>";
#endif

LRESULT
H2OHelpDialogProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_HELP_DIALOG               *This;
  UI_CONTROL                    *Control;
  CHAR16                        *BodyString;

  Control = GetUiControl (Wnd);
  This    = (H2O_HELP_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_HELP_DIALOG *) AllocateZeroPool (sizeof (H2O_HELP_DIALOG));
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
      XmlCreateControl (mHelpDialogChilds, Control);
    }
    break;

  case UI_NOTIFY_WINDOWINIT:
    Control = UiFindChildByName (This, L"DialogText");
    BodyString = CatStringArray (mFbDialog->BodyStringCount, (CONST CHAR16 **) mFbDialog->BodyStringArray);
    if (BodyString != NULL) {
      UiSetAttribute (Control, L"text", BodyString);
      FreePool (BodyString);
    }

    Control = UiFindChildByName (This, L"DialogButton");
    SetFocus (Control->Wnd);
    break;

  case UI_NOTIFY_CLICK:
    SendShutDNotify ();
    break;

  case WM_HOTKEY:
    if (HIWORD(LParam) == VK_ESCAPE) {
      SendShutDNotify ();
      return 0;
    }
    return 1;

  default:
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }
  return 1;
}

H2O_HELP_DIALOG_CLASS *
EFIAPI
GetH2OHelpDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OHelpDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = H2OHelpDialogProc;

  return CURRENT_CLASS;
}

