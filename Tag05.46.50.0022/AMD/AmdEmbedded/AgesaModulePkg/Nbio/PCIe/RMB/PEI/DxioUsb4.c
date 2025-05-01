/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/* $NoKeywords:$ */
/**
 * @file
 *
 * DxioUsb4 - Update DXIO topology structure for USB4 entries
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <AMD.h>
#include <Gnb.h>
#include <PiPei.h>
#include <GnbDxio.h>
#include <Library/PeiServicesTablePointerLib.h>

//From AgesaPkg
#include <AmdPcieComplex.h>
#include <Ppi/NbioPcieTopologyPpi.h>

#define FILECODE        NBIO_PCIE_RMB_PEI_DXIOUSB4_FILECODE
/**
  Count the number of socket entries so we allocate the right size of a table

  @param Socket0Entries   Pointer to variable to store socket 0 entries
  @param Entries          Pointer to array of structures containing topology data
**/
STATIC
UINTN
CountEntries (
  IN  DXIO_COMPLEX_DESCRIPTOR         *ComplexTable
  )
{
  UINTN        Counter;

  Counter = 0;
  // Figure out how many entries are currently in the table, including terminator (1 based quantity)
  do {
    Counter++;
  } while ((ComplexTable->PciePortList[Counter].Flags & DESCRIPTOR_TERMINATE_LIST ) == 0 );
  return ++Counter;
}

/**
  Adds USB4 entries to DXIO complex descriptor

  @param PeiServices          Pointer to EFI_PEI_SERVICES pointer
  @param ControllerCount      Number of USB4 PCIE controllers to add to the topology
  @param ComplexDescriptor    Pointer to DXIO complex descriptor data

  @return BOOLEAN        True if table was enabled and built, false if not
**/
VOID
DxioTopologyAddUSB4 (
  IN     CONST EFI_PEI_SERVICES     **PeiServices,
  IN     UINTN                      ControllerCount,
  IN     DXIO_COMPLEX_DESCRIPTOR    **ComplexDescriptor
  )
{
  PEI_AMD_NBIO_PCIE_TOPOLOGY_PPI  *TopologyProtocol;
  DXIO_COMPLEX_DESCRIPTOR         *ComplexTable;
  UINTN                           EntryCount;
  EFI_STATUS                      Status;
  BOOLEAN                         Result;
  UINTN                           Index;
  UINT64                          EntryHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Entry\n", __FUNCTION__);

  if ((ComplexDescriptor == NULL) ||
      (*ComplexDescriptor == NULL) ||
      (PeiServices == NULL) ||
      (ControllerCount == 0)) {
    return;
  }

  // Get instance of Topology services PPI
  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  Status = (**PeiServices).LocatePpi (
                             PeiServices,
                             &gAmdNbioPcieTopologyPpiGuid,
                             0,
                             NULL,
                             &TopologyProtocol
                             );
  if (Status != EFI_SUCCESS) {
    return;
  }

  ComplexTable = *ComplexDescriptor;
  Result = FALSE;
  EntryCount = CountEntries (ComplexTable);
  IDS_HDT_CONSOLE (GNB_TRACE, " Entry Count returned = %d\n", EntryCount);

  EntryCount += 2;
  Status = TopologyProtocol->IncreaseTableSize (&ComplexTable, EntryCount, &Result);
  if ((Status != EFI_SUCCESS) || (Result == FALSE)) {
    IDS_HDT_CONSOLE (GNB_TRACE, " IncreaseTableSize Returned ERROR!!!\n", __FUNCTION__);
    return;
  }
  // Add entry for
  for (Index = 0; Index < ControllerCount; Index++) {
    IDS_HDT_CONSOLE (GNB_TRACE, " Calling InitializeTopologyEntry for Index = %d\n", Index);
    Status = TopologyProtocol->InitializeTopologyEntry
                                 (&ComplexTable, DxioUSB_OVER_PCIE, 0, 0, 0, 0, 0, &Result, &EntryHandle);
    if ((Status != EFI_SUCCESS) || (Result == FALSE)) {
      IDS_HDT_CONSOLE (GNB_TRACE, " InitializeTopologyEntry Returned ERROR!!!\n", __FUNCTION__);
      return;
    }
  }
  // Update address of Complex to caller
  *ComplexDescriptor = ComplexTable;
  return;
}
