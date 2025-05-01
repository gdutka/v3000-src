/** @file
  Defination GUID of Dialog Formset Id

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

#ifndef _H2O_FORM_DIALOG_H_
#define _H2O_FORM_DIALOG_H_

//
// Lmde Common Question Formset
//
#define H2O_FORM_DIALOG_FORMSET_CLASS_ID_LMDE_COMMON_QUESTION \
  { 0x2c922c3b, 0xd814, 0x4f22, { 0xaa, 0xbb, 0x5a, 0x85, 0xa3, 0x7e, 0x17, 0xf4 } }

#define H2O_FORM_DIALOG_FORMSET_ID_LMDE_CHECKBOX \
  { 0xa770d44c, 0xfd0c, 0x4a93, { 0x9d, 0xd9, 0xfd, 0x51, 0x86, 0xb2, 0x40, 0x37 } }

#define H2O_FORM_DIALOG_FORMSET_ID_LMDE_DATE \
  { 0x97c80457, 0x12f3, 0x4ce2, { 0x88, 0xd2, 0x6c, 0xf8, 0xc4, 0x66, 0x3e, 0xe8 } }

#define H2O_FORM_DIALOG_FORMSET_ID_LMDE_ONE_OF \
  { 0xf8dd52f1, 0x2327, 0x4f64, { 0x88, 0x3a, 0x49, 0x71, 0x8d, 0xd7, 0x9b, 0xe7 } }

#define H2O_FORM_DIALOG_FORMSET_ID_LMDE_ORDERED_LIST \
  { 0x63e3c9de, 0x21b4, 0x41e9, { 0xbf, 0xf6, 0x2a, 0xf6, 0x41, 0x84, 0xba, 0x18 } }

#define H2O_FORM_DIALOG_FORMSET_ID_LMDE_NUMERIC \
  { 0xc31b2733, 0x37a7, 0x43c1, { 0xba, 0xb7, 0xcb, 0xb1, 0xf1, 0x9e, 0xde, 0xf9 } }

#define H2O_FORM_DIALOG_FORMSET_ID_LMDE_PASSWORD \
  { 0xa294e412, 0xe111, 0x4a1b, { 0xb2, 0xf9, 0x97, 0x18, 0x5f, 0xbb, 0xfc, 0x76 } }

#define H2O_FORM_DIALOG_FORMSET_ID_LMDE_STRING \
  { 0xa65e6784, 0xd536, 0x4d44, { 0xaa, 0xb4, 0x70, 0x83, 0x5f, 0xa5, 0x85, 0x34 } }

#define H2O_FORM_DIALOG_FORMSET_ID_LMDE_TIME \
  { 0x727efa2e, 0x92d2, 0x457c, { 0xbd, 0xb6, 0xac, 0xe1, 0xd7, 0x6f, 0x5b, 0x20 } }

//
// Common Question Form
//
#define H2O_FORM_DIALOG_FORM_ID_ROOT                  0x0001
#define H2O_FORM_DIALOG_FORM_ID_LAST                  0xFFFF

//
// Form Dialog String Identifiers
//
#define STR_POPUP_CURRENT_Q_PROMPT                    0xFE00
#define STR_POPUP_CURRENT_Q_HELP                      0xFE01
#define STR_POPUP_CURRENT_F_TITLE                     0xFE02
#define STR_POPUP_CURRENT_FS_TITLE                    0xFE03

//
// Form Dialog Image Identifiers
//
#define IMG_POPUP_CURRENT_Q_PROMPT                    0xFE00
#define IMG_POPUP_CURRENT_Q_HELP                      0xFE01
#define IMG_POPUP_CURRENT_F_TITLE                     0xFE02
#define IMG_POPUP_CURRENT_FS_TITLE                    0xFE03

#define QID_OK                                        0xF001
#define QID_CANCEL                                    0xF002

#endif
