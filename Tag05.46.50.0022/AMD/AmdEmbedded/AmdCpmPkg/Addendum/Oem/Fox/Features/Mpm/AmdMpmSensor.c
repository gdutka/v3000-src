/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/MpmLib.h>
#include <Library/MpmMboxLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiTable.h>
#include <Library/DxeServicesLib.h>


/**
 * @brief Publish MPM Sensor ACPI table
 *
 * @return EFI_STATUS
 */
VOID
PublishMpmSensorSsdt (
  IN EFI_EVENT  Event,
  IN VOID       *Context
)
{
  EFI_STATUS                                Status;
  UINTN                                     Size;
  UINTN                                     TableList;
  EFI_ACPI_COMMON_HEADER                    *CurrentTable;
  INTN                                      Instance;
  EFI_ACPI_TABLE_PROTOCOL                   *AcpiTableProtocol;

  Status = gBS->LocateProtocol (
                    &gEfiAcpiTableProtocolGuid,
                    NULL,
                    &AcpiTableProtocol
                    );
  if (EFI_ERROR (Status)) {
    return;
  }

  Instance     = 0;
  CurrentTable = NULL;
  Status = GetSectionFromAnyFv (
            &gEfiCallerIdGuid,
            EFI_SECTION_RAW,
            Instance,
            (VOID**)&CurrentTable,
            &Size
          );
  if (!EFI_ERROR (Status)) {
    if (((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId != SIGNATURE_64 ('M', 'P', 'M', 'S', 0, 0, 0, 0)) {
      return;
    }
  } else {
    return;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("MPM Sensor SSDT table length = 0x%x\n", CurrentTable->Length);

  TableList = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (
              AcpiTableProtocol,
              CurrentTable,
              Size,
              &TableList);
  if (!EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Install MPM Sensor SSDT Success.\n");
    FreePool(CurrentTable);
  }

  return;
}

EFI_STATUS
MpmSensorAcpiEntry (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS            EfiStatus;
  EFI_EVENT             Event;

  EfiStatus = EFI_SUCCESS;
  IDS_HDT_CONSOLE_PSP_TRACE ("MpmSensorAcpiEntry Enter\n");

  if (PcdGetBool (PcdMpmWirelessManageability) == FALSE) {
    return EFI_UNSUPPORTED;
  }

  // Check MPM Enable
  if (!IsMpmEnabled()) {
    return EFI_UNSUPPORTED;
  }

  EfiStatus = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  PublishMpmSensorSsdt,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &Event
                  );
  if (EFI_ERROR (EfiStatus)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Create gEfiEventReadyToBootGuid Fail\n");
  }

  return EfiStatus;
}


