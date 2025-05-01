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
#include "H2ODateItem.h"

STATIC H2O_DATE_ROLLER_CLASS     *mDateRollerClass = NULL;
#define CURRENT_CLASS            mDateRollerClass

BOOLEAN                          IsDateKillFocus = TRUE;

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mDateRollerChilds = L""
  L"<DateItem name='Date' width='match_parent' height='match_parent' tabstop='true'/>"
  ;
#endif

STATIC
VOID
ChildOnDateChange (
  IN H2O_DATE_ITEM               *This,
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
  // Update Date
  //
  StatementControl->ValueChanged = TRUE;
  StatementControl->HiiValue.Value.date.Year  = EfiTime->Year;
  StatementControl->HiiValue.Value.date.Month = EfiTime->Month;
  StatementControl->HiiValue.Value.date.Day   = EfiTime->Day;
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
    IsDateKillFocus = TRUE;
  }
}

STATIC
EFI_STATUS
CreateDateRollerControl (
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

  Child = UiFindChildByName (Control, L"Date");
  if (Child == NULL) {
    return EFI_NOT_FOUND;
  }
  ((H2O_DATE_ITEM *)Child)->OnDateChange = ChildOnDateChange;
  Child->OnSetState = ChildOnSetState;

  //
  // Update Date
  //
  CopyMem (&StatementControl->HiiValue.Value.date, &Statement->HiiValue.Value.date, sizeof (EFI_HII_DATE));

  EfiTime.Year  = StatementControl->HiiValue.Value.date.Year;
  EfiTime.Month = StatementControl->HiiValue.Value.date.Month;
  EfiTime.Day   = StatementControl->HiiValue.Value.date.Day;
  UnicodeSPrint (Str, sizeof (Str), L"0x%p", &EfiTime);
  UiSetAttribute (Child, L"efitime", Str);
  SetFocus (Child->Wnd);

  CONTROL_CLASS_INVALIDATE (Control);

  return EFI_SUCCESS;
}

LRESULT
EFIAPI
H2ODateRollerProc (
  IN HWND                        Hwnd,
  IN UINT32                      Msg,
  IN WPARAM                      WParam,
  IN LPARAM                      LParam
  )
{
  H2O_DATE_ROLLER                *This;
  UI_CONTROL                     *Control;
  UI_CONTROL                     *Child;
  UI_CONTROL                     *FocusedControl;

  Control = GetUiControl (Hwnd);
  if (Control == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  This    = (H2O_DATE_ROLLER *) Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_DATE_ROLLER *) AllocateZeroPool (sizeof (H2O_DATE_ROLLER));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    XmlCreateControl (mDateRollerChilds, Control);
    break;

  case UI_NOTIFY_WINDOWINIT:
    CreateDateRollerControl (Control);
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
    if (Control == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    FocusedControl = GetUiControl (GetFocus ());
    if (StrCmp (FocusedControl->Name, L"Date") == 0) {
      FocusedControl = (UI_CONTROL *)GetNextStatementControlProc ((H2O_STATEMENT_CONTROL *)Control);
      if (FocusedControl != NULL) {
        SetFocus (FocusedControl->Wnd);
      }
    }
    break;

  case WM_SETFOCUS:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
    Child = UiFindChildByName (Control, L"Date");
    if (Child == NULL) {
      break;
    }
    if (IsDateKillFocus) {
      IsDateKillFocus = FALSE;
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

H2O_DATE_ROLLER_CLASS *
EFIAPI
GetH2ODateRollerClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"DateRoller", (UI_CONTROL_CLASS *)GetH2OStatementControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2ODateRollerProc;

  return CURRENT_CLASS;
}

