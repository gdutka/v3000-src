/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/


#include "MpmAsfDxe.h"

#include <Library/SerialPortLib.h>
#include <Library/PrintLib.h>
#include <Filecode.h>

#define FILECODE MPM_MPMASF_MPMASFDXE_MPMASFDXE_FILECODE

EFI_STATUS
EFIAPI
AsfRscHandler (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId,
  IN EFI_STATUS_CODE_DATA     *Data
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINTN                 Index;

  for (Index = 0; Index < sizeof(mAsfMsgList) / sizeof(ASF_MESSAGE); Index++) {
    if ((mAsfMsgList[Index].MsgData.StatusCode == Value) &&
        (mAsfMsgList[Index].MsgType == CodeType)) {
      Status = AsfPushMessage(mAsfMsgList[Index].MsgId);
      if (EFI_ERROR(Status)) {
        break;
      }
    }
  }

  if (Value == PcdGet32(PcdProgressCodeOsLoaderStart)) {
    Status = AsfPushMessage(MsgStartOsWdt);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    Status = AsfPushMessage(MsgStartOSBootEntry);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  if (Value == (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_DXE_BS_EC_BOOT_OPTION_FAILED)) {
    // Boot failed, send "stop watchdog timer" command
    Status = AsfPushMessage(MsgStopWdt);
    if (EFI_ERROR(Status)) {
      return Status;
    }
  }

  return Status;
}

VOID
EFIAPI
AsfReadyToBootCallBack (
  IN EFI_EVENT      Event,
  IN VOID           *Context
)
{
  // Push message: BIOS boot complete
  AsfPushMessage(MsgBiosBootCompleteExit);

  //Push System State S0 - "Working"
  AsfPushMessage(MsgSystemStateS0);

  gBS->CloseEvent(Event);
  return;
}

STATIC
VOID
EFIAPI
AsfExitBootServiceCallBack (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_RSC_HANDLER_PROTOCOL *RscHandler;

  RscHandler = Context;
  RscHandler->Unregister (AsfRscHandler);
}

VOID
PrintMsgName(
  IN ASF_MSG_ID MsgId
)
{
  UINTN           Index;
  CHAR8           PrintBuffer[0x100];
  UINTN           CharCount;

  PrintBuffer[0] = '\0';

  for (Index = 0; Index < sizeof(gAsfMsgNameMap) / sizeof(ASF_MSG_NAME_MAP); Index++) {
    if (MsgId == gAsfMsgNameMap[Index].MsgId) {
      CharCount = AsciiSPrint (
              PrintBuffer,
              sizeof (PrintBuffer),
              "    Message Name: %s\n",
              gAsfMsgNameMap[Index].MsgName
              );
      SerialPortWrite ((UINT8 *) PrintBuffer, CharCount);
      return;
    }
  }
}

EFI_STATUS
AsfPushMessage (
  IN ASF_MSG_ID  MsgId
)
{
  EFI_STATUS    Status = EFI_NOT_FOUND;
  ASF_MESSAGE   Msg;
  UINTN         Index;
  BOOLEAN       IsFound = FALSE;
  EFI_TIME      Time;

  for (Index = 0; Index < sizeof(mAsfMsgList) / sizeof(ASF_MESSAGE); Index++) {
    if (mAsfMsgList[Index].MsgId == MsgId) {
      Msg = mAsfMsgList[Index];
      IsFound = TRUE;
      break;
    }
  }

  if (!IsFound) {
    Status = EFI_NOT_FOUND;
    return Status;
  }
  Status = gRT->GetTime (&Time, NULL);
  if (!EFI_ERROR (Status)) {
    Msg.AsfTime.Year = Time.Year;
    Msg.AsfTime.Day = Time.Day;
    Msg.AsfTime.Month = Time.Month;
    Msg.AsfTime.Day = Time.Day;
    Msg.AsfTime.Hour = Time.Hour;
    Msg.AsfTime.Minute = Time.Minute;
    Msg.AsfTime.Second = Time.Second;
    IDS_HDT_CONSOLE_PSP_TRACE ("Time Stamp = %d-%d-%d, %d:%d:%d\n", Msg.AsfTime.Year, Msg.AsfTime.Month, Msg.AsfTime.Day, Msg.AsfTime.Hour, Msg.AsfTime.Minute, Msg.AsfTime.Second);
  }

  {
    // only for debug
    CHAR8           PrintBuffer[0x100];
    UINTN           CharCount;
    PrintBuffer[0] = '\0';
    CharCount = AsciiSPrint (
                    PrintBuffer,
                    sizeof (PrintBuffer),
                    "AsfPushMessage: Message ID = 0x%x, Message Type = 0x%x\n",
                    Msg.MsgId,
                    Msg.MsgType
                    );
    SerialPortWrite ((UINT8 *) PrintBuffer, CharCount);
    PrintMsgName(MsgId);
  }

  Status = BiosMpmAlertMsg(&Msg, sizeof(ASF_MESSAGE));

  return Status;
}

VOID
EFIAPI
AsfRscProtocolCallback (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  )
{
  EFI_STATUS                        Status;
  EFI_RSC_HANDLER_PROTOCOL  *RscHandler;

  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &RscHandler
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate gEfiRscHandlerProtocolGuid failed\n");
    return;
  }

  //
  // Register the worker function to ReportStatusCodeRouter
  //
  Status = RscHandler->Register (AsfRscHandler, TPL_CALLBACK);

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("RscHandler->Register failed\n");
    return;
  }

  return;
}

EFI_STATUS
EFIAPI
MpmAsfDxeEntry (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS                Status;
  EFI_RSC_HANDLER_PROTOCOL  *RscHandler;
  EFI_EVENT                 AsfReadyToBoot;
  VOID                      *Registration;
  EFI_EVENT                 ExitBootEvent;

  IDS_HDT_CONSOLE_PSP_TRACE ("%a, Enter\n", __FUNCTION__);

  if (PcdGetBool (PcdMpmWirelessManageability) == FALSE) {
    return EFI_UNSUPPORTED;
  }

  // Check MPM Enable
  if (!IsMpmEnabled()) {
    return EFI_UNSUPPORTED;
  }

  // Push "Motherboard init" message
  AsfPushMessage(MsgMotherBoardInitExit);


  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &RscHandler
                  );
  if (EFI_ERROR (Status)) {
    //
    // Create callback for registering the Beep Status Code worker function.
    //
    EfiCreateProtocolNotifyEvent (
      &gEfiRscHandlerProtocolGuid,
      TPL_NOTIFY,
      AsfRscProtocolCallback,
      NULL,
      &Registration
      );
  } else {
    //
    // Register the worker function to ReportStatusCodeRouter.
    //
    Status = RscHandler->Register (AsfRscHandler, TPL_CALLBACK);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  // Create ReadyToBoot event call back for ASF bootoverride and SMBios
  Status = EfiCreateEventReadyToBootEx (
                TPL_CALLBACK,
                AsfReadyToBootCallBack,
                NULL,
                &AsfReadyToBoot);

  Status = gBS->CreateEvent (
                  EVT_SIGNAL_EXIT_BOOT_SERVICES, // Type
                  TPL_CALLBACK,                  // NotifyTpl
                  AsfExitBootServiceCallBack,    // NotifyFunction
                  RscHandler,                    // NotifyContext
                  &ExitBootEvent                 // Event
                  );

  return EFI_SUCCESS;
}
