/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/**
 *  @file MpioTrainingResults.c
 *  @brief Parses the results of MPIO training and updates PCIe complex status
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Filecode.h>
#include <PiPei.h>
#include <AmdPcieComplex.h>
#include <GnbDxio.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Library/BaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/MpioInitLib.h>
#include "MpioLibLocal.h"
#include <Ppi/PcieAuthenticationPpi.h>
#include <Library/PeiServicesTablePointerLib.h>

#define FILECODE NBIO_LIBRARY_MPIOINITLIB_MPIOTRAININGRESULTS_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Mpio Pcie Authentication After Training
 *
 * @param[in]  Engine          Pointer per PCIE port Engine data
 *
 */
VOID
MpioPcieAuthenticationAfterTraining (
  PCIe_ENGINE_CONFIG              *Engine
) {
  EFI_STATUS                      Status;
  CONST EFI_PEI_SERVICES          **PeiServices;
  PEI_AMD_PCIE_AUTHENTICATION_PPI *AmdPcieAuthentication = NULL;
  PORT_AUTHENTICATION_DESC        PortAuth;
  BOOLEAN                         Authenticated;

  Authenticated = FALSE;
  PeiServices = GetPeiServicesTablePointer ();

  Status = (**PeiServices).LocatePpi (PeiServices, &gAmdPcieAuthenticationGuid, 0, NULL, (VOID **)&AmdPcieAuthentication);
  if (EFI_ERROR(Status)) {
    return;
  }

  PortAuth.PciLocation.AsBits.Segment = Engine->Type.Port.Address.Address.Segment;
  PortAuth.PciLocation.AsBits.Bus = Engine->Type.Port.Address.Address.Bus;
  PortAuth.PciLocation.AsBits.Device = Engine->Type.Port.PortData.DeviceNumber;
  PortAuth.PciLocation.AsBits.Function = Engine->Type.Port.PortData.FunctionNumber;
  PortAuth.StartLaneIndex = Engine->EngineData.StartLane;
  PortAuth.EndLaneIndex = Engine->EngineData.EndLane;

  Status = AmdPcieAuthentication->PcieAuthenticationAfterTraining (&PortAuth, &Authenticated);
  if (EFI_ERROR(Status)) {
    return;
  }
  if (Authenticated) {
    IDS_HDT_CONSOLE( GNB_TRACE, "PcieAuthenticationAfterTraining TRUE\n");
  } else {
    IDS_HDT_CONSOLE( GNB_TRACE, "PcieAuthenticationAfterTraining FALSE\n");
    Engine->InitStatus = INIT_STATUS_PCIE_PORT_TRAINING_FAIL;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * DXIO Update Port Training Status
 *
 *
 *
 * @param[in]  GnbHandle          Pointer to the Silicon Descriptor for this node
 * @param[in]  MpioData           Pointer to the platform descriptor for this node
 * @retval     AGESA_STATUS
 */
VOID
MpioUpdatePortTrainingStatus (
  IN       GNB_HANDLE                *GnbHandle,
  OUT      MPIO_DATA                 *MpioData
  )
{
  // DXIO Firmware Structures
  FW_ASK_STRUCT             *AskEntry;
  PCIe_ENGINE_CONFIG        *Engine;
  UINT32                    Index;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a : Enter\n", __FUNCTION__);

  AskEntry = MpioData->MpioAsk;

  // Sync Platform Topology, Ask, and Complex Entries
  for (Index = 0; Index < MpioData->MpioAskCount; Index++) {
    if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
      IDS_HDT_CONSOLE (GNB_TRACE, "  Training State for Device %d Function %d is 0x%x\n",
                       AskEntry->desc.link_attributes.devfuncnumber >> 3,
                       AskEntry->desc.link_attributes.devfuncnumber & 0x7,
                       AskEntry->status.state
                       );
      Engine = MpioFindEngineForAsk (GnbHandle, AskEntry);
      if (Engine != NULL) {
        if (AskEntry->status.state == LINK_TRAINED) {
          Engine->InitStatus = INIT_STATUS_PCIE_TRAINING_SUCCESS;

          // PCIE port autentication after training
          MpioPcieAuthenticationAfterTraining (Engine);

          IDS_HDT_CONSOLE(
            GNB_TRACE, "%a Device is on Socket %d StartLane %d Endlane %d Device %d Function %d\n",
            (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS)? "Successful": "Authentication Failed",
            GnbHandle->SocketId,
            Engine->EngineData.StartLane,
            Engine->EngineData.EndLane,
            Engine->Type.Port.PortData.DeviceNumber,
            Engine->Type.Port.PortData.FunctionNumber
            );
        } else {
          if (Engine->Type.Port.PortData.PortPresent && PcdGet8 (PcdAmdCfgExposeUnusedPciePorts) != 0) {
            if (PcdGet8 (PcdAmdCfgExposeUnusedPciePorts) == 1 || Engine->Type.Port.AlwaysExpose == 1) {
              Engine->InitStatus = INIT_STATUS_PCIE_PORT_ALWAYS_EXPOSE;
            }
          }
          IDS_HDT_CONSOLE (GNB_TRACE, "  linkTrainingState indicates FAILURE\n");
        }
      }
    }
    AskEntry++;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "%a : Exit\n", __FUNCTION__);
  return;
}



