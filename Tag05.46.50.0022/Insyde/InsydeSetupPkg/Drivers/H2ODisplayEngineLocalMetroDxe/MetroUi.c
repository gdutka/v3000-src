/** @file
  Entry point and initial functions for H2O local Metro display engine driver

;******************************************************************************
;* Copyright (c) 2013 - 2021, Insyde Software Corp. All Rights Reserved.
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
#include "H2OControls.h"
#include "H2OPanels.h"
#include "H2ODialogs.h"
#include "StatementControls.h"
#include "MetroUi.h"
#include <Guid/ZeroGuid.h>

extern H2O_DISPLAY_ENGINE_METRO_PRIVATE_DATA      *mMetroPrivate;
extern HWND                                       gLastFocus;
H2O_LAYOUT_INFO                                   *mLayoutInfo = NULL;
EFI_INPUT_KEY                                     mPrivateHotKeyList[] = {{SCAN_NULL , CHAR_CARRIAGE_RETURN},
                                                                          {SCAN_UP   , CHAR_NULL},
                                                                          {SCAN_DOWN , CHAR_NULL},
                                                                          {SCAN_RIGHT, CHAR_NULL},
                                                                          {SCAN_LEFT , CHAR_NULL},
                                                                          {SCAN_ESC  , CHAR_NULL},
                                                                          };

typedef struct _DISPLAY_ENGINE_FRAME DISPLAY_ENGINE_FRAME;

struct _DISPLAY_ENGINE_FRAME {
  UI_FRAME                      Frame;

  UI_CONTROL                    *H2OSetupMenuPanel;
  UI_CONTROL                    *H2OSetupPagePanel;
  UI_CONTROL                    *H2OFormTitlePanel;
  UI_CONTROL                    *H2OHelpTextPanel;
  UI_CONTROL                    *H2OHotkeyPanel;
  UI_CONTROL                    *H2OOwnerDrawPanel;
  UI_CONTROL                    **VfcfPanelControlList;
  UINTN                         VfcfPanelControlCount;
};

typedef struct _PANEL_VFCF_DATA {
  CHAR16                        *PanelName;
  UINT32                        PanelId;
} PANEL_VFCF_DATA;

PANEL_VFCF_DATA                 mPanelVfcfData[] = {
                                  {L"H2OOwnerDrawPanel", OWNER_DRAW_PANEL_ID},
                                  {L"H2OSetupMenuPanel", SETUP_MENU_PANEL_ID},
                                  {L"H2OFormTitlePanel", TITLE_PANEL_ID},
                                  {L"H2OSetupPagePanel", SETUP_PAGE_PANEL_ID},
                                  {L"H2OHelpTextPanel" , HELP_TEXT_PANEL_ID},
                                  {L"H2OHotkeyPanel"   , HOTKEY_PANEL_ID},
                                  {NULL, 0},
                                  };

typedef struct _DISPLAY_ENGINE_FRAME_CLASS {
  UI_FRAME_CLASS                ParentClass;
} DISPLAY_ENGINE_FRAME_CLASS;

STATIC DISPLAY_ENGINE_FRAME_CLASS    *mDisplayEngineFrameClass = NULL;
#define CURRENT_CLASS                mDisplayEngineFrameClass

DISPLAY_ENGINE_FRAME_CLASS *
EFIAPI
GetDisplayEngineFrameClass (
  VOID
  );

#if FixedPcdGet32(PcdH2OLmdeMultiLayout) == 0
CHAR16 *mDialogPanelChilds = L""
  L"<VerticalLayout name='DialogPanel' background-color='0xFF000000'/>";

CHAR16 *mScreenPanelChilds = L""
  L"<Control name='background' background-color='0xFF000000'>"
    L"<Texture float='true' name='ContextBkg'     background-color='0xFFE8E8E8'/>"
    L"<H2OOwnerDrawPanel name='H2OOwnerDrawPanel' background-color='0xFF000000' RefreshInterval='100'/>"
    L"<H2OSetupMenuPanel name='H2OSetupMenuPanel' background-color='0xFF000000'/>"
    L"<H2OSetupPagePanel name='H2OSetupPagePanel' background-color='0x0'/>"
    L"<H2OFormTitlePanel name='H2OFormTitlePanel' background-color='0x0'/>"
    L"<H2OHelpTextPanel  name='H2OHelpTextPanel'  background-color='0x0'/>"
    L"<H2OHotkeyPanel    name='H2OHotkeyPanel'    />"
    //L"<Texture background-image='@FormHalo' float='true' name='FormHalo' scale9grid='23,26,22,31'/>"
    L"<Texture name='overlay' float='true' visibility='false' height='-1' width='-1' background-color='0x80C8C8C8'/>"
  L"</Control>";
#endif

EFI_STATUS
GetInformationField (
  OUT RECT                                    *InfoField
  )
{
  EFI_STATUS                                  Status;
  RECT                                        SetupPagePanelField;
  RECT                                        HelpTextPanelField;

  Status = GetRectByName (gWnd, L"H2OSetupPagePanel", &SetupPagePanelField);
  if (EFI_ERROR (Status)) {
    SetRectEmpty (&SetupPagePanelField);
  }

  Status = GetRectByName (gWnd, L"H2OHelpTextPanel", &HelpTextPanelField);
  if (EFI_ERROR (Status)) {
    SetRectEmpty (&HelpTextPanelField);
  }

  if ((SetupPagePanelField.left == SetupPagePanelField.right ||
    SetupPagePanelField.top == SetupPagePanelField.bottom) &&
    (HelpTextPanelField.left == HelpTextPanelField.right ||
    HelpTextPanelField.top == HelpTextPanelField.bottom)){
    //
    // If two panels' fields are both empty, return screen field
    //
    InfoField->left   = 0;
    InfoField->top    = 0;
    InfoField->right  = (INT32) GetSystemMetrics (SM_CXSCREEN);
    InfoField->bottom = (INT32) GetSystemMetrics (SM_CYSCREEN);
    return EFI_SUCCESS;
  }

  if (SetupPagePanelField.top    == HelpTextPanelField.top &&
      SetupPagePanelField.bottom == HelpTextPanelField.bottom) {
    if (SetupPagePanelField.right == HelpTextPanelField.left ||
        SetupPagePanelField.left  == HelpTextPanelField.right) {
      UnionRect (InfoField, &SetupPagePanelField, &HelpTextPanelField);
      return EFI_SUCCESS;
    }
  } else if (SetupPagePanelField.left  == HelpTextPanelField.left &&
             SetupPagePanelField.right == HelpTextPanelField.right) {
    if (SetupPagePanelField.bottom == HelpTextPanelField.top ||
        SetupPagePanelField.top    == HelpTextPanelField.bottom) {
      UnionRect (InfoField, &SetupPagePanelField, &HelpTextPanelField);
      return EFI_SUCCESS;
    }
  }

  CopyRect (InfoField, &SetupPagePanelField);
  return EFI_SUCCESS;
}

EFI_STATUS
DisableHalo (
  IN DISPLAY_ENGINE_FRAME                    *This
  )
{
  UI_MANAGER                               *Manager;
  UI_CONTROL                               *Control;

  Manager = ((UI_CONTROL *) This)->Manager;
  Control = Manager->FindControlByName (Manager, L"FormHalo");
  if (Control == NULL) {
    return EFI_NOT_FOUND;
  }
  UiSetAttribute (Control, L"visibility", L"false");

  return EFI_SUCCESS;
}

UI_CONTROL *
CreateSubElementControl (
  IN UI_CONTROL                 *Parent,
  IN CHAR16                     *SubElementName
  )
{
  UINTN                         NameLen;

  if (SubElementName == NULL) {
    return NULL;
  }

  NameLen = StrLen(SubElementName);

  if (STR_MATCH(SubElementName, NameLen, L"form-title")    ||
      STR_MATCH(SubElementName, NameLen, L"formset-help")  ||
      STR_MATCH(SubElementName, NameLen, L"formset-title") ||
      STR_MATCH(SubElementName, NameLen, L"help")          ||
      STR_MATCH(SubElementName, NameLen, L"prompt")        ||
      STR_MATCH(SubElementName, NameLen, L"text2")         ||
      STR_MATCH(SubElementName, NameLen, L"value")         ||
      PREFIX_STR_MATCH(SubElementName, NameLen, L"str")) {
    return CreateAddControl (L"Label", Parent);
  }

  return CreateAddControl (L"Control", Parent);
}

VOID
SetSubElementSizeToWarpContent (
  IN UI_CONTROL                 *Control
  )
{
  UI_CONTROL                    *Order;
  UINTN                         Index;
  UI_CONTROL                    *Child;


  Order = CONTROL_CLASS(Control)->FindChildByName (Control, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
  if (Order == NULL) {
    return;
  }

  for (Index = 0; Index < Order->ItemCount; Index++) {
    if (!IS_CONTROL_WIDTH_DEFINED(Order->Items[Index])) {
      UiSetAttribute (Order->Items[Index], L"width", L"wrap_content");
    }
    if (!IS_CONTROL_HEIGHT_DEFINED(Order->Items[Index])) {
      UiSetAttribute (Order->Items[Index], L"height", L"wrap_content");
    }

    if (StrCmp (Order->Items[Index]->Name, L"container") == 0) {
      Child = CONTROL_CLASS(Order->Items[Index])->FindChildByName (Order->Items[Index], VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Child == NULL) {
        continue;
      }

      SetSubElementSizeToWarpContent (Child);
    }
  }
}

UI_CONTROL *
GetVfcfTargetControl (
  IN     UI_CONTROL             **OrderList,
  IN     UINTN                  OrderCount,
  IN OUT CHAR16                 **PropertyName
  )
{
  UINTN                         OrderIndex;
  UINTN                         ItemIndex;
  UI_CONTROL                    *Order;
  UI_CONTROL                    *Control;
  UINTN                         Length;
  CHAR16                        *PropertyNameStrPtr;

  if (OrderList == NULL || OrderCount == 0) {
    return NULL;
  }

  PropertyNameStrPtr = *PropertyName;

  for (OrderIndex = 0; OrderIndex < OrderCount; OrderIndex++) {
    Order = OrderList[OrderIndex];

    for (ItemIndex = 0; ItemIndex < Order->ItemCount; ItemIndex++) {
      Control = Order->Items[ItemIndex];
      if (Control->Name == NULL) {
        continue;
      }

      Length = StrLen (Control->Name);
      if (StrnCmp (Control->Name, PropertyNameStrPtr, Length) == 0 && PropertyNameStrPtr[Length] == '-') {
        *PropertyName = &PropertyNameStrPtr[Length + 1];
        return Control;
      }
    }
  }

  return NULL;
}

VOID
SetAttributeFromVfcf (
  IN UI_CONTROL                 *Control,
  IN H2O_PROPERTY_INFO          **PropertyList,
  IN UINTN                      PropertyCount,
  IN RECT                       *ScreenField
  )
{
  UINTN                         PropertyIndex;
  H2O_PROPERTY_INFO             *Property;
  H2O_PROPERTY_VALUE            Value;
  UINTN                         PropertyNameLen;
  CHAR16                        *PropertyName;
  CHAR16                        *AttrName;
  CHAR16                        NameStrBuf[50];
  CHAR16                        ValueStrBuf[50];
  EFI_STATUS                    Status;
  UI_CONTROL                    *Target;
  UI_CONTROL                    *Child;
  UI_CONTROL                    *Order;
  UI_CONTROL                    **OrderList;
  UINTN                         OrderCount;
  EFI_IMAGE_INPUT               *Image;
  VOID                          *Buffer;
  UINTN                         Index;
  UINTN                         StringLength;

  OrderList  = NULL;
  OrderCount = 0;

  for (PropertyIndex = 0; PropertyIndex < PropertyCount; PropertyIndex++) {
    Property = PropertyList[PropertyIndex];

    Status = ParseProperty (ScreenField, Property, &Value);
    if (EFI_ERROR (Status)) {
      continue;
    }

    UnicodeSPrint (NameStrBuf, sizeof(NameStrBuf), L"%a", Property->IdentifierStr);
    PropertyName = NameStrBuf;

    Target = GetVfcfTargetControl (OrderList, OrderCount, &PropertyName);
    if (Target == NULL) {
      Target = Control;
    }
    PropertyNameLen = StrLen(PropertyName);

    if (STR_MATCH(PropertyName, PropertyNameLen, L"padding")      ||
        STR_MATCH(PropertyName, PropertyNameLen, L"border-width") ||
        STR_MATCH(PropertyName, PropertyNameLen, L"margin")) {
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%a,%a,%a,%a", Property->ValueStr, Property->ValueStr, Property->ValueStr, Property->ValueStr);
      AttrName = PropertyName;
    } else if (PREFIX_STR_MATCH(PropertyName, PropertyNameLen, L"padding-") ||
               PREFIX_STR_MATCH(PropertyName, PropertyNameLen, L"margin-")) {
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%a", Property->ValueStr);
      AttrName = PropertyName;
    } else if (PREFIX_STR_MATCH(PropertyName, PropertyNameLen, L"border-")) {
      if (StrCmp (&PropertyName[7], L"top")    == 0 ||
          StrCmp (&PropertyName[7], L"bottom") == 0 ||
          StrCmp (&PropertyName[7], L"left")   == 0 ||
          StrCmp (&PropertyName[7], L"right")  == 0) {
        UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%a", Property->ValueStr);
        AttrName = PropertyName;
      } else {
        continue;
      }
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"height") ||
               STR_MATCH(PropertyName, PropertyNameLen, L"width")) {
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%a", Value.Value.Length.Type == H2O_CSS_LENGTH_TYPE_AUTO ? "wrap_content" : Property->ValueStr);
      AttrName = PropertyName;
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"position")         ||
               STR_MATCH(PropertyName, PropertyNameLen, L"text-align")       ||
               STR_MATCH(PropertyName, PropertyNameLen, L"background-color") ||
               STR_MATCH(PropertyName, PropertyNameLen, L"z-index")          ||
               STR_MATCH(PropertyName, PropertyNameLen, L"left")             ||
               STR_MATCH(PropertyName, PropertyNameLen, L"top")              ||
               STR_MATCH(PropertyName, PropertyNameLen, L"right")            ||
               STR_MATCH(PropertyName, PropertyNameLen, L"bottom")) {
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%a", Property->ValueStr);
      AttrName = PropertyName;
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"font-size")) {
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%d", Value.H2OValue.Value.U32);
      AttrName = PropertyName;
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"color")) {
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"0x%08x", (UINT32) Value.H2OValue.Value.U32);
      AttrName = L"textcolor";
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"visibility")) {
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%s", AsciiStrCmp (Property->ValueStr, "visible") == 0 ? L"true" : L"false");
      AttrName = PropertyName;
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"text-control")) {
      if (AsciiStrCmp (Property->ValueStr, "ellipsis") == 0) {
        UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%a", Property->ValueStr);
      } else {
        continue;
      }
      AttrName = L"text-overflow";
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"background-image")) {
      Image = GetVfcfImage (Value.H2OValue.Value.U16);
      if (Image == NULL) {
        continue;
      }
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"0x%lx", Image);
      AttrName = PropertyName;
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"background-size")) {
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%a", Property->ValueStr);
      AttrName = PropertyName;

      UiSetAttribute (Target, L"scale9grid", L"0,0,1,1");
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"control-type")) {
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%a", Property->ValueStr);
      if (StrCmp (Target->Name, L"value") == 0) {
        AttrName = L"value-control-type";
      } else {
        continue;
      }
      Target = Control;
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"token")) {
      Buffer = GetStringById (Value.H2OValue.Value.U16);
      if (Buffer == NULL) {
        continue;
      }
      StrCpyS (ValueStrBuf, sizeof(ValueStrBuf) / sizeof(CHAR16), Buffer);
      FreePool (Buffer);
      AttrName = L"text";
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"filter")) {
      if (AsciiStrCmp (Property->ValueStr, "grayscale") == 0) {
        UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"stretch|gray");
      } else {
        continue;
      }
      AttrName = L"background-image-style";
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"display")) {
      if (AsciiStrCmp (Property->ValueStr, "inline") == 0) {
        if (StrnCmp (Control->Name, L"H2OSetupMenuPanel", StrLen (L"H2OSetupMenuPanel")) == 0) {
          UiSetAttribute (Control, L"layout", L"horizontal");
          continue;
        }
      }

      Order = CONTROL_CLASS(Target)->FindChildByName (Target, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Order != NULL) {
        continue;
      }

      if (AsciiStrCmp (Property->ValueStr, "block") == 0) {
        Order = CreateAddControl (L"VerticalLayout", Target);
      } else if (AsciiStrCmp (Property->ValueStr, "inline") == 0) {
        Order = CreateAddControl (L"HorizontalLayout", Target);
      } else if (AsciiStrCmp (Property->ValueStr, "flex") == 0) {
        Order = CreateAddControl (L"FlexLayout", Target);
      } else {
        ASSERT (FALSE);
        continue;
      }
      UiSetAttribute (Order, L"name", VFCF_PROPERTY_ORDER_CONTROL_NAME);
      if (!IS_CONTROL_WIDTH_DEFINED(Order)) {
        UiSetAttribute (Order, L"width", L"wrap_content");
      }
      if (!IS_CONTROL_HEIGHT_DEFINED(Order)) {
        UiSetAttribute (Order, L"height", L"wrap_content");
      }

      OrderList = ReallocatePool(
                    sizeof (UI_CONTROL *) * OrderCount,
                    sizeof (UI_CONTROL *) * (OrderCount + 1),
                    OrderList
                    );
      if (OrderList == NULL) {
        OrderCount = 0;
        continue;
      }
      OrderList[OrderCount++] = Order;
      continue;
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"flex-direction") ||
               STR_MATCH(PropertyName, PropertyNameLen, L"align-items")    ||
               STR_MATCH(PropertyName, PropertyNameLen, L"align-content")  ||
               STR_MATCH(PropertyName, PropertyNameLen, L"justify-content")) {
      Order = CONTROL_CLASS(Target)->FindChildByName (Target, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Order == NULL) {
        ASSERT (FALSE);
        continue;
      }
      Target = Order;
      UnicodeSPrint (ValueStrBuf, sizeof (ValueStrBuf), L"%a", Property->ValueStr);
      AttrName = PropertyName;
    } else if (STR_MATCH(PropertyName, PropertyNameLen, L"order")) {
      Order = CONTROL_CLASS(Target)->FindChildByName (Target, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Order == NULL) {
        ASSERT (FALSE);
        continue;
      }

      Index = 0;
      StringLength = 0;
      do {
        if (Property->ValueStr[Index] == '|' ||
            Property->ValueStr[Index] == 0) {
          ValueStrBuf[StringLength] = 0;
          StringLength = 0;
          Child = CONTROL_CLASS(Order)->FindChildByName (Order, ValueStrBuf, UI_SEARCH_TYPE_ONLY_CHILD);
          if (Child != NULL) {
            continue;
          }

          Child = CreateSubElementControl (Order, ValueStrBuf);
          if (Child != NULL) {
            UiSetAttribute (Child, L"name", ValueStrBuf);
          }
        } else {
          ValueStrBuf[StringLength++] = (CHAR16) Property->ValueStr[Index];
        }
      } while (Property->ValueStr[Index++] != 0);

      continue;
    } else {
      continue;
    }

    UiSetAttribute (Target, AttrName, ValueStrBuf);
  }

  SetSubElementSizeToWarpContent (Control);

  if (OrderList != NULL) {
    FreePool (OrderList);
  }
}

EFI_STATUS
SetPanelPosFromVfcf (
  IN DISPLAY_ENGINE_FRAME       *This
  )
{
  EFI_STATUS                    Status;
  H2O_LAYOUT_INFO               *LayoutInfo;
  RECT                          ScreenField;
  RECT                          PanelField;
  UI_MANAGER                    *Manager;
  UI_CONTROL                    *Control;
  UI_CONTROL                    *Child;
  CHAR16                        String[20];
  PANEL_VFCF_DATA               *PanelVfcfDataPtr;
  H2O_PANEL_INFO                *PanelInfo;
  BOOLEAN                       IsVisible;
  UINT32                        LayoutId;
  EFI_GUID                      ScuFormSetGuid = {0x9f85453e, 0x2f03, 0x4989, 0xad, 0x3b, 0x4a, 0x84, 0x07, 0x91, 0xaf, 0x3a};
  H2O_FORM_BROWSER_SM           *SetupMenuData;
  EFI_GUID                      FormSetGuid;
  H2O_PROPERTY_INFO             **PropertyList;
  UINTN                         PropertyCount;

  Manager = ((UI_CONTROL *) This)->Manager;

  Status = gFB->GetSMInfo (gFB, &SetupMenuData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CopyGuid (&FormSetGuid, &SetupMenuData->FormSetGuid);
  FreeSetupMenuData (SetupMenuData);

  LayoutInfo = NULL;

  LayoutId = 0;
  Status = EFI_NOT_FOUND;
  if (NeedShowSetupMenu ()) {
    Status = GetLayoutIdByGuid (&ScuFormSetGuid, &LayoutId);
  }
  if (EFI_ERROR (Status)) {
    Status = GetLayoutIdByGuid (&FormSetGuid, &LayoutId);
    if (EFI_ERROR (Status)) {
      Status = GetLayoutIdByGuid (&gZeroGuid, &LayoutId);
    }

    ASSERT_EFI_ERROR (Status);
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetLayoutById (LayoutId, &mMetroPrivate->DisplayEngine.Id, &LayoutInfo);
  if (EFI_ERROR (Status)) {
    Status = GetLayoutIdByGuid (&gZeroGuid, &LayoutId);
    ASSERT_EFI_ERROR (Status);
    Status = GetLayoutById (LayoutId, &mMetroPrivate->DisplayEngine.Id, &LayoutInfo);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  mLayoutInfo = LayoutInfo;

  //DisableHalo (This);

  ScreenField.left   = 0;
  ScreenField.top    = 0;
  ScreenField.right  = (INT32) GetSystemMetrics (SM_CXSCREEN);
  ScreenField.bottom = (INT32) GetSystemMetrics (SM_CYSCREEN);

  PanelVfcfDataPtr = mPanelVfcfData;
  while (PanelVfcfDataPtr->PanelName != NULL) {
    Control = Manager->FindControlByName (Manager, PanelVfcfDataPtr->PanelName);
    PanelInfo = GetPanelInfo (LayoutInfo, PanelVfcfDataPtr->PanelId);
    SetWindowLongPtr (Control->Wnd, GWLP_USERDATA, (INTN)PanelInfo);

    IsVisible = FALSE;
    SetRectEmpty (&PanelField);

    if (PanelInfo != NULL) {
      GetPanelField (PanelInfo, NULL, H2O_IFR_STYLE_TYPE_PANEL, H2O_STYLE_PSEUDO_CLASS_NORMAL, &ScreenField, &PanelField);
      IsVisible = IsVisibility (PanelInfo, NULL, H2O_IFR_STYLE_TYPE_PANEL, H2O_STYLE_PSEUDO_CLASS_NORMAL);
    }
    UiSetAttribute (Control, L"visibility", IsVisible ? L"true" : L"false");

    if (PcdGet32(PcdH2OLmdeMultiLayout) == 0) {
      if (PanelInfo != NULL) {
        Status = GetVfcfPanelPropertyList (
                   PanelInfo,
                   H2O_IFR_STYLE_TYPE_PANEL,
                   H2O_STYLE_PSEUDO_CLASS_NORMAL,
                   &PropertyList,
                   &PropertyCount
                   );
        if (!EFI_ERROR (Status)) {
          SetAttributeFromVfcf (Control, PropertyList, PropertyCount, &ScreenField);

          Child = CONTROL_CLASS(Control)->FindChildByName (Control, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
          if (Child != NULL) {
            UiApplyAttributeList (Child, L"width='0' height='0'");
          }
          FreePool (PropertyList);
        }

        if (PanelVfcfDataPtr->PanelId == HOTKEY_PANEL_ID && mHotkeyItemChilds != NULL) {
          UiApplyAttributeList (Control, L"background-color='0xFF333333' background-image='@OwnerDrawBkg' background-image-style='stretch' padding='5,5,5,5'");
        }
      }
    } else {
      UnicodeSPrint (String, sizeof (String), L"%d", PanelField.left);
      UiSetAttribute (Control, L"left", String);
      UnicodeSPrint (String, sizeof (String), L"%d", PanelField.top);
      UiSetAttribute (Control, L"top", String);
      UnicodeSPrint (String, sizeof (String), L"%d", PanelField.right - PanelField.left);
      UiSetAttribute (Control, L"width", String);
      UnicodeSPrint (String, sizeof (String), L"%d", PanelField.bottom - PanelField.top);
      UiSetAttribute (Control, L"height", String);
      UiSetAttribute (Control, L"position", L"absolute");
    }
    PanelVfcfDataPtr++;
  }
  UiSetAttribute ((UI_CONTROL *) This, L"visibility", L"true");

  return Status;
}

EFI_STATUS
CreatePanelControlFromVfcf (
  IN DISPLAY_ENGINE_FRAME       *This
  )
{
  RECT                          ScreenField;
  H2O_LAYOUT_INFO               *LayoutInfo;
  UI_CONTROL                    *ParentControl;
  UI_CONTROL                    *Control;
  UI_MANAGER                    *Manager;
  H2O_PANEL_INFO                *PanelInfo;
  PANEL_VFCF_DATA               *PanelVfcfDataPtr;
  H2O_PROPERTY_INFO             **PropertyList;
  UINTN                         PropertyCount;
  EFI_STATUS                    Status;
  UI_CONTROL                    *Child;

  if (mLayoutInfo == NULL) {
    return EFI_NOT_READY;
  }

  ParentControl = (UI_CONTROL *) This;
  Manager       = ((UI_CONTROL *) This)->Manager;

  LayoutInfo = mLayoutInfo;

  ScreenField.left   = 0;
  ScreenField.top    = 0;
  ScreenField.right  = (INT32) GetSystemMetrics (SM_CXSCREEN);
  ScreenField.bottom = (INT32) GetSystemMetrics (SM_CYSCREEN);

  PanelInfo = NULL;
  while (TRUE) {
    PanelInfo = GetNextPanel (LayoutInfo, PanelInfo);
    if (PanelInfo == NULL) {
      break;
    }

    Control = NULL;
    PanelVfcfDataPtr = mPanelVfcfData;
    while (PanelVfcfDataPtr->PanelName != NULL) {
      if (PanelVfcfDataPtr->PanelId == PanelInfo->PanelId) {
        Control = Manager->FindControlByName (Manager, PanelVfcfDataPtr->PanelName);
        break;
      }
      PanelVfcfDataPtr++;
    }
    if (Control != NULL) {
      continue;
    }

    if (PanelInfo->PanelType == H2O_PANEL_TYPE_OWNER_DRAW) {
      Control = CreateAddControl (L"H2OOwnerDrawPanel", ParentControl);
    } else if (PanelInfo->PanelType == H2O_PANEL_TYPE_FORM) {
      Control = CreateAddControl (L"FormPanel", ParentControl);
    }
    if (Control == NULL) {
      continue;
    }
    SetWindowLongPtr (Control->Wnd, GWLP_USERDATA, (INTN)PanelInfo);

    Status = GetVfcfPanelPropertyList (
               PanelInfo,
               H2O_IFR_STYLE_TYPE_PANEL,
               H2O_STYLE_PSEUDO_CLASS_NORMAL,
               &PropertyList,
               &PropertyCount
               );
    if (!EFI_ERROR (Status)) {
      SetAttributeFromVfcf (Control, PropertyList, PropertyCount, &ScreenField);

      Child = CONTROL_CLASS(Control)->FindChildByName (Control, VFCF_PROPERTY_ORDER_CONTROL_NAME, UI_SEARCH_TYPE_ONLY_CHILD);
      if (Child != NULL) {
        UiApplyAttributeList (Child, L"width='0' height='0'");
      }
      FreePool (PropertyList);
    }

    This->VfcfPanelControlList = ReallocatePool (
                                   sizeof(UI_CONTROL *) * This->VfcfPanelControlCount,
                                   sizeof(UI_CONTROL *) * (This->VfcfPanelControlCount + 1),
                                   This->VfcfPanelControlList
                                   );
    if (This->VfcfPanelControlList == NULL) {
      This->VfcfPanelControlCount = 0;
      return EFI_OUT_OF_RESOURCES;
    }
    This->VfcfPanelControlList[This->VfcfPanelControlCount++] = Control;
  }

  return EFI_SUCCESS;
}

VOID
CallPanelControlProc (
  DISPLAY_ENGINE_FRAME            *This,
  UINT Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  UINTN                           Index;

  if (This->VfcfPanelControlList == NULL) {
    return;
  }

  for (Index = 0; Index < This->VfcfPanelControlCount; Index++) {
    This->VfcfPanelControlList[Index]->Class->WndProc (This->VfcfPanelControlList[Index]->Wnd, Msg, WParam, LParam);
  }
}

VOID
LocalMetroSetComponent (
  DISPLAY_ENGINE_FRAME            *This
  )
{
  UI_MANAGER  *Manager;

  Manager = ((UI_CONTROL *) This)->Manager;

  This->H2OSetupMenuPanel = Manager->FindControlByName (Manager, L"H2OSetupMenuPanel");
  This->H2OSetupPagePanel = Manager->FindControlByName (Manager, L"H2OSetupPagePanel");
  This->H2OFormTitlePanel = Manager->FindControlByName (Manager, L"H2OFormTitlePanel");
  This->H2OHelpTextPanel  = Manager->FindControlByName (Manager, L"H2OHelpTextPanel");
  This->H2OHotkeyPanel    = Manager->FindControlByName (Manager, L"H2OHotkeyPanel");
  This->H2OOwnerDrawPanel = Manager->FindControlByName (Manager, L"H2OOwnerDrawPanel");
}

VOID
LocalMetroProcessUiNotify (
  DISPLAY_ENGINE_FRAME            *This,
  UINT                          Msg,
  UI_CONTROL                    *Sender
  )
{
  H2O_STATEMENT_ID              StatementId;
  H2O_FORM_BROWSER_S            *Statement;
  HOT_KEY_INFO                  *HotkeyInfo;
  EFI_STATUS                    Status;
  EFI_HII_VALUE                 HiiValue;

  switch (Msg) {

  case UI_NOTIFY_CLICK:
  case UI_NOTIFY_CARRIAGE_RETURN:
    if (StrnCmp (Sender->Name, L"Hotkey", StrLen (L"Hotkey")) == 0) {
      HotkeyInfo = (HOT_KEY_INFO *) GetWindowLongPtr (Sender->Wnd, GWLP_USERDATA);
      HotKeyFunc (HotkeyInfo);
      break;
    }

    if (StrCmp (Sender->Name, L"CheckBox") == 0) {
      StatementId = (H2O_PAGE_ID) GetWindowLongPtr (Sender->Wnd, GWLP_USERDATA);
      Status = gFB->GetSInfo (gFB, gFB->CurrentP->PageId, StatementId, &Statement);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        break;
      }
      SendSelectQNotify (Statement->PageId, Statement->QuestionId, Statement->IfrOpCode);
      CopyMem (&HiiValue, &Statement->HiiValue, sizeof (EFI_HII_VALUE));
      HiiValue.Value.b = !HiiValue.Value.b;
      SendChangeQNotify (Statement->PageId, Statement->QuestionId, &HiiValue);
      break;
    }
    DEBUG ((EFI_D_INFO, "Unsuppoert item click: %s", Sender->Name));
    ASSERT (FALSE);
    break;
  }
}

STATIC
VOID
UpdateHaloColor (
  IN DISPLAY_ENGINE_FRAME            *This
  )
{
  UI_CONTROL                    *Control;
  HSV_VALUE                     MenuHsv;
  HSV_VALUE                     HaloHsv;
  INT16                         HueDiff;
  INT8                          SaturationDiff;
  INT8                          ValueDiff;
  CHAR16                        Str[20];

  ASSERT (This != NULL);
  GetCurrentHaloHsv (&HaloHsv);
  GetCurrentMenuHsv (&MenuHsv);
  if (MenuHsv.Hue == 0 && MenuHsv.Saturation == 0 && MenuHsv.Value == 0) {
    return;
  }
  HueDiff        = (INT16) (MenuHsv.Hue - HaloHsv.Hue);
  SaturationDiff = (INT8)  (MenuHsv.Saturation - HaloHsv.Saturation);
  ValueDiff      = (INT8)  (MenuHsv.Value - HaloHsv.Value);
  UnicodeSPrint (Str, sizeof (Str), L"%d,%d,%d",  HueDiff, SaturationDiff, ValueDiff);
  Control = UiFindChildByName (This, L"FormHalo");
  UiSetAttribute (Control, L"hsvadjust", Str);
}

EFI_STATUS
UpdateHaloPos (
  IN DISPLAY_ENGINE_FRAME                    *This
  )
{
  EFI_STATUS                               Status;
  RECT                                     Field;
  UI_MANAGER                               *Manager;
  UI_CONTROL                               *Control;

  Status = GetInformationField (&Field);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Manager = ((UI_CONTROL *) This)->Manager;

  Control = Manager->FindControlByName (Manager, L"FormHalo");
  if (Control == NULL) {
    return EFI_NOT_FOUND;
  }

  UiSetAttributeEx (Control, L"left", L"%d", Field.left);
  UiSetAttributeEx (Control, L"top", L"%d", Field.top);
  UiSetAttributeEx (Control, L"width", L"%d", (Field.right - Field.left));
  UiSetAttributeEx (Control, L"height", L"%d", (Field.bottom - Field.top));
  UiSetAttributeEx (Control, L"position", L"absolute");

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
GetContextField (
  OUT RECT                                    *ContextField
  )
{
  EFI_STATUS                                  Status;
  RECT                                        SetupMenuPanelField;
  RECT                                        FormTitlePanelField;
  RECT                                        SetupPagePanelField;
  RECT                                        HelpTextPanelField;
  RECT                                        HotkeyPanelField;

  Status = GetRectByName (gWnd, L"H2OSetupMenuPanel", &SetupMenuPanelField);
  if (EFI_ERROR (Status)) {
    SetRectEmpty (&SetupMenuPanelField);
  }

  Status = GetRectByName (gWnd, L"H2OFormTitlePanel", &FormTitlePanelField);
  if (EFI_ERROR (Status)) {
    SetRectEmpty (&FormTitlePanelField);
  }

  Status = GetRectByName (gWnd, L"H2OSetupPagePanel", &SetupPagePanelField);
  if (EFI_ERROR (Status)) {
    SetRectEmpty (&SetupPagePanelField);
  }

  Status = GetRectByName (gWnd, L"H2OHelpTextPanel", &HelpTextPanelField);
  if (EFI_ERROR (Status)) {
    SetRectEmpty (&HelpTextPanelField);
  }

  Status = GetRectByName (gWnd, L"H2OHotkeyPanel", &HotkeyPanelField);
  if (EFI_ERROR (Status)) {
    SetRectEmpty (&HotkeyPanelField);
  }

  UnionRect (ContextField, &SetupMenuPanelField, &FormTitlePanelField);
  UnionRect (ContextField, ContextField, &SetupPagePanelField);
  UnionRect (ContextField, ContextField, &HelpTextPanelField);
  UnionRect (ContextField, ContextField, &HotkeyPanelField);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateContextPos (
  IN VOID                                  *Frame
  )
{
  EFI_STATUS                               Status;
  RECT                                     Field;
  UI_MANAGER                               *Manager;
  UI_CONTROL                               *Control;
  CHAR16                                   String[20];

  Status = GetContextField (&Field);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Manager = ((UI_CONTROL *) Frame)->Manager;

  Control = Manager->FindControlByName (Manager, L"ContextBkg");
  if (Control == NULL) {
    return EFI_NOT_FOUND;
  }

  UnicodeSPrint (String, sizeof (String), L"%d", Field.left);
  UiSetAttribute (Control, L"left", String);
  UnicodeSPrint (String, sizeof (String), L"%d", Field.top);
  UiSetAttribute (Control, L"top", String);
  UnicodeSPrint (String, sizeof (String), L"%d", Field.right - Field.left);
  UiSetAttribute (Control, L"width", String);
  UnicodeSPrint (String, sizeof (String), L"%d", Field.bottom - Field.top);
  UiSetAttribute (Control, L"height", String);
  UiSetAttribute (Control, L"position", L"absolute");

  return EFI_SUCCESS;
}

BOOLEAN
IsMetroPrivateHotKey (
  IN EFI_KEY_DATA               *KeyData
  )
{
  UINTN                         Index;

  if (KeyData == NULL) {
    return FALSE;
  }

  for (Index = 0; Index < sizeof (mPrivateHotKeyList) / sizeof (EFI_INPUT_KEY); Index++) {
    if (KeyData->Key.ScanCode    == mPrivateHotKeyList[Index].ScanCode &&
        KeyData->Key.UnicodeChar == mPrivateHotKeyList[Index].UnicodeChar &&
        KeyData->KeyState.KeyShiftState == EFI_SHIFT_STATE_VALID) {
      return TRUE;
    }
  }

  return FALSE;
}

VOID
LocalMetroRegisterHotkey (
  HWND                          Wnd
  )
{
  INT32                         Id;

  Id = 1;
  while (UnregisterHotKey (Wnd, Id)) {
    Id++;
  }

  Id = 1;
  MwRegisterHotKey (Wnd, Id++,  0, VK_ESCAPE);
}

LRESULT
CALLBACK
DisplayEngineProc (
  HWND Wnd,
  UINT Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  DISPLAY_ENGINE_FRAME            *This;
  UI_MANAGER                    *Manager;
  UI_CONTROL                    *Control;
  UINTN                         VKCode;
  HOT_KEY_INFO                  HotKeyInfo;
  UI_LIST_VIEW                  *ListView;

  This    = (DISPLAY_ENGINE_FRAME *) GetWindowLongPtr (Wnd, 0);
  if (This == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  Control = (UI_CONTROL *) This;

  switch ( Msg ) {

  case WM_CREATE:
    This = (DISPLAY_ENGINE_FRAME *) AllocateZeroPool (sizeof (DISPLAY_ENGINE_FRAME));
    if (This != NULL) {
      CONTROL_CLASS(This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Wnd, 0);
      SetWindowLongPtr (Wnd, 0, (INTN)This);
      SendMessage (Wnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam,LParam);
    //
    // create UiContainer as root
    //
    Manager = Control->Manager;
    SetWindowLongPtr (Wnd, GWL_EXSTYLE, GetWindowLongPtr (Control->Wnd, GWL_EXSTYLE) & (~WS_EX_NOACTIVATE));
    if (gFB->CurrentP != NULL) {
      XmlCreateControl (mScreenPanelChilds, Control);
      LocalMetroSetComponent (This);
      SetPanelPosFromVfcf (This);
      CreatePanelControlFromVfcf (This);
    } else {
      XmlCreateControl (mDialogPanelChilds, Control);
    }
    break;

  case WM_SETFOCUS:
    if (gFB->CurrentP == NULL) {
      break;
    }
    if (gLastFocus != NULL) {
      if (gLastFocus == This->H2OSetupMenuPanel->Wnd && IsWindowVisible (This->H2OSetupMenuPanel->Wnd)) {
        SetFocus (gLastFocus);
        break;
      } else if (gLastFocus == This->H2OSetupPagePanel->Wnd) {
        SetFocus (gLastFocus);
        break;
      }
    }

    ListView = (UI_LIST_VIEW *) This->H2OSetupMenuPanel;
    if (IsWindowVisible(This->H2OSetupMenuPanel->Wnd) && ListView->Layout != UiListViewHorizontalLayout) {
      SetFocus (This->H2OSetupMenuPanel->Wnd);
    } else {
      SetFocus (This->H2OSetupPagePanel->Wnd);
    }
    break;

  case WM_HOTKEY:
    if (GetWindowLongPtr (Wnd, GWL_STYLE) & WS_DISABLED) {
      break;
    }

    VKCode = (UINTN) HIWORD(LParam);

    switch (VKCode) {

    case VK_ESCAPE:
      ZeroMem (&HotKeyInfo, sizeof (HOT_KEY_INFO));
      HotKeyInfo.HotKeyAction = HotKeyDiscardAndExit;
      HotKeyFunc (&HotKeyInfo);
      break;
    }
    break;

  case FB_NOTIFY_SELECT_Q:
    if (gFB->CurrentP != NULL) {
      This->H2OSetupMenuPanel->Class->WndProc (This->H2OSetupMenuPanel->Wnd, Msg, WParam, LParam);
      This->H2OSetupPagePanel->Class->WndProc (This->H2OSetupPagePanel->Wnd, Msg, WParam, LParam);
      This->H2OFormTitlePanel->Class->WndProc (This->H2OFormTitlePanel->Wnd, Msg, WParam, LParam);
      This->H2OHelpTextPanel->Class->WndProc (This->H2OHelpTextPanel->Wnd, Msg, WParam, LParam);
      This->H2OHotkeyPanel->Class->WndProc (This->H2OHotkeyPanel->Wnd, Msg, WParam, LParam);
      This->H2OOwnerDrawPanel->Class->WndProc (This->H2OOwnerDrawPanel->Wnd, Msg, WParam, LParam);
      CallPanelControlProc(This, Msg, WParam, LParam);
    }
    break;

  case FB_NOTIFY_REPAINT:
    if (gFB->CurrentP != NULL) {
      This->H2OSetupMenuPanel->Class->WndProc (This->H2OSetupMenuPanel->Wnd, Msg, WParam, LParam);
      This->H2OSetupPagePanel->Class->WndProc (This->H2OSetupPagePanel->Wnd, Msg, WParam, LParam);
      This->H2OFormTitlePanel->Class->WndProc (This->H2OFormTitlePanel->Wnd, Msg, WParam, LParam);
      This->H2OHelpTextPanel->Class->WndProc (This->H2OHelpTextPanel->Wnd, Msg, WParam, LParam);
      This->H2OHotkeyPanel->Class->WndProc (This->H2OHotkeyPanel->Wnd, Msg, WParam, LParam);
      This->H2OOwnerDrawPanel->Class->WndProc (This->H2OOwnerDrawPanel->Wnd, Msg, WParam, LParam);
      CallPanelControlProc(This, Msg, WParam, LParam);
      //UpdateHaloPos (This);
      //UpdateHaloColor (This);
      UpdateContextPos (This);
      LocalMetroRegisterHotkey (Wnd);
      SetFocus (Wnd);
    }
    break;

  case FB_NOTIFY_REFRESH_Q:
    if (gFB->CurrentP != NULL) {
      This->H2OSetupPagePanel->Class->WndProc (This->H2OSetupPagePanel->Wnd, Msg, WParam, LParam);
      This->H2OHelpTextPanel->Class->WndProc (This->H2OHelpTextPanel->Wnd, Msg, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CLICK:
  case UI_NOTIFY_CARRIAGE_RETURN:
    LocalMetroProcessUiNotify (This, Msg, (UI_CONTROL *)WParam);
    break;

  case WM_DESTROY:
    if (This->VfcfPanelControlList != NULL) {
      FreePool (This->VfcfPanelControlList);
    }
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam,LParam);
    break;

  case WM_CLOSE:
    DestroyWindow (Wnd);
    break;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam,LParam);
  }
  return 0;
}

DISPLAY_ENGINE_FRAME_CLASS *
GetDisplayEngineFrameClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), DISPLAY_ENGINE_CLASS_NAME, (UI_CONTROL_CLASS *)GetFrameClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc = DisplayEngineProc;

  return CURRENT_CLASS;
}



UI_GET_CLASS mGetClassTable[] = {
               (UI_GET_CLASS) GetControlClass          ,
               (UI_GET_CLASS) GetTextureClass          ,

               (UI_GET_CLASS) GetLabelClass            ,
               (UI_GET_CLASS) GetButtonClass           ,
               (UI_GET_CLASS) GetSwitchClass           ,

               (UI_GET_CLASS) GetEditClass             ,
               (UI_GET_CLASS) GetScrollBarClass        ,
               (UI_GET_CLASS) GetVerticalLayoutClass   ,
               (UI_GET_CLASS) GetHorizontalLayoutClass ,
               (UI_GET_CLASS) GetFlexLayoutClass       ,

               (UI_GET_CLASS) GetListViewClass,
               (UI_GET_CLASS) GetNumberPickerClass     ,

               (UI_GET_CLASS) GetDialogClass           ,
               (UI_GET_CLASS) GetFrameClass            ,


               (UI_GET_CLASS) GetFrontPageItemClass    ,
               (UI_GET_CLASS) GetSetupPageItemClass    ,
               (UI_GET_CLASS) GetSetupMenuItemClass    ,
               (UI_GET_CLASS) GetDateItemClass         ,
               (UI_GET_CLASS) GetTimeItemClass         ,
               (UI_GET_CLASS) GetHotkeyItemClass       ,

               (UI_GET_CLASS) GetDisplayEngineFrameClass,

               (UI_GET_CLASS) GetH2OFormTitlePanelClass,
               (UI_GET_CLASS) GetH2OHotkeyPanelClass,
               (UI_GET_CLASS) GetH2OHelpTextPanelClass,
               (UI_GET_CLASS) GetH2OOwnerDrawPanelClass,
               (UI_GET_CLASS) GetH2OSetupMenuPanelClass,
               (UI_GET_CLASS) GetH2OSetupPagePanelClass,

               (UI_GET_CLASS) GetScrollViewClass,
               (UI_GET_CLASS) GetFormPanelClass,
               (UI_GET_CLASS) GetH2OConfirmPageDialogClass,
               (UI_GET_CLASS) GetH2OConfirmPasswordDialogClass,
               (UI_GET_CLASS) GetH2ODateTimeDialogClass,
               (UI_GET_CLASS) GetH2OFormDialogClass,
               (UI_GET_CLASS) GetH2OHelpDialogClass,
               (UI_GET_CLASS) GetH2OMsgPopUpDialogClass,
               (UI_GET_CLASS) GetH2ONumericDialogClass,
               (UI_GET_CLASS) GetH2OOneOfInputDialogClass,
               (UI_GET_CLASS) GetH2OOrderedListDialogClass,
               (UI_GET_CLASS) GetH2OPasswordDialogClass,
               (UI_GET_CLASS) GetH2OPopupDialogClass,
               (UI_GET_CLASS) GetH2OStringDialogClass,

               (UI_GET_CLASS) GetH2OStatementControlClass,
               (UI_GET_CLASS) GetH2OActionButtonClass,
               (UI_GET_CLASS) GetH2ODateRollerClass,
               (UI_GET_CLASS) GetH2ONumericInputClass,
               (UI_GET_CLASS) GetH2OOptionListClass,
               (UI_GET_CLASS) GetH2OOrderedListOptionListClass,
               (UI_GET_CLASS) GetH2OPasswordInputClass,
               (UI_GET_CLASS) GetH2OStringInputClass,
               (UI_GET_CLASS) GetH2OTimeRollerClass,

               NULL,
            };


EFI_STATUS
InitializeGUI (
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput
  )
{
  EFI_STATUS                    Status;

  Status = gBS->LocateProtocol (
                  &gSetupMouseProtocolGuid,
                  NULL,
                  (VOID **) &mSetupMouse
                  );
  if (EFI_ERROR (Status)) {
    mSetupMouse = NULL;
  }

  AddHiiImagePackage ();

  GdAddGopDevice (GraphicsOutput);

  if (MwOpen() < 0) {
    return EFI_INVALID_PARAMETER;
  }
  RegisterClassTable (mGetClassTable);

  return EFI_SUCCESS;
}

EFI_STATUS
InitializeWindows (
  VOID
  )
{
  MSG                                      Msg;

  if (gWnd != NULL) {
    return EFI_ALREADY_STARTED;
  }

  //
  // CreateWindow
  //
  gWnd = CreateWindowEx (
           0,
           DISPLAY_ENGINE_CLASS_NAME,
           L"Insyde H2O",
           WS_OVERLAPPED | WS_VISIBLE,
           0, 0, GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN),
           NULL,
           NULL,
           NULL,
           NULL
           );
  //
  // Process Message
  //
  while (PeekMessage (&Msg, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage (&Msg);
    DispatchMessage (&Msg);
  }

  return EFI_SUCCESS;
}

/**
  Destroy window and disconnect server
**/
VOID
ShutdownGUI (
  VOID
  )
{
  MSG                                      Msg;

  if (gWnd != NULL) {
    DestroyWindow (gWnd);
    gWnd = NULL;
    //
    // remove all message (ex: DeskTop WM_PAINT)
    //
    while (PeekMessage (&Msg, NULL, 0, 0, PM_REMOVE));
  }
  MwClose ();
}
