/** @file
  Header file for H2O Form Dialog

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_FORM_DIALOG_DXE_H_
#define _H2O_FORM_DIALOG_DXE_H_

#include <Library/DebugLib.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/VariableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/HiiLib.h>
#include <Library/HiiExLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiHiiServicesLib.h>

#include <Protocol/HiiConfigAccess.h>
#include <Protocol/H2OFormBrowser.h>

#include <Guid/MdeModuleHii.h>
#include <Guid/InsydeModuleHii.h>

#include <Guid/H2OControlType.h>
#include <Guid/H2OFormDialog.h>

#include "H2OFormDialogConfig.h"

extern UINT8 H2OFormDialogFsCheckboxVfrBin[];
extern UINT8 H2OFormDialogFsDateVfrBin[];
extern UINT8 H2OFormDialogFsOneOfVfrBin[];
extern UINT8 H2OFormDialogFsOrderedListVfrBin[];
extern UINT8 H2OFormDialogFsNumericVfrBin[];
extern UINT8 H2OFormDialogFsPasswordVfrBin[];
extern UINT8 H2OFormDialogFsStringVfrBin[];
extern UINT8 H2OFormDialogFsTimeVfrBin[];

#define H2O_FORM_DIALOG_PRIVATE_SIGNATURE SIGNATURE_32 ('H', 'F', 'D', 'P')

#pragma pack(1)

typedef struct {
  VENDOR_DEVICE_PATH                 VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL           End;
} HII_VENDOR_DEVICE_PATH;

typedef struct {
  LIST_ENTRY                         Link;
  EFI_HANDLE                         DriverHandle;
  EFI_HII_HANDLE                     HiiHandle;
  HII_VENDOR_DEVICE_PATH             VendorDevicePath;
  EFI_HII_CONFIG_ACCESS_PROTOCOL     HiiConfigAccess;
  EFI_GUID                           FormsetId;
  UINT16                             FormId;
  UINT8                              Operand;
  H2O_FORM_DIALOG_CONFIGURATION      Configuration;
} H2O_FORM_DIALOG_INFO;

#define H2O_FORM_DIALOG_INFO_FROM_LINK(a)  BASE_CR (a, H2O_FORM_DIALOG_INFO, Link)

typedef struct {
  //
  // Consumed protocol
  //
  //
  // H2O Form Dialog List
  //
  LIST_ENTRY                         H2OFormDialogList;
} H2O_FORM_DIALOG_PRIVATE_DATA;

#define H2O_FORM_DIALOG_PRIVATE_FROM_THIS(a)  CR (a, H2O_FORM_DIALOG_PRIVATE_DATA, ConfigAccess, H2O_FORM_DIALOG_PRIVATE_SIGNATURE)

#pragma pack()

#endif
