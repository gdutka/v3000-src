/** @file
  Driver health sample driver which implement the driver health protocol instance.

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
#include "DriverHealthSampleDxe.h"

EFI_HII_HANDLE                     mDriverHealthSampleHiiHandle      = NULL;
EFI_GUID                           mDriverHealthSampleBindingGuid    = {0xd2781bab, 0x4f1d, 0x415a, {0x96, 0x31, 0x6a, 0x6f, 0xeb, 0xbc, 0x32, 0xad}};
DRIVER_HEALTH_SAMPLE_DEVICE_PATH   mDriverHealthSampleHiiDevicePath  = {{{HARDWARE_DEVICE_PATH,
                                                                          HW_VENDOR_DP,
                                                                          {(UINT8) (sizeof (VENDOR_DEVICE_PATH)),
                                                                           (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)}},
                                                                          EFI_CALLER_ID_GUID},
                                                                        {END_DEVICE_PATH_TYPE,
                                                                         END_ENTIRE_DEVICE_PATH_SUBTYPE,
                                                                         {(UINT8) (END_DEVICE_PATH_LENGTH),
                                                                          (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)}}
                                                                        };
EFI_HII_CONFIG_ACCESS_PROTOCOL     mDriverHealthSampleConfigAccess   = {DriverHealthSampleFakeExtractConfig,
                                                                        DriverHealthSampleFakeRouteConfig,
                                                                        DriverHealthSampleCallback
                                                                        };
EFI_DRIVER_HEALTH_PROTOCOL         mDriverHealthSampleDriverHealth   = {DriverHealthSampleGetHealthStatus,
                                                                        DriverHealthSampleRepair
                                                                        };
EFI_COMPONENT_NAME_PROTOCOL        mDriverHealthSampleComponentName  = {DriverHealthSampleGetDriverName,
                                                                        DriverHealthSampleGetControllerName,
                                                                        "eng"
                                                                        };
EFI_COMPONENT_NAME2_PROTOCOL       mDriverHealthSampleComponentName2 = {(EFI_COMPONENT_NAME2_GET_DRIVER_NAME)     DriverHealthSampleGetDriverName,
                                                                        (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME) DriverHealthSampleGetControllerName,
                                                                        "en"
                                                                        };
EFI_UNICODE_STRING_TABLE           mDriverHealthSampleNameTable[]    = {{"eng;en",
                                                                         L"DriverHealthSample Driver"
                                                                        },
                                                                        {NULL,
                                                                         NULL
                                                                        }};
EFI_DRIVER_BINDING_PROTOCOL        mDriverHealthSampleDriverBinding  = {DriverHealthSampleDriverSupported,
                                                                        DriverHealthSampleDriverStart,
                                                                        DriverHealthSampleDriverStop,
                                                                        0xa,
                                                                        NULL,
                                                                        NULL
                                                                        };
DRIVER_HEALTH_SAMPLE_DEVICE_INFO   mDriverHealthSampleDevList[] = {
                                     {NULL, L"Device 01 (healthy)"          , EfiDriverHealthStatusHealthy              , FALSE},
                                     {NULL, L"Device 02 (healthy)"          , EfiDriverHealthStatusHealthy              , FALSE},
                                     {NULL, L"Device 03 (RepairRequired)"   , EfiDriverHealthStatusRepairRequired       , FALSE},
                                     {NULL, L"Device 04 (RepairRequired)"   , EfiDriverHealthStatusRepairRequired       , FALSE},
                                     {NULL, L"Device 05 (ConfigRequired)"   , EfiDriverHealthStatusConfigurationRequired, FALSE},
                                     {NULL, L"Device 06 (ConfigRequired)"   , EfiDriverHealthStatusConfigurationRequired, FALSE},
                                     {NULL, L"Device 07 (Failed)"           , EfiDriverHealthStatusFailed               , FALSE},
                                     {NULL, L"Device 08 (Failed)"           , EfiDriverHealthStatusFailed               , FALSE},
                                     {NULL, L"Device 09 (ReconnectRequired)", EfiDriverHealthStatusReconnectRequired    , FALSE},
                                     {NULL, L"Device 10 (ReconnectRequired)", EfiDriverHealthStatusReconnectRequired    , FALSE},
                                     {NULL, L"Device 11 (RebootRequired)"   , EfiDriverHealthStatusRebootRequired       , FALSE},
                                     {NULL, L"Device 12 (RebootRequired)"   , EfiDriverHealthStatusRebootRequired       , FALSE},
                                     };
UINTN                              mDriverHealthSampleDevCount = sizeof(mDriverHealthSampleDevList) / sizeof(DRIVER_HEALTH_SAMPLE_DEVICE_INFO);


STATIC
DRIVER_HEALTH_SAMPLE_DEVICE_INFO *
GetDeviceInfo (
  IN EFI_HANDLE                        Controller
  )
{
  UINTN                                Index;

  for (Index = 0; Index < mDriverHealthSampleDevCount; Index++) {
    if (mDriverHealthSampleDevList[Index].Controller == Controller) {
      return &mDriverHealthSampleDevList[Index];
    }
  }

  return NULL;
}

//
// EFI_HII_CONFIG_ACCESS_PROTOCOL instance
//
EFI_STATUS
EFIAPI
DriverHealthSampleFakeExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *Progress = Request;
  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
DriverHealthSampleFakeRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
DriverHealthSampleCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  if ((Value == NULL) || (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

//
// EFI_DRIVER_BINDING_PROTOCOL instance
//
EFI_STATUS
EFIAPI
DriverHealthSampleDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL       *This,
  IN EFI_HANDLE                        Controller,
  IN EFI_DEVICE_PATH_PROTOCOL          *RemainingDevicePath
  )
{
  EFI_STATUS                           Status;
  VOID                                 *Instance;

  Status = gBS->HandleProtocol (
                  Controller,
                  &mDriverHealthSampleBindingGuid,
                  (VOID **) &Instance
                  );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->OpenProtocol (
                  Controller,
                  &mDriverHealthSampleBindingGuid,
                  (VOID **) &Instance,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return gBS->CloseProtocol (
                Controller,
                &mDriverHealthSampleBindingGuid,
                This->DriverBindingHandle,
                Controller
                );
}

EFI_STATUS
EFIAPI
DriverHealthSampleDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL       *This,
  IN EFI_HANDLE                        Controller,
  IN EFI_DEVICE_PATH_PROTOCOL          *RemainingDevicePath
  )
{
  VOID                                 *Instance;

  return gBS->OpenProtocol (
                Controller,
                &mDriverHealthSampleBindingGuid,
                (VOID **) &Instance,
                This->DriverBindingHandle,
                Controller,
                EFI_OPEN_PROTOCOL_BY_DRIVER
                );
}

EFI_STATUS
EFIAPI
DriverHealthSampleDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL      *This,
  IN  EFI_HANDLE                       Controller,
  IN  UINTN                            NumberOfChildren,
  IN  EFI_HANDLE                       *ChildHandleBuffer
  )
{
  DRIVER_HEALTH_SAMPLE_DEVICE_INFO     *DeviceInfo;

  DeviceInfo = GetDeviceInfo (Controller);
  if (DeviceInfo == NULL) {
    return EFI_UNSUPPORTED;
  }

  DeviceInfo->Reconnected = TRUE;

  gBS->CloseProtocol (
         Controller,
         &mDriverHealthSampleBindingGuid,
         This->DriverBindingHandle,
         Controller
         );

  return EFI_SUCCESS;
}

//
// EFI_COMPONENT_NAME_PROTOCOL instance
//
EFI_STATUS
EFIAPI
DriverHealthSampleGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL      *This,
  IN  CHAR8                            *Language,
  OUT CHAR16                           **DriverName
  )
{
  return LookupUnicodeString2 (
           Language,
           This->SupportedLanguages,
           mDriverHealthSampleNameTable,
           DriverName,
           (BOOLEAN)(This == &mDriverHealthSampleComponentName)
           );
}

EFI_STATUS
EFIAPI
DriverHealthSampleGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL      *This,
  IN  EFI_HANDLE                       ControllerHandle,
  IN  EFI_HANDLE                       ChildHandle        OPTIONAL,
  IN  CHAR8                            *Language,
  OUT CHAR16                           **ControllerName
  )
{
  DRIVER_HEALTH_SAMPLE_DEVICE_INFO     *DeviceInfo;

  if (ControllerHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (ChildHandle != NULL) {
    return EFI_UNSUPPORTED;
  }

  DeviceInfo = GetDeviceInfo (ControllerHandle);
  if (DeviceInfo == NULL) {
    return EFI_UNSUPPORTED;
  }

  *ControllerName = AllocateCopyPool (StrSize (DeviceInfo->DeviceName), DeviceInfo->DeviceName);
  return EFI_SUCCESS;
}

//
// EFI_DRIVER_HEALTH_PROTOCOL instance
//
EFI_STATUS
EFIAPI
DriverHealthSampleGetHealthStatus (
  IN  EFI_DRIVER_HEALTH_PROTOCOL       *This,
  IN  EFI_HANDLE                       ControllerHandle OPTIONAL,
  IN  EFI_HANDLE                       ChildHandle      OPTIONAL,
  OUT EFI_DRIVER_HEALTH_STATUS         *HealthStatus,
  OUT EFI_DRIVER_HEALTH_HII_MESSAGE    **MessageList    OPTIONAL,
  OUT EFI_HII_HANDLE                   *FormHiiHandle   OPTIONAL
  )
{
  UINTN                                Index;
  DRIVER_HEALTH_SAMPLE_DEVICE_INFO     *DeviceInfo;
  EFI_DRIVER_HEALTH_HII_MESSAGE        *MsgList;

  if (HealthStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (ControllerHandle == NULL) {
    for (Index = 0; Index < mDriverHealthSampleDevCount; Index++) {
      if (mDriverHealthSampleDevList[Index].HealthStatus != EfiDriverHealthStatusHealthy) {
        *HealthStatus = EfiDriverHealthStatusFailed;
        return EFI_SUCCESS;
      }
    }

    *HealthStatus = EfiDriverHealthStatusHealthy;
    return EFI_SUCCESS;
  }

  if (ChildHandle != NULL) {
    return EFI_UNSUPPORTED;
  }

  DeviceInfo = GetDeviceInfo (ControllerHandle);
  if (DeviceInfo == NULL) {
    return EFI_UNSUPPORTED;
  }

  *HealthStatus = DeviceInfo->HealthStatus;
  if (MessageList != NULL) {
    MsgList = NULL;
    if (DeviceInfo->HealthStatus == EfiDriverHealthStatusReconnectRequired) {
      MsgList = AllocateZeroPool (sizeof (EFI_DRIVER_HEALTH_HII_MESSAGE) * 2);
      if (MsgList != NULL) {
        MsgList->HiiHandle = mDriverHealthSampleHiiHandle;
        MsgList->StringId  = DeviceInfo->Reconnected ? STRING_TOKEN (STR_DRIVER_HEALTH_SAMPLE_RECONNECTED) :
                                                       STRING_TOKEN (STR_DRIVER_HEALTH_SAMPLE_NOT_RECONNECTED);
      }
    }
    *MessageList = MsgList;
  }
  if (FormHiiHandle != NULL) {
    *FormHiiHandle = (DeviceInfo->HealthStatus == EfiDriverHealthStatusConfigurationRequired) ? mDriverHealthSampleHiiHandle : NULL;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DriverHealthSampleRepair (
  IN EFI_DRIVER_HEALTH_PROTOCOL        *This,
  IN EFI_HANDLE                        ControllerHandle,
  IN EFI_HANDLE                        ChildHandle       OPTIONAL,
  IN EFI_DRIVER_HEALTH_REPAIR_NOTIFY   RepairNotify      OPTIONAL
  )
{
  DRIVER_HEALTH_SAMPLE_DEVICE_INFO     *DeviceInfo;

  if (ControllerHandle == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (ChildHandle != NULL) {
    return EFI_UNSUPPORTED;
  }

  DeviceInfo = GetDeviceInfo (ControllerHandle);
  if (DeviceInfo == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (DeviceInfo->HealthStatus != EfiDriverHealthStatusRepairRequired) {
    return EFI_UNSUPPORTED;
  }

  DeviceInfo->HealthStatus = EfiDriverHealthStatusHealthy;
  return EFI_SUCCESS;
}

/**
  Install necessary protocol instances.

  @param[in] ImageHandle     The image handle.
  @param[in] SystemTable     The system table.

  @retval  EFI_SUCEESS       Success to install necessary protocol instances
  @retval  Other             Failed to install protocol
**/
EFI_STATUS
EFIAPI
DriverHealthSampleDxeEntry (
  IN EFI_HANDLE                        ImageHandle,
  IN EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_STATUS                           Status;
  UINTN                                Index;
  EFI_HANDLE                           Handle;

  //
  // Install HII related protocol and data
  //
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEfiDevicePathProtocolGuid,
                  &mDriverHealthSampleHiiDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mDriverHealthSampleConfigAccess,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mDriverHealthSampleHiiHandle = HiiAddPackages (
                                   &gEfiCallerIdGuid,
                                   Handle,
                                   DriverHealthSampleVfrBin,
                                   STRING_ARRAY_NAME,
                                   NULL
                                   );
  if (mDriverHealthSampleHiiHandle == NULL) {
    return Status;
  }

  //
  // Install driver binding, component name(2) and driver health protocols
  //
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &mDriverHealthSampleDriverBinding,
             ImageHandle,
             &mDriverHealthSampleComponentName,
             &mDriverHealthSampleComponentName2
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiDriverHealthProtocolGuid,
                  &mDriverHealthSampleDriverHealth,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Produce fake devices which are controled by this driver
  //
  for (Index = 0; Index < mDriverHealthSampleDevCount; Index++) {
    gBS->InstallProtocolInterface (
           &mDriverHealthSampleDevList[Index].Controller,
           &mDriverHealthSampleBindingGuid,
           EFI_NATIVE_INTERFACE,
           &mDriverHealthSampleDevList[Index]
           );
  }

  return EFI_SUCCESS;
}

