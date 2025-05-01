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
#include "H2OControls.h"

STATIC H2O_HOTKEY_PANEL_CLASS            *mH2OHotkeyPanelClass = NULL;
#define CURRENT_CLASS                    mH2OHotkeyPanelClass

EFI_STATUS
RefreshHotkeyList (
  H2O_HOTKEY_PANEL              *This
  )
{
  UINTN                         Index;
  HOT_KEY_INFO                  *HotkeyInfo;
  HOT_KEY_INFO                  *CurrentHotkey;
  UI_CONTROL                    *HotkeyControl;
  UI_CONTROL                    *Control;
  UI_CONTROL                    *Child;
  CHAR16                        Str[20];
  UINTN                         GroupIdIndex;
  UINT8                         GroupId[HOT_KEY_INFO_GROUP_ID_MAX];
  BOOLEAN                       NewGroup;
  BOOLEAN                       OldGroup;
  H2O_PANEL_INFO                *PanelInfo;
  H2O_PROPERTY_INFO             **PropertyList;
  UINTN                         PropertyCount;
  EFI_STATUS                    Status;

  Control = (UI_CONTROL *)This;

  Child = CONTROL_CLASS(Control)->FindChildByName (Control, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
  if (Child != NULL) {
    Control = Child;
  }

  //
  // remove old items
  //
  CONTROL_CLASS (Control)->RemoveAllChild ((UI_CONTROL *)Control);


  //
  // Create hotkey items
  //
  ASSERT (gFB->CurrentP != NULL);
  ASSERT (gFB->CurrentP->HotKeyInfo != NULL);
  HotkeyInfo = gFB->CurrentP->HotKeyInfo;

  for (GroupIdIndex = 0; GroupIdIndex < HOT_KEY_INFO_GROUP_ID_MAX; GroupIdIndex ++) {
    GroupId[GroupIdIndex] = HOT_KEY_INFO_GROUP_ID_NONE;
  }

  PanelInfo = GetPanelInfo (mLayoutInfo, HOTKEY_PANEL_ID);
  Status = GetVfcfPanelPropertyList (
             PanelInfo,
             H2O_IFR_STYLE_TYPE_GROUP,
             H2O_STYLE_PSEUDO_CLASS_NORMAL,
             &PropertyList,
             &PropertyCount
             );
  if (EFI_ERROR (Status)) {
    PropertyList  = NULL;
    PropertyCount = 0;
  }

  for (Index = 0; !IS_END_OF_HOT_KEY_INFO(&HotkeyInfo[Index]); Index++) {
    CurrentHotkey = &HotkeyInfo[Index];
    if (!CurrentHotkey->Display) {
      continue;
    }

    NewGroup = FALSE;
    OldGroup = FALSE;
    if (CurrentHotkey->GroupId != HOT_KEY_INFO_GROUP_ID_NONE) {
      for (GroupIdIndex = 0; GroupIdIndex < HOT_KEY_INFO_GROUP_ID_MAX; GroupIdIndex ++) {
        if (GroupId[GroupIdIndex] == HOT_KEY_INFO_GROUP_ID_NONE) {
          //
          // New GroupId
          //
          NewGroup = TRUE;
          GroupId[GroupIdIndex] = CurrentHotkey->GroupId;
          break;
        } else if (GroupId[GroupIdIndex] == CurrentHotkey->GroupId) {
          //
          // Same GroupId
          //
          OldGroup = TRUE;
          UnicodeSPrint (Str, sizeof (Str), L"Hotkey_Group%02x", CurrentHotkey->GroupId);
          HotkeyControl = UiFindChildByName (Control, Str);
          UnicodeSPrint (Str, sizeof (Str), L"0x%p", CurrentHotkey);
          UiSetAttribute (HotkeyControl, L"hotkey", Str);
          if (mHotkeyItemChilds == NULL) {
            Child = CONTROL_CLASS(HotkeyControl)->FindChildByName (HotkeyControl, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
            if (Child != NULL) {
              H2OHotkeyItemUpdateVfcfOrder (Child, CurrentHotkey);
            }
          }
          break;
        }
      }
    }

    if (OldGroup) {
      continue;
    }

    //
    // Create new HotkeyItem
    //
    HotkeyControl = CreateControl (L"HotkeyItem", Control);
    UnicodeSPrint (Str, sizeof (Str), L"0x%p", CurrentHotkey);
    UiSetAttribute (HotkeyControl, L"hotkey", Str);
    if (mHotkeyItemChilds == NULL) {
      UiApplyAttributeList (HotkeyControl, L"width='wrap_content' height='wrap_content'");
      SetAttributeFromVfcf (HotkeyControl, PropertyList, PropertyCount, NULL);

      Child = CONTROL_CLASS(HotkeyControl)->FindChildByName (HotkeyControl, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Child != NULL) {
        H2OHotkeyItemUpdateVfcfOrder (Child, CurrentHotkey);
      }
    }

    if (NewGroup) {
      UnicodeSPrint (Str, sizeof (Str), L"Hotkey_Group%02x", CurrentHotkey->GroupId);
      UiSetAttribute (HotkeyControl, L"name", Str);
    } else {
      UiSetAttribute (HotkeyControl, L"name", L"Hotkey");
    }

    CONTROL_CLASS(Control)->AddChild (Control, HotkeyControl);
  }

  if (PropertyList != NULL) {
    FreePool (PropertyList);
  }
  return EFI_SUCCESS;
}

LRESULT
EFIAPI
H2OHotkeyPanelProc (
  HWND   Wnd,
  UINT32 Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  H2O_HOTKEY_PANEL              *This;
  UI_CONTROL                    *Control;


  This = (H2O_HOTKEY_PANEL *) GetWindowLongPtr (Wnd, 0);
  if (This == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  Control = (UI_CONTROL *)This;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_HOTKEY_PANEL *) AllocateZeroPool (sizeof (H2O_HOTKEY_PANEL));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Wnd, 0);
      SetWindowLongPtr (Wnd, 0, (INTN)This);
      SendMessage (Wnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, UI_NOTIFY_CREATE, WParam, LParam);
    break;

  case FB_NOTIFY_REPAINT:
    RefreshHotkeyList (This);
    break;


  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

  }
  return 0;

}

H2O_HOTKEY_PANEL_CLASS *
EFIAPI
GetH2OHotkeyPanelClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OHotkeyPanel", (UI_CONTROL_CLASS *)GetHorizontalLayoutClass ());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OHotkeyPanelProc;

  return CURRENT_CLASS;
}

