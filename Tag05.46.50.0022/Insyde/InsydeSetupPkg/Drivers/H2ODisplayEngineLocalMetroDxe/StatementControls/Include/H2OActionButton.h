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

#ifndef _H2O_ACTION_BUTTON_H
#define _H2O_ACTION_BUTTON_H

typedef struct _H2O_ACTION_BUTTON       H2O_ACTION_BUTTON;
typedef struct _H2O_ACTION_BUTTON_CLASS H2O_ACTION_BUTTON_CLASS;

typedef enum {
  ActionButtonUnsupported = 0,
  ActionButtonDialogOk,
  ActionButtonDialogCancel,
  ActionButtonMax
} H2O_ACTION_BUTTON_ACTION;

typedef
H2O_ACTION_BUTTON_ACTION
(EFIAPI *H2O_ACTION_BUTTON_ON_ACTIVE) (
  IN H2O_ACTION_BUTTON                  *This
  );

struct _H2O_ACTION_BUTTON {
  H2O_STATEMENT_CONTROL                 StatementControl;
  H2O_ACTION_BUTTON_ON_ACTIVE           OnActive;
};

struct _H2O_ACTION_BUTTON_CLASS {
  H2O_STATEMENT_CONTROL_CLASS           ParentClass;
};

H2O_ACTION_BUTTON_CLASS *
EFIAPI
GetH2OActionButtonClass (
  VOID
  );

#endif
