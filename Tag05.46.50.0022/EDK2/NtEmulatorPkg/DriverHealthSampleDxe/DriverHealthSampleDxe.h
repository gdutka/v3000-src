/** @file
  Header file for Driver health sample driver

;******************************************************************************
;* Copyright (c) 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#ifndef _DRIVER_HEALTH_SAMPLE_DXE_H_
#define _DRIVER_HEALTH_SAMPLE_DXE_H_

#include <Uefi.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DriverHealth.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/HiiConfigAccess.h>
#include <Guid/MdeModuleHii.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

typedef struct {
  VENDOR_DEVICE_PATH                         VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL                   End;
} DRIVER_HEALTH_SAMPLE_DEVICE_PATH;

typedef struct _DRIVER_HEALTH_SAMPLE_DEVICE_INFO {
  EFI_HANDLE                                 Controller;
  CHAR16                                     *DeviceName;
  EFI_DRIVER_HEALTH_STATUS                   HealthStatus;
  BOOLEAN                                    Reconnected;
} DRIVER_HEALTH_SAMPLE_DEVICE_INFO;

EFI_STATUS
EFIAPI
DriverHealthSampleFakeExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  );

EFI_STATUS
EFIAPI
DriverHealthSampleFakeRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  );

EFI_STATUS
EFIAPI
DriverHealthSampleCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  );

EFI_STATUS
EFIAPI
DriverHealthSampleDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL             *This,
  IN EFI_HANDLE                              Controller,
  IN EFI_DEVICE_PATH_PROTOCOL                *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
DriverHealthSampleDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL             *This,
  IN EFI_HANDLE                              Controller,
  IN EFI_DEVICE_PATH_PROTOCOL                *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
DriverHealthSampleDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL            *This,
  IN  EFI_HANDLE                             Controller,
  IN  UINTN                                  NumberOfChildren,
  IN  EFI_HANDLE                             *ChildHandleBuffer
  );

EFI_STATUS
EFIAPI
DriverHealthSampleGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL            *This,
  IN  CHAR8                                  *Language,
  OUT CHAR16                                 **DriverName
  );

EFI_STATUS
EFIAPI
DriverHealthSampleGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL            *This,
  IN  EFI_HANDLE                             ControllerHandle,
  IN  EFI_HANDLE                             ChildHandle        OPTIONAL,
  IN  CHAR8                                  *Language,
  OUT CHAR16                                 **ControllerName
  );

EFI_STATUS
DriverHealthSampleGetHealthStatus (
  IN  EFI_DRIVER_HEALTH_PROTOCOL             *This,
  IN  EFI_HANDLE                             ControllerHandle OPTIONAL,
  IN  EFI_HANDLE                             ChildHandle      OPTIONAL,
  OUT EFI_DRIVER_HEALTH_STATUS               *HealthStatus,
  OUT EFI_DRIVER_HEALTH_HII_MESSAGE          **MessageList    OPTIONAL,
  OUT EFI_HII_HANDLE                         *FormHiiHandle   OPTIONAL
  );

EFI_STATUS
DriverHealthSampleRepair (
  IN  EFI_DRIVER_HEALTH_PROTOCOL             *This,
  IN  EFI_HANDLE                             ControllerHandle,
  IN  EFI_HANDLE                             ChildHandle       OPTIONAL,
  IN  EFI_DRIVER_HEALTH_REPAIR_NOTIFY        RepairNotify      OPTIONAL
  );

extern UINT8                                 DriverHealthSampleVfrBin[];
#endif
