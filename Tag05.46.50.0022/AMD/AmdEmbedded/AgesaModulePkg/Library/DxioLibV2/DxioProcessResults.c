/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
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
#include <Library/GnbLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/NbioRegisterAccLib.h>
#include <Library/DxioLibV2.h>
#include <Library/OemGpioResetControlLib.h>
#include "DxioLibLocal.h"
#include <Library/TimerLib.h>

#define FILECODE LIBRARY_DXIOLIBV2_DXIOPROCESSRESULTS_FILECODE
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

VOID
DxioSleep (
  IN     GNB_HANDLE     *GnbHandle,
  IN     UINT32         Milliseconds
  );

AGESA_STATUS
DxioProcessResults (
  IN       GNB_HANDLE                      *GnbHandle,
  IN       DXIO_COMPLEX_DESCRIPTOR         *UserConfig,
  IN       DXIO_PLATFORM_DESCRIPTOR        *ThisNode,
  IN       UINT32                          ThisNodeSize,
  IN       EARLY_TRAIN_DATA                *EarlyTrainData,
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi
  )
{
  AGESA_STATUS              AgesaStatus;
  UINT32                    SmuArg[6];
  UINT8                     DxioRetval;
  BOOLEAN                   DxioComplete;
  DXIO_RETURN_PARAMS        *ReturnArgs;
  LISM_RETURN_PARAMS        *LismRetParams;
  RESET_REQUEST_PARAMS      *ResetReqParams;
  GPIO_RESET_INFO           GpioResetInfo;
  UINT16                    ResetDelay;

  // Process results
  AgesaStatus = AGESA_SUCCESS;
  ReturnArgs = (DXIO_RETURN_PARAMS*) &SmuArg[0];
  DxioComplete = FALSE;
  ResetDelay = PcdGet16 (PcdAfterResetDelay);

  do {
    LibAmdMemFill (SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *) NULL);
    SmuServicesPpi->SmuDxioServiceRequest (SmuServicesPpi, GnbHandle->SocketId, DXIO_MSG_LISM_COLLECTRESULTS, SmuArg, SmuArg);

    DxioRetval = (UINT8) (SmuArg[0] & 0xFF);
    switch (DxioRetval) {
    case DXIO_MBOX_RETVAL_OK:
      switch (ReturnArgs->RetParams_Type) {
      case DXIO_MBOX_RETPARAMS_LISM:
        LismRetParams = (LISM_RETURN_PARAMS*) &SmuArg[2];
        IDS_HDT_CONSOLE (GNB_TRACE, "Current State = 0x%x\n", LismRetParams->CurrentState);
        switch (LismRetParams->CurrentState) {
        case DXIO_LinkInitState_done:
            DxioComplete = TRUE;
            break;
        case DXIO_LinkInitState_mapped:
          // Ports are mapped.  Allow pre-configuration options to be programmed.
          IDS_HDT_CONSOLE (GNB_TRACE, "PORTS ARE MAPPED\n");
          DxioLibServiceRequest (SmuServicesPpi, GnbHandle->SocketId, DXIO_MSG_INIT_DMAENGINECONIFGTODRAM, 0, (UINT32) ThisNode, ThisNodeSize, 0, 0);
          DxioPortMapping (GnbHandle,
                           UserConfig,
                           ThisNode,
                           (DXIO_ENGINE_DESCRIPTOR**)&(EarlyTrainData->EarlyTrainDesc),
                           &(EarlyTrainData->EarlyTrainAddress.AddressValue),
                           (PCIe_ENGINE_CONFIG **) &(EarlyTrainData->EarlyTrainEngine)
                           );
          DxioCfgBeforeReconfig (GnbHandle);
          break;
        case DXIO_LinkInitState_configured:
          // Configuration is done.  Allow post-configuration, pre-training options to be programmed.
          IDS_HDT_CONSOLE (GNB_TRACE, "RECONFIG IS COMPLETE\n");
          DxioCfgAfterReconfig (GnbHandle);
          DxioComplete = TRUE;
          break;
        case DXIO_LinkInitState_earlyTrained:
          // Read back training results
          IDS_HDT_CONSOLE (GNB_TRACE, "EarlyTrained state detected...\n");
          if (EarlyTrainData->EarlyTrainDesc == NULL) {
            IDS_HDT_CONSOLE (GNB_TRACE, "EarlyTrainLink NOT FOUND\n");
          } else {
            AGESA_TESTPOINT (TpEarlyTrained, NULL);
            IDS_HDT_CONSOLE (GNB_TRACE, "\nEarlyTrainLink found at 0x%x\n", EarlyTrainData->EarlyTrainAddress);
            EarlyTrainData->EarlyLinkMapped = TRUE;
            EarlyTrainData->EarlyTrainAddress.AddressValue = EarlyTrainData->EarlyTrainAddress.AddressValue << 12;
            if (EarlyTrainData->EarlyTrainEngine != NULL) {
              DxioProcessEarlyTrain (GnbHandle, ThisNode, ThisNodeSize, EarlyTrainData, SmuServicesPpi);
            }
          }
          break;
        case DXIO_LinkInitState_vetting:
          IDS_HDT_CONSOLE (GNB_TRACE, "Vetting state detected...\n");
          break;
        default:
          break;
        }
        break;
      case DXIO_MBOX_RETPARAMS_REQ_RESET:
        // Issue reset request for specific PCIe Engine
        ResetReqParams = (RESET_REQUEST_PARAMS*) &SmuArg[2];
        GpioResetInfo.ResetId = ResetReqParams->GpioId;
        GpioResetInfo.ResetControl = ResetReqParams->resetAssertDeassert;
        if (ResetReqParams->resetAssertDeassert == 0) {
          AgesaGpioSlotResetControl ((UINTN) GnbHandle->Address.Address.Bus, &GpioResetInfo);
          IDS_HDT_CONSOLE (GNB_TRACE, "Reset Assert Request for GpioId 0x%x\n", ResetReqParams->GpioId);
        } else {
          AgesaGpioSlotResetControl ((UINTN) GnbHandle->Address.Address.Bus, &GpioResetInfo);
          if (ResetDelay != 0) {
            IDS_HDT_CONSOLE (GNB_TRACE, "Reset delay %dmS\n", PcdGet16 (PcdAfterResetDelay));
            MicroSecondDelay (PcdGet16 (PcdAfterResetDelay) * 1000);
            ResetDelay = 0;
          }
          IDS_HDT_CONSOLE (GNB_TRACE, "Reset Deassert Request for GpioId 0x%x\n", ResetReqParams->GpioId);
        }
        break;
      case DXIO_MBOX_RETPARAMS_NONE:
        DxioComplete = TRUE;
        break;
      case DXIO_MBOX_RETPARAMS_GENERIC:
      default:
        IDS_HDT_CONSOLE (GNB_TRACE, "Invalid return parameter [0x%x]\n", DxioRetval);
        ASSERT (FALSE);
        break;
      }
      break;
    case DXIO_MBOX_RETVAL_ERROR:
      switch (ReturnArgs->RetParams_Type) {
      case DXIO_MBOX_RETPARAMS_LISM:
        LismRetParams = (LISM_RETURN_PARAMS*) &SmuArg[2];
        IDS_HDT_CONSOLE (GNB_TRACE, "Current State = 0x%x", LismRetParams->CurrentState);
        AgesaStatus = AGESA_CRITICAL;
        DxioComplete = TRUE;
        break;
      default:
        ASSERT (FALSE);
        break;
      }
      break;
    default:
      IDS_HDT_CONSOLE (GNB_TRACE, "DXIO Mailbox Request did not return OK [0x%x]\n", DxioRetval);
      ASSERT (FALSE);
      DxioComplete = TRUE;
      break;
    }
    if (FALSE == DxioComplete) {
      LibAmdMemFill (SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *) NULL);
      SmuServicesPpi->SmuDxioServiceRequest (SmuServicesPpi, GnbHandle->SocketId, DXIO_MSG_LISM_RESUME, SmuArg, SmuArg);
    }
  } while (FALSE == DxioComplete);
  if ((EarlyTrainData->EarlyLinkMapped == TRUE) && (EarlyTrainData->EarlyTrainDesc != NULL)) {
    GnbLibPciRMW (EarlyTrainData->EarlyTrainAddress.AddressValue | 0x18, AccessWidth32, 0xFF0000FF, 0x000000, (AMD_CONFIG_PARAMS *) NULL);
  }
  return  AgesaStatus;
}




