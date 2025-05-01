/** @file
 DXE Chipset Services driver.

 It produces an instance of the DXE Chipset Services protocol to provide the chipset related functions
 which will be used by Kernel or Project code. These protocol functions should be used by calling the
 corresponding functions in DxeChipsetSvcLib to have the protocol size checking

***************************************************************************
* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/H2ODxeChipsetServices.h>

H2O_CHIPSET_SERVICES_PROTOCOL *mDxeChipsetSvc = NULL;

/**
 This is the declaration of DXE Chipset Services driver entry point.

 @param[in]         ImageHandle         The firmware allocated handle for the UEFI image.
 @param[in]         SystemTable         A pointer to the EFI System Table.

 @retval            EFI_SUCCESS         The operation completed successfully.
 @retval            !EFI_SUCCESS        Error to installing Chipset Services Protocol.
*/
EFI_STATUS
EFIAPI
ChipsetSvcDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS           Status;

  //
  // Create an instance of the H2O Chipset Services protocol.
  // Then install it on the image handle.
  //
  mDxeChipsetSvc = AllocateZeroPool (sizeof (H2O_CHIPSET_SERVICES_PROTOCOL));
  if (mDxeChipsetSvc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  mDxeChipsetSvc->Size = sizeof (H2O_CHIPSET_SERVICES_PROTOCOL);

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gH2OChipsetServicesProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  mDxeChipsetSvc
                  );
  if (EFI_ERROR (Status)) {
    FreePool (mDxeChipsetSvc);
    return Status;
  }

  return Status;
}

