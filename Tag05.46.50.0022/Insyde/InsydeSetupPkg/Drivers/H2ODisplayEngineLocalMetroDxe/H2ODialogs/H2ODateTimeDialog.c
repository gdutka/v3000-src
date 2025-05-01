/** @file
  Date and time dialog function implementation

;******************************************************************************
;* Copyright (c) 2014 - 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "MetroDialog.h"

EFI_TIME                        mEditTime;

STATIC H2O_DATE_TIME_DIALOG_CLASS    *mH2ODateTimeDialogClass = NULL;
#define CURRENT_CLASS                 mH2ODateTimeDialogClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mTimeDialogChilds = L""
  L"<VerticalLayout>"
    L"<TimeItem name='Time' height='156'/>"
    L"<Control height='12'/>"
    L"<HorizontalLayout child-padding='2' width='280' height='30'>"
      L"<Control/>"
      L"<Button name='ok' focusbkcolor='@menulightcolor' text='Enter' text-align='singleline|center' height='30' width='55' taborder='2' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF'/>"
      L"<Button name='cancel' focusbkcolor='@menulightcolor' text='Close' text-align='singleline|center' height='30' width='55' taborder='3' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF'/>"
    L"</HorizontalLayout>"
  L"</VerticalLayout>";

CHAR16 *mDateDialogChilds = L""
  L"<VerticalLayout>"
    L"<DateItem taborder='1' name='Date' height='156'/>"
    L"<Control height='12'/>"
    L"<HorizontalLayout child-padding='2' width='286' height='30'>"
      L"<Control/>"
      L"<Button name='ok' focusbkcolor='@menulightcolor' text='Enter' text-align='singleline|center' height='30' width='55' taborder='2' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF'/>"
      L"<Button name='cancel' focusbkcolor='@menulightcolor' text='Close' text-align='singleline|center' height='30' width='55' taborder='3' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF'/>"
    L"</HorizontalLayout>"
  L"</VerticalLayout>";
#endif

EFI_STATUS
SetHiiTimeToEfiTime (
  IN  EFI_HII_VALUE             *HiiValue,
  OUT EFI_TIME                  *EfiTime
  )
{
  if (HiiValue == NULL || EfiTime == NULL || (HiiValue->Type != EFI_IFR_TYPE_TIME && HiiValue->Type != EFI_IFR_TYPE_DATE)) {
    return EFI_INVALID_PARAMETER;
  }

  if (HiiValue->Type == EFI_IFR_TYPE_TIME) {
    EfiTime->Hour   = HiiValue->Value.time.Hour;
    EfiTime->Minute = HiiValue->Value.time.Minute;
    EfiTime->Second = HiiValue->Value.time.Second;
  } else {
    EfiTime->Year  = HiiValue->Value.date.Year;
    EfiTime->Month = HiiValue->Value.date.Month;
    EfiTime->Day   = HiiValue->Value.date.Day;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SetEfiTimeToHiiTime (
  IN     EFI_TIME               *EfiTime,
  IN OUT EFI_HII_VALUE          *HiiValue
  )
{
  if (HiiValue == NULL || EfiTime == NULL || (HiiValue->Type != EFI_IFR_TYPE_TIME && HiiValue->Type != EFI_IFR_TYPE_DATE)) {
    return EFI_INVALID_PARAMETER;
  }

  if (HiiValue->Type == EFI_IFR_TYPE_TIME) {
    HiiValue->Value.time.Hour   = EfiTime->Hour;
    HiiValue->Value.time.Minute = EfiTime->Minute;
    HiiValue->Value.time.Second = EfiTime->Second;
  } else {
    HiiValue->Value.date.Year  = EfiTime->Year;
    HiiValue->Value.date.Month = EfiTime->Month;
    HiiValue->Value.date.Day   = EfiTime->Day;
  }

  return EFI_SUCCESS;
}

STATIC
VOID
DateItemOnDateChange (
  H2O_DATE_ITEM                 *This,
  EFI_TIME                      *EfiTime
  )
{
  CopyMem (&mEditTime, EfiTime, sizeof (EFI_TIME));
}

STATIC
VOID
TimeItemOnTimeChange (
  H2O_TIME_ITEM                 *This,
  EFI_TIME                      *EfiTime
  )
{
  CopyMem (&mEditTime, EfiTime, sizeof (EFI_TIME));
}

STATIC
INTN
H2ODateTimeDialogProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_DATE_TIME_DIALOG          *This;
  H2O_FORM_BROWSER_Q            *CurrentQ;
  UI_CONTROL                    *Control;
  CHAR16                        Str[20];
  EFI_IMAGE_INPUT               *FormsetImage;

  Control = GetUiControl (Wnd);
  This    = (H2O_DATE_TIME_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_DATE_TIME_DIALOG *) AllocateZeroPool (sizeof (H2O_DATE_TIME_DIALOG));
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
    Control = UiFindChildByName (This, L"Content");
    if (gFB->CurrentQ->Operand == EFI_IFR_DATE_OP) {
      XmlCreateControl (mDateDialogChilds, Control);
    } else {
      XmlCreateControl (mTimeDialogChilds, Control);
    }

    CurrentQ = gFB->CurrentQ;
    SetHiiTimeToEfiTime (&CurrentQ->HiiValue, &mEditTime);

    if (CurrentQ->Prompt != NULL) {
      Control = UiFindChildByName (This, L"DialogTitle");
      UiSetAttribute (Control, L"text", CurrentQ->Prompt);
    }
    if (CurrentQ->Help != NULL) {
      Control = UiFindChildByName (This, L"DialogText");
      UiSetAttribute (Control, L"text", CurrentQ->Help);
    }

    FormsetImage = GetCurrentFormSetImage ();
    if (FormsetImage != NULL) {
      Control = UiFindChildByName (This, L"DialogImage");
      UnicodeSPrint (Str, sizeof (Str), L"0x%p", FormsetImage);
      UiSetAttribute (Control, L"background-image", Str);
      UiSetAttribute (Control, L"visibility", L"true");
    }

    if (CurrentQ->HiiValue.Type == EFI_IFR_TYPE_DATE) {
      Control = UiFindChildByName (This, L"Date");
      ((H2O_DATE_ITEM *)Control)->OnDateChange = DateItemOnDateChange;
    } else {
      Control = UiFindChildByName (This, L"Time");
      ((H2O_TIME_ITEM *)Control)->OnTimeChange = TimeItemOnTimeChange;
    }
    UnicodeSPrint (Str, sizeof (Str), L"0x%p", &mEditTime);
    UiSetAttribute (Control, L"efitime", Str);
    SetFocus (Control->Wnd);

    Control = UiFindChildByName (This, L"DialogTextScrollView");
    if (Control != NULL) {
      Control->OnSetState = H2OCommonDialogWithHelpOnSetState;
    }
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
  case UI_NOTIFY_CLICK:
    CurrentQ = gFB->CurrentQ;
    Control = (UI_CONTROL *) WParam;
    if (StrCmp (Control->Name, L"Time") == 0 ||
        StrCmp (Control->Name, L"Date") == 0) {
      Control = UiFindChildByName (This, L"ok");
      SetFocus (Control->Wnd);
    } else if (StrCmp (Control->Name, L"ok") == 0) {
      SetEfiTimeToHiiTime (&mEditTime, &CurrentQ->HiiValue);
      SendChangeQNotify (CurrentQ->PageId, CurrentQ->QuestionId, &CurrentQ->HiiValue);
    } else if (StrCmp (Control->Name, L"cancel") == 0) {
      SendShutDNotify ();
    }
    break;

  case WM_HOTKEY:
    if (HIWORD(LParam) == VK_ESCAPE) {
      SendShutDNotify ();
      return 1;
    }
    return 0;

  default:
    return (BOOLEAN) PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }

  return 1;
}

H2O_DATE_TIME_DIALOG_CLASS *
EFIAPI
GetH2ODateTimeDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2ODateTimeDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2ODateTimeDialogProc;

  return CURRENT_CLASS;
}

