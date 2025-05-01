/** @file
  Header file for H2O Form Dialog Formsets's Configuration

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

#ifndef _H2O_FORM_DIALOG_CONFIG_H_
#define _H2O_FORM_DIALOG_CONFIG_H_

#include <Guid/H2OControlType.h>
#include <Guid/H2OFormDialog.h>

#include "H2OFormDialogDxeIdfDefs.h"

#define H2O_FORM_DIALOG_VARSTORE_NAME L"H2OFormDialogConfig"

#define KEY_EMPTY 0x1089

#define H2O_FORM_DIALOG_CONFIGURATION_GUID \
  { 0x98ae8272, 0xce5a, 0x46be, { 0x9f, 0x5d, 0xd9, 0xf9, 0xcb, 0xbb, 0x99, 0xf2 } }

#define H2O_FORM_DIALOG_CONFIGURATION_VARSTORE_ID 0xD000

typedef struct {
  UINT8                              Empty;
} H2O_FORM_DIALOG_CONFIGURATION;

#endif

