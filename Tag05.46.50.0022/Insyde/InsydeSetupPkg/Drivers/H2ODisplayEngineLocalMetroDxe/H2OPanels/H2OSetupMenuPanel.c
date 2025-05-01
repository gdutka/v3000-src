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
#include "UiControls.h"
#include "H2OPanels.h"
#include "MetroUi.h"
#include "H2OControls.h"

STATIC H2O_SETUP_MENU_PANEL_CLASS        *mH2OSetupMenuPanelClass = NULL;
#define CURRENT_CLASS                    mH2OSetupMenuPanelClass

extern HWND                     gLastFocus;

VOID
SetupMenuItemSelected (
  UI_LIST_VIEW                  *This,
  UI_CONTROL                    *Child,
  UINT32                        Index
  )
{
  EFI_STATUS                    Status;
  H2O_FORM_BROWSER_SM           *SetupMenuData;
  SETUP_MENU_INFO               CurrentSetupMenuInfo;

  ASSERT (gFB->CurrentP != NULL);


  Status = gFB->GetSMInfo (gFB, &SetupMenuData);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return ;
  }

  ASSERT (Index < SetupMenuData->NumberOfSetupMenus);
  if (Index >= SetupMenuData->NumberOfSetupMenus) {
    goto Done;
  }

  Status = GetSetupMenuInfoByPage (gFB->CurrentP, &CurrentSetupMenuInfo);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  if (SetupMenuData->SetupMenuInfoList[Index].PageId == CurrentSetupMenuInfo.PageId) {
    goto Done;
  }

  SendSelectPNotify (SetupMenuData->SetupMenuInfoList[Index].PageId);

Done:
  FreeSetupMenuData (SetupMenuData);
}

VOID
SetupMenuItemClick (
  UI_LIST_VIEW                  *This,
  UI_CONTROL                    *Item,
  UINT32                        Index
  )
{
  EFI_STATUS                    Status;
  H2O_FORM_BROWSER_SM           *SetupMenuData;
  SETUP_MENU_INFO               CurrentSetupMenuInfo;

  ASSERT (gFB->CurrentP != NULL);


  Status = gFB->GetSMInfo (gFB, &SetupMenuData);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return ;
  }

  ASSERT (Index < SetupMenuData->NumberOfSetupMenus);
  if (Index >= SetupMenuData->NumberOfSetupMenus) {
    goto Done;
  }

  Status = GetSetupMenuInfoByPage (gFB->CurrentP, &CurrentSetupMenuInfo);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Done;
  }

  if (SetupMenuData->SetupMenuInfoList[Index].PageId == CurrentSetupMenuInfo.PageId) {
    goto Done;
  }

  SendSelectPNotify (SetupMenuData->SetupMenuInfoList[Index].PageId);

Done:
  FreeSetupMenuData (SetupMenuData);
}

EFI_STATUS
RefreshSetupMenuList (
  H2O_SETUP_MENU_PANEL          *This
  )
{
  UINTN                         Index;
  EFI_STATUS                    Status;
  H2O_FORM_BROWSER_SM           *SetupMenuData;
  SETUP_MENU_INFO               *SetupMenuInfo;
  SETUP_MENU_INFO               CurrentSetupMenuInfo;
  UI_CONTROL                    *MenuControl;
  UI_CONTROL                    *Child;
  CHAR16                        Str[20];
  H2O_PANEL_INFO                *PanelInfo;
  H2O_PROPERTY_INFO             **PropertyList;
  UINTN                         PropertyCount;


  if (!NeedShowSetupMenu ()) {
    UiSetAttribute (This, L"visibility", L"false");
    return EFI_SUCCESS;
  } else {
    UiSetAttribute (This, L"visibility", L"true");
  }

  //
  // remove old items
  //
  CONTROL_CLASS (This)->RemoveAllChild ((UI_CONTROL *)This);

  ((UI_LIST_VIEW *)This)->OnItemClick = SetupMenuItemClick;
  ((UI_LIST_VIEW *)This)->OnItemSelected = SetupMenuItemSelected;


  Status = gFB->GetSMInfo (gFB, &SetupMenuData);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (SetupMenuData->NumberOfSetupMenus == 0) {
    FreePool (SetupMenuData);
    return EFI_UNSUPPORTED;
  }

  PanelInfo = GetPanelInfo (mLayoutInfo, SETUP_MENU_PANEL_ID);
  Status = GetVfcfPanelPropertyList (
             PanelInfo,
             H2O_IFR_STYLE_TYPE_BUTTON,
             H2O_STYLE_PSEUDO_CLASS_NORMAL,
             &PropertyList,
             &PropertyCount
             );
  if (EFI_ERROR (Status)) {
    PropertyList  = NULL;
    PropertyCount = 0;
  }

  Status = GetSetupMenuInfoByPage (gFB->CurrentP, &CurrentSetupMenuInfo);
  if (EFI_ERROR (Status)) {
    ZeroMem (&CurrentSetupMenuInfo, sizeof (SETUP_MENU_INFO));
  }

  for (Index = 0; Index < SetupMenuData->NumberOfSetupMenus; Index++) {

    SetupMenuInfo = &SetupMenuData->SetupMenuInfoList[Index];

    MenuControl = CreateControl (L"SetupMenuItem", (UI_CONTROL*) This);
    UnicodeSPrint (Str, sizeof (Str), L"0x%p", SetupMenuInfo);
    UiSetAttribute (MenuControl, L"setupmenuinfo", Str);
    if (mSetupMenuItemChilds == NULL) {
      SetAttributeFromVfcf (MenuControl, PropertyList, PropertyCount, NULL);

      Child = CONTROL_CLASS(MenuControl)->FindChildByName (MenuControl, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Child != NULL) {
        UiApplyAttributeList (Child, L"width='0' height='0'");
        H2OSetupMenuItemUpdateVfcfOrder (Child, SetupMenuInfo);
      }
    }

    CONTROL_CLASS(This)->AddChild ((UI_CONTROL *)This, MenuControl);

    //
    // performItemClick to select
    //
    if (SetupMenuData->SetupMenuInfoList[Index].PageId == CurrentSetupMenuInfo.PageId) {
      LIST_VIEW_CLASS (This)->SetSelection (
                                (UI_LIST_VIEW *)This,
                                (INT32)Index,
                                TRUE
                                );
    }
  }

  FreeSetupMenuData (SetupMenuData);
  if (PropertyList != NULL) {
    FreePool (PropertyList);
  }
  return EFI_SUCCESS;
}

BOOLEAN
H2OSetupMenuPanelSetAttribute (
  UI_CONTROL                    *Control,
  CHAR16                        *Name,
  CHAR16                        *Value
  )
{
  UI_LIST_VIEW                  *This;

  This = (UI_LIST_VIEW *) Control;

  if (StrCmp (Name, L"layout") == 0) {
    if (StrCmp (Value, L"horizontal") == 0) {
      //
      // don't need keyboard focus
      //
      SetWindowLongPtr (
        Control->Wnd,
        GWL_EXSTYLE, GetWindowLongPtr (Control->Wnd, GWL_EXSTYLE) | WS_EX_NOACTIVATE
        );
    } else {
       SetWindowLongPtr (
        Control->Wnd,
        GWL_EXSTYLE, GetWindowLongPtr (Control->Wnd, GWL_EXSTYLE) & ~WS_EX_NOACTIVATE
        );
    }
  }

  return PARENT_CLASS_SET_ATTRIBUTE (CURRENT_CLASS, Control, Name, Value);
}


LRESULT
EFIAPI
H2OSetupMenuPanelProc (
  HWND   Wnd,
  UINT32 Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  H2O_SETUP_MENU_PANEL          *This;
  UI_CONTROL                    *Control;
  UI_LIST_VIEW                  *ListView;
  UI_CONTROL                    *SetupPagePanel;

  This = (H2O_SETUP_MENU_PANEL *) GetWindowLongPtr (Wnd, 0);
  if (This == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  Control = (UI_CONTROL *)This;
  ListView = (UI_LIST_VIEW *)This;

  switch (Msg) {

  case WM_CREATE:
    This = (H2O_SETUP_MENU_PANEL *) AllocateZeroPool (sizeof (H2O_SETUP_MENU_PANEL));
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
    RefreshSetupMenuList (This);
    break;

  case WM_SETFOCUS:
    if (ListView->Layout != UiListViewHorizontalLayout) {
      gLastFocus = Wnd;
    }
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
    break;

  case WM_KEYDOWN:
    if (ListView->Layout == UiListViewHorizontalLayout) {
      if (WParam == VK_LEFT || WParam == VK_RIGHT) {
        PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
      }

      if (WParam == VK_LEFT || WParam == VK_RIGHT || WParam == VK_DOWN || WParam == VK_UP) {
        SetupPagePanel = UiFindChildByName ((UI_CONTROL *)GetWindowLongPtr(gWnd, 0), L"H2OSetupPagePanel");
        gLastFocus = SetupPagePanel->Wnd;
        SetFocus (SetupPagePanel->Wnd);
        return 0;
      }

    }
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);


  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);

  }
  return 0;

}


H2O_SETUP_MENU_PANEL_CLASS *
EFIAPI
GetH2OSetupMenuPanelClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"H2OSetupMenuPanel", (UI_CONTROL_CLASS *) GetListViewClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = H2OSetupMenuPanelProc;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetAttribute = H2OSetupMenuPanelSetAttribute;

  return CURRENT_CLASS;
}

