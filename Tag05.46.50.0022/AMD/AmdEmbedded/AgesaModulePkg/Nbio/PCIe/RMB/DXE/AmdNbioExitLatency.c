/*
*****************************************************************************
*
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <GnbDxio.h>
#include <Filecode.h>
#include <Library/IdsLib.h>
#include <Library/GnbPcieConfigLib.h>
#include <Library/GnbCommonLib.h>
#include <Library/NbioHandleLib.h>
#include <GnbRegistersRMB.h>
#include <Library/AmdBaseLib.h>
#include <Library/PcieMiscCommLib.h>
#include "PcieStraps.h"

#define FILECODE NBIO_PCIE_RMB_DXE_AMDNBIOEXITLATENCY_FILECODE

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

/**----------------------------------------------------------------------------------------*/
/**
 * Interface to set L1 Exit Latency
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 *
 */
 /*----------------------------------------------------------------------------------------*/
VOID
PcieL1ExitLatencyInterface (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  UINT16                 L1ExitLatency;
  PCIe_ENGINE_CONFIG     *PcieEngine;
  PCIe_WRAPPER_CONFIG    *PcieWrapper;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieL1ExitLatencyInterface Enter\n");
  L1ExitLatency = 0xFFFF;
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine) && PcieConfigCheckPortStatus (PcieEngine, INIT_STATUS_PCIE_TRAINING_SUCCESS)) {
        if (TRUE == PcieLinkL1ExitLatencyWA (PcieEngine, (UINT16*)&L1ExitLatency)) {
          if (L1ExitLatency != 0xFFFF) {
            IDS_HDT_CONSOLE (GNB_TRACE, "  set L1ExitLatency = %d to Device = %d:%d:%d\n",
                              L1ExitLatency,
                              PcieEngine->Type.Port.Address.Address.Bus,
                              PcieEngine->Type.Port.Address.Address.Device,
                              PcieEngine->Type.Port.Address.Address.Function
                              );
            WritePcieStrap (
              GnbHandle,
              PORT_STRAP_INDEX(STRAP_BIF_L1_EXIT_LATENCY_A_INDEX, PcieEngine->Type.Port.PortId),
              L1ExitLatency,
              PcieWrapper->WrapId
              );
            IDS_HDT_CONSOLE (GNB_TRACE, "   - writing strap 0x%x - %d\n", PORT_STRAP_INDEX(STRAP_BIF_L1_EXIT_LATENCY_A_INDEX, PcieEngine->Type.Port.PortId), L1ExitLatency);
          }
        }
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieL1ExitLatencyInterface Exit\n");
}


