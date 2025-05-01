/*****************************************************************************
 * Copyright 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************
*/

#include "AsfMsg.h"

EFI_SMBUS_HC_PROTOCOL    *mSmBusHc;
EFI_SMBUS_DEVICE_ADDRESS mSmbusDeviceAddr;
COMMON_SETUP_OPTIONS     mCommonSetupOptions;

/**
  @brief This function will change ASF mode to master or slave.

  @param[in] SetToMasterMode    - Mode change.

  @return EFI_STATUS
**/
EFI_STATUS
ChangeAsfSmbusModeToMaster(
  BOOLEAN   SetToMasterMode
)
{
  UINT8 AsfHostStatus;
  UINT8 AsfStatus;
  UINT8 SemaphoreStatus;

  if (SetToMasterMode) {
    // Check HostBusy and SlaveBusy are idle
    do{
      AsfHostStatus = DashSmbusIoRead(ASF_HOST_STATUS);
      AsfStatus = DashSmbusIoRead(ASF_STATUS);
    }while((AsfHostStatus & BIT0) || (AsfStatus & BIT7));

    DashAsfMasterEnable(TRUE);
    DashResetAsfSlave();

    //
    // Read EcSemaphre to check HostSemaphore
    //
    do{
      SemaphoreStatus = DashSmbusIoRead(ASF_SEMAPHORE);
    }while(SemaphoreStatus & BIT2);

    SemaphoreStatus |= BIT0;
    DashSmbusIoWrite(ASF_SEMAPHORE, SemaphoreStatus);
  } else {
    //
    // Clear HostSemaphore
    //
    SemaphoreStatus = DashSmbusIoRead(ASF_SEMAPHORE);
    SemaphoreStatus |= BIT1;
    DashSmbusIoWrite(ASF_SEMAPHORE, SemaphoreStatus);

    DashAsfMasterEnable(FALSE);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SendAsfMsg(
  IN UINT8  Command,
  IN UINTN  Length,
  IN VOID   *Buffer
)
{
  EFI_STATUS  Status;
  BOOLEAN     PecCheck = 1;

  ChangeAsfSmbusModeToMaster(TRUE);
  Status = mSmBusHc->Execute (
                    mSmBusHc,
                    mSmbusDeviceAddr,
                    Command,
                    EfiSmbusWriteBlock,
                    PecCheck,
                    &Length,
                    Buffer);
  ChangeAsfSmbusModeToMaster(FALSE);
  return Status;
}

EFI_STATUS
EFIAPI
GetAsfMsg(
  IN     UINT8  Command,
  IN     UINTN  Length,
  IN OUT VOID   *Buffer
)
{
  EFI_STATUS  Status;
  BOOLEAN     PecCheck = 1;

  ChangeAsfSmbusModeToMaster(TRUE);
  Status = mSmBusHc->Execute (
                    mSmBusHc,
                    mSmbusDeviceAddr,
                    Command,
                    EfiSmbusReadBlock,
                    PecCheck,
                    &Length,
                    Buffer);
  ChangeAsfSmbusModeToMaster(FALSE);
  return Status;
}

EFI_STATUS
EFIAPI
GetBootOptions()
{
  EFI_STATUS        Status;
  UINTN             DataLength;
  UINT8             Buffer[RETURN_BOOT_OPTIONS_RESPONSE_SIZE];
  UINT8             Command;

  DataLength = RETURN_BOOT_OPTIONS_RESPONSE_SIZE;
  Command = ASF_MESSAGE_CONFIGURATION;

  Status = GetAsfMsg(Command, DataLength, Buffer);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "[AsfMsg] Get GetBootOptions Asf Message fail!\n"));
  }

  return Status;
}

EFI_STATUS
EFIAPI
AsfMsgBootOptionsClear()
{
  EFI_STATUS                  Status;
  ASF_MSG_BOOT_OPTIONS_CLEAR  AsfBootOptionsClear;
  UINTN                       DataLength;
  UINT8                       Command;

  DataLength = BOOT_OPTIONS_CLEAR_SIZE;
  Command = ASF_MESSAGE_CONFIGURATION;

  AsfBootOptionsClear.SubCmd = ASF_MESSAGE_CLEAR_BOOT_OPTIONS;
  AsfBootOptionsClear.Version = ASF_MESSAGE_VERSIONNUMBER;

  Status = SendAsfMsg(Command, DataLength, &AsfBootOptionsClear);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "[AsfMsg] Get AsfMsgBootOptionsClear Asf Message fail!\n"));
  }

  return Status;
}

EFI_STATUS
EFIAPI
AsfMsgStartWatchdog (
  IN UINT8  Entity,
  IN UINT8  Data1,
  IN UINT16 TimeOut
  )
{
  EFI_STATUS                    Status;
  ASF_MSG_START_WATCHDOG_TIMER  AsfStartWatchDog;
  UINTN                         DataLength;
  UINT8                         Command;

  DataLength = sizeof(ASF_MSG_START_WATCHDOG_TIMER);
  Command = ASF_MESSAGE_MANAGEMENT_CONTROL;

  AsfStartWatchDog.SubCmd = ASF_MESSAGE_START_WATCH_DOG_TIMER_SUBCOMMAND;
  AsfStartWatchDog.Version = ASF_MESSAGE_VERSIONNUMBER;
  AsfStartWatchDog.TimeoutLo = (UINT8)TimeOut;
  AsfStartWatchDog.TimeoutHi = (UINT8)(TimeOut >> 8);
  AsfStartWatchDog.EventSensorType = ASF_EVENT_SENSOR_TYPE_WATCHDOG2;
  AsfStartWatchDog.EventType = ASF_EVENT_TYPE_SENSOR_SPECIFIC;
  AsfStartWatchDog.EventOffset = ASF_EVENT_OFFSET_TIMER_EXPIRED;
  AsfStartWatchDog.EventSourceType = ASF_EVENT_SOURCE_TYPE_ASF_1_0_IMPLEMENTATION;
  AsfStartWatchDog.EventSeverity = ASF_EVENT_SEVERITY_CRITICAL_CONDITION;
  AsfStartWatchDog.SensorDevice = ASF_SENSOR_DEVICE_UNSPECIFIED;
  AsfStartWatchDog.SensorNumber = ASF_SENSOR_NUMBER_UNSPECIFIED;
  AsfStartWatchDog.Entity = Entity;
  AsfStartWatchDog.EntityInstance  = 0;
  AsfStartWatchDog.Data0 = ASF_WATCHDOG_DATA0;
  AsfStartWatchDog.Data1 = Data1;
  AsfStartWatchDog.Data2 = 0;
  AsfStartWatchDog.Data3 = 0;
  AsfStartWatchDog.Data4 = 0;
  AsfStartWatchDog.Data5 = 0;

  Status = SendAsfMsg (Command, DataLength, &AsfStartWatchDog);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "[AsfMsg] Send AsfMsgStartWatchdog Asf Message fail!\n"));
  }

  return Status;
}

EFI_STATUS
EFIAPI
AsfMsgStopWatchdog (VOID)
{
  EFI_STATUS                  Status;
  ASF_MSG_STOP_WATCHDOG_TIMER AsfStopWatchDog;
  UINTN                       DataLength;
  UINT8                       Command;

  DataLength = sizeof(ASF_MSG_STOP_WATCHDOG_TIMER);
  Command = ASF_MESSAGE_MANAGEMENT_CONTROL;

  AsfStopWatchDog.SubCmd = ASF_MESSAGE_STOP_WATCH_DOG_TIMER_SUBCOMMAND;
  AsfStopWatchDog.Version = ASF_MESSAGE_VERSIONNUMBER;

  Status = SendAsfMsg (Command, DataLength, &AsfStopWatchDog);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "[AsfMsg] Send AsfMsgStopWatchdog Asf Message fail!\n"));
  }

  return Status;
}

EFI_STATUS
EFIAPI
AsfMsgSetSystemState(
  UINT8 State
)
{
  EFI_STATUS                Status;
  UINTN                     DataLength;
  ASF_MSG_SET_SYSTEM_STATE  AsfSetSystemState;
  UINT8                     Command;

  AsfSetSystemState.SubCmd = ASF_MESSAGE_SYSTEM_STATE_SUBCOMMAND;
  AsfSetSystemState.Version = ASF_MESSAGE_VERSIONNUMBER;
  AsfSetSystemState.SystemState = State;

  DataLength = sizeof(ASF_MSG_SET_SYSTEM_STATE);
  Command = ASF_MESSAGE_SENSOR_DEVICE_AND_SYSTEM_STATE;

  Status = SendAsfMsg (Command, DataLength, &AsfSetSystemState);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "[AsfMsg] Send AsfMsgSetSystemState Asf Message fail!\n"));
  }

  return Status;
}

VOID
AsfMsgPciEnumerationCompleteCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                      Status;
  UINT8                           Command;
  UINTN                           DataLength;
  ASF_MSG_WITHOUT_RETRANSMISSION  AsfWithOutRetransmission;
  VOID                            *Interface;

  DEBUG ((DEBUG_INFO, "[AsfMsg] AsfMsgPciEnumerationCompleteCallBack Entry!\n"));

  Status = gBS->LocateProtocol (
              &gEfiPciEnumerationCompleteProtocolGuid,
              NULL,
              &Interface
              );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[AsfMsg] Locate gEfiPciEnumerationCompleteProtocolGuid Protocol fail!\n"));
    return;
  }

  Command = ASF_MESSAGE_MESSAGING;
  DataLength = MESSAGE_WITHOUT_RETRANSMISSION_SIZE;

  AsfWithOutRetransmission.SubCmd = ASF_MESSAGE_WITHOUT_RETRANSMISSION;
  AsfWithOutRetransmission.Version = ASF_MESSAGE_VERSIONNUMBER;
  AsfWithOutRetransmission.EventSensorType = ASF_EVENT_SENSOR_TYPE_SYS_FW_ERR_OR_PROGRESS;
  AsfWithOutRetransmission.EventType = ASF_EVENT_TYPE_SENSOR_SPECIFIC;
  AsfWithOutRetransmission.EventOffset = ASF_EVENT_OFFSET_SYSTEM_FIRMWARE_PROGRESS_ENTRY;
  AsfWithOutRetransmission.EventSourceType = ASF_EVENT_SOURCE_TYPE_ASF_1_0_IMPLEMENTATION;
  AsfWithOutRetransmission.EventSeverity = ASF_EVENT_SEVERITY_NON_CRITICAL;
  AsfWithOutRetransmission.SensorDevice = ASF_SENSOR_DEVICE_UNSPECIFIED;
  AsfWithOutRetransmission.SensorNumber = ASF_SENSOR_NUMBER_UNSPECIFIED;
  AsfWithOutRetransmission.Entity = ASF_ENTITY_CODE_BIOS;
  AsfWithOutRetransmission.EntityInstance = 0;
  AsfWithOutRetransmission.Data0 = ASF_SYSTEM_FW_PROGRESS_DATA0;
  AsfWithOutRetransmission.Data1 = ASF_SYSTEM_FW_PRPGRESS_EVENT_CODE_PCI_RESOURCE_CONFIG;

  Status = SendAsfMsg (Command, DataLength, &AsfWithOutRetransmission);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "[AsfMsg] Send AsfWithOutRetransmission Asf Message fail!\n"));
  }

  DEBUG ((DEBUG_INFO, "[AsfMsg] AsfMsgPciEnumerationCompleteCallBack Exit!\n"));
  return;
}

VOID
EFIAPI
AsfMsgExitBootServicesCallBack (
  IN EFI_EVENT      Event,
  IN VOID           *Context
)
{
  UINT16  OSTimeOutValue;

  OSTimeOutValue = mCommonSetupOptions.DashOSWatchDogToimeOut;
  AsfMsgStartWatchdog(ASF_ENTITY_CODE_OS, ASF_OS_BOOT_FAILURED, OSTimeOutValue);

  gBS->CloseEvent(Event);
  return;
}

EFI_STATUS
EFIAPI
AsfMsgEntry (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS      Status;
  SMBUS_INSTANCE  *AmdSmbus;
  VOID            *Register;
  EFI_EVENT       AsfMsgExitBootServices;
  UINT16          BiosTimeOutValue;

  DEBUG ((DEBUG_INFO, "AsfMsg Driver Entry!!\n"));

  Status = GetCommonSetupOptions (&mCommonSetupOptions);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO,"[AsfMsgEntry] Get Dash variable fail.\n"));
    return Status;
  }

  if (mCommonSetupOptions.DashOption != 1) {
    DEBUG ((DEBUG_INFO,"[AsfMsgEntry] Dash variable is Disabled.\n"));
    return Status;
  }

  if (mCommonSetupOptions.AsfMsgOption != 1) {
    DEBUG ((DEBUG_INFO,"[AsfMsgEntry] AsfMsg variable is Disabled.\n"));
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gEfiSmbusHcProtocolGuid,
                  NULL,
                  &mSmBusHc);

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO,"[AsfMsgEntry] Locate SMBUS Protocol Fail, %r.\n", Status));
    return Status;
  }

  AmdSmbus = SMBUS_INSTANCE_FROM_THIS(mSmBusHc);
  AmdSmbus++; // ASF SMBUS
  mSmBusHc = (EFI_SMBUS_HC_PROTOCOL *)&AmdSmbus->SmbusController;

  mSmbusDeviceAddr.SmbusDeviceAddress = SMBUS_LIB_SLAVE_ADDRESS(LAN_SMBUS_SLAVE_ADDRESS);
  DEBUG ((DEBUG_INFO, "[AsfMsgEntry] mSmbusDeviceAddr.SmbusDeviceAddress = 0x%x\n", mSmbusDeviceAddr.SmbusDeviceAddress));

  //
  // Send start WatchDog Asf message
  //
  BiosTimeOutValue = mCommonSetupOptions.DashBIOSWatchDogToimeOut;
  AsfMsgStartWatchdog(ASF_ENTITY_CODE_BIOS, ASF_SYSTEM_BOOT_FAILURED, BiosTimeOutValue);

  //
  // Send set system state to S0
  //
  AsfMsgSetSystemState(SYSTEM_STATE_S0);

  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_NOTIFY,
    AsfMsgPciEnumerationCompleteCallBack,
    NULL,
    &Register
    );

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AsfMsgExitBootServicesCallBack,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &AsfMsgExitBootServices
                  );

  DEBUG ((DEBUG_INFO, "AsfMsg Driver Exit!!\n"));
  return Status;
}
