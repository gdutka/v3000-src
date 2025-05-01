/** @file

   The definition of Memory Info Setup Utility HII.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _MEMORY_INFO_SETUP_UTILITY_HII_H_
#define _MEMORY_INFO_SETUP_UTILITY_HII_H_

// {929C3D76-E9CA-4330-95A0-954BB46E3D50}
#define H2O_MEMORY_INFO_FORMSET_GUID \
  { \
    0x929C3D76, 0xE9CA, 0x4330, { 0x95, 0xA0, 0x95, 0x4B, 0xB4, 0x6E, 0x3D, 0x50 } \
  }

// {E0C8FF7D-A3EC-44F2-9AAE-0BABED2FD0D4}
#define H2O_MEMORY_INFO_SETUP_VARSTORE_GUID \
  { \
    0xE0C8FF7D, 0xA3EC, 0x44F2, { 0x9A, 0xAE, 0x0B, 0xAB, 0xED, 0x2F, 0xD0, 0xD4 } \
  }

#define H2O_MEMORY_INFO_SETUP_VARSTORE_NAME      L"MemoryInfoSetup"

//
// Variable Store ID
//
#define MEMORY_INFO_VARSTORE_ID                                     0x0100

//
// Form ID definition
//
#define H2O_MEMORY_INFO_DIMM_LIST_FORM_ID                           0x0001

//
// Question ID definition
//
#define H2O_MEMORY_INFO_TOTAL_MEMORY_QUESTION_ID                    0x0001
#define H2O_MEMORY_INFO_INSTALLED_MEMORY_QUESTION_ID                0x0002
#define H2O_MEMORY_INFO_UNINSTALL_MEMORY_QUESTION_ID                0x0003
#define H2O_MEMORY_INFO_ECC_TYPE_QUESTION_ID                        0x0004
#define H2O_MEMORY_INFO_TOTAL_MEMORY_SIZE_QUESTION_ID               0x0005



typedef struct {
  UINT8  HiddenNotInstalledMemory;
} MEMORY_INFO_SETUP;

extern EFI_GUID gH2OMemoryInfoFormsetGuid;
extern EFI_GUID gH2OMemoryInfoSetupVarstoreGuid;

#endif
