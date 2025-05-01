/** @file
  Install UEFI ACPI table which contains BVDT(BIOS Version Date and Time) data.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Protocol/AcpiTable.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/H2OLib.h>
#include <Library/FlashRegionLib.h>
#include <AcpiHeaderDefaultValue.h>
#include "UefiTableDxe.h"


STATIC UINT8 mAcpmOemId[6]               = { EFI_ACPI_OEM_ID };
STATIC EFI_GUID mAcpiTableInsydeBiosGuid = UEFI_ACPI_TABLE_INSYDE_BIOS_GUID;
/**
 This function calculates and updates an UINT8 checksum.

 @param [in]   Buffer
 @param [in]   Size             Number of bytes to checksum.

 @retval None

**/
VOID
UefiAcpiTableChecksum (
  IN UINT8      *Buffer,
  IN UINTN      Size
  )
{
  UINTN ChecksumOffset;

  ChecksumOffset = OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER, Checksum);

  //
  // Set checksum to 0 first.
  //
  Buffer[ChecksumOffset] = 0;

  //
  // Update checksum value.
  //
  Buffer[ChecksumOffset] = CalculateCheckSum8 (Buffer, Size);
}


/**
 Retreive BVDT info.

 @param [in]   Bvdt                 Pointer to BVDT binary.
 @param [in]   BvdtRegionSize       Size of BVDT region.
 @param [in]   DataSize             Data Size of BVDT.

  @retval EFI_SUCCESS               Get BVDT info succesfully.
  @retval Others                    Get BVDT info fail.

**/
EFI_STATUS
EFIAPI
RetreiveBvdt (
  IN UINT8      *Bvdt,
  IN UINTN      BvdtRegionSize,
  OUT UINTN     *DataSize
)
{
  UINT8         *Ptr;

  if (Bvdt == NULL || DataSize == NULL){
    return EFI_INVALID_PARAMETER;
  }

  if (CompareMem ((VOID*) BVDT_SIGNATURE, (VOID*) Bvdt, AsciiStrLen (BVDT_SIGNATURE) != 0)){
    return EFI_INVALID_PARAMETER;
  }

  Ptr = Bvdt;
  Ptr += AsciiStrLen (BVDT_SIGNATURE);

  while (Ptr + AsciiStrLen (END_OF_BVDT_SIGNATURE) <= Bvdt + BvdtRegionSize){
    if (CompareMem ((VOID*) END_OF_BVDT_SIGNATURE, (VOID*) Ptr, AsciiStrLen (END_OF_BVDT_SIGNATURE)) == 0){
      *DataSize = (UINTN)(Ptr + AsciiStrLen (END_OF_BVDT_SIGNATURE) - Bvdt);
      return EFI_SUCCESS;
    }
    Ptr++;
  }
  return EFI_NOT_FOUND;
}


/**
 Install UEFI ACPI table which contains BVDT data.

 @param [in]   Event
 @param [in]   Context

 @retval None

**/
VOID
EFIAPI
InstallUefiTable (
  IN EFI_EVENT        Event,
  IN VOID             *Context
)
{
  EFI_STATUS                        Status;
  UINTN                             TableHandle;
  EFI_ACPI_TABLE_PROTOCOL           *AcpiTable;
  UINTN                             DataSize;
  EFI_ACPI_UEFI_TABLE               *UefiTable;

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = RetreiveBvdt (
             (UINT8*)(UINTN)FdmGetNAtAddr (&gH2OFlashMapRegionBvdtGuid, 1),
             (UINTN)FdmGetNAtSize(&gH2OFlashMapRegionBvdtGuid, 1),
             &DataSize
             );
  if (EFI_ERROR (Status)){
    return;
  }


  UefiTable = H2OAllocate (DataSize + sizeof (EFI_ACPI_UEFI_TABLE));
  if (UefiTable == NULL){
    return;
  }

  UefiTable->UefiAcpiDataTable.Header.Signature = UEFI_ACPI_TABLE_SIGNATURE;
  UefiTable->UefiAcpiDataTable.Header.Length    = (UINT32)DataSize + sizeof (EFI_ACPI_UEFI_TABLE);
  UefiTable->UefiAcpiDataTable.Header.Revision  = 1;
  // Checksum will be updated at runtime
  UefiTable->UefiAcpiDataTable.Header.Checksum  = 0;
  CopyMem (&UefiTable->UefiAcpiDataTable.Header.OemId, &mAcpmOemId, sizeof (UefiTable->UefiAcpiDataTable.Header.OemId));
  UefiTable->UefiAcpiDataTable.Header.OemTableId = EFI_ACPI_OEM_TABLE_ID;
  UefiTable->UefiAcpiDataTable.Header.OemRevision = EFI_ACPI_OEM_REVISION;
  UefiTable->UefiAcpiDataTable.Header.CreatorId = EFI_ACPI_CREATOR_ID;
  UefiTable->UefiAcpiDataTable.Header.CreatorRevision = EFI_ACPI_CREATOR_REVISION;
  CopyGuid (&UefiTable->UefiAcpiDataTable.Identifier, &mAcpiTableInsydeBiosGuid);
  UefiTable->UefiAcpiDataTable.DataOffset = (UINT16) sizeof (EFI_ACPI_UEFI_TABLE);
  CopyMem (
    (VOID *)(UINTN) (UefiTable + 1),
    (VOID *) (UINTN) FdmGetNAtAddr (&gH2OFlashMapRegionBvdtGuid, 1),
    DataSize
    );

  UefiAcpiTableChecksum ((UINT8 *) UefiTable, DataSize + sizeof (EFI_ACPI_UEFI_TABLE));

  TableHandle = 0x00;
  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        UefiTable,
                        DataSize + sizeof (EFI_ACPI_UEFI_TABLE),
                        &TableHandle
                        );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "InstallAcpiTable(%r)\n", Status));
  }

  H2OFreePool ((VOID**)&UefiTable);

  return;
}

/**
  Entry point for the driver.

 @param [in]   ImageHandle      The firmware allocated handle for the EFI image.
 @param [in]   SystemTable      A pointer to the EFI System Table.

 @retval       EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
UefiTableDxeEntryPoint (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE    *SystemTable
  )
{
  VOID         *Registration;

  EfiCreateProtocolNotifyEvent (
    &gEfiAcpiTableProtocolGuid,
    TPL_CALLBACK,
    InstallUefiTable,
    NULL,
    &Registration
    );

  return EFI_SUCCESS;
}
