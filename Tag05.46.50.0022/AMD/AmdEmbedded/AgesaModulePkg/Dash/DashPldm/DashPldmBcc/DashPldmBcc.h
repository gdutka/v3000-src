/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#pragma once

#include <Uefi.h>
#include <Uefi/UefiSpec.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AmdDashPldmProtocol.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/IdsLib.h>
#include <Library/CommonSetupLib.h>


// The initial & incremental size of a BIOS table in bytes.
#define GRANULE_BIOS_TABLE_BUFFER_SIZE                  0x1000

#define PLDM_BIOS_STRING_HANDLE_Disabled                0x0000
#define PLDM_BIOS_STRING_HANDLE_Enabled                 0x0001
#define PLDM_BIOS_STRING_HANDLE_On                      0x0002
#define PLDM_BIOS_STRING_HANDLE_Off                     0x0003
#define PLDM_BIOS_STRING_HANDLE_BIOS_ADMIN_PASSWORD     0x0004
#define PLDM_BIOS_STRING_HANDLE_BIOS_USER_PASSWORD      0x0005
#define PLDM_BIOS_STRING_HANDLE_NumLock_LED             0x0020
#define PLDM_BIOS_STRING_HANDLE_USB_Emulation           0x0021

#define PLDM_BIOS_STRING_HANDLE_BOOT_CONFIG_SETTING     0x8000
#define PLDM_BIOS_STRING_HANDLE_EFI_BOOT_OPTION         0x8001
#define PLDM_BIOS_STRING_HANDLE_EFI_BOOT_OPTION_MAX     (PLDM_BIOS_STRING_HANDLE_EFI_BOOT_OPTION + 0xFF)

#define PLDM_BIOS_ATTRIBUTE_HANDLE_BOOT_CONFIG_SETTING  0x8000

// Count padding bytes for alignment from the modular value.
#define PaddingCount(v, x) ((x - (v % x)) % x)
#define PaddingCount4(v) ((UINTN) PaddingCount(v, 4))


EFI_STATUS EFIAPI SetBiosTablesTags (VOID);
