/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Protocol/SmmBase2.h>
#include <Library/debugLib.h>

#include <Protocol/FchSmmMiscDispatch.h>
#include <Filecode.h>

#include "FchProm21Smm.h"

#define FILECODE FCH_PROM21_SMM_FCHPROM21SMMCF9RESETSMI_FILECODE

//extern UINT8                        XhciBtRemoteWakeup;

#define FCH_SMIx90_SmiStatus4       0x90
#define SmiStatus4_Cf9WriteEvent141 (1 << 13)

EFI_STATUS
EFIAPI
PT21SmiCf9ResetCallback (
  IN        EFI_HANDLE                          DispatchHandle,
  IN CONST  FCH_SMM_MISC_REGISTER_CONTEXT       *DispatchContext
  );

EFI_STATUS
PT21SmmRegisterCf9ResetSmi (
  VOID
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
//  FCH_SMM_MISC_DISPATCH_PROTOCOL        *PTPlusMiscDispatch;
//  FCH_SMM_MISC_REGISTER_CONTEXT         PTPlusMiscRegisterContext;
//  EFI_HANDLE                            PTPlusMiscHandle;

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-Start\n", __FUNCTION__));
 // DEBUG ((DEBUG_INFO, "PT-SMM-%a-XhciBtRemoteWakeup = %x\n", __FUNCTION__, XhciBtRemoteWakeup));
/*
  if (XhciBtRemoteWakeup == 0) {
    //
    // Locate SMM Misc Dispatch Protocol
    //
    DEBUG ((DEBUG_INFO, "PT-SMM-%a-SmmLocateProtocol-gFchSmmSxDispatch2ProtocolGuid\n", __FUNCTION__));
    Status = gSmst->SmmLocateProtocol (
                              &gFchSmmMiscDispatchProtocolGuid,
                              NULL,
                              &PTPlusMiscDispatch
                              );
    ASSERT_EFI_ERROR (Status);

    //
    // Register SMM CF9 Reset Callback
    //
    PTPlusMiscRegisterContext.SmiStatusReg = FCH_SMIx90_SmiStatus4;
    PTPlusMiscRegisterContext.SmiStatusBit = SmiStatus4_Cf9WriteEvent141;
    PTPlusMiscRegisterContext.Order        = 0x40;

    Status = PTPlusMiscDispatch->Register (
                              PTPlusMiscDispatch,
                              PTPlusSmiCf9ResetCallback,
                              &PTPlusMiscRegisterContext,
                              &PTPlusMiscHandle
                              );
    DEBUG ((DEBUG_INFO, "PT-SMM-%a-Register-PTPlusSmiCf9ResetCallback-Status = %r\n", __FUNCTION__, Status));
  } // End of if (XhciBtRemoteWakeup == 0)
*/
  DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

EFI_STATUS
EFIAPI
PT21SmiCf9ResetCallback (
  IN      EFI_HANDLE                              DispatchHandle,
  IN      CONST FCH_SMM_MISC_REGISTER_CONTEXT     *DispatchContext
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-Start\n", __FUNCTION__));

  Status = PT21SmiBTSwPatch ();

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

