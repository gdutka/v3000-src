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
#include <Protocol/AmdMpmGetBatteryInfoProtocol.h>
#include <Library/MpmLib.h>
#include <Library/IdsLib.h>
#include <Filecode.h>
#include <Library/BaseMemoryLib.h>

#define FILECODE MPM_MPMBATTERYINFO_MPMBATTERYINFO_FILECODE

BATTERY_INFO_T                    mBatteryInfo;
VOID
EFIAPI
ReadBatteryInfoCallback(
IN  EFI_EVENT          Event,
IN  VOID               *Context
)
{
  EFI_STATUS                        Status;
  AMD_MPM_GET_BATTERY_INFO_PROTOCOL *AmdMpmGetBatteryInfo = NULL;
  BATTERY_INFO_T                    *BatteryInfo;

  IDS_HDT_CONSOLE_PSP_TRACE ("%a: enter\n", __FUNCTION__);
  Status = gBS->LocateProtocol (&gAmdMpmGetBatteryInfoProtocol, NULL, (VOID**)&AmdMpmGetBatteryInfo);
  if (EFI_ERROR(Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("ReadBatteryInfoCallback: failed to locate gAmdMpmGetBatteryInfoProtocol, Status = %r\n", Status);
    return;
  }

  BatteryInfo = &mBatteryInfo;
  SetMem (BatteryInfo, sizeof (BATTERY_INFO_T), 0);
  Status = AmdMpmGetBatteryInfo->GetBatteryInfo(BatteryInfo);
  if (EFI_ERROR(Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("ReadBatteryInfoCallback: Failed to get the battery information\n");
    return;
  }

  Status = BiosMpmBatteryMsg(BatteryInfo);
  if (EFI_ERROR(Status)) {
    if (Event != NULL) {
      gBS->CloseEvent (Event);
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("ReadBatteryInfoCallback: Send battery information to MPM, Status : %r\n", Status);
}

EFI_STATUS
EFIAPI
MpmBatteryInfoEntry (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS        Status;
  EFI_EVENT         ReadBatteryInfoEvent;

  IDS_HDT_CONSOLE_PSP_TRACE ("%a: enter\n", __FUNCTION__);
  if (PcdGetBool (PcdMpmWirelessManageability) == FALSE) {
    return EFI_UNSUPPORTED;
  }

  if (IsMpmEnabled() == FALSE) {
     return EFI_UNSUPPORTED;
  }

  Status = gBS->CreateEvent (
             EVT_TIMER | EVT_NOTIFY_SIGNAL,
             TPL_CALLBACK,
             ReadBatteryInfoCallback,
             NULL,
             &ReadBatteryInfoEvent
             );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = gBS->SetTimer (ReadBatteryInfoEvent, TimerPeriodic, 100000000U); // 10s
  if (EFI_ERROR (Status)) {
      gBS->CloseEvent (ReadBatteryInfoEvent);
      return Status;
  }

  return Status;
}
