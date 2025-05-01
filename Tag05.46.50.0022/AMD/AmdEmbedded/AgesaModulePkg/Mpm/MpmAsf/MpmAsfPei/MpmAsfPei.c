/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include "MpmAsfPei.h"

#include <Library/SerialPortLib.h>
#include <Library/PrintLib.h>
#include <Library/IoLib.h>
#include <Filecode.h>
#include <Library/AmdRtcLib.h>

#define FILECODE MPM_MPMASF_MPMASFPEI_MPMASFPEI_FILECODE
EFI_STATUS
AsfPushMessage (
  IN ASF_MSG_ID  MsgId
);

EFI_STATUS
EFIAPI
AsfMemoryCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
);

static EFI_PEI_NOTIFY_DESCRIPTOR AsfMemoryMessageNotify[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST, \
    &gEfiPeiMemoryDiscoveredPpiGuid, \
    AsfMemoryCallback,
  }
};

EFI_STATUS
EFIAPI
AsfMemoryCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
)
{
  EFI_STATUS      Status;
  EFI_BOOT_MODE   BootMode;
  IDS_HDT_CONSOLE_PSP_TRACE ("%a\n", __FUNCTION__);

  // Push "Memory Init End" Message
  IDS_HDT_CONSOLE_PSP_TRACE ("%a, push memory init end message\n", __FUNCTION__);
  AsfPushMessage(MsgMemIntiExit);

  // Push "Cache Init" Message
  IDS_HDT_CONSOLE_PSP_TRACE ("%a, push cache init message\n", __FUNCTION__);
  AsfPushMessage(MsgCacheInitExit);

  // Push "System State of S0" if resuming from S3
  Status = PeiServicesGetBootMode(&BootMode);
  if (!EFI_ERROR (Status) && (BootMode == BOOT_ON_S3_RESUME)) {
    AsfPushMessage(MsgSystemStateS0);
  }

  return Status;
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
AsfGetDateTime (EFI_TIME *AsfTime)
{
  RTC_REGISTER_B  RegisterB;
  EFI_STATUS  Status;
  //
  // Read Register B
  //
  RegisterB.Data = RtcRead (RTC_ADDRESS_REGISTER_B);
  //
  // Get the Time/Date/Daylight Savings values.
  //
  AsfTime->Second  = RtcRead (RTC_ADDRESS_SECONDS);
  AsfTime->Minute  = RtcRead (RTC_ADDRESS_MINUTES);
  AsfTime->Hour    = RtcRead (RTC_ADDRESS_HOURS);
  AsfTime->Day     = RtcRead (RTC_ADDRESS_DAY_OF_THE_MONTH);
  AsfTime->Month   = RtcRead (RTC_ADDRESS_MONTH);
  AsfTime->Year    = RtcRead (RTC_ADDRESS_YEAR);
  Status = ConvertRtcTimeToEfiTime (AsfTime, RegisterB);
  if (!EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Time Stamp = %d-%d-%d, %d:%d:%d\n", AsfTime->Year, AsfTime->Month, AsfTime->Day, AsfTime->Hour, AsfTime->Minute, AsfTime->Second);
  }
  return Status;
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

  IDS_HDT_CONSOLE_PSP_TRACE ("%a\n", __FUNCTION__);

  for (Index = 0; Index < sizeof(mAsfMsgList) / sizeof(ASF_MESSAGE); Index++) {
    if (mAsfMsgList[Index].MsgId == MsgId) {
      Msg = mAsfMsgList[Index];
      IsFound = TRUE;
      break;
    }
  }
  if (!IsFound) {
    IDS_HDT_CONSOLE_PSP_TRACE ("%a, return EFI_NOT_FOUND\n", __FUNCTION__);
    Status = EFI_NOT_FOUND;
    return Status;
  }

  AsfGetDateTime(&Time);
  Msg.AsfTime.Year = Time.Year;
  Msg.AsfTime.Day = Time.Day;
  Msg.AsfTime.Month = Time.Month;
  Msg.AsfTime.Day = Time.Day;
  Msg.AsfTime.Hour = Time.Hour;
  Msg.AsfTime.Minute = Time.Minute;
  Msg.AsfTime.Second = Time.Second;
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

EFI_STATUS
EFIAPI
AsfPeiStatusCodelistener (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN EFI_STATUS_CODE_TYPE         Type,
  IN EFI_STATUS_CODE_VALUE        Value,
  IN UINT32                       Instance,
  IN EFI_GUID                     *CallerId,
  IN EFI_STATUS_CODE_DATA         *Data
)
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINTN           Index;

  //
  // Filter the Status Code and push the associated message
  //
  for (Index = 0; Index < sizeof(mAsfMsgList) / sizeof(ASF_MESSAGE); Index++) {
    if ((mAsfMsgList[Index].MsgData.StatusCode == Value) &&
        (mAsfMsgList[Index].MsgType == Type)) {
      Status = AsfPushMessage(mAsfMsgList[Index].MsgId);
      if (EFI_ERROR(Status)) {
        break;
      }
    }
  }

  return Status;
}

EFI_STATUS
EFIAPI
MpmAsfPeiEntry (
  IN        EFI_PEI_FILE_HANDLE   FileHanle,
  IN CONST  EFI_PEI_SERVICES      **PeiServices
)
{
  EFI_STATUS                Status;
  EFI_PEI_RSC_HANDLER_PPI   *RscHandler;

  IDS_HDT_CONSOLE_PSP_TRACE ("%a enter\n", __FUNCTION__);

  if (PcdGetBool (PcdMpmWirelessManageability) == FALSE) {
    return EFI_UNSUPPORTED;
  }

  if (!IsMpmEnabled()) {
    return EFI_UNSUPPORTED;
  }

  Status = PeiServicesNotifyPpi(AsfMemoryMessageNotify);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Notify AsfMemoryMessageNotify failed\n");
    return Status;
  }

  Status = PeiServicesLocatePpi (
                    &gEfiPeiRscHandlerPpiGuid,
                    0,
                    NULL,
                    (VOID **) &RscHandler
                    );
  if (!EFI_ERROR (Status)) {
    Status = RscHandler->Register((EFI_PEI_RSC_HANDLER_CALLBACK)AsfPeiStatusCodelistener);
  } else {
    return Status;
  }

  // Push "Stop watchdog timer" message
  IDS_HDT_CONSOLE_PSP_TRACE ("%a, push stop watchdog timer message\n", __FUNCTION__);
  AsfPushMessage(MsgStopWdt);

  return Status;
}
