/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _FCH_KEITHSSDT_H_
#define _FCH_KEITHSSDT_H_

#include <IndustryStandard/Acpi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/FchDxeLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/FchInitProtocol.h>
#include "FchPlatform.h"


#define AMD_FCH_KEITH_SSDT_GUID \
  {0x8F5D27AB, 0x81F6, 0x4668, 0x94, 0xFF, 0x89, 0x4C, 0x52, 0xE0, 0x5, 0xEF}

#define NAME_SIZE               0x04
#define OPCODE_SIZE             0x01
#define PREFIX_SIZE             0x01
#define MAX_DATA_BUFFER_SIZE    0x04

#define USB4_ASL_SUPPORT        0

typedef struct{
  UINT64        TableId;
  BOOLEAN       Enable;
}OEM_TABLE_ID;

VOID
EFIAPI
FchKeithSsdtInit (
  IN EFI_EVENT        Event,
  IN VOID             *Context
);

EFI_STATUS
EFIAPI
FchKeithSsdtInitEntry (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
);


#endif  // _FCH_KEITHSSDT_H_
