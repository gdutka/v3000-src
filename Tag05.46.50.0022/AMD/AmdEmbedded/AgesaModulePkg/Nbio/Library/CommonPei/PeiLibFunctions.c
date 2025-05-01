/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiPei.h>
#include <Filecode.h>
#include <GnbDxio.h>
#include <GnbRegistersBA.h>
//In AgesaPkg
#include <AmdPcieComplex.h>
#include <Ppi/NbioPcieComplexPpi.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Ppi/FabricTopologyServices2Ppi.h>
#include <Ppi/SocLogicalIdPpi.h>

#include <Library/AmdBaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/IdsLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DxioLibV2.h>
#include <Guid/GnbPcieInfoHob.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

#include <Library/NbioCommonLib.h>

#define FILECODE        NBIO_LIBRARY_COMMONPEI_PEILIBFUNCTIONS_FILECODE

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

/*----------------------------------------------------------------------------------------
 *                    P P I   N O T I F Y   D E S C R I P T O R S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/**
 * Function to retrieve SOC_LOGICAL_ID
 *
 *
 * @param[out]  LogicalId       Pointer to SOC_LOGICAL_ID
 * @retval      EFI_STATUS

 */

EFI_STATUS
PcieGetLogicalId (
     OUT   SOC_LOGICAL_ID           *LogicalId
  )
{
  EFI_STATUS                        Status;
  CONST EFI_PEI_SERVICES            **PeiServices;
  AMD_PEI_SOC_LOGICAL_ID_PPI        *SocLogicalIdPpi;

  PeiServices = GetPeiServicesTablePointer();
  // Get Logical CPU ID info
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdSocLogicalIdPpiGuid,
                             0,
                             NULL,
                             (VOID **)&SocLogicalIdPpi
                             );
  if (!EFI_ERROR (Status)) {
    Status = SocLogicalIdPpi->GetLogicalIdOnCurrentCore(LogicalId);
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Function to retrieve PCIe_PLATFORM_CONFIG
 *
 *
 * @param[out]  Pcie            Pointer to PCIe_PLATFORM_CONFIG Pointer
 * @retval      EFI_STATUS

 */

EFI_STATUS
PcieGetPciePei (
     OUT     PCIe_PLATFORM_CONFIG          **Pcie
  )
{
  EFI_STATUS                     Status;
  EFI_PEI_HOB_POINTERS           Hob;
  GNB_PCIE_INFORMATION_DATA_HOB  *GnbPcieInfoDataHob;

  GnbPcieInfoDataHob = NULL;
  Status = PeiServicesGetHobList ((VOID **)&Hob.Raw);

  if (!EFI_ERROR (Status)) {
    while (!END_OF_HOB_LIST (Hob)) {
      if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
          CompareGuid ( &Hob.Guid->Name, &gGnbPcieHobInfoGuid)) {
        Status = EFI_SUCCESS;
        break;
      }
      Hob.Raw = GET_NEXT_HOB (Hob);
    }
    GnbPcieInfoDataHob = (GNB_PCIE_INFORMATION_DATA_HOB *) (Hob.Raw);
    *Pcie = &(GnbPcieInfoDataHob->PciePlatformConfigHob);
  }
  return Status;
}

