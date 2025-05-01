/******************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
**/

#ifndef _FCH_SMM_RESET_H_
#define _FCH_SMM_RESET_H_

#define AMD_SMM_RESET_SYSTEM_PROTOCOL_GUID \
  { \
    0xbb74386f, 0xdfff, 0x4c36, {0x8a, 0x2e, 0xde, 0x2d, 0x3a, 0x62, 0x2b, 0x85}\
  }

typedef
VOID
(EFIAPI *AMD_SMM_RESET_SYSTEM)(
    IN EFI_RESET_TYPE   ResetType,
    IN EFI_STATUS       ResetStatus,
    IN UINTN            DataSize,
    IN CHAR16           *ResetData OPTIONAL
  );

struct _AMD_SMM_RESET_SYSTEM_PROTOCOL {
  ///
  /// Allows Reset the system in SMM.
  ///
  AMD_SMM_RESET_SYSTEM    ResetSystem;
};

typedef struct _AMD_SMM_RESET_SYSTEM_PROTOCOL  AMD_SMM_RESET_SYSTEM_PROTOCOL;

extern EFI_GUID gAmdSmmResetSystemProtocolGuid;

#endif
