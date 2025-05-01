/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AmdPlatformRasRmbDxe.h"
#include "AmdApeiBert.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern  EFI_BOOT_SERVICES       *gBS;



/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_ACPI_6_3_BOOT_ERROR_RECORD_TABLE_HEADER BertAcpiTable = {
  //EFI_ACPI_DESCRIPTION_HEADER
  { BERT_SIG,
    sizeof (EFI_ACPI_6_0_BOOT_ERROR_RECORD_TABLE_HEADER),
    ACPI_REV1,
    0,
    {'A','M','D',' ',' ',' '},
    AMD_BERT_TABLE_ID,
    1,
    AMD_CREATOR_ID,
    1
  },
  //  APEI_BERT_CONTEXT
  sizeof (EFI_ACPI_6_3_BOOT_ERROR_REGION_STRUCTURE),              // UINT32 nBootErrRegLen - Size of Boot Error Region Table
  0                                                               // UNIT64 BootErrRegPtr - Address of Boot Error Region Table Address
};

EFI_ACPI_6_3_BOOT_ERROR_REGION_STRUCTURE ApeiBootErrRegTbl = {
  {0,0,0,0,0},                                                      // UINT32 BlockStatus - WHEA Platform Design Guide Table 3-2 Block Status
  sizeof (EFI_ACPI_6_3_BOOT_ERROR_REGION_STRUCTURE),                // UINT32 RawDataOffset - WHEA Platform Design Guide Table 3-2 Raw Data Offset
  0,                                                                // UINT32 RawDataLength - WHEA Platform Design Guide Table 3-2 Raw Data Offset
  0,                                                                // UINT32 DataLength - WHEA Platform Design Guide Table 3-2 Raw Data Offset
  0x03                                                              // UINT32 ErrorSeverity - WHEA Platform Design Guide Table 3-2 Raw Data Offset (Initialize Severity to [None] = 0x03)
};

/**
 *---------------------------------------------------------------------------------------
 *
 *  ApeiBertInstall
 *
 *  Description:
 *    Allocate ACPI NVS memory for Boot error region table and provides the pointer to BERT
 *    ACPI table interface.  Copy BERT ACPI table to Boot Services memory space. Updates the ACPI
 *    table address in APEI private data structure which is shared by a protocol interface.
 *
 *  Parameters:
 *    @retval     EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
ApeiBertInstall ( VOID )
{
  EFI_STATUS                                    Status = EFI_SUCCESS;
  UINT8                                         *BootErrRgnTbl;
  EFI_ACPI_6_3_BOOT_ERROR_RECORD_TABLE_HEADER   *ApeiBertAcpiTable;
  UINTN                                         TableKey;

  //
  // Reserve EFI ACPI NVS Memory for Boot Error Region Table
  //
  Status = gBS->AllocatePool (EfiACPIMemoryNVS, sizeof (EFI_ACPI_6_3_BOOT_ERROR_REGION_STRUCTURE), &BootErrRgnTbl);
  // Copy Boot Error Region Table into EFI ACPI NVS Memory
  gBS->CopyMem (BootErrRgnTbl, &ApeiBootErrRegTbl, sizeof (EFI_ACPI_6_3_BOOT_ERROR_REGION_STRUCTURE));
  // Assign  Boot Error Region pointer
  BertAcpiTable.BootErrorRegion = (UINT64) ((UINTN) BootErrRgnTbl);

  //
  // Reserve EFI Boot Services Memory for APEI BERT Table
  //
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (EFI_ACPI_6_3_BOOT_ERROR_RECORD_TABLE_HEADER), &ApeiBertAcpiTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  gBS->CopyMem (&BertAcpiTable.Header.OemTableId, (UINT64 *)PcdGetPtr (PcdAmdAcpiBertTableHeaderOemTableId), sizeof (UINT64));
  gBS->CopyMem (ApeiBertAcpiTable, &BertAcpiTable, sizeof (EFI_ACPI_6_3_BOOT_ERROR_RECORD_TABLE_HEADER));

  // Update BERT header Checksum
  ChecksumAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *) ApeiBertAcpiTable);

  TableKey = 0;
  AcpiTableProtocol->InstallAcpiTable (
                  AcpiTableProtocol,
                  ApeiBertAcpiTable,
                  sizeof (EFI_ACPI_6_3_BOOT_ERROR_RECORD_TABLE_HEADER),
                  &TableKey
                  );

  gBS->FreePool (ApeiBertAcpiTable);

  return EFI_SUCCESS;
}

