/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_PLATFORM_RAS_RMB_DXE_H_
#define _AMD_PLATFORM_RAS_RMB_DXE_H_

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "PiDxe.h"
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiTable.h>
#include "AmdRasRegistersRmb.h"
#include <Library/AmdCpmRasLib.h>
#include "AmdRas.h"
#include "AmdCpmRas.h"
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/SocLogicalIdProtocol.h>

extern EFI_GUID gAmdPlatformApeiDataProtocolGuid;
extern PLATFORM_APEI_PRIVATE_BUFFER_RMB  *mPlatformApeiPrivate;
extern EFI_ACPI_TABLE_PROTOCOL       *AcpiTableProtocol;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
ChecksumAcpiTable (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER *Table
  );

EFI_STATUS
SearchMaxMcaBankCnt (
  OUT      UINT8* MaxBankCount
  );

EFI_STATUS
BldPcieMapAndBootErrDet (
  VOID
);

EFI_STATUS
RasLateBootInit2 (
  VOID
);

EFI_STATUS
AcquireNbioSecBusNumHelper (
  IN       UINT8 DfNode,
  IN       UINT8 NbioNumber,
  OUT      UINT8 *NbioSecBusNum
  );

#endif // _AMD_PLATFORM_RAS_RMB_DXE_H_

