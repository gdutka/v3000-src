/*****************************************************************************
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *****************************************************************************/

#include "AsfTableAsl.h"

extern ASF_TABLE_DESCRIPTION ASFT;

EFI_STATUS
EFIAPI
InstallAsfAcpiTableEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_ACPI_TABLE_PROTOCOL   *AcpiTable;
  UINTN                     TableHandle;
  VOID                      *AsfTablePtr;

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  ASSERT_EFI_ERROR (Status);

  AsfTablePtr = &ASFT;
  TableHandle = 0;
  Status = AcpiTable->InstallAcpiTable (
                              AcpiTable,
                              AsfTablePtr,
                              ((EFI_ACPI_COMMON_HEADER*)AsfTablePtr)->Length,
                              &TableHandle
                              );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
