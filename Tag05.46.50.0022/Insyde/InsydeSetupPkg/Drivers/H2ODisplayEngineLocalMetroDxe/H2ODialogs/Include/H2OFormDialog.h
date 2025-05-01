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

#ifndef _H2O_FORM_PANEL_DIALOG_H
#define _H2O_FORM_PANEL_DIALOG_H

typedef struct _H2O_FORM_DIALOG H2O_FORM_DIALOG;
typedef struct _H2O_FORM_DIALOG_CLASS H2O_FORM_DIALOG_CLASS;

struct _H2O_FORM_DIALOG {
  UI_DIALOG                           Parent;
};

struct _H2O_FORM_DIALOG_CLASS {
  UI_DIALOG_CLASS                     ParentClass;
};

H2O_FORM_DIALOG_CLASS *
EFIAPI
GetH2OFormDialogClass (
  VOID
  );

EFI_STATUS
GetPopUpForm (
  IN H2O_FORM_BROWSER_Q         *Question,
  OUT EFI_GUID                  *PopUpFormsetId,
  OUT UINT16                    *PopUpFormId
  );

EFI_STATUS
GetPopUpFormRect (
  IN EFI_GUID                   *PopUpFormsetId,
  IN UINT16                     PopUpFormId,
  OUT RECT                      *PopUpRect
  );

#endif

