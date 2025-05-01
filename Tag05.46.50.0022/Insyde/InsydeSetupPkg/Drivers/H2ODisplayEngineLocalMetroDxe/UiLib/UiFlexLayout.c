/** @file
  UI Common Controls

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "UiControls.h"

STATIC UI_FLEX_LAYOUT_CLASS         *mFlexLayoutClass = NULL;
#define CURRENT_CLASS               mFlexLayoutClass

typedef struct {
  INT32                         Width;
  INT32                         Height;

  SIZE                          *ItemsSize;
  UINTN                         ItemCount;
  UINTN                         FlexItemCount;
} UI_FLEX_LAYOUT_ROW_INFO, UI_FLEX_LAYOUT_COLUMN_INFO;

STATIC
EFI_STATUS
UiFlexLayoutGetRowInfo (
  IN  UI_CONTROL                *Control,
  IN  SIZE                      ChildAvailable,
  OUT UINT32                    *RowInfoCount,
  OUT UI_FLEX_LAYOUT_ROW_INFO   **RowInfoList
  )
{
  UI_FLEX_LAYOUT                *This;
  SIZE                          *SizeList;
  SIZE                          *ItemSize;
  UINTN                         Index;
  UINTN                         ItemCount;
  UINTN                         FlexItemCount;
  INT32                         RowWidth;
  INT32                         RowHeight;
  UI_FLEX_LAYOUT_ROW_INFO       *RowInfo;
  UI_FLEX_LAYOUT_ROW_INFO       *InfoList;
  UINT32                        InfoCount;
  UI_CONTROL                    *Item;
  INT32                         ItemMarginWidth;
  INT32                         ItemMarginHeight;
  SIZE                          ItemAvailable;

  if (Control->ItemCount == 0) {
    return EFI_NOT_FOUND;
  }

  SizeList = AllocateZeroPool (Control->ItemCount * sizeof(SIZE));
  if (SizeList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  This      = (UI_FLEX_LAYOUT *) Control;
  InfoList  = NULL;
  InfoCount = 0;
  RowWidth  = 0;
  RowHeight = 0;
  ItemCount = 0;
  FlexItemCount = 0;

  for (Index = 0; Index < Control->ItemCount; Index++) {
    Item     = Control->Items[Index];
    ItemCount++;

    if ((GetWindowLongPtr (Item->Wnd, GWL_STYLE) & WS_VISIBLE) == 0) {
      continue;
    }

    ItemSize = &SizeList[Index];
    CopyMem (&Item->Available, &ChildAvailable, sizeof (SIZE));
    CopyMem (&ItemAvailable  , &ChildAvailable, sizeof (SIZE));
    ItemMarginWidth  = GetControlMarginWidth (Item);
    ItemMarginHeight = GetControlMarginHeight (Item);
    ItemAvailable.cx -= ItemMarginWidth;
    ItemAvailable.cy -= ItemMarginHeight;
    SizeList[Index] = CONTROL_CLASS (Item)->EstimateSize (Item, ItemAvailable);
    if (ItemSize->cx <= 0) {
      continue;
    }
    ItemSize->cx += ItemMarginWidth;
    ItemSize->cy += ItemMarginHeight;

    if (Item->Float || Item->Position == UI_POSITION_ABSOLUTE) {
      continue;
    }
    FlexItemCount++;

    if (This->FlexWrap == UI_FLEX_LAYOUT_FLEX_WRAP_TYPE_NO_WRAP || RowWidth + ItemSize->cx <= ChildAvailable.cx) {
      RowWidth += ItemSize->cx;
      RowHeight = MAX(RowHeight, ItemSize->cy);
    } else {
      InfoList = ReallocatePool (sizeof(UI_FLEX_LAYOUT_ROW_INFO) * InfoCount, sizeof(UI_FLEX_LAYOUT_ROW_INFO) * (InfoCount + 1), InfoList);
      if (InfoList == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      RowInfo = &InfoList[InfoCount++];

      if (RowWidth > 0) {
        RowInfo->Width         = RowWidth;
        RowInfo->Height        = RowHeight;
        RowInfo->ItemCount     = ItemCount - 1;
        RowInfo->FlexItemCount = FlexItemCount - 1;
        RowInfo->ItemsSize     = &SizeList[Index - RowInfo->ItemCount];

        ItemCount = 1;
        FlexItemCount = 1;
        RowWidth  = ItemSize->cx;
        RowHeight = ItemSize->cy;
      } else {
        RowInfo->Width         = ItemSize->cx;
        RowInfo->Height        = ItemSize->cy;
        RowInfo->ItemCount     = ItemCount;
        RowInfo->FlexItemCount = FlexItemCount;
        RowInfo->ItemsSize     = &SizeList[Index + 1 - RowInfo->ItemCount];

        ItemCount = 0;
        FlexItemCount = 0;
        RowWidth  = 0;
        RowHeight = 0;
      }
    }
  }
  if (RowWidth > 0) {
    InfoList = ReallocatePool (sizeof(UI_FLEX_LAYOUT_ROW_INFO) * InfoCount, sizeof(UI_FLEX_LAYOUT_ROW_INFO) * (InfoCount + 1), InfoList);
    if (InfoList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    RowInfo = &InfoList[InfoCount++];

    RowInfo->Width         = RowWidth;
    RowInfo->Height        = RowHeight;
    RowInfo->ItemCount     = ItemCount;
    RowInfo->FlexItemCount = FlexItemCount;
    RowInfo->ItemsSize     = &SizeList[Index - RowInfo->ItemCount];
  }

  if (InfoCount == 0) {
    FreePool (SizeList);
    return EFI_NOT_FOUND;
  }

  *RowInfoList  = InfoList;
  *RowInfoCount = InfoCount;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UiFlexLayoutGetColumnInfo (
  IN  UI_CONTROL                  *Control,
  IN  SIZE                        ChildAvailable,
  OUT UINT32                      *ColumnInfoCount,
  OUT UI_FLEX_LAYOUT_COLUMN_INFO  **ColumnInfoList
  )
{
  UI_FLEX_LAYOUT                  *This;
  SIZE                            *SizeList;
  SIZE                            *ItemSize;
  UINTN                           Index;
  UINTN                           ItemCount;
  UINTN                           FlexItemCount;
  INT32                           ColumnWidth;
  INT32                           ColumnHeight;
  UI_FLEX_LAYOUT_COLUMN_INFO      *ColumnInfo;
  UI_FLEX_LAYOUT_COLUMN_INFO      *InfoList;
  UINT32                          InfoCount;
  UI_CONTROL                      *Item;
  INT32                           ItemMarginWidth;
  INT32                           ItemMarginHeight;
  SIZE                            ItemAvailable;

  if (Control->ItemCount == 0) {
    return EFI_NOT_FOUND;
  }

  SizeList = AllocateZeroPool (Control->ItemCount * sizeof(SIZE));
  if (SizeList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  This      = (UI_FLEX_LAYOUT *) Control;
  InfoList  = NULL;
  InfoCount = 0;
  ColumnWidth  = 0;
  ColumnHeight = 0;
  ItemCount = 0;
  FlexItemCount = 0;

  for (Index = 0; Index < Control->ItemCount; Index++) {
    Item     = Control->Items[Index];
    ItemCount++;

    if ((GetWindowLongPtr (Item->Wnd, GWL_STYLE) & WS_VISIBLE) == 0) {
      continue;
    }

    ItemSize = &SizeList[Index];
    CopyMem (&Item->Available, &ChildAvailable, sizeof (SIZE));
    CopyMem (&ItemAvailable, &ChildAvailable, sizeof (SIZE));
    ItemMarginWidth  = GetControlMarginWidth (Item);
    ItemMarginHeight = GetControlMarginHeight (Item);
    ItemAvailable.cx -= ItemMarginWidth;
    ItemAvailable.cy -= ItemMarginHeight;
    SizeList[Index] = CONTROL_CLASS (Item)->EstimateSize (Item, ItemAvailable);
    if (ItemSize->cy <= 0) {
      continue;
    }
    ItemSize->cx += ItemMarginWidth;
    ItemSize->cy += ItemMarginHeight;

    if (Item->Float || Item->Position == UI_POSITION_ABSOLUTE) {
      continue;
    }
    FlexItemCount++;

    if (This->FlexWrap == UI_FLEX_LAYOUT_FLEX_WRAP_TYPE_NO_WRAP || ColumnHeight + ItemSize->cy <= ChildAvailable.cy) {
      ColumnHeight += ItemSize->cy;
      ColumnWidth = MAX(ColumnWidth, ItemSize->cx);
    } else {
      InfoList = ReallocatePool (sizeof(UI_FLEX_LAYOUT_COLUMN_INFO) * InfoCount, sizeof(UI_FLEX_LAYOUT_COLUMN_INFO) * (InfoCount + 1), InfoList);
      if (InfoList == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      ColumnInfo = &InfoList[InfoCount++];

      if (ColumnHeight > 0) {
        ColumnInfo->Width         = ColumnWidth;
        ColumnInfo->Height        = ColumnHeight;
        ColumnInfo->ItemCount     = ItemCount - 1;
        ColumnInfo->FlexItemCount = FlexItemCount - 1;
        ColumnInfo->ItemsSize     = &SizeList[Index - ColumnInfo->ItemCount];

        ItemCount = 1;
        FlexItemCount = 1;
        ColumnWidth  = ItemSize->cx;
        ColumnHeight = ItemSize->cy;
      } else {
        ColumnInfo->Width         = ItemSize->cx;
        ColumnInfo->Height        = ItemSize->cy;
        ColumnInfo->ItemCount     = ItemCount;
        ColumnInfo->FlexItemCount = FlexItemCount;
        ColumnInfo->ItemsSize     = &SizeList[Index + 1 - ColumnInfo->ItemCount];

        ItemCount = 0;
        FlexItemCount = 0;
        ColumnWidth  = 0;
        ColumnHeight = 0;
      }
    }
  }
  if (ColumnHeight > 0) {
    InfoList = ReallocatePool (sizeof(UI_FLEX_LAYOUT_COLUMN_INFO) * InfoCount, sizeof(UI_FLEX_LAYOUT_COLUMN_INFO) * (InfoCount + 1), InfoList);
    if (InfoList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    ColumnInfo = &InfoList[InfoCount++];

    ColumnInfo->Width         = ColumnWidth;
    ColumnInfo->Height        = ColumnHeight;
    ColumnInfo->ItemCount     = ItemCount;
    ColumnInfo->FlexItemCount = FlexItemCount;
    ColumnInfo->ItemsSize     = &SizeList[Index - ColumnInfo->ItemCount];
  }

  if (InfoCount == 0) {
    FreePool (SizeList);
    return EFI_NOT_FOUND;
  }

  *ColumnInfoList  = InfoList;
  *ColumnInfoCount = InfoCount;
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UiFlexLayoutEstimateChildSizeByRow (
  IN  UI_CONTROL                *Control,
  IN  SIZE                      ChildAvailable,
  OUT INTN                      *ChildWidth,
  OUT INTN                      *ChildHeight
  )
{
  EFI_STATUS                    Status;
  UI_FLEX_LAYOUT_ROW_INFO       *RowInfoList;
  UINT32                        Count;
  UINT32                        Index;
  INTN                          Height;
  INTN                          Width;

  Status = UiFlexLayoutGetRowInfo (Control, ChildAvailable, &Count, &RowInfoList);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Width  = 0;
  Height = 0;
  for (Index = 0; Index < Count; Index++) {
    Width   = MAX(Width, RowInfoList[Index].Width);
    Height += RowInfoList[Index].Height;
  }
  *ChildWidth  = Width;
  *ChildHeight = Height;

  FreePool (RowInfoList->ItemsSize);
  FreePool (RowInfoList);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UiFlexLayoutEstimateChildSizeByColumn (
  IN  UI_CONTROL                *Control,
  IN  SIZE                      ChildAvailable,
  OUT INTN                      *ChildWidth,
  OUT INTN                      *ChildHeight
  )
{
  EFI_STATUS                    Status;
  UI_FLEX_LAYOUT_COLUMN_INFO    *ColumnInfoList;
  UINT32                        Count;
  UINT32                        Index;
  INTN                          Height;
  INTN                          Width;

  Status = UiFlexLayoutGetColumnInfo (Control, ChildAvailable, &Count, &ColumnInfoList);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Width  = 0;
  Height = 0;
  for (Index = 0; Index < Count; Index++) {
    Height   = MAX(Height, ColumnInfoList[Index].Height);
    Width += ColumnInfoList[Index].Width;
  }
  *ChildWidth  = Width;
  *ChildHeight = Height;

  FreePool (ColumnInfoList->ItemsSize);
  FreePool (ColumnInfoList);
  return EFI_SUCCESS;
}

SIZE
UiFlexLayoutEstimateSize (
  UI_CONTROL                    *Control,
  SIZE                          AvailableSize
  )
{
  UI_FLEX_LAYOUT                *This;
  SIZE                          Size;
  SIZE                          Available;
  INTN                          Height;
  INTN                          Width;

  if (Control->Width.Type  != UI_LENGTH_TYPE_WRAP_CONTENT &&
      Control->Height.Type != UI_LENGTH_TYPE_WRAP_CONTENT) {
    return CalculateControlDisplaySize (Control, AvailableSize);
  }

  Available.cx = AvailableSize.cx - GetControlPaddingWidth (Control)  - GetControlBorderWidth (Control);
  Available.cy = AvailableSize.cy - GetControlPaddingHeight (Control) - GetControlBorderHeight (Control);

  if (IS_CONTROL_HEIGHT_VALUE_ASSIGNED (Control)) {
    Available.cy = CalculateControlDisplayHeight (Control, AvailableSize) - GetControlPaddingHeight (Control) - GetControlBorderHeight (Control);
  }
  if (IS_CONTROL_WIDTH_VALUE_ASSIGNED (Control)) {
    Available.cx = CalculateControlDisplayWidth (Control, AvailableSize) - GetControlPaddingWidth (Control) - GetControlBorderWidth (Control);
  }

  This   = (UI_FLEX_LAYOUT *) Control;
  Width  = 0;
  Height = 0;
  if (This->FlexDirection == UI_FLEX_LAYOUT_FLEX_DIRECTION_TYPE_ROW) {
    UiFlexLayoutEstimateChildSizeByRow (Control, Available, &Width, &Height);
  } else {
    UiFlexLayoutEstimateChildSizeByColumn (Control, Available, &Width, &Height);
  }

  if (Width > 0) {
    Width += (GetControlPaddingWidth (Control)  + GetControlBorderWidth (Control));
  }
  if (Height > 0) {
    Height += (GetControlPaddingHeight (Control) + GetControlBorderHeight (Control));
  }

  Width  = MAX (Width , Control->MinSize.cx);
  Height = MAX (Height, Control->MinSize.cy);

  Size = CalculateControlDisplaySize (Control, AvailableSize);
  if (Size.cy == WRAP_CONTENT) {
    Size.cy = (GDICOORD)Height;
  }

  if (Size.cx == WRAP_CONTENT) {
    Size.cx = (GDICOORD)Width;
  }

  return Size;
}

STATIC
VOID
UiFlexLayoutSetChildPositionByRow (
  UI_CONTROL                   *Control,
  CONST RECT                   *Pos
  )
{
  UI_FLEX_LAYOUT               *This;
  UI_CONTROL                   *Item;
  UINTN                        Index;
  UINTN                        ItemIndex;
  UINTN                        Count;
  RECT                         ItemRc;
  UINT32                       Left;
  UINT32                       Top;
  UINT32                       JustifyContentPadding;
  UINT32                       AlignContentPadding;
  UINT32                       AlignItemsPadding;
  SIZE                         Available;
  SIZE                         *ItemSize;
  POINT                        Point;
  UINT32                       RowInfoCount;
  UI_FLEX_LAYOUT_ROW_INFO      *RowInfoList;
  UI_FLEX_LAYOUT_ROW_INFO      *RowInfo;
  EFI_STATUS                   Status;
  INT32                        Height;

  This = (UI_FLEX_LAYOUT *) Control;

  Available.cx = Pos->right - Pos->left;
  Available.cy = Pos->bottom - Pos->top;

  Status = UiFlexLayoutGetRowInfo (Control, Available, &RowInfoCount, &RowInfoList);
  if (EFI_ERROR (Status)) {
    return;
  }

  Top                   = Pos->top;
  Left                  = Pos->left;
  AlignItemsPadding     = 0;
  AlignContentPadding   = 0;
  JustifyContentPadding = 0;

  switch (This->AlignContent) {

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_STRETCH:
    Top = Pos->top;
    for (Index = 0, Height = 0; Index < RowInfoCount; Index++) {
      Height += RowInfoList[Index].Height;
    }
    if (Available.cy > Height) {
      for (Index = 0; Index < RowInfoCount; Index++) {
        RowInfoList[Index].Height += (Available.cy - Height) / RowInfoCount;
      }
    }
    break;

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_FLEX_START:
    Top = Pos->top;
    break;

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_FLEX_END:
    Top = Pos->bottom;
    for (Index = 0; Index < RowInfoCount; Index++) {
      Top -= RowInfoList[Index].Height;
    }
    break;

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_CENTER:
    for (Index = 0, Height = 0; Index < RowInfoCount; Index++) {
      Height += RowInfoList[Index].Height;
    }
    Top = Pos->top + (Available.cy - Height) / 2;
    break;

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_SPACE_BETWEEN:
    Top = Pos->top;
    if (RowInfoCount > 1) {
      for (Index = 0, Height = 0; Index < RowInfoCount; Index++) {
        Height += RowInfoList[Index].Height;
      }
      if (Available.cy > Height) {
        AlignContentPadding = (Available.cy - Height) / (RowInfoCount - 1);
      }
    }
    break;

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_SPACE_AROUND:
    for (Index = 0, Height = 0; Index < RowInfoCount; Index++) {
      Height += RowInfoList[Index].Height;
    }
    if (Available.cy > Height) {
      AlignContentPadding = (Available.cy - Height) / (RowInfoCount * 2);
    }

    Top = Pos->top + AlignContentPadding;
    AlignContentPadding = AlignContentPadding * 2;
    break;
  }

  for (Index = 0, Count = 0; Index < RowInfoCount; Index++) {
    RowInfo = &RowInfoList[Index];

    switch (This->JustifyContent) {

    case UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_FLEX_START:
      Left = Pos->left;
      break;

    case UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_FLEX_END:
      Left = Pos->right - RowInfo->Width;
      break;

    case UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_CENTER:
      Left = Pos->left + (Available.cx - RowInfo->Width) / 2;
      break;

    case UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_SPACE_BETWEEN:
      Left = Pos->left;
      if (Available.cx > RowInfo->Width && RowInfo->FlexItemCount > 1) {
        JustifyContentPadding = (UINT32) ((Available.cx - RowInfo->Width) / (RowInfo->FlexItemCount - 1));
      } else {
        JustifyContentPadding = 0;
      }
      break;

    case UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_SPACE_AROUND:
      if (Available.cx > RowInfo->Width) {
        JustifyContentPadding = (UINT32) ((Available.cx - RowInfo->Width) / (RowInfo->FlexItemCount * 2));
      } else {
        JustifyContentPadding = 0;
      }
      Left = Pos->left + JustifyContentPadding;
      JustifyContentPadding = JustifyContentPadding * 2;
      break;
    }

    for (ItemIndex = 0; ItemIndex < RowInfo->ItemCount; ItemIndex++) {
      Item     = Control->Items[Count++];
      ItemSize = &RowInfo->ItemsSize[ItemIndex];

      if ((GetWindowLongPtr (Item->Wnd, GWL_STYLE) & WS_VISIBLE) == 0) {
        continue;
      }
      if (Item->Float || Item->Position == UI_POSITION_ABSOLUTE) {
        if (ItemSize->cx == 0 || ItemSize->cx == MATCH_PARENT) {
          ItemSize->cx = Available.cx;
        }
        if (ItemSize->cy == 0 || ItemSize->cy == MATCH_PARENT) {
          ItemSize->cy = Available.cy;
        }

        Point = CalculateControlDisplayXY (Item, ItemSize, &Item->Available);
        SetRect (
          &ItemRc,
          Point.x + GetControlMarginLeft (Item),
          Point.y + GetControlMarginTop (Item),
          Point.x + GetControlMarginLeft (Item) + ItemSize->cx,
          Point.y + GetControlMarginTop (Item) + ItemSize->cy
          );
        mSetPositionLevel++;
        CONTROL_CLASS (Item)->SetPosition (Item, &ItemRc);
        mSetPositionLevel--;
        continue;
      }

      switch (This->AlignItems) {

      case UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_FLEX_END:
        if (RowInfo->Height > ItemSize->cy) {
          AlignItemsPadding = RowInfo->Height - ItemSize->cy;
        } else {
          AlignItemsPadding = 0;
        }
        break;

      case UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_CENTER:
        if (RowInfo->Height > ItemSize->cy) {
          AlignItemsPadding = (RowInfo->Height - ItemSize->cy) / 2;
        } else {
          AlignItemsPadding = 0;
        }
        break;

      case UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_STRETCH:
        if (!IS_UI_LENGTH_VALUE_ASSIGNED (Item->Height) && RowInfo->Height > ItemSize->cy) {
          ItemSize->cy = RowInfo->Height;
        }
        break;

      default:
        break;
      }

      if ((ItemSize->cy == 0) || (ItemSize->cy == MATCH_PARENT)) {
        ItemSize->cy = RowInfo->Height;
      }

      Left += GetControlMarginLeft (Item);

      ItemRc.left   = Left;
      ItemRc.top    = Top + GetControlMarginTop (Item) + AlignItemsPadding;
      ItemRc.right  = ItemRc.left + ItemSize->cx - GetControlMarginWidth (Item);
      ItemRc.bottom = ItemRc.top  + ItemSize->cy - GetControlMarginHeight (Item);

      mSetPositionLevel++;
      CONTROL_CLASS (Item)->SetPosition (Item, &ItemRc);
      mSetPositionLevel--;

      Left = ItemRc.right + GetControlMarginRight (Item) + JustifyContentPadding;
    }

    Top += RowInfo->Height + AlignContentPadding;
  }

  FreePool (RowInfoList->ItemsSize);
  FreePool (RowInfoList);
}

STATIC
VOID
UiFlexLayoutSetChildPositionByColumn (
  UI_CONTROL                   *Control,
  CONST RECT                   *Pos
  )
{
  UI_FLEX_LAYOUT               *This;
  UI_CONTROL                   *Item;
  UINTN                        Index;
  UINTN                        ItemIndex;
  UINTN                        Count;
  RECT                         ItemRc;
  UINT32                       Left;
  UINT32                       Top;
  UINT32                       JustifyContentPadding;
  UINT32                       AlignContentPadding;
  UINT32                       AlignItemsPadding;
  SIZE                         Available;
  SIZE                         *ItemSize;
  POINT                        Point;
  UINT32                       ColumnInfoCount;
  UI_FLEX_LAYOUT_COLUMN_INFO   *ColumnInfoList;
  UI_FLEX_LAYOUT_COLUMN_INFO   *ColumnInfo;
  EFI_STATUS                   Status;
  INT32                        Width;

  This = (UI_FLEX_LAYOUT *) Control;

  Available.cx = Pos->right - Pos->left;
  Available.cy = Pos->bottom - Pos->top;

  Status = UiFlexLayoutGetColumnInfo (Control, Available, &ColumnInfoCount, &ColumnInfoList);
  if (EFI_ERROR (Status)) {
    return;
  }

  Top                   = Pos->top;
  Left                  = Pos->left;
  AlignItemsPadding     = 0;
  AlignContentPadding   = 0;
  JustifyContentPadding = 0;

  switch (This->AlignContent) {

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_STRETCH:
    Left = Pos->left;
    for (Index = 0, Width = 0; Index < ColumnInfoCount; Index++) {
      Width += ColumnInfoList[Index].Width;
    }
    if (Available.cx > Width) {
      for (Index = 0; Index < ColumnInfoCount; Index++) {
        ColumnInfoList[Index].Width += (Available.cx - Width) / ColumnInfoCount;
      }
    }
    break;

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_FLEX_START:
    Left = Pos->left;
    break;

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_FLEX_END:
    Left = Pos->right;
    for (Index = 0; Index < ColumnInfoCount; Index++) {
      Left -= ColumnInfoList[Index].Width;
    }
    break;

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_CENTER:
    for (Index = 0, Width = 0; Index < ColumnInfoCount; Index++) {
      Width += ColumnInfoList[Index].Width;
    }
    Left = Pos->left + (Available.cx - Width) / 2;
    break;

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_SPACE_BETWEEN:
    Left = Pos->left;
    if (ColumnInfoCount > 1) {
      for (Index = 0, Width = 0; Index < ColumnInfoCount; Index++) {
        Width += ColumnInfoList[Index].Width;
      }
      if (Available.cx > Width) {
        AlignContentPadding = (Available.cx - Width) / (ColumnInfoCount - 1);
      }
    }
    break;

  case UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_SPACE_AROUND:
    for (Index = 0, Width = 0; Index < ColumnInfoCount; Index++) {
      Width += ColumnInfoList[Index].Width;
    }
    if (Available.cx > Width) {
      AlignContentPadding = (Available.cx - Width) / (ColumnInfoCount * 2);
    }

    Left = Pos->left + AlignContentPadding;
    AlignContentPadding = AlignContentPadding * 2;
    break;
  }

  for (Index = 0, Count = 0; Index < ColumnInfoCount; Index++) {
    ColumnInfo = &ColumnInfoList[Index];

    switch (This->JustifyContent) {

    case UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_FLEX_START:
      Top = Pos->top;
      break;

    case UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_FLEX_END:
      Top = Pos->bottom - ColumnInfo->Height;
      break;

    case UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_CENTER:
      Top = Pos->top + (Available.cy - ColumnInfo->Height) / 2;
      break;

    case UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_SPACE_BETWEEN:
      Top = Pos->top;
      if (Available.cy > ColumnInfo->Height && ColumnInfo->FlexItemCount > 1) {
        JustifyContentPadding = (UINT32) ((Available.cy - ColumnInfo->Height) / (ColumnInfo->FlexItemCount - 1));
      } else {
        JustifyContentPadding = 0;
      }
      break;

    case UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_SPACE_AROUND:
      if (Available.cy > ColumnInfo->Height) {
        JustifyContentPadding = (UINT32) ((Available.cy - ColumnInfo->Height) / (ColumnInfo->FlexItemCount * 2));
      } else {
        JustifyContentPadding = 0;
      }
      Top = Pos->top + JustifyContentPadding;
      JustifyContentPadding = JustifyContentPadding * 2;
      break;
    }

    for (ItemIndex = 0; ItemIndex < ColumnInfo->ItemCount; ItemIndex++) {
      Item     = Control->Items[Count++];
      ItemSize = &ColumnInfo->ItemsSize[ItemIndex];

      if ((GetWindowLongPtr (Item->Wnd, GWL_STYLE) & WS_VISIBLE) == 0) {
        continue;
      }
      if (Item->Float || Item->Position == UI_POSITION_ABSOLUTE) {
        if (ItemSize->cx == 0 || ItemSize->cx == MATCH_PARENT) {
          ItemSize->cx = Available.cx;
        }
        if (ItemSize->cy == 0 || ItemSize->cy == MATCH_PARENT) {
          ItemSize->cy = Available.cy;
        }

        Point = CalculateControlDisplayXY (Item, ItemSize, &Item->Available);
        SetRect (
          &ItemRc,
          Point.x + GetControlMarginLeft (Item),
          Point.y + GetControlMarginTop (Item),
          Point.x + GetControlMarginLeft (Item) + ItemSize->cx,
          Point.y + GetControlMarginTop (Item) + ItemSize->cy
          );
        mSetPositionLevel++;
        CONTROL_CLASS (Item)->SetPosition (Item, &ItemRc);
        mSetPositionLevel--;
        continue;
      }

      switch (This->AlignItems) {

      case UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_FLEX_END:
        if (ColumnInfo->Width > ItemSize->cx) {
          AlignItemsPadding = ColumnInfo->Width - ItemSize->cx;
        } else {
          AlignItemsPadding = 0;
        }
        break;

      case UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_CENTER:
        if (ColumnInfo->Width > ItemSize->cx) {
          AlignItemsPadding = (ColumnInfo->Width - ItemSize->cx) / 2;
        } else {
          AlignItemsPadding = 0;
        }
        break;

      case UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_STRETCH:
        if (!IS_UI_LENGTH_VALUE_ASSIGNED (Item->Width) && ColumnInfo->Width > ItemSize->cx) {
          ItemSize->cx = ColumnInfo->Width;
        }
        break;

      default:
        break;
      }

      if ((ItemSize->cx == 0) || (ItemSize->cx == MATCH_PARENT)) {
        ItemSize->cx = ColumnInfo->Width;
      }

      Top += GetControlMarginTop (Item);

      ItemRc.left   = Left + GetControlMarginLeft (Item)+ AlignItemsPadding;
      ItemRc.top    = Top;
      ItemRc.right  = ItemRc.left + ItemSize->cx - GetControlMarginWidth (Item);
      ItemRc.bottom = ItemRc.top  + ItemSize->cy - GetControlMarginHeight (Item);

      mSetPositionLevel++;
      CONTROL_CLASS (Item)->SetPosition (Item, &ItemRc);
      mSetPositionLevel--;

      Top = ItemRc.bottom + GetControlMarginBottom (Item) + JustifyContentPadding;
    }

    Left += ColumnInfo->Width + AlignContentPadding;
  }

  FreePool (ColumnInfoList->ItemsSize);
  FreePool (ColumnInfoList);
}

VOID
UiFlexLayoutSetPosition (
  UI_CONTROL                   *Control,
  CONST RECT                   *Pos
  )
{
  UI_FLEX_LAYOUT               *This;
  RECT                         Rc;

  This = (UI_FLEX_LAYOUT *) Control;

  ShowSetPositionDbgInfo (Control, Pos);
  GetWindowRect(Control->Wnd, &Rc);
  MapWindowPoints(HWND_DESKTOP, GetParent(Control->Wnd), (LPPOINT) &Rc, 2);
  if (!EqualRect (&Rc, Pos)) {
    ShowSetPositionDbgInfo (Control, Pos);
    MoveWindow (Control->Wnd, Pos->left, Pos->top, Pos->right - Pos->left, Pos->bottom - Pos->top, TRUE);
    CONTROL_CLASS_INVALIDATE (This);
    Control->NeedUpdateChildPos = TRUE;
  }
  if (Control->ItemCount == 0) {
    return ;
  }

  if (!Control->NeedUpdateChildPos) {
    return ;
  }
  Control->NeedUpdateChildPos = FALSE;

  CopyRect (&Rc, Pos);
  OffsetRect (&Rc, -Rc.left, -Rc.top);

  Rc.left   += (GetControlPaddingLeft (Control)   + GetControlBorderLeft (Control)  );
  Rc.top    += (GetControlPaddingTop (Control)    + GetControlBorderTop (Control)   );
  Rc.right  -= (GetControlPaddingRight (Control)  + GetControlBorderRight (Control) );
  Rc.bottom -= (GetControlPaddingBottom (Control) + GetControlBorderBottom (Control));

  if (Rc.left > Rc.right) {
    Rc.left = Rc.right;
  }
  if (Rc.top > Rc.bottom) {
    Rc.top = Rc.bottom;
  }

  if (This->FlexDirection == UI_FLEX_LAYOUT_FLEX_DIRECTION_TYPE_ROW) {
    UiFlexLayoutSetChildPositionByRow (Control, &Rc);
  } else {
    UiFlexLayoutSetChildPositionByColumn (Control, &Rc);
  }
}

BOOLEAN
EFIAPI
UiFlexLayoutSetAttribute (
  UI_CONTROL *Control,
  CHAR16     *Name,
  CHAR16     *Value
  )
{
  UI_FLEX_LAYOUT                *This;

  This = (UI_FLEX_LAYOUT *) Control;

  if (StrCmp (Name, L"flex-direction") == 0) {
    if (StrCmp (Value, L"row") == 0 || StrCmp (Value, L"none") == 0) {
      This->FlexDirection = UI_FLEX_LAYOUT_FLEX_DIRECTION_TYPE_ROW;
    } else if (StrCmp (Value, L"column") == 0) {
      This->FlexDirection = UI_FLEX_LAYOUT_FLEX_DIRECTION_TYPE_COLUMN;
    } else {
      DEBUG ((EFI_D_ERROR, "flex-direction can only use { none, row, column }"));
      ASSERT (FALSE);
    }
  } else if (StrCmp (Name, L"flex-wrap") == 0) {
    if (StrCmp (Value, L"nowrap") == 0) {
      This->FlexWrap = UI_FLEX_LAYOUT_FLEX_WRAP_TYPE_NO_WRAP;
    } else if (StrCmp (Value, L"wrap") == 0) {
      This->FlexWrap = UI_FLEX_LAYOUT_FLEX_WRAP_TYPE_WRAP;
    } else {
      DEBUG ((EFI_D_ERROR, "flex-wrap can only use { nowrap, wrap}"));
      ASSERT (FALSE);
    }
  } else if (StrCmp (Name, L"justify-content") == 0) {
    if (StrCmp (Value, L"flex-start") == 0) {
      This->JustifyContent = UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_FLEX_START;
    } else if (StrCmp (Value, L"flex-end") == 0) {
      This->JustifyContent = UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_FLEX_END;
    } else if (StrCmp (Value, L"center") == 0) {
      This->JustifyContent = UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_CENTER;
    } else if (StrCmp (Value, L"space-between") == 0) {
      This->JustifyContent = UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_SPACE_BETWEEN;
    } else if (StrCmp (Value, L"space-around") == 0) {
      This->JustifyContent = UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_SPACE_AROUND;
    } else {
      DEBUG ((EFI_D_ERROR, "justify-content can only use { flex-start, flex-end, center, space-between, space-around }"));
      ASSERT (FALSE);
    }
  } else if (StrCmp (Name, L"align-content") == 0) {
    if (StrCmp (Value, L"stretch") == 0) {
      This->AlignContent = UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_STRETCH;
    } else if (StrCmp (Value, L"flex-start") == 0) {
      This->AlignContent = UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_FLEX_START;
    } else if (StrCmp (Value, L"flex-end") == 0) {
      This->AlignContent = UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_FLEX_END;
    } else if (StrCmp (Value, L"center") == 0) {
      This->AlignContent = UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_CENTER;
    } else if (StrCmp (Value, L"space-between") == 0) {
      This->AlignContent = UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_SPACE_BETWEEN;
    } else if (StrCmp (Value, L"space-around") == 0) {
      This->AlignContent = UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_SPACE_AROUND;
    } else {
      DEBUG ((EFI_D_ERROR, "align-content can only use { flex-start, flex-end, center, space-between, space-around, stretch }"));
      ASSERT (FALSE);
    }
  } else if (StrCmp (Name, L"align-items") == 0) {
    if (StrCmp (Value, L"flex-start") == 0) {
      This->AlignItems = UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_FLEX_START;
    } else if (StrCmp (Value, L"flex-end") == 0) {
      This->AlignItems = UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_FLEX_END;
    } else if (StrCmp (Value, L"center") == 0) {
      This->AlignItems = UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_CENTER;
    } else if (StrCmp (Value, L"stretch") == 0) {
      This->AlignItems = UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_STRETCH;
    } else {
      DEBUG ((EFI_D_ERROR, "align-items can only use { flex-start, flex-end, center, stretch }"));
      ASSERT (FALSE);
    }
  } else {
    return PARENT_CLASS_SET_ATTRIBUTE (CURRENT_CLASS, Control, Name, Value);
  }

  CONTROL_CLASS_INVALIDATE (This);
  return TRUE;
}

LRESULT
EFIAPI
UiFlexLayoutProc (
  HWND   Wnd,
  UINT32 Msg,
  WPARAM WParam,
  LPARAM LParam
  )
{
  UI_FLEX_LAYOUT                *This;
  UI_CONTROL                    *Control;

  This = (UI_FLEX_LAYOUT *) GetWindowLongPtr (Wnd, 0);
  if (This == NULL && Msg != WM_CREATE && Msg != WM_NCCALCSIZE) {
    ASSERT (FALSE);
    return 0;
  }
  Control = (UI_CONTROL *)This;

  switch (Msg) {

  case WM_CREATE:
    This = (UI_FLEX_LAYOUT *) AllocateZeroPool (sizeof (UI_FLEX_LAYOUT));
    if (This != NULL) {
      CONTROL_CLASS (This) = (UI_CONTROL_CLASS *) GetClassLongPtr (Wnd, 0);
      SetWindowLongPtr (Wnd, 0, (INTN)This);
      SendMessage (Wnd, UI_NOTIFY_CREATE, WParam, LParam);
    }
    break;

  case UI_NOTIFY_CREATE:
    PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
    break;

  case WM_NCHITTEST:
    return HTTRANSPARENT;

  default:
    return PARENT_CLASS_WNDPROC (CURRENT_CLASS, Wnd, Msg, WParam, LParam);
  }

  return 0;
}

UI_FLEX_LAYOUT_CLASS *
EFIAPI
GetFlexLayoutClass (
  VOID
  )
{
  if (CURRENT_CLASS != NULL) {
    return CURRENT_CLASS;
  }

  InitUiClass ((UI_CONTROL_CLASS **)&CURRENT_CLASS, sizeof (*CURRENT_CLASS), L"FlexLayout", (UI_CONTROL_CLASS *)GetControlClass());
  if (CURRENT_CLASS == NULL) {
    return NULL;
  }
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->WndProc      = UiFlexLayoutProc;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetPosition  = UiFlexLayoutSetPosition;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->EstimateSize = UiFlexLayoutEstimateSize;
  ((UI_CONTROL_CLASS *)CURRENT_CLASS)->SetAttribute = UiFlexLayoutSetAttribute;
  return CURRENT_CLASS;
}

