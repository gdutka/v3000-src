/** @file
  Header file for H2O Form Dialog

;******************************************************************************
;* Copyright (c) 2019 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_FORM_PANEL_DXE_H_
#define _H2O_FORM_PANEL_DXE_H_

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

#include "H2OFormPanelConfig.h"

extern UINT8 H2OFormPanelVfrBin[];

#define H2O_FORM_PANEL_PRIVATE_SIGNATURE SIGNATURE_32 ('H', 'F', 'P', 'D')

#pragma pack(1)

typedef struct {
  VENDOR_DEVICE_PATH                 VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL           End;
} HII_VENDOR_DEVICE_PATH;

#define H2O_FORM_PANEL_INFO_FROM_LINK(a)  BASE_CR (a, H2O_FORM_PANEL_INFO, Link)

typedef struct {
  UINTN                              Signature;

  EFI_HANDLE                         DriverHandle;
  EFI_HII_HANDLE                     HiiHandle;

  //
  // Consumed protocol
  //
  H2O_FORM_BROWSER_PROTOCOL          *H2OFB;

  //
  // Produced protocol
  //
  EFI_HII_CONFIG_ACCESS_PROTOCOL     HiiConfigAccess;
} H2O_FORM_PANEL_PRIVATE_DATA;

#define H2O_FORM_PANEL_PRIVATE_FROM_HII_CONFIG_ACCESS(a)  CR (a, H2O_FORM_PANEL_PRIVATE_DATA, ConfigAccess, H2O_FORM_PANEL_PRIVATE_SIGNATURE)

#pragma pack()

#endif
