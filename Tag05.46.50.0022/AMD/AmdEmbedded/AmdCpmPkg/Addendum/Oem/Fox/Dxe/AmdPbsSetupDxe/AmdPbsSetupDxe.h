/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _AMD_PBS_SETUP_DXE_H_
#define _AMD_PBS_SETUP_DXE_H_

#include <Uefi.h>

#include <Protocol/HiiConfigRouting.h>
#include <Protocol/FormBrowser2.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>

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

#define AMD_PBS_DXE_PRIVATE_SIGNATURE SIGNATURE_32 ('P', 'B', 'S', 'D')

typedef struct _AMD_PBS_DXE_PRIVATE_DATA {
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
} AMD_PBS_DXE_PRIVATE_DATA;

#define AMD_PBS_DXE_PRIVATE_FROM_THIS(a)  CR (a, AMD_PBS_DXE_PRIVATE_DATA, ConfigAccess, AMD_PBS_DXE_PRIVATE_SIGNATURE)

///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH                VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL          End;
} HII_VENDOR_DEVICE_PATH;


#endif
