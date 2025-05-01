/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Protocol/SmmBase2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/debugLib.h>
#include <Protocol/FchProm21SmmInitProtocol.h>
#include <Protocol/FchProm21InitProtocol.h>


#include "FchProm21Smm.h"

#include <Filecode.h>

#define FILECODE FCH_PROM21_SMM_FCHPROM21SMM_FILECODE

//UINT8                         XhciBtRemoteWakeup;
UINT32                        GppPcieAddr;

/*********************************************************************************
 * Name: FchProm21SmmEntryPoint
 *
 * Description
 *   Entry point of the AMD FCH SMM driver
 *   copy PT_INIT_PROTOCOL to SMM, and register Fch Smm callbacks
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchProm21SmmEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;
  FCH_PROM21_SMM_INIT_PROTOCOL  *pFchProm21SmmInitProtocolGuid;
  PT21_INIT_PROTOCOL            *PT21InitProtocol;
  EFI_HANDLE                    PT21SmmInitHandle;

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-Start\n", __FUNCTION__));

 // XhciBtRemoteWakeup = PcdGet8 (PcdPTPlusXhciBtRemoteWakeup);
  GppPcieAddr = PcdGet32 (PcdPT21GppPcieAddress);
  DEBUG ((DEBUG_INFO, "PT-SMM-%a- GppPcieAddr = 0x%X\n", __FUNCTION__, GppPcieAddr));

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-SmmAllocatePool-pFchProm21SmmInitProtocolGuid-Size = 0x%X\n", __FUNCTION__, sizeof (FCH_PROM21_SMM_INIT_PROTOCOL)));
  Status = gSmst->SmmAllocatePool (
                              EfiRuntimeServicesData,
                              sizeof (FCH_PROM21_SMM_INIT_PROTOCOL),
                              &pFchProm21SmmInitProtocolGuid
                              );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-1-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-LocateProtocol-gAmdFchProm21InitProtocolGuid\n", __FUNCTION__));
  Status = gBS->LocateProtocol (
                              &gAmdFchProm21InitProtocolGuid,
                              NULL,
                              &PT21InitProtocol
                              );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-2-Status = EFI_UNSUPPORTED\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21SmmRegisterSwSmi\n", __FUNCTION__));
  Status = PT21SmmRegisterSwSmi ();
  DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21SmmRegisterSwSmi-Status = %r\n", __FUNCTION__, Status));

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21SmmRegisterSxSmi\n", __FUNCTION__));
  Status = PT21SmmRegisterSxSmi ();
  DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21SmmRegisterSxSmi-Status = %r\n", __FUNCTION__, Status));

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21SmmRegisterPowerButtonSmi\n", __FUNCTION__));
  Status = PT21SmmRegisterPowerButtonSmi ();
  DEBUG ((DEBUG_INFO, "PT-SMM-%a-PTPlusSmmRegisterPowerButtonSmi-Status = %r\n", __FUNCTION__, Status));

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21SmmRegisterCf9ResetSmi\n", __FUNCTION__));
  Status = PT21SmmRegisterCf9ResetSmi ();
  DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21SmmRegisterCf9ResetSmi-Status = %r\n", __FUNCTION__, Status));

  //
  // Install SMM protocol
  //
  pFchProm21SmmInitProtocolGuid->Revision        = PT21InitProtocol->Revision;
  PT21SmmInitHandle =  NULL;

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-SmmInstallProtocolInterface-gFchProm21SmmInitProtocolGuid\n", __FUNCTION__));
  Status = gSmst->SmmInstallProtocolInterface (
                              &PT21SmmInitHandle,
                              &gFchProm21SmmInitProtocolGuid,
                              EFI_NATIVE_INTERFACE,
                              pFchProm21SmmInitProtocolGuid
                              );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-3-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-4-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

