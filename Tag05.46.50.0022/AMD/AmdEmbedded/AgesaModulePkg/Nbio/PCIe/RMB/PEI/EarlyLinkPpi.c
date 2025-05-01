/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Early Link Ppi Implementation
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdCoreLaunchServicePpi
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <AMD.h>
#include <PiPei.h>
#include <Ppi/NbioEarlyLinkPpi.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <Library/IdsLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <GnbRegistersRMB.h>
#include <Filecode.h>

#define FILECODE        NBIO_PCIE_RMB_PEI_EARLYLINKPPI_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init hotplug features on all hotplug ports
 *
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
FindEarlyLink (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GNB_HANDLE            *GnbHandle;
  EARLY_LINK_STATUS     *EarlyLinkStatus;

  if (Engine->Type.Port.PortData.MiscControls.SbLink == 1) {
    if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
      EarlyLinkStatus = (EARLY_LINK_STATUS *)Buffer;
      EarlyLinkStatus->EarlyLinkStatus = TRUE;
      GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);
      EarlyLinkStatus->PhysicalRootBridge = GnbHandle->RBIndex;
      EarlyLinkStatus->LogicalRootBridge = GnbHandle->LogicalRBIndex;
      EarlyLinkStatus->RootPortBus = (UINT8) GnbHandle->Address.Address.Bus;
      EarlyLinkStatus->RootPortDevice = Engine->Type.Port.PortData.DeviceNumber;
      EarlyLinkStatus->RootPortFunction = Engine->Type.Port.PortData.FunctionNumber;
    }
  }
}

/**
 * This service retrieves information the location of the Early Train Link.
 *
 * @param[out] EarlyLinkStatus                Pointer to the structure to hold the early train status
 *
 * @retval EFI_SUCCESS                        The core topology information was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER              Socket is non-existent.
 * @retval EFI_INVALID_PARAMETER              Die is non-existent.
 * @retval EFI_INVALID_PARAMETER              All output parameter pointers are NULL.
 *
 **/
EFI_STATUS
GetEarlyLinkConfig (
  IN OUT   EARLY_LINK_STATUS      *EarlyLinkStatus
  )
{
  CONST EFI_PEI_SERVICES                        **PeiServices;
  PEI_AMD_NBIO_PCIE_SERVICES_PPI                *PcieServicesPpi;
  PCIe_PLATFORM_CONFIG                          *Pcie;
  EFI_STATUS                                    Status;
  GNB_PCIE_INFORMATION_DATA_HOB                 *PciePlatformConfigHob;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  EarlyLinkStatus->EarlyLinkStatus = FALSE;
  EarlyLinkStatus->PhysicalRootBridge = 0;
  EarlyLinkStatus->LogicalRootBridge = 0;
  EarlyLinkStatus->RootPortBus = 0;
  EarlyLinkStatus->RootPortDevice = 0;
  EarlyLinkStatus->RootPortFunction = 0;

  PeiServices = GetPeiServicesTablePointer();
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdNbioPcieServicesPpiGuid,
                             0,
                             NULL,
                             (VOID **)&PcieServicesPpi
                             );
  ASSERT (Status == EFI_SUCCESS);
  if (Status == EFI_SUCCESS) {
    PcieServicesPpi->PcieGetTopology (PcieServicesPpi, &PciePlatformConfigHob);
    Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);

    PcieConfigRunProcForAllEngines (
      DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
      FindEarlyLink,
      EarlyLinkStatus,
      Pcie
      );
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------
 *                          P P I   D E S C R I P T O R
 *----------------------------------------------------------------------------------------
 */

PEI_AMD_NBIO_EARLY_LINK_PPI  mAmdNbioEarlyLinkPpi = {
  GetEarlyLinkConfig
};



