/*****************************************************************************
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
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/NbioRegisterAccLib.h>
#include <Library/DxioLibV2.h>
#include "DxioLibLocal.h"

#define FILECODE LIBRARY_DXIOLIBV2_DXIOLIBV2_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define   ZP_PLATFORM_TYPE  0
#define   MAX_INSTANCE_ID   8

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
 * Set PCIe training timers
 *
 *
 *
 * @param[in]  SmuServicesPpi  Pointer to SMU Services PPI or similar entry point structure
 * @param[in]  InstanceId      The unique identifier of the NBIO instance associated with this socket/die
 */

VOID
DxioSetPCIeTrainingTimers (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi,
  IN       UINT32                          InstanceId
)
{
  UINT32 EngineType;
  UINT32 LinkResetToTrainingTime;
  UINT32 LinkReceiverDetectionPolling;
  UINT32 LinkL0Polling;

  EngineType = DxioPcieEngine;
  LinkResetToTrainingTime = 0;
  LinkReceiverDetectionPolling = 0;
  LinkL0Polling = 0;
  if (SetPCIeTrainingTimers(&LinkResetToTrainingTime, &LinkReceiverDetectionPolling, &LinkL0Polling)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "SetPCIeTrainingTimers: LinkResetToTrainingTime = %d LinkReceiverDetectionPolling = %d LinkL0Polling = %d\n", LinkResetToTrainingTime, LinkReceiverDetectionPolling, LinkL0Polling);
    DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_RNTM_MBOXROUTINES, EngineType, 2, LinkResetToTrainingTime, LinkReceiverDetectionPolling, LinkL0Polling);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * DXIO Early Initialization
 *
 *
 *
 * @param[in]  PeiServices     Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  SmuServicesPpi  Pointer to SMU Services PPI or similar entry point structure
 * @param[in]  Pcie            Pointer to the AGESA platfom complex
 * @param[in]  UserConfig      Pointer to the platform BIOS supplied platform configuration
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
DxioEarlyInitV2 (
  IN      CONST EFI_PEI_SERVICES          **PeiServices,
  IN      PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi,
  IN      PCIe_PLATFORM_CONFIG            *Pcie,
  IN      GNB_HANDLE                      *StartHandle,
  IN      DXIO_COMPLEX_DESCRIPTOR         *UserConfig
  )
{
  AGESA_STATUS              AgesaStatus;
  AGESA_STATUS              Status;
  GNB_HANDLE                *GnbHandle;
  UINT32                    SmuArg[6];
  UINT8                     InstanceId;
  UINT8                     DxioRetval;
  DXIO_PLATFORM_DESCRIPTOR  *PlatformDesc[MAX_INSTANCE_ID];
  UINT32                    PlatformDescSize[MAX_INSTANCE_ID];
  UINT8                     *AncillaryData[MAX_INSTANCE_ID];
  UINT32                    AncillaryDataSize[MAX_INSTANCE_ID];
  EARLY_TRAIN_DATA          EarlyTrainData;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a : Enter\n", __FUNCTION__);

  AgesaStatus = AGESA_SUCCESS;

  //Test/Debug implementation
  DxioRetval = DXIO_MBOX_RETVAL_NULL;
  GnbHandle = StartHandle;

  // Initialize EarlyTrain data
  EarlyTrainData.EarlyLinkMapped = FALSE;
  EarlyTrainData.EarlyTrainAddress.AddressValue = 0;
  EarlyTrainData.EarlyTrainDesc = NULL;
  EarlyTrainData.EarlyTrainEngine = NULL;

  InstanceId = 0xFF;
  while (GnbHandle != NULL) {
    InstanceId = GnbHandle->SocketId;
    DxioParseUserConfig (PeiServices,
                         GnbHandle,
                         UserConfig,
                         Pcie,
                         &PlatformDesc[InstanceId],
                         &PlatformDescSize[InstanceId],
                         &AncillaryData[InstanceId],
                         &AncillaryDataSize[InstanceId]
                         );

    if (PlatformDesc[InstanceId] != NULL) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Platform Descriptor for Die %d\n", InstanceId);
      GnbHandle->NumEngineDesc = (UINT8)PlatformDesc[InstanceId]->NumEngineDesc;
      DxioDebugDump (PlatformDesc[InstanceId],
                     PlatformDescSize[InstanceId],
                     AncillaryData[InstanceId],
                     AncillaryDataSize[InstanceId]);

      if (InstanceId == UserConfig->BmcLinkLocation) {
        DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_LISM_RELOAD, \
            (UserConfig->BmcLinkLaneNum < 128) ? 0 : 1 \
            , 0, 0, 0, 0);
      }

      LibAmdMemFill (SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *) NULL);
      SmuServicesPpi->SmuDxioServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_GETVERSION, SmuArg, SmuArg);

      PcdSet32S (PcdDxioMajorRevision, SmuArg[2]);
      PcdSet32S (PcdDxioMinorRevision, SmuArg[3]);

      LibAmdMemFill (SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *) NULL);
      Status = SmuServicesPpi->SmuDxioServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_INIT_INIT, SmuArg, SmuArg);
      if (!EFI_ERROR(Status)) {
        DxioRetval = (UINT8) (SmuArg[0] & 0xFF);
        if (0 == PcdGet32 (PcdDxioMinorRevision)) {
          DxioRetval = DXIO_MBOX_RETVAL_OK;
          IDS_HDT_CONSOLE (GNB_TRACE, "SMU simulation is not enabled%d\n", InstanceId);
        }
      }
      if (DxioRetval == DXIO_MBOX_RETVAL_OK) {

        DxioCfgAfterInitInit (GnbHandle, SmuServicesPpi, InstanceId);

        DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_SETSCRATCH, DBUG_SETSCRATCHFLAG_RETURNAFTERMAPPING, 1, 0, 0, 0);
        DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_SETSCRATCH, DBUG_SETSCRATCHFLAG_RETURNAFTERCONFIG, 1, 0, 0, 0);
        DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_SETSCRATCH, DBUG_SETSCRATCHFLAG_V1ANCILLARYDATA, 1, 0, 0, 0);
        if (PcdGetBool(PcdPCIeExactMatchEnable)) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_MATCHPCIEPORTSIZEEXACTLY, 1, 0, 0, 0);
        }
        DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_VALIDPHYFWFLAG, PcdGet8 (PcdDxioPhyValid), 0, 0, 0);
        DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_USEPHYSRAMFLAG, PcdGet8 (PcdDxioPhyProgramming), 0, 0, 0);

        if (PcdGet8 (PcdCfgSkipPspMessage) == 0) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_SETSCRATCH, DBUG_SETSCRATCHFLAG_SKIPPSPMESSAGES, 0, 0, 0, 0);
        } else {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_SETSCRATCH, DBUG_SETSCRATCHFLAG_SKIPPSPMESSAGES, 1, 0, 0, 0);
        }

        if (PcdGet8 (PcdDxioSaveRestoreModes) != 0xFF) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_DBUG_SETSCRATCH, DBUG_SETSCRATCHFLAG_SAVERESTOREMODE, PcdGet8 (PcdDxioSaveRestoreModes), 0, 0, 0);
        }

        DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_INIT_LOADCAPABILITIES, 0, 0, 0, 0, 0);

        if (AncillaryDataSize[InstanceId] != 0) {
          DxioRetval = DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_INIT_LOADHEAP, 0, (UINT32) AncillaryData[InstanceId], AncillaryDataSize[InstanceId], 0, DXIO_HEAPTYPE_ANCDATA);
          ASSERT (DXIO_MBOX_RETVAL_OK == DxioRetval);
        }
        DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_INIT_LOADHEAP, 0, 0, 0, 1, DXIO_HEAPTYPE_MACPCS);
        DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_INIT_LOADHEAP, 0, 0, 0, 1, DXIO_HEAPTYPE_GPIOPTR);
        DxioRetval = DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_INIT_LOADHEAP, 0, (UINT32) PlatformDesc[InstanceId], PlatformDescSize[InstanceId], 0, DXIO_HEAPTYPE_CFGPTR);
        ASSERT (DXIO_MBOX_RETVAL_OK == DxioRetval);

        DxioSetPCIeTrainingTimers (SmuServicesPpi, InstanceId);
        if (PcdGet8 (PcdCfgDxioWA) == 0x1) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_RNTM_MBOXROUTINES, DxioPcieEngine, 0, 3, 1, 0);
        }
        if (PcdGet8 (PcdCfgDxioSPCWA) == 0x1) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_RNTM_MBOXROUTINES, DxioPcieEngine, 0, 4, 1, 0);
        }
        if (PcdGet8 (PcdCfgDxioFCCreditWADisable) == 0x1) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_RNTM_MBOXROUTINES, DxioPcieEngine, 0, 5, 1, 0);
        }
        if (PcdGetBool (PcdCfgDxioTXClockGating) == FALSE) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_RNTM_MBOXROUTINES, DxioPcieEngine, 0, 7, 0, 0);
        }
        if (PcdGet8 (PcdSrisEnableMode) == 0x1) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_RNTM_MBOXROUTINES, DxioPcieEngine, 0, 8, 1, 0);
        }

        if (PcdGet8 (PcdCfgDxioTxFIFOMode) == 0x1) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_RNTM_MBOXROUTINES, DxioPcieEngine, 0, 9, 1, 0);
        } else {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_RNTM_MBOXROUTINES, DxioPcieEngine, 0, 9, 0, 0);
        }

         if (PcdGetBool (PcdCfgDxioEnableCorrectableSRAMECCReporing) == TRUE){
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_RNTM_MBOXROUTINES, DxioPcieEngine, 0, 11, 1, 0);
        }

        if (PcdGetBool (PcdCfgDxioAllowCompPass) == TRUE) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PCIE_ALLOW_COMPLETION_PASS, 1, 0, 0, 0);
        } else {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PCIE_ALLOW_COMPLETION_PASS, 0, 0, 0, 0);
        }

        SmuArg[1] = PcdGet32 (PcdGen3InitRxParam1);
        SmuArg[2] = PcdGet32 (PcdGen3InitRxParam2);
        if (SmuArg[1] != 0x0) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_GEN3_INIT_RX_SETTING, SmuArg[1], SmuArg[2], 0, 0);
        }
        SmuArg[1] = PcdGet32 (PcdGen4InitRxParam1);
        SmuArg[2] = PcdGet32 (PcdGen4InitRxParam2);
        if (SmuArg[1] != 0x0) {
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_E_MBOXMSG_DBUG_SETSCRATCH, DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_GEN4_INIT_RX_SETTING, SmuArg[1], SmuArg[2], 0, 0);
        }

        // Inititiate configuration
        DxioRetval = DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_LISM_GO, 0, 0, 0, 0, 0);

        // Process results
        Status = DxioProcessResults (GnbHandle, UserConfig, PlatformDesc[InstanceId], PlatformDescSize[InstanceId], &EarlyTrainData, SmuServicesPpi);
        AGESA_STATUS_UPDATE (Status, AgesaStatus);
      }
    }
    do {
      GnbHandle = GnbGetNextHandle(GnbHandle);
    } while ((GnbHandle != NULL) && (InstanceId == GnbHandle->SocketId));
  }

  if (DxioRetval == DXIO_MBOX_RETVAL_OK) {
    GnbHandle = StartHandle;
    InstanceId = 0xFF;
    while (GnbHandle != NULL) {
      InstanceId = GnbHandle->SocketId;
      if (PlatformDesc[InstanceId] != NULL) {
        DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_LISM_RESUME, 0, 0, 0, 0, 0);
        // Process results
        Status = DxioProcessResults (GnbHandle, UserConfig, PlatformDesc[InstanceId], PlatformDescSize[InstanceId], &EarlyTrainData, SmuServicesPpi);
        AGESA_STATUS_UPDATE (Status, AgesaStatus);

        // Read back training results
        if (0 != PcdGet32 (PcdDxioMinorRevision)) {
          LibAmdMemFill ((VOID *)(PlatformDesc[InstanceId]), 0x00, PlatformDescSize[InstanceId] << 2, (AMD_CONFIG_PARAMS *) NULL);
          DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_INIT_DMAENGINECONIFGTODRAM, 0, (UINT32) PlatformDesc[InstanceId], PlatformDescSize[InstanceId], 0, 0);
        }
        DxioUpdatePortTrainingStatus (GnbHandle, UserConfig, PlatformDesc[InstanceId]);
      }
      do {
        GnbHandle = GnbGetNextHandle(GnbHandle);
      } while ((GnbHandle != NULL) && (InstanceId == GnbHandle->SocketId));
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a : Exit [0x%x]\n", __FUNCTION__, AgesaStatus);
  return  AgesaStatus;
}


/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Port Speed Update Request
 *
 *
 *
 * @param[in]  PeiServices     Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  SmuServicesPpi  Pointer to SMU Services PPI or similar entry point structure
 * @param[in]  InstanceId      The unique identifier of the NBIO instance associated with this socket/die
 * @param[in]  EngineId        PCIe bridge ID
 * @param[in]  TargetSpeed     Desired speed of the target port (Gen1, Gen2, Gen3)
 */

VOID
DxioSetLinkSpeedV2 (
  IN       CONST EFI_PEI_SERVICES          **PeiServices,
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi,
  IN       UINT32                          InstanceId,
  IN       UINT32                          EngineId,
  IN       UINT32                          TargetSpeed
  )
{
  DxioLibServiceRequest (SmuServicesPpi, InstanceId, DXIO_MSG_PHYS_TARGETFREQUPDATE | (DXIO_freqUpdType_single << 10), DxioPcieEngine, TargetSpeed, EngineId, 0, 0);
}



