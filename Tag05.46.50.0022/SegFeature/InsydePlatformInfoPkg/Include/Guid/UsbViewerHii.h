/** @file
 This file stores Setup Fromset GUID of this SataDriveInfo

 Note that the GUID MUST be the same as gSataDriveInfoSetupFormsetGuid in
 InsydePlatformInfoPkg.dec

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef __USB_VIEWER_HII_H__
#define __USB_VIEWER_HII_H__

// {280C409D-E5FB-4337-877F-504F5F50537F}
#define H2O_USB_VIEWER_FORMSET_GUID \
  { \
    0x280C409D, 0xE5FB, 0x4337, { 0x87, 0x7F, 0x50, 0x4F, 0x5F, 0x50, 0x53, 0x7F } \
  }

//
// Each depth is a separate form so the Form Browser may implement breadcrumb
// navigation without problems.
//
#define H2O_USB_VIEWER_ENTRY_FORM_ID                     0x0001 // (Host controller list)

#pragma pack(1)
typedef struct _H2O_USB_VIEWER_VIEW_OPTION {
  UINT8   FlatView;
  UINT8   ShowHubInformation;
  UINT8   ShowFullPortNumbers; // not implemented yet
  UINT16  SelectedLangId;      // not implemented yet
} H2O_USB_VIEWER_VIEW_OPTION;
#pragma pack()

extern EFI_GUID gH2OUsbViewerFormsetGuid;

#endif
