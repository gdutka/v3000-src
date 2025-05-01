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

#ifndef __NVME_INFO_SETUP_UTILITY_HII_H__
#define __NVME_INFO_SETUP_UTILITY_HII_H__

// {75A179A8-0628-49CA-845A-71F81EF53A10}
#define H2O_NVME_INFO_FORMSET_GUID \
  { \
    0x75A179A8, 0x0628, 0x49CA, { 0x84, 0x5A, 0x71, 0xF8, 0x1E, 0xF5, 0x3A, 0x10 } \
  }

//
// Root Form ID definition
//
#define H2O_NVME_INFO_CONTROLLER_LIST_FORM_ID    0x0001

extern EFI_GUID gH2ONvmeInfoFormsetGuid;

#endif
