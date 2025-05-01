/** @file
  UI Common Controls

;******************************************************************************
;* Copyright (c) 2012 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "H2ODisplayEngineLocalMetro.h"
#include "H2OPanels.h"
#include "MetroUi.h"

STATIC H2O_HELP_TEXT_PANEL_CLASS         *mH2OHelpTextPanelClass = NULL;
#define CURRENT_CLASS                    mH2OHelpTextPanelClass

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
//CHAR16 *mHelpTextPanelChilds = L""
//  L"<VerticalLayout width='match_parent' float='true' height='match_parent'>"
//    L"<Control />"
//    L"<Texture width='300' height='300' name='HelpBkgImage' background-image-style='stretch|gray'/>"
//  L"</VerticalLayout>"
//  L"<VerticalLayout padding='40,30,40,30' width='-1' float='true' name='HelpMenu' height='-1'>"
//    L"<HorizontalLayout padding='0,0,8,0' min-height='50' height='wrap_content'>"
//      L"<Label width='200' textcolor='0xFF268FB0' font-size='29' name='HelpTitle' height='wrap_content'/>"
//      L"<Control padding='0,25,0,25' height='50'>"
//        L"<Texture width='50' height='50' name='HelpImage' background-image-style='stretch'/>"
//      L"</Control>"
//    L"</HorizontalLayout>"
//    L"<ScrollView taborder='20' name='HelpTextScrollView' vscrollbar='true' scale9grid='1,13,1,13'>"
//      L"<Label width='match_parent' height='wrap_content' textcolor='0xFF666666' font-size='19' name='HelpText'/>"
//    L"</ScrollView>"
//  L"</VerticalLayout>";
CHAR16 *mHelpTextPanelChilds = NULL;
#endif

STATIC
VOID
H2OHelpTextPanelUpdateVfcfOrder (
  IN UI_CONTROL                        *Order,
  IN H2O_FORM_BROWSER_Q                *CurrentQ

  )
{
  UINTN                                Index;
  UI_CONTROL                           *Child;
  CHAR16                               Str[20];
  EFI_IMAGE_INPUT                      *PageImage;
  UINTN                                NameLen;

  if (Order == NULL) {
    return;
  }

  for (Index = 0; Index < Order->ItemCount; Index++) {
    Child = Order->Items[Index];
    if (Child->Name == NULL) {
      continue;
    }

    NameLen = StrLen(Child->Name);

    if (STR_MATCH(Child->Name, NameLen, L"prompt")) {
      UiSetAttribute (Child, L"text", (CurrentQ != NULL && CurrentQ->Prompt != NULL) ? CurrentQ->Prompt : L" ");
    } else if (STR_MATCH(Child->Name, NameLen, L"help")) {
      UiSetAttribute (Child, L"text", (CurrentQ != NULL && CurrentQ->Help != NULL) ? CurrentQ->Help : L" ");
    } else if (STR_MATCH(Child->Name, NameLen, L"formset-image") ||
               STR_MATCH(Child->Name, NameLen, L"formset-help-image")) {
      PageImage = GetCurrentFormSetImage ();
      if (PageImage == NULL) {
        UiSetAttribute (Child, L"visibility", L"false");
        continue;
      }

      UnicodeSPrint (Str, sizeof (Str), L"0x%p", PageImage);
      UiSetAttribute (Child, L"background-image", Str);
    } else if (STR_MATCH(Child->Name, NameLen, L"container")) {
      Child = CONTROL_CLASS(Child)->FindChildByName (Child, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Child == NULL) {
        continue;
      }

      H2OHelpTextPanelUpdateVfcfOrder (Child, CurrentQ);
    }
  }
}

EFI_STATUS
RefreshHelpMenu (
  H2O_HELP_TEXT_PANEL          *This
  )
{
  UI_CONTROL                    *Control;
  UI_MANAGER                    *Manager;
  H2O_FORM_BROWSER_Q            *CurrentQ;
  CHAR16                        Str[20];
  EFI_IMAGE_INPUT               *PageImage;

  Control   = (UI_CONTROL *) This;
  Manager   = Control->Manager;
  CurrentQ  = gFB->CurrentQ;

  if (CurrentQ != NULL && CurrentQ->Prompt != NULL) {
    Control = UiFindChildByName (This, L"HelpTitle");
    UiSetAttribute (Control, L"text", CurrentQ->Prompt);
  } else {
    Control = UiFindChildByName (This, L"HelpTitle");
    UiSetAttribute (Control, L"text", L" ");
  }

  PageImage = GetCurrentFormSetImage ();
  if (PageImage != NULL) {
    Control = UiFindChildByName (This, L"HelpImage");
    UnicodeSPrint (Str, sizeof (Str), L"0x%p", PageImage);
    UiSetAttribute (Control, L"background-image", Str);
    if (PcdGet32(PcdH2OLmdeMultiLayout) == 1) {
      AdjustImageToMenuColor (Control);
    }
    UiSetAttribute (Control, L"visibility", L"true");
    Control = UiFindChildByName (This, L"HelpBkgImage");
    UiSetAttribute (Control, L"background-image", Str);
    UiSetAttribute (Control, L"visibility", L"true");
  } else {
    Control = UiFindChildByName (This, L"HelpImage");
    UiSetAttribute (Control, L"visibility", L"false");
    Control = UiFindChildByName (This, L"HelpBkgImage");
    UiSetAttribute (Control, L"visibility", L"false");
  }
  if (CurrentQ != NULL && CurrentQ->Help != NULL) {
    Control = UiFindChildByName (This, L"HelpText");
    UiSetAttribute (Control, L"text", CurrentQ->Help);
  } else {
    Control = UiFindChildByName (This, L"HelpText");
    UiSetAttribute (Control, L"text", L" ");
  }
  return EFI_SUCCESS;
}



LRESULT
EFIAPI
H2OHelpTextPanelProc (
  HWND   Hwnd,
  UINT32 Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  H2O_HELP_TEXT_PANEL           *This;
  UI_CONTROL                    *Control;
  UI_CONTROL                    *Child;

  This = (H2O_HELP_TEXT_PANEL *) GetWindowLongPtr (Hwnd, 0);
  Control = (UI_CONTROL *)This;
  if (This == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_HELP_TEXT_PANEL *) AllocateZeroPool (sizeof (H2O_HELP_TEXT_PANEL));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Hwnd, 0);
      SetWindowLongPtr (Hwnd, 0, (INTN)This);
      SendMessage (Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, UI_NOTIFY_CREATE, WParam, LParam);
    if (mHelpTextPanelChilds != NULL) {
      XmlCreateControl (mHelpTextPanelChilds, Control);
    }
    break;

  case FB_NOTIFY_SELECT_Q:
  case FB_NOTIFY_REPAINT:
    if (mHelpTextPanelChilds != NULL) {
      RefreshHelpMenu (This);
    } else {
      Child = CONTROL_CLASS(Control)->FindChildByName (Control, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Child != NULL) {
        H2OHelpTextPanelUpdateVfcfOrder (Child, gFB->CurrentQ);
      }
    }
    break;

  case WM_NCHITTEST:
    return HTCLIENT;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Hwnd, Msg, WParam, LParam);

  }
  return 0;

}


H2O_HELP_TEXT_PANEL_CLASS *
EFIAPI
GetH2OHelpTextPanelClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OHelpTextPanel", (UI_CONTROL_CLASS *)GetControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }

  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OHelpTextPanelProc;

  return CURRENT_CLASS;
}

