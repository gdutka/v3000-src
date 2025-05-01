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

STATIC
UINT32
InitLayoutRevision (
  IN UINT8                                     *BlockDataBuffer,
  IN UINT32                                    BlockDataBufferLength,
  IN UINT32                                    CurrentBlockOffset
  )
{
  UINT32                                       BlockOffset;
  UINT8                                        *BlockData;
  UINT8                                        BlockType;
  UINT8                                        BlockSize;
  CHAR8                                        *IdentifierStr;
  UINT32                                       ValueType;

  if (BlockDataBuffer == NULL || BlockDataBufferLength == 0 || CurrentBlockOffset == 0) {
    return LAYOUT_REVISION_UNKNOWN;
  }

  //
  // Parse each layout block
  //
  BlockOffset = CurrentBlockOffset;
  while (BlockOffset < BlockDataBufferLength) {
    BlockData = BlockDataBuffer + BlockOffset;

    BlockType = ((H2O_HII_LAYOUT_BLOCK *) BlockData)->BlockType;
    BlockSize = ((H2O_HII_LAYOUT_BLOCK *) BlockData)->BlockSize;

    if (BlockSize == 0) {
      break;
    }

    BlockOffset += BlockSize;

    switch (BlockType) {

    case H2O_HII_LIBT_LAYOUT_BEGIN:
    case H2O_HII_LIBT_LAYOUT_END:
    case H2O_HII_LIBT_LAYOUT_DUP:
      return LAYOUT_REVISION_2;

    case H2O_HII_LIBT_PROPERTY:
      IdentifierStr = (CHAR8 *)BlockData + ((H2O_HII_LIBT_PROPERTY_BLOCK *)BlockData)->IdentifierOffset;
      ValueType = ((H2O_HII_LIBT_PROPERTY_BLOCK *)BlockData)->ValueType;
      if ((AsciiStrCmp (IdentifierStr, "layout") == 0) ||
        (AsciiStrCmp (IdentifierStr, "foreground-color") == 0) ||
        (AsciiStrCmp (IdentifierStr, "visibility") == 0 && ValueType == H2O_PROPERTY_VALUE_TYPE_BOOLEAN)) {
        return LAYOUT_REVISION_1;
      }
      break;

    default:
      break;
    }
  }

  return LAYOUT_REVISION_2;
}

STATIC
UINT32
ParsePseudoClass (
  IN CONST CHAR8                                 *PseudoClassStr
  )
{
  if (AsciiStrCmp (PseudoClassStr, ":selectable") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_SELECTABLE;
  } else if (AsciiStrCmp (PseudoClassStr, ":grayout") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_GRAYOUT;
  } else if (AsciiStrCmp (PseudoClassStr, ":highlight") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_HIGHLIGHT;
  } else if (AsciiStrCmp (PseudoClassStr, ":link") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_LINK;
  } else if (AsciiStrCmp (PseudoClassStr, ":visited") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_VISITED;
  } else if (AsciiStrCmp (PseudoClassStr, ":hover") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_HOVER;
  } else if (AsciiStrCmp (PseudoClassStr, ":focus") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_FOCUS;
  } else if (AsciiStrCmp (PseudoClassStr, ":active") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_ACTIVE;
  } else if (AsciiStrCmp (PseudoClassStr, ":enabled") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_ENABLED;
  } else if (AsciiStrCmp (PseudoClassStr, ":disabled") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_DISABLED;
  } else if (AsciiStrCmp (PseudoClassStr, ":not") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_NOT;
  } else if (AsciiStrCmp (PseudoClassStr, ":help") == 0) {
    return H2O_STYLE_PSEUDO_CLASS_HELP;
  } else {
    //
    // default
    //
    return H2O_STYLE_PSEUDO_CLASS_NORMAL;
  }
}

STATIC
VOID
CountLayoutOpCodes (
  IN  UINT8                                    *BlockDataBuffer,
  IN  UINT32                                   BlockDataBufferLength,
  OUT UINT32                                   *NumberOfPanel,
  OUT UINT32                                   *NumberOfStyle,
  OUT UINT32                                   *NumberOfProperty,
  OUT UINT32                                   *NumberOfHotKey
  )
{
  UINT32                                       BlockOffset;
  UINT8                                        *BlockData;
  UINT8                                        BlockType;
  UINT8                                        BlockSize;
  UINT32                                       PanelNum;
  UINT32                                       StyleNum;
  UINT32                                       PropertyNum;
  UINT32                                       HotKeyNum;

  PanelNum    = 0;
  StyleNum    = 0;
  PropertyNum = 0;
  HotKeyNum   = 0;

  BlockOffset = 0;
  while (BlockOffset < BlockDataBufferLength) {
    BlockData = BlockDataBuffer + BlockOffset;

    BlockType = ((H2O_HII_LAYOUT_BLOCK *) BlockData)->BlockType;
    BlockSize = ((H2O_HII_LAYOUT_BLOCK *) BlockData)->BlockSize;
    if (BlockSize == 0) {
      break;
    }

    BlockOffset += BlockSize;

    switch (BlockType) {

    case H2O_HII_LIBT_PANEL_BEGIN:
      PanelNum++;
      break;

    case H2O_HII_LIBT_STYLE_BEGIN:
      StyleNum++;
      break;

    case H2O_HII_LIBT_PROPERTY:
      StyleNum++;
      PropertyNum++;
      break;

    case H2O_HII_LIBT_EXT_HOTKEY:
      HotKeyNum++;
      break;
    }
  }

  *NumberOfPanel    = PanelNum;
  *NumberOfStyle    = StyleNum;
  *NumberOfProperty = PropertyNum;
  *NumberOfHotKey   = HotKeyNum;
}

STATIC
EFI_STATUS
ParseLayoutPkg (
  IN     UINT8                                 *PkgBuffer,
  OUT    LIST_ENTRY                            **LayoutListHead,
  OUT    LIST_ENTRY                            **VfrListHead
  )
{
  EFI_STATUS                                   Status;

  H2O_LAYOUT_PACKAGE_HDR                       *CurrentLayoutPkg;
  H2O_LAYOUT_INFO                              *CurrentLayout;
  H2O_PANEL_INFO                               *CurrentPanel;
  H2O_STYLE_INFO                               *CurrentStyle;
  H2O_PROPERTY_INFO                            *CurrentProperty;

  H2O_VFR_INFO                                 *CurrentVfr;
  H2O_FORMSET_INFO                             *CurrentFormset;
  H2O_FORM_INFO                                *CurrentForm;
  H2O_HOTKEY_INFO                              *CurrentHotkey;
  H2O_IMPORT_INFO                              *CurrentImport;
  H2O_TARGET_INFO                              *CurrentTargetInfo;
  H2O_STATEMENT_INFO                           *CurrentStatement;
  H2O_EXPRESSION_INFO                          *CurrentExpression;
  H2O_EXPRESSION_QID_MAP_INFO                  *CurrentExpressionQidMap;

  UINT8                                        *BlockDataBuffer;
  UINT32                                       BlockDataBufferLength;
  UINT8                                        *BlockData;
  UINT8                                        BlockType;
  UINT8                                        BlockSize;
  UINT32                                       BlockOffset;
  UINTN                                        ExpressionSize;

  H2O_LAYOUT_ID                                OldLayoutId;
  H2O_PANEL_ID                                 OldPanelId;
  LIST_ENTRY                                   *Link;
  LIST_ENTRY                                   *LinkList;
  H2O_LAYOUT_INFO                              *TempLayout;
  H2O_PANEL_INFO                               *TempPanel;
  H2O_STYLE_INFO                               *TempStyle;
  H2O_STYLE_INFO                               NewStyleInfo;
  H2O_PROPERTY_INFO                            NewProperty;
  H2O_PROPERTY_INFO                            *TempProperty;

  H2O_HII_LIBT_EXT_IMPORT_BLOCK                *ImportBlock;
  H2O_HII_LIBT_IMPORT_TARGET_BEGIN_BLOCK       *ImportTargetBlock;
  H2O_HII_LIBT_LINK_TARGET_BEGIN_BLOCK         *LinkTargetBlock;
  H2O_HII_LIBT_LAYOUT_BEGIN_BLOCK              *LayoutBeginBlock;
  H2O_HII_LIBT_EXT_STATEMENT_BLOCK             *StatementBlock;

  UINT32                                       PanelIndex;
  UINT32                                       PanelCount;
  H2O_PANEL_INFO                               *PanelBuffer;
  UINT32                                       StyleIndex;
  UINT32                                       StyleCount;
  H2O_STYLE_INFO                               *StyleBuffer;
  UINT32                                       PropertyIndex;
  UINT32                                       PropertyCount;
  H2O_PROPERTY_INFO                            *PropertyBuffer;
  UINT32                                       HotKeyIndex;
  UINT32                                       HotKeyCount;
  H2O_HOTKEY_INFO                              *HotKeyBuffer;

  if (PkgBuffer == NULL || LayoutListHead == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_SUCCESS;

  CurrentLayoutPkg = NULL;
  CurrentLayout    = NULL;
  CurrentPanel     = NULL;
  CurrentStyle     = NULL;
  CurrentProperty  = NULL;
  CurrentVfr       = NULL;
  CurrentFormset   = NULL;
  CurrentForm      = NULL;
  CurrentHotkey    = NULL;
  CurrentStatement = NULL;
  CurrentTargetInfo = NULL;

  TempLayout = NULL;
  TempPanel  = NULL;

  //
  // Layout Pkg
  //
  CurrentLayoutPkg = (H2O_LAYOUT_PACKAGE_HDR *) (PkgBuffer + sizeof(UINT32));
  if (CurrentLayoutPkg->Header.Type != H2O_HII_PACKAGE_LAYOUTS) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Block Data Buffer
  //
  BlockDataBuffer       = (UINT8 *)(CurrentLayoutPkg + 1);
  BlockDataBufferLength = CurrentLayoutPkg->Header.Length - CurrentLayoutPkg->HdrSize;
  CountLayoutOpCodes (BlockDataBuffer, BlockDataBufferLength, &PanelCount, &StyleCount, &PropertyCount, &HotKeyCount);
  PanelBuffer    = AllocateZeroPool (sizeof (H2O_PANEL_INFO)    * PanelCount);
  StyleBuffer    = AllocateZeroPool (sizeof (H2O_STYLE_INFO)    * StyleCount);
  PropertyBuffer = AllocateZeroPool (sizeof (H2O_PROPERTY_INFO) * PropertyCount);
  HotKeyBuffer   = AllocateZeroPool (sizeof (H2O_HOTKEY_INFO)   * HotKeyCount);
  if ((PanelBuffer    == NULL && PanelCount    != 0) ||
      (StyleBuffer    == NULL && StyleCount    != 0) ||
      (PropertyBuffer == NULL && PropertyCount != 0) ||
      (HotKeyBuffer   == NULL && HotKeyCount   != 0)) {
    return EFI_OUT_OF_RESOURCES;
  }
  PanelIndex    = 0;
  StyleIndex    = 0;
  PropertyIndex = 0;
  HotKeyIndex   = 0;

  //
  // Init Layout List
  //
  *LayoutListHead = AllocateZeroPool (sizeof (LIST_ENTRY));
  if (*LayoutListHead == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  InitializeListHead (*LayoutListHead);

  //
  // Init Vfr List
  //
  *VfrListHead = AllocateZeroPool (sizeof (LIST_ENTRY));
  if (*VfrListHead == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  InitializeListHead (*VfrListHead);

  //
  // Parse each layout block
  //
  BlockOffset = 0;
  while (BlockOffset < BlockDataBufferLength) {
    BlockData = BlockDataBuffer + BlockOffset;

    BlockType = ((H2O_HII_LAYOUT_BLOCK *) BlockData)->BlockType;
    BlockSize = ((H2O_HII_LAYOUT_BLOCK *) BlockData)->BlockSize;

    if (BlockSize == 0) {
      break;
    }

    BlockOffset += BlockSize;

    switch (BlockType) {

    case H2O_HII_LIBT_LAYOUT_BEGIN:
      LayoutBeginBlock = (H2O_HII_LIBT_LAYOUT_BEGIN_BLOCK *)BlockData;

      LinkList = *LayoutListHead;
      for (Link = GetFirstNode (LinkList), CurrentLayout = NULL; !IsNull (LinkList, Link); Link = GetNextNode (LinkList, Link)) {
        TempLayout = H2O_LAYOUT_INFO_NODE_FROM_LINK (Link);
        if (TempLayout->LayoutId == LayoutBeginBlock->LayoutId &&
            CompareGuid (&TempLayout->DisplayEngineGuid[0], &LayoutBeginBlock->DisplayEngineGuid[0]) &&
            CompareGuid (&TempLayout->DisplayEngineGuid[1], &LayoutBeginBlock->DisplayEngineGuid[1]) &&
            CompareGuid (&TempLayout->DisplayEngineGuid[2], &LayoutBeginBlock->DisplayEngineGuid[2])) {
          CurrentLayout = TempLayout;
          break;
        }
      }
      if (CurrentLayout != NULL) {
        break;
      }

      CurrentLayout = AllocateZeroPool (sizeof (H2O_LAYOUT_INFO));
      if (CurrentLayout == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CurrentLayout->Signature = H2O_LAYOUT_INFO_NODE_SIGNATURE;
      CurrentLayout->LayoutId = LayoutBeginBlock->LayoutId;
      CopyGuid (&CurrentLayout->DisplayEngineGuid[0], &LayoutBeginBlock->DisplayEngineGuid[0]);
      CopyGuid (&CurrentLayout->DisplayEngineGuid[1], &LayoutBeginBlock->DisplayEngineGuid[1]);
      CopyGuid (&CurrentLayout->DisplayEngineGuid[2], &LayoutBeginBlock->DisplayEngineGuid[2]);
      CurrentLayout->Revision = InitLayoutRevision (BlockDataBuffer, BlockDataBufferLength, BlockOffset);
      InitializeListHead (&CurrentLayout->PanelListHead);
      InitializeListHead (&CurrentLayout->HotkeyListHead);
      InitializeListHead (&CurrentLayout->StyleListHead);
      InsertTailList (*LayoutListHead, &CurrentLayout->Link);
      break;

    case H2O_HII_LIBT_LAYOUT_END:
      CurrentLayout = NULL;
      CurrentPanel  = NULL;
      CurrentHotkey = NULL;
      CurrentStyle  = NULL;
      break;

    case H2O_HII_LIBT_PANEL_BEGIN:
      if (CurrentLayout == NULL || PanelBuffer == NULL) {
        ASSERT (FALSE);
        CurrentPanel = NULL;
        break;
      }

      LinkList = &CurrentLayout->PanelListHead;
      for (Link = GetFirstNode (LinkList), CurrentPanel = NULL; !IsNull (LinkList, Link); Link = GetNextNode (LinkList, Link)) {
        TempPanel = H2O_PANEL_INFO_NODE_FROM_LINK (Link);
        if (TempPanel->PanelId   == ((H2O_HII_LIBT_PANEL_BEGIN_BLOCK *)BlockData)->PanelId   &&
            TempPanel->PanelType == ((H2O_HII_LIBT_PANEL_BEGIN_BLOCK *)BlockData)->PanelType &&
            CompareGuid  (&TempPanel->PanelTypeGuid, &((H2O_HII_LIBT_PANEL_BEGIN_BLOCK *)BlockData)->PanelTypeGuid)) {
          CurrentPanel = TempPanel;
          break;
        }
      }
      if (CurrentPanel != NULL) {
        break;
      }

      CurrentPanel = &PanelBuffer[PanelIndex++];
      CurrentPanel->Signature = H2O_PANEL_INFO_NODE_SIGNATURE;
      CurrentPanel->PanelId = ((H2O_HII_LIBT_PANEL_BEGIN_BLOCK *)BlockData)->PanelId;
      CurrentPanel->PanelType = ((H2O_HII_LIBT_PANEL_BEGIN_BLOCK *)BlockData)->PanelType;
      CopyMem (&CurrentPanel->PanelTypeGuid, &((H2O_HII_LIBT_PANEL_BEGIN_BLOCK *)BlockData)->PanelTypeGuid, sizeof (EFI_GUID));
      CopyMem (&CurrentPanel->TargetFormsetGuid, &((H2O_HII_LIBT_PANEL_BEGIN_BLOCK *)BlockData)->TargetFormsetGuid, sizeof (EFI_GUID));
      CurrentPanel->TargetFormId = ((H2O_HII_LIBT_PANEL_BEGIN_BLOCK *)BlockData)->TargetFormId;
      CurrentPanel->ParentLayout = CurrentLayout;
      InitializeListHead (&CurrentPanel->StyleListHead);
      InsertTailList (&CurrentLayout->PanelListHead, &CurrentPanel->Link);
      break;

    case H2O_HII_LIBT_PANEL_END:
      CurrentPanel = NULL;
      CurrentStyle = NULL;
      break;

    case H2O_HII_LIBT_STYLE_BEGIN:
      if (CurrentHotkey != NULL) {
        LinkList = &CurrentHotkey->StyleListHead;
      } else if (CurrentStatement != NULL) {
        LinkList = &CurrentStatement->StyleListHead;
      } else if (CurrentTargetInfo != NULL) {
        LinkList = &CurrentTargetInfo->StyleListHead;
      } else if (CurrentForm != NULL) {
        LinkList = &CurrentForm->StyleListHead;
      } else if (CurrentFormset != NULL) {
        LinkList = &CurrentFormset->StyleListHead;
      } else if (CurrentPanel != NULL) {
        LinkList = &CurrentPanel->StyleListHead;
      } else if (CurrentLayout != NULL) {
        LinkList = &CurrentLayout->StyleListHead;
      } else {
        LinkList = NULL;
      }
      if (LinkList == NULL || StyleBuffer == NULL) {
        ASSERT (FALSE);
        CurrentStyle = NULL;
        break;
      }

      NewStyleInfo.StyleType   = ((H2O_HII_LIBT_STYLE_BEGIN_BLOCK *)BlockData)->Type;
      NewStyleInfo.ClassName   = (CHAR8 *)BlockData + ((H2O_HII_LIBT_STYLE_BEGIN_BLOCK *)BlockData)->ClassNameOffset;
      NewStyleInfo.PseudoClass = ParsePseudoClass ((CHAR8 *)BlockData + ((H2O_HII_LIBT_STYLE_BEGIN_BLOCK *)BlockData)->PseudoClassOffset);

      for (Link = GetFirstNode (LinkList), CurrentStyle = NULL; !IsNull (LinkList, Link); Link = GetNextNode (LinkList, Link)) {
        TempStyle = H2O_STYLE_INFO_NODE_FROM_LINK (Link);
        if (TempStyle->StyleType   == NewStyleInfo.StyleType &&
            TempStyle->PseudoClass == NewStyleInfo.PseudoClass &&
            AsciiStrCmp (TempStyle->ClassName, NewStyleInfo.ClassName) == 0) {
          CurrentStyle = TempStyle;
          break;
        }
      }
      if (CurrentStyle != NULL) {
        break;
      }

      CurrentStyle = &StyleBuffer[StyleIndex++];
      CurrentStyle->Signature   = H2O_STYLE_INFO_NODE_SIGNATURE;
      CurrentStyle->StyleType   = NewStyleInfo.StyleType;
      CurrentStyle->ClassName   = NewStyleInfo.ClassName;
      CurrentStyle->PseudoClass = NewStyleInfo.PseudoClass;
      InitializeListHead (&CurrentStyle->PropertyListHead);
      InsertTailList (LinkList, &CurrentStyle->Link);
      break;

    case H2O_HII_LIBT_STYLE_END:
      CurrentStyle = NULL;
      break;

    case H2O_HII_LIBT_PROPERTY:
      if (PropertyBuffer == NULL) {
        ASSERT (FALSE);
        CurrentProperty = NULL;
        break;
      }
      if (CurrentStyle == NULL) {
        if (StyleBuffer == NULL) {
          break;
        }

        NewStyleInfo.ClassName   = NULL;
        NewStyleInfo.PseudoClass = H2O_STYLE_PSEUDO_CLASS_NORMAL;
        if (CurrentHotkey != NULL) {
          LinkList = &CurrentHotkey->StyleListHead;
          NewStyleInfo.StyleType = H2O_IFR_STYLE_TYPE_VFR_TREE_FLAG | H2O_IFR_STYLE_TYPE_HOTKEY;
        } else if (CurrentStatement != NULL) {
          LinkList = &CurrentStatement->StyleListHead;
          NewStyleInfo.StyleType = H2O_IFR_STYLE_TYPE_VFR_TREE_FLAG;
        } else if (CurrentTargetInfo != NULL) {
          LinkList = &CurrentTargetInfo->StyleListHead;
          NewStyleInfo.StyleType = H2O_IFR_STYLE_TYPE_VFR_TREE_FLAG;
        } else if (CurrentForm != NULL) {
          LinkList = &CurrentForm->StyleListHead;
          NewStyleInfo.StyleType = H2O_IFR_STYLE_TYPE_VFR_TREE_FLAG | H2O_IFR_STYLE_TYPE_FORM;
        } else if (CurrentFormset != NULL) {
          LinkList = &CurrentFormset->StyleListHead;
          NewStyleInfo.StyleType = H2O_IFR_STYLE_TYPE_VFR_TREE_FLAG | H2O_IFR_STYLE_TYPE_FORMSET;
        } else if (CurrentPanel != NULL) {
          LinkList = &CurrentPanel->StyleListHead;
          NewStyleInfo.StyleType = H2O_IFR_STYLE_TYPE_PANEL;
        } else if (CurrentLayout != NULL) {
          LinkList = &CurrentLayout->StyleListHead;
          NewStyleInfo.StyleType = H2O_IFR_STYLE_TYPE_LAYOUT;
        } else {
          ASSERT (FALSE);
          CurrentProperty = NULL;
          break;
        }

        for (Link = GetFirstNode (LinkList); !IsNull (LinkList, Link); Link = GetNextNode (LinkList, Link)) {
          TempStyle = H2O_STYLE_INFO_NODE_FROM_LINK (Link);
          if (TempStyle->StyleType   == NewStyleInfo.StyleType &&
              TempStyle->PseudoClass == NewStyleInfo.PseudoClass &&
              TempStyle->ClassName   == NewStyleInfo.ClassName) {
            CurrentStyle = TempStyle;
            break;
          }
        }
        if (CurrentStyle == NULL) {
          CurrentStyle = &StyleBuffer[StyleIndex++];
          CurrentStyle->Signature   = H2O_STYLE_INFO_NODE_SIGNATURE;
          CurrentStyle->StyleType   = NewStyleInfo.StyleType;
          CurrentStyle->ClassName   = NewStyleInfo.ClassName;
          CurrentStyle->PseudoClass = NewStyleInfo.PseudoClass;
          InitializeListHead (&CurrentStyle->PropertyListHead);
          InsertTailList (LinkList, &CurrentStyle->Link);
        }
      }

      NewProperty.ValueType         = ((H2O_HII_LIBT_PROPERTY_BLOCK *)BlockData)->ValueType;
      NewProperty.IdentifierStr     = (CHAR8 *)BlockData + ((H2O_HII_LIBT_PROPERTY_BLOCK *)BlockData)->IdentifierOffset;
      NewProperty.IdentifierStrSize = (UINT32) AsciiStrSize (NewProperty.IdentifierStr);

      CurrentProperty = NULL;
      LinkList = &CurrentStyle->PropertyListHead;
      for (Link = GetFirstNode (LinkList), CurrentProperty = NULL; !IsNull (LinkList, Link); Link = GetNextNode (LinkList, Link)) {
        TempProperty = H2O_PROPERTY_INFO_NODE_FROM_LINK (Link);
        if (TempProperty->IdentifierStrSize == NewProperty.IdentifierStrSize &&
            TempProperty->ValueType         == NewProperty.ValueType &&
            AsciiStrCmp (TempProperty->IdentifierStr, NewProperty.IdentifierStr) == 0) {
          CurrentProperty = TempProperty;
          break;
        }
      }
      if (CurrentProperty == NULL) {
        CurrentProperty = &PropertyBuffer[PropertyIndex++];
        CurrentProperty->Signature         = H2O_PROPERTY_INFO_NODE_SIGNATURE;
        CurrentProperty->ValueType         = NewProperty.ValueType;
        CurrentProperty->IdentifierStr     = NewProperty.IdentifierStr;
        CurrentProperty->IdentifierStrSize = NewProperty.IdentifierStrSize;
        InsertTailList (&CurrentStyle->PropertyListHead, &CurrentProperty->Link);
        CurrentStyle->PropertyCount++;
      }
      CopyMem (&CurrentProperty->H2OValue, &((H2O_HII_LIBT_PROPERTY_BLOCK *)BlockData)->H2OValue, sizeof (H2O_VALUE));
      CurrentProperty->ValueStr = (CHAR8 *)BlockData + ((H2O_HII_LIBT_PROPERTY_BLOCK *)BlockData)->ValueOffset;
      break;

    case H2O_HII_LIBT_LAYOUT_DUP:
      OldLayoutId = ((H2O_HII_LIBT_LAYOUT_DUP_BLOCK *)BlockData)->OldLayoutId;
      Link = *LayoutListHead;
      Link = Link->ForwardLink;
      do {
        TempLayout = H2O_LAYOUT_INFO_NODE_FROM_LINK (Link);
        if (OldLayoutId == TempLayout->LayoutId) {
          CurrentLayout = AllocateCopyPool (sizeof (H2O_LAYOUT_INFO), TempLayout);
          if (CurrentLayout == NULL) {
            return EFI_OUT_OF_RESOURCES;
          }
          //
          // New LayoutId
          //
          CurrentLayout->LayoutId = ((H2O_HII_LIBT_LAYOUT_DUP_BLOCK *)BlockData)->LayoutId;
          InitializeListHead (&CurrentLayout->PanelListHead);
          InitializeListHead (&CurrentLayout->HotkeyListHead);
          InsertTailList (*LayoutListHead, &CurrentLayout->Link);
          break;
        }
        Link = Link->ForwardLink;
      } while (IsNodeAtEnd (*LayoutListHead, Link));
      break;

    case H2O_HII_LIBT_PANEL_DUP:
      if (CurrentLayout == NULL) {
        ASSERT (FALSE);
        CurrentPanel = NULL;
        break;
      }
      OldLayoutId = ((H2O_HII_LIBT_PANEL_DUP_BLOCK *)BlockData)->OldLayoutId;
      Link = *LayoutListHead;
      Link = Link->ForwardLink;
      do {
        //
        // Found OldLayoutId
        //
        TempLayout = H2O_LAYOUT_INFO_NODE_FROM_LINK (Link);
        if (OldLayoutId == TempLayout->LayoutId) {

          OldPanelId = ((H2O_HII_LIBT_PANEL_DUP_BLOCK *)BlockData)->OldPanelId;
          Link = &TempLayout->PanelListHead;
          Link = Link->ForwardLink;
          do {
            //
            // Found OldPanelId
            //
            TempPanel = H2O_PANEL_INFO_NODE_FROM_LINK (Link);
            if (OldPanelId == TempPanel->PanelId) {
              CurrentPanel = AllocateCopyPool (sizeof (H2O_PANEL_INFO), TempPanel);
              if (CurrentPanel == NULL) {
                return EFI_OUT_OF_RESOURCES;
              }
              //
              // New PanelId
              //
              CurrentPanel->PanelId = ((H2O_HII_LIBT_PANEL_DUP_BLOCK *)BlockData)->PanelId;
              ASSERT (CurrentLayout != NULL);
              if (CurrentLayout != NULL) {
                InsertTailList (&CurrentLayout->PanelListHead, &CurrentPanel->Link);
              }
              break;
            }
            Link = Link->ForwardLink;
          } while (IsNodeAtEnd (&TempLayout->PanelListHead, Link));

          break;
        }
        Link = Link->ForwardLink;
      } while (IsNodeAtEnd (*LayoutListHead, Link));
      break;

    case H2O_HII_LIBT_EXT2:
      break;

    case H2O_HII_LIBT_EXT4:
      break;

    case H2O_HII_LIBT_EXT_VFR:
      CurrentVfr = AllocateZeroPool (sizeof (H2O_VFR_INFO));
      if (CurrentVfr == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CurrentVfr->Signature = H2O_VFR_INFO_NODE_SIGNATURE;
      CurrentVfr->VfrId = ((H2O_HII_LIBT_EXT_VFR_BLOCK *)BlockData)->VfrId;
      InitializeListHead (&CurrentVfr->FormsetListHead);
      InsertTailList (*VfrListHead, &CurrentVfr->Link);
      break;

    case H2O_HII_LIBT_EXT_VFR_END:
      CurrentVfr     = NULL;
      CurrentFormset = NULL;
      CurrentForm    = NULL;
      CurrentHotkey  = NULL;
      CurrentStyle   = NULL;
      break;

    case H2O_HII_LIBT_EXT_FORMSET:
      if (CurrentVfr == NULL) {
        ASSERT (FALSE);
        break;
      }
      CurrentFormset = AllocateZeroPool (sizeof (H2O_FORMSET_INFO));
      if (CurrentFormset == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      CurrentFormset->Signature = H2O_FORMSET_INFO_NODE_SIGNATURE;
      CurrentFormset->FormsetBlock = (H2O_HII_LIBT_EXT_FORMSET_BLOCK *)BlockData;
      CopyMem (&CurrentFormset->FormsetId, &((H2O_HII_LIBT_EXT_FORMSET_BLOCK *)BlockData)->FormsetId, sizeof (EFI_GUID));
      InitializeListHead (&CurrentFormset->FormListHead);
      InitializeListHead (&CurrentFormset->HotkeyListHead);
      InitializeListHead (&CurrentFormset->StyleListHead);
      InitializeListHead (&CurrentFormset->ExpressionQIdMapList);
      InsertTailList (&CurrentVfr->FormsetListHead, &CurrentFormset->Link);
      break;

    case H2O_HII_LIBT_EXT_FORMSET_END:
      CurrentFormset = NULL;
      CurrentForm    = NULL;
      CurrentHotkey  = NULL;
      CurrentStyle   = NULL;
      break;

    case H2O_HII_LIBT_EXT_FORM:
      if (CurrentFormset == NULL) {
        ASSERT (FALSE);
        break;
      }
      CurrentForm = AllocateZeroPool (sizeof (H2O_FORM_INFO));
      if (CurrentForm == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CurrentForm->Signature = H2O_FORM_INFO_NODE_SIGNATURE;
      CurrentForm->FormBlock = (H2O_HII_LIBT_EXT_FORM_BLOCK *)BlockData;
      InitializeListHead (&CurrentForm->StyleListHead);
      InitializeListHead (&CurrentForm->HotkeyListHead);
      InitializeListHead (&CurrentForm->ImportListHead);
      InitializeListHead (&CurrentForm->StatementListHead);
      InitializeListHead (&CurrentForm->TargetListHead);
      InsertTailList (&CurrentFormset->FormListHead, &CurrentForm->Link);
      break;

    case H2O_HII_LIBT_EXT_FORM_END:
      CurrentForm = NULL;
      CurrentHotkey  = NULL;
      CurrentStyle   = NULL;
      break;

    case H2O_HII_LIBT_EXT_HOTKEY:
      if ((CurrentStatement == NULL &&
           CurrentForm == NULL &&
           CurrentFormset == NULL &&
           CurrentLayout == NULL) ||
          HotKeyBuffer == NULL) {
        ASSERT (FALSE);
        break;
      }
      CurrentHotkey = &HotKeyBuffer[HotKeyIndex++];
      CurrentHotkey->Signature = H2O_HOTKEY_INFO_NODE_SIGNATURE;
      CurrentHotkey->HotkeyBlock = (H2O_HII_LIBT_EXT_HOTKEY_BLOCK *)BlockData;
      InitializeListHead (&CurrentHotkey->StyleListHead);
      if (CurrentStatement != NULL) {
        InsertTailList (&CurrentStatement->HotkeyListHead, &CurrentHotkey->Link);
      } else if (CurrentForm != NULL) {
        InsertTailList (&CurrentForm->HotkeyListHead, &CurrentHotkey->Link);
      } else if (CurrentFormset != NULL) {
        InsertTailList (&CurrentFormset->HotkeyListHead, &CurrentHotkey->Link);
      } else if (CurrentLayout != NULL) {
        InsertTailList (&CurrentLayout->HotkeyListHead, &CurrentHotkey->Link);
      }
      break;

    case H2O_HII_LIBT_EXT_HOTKEY_END:
      CurrentHotkey = NULL;
      CurrentStyle   = NULL;
      break;

    case H2O_HII_LIBT_EXT_IMPORT:
      ASSERT (CurrentForm != NULL);
      if (CurrentForm == NULL) {
        break ;
      }

      CurrentImport = AllocateZeroPool (sizeof (H2O_IMPORT_INFO));
      if (CurrentImport == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      ImportBlock = (H2O_HII_LIBT_EXT_IMPORT_BLOCK *)BlockData;

      CurrentImport->Signature = H2O_IMPORT_INFO_NODE_SIGNATURE;
      CopyGuid (&CurrentImport->TargetFormsetGuid, &ImportBlock->TargetFormsetGuid);
      CurrentImport->Flags        = ImportBlock->Flags;
      CurrentImport->TargetFormId = ImportBlock->TargetFormId;
      CurrentImport->TargetId     = ImportBlock->TargetId;
      CurrentImport->LocalId      = ImportBlock->LocalId;

      InitializeListHead (&CurrentImport->Link);
      InsertTailList (&CurrentForm->ImportListHead, &CurrentImport->Link);
      break;

    case H2O_HII_LIBT_EXT_STATEMENT:
      if (CurrentForm == NULL) {
        ASSERT (FALSE);
        break;
      }
      CurrentStatement = AllocateZeroPool (sizeof (H2O_STATEMENT_INFO));
      if (CurrentStatement == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      StatementBlock = (H2O_HII_LIBT_EXT_STATEMENT_BLOCK *)BlockData;

      CurrentStatement->Signature   = H2O_STATEMENT_INFO_NODE_SIGNATURE;
      CurrentStatement->IsQuestion  = ((StatementBlock->Flags & H2O_HII_STATEMENT_QUESTION) == H2O_HII_STATEMENT_QUESTION);
      CurrentStatement->Position    = StatementBlock->Position;
      CurrentStatement->StatementId = StatementBlock->StatementId;
      CurrentStatement->QuestionId  = StatementBlock->QuestionId;

      InitializeListHead (&CurrentStatement->Link);
      InitializeListHead (&CurrentStatement->StyleListHead);
      InitializeListHead (&CurrentStatement->HotkeyListHead);
      InitializeListHead (&CurrentStatement->ExpressionListHead);
      InsertTailList (&CurrentForm->StatementListHead, &CurrentStatement->Link);
      break;

    case H2O_HII_LIBT_EXT_STATEMENT_END:
      CurrentStatement = NULL;
      CurrentHotkey    = NULL;
      CurrentStyle     = NULL;
      break;

    case H2O_HII_LIBT_EXPRESSION:
      ASSERT (CurrentStatement != NULL);
      if (CurrentStatement == NULL) {
        break;
      }

      ExpressionSize = ((H2O_HII_LAYOUT_BLOCK *) BlockData)->BlockSize - sizeof (H2O_HII_LIBT_EXPRESSION_BLOCK) + 1;
      CurrentExpression = AllocateZeroPool (sizeof (H2O_EXPRESSION_INFO) + ExpressionSize - 1);
      ASSERT (CurrentExpression != NULL);
      if (CurrentExpression == NULL) {
        break;
      }

      CurrentExpression->Signature      = H2O_EXPRESSION_INFO_NODE_SIGNATURE;
      CurrentExpression->ExpressionSize = ExpressionSize;
      CopyMem (
        CurrentExpression->ExpressionOpcodes,
        &(((H2O_HII_LIBT_EXPRESSION_BLOCK *)BlockData)->ExpressionOpcodes),
        ExpressionSize
        );

      InitializeListHead (&CurrentExpression->Link);
      InsertTailList (&CurrentStatement->ExpressionListHead, &CurrentExpression->Link);
      CurrentStatement->ExpressionCount++;
      break;

    case H2O_HII_LIBT_EXPRESSION_QID_MAP:
      ASSERT (CurrentFormset != NULL);
      if (CurrentFormset == NULL) {
        break;
      }

      CurrentExpressionQidMap = AllocateZeroPool (sizeof (H2O_EXPRESSION_QID_MAP_INFO));
      if (CurrentExpressionQidMap == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      CurrentExpressionQidMap->Signature = H2O_EXPRESSION_QID_MAP_INFO_NODE_SIGNATURE;
      CurrentExpressionQidMap->ExpressionQIdMapBlock = (H2O_HII_LIBT_EXPRESSION_QID_MAP_BLOCK *) BlockData;

      InitializeListHead (&CurrentExpressionQidMap->Link);
      InsertTailList (&CurrentFormset->ExpressionQIdMapList, &CurrentExpressionQidMap->Link);
      break ;

    case H2O_HII_LIBT_IMPORT_TARGET_BEGIN:
    case H2O_HII_LIBT_LINK_TARGET_BEGIN:
      ASSERT (CurrentForm != NULL);
      if (CurrentForm == NULL) {
        break;
      }

      CurrentTargetInfo = AllocateZeroPool (sizeof (H2O_TARGET_INFO));
      if (CurrentTargetInfo == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CurrentTargetInfo->Signature  = H2O_TARGET_INFO_NODE_SIGNATURE;
      CurrentTargetInfo->VfcfOpcode = BlockType;
      if (BlockType == H2O_HII_LIBT_IMPORT_TARGET_BEGIN) {
        ImportTargetBlock                = (H2O_HII_LIBT_IMPORT_TARGET_BEGIN_BLOCK *) BlockData;
        CurrentTargetInfo->TargetLabelId = ImportTargetBlock->TargetLabelId;
        CurrentTargetInfo->TargetFormId  = ImportTargetBlock->TargetFormId;
        CopyGuid (&CurrentTargetInfo->TargetFormsetGuid, (VOID *) &ImportTargetBlock->TargetFormsetGuid);
      } else {
        LinkTargetBlock                  = (H2O_HII_LIBT_LINK_TARGET_BEGIN_BLOCK *) BlockData;
        CurrentTargetInfo->TargetFormId  = LinkTargetBlock->TargetFormId;
        CurrentTargetInfo->TargetLabelId = LinkTargetBlock->TargetLabelId;
        CopyGuid (&CurrentTargetInfo->TargetFormsetGuid, (VOID *) &LinkTargetBlock->TargetFormsetGuid);
      }
      InitializeListHead (&CurrentTargetInfo->Link);
      InitializeListHead (&CurrentTargetInfo->StyleListHead);
      InsertTailList (&CurrentForm->TargetListHead, &CurrentTargetInfo->Link);
      break;

    case H2O_HII_LIBT_IMPORT_TARGET_END:
    case H2O_HII_LIBT_LINK_TARGET_END:
      CurrentTargetInfo = NULL;
      CurrentStyle      = NULL;
      break;

    default:
      ASSERT (FALSE);
      break;
    }
  }

  return Status;
}

STATIC
EFI_STATUS
GetLayoutPackage (
  IN     EFI_HII_HANDLE                  HiiHandle,
  OUT    UINT8                           **LayoutPackage
  )
{
  EFI_STATUS                             Status;

  EFI_HII_DATABASE_PROTOCOL              *HiiDatabase;
  EFI_HII_PACKAGE_LIST_HEADER            *HiiPackageList;
  EFI_HII_PACKAGE_HEADER                 *Package;
  UINT32                                 PackageLength;
  UINTN                                  BufferSize;
  UINTN                                  Offset;

  if (LayoutPackage == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *LayoutPackage = NULL;

  //
  // Locate HiiDatabase Protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &HiiDatabase);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get the HII package list by HiiHandle
  //
  BufferSize = 0;
  HiiPackageList = NULL;
  Status = HiiDatabase->ExportPackageLists (HiiDatabase, HiiHandle, &BufferSize, HiiPackageList);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    HiiPackageList = (EFI_HII_PACKAGE_LIST_HEADER *) AllocatePool (BufferSize);
    if (HiiPackageList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    Status = HiiDatabase->ExportPackageLists (HiiDatabase, HiiHandle, &BufferSize, HiiPackageList);
    if (EFI_ERROR (Status)) {
      SafeFreePool ((VOID **) &HiiPackageList);
      return Status;
    }
  } else {
    return Status;
  }

  //
  // Find layout package by package type
  //
  Status = EFI_NOT_FOUND;
  Offset = sizeof (EFI_HII_PACKAGE_LIST_HEADER);
  PackageLength = HiiPackageList->PackageLength;
  while (Offset < PackageLength) {
    Package = (EFI_HII_PACKAGE_HEADER *) (((UINT8 *) HiiPackageList) + Offset);

    if (Package->Type == H2O_HII_PACKAGE_LAYOUTS) {
      *LayoutPackage = (UINT8 *) AllocatePool (PackageLength);
      if (*LayoutPackage != NULL) {
        CopyMem (*LayoutPackage, Package, PackageLength);
        Status = EFI_SUCCESS;
      } else {
        Status = EFI_BUFFER_TOO_SMALL;
      }
      break;
    }
    Offset += Package->Length;
  }

  if (Offset >= PackageLength) {
    Status = EFI_NOT_FOUND;
  }

  SafeFreePool ((VOID **) &HiiPackageList);

  return Status;
}

EFI_STATUS
GetLayoutTreeAndVfrTree (
  OUT    LIST_ENTRY                              **LayoutListHead,
  OUT    LIST_ENTRY                              **VfrListHead
  )
{
  EFI_STATUS                                     Status;
  LAYOUT_DATABASE_PROTOCOL                       *LayoutDatabase;
  UINT8                                          *PkgBuffer;
  LIST_ENTRY                                     *TempLayoutListHead;
  LIST_ENTRY                                     *TempVfrListHead;

  ASSERT (LayoutListHead != NULL);
  ASSERT (VfrListHead != NULL);

  if (LayoutListHead == NULL || VfrListHead == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *LayoutListHead = NULL;
  *LayoutListHead = NULL;

  Status = gBS->LocateProtocol (&gLayoutDatabaseProtocolGuid, NULL, (VOID **) &LayoutDatabase);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TempLayoutListHead = LayoutDatabase->LayoutListHead;
  TempVfrListHead = LayoutDatabase->VfrListHead;
  if (TempLayoutListHead == NULL) {
    //
    // Get layout pkg buffer
    //
    PkgBuffer = NULL;
    Status = GetLayoutPackage (LayoutDatabase->LayoutPkgHiiHandle, &PkgBuffer);
    //
    // BUGBUG: HiiDatabase not support layout pkg yet
    //
    if (EFI_ERROR (Status)) {
      //return Status;
    }
    PkgBuffer = (UINT8 *)(UINTN *)LayoutDatabase->LayoutPkgAddr;
    if (PkgBuffer == NULL) {
      return EFI_NOT_FOUND;
    }

    //
    // Parse layout pkg into layout list
    //
    Status = ParseLayoutPkg(PkgBuffer, &TempLayoutListHead, &TempVfrListHead);
    if (Status != EFI_SUCCESS) {
      return Status;
    }
    LayoutDatabase->LayoutListHead = TempLayoutListHead;
    LayoutDatabase->VfrListHead = TempVfrListHead;
  }

  *LayoutListHead = LayoutDatabase->LayoutListHead;
  *VfrListHead = LayoutDatabase->VfrListHead;

  return EFI_SUCCESS;
}

EFI_STATUS
GetLayoutById (
  IN     UINT32                                  LayoutId,
  IN     EFI_GUID                                *DisplayEngineGuid,
  OUT    H2O_LAYOUT_INFO                         **Layout
  )
{
  EFI_STATUS                                     Status;
  LIST_ENTRY                                     *LayoutList;
  LIST_ENTRY                                     *VfrListHead;
  LIST_ENTRY                                     *LayoutLink;
  H2O_LAYOUT_INFO                                *LayoutInfo;

  ASSERT (Layout != NULL);
  ASSERT (DisplayEngineGuid != NULL);
  if ((Layout == NULL) || (DisplayEngineGuid == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetLayoutTreeAndVfrTree (&LayoutList, &VfrListHead);
  if (EFI_ERROR (Status) || LayoutList == NULL) {
    return Status;
  }

  //
  // Get Layout
  //
  LayoutLink = GetFirstNode (LayoutList);
  while (!IsNull (LayoutList, LayoutLink)) {
    LayoutInfo = H2O_LAYOUT_INFO_NODE_FROM_LINK (LayoutLink);
    if ((LayoutInfo->LayoutId == LayoutId) &&
        ((CompareGuid(&LayoutInfo->DisplayEngineGuid[0], DisplayEngineGuid)) ||
         (CompareGuid(&LayoutInfo->DisplayEngineGuid[1], DisplayEngineGuid)) ||
         (CompareGuid(&LayoutInfo->DisplayEngineGuid[2], DisplayEngineGuid)))) {
      *Layout = LayoutInfo;
      return EFI_SUCCESS;
    }
    LayoutLink = GetNextNode (LayoutList, LayoutLink);
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
GetFormsetLayoutByGuid (
  IN     EFI_GUID                                *FormsetGuid,
  OUT    H2O_FORMSET_INFO                        **Formset
  )
{
  EFI_STATUS                                     Status;
  LIST_ENTRY                                     *LayoutListHead;
  LIST_ENTRY                                     *VfrListHead;
  LIST_ENTRY                                     *FormsetListHead;
  LIST_ENTRY                                     *VfrLink;
  LIST_ENTRY                                     *FormsetLink;
  H2O_VFR_INFO                                   *TempVfr;
  H2O_FORMSET_INFO                               *TempFormset;

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || VfrListHead == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Get vfr
  //
  VfrLink = VfrListHead;
  if (IsNull (VfrLink, VfrLink->ForwardLink)) {
    return EFI_NOT_FOUND;
  }

  *Formset = NULL;
  TempFormset = NULL;
  do {
    VfrLink = VfrLink->ForwardLink;
    TempVfr = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);

    //
    // Get formset
    //
    FormsetListHead = &TempVfr->FormsetListHead;
    FormsetLink = FormsetListHead;
    if (IsNull (FormsetLink, FormsetLink->ForwardLink)) {
      continue;
    }
    do {
      FormsetLink = FormsetLink->ForwardLink;;
      TempFormset = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);

      if (CompareGuid (&(TempFormset->FormsetId), FormsetGuid)) {
        *Formset = TempFormset;
        break;
      }
    } while (!IsNodeAtEnd (FormsetListHead, FormsetLink));
    if (*Formset != NULL) {
      break;
    }

  } while (!IsNodeAtEnd (VfrListHead, VfrLink));

  if (*Formset == NULL) {
    *Formset = TempFormset;
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetLayoutIdByGuid (
  IN     EFI_GUID                                *FormsetGuid,
  OUT    UINT32                                  *LayoutId
  )
{
  EFI_STATUS                                     Status;
  H2O_PROPERTY_INFO                              *Property;
  H2O_PROPERTY_VALUE                             PropValue;
  UINT32                                         FormsetLayoutId;

  Status = GetVfcfFormsetLayoutId (FormsetGuid, &FormsetLayoutId);
  if (!EFI_ERROR (Status)) {
    *LayoutId = FormsetLayoutId;
    return Status;
  }

  Status = GetVfcfFormsetPropertyValue (
             FormsetGuid,
             H2O_IFR_STYLE_TYPE_FORMSET,
             H2O_STYLE_PSEUDO_CLASS_NORMAL,
             "layout",
             NULL,
             &Property,
             &PropValue
             );

  if (!EFI_ERROR (Status)) {
    *LayoutId = PropValue.H2OValue.Value.U16;
  }

  return Status;
}

STATIC
VOID
FreeSetupMenuData (
  IN H2O_FORM_BROWSER_SM                      *SetupMenuData
  )
{
  if (SetupMenuData == NULL) {
    return;
  }

  if (SetupMenuData->TitleString != NULL) {
    FreePool (SetupMenuData->TitleString);
  }
  if (SetupMenuData->CoreVersionString != NULL) {
    FreePool (SetupMenuData->CoreVersionString);
  }
  if (SetupMenuData->SetupMenuInfoList != NULL) {
    FreePool (SetupMenuData->SetupMenuInfoList);
  }
  FreePool (SetupMenuData);
}

STATIC
EFI_STATUS
GetLayoutByDEGuid (
  IN EFI_GUID                                    *FsGuid,
  IN EFI_GUID                                    *DEGuid,
  OUT H2O_LAYOUT_INFO                            **Layout
  )
{
  EFI_STATUS                                     Status;
  BOOLEAN                                        IsScu;
  EFI_SETUP_UTILITY_APPLICATION_PROTOCOL         *SetupUtilityApp;
  H2O_FORM_BROWSER_PROTOCOL                      *FB;
  H2O_FORM_BROWSER_SM                            *SetupMenuData;
  EFI_GUID                                       ScuFormSetGuid = {0x9f85453e, 0x2f03, 0x4989, 0xad, 0x3b, 0x4a, 0x84, 0x07, 0x91, 0xaf, 0x3a};
  UINT32                                         LayoutId;
  H2O_LAYOUT_INFO                                *LayoutInfo;

  if (FsGuid == NULL || DEGuid == NULL || Layout == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IsScu = FALSE;
  Status = gBS->LocateProtocol (&gEfiSetupUtilityApplicationProtocolGuid, NULL, (VOID **) &SetupUtilityApp);
  if (!EFI_ERROR (Status) && SetupUtilityApp->VfrDriverState == InitializeSetupUtility) {
    IsScu = TRUE;
  } else {
    Status = gBS->LocateProtocol (&gH2OFormBrowserProtocolGuid, NULL, (VOID **) &FB);
    if (!EFI_ERROR (Status)) {
      Status = FB->GetSMInfo (FB, &SetupMenuData);
      if (!EFI_ERROR (Status)) {
        if (SetupMenuData->NumberOfSetupMenus > 1) {
          IsScu = TRUE;
        }
        FreeSetupMenuData (SetupMenuData);
      }
    }
  }

  Status = EFI_NOT_FOUND;
  LayoutId = 0;
  if (IsScu) {
    //
    // The formset has setup menu will get SCU layout first
    //
    Status = GetLayoutIdByGuid (&ScuFormSetGuid, &LayoutId);
  }
  if (EFI_ERROR (Status)) {
    Status = GetLayoutIdByGuid (FsGuid, &LayoutId);
    if (EFI_ERROR (Status)) {
      Status = GetLayoutIdByGuid (&gZeroGuid, &LayoutId);
    }
  }
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetLayoutById (LayoutId, DEGuid, &LayoutInfo);
  if (EFI_ERROR (Status)) {
    Status = GetLayoutIdByGuid (&gZeroGuid, &LayoutId);
    ASSERT_EFI_ERROR (Status);
    Status = GetLayoutById (LayoutId, DEGuid, &LayoutInfo);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  *Layout = LayoutInfo;

  return EFI_SUCCESS;
}

H2O_FORM_INFO *
GetFormLayoutByFormId (
  IN H2O_FORMSET_INFO                            *Formset,
  IN UINT32                                      FormId
  )
{
  LIST_ENTRY                                     *FormLink;
  H2O_FORM_INFO                                  *FormInfo;

  if (Formset == NULL) {
    return NULL;
  }

  FormLink = &Formset->FormListHead;
  if (IsNull (FormLink, FormLink->ForwardLink)) {
    return NULL;
  }

  do {
    FormLink = FormLink->ForwardLink;
    FormInfo = H2O_FORM_INFO_NODE_FROM_LINK (FormLink);
    if (FormInfo->FormBlock->FormId == FormId) {
      return FormInfo;
    }
  } while (!IsNodeAtEnd (&Formset->FormListHead, FormLink));

  return NULL;
}

EFI_STATUS
GetPanelById (
  IN EFI_GUID                                    *FsGuid,
  IN EFI_GUID                                    *DEGuid,
  IN UINT32                                      PanelId,
  OUT H2O_PANEL_INFO                             **Panel
  )
{
  EFI_STATUS                                     Status;
  H2O_LAYOUT_INFO                                *LayoutInfo;
  H2O_PANEL_INFO                                 *PanelInfo;

  if (FsGuid == NULL || DEGuid == NULL || Panel == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetLayoutByDEGuid (FsGuid, DEGuid, &LayoutInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PanelInfo = GetPanelInfo (LayoutInfo, PanelId);
  if (PanelInfo == NULL) {
    return EFI_NOT_FOUND;
  }

  *Panel = PanelInfo;

  return EFI_SUCCESS;
}

H2O_STATEMENT_INFO *
GetStatementLayoutByQuestionId (
  IN H2O_FORMSET_INFO                            *Formset,
  IN H2O_FORM_INFO                               *FormInfo,
  IN BOOLEAN                                     IsQuestion,
  IN UINT16                                      Id
  )
{
  LIST_ENTRY                                     *FormLink;
  LIST_ENTRY                                     *StatementLink;
  H2O_FORM_INFO                                  *Form;
  H2O_STATEMENT_INFO                             *Statement;

  if (Formset == NULL) {
    return NULL;
  }

  if (IsListEmpty (&Formset->FormListHead)) {
    return NULL;
  }

  FormLink = GetFirstNode (&Formset->FormListHead);
  while (!IsNull (&Formset->FormListHead, FormLink)) {
    Form     = H2O_FORM_INFO_NODE_FROM_LINK (FormLink);
    FormLink = GetNextNode (&Formset->FormListHead, FormLink);
    if (IsListEmpty (&Form->StatementListHead)) {
      continue;
    }

    StatementLink = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, StatementLink)) {
      Statement     = H2O_STATEMENT_INFO_NODE_FROM_LINK (StatementLink);
      StatementLink = GetNextNode (&Form->StatementListHead, StatementLink);
      if (Statement->IsQuestion == IsQuestion &&
          Statement->QuestionId == Id) {
        return Statement;
      }
    }
  }

  return NULL;
}

H2O_PANEL_INFO *
GetNextPanel (
  IN H2O_LAYOUT_INFO                           *LayoutInfo,
  IN H2O_PANEL_INFO                            *PanelInfo
  )
{
  LIST_ENTRY                                   *PanelLink;
  H2O_PANEL_INFO                               *Panel;

  if (LayoutInfo == NULL) {
    return NULL;
  }
  if (IsListEmpty (&LayoutInfo->PanelListHead)) {
    return NULL;
  }

  if (PanelInfo == NULL) {
    PanelLink = GetFirstNode (&LayoutInfo->PanelListHead);
    Panel     = H2O_PANEL_INFO_NODE_FROM_LINK (PanelLink);
    return Panel;
  }

  PanelLink = GetFirstNode (&LayoutInfo->PanelListHead);
  while (!IsNull (&LayoutInfo->PanelListHead, PanelLink)) {
    Panel     = H2O_PANEL_INFO_NODE_FROM_LINK (PanelLink);
    PanelLink = GetNextNode (&LayoutInfo->PanelListHead, PanelLink);
    if (Panel == PanelInfo) {
      break;
    }
  }
  if (IsNull (&LayoutInfo->PanelListHead, PanelLink)) {
    return NULL;
  }

  Panel = H2O_PANEL_INFO_NODE_FROM_LINK (PanelLink);
  return Panel;
}

VOID
SafeFreePool(
  IN   VOID                                **Buffer
  )
{
  if (*Buffer != NULL) {
    FreePool (*Buffer);
  }
  *Buffer = NULL;
}

