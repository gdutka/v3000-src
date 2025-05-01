/*
*****************************************************************************
*
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

#ifndef _RAS_ACPI_LIB_H_
#define _RAS_ACPI_LIB_H_

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "PiDxe.h"
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/BaseLib.h>
#include "AmdRas.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
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


/*---------------------------------------------------------------------------------------*/
VOID
ChecksumAcpiTable (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER *Table
  );

EFI_STATUS
SearchAcpiTable (
  IN       EFI_ACPI_TABLE_PROTOCOL      *AcpiTableProtocol,
  IN       EFI_ACPI_SDT_PROTOCOL        *AcpiSdtProtocol,
  IN       UINT32                       TableSignature,
  OUT      EFI_ACPI_SDT_HEADER          **Table,
  OUT      UINTN                        *TableKey
);

EFI_STATUS
AddBertErrorRecord (
  IN UINT8  *ErrorRecord,
  IN UINT32 RecordLen,
  IN UINT8  ErrorType,
  IN UINT8  SeverityType
  );

EFI_STATUS
AddMultipleHestErrorRecord (
  IN UINT8  *ErrorRecord,
  IN UINT32 RecordLen,
  IN UINT32 RecordCount
  );

#endif //_RAS_ACPI_LIB_H_


