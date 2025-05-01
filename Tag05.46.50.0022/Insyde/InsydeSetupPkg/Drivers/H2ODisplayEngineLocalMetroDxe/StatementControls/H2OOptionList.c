/** @file
  UI Option List Controls

;******************************************************************************
;* Copyright (c) 2019 - 2021, Insyde Software Corp. All Rights Reserved.
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

STATIC H2O_OPTION_LIST_CLASS      *mOptionListClass = NULL;
#define CURRENT_CLASS             mOptionListClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mOptionListChilds = L""
  L"<ListView name='OptionList' width='match_parent' height='match_parent' child-padding='2' vscrollbar='true' tabstop='true' taborder='1'/>";

CHAR16 *mOptionChilds = L""
  L"<HorizontalLayout height='40'>"
    L"<Label name='Option' padding='0,20,0,20' text-align='singleline|center' height='40' font-size='19' background-color='0x0' textcolor='0xFFFFFFFF' text-overflow='ellipsis'/>"
    L"<Texture name='OptionSelectedIcon' float='true' left='260' top='12' width='21' height='17' visibility='false' background-image='@DialogSelectedIcon' background-color='0x0'/>"
  L"</HorizontalLayout>";
#endif

STATIC
VOID
OptionOnSetState (
  UI_CONTROL                    *Control,
  UI_STATE                      SetState,
  UI_STATE                      ClearState
  )
{
  UI_CONTROL                    *Child;
  COLORREF                      Color;
  EFI_IMAGE_INPUT               *Image;

  if (!((SetState & UISTATE_SELECTED) || (ClearState & UISTATE_SELECTED))) {
    return ;
  }

  if (SetState & UISTATE_SELECTED) {
    Child = UiFindChildByName (Control, L"Option");
    UiSetAttribute (Child, L"marquee", L"true");
    UiApplyAttributeList (Child, L"textcolor='0xFFFFFFFF' background-color='@menulightcolor' ");
    if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
      UiApplyAttributeList (Child, L"textcolor='0xFF404040'");
    }
    Child = UiFindChildByName (Control, L"OptionSelectedIcon");
    ASSERT (Child != NULL);
    if (Child == NULL) {
      return;
    }

    if (IsWindowVisible (Child->Wnd)) {
      Color = GetColorValue (L"0xFFFFFFFF");
      Image = GetImageByString (Child->BkImage);
      if (Image != NULL) {
        AdjustImage (Child, Image->Bitmap, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)&Color);
      }
    }

  } else {
    Child = UiFindChildByName (Control, L"Option");
    UiSetAttribute (Child, L"marquee", L"false");
    UiApplyAttributeList (Child, L"textcolor='0xFF404040' background-color='0xFFE6E6E6' ");
    if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
      UiApplyAttributeList (Child, L"textcolor='0xFFFFFFFF' background-color='0x0'");
    }
    Child = UiFindChildByName (Control, L"OptionSelectedIcon");
    ASSERT (Child != NULL);
    if (Child == NULL) {
      return;
    }

    if (IsWindowVisible (Child->Wnd)) {
      Color = GetColorValue (L"@menulightcolor");
      Image = GetImageByString (Child->BkImage);
      if (Image != NULL) {
        AdjustImage (Child, Image->Bitmap, (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)&Color);
      }
    }
  }
}

STATIC
VOID
ChildOnItemClick (
  UI_LIST_VIEW                  *This,
  UI_CONTROL                    *Item,
  UINT32                        Index
  )
{
  UI_CONTROL                    *Control;
  UI_CONTROL                    *Parent;
  H2O_STATEMENT_CONTROL         *StatementControl;

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

  Index = (UINT32) GetWindowLongPtr (Item->Wnd, GWLP_USERDATA);
  if (StatementControl->Statement->NumberOfOptions > Index) {
    CopyMem (
      &StatementControl->HiiValue,
      &StatementControl->Statement->Options[Index].HiiValue,
      sizeof (EFI_HII_VALUE)
      );
    StatementControl->ValueChanged = TRUE;
    if (StatementControl->OnSendChangeQ != NULL) {
      StatementControl->OnSendChangeQ (StatementControl);
    }
  }
}

STATIC
EFI_STATUS
CreateOptionListControl (
  IN OUT UI_CONTROL              *Control
  )
{
  EFI_STATUS                     Status;
  H2O_OPTION_LIST                *This;
  H2O_FORM_BROWSER_S             *Statement;
  UINT32                         Index;
  H2O_FORM_BROWSER_O             *Option;
  UI_CONTROL                     *OptionList;
  UI_CONTROL                     *OptionControl;
  UI_CONTROL                     *Child;
  UI_CONTROL                     *Icon;
  INTN                           Result;

  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  This = (H2O_OPTION_LIST *) Control;

  Statement = This->StatementControl.Statement;
  if (Statement == NULL) {
    return EFI_NOT_FOUND;
  }

  if (Statement == NULL ||
    Statement->NumberOfOptions == 0 ||
    Statement->Options == NULL) {
    return EFI_NOT_FOUND;
  }

  OptionList = UiFindChildByName (Control, L"OptionList");
  if (OptionList == NULL) {
    return EFI_NOT_FOUND;
  }

  CONTROL_CLASS (This)->RemoveAllChild (OptionList);

  if (OptionList->VScrollBar != NULL) {
    UiSetAttribute (OptionList->VScrollBar, L"displayscrollbar", L"false");
    CONTROL_CLASS_INVALIDATE (OptionList);
  }

  ((UI_LIST_VIEW *)OptionList)->OnItemClick = ChildOnItemClick;

  for (Index = 0; Index < (UINT32)Statement->NumberOfOptions; Index ++) {
    Option = &Statement->Options[Index];

    OptionControl = XmlCreateControl (mOptionChilds, OptionList);
    if (OptionControl == NULL) {
      break;
    }

    SetWindowLongPtr (OptionControl->Wnd, GWLP_USERDATA, (INTN)Index);
    OptionControl->OnSetState = OptionOnSetState;

    if (Option->Text != NULL) {
      Child = UiFindChildByName (OptionControl, L"Option");
      UiSetAttribute (Child, L"text", Option->Text);
    }

    //
    // Set Selection
    //
    Icon = UiFindChildByName (OptionControl, L"OptionSelectedIcon");
    if (Icon != NULL) {
      Status = CompareHiiValue (&Statement->HiiValue, &Option->HiiValue, &Result);
      if (!EFI_ERROR (Status) && Result == 0) {
        UiSetAttribute (Icon, L"visibility", L"true");
        LIST_VIEW_CLASS (OptionList)->SetSelection ((UI_LIST_VIEW *)OptionList, (INT32)Index, TRUE);
      } else {
        UiSetAttribute (Icon, L"visibility", L"false");
      }
    }
  }

  SetFocus (OptionList->Wnd);

  CONTROL_CLASS_INVALIDATE (This);

  return EFI_SUCCESS;
}

LRESULT
EFIAPI
H2OOptionListProc (
  IN HWND                        Hwnd,
  IN UINT32                      Msg,
  IN WPARAM                      WParam,
  IN LPARAM                      LParam
  )
{
  H2O_OPTION_LIST                *This;
  UI_CONTROL                     *Control;
  UI_CONTROL                     *Child;

  Control = GetUiControl (Hwnd);
  if (Control == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  This    = (H2O_OPTION_LIST *) Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_OPTION_LIST *) AllocateZeroPool (sizeof (H2O_OPTION_LIST));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    XmlCreateControl (mOptionListChilds, Control);
    break;

  case UI_NOTIFY_WINDOWINIT:
    CreateOptionListControl (Control);
    break;

  case WM_SETFOCUS:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
    Child = UiFindChildByName (Control, L"OptionList");
    if (Child != NULL) {
      SetFocus (Child->Wnd);
    }
    break;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

H2O_OPTION_LIST_CLASS *
EFIAPI
GetH2OOptionListClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"OptionList", (UI_CONTROL_CLASS *)GetH2OStatementControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OOptionListProc;

  return CURRENT_CLASS;
}

