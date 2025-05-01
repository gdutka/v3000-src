/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _H_SMBIOS_TYPE_ADD
#define _H_SMBIOS_TYPE_ADD

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CommonSetupLib.h>

#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>

typedef struct {
  EFI_SMBIOS_TABLE_HEADER Hdr;
  UINT16                  SensorHandle;
  UINT16                  Flags;
  UINT8                   PldmTid;
  UINT8                   PldmSensorId;
  UINT8                   PldmMcSlaveAddress;
  UINT8                   LegacySensorAddress;
  UINT8                   LegacySensorRegister;
  UINT8                   SensorReadingModifier;
}SMBIOS_TABLE_TYPE255;

#endif // _H_SMBIOS_TYPE_ADD
