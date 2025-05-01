/** @file
 Structure and function definition for Layout Library

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

#ifndef _LAYOUT_LIB_H_
#define _LAYOUT_LIB_H_

#include <Uefi.h>
#include <Protocol/H2OFormBrowser.h>
#include <Protocol/H2ODisplayEngine.h>
#include <Protocol/HiiFont.h>

#include <Library/RectLib.h>

#define KEYWORD_ALIGN_CONTENT_FLEX_START         BIT0
#define KEYWORD_ALIGN_CONTENT_FLEX_END           BIT1
#define KEYWORD_ALIGN_CONTENT_CENTER             BIT2
#define KEYWORD_ALIGN_CONTENT_SPACE_BETWEEN      BIT3
#define KEYWORD_ALIGN_CONTENT_SPACE_AROUND       BIT4
#define KEYWORD_ALIGN_CONTENT_STRETCH            BIT5

#define KEYWORD_ALIGN_ITEMS_FLEX_START           BIT0
#define KEYWORD_ALIGN_ITEMS_FLEX_END             BIT1
#define KEYWORD_ALIGN_ITEMS_CENTER               BIT2
#define KEYWORD_ALIGN_ITEMS_STRETCH              BIT3

#define KEYWORD_BACKGROUND_REPEAT_NO_REPEAT      BIT0
#define KEYWORD_BACKGROUND_REPEAT_REPEAT_X       BIT1
#define KEYWORD_BACKGROUND_REPEAT_REPEAT_Y       BIT2
#define KEYWORD_BACKGROUND_REPEAT_REPEAT         BIT3

#define KEYWORD_BORDER_SHADOW_NONE               BIT0
#define KEYWORD_BORDER_SHADOW_OUTSET             BIT1
#define KEYWORD_BORDER_SHADOW_INSET              BIT2

#define KEYWORD_BORDER_STYLE_NONE                BIT0
#define KEYWORD_BORDER_STYLE_SOLID               BIT1
#define KEYWORD_BORDER_STYLE_DOTTED              BIT2
#define KEYWORD_BORDER_STYLE_DASHED              BIT3
#define KEYWORD_BORDER_STYLE_DOUBLE              BIT4
#define KEYWORD_BORDER_STYLE_GROOVE              BIT5
#define KEYWORD_BORDER_STYLE_RIDGE               BIT6
#define KEYWORD_BORDER_STYLE_INSET               BIT7
#define KEYWORD_BORDER_STYLE_OUTSET              BIT8
#define KEYWORD_BORDER_STYLE_HIDDEN              BIT9

#define KEYWORD_DISPLAY_NONE                     BIT0
#define KEYWORD_DISPLAY_INLINE                   BIT1
#define KEYWORD_DISPLAY_BLOCK                    BIT2
#define KEYWORD_DISPLAY_FLEX                     BIT3

#define KEYWORD_FILTER_NONE                      BIT0
#define KEYWORD_FILTER_BRIGHTNESS                BIT1
#define KEYWORD_FILTER_GRAYSCALE                 BIT2

#define KEYWORD_FLEX_DIRECTION_NONE              BIT0
#define KEYWORD_FLEX_DIRECTION_ROW               BIT1
#define KEYWORD_FLEX_DIRECTION_COLUMN            BIT2

#define KEYWORD_FLEX_WRAP_NOWRAP                 BIT0
#define KEYWORD_FLEX_WRAP_WRAP                   BIT1

#define KEYWORD_JUSTIFY_CONTENT_FLEX_START       BIT0
#define KEYWORD_JUSTIFY_CONTENT_FLEX_END         BIT1
#define KEYWORD_JUSTIFY_CONTENT_CENTER           BIT2
#define KEYWORD_JUSTIFY_CONTENT_SPACE_BETWEEN    BIT3
#define KEYWORD_JUSTIFY_CONTENT_SPACE_AROUND     BIT4

#define KEYWORD_LINEAR_GRADIENT_NONE             BIT0
#define KEYWORD_LINEAR_GRADIENT_DIRECTION_LEFT   BIT1
#define KEYWORD_LINEAR_GRADIENT_DIRECTION_RIGHT  BIT2
#define KEYWORD_LINEAR_GRADIENT_DIRECTION_TOP    BIT3
#define KEYWORD_LINEAR_GRADIENT_DIRECTION_BOTTOM BIT4
#define KEYWORD_LINEAR_GRADIENT_COLOR_START      BIT5
#define KEYWORD_LINEAR_GRADIENT_COLOR_STOP       BIT6
#define KEYWORD_LINEAR_GRADIENT_COLOR_END        BIT7

#define KEYWORD_OVERFLOW_AUTO                    BIT0
#define KEYWORD_OVERFLOW_VISIBLE                 BIT1
#define KEYWORD_OVERFLOW_HIDDEN                  BIT2
#define KEYWORD_OVERFLOW_SCROLL                  BIT3

#define KEYWORD_POSITION_ABSOLUTE                BIT0
#define KEYWORD_POSITION_RELATIVE                BIT1
#define KEYWORD_POSITION_FIXED                   BIT2
#define KEYWORD_POSITION_STATIC                  BIT3

#define KEYWORD_TEXT_ALIGN_LEFT                  BIT0
#define KEYWORD_TEXT_ALIGN_RIGHT                 BIT1
#define KEYWORD_TEXT_ALIGN_CENTER                BIT2
#define KEYWORD_TEXT_ALIGN_JUSTIFY               BIT3

#define KEYWORD_TEXT_CONTROL_ELLIPSIS            BIT0
#define KEYWORD_TEXT_CONTROL_NOWRAP              BIT1
#define KEYWORD_TEXT_CONTROL_WRAP                BIT2
#define KEYWORD_TEXT_CONTROL_TRUNCATE            BIT3

#define KEYWORD_VISIBILITY_VISIBLE               BIT0
#define KEYWORD_VISIBILITY_HIDDEN                BIT1
#define KEYWORD_VISIBILITY_INHERIT               BIT2

typedef struct _RGB_ATTR {
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL                Rgb;
  UINT32                                       Attribute;
} RGB_ATTR;

typedef struct _H2O_COLOR_INFO {
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL                Rgb;
  UINT32                                       Attribute;
} H2O_COLOR_INFO;

typedef struct _H2O_STRING_INFO {
  RECT                                         StringField;
  CHAR16                                       *String;
} H2O_STRING_INFO;

typedef struct _H2O_IMAGE_INFO {
  RECT                                         ImageField;

  CHAR16                                       *ImageMapStr;
  EFI_IMAGE_ID                                 ImageId;
  EFI_ANIMATION_ID                             AnimationId;
  EFI_EVENT                                    AnimationRefreshEvent;

  EFI_GRAPHICS_OUTPUT_BLT_PIXEL                *CurrentBlt;
  UINT32                                       CurrentBltWidth;
  UINT32                                       CurrentBltHeight;
} H2O_IMAGE_INFO;

typedef enum {
  H2O_CSS_LENGTH_TYPE_PX = 0,
  H2O_CSS_LENGTH_TYPE_EM,
  H2O_CSS_LENGTH_TYPE_PERCENTAGE,
  H2O_CSS_LENGTH_TYPE_AUTO,
  H2O_CSS_LENGTH_TYPE_MAX
} H2O_CSS_LENGTH_TYPE;

typedef struct _H2O_CSS_LENGTH {
  UINT8                                        Type;
  union {
    //
    // Relative
    //
    UINT32                                     Px; ///< Pixels
    UINT32                                     Em; ///< Relative to font size
    UINT32                                     Ex; ///< Relative to Lowercase x height
    UINT32                                     Percentage;
    BOOLEAN                                    Auto;
    //
    // Absolute
    //
    UINT32                                     In; ///< Inch
    UINT32                                     Cm; ///< Centimeter
    UINT32                                     Mm; ///< Millimeter
    UINT32                                     Pt; ///< points, 1 pt = 1/72 in
    UINT32                                     Pc; ///< picas, 1 pc = 12 pt
  } Value;
} H2O_CSS_LENGTH;

typedef struct {
  UINT32                                       Type;
  H2O_VALUE                                    H2OValue;
  union {
    H2O_CSS_LENGTH                             Length;
    H2O_COLOR_INFO                             Color;
    H2O_IMAGE_INFO                             Image;
    RECT                                       Rect;
  } Value;
} H2O_PROPERTY_VALUE;

typedef struct _H2O_PANEL_INFO H2O_PANEL_INFO;
typedef struct _H2O_LAYOUT_INFO H2O_LAYOUT_INFO;

#define H2O_PROPERTY_INFO_NODE_SIGNATURE SIGNATURE_32 ('P', 'R', 'O', 'P')
typedef struct _H2O_PROPERTY_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  UINT32                                       ValueType;
  H2O_VALUE                                    H2OValue;
  CHAR8                                        *IdentifierStr;
  CHAR8                                        *ValueStr;
  UINT32                                       IdentifierStrSize;
} H2O_PROPERTY_INFO;
#define H2O_PROPERTY_INFO_NODE_FROM_LINK(a) CR (a, H2O_PROPERTY_INFO, Link, H2O_PROPERTY_INFO_NODE_SIGNATURE)

#define H2O_STYLE_INFO_NODE_SIGNATURE SIGNATURE_32 ('S', 'T', 'Y', 'L')
typedef struct _H2O_STYLE_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;

  UINT32                                       StyleType;          ///< From styletypekeyword. See H2O_IFR_STYLE_TYPE_xxx.
  CHAR8                                        *ClassName;
  UINT32                                       PseudoClass;        ///< See H2O_STYLE_PSEUDO_CLASS_xxxx.

  LIST_ENTRY                                   PropertyListHead;
  UINT32                                       PropertyCount;
} H2O_STYLE_INFO;
#define H2O_STYLE_INFO_NODE_FROM_LINK(a) CR (a, H2O_STYLE_INFO, Link, H2O_STYLE_INFO_NODE_SIGNATURE)

#define H2O_IFR_STYLE_TYPE_VFR_TREE_FLAG       0x80

typedef enum {
  H2O_STYLE_PSEUDO_CLASS_NORMAL = 0,
  H2O_STYLE_PSEUDO_CLASS_SELECTABLE,
  H2O_STYLE_PSEUDO_CLASS_GRAYOUT,
  H2O_STYLE_PSEUDO_CLASS_HIGHLIGHT,
  H2O_STYLE_PSEUDO_CLASS_LINK,
  H2O_STYLE_PSEUDO_CLASS_VISITED,
  H2O_STYLE_PSEUDO_CLASS_HOVER,
  H2O_STYLE_PSEUDO_CLASS_FOCUS,
  H2O_STYLE_PSEUDO_CLASS_ACTIVE,
  H2O_STYLE_PSEUDO_CLASS_ENABLED,
  H2O_STYLE_PSEUDO_CLASS_DISABLED,
  H2O_STYLE_PSEUDO_CLASS_NOT,
  H2O_STYLE_PSEUDO_CLASS_HELP,
  H2O_STYLE_PSEUDO_CLASS_MAX
} H2O_STYLE_PSEUDO_CLASS;

typedef enum {
  H2O_CONTROL_ID_DIALOG_TITLE = 1,
  H2O_CONTROL_ID_DIALOG_BODY,
  H2O_CONTROL_ID_DIALOG_BODY_INPUT,
  H2O_CONTROL_ID_DIALOG_BUTTON,
  H2O_CONTROL_ID_MAX
} H2O_CONTROL_ID;

typedef struct _H2O_CONTROL_INFO {
  UINT32                                       ControlId;
  H2O_STYLE_INFO                               ControlStyle;

  RECT                                         ControlField;
  RECT                                         ControlRelativeField; ///< from panel

  H2O_STRING_INFO                              Text;
  H2O_STRING_INFO                              HelpText;
  H2O_STRING_INFO                              ValueStrInfo;
  H2O_IMAGE_INFO                               BackgroundImage;
  EFI_IMAGE_INPUT                              HelpImage;
  EFI_HII_VALUE                                HiiValue;

  H2O_PANEL_INFO                               *ParentPanel;
  BOOLEAN                                      Editable;
  //
  // Statement
  //
  H2O_PAGE_ID                                  PageId;
  H2O_STATEMENT_ID                             StatementId;
  EFI_QUESTION_ID                              QuestionId;
  UINT8                                        Operand;
  EFI_IFR_OP_HEADER                            *IfrOpCode;
  BOOLEAN                                      Visible;
  BOOLEAN                                      Selectable;
  BOOLEAN                                      Modifiable;

  UINT64                                       Minimum;
  UINT64                                       Maximum;
  UINT64                                       Step;

  //
  // Display item sequence for date/time
  //  Date:      Month/Day/Year
  //  Sequence:  0     1   2
  //
  //  Time:      Hour : Minute : Second
  //  Sequence:  0      1        2
  //
  UINT8                                        Sequence;
} H2O_CONTROL_INFO;

typedef struct _H2O_CONTROL_LIST {
  UINT32                                       Count;
  H2O_CONTROL_INFO                             *ControlArray;
} H2O_CONTROL_LIST;

#define H2O_PANEL_INFO_NODE_SIGNATURE SIGNATURE_32 ('P', 'A', 'N', 'E')
struct _H2O_PANEL_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  UINT32                                       PanelId;
  UINT32                                       PanelType; ///< See H2O_PANEL_TYPE_xxx.
  EFI_GUID                                     PanelTypeGuid;

  EFI_GUID                                     TargetFormsetGuid; //< For panel type H2O_PANEL_TYPE_FORM
  UINT16                                       TargetFormId;

  RECT                                         PanelField;
  RECT                                         PanelRelField; ///< for page-up and page-down

  H2O_IMAGE_INFO                               ContentsImage;
  H2O_IMAGE_INFO                               BackgroundImage;

  UINT32                                       OrderFlag; ///< See H2O_PANEL_ORDER_FLAG_xxxx.
  BOOLEAN                                      WithScrollBarVertical;
  BOOLEAN                                      WithScrollBarHorizontal;

  H2O_LAYOUT_INFO                              *ParentLayout;
  LIST_ENTRY                                   StyleListHead;
  H2O_CONTROL_LIST                             ControlList;
} ;
#define H2O_PANEL_INFO_NODE_FROM_LINK(a) CR (a, H2O_PANEL_INFO, Link, H2O_PANEL_INFO_NODE_SIGNATURE)

#define H2O_PANEL_ORDER_FLAG_HORIZONTAL_FIRST  0x00000001 ///< Default Vertical First Order
#define H2O_PANEL_ORDER_FLAG_NO_SCROLL_BAR     0x00000002
#define H2O_PANEL_ORDER_FLAG_JUSTIFIED         0x00000004
#define H2O_PANEL_ORDER_FLAG_MINIMUM           0x00000008

#define DISPLAY_NONE                           0x00000000 ///< Default
#define DISPLAY_LEFT                           0x00000001
#define DISPLAY_TOP                            0x00000002
#define DISPLAY_RIGHT                          0x00000004
#define DISPLAY_BOTTOM                         0x00000008

typedef struct _H2O_PANEL_LIST {
  UINT32                                       Count;
  H2O_PANEL_INFO                               *PanelArray;
} H2O_PANEL_LIST;

#define H2O_LAYOUT_INFO_NODE_SIGNATURE SIGNATURE_32 ('L', 'A', 'Y', 'O')
struct _H2O_LAYOUT_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  UINT32                                       LayoutId;
  EFI_GUID                                     DisplayEngineGuid[3];
  UINT32                                       Revision;
  LIST_ENTRY                                   PanelListHead;
  LIST_ENTRY                                   HotkeyListHead;
  LIST_ENTRY                                   StyleListHead;
};
#define H2O_LAYOUT_INFO_NODE_FROM_LINK(a) CR (a, H2O_LAYOUT_INFO, Link, H2O_LAYOUT_INFO_NODE_SIGNATURE)

typedef enum {
  LAYOUT_REVISION_UNKNOWN = 0, // not init
  LAYOUT_REVISION_1,           // visibility property value type is boolean, layout id is a property
  LAYOUT_REVISION_2,           // visibility property value type is keyword, layout id is an attribute
  LAYOUT_REVISION_MAX
} LAYOUT_REVISION;

typedef struct _H2O_VFR_INFO H2O_VFR_INFO;
typedef struct _H2O_FORMSET_INFO H2O_FORMSET_INFO;
typedef struct _H2O_FORM_INFO H2O_FORM_INFO;
typedef struct _H2O_STATEMENT_INFO H2O_STATEMENT_INFO;
typedef struct _H2O_HOTKEY_INFO H2O_HOTKEY_INFO;
typedef struct _H2O_IMPORT_INFO H2O_IMPORT_INFO;
typedef struct _H2O_TARGET_INFO H2O_TARGET_INFO;
typedef struct _H2O_EXPRESSION_INFO H2O_EXPRESSION_INFO;
typedef struct _H2O_EXPRESSION_QID_MAP_INFO H2O_EXPRESSION_QID_MAP_INFO;

#define H2O_HOTKEY_INFO_NODE_SIGNATURE SIGNATURE_32 ('H', 'O', 'T', 'K')
struct _H2O_HOTKEY_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  H2O_HII_LIBT_EXT_HOTKEY_BLOCK                *HotkeyBlock;
  LIST_ENTRY                                   StyleListHead;
};
#define H2O_HOTKEY_INFO_NODE_FROM_LINK(a) CR (a, H2O_HOTKEY_INFO, Link, H2O_HOTKEY_INFO_NODE_SIGNATURE)

#define H2O_IMPORT_INFO_NODE_SIGNATURE SIGNATURE_32 ('I', 'M', 'P', 'O')
struct _H2O_IMPORT_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  UINT16                                       Flags;
  EFI_GUID                                     TargetFormsetGuid;
  EFI_FORM_ID                                  TargetFormId;
  EFI_QUESTION_ID                              TargetId;
  EFI_QUESTION_ID                              LocalId;
};
#define H2O_IMPORT_INFO_NODE_FROM_LINK(a) CR (a, H2O_IMPORT_INFO, Link, H2O_IMPORT_INFO_NODE_SIGNATURE)

#define H2O_TARGET_INFO_NODE_SIGNATURE SIGNATURE_32 ('H', 'T', 'I', 'N')
struct _H2O_TARGET_INFO {
  UINT32                                       Signature; 
  LIST_ENTRY                                   Link;
  UINT8                                        VfcfOpcode;
  EFI_GUID                                     TargetFormsetGuid;
  EFI_FORM_ID                                  TargetFormId;
  UINT16                                       TargetLabelId;
  LIST_ENTRY                                   StyleListHead;
};
#define H2O_TARGET_INFO_NODE_FROM_LINK(a) CR (a, H2O_TARGET_INFO, Link, H2O_TARGET_INFO_NODE_SIGNATURE)

#define H2O_EXPRESSION_INFO_NODE_SIGNATURE SIGNATURE_32 ('H', 'E', 'I', 'N')
struct _H2O_EXPRESSION_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  UINTN                                        ExpressionSize;
  UINT8                                        ExpressionOpcodes[1];
};
#define H2O_EXPRESSION_INFO_NODE_FROM_LINK(a) CR (a, H2O_EXPRESSION_INFO, Link, H2O_EXPRESSION_INFO_NODE_SIGNATURE)

#define H2O_STATEMENT_INFO_NODE_SIGNATURE SIGNATURE_32 ('H', 'S', 'I', 'N')
struct _H2O_STATEMENT_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  BOOLEAN                                      IsQuestion;
  UINT16                                       Position;
  UINT16                                       StatementId;
  EFI_QUESTION_ID                              QuestionId;
  LIST_ENTRY                                   HotkeyListHead;
  LIST_ENTRY                                   StyleListHead;
  UINTN                                        ExpressionCount;
  LIST_ENTRY                                   ExpressionListHead;
};
#define H2O_STATEMENT_INFO_NODE_FROM_LINK(a) CR (a, H2O_STATEMENT_INFO, Link, H2O_STATEMENT_INFO_NODE_SIGNATURE)

#define H2O_FORM_INFO_NODE_SIGNATURE SIGNATURE_32 ('F', 'O', 'R', 'M')
struct _H2O_FORM_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  H2O_HII_LIBT_EXT_FORM_BLOCK                  *FormBlock;
  LIST_ENTRY                                   HotkeyListHead;
  LIST_ENTRY                                   StyleListHead;
  LIST_ENTRY                                   ImportListHead;
  LIST_ENTRY                                   StatementListHead;
  LIST_ENTRY                                   TargetListHead;
};
#define H2O_FORM_INFO_NODE_FROM_LINK(a) CR (a, H2O_FORM_INFO, Link, H2O_FORM_INFO_NODE_SIGNATURE)

#define H2O_EXPRESSION_QID_MAP_INFO_NODE_SIGNATURE SIGNATURE_32 ('E', 'Q', 'M', 'N')
struct _H2O_EXPRESSION_QID_MAP_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  H2O_HII_LIBT_EXPRESSION_QID_MAP_BLOCK        *ExpressionQIdMapBlock;
};
#define H2O_EXPRESSION_QID_MAP_INFO_NODE_FROM_LINK(a) CR (a, H2O_EXPRESSION_QID_MAP_INFO, Link, H2O_EXPRESSION_QID_MAP_INFO_NODE_SIGNATURE)


#define H2O_FORMSET_INFO_NODE_SIGNATURE SIGNATURE_32 ('F', 'M', 'S', 'T')
struct _H2O_FORMSET_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  EFI_GUID                                     FormsetId;
  H2O_HII_LIBT_EXT_FORMSET_BLOCK               *FormsetBlock;
  LIST_ENTRY                                   FormListHead;
  LIST_ENTRY                                   HotkeyListHead;
  LIST_ENTRY                                   StyleListHead;
  LIST_ENTRY                                   ExpressionQIdMapList;
};
#define H2O_FORMSET_INFO_NODE_FROM_LINK(a) CR (a, H2O_FORMSET_INFO, Link, H2O_FORMSET_INFO_NODE_SIGNATURE)

#define H2O_VFR_INFO_NODE_SIGNATURE SIGNATURE_32 ('V', 'F', 'R', 'I')
struct _H2O_VFR_INFO {
  UINT32                                       Signature;
  LIST_ENTRY                                   Link;
  UINT32                                       VfrId;
  LIST_ENTRY                                   FormsetListHead;
};
#define H2O_VFR_INFO_NODE_FROM_LINK(a) CR (a, H2O_VFR_INFO, Link, H2O_VFR_INFO_NODE_SIGNATURE)

typedef struct {
  CHAR16              *KeyStr;
  CHAR16              *PromptStrTokenName;
  CHAR16              *HelpStrTokenName;
  EFI_IMAGE_INPUT     *Image;
  BOOLEAN             Display;
  UINT8               GroupId;
  EFI_GUID            FormSetGuid;
  UINT16              FormId;
  UINT16              QuestionId;
  UINT32              HotKeyAction;
  UINT16              HotKeyDefaultId;
  EFI_GUID            HotKeyTargetFormSetGuid;
  UINT16              HotKeyTargetFormId;
  UINT16              HotKeyTargetQuestionId;
  EFI_HII_VALUE       HotKeyHiiValue;
} H2O_HOTKEY;

typedef enum {
  VFR_LABEL,
  VFR_IMPORT,
  VFR_IMPORT_TARGET,
  VFR_LINK_TARGET,
  VFCF_IMPORT,
  VFCF_IMPORT_TARGET,
  VFCF_LINK_TARGET,
} IMPORT_INFO_TYPE;

#define IMPORT_INFO_FLAG_IS_QUESTION(Flag) ((BOOLEAN) ((Flag & H2O_HII_IMPORT_QUESTION) == H2O_HII_IMPORT_QUESTION))

typedef struct {
  UINT16              Flags;
  EFI_GUID            SrcFormSetGuid;
  UINT16              SrcFormId;
  UINT16              SrcId;

  UINT16              DstFormId;
  UINT16              DstId;

  EFI_QUESTION_ID     ExpressionQId;

  //
  // For VFR Import Op
  //
  VOID                *VfrFakeStatement;
  IMPORT_INFO_TYPE    Type;
} IMPORT_INFO;

EFI_STATUS
ParseProperty (
  IN     RECT                                  *RelativeField OPTIONAL,
  IN     H2O_PROPERTY_INFO                     *Property,
  OUT    H2O_PROPERTY_VALUE                    *PropValue
  );

EFI_STATUS
GetImportInfoList (
  IN  EFI_GUID                                *FormsetGuid,
  OUT IMPORT_INFO                             **ImportInfoList,
  OUT UINT16                                  *ImportInfoCount
  );

EFI_STATUS
GetFormsetLayoutByGuid (
  IN  EFI_GUID                                *FormsetGuid,
  OUT H2O_FORMSET_INFO                        **Formset
  );

H2O_FORM_INFO *
GetFormLayoutByFormId (
  IN H2O_FORMSET_INFO                         *Formset,
  IN UINT32                                   FormId
  );

EFI_STATUS
GetPanelById (
  IN EFI_GUID                                    *FsGuid,
  IN EFI_GUID                                    *DEGuid,
  IN UINT32                                      PanelId,
  OUT H2O_PANEL_INFO                             **Panel
  );

H2O_STATEMENT_INFO *
GetStatementLayoutByQuestionId (
  IN H2O_FORMSET_INFO                            *Formset,
  IN H2O_FORM_INFO                               *FormInfo,
  IN BOOLEAN                                     IsQuestion,
  IN UINT16                                      Id
  );

H2O_PANEL_INFO *
GetNextPanel (
  IN H2O_LAYOUT_INFO                           *LayoutInfo,
  IN H2O_PANEL_INFO                            *PanelInfo
  );

EFI_STATUS
GetPropertyFromPropertyList (
  IN     LIST_ENTRY                            *PropertyListHead,
  IN     CHAR8                                 *IdentifierStr,
  IN     RECT                                  *RelativeField OPTIONAL,
  OUT    H2O_PROPERTY_INFO                     **Property,
  OUT    H2O_PROPERTY_VALUE                    *PropValue
  );

EFI_STATUS
GetPropertyFromStyleList (
  IN  LIST_ENTRY                              *StyleListHead,
  IN  UINT32                                  StyleType,
  IN  UINT32                                  PseudoClass,
  IN  CHAR8                                   *IdentifierStr,
  IN  RECT                                    *RelativeField OPTIONAL,
  OUT H2O_PROPERTY_INFO                       **Property,
  OUT H2O_PROPERTY_VALUE                      *PropValue
  );

EFI_STATUS
BltCopy(
  IN     EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *Blt,
  IN     UINTN                           BltStartX,
  IN     UINTN                           BltStartY,
  IN     UINTN                           BltWidth,
  IN     EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *DestBlt,
  IN     UINTN                           DestStartX,
  IN     UINTN                           DestStartY,
  IN     UINTN                           DestWidth,
  IN     UINTN                           DestHeight,
  IN     UINTN                           CopyWidth,
  IN     UINTN                           CopyHeight
  );

EFI_STATUS
BltCombine(
  IN     EFI_GRAPHICS_OUTPUT_BLT_PIXEL   **DestBlt,
  IN     EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *BgBlt,
  IN     EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *FgBlt,
  IN     UINTN                           BltWidth,
  IN     UINTN                           BltHeight
  );

EFI_STATUS
BltTransparent(
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL   **DestBlt,
  IN     EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *SrcBlt,
  IN     UINTN                           BltWidth,
  IN     UINTN                           BltHeight
  );

EFI_STATUS
CreateImageByColor (
  IN     EFI_GRAPHICS_OUTPUT_BLT_PIXEL       *Rgb,
  IN     RECT                                *Field,
  IN OUT EFI_IMAGE_INPUT                     *ImageInput
  );

EFI_STATUS
GetLayoutById (
  IN     UINT32                                  LayoutId,
  IN     EFI_GUID                                *DisplayEngineGuid,
  OUT    H2O_LAYOUT_INFO                         **Layout
  );

EFI_STATUS
EFIAPI
GetLayoutIdByGuid (
  IN     EFI_GUID                                *FormsetGuid,
  OUT    UINT32                                  *LayoutId
  );

EFI_STATUS
GetImageById (
  IN     UINT32                                ImageId,
  OUT    EFI_IMAGE_INPUT                       *ImageInput
  );

CHAR16 *
GetStringById (
  IN EFI_STRING_ID                             StringId
  );

EFI_STATUS
SeparateStringByFont (
  IN     CHAR16                                *String,
  IN     EFI_FONT_DISPLAY_INFO                 *FontDisplayInfo,
  IN     UINT32                                LimitWidth,
  OUT    EFI_IMAGE_OUTPUT                      **CharImageOutArray OPTIONAL,
  OUT    UINT32                                *SeparateStrCount,
  OUT    UINT32                                **SeparateStrOffsetArray OPTIONAL,
  OUT    UINT32                                **SeparateStrWidthArray OPTIONAL,
  OUT    UINTN                                 **SeparateStrArray
  );

EFI_STATUS
GetStringImageByFont (
  IN     CHAR16                                *String,
  IN     RECT                                  *Field,
  IN     EFI_FONT_DISPLAY_INFO                 *FontDisplayInfo,
  OUT    EFI_IMAGE_OUTPUT                      **ImageOutput
  );

BOOLEAN
IsVisibility (
  IN     H2O_PANEL_INFO                        *Panel,
  IN     H2O_VFR_PROPERTY                      *VfrProperty,
  IN     UINT32                                StyleType,
  IN     UINT32                                PseudoClass
  );

EFI_STATUS
GetPanelField (
  IN     H2O_PANEL_INFO                        *Panel,
  IN     H2O_VFR_PROPERTY                      *VfrProperty,
  IN     UINT32                                StyleType,
  IN     UINT32                                PseudoClass,
  IN     RECT                                  *RelativeField,
  OUT    RECT                                  *PanelField
  );

EFI_STATUS
GetForegroundColor (
  IN     H2O_PANEL_INFO                        *Panel,
  IN     H2O_VFR_PROPERTY                      *VfrProperty,
  IN     UINT32                                StyleType,
  IN     UINT32                                PseudoClass,
  OUT    H2O_COLOR_INFO                        *Color
  );

EFI_STATUS
GetBackgroundColor (
  IN     H2O_PANEL_INFO                        *Panel,
  IN     H2O_VFR_PROPERTY                      *VfrProperty,
  IN     UINT32                                StyleType,
  IN     UINT32                                PseudoClass,
  OUT    H2O_COLOR_INFO                        *Color
  );

EFI_STATUS
GetPanelColorAttribute (
  IN     H2O_PANEL_INFO                        *Panel,
  IN     H2O_VFR_PROPERTY                      *VfrProperty,
  IN     UINT32                                StyleType,
  IN     UINT32                                PseudoClass,
  OUT    UINT32                                *PanelColorAttribute
  );

UINT32
GetBorderWidth (
  IN     H2O_PANEL_INFO                        *Panel,
  IN     H2O_VFR_PROPERTY                      *VfrProperty,
  IN     UINT32                                StyleType,
  IN     UINT32                                PseudoClass
  );

EFI_STATUS
GetHeight(
  IN     H2O_PANEL_INFO                        *Panel,
  IN     H2O_VFR_PROPERTY                      *VfrProperty,
  IN     UINT32                                StyleType,
  IN     UINT32                                PseudoClass,
  IN     RECT                                  *RelativeField,
  OUT    INT32                                 *Height
  );

EFI_STATUS
GetWidth(
  IN     H2O_PANEL_INFO                        *Panel,
  IN     H2O_VFR_PROPERTY                      *VfrProperty,
  IN     UINT32                                StyleType,
  IN     UINT32                                PseudoClass,
  IN     RECT                                  *RelativeField,
  OUT    INT32                                 *Width
  );

EFI_STATUS
GetHotKeyList (
  IN  EFI_GUID                                *FormsetGuid,
  IN  H2O_FORM_ID                             FormId,
  OUT UINT32                                  *HotKeyInfoCount,
  OUT HOT_KEY_INFO                            **HotKeyInfo
  );

EFI_STATUS
GetHotKeyListByQuestion (
  IN  EFI_GUID                                *FormsetGuid,
  IN  H2O_FORM_ID                             FormId,
  IN  BOOLEAN                                 IsQuestion,
  IN  UINT16                                  Id,
  OUT UINT32                                  *HotKeyInfoCount,
  OUT HOT_KEY_INFO                            **HotKeyInfo
  );

EFI_STATUS
GetHotKeyListByLayoutInfo (
  IN  H2O_LAYOUT_INFO                          *LayoutInfo,
  IN  EFI_GUID                                 *FormsetGuid,
  IN  H2O_FORM_ID                              FormId,
  IN  EFI_QUESTION_ID                          QuestionId,
  OUT UINT32                                   *HotKeyInfoCount,
  OUT HOT_KEY_INFO                             **HotKeyInfo
  );

EFI_STATUS
GetVfrPropertyByQuestion (
  IN EFI_GUID                                  *FormsetId,
  IN UINT16                                    FormId OPTIONAL,
  IN UINT32                                    StatementId OPTIONAL,
  IN UINT64                                    Value OPTIONAL,
  OUT H2O_VFR_PROPERTY                         *VfrProperty
  );

EFI_STATUS
GetPopUpTargetByQuestion (
  IN H2O_FORM_BROWSER_Q                        *Question,
  IN H2O_PANEL_INFO                            *Panel,
  OUT EFI_GUID                                 *PopUpFormsetId,
  OUT UINT16                                   *PopUpFormId
  );

EFI_STATUS
GetFormPanelRect (
  IN EFI_GUID                                  *FormsetId,
  IN UINT16                                    FormId,
  IN H2O_PANEL_INFO                            *Panel,
  IN RECT                                      *RelativeField,
  OUT RECT                                     *FormPanelRect
  );

EFI_STATUS
GetStatementRect (
  IN H2O_FORM_BROWSER_S                        *Statement,
  IN H2O_PANEL_INFO                            *Panel,
  IN RECT                                      *RelativeField,
  OUT RECT                                     *StatementRect
  );

H2O_PANEL_INFO *
GetPanelInfo (
  IN H2O_LAYOUT_INFO                           *LayoutInfo,
  IN UINT32                                    PanelId
  );

EFI_STATUS
GetStatementPropValue (
  IN CHAR8                                     *PropStr,
  IN H2O_FORM_BROWSER_S                        *Statement,
  IN H2O_PANEL_INFO                            *Panel,
  IN UINT32                                    StyleType,
  IN UINT32                                    PseudoClass,
  IN RECT                                      *RelativeField,
  OUT H2O_PROPERTY_VALUE                       *PropValue
  );

UINT32
GetStyleTypeByOpCode (
  IN    UINT8                                  OpCode
  );

VOID
SafeFreePool(
  IN   VOID                                **Buffer
  );

UINT32
GetPseudoClassByStatement (
  IN H2O_FORM_BROWSER_S                        *Statement
  );

EFI_STATUS
GetPropertyFromVfrAndPanel (
  IN     H2O_PANEL_INFO                        *Panel,
  IN     H2O_VFR_PROPERTY                      *VfrProperty,
  IN     UINT32                                StyleType,
  IN     UINT32                                PseudoClass,
  IN     CHAR8                                 *IdentifierStr,
  IN     RECT                                  *RelativeField OPTIONAL,
  OUT    H2O_PROPERTY_INFO                     **Property,
  OUT    H2O_PROPERTY_VALUE                    *PropValue
  );

EFI_STATUS
GetVfcfLayoutPropertyList (
  IN  UINT32                                   LayoutId,
  IN  EFI_GUID                                 *DisplayEngineGuid,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  OUT H2O_PROPERTY_INFO                        ***PropertyList,
  OUT UINTN                                    *PropertyCount
  );

EFI_STATUS
GetVfcfPanelPropertyList (
  IN  H2O_PANEL_INFO                           *PanelInfo,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  OUT H2O_PROPERTY_INFO                        ***PropertyList,
  OUT UINTN                                    *PropertyCount
  );

EFI_STATUS
GetVfcfFormsetPropertyList (
  IN  EFI_GUID                                 *FormsetGuid,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  OUT H2O_PROPERTY_INFO                        ***PropertyList,
  OUT UINTN                                    *PropertyCount
  );

EFI_STATUS
GetVfcfFormsetPropertyValue (
  IN  EFI_GUID                                 *FormsetGuid,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  IN  CHAR8                                    *IdentifierStr,
  IN  RECT                                     *RelativeField OPTIONAL,
  OUT H2O_PROPERTY_INFO                        **PropertyInfo,
  OUT H2O_PROPERTY_VALUE                       *PropertyValue
  );

EFI_STATUS
GetVfcfFormPropertyList (
  IN  EFI_GUID                                 *FormsetGuid,
  IN  UINT16                                   FormId,
  IN  UINT32                                   StyleType,
  IN  UINT32                                   PseudoClass,
  OUT H2O_PROPERTY_INFO                        ***PropertyList,
  OUT UINTN                                    *PropertyCount
  );

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
  );

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
  );

BOOLEAN
HaveVfcfFormsetInfo (
  IN EFI_GUID                                  *FormsetGuid
  );

EFI_STATUS
GetVfcfStatementExpressionList (
  IN  EFI_GUID                                 *FormsetGuid,
  IN  UINTN                                    FormId,
  IN  BOOLEAN                                  IsQuestion,
  IN  UINT16                                   Id,
  IN  UINT16                                   Position,
  IN OUT UINTN                                 *NumberExpression,
  OUT H2O_EXPRESSION_INFO                      ***ExpressionInfoList
  );

BOOLEAN
HaveVfcfFormInfo (
  IN EFI_GUID                                  *FormsetGuid,
  IN UINTN                                     FormId
  );

EFI_STATUS
GetLayoutTreeAndVfrTree (
  OUT    LIST_ENTRY                              **LayoutListHead,
  OUT    LIST_ENTRY                              **VfrListHead
  );

#endif

