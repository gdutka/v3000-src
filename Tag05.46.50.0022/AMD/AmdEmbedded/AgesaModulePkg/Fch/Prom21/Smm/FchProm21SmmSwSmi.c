/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Protocol/SmmBase2.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/debugLib.h>
#include <Protocol/FchSmmSwDispatch2.h>

#include <Filecode.h>

#include "FchProm21Smm.h"

#define FILECODE FCH_PROM21_SMM_FCHPROM21SMMSWSMI_FILECODE

EFI_STATUS
EFIAPI
PT21SmiS3RestoreCallback (
  IN        EFI_HANDLE                          DispatchHandle,
  IN        CONST FCH_SMM_SW_REGISTER_CONTEXT   *DispatchContext,
  IN OUT    FCH_SMM_SW_CONTEXT                  *SwContext,
  IN OUT    UINTN                               *SizeOfSwContext
  );

EFI_STATUS
PT21SmmRegisterSwSmi (
  VOID
  )
{
  EFI_STATUS                        Status;
  FCH_SMM_SW_DISPATCH2_PROTOCOL     *PT21SwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT       PT21SwRegisterContext;
  EFI_HANDLE                        PT21SwHandle;

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-Start\n", __FUNCTION__));
  //
  // Locate SMM SW Dispatch Protocol
  //
  DEBUG ((DEBUG_INFO, "PT-SMM-%a-SmmLocateProtocol-gFchSmmSwDispatch2ProtocolGuid\n", __FUNCTION__));
  Status = gSmst->SmmLocateProtocol (
                              &gFchSmmSwDispatch2ProtocolGuid,
                              NULL,
                              &PT21SwDispatch
                              );
  ASSERT_EFI_ERROR (Status);

  //
  // Register SMM S3 Restore Callback
  //
  PT21SwRegisterContext.AmdSwValue = PcdGet8 (PcdFchOemAfterPciRestoreSwSmi);
  PT21SwRegisterContext.Order      = 0x80;

  Status = PT21SwDispatch->Register (
                              PT21SwDispatch,
                              PT21SmiS3RestoreCallback,
                              &PT21SwRegisterContext,
                              &PT21SwHandle
                              );
  DEBUG ((DEBUG_INFO, "PT-SMM-%a-Register-PT21SmiS3RestoreCallback-Status = %r\n", __FUNCTION__, Status));

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

EFI_STATUS
EFIAPI
PT21SmiS3RestoreCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "PT-SMM-%a-Start\n", __FUNCTION__));

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

