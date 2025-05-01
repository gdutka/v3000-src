/** @file
  PEIM for Storage Status Code Handler.

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

#ifndef _STORAGE_STATUS_CODE_PEI_H_
#define _STORAGE_STATUS_CODE_PEI_H_

#include <Uefi.h>
#include <Ppi/ReportStatusCodeHandler.h>
#include <Ppi/MemoryDiscovered.h>
#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>
#include <Guid/H2OStatusCodeStorage.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>

#define STORAGE_STATUS_CODE_MAX_DEBUG_MESSAGE_LENGTH      0x200

EFI_STATUS
EFIAPI
RscHandlerPpiNotifyCallback (
  IN  EFI_PEI_SERVICES                  **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR         *NotifyDescriptor,
  IN  VOID                              *Ppi
  );

EFI_STATUS
EFIAPI
RamInitialize (
  IN  EFI_PEI_SERVICES                  **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR         *NotifyDescriptor,
  IN  VOID                              *Ppi
  );

#endif
