/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_PROM21_SMM_H_
#define _FCH_PROM21_SMM_H_

EFI_STATUS
EFIAPI
FchProm21SmmEntryPoint (
  IN        EFI_HANDLE                          ImageHandle,
  IN        EFI_SYSTEM_TABLE                    *SystemTable
  );

EFI_STATUS
PT21SmmRegisterSwSmi (
  VOID
  );

EFI_STATUS
PT21SmmRegisterSxSmi (
  VOID
  );

EFI_STATUS
PT21SmmRegisterPowerButtonSmi (
  VOID
  );

EFI_STATUS
PT21SmmRegisterCf9ResetSmi (
  VOID
  );

EFI_STATUS
PT21SmiBTSwPatch (
  VOID
  );

#endif
