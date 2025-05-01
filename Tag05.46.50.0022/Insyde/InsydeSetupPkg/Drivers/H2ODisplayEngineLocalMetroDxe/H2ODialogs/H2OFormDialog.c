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
#include "H2OFormPanel.h"

STATIC H2O_FORM_DIALOG_CLASS    *mH2OFormDialogClass = NULL;
#define CURRENT_CLASS           mH2OFormDialogClass

EFI_STATUS
GetPopUpForm (
  IN H2O_FORM_BROWSER_Q         *Question,
  OUT EFI_GUID                  *PopUpFormsetId,
  OUT UINT16                    *PopUpFormId
  )
{
  EFI_STATUS                    Status;
  H2O_PANEL_INFO                *Panel;

  if (Question == NULL || PopUpFormsetId == NULL || PopUpFormId == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Question->Signature != H2O_FORM_BROWSER_STATEMENT_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetPanelById (&Question->FormsetGuid, &mMetroPrivate->DisplayEngine.Id, SETUP_PAGE_PANEL_ID, &Panel);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetPopUpTargetByQuestion (
             Question,
             Panel,
             PopUpFormsetId,
             PopUpFormId
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetPopUpFormRect (
  IN EFI_GUID                   *PopUpFormsetId,
  IN UINT16                     PopUpFormId,
  OUT RECT                      *PopUpRect
  )
{
  EFI_STATUS                    Status;
  RECT                          ScreenField;

  if (PopUpFormsetId == NULL || PopUpFormId == 0 || PopUpRect == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ScreenField.left   = 0;
  ScreenField.top    = 0;
  ScreenField.right  = (INT32) GetSystemMetrics (SM_CXSCREEN);
  ScreenField.bottom = (INT32) GetSystemMetrics (SM_CYSCREEN);

  Status = GetFormPanelRect (
             PopUpFormsetId,
             PopUpFormId,
             NULL,
             &ScreenField,
             PopUpRect
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CreateFormDialog (
  IN UI_CONTROL                 *Control
  )
{
  EFI_STATUS                    Status;
  RECT                          PopUpRect;
  EFI_GUID                      PopUpFormsetId;
  UINT16                        PopUpFormId;
  H2O_PANEL_INFO                *Panel;
  UINT8                         RefreshInterval;
  H2O_FORM_BROWSER_D            *FbDialog;

  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  FbDialog = (H2O_FORM_BROWSER_D *) GetWindowLongPtr (Control->Wnd, GWLP_USERDATA);
  if (FbDialog == NULL) {
    return EFI_UNSUPPORTED;
  }

  Status = GetPopUpForm (FbDialog->H2OStatement, &PopUpFormsetId, &PopUpFormId);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetPanelById (&PopUpFormsetId, &mMetroPrivate->DisplayEngine.Id, QUESTION_PANEL_ID, &Panel);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GetWindowRect(Control->Wnd, &PopUpRect);
  Status = FormPanelCreateChildByVfrForm (
             Control,
             Panel,
             &PopUpFormsetId,
             PopUpFormId,
             &PopUpRect,
             &RefreshInterval
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
LRESULT
H2OFormDialogProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  EFI_STATUS                    Status;
  H2O_FORM_DIALOG               *This;
  UI_CONTROL                    *Control;

  Control = GetUiControl (Wnd);
  This    = (H2O_FORM_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_FORM_DIALOG *) AllocateZeroPool (sizeof (H2O_FORM_DIALOG));
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
    break;

  case UI_NOTIFY_WINDOWINIT:
    if (Control == NULL) {
      break;
    }
    SetFocus (Control->Wnd);

    CreateFormDialog (Control);
    FormPanelCallAllChildProc ((UI_CONTROL_CLASS *)CURRENT_CLASS, Control, Msg, WParam, LParam);
    break;

  case WM_HOTKEY:
    if (HIWORD (LParam) == VK_ESCAPE) {
      SendShutDNotify ();
      return 0;
    }
    FormPanelCallAllChildProc ((UI_CONTROL_CLASS *)CURRENT_CLASS, Control, Msg, WParam, LParam);
    return 0;

  case WM_DESTROY:
    FreeDialogEvent (&mFbDialog);
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

  case UI_NOTIFY_CLICK:
  case UI_NOTIFY_CARRIAGE_RETURN:
    Status = FormPanelChkAction (Control);
    if (!EFI_ERROR (Status)) {
      break;
    }
    FormPanelCallAllChildProc ((UI_CONTROL_CLASS *)CURRENT_CLASS, Control, Msg, WParam, LParam);
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }

  return 1;
}

H2O_FORM_DIALOG_CLASS *
EFIAPI
GetH2OFormDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OFormDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OFormDialogProc;

  return CURRENT_CLASS;
}

