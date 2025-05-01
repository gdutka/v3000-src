/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _CBS_CONFIG_H_
#define _CBS_CONFIG_H_

#define FORMSET_ID_GUID_AMD_CBS \
  { \
    0xB04535E3, 0x3004, 0x4946, {0x9E, 0xB7, 0x14, 0x94, 0x28, 0x98, 0x30, 0x53} \
  }

#define FORMSET_ID_GUID_AMD_CBS_ZP \
  { \
    0x7C3CCF08, 0xB8F4, 0x4EF4, { 0xB5, 0x8D, 0xA4, 0x70, 0xBF, 0xD2, 0xDE, 0x05 } \
  }

#define FORMSET_ID_GUID_AMD_CBS_SSP \
  { \
    0x7A6A3896, 0x4AF0, 0x45E5, { 0xBA, 0x63, 0x89, 0x35, 0x7F, 0xBD, 0x6D, 0x63 } \
  }

#define FORMSET_ID_GUID_AMD_CBS_GN \
  { \
    0x22235FC6, 0x01E1, 0x4E64, { 0xA2, 0xA5, 0x65, 0x9C, 0x28, 0xC3, 0xD3, 0x72 } \
  }

#define CBS_SYSTEM_CONFIGURATION_GUID \
  { \
    0x3A997502, 0x647A, 0x4c82, {0x99, 0x8E, 0x52, 0xEF, 0x94, 0x86, 0xA2, 0x47} \
  }

extern EFI_GUID gCbsSystemConfigurationGuid;

#define AMD_SYSTEM_CONFIGURATION_GUID  \
  { \
    0x3A997502, 0x647A, 0x4c82, {0x99, 0x8E, 0x52, 0xEF, 0x94, 0x86, 0xA2, 0x47} \
  }

#define CBS_SYSTEM_CONFIGURATION_NAME L"AmdSetup"

#define CBS_SYSTEM_CONFIGURATION_NAME_ZP L"AmdSetupZP"

#define CBS_SYSTEM_CONFIGURATION_NAME_SSP L"AmdSetupSSP"

#define CBS_SYSTEM_CONFIGURATION_NAME_GN L"AmdSetupGN"

#define CBS_SYSTEM_CONFIGURATION_NAME_BA L"AmdSetupBA"

#endif // _CBS_CONFIG_H_
