/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Fabric - Rembrandt API, and related functions.
 *
 * Contains code that initializes the data fabric
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Fabric
 * @e \$Revision$   @e \$Date$
 *
 */

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Guid/EventGroup.h>
#include <Filecode.h>
#include <Library/DxeFabricTopologyServices2Lib.h>
#include <Library/DxeFabricResourceManagerServicesLib.h>
#include <Protocol/SocLogicalIdProtocol.h>
#include <Protocol/FabricNumaServicesProtocol.h>
#include "FabricPiePwrMgmt.h"
#include "FabricAcpiTable.h"
#include "FabricReadyToBoot.h"
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IdsLib.h>
#include <Library/BaseFabricTopologyLib.h>

#define FILECODE FABRIC_RMB_FABRICRMBDXE_AMDFABRICRMBDXE_FILECODE
/*++
Module Name:

  AmdFabricRmbDxe.c
  Init Data Fabric interface

Abstract:
--*/

VOID
EFIAPI
FabricNbioSmuServicesCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  );


EFI_EVENT            FabricNbioSmuServicesEvent;
VOID                 *mRegistrationForFabricNbioSmuServicesEvent;
extern BOOLEAN       mIsMixedDimmConfig;

/*++

Routine Description:

  Rembrandt data fabric driver entry.  Initialize the data fabric.

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
EFIAPI
AmdFabricRmbDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_STATUS                      CalledStatus;
  EFI_EVENT                       ReadyToBootEvent;
  FABRIC_NUMA_SERVICES2_PROTOCOL *FabricNuma;
  UINT32                          NumaDomainCount;

  AGESA_TESTPOINT (TpDfDxeEntry, NULL);

  Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdFabricRmbDxeInit Entry\n");

  // Publish DXE topology services
  CalledStatus = FabricTopologyService2ProtocolInstall (ImageHandle, SystemTable);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  // Publish DF Resource Manager services
  CalledStatus = FabricResourceManagerServiceProtocolInstall (ImageHandle, SystemTable);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  // Publish NUMA services 2 protocol
  CalledStatus = FabricRmbNumaServices2ProtocolInstall (ImageHandle, SystemTable);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  if (gBS->LocateProtocol (&gAmdFabricNumaServices2ProtocolGuid, NULL, (VOID **) &FabricNuma) == EFI_SUCCESS) {
    if (FabricNuma->GetDomainInfo (FabricNuma, &NumaDomainCount, NULL, NULL) == EFI_SUCCESS) {
      if ((NumaDomainCount > 1) || mIsMixedDimmConfig) {
        // Publish ACPI Fabric SRAT services protocol
        CalledStatus = FabricRmbAcpiSratProtocolInstall (ImageHandle, SystemTable);
        Status = (CalledStatus > Status) ? CalledStatus : Status;

        // Publish ACPI Fabric SRAT services V2 protocol to support GPU domains
        CalledStatus = FabricRmbAcpiSratV2ProtocolInstall (ImageHandle, SystemTable);
        Status = (CalledStatus > Status) ? CalledStatus : Status;

        // Publish ACPI Fabric MSCT services protocol
        CalledStatus = FabricRmbAcpiMsctProtocolInstall (ImageHandle, SystemTable);
        Status = (CalledStatus > Status) ? CalledStatus : Status;

        // Publish ACPI SLIT services protocol
        CalledStatus = FabricRmbAcpiSlitProtocolInstall (ImageHandle, SystemTable);
        Status = (CalledStatus > Status) ? CalledStatus : Status;
      }
    }

    // Publish ACPI Fabric CRAT services protocol
    CalledStatus = FabricRmbAcpiCratProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    // Publish ACPI CDIT services
    CalledStatus = FabricRmbAcpiCditProtocolInstall (ImageHandle, SystemTable);
    Status = (CalledStatus > Status) ? CalledStatus : Status;
  }

  //
  // Set up call back after NBIO SMU services are available.
  //
//  CalledStatus = gBS->CreateEventEx (
//                           EVT_NOTIFY_SIGNAL,
//                           TPL_NOTIFY,
//                           FabricNbioSmuServicesCallback,
//                           NULL,
//                           NULL,
//                           &FabricNbioSmuServicesEvent
//                           );
//  ASSERT (CalledStatus == EFI_SUCCESS);
//  Status = (CalledStatus > Status) ? CalledStatus : Status;

//  CalledStatus = gBS->RegisterProtocolNotify (
//                          &gAmdNbioSmuServicesProtocolGuid,
//                          FabricNbioSmuServicesEvent,
//                          &(mRegistrationForFabricNbioSmuServicesEvent)
//                          );
//  ASSERT (CalledStatus == EFI_SUCCESS);
//  Status = (CalledStatus > Status) ? CalledStatus : Status;

  IDS_HDT_CONSOLE (CPU_TRACE, "    PIE power management\n");
  FabricPiePwrMgmtInit ();

  CalledStatus = gBS->CreateEventEx (
                      EVT_NOTIFY_SIGNAL,
                      TPL_NOTIFY,
                      FabricReadyToBoot,
                      NULL,
                      &gEfiEventReadyToBootGuid,
                      &ReadyToBootEvent
                      );
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdFabricRmbDxeInit End\n");

  AGESA_TESTPOINT (TpDfDxeExit, NULL);

  return (Status);
}

//VOID
//EFIAPI
//FabricNbioSmuServicesCallback (
//  IN EFI_EVENT        Event,
//  IN VOID             *Context
//  )
//{
//  EFI_STATUS                     CalledStatus;
//  AMD_SOC_LOGICAL_ID_PROTOCOL    *SocLogicalIdProtocol;

//  AGESA_TESTPOINT (TpDfNbioSmuServicesProtocolCallbackEntry, NULL);
//  IDS_HDT_CONSOLE (CPU_TRACE, "  FabricNbioSmuServicesCallback Entry\n");

//  CalledStatus = gBS->LocateProtocol (
//                      &gAmdSocLogicalIdProtocolGuid,
//                      NULL,
//                      &SocLogicalIdProtocol
//                      );

//  ASSERT (CalledStatus == EFI_SUCCESS);

//  IDS_HDT_CONSOLE (CPU_TRACE, "    PIE power management\n");
//  FabricPiePwrMgmtInit (SocLogicalIdProtocol);

//  IDS_HDT_CONSOLE (CPU_TRACE, "  FabricNbioSmuServicesCallback Exit\n");
//  AGESA_TESTPOINT (TpDfNbioSmuServicesProtocolCallbackExit, NULL);
//}

