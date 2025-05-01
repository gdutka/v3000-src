/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _MPM_ASF_DXE_H_
#define _MPM_ASF_DXE_H_

#include <Protocol/ReportStatusCodeHandler.h>

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MpmLib.h>
#include <Library/IdsLib.h>
#include <Guid/GlobalVariable.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Pi/PiStatusCode.h>

#include "../MpmAsf.h"


EFI_STATUS
AsfPushMessage (
  IN ASF_MSG_ID  MsgId
);

#endif
