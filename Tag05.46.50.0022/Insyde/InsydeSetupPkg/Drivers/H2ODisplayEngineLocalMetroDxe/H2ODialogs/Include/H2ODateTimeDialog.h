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

#ifndef _H2O_DATE_TIME_DIALOG_H
#define _H2O_DATE_TIME_DIALOG_H

typedef struct _H2O_DATE_TIME_DIALOG H2O_DATE_TIME_DIALOG;
typedef struct _H2O_DATE_TIME_DIALOG_CLASS H2O_DATE_TIME_DIALOG_CLASS;

struct _H2O_DATE_TIME_DIALOG {
  UI_DIALOG                           Parent;
};

struct _H2O_DATE_TIME_DIALOG_CLASS {
  UI_DIALOG_CLASS                     ParentClass;
};

H2O_DATE_TIME_DIALOG_CLASS *
EFIAPI
GetH2ODateTimeDialogClass (
  VOID
  );
#endif
