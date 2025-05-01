/** @file
 This file stores Setup Fromset GUID of this SataDriveInfo

 Note that the GUID MUST be the same as gSataDriveInfoSetupFormsetGuid in
 SataDriveInfoPkg.dec

;******************************************************************************
;* Copyright 2017 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef _SD_EMMC_INFO_SETUP_UTILITY_HII_H_
#define _SD_EMMC_INFO_SETUP_UTILITY_HII_H_

// {A19CD60D-FA6B-DC01-B66A-6F0ED94890FB}
#define SD_EMMC_INFO_SETUP_FORMSET_GUID \
  { \
    0xA19CD60D, 0xFA6B, 0xDC01, { 0xB6, 0x6A, 0x6F, 0x0E, 0xD9, 0x48, 0x90, 0xFB } \
  }

// {6875C1B4-C562-407C-B55F-6F9E3CD2832C}
#define SD_EMMC_INFO_SETUP_VARSTORE_GUID \
  { \
    0x6875C1B4, 0xC562, 0x407C, { 0xB5, 0x5F, 0x6F, 0x9E, 0x3C, 0xD2, 0x83, 0x2C } \
  }

#define SD_EMMC_INFO_SETUP_VARSTORE_NAME   L"SDeMMCInfoSetupConfig"

//
// Variable Store ID
//
#define SD_EMMC_INFO_VARSTORE_ID                                       0xA000

//
// Form ID definition
//
#define H2O_SD_EMMC_INFO_LIST_FORM_ID                                  0x0001  // Device List form



#pragma pack(1)
typedef struct {
  UINT8   DisplayMode;
  UINT8   NumberOfOemPorts;
} SD_EMMC_SETUP_CONFIGURATION;
#pragma pack()

extern EFI_GUID gH2OSDEmmcInfoSetupFormsetGuid;
extern EFI_GUID gH2OSDEmmcInfoSetupVarstoreGuid;

#endif
