/** @file
  H2O Numeric Input Controls

;******************************************************************************
;* Copyright (c) 2019 - 2020, Insyde Software Corp. All Rights Reserved.
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

STATIC H2O_NUMERIC_INPUT_CLASS      *mNumericInputClass = NULL;
#define CURRENT_CLASS               mNumericInputClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mNumericInputChilds = L""
  L"<VerticalLayout>"
    L"<VerticalLayout width='300' background-color='0xFFF2F2F2' height='49'>"
      L"<Label name='NumericLabel' text='input number:' height='20' padding='2,0,0,2' font-size='16' textcolor='0xFF4D4D4D'/>"
      L"<VerticalLayout padding='2,2,0,2' height='28' >"
        L"<Control padding='2,1,1,1' background-color='0xFF999999' >"
          L"<HorizontalLayout>"
            L"<UiEdit name='InputText' padding='0,0,0,19' font-size='19' focusbkcolor='@menulightcolor' focustextcolor='0xFF404040' tabstop='true' height='27' taborder='1' background-color='0xFFF2F2F2'/>"
            L"<Label name='Prefix' text=''  font-size='19' height='27' width='wrap_content' background-color='0x00000000' float='true' />"
          L"</HorizontalLayout>"
        L"</Control>"
      L"</VerticalLayout>"
    L"</VerticalLayout>";
#endif

EFI_STATUS
EFIAPI
H2ONumericInputOnSendChangeQ (
  IN H2O_STATEMENT_CONTROL      *This
  )
{
  UI_CONTROL                    *Control;
  UI_CONTROL                    *InputText;
  CHAR16                        *ValueStr;
  H2O_FORM_BROWSER_S            *Statement;
  UINT64                        EditValue;
  EFI_HII_VALUE                 HiiValue;
  UI_EDIT                       *Edit;
  UINT8                         NumericFlags;

  Control = (UI_CONTROL *)This;
  Edit    = (UI_EDIT *) Control;

  InputText = UiFindChildByName (Control, L"InputText");
  if (InputText == NULL) {
    return EFI_NOT_FOUND;
  }

  ValueStr = ((UI_LABEL *) InputText)->Text;

  Statement = This->Statement;
  if (Statement == NULL) {
    return EFI_NOT_FOUND;
  }

  NumericFlags = ConvertToIfrNumericFlags (Edit->ValueType == HEX_VALUE, Edit->ValueType == SIGNED_DEC_VALUE, Edit->ValueSize);
  EditValue    = IfrNumericConvertStrToValue (NumericFlags, ValueStr);

  if (IfrNumericIsValueInRange (NumericFlags, EditValue, Edit->MinValue, Edit->MaxValue)) {
    ZeroMem (&HiiValue, sizeof (HiiValue));
    HiiValue.Value.u64 = EditValue;
    This->ValueChanged = TRUE;
    CopyMem (&This->HiiValue, &HiiValue, sizeof (EFI_HII_VALUE));
    H2OStatementControlOnSendChangeQ (This);
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CreateNumericInputControl (
  IN OUT UI_CONTROL              *Control
  )
{
  H2O_NUMERIC_INPUT              *This;
  H2O_FORM_BROWSER_S             *Statement;
  UI_CONTROL                     *InputText;
  UI_CONTROL                     *Label;
  CHAR16                         Str[20];
  UINTN                          Size;

  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  This = (H2O_NUMERIC_INPUT *) Control;

  Statement = This->StatementControl.Statement;
  if (Statement == NULL) {
    return EFI_NOT_FOUND;
  }

  InputText = UiFindChildByName (Control, L"InputText");
  if (InputText == NULL) {
    return EFI_NOT_FOUND;
  }

  CONTROL_CLASS (This)->RemoveAllChild (InputText);

  UnicodeSPrint (Str, sizeof (Str), L"0x%lx", Statement->Minimum);
  UiSetAttribute (InputText, L"minvalue", Str);
  UnicodeSPrint (Str, sizeof (Str), L"0x%lx", Statement->Maximum);
  UiSetAttribute (InputText, L"maxvalue", Str);
  Size = IfrNumericGetValueSize (Statement->Flags);
  UnicodeSPrint (Str, sizeof (Str), L"%d", Size);
  UiSetAttribute (Control, L"valuesize", Str);

  if ((Statement->Flags & EFI_IFR_DISPLAY) == EFI_IFR_DISPLAY_UINT_HEX) {
    UiSetAttribute (InputText, L"valuetype", L"hex");
    Label = UiFindChildByName (Control, L"Prefix");
    if (Label != NULL) {
      UiSetAttribute (Label, L"text", L"0x");
    }
  } else {
    UiSetAttribute (InputText, L"valuetype", ((Statement->Flags & EFI_IFR_DISPLAY) == EFI_IFR_DISPLAY_INT_DEC) ? L"signeddec" : L"dec");
    UiSetAttribute (InputText, L"padding",   L"0,0,0,0");
  }

  SetFocus (InputText->Wnd);

  CONTROL_CLASS_INVALIDATE (This);

  return EFI_SUCCESS;
}

LRESULT
EFIAPI
H2ONumericInputProc (
  IN HWND                        Hwnd,
  IN UINT32                      Msg,
  IN WPARAM                      WParam,
  IN LPARAM                      LParam
  )
{
  H2O_NUMERIC_INPUT             *This;
  UI_CONTROL                    *Control;
  UI_CONTROL                    *Child;
  UI_CONTROL                    *FocusedControl;

  Control = GetUiControl (Hwnd);
  if (Control == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  This    = (H2O_NUMERIC_INPUT *) Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_NUMERIC_INPUT *) AllocateZeroPool (sizeof (H2O_NUMERIC_INPUT));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    XmlCreateControl (mNumericInputChilds, Control);
    break;

  case UI_NOTIFY_WINDOWINIT:
    CreateNumericInputControl (Control);
    ((H2O_STATEMENT_CONTROL *)(Control))->OnSendChangeQ = H2ONumericInputOnSendChangeQ;
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
    if (Control == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    FocusedControl = GetUiControl (GetFocus ());
    if (StrCmp (FocusedControl->Name, L"InputText") == 0) {
      FocusedControl = (UI_CONTROL *)GetNextStatementControlProc ((H2O_STATEMENT_CONTROL *)Control);
      if (FocusedControl != NULL) {
        SetFocus (FocusedControl->Wnd);
      }
    }
    break;

  case WM_SETFOCUS:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
    Child = UiFindChildByName (Control, L"InputText");
    if (Child != NULL) {
      SetFocus (Child->Wnd);
    }
    break;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

H2O_NUMERIC_INPUT_CLASS *
EFIAPI
GetH2ONumericInputClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"NumericInput", (UI_CONTROL_CLASS *)GetH2OStatementControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = H2ONumericInputProc;

  return CURRENT_CLASS;
}

