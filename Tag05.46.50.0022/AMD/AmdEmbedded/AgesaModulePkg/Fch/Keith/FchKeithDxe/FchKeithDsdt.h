/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _FCH_KEITHDSDT_H_
#define _FCH_KEITHDSDT_H_

#include <IndustryStandard/Acpi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/FchDxeLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/FchInitProtocol.h>

#define NAME_SIZE               0x04
#define PREFIX_SIZE             0x01
#define OPCODE_SIZE             0x01
#define MAX_DATA_BUFFER_SIZE    0x04

typedef struct{
  UINT32    Signature;
  UINTN     Size;
  UINT64    Data;
}ASL_NAME_OP_INFO;

EFI_STATUS
EFIAPI
FchKeithDsdtInitEntry (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
);


#endif

