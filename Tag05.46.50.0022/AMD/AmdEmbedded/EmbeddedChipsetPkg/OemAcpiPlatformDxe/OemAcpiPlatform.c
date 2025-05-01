/** @file

;*******************************************************************************
;* Copyright (c) 2013 - 2017, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#include "OemAcpiPlatform.h"

#include <ChipsetSetupConfig.h>

#include <IndustryStandard/Acpi.h>

#include <Protocol/AcpiSupport.h>

#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/KernelConfigLib.h>

EFI_ACPI_SUPPORT_PROTOCOL               *mAcpiSupport = NULL;

/**

  OEM ACPI Platform driver installation function.

  @param     ImageHandle     EFI_HANDLE
  @param     SystemTable     EFI_SYSTEM_TABLE pointer

  @retval    EFI_SUCCESS    The driver installed without error.
             EFI_ABORTED    The driver encountered an error and could not complete installation of
                            the ACPI tables.

*/
EFI_STATUS
EFIAPI
InstallOemAcpiPlatform (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                            Status;
  EFI_ACPI_TABLE_VERSION                Version;
  KERNEL_CONFIGURATION                  KernelSetup;

  //
  // Locate ACPI support protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiSupportProtocolGuid, NULL, (VOID **)&mAcpiSupport);
  ASSERT_EFI_ERROR (Status);

  //
  // Get SYSTEM_CONFIGURATION
  //
  Status = GetKernelConfiguration (&KernelSetup);
  if (EFI_ERROR (Status)) {
    return Status;
  }
	
  Status = UpdateOemDsdt ();
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = UpdateOemTableID ();
  if (EFI_ERROR(Status)) {
    return Status;
  }
  
  //
  // By default, a table belongs in all ACPI table versions published.
  //
  Version = EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0;
  if (KernelSetup.AcpiVer == 2) {
    Version |= EFI_ACPI_TABLE_VERSION_4_0;
  }
	
  return mAcpiSupport->PublishTables (mAcpiSupport, Version);
}

