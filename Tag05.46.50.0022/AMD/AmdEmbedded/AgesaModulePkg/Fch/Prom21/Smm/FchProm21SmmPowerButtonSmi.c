/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Protocol/SmmBase2.h>
#include <Library/debugLib.h>
#include <Filecode.h>
#include <Library/SmmServicesTableLib.h>

#include <Protocol/FchSmmPwrBtnDispatch2.h>

#include "FchProm21Smm.h"

#define FILECODE FCH_PROM21_SMM_FCHPROM21SMMPOWERBUTTONSMI_FILECODE

//extern UINT8                        XhciBtRemoteWakeup;

EFI_STATUS
EFIAPI
PT21SmiPowerButtonCallback (
  IN        EFI_HANDLE                          DispatchHandle,
  IN CONST  FCH_SMM_PWRBTN_REGISTER_CONTEXT     *DispatchContext,
  IN OUT    VOID                                *CommBuffer       OPTIONAL,
  IN OUT    UINTN                               *CommBufferSize   OPTIONAL
  );

EFI_STATUS
PT21SmmRegisterPowerButtonSmi (
  VOID
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-Start\n", __FUNCTION__));

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

EFI_STATUS
EFIAPI
PT21SmiPowerButtonCallback (
  IN      EFI_HANDLE                              DispatchHandle,
  IN      CONST FCH_SMM_PWRBTN_REGISTER_CONTEXT   *DispatchContext,
  IN OUT  VOID                                    *CommBuffer OPTIONAL,
  IN OUT  UINTN                                   *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-Start\n", __FUNCTION__));

  Status = PT21SmiBTSwPatch ();

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

