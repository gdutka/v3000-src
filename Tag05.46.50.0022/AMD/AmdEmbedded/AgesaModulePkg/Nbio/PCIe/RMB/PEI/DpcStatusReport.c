/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe DPC Status PPI.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 276746 $   @e \$Date: 2014-10-28 12:22:52 -0600 (Mon, 28 Oct 2013) $
 *
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <PiPei.h>
#include <Filecode.h>
#include <GnbDxio.h>
#include <Ppi/NbioPcieDpcStatusPpi.h>

#define FILECODE NBIO_PCIE_RMB_PEI_DPCSTATUSREPORT_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
typedef struct {
  PEI_AMD_NBIO_PCIE_DPCSTATUS_PPI Ppi;
  EFI_PEI_PPI_DESCRIPTOR          PpiList;
  PCIe_DPC_STATUS_DATA            DpcStatusData;
} PEI_DPCSTATUS_INTERNAL_DATA;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
PcieGetDpcStatusData (
  IN  PEI_AMD_NBIO_PCIE_DPCSTATUS_PPI  *This,
  OUT PCIe_DPC_STATUS_DATA             **DpcStatus
  );

/*----------------------------------------------------------------------------------------
 *                     L O C A L   D A T A   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
static PEI_AMD_NBIO_PCIE_DPCSTATUS_PPI PcieDpcStatusPpi = {
  PcieGetDpcStatusData
};

/*----------------------------------------------------------------------------------------*/
/**
 * Returns the DPC Status data structure
 *
 *
 * @param[in]  This         A pointer to the PEI_AMD_NBIO_PCIE_DPCSTATUS_PPI instance.
 * @param[out] DpcStatus    A pointer to a pointer to store the address of DPC Status data array
 */
EFI_STATUS
PcieGetDpcStatusData (
  IN  PEI_AMD_NBIO_PCIE_DPCSTATUS_PPI  *This,
  OUT PCIe_DPC_STATUS_DATA             **DpcStatus
  )
{
  PEI_DPCSTATUS_INTERNAL_DATA *DpcThis;
  DpcThis = (PEI_DPCSTATUS_INTERNAL_DATA *) This;
  *DpcStatus = (PCIe_DPC_STATUS_DATA*)&DpcThis->DpcStatusData;
  IDS_HDT_CONSOLE (GNB_TRACE, "%a (0x%x)\n", __FUNCTION__, *DpcStatus);
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Install DPC Status report PPI
 *
 *
 *
 * @param[in]  PeiServices       Pointer to EFI_PEI_SERVICES pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
PcieDpcStatusServiceInit (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;
  PEI_DPCSTATUS_INTERNAL_DATA *pDpcStatus;

  Status = (*PeiServices)->AllocatePool (PeiServices,
                                         sizeof (PEI_DPCSTATUS_INTERNAL_DATA),
                                         (VOID **) & pDpcStatus);
  if (Status == EFI_SUCCESS) {
    (*PeiServices)->SetMem (pDpcStatus,
                           sizeof (PEI_DPCSTATUS_INTERNAL_DATA),
                           0
                           );
    (*PeiServices)->CopyMem (pDpcStatus,
                             &PcieDpcStatusPpi,
                             sizeof (PEI_AMD_NBIO_PCIE_DPCSTATUS_PPI) );
    pDpcStatus->PpiList.Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    pDpcStatus->PpiList.Guid  = &gAmdNbioPcieDpcStatusPpiGuid;
    pDpcStatus->PpiList.Ppi   = pDpcStatus;
    Status = (*PeiServices)->InstallPpi (PeiServices, &pDpcStatus->PpiList);
  }

  return Status;
}



