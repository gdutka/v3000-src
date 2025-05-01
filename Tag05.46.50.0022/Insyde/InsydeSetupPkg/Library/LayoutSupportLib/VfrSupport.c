/** @file

;******************************************************************************
;* Copyright (c) 2013 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "LayoutSupportLib.h"

typedef struct _LAYOUT_LIB_STYLE_PRIORITY_INFO {
  H2O_STYLE_INFO                               **StyleList;
  UINT32                                       StyleCount;
} LAYOUT_LIB_STYLE_PRIORITY_INFO;

EFI_STATUS
GetImportInfoList (
  IN  EFI_GUID                                *FormSetGuid,
  OUT IMPORT_INFO                             **ImportInfoList,
  OUT UINT16                                  *ImportInfoCount
  )
{
  EFI_STATUS                                   Status;
  LIST_ENTRY                                   *VfrLink;
  LIST_ENTRY                                   *FormSetLink;
  LIST_ENTRY                                   *FormLink;
  LIST_ENTRY                                   *ImportLink;
  LIST_ENTRY                                   *ExpressionQIdMapLink;

  H2O_VFR_INFO                                 *VfrInfo;
  H2O_FORMSET_INFO                             *FormSetInfo;
  H2O_FORM_INFO                                *FormInfo;
  H2O_IMPORT_INFO                              *ImportInfo;
  H2O_EXPRESSION_QID_MAP_INFO                  *ExpressionQIdMapInfo;
  H2O_HII_LIBT_EXPRESSION_QID_MAP_BLOCK        *ExpressionQIdMapBlock;


  IMPORT_INFO                                  *ImportList;
  UINT16                                       ImportCount;

  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status) || VfrListHead == NULL) {
    return EFI_NOT_FOUND;
  }

  ASSERT (FormSetGuid != NULL);
  ASSERT (ImportInfoCount != NULL);
  if (FormSetGuid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ImportCount = 0;

  VfrLink = GetFirstNode (VfrListHead);
  while (!IsNull (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);
    VfrLink = GetNextNode (VfrListHead, VfrLink);

    FormSetLink = GetFirstNode (&VfrInfo->FormsetListHead);
    while (!IsNull (&VfrInfo->FormsetListHead, FormSetLink)) {
      FormSetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormSetLink);
      FormSetLink = GetNextNode (&VfrInfo->FormsetListHead, FormSetLink);

      if (!CompareGuid (&(FormSetInfo->FormsetId), FormSetGuid)) {
        continue ;
      }

      //
      // Expreesion question id map
      //
      ExpressionQIdMapLink = GetFirstNode (&FormSetInfo->ExpressionQIdMapList);
      while (!IsNull (&FormSetInfo->ExpressionQIdMapList, ExpressionQIdMapLink)) {
        ExpressionQIdMapLink = GetNextNode (&FormSetInfo->ExpressionQIdMapList, ExpressionQIdMapLink);
        ImportCount++;
      }

      //
      // import question
      //
      FormLink = GetFirstNode (&FormSetInfo->FormListHead);
      while (!IsNull (&FormSetInfo->FormListHead, FormLink)) {
        FormInfo = H2O_FORM_INFO_NODE_FROM_LINK (FormLink);
        FormLink = GetNextNode (&FormSetInfo->FormListHead, FormLink);

        ImportLink = GetFirstNode (&FormInfo->ImportListHead);
        while (!IsNull (&FormInfo->ImportListHead, ImportLink)) {
          ImportLink = GetNextNode (&FormInfo->ImportListHead, ImportLink);
          ImportCount++;
        }
      }
    }
  }

  if (ImportCount == 0) {
    return EFI_NOT_FOUND;
  }

  ImportList = AllocateZeroPool (ImportCount * sizeof (IMPORT_INFO));
  if (ImportList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *ImportInfoList  = ImportList;
  *ImportInfoCount = ImportCount;


  VfrLink = GetFirstNode (VfrListHead);
  while (!IsNull (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);
    VfrLink = GetNextNode (VfrListHead, VfrLink);

    FormSetLink = GetFirstNode (&VfrInfo->FormsetListHead);
    while (!IsNull (&VfrInfo->FormsetListHead, FormSetLink)) {
      FormSetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormSetLink);
      FormSetLink = GetNextNode (&VfrInfo->FormsetListHead, FormSetLink);

      if (!CompareGuid (&(FormSetInfo->FormsetId), FormSetGuid)) {
        continue ;
      }

      //
      // Expreesion question id map
      //
      ExpressionQIdMapLink = GetFirstNode (&FormSetInfo->ExpressionQIdMapList);
      while (!IsNull (&FormSetInfo->ExpressionQIdMapList, ExpressionQIdMapLink)) {
        ExpressionQIdMapInfo = H2O_EXPRESSION_QID_MAP_INFO_NODE_FROM_LINK (ExpressionQIdMapLink);
        ExpressionQIdMapLink = GetNextNode (&FormSetInfo->ExpressionQIdMapList, ExpressionQIdMapLink);

        ExpressionQIdMapBlock = ExpressionQIdMapInfo->ExpressionQIdMapBlock;

        CopyMem (&ImportList->SrcFormSetGuid, &ExpressionQIdMapBlock->TargetFormSetGuid, sizeof (EFI_GUID));
        ImportList->Flags         = H2O_HII_IMPORT_QUESTION;
        ImportList->SrcFormId     = ExpressionQIdMapBlock->TargetFormId;
        ImportList->SrcId         = ExpressionQIdMapBlock->TargetQId;
        ImportList->DstFormId     = H2O_IFR_GUID_IMPORT_LOCAL_ID_USE_TARGET_ID;
        ImportList->DstId         = ExpressionQIdMapBlock->LocalQId;
        ImportList->ExpressionQId = ExpressionQIdMapBlock->ExpressionQId;
        ImportList++;
      }

      //
      // import question
      //
      FormLink = GetFirstNode (&FormSetInfo->FormListHead);
      while (!IsNull (&FormSetInfo->FormListHead, FormLink)) {
        FormInfo = H2O_FORM_INFO_NODE_FROM_LINK (FormLink);
        FormLink = GetNextNode (&FormSetInfo->FormListHead, FormLink);

        ImportLink = GetFirstNode (&FormInfo->ImportListHead);
        while (!IsNull (&FormInfo->ImportListHead, ImportLink)) {
          ImportInfo = H2O_IMPORT_INFO_NODE_FROM_LINK (ImportLink);
          ImportLink = GetNextNode (&FormInfo->ImportListHead, ImportLink);

          CopyGuid (&ImportList->SrcFormSetGuid, &ImportInfo->TargetFormsetGuid);
          ImportList->Flags         = ImportInfo->Flags;
          ImportList->SrcFormId     = ImportInfo->TargetFormId;
          ImportList->SrcId         = ImportInfo->TargetId;
          ImportList->DstFormId     = (UINT16)FormInfo->FormBlock->FormId;
          ImportList->DstId         = ImportInfo->LocalId;
          ImportList->ExpressionQId = 0;
          ImportList++;
        }
      }
    }

  }

  return EFI_SUCCESS;
}

UINT32
GetStyleTypeByOpCode (
  IN    UINT8                                  OpCode
  )
{
  switch (OpCode) {

  case EFI_IFR_SUBTITLE_OP:
    return H2O_IFR_STYLE_TYPE_SUBTITLE;

  case EFI_IFR_CHECKBOX_OP:
    return H2O_IFR_STYLE_TYPE_CHECKBOX;

  case EFI_IFR_TEXT_OP:
    return H2O_IFR_STYLE_TYPE_TEXT;

  case EFI_IFR_ACTION_OP:
    return H2O_IFR_STYLE_TYPE_ACTION;

  case EFI_IFR_REF_OP:
    return H2O_IFR_STYLE_TYPE_GOTO;

  case EFI_IFR_PASSWORD_OP:
    return H2O_IFR_STYLE_TYPE_PASSWORD;

  case EFI_IFR_NUMERIC_OP:
    return H2O_IFR_STYLE_TYPE_NUMERIC;

  case EFI_IFR_ONE_OF_OP:
    return H2O_IFR_STYLE_TYPE_ONEOF;

  case EFI_IFR_TIME_OP:
    return H2O_IFR_STYLE_TYPE_TIME;

  case EFI_IFR_DATE_OP:
    return H2O_IFR_STYLE_TYPE_DATE;

  case EFI_IFR_ORDERED_LIST_OP:
    return H2O_IFR_STYLE_TYPE_ORDEREDLIST;

  case EFI_IFR_RESET_BUTTON_OP:
    return H2O_IFR_STYLE_TYPE_RESETBUTTON;

  case EFI_IFR_STRING_OP:
    return H2O_IFR_STYLE_TYPE_STRING;

  default:
    break;
  }

  ASSERT (FALSE);
  return H2O_IFR_STYLE_TYPE_SHEET;
}

EFI_STATUS
GetVfcfFormsetLayoutId (
  IN  EFI_GUID                                 *FormsetGuid,
  OUT UINT32                                   *FormsetLayoutId
  )
{
  EFI_STATUS                                   Status;
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *VfrLink;
  H2O_VFR_INFO                                 *VfrInfo;
  LIST_ENTRY                                   *FormsetListHead;
  LIST_ENTRY                                   *FormsetLink;
  H2O_FORMSET_INFO                             *FormSetInfo;
  UINT32                                       LayoutId;

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || VfrListHead == NULL || IsListEmpty (VfrListHead)) {
    return EFI_NOT_FOUND;
  }

  LayoutId = 0;
  for (VfrLink = GetFirstNode (VfrListHead); !IsNull (VfrListHead, VfrLink); VfrLink = GetNextNode (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);

    FormsetListHead = &VfrInfo->FormsetListHead;
    for (FormsetLink = GetFirstNode (FormsetListHead); !IsNull (FormsetListHead, FormsetLink); FormsetLink = GetNextNode (FormsetListHead, FormsetLink)) {
      FormSetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);
      if (FormSetInfo->FormsetBlock->LayoutId != 0 &&
          CompareGuid (&FormSetInfo->FormsetId, FormsetGuid)) {
        LayoutId = FormSetInfo->FormsetBlock->LayoutId;
      }
    }
  }
  if (LayoutId == 0) {
    return EFI_NOT_FOUND;
  }

  *FormsetLayoutId = LayoutId;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
StylePriorityInfoAddStyle (
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  IN  LIST_ENTRY                               *StyleListHead,
  IN  UINTN                                    StylePriorityInfoCount,
  OUT LAYOUT_LIB_STYLE_PRIORITY_INFO           *StylePriorityInfo
  )
{
  LIST_ENTRY                                   *StyleLink;
  H2O_STYLE_INFO                               *CurrentStyle;
  UINTN                                        PriorityIndex;

  if (StyleListHead == NULL || StylePriorityInfoCount < 4 || StylePriorityInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (StyleLink = StyleListHead->ForwardLink; !IsNull (StyleListHead, StyleLink); StyleLink = StyleLink->ForwardLink) {
    CurrentStyle = H2O_STYLE_INFO_NODE_FROM_LINK (StyleLink);
    if ((StyleType   != 0 && CurrentStyle->StyleType   != 0 && StyleType   != CurrentStyle->StyleType) ||
        (PseudoClass != 0 && CurrentStyle->PseudoClass != 0 && PseudoClass != CurrentStyle->PseudoClass) ||
        (PseudoClass == 0 && CurrentStyle->PseudoClass != 0)) {
      continue;
    }

    if (StyleType != 0 && CurrentStyle->StyleType != 0 && PseudoClass != 0 && CurrentStyle->PseudoClass != 0) {
      if (StyleType != CurrentStyle->StyleType || PseudoClass != CurrentStyle->PseudoClass) {
        continue;
      }
      //
      // ".ClassName StyleType : PseudoClass" priority is the first
      //
      PriorityIndex = 3;
    } else if (StyleType != 0 && CurrentStyle->StyleType != 0) {
      if (StyleType != CurrentStyle->StyleType) {
        continue;
      }
      //
      // ".ClassName StyleType" priority is the second
      //
      PriorityIndex = 2;
    } else if (PseudoClass != 0 && CurrentStyle->PseudoClass != 0) {
      if (PseudoClass != CurrentStyle->PseudoClass) {
        continue;
      }
      //
      // ".ClassName : PseudoClass" priority is the third
      //
      PriorityIndex = 1;
    } else if (CurrentStyle->StyleType == 0 && CurrentStyle->PseudoClass == 0) {
      //
      // ".ClassName" priority is the last
      //
      PriorityIndex = 0;
    } else {
      continue;
    }

    StylePriorityInfo[PriorityIndex].StyleList = ReallocatePool (
                                                   sizeof(H2O_STYLE_INFO *) * (StylePriorityInfo[PriorityIndex].StyleCount),
                                                   sizeof(H2O_STYLE_INFO *) * (StylePriorityInfo[PriorityIndex].StyleCount + 1),
                                                   StylePriorityInfo[PriorityIndex].StyleList
                                                   );
    if (StylePriorityInfo[PriorityIndex].StyleList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    StylePriorityInfo[PriorityIndex].StyleList[StylePriorityInfo[PriorityIndex].StyleCount] = CurrentStyle;
    StylePriorityInfo[PriorityIndex].StyleCount++;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
StylePriorityInfoGetPropertyList (
  IN  UINTN                                    StylePriorityInfoCount,
  IN  LAYOUT_LIB_STYLE_PRIORITY_INFO           *StylePriorityInfo,
  OUT UINTN                                    *PropertyCount,
  OUT H2O_PROPERTY_INFO                        ***PropertyList
  )
{
  UINTN                                        PriorityIndex;
  UINTN                                        StyleIndex;
  UINTN                                        TotalPropCount;
  H2O_PROPERTY_INFO                            **PropList;
  H2O_PROPERTY_INFO                            *CurrentProp;
  LIST_ENTRY                                   *PropListHead;
  LIST_ENTRY                                   *PropLink;
  UINTN                                        PropCount;
  UINTN                                        PropIndex;

  if (StylePriorityInfoCount == 0 || StylePriorityInfo == NULL || PropertyCount == NULL || PropertyList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  TotalPropCount = 0;
  for (PriorityIndex = 0; PriorityIndex < StylePriorityInfoCount; PriorityIndex++) {
    for (StyleIndex = 0; StyleIndex < StylePriorityInfo[PriorityIndex].StyleCount; StyleIndex++) {
      TotalPropCount += StylePriorityInfo[PriorityIndex].StyleList[StyleIndex]->PropertyCount;
    }
  }
  if (TotalPropCount == 0) {
    return EFI_NOT_FOUND;
  }

  PropList = AllocatePool (sizeof (H2O_PROPERTY_INFO *) * TotalPropCount);
  if (PropList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  PropCount = 0;
  for (PriorityIndex = 0; PriorityIndex < StylePriorityInfoCount; PriorityIndex++) {
    for (StyleIndex = 0; StyleIndex < StylePriorityInfo[PriorityIndex].StyleCount; StyleIndex++) {
      PropListHead = &StylePriorityInfo[PriorityIndex].StyleList[StyleIndex]->PropertyListHead;

      for (PropLink = PropListHead->ForwardLink; !IsNull (PropListHead, PropLink); PropLink = PropLink->ForwardLink) {
        CurrentProp = H2O_PROPERTY_INFO_NODE_FROM_LINK (PropLink);

        for (PropIndex = 0; PropIndex < PropCount; PropIndex++) {
          if (CurrentProp->IdentifierStrSize == PropList[PropIndex]->IdentifierStrSize &&
              AsciiStrCmp (CurrentProp->IdentifierStr, PropList[PropIndex]->IdentifierStr) == 0) {
            PropList[PropIndex] = CurrentProp;
            break;
          }
        }
        if (PropIndex == PropCount) {
          PropList[PropCount] = CurrentProp;
          PropCount++;
        }
      }
    }
  }
  if (PropCount == 0) {
    FreePool (PropList);
    return EFI_NOT_FOUND;
  }

  if (PropCount < TotalPropCount) {
    PropList = ReallocatePool (
                 sizeof (H2O_PROPERTY_INFO *) * TotalPropCount,
                 sizeof (H2O_PROPERTY_INFO *) * PropCount,
                 PropList
                 );
    if (PropList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  *PropertyList  = PropList;
  *PropertyCount = PropCount;
  return EFI_SUCCESS;
}

EFI_STATUS
GetVfcfLayoutPropertyList (
  IN  UINT32                                   LayoutId,
  IN  EFI_GUID                                 *DisplayEngineGuid,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  OUT H2O_PROPERTY_INFO                        ***PropertyList,
  OUT UINTN                                    *PropertyCount
  )
{
  EFI_STATUS                                   Status;
  LAYOUT_LIB_STYLE_PRIORITY_INFO               PriorityInfo[4];
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *LayoutLink;
  H2O_LAYOUT_INFO                              *CurrentLayout;
  UINTN                                        PriorityIndex;

  if (DisplayEngineGuid == NULL || PropertyList == NULL || PropertyCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || LayoutListHead == NULL) {
    return EFI_NOT_FOUND;
  }

  ZeroMem (PriorityInfo, sizeof (PriorityInfo));

  for (LayoutLink = LayoutListHead->ForwardLink; !IsNull (LayoutListHead, LayoutLink); LayoutLink = LayoutLink->ForwardLink) {
    CurrentLayout = H2O_LAYOUT_INFO_NODE_FROM_LINK (LayoutLink);
    if ((CurrentLayout->LayoutId != LayoutId) ||
        (!((CompareGuid(&CurrentLayout->DisplayEngineGuid[0], DisplayEngineGuid)) ||
           (CompareGuid(&CurrentLayout->DisplayEngineGuid[1], DisplayEngineGuid)) ||
           (CompareGuid(&CurrentLayout->DisplayEngineGuid[2], DisplayEngineGuid))))) {
      continue;
    }

    StylePriorityInfoAddStyle (
      StyleType,
      PseudoClass,
      &CurrentLayout->StyleListHead,
      sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO),
      PriorityInfo
      );
  }

  Status = StylePriorityInfoGetPropertyList (
             sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO),
             PriorityInfo,
             PropertyCount,
             PropertyList
             );

  for (PriorityIndex = 0; PriorityIndex < (sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO)); PriorityIndex++) {
    if (PriorityInfo[PriorityIndex].StyleList != NULL) {
      FreePool (PriorityInfo[PriorityIndex].StyleList);
    }
  }
  return Status;
}

EFI_STATUS
GetVfcfPanelPropertyList (
  IN  H2O_PANEL_INFO                           *PanelInfo,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  OUT H2O_PROPERTY_INFO                        ***PropertyList,
  OUT UINTN                                    *PropertyCount
  )
{
  EFI_STATUS                                   Status;
  LAYOUT_LIB_STYLE_PRIORITY_INFO               PriorityInfo[4];
  UINTN                                        PriorityIndex;

  if (PanelInfo == NULL || PropertyList == NULL || PropertyCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (PriorityInfo, sizeof (PriorityInfo));

  StylePriorityInfoAddStyle (
    StyleType,
    PseudoClass,
    &PanelInfo->StyleListHead,
    sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO),
    PriorityInfo
    );

  Status = StylePriorityInfoGetPropertyList (
             sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO),
             PriorityInfo,
             PropertyCount,
             PropertyList
             );

  for (PriorityIndex = 0; PriorityIndex < (sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO)); PriorityIndex++) {
    if (PriorityInfo[PriorityIndex].StyleList != NULL) {
      FreePool (PriorityInfo[PriorityIndex].StyleList);
    }
  }
  return Status;
}


EFI_STATUS
GetVfcfFormsetPropertyList (
  IN  EFI_GUID                                 *FormsetGuid,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  OUT H2O_PROPERTY_INFO                        ***PropertyList,
  OUT UINTN                                    *PropertyCount
  )
{
  EFI_STATUS                                   Status;
  LAYOUT_LIB_STYLE_PRIORITY_INFO               PriorityInfo[4];
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *VfrLink;
  LIST_ENTRY                                   *FormsetListHead;
  LIST_ENTRY                                   *FormsetLink;
  H2O_VFR_INFO                                 *CurrentVfr;
  H2O_FORMSET_INFO                             *CurrentFormset;
  UINTN                                        PriorityIndex;

  if (FormsetGuid == NULL || PropertyList == NULL || PropertyCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || VfrListHead == NULL) {
    return EFI_NOT_FOUND;
  }

  ZeroMem (PriorityInfo, sizeof (PriorityInfo));

  for (VfrLink = VfrListHead->ForwardLink; !IsNull (VfrListHead, VfrLink); VfrLink = VfrLink->ForwardLink) {
    CurrentVfr = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);
    FormsetListHead = &CurrentVfr->FormsetListHead;

    for (FormsetLink = FormsetListHead->ForwardLink; !IsNull (FormsetListHead, FormsetLink); FormsetLink = FormsetLink->ForwardLink) {
      CurrentFormset = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);
      if (!CompareGuid (&CurrentFormset->FormsetId, FormsetGuid)) {
        continue;
      }

      StylePriorityInfoAddStyle (
        StyleType,
        PseudoClass,
        &CurrentFormset->StyleListHead,
        sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO),
        PriorityInfo
        );
    }
  }

  Status = StylePriorityInfoGetPropertyList (
             sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO),
             PriorityInfo,
             PropertyCount,
             PropertyList
             );

  for (PriorityIndex = 0; PriorityIndex < (sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO)); PriorityIndex++) {
    if (PriorityInfo[PriorityIndex].StyleList != NULL) {
      FreePool (PriorityInfo[PriorityIndex].StyleList);
    }
  }
  return Status;
}

EFI_STATUS
GetVfcfFormsetPropertyValue (
  IN  EFI_GUID                                 *FormsetGuid,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  IN  CHAR8                                    *IdentifierStr,
  IN  RECT                                     *RelativeField OPTIONAL,
  OUT H2O_PROPERTY_INFO                        **PropertyInfo,
  OUT H2O_PROPERTY_VALUE                       *PropertyValue
  )
{
  EFI_STATUS                                   Status;
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *VfrLink;
  H2O_VFR_INFO                                 *VfrInfo;
  LIST_ENTRY                                   *FormsetListHead;
  LIST_ENTRY                                   *FormsetLink;
  H2O_FORMSET_INFO                             *FormsetInfo;
  EFI_STATUS                                   ResultStatus;
  H2O_PROPERTY_INFO                            *FoundProperty;
  H2O_PROPERTY_VALUE                           FoundPropertyValue;

  if (FormsetGuid == NULL || IdentifierStr == NULL || PropertyInfo == NULL || PropertyValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || VfrListHead == NULL) {
    return EFI_NOT_FOUND;
  }

  ResultStatus = EFI_NOT_FOUND;
  for (VfrLink = GetFirstNode (VfrListHead); !IsNull (VfrListHead, VfrLink); VfrLink = GetNextNode (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);

    FormsetListHead = &VfrInfo->FormsetListHead;
    for (FormsetLink = GetFirstNode (FormsetListHead); !IsNull (FormsetListHead, FormsetLink); FormsetLink = GetNextNode (FormsetListHead, FormsetLink)) {
      FormsetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);
      if (!CompareGuid (&FormsetInfo->FormsetId, FormsetGuid)) {
        continue;
      }

      Status = GetPropertyFromStyleList (
                 &FormsetInfo->StyleListHead,
                 StyleType,
                 PseudoClass,
                 IdentifierStr,
                 RelativeField,
                 &FoundProperty,
                 &FoundPropertyValue
                 );
      if (!EFI_ERROR (Status)) {
        ResultStatus = EFI_SUCCESS;
        *PropertyInfo = FoundProperty;
        CopyMem (PropertyValue, &FoundPropertyValue, sizeof (H2O_PROPERTY_VALUE));
      }
    }
  }

  return ResultStatus;
}

EFI_STATUS
GetVfcfFormPropertyList (
  IN  EFI_GUID                                 *FormsetGuid,
  IN  UINT16                                   FormId,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  OUT H2O_PROPERTY_INFO                        ***PropertyList,
  OUT UINTN                                    *PropertyCount
  )
{
  EFI_STATUS                                   Status;
  LAYOUT_LIB_STYLE_PRIORITY_INFO               PriorityInfo[4];
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *VfrLink;
  LIST_ENTRY                                   *FormsetListHead;
  LIST_ENTRY                                   *FormsetLink;
  H2O_VFR_INFO                                 *CurrentVfr;
  H2O_FORMSET_INFO                             *CurrentFormset;
  LIST_ENTRY                                   *FormListHead;
  LIST_ENTRY                                   *FormLink;
  H2O_FORM_INFO                                *FormInfo;
  UINTN                                        PriorityIndex;

  if (FormsetGuid == NULL || PropertyList == NULL || PropertyCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || VfrListHead == NULL) {
    return EFI_NOT_FOUND;
  }

  ZeroMem (PriorityInfo, sizeof (PriorityInfo));

  for (VfrLink = VfrListHead->ForwardLink; !IsNull (VfrListHead, VfrLink); VfrLink = VfrLink->ForwardLink) {
    CurrentVfr = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);
    FormsetListHead = &CurrentVfr->FormsetListHead;

    for (FormsetLink = FormsetListHead->ForwardLink; !IsNull (FormsetListHead, FormsetLink); FormsetLink = FormsetLink->ForwardLink) {
      CurrentFormset = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);
      if (!CompareGuid (&CurrentFormset->FormsetId, FormsetGuid)) {
        continue;
      }

      FormListHead = &CurrentFormset->FormListHead;
      for (FormLink = GetFirstNode (FormListHead); !IsNull (FormListHead, FormLink); FormLink = GetNextNode (FormListHead, FormLink)) {
        FormInfo = H2O_FORM_INFO_NODE_FROM_LINK (FormLink);
        if (FormInfo->FormBlock->FormId != FormId) {
          continue;
        }

        StylePriorityInfoAddStyle (
          StyleType,
          PseudoClass,
          &FormInfo->StyleListHead,
          sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO),
          PriorityInfo
          );
      }
    }
  }

  Status = StylePriorityInfoGetPropertyList (
             sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO),
             PriorityInfo,
             PropertyCount,
             PropertyList
             );

  for (PriorityIndex = 0; PriorityIndex < (sizeof(PriorityInfo) / sizeof(LAYOUT_LIB_STYLE_PRIORITY_INFO)); PriorityIndex++) {
    if (PriorityInfo[PriorityIndex].StyleList != NULL) {
      FreePool (PriorityInfo[PriorityIndex].StyleList);
    }
  }
  return Status;
}

EFI_STATUS
GetVfcfFormPropertyValue (
  IN  EFI_GUID                                 *FormsetGuid,
  IN  UINT16                                   FormId,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  IN  CHAR8                                    *IdentifierStr,
  IN  RECT                                     *RelativeField OPTIONAL,
  OUT H2O_PROPERTY_INFO                        **PropertyInfo,
  OUT H2O_PROPERTY_VALUE                       *PropertyValue
  )
{
  EFI_STATUS                                   Status;
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *VfrLink;
  H2O_VFR_INFO                                 *VfrInfo;
  LIST_ENTRY                                   *FormsetListHead;
  LIST_ENTRY                                   *FormsetLink;
  H2O_FORMSET_INFO                             *FormsetInfo;
  LIST_ENTRY                                   *FormListHead;
  LIST_ENTRY                                   *FormLink;
  H2O_FORM_INFO                                *FormInfo;
  EFI_STATUS                                   ResultStatus;
  H2O_PROPERTY_INFO                            *FoundProperty;
  H2O_PROPERTY_VALUE                           FoundPropertyValue;

  if (FormsetGuid == NULL || IdentifierStr == NULL || PropertyInfo == NULL || PropertyValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || VfrListHead == NULL) {
    return EFI_NOT_FOUND;
  }

  ResultStatus = EFI_NOT_FOUND;
  for (VfrLink = GetFirstNode (VfrListHead); !IsNull (VfrListHead, VfrLink); VfrLink = GetNextNode (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);

    FormsetListHead = &VfrInfo->FormsetListHead;
    for (FormsetLink = GetFirstNode (FormsetListHead); !IsNull (FormsetListHead, FormsetLink); FormsetLink = GetNextNode (FormsetListHead, FormsetLink)) {
      FormsetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);
      if (!CompareGuid (&FormsetInfo->FormsetId, FormsetGuid)) {
        continue;
      }

      FormListHead = &FormsetInfo->FormListHead;
      for (FormLink = GetFirstNode (FormListHead); !IsNull (FormListHead, FormLink); FormLink = GetNextNode (FormListHead, FormLink)) {
        FormInfo = H2O_FORM_INFO_NODE_FROM_LINK (FormLink);
        if (FormInfo->FormBlock->FormId != FormId) {
          continue;
        }

        Status = GetPropertyFromStyleList (
                   &FormInfo->StyleListHead,
                   StyleType,
                   PseudoClass,
                   IdentifierStr,
                   RelativeField,
                   &FoundProperty,
                   &FoundPropertyValue
                   );
        if (!EFI_ERROR (Status)) {
          ResultStatus = EFI_SUCCESS;
          *PropertyInfo = FoundProperty;
          CopyMem (PropertyValue, &FoundPropertyValue, sizeof (H2O_PROPERTY_VALUE));
        }
      }
    }
  }

  return ResultStatus;
}

EFI_STATUS
GetVfcfStatementPropertyValue (
  IN  EFI_GUID                                 *FormsetGuid,
  IN  BOOLEAN                                  IsQuestion,
  IN  UINT16                                   Id,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  IN  CHAR8                                    *IdentifierStr,
  IN  RECT                                     *RelativeField OPTIONAL,
  OUT H2O_PROPERTY_INFO                        **PropertyInfo,
  OUT H2O_PROPERTY_VALUE                       *PropertyValue
  )
{
  EFI_STATUS                                   Status;
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *VfrLink;
  H2O_VFR_INFO                                 *VfrInfo;
  LIST_ENTRY                                   *FormsetListHead;
  LIST_ENTRY                                   *FormsetLink;
  H2O_FORMSET_INFO                             *FormsetInfo;
  LIST_ENTRY                                   *FormListHead;
  LIST_ENTRY                                   *FormLink;
  H2O_FORM_INFO                                *FormInfo;
  LIST_ENTRY                                   *StatementListHead;
  LIST_ENTRY                                   *StatementLink;
  H2O_STATEMENT_INFO                           *StatementInfo;
  EFI_STATUS                                   ResultStatus;
  H2O_PROPERTY_INFO                            *FoundProperty;
  H2O_PROPERTY_VALUE                           FoundPropertyValue;

  if (FormsetGuid == NULL || IdentifierStr == NULL || PropertyInfo == NULL || PropertyValue == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || VfrListHead == NULL) {
    return EFI_NOT_FOUND;
  }

  ResultStatus = EFI_NOT_FOUND;
  for (VfrLink = GetFirstNode (VfrListHead); !IsNull (VfrListHead, VfrLink); VfrLink = GetNextNode (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);

    FormsetListHead = &VfrInfo->FormsetListHead;
    for (FormsetLink = GetFirstNode (FormsetListHead); !IsNull (FormsetListHead, FormsetLink); FormsetLink = GetNextNode (FormsetListHead, FormsetLink)) {
      FormsetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);
      if (!CompareGuid (&FormsetInfo->FormsetId, FormsetGuid)) {
        continue;
      }

      FormListHead = &FormsetInfo->FormListHead;
      for (FormLink = GetFirstNode (FormListHead); !IsNull (FormListHead, FormLink); FormLink = GetNextNode (FormListHead, FormLink)) {
        FormInfo = H2O_FORM_INFO_NODE_FROM_LINK (FormLink);

        StatementListHead = &FormInfo->StatementListHead;
        for (StatementLink = GetFirstNode (StatementListHead); !IsNull (StatementListHead, StatementLink); StatementLink = GetNextNode (StatementListHead, StatementLink)) {
          StatementInfo = H2O_STATEMENT_INFO_NODE_FROM_LINK (StatementLink);
          if (StatementInfo->IsQuestion != IsQuestion ||
              StatementInfo->QuestionId != Id) {
            continue;
          }

          Status = GetPropertyFromStyleList (
                     &StatementInfo->StyleListHead,
                     StyleType,
                     PseudoClass,
                     IdentifierStr,
                     RelativeField,
                     &FoundProperty,
                     &FoundPropertyValue
                     );
          if (!EFI_ERROR (Status)) {
            ResultStatus = EFI_SUCCESS;
            *PropertyInfo = FoundProperty;
            CopyMem (PropertyValue, &FoundPropertyValue, sizeof (H2O_PROPERTY_VALUE));
          }
        }
      }
    }
  }

  return ResultStatus;
}

BOOLEAN
HaveVfcfFormsetInfo (
  IN EFI_GUID                                  *FormsetGuid
  )
{
  EFI_STATUS                                   Status;
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *VfrLink;
  H2O_VFR_INFO                                 *VfrInfo;
  LIST_ENTRY                                   *FormsetListHead;
  LIST_ENTRY                                   *FormsetLink;
  H2O_FORMSET_INFO                             *FormsetInfo;

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || VfrListHead == NULL || IsListEmpty (VfrListHead)) {
    return FALSE;
  }

  for (VfrLink = GetFirstNode (VfrListHead); !IsNull (VfrListHead, VfrLink); VfrLink = GetNextNode (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);

    FormsetListHead = &VfrInfo->FormsetListHead;
    for (FormsetLink = GetFirstNode (FormsetListHead); !IsNull (FormsetListHead, FormsetLink); FormsetLink = GetNextNode (FormsetListHead, FormsetLink)) {
      FormsetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);
      if (CompareGuid (&FormsetInfo->FormsetId, FormsetGuid)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

BOOLEAN
HaveVfcfFormInfo (
  IN EFI_GUID                                  *FormsetGuid,
  IN UINTN                                     FormId
  )
{
  EFI_STATUS                                   Status;
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *VfrLink;
  H2O_VFR_INFO                                 *VfrInfo;
  LIST_ENTRY                                   *FormsetListHead;
  LIST_ENTRY                                   *FormsetLink;
  H2O_FORMSET_INFO                             *FormsetInfo;
  LIST_ENTRY                                   *FormListHead;
  LIST_ENTRY                                   *FormLink;
  H2O_FORM_INFO                                *FormInfo;

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || VfrListHead == NULL || IsListEmpty (VfrListHead)) {
    return FALSE;
  }

  for (VfrLink = GetFirstNode (VfrListHead); !IsNull (VfrListHead, VfrLink); VfrLink = GetNextNode (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);

    FormsetListHead = &VfrInfo->FormsetListHead;
    for (FormsetLink = GetFirstNode (FormsetListHead); !IsNull (FormsetListHead, FormsetLink); FormsetLink = GetNextNode (FormsetListHead, FormsetLink)) {
      FormsetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);
      if (!CompareGuid (&FormsetInfo->FormsetId, FormsetGuid)) {
        continue;
      }
      FormListHead = &FormsetInfo->FormListHead;
      for (FormLink = GetFirstNode (FormListHead); !IsNull (FormListHead, FormLink); FormLink = GetNextNode (FormListHead, FormLink)) {
        FormInfo = H2O_FORM_INFO_NODE_FROM_LINK (FormLink);
        if (FormInfo->FormBlock->FormId == FormId) {
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}



EFI_STATUS
GetVfcfStatementExpressionList (
  IN  EFI_GUID                                 *FormsetGuid,
  IN  UINTN                                    FormId,
  IN  BOOLEAN                                  IsQuestion,
  IN  UINT16                                   Id,
  IN  UINT16                                   Position,
  IN OUT UINTN                                 *NumberExpression,
  OUT H2O_EXPRESSION_INFO                      ***ExpressionInfoList
  )
{
  EFI_STATUS                                   Status;
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *VfrLink;
  H2O_VFR_INFO                                 *VfrInfo;
  LIST_ENTRY                                   *FormsetListHead;
  LIST_ENTRY                                   *FormsetLink;
  H2O_FORMSET_INFO                             *FormsetInfo;
  LIST_ENTRY                                   *FormListHead;
  LIST_ENTRY                                   *FormLink;
  H2O_FORM_INFO                                *FormInfo;
  LIST_ENTRY                                   *StatementListHead;
  LIST_ENTRY                                   *StatementLink;
  H2O_STATEMENT_INFO                           *StatementInfo;
  LIST_ENTRY                                   *ExpressionListHead;
  LIST_ENTRY                                   *ExpressionLink;
  H2O_EXPRESSION_INFO                          *ExpressionInfo;


  UINTN                                        ResultNumber;
  H2O_EXPRESSION_INFO                          **ResultBuffer;


  ASSERT (FormsetGuid != NULL);
  ASSERT (NumberExpression != NULL);
  ASSERT (ExpressionInfoList != NULL);

  if (FormsetGuid == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (NumberExpression == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (ExpressionInfoList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *NumberExpression = 0;
  *ExpressionInfoList   = NULL;

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status) || VfrListHead == NULL) {
    return EFI_NOT_FOUND;
  }

  ResultNumber = 0;
  ResultBuffer = NULL;

  for (VfrLink = GetFirstNode (VfrListHead); !IsNull (VfrListHead, VfrLink); VfrLink = GetNextNode (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);

    FormsetListHead = &VfrInfo->FormsetListHead;
    for (FormsetLink = GetFirstNode (FormsetListHead); !IsNull (FormsetListHead, FormsetLink); FormsetLink = GetNextNode (FormsetListHead, FormsetLink)) {
      FormsetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);
      if (!CompareGuid (&FormsetInfo->FormsetId, FormsetGuid)) {
        continue;
      }

      FormListHead = &FormsetInfo->FormListHead;
      for (FormLink = GetFirstNode (FormListHead); !IsNull (FormListHead, FormLink); FormLink = GetNextNode (FormListHead, FormLink)) {
        FormInfo = H2O_FORM_INFO_NODE_FROM_LINK (FormLink);
        if (FormInfo->FormBlock->FormId != FormId) {
          continue;
        }

        StatementListHead = &FormInfo->StatementListHead;
        for (StatementLink = GetFirstNode (StatementListHead); !IsNull (StatementListHead, StatementLink); StatementLink = GetNextNode (StatementListHead, StatementLink)) {
          StatementInfo = H2O_STATEMENT_INFO_NODE_FROM_LINK (StatementLink);

          if (StatementInfo->ExpressionCount == 0) {
            continue;
          }

          if (StatementInfo->IsQuestion) {
            if (!IsQuestion) {
              continue;
            }
            if (StatementInfo->QuestionId != Id) {
              continue;
            }
          } else {
            if (StatementInfo->QuestionId != Position) {
              continue;
            }
          }

          ResultBuffer = ReallocatePool (
                           ResultNumber * sizeof (H2O_EXPRESSION_INFO *),
                           (ResultNumber + StatementInfo->ExpressionCount) * sizeof (H2O_EXPRESSION_INFO *),
                           ResultBuffer
                           );
          if (ResultBuffer == NULL) {
            return EFI_OUT_OF_RESOURCES;
          }

          ExpressionListHead = &StatementInfo->ExpressionListHead;
          for (ExpressionLink = GetFirstNode(ExpressionListHead); !IsNull (ExpressionListHead, ExpressionLink); ExpressionLink = GetNextNode (ExpressionListHead, ExpressionLink)) {
            ExpressionInfo = H2O_EXPRESSION_INFO_NODE_FROM_LINK (ExpressionLink);
            ResultBuffer[ResultNumber++] = ExpressionInfo;
          }
        }
      }
    }
  }

  if (ResultNumber == 0) {
    return EFI_NOT_FOUND;
  }

  *NumberExpression = ResultNumber;
  *ExpressionInfoList   = ResultBuffer;

  return EFI_SUCCESS;
}

/**
  Get FormSet Expression QuestionId map list on the input Formset

  @param  FormSet                Pointer of the current FormSet.
  @param  NumberExpressionQIdMap The number of expresion question id map returned in Buffer.
  @param  ExpressionQIdMapList   A pointer to the buffer to return the requested
                                 array of Expression QuestionId map

  @retval EFI_SUCCESS            The result array of Expression QuestionId map was returned.
  @retval EFI_NOT_FOUND          No Expression QuestionId map match the search.
  @retval EFI_OUT_OF_RESOURCES   There is not enough pool memory to store the
                                 matching results.

**/

EFI_STATUS
GetVfcfStatementExpressionQIdMapList (
  IN  EFI_GUID                                 *FormSetGuid,
  IN OUT UINTN                                 *NumberExpressionQIdMap,
  OUT H2O_HII_LIBT_EXPRESSION_QID_MAP_BLOCK    ***ExpressionQIdMapList
  )
{
  EFI_STATUS                                   Status;
  LIST_ENTRY                                   *LayoutListHead;
  LIST_ENTRY                                   *VfrListHead;
  LIST_ENTRY                                   *VfrLink;
  H2O_VFR_INFO                                 *VfrInfo;
  LIST_ENTRY                                   *FormSetListHead;
  LIST_ENTRY                                   *FormSetLink;
  H2O_FORMSET_INFO                             *FormSetInfo;
  LIST_ENTRY                                   *ExpressionQIdMapListHead;
  LIST_ENTRY                                   *ExpressionQIdMapLink;
  H2O_EXPRESSION_QID_MAP_INFO                  *ExpressionQIdMapInfo;

  UINTN                                        ResultNumber;
  H2O_HII_LIBT_EXPRESSION_QID_MAP_BLOCK        **ResultBuffer;

  UINTN                                        Index;

  ASSERT (FormSetGuid != NULL);
  ASSERT (NumberExpressionQIdMap != NULL);
  ASSERT (ExpressionQIdMapList != NULL);

  if (FormSetGuid == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (NumberExpressionQIdMap == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (ExpressionQIdMapList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *NumberExpressionQIdMap = 0;
  *ExpressionQIdMapList   = NULL;

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status) || VfrListHead == NULL) {
    return EFI_NOT_FOUND;
  }

  ResultNumber = 0;
  ResultBuffer = NULL;

  for (VfrLink = GetFirstNode (VfrListHead); !IsNull (VfrListHead, VfrLink); VfrLink = GetNextNode (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);

    FormSetListHead = &VfrInfo->FormsetListHead;
    for (FormSetLink = GetFirstNode (FormSetListHead); !IsNull (FormSetListHead, FormSetLink); FormSetLink = GetNextNode (FormSetListHead, FormSetLink)) {
      FormSetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormSetLink);
      if (!CompareGuid (&FormSetInfo->FormsetId, FormSetGuid)) {
        continue;
      }

      ExpressionQIdMapListHead = &FormSetInfo->ExpressionQIdMapList;
      for (ExpressionQIdMapLink = GetFirstNode (ExpressionQIdMapListHead)
        ; !IsNull (ExpressionQIdMapListHead, ExpressionQIdMapLink)
        ; ExpressionQIdMapLink = GetNextNode (ExpressionQIdMapListHead, ExpressionQIdMapLink)
        ) {
        ResultNumber++;
      }
    }
  }

  if (ResultNumber == 0) {
    return EFI_NOT_FOUND;
  }

  ResultBuffer = AllocateZeroPool (sizeof (H2O_HII_LIBT_EXPRESSION_QID_MAP_BLOCK *) * ResultNumber);
  if (ResultBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Index = 0;
  for (VfrLink = GetFirstNode (VfrListHead); !IsNull (VfrListHead, VfrLink); VfrLink = GetNextNode (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);

    FormSetListHead = &VfrInfo->FormsetListHead;
    for ( FormSetLink = GetFirstNode (FormSetListHead)
        ; !IsNull (FormSetListHead, FormSetLink)
        ; FormSetLink = GetNextNode (FormSetListHead, FormSetLink)
        ) {

      FormSetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormSetLink);
      if (!CompareGuid (&FormSetInfo->FormsetId, FormSetGuid)) {
        continue;
      }

      ExpressionQIdMapListHead = &FormSetInfo->ExpressionQIdMapList;
      for (ExpressionQIdMapLink = GetFirstNode (ExpressionQIdMapListHead)
        ; !IsNull (ExpressionQIdMapListHead, ExpressionQIdMapLink)
        ; ExpressionQIdMapLink = GetNextNode (ExpressionQIdMapListHead, ExpressionQIdMapLink)
        ) {

        ExpressionQIdMapInfo = H2O_EXPRESSION_QID_MAP_INFO_NODE_FROM_LINK (ExpressionQIdMapLink);
        ResultBuffer[Index++] = ExpressionQIdMapInfo->ExpressionQIdMapBlock;
      }
    }
  }

  *NumberExpressionQIdMap = ResultNumber;
  *ExpressionQIdMapList   = ResultBuffer;

  return EFI_SUCCESS;
}
