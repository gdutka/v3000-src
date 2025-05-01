/**
 *  @file   PciePeiServices.c
 *  @brief  PPI implementation of PciePeiServices
 */
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

#include <Library/AmdBaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/IdsLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DxioLibV2.h>
#include <Guid/GnbPcieInfoHob.h>

#include <Library/NbioCommonLib.h>

#define FILECODE        NBIO_LIBRARY_COMMONPEI_PCIEPEISERVICES_FILECODE

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


EFI_STATUS
PcieGetTopology (
  IN       PEI_AMD_NBIO_PCIE_SERVICES_PPI  *This,
     OUT   GNB_PCIE_INFORMATION_DATA_HOB   **DebugOptions
  )
{
  EFI_STATUS                     Status;
  EFI_PEI_HOB_POINTERS           Hob;
  GNB_PCIE_INFORMATION_DATA_HOB  *GnbPcieInfoDataHob;

  GnbPcieInfoDataHob = NULL;

  Status = PeiServicesGetHobList ((VOID **)&Hob.Raw);

  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
        CompareGuid ( &Hob.Guid->Name, &gGnbPcieHobInfoGuid)) {
      GnbPcieInfoDataHob = (GNB_PCIE_INFORMATION_DATA_HOB *) (Hob.Raw);
      Status = EFI_SUCCESS;
      break;
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  *DebugOptions = GnbPcieInfoDataHob;

  return Status;
}

EFI_STATUS
PcieSetSpeed (
  IN  PEI_AMD_NBIO_PCIE_SERVICES_PPI  *This,
  IN  CONST EFI_PEI_SERVICES          **PeiServices,
  IN  UINT8                           PciDevice,
  IN  UINT8                           PciFunction,
  IN  UINT8                           TargetSpeed
  )
{
  EFI_STATUS                     Status;
  GNB_HANDLE                     *GnbHandle;
  PCIe_PLATFORM_CONFIG           *Pcie;
  GNB_PCIE_INFORMATION_DATA_HOB  *PciePlatformConfigHob;
  PEI_AMD_NBIO_SMU_SERVICES_PPI  *SmuServicesPpi;
  UINT32                         EngineId;
  PCIe_ENGINE_CONFIG             *PcieEngine;
  PCIe_WRAPPER_CONFIG            *PcieWrapper;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry for Device %d Function %d to Gen%d\n", __FUNCTION__, PciDevice, PciFunction, TargetSpeed);
  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioSmuServicesPpiGuid, 0, NULL, (VOID **)&SmuServicesPpi);

  PcieGetTopology (This, &PciePlatformConfigHob);
  Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);
  GnbHandle = NbioGetHandle (Pcie);
  EngineId = 0xFF;

  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if ((PcieEngine->Type.Port.PortData.DeviceNumber == PciDevice)
          && (PcieEngine->Type.Port.PortData.FunctionNumber == PciFunction)
          && ((PcieEngine->Header.DescriptorFlags & DESCRIPTOR_ALLOCATED) == DESCRIPTOR_ALLOCATED)) {
        EngineId = PcieEngine->Type.Port.PcieBridgeId;
        break;
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    if (EngineId != 0xFF) {
      break;
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }

//  EngineId = 3; //Temporary for PT debug
///////@TODO  DxioSetLinkSpeedV2 (PeiServices, SmuServicesPpi, GnbHandle->InstanceId, EngineId, TargetSpeed);

  return Status;
}

STATIC PEI_AMD_NBIO_PCIE_SERVICES_PPI mNbioPcieServicePpi = {
  AMD_NBIO_PCIE_SERVICES_REVISION,  ///< revision
  PcieGetTopology,
  PcieSetSpeed
};

STATIC EFI_PEI_PPI_DESCRIPTOR mNbioPcieServicesPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioPcieServicesPpiGuid,
  &mNbioPcieServicePpi,
};

EFI_STATUS
EFIAPI
PcieInstallServicesPpi (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS       Status;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  // Install PPI to provide Topology to other drivers
  Status = (**PeiServices).InstallPpi (PeiServices, &mNbioPcieServicesPpiList);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  return Status;
}


