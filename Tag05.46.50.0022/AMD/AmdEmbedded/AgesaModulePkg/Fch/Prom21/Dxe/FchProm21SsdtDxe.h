/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_PROM21_SSDT_DXE_H_
#define _FCH_PROM21_SSDT_DXE_H_

VOID
EFIAPI
FchProm21SsdtDxeInit (
  IN EFI_EVENT        Event,
  IN VOID             *Context
);

EFI_STATUS
EFIAPI
FchProm21SsdtDxeInitEntry (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
);

#endif

