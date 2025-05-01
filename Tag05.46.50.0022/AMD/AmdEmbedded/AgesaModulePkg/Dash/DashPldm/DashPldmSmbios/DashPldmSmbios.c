/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <Uefi/UefiSpec.h>
#include <Library/UefiLib.h>
#include <IndustryStandard/SmBios.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AmdDashPldmProtocol.h>
#include <Library/IdsLib.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Library/ApcbLibV3.h>
#include <Addendum/Apcb/Inc/RMB/ApcbV3TokenUid.h>
#include <Addendum/Apcb/Inc/RMB/ApcbV3Priority.h>
#include <Library/CommonSetupLib.h>


VOID
PldmSmbiosReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                Status;
  AMD_DASH_PLDM_PROTOCOL    *PldmProtocol;
  UINT32                    ResponseDataSize;
  SMBIOS_TABLE_ENTRY_POINT  *SmbiosEntry;
  UINT32                    Crc32 = 0;
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT ResponseData;

  DEBUG ((DEBUG_INFO, "%a, Enter\n", __FUNCTION__));
  Status = gBS->LocateProtocol (
                  &gAmdDashPldmProtocolGuid,
                  NULL,
                  (VOID **) &PldmProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate AmdPldmProtocol failed, Status = %r\n", Status));
    return;
  }

  Status = EfiGetSystemConfigurationTable (
             &gEfiSmbiosTableGuid,
             (VOID **)&SmbiosEntry
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Smbios table failed, Status = %r\n", Status));
    return;
  }

  gBS->CalculateCrc32 ((VOID *)(UINTN) SmbiosEntry->TableAddress, SmbiosEntry->TableLength, &Crc32);

  //
  // Send Pldm Smbios command
  //
  // DSP0246, 9.2 SMBIOS table transfer from BIOS to Dash FW
  //   1. The BIOS first queries the SMBIOS table metadata by using the GetSMBIOSStructureTableMetadata command.
  //      Get the response data
  //   2. BIOS transfers the SMBIOS structure table to the MC by using the SetSMBIOSStructureTable command
  //   3. After transferring the latest SMBIOS structure table, the BIOS sets up the SMBIOS structure table metadata
  //     on the MC by using the SetSMBIOSStructureTableMetadata command.
  ResponseDataSize = sizeof(ResponseData);
  Status = PldmProtocol->DashGetSmbiosStructureTableMetaData (PldmProtocol, &ResponseData, &ResponseDataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Smbios Structure table failed, Status = %r\n", Status));
    return;
  }

  if ((ResponseData.CompletionCode != DASH_PLDM_COMPLETION_CODE_SUCCESS) || (ResponseData.SmbiosStructureTableIntegrityChecksum != Crc32)) {
    Status = PldmProtocol->DashSetSmbiosStructureTable (PldmProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set Smbios Structure table failed, Status = %r\n", Status));
      return;
    }
    Status = PldmProtocol->DashSetSmbiosStructureTableMetaData (PldmProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set Smbios Structure table MetaData failed, Status = %r\n", Status));
      return;
    }
  }

  gBS->CloseEvent (Event);
}

EFI_STATUS
EFIAPI
DashPldmSmbiosEntry (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS            Status;
  EFI_EVENT             Event;
  COMMON_SETUP_OPTIONS  CommonSetupOptions;

  DEBUG ((DEBUG_INFO, "%a, Enter\n", __FUNCTION__));

  Status = GetCommonSetupOptions (&CommonSetupOptions);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[DashPldmSmbiosEntry] Get Dash variable fail.\n"));
    return Status;
  }

  if (CommonSetupOptions.DashOption != 1) {
    DEBUG ((DEBUG_INFO, "[DashPldmSmbiosEntry] Dash variable is Disabled.\n"));
    return Status;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  PldmSmbiosReadyToBoot,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &Event
                  );

  DEBUG ((DEBUG_INFO, "%a, Exit %r\n", __FUNCTION__, Status));
  return Status;
}
