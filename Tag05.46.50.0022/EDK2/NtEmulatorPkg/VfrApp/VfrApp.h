/** @file
 VFR application

;******************************************************************************
;* Copyright (c) 2015, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#ifndef _VFR_APP_H_
#define _VFR_APP_H_

#include <Uefi.h>

#include <Protocol/DevicePath.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/FormBrowser2.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>
#include <Protocol/FormBrowserEx.h>

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

#include "VfrAppNVDataStruc.h"

extern UINT8  VfrAppVfrBin[];
extern UINT8  VfrAppStrings[];

#define DYNAMIC_ONE_OF_VAR_OFFSET        OFFSET_OF (VFR_APP_CONFIGURATION, DynamicOneof)
#define DYNAMIC_ORDERED_LIST_VAR_OFFSET  OFFSET_OF (VFR_APP_CONFIGURATION, DynamicOrderedList)

#define DEFAULT_CLASS_MANUFACTURING_VALUE     0xFF
#define DEFAULT_CLASS_STANDARD_VALUE          0x0

//
// Number of name in Name/Value storage
//
#define NAME_VALUE_NAME_NUMBER       3

#define VFR_APP_PRIVATE_SIGNATURE SIGNATURE_32 ('V', 'A', 'p', 's')

typedef struct {
  UINTN                            Signature;

  EFI_HANDLE                       DriverHandle;
  EFI_HII_HANDLE                   HiiHandle;
  VFR_APP_CONFIGURATION            Configuration;
  VFR_APP_EFI_VARSTORE_DATA        VarStoreConfig;
  UINT8                            PasswordState;

  //
  // Name/Value storage Name list
  //
  EFI_STRING_ID                    NameStringId[NAME_VALUE_NAME_NUMBER];
  EFI_STRING                       NameValueName[NAME_VALUE_NAME_NUMBER];

  //
  // Consumed protocol
  //
  EFI_HII_DATABASE_PROTOCOL        *HiiDatabase;
  EFI_HII_STRING_PROTOCOL          *HiiString;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;
  EFI_FORM_BROWSER2_PROTOCOL       *FormBrowser2;

  //
  // Produced protocol
  //
  EFI_HII_CONFIG_ACCESS_PROTOCOL   ConfigAccess;
} VFR_APP_PRIVATE_DATA;

#define VFR_APP_PRIVATE_FROM_THIS(a)  CR (a, VFR_APP_PRIVATE_DATA, ConfigAccess, VFR_APP_PRIVATE_SIGNATURE)

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH               VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL         End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

extern VFR_APP_PRIVATE_DATA        *mVfrAppPrivate;

#endif
