/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_MPM_GET_BATTERY_INFO_PROTOCOL_H_
#define _AMD_MPM_GET_BATTERY_INFO_PROTOCOL_H_

#include <Library/MpmLib.h>

//
// Extern the GUID for protocol users
//
extern EFI_GUID gAmdMpmGetBatteryInfoProtocol;

typedef struct _AMD_MPM_GET_BATTERY_INFO_PROTOCOL AMD_MPM_GET_BATTERY_INFO_PROTOCOL;

/**
  Get battery information

  @param[out] BatteryInfo

  @retval EFI_SUCCESS      Successfully get the battery info
**/
typedef
EFI_STATUS
(EFIAPI *GET_BATTERY_INFO) (
  OUT BATTERY_INFO_T  *BatteryInfo
);

//
// Protocol Interface Structure
//
typedef struct _AMD_MPM_GET_BATTERY_INFO_PROTOCOL
{
  GET_BATTERY_INFO      GetBatteryInfo;
} AMD_MPM_GET_BATTERY_INFO_PROTOCOL;

#endif
