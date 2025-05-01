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

#ifndef _H2O_DATE_ROLLER_H
#define _H2O_DATE_ROLLER_H

typedef struct _H2O_DATE_ROLLER       H2O_DATE_ROLLER;
typedef struct _H2O_DATE_ROLLER_CLASS H2O_DATE_ROLLER_CLASS;

struct _H2O_DATE_ROLLER {
  H2O_STATEMENT_CONTROL               StatementControl;
};

struct _H2O_DATE_ROLLER_CLASS {
  H2O_STATEMENT_CONTROL_CLASS         ParentClass;
};

H2O_DATE_ROLLER_CLASS *
EFIAPI
GetH2ODateRollerClass (
  VOID
  );

#endif
