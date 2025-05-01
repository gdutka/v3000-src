/** @file

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef _UI_FLEX_LAYOUT_H
#define _UI_FLEX_LAYOUT_H

typedef struct _UI_FLEX_LAYOUT UI_FLEX_LAYOUT;
typedef struct _UI_FLEX_LAYOUT_CLASS UI_FLEX_LAYOUT_CLASS;

UI_FLEX_LAYOUT_CLASS  *
EFIAPI
GetFlexLayoutClass (
  VOID
  );

typedef enum {
  UI_FLEX_LAYOUT_FLEX_DIRECTION_TYPE_ROW = 0,
  UI_FLEX_LAYOUT_FLEX_DIRECTION_TYPE_COLUMN,
} UI_FLEX_LAYOUT_FLEX_DIRECTION_TYPE;

typedef enum {
  UI_FLEX_LAYOUT_FLEX_WRAP_TYPE_NO_WRAP = 0,
  UI_FLEX_LAYOUT_FLEX_WRAP_TYPE_WRAP,
} UI_FLEX_LAYOUT_FLEX_WRAP_TYPE;

typedef enum {
  UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_FLEX_START = 0,
  UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_FLEX_END,
  UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_CENTER,
  UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_SPACE_BETWEEN,
  UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE_SPACE_AROUND,
} UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE;

typedef enum {
  UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_STRETCH = 0,
  UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_FLEX_START,
  UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_FLEX_END,
  UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_CENTER,
  UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_SPACE_BETWEEN,
  UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE_SPACE_AROUND,
} UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE;

typedef enum {
  UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_FLEX_START = 0,
  UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_FLEX_END,
  UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_CENTER,
  UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE_STRETCH,
} UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE;

struct _UI_FLEX_LAYOUT {
  UI_CONTROL                           Container;

  UI_FLEX_LAYOUT_FLEX_DIRECTION_TYPE   FlexDirection;
  UI_FLEX_LAYOUT_FLEX_WRAP_TYPE        FlexWrap;
  UI_FLEX_LAYOUT_JUSTIFY_CONTENT_TYPE  JustifyContent;
  UI_FLEX_LAYOUT_ALIGN_CONTENT_TYPE    AlignContent;
  UI_FLEX_LAYOUT_ALIGN_ITEMS_TYPE      AlignItems;
};

struct _UI_FLEX_LAYOUT_CLASS {
  UI_CONTROL_CLASS                   ParentClass;
};

#endif
