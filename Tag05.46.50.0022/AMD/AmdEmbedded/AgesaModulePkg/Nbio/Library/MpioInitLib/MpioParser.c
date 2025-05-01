/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/**
 *  @file MpioParser.c
 *  @brief Mpio Platform Topology Parser
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
#include <Library/MemoryAllocationLib.h>
#include "MpioLibLocal.h"

#define FILECODE NBIO_LIBRARY_MPIOINITLIB_MPIOPARSER_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

///Port information needed for lane translation callback
typedef struct {
  UINT8           StartLane;          ///< Start Lane from User Config
  UINT8           EndLane;            ///< End Lane from User Config
  UINT8           DxioStartLane;      ///< Translated Start Lane
  UINT8           DxioEndLane;        ///< Translated End Lane
  BOOLEAN         LinkReversal;       ///< Link Reversal should be set
  BOOLEAN         LanesFound;         ///< Flag for lanes found
} PORT_INFO_CALLBACK;


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/**
 *  @brief Init Attibutes in an AskEntry from the Platform Topology
 *
 *  @param [in] AskEntry Description for AskEntry
 *  @param [in] TopologyEntry Description for TopologyEntry
 *  @return Return description
 *
 *  @details More details
 */
VOID
InitAskEntryAttributes (
  IN     FW3_LINK_ATTR          *Attributes,
  IN     DXIO_PORT_DESCRIPTOR   *TopologyEntry
  )
{
  Attributes->maxLinkSpeedCap = TopologyEntry->Port.LinkSpeedCapability;
  Attributes->targetLinkSpeed = TopologyEntry->Port.TargetLinkSpeed;
  Attributes->devfuncnumber = (TopologyEntry->Port.DeviceNumber << 3) | TopologyEntry->Port.FunctionNumber;
  Attributes->portPresent = TopologyEntry->Port.PortPresent;
  Attributes->earlyTrainLink = TopologyEntry->Port.MiscControls.SbLink;
  Attributes->linkComplianceMode = TopologyEntry->Port.MiscControls.LinkComplianceMode;
  Attributes->linkHotplugType = TopologyEntry->Port.LinkHotplug;

  Attributes->isMasterPLL = TopologyEntry->Port.IsMasterPLL;

  if (TopologyEntry->Port.TXDeEmphasisOride) {
    Attributes->txDeEmphasisEn = 1;
    Attributes->txDeEmphasis = TopologyEntry->Port.TXDeEmphasis;
  }
  Attributes->txVetting = TopologyEntry->Port.TxVetting;
  Attributes->rxVetting = TopologyEntry->Port.RxVetting;
  //TO DO Add rxVettingGen5 and txVettingGen5 fields in DXIO_PORT_DESCRIPTOR and then uncomment this code
  // Attributes->txVettingGen5 = TopologyEntry->Port.txVettingGen5;
  // Attributes->rxVettingGen5 = TopologyEntry->Port.rxVettingGen5;

  Attributes->invertPolarityRx = TopologyEntry->Port.InvertPolarity;
  Attributes->invertPolarityTx = TopologyEntry->Port.InvertPolarity2;

  // Attributes->PSPPMode           :3;
  // Attributes->partnerDeviceType  :2;
  // Attributes->localPerst         :1;
  // Attributes->bifMode            :1;
  // Attributes->gen3EqSearchMode   :2;
  // Attributes->gen3EqSearchModeEn :2;
  // Attributes->gen4EqSearchMode   :2;
  // Attributes->gen4EqSearchModeEn :2;
  // Attributes->gen5EqSearchMode   :2;
  // Attributes->gen5EqSearchModeEn :2;
  // Attributes->gen6EqSearchMode   :2;
  // Attributes->gen6EqSearchModeEn :2;
  // Attributes->esmSpeed           :6;
  // Attributes->esmMode            :2;

  // if (TopologyEntry->Port.EqPreset != 0) {
    // Attributes->eqSearchModeOride = 1;
    // Attributes->eqSearchMode = TopologyEntry->Port.EqPreset;
    // IDS_HDT_CONSOLE (GNB_TRACE, "EqSearchMode: %d\n",
        // Attributes->eqSearchMode);
  // }
  // Attributes->enableTurnOffSettings = 1;
  // Attributes->turnOffUnusedLanes = TopologyEntry->Port.MiscControls.TurnOffUnusedLanes;
  // Attributes->bypassGen3EQ = TopologyEntry->Port.BypassGen3EQ;
  // Attributes->bypassGen4EQ = TopologyEntry->Port.BypassGen4EQ;

  // Attributes->ntbHotPlug = TopologyEntry->Port.NtbHotplug;
  return;
}


/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]       Wrapper         Pointer to wrapper config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 */
VOID
STATIC
MpioTranslateLanesCallback (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,
  IN      GNB_HANDLE            *GnbHandle
  )
{
  PORT_INFO_CALLBACK    *PortInfo;

  PortInfo = (PORT_INFO_CALLBACK *) Buffer;
  if ((PortInfo->StartLane >= Wrapper->StartPhyLane) && (PortInfo->StartLane <= Wrapper->EndPhyLane)) {
    if (Wrapper->Features.CoreReversed == 0) {
      if (PortInfo->StartLane <= PortInfo->EndLane) {
        PortInfo->DxioStartLane = Wrapper->StartDxioLane + (PortInfo->StartLane - Wrapper->StartPhyLane);
        PortInfo->DxioEndLane = Wrapper->StartDxioLane + (PortInfo->EndLane - Wrapper->StartPhyLane);
        PortInfo->LinkReversal = FALSE;
      } else {
        IDS_HDT_CONSOLE (GNB_TRACE, "  Core is NOT reversed, Link IS reversed\n");
        PortInfo->DxioEndLane = Wrapper->StartDxioLane + (PortInfo->StartLane - Wrapper->StartPhyLane);
        PortInfo->DxioStartLane = Wrapper->StartDxioLane + (PortInfo->EndLane - Wrapper->StartPhyLane);
        PortInfo->LinkReversal = TRUE;
      }
    } else {
      if (PortInfo->StartLane <= PortInfo->EndLane) {
        IDS_HDT_CONSOLE (GNB_TRACE, "  Core IS reversed, Link is NOT reversed\n");
        PortInfo->DxioEndLane = Wrapper->EndDxioLane - (PortInfo->StartLane - Wrapper->StartPhyLane);
        PortInfo->DxioStartLane = Wrapper->EndDxioLane - (PortInfo->EndLane - Wrapper->StartPhyLane);
        PortInfo->LinkReversal = TRUE;
      } else {
        IDS_HDT_CONSOLE (GNB_TRACE, "  Core IS reversed, Link IS reversed\n");
        PortInfo->DxioStartLane = Wrapper->EndDxioLane - (PortInfo->StartLane - Wrapper->StartPhyLane);
        PortInfo->DxioEndLane = Wrapper->EndDxioLane - (PortInfo->EndLane - Wrapper->StartPhyLane);
        PortInfo->LinkReversal = FALSE;
      }
    }
    PortInfo->LanesFound = TRUE;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 *  @brief Get translated lane numbers based on logical lane inputs
 *
 *  @param [in] GnbHandle     Pointer to silicon descriptory
 *  @param [in] StartLane     Start lane for this port from Platform Topology
 *  @param [in] EndLane End   lane for this port from Platform Topology
 *  @param [in] StartDxioLane Pointer to storage for translated start lane
 *  @param [in] EndDxioLane   Pointer to storage for translated end lane
 *  @param [in] Reverse       Pointer to boolean identifier for reversed lanes
 *  @return                   Return TRUE if lanes were translated
 *
 *  @details More details
 */
 BOOLEAN
MpioTranslateLanes (
  IN       GNB_HANDLE                *GnbHandle,
  IN       UINT8                     StartLane,
  IN       UINT8                     EndLane,
  IN OUT   UINT8                     *StartDxioLane,
  IN OUT   UINT8                     *EndDxioLane,
  IN OUT   BOOLEAN                   *Reverse
  )
{
  PORT_INFO_CALLBACK    PortInfo;
  GNB_HANDLE            *Handle;

  PortInfo.StartLane = StartLane;
  PortInfo.EndLane = EndLane;
  PortInfo.LanesFound = FALSE;
  PortInfo.DxioStartLane = 0;
  PortInfo.DxioEndLane = 0;
  PortInfo.LinkReversal = FALSE;
  Handle = GnbHandle;
  while (Handle != NULL) {
    if (Handle->SocketId == GnbHandle->SocketId) {
      PcieConfigRunProcForAllWrappersInNbio (DESCRIPTOR_ALL_WRAPPERS,
                                             MpioTranslateLanesCallback,
                                             (VOID *)
                                             &PortInfo,
                                             Handle
                                             );
    }
    if (PortInfo.LanesFound) {
      break;
    }
    Handle = GnbGetNextHandle (Handle);
  }

  if (PortInfo.LanesFound) {
    *StartDxioLane = PortInfo.DxioStartLane;
    *EndDxioLane = PortInfo.DxioEndLane;
    *Reverse = PortInfo.LinkReversal;
  } else {
    IDS_HDT_CONSOLE (GNB_TRACE, "  Invalid request for StartLane %d, EndLane %D\n", StartLane, EndLane);
    *StartDxioLane = 0;
    *EndDxioLane = 0;
  }
  ASSERT (PortInfo.LanesFound);
  return PortInfo.LanesFound;
}


/*----------------------------------------------------------------------------------------*/
/**
 *  @brief Calculate the size of the global ancillary data
 *
 *  @param [in] Pcie          Pointer to PCIe platform config structure
 *  @param [in] SocketTopology  Pointer to Platform Topology
 *  @param [in] StrapSize     Pointer to UINT32 to store size of PCIe strap values
 *  @return Return description
 *
 *  @details More details
 */
 UINT32
 STATIC
GetGlobalAncillaryDataSize (
  IN       GNB_HANDLE              *GnbHandle,
  IN       DXIO_PORT_DESCRIPTOR    *SocketTopology,
  IN       UINT32                  *StrapSize
  )
{
//  DXIO_PHYOVR_TABLE     *PhyOverrideData;
//  UINT32                StrapListSize;
  UINT32                  GlobalDataSize;

  GlobalDataSize = 0;
  // Total Ancillary Data size includes per-port size calculated above plus additional storage for
  // Add Ancillary Data allocation for PSPP
//  if (Pcie->PsppPolicy != AgesaPsppDisabled) {
//    GlobalDataSize += sizeof (DXIO_ANCILLARYPORTDATA_PSPP) + sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
//    if (Pcie->PsppTuningParams2 != 0) {
//      GlobalDataSize += sizeof (DXIO_ANCILLARYPORTDATA_PSPP_2);
//    }
//  }
//  // Add Ancillary Data allocation for PHY DATA (server)
//  PhyOverrideData = (DXIO_PHYOVR_TABLE *)Pcie->PhyConfigData;
//  if (PhyOverrideData != NULL) {
//    GlobalDataSize += sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
//    GlobalDataSize += PhyOverrideData->size; // size of config block
//  }
//  // Add Ancillary Data allocation for STRAP settings
//  StrapListSize = DxioCfgGetStrapListSize (GnbHandle, SocketTopology);
//  if (StrapListSize != 0) {
//    GlobalDataSize += StrapListSize;
//    *StrapSize = StrapListSize;
//  }
//  // Anciallary Data calculation is complete
//  if (GlobalDataSize != 0) {
//    GlobalDataSize += sizeof (DXIO_HEAPDATA_CONTAINER) + sizeof (DXIO_ANCILLARYDATA);
//  }
  return GlobalDataSize;
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief Platform Topology Parser for MPIO Ask
 *
 * @param [in]  GnbHandle        Pointer to the Silicon Descriptor for this node
 * @param [in]  PlatformTopology Pointer to the platform BIOS supplied platform configuration
 * @param [out] MpioData         Pointer to the data structure to describe this "node"
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
MpioParsePlatformTopology (
  IN       GNB_HANDLE                *GnbHandle,
  IN       DXIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
     OUT   MPIO_DATA                 *MpioData
  )
{
  AGESA_STATUS               AgesaStatus;

  UINT8                      PortStartLane;
  UINT8                      PortEndLane;
  BOOLEAN                    Reverse;
  // DXIO Firmware Structures
  FW_ASK_STRUCT              *AskBufferStart;
  FW_ASK_STRUCT              *AskEntry;
  FW3_LINK_ATTR              *Attributes;
  UINT32                     AskInstanceCount;
  UINT32                     AskInstanceSize;
  // Ancillary Data Structures
  UINT32                     StrapSize;
//  UINT32                     DataSize;
  UINT32                     AncillaryDataSize;
//  DXIO_ANCILLARYDATA         *DxioAncData;
//  DXIO_ANCILLARYDATA_SUBHEAP *DxioSubHeapData;
//  DXIO_PHYOVR_TABLE          *PhyOverrideData;
  UINT8                      PortParamIndex;
  UINT8                      FoundAllParams;
  UINT8                      Counter = 0;
  // Platform Topology Pointers
  DXIO_PORT_DESCRIPTOR      *SocketTopology;              // Local pointer to start of User Config for this socket
  DXIO_PORT_DESCRIPTOR      *TopologyEntry;               // Local updatable pointer for SocketTopology processing

  IDS_HDT_CONSOLE (GNB_TRACE, "%a : Enter with MpioData = 0x%x\n", __FUNCTION__, (UINT32) MpioData);


  AgesaStatus = AGESA_SUCCESS;
  SocketTopology = NULL;

  AgesaStatus = MpioGetTopologyForSocket (GnbHandle->SocketId, PlatformTopology, &SocketTopology);

  if (AGESA_SUCCESS == AgesaStatus) {
    // Parse structure and count entries for this node
    AskEntry = NULL;
    AskInstanceSize = 0;
    AskInstanceCount = 0;

    // Initialize AncillaryDataSize to the size of the global ancillary data
    AncillaryDataSize = GetGlobalAncillaryDataSize (GnbHandle, SocketTopology, &StrapSize);
    // Calculate size of Platform Structure and Ancillary Data for this node
    TopologyEntry = SocketTopology;
    while (TopologyEntry != NULL) {
      if (TopologyEntry->EngineData.EngineType != DxioUnusedEngine) {
        if (MpioTranslateLanes(GnbHandle,
                               TopologyEntry->EngineData.StartLane,
                               TopologyEntry->EngineData.EndLane,
                               &PortStartLane,
                               &PortEndLane,
                               &Reverse
                               )) {
          AskInstanceSize += sizeof (FW_ASK_STRUCT);
          AskInstanceCount++;
          IDS_HDT_CONSOLE (GNB_TRACE,
                           "AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
                           AskInstanceSize,
                           AskInstanceCount
                           );

//          AncillaryDataSize += DxioAncDataCalcSize (TopologyEntry);
          IDS_HDT_CONSOLE (GNB_TRACE, "Total Ancillary Data Size %d\n", AncillaryDataSize);
        } else {
          // Could not translate lanes.  This entry will be ignored. Note that it would ASSERT earlier;
          TopologyEntry->EngineData.EngineType = DxioUnusedEngine;
        }
      }
      TopologyEntry = PcieConfigGetNextDataDescriptor(TopologyEntry);
    }
    IDS_HDT_CONSOLE (GNB_TRACE,
                     "Final AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
                     AskInstanceSize,
                     AskInstanceCount
                     );

    // Initialize values
    IDS_HDT_CONSOLE (GNB_TRACE, "MpioData at 0x%x\n", (UINT32) MpioData);
    MpioData->MpioAsk = NULL;
    MpioData->MpioAskCount = 0;
    MpioData->ExtAttributes = NULL;
    MpioData->ExtAttributeSize = 0;
    IDS_HDT_CONSOLE (GNB_TRACE,
                     "MpioData at 0x%x\n -- MpioAsk = 0x%x\n -- MpioAskCount = %d\n",
                     (UINT32) MpioData,
                     (UINT32) MpioData->MpioAsk,
                     MpioData->MpioAskCount
                     );

    // Build data structures for this node if there is any configuration required
    if (AskInstanceCount != 0) {
      // Allocate memory for this node ancillary data

      //----------@TODO-----------------
      //EGH @TODO ANCILLARY DATA  //<<<< @TODO
      //AncillaryDataSize = 0;      //<<<< @TODO
      //^^^^^^^^^^@TODO^^^^^^^^^^^^^^^^^

      /*if (AncillaryDataSize != 0) {
        AncillaryData = (UINT8 *) AllocatePool (AncillaryDataSize);
        if (AncillaryData != NULL) {
          // Store the information in MPIO_DATA
          MpioData->ExtAttributes = AncillaryData;
          MpioData->ExtAttributeSize = AncillaryDataSize;

          LibAmdMemFill (AncillaryData, 0, AncillaryDataSize, (AMD_CONFIG_PARAMS *) NULL);

          InitGlobalAncillaryData (GnbHandle, SocketTopology);
        }
      }*/

      IDS_HDT_CONSOLE (GNB_TRACE,
                       "AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
                       AskInstanceSize,
                       AskInstanceCount
                       );
      // Allocate memory for this node platform structure
      AskBufferStart = (FW_ASK_STRUCT *) AllocatePool (AskInstanceSize);
      IDS_HDT_CONSOLE (GNB_TRACE,
                       "AskBufferStart allocated at 0x%x with size 0x%x\n",
                       (UINT32) AskBufferStart,
                       AskInstanceSize
                       );

      if (AskBufferStart != NULL) {
        //ASSERT_EFI_ERROR (AgesaStatus);
        MpioData->MpioAsk = AskBufferStart;             // Save pointer to allocated memory
        MpioData->MpioAskCount = AskInstanceCount;


        // Initialize platform level entries for this structure
        LibAmdMemFill (AskBufferStart, 0, AskInstanceSize, (AMD_CONFIG_PARAMS *) NULL);

        IDS_HDT_CONSOLE (GNB_TRACE,
                         "AskInstanceSize = 0x%x and AskInstanceCount = %d\n",
                         AskInstanceSize,
                         AskInstanceCount
                         );

        // Parse structure and copy entries for this node to this node platform structure
        AskEntry = (FW_ASK_STRUCT *)AskBufferStart;       // Initialize engine pointer to first engine
        AncillaryDataSize = 0;
        TopologyEntry = SocketTopology;
        while (TopologyEntry != NULL) {
          if (TopologyEntry->EngineData.EngineType != DxioUnusedEngine) {
            if (MpioTranslateLanes(GnbHandle,
                                   TopologyEntry->EngineData.StartLane,
                                   TopologyEntry->EngineData.EndLane,
                                   &PortStartLane,
                                   &PortEndLane,
                                   &Reverse
                                   )) {
              TopologyEntry->EngineData.DxioStartLane = PortStartLane;
              TopologyEntry->EngineData.DxioEndLane = PortEndLane;
              AskEntry->desc.startLaneIndex = PortStartLane;
              AskEntry->desc.numberOfLanes = (PortEndLane - PortStartLane) + 1;
              Attributes = &AskEntry->desc.link_attributes;
              if (Reverse) {
                AskEntry->desc.reversed = 1;
              }
              switch (TopologyEntry->EngineData.EngineType) {
              case DxioUSB_OVER_PCIE:
                // todo - PXH temp solution
                if (Counter == 0) {
                  PortStartLane = 30;
                  PortEndLane = 30;
                  AskEntry->desc.startLaneIndex = PortStartLane;
                  AskEntry->desc.numberOfLanes = 1;
                  TopologyEntry->EngineData.DxioStartLane = PortStartLane;
                  TopologyEntry->EngineData.DxioEndLane = PortEndLane;
                } else {
                  PortStartLane = 34;
                  PortEndLane = 34;
                  AskEntry->desc.startLaneIndex = PortStartLane;
                  AskEntry->desc.numberOfLanes = 1;
                  TopologyEntry->EngineData.DxioStartLane = PortStartLane;
                  TopologyEntry->EngineData.DxioEndLane = PortEndLane;
                }

                AskEntry->desc.ctrlType = ASK_TYPE_PCIe;
                AskEntry->desc.gpioHandle = TopologyEntry->EngineData.GpioGroupId;
                InitAskEntryAttributes (Attributes, TopologyEntry);

                // Add HFC and DFC indices to link attributes of AskEntry
                PortParamIndex = 0;
                FoundAllParams = 0;
                while (PortParamIndex < PCIE_PORT_PARAMETER_COUNT && FoundAllParams < 2) {
                  if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == PP_UBM_HFC_INDEX) {
                    AskEntry->desc.link_attributes.hfc_index = (UINT8) TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue;
                    FoundAllParams++;
                  }
                  if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == PP_UBM_DFC_INDEX) {
                    AskEntry->desc.link_attributes.dfc_index = (UINT8) TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue;
                    FoundAllParams++;
                  }
                  PortParamIndex++;
                }

                Counter++;
                break;
              case DxioPcieEngine:
                AskEntry->desc.ctrlType = ASK_TYPE_PCIe;
                AskEntry->desc.gpioHandle = TopologyEntry->EngineData.GpioGroupId;
                InitAskEntryAttributes (Attributes, TopologyEntry);

                // Add HFC and DFC indices to link attributes of AskEntry
                PortParamIndex = 0;
                FoundAllParams = 0;
                while (PortParamIndex < PCIE_PORT_PARAMETER_COUNT && FoundAllParams < 2) {
                  if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == PP_UBM_HFC_INDEX) {
                    AskEntry->desc.link_attributes.hfc_index = (UINT8) TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue;
                    FoundAllParams++;
                  }
                  if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == PP_UBM_DFC_INDEX) {
                    AskEntry->desc.link_attributes.dfc_index = (UINT8) TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue;
                    FoundAllParams++;
                  }
                  PortParamIndex++;
                }
//              if (TopologyEntry->Port.CsLink == 1) {
//                AskEntry->forceKpnpReset = 1;
//                IDS_HDT_CONSOLE (GNB_TRACE,
//                                 "Dev%d Func%d: forceKpnpReset=%d CsLink=%d \n",
//                                 TopologyEntry->Port.DeviceNumber,
//                                 TopologyEntry->Port.FunctionNumber,
//                                 AskEntry->forceKpnpReset,
//                                 TopologyEntry->Port.CsLink
//                                 );
//              }


                // Populate Ancillary Data for this port
                // DataSize = DxioAncDataPopulate (AskEntry, TopologyEntry, AncillaryData);
                // if (0 != DataSize) {
                  // AskEntry->desc.ancillaryDataIdx = AncillaryDataOffset >> 2;
                  // AncillaryData += DataSize;
                  // AncillaryDataOffset += DataSize;
                // }
                break;
              case DxioSATAEngine:
                AskEntry->desc.ctrlType = ASK_TYPE_SATA;
                AskEntry->desc.channelType = TopologyEntry->Port.MiscControls.ChannelType;
                Attributes->invertPolarityRx = TopologyEntry->Port.InvertPolarity;
                Attributes->invertPolarityTx = TopologyEntry->Port.InvertPolarity2;

                // Populate Ancillary Data for this port
                // DataSize = DxioAncDataPopulate (AskEntry, TopologyEntry, AncillaryData);
                // if (0 != DataSize) {
                  // AskEntry->desc.ancillaryDataIdx = AncillaryDataOffset >> 2;
                  // AncillaryData += DataSize;
                  // AncillaryDataOffset += DataSize;
                // }
                break;
              default:
                break;
              }
            }
            AskEntry++;
          }
          TopologyEntry = PcieConfigGetNextDataDescriptor(TopologyEntry);
        }
      }
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE,
                   "MpioData at 0x%x\n -- MpioAsk = 0x%x\n -- MpioAskCount = %d\n",
                   (UINT32) MpioData,
                   (UINT32) MpioData->MpioAsk,
                   MpioData->MpioAskCount
                   );
  // Return pointer to this node platform structure
  return (AgesaStatus);
}



