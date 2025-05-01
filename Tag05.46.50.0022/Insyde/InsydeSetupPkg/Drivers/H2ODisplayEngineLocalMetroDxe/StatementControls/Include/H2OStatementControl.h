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

#ifndef _H2O_STATEMENT_CONTROL_H
#define _H2O_STATEMENT_CONTROL_H

typedef struct _H2O_STATEMENT_CONTROL          H2O_STATEMENT_CONTROL;
typedef struct _H2O_STATEMENT_CONTROL_CLASS    H2O_STATEMENT_CONTROL_CLASS;

typedef
EFI_STATUS
(EFIAPI *H2O_STATEMENT_CONTROL_ON_SEND_CHANGE_Q) (
  IN H2O_STATEMENT_CONTROL                     *This
  );

struct _H2O_STATEMENT_CONTROL {
  UI_BUTTON                                    Button;
  H2O_FORM_BROWSER_S                           *Statement;
  EFI_HII_VALUE                                HiiValue;
  BOOLEAN                                      ValueChanged;

  H2O_STATEMENT_CONTROL_ON_SEND_CHANGE_Q       OnSendChangeQ;
};

struct _H2O_STATEMENT_CONTROL_CLASS {
  UI_BUTTON_CLASS                              ParentClass;
};

H2O_STATEMENT_CONTROL_CLASS *
EFIAPI
GetH2OStatementControlClass (
  VOID
  );

BOOLEAN
IsStatementControlClassName (
  IN CHAR16                                    *ClassName
  );

EFI_STATUS
EFIAPI
H2OStatementControlOnSendChangeQ (
  IN H2O_STATEMENT_CONTROL                     *This
  );

H2O_STATEMENT_CONTROL *
GetNextStatementControlProc (
  IN H2O_STATEMENT_CONTROL                     *This
  );

#endif
