/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <Uefi/UefiSpec.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AmdMpmGetSensorInfoProtocol.h>
#include <Protocol/AmdPldmProtocol.h>
#include <Library/MpmLib.h>
#include <Library/IdsLib.h>
#include <Library/UefiLib.h>
#include <Filecode.h>
#include <Library/BaseMemoryLib.h>

#define FILECODE MPM_MPMPLDM_MPMPLDMMONITOR_MPMPLDMSENSOR_FILECODE
PLDM_SENSOR_T                    mSensorInfo;

/**
  Read Sensor info call back function.

  Platform sensor data collection driver need to install gAmdMpmGetSensorInfoProtocol,
  Then this function send the PLDM Platform_Event_Message command to MPM.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.
**/
VOID
EFIAPI
ReadSensorInfoCallback(
IN  EFI_EVENT          Event,
IN  VOID               *Context
)
{
  EFI_STATUS                       Status;
  AMD_MPM_GET_SENSOR_INFO_PROTOCOL *AmdMpmGetSensorInfo = NULL;
  AMD_PLDM_PROTOCOL                *PldmProtocol;
  PLDM_SENSOR_T                    *SensorInfo;
  UINTN                            NoHandles = 0;
  EFI_HANDLE                       *Handles;
  UINTN                            Index = 0;

  IDS_HDT_CONSOLE_PSP_TRACE ("%a: enter\n", __FUNCTION__);
  SensorInfo = &mSensorInfo;
  Status = gBS->LocateProtocol (
             &gAmdPldmProtocolGuid,
             NULL,
             (VOID **) &PldmProtocol
           );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("ReadSensorInfoCallback, Locate AmdPldmProtocol, Status = %r\n", Status);
    if (Event != NULL) {
      gBS->CloseEvent (Event);
    }
    return;
  }

  Status = gBS->LocateHandleBuffer (
             ByProtocol,
             &gAmdMpmGetSensorInfoProtocol,
             NULL,
             &NoHandles,
             &Handles
           );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate gAmdMpmGetSensorInfoProtocol, NoHandles= %d Status = %r\n", NoHandles, Status);
    return;
  }

  for (Index = 0; Index < NoHandles; Index++) {
    Status = gBS->HandleProtocol (
      Handles[Index],
      &gAmdMpmGetSensorInfoProtocol,
      (VOID **)&AmdMpmGetSensorInfo
    );
    if (!EFI_ERROR (Status)) {
      //Init SensorInfo to 0 for each sensor
      SetMem (SensorInfo, sizeof (PLDM_SENSOR_T), 0);
      Status = AmdMpmGetSensorInfo->GetSensorInfo(SensorInfo);
      if (EFI_ERROR(Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("ReadSensorInfoCallback: Failed to get the Sensor Index: %d information\n", Index);
        return;
      }
      Status = PldmProtocol->SensorPlatformEventMessage (PldmProtocol, SensorInfo);
      if (EFI_ERROR(Status)) {
        if (Event != NULL) {
          gBS->CloseEvent (Event);
        }
         break;
      }
      IDS_HDT_CONSOLE_PSP_TRACE ("ReadSensorInfoCallback: Send Sensor Index: %d information to MPM,\
      SendorId = %d, Status: %r\n", Index, SensorInfo->Sensor.Header.SensorID,Status);
    }
  }
  return;
}

/**
  Register Read Sensor info call back function.

  Only create the 10S timer event when gAmdMpmGetSensorInfoProtocol installed.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.
**/
VOID
EFIAPI
RegisterReadSensorInfoCallback (
  IN  EFI_EVENT          Event,
  IN  VOID               *Context
)
{
  EFI_STATUS        Status;
  EFI_EVENT         ReadSensorInfoEvent;
  VOID              *Interface;

  IDS_HDT_CONSOLE_PSP_TRACE ("%a: enter\n", __FUNCTION__);
  Status = gBS->LocateProtocol (&gAmdMpmGetSensorInfoProtocol, NULL, &Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = gBS->CreateEvent (
             EVT_TIMER | EVT_NOTIFY_SIGNAL,
             TPL_CALLBACK,
             ReadSensorInfoCallback,
             NULL,
             &ReadSensorInfoEvent
             );
  if (EFI_ERROR(Status)) {
    return;
  }

  Status = gBS->SetTimer (ReadSensorInfoEvent, TimerPeriodic, 100000000U); // 10s
  if (EFI_ERROR (Status)) {
      gBS->CloseEvent (ReadSensorInfoEvent);
      return;
  }
}

EFI_STATUS
EFIAPI
MpmSensorInfoEntry (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  VOID              *Registration;

  IDS_HDT_CONSOLE_PSP_TRACE ("%a: enter\n", __FUNCTION__);
  if (PcdGetBool (PcdMpmWirelessManageability) == FALSE) {
    return EFI_UNSUPPORTED;
  }

  if (IsMpmEnabled() == FALSE) {
     return EFI_UNSUPPORTED;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("RegisterReadSensorInfoCallback\n\n");
  EfiCreateProtocolNotifyEvent (
      &gAmdMpmGetSensorInfoProtocol,
      TPL_CALLBACK,
      RegisterReadSensorInfoCallback,
      NULL,
      &Registration
      );

  return EFI_SUCCESS;
}
