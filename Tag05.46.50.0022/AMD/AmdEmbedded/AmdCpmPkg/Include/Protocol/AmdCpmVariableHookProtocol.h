/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_CPM_VARIABLE_HOOK_PROTOCOL_H_
#define _AMD_CPM_VARIABLE_HOOK_PROTOCOL_H_

///
/// Global ID for the CPM Variable Hook Protocol
///
#define AMD_CPM_VARIABLE_HOOK_PROTOCOL_GUID \
  { \
    0x29e22ccd, 0xfd53, 0x4a54, {0xb7, 0xde, 0x2c, 0x5b, 0x26, 0x9b, 0x80, 0x8f} \
  }

typedef struct _AMD_CPM_VARIABLE_HOOK_PROTOCOL   AMD_CPM_VARIABLE_HOOK_PROTOCOL;

/**
  Hook to sync something with variable

  @param[in]  Variable             Pointer to the variable data

  @retval EFI_SUCCESS            The hook has been successfully run

**/
typedef
EFI_STATUS
(EFIAPI *AMD_CPM_VARIABLE_SYNC_HOOK) (
  IN    VOID           *Variable
  );

//
// The _AMD_CPM_VARIABLE_HOOK_PROTOCOL provides the ability to sync variable into somthing like APCB
//
#define AMD_CPM_VARIABLE_HOOK_PROTOCOL_VERSION   1
struct _AMD_CPM_VARIABLE_HOOK_PROTOCOL {
  UINT32                        Version;
  CHAR16                        *VariableName;
  EFI_GUID                      VariableGuid;
  AMD_CPM_VARIABLE_SYNC_HOOK    VariableHook;
};
#endif
