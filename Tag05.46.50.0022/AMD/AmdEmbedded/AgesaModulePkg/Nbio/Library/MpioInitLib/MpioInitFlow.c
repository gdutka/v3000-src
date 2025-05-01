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
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/NbioRegisterAccLib.h>
#include <MpioLib.h>
#include <Library/MpioInitLib.h>
#include "MpioLibLocal.h"
#include <Library/OemGpioResetControlLib.h>
#include <Library/DxioLibV2.h>
#include <Library/TimerLib.h>
#include <Ppi/PcieAuthenticationPpi.h>
#include <Library/PeiServicesTablePointerLib.h>

#define FILECODE NBIO_LIBRARY_MPIOINITLIB_MPIOINITFLOW_FILECODE
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
 * Wait for MPIO ready to process a message
 *
 *
 *
 * @param[in]  GnbHandle  Pointer to the silicon descriptor for this NBIO
 * @param[in]  MpioData   Pointer to the ASK structure descriptor for this Instance
 */

STATIC
VOID
WaitReady (
  IN        GNB_HANDLE    *GnbHandle
)
{
  UINT32              MpioArg[6];
  GET_STATUS_RESULTS  *ArgPtr;

  IDS_HDT_CONSOLE (GNB_TRACE, "Wait for MPIO ready...\n");
  do {
    NbioMpioServiceCommonInitArguments (MpioArg);
    ArgPtr = (GET_STATUS_RESULTS *) MpioArg;
    MpioServiceRequest (GnbHandle->Address, MPIO_GET_STATUS, MpioArg, 0);
  } while (ArgPtr->CmdStatus  != 0);
  IDS_HDT_CONSOLE (GNB_TRACE,
    "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );
  return;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Transfer FW_ASK_STRUCT to MPIO
 *
 *
 *
 * @param[in]  GnbHandle  Pointer to the silicon descriptor for this NBIO
 * @param[in]  MpioData   Pointer to the ASK structure descriptor for this Instance
 */

STATIC
VOID
SendAsk (
  IN        GNB_HANDLE    *GnbHandle,
  IN        MPIO_DATA     *MpioData
)
{
  UINT32              MpioArg[6];
  TRANSFER_ASK_ARGS*  ArgPtr;

  WaitReady (GnbHandle);
  IDS_HDT_CONSOLE (GNB_TRACE,
                   "MpioData at 0x%x\n -- MpioAsk = 0x%x\n -- MpioAskCount = %d\n",
                   (UINT32) MpioData,
                   (UINT32) MpioData->MpioAsk,
                   MpioData->MpioAskCount
                   );

  NbioMpioServiceCommonInitArguments (MpioArg);
  ArgPtr = (TRANSFER_ASK_ARGS *) MpioArg;
  ArgPtr->DramAddressLo = (UINT32) MpioData->MpioAsk;
  ArgPtr->DramAddressHi = 0;
  ArgPtr->Links = SELECTED_LINKS;
  ArgPtr->Direction = DRAM_TO_MPIO;
  ArgPtr->LinkCount = MpioData->MpioAskCount;
  IDS_HDT_CONSOLE (GNB_TRACE,
    "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );
  MpioServiceRequest (GnbHandle->Address, MPIO_TRANSFER_ASK, MpioArg, 0);
  IDS_HDT_CONSOLE (GNB_TRACE,
    "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );
  if (MpioArg[0] != 1) {
    IDS_HDT_CONSOLE (GNB_TRACE, "MPIO firmware rejected the Ask structure!");
    ASSERT (FALSE);
  }
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Transfer FW_ASK_STRUCT to MPIO
 *
 *
 *
 * @param[in]  GnbHandle  Pointer to the silicon descriptor for this NBIO
 * @param[in]  MpioData   Pointer to the ASK structure descriptor for this Instance
 */

STATIC
UINT32
GetAsk (
  IN        GNB_HANDLE    *GnbHandle,
  IN        MPIO_DATA     *MpioData
)
{
  UINT32                MpioArg[6];
  GET_ASK_RESULT_ARGS*  ArgPtr;

  WaitReady (GnbHandle);
  NbioMpioServiceCommonInitArguments (MpioArg);
  ArgPtr = (VOID *) MpioArg;
  ArgPtr->DestAddressLo = (UINT32) MpioData->MpioAsk;
  ArgPtr->DestAddressHi = 0;
  ArgPtr->LinksOnly = ALL_LINKS;
  IDS_HDT_CONSOLE (GNB_TRACE,
    "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );
  MpioServiceRequest (GnbHandle->Address, MPIO_GET_ASK_RESULT, MpioArg, 0);
  IDS_HDT_CONSOLE (GNB_TRACE,
    "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
    MpioArg[0],
    MpioArg[1],
    MpioArg[2],
    MpioArg[3],
    MpioArg[4],
    MpioArg[5]
    );

  return (MpioArg[0]);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set Global Config Defaults
 *
 *
 *
 * @param[in]  SmuServicesPpi  Pointer to SMU Services PPI or similar entry point structure
 * @param[in]  InstanceId      The unique identifier of the NBIO instance associated with this socket/die
 */

VOID
MpioSetGlobalConfigDefaults (
  IN OUT    UINT32    *ArgList
)
{
  MPIO_GLOBAL_CONFIG    *GlobalConfig;

  GlobalConfig = (MPIO_GLOBAL_CONFIG *) ArgList;

  // DWORD 0 - General
  GlobalConfig->skipVetting = 0;
  GlobalConfig->ntbhotpluginterval = 0;
  GlobalConfig->saverestoremode = 0;
  GlobalConfig->matchPortSizeExactly = 0;
  GlobalConfig->skipLinkDisableOnFailure = 0;
  GlobalConfig->usePhySram = 1;
  GlobalConfig->ValidPhyFWFlag = 1;

  // DWORD 1 - Power Management
  GlobalConfig->PWRMNGMT_PRFRM_CLK_GATING = 0;
  GlobalConfig->PWRMNGMT_PRFRM_STATIC_PWR_GATING = 0;
  GlobalConfig->PWRMNGMT_PRFRM_REFCLK_SHUTDOWN = 0;
  GlobalConfig->CBSOPTIONS_ENABLEPOWERMANAGEMENT = 0;
  GlobalConfig->PWRMNGMT_PRFRM_PMA_POWER_GATING = 0;
  GlobalConfig->PWRMNGMT_PRFRM_PMA_CLOCK_GATING = 0;

  // DWORD 2  - PCIE Link Timeouts (in msec)
  GlobalConfig->LinkReceiverDetectionPolling = 0;
  GlobalConfig->LinkResetToTrainingTime = 0;
  GlobalConfig->LinkL0Polling = 0;

  // DWORD 3 - Protocol
  GlobalConfig->PCIE_ALLOW_COMPLETION_PASS = 0;
  GlobalConfig->CBSOPTIONS_ALLOWPOINTERSLIPINTERVAL = 0;

  // DWORD 4 - PCIE PERST
  GlobalConfig->GPIO26_GENERIC_RST = 0;
  GlobalConfig->GPIO40_NVME_RST = 0;
  GlobalConfig->GLOBAL_RST = 0;

  return;

}

/*----------------------------------------------------------------------------------------*/
/**
 * Mpio Port Authentication before Training
 *
 * @param[in]  GnbHandle              Pointer to GnbHandle pointer
 * @param[in]  PlatformTopology       Pointer to the platform BIOS supplied platform configuration
 * @param[in]  MpioData               Pointer to the platform descriptor for this node
 *
 */
VOID
MpioPcieAuthenticationBeforeTraining (
  IN      GNB_HANDLE                *GnbHandle,
  IN      DXIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  IN      MPIO_DATA                 *MpioData
)
{
  EFI_STATUS                      Status;
  AGESA_STATUS                    AgesaStatus;
  CONST EFI_PEI_SERVICES          **PeiServices;
  PEI_AMD_PCIE_AUTHENTICATION_PPI *AmdPcieAuthentication;
  UINT8                           Index;
  FW_ASK_STRUCT                   *AskEntry;
  DXIO_PORT_DESCRIPTOR            *SocketTopology;
  PCIe_ENGINE_CONFIG              *Engine;

  SocketTopology = NULL;
  Engine = NULL;

  PeiServices = GetPeiServicesTablePointer ();
  Status = (**PeiServices).LocatePpi (PeiServices, &gAmdPcieAuthenticationGuid, 0, NULL, (VOID **)&AmdPcieAuthentication);
  if (EFI_ERROR(Status)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Non-support PCIE Authentication\n");
    return;
  }

  AgesaStatus = MpioGetTopologyForSocket (GnbHandle->SocketId, PlatformTopology, &SocketTopology);
  if (AgesaStatus != AGESA_SUCCESS) {
    return;
  }

  AskEntry = MpioData->MpioAsk;

  for (Index = 0; Index < MpioData->MpioAskCount; Index++) {
    if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
      Engine = MpioFindEngineForAsk (GnbHandle, AskEntry);
      if (Engine != NULL) {
        PORT_AUTHENTICATION_DESC        PortAuth;
        BOOLEAN                         Authenticated;

        PortAuth.PciLocation.AsBits.Segment = Engine->Type.Port.Address.Address.Segment;
        PortAuth.PciLocation.AsBits.Bus = Engine->Type.Port.Address.Address.Bus;
        PortAuth.PciLocation.AsBits.Device = Engine->Type.Port.PortData.DeviceNumber;
        PortAuth.PciLocation.AsBits.Function = Engine->Type.Port.PortData.FunctionNumber;
        PortAuth.StartLaneIndex = Engine->EngineData.StartLane;
        PortAuth.EndLaneIndex = Engine->EngineData.EndLane;
        Authenticated = FALSE;

        Status = AmdPcieAuthentication->PcieAuthenticationBeforeTraining (&PortAuth, &Authenticated);
        if (!EFI_ERROR(Status)) {
          if (!Authenticated) {
            AskEntry->desc.link_attributes.portPresent = 0;
            IDS_HDT_CONSOLE (GNB_TRACE, "MpioPortAuthentication = FALSE\n");
          } else {
            IDS_HDT_CONSOLE (GNB_TRACE, "MpioPortAuthentication = TRUE\n");
          }
        }
      }
    }
    AskEntry++;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Mpio Early Initialization
 *
 *
 *
 * @param[in]  PeiServices     Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  SmuServicesPpi  Pointer to SMU Services PPI or similar entry point structure
 * @param[in]  Pcie            Pointer to the AGESA platfom complex
 * @param[in]  PlatformTopology      Pointer to the platform BIOS supplied platform configuration
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
MpioEarlyInitV1 (
  IN      PCIe_PLATFORM_CONFIG            *Pcie,
  IN      GNB_HANDLE                      *StartHandle,
  IN      DXIO_COMPLEX_DESCRIPTOR         *PlatformTopology
  )
{
  AGESA_STATUS              AgesaStatus;
//  AGESA_STATUS              Status;
  GNB_HANDLE                *GnbHandle;
  UINT32                    MpioArg[6];
  VOID                      *ArgPtr;
  UINT8                     InstanceId;
  MPIO_DATA                 MpioData[MAX_INSTANCE_ID];
  GPIO_RESET_INFO           GpioResetInfo;
  UINT16                    ResetDelay;
//@todo  EARLY_TRAIN_DATA          EarlyTrainData;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a : Enter\n", __FUNCTION__);

  AgesaStatus = AGESA_SUCCESS;

  //Test/Debug implementation
  GnbHandle = StartHandle;

  InstanceId = 0xFF;
  while (GnbHandle != NULL) {
    InstanceId = GnbHandle->SocketId;
    MpioParsePlatformTopology (GnbHandle,
                               PlatformTopology,
                               &MpioData[InstanceId]
                               );

    IDS_HDT_CONSOLE (GNB_TRACE,
                     "MpioData at 0x%x\n -- MpioAsk = 0x%x\n -- MpioAskCount = %d\n",
                     (UINT32) &(MpioData[InstanceId]),
                     (UINT32) (MpioData[InstanceId].MpioAsk),
                     MpioData[InstanceId].MpioAskCount
                     );
    if (MpioData[InstanceId].MpioAsk != NULL) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Platform Descriptor for Instance %d\n", InstanceId);
      GnbHandle->NumEngineDesc = (UINT8)MpioData[InstanceId].MpioAskCount;
      MpioDebugDump (&(MpioData[InstanceId]));

      // Set Global Config;
      NbioMpioServiceCommonInitArguments (MpioArg);
      MpioSetGlobalConfigDefaults (MpioArg);
      MpioCfgGlobalConfig (GnbHandle, MpioArg);
      IDS_HDT_CONSOLE (GNB_TRACE,
        "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      MpioServiceRequest (GnbHandle->Address, MPIO_SET_GLOBAL_CONFIG, MpioArg, 0);
      IDS_HDT_CONSOLE (GNB_TRACE,
        "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );

      SendAsk (GnbHandle, &MpioData[InstanceId]);

      NbioMpioServiceCommonInitArguments (MpioArg);
      ArgPtr = (VOID *) MpioArg;
      ((SETUP_LINK_ARGS *) ArgPtr)->Map = 1;
      IDS_HDT_CONSOLE (GNB_TRACE,
                       "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
                       MpioArg[0],
                       MpioArg[1],
                       MpioArg[2],
                       MpioArg[3],
                       MpioArg[4],
                       MpioArg[5]
                       );
      MpioServiceRequest (GnbHandle->Address, POSTED_MSG (MPIO_SETUP_LINK), MpioArg, 0);
      GetAsk (GnbHandle, &MpioData[InstanceId]);
      MpioPortMapping (GnbHandle, PlatformTopology, & MpioData[InstanceId]);
      MpioCfgBeforeReconfig (GnbHandle);

      NbioMpioServiceCommonInitArguments (MpioArg);
      ArgPtr = (VOID *) MpioArg;
      ((SETUP_LINK_ARGS *) ArgPtr)->Configure = 1;
      ((SETUP_LINK_ARGS *) ArgPtr)->Reconfigure = 1;
      IDS_HDT_CONSOLE (GNB_TRACE,
        "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      MpioServiceRequest (GnbHandle->Address, POSTED_MSG (MPIO_SETUP_LINK), MpioArg, 0);
      IDS_HDT_CONSOLE (GNB_TRACE,
        "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      GetAsk (GnbHandle, &MpioData[InstanceId]);
      MpioCfgAfterReconfig (GnbHandle);

      NbioMpioServiceCommonInitArguments (MpioArg);
      ArgPtr = (VOID *) MpioArg;
      ((SETUP_LINK_ARGS *) ArgPtr)->PerstReq = 1;
      IDS_HDT_CONSOLE (GNB_TRACE,
        "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      MpioServiceRequest (GnbHandle->Address, POSTED_MSG (MPIO_SETUP_LINK), MpioArg, 0);
      IDS_HDT_CONSOLE (GNB_TRACE,
        "Response = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
        MpioArg[0],
        MpioArg[1],
        MpioArg[2],
        MpioArg[3],
        MpioArg[4],
        MpioArg[5]
        );
      GetAsk (GnbHandle, &MpioData[InstanceId]);

      // de-assert PCIe reset all Engines
      GpioResetInfo.ResetId = MpioData->MpioAsk->desc.gpioHandle;
      GpioResetInfo.ResetControl = 1;
      AgesaGpioSlotResetControl ((UINTN) GnbHandle->Address.Address.Bus, &GpioResetInfo);

      // PCIE port autentication before training
      MpioPcieAuthenticationBeforeTraining(GnbHandle, PlatformTopology, &MpioData[InstanceId]);

      ResetDelay = PcdGet16 (PcdAfterResetDelay);
      if (ResetDelay != 0) {
        IDS_HDT_CONSOLE (GNB_TRACE, "Reset delay %dmS\n", PcdGet16 (PcdAfterResetDelay));
        MicroSecondDelay (PcdGet16 (PcdAfterResetDelay) * 1000);
        ResetDelay = 0;
      }
      IDS_HDT_CONSOLE (GNB_TRACE, "Reset Deassert Request for GpioId 0x%x\n", GpioResetInfo.ResetId);

      NbioMpioServiceCommonInitArguments (MpioArg);
      ArgPtr = (VOID *) MpioArg;
      ((SETUP_LINK_ARGS *) ArgPtr)->Training = 1;
      ((SETUP_LINK_ARGS *) ArgPtr)->Enumerate = 1;
      IDS_HDT_CONSOLE (GNB_TRACE,
                       "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
                       MpioArg[0],
                       MpioArg[1],
                       MpioArg[2],
                       MpioArg[3],
                       MpioArg[4],
                       MpioArg[5]
                       );
      MpioServiceRequest (GnbHandle->Address, POSTED_MSG (MPIO_SETUP_LINK), MpioArg, 0);
      GetAsk (GnbHandle, &MpioData[InstanceId]);
      MpioUpdatePortTrainingStatus (GnbHandle, &MpioData[InstanceId]);
    }
    do {
      GnbHandle = GnbGetNextHandle(GnbHandle);
    } while ((GnbHandle != NULL) && (InstanceId == GnbHandle->SocketId));
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "%a : Exit [0x%x]\n", __FUNCTION__, AgesaStatus);
  return  AgesaStatus;
}
