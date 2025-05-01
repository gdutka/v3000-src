/** @file
  SMM driver for Status Code Handler.

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

#ifndef _STORAGE_STATUS_CODE_SMM_H_
#define _STORAGE_STATUS_CODE_SMM_H_

#include <Uefi.h>
#include <Protocol/SmmReportStatusCodeHandler.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>
#include <Guid/H2OStatusCodeStorage.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>

#define STORAGE_STATUS_CODE_MAX_DEBUG_MESSAGE_LENGTH      0x200

#endif
