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

#ifndef _H2O_OPTION_LIST_H
#define _H2O_OPTION_LIST_H

typedef struct _H2O_OPTION_LIST       H2O_OPTION_LIST;
typedef struct _H2O_OPTION_LIST_CLASS H2O_OPTION_LIST_CLASS;

struct _H2O_OPTION_LIST {
  H2O_STATEMENT_CONTROL               StatementControl;
};

struct _H2O_OPTION_LIST_CLASS {
  H2O_STATEMENT_CONTROL_CLASS         ParentClass;
};

H2O_OPTION_LIST_CLASS *
EFIAPI
GetH2OOptionListClass (
  VOID
  );

#endif
