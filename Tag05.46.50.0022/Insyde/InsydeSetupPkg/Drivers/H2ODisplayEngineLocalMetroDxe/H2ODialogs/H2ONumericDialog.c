/** @file
  UI ordered list control

;******************************************************************************
;* Copyright (c) 2014 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "MetroDialog.h"

STATIC H2O_NUMERIC_DIALOG_CLASS    *mH2ONumericDialogClass = NULL;
#define CURRENT_CLASS               mH2ONumericDialogClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mNumericDialogChilds = L""
  L"<VerticalLayout>"
    L"<VerticalLayout width='300' background-color='0xFFF2F2F2' height='49'>"
      L"<Label name='NumericLabel' text='input number:' height='18' padding='2,0,0,2' font-size='16' textcolor='0xFF4D4D4D'/>"
      L"<VerticalLayout padding='2,2,0,2' height='26' >"
        L"<Control padding='2,1,1,1' background-color='0xFF999999' >"
          L"<HorizontalLayout>"
            L"<UiEdit name='InputNumericText' padding='0,0,0,19' font-size='19' focusbkcolor='@menulightcolor' tabstop='true' height='27' taborder='1' background-color='0xFFF2F2F2' valuetype='dec'/>"
            L"<Label name='Prefix' text=''  font-size='19' height='27' width='wrap_content' background-color='0x00000000' float='true' />"
          L"</HorizontalLayout>"
        L"</Control>"
      L"</VerticalLayout>"
    L"</VerticalLayout>"
    L"<Control height='2'/>"
    L"<Control height='10'/>"
    L"<HorizontalLayout padding='0,0,30,0' child-padding='2' width='300' height='25'>"
      L"<Control/>"
      L"<Button name='Ok' focusbkcolor='@menulightcolor' text='Enter' text-align='singleline|center' height='30' width='55' taborder='2' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF'/>"
      L"<Button name='Cancel' focusbkcolor='@menulightcolor' text='Cancel' text-align='singleline|center' height='30' width='55' taborder='3' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF'/>"
    L"</HorizontalLayout>"
  L"</VerticalLayout>";

CHAR16 *mNumericDialogWithoutSendFormChilds = L""
  L"<Control name='NumericDialogWithoutSendForm'>"
    L"<VerticalLayout padding='20,30,20,30' background-color='@menucolor'>"
      L"<VerticalLayout name='TitleLayout'>"
        L"<Label text-align='center' textcolor='0xFFFFFFFF' font-size='19' name='DialogTitle' height='40'/>"
        L"<Control height='15'/>"
        L"<Control height='10' background-image='@DialogSeparator' background-color='0x0' background-image-style='center'/>"
        L"<Control height='15'/>"
      L"</VerticalLayout>"
      L"<HorizontalLayout padding='0,0,10,0' name='DialogPasswordInput' height='41'>"
        L"<VerticalLayout padding='6,8,6,8' background-color='0xFFF2F2F2' height='29'>"
          L"<Control padding='1,1,1,1' background-color='0xFF999999' height='27'>"
            L"<HorizontalLayout>"
              L"<UiEdit font-size='19' padding='7,3,0,22' name='InputNumericText' background-color='0xFFF2F2F2' focusbkcolor='@menulightcolor'  height='20' tabstop='true' taborder='1'  valuetype='dec'/>"
              L"<Label name='Prefix' text=''  font-size='19' height='27' width='wrap_content' background-color='0x00000000' float='true' />"
            L"</HorizontalLayout>"
          L"</Control>"
        L"</VerticalLayout>"
      L"</HorizontalLayout>"
      L"<HorizontalLayout child-padding='2' visibility='false' name='DialogButtonList' height='30'/>"
    L"</VerticalLayout>"
    L"<Texture name='FormHalo' float='true' height='-1' width='-1' background-image='@FormHalo' scale9grid='23,26,22,31'/>"
  L"</Control>";
#endif

STATIC
LRESULT
H2ONumericDialogWithoutSendFormProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  );

STATIC
EFI_STATUS
SendNumericChange (
  IN H2O_NUMERIC_DIALOG         *This
  )
{
  UI_CONTROL                    *Control;
  CHAR16                        *ValueStr;
  H2O_FORM_BROWSER_Q            *CurrentQ;
  EFI_HII_VALUE                 HiiValue;
  UINT64                        EditValue;
  UINT8                         NumericFlags;
  UI_EDIT                       *Edit;

  Control  = UiFindChildByName (This, L"InputNumericText");
  Edit     = (UI_EDIT *) Control;
  ValueStr = ((UI_LABEL *) Control)->Text;
  CurrentQ = mFbDialog->H2OStatement;

  ZeroMem (&HiiValue, sizeof (HiiValue));

  NumericFlags = ConvertToIfrNumericFlags (Edit->ValueType == HEX_VALUE, Edit->ValueType == SIGNED_DEC_VALUE, Edit->ValueSize);
  if (IfrNumericIsEmptyStr (NumericFlags, ValueStr)) {
    EditValue = CurrentQ->HiiValue.Value.u64;
  } else {
    EditValue = IfrNumericConvertStrToValue (NumericFlags, ValueStr);
  }

  if (IfrNumericIsValueInRange (NumericFlags, EditValue, Edit->MinValue, Edit->MaxValue)) {
    CopyMem (&HiiValue, &CurrentQ->HiiValue, sizeof (EFI_HII_VALUE));
    HiiValue.Value.u64 = EditValue;
    SendChangeQNotify (0, 0, &HiiValue);
  }

  return EFI_SUCCESS;
}

STATIC
LRESULT
H2ONumericDialogProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_NUMERIC_DIALOG            *This;
  UI_CONTROL                    *Control;
  EFI_IMAGE_INPUT               *FormsetImage;
  CHAR16                        Str[30];
  UINTN                         Index;
  HWND                          FocusedWnd;
  H2O_FORM_BROWSER_Q            *CurrentQ;
  UINTN                         Size;

  if (!mIsSendForm) {
    return H2ONumericDialogWithoutSendFormProc (Wnd, Msg, WParam, LParam);
  }

  CurrentQ = mFbDialog->H2OStatement;
  Control = GetUiControl (Wnd);
  This    = (H2O_NUMERIC_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_NUMERIC_DIALOG *) AllocateZeroPool (sizeof (H2O_NUMERIC_DIALOG));
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
      XmlCreateControl (mCommonDialogWithHelpChilds, Control);
    }
    break;

  case UI_NOTIFY_WINDOWINIT:
    if (CurrentQ == NULL) {
      break;
    }

    Control = UiFindChildByName (This, L"DialogTitle");
    if (mFbDialog->TitleString != NULL) {
      UiSetAttribute (Control, L"text", mFbDialog->TitleString);
    } else {
      UiSetAttribute (Control, L"text", CurrentQ->Prompt);
    }

    FormsetImage = GetCurrentFormSetImage ();
    if (FormsetImage != NULL) {
      Control = UiFindChildByName (This, L"DialogImage");
      UnicodeSPrint (Str, sizeof (Str), L"0x%p", FormsetImage);
      UiSetAttribute (Control, L"background-image", Str);
      UiSetAttribute (Control, L"visibility", L"true");
    }

    if (CurrentQ->Help != NULL) {
      Control = UiFindChildByName (This, L"DialogText");
      UiSetAttribute (Control, L"text", CurrentQ->Help);
    }

    Control = UiFindChildByName (This, L"DialogTextScrollView");
    if (Control != NULL) {
      Control->OnSetState = H2OCommonDialogWithHelpOnSetState;
    }

    Control = UiFindChildByName (This, L"Content");
    XmlCreateControl (mNumericDialogChilds, Control);

    //
    // set maximum value and display type
    //
    if (mFbDialog->BodyInputCount != 0) {
      Control = UiFindChildByName (This, L"InputNumericText");
      UnicodeSPrint (Str, sizeof (Str), L"0x%lx", CurrentQ->Maximum);
      UiSetAttribute (Control, L"maxvalue", Str);
      UnicodeSPrint (Str, sizeof (Str), L"0x%lx", CurrentQ->Minimum);
      UiSetAttribute (Control, L"minvalue", Str);
      Size = IfrNumericGetValueSize (CurrentQ->Flags);
      UnicodeSPrint (Str, sizeof (Str), L"%d", Size);
      UiSetAttribute (Control, L"valuesize", Str);

      if ((CurrentQ->Flags & EFI_IFR_DISPLAY) == EFI_IFR_DISPLAY_UINT_HEX) {
        UiSetAttribute (Control, L"valuetype", L"hex");
        Control = UiFindChildByName (This, L"Prefix");
        if (Control != NULL) {
          UiSetAttribute (Control, L"text", L"0x");
        }
      } else {
        UiSetAttribute (Control, L"valuetype", ((CurrentQ->Flags & EFI_IFR_DISPLAY) == EFI_IFR_DISPLAY_INT_DEC) ? L"signeddec" : L"dec");
        UiSetAttribute (Control, L"padding",   L"7,3,0,3");
      }
    }

    if (mFbDialog->ButtonCount == 2) {
      Index = 0;
      Control = UiFindChildByName (This, L"Ok");
      UiSetAttribute (Control, L"text", mFbDialog->ButtonStringArray[Index++]);
      Control = UiFindChildByName (This, L"Cancel");
      UiSetAttribute (Control, L"text", mFbDialog->ButtonStringArray[Index++]);
    }

    if (mFbDialog->BodyInputCount != 0) {
      Control = UiFindChildByName (This, L"InputNumericText");
    } else {
      Control = UiFindChildByName (This, L"Ok");
    }
    SetFocus (Control->Wnd);

    mTitleVisible = TRUE;
    SetTimer (Wnd, 0, 1, DialogCallback);
    break;

  case UI_NOTIFY_CLICK:
    FocusedWnd = GetFocus ();
    Control  = (UI_CONTROL *) GetWindowLongPtr (FocusedWnd, 0);
    if (StrCmp (Control->Name, L"Cancel") == 0) {
      SendShutDNotify ();
    } else {
      SendNumericChange (This);
    }
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
    FocusedWnd = GetFocus ();
    Control    = (UI_CONTROL *) GetWindowLongPtr (FocusedWnd, 0);
    if (StrCmp (Control->Name, L"InputNumericText") == 0) {
      Control = UiFindChildByName (This, L"Ok");
      if (Control == NULL) {
        SendNumericChange (This);
      } else {
        SetFocus (Control->Wnd);
      }
    } else if (StrCmp (Control->Name, L"Ok") == 0) {
      SendNumericChange (This);
    } else if (StrCmp (Control->Name, L"Cancel") == 0) {
      SendShutDNotify ();
    }
    return 0;

  case WM_HOTKEY:
    if (HIWORD(LParam) == VK_ESCAPE) {
      SendShutDNotify ();
      return 0;
    }
    return 1;

  case WM_DESTROY:
    KillTimer (Wnd, 0);
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

  default:
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }
  return 1;
}

STATIC
LRESULT
H2ONumericDialogWithoutSendFormProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_NUMERIC_DIALOG            *This;
  UI_CONTROL                    *Control;
  CHAR16                        Str[20];
  HWND                          FocusedWnd;
  UI_CONTROL                    *ListControl;
  UINTN                         Size;

  Control = GetUiControl (Wnd);
  This    = (H2O_NUMERIC_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_NUMERIC_DIALOG *) AllocateZeroPool (sizeof (H2O_NUMERIC_DIALOG));
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
      XmlCreateControl (mNumericDialogWithoutSendFormChilds, Control);
    }
    break;

  case UI_NOTIFY_WINDOWINIT:
    if (mFbDialog->TitleString != NULL) {
      Control = UiFindChildByName (This, L"DialogTitle");
      UiSetAttribute (Control, L"text", mFbDialog->TitleString);
    }

    if (mFbDialog->BodyInputCount != 0) {
      Control = UiFindChildByName (This, L"InputNumericText");

      UnicodeSPrint (Str, sizeof (Str), L"0x%lx", mFbDialog->H2OStatement->Minimum);
      UiSetAttribute (Control, L"minvalue", Str);
      UnicodeSPrint (Str, sizeof (Str), L"0x%lx", mFbDialog->H2OStatement->Maximum);
      UiSetAttribute (Control, L"maxvalue", Str);
      Size = IfrNumericGetValueSize (mFbDialog->H2OStatement->Flags);
      UnicodeSPrint (Str, sizeof (Str), L"%d", Size);
      UiSetAttribute (Control, L"valuesize", Str);

      if ((mFbDialog->H2OStatement->Flags & EFI_IFR_DISPLAY) == EFI_IFR_DISPLAY_UINT_HEX) {
        UiSetAttribute (Control, L"valuetype", L"hex");
      } else {
        UiSetAttribute (Control, L"valuetype", ((mFbDialog->H2OStatement->Flags & EFI_IFR_DISPLAY) == EFI_IFR_DISPLAY_INT_DEC) ? L"signeddec" : L"dec");
        UiSetAttribute (Control, L"padding",   L"7,3,0,3");
        Control = UiFindChildByName (This, L"Prefix");
        if (Control != NULL) {
          UiSetAttribute (Control, L"text", L"");
        }
      }
    }

    if (mFbDialog->ButtonCount != 0) {
      ListControl = UiFindChildByName (This, L"DialogButtonList");
      UiSetAttribute (ListControl, L"visibility", L"true");

      Control = CreateControl (L"Control", ListControl);
      CONTROL_CLASS(ListControl)->AddChild (ListControl, Control);

      Control = CreateControl (L"Button", ListControl);
      UiApplyAttributeList (Control, L"name='Ok' text='Enter' height='30' width='75' font-size='19' textcolor='0xFFFFFFFF' text-align='center' text-align='singleline' background-color='0xFFCCCCCC' focusbkcolor='@menulightcolor'  tabstop='true' taborder='2'");
      if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
        UiApplyAttributeList (Control, L"background-color='0x0' focustextcolor='0xFF404040'");
      }
      CONTROL_CLASS(ListControl)->AddChild (ListControl, Control);

      Control = CreateControl (L"Button", ListControl);
      UiApplyAttributeList (Control, L"name='Cancel' text='Cancel' height='30' width='75' font-size='19' textcolor='0xFFFFFFFF' text-align='center' text-align='singleline' background-color='0xFFCCCCCC' focusbkcolor='@menulightcolor'  tabstop='true' taborder='3'");
      if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
        UiApplyAttributeList (Control, L"background-color='0x0' focustextcolor='0xFF404040'");
      }
      CONTROL_CLASS(ListControl)->AddChild (ListControl, Control);
    }

    if (mFbDialog->BodyInputCount != 0) {
      Control = UiFindChildByName (This, L"InputNumericText");
    } else {
      Control = UiFindChildByName (This, L"Ok");
    }
    SetFocus (Control->Wnd);
    break;

  case UI_NOTIFY_CLICK:
    FocusedWnd = GetFocus ();
    Control  = (UI_CONTROL *) GetWindowLongPtr (FocusedWnd, 0);
    if (StrCmp (Control->Name, L"Cancel") == 0) {
      SendShutDNotify ();
    } else {
      SendNumericChange (This);
    }
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
    FocusedWnd = GetFocus ();
    Control    = (UI_CONTROL *) GetWindowLongPtr (FocusedWnd, 0);
    if (StrCmp (Control->Name, L"InputNumericText") == 0) {
      Control = UiFindChildByName (This, L"Ok");
      if (Control == NULL) {
        SendNumericChange (This);
      } else {
        SetFocus (Control->Wnd);
      }
    } else if (StrCmp (Control->Name, L"Ok") == 0) {
      SendNumericChange (This);
    } else if (StrCmp (Control->Name, L"Cancel") == 0) {
      SendShutDNotify ();
    }
    return 0;

  default:
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }

  return 1;
}

H2O_NUMERIC_DIALOG_CLASS *
EFIAPI
GetH2ONumericDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2ONumericDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2ONumericDialogProc;

  return CURRENT_CLASS;
}

