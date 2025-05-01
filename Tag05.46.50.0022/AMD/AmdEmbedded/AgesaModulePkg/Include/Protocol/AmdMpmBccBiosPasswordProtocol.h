/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_MPM_BCC_BIOS_PASSWPRD_PROTOCOL_H_
#define _AMD_MPM_BCC_BIOS_PASSWPRD_PROTOCOL_H_

#include <Uefi/UefiBaseType.h>

#define PASSWORD_ADMIN 1
#define PASSWORD_USER  2

//
// Extern the GUID for protocol users, get pending value of password,
// OEM should update the password after getting it
//
extern EFI_GUID gAmdMpmBccBiosPasswordProtocolGuid;
typedef struct _AMD_MPM_BCC_BIOS_PASSWORD_PROTOCOL AMD_MPM_BCC_BIOS_PASSWORD_PROTOCOL;

/**
 * @brief Get value of PendingValue table BIOS password
 *
 * @details This protocol provide the interface to get Pending value table BIOS password and length
 *
 * @return EFI_STATUS                 0: Success, NonZero Error
 */
typedef
EFI_STATUS
(EFIAPI *AMD_MPM_BCC_PROTOCOL_GET_BIOS_PASSWORD) (
  IN OUT UINT16 *BccBiosPasswordLength,
  IN OUT CHAR8 *BccBiosPassword,
  IN OUT UINT8 *PasswordType
);


//
// Protocol Interface Structure
//
typedef struct _AMD_MPM_BCC_BIOS_PASSWORD_PROTOCOL
{
  AMD_MPM_BCC_PROTOCOL_GET_BIOS_PASSWORD AmdMpmBccGetBiosPassword;
} AMD_MPM_BCC_BIOS_PASSWORD_PROTOCOL;

//
// Extern the GUID for protocol users, get OEM BIOS password from user,
// then pass it to MPM
//
extern EFI_GUID gAmdMpmBccGetOemBiosPasswordProtocolGuid;
typedef struct _AMD_MPM_BCC_GET_OEM_BIOS_PASSWORD_PROTOCOL AMD_MPM_BCC_GET_OEM_BIOS_PASSWORD_PROTOCOL;


/**
 * @brief Get value of OEM BIOS password
 *
 * @details This protocol provide the interface to get OEM BIOS password and length
 *
 * @return EFI_STATUS                 0: Success, NonZero Error
 */
typedef
EFI_STATUS
(EFIAPI *AMD_MPM_BCC_PROTOCOL_GET_OEM_BIOS_PASSWORD) (
  IN OUT UINT8 *AdminBiosPasswordLength,
  IN OUT UINT8 *UserBiosPasswordLength,
  IN OUT CHAR8 *AdminBiosPassword,
  IN OUT CHAR8 *UserBiosPassword,
  IN OUT UINT8 *BiosPasswordType //1: Admin only, 2: User Only, 3: Both have
);


//
// Protocol Interface Structure
//
typedef struct _AMD_MPM_BCC_GET_OEM_BIOS_PASSWORD_PROTOCOL
{
  AMD_MPM_BCC_PROTOCOL_GET_OEM_BIOS_PASSWORD AmdMpmBccGetOemBiosPassword;
} AMD_MPM_BCC_GET_OEM_BIOS_PASSWORD_PROTOCOL;
#endif
