/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <Uefi/UefiSpec.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <IndustryStandard/SmBios.h>
#include <Guid/SmBios.h>
#include <Library/MpmLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AmdPldmProtocol.h>
#include <Library/IdsLib.h>
#include <Filecode.h>
#include <MpmPldmSmbios.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Library/ApcbLibV3.h>
#include <Addendum/Apcb/Inc/RMB/ApcbV3TokenUid.h>
#include <Addendum/Apcb/Inc/RMB/ApcbV3Priority.h>

#define FILECODE MPM_MPMPLDM_MPMPLDMSMBIOS_MPMPLDMSMBIOS_FILECODE
#define SYSTEM_IS_PROVISIONED  5

/**
 * @brief Publish SMBIOS type A1 and A2
 * A1: This consists of information with which the AIM-T solution is configured to function,
 *     It consists of capability options and firmware versions, such as whether AIM-T is enabled at OEM factory or not,
 *     AIM-T version, options of AIM-T enabled by core BIOS, provisioned state, supported DASH specification version, and so forth.
 * A2: This consists of information with which the AIM-T solution is provided to the function.
 *     These details are provided by Enterprise IT Administrators and consist of organization details, authentication & access details,
 *     network details, signing & encryption certificates, enabled DASH profiles, and so forth
 * @return EFI_STATUS
 */
EFI_STATUS
PublishSmbiosTypeA1A2 ()
{
  EFI_STATUS Status;
  UINT32 StructureSize;
  PLDM_GET_SMBIOS_BY_TYPE_A1 *SmbiosTableTypeA1 = NULL;
  PLDM_GET_SMBIOS_BY_TYPE_A2 *SmbiosTableTypeA2 = NULL;
  EFI_SMBIOS_PROTOCOL *Smbios;
  AMD_PLDM_PROTOCOL   *PldmProtocol;
  UINT16              ReservedTableSize = 0x200;
  AMD_APCB_SERVICE_PROTOCOL       *ApcbDxeServiceProtocol;

  IDS_HDT_CONSOLE_PSP_TRACE ("Enter PublishSmbiosTypeA1A2 >>>\n");
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PublishSmbiosTypeA1A2, Locate SmbiosProtocol Status = %r\n", Status));
    return Status;
  }

  Status = gBS->LocateProtocol (
                  &gAmdPldmProtocolGuid,
                  NULL,
                  (VOID **) &PldmProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate AmdPldmProtocol failed, Status = %r\n", Status));
    return Status;
  }

  if (PcdGetBool (PcdMpmCreateSmBiosTypeA1)) {
    StructureSize = sizeof (PLDM_GET_SMBIOS_BY_TYPE_A1) + ReservedTableSize;
    SmbiosTableTypeA1 = AllocateZeroPool (StructureSize);
    if (SmbiosTableTypeA1 == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Status = PldmProtocol->GetSmbiosStructureByType (PldmProtocol, (PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT*)SmbiosTableTypeA1, &StructureSize, 0xA1, 0xFF00);
    if (!EFI_ERROR (Status)) {
      SmbiosTableTypeA1->SmbiosStructureA1.Hdr.Type = 0xA1;
      SmbiosTableTypeA1->SmbiosStructureA1.Hdr.Handle = SMBIOS_HANDLE_PI_RESERVED;
      IDS_HDT_CONSOLE_PSP_TRACE ("Get SMBIOS Type 0xA1 and update AIM-T Oem Status and Bios Options:\n");
      IdsDumpBufferByte (PSP_TRACE, (VOID *) (UINTN)SmbiosTableTypeA1, StructureSize);
      IDS_HDT_CONSOLE_PSP_TRACE ("SmbiosTableTypeA1->SmbiosStructureA1.AimtState = %d\n", SmbiosTableTypeA1->SmbiosStructureA1.AimtState.Field.AimtState);
      if (SmbiosTableTypeA1->SmbiosStructureA1.AimtState.Field.AimtState == SYSTEM_IS_PROVISIONED) {
         PcdSetBoolS (PcdMpmProvisioned, TRUE);
         Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &ApcbDxeServiceProtocol);
         if (!EFI_ERROR (Status)) {
           ApcbDxeServiceProtocol->ApcbSetTokenBool (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_ADMIN, APCB_TOKEN_UID_MPM_SYSTEM_PROVISIONED, 1);
           ApcbDxeServiceProtocol->ApcbFlushData (ApcbDxeServiceProtocol);
         }
      }

      Status = Smbios->Add (Smbios, NULL, &SmbiosTableTypeA1->SmbiosStructureA1.Hdr.Handle, (EFI_SMBIOS_TABLE_HEADER *) &(SmbiosTableTypeA1->SmbiosStructureA1));
      DEBUG ((DEBUG_ERROR, "\nPublish Smbios table A1, Status = %r\n", Status));
    }
  }

  if (PcdGetBool (PcdMpmCreateSmBiosTypeA2)) {
    StructureSize = sizeof (PLDM_GET_SMBIOS_BY_TYPE_A2) + ReservedTableSize;
    SmbiosTableTypeA2 = AllocateZeroPool (StructureSize);
    if (SmbiosTableTypeA2 == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Status = PldmProtocol->GetSmbiosStructureByType (PldmProtocol, (PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT*)SmbiosTableTypeA2, &StructureSize, 0xA2, 0xFF01);
    if (!EFI_ERROR (Status)) {
      SmbiosTableTypeA2->SmbiosStructureA2.Hdr.Type = 0xA2;
      SmbiosTableTypeA2->SmbiosStructureA2.Hdr.Handle = SMBIOS_HANDLE_PI_RESERVED;
      IDS_HDT_CONSOLE_PSP_TRACE ("Get SMBIOS Type 0xA2:\n");
      IdsDumpBufferByte (PSP_TRACE, (VOID *) (UINTN)SmbiosTableTypeA2, StructureSize);
      Status = Smbios->Add (Smbios, NULL, &SmbiosTableTypeA2->SmbiosStructureA2.Hdr.Handle, (EFI_SMBIOS_TABLE_HEADER *) &(SmbiosTableTypeA2->SmbiosStructureA2));
      DEBUG ((DEBUG_ERROR, "\nPublish Smbios table A2, Status = %r\n", Status));
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Exit PublishSmbiosTypeA1A2 <<<\n");
  return Status;
}

VOID
PldmSmbiosEndOfDxe (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                Status;
  AMD_PLDM_PROTOCOL         *PldmProtocol;
  UINT32                    ResponseDataSize;
  SMBIOS_TABLE_ENTRY_POINT  *SmbiosEntry;
  UINT32                    Crc32 = 0;
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT ResponseData;

  DEBUG ((DEBUG_INFO, "%a, Enter\n", __FUNCTION__));
  Status = gBS->LocateProtocol (
                  &gAmdPldmProtocolGuid,
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
  // DSP0246, 9.2 SMBIOS table transfer from BIOS to MPM
  //   1. The BIOS first queries the SMBIOS table metadata by using the GetSMBIOSStructureTableMetadata command.
  //      Get the response data
  //   2. BIOS transfers the SMBIOS structure table to the MC by using the SetSMBIOSStructureTable command
  //   3. After transferring the latest SMBIOS structure table, the BIOS sets up the SMBIOS structure table metadata
  //     on the MC by using the SetSMBIOSStructureTableMetadata command.
  ResponseDataSize = sizeof(ResponseData);
  Status = PldmProtocol->GetSmbiosStructureTableMetaData (PldmProtocol, &ResponseData, &ResponseDataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Smbios Structure table failed, Status = %r\n", Status));
    return;
  }

  if ((ResponseData.CompletionCode != PLDM_COMPLETION_CODE_SUCCESS) || (ResponseData.SmbiosStructureTableIntegrityChecksum != Crc32)) {
    Status = PldmProtocol->SetSmbiosStructureTable (PldmProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set Smbios Structure table failed, Status = %r\n", Status));
      return;
    }
    Status = PldmProtocol->SetSmbiosStructureTableMetaData (PldmProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set Smbios Structure table MetaData failed, Status = %r\n", Status));
      return;
    }
  }

  //A1 A2 table
  if (PcdGetBool (PcdMpmCreateSmBiosTypeA1) || PcdGetBool (PcdMpmCreateSmBiosTypeA2)) {
    Status = PublishSmbiosTypeA1A2 ();
    IDS_HDT_CONSOLE_PSP_TRACE ("PublishSmbiosTypeA1A2: %r\n", Status);
  }
  gBS->CloseEvent (Event);
}

EFI_STATUS
EFIAPI
PldmSmbiosEntry (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS                Status;
  EFI_EVENT                 Event;

  DEBUG ((DEBUG_INFO, "%a, Enter\n", __FUNCTION__));

  if (PcdGetBool (PcdMpmWirelessManageability) == FALSE) {
    return EFI_UNSUPPORTED;
  }

  // Check MPM Enable
  if (!IsMpmEnabled()) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  PldmSmbiosEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &Event
                  );

  DEBUG ((DEBUG_INFO, "%a, Exit %r\n", __FUNCTION__, Status));
  return Status;
}
