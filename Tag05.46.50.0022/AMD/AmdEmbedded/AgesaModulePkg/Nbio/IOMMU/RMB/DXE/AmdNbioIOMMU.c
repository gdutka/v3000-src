/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AmdNbioIommu Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioPcieZPDxe
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <PiDxe.h>
#include <Filecode.h>
#include <Guid/GnbPcieInfoHob.h>

#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/AmdIOMMUDmarLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/NbioIommuIvrsLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AmdNbioPcieServicesProtocol.h>
#include <Protocol/AmdNbioIommuProtocol.h>
#include <Protocol/SocLogicalIdProtocol.h>
#include <IdsHookId.h>
#include <SocLogicalId.h>

#define FILECODE        NBIO_IOMMU_RMB_DXE_AMDNBIOIOMMU_FILECODE

// Tag protocol
AMD_NBIO_IOMMU_PROTOCOL mAmdNbioIommuProtocol = {
  0
};

/**
 *---------------------------------------------------------------------------------------
 *  InstallIvrsAcpiTable
 *
 *  Description:
 *     notification event handler for install Ivrs Acpi Table
 *  Parameters:
 *    @param[in]     Event      Event whose notification function is being invoked.
 *    @param[in]     *Context   Pointer to the notification function's context.
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
EFIAPI
InstallIvrsAcpiTable (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_ACPI_TABLE_PROTOCOL             *AcpiTable;
  EFI_STATUS                          Status;
  UINT64                              EfrAndMask;
  UINT64                              EfrOrMask;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Start\n", __FUNCTION__);

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Create ACPI IVRS SSDT table
  //
  EfrAndMask = 0xFFFFFFFFFFFFFFFF;
  EfrOrMask = 0;

  // Create IVRS SSDT Table
  if (PcdGetBool (PcdCfgIommuSupport) && PcdGetBool (PcdIvrsControl)) {
    GnbIommuIvrsTable (EfrAndMask, EfrOrMask);
  }

  //
  // Close event, so it will not be invoked again.
  //
  gBS->CloseEvent (Event);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a End\n", __FUNCTION__);
}

/**
 *---------------------------------------------------------------------------------------
 *  AcpiTableProtocolCallback
 *
 *  Description:
 *     notification event handler
 *  Parameters:
 *    @param[in]     Event      Event whose notification function is being invoked.
 *    @param[in]     *Context   Pointer to the notification function's context.
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
EFIAPI
AcpiTableProtocolCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_ACPI_TABLE_PROTOCOL             *AcpiTable;
  EFI_STATUS                          Status;
  GNB_HANDLE                          *GnbHandle;
  GNB_PCIE_INFORMATION_DATA_HOB       *PciePlatformConfigHobData;
  DXE_AMD_NBIO_PCIE_SERVICES_PROTOCOL *PcieServicesProtocol;
  PCIe_PLATFORM_CONFIG                *Pcie;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Start\n", __FUNCTION__);

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Get topology structure
  Status = gBS->LocateProtocol (
                  &gAmdNbioPcieServicesProtocolGuid,
                  NULL,
                  &PcieServicesProtocol
                  );

  if (Status == EFI_SUCCESS) {
    PcieServicesProtocol->PcieGetTopology (PcieServicesProtocol, (UINT32 **) &PciePlatformConfigHobData);
    Pcie = &(PciePlatformConfigHobData->PciePlatformConfigHob);
    GnbHandle = NbioGetHandle (Pcie);

    NbioAssignIoapicIds (GnbHandle);
  }
  //
  // Close event, so it will not be invoked again.
  //
  gBS->CloseEvent (Event);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a End\n", __FUNCTION__);
}

/**
  Entry point for DXE IOMMU Initialization


  @param ImageHandle UEFI Image handle
  @param SystemTable Pointer to system table

  @return EFI_STATUS
**/
EFI_STATUS
EFIAPI
AmdNbioIOMMUDxeEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  DXE_AMD_NBIO_PCIE_SERVICES_PROTOCOL *PcieServicesProtocol;
  PCIe_PLATFORM_CONFIG                *Pcie;
  EFI_STATUS                          Status;
  EFI_STATUS                          AgesaStatus;
  GNB_HANDLE                          *GnbHandle;
  GNB_PCIE_INFORMATION_DATA_HOB       *PciePlatformConfigHobData;
  EFI_HANDLE                          NbioIommuHandle;
  EFI_EVENT                           Event;
  VOID                                *Registration;

  AGESA_TESTPOINT (TpNbioIommuDxeEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  AgesaStatus = EFI_SUCCESS;
  GnbHandle = NULL;
  PciePlatformConfigHobData = NULL;

  NbioIommuHandle = NULL;

  // Get topology structure
  Status = gBS->LocateProtocol (
                  &gAmdNbioPcieServicesProtocolGuid,
                  NULL,
                  &PcieServicesProtocol
                  );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  if (Status == EFI_SUCCESS) {
    PcieServicesProtocol->PcieGetTopology (PcieServicesProtocol, (UINT32 **) &PciePlatformConfigHobData);
    Pcie = &(PciePlatformConfigHobData->PciePlatformConfigHob);
    GnbHandle = NbioGetHandle (Pcie);

    EfiCreateProtocolNotifyEvent (&gEfiAcpiTableProtocolGuid, TPL_CALLBACK, AcpiTableProtocolCallback, NULL, &Registration);

    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    InstallIvrsAcpiTable,
                    NULL,
                    &gEfiEventReadyToBootGuid,
                    &Event
                    );

    // If DMAr Mitigation is enabled, turn on IOMMU and setup translations
    if (PcdGetBool(PcdIvInfoDmaReMap)) {
      GnbHandle = NbioGetHandle (Pcie);
      SetupAmdIommuDmar (GnbHandle);
    }

    IDS_HOOK (IDS_HOOK_NBIO_AFTER_CCX, NULL, (void *)NbioGetHandle (Pcie));

    // Install tag protocol for other driver dependencies
    Status = gBS->InstallProtocolInterface (
              &NbioIommuHandle,
              &gAmdNbioIommuProtocolGuid,
              EFI_NATIVE_INTERFACE,
              &mAmdNbioIommuProtocol
              );
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "publish AMD_NBIO_IOMMU_PROTOCOL error(%x)\n", Status);
    }
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  AGESA_TESTPOINT (TpNbioIommuDxeExit, NULL);

  return AgesaStatus;
}


