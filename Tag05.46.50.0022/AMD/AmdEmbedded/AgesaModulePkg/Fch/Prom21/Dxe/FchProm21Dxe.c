/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Filecode.h>
#include <Protocol/FchProm21InitProtocol.h>

#define FILECODE FCH_PROM21_DXE_FCHPROM21DXE_FILECODE

/*********************************************************************************
 * Name: FchDxeInit
 *
 * Description
 *   Entry point of the AMD FCH PROMONTORY DXE driver
 *   Perform the configuration init, resource reservation, early post init
 *   and install all the supported protocol
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
FchProm21DxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  PT_21_DXE_PRIVATE    *FchPrivate;
  EFI_STATUS             Status;
  EFI_HANDLE             Handle;

  DEBUG ((DEBUG_INFO, "PT-DXE-%a-Start\n", __FUNCTION__));

  if (PcdGet32 (PcdPT21XhciID) == 0) {
    DEBUG ((DEBUG_INFO, "PT-DXE-%a(%d)-End-ERROR: Promontory is NOT FOUND!\n", __FUNCTION__, __LINE__));
    return EFI_UNSUPPORTED;
  }

  // Allocate memory for the private data
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  sizeof (PT_21_DXE_PRIVATE),
                  &FchPrivate
                  );
  ASSERT_EFI_ERROR (Status);

  // Initialize the private data structure
  FchPrivate->Signature = PT_21_DXE_PRIVATE_DATA_SIGNATURE;

  // Initialize the PtInit protocol
  FchPrivate->PtInit.Revision = PT21_INIT_REV;

  // Publish the PtInit protocol
  DEBUG ((EFI_D_INFO, "  Install gAmdFchProm21InitProtocolGuid !\n"));
  Handle = ImageHandle;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdFchProm21InitProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &FchPrivate->PtInit
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PT-DXE-%a(%d)-End-ERROR: Install gAmdFchProm21InitProtocolGuid FAIL !\n", __FUNCTION__, __LINE__));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "PT-DXE-%a-End\n", __FUNCTION__));
  return Status;
}

