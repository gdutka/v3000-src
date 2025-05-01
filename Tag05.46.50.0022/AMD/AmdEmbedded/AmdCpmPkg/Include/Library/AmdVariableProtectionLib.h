/*****************************************************************************
 *
 * Copyright (C) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#ifndef _AMD_VARIABLE_PROTECTION_LIB_H_
#define _AMD_VARIABLE_PROTECTION_LIB_H_

#define AMD_VARIABLE_PROTECTION_VARIABLE_NAME  L"AmdVariableProtection"

// {408F573D-65EE-49ED-8BC5-5A32BBEAE745}
#define AMD_VARIABLE_PROTECTION_VARIABLE_GUID \
  { \
    0x408f573d, 0x65ee, 0x49ed, {0x8b, 0xc5, 0x5a, 0x32, 0xbb, 0xea, 0xe7, 0x45 } \
  }

extern EFI_GUID  gAmdVariableProtectionVariableGuid;

typedef enum _AMD_VAR_LOCK_STATE {
  AMD_VAR_UNLOCK      = 0,
  AMD_VAR_LOCK        = 1
} AMD_VAR_LOCK_STATE;

EFI_STATUS
EFIAPI
AmdVariableProtectionLibInitialize (
  VOID
  );

BOOLEAN
EFIAPI
AmdVariableProtectionLibGetLockState (
  VOID
  );

EFI_STATUS
EFIAPI
AmdVariableProtectionLibSetLockState (
  IN  AMD_VAR_LOCK_STATE           VarLockState
  );

EFI_STATUS
EFIAPI
AmdVariableProtectionLibLockVariable (
  IN  CHAR16                       *VariableName,
  IN  EFI_GUID                     *VendorGuid
  );

#endif
