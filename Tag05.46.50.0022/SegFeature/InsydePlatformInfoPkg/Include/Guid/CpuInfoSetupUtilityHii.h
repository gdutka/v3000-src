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

#ifndef __CPU_INFO_SETUP_UTILITY_HII_H__
#define __CPU_INFO_SETUP_UTILITY_HII_H__

// {4B4B81A5-4C87-4DB1-A0AC-FEDEC5C3122F}
#define H2O_CPU_INFO_FORMSET_GUID \
  { \
    0x4B4B81A5, 0x4C87, 0x4DB1, { 0xA0, 0xAC, 0xFE, 0xDE, 0xC5, 0xC3, 0x12, 0x2F } \
  }

//
// Form ID definition
//
#define H2O_CPU_INFO_FORM_ID                            0x0001

//
// Question ID definition
//
#define H2O_CPU_INFO_TOTAL_CPU_QUESTION_ID              0x0001

extern EFI_GUID gH2OCpuInfoFormsetGuid;

#endif
