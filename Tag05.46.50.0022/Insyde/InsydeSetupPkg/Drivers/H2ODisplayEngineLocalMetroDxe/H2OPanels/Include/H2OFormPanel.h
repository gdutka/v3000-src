/** @file

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_FORM_PANEL_H
#define _H2O_FORM_PANEL_H

typedef struct _FORM_PANEL             FORM_PANEL;
typedef struct _FORM_PANEL_CLASS       FORM_PANEL_CLASS;

struct _FORM_PANEL {
  UI_CONTROL                           Parent;
  BOOLEAN                              FormPanelChildsInitialized;
};

struct _FORM_PANEL_CLASS {
  UI_CONTROL_CLASS                     ParentClass;
};

EFI_STATUS
FormPanelCreateChildByVfrForm (
  IN UI_CONTROL                        *Control,
  IN H2O_PANEL_INFO                    *Panel,
  IN EFI_GUID                          *FormsetId,
  IN UINT16                            FormId,
  IN RECT                              *FormPanelRect,
  OUT UINT8                            *RefreshInterval
  );

VOID
FormPanelCallAllChildProc (
  IN UI_CONTROL_CLASS                  *CurrentClass,
  IN UI_CONTROL                        *Control,
  IN UINT                              Msg,
  IN WPARAM                            WParam,
  IN LPARAM                            LParam
  );

EFI_STATUS
FormPanelChkAction (
  IN UI_CONTROL                        *Control
  );

FORM_PANEL_CLASS *
EFIAPI
GetFormPanelClass (
  VOID
  );

#endif
