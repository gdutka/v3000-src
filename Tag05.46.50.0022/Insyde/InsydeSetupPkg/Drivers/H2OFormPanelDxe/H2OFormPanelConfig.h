/** @file
  Header file for H2O Form Panel Formsets's Configuration

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

#ifndef _H2O_FORM_PANEL_CONFIG_H_
#define _H2O_FORM_PANEL_CONFIG_H_

#include "H2OFormPanelDxeIdfDefs.h"

#define H2O_FORM_PANEL_VARSTORE_NAME L"H2OFormPanelConfig"

#define H2O_FORM_PANEL_FORMSET_ID \
  { 0x226c29e1, 0xfd32, 0x423a, { 0x82, 0x30, 0x93, 0xe3, 0xc9, 0x71, 0x2a, 0x65 } }

#define H2O_FORM_PANEL_FORMSET_CLASS_ID \
  { 0x8a3d9815, 0x9b73, 0x401c, { 0x9b, 0x56, 0x17, 0xdf, 0xfe, 0xf6, 0x3e, 0x8a } }

#define H2O_FORM_PANEL_CONFIGURATION_GUID \
  { 0xb2438e61, 0x316c, 0x4f5f, { 0x86, 0x79, 0xa8, 0x70, 0x19, 0x3c, 0xfe, 0x13 } }

#define H2O_FORM_PANEL_CONFIGURATION_VARSTORE_ID 0xD000

#define H2O_FORM_PANEL_FORM_ID_ROOT              0x0001
#define H2O_FORM_PANEL_FORM_ID_LAST              0xFFFF

#define DATE_REFRESH_QUESTION_ID                        0x3000
#define WEEK_REFRESH_QUESTION_ID                        0x3001
#define TIME_REFRESH_QUESTION_ID                        0x3002
#define MAIN_BOARD_TEMPERATURE_REFRESH_QUESTION_ID      0x3003
#define CPU_TEMPERATURE_REFRESH_QUESTION_ID             0x3004


typedef struct {
  UINT8                              Empty;
} H2O_FORM_PANEL_CONFIGURATION;

#endif

