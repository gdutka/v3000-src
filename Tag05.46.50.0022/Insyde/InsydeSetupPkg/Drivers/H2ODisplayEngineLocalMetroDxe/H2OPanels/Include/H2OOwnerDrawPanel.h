/** @file

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

#ifndef _H2O_OWNER_DRAW_PANEL_H
#define _H2O_OWNER_DRAW_PANEL_H

#include <Protocol/H2OOwnerDrawPanelProtocol.h>

typedef struct _H2O_OWNER_DRAW_PANEL H2O_OWNER_DRAW_PANEL;
typedef struct _H2O_OWNER_DRAW_PANEL_CLASS H2O_OWNER_DRAW_PANEL_CLASS;

H2O_OWNER_DRAW_PANEL_CLASS *
EFIAPI
GetH2OOwnerDrawPanelClass (
  VOID
  );

struct _H2O_OWNER_DRAW_PANEL {
  UI_CONTROL                    Parent;
  UI_CONTROL                    *OwnerDrawList;
  UINT32                        RefreshInterval;
  BOOLEAN                       OwnerDrawChildsInitialized;

  //
  // For H2O Owner Draw Panel Protocol
  //
  UINTN                         OwnerDrawProtocolCount;
  H2O_OWNER_DRAW_PANEL_PROTOCOL **OwnerDrawProtocolList;
  EFI_IMAGE_INPUT               OwnerDrawProtocolImage;
  EFI_IMAGE_INPUT               ImageBuf;
};

struct _H2O_OWNER_DRAW_PANEL_CLASS {
  UI_CONTROL_CLASS              ParentClass;
};

#endif
