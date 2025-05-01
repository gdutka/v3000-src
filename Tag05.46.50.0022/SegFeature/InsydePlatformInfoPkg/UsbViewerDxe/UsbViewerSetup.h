/** @file
  Header file for H2O USB Viewer Setup Page

;******************************************************************************
;* Copyright 2020 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef H2O_USB_VIEWER_SETUP_H_
#define H2O_USB_VIEWER_SETUP_H_

#include <Guid/UsbViewerHii.h>

//
// Each depth is a separate form so the Form Browser may implement breadcrumb
// navigation without problems.
//
//  H2O_USB_VIEWER_ENTRY_FORM_ID                0x0001 // (Host controller list)    // Referenced from SDEmmcInfoSetupUtilityHii.c
#define H2O_USB_VIEWER_HOST_CONTROLLER_FORM_ID  0x0002 // Tier 1 (Host information & depth 0 device list)
#define H2O_USB_VIEWER_DEPTH_0_DEVICE_FORM_ID   0x0003 // Tier 2 (Hub / device information)
#define H2O_USB_VIEWER_DEPTH_1_DEVICE_FORM_ID   0x0004 // Tier 3 (Hub / device information)
#define H2O_USB_VIEWER_DEPTH_2_DEVICE_FORM_ID   0x0005 // Tier 4 (Hub / device information)
#define H2O_USB_VIEWER_DEPTH_3_DEVICE_FORM_ID   0x0006 // Tier 5 (Hub / device information)
#define H2O_USB_VIEWER_DEPTH_4_DEVICE_FORM_ID   0x0007 // Tier 6 (Hub / device information)
#define H2O_USB_VIEWER_DEPTH_5_DEVICE_FORM_ID   0x0008 // Tier 7 (Device information)

#define H2O_USB_VIEWER_ENTRY_REFRESH_QUESTION_ID         0x0001
#define H2O_USB_VIEWER_HC_REFRESH_QUESTION_ID            0x0002
#define H2O_USB_VIEWER_DEPTH_0_REFRESH_QUESTION_ID       0x0003
#define H2O_USB_VIEWER_DEPTH_1_REFRESH_QUESTION_ID       0x0004
#define H2O_USB_VIEWER_DEPTH_2_REFRESH_QUESTION_ID       0x0005
#define H2O_USB_VIEWER_DEPTH_3_REFRESH_QUESTION_ID       0x0006
#define H2O_USB_VIEWER_DEPTH_4_REFRESH_QUESTION_ID       0x0007
#define H2O_USB_VIEWER_DEPTH_5_REFRESH_QUESTION_ID       0x0008
#define H2O_USB_VIEWER_GO_BACK_QUESTION_ID               0x0009

#define H2O_USB_VIEWER_FLAT_VIEW_QUESTION_ID             0x0011
#define H2O_USB_VIEWER_SHOW_HUB_INFO_HIDDEN_QUESTION_ID  0x0012
#define H2O_USB_VIEWER_SHOW_HUB_INFO_ACTION_QUESTION_ID  0x0013

#define H2O_USB_VIEWER_DYNAMIC_QUESTION_ID_BASE          0x8001
#define H2O_USB_VIEWER_DYNAMIC_STATEMENT_LIMIT           0x3FFF

#define H2O_USB_VIEWER_LABEL_START                       0x0001
#define H2O_USB_VIEWER_LABEL_END                         0x0002

#endif // H2O_USB_VIEWER_SETUP_H_
