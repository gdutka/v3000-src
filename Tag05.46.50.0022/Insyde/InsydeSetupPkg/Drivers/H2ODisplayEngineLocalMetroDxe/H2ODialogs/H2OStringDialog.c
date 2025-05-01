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

STATIC H2O_STRING_DIALOG_CLASS    *mH2OStringDialogClass = NULL;
#define CURRENT_CLASS              mH2OStringDialogClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mStringDialogChilds = L""
  L"<VerticalLayout>"
    L"<HorizontalLayout width='300' background-color='0xFFF2F2F2' height='wrap_content' >"
      L"<Label name='StringLabel' text='input string:' height='29' padding='12,0,0,0' width='119' font-size='16' textcolor='0xFF4D4D4D'/>"
      L"<VerticalLayout name='InputStringArea' padding='6,8,6,0' width='173' height='wrap_content' >"
        L"<Control/>"
        L"<Control name='InputStringBox' padding='1,1,1,1' width='171' background-color='0xFF999999' height='wrap_content'>"
          L"<UiEdit name='InputStringText' focusbkcolor='@menulightcolor' tabstop='true' font-size='19' height='wrap_content' width='171' taborder='1' background-color='0xFFF2F2F2'/>"
        L"</Control>"
        L"<Control/>"
      L"</VerticalLayout>"
    L"</HorizontalLayout>"
    L"<Control height='2'/>"
    L"<Control height='10'/>"
    L"<HorizontalLayout padding='0,0,30,0' child-padding='2' width='300' height='25'>"
      L"<Control/>"
      L"<Button name='Ok' focusbkcolor='@menulightcolor' text='Enter' text-align='singleline|center' height='30' width='55' taborder='2' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF' tabstop='true'/>"
      L"<Button name='Cancel' focusbkcolor='@menulightcolor' text='Cancel' text-align='singleline|center' height='30' width='55' taborder='3' font-size='19' background-color='0xFFCCCCCC' textcolor='0xFFFFFFFF' tabstop='true'/>"
    L"</HorizontalLayout>"
  L"</VerticalLayout>";
#endif

STATIC
EFI_STATUS
SendStringChange (
  IN H2O_STRING_DIALOG          *This
  )
{
  UI_CONTROL                    *Control;
  CHAR16                        *String;
  UINTN                         StringSize;
  EFI_HII_VALUE                 HiiValue;

  Control    = UiFindChildByName (This, L"InputStringText");
  String     = ((UI_LABEL *) Control)->Text;
  StringSize = StrSize (String);

  ZeroMem (&HiiValue, sizeof (HiiValue));
  HiiValue.Type      = EFI_IFR_TYPE_STRING;
  HiiValue.BufferLen = (UINT16) StringSize;
  HiiValue.Buffer    = (UINT8 *) AllocateCopyPool (StringSize, String);
  SendChangeQNotify (0, 0, &HiiValue);

  return EFI_SUCCESS;
}

STATIC
LRESULT
H2OStringDialogProc (
  IN HWND                       Wnd,
  IN UINT                       Msg,
  IN WPARAM                     WParam,
  IN LPARAM                     LParam
  )
{
  H2O_STRING_DIALOG             *This;
  UI_CONTROL                    *Control;
  EFI_IMAGE_INPUT               *FormsetImage;
  CHAR16                        Str[20];
  UINTN                         Index;
  HWND                          FocusedWnd;
  H2O_FORM_BROWSER_Q            *CurrentQ;
  SIZE                          ContentSize;
  CHAR16                        *MaxStr;

  CurrentQ = mFbDialog->H2OStatement;
  Control  = GetUiControl (Wnd);
  This     = (H2O_STRING_DIALOG *)Control;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_STRING_DIALOG *) AllocateZeroPool (sizeof (H2O_STRING_DIALOG));
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
    XmlCreateControl (mStringDialogChilds, Control);

    if (mFbDialog->BodyInputCount != 0) {
      Control = UiFindChildByName (This, L"InputStringText");

      //
      // calculate content height for estimate max string length correctly.
      //
      ContentSize.cy = 0;
      if (CurrentQ->HiiValue.BufferLen > sizeof (CHAR16)) {
        MaxStr = AllocateZeroPool (CurrentQ->HiiValue.BufferLen);
        if (MaxStr != NULL) {
          for (Index = 0; Index < CurrentQ->HiiValue.BufferLen / sizeof (CHAR16) - 1; Index++) {
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
      UiSetAttribute (Control, L"height", Str);

      UiSetAttribute (Control, L"text", (CHAR16 *)CurrentQ->HiiValue.Buffer);
      UnicodeSPrint (Str, sizeof (Str), L"%d", CurrentQ->Maximum);
      UiSetAttribute (Control, L"maxlength", Str);

      Control = UiFindChildByName (This, L"InputStringBox");
      UnicodeSPrint (Str, sizeof (Str), L"%d", ContentSize.cy);
      UiSetAttribute (Control, L"height", Str);

      Control = UiFindChildByName (This, L"InputStringArea");
      UnicodeSPrint (Str, sizeof (Str), L"%d", ContentSize.cy);
      UiSetAttribute (Control, L"height", Str);
    }

    if (mFbDialog->ButtonCount == 2) {
      Index = 0;
      Control = UiFindChildByName (This, L"Ok");
      UiSetAttribute (Control, L"text", mFbDialog->ButtonStringArray[Index++]);
      Control = UiFindChildByName (This, L"Cancel");
      UiSetAttribute (Control, L"text", mFbDialog->ButtonStringArray[Index++]);
    }

    if (mFbDialog->BodyInputCount != 0) {
      Control = UiFindChildByName (This, L"InputStringText");
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
      SendStringChange (This);
    }
    break;

  case UI_NOTIFY_CARRIAGE_RETURN:
    FocusedWnd = GetFocus ();
    Control    = (UI_CONTROL *) GetWindowLongPtr (FocusedWnd, 0);
    if (StrCmp (Control->Name, L"InputStringText") == 0) {
      Control = UiFindChildByName (This, L"Ok");
      if (Control == NULL) {
        SendStringChange (This);
      } else {
        SetFocus (Control->Wnd);
      }
    } else if (StrCmp (Control->Name, L"Ok") == 0) {
      SendStringChange (This);
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

H2O_STRING_DIALOG_CLASS *
EFIAPI
GetH2OStringDialogClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OStringDialog", (UI_CONTROL_CLASS *)GetDialogClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OStringDialogProc;

  return CURRENT_CLASS;
}

