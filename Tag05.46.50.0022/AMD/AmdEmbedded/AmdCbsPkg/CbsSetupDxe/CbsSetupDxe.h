/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/** @file
The functions export data to the HII protocol to be later utilized by the
Setup Protocol

Copyright (c) 2004 - 2017, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available
under the terms and conditions of the BSD License which accompanies this
distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS, WITHOUT
WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CBS_DXE_H_
#define _CBS_DXE_H_

#include <Uefi.h>

#include <Protocol/HiiConfigRouting.h>
#include <Protocol/FormBrowser2.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>
#include <Protocol/AmdCbsSetupProtocol.h>

#include <Guid/MdeModuleHii.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HiiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/AmdBaseLib.h>

#include <Guid/AmdCbsConfig.h>
#include <Library/AmdCbsSetupLib.h>

#define CBS_DXE_PRIVATE_SIGNATURE SIGNATURE_32 ('C', 'B', 'S', 'D')

typedef struct _CBS_DXE_PRIVATE_DATA {
  UINTN                            Signature;

  EFI_HANDLE                       DriverHandle;
  EFI_HII_HANDLE                   HiiHandle;
  UINT8 *                          IfrData;

  //
  // Consumed protocol
  //
  EFI_HII_DATABASE_PROTOCOL        *HiiDatabase;
  EFI_HII_STRING_PROTOCOL          *HiiString;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;

  //
  // Produced protocol
  //
  EFI_HII_CONFIG_ACCESS_PROTOCOL   ConfigAccess;
} CBS_DXE_PRIVATE_DATA;

#define CBS_DXE_PRIVATE_FROM_THIS(a)  CR (a, CBS_DXE_PRIVATE_DATA, ConfigAccess, CBS_DXE_PRIVATE_SIGNATURE)

#pragma pack(1)

//
// HII specific Vendor Device Path definition.
//
typedef struct {
  VENDOR_DEVICE_PATH           VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL     End;
} HII_VENDOR_DEVICE_PATH;

#pragma pack()

EFI_STATUS
EFIAPI
CbsSetupLoadDefault (
  IN  CONST CBS_SETUP_PROTOCOL   *This
  );

#endif

