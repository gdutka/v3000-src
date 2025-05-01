/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _AMD_MODERN_STANDBY_RES_CHECK_VARIABL_H_
#define _AMD_MODERN_STANDBY_RES_CHECK_VARIABL_H_

//  {E3539E26-105D-4068-A924-9B11F3A2F1E6}
#define AMD_MODERN_STANDBY_RES_CHECK_VARIABLE_GUID \
  { 0xE3539E26, 0x105D, 0x4068, 0xA9, 0x24, 0x9B, 0x11, 0xF3, 0xA2, 0xF1, 0xE6}


#define AMD_MODERN_STANDBY_RES_CHECK_VARIABLE_NAME   L"AMD_MODS_RES"

#define AMD_MODS_RES_INSTALLED  1

#define AMD_MODERN_STANDBY_RES_CHECK_VARIABLE_ATTRIBUTES \
        (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | \
         EFI_VARIABLE_RUNTIME_ACCESS)

typedef EFI_PHYSICAL_ADDRESS AMD_MODERN_STANDBY_RES_CHECK_VARIABLE_DATA;

extern EFI_GUID gAmdModernStandbyResourceCheckVariableGuid;

typedef struct _AMD_MODS_RES{
  UINT32  ModsResStatus;
  
}AMD_MODS_RES;
#endif
