/** @file
 This file stores Setup Fromset GUID of this SataDriveInfo

 Note that the GUID MUST be the same as gSataDriveInfoSetupFormsetGuid in
 InsydePlatformInfoPkg.dec

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef __SATA_DRIVE_INFO_SETUP_UTILITY_HII_H__
#define __SATA_DRIVE_INFO_SETUP_UTILITY_HII_H__

// {A19C060D-F46B-D601-366E-6201694AC0FF}
#define SATA_DRIVE_INFO_SETUP_FORMSET_GUID \
  { \
    0xA19C060D, 0xF46B, 0xD601, { 0x36, 0x6E, 0x62, 0x01, 0x69, 0x4A, 0xC0, 0xFF } \
  }

// {88E0A9AA-37BE-4506-9650-EEAD5EE911B0}
#define SATA_DRIVE_INFO_CONFIGURATION_GUID \
  { \
    0x88E0A9AA, 0x37BE, 0x4506, { 0x96, 0x50, 0xEE, 0xAD, 0x5E, 0xE9, 0x11, 0xB0 } \
  }

#define SATA_DRIVE_INFO_CONFIGURATION_VARIABLE_NAME   L"SataDriveInfoSetupConfig"

//
// Varstore Id
//
#define SATA_DRIVE_INFO_CONFIGURATION_VARSTORE_ID     0x4000

//
// Root Form Id
//
#define H2O_SATA_DRIVE_INFO_ROOT_FORM_ID              0x2000

#pragma pack(1)
typedef struct {
  UINT8   HideEmptyPorts;
  UINT8   DisplayMode;
  UINT8   NumberOfPhysicalPorts;
  UINT8   FormRefresh;
} SATA_DRIVE_INFO_CONFIGURATION;
#pragma pack()

extern EFI_GUID gH2OSataDriveInfoSetupFormsetGuid;
extern EFI_GUID gH2OSataDriveInfoConfigurationGuid;

#endif
