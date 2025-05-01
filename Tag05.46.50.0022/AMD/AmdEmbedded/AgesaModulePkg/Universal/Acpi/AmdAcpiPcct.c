/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Generate Platform Communications Channel Table (PCCT)
 *
 * Contains code that generate Platform Communications Channel Table (PCCT)
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Universal
 * @e \$Revision$   @e \$Date$
 *
 */


#include "AGESA.h"
#include "Filecode.h"
#include "AmdAcpiDxe.h"
#include "AmdAcpiPcct.h"
#include <Library/AmdBaseLib.h>
#include <Library/AmdHeapLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/CcxPstatesLib.h>
#include <Protocol/AmdNbioCppcServicesProtocol.h>
#include <Protocol/AmdAcpiPcctServicesProtocol.h>
#include <Protocol/AmdAcpiCpuSsdtServicesProtocol.h>

#define FILECODE UNIVERSAL_ACPI_AMDACPIPCCT_FILECODE

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *  All of the DATA should be defined in _CODE segment.
 *  Use ROMDATA to specify that it belongs to _CODE.
 *----------------------------------------------------------------------------
 */


STATIC PCCT_HEADER  ROMDATA PcctHeaderStruct =
{
  {'P','C','C','T'},    // Signature
  0,                    // Length
  2,                    // Revision
  0,                    // Checksum
  {0},                  // OEMID
  {0},                  // OEM Table ID
  1,                    // OEM Revision
  {'A','M','D',' '},    // Creator ID
  1,                    // Creator Revision
  0,                    // Flags
  {0}                   // Reserved
};

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *
 *  Generate PCCT
 *
 *  Description:
 *    This function will generate a complete Platform Communications Channel Table
 *    i.e. PCCT into a memory buffer.
 *
 *  Parameters:
 *    @param[in]       AcpiTableProtocol          Pointer to gEfiAcpiTableProtocolGuid protocol
 *    @param[in]       StdHeader                  Header for library and services
 *
 *    @retval          AGESA_STATUS
 *
 *---------------------------------------------------------------------------------------
 */
AGESA_STATUS
AmdAcpiPcct (
  IN       EFI_ACPI_TABLE_PROTOCOL  *AcpiTableProtocol,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{

  UINTN                                TableSize;
  UINTN                                TableKey;
  UINT8                                *EndOfTable;
  EFI_STATUS                           Status;
  EFI_STATUS                           CalledStatus;
  PCCT_HEADER                          *PcctHeaderStructPtr;
  ALLOCATE_HEAP_PARAMS                 AllocParams;
  AMD_CPPC_INFO                        AmdCppcInfo;
  AMD_CCX_ACPI_PCCT_SERVICES_PROTOCOL  *AcpiPcctServices;
  AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL  *AmdAcpiCpuSsdtServices;
  DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL  *NbioCppcServices;
  UINT8 ReportingVersion;

  IDS_HDT_CONSOLE (MAIN_FLOW, "  AmdAcpiPcct Entry\n");

  Status = EFI_SUCCESS;
  CalledStatus = gBS->LocateProtocol (&gAmdCcxAcpiPcctServicesProtocolGuid, NULL, (VOID **) &AcpiPcctServices);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  CalledStatus = gBS->LocateProtocol (&gAmdAcpiCpuSsdtServicesProtocolGuid, NULL, (VOID **)&AmdAcpiCpuSsdtServices);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  CalledStatus = gBS->LocateProtocol (&gAmdNbioCppcServicesProtocolGuid, NULL, (VOID **)&NbioCppcServices);
  Status = (CalledStatus > Status) ? CalledStatus : Status;

  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "  gAmdCcxAcpiPcctServicesProtocolGuid/gAmdAcpiCpuSsdtServicesProtocolGuid not found. Returning AGESA_UNSUPPORTED\n");
    return AGESA_UNSUPPORTED;
  }

  CalledStatus = EFI_UNSUPPORTED;
  NbioCppcServices->GetReportingVersion (&ReportingVersion);
  NbioCppcServices->CppcGetInfo   (NbioCppcServices, &AmdCppcInfo);
  if ((AmdCppcInfo.IsCppcEnabled) && (ReportingVersion == CppcReportingVersionMailbox)) {
    // Allocate space for the table
    AllocParams.RequestedBufferSize = ACPI_TABLE_MAX_LENGTH;
    AllocParams.BufferHandle = AMD_ACPI_TABLE_BUFFER_HANDLE;
    AllocParams.Persist = HEAP_SYSTEM_MEM;

    if (HeapAllocateBuffer (&AllocParams, StdHeader) != AGESA_SUCCESS) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "  Heap Allocate error. Returning AGESA_ERROR\n");
      return AGESA_ERROR;
    }

    PcctHeaderStructPtr = (PCCT_HEADER *) AllocParams.BufferPtr;
    EndOfTable = (UINT8 *) PcctHeaderStructPtr + sizeof (PCCT_HEADER);

    // Copies header information to the buffer
    LibAmdMemCopy ((VOID *) PcctHeaderStructPtr, (VOID *) &PcctHeaderStruct, (UINTN) (sizeof (PCCT_HEADER)), StdHeader);

    ASSERT (AsciiStrnLenS ((CHAR8 *)PcdGetPtr (PcdAmdAcpiTableHeaderOemId), 100) <= 6);
    ASSERT (AsciiStrnLenS ((CHAR8 *)PcdGetPtr (PcdAmdAcpiPcctTableHeaderOemTableId), 100) <= 8);

    LibAmdMemCopy ((VOID *) &PcctHeaderStructPtr->OemId,
                   (VOID *) PcdGetPtr (PcdAmdAcpiTableHeaderOemId),
                   AsciiStrnLenS ((CHAR8 *)PcdGetPtr (PcdAmdAcpiTableHeaderOemId), 6),
                   StdHeader);
    LibAmdMemCopy ((VOID *) &PcctHeaderStructPtr->OemTableId,
                   (VOID *) PcdGetPtr (PcdAmdAcpiPcctTableHeaderOemTableId),
                   AsciiStrnLenS ((CHAR8 *)PcdGetPtr (PcdAmdAcpiPcctTableHeaderOemTableId), 8),
                   StdHeader);

    CalledStatus = AcpiPcctServices->AddGenCommSubspaceStruct (AcpiPcctServices, PcctHeaderStructPtr, &AmdCppcInfo, &EndOfTable);

    PcctHeaderStructPtr->Length = (UINT32) (EndOfTable - (UINT8 *) PcctHeaderStructPtr);

    // Boundary check
    ASSERT (PcctHeaderStructPtr->Length <= ACPI_TABLE_MAX_LENGTH);
    if (PcctHeaderStructPtr->Length > ACPI_TABLE_MAX_LENGTH) {
      IDS_HDT_CONSOLE (CPU_TRACE, "ERROR: ACPI table buffer is overflow\n");
      IDS_DEADLOOP ()
    }

    // Update PCCT header Checksum
    ChecksumAcpiTable ((ACPI_TABLE_HEADER *) PcctHeaderStructPtr, StdHeader);

    // Publish PCCT
    TableSize = PcctHeaderStructPtr->Length;
    TableKey = 0;
    AcpiTableProtocol->InstallAcpiTable (
                    AcpiTableProtocol,
                    PcctHeaderStructPtr,
                    TableSize,
                    &TableKey
                    );

    // Deallocate the buffer
    HeapDeallocateBuffer (AMD_ACPI_TABLE_BUFFER_HANDLE, StdHeader);

  }

  if (CalledStatus == EFI_SUCCESS) {
    Status = AGESA_SUCCESS;
    IDS_HDT_CONSOLE (MAIN_FLOW, "  PCCT is created\n");
  } else {
    Status = AGESA_ERROR;
    IDS_HDT_CONSOLE (MAIN_FLOW, "  PCCT was not created\n");
  }

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
