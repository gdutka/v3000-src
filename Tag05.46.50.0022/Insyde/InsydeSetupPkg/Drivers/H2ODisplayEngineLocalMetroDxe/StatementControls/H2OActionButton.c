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

#include <Guid/H2OFormDialog.h>
#include "H2ODisplayEngineLocalMetro.h"
#include "MetroUi.h"
#include "StatementControls.h"

STATIC H2O_ACTION_BUTTON_CLASS   *mActionButtonClass = NULL;
#define CURRENT_CLASS            mActionButtonClass

STATIC
H2O_ACTION_BUTTON_ACTION
H2OActionButtonOnActive (
  IN H2O_ACTION_BUTTON           *This
  )
{
  H2O_STATEMENT_CONTROL          *StatementControl;

  StatementControl = (H2O_STATEMENT_CONTROL *)This;

  if (StatementControl->Statement == NULL) {
    return ActionButtonUnsupported;
  }

  switch (StatementControl->Statement->QuestionId) {

  case QID_OK:
    return ActionButtonDialogOk;

  case QID_CANCEL:
    return ActionButtonDialogCancel;
  }

  return ActionButtonUnsupported;
}

STATIC
EFI_STATUS
CreateActionButtonControl (
  IN OUT UI_CONTROL              *Control
  )
{
  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  UiApplyAttributeList (Control, L"name='ActionButton' focusbkcolor='@menulightcolor' text-align='singleline|center' tabstop='true'");

  CONTROL_CLASS_INVALIDATE (Control);

  return EFI_SUCCESS;
}

LRESULT
EFIAPI
H2OActionButtonProc (
  IN HWND                        Hwnd,
  IN UINT32                      Msg,
  IN WPARAM                      WParam,
  IN LPARAM                      LParam
  )
{
  H2O_ACTION_BUTTON              *This;
  UI_CONTROL                     *Control;

  Control = GetUiControl (Hwnd);
  if (Control == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  This = (H2O_ACTION_BUTTON *) Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_ACTION_BUTTON *) AllocateZeroPool (sizeof (H2O_ACTION_BUTTON));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    break;

  case UI_NOTIFY_WINDOWINIT:
    CreateActionButtonControl (Control);
    This->OnActive = H2OActionButtonOnActive;
    break;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

H2O_ACTION_BUTTON_CLASS *
EFIAPI
GetH2OActionButtonClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"ActionButton", (UI_CONTROL_CLASS *)GetH2OStatementControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OActionButtonProc;

  return CURRENT_CLASS;
}

