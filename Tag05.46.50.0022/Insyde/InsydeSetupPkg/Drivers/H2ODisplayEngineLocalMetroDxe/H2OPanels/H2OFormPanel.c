/** @file
  Form Panel

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

#include <Library/LayoutLib.h>
#include <Library/HiiDbLib.h>
#include <Library/HiiStringLib.h>
#include <Library/HiiConfigAccessLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <Guid/H2OFormDialog.h>
#include "H2ODisplayEngineLocalMetro.h"
#include "MetroUi.h"
#include "MetroDialog.h"
#include "H2OPanels.h"
#include "StatementControls.h"

#define FORM_PANEL_TIMER_ID 1
STATIC FORM_PANEL_CLASS                *mFormPanelClass = NULL;
#define CURRENT_CLASS                  mFormPanelClass

STATIC
EFI_STATUS
FormPanelSendChangeQ (
  IN UI_CONTROL                        *Control
  )
{
  EFI_STATUS                           Status;
  UI_CONTROL                           *Child;
  UINTN                                Index;
  H2O_STATEMENT_CONTROL                *StatementControl;

  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < Control->ItemCount; Index++) {
    Child = Control->Items[Index];
    if (IsStatementControlClassName (Child->Class->ClassName)) {
      StatementControl = (H2O_STATEMENT_CONTROL *) Child;
      Status = StatementControl->OnSendChangeQ (StatementControl);
    } else {
      Status = FormPanelSendChangeQ (Child);
    }
    if (!EFI_ERROR (Status)) {
      //
      // Send Change Q by first StatementControl which value was changed
      //
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
FormPanelChkAction (
  IN UI_CONTROL                        *Control
  )
{
  EFI_STATUS                           Status;
  HWND                                 FocusedWnd;
  UI_CONTROL                           *FocusedControl;
  UI_CONTROL                           *ParentControl;
  H2O_ACTION_BUTTON                    *ActionButton;
  H2O_ACTION_BUTTON_ACTION             Action;

  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  FocusedWnd     = GetFocus ();
  FocusedControl = GetUiControl (FocusedWnd);
  ParentControl  = (FocusedWnd->parent != NULL) ? GetUiControl (FocusedWnd->parent) : NULL;

  if (FocusedControl != NULL && StrCmp (FocusedControl->Class->ClassName, L"ActionButton") == 0) {
    ActionButton = (H2O_ACTION_BUTTON *)FocusedControl;
  } else if (ParentControl != NULL && StrCmp (ParentControl->Class->ClassName, L"ActionButton") == 0) {
    ActionButton = (H2O_ACTION_BUTTON *)ParentControl;
  } else {
    ActionButton = NULL;
  }

  if (ActionButton != NULL) {
    Action = ActionButton->OnActive (ActionButton);
    switch (Action) {

    case ActionButtonDialogOk:
      Status = FormPanelSendChangeQ (Control);
      if (EFI_ERROR (Status)) {
        SendShutDNotify ();
      }
      return EFI_SUCCESS;

    case ActionButtonDialogCancel:
      SendShutDNotify ();
      return EFI_SUCCESS;

    default:
      break;
    }
  }

  return EFI_UNSUPPORTED;
}

STATIC
VOID
FormPanelCallAllStatementControlProc (
  IN UI_CONTROL                        *Control,
  IN UINT                              Msg,
  IN WPARAM                            WParam,
  IN LPARAM                            LParam
  )
{
  UI_CONTROL                           *Child;
  UINTN                                Index;

  if (Control == NULL) {
    return;
  }

  for (Index = 0; Index < Control->ItemCount; Index++) {
    Child = Control->Items[Index];
    if (IsStatementControlClassName (Child->Class->ClassName)) {
      Child->Class->WndProc (Child->Wnd, Msg, WParam, LParam);
    } else {
      FormPanelCallAllStatementControlProc (Child, Msg, WParam, LParam);
    }
  }
}

VOID
FormPanelCallAllChildProc (
  IN UI_CONTROL_CLASS                  *CurrentClass,
  IN UI_CONTROL                        *Control,
  IN UINT                              Msg,
  IN WPARAM                            WParam,
  IN LPARAM                            LParam
  )
{
  UI_CONTROL                           *Parent;

  if (CurrentClass == NULL || Control == NULL) {
    return;
  }

  Parent = NULL;
  if (Control->Class == CurrentClass) {
    Parent = Control;
  } else {
    Parent = Control->Class->GetParent (Control);
    while (TRUE) {
      if (Parent == NULL) {
        return;
      }
      if (Parent->Class == CurrentClass) {
        break;
      }
      Parent = Parent->Class->GetParent (Parent);
    }
  }
  if (Parent == NULL) {
    return;
  }

  FormPanelCallAllStatementControlProc (Parent, Msg, WParam, LParam);
}

EFI_STATUS
FormPanelCreateChildByVfrForm (
  IN UI_CONTROL                        *Control,
  IN H2O_PANEL_INFO                    *Panel,
  IN EFI_GUID                          *FormsetId,
  IN UINT16                            FormId,
  IN RECT                              *FormPanelRect,
  OUT UINT8                            *RefreshInterval
  )
{
  EFI_STATUS                           Status;
  H2O_FORM_BROWSER_P                   *Page;
  UINT32                               Index;
  UI_CONTROL                           *Child;
  UINT32                               TabOrder;
  H2O_FORM_BROWSER_S                   *Statement;
  H2O_PROPERTY_VALUE                   ControlTypePropValue;
  H2O_PROPERTY_VALUE                   SizePropValue;
  H2O_PROPERTY_VALUE                   ColorPropValue;
  H2O_PROPERTY_VALUE                   KerwordPropValue;
  RECT                                 StatementRect;
  CHAR16                               *ClassName;
  CHAR16                               *ImageStyle;
  UINT8                                RefreshTime;
  BOOLEAN                              IsStatementControl;
  CHAR16                               Str[100];

  if (Panel == NULL || FormsetId == NULL || FormPanelRect == NULL || RefreshInterval == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gFB->GetPInfoByForm (gFB, FormsetId, FormId, &Page);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (Page->NumberOfStatementIds == 0 || Page->StatementIds == NULL) {
    FreePool (Page);
    return EFI_NOT_FOUND;
  }

  RefreshTime = 0;
  TabOrder    = 0;
  for (Index = 0; Index < Page->NumberOfStatementIds; Index++) {
    Statement = NULL;
    Status = gFB->GetSInfo (gFB, Page->PageId, Page->StatementIds[Index], &Statement);
    if (EFI_ERROR (Status) || Statement == NULL) {
      continue;
    }

    if (Statement->QuestionId != 0 && Statement->RefreshInterval != 0) {
      RefreshTime = (RefreshTime == 0) ? Statement->RefreshInterval : MIN (Statement->RefreshInterval, RefreshTime);
    }

    //
    // Create a control which's class is from control type
    //
    ClassName = NULL;
    Status = GetStatementPropValue ("control-type", Statement, Panel, 0, 0, NULL, &ControlTypePropValue);
    if (!EFI_ERROR (Status)) {
      ClassName = GetClassNameByControlType (&ControlTypePropValue.H2OValue.Value.Guid);
    }
    if (ClassName == NULL) {
      ClassName = L"Label";
    }

    Child = CreateAddControl (ClassName, Control);
    ASSERT(Child != NULL);
    if (Child == NULL) {
      continue;
    }
    UnicodeSPrint (Str, sizeof(Str), L"name='FormPanel%08x_%08x'", Page->PageId,Statement->StatementId);
    UiApplyAttributeList (Child, Str);

    //
    // For Statement Control
    //
    IsStatementControl = IsStatementControlClassName (ClassName);
    if (IsStatementControl) {
      TabOrder++;
      UnicodeSPrint (Str, sizeof(Str), L"statement='0x%p' taborder='%d'", Statement, TabOrder);
      UiApplyAttributeList (Child, Str);
    }

    Status = GetStatementRect (Statement, Panel, FormPanelRect, &StatementRect);
    if (!EFI_ERROR (Status)) {
      UnicodeSPrint (
        Str,
        sizeof(Str),
        L"float='true' left='%d' top='%d' width='%d' height='%d'",
        StatementRect.left,
        StatementRect.top,
        (StatementRect.right - StatementRect.left),
        (StatementRect.bottom - StatementRect.top)
        );
      UiApplyAttributeList (Child, Str);
    }

    Status = GetStatementPropValue ("background-color", Statement, Panel, 0, 0, NULL, &ColorPropValue);
    if (!EFI_ERROR (Status)) {
      UnicodeSPrint (Str, sizeof(Str), L"background-color='0x%08x'", ColorPropValue.H2OValue.Value.U32);
      UiApplyAttributeList (Child, Str);
    }

    if (StrStr (ClassName, L"ActionButton") != NULL ||
      StrStr (ClassName, L"Label") != NULL) {
      Status = GetStatementPropValue ("font-size", Statement, Panel, 0, 0, NULL, &SizePropValue);
      if (EFI_ERROR (Status)) {
        SizePropValue.H2OValue.Value.U32 = 0;
      }
      Status = GetStatementPropValue ("color", Statement, Panel, 0, 0, NULL, &ColorPropValue);
      if (EFI_ERROR (Status)) {
        ColorPropValue.H2OValue.Value.U32 = 0;
      }
      UnicodeSPrint (
        Str,
        sizeof(Str),
        L"font-size='%d' textcolor='0x%08x' ",
        (SizePropValue.H2OValue.Value.U32 == 0)? 20 : SizePropValue.H2OValue.Value.U32,
        ColorPropValue.H2OValue.Value.U32
        );
      UiApplyAttributeList (Child, Str);
      UiSetAttribute (Child, L"text", Statement->Prompt);
    }

    if (Statement->Image != NULL) {
      ImageStyle = L"stretch";
      Status = GetStatementPropValue ("filter", Statement, Panel, 0, 0, NULL, &KerwordPropValue);
      if (!EFI_ERROR (Status) && KerwordPropValue.H2OValue.Type == H2O_VALUE_TYPE_UINT16) {
        if ((KerwordPropValue.H2OValue.Value.U16 | KEYWORD_FILTER_BRIGHTNESS) ==  KEYWORD_FILTER_BRIGHTNESS) {
          ImageStyle = L"stretch|light";
        } else if ((KerwordPropValue.H2OValue.Value.U16 | KEYWORD_FILTER_GRAYSCALE) ==  KEYWORD_FILTER_GRAYSCALE) {
          ImageStyle = L"stretch|gray";
        }
      }
      UnicodeSPrint (Str, sizeof(Str), L"background-image='0x%p' background-image-style='%s'", Statement->Image, ImageStyle);
      UiApplyAttributeList (Child, Str);
    }

    FreePool (Statement);
  }
  FreePool (Page);

  *RefreshInterval = RefreshTime;
  return EFI_SUCCESS;
}

EFI_STATUS
FormPanelCreateChildControls (
  IN UI_CONTROL                        *Control
  )
{
  H2O_PANEL_INFO                       *PanelInfo;
  EFI_STATUS                           Status;
  RECT                                 Rect;
  UINT8                                RefreshInterval;
  RECT                                 ScreenField;

  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  PanelInfo = (H2O_PANEL_INFO *) GetWindowLongPtr (Control->Wnd, GWLP_USERDATA);
  if (PanelInfo == NULL || PanelInfo->TargetFormId == 0) {
    return EFI_NOT_FOUND;
  }

  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ScreenField.left   = 0;
  ScreenField.top    = 0;
  ScreenField.right  = (INT32) GetSystemMetrics (SM_CXSCREEN);
  ScreenField.bottom = (INT32) GetSystemMetrics (SM_CYSCREEN);

  Status = GetFormPanelRect (
             &PanelInfo->TargetFormsetGuid,
             PanelInfo->TargetFormId,
             PanelInfo,
             &ScreenField,
             &Rect
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CONTROL_CLASS(Control)->RemoveAllChild (Control);

  Status = FormPanelCreateChildByVfrForm (
             Control,
             PanelInfo,
             &PanelInfo->TargetFormsetGuid,
             PanelInfo->TargetFormId,
             &Rect,
             &RefreshInterval
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  KillTimer (Control->Wnd, (UINT) FORM_PANEL_TIMER_ID);
  if (RefreshInterval != 0) {
    SetTimer (Control->Wnd, FORM_PANEL_TIMER_ID, (UINT) RefreshInterval == 1 ? 300 : RefreshInterval * 1000, NULL);
  }

  return EFI_SUCCESS;
}

LRESULT
EFIAPI
FormPanelProc (
  IN HWND                              Hwnd,
  IN UINT32                            Msg,
  IN WPARAM                            WParam,
  IN LPARAM                            LParam
  )
{
  FORM_PANEL                           *This;
  UI_CONTROL                           *Control;
  EFI_STATUS                           Status;

  This = (FORM_PANEL *) GetUiControl (Hwnd);
  if (This == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  Control = (UI_CONTROL *)This;

  switch (Msg) {

  case WM_CREATE:
    This = (FORM_PANEL *) AllocateZeroPool (sizeof (FORM_PANEL));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    break;

  case FB_NOTIFY_REPAINT:
    if (!This->FormPanelChildsInitialized) {
      This->FormPanelChildsInitialized = TRUE;
      FormPanelCreateChildControls (Control);
    }
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
    CONTROL_CLASS_INVALIDATE (Control);
    break;

  case WM_TIMER:
    if (WParam == FORM_PANEL_TIMER_ID) {
      FormPanelCreateChildControls (Control);
    }
    break;

  case WM_DESTROY:
    KillTimer (Hwnd, FORM_PANEL_TIMER_ID);
    break;

  case UI_NOTIFY_CLICK:
  case UI_NOTIFY_CARRIAGE_RETURN:
    Status = FormPanelChkAction (Control);
    if (!EFI_ERROR (Status)) {
      break;
    }
    FormPanelCallAllChildProc ((UI_CONTROL_CLASS *)CURRENT_CLASS, Control, Msg, WParam, LParam);
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);

  case WM_NCHITTEST:
    return HTCLIENT;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

FORM_PANEL_CLASS *
EFIAPI
GetFormPanelClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"FormPanel", (UI_CONTROL_CLASS *)GetControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = FormPanelProc;

  return CURRENT_CLASS;
}

