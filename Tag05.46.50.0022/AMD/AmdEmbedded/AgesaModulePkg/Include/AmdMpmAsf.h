/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_ASF_H_
#define _AMD_ASF_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Pi/PiStatusCode.h>
#include <Library/MpmLib.h>

//
// ASF Message Type
//
#define ASF_MSG_TYPE_PROGRESS               (UINT8)(EFI_PROGRESS_CODE & 0xFF)   //0x01
#define ASF_MSG_TYPE_ERROR                  (UINT8)(EFI_ERROR_CODE & 0xFF)      //0x02
#define ASF_MSG_TYPE_START_WATCHDOG         0x11u
#define ASF_MSG_TYPE_STOP_WATCHDOG          0x12u
#define ASF_MSG_TYPE_SYSTEM_STATE           0x13u

//AMD Customized Error Code
#define WLAN_FW_LOAD_FAILED                 0x1A0

typedef enum {
  MsgMemInitEntry,
  MsgMemIntiExit,
  MsgHddInitEntry,
  MsgHddInitExit,
  MsgSecondProcessorInitEntry,
  MsgSecondProcessorInitExit,
  MsgUserAuthEntry,
  MsgUserAuthExit,
  MsgUserInitiateSetupEntry,
  MsgUserInitiateSetupExit,
  MsgUsbResourceConfigEntry,
  MsgUsbResourceConfigExit,
  MsgPciResourceConfigEntry,
  MsgPciResourceConfigExit,
  MsgOpRomInitEntry,
  MsgOpRomInitExit,
  MsgVideoInitEntry,
  MsgVideoInitExit,
  MsgCacheInitEntry,
  MsgCacheInitExit,
  MsgSmBusInitEntry,
  MsgSmBusInitExit,
  MsgKbdInitEntry,
  MsgKbdInitExit,
  MsgEcInitEntry,
  MsgEcInitExit,
  MsgDockStationAttachEntry,
  MsgDockStationAttachExit,
  MsgDockEnableEntry,
  MsgDockEnableExit,
  MsgDockStationEjectionEntry,
  MsgDockStationEjectionExit,
  MsgDockDisableEntry,
  MsgDockDisableExit,
  MsgCallOSWakeVectorEntry,
  MsgCallOSWakeVectorExit,
  MsgStartOSBootEntry,
  MsgStartOSBootExit,
  MsgMotherBoardInitEntry,
  MsgMotherBoardInitExit,
  MsgFloppyInitEntry,
  MsgFloppyInitExit,
  MsgKbdTestEntry,
  MsgKbdTestExit,
  MsgPointDeviceTestEntry,
  MsgPointDeviceTestExit,
  MsgPrimaryProcessorInitEntry,
  MsgPrimaryProcessorInitExit,
  MsgBiosBootCompleteEntry,
  MsgBiosBootCompleteExit,
  MsgMemMissing,
  MsgMemFailure,
  MsgHddFailure,
  MsgSystemBoardFailure,
  MsgHddControllerFailure,
  MsgKbdFailure,
  MsgNoBootMedia,
  MsgVideoFailure,
  MsgNoVideo,
  MsgFirmwareRomCorruption,
  MsgCpuVidMismatch,
  MsgCpuSpeedMatchingFailure,
  MsgSystemStateS0,
  MsgSystemStateS3,
  MsgSystemStateS4,
  MsgSystemStateS5,
  MsgStartBiosWdt,
  MsgStartOsWdt,
  MsgStopWdt,
  MsgWlanFwLoadError = 1200,
} ASF_MSG_ID;

#pragma pack(1)
///
/// ASF Time Abstraction:
///  Year:       1900 - 9999
///  Month:      1 - 12
///  Day:        1 - 31
///  Hour:       0 - 23
///  Minute:     0 - 59
///  Second:     0 - 59
///
typedef struct {
  UINT16  Year;
  UINT8   Month;
  UINT8   Day;
  UINT8   Hour;
  UINT8   Minute;
  UINT8   Second;
  UINT8   Reserve;
} ASF_TIME;

typedef struct {
  ASF_MSG_ID        MsgId;
  UINT8             MsgType;
  union {
    UINT32                    WdtTimeoutValue;  // Available for ASF_MSG_TYPE_START_WATCHDOG
    EFI_STATUS_CODE_VALUE     StatusCode;       // Available for ASF_MSG_TYPE_PROGRESS and ASF_MSG_TYPE_ERROR
    UINT32                    SystemState;      // Available for ASF_MSG_TYPE_SYSTEM_STATE
  } MsgData;
  ASF_TIME                    AsfTime;
} ASF_MESSAGE;
#pragma pack ()

#endif
