/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiDxe.h>
#include <Filecode.h>
#include <GnbDxio.h>
//In AgesaPkg
#include <Protocol/SocLogicalIdProtocol.h>
#include <Guid/GnbPcieInfoHob.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/UefiLib.h>
#include <Guid/HobList.h>
#include <Library/IdsLib.h>
#include <Library/HobLib.h>

#include <Library/NbioCommonLibDxe.h>

#define FILECODE        NBIO_LIBRARY_COMMONDXE_DXELIBFUNCTIONS_FILECODE

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
  EFI_STATUS                    Status;
  AMD_SOC_LOGICAL_ID_PROTOCOL   *SocLogicalId;

  Status = gBS->LocateProtocol (&gAmdSocLogicalIdProtocolGuid, NULL, (VOID **) &SocLogicalId);
  if (!EFI_ERROR (Status)) {
    ASSERT (!EFI_ERROR (Status));
    SocLogicalId->GetLogicalIdOnCurrentCore (SocLogicalId, LogicalId);
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
PcieGetPcieDxe (
     OUT     PCIe_PLATFORM_CONFIG          **Pcie
  )
{
  EFI_STATUS                           Status;
  GNB_PCIE_INFORMATION_DATA_HOB        *GnbPcieInformationDataHob;

  // Correcting Pcie information from Hob

  //
  // Get information data from HOB.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, (VOID **)&GnbPcieInformationDataHob);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GnbPcieInformationDataHob = GetNextGuidHob (&gGnbPcieHobInfoGuid, GnbPcieInformationDataHob);
  if (GnbPcieInformationDataHob == NULL) {
    return EFI_NOT_FOUND;
  }
  *Pcie = &(GnbPcieInformationDataHob->PciePlatformConfigHob);

  return Status;
}


