/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * SMU Early Initialization Function
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioSmuV13Pei
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <PiPei.h>
#include <Filecode.h>
#include <GnbDxio.h>
#include <GnbRegistersRMB.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcieConfigLib.h>
//#include <PcieComplexDataZP.h>
#include <Ppi/NbioBaseServicesZPPpi.h>
#include <Guid/GnbNbioBaseZPInfoHob.h>

#define FILECODE        NBIO_SMU_RMB_PEI_EARLYINITFEATURE_FILECODE

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
 * Smu Early Init
 *
 * This is the main flow of the early initialization of the SMU.
 *
 * @param[in]       Pcie           Pointer to global PCIe configuration
 */
EFI_STATUS
AmdNbioSmuEarlyInit (
  IN       PCIe_PLATFORM_CONFIG            *Pcie
  )
{
  EFI_STATUS                      Status;
  GNB_HANDLE                      *NbioHandle;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);

  Status = EFI_SUCCESS;
  NbioHandle = NbioGetHandle (Pcie);
  while (NbioHandle != NULL) {
    // Repeat initialization for each NbioHandle of the platform
    // NbioSmuFirmwareTestV13 (NbioHandle);
    // TBD - Add SMU initialization functions here

    NbioHandle = GnbGetNextHandle (NbioHandle);
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  return Status;
}



