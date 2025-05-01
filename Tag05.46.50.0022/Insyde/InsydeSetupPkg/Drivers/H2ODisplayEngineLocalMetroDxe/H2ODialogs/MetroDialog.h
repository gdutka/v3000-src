/** @file
  Entry point and initial functions for H2O local Metro display engine driver

;******************************************************************************
;* Copyright (c) 2013 - 2015, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef _H2O_DIALOGS_H_
#define _H2O_DIALOGS_H_

#include "H2ODisplayEngineLocalMetro.h"
#include "UiControls.h"
#include "H2OControls.h"
#include "H2ODialogs.h"
#include "MetroUi.h"

extern BOOLEAN                  mIsSendForm;
extern BOOLEAN                  mTitleVisible;
extern H2O_FORM_BROWSER_D       *mFbDialog;

UINT32
GetButtonWidthByStr (
  VOID
  );

VOID
FreeDialogEvent (
  IN H2O_FORM_BROWSER_D         **DialogEvt
  );

LRESULT
WINAPI
DialogCallback (
  HWND Wnd,
  UINT Msg,
  WPARAM WParam,
  LPARAM LParam
  );

#endif
