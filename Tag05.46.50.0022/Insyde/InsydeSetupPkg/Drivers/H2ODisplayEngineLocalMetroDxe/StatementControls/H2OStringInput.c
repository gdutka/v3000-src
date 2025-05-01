/** @file
  H2O String Input Controls

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

STATIC H2O_STRING_INPUT_CLASS      *mStringInputClass = NULL;
#define CURRENT_CLASS               mStringInputClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mStringInputChilds = L""
  L"<HorizontalLayout width='300' background-color='0xFFF2F2F2' height='wrap_content' >"
    L"<Label name='StringLabel' text='input string:' height='41' padding='12,0,0,0' width='119' font-size='16' textcolor='0xFF4D4D4D'/>"
    L"<VerticalLayout name='InputStringArea' padding='6,8,6,0' width='181' height='wrap_content' >"
      L"<Control/>"
      L"<Control name='InputStringBox' padding='1,1,1,1' width='173' background-color='0xFF999999' height='wrap_content'>"
        L"<UiEdit name='InputStringText' focusbkcolor='@menulightcolor' tabstop='true' font-size='19' height='wrap_content' width='171' taborder='1' background-color='0xFFF2F2F2'/>"
      L"</Control>"
      L"<Control/>"
    L"</VerticalLayout>"
  L"</HorizontalLayout>";
#endif

EFI_STATUS
EFIAPI
H2OStringInputOnSendChangeQ (
  IN H2O_STATEMENT_CONTROL      *This
  )
{
  UI_CONTROL                    *Control;
  UI_CONTROL                    *InputText;
  CHAR16                        *String;
  UINTN                         StringSize;
  EFI_HII_VALUE                 HiiValue;

  Control = (UI_CONTROL *) This;

  InputText = UiFindChildByName (Control, L"InputStringText");
  if (InputText == NULL) {
    return EFI_NOT_FOUND;
  }

  String     = ((UI_LABEL *) InputText)->Text;
  StringSize = StrSize (String);

  ZeroMem (&HiiValue, sizeof (HiiValue));
  HiiValue.Type      = EFI_IFR_TYPE_STRING;
  HiiValue.BufferLen = (UINT16) StringSize;
  HiiValue.Buffer    = (UINT8 *) AllocateCopyPool (StringSize, String);

  This->ValueChanged = TRUE;
  CopyMem (&This->HiiValue, &HiiValue, sizeof (EFI_HII_VALUE));
  H2OStatementControlOnSendChangeQ (This);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CreateStringInputControl (
  IN OUT UI_CONTROL              *Control
  )
{
  H2O_STRING_INPUT               *This;
  H2O_FORM_BROWSER_S             *Statement;
  UI_CONTROL                     *InputText;
  UI_CONTROL                     *InputBox;
  UI_CONTROL                     *InputArea;
  UINTN                          Index;
  SIZE                           ContentSize;
  CHAR16                         Str[20];
  CHAR16                        *MaxStr;

  if (Control == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  This = (H2O_STRING_INPUT *) Control;

  Statement = This->StatementControl.Statement;
  if (Statement == NULL) {
    return EFI_NOT_FOUND;
  }

  InputText = UiFindChildByName (Control, L"InputStringText");
  if (InputText == NULL) {
    return EFI_NOT_FOUND;
  }

  CONTROL_CLASS (This)->RemoveAllChild (InputText);

  ContentSize.cy = 0;
  if (Statement->HiiValue.BufferLen > sizeof (CHAR16)) {
    MaxStr = AllocateZeroPool (Statement->HiiValue.BufferLen);
    if (MaxStr != NULL) {
      for (Index = 0; Index < Statement->HiiValue.BufferLen / sizeof (CHAR16) - 1; Index++) {
        MaxStr[Index] = 'M';
      }
      UiSetAttribute (Control, L"text", MaxStr);
      ContentSize.cx = 9999;
      ContentSize.cy = 9999;
      ContentSize = CONTROL_CLASS (Control)->EstimateSize (Control, ContentSize);
      FreePool (MaxStr);
    }
  }
  ContentSize.cy = MIN (MAX(ContentSize.cy, 27), 100);
  UnicodeSPrint (Str, sizeof (Str), L"%d", ContentSize.cy);
  UiSetAttribute (InputText, L"height", Str);
  UiSetAttribute (InputText, L"text", (CHAR16 *)Statement->HiiValue.Buffer);

  UnicodeSPrint (Str, sizeof (Str), L"%d", Statement->Maximum);
  UiSetAttribute (InputText, L"maxlength", Str);

  InputBox = UiFindChildByName (Control, L"InputStringBox");
  ContentSize.cy += 2;
  UnicodeSPrint (Str, sizeof (Str), L"%d", ContentSize.cy);
  UiSetAttribute (InputBox, L"height", Str);

  InputArea = UiFindChildByName (Control, L"InputStringArea");
  ContentSize.cy += 12;
  UnicodeSPrint (Str, sizeof (Str), L"%d", ContentSize.cy);
  UiSetAttribute (InputArea, L"height", Str);

  SetFocus (InputText->Wnd);

  CONTROL_CLASS_INVALIDATE (This);

  return EFI_SUCCESS;
}

LRESULT
EFIAPI
H2OStringInputProc (
  IN HWND                        Hwnd,
  IN UINT32                      Msg,
  IN WPARAM                      WParam,
  IN LPARAM                      LParam
  )
{
  H2O_STRING_INPUT              *This;
  UI_CONTROL                    *Control;
  UI_CONTROL                    *Child;
  UI_CONTROL                    *FocusedControl;

  Control = GetUiControl (Hwnd);
  if (Control == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  This    = (H2O_STRING_INPUT *) Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_STRING_INPUT *) AllocateZeroPool (sizeof (H2O_STRING_INPUT));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    XmlCreateControl (mStringInputChilds, Control);
    break;

  case UI_NOTIFY_WINDOWINIT:
    CreateStringInputControl (Control);
    ((H2O_STATEMENT_CONTROL *)(Control))->OnSendChangeQ = H2OStringInputOnSendChangeQ;
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
    if (Control == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    FocusedControl = GetUiControl (GetFocus ());
    if (StrCmp (FocusedControl->Name, L"InputStringText") == 0) {
      FocusedControl = (UI_CONTROL *)GetNextStatementControlProc ((H2O_STATEMENT_CONTROL *)Control);
      if (FocusedControl != NULL) {
        SetFocus (FocusedControl->Wnd);
      }
    }
    break;

  case WM_SETFOCUS:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
    Child = UiFindChildByName (Control, L"InputStringText");
    if (Child != NULL) {
      SetFocus (Child->Wnd);
    }
    break;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);
  }

  return 0;
}

H2O_STRING_INPUT_CLASS *
EFIAPI
GetH2OStringInputClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"StringInput", (UI_CONTROL_CLASS *)GetH2OStatementControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = H2OStringInputProc;

  return CURRENT_CLASS;
}

