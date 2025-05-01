/** @file
  DXE driver for Storage Status Code Handler.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _STORAGE_STATUS_CODE_DXE_H_
#define _STORAGE_STATUS_CODE_DXE_H_

#include <Uefi.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DevicePath.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>
#include <Guid/H2OStatusCodeStorage.h>
#include <Guid/EventGroup.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/H2OCpLib.h>
#include <Library/H2OLib.h>

#define STORAGE_STATUS_CODE_MAX_DEBUG_MESSAGE_LENGTH      0x200

#define STORAGE_STATUS_CODE_TIMER_DIRECT_WRITE            0
#define STORAGE_STATUS_CODE_TIMER_DISABLED                0xFFFFFFFF

#endif
