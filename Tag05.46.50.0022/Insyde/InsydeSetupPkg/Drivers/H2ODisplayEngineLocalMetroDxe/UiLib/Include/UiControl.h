/** @file

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

#ifndef _UI_CONTROL_H
#define _UI_CONTROL_H

#include <Protocol/H2OWindow.h>

typedef struct _UI_MANAGER       UI_MANAGER;

typedef struct _UI_CONTROL       UI_CONTROL;
typedef struct _UI_SCROLLBAR     UI_SCROLLBAR;
typedef struct _UI_CONTROL_CLASS UI_CONTROL_CLASS;
typedef struct _UI_LENGTH        UI_LENGTH;

#define CONTROL_CLASS(Control)                                        (((UI_CONTROL *)(Control))->Class)
#define CONTROL_CLASS_GET_PARENT(Control)                             (CONTROL_CLASS(Control)->GetParent((UI_CONTROL *)(Control)))
#define CONTROL_CLASS_INVALIDATE(Control)                             (CONTROL_CLASS(Control)->Invalidate ((UI_CONTROL *)(Control)))
#define CONTROL_CLASS_GET_STATE(Control)                              (CONTROL_CLASS(Control)->GetState((UI_CONTROL *)(Control)))
#define CONTROL_CLASS_SET_STATE(Control, Set, Clear)                  (CONTROL_CLASS(Control)->SetState((UI_CONTROL *)(Control), Set, Clear))
#define CONTROL_CLASS_WNDPROC(Control, Wnd, Msg, WParam, LParam)      (CONTROL_CLASS(Control)->WndProc(Wnd, Msg, WParam, LParam))

#define PARENT_CONTROL_CLASS(Class)                                   (((UI_CONTROL_CLASS *)(Class))->Parent)
#define PARENT_CLASS_SET_ATTRIBUTE(Class, Control, Name, Value)       ((PARENT_CONTROL_CLASS(Class)->SetAttribute)(Control, Name, Value))
#define PARENT_CLASS_WNDPROC(Class, Wnd, Msg, WParam, LParam)         ((PARENT_CONTROL_CLASS(Class)->WndProc)(Wnd, Msg, WParam, LParam))

EFI_STATUS
EFIAPI
InitUiClass (
  UI_CONTROL_CLASS              **Class,
  UINT32                        ClassSize,
  CHAR16                        *ControlName,
  UI_CONTROL_CLASS              *ParentClass
  );

UI_CONTROL_CLASS *
EFIAPI
GetControlClass (
  VOID
  );

UI_CONTROL *
EFIAPI
CreateControl (
  CHAR16     *ControlName,
  UI_CONTROL *Parent
  );

UI_CONTROL *
EFIAPI
CreateAddControl (
  CHAR16     *ControlName,
  UI_CONTROL *Parent
  );

UI_CONTROL *
EFIAPI
GetUiControl (
  HWND Hwnd
  );

VOID
EFIAPI
UiApplyAttributeList (
  UI_CONTROL *Control,
  CHAR16     *StrList
  );

//
// Flags used for control state
//
typedef enum {
  UISTYLE_CLICKABLE     = 0x00010000,
  UISTYLE_LONGCLICKABLE = 0x00020000,
  UISTYLE_FOCUSABLE     = 0x00040000,
} UI_STYLE;


//
// Flags used for controlling the paint
//
typedef enum {
  UISTATE_FOCUSED     = 0x00000001,
  UISTATE_SELECTED    = 0x00000002,
  UISTATE_DISABLED    = 0x00000004,
  UISTATE_HOVER       = 0x00000008,
  UISTATE_PRESSED     = 0x00000010,
  UISTATE_READONLY    = 0x00000020,
  UISTATE_CAPTURED    = 0x00000040,
  UISTATE_INVALIDATED = 0x00000080,
  UISTATE_PREPRESSED  = 0x00000100,
} UI_STATE;

typedef enum {
  PAINT_BKCOLOR,
  PAINT_BKIMAGE,
  PAINT_STATUSIMAGE,
  PAINT_TEXT,
  PAINT_BORDER,
  PAINT_ALL
} PAINT_TYPE;

typedef enum {
  UI_SEARCH_TYPE_ALL,
  UI_SEARCH_TYPE_ONLY_CHILD,
} UI_SEARCH_TYPE;

typedef enum {
  UI_POSITION_RELATIVE,
  UI_POSITION_ABSOLUTE,
//  UI_POSITION_STATIC,
//  UI_POSITION_FIXED,
} UI_POSITION_TYPE;

typedef enum {
  UI_Z_INDEX_TYPE_AUTO,  // Inherit from parent
  UI_Z_INDEX_TYPE_NUMBER,
} UI_Z_INDEX_TYPE;

typedef struct _UI_Z_INDEX {
  UI_Z_INDEX_TYPE               Type;
  INT32                         Value;
} UI_Z_INDEX;

typedef enum {
  MATCH_PARENT = -1,
  WRAP_CONTENT = -2,
} UI_LAYOUT_PARAM;

typedef enum {
  UI_LENGTH_TYPE_WRAP_CONTENT = -2,
  UI_LENGTH_TYPE_MATCH_PARENT = -1,
  UI_LENGTH_TYPE_UNDEFINED    =  0,
  UI_LENGTH_TYPE_PIXEL,
  UI_LENGTH_TYPE_PERCENTAGE,
  UI_LENGTH_TYPE_EM,
  UI_LENGTH_TYPE_MAX
} UI_LENGTH_TYPE;

typedef enum {
  UI_BOX_SIZING_CONTENT_BOX = 0,
  UI_BOX_SIZING_BORDER_BOX = 1,
  UI_BOX_SIZING_MAX
} UI_BOX_SIZING;

typedef struct _UI_LENGTH {
  UI_LENGTH_TYPE                Type;
  INT32                         Value;
} UI_LENGTH;

typedef struct _UI_SIZE {
  UI_LENGTH                     Width;
  UI_LENGTH                     Height;
} UI_SIZE;

typedef struct _UI_RECT {
  UI_LENGTH                     Left;
  UI_LENGTH                     Top;
  UI_LENGTH                     Right;
  UI_LENGTH                     Bottom;
} UI_RECT;

#define IS_UI_LENGTH_DEFINED(UiLength)            ((BOOLEAN)(UiLength.Type != UI_LENGTH_TYPE_UNDEFINED))
#define IS_UI_LENGTH_VALUE_ASSIGNED(UiLength)     ((BOOLEAN)((UiLength.Type == UI_LENGTH_TYPE_PIXEL) || (UiLength.Type == UI_LENGTH_TYPE_PERCENTAGE) || (UiLength.Type == UI_LENGTH_TYPE_EM)))

#define IS_CONTROL_WIDTH_DEFINED(Control)         ((BOOLEAN)((IS_UI_LENGTH_DEFINED(Control->Width) ) || (IS_UI_LENGTH_VALUE_ASSIGNED(Control->Left) && IS_UI_LENGTH_VALUE_ASSIGNED(Control->Right ))))
#define IS_CONTROL_HEIGHT_DEFINED(Control)        ((BOOLEAN)((IS_UI_LENGTH_DEFINED(Control->Height)) || (IS_UI_LENGTH_VALUE_ASSIGNED(Control->Top ) && IS_UI_LENGTH_VALUE_ASSIGNED(Control->Bottom))))
#define IS_CONTROL_WIDTH_VALUE_ASSIGNED(Control)  ((BOOLEAN)((IS_UI_LENGTH_VALUE_ASSIGNED(Control->Width )) || (IS_UI_LENGTH_VALUE_ASSIGNED(Control->Left) && IS_UI_LENGTH_VALUE_ASSIGNED(Control->Right ))))
#define IS_CONTROL_HEIGHT_VALUE_ASSIGNED(Control) ((BOOLEAN)((IS_UI_LENGTH_VALUE_ASSIGNED(Control->Height)) || (IS_UI_LENGTH_VALUE_ASSIGNED(Control->Top ) && IS_UI_LENGTH_VALUE_ASSIGNED(Control->Bottom))))

#define UI_COMMON_NOTIFY   (WM_USER + 0x100)

#define UI_NOTIFY_FIRST         (UI_COMMON_NOTIFY)

#define UI_NOTIFY_PAINT           (UI_COMMON_NOTIFY)
#define UI_NOTIFY_ACTIVATE        (UI_COMMON_NOTIFY + 1)
//#define UI_NOTIFY_SET_POSITION    (UI_COMMON_NOTIFY + 2)
#define UI_NOTIFY_CREATE          (UI_COMMON_NOTIFY + 3)
#define UI_NOTIFY_WINDOWINIT      (UI_COMMON_NOTIFY + 4)
#define UI_NOTIFY_CARRIAGE_RETURN (UI_COMMON_NOTIFY + 5)
#define UI_NOTIFY_CLICK           (UI_COMMON_NOTIFY + 6)
#define UI_NOTIFY_BLUR            (UI_COMMON_NOTIFY + 7)
#define UI_NOTIFY_EDITFOCUS       (UI_COMMON_NOTIFY + 8)
#define UI_NOTIFY_LAST            (UI_COMMON_NOTIFY + 9)
#define UI_NOTIFY_CHANGE          (UI_COMMON_NOTIFY + 10)

typedef
BOOLEAN
(EFIAPI *UI_CONTROL_SET_ATTRIBUTE) (
  UI_CONTROL *Control,
  CHAR16     *Name,
  CHAR16     *Value
  );

typedef
VOID
(EFIAPI *UI_CONTROL_SET_POSITION) (
  UI_CONTROL *Control,
  CONST RECT *Position
  );


typedef
UI_CONTROL *
(EFIAPI *UI_CONTROL_GET_INTERFACE) (
  UI_CONTROL *Control,
  CHAR16     *ClassName
  );

typedef
SIZE
(EFIAPI *UI_CONTROL_ESTIMATE_SIZE) (
  UI_CONTROL *Control,
  SIZE       AvailableSize
  );

typedef
VOID
(EFIAPI *UI_CONTROL_INVALIDATE) (
  UI_CONTROL *Control
  );

typedef
UI_CONTROL *
(EFIAPI *UI_CONTROL_FIND_NEXT_FOCUS) (
  UI_CONTROL *Control,
  UI_CONTROL *Focused,
  UINTN      Direction
  );

typedef
UI_CONTROL *
(EFIAPI *UI_CONTROL_GET_PARENT) (
  UI_CONTROL *Control
  );

typedef
BOOLEAN
(EFIAPI *UI_CONTROL_SHOULD_DELAY_CHILD_PRESSED_STATE) (
  UI_CONTROL                    *Control
  );

typedef
UI_CONTROL *
(EFIAPI *UI_CONTROL_FIND_CHILD_BY_NAME) (
  UI_CONTROL                    *Control,
  CHAR16                        *Name,
  UI_SEARCH_TYPE                SearchType
  );

typedef
BOOLEAN
(EFIAPI *UI_CONTROL_ADD_CHILD) (
  UI_CONTROL                    *Control,
  UI_CONTROL                    *Child
  );

typedef
BOOLEAN
(EFIAPI *UI_CONTROL_REMOVE_CHILD) (
  UI_CONTROL                    *Control,
  UI_CONTROL                    *Child
  );

typedef
BOOLEAN
(EFIAPI *UI_CONTROL_REMOVE_ALL_CHILD) (
  UI_CONTROL                    *Control
  );

typedef
VOID
(EFIAPI *UI_CONTROL_PAINT_ALL) (
  UI_CONTROL                    *Control,
  PAINT_TYPE                    PaintType
  );

typedef
UI_STATE
(EFIAPI *UI_CONTROL_GET_STATE) (
  UI_CONTROL                    *Control
  );

typedef
VOID
(EFIAPI *UI_CONTROL_ON_SET_STATE) (
  UI_CONTROL                    *Control,
  UI_STATE                      SetState,
  UI_STATE                      ClearState
  );

typedef
VOID
(EFIAPI *UI_CONTROL_SET_STATE) (
  UI_CONTROL                    *Control,
  UI_STATE                      SetState,
  UI_STATE                      ClearState
  );

struct _UI_CONTROL {
  HWND                          Wnd;
  UI_CONTROL_CLASS              *Class;

  //
  // origianl container
  //
  UINTN                         ItemCount;
  UI_CONTROL                    **Items;
  UINT32                        ChildPadding;

  UI_SCROLLBAR                  *VScrollBar;
  UI_SCROLLBAR                  *HScrollBar;

  INT32                         LastMotionX;
  INT32                         LastMotionY;

  BOOLEAN                       IsBeginDraged;
  BOOLEAN                       Dragged;

  POINT                         ScrollPos;

  INT32                         TouchSlop;

  BOOLEAN                       NeedUpdateChildPos;


  UI_STYLE                      StyleFlags;
  UI_STATE                      StateFlags;

  INT32                         LastX;
  INT32                         LastY;

  COLORREF                      BkColor;
  COLORREF                      FocusBkColor;
  COLORREF                      BorderColor;
  UI_LENGTH                     Left;          ///< CSS Left. (Left,Right) include padding and border, NOT include margin.
  UI_LENGTH                     Top;           ///< CSS Top.  (Top,Bottom) include padding and border, NOT include margin.
  UI_LENGTH                     Right;         ///< CSS Right
  UI_LENGTH                     Bottom;        ///< CSS Bottom
  UI_LENGTH                     Width;         ///< CSS box model: Width.  NOT include padding, border and margin.
  UI_LENGTH                     Height;        ///< CSS box model: Height. NOT include padding, border and margin.
  SIZE                          MinSize;
  SIZE                          MaxSize;
  SIZE                          Available; ///< In order to calculate % for padding/border/margin, record available size set by parent.
  UI_RECT                       Padding;
  UI_RECT                       Border;
  UI_RECT                       Margin;
  RECT                          Scale9Grid;
  VOID                          *UserData;
  HSV_DIFF_VALUE                HsvDiff;
  CHAR16                        *Name;
  UI_MANAGER                    *Manager;
  CHAR16                        *BkImage;
  EFI_IMAGE_INPUT               BkImageBuf;
  UI_SIZE                       BkImageSize;
  UINT32                        BkImageStyle;
  UINT32                        Filter;
  BOOLEAN                       Float;
  BOOLEAN                       GradientVertial;
  BOOLEAN                       AlwaysOnTop;
  UINT32                        TabOrder;
  UINT32                        FontSize;
  UINT32                        TextColor;
  UI_POSITION_TYPE              Position;
  UI_Z_INDEX                    ZIndex;
  UI_BOX_SIZING                 BoxSizing;
  UI_CONTROL_ON_SET_STATE       OnSetState;
};

struct _UI_CONTROL_CLASS {
  UI_CONTROL_CLASS              *Parent;
  UINT32                        ClassSize;
  CHAR16                        *ClassName;

  WNDPROC                       WndProc;
  UI_CONTROL_FIND_CHILD_BY_NAME FindChildByName;

  UI_CONTROL_ADD_CHILD          AddChild;
  UI_CONTROL_REMOVE_CHILD       RemoveChild;
  UI_CONTROL_REMOVE_ALL_CHILD   RemoveAllChild;


  UI_CONTROL_SET_ATTRIBUTE      SetAttribute;
  UI_CONTROL_SET_POSITION       SetPosition;

  UI_CONTROL_ESTIMATE_SIZE      EstimateSize;
  UI_CONTROL_INVALIDATE         Invalidate;
  UI_CONTROL_PAINT_ALL          PaintAll;
  UI_CONTROL_FIND_NEXT_FOCUS    FindNextFocus;
  UI_CONTROL_GET_PARENT         GetParent;

  UI_CONTROL_SET_STATE          SetState;
  UI_CONTROL_GET_STATE          GetState;

  UI_CONTROL_SHOULD_DELAY_CHILD_PRESSED_STATE ShouldDelayChildPressedState;
};

#endif
