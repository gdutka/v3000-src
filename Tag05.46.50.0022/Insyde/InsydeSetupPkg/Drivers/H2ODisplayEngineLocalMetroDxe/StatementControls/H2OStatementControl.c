/** @file
  UI Statement Control

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

STATIC H2O_STATEMENT_CONTROL_CLASS *mH2OStatementControlClass = NULL;
#define CURRENT_CLASS              mH2OStatementControlClass

BOOLEAN
IsStatementControlClassName (
  IN CHAR16                       *ClassName
  )
{
  UINT32                          Index;
  CHAR16                          *H2OStatementControlClassName[] = {
                                     L"ActionButton",
                                     L"DateRoller",
                                     L"NumericInput",
                                     L"OptionList",
                                     L"OrderedListOptionList",
                                     L"PasswordInput",
                                     L"StringInput",
                                     L"TimeRoller",
                                     };

  if (ClassName == NULL) {
    return FALSE;
  }

  for (Index = 0; Index < ARRAY_SIZE(H2OStatementControlClassName); Index ++) {
    if (StrCmp (H2OStatementControlClassName[Index], ClassName) == 0) {
      return TRUE;
    }
  }

  return FALSE;
}

H2O_STATEMENT_CONTROL *
GetNextStatementControlProc (
  IN H2O_STATEMENT_CONTROL        *This
  )
{
  UI_CONTROL                      *Control;
  UI_CONTROL                      *Parent;
  UI_CONTROL                      *Brother;
  UINTN                           Index;
  BOOLEAN                         Found;

  if (This == NULL) {
    return NULL;
  }

  Control = (UI_CONTROL *)This;

  Parent = Control->Class->GetParent (Control);
  if (Parent == NULL) {
    return NULL;
  }

  Found = FALSE;
  for (Index = 0; Index < Parent->ItemCount; Index++) {
    Brother = Parent->Items[Index];
    if (IsStatementControlClassName (Brother->Class->ClassName)) {
      if (Found) {
        return (H2O_STATEMENT_CONTROL *)Brother;
      }
      if (Brother == Control) {
        Found = TRUE;
      }
    }
  }
  return NULL;
}

EFI_STATUS
EFIAPI
H2OStatementControlOnSendChangeQ (
  IN H2O_STATEMENT_CONTROL        *This
  )
{
  if (!This->ValueChanged) {
    return EFI_NOT_READY;
  }

  return SendChangeQNotify (
           This->Statement->PageId,
           This->Statement->QuestionId,
           &This->HiiValue
           );
}

BOOLEAN
EFIAPI
H2OStatementControlSetAttribute (
  IN OUT UI_CONTROL              *Control,
  IN CHAR16                      *Name,
  IN CHAR16                      *Value
  )
{
  EFI_STATUS                     Status;
  H2O_STATEMENT_CONTROL          *This;
  H2O_FORM_BROWSER_S             *Statement;

  This = (H2O_STATEMENT_CONTROL *) Control;

  if (StrCmp (Name, L"statement") == 0) {
    if (This->Statement != NULL) {
      FreePool (This->Statement);
      This->Statement = NULL;
    }

    Statement = (H2O_FORM_BROWSER_S *)(UINTN) StrToUInt (Value, 16, &Status);
    if (Statement != NULL) {
      This->Statement = AllocateCopyPool (sizeof(H2O_FORM_BROWSER_S), Statement);
    }
  } else {
    return PARENT_CLASS_SET_ATTRIBUTE (CURRENT_CLASS, Control, Name, Value);
  }
  CONTROL_CLASS_INVALIDATE (This);

  return TRUE;
}

LRESULT
EFIAPI
H2OStatementControlProc (
  IN HWND                        Hwnd,
  IN UINT32                      Msg,
  IN WPARAM                      WParam,
  IN LPARAM                      LParam
  )
{
  H2O_STATEMENT_CONTROL          *This;
  UI_CONTROL                     *Control;

  Control = (UI_CONTROL *) GetWindowLongPtr (Hwnd, 0);
  if (Control == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  This = (H2O_STATEMENT_CONTROL *) Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_STATEMENT_CONTROL *) AllocateZeroPool (sizeof (H2O_STATEMENT_CONTROL));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    if (This != NULL) {
      This->OnSendChangeQ = H2OStatementControlOnSendChangeQ;
    }
    break;

  case WM_DESTROY:
    if (This->Statement != NULL) {
      FreePool (This->Statement);
    }
    break;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

H2O_STATEMENT_CONTROL_CLASS *
EFIAPI
GetH2OStatementControlClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OStatementControl", (UI_CONTROL_CLASS *)GetButtonClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = H2OStatementControlProc;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetAttribute = H2OStatementControlSetAttribute;

  return CURRENT_CLASS;
}

