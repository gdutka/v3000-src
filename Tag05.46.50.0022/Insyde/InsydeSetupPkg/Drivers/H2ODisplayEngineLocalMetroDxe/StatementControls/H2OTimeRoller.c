/** @file
  UI Time Roller Controls

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
#include "H2OTimeItem.h"

STATIC H2O_TIME_ROLLER_CLASS     *mTimeRollerClass = NULL;
#define CURRENT_CLASS            mTimeRollerClass

BOOLEAN                          IsTimeKillFocus = FALSE;

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mTimeRollerChilds = L""
  L"<TimeItem name='Time' width='match_parent' height='match_parent' tabstop='true'/>"
  ;
#endif

STATIC
VOID
ChildOnTimeChange (
  IN H2O_TIME_ITEM               *This,
  IN EFI_TIME                    *EfiTime
  )
{
  UI_CONTROL                     *Control;
  UI_CONTROL                     *Parent;
  H2O_STATEMENT_CONTROL          *StatementControl;

  Control = (UI_CONTROL *)This;

  Parent = Control->Class->GetParent (Control);
  while (TRUE) {
    if (Parent == NULL) {
      return;
    }
    if (Parent->Class == (UI_CONTROL_CLASS *)CURRENT_CLASS) {
      break;
    }
    Parent = Parent->Class->GetParent (Parent);
  }

  StatementControl = (H2O_STATEMENT_CONTROL *)Parent;

  //
  // Update Time
  //
  StatementControl->ValueChanged = TRUE;
  StatementControl->HiiValue.Value.time.Hour   = EfiTime->Hour;
  StatementControl->HiiValue.Value.time.Minute = EfiTime->Minute;
  StatementControl->HiiValue.Value.time.Second = EfiTime->Second;
}

STATIC
VOID
ChildOnSetState (
  UI_CONTROL                    *Control,
  UI_STATE                      SetState,
  UI_STATE                      ClearState
  )
{
  if (!((SetState & UISTATE_FOCUSED) || (ClearState & UISTATE_FOCUSED))) {
    return ;
  }

  if (ClearState & UISTATE_FOCUSED) {
    IsTimeKillFocus = TRUE;
  }
}

STATIC
EFI_STATUS
CreateTimeRollerControl (
  IN OUT UI_CONTROL              *Control
  )
{
  H2O_STATEMENT_CONTROL          *StatementControl;
  H2O_FORM_BROWSER_S             *Statement;
  UI_CONTROL                     *Child;
  EFI_TIME                       EfiTime;
  CHAR16                         Str[20];

  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  StatementControl = (H2O_STATEMENT_CONTROL *) Control;

  Statement = StatementControl->Statement;
  if (Statement == NULL) {
    return EFI_NOT_FOUND;
  }

  Child = UiFindChildByName (Control, L"Time");
  if (Child == NULL) {
    return EFI_NOT_FOUND;
  }
  ((H2O_TIME_ITEM *)Child)->OnTimeChange = ChildOnTimeChange;
  Child->OnSetState = ChildOnSetState;

  //
  // Update Time
  //
  CopyMem (&StatementControl->HiiValue.Value.time, &Statement->HiiValue.Value.time, sizeof (EFI_HII_TIME));

  EfiTime.Hour   = StatementControl->HiiValue.Value.time.Hour;
  EfiTime.Minute = StatementControl->HiiValue.Value.time.Minute;
  EfiTime.Second = StatementControl->HiiValue.Value.time.Second;
  UnicodeSPrint (Str, sizeof (Str), L"0x%p", &EfiTime);
  UiSetAttribute (Child, L"efitime", Str);
  SetFocus (Child->Wnd);

  CONTROL_CLASS_INVALIDATE (Control);

  return EFI_SUCCESS;
}

LRESULT
EFIAPI
H2OTimeRollerProc (
  IN HWND                        Hwnd,
  IN UINT32                      Msg,
  IN WPARAM                      WParam,
  IN LPARAM                      LParam
  )
{
  H2O_TIME_ROLLER                *This;
  UI_CONTROL                     *Control;
  UI_CONTROL                     *Child;
  UI_CONTROL                     *FocusedControl;

  Control = GetUiControl (Hwnd);
  if (Control == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  This    = (H2O_TIME_ROLLER *) Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_TIME_ROLLER *) AllocateZeroPool (sizeof (H2O_TIME_ROLLER));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    XmlCreateControl (mTimeRollerChilds, Control);
    break;

  case UI_NOTIFY_WINDOWINIT:
    CreateTimeRollerControl (Control);
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
    if (Control == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    FocusedControl = GetUiControl (GetFocus ());
    if (StrCmp (FocusedControl->Name, L"Time") == 0) {
      FocusedControl = (UI_CONTROL *)GetNextStatementControlProc ((H2O_STATEMENT_CONTROL *)Control);
      if (FocusedControl != NULL) {
        SetFocus (FocusedControl->Wnd);
      }
    }
    break;

  case WM_SETFOCUS:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
    Child = UiFindChildByName (Control, L"Time");
    if (Child == NULL) {
      break;
    }
    if (IsTimeKillFocus) {
      IsTimeKillFocus = FALSE;
      FocusedControl = (UI_CONTROL *)GetNextStatementControlProc ((H2O_STATEMENT_CONTROL *)Control);
      if (FocusedControl != NULL) {
        SetFocus (FocusedControl->Wnd);
      }
    } else {
      SetFocus (Child->Wnd);
    }
    break;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

H2O_TIME_ROLLER_CLASS *
EFIAPI
GetH2OTimeRollerClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"TimeRoller", (UI_CONTROL_CLASS *)GetH2OStatementControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OTimeRollerProc;

  return CURRENT_CLASS;
}

