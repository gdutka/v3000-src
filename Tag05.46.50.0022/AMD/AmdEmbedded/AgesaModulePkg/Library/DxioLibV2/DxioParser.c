/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * DXIO User Configuration Parser
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
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

#define FILECODE LIBRARY_DXIOLIBV2_DXIOPARSER_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define   SSP_PLATFORM_TYPE  0
#define   MAX_INSTANCE_ID    2

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

///Port information needed for lane translation callback
typedef struct {
  UINT8           SocketNumber;       ///< SocketNumber
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
AGESA_STATUS
STATIC
DxioTranslateLanesCallback (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,
  IN      GNB_HANDLE            *GnbHandle
  )
{
  PORT_INFO_CALLBACK    *PortInfo;

  PortInfo = (PORT_INFO_CALLBACK *) Buffer;
  if (GnbHandle->SocketId == PortInfo->SocketNumber) {
    if ((PortInfo->StartLane >= Wrapper->StartPhyLane) && (PortInfo->StartLane <= Wrapper->EndPhyLane)) {
      if (Wrapper->Features.CoreReversed == 0) {
        if (PortInfo->StartLane <= PortInfo->EndLane) {
          IDS_HDT_CONSOLE (PCIE_MISC, "  Core is NOT reversed, Link is NOT reversed\n");
          PortInfo->DxioStartLane = Wrapper->StartDxioLane + (PortInfo->StartLane - Wrapper->StartPhyLane);
          PortInfo->DxioEndLane = Wrapper->StartDxioLane + (PortInfo->EndLane - Wrapper->StartPhyLane);
          PortInfo->LinkReversal = FALSE;
        } else {
          IDS_HDT_CONSOLE (PCIE_MISC, "  Core is NOT reversed, Link IS reversed\n");
          PortInfo->DxioEndLane = Wrapper->StartDxioLane + (PortInfo->StartLane - Wrapper->StartPhyLane);
          PortInfo->DxioStartLane = Wrapper->StartDxioLane + (PortInfo->EndLane - Wrapper->StartPhyLane);
          PortInfo->LinkReversal = TRUE;
        }
      } else {
        if (PortInfo->StartLane <= PortInfo->EndLane) {
          IDS_HDT_CONSOLE (PCIE_MISC, "  Core IS reversed, Link is NOT reversed\n");
          PortInfo->DxioEndLane = Wrapper->EndDxioLane - (PortInfo->StartLane - Wrapper->StartPhyLane);
          PortInfo->DxioStartLane = Wrapper->EndDxioLane - (PortInfo->EndLane - Wrapper->StartPhyLane);
          PortInfo->LinkReversal = TRUE;
        } else {
          IDS_HDT_CONSOLE (PCIE_MISC, "  Core IS reversed, Link IS reversed\n");
          PortInfo->DxioStartLane = Wrapper->EndDxioLane - (PortInfo->StartLane - Wrapper->StartPhyLane);
          PortInfo->DxioEndLane = Wrapper->EndDxioLane - (PortInfo->EndLane - Wrapper->StartPhyLane);
          PortInfo->LinkReversal = FALSE;
        }
      }
      PortInfo->LanesFound = TRUE;
    }
  }
  return AGESA_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * DXIO User Config Parser
 *
 *
 *
 * @param[in]  Pcie           PCIe configuration info
 * @param[in]  StartLane      StartLane for this port from User Config
 * @param[in]  EndLane        EndLane for this port from User Config
 * @retval     BOOLEAN
 */

BOOLEAN
DxioTranslateLanes (
  IN       GNB_HANDLE                *GnbHandle,
  IN       UINT8                     SocketNumber,
  IN       UINT8                     StartLane,
  IN       UINT8                     EndLane,
  IN OUT   UINT8                     *StartDxioLane,
  IN OUT   UINT8                     *EndDxioLane,
  IN OUT   BOOLEAN                   *Reverse
  )
{
  PORT_INFO_CALLBACK    PortInfo;
  GNB_HANDLE            *LocalHandle;

  PortInfo.SocketNumber = SocketNumber;
  PortInfo.StartLane = StartLane;
  PortInfo.EndLane = EndLane;
  PortInfo.LanesFound = FALSE;
  PortInfo.DxioStartLane = 0;
  PortInfo.DxioEndLane = 0;
  PortInfo.LinkReversal = FALSE;
  LocalHandle = GnbHandle;
  while (LocalHandle != NULL) {
    PcieConfigRunProcForAllWrappersInNbio (DESCRIPTOR_ALL_WRAPPERS, (PCIe_RUN_ON_WRAPPER_CALLBACK2) DxioTranslateLanesCallback, (VOID *) &PortInfo, LocalHandle);
    LocalHandle = GnbGetNextHandle (LocalHandle);
  }

  if (PortInfo.LanesFound) {
    *StartDxioLane = PortInfo.DxioStartLane;
    *EndDxioLane = PortInfo.DxioEndLane;
    *Reverse = PortInfo.LinkReversal;
  } else {
    *StartDxioLane = 0;
    *EndDxioLane = 0;
  }
  ASSERT (PortInfo.LanesFound);
  return PortInfo.LanesFound;
}

/*----------------------------------------------------------------------------------------*/
/**
 * DXIO User Config Parser
 *
 *
 *
 * @param[in]  PeiServices    Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]  UserConfig     Pointer to the platform BIOS supplied platform configuration
 * @param[out] ThisNode       Pointer to a location to store the address of the platform descriptor for this node
 * @param[out] NodeSize       Pointer to a location to store the size (in dwords) of the platforrm desciptor for this node
 * @param[out] AncData        Pointer to a location to store the address of the ancillary port data
 * @param[out] AncDataSize    Pointer to a location to store the size (in dwords) of the ancillary port data
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
DxioParseUserConfig (
  IN       CONST EFI_PEI_SERVICES    **PeiServices,
  IN       GNB_HANDLE                *GnbHandle,
  IN       DXIO_COMPLEX_DESCRIPTOR   *UserConfig,
  IN       PCIe_PLATFORM_CONFIG      *Pcie,
  OUT      DXIO_PLATFORM_DESCRIPTOR  **ThisNode,
  OUT      UINT32                    *NodeSize,
  OUT      UINT8                     **AncData,
  OUT      UINT32                    *AncDataSize
  )
{
  AGESA_STATUS               AgesaStatus;
  UINT8                      SocketNumber;
  UINT8                      PortStartLane;
  UINT8                      PortEndLane;
  BOOLEAN                    Reverse;
  // DXIO Firmware Structures
  DXIO_PLATFORM_DESCRIPTOR   *NodePlatformDesc;
  DXIO_ENGINE_DESCRIPTOR     *EnginePointer;
  UINT32                     ThisNodeCount;
  UINT32                     ThisNodeSize;
  // Ancillary Data Structures
  UINT32                     StrapSize;
  UINT32                     DataSize;
  UINT8                      *AncillaryData;
  UINT32                     AncillaryDataSize;
  UINT32                     AncillaryDataOffset;
  UINT8                      *EtherNetPortHeap;
  UINT32                     EtherNetPortOffset;
  DXIO_ANCILLARYDATA         *DxioAncData;
  DXIO_ANCILLARYDATA_SUBHEAP *DxioSubHeapData;
  DXIO_PHYOVR_TABLE          *PhyOverrideData;
  UINT32                     EtherNetPortCount;
  UINT8                      EthernetDataSize;
  //User Config Pointers
  DXIO_PORT_DESCRIPTOR       *PortList;              // Pointer to start of User Config for this socket
  DXIO_PORT_DESCRIPTOR       *PortPointer;           // Updatable pointer for PortList processing

  EthernetDataSize = sizeof (ETHERNET_PORT_DATA) + sizeof (ETHERNET_PORT_DATA_EXT);
  // Get Socket Number
  SocketNumber = GnbHandle->SocketId;

  AgesaStatus = AGESA_SUCCESS;
  PortList = NULL;

  AgesaStatus = DxioGetUserCfgForSocket (SocketNumber, UserConfig, &PortList);

  if (AGESA_SUCCESS == AgesaStatus) {
    // Parse structure and count entries for this node
    PortPointer = PortList;
    ThisNodeSize = sizeof (DXIO_PLATFORM_DESCRIPTOR) + 8;  // Include 2 reserved UINT32 at the end
    ThisNodeCount = 0;
    EtherNetPortCount = 0;
    EtherNetPortOffset = 0;
    EtherNetPortHeap = NULL;
    AncillaryDataSize = 0;
    AncillaryDataOffset = 0;
    EnginePointer = NULL;

    // Calculate size of Platform Structure and Ancillary Data for this node
    while (PortPointer != NULL) {
      if (PortPointer->EngineData.EngineType != DxioUnusedEngine) {
        if (DxioTranslateLanes(GnbHandle,
                               SocketNumber,
                               PortPointer->EngineData.StartLane,
                               PortPointer->EngineData.EndLane,
                               &PortStartLane,
                               &PortEndLane,
                               &Reverse
                               )) {
          ThisNodeSize += sizeof (DXIO_ENGINE_DESCRIPTOR);
          ThisNodeCount++;
        if (PortPointer->EngineData.EngineType == DxioEthernetEngine) {
          EtherNetPortCount++;
        } else {
          AncillaryDataSize += DxioAncDataCalcSize (PortPointer);
          IDS_HDT_CONSOLE (PCIE_MISC, "Total Ancillary Data Size %d\n", AncillaryDataSize);
          }
        } else {
          // Could not translate lanes.  This entry will be ignored. Note that it would ASSERT earlier;
          PortPointer->EngineData.EngineType = DxioUnusedEngine;
        }
      }
      PortPointer = PcieConfigGetNextDataDescriptor(PortPointer);
    }
    // Add Ancillary Data allocation for PSPP
    if (Pcie->PsppPolicy != AgesaPsppDisabled) {
      AncillaryDataSize += sizeof (DXIO_ANCILLARYPORTDATA_PSPP) + sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
      if (Pcie->PsppTuningParams2 != 0) {
        AncillaryDataSize += sizeof (DXIO_ANCILLARYPORTDATA_PSPP_2);
      }
    }
    if (PcdGet8 (PcdCfgDxioPhySeqModeIqOffset) != 0x0) {
      AncillaryDataSize += sizeof (DXIO_ANCILLARYDATA_PHYVALUE) + sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
    }

if (EtherNetPortCount != 0) {
      AncillaryDataSize += (4 * EthernetDataSize) + sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
    }
    // Add Ancillary Data allocation for PHY DATA (server)
    PhyOverrideData = (DXIO_PHYOVR_TABLE *)Pcie->PhyConfigData;
    if (PhyOverrideData != NULL) {
      AncillaryDataSize += sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
      AncillaryDataSize += PhyOverrideData->size; // size of config block
    }

    // Add Ancillary Data allocation for STRAP settings
    StrapSize = DxioCfgGetStrapListSize (GnbHandle, PortList);
    if (StrapSize != 0) {
      AncillaryDataSize += StrapSize;
    }
    // Anciallary Data calculation is complete
    if (AncillaryDataSize != 0) {
      AncillaryDataSize += sizeof (DXIO_HEAPDATA_CONTAINER) + sizeof (DXIO_ANCILLARYDATA);
    }

    // Build data structures for this node if there is any configuration required
    if (ThisNodeCount != 0) {
      // Allocate memory for this node platform structure
      AgesaStatus = (*PeiServices)->AllocatePool (
                                      PeiServices,
                                      ThisNodeSize,
                                      (VOID **)&NodePlatformDesc
                                      );
      //ASSERT_EFI_ERROR (AgesaStatus);
      *ThisNode = NodePlatformDesc;             // Save pointer to allocated memory
      *NodeSize = (ThisNodeSize + 3) >> 2;      // Platfom Descriptor size in dwords


      // Allocate memory for this node ancillary data
      AncillaryData = NULL;
      if (AncillaryDataSize != 0) {
        AgesaStatus = (*PeiServices)->AllocatePool (
                                        PeiServices,
                                        AncillaryDataSize,
                                        (VOID **)&AncillaryData
                                        );
        if (AgesaStatus == AGESA_SUCCESS) {
          *AncData = AncillaryData;                          // Save pointer to allocated memory
          *AncDataSize = AncillaryDataSize >> 2;             // Ancillary Data size in dwords

          LibAmdMemFill (AncillaryData, 0, AncillaryDataSize, (AMD_CONFIG_PARAMS *) NULL);

          DxioAncData = (DXIO_ANCILLARYDATA *) AncillaryData;
          DxioAncData->descriptorType = DXIO_HEAPTYPE_ANCDATA;
          DxioAncData->version = DXIO_ANCILLARYDATA_VERSION;
          DxioAncData->numDWords = (AncillaryDataSize >> 2) -1;
          AncillaryData += sizeof (DXIO_ANCILLARYDATA);

          AncillaryDataOffset = sizeof (DXIO_ANCILLARYDATA);

          if (EtherNetPortCount != 0) {
            DxioSubHeapData = (DXIO_ANCILLARYDATA_SUBHEAP *) AncillaryData;
            DxioSubHeapData->descriptorType = DXIO_ANCDATA_SUBHEAPTYPE_XGBE;
            DxioSubHeapData->version = DXIO_ANCILLARYDATA_SUBHEAP_VERSION;
            DxioSubHeapData->numDWORDs = ((4 * EthernetDataSize) + sizeof (DXIO_ANCILLARYDATA_SUBHEAP)) >> 2;
            AncillaryData += sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
            EtherNetPortHeap = (UINT8 *) AncillaryData;
            EtherNetPortOffset = AncillaryDataOffset;
            AncillaryData += (4 * EthernetDataSize);
            AncillaryDataOffset += (4 * EthernetDataSize) + sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
          }
          if (Pcie->PsppPolicy != AgesaPsppDisabled) {
            DxioSubHeapData = (DXIO_ANCILLARYDATA_SUBHEAP *) AncillaryData;
            DxioSubHeapData->descriptorType = DXIO_ANCDATA_SUBHEAPTYPE_PSPP;
            DxioSubHeapData->version = DXIO_ANCILLARYDATA_SUBHEAP_VERSION;
            DxioSubHeapData->numDWORDs = (sizeof (DXIO_ANCILLARYPORTDATA_PSPP) + sizeof (DXIO_ANCILLARYDATA_SUBHEAP)) >> 2;
            AncillaryData += sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
            LibAmdMemCopy ((VOID *) AncillaryData,
                           (VOID *) &Pcie->PsppTuningParams,
                           sizeof (DXIO_ANCILLARYPORTDATA_PSPP),
                           (AMD_CONFIG_PARAMS *)NULL
                           );
            AncillaryData += sizeof (DXIO_ANCILLARYPORTDATA_PSPP);
            AncillaryDataOffset += sizeof (DXIO_ANCILLARYPORTDATA_PSPP) + sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
            if (Pcie->PsppTuningParams2 != 0) {
              DxioSubHeapData->numDWORDs += sizeof (DXIO_ANCILLARYPORTDATA_PSPP_2) >> 2;
              LibAmdMemCopy ((VOID *) AncillaryData,
                            (VOID *) &Pcie->PsppTuningParams2,
                            sizeof (DXIO_ANCILLARYPORTDATA_PSPP_2),
                            (AMD_CONFIG_PARAMS *)NULL
                            );
              AncillaryData += sizeof (DXIO_ANCILLARYPORTDATA_PSPP_2);
              AncillaryDataOffset += sizeof (DXIO_ANCILLARYPORTDATA_PSPP_2);
            }
          }
          if (PhyOverrideData != NULL) {
            DxioSubHeapData = (DXIO_ANCILLARYDATA_SUBHEAP *) AncillaryData;
            DxioSubHeapData->descriptorType = DXIO_ANCDATA_SUBHEAPTYPE_PHYCONFIG;
            DxioSubHeapData->version = DXIO_ANCILLARYDATA_SUBHEAP_VERSION;
            DxioSubHeapData->numDWORDs = ( PhyOverrideData->size +
                                          sizeof (DXIO_ANCILLARYDATA_SUBHEAP)) >> 2;
            AncillaryData += sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
            LibAmdMemCopy ((VOID *) AncillaryData,
                           (VOID *) PhyOverrideData->data,
                           PhyOverrideData->size,
                           (AMD_CONFIG_PARAMS *)NULL
                           );
            AncillaryData += PhyOverrideData->size;
            AncillaryDataOffset += PhyOverrideData->size +
                                   sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
          }
          if (PcdGet8 (PcdCfgDxioPhySeqModeIqOffset) != 0x0) {
            DXIO_ANCILLARYDATA_PHYVALUE *pAncillaryDataPhyValue;
            IDS_HDT_CONSOLE (PCIE_MISC, "Add Ancillary Data - SEQ_MODE_IQOFFSET[5]=0x%x\n", PcdGet8 (PcdCfgDxioPhySeqModeIqOffset));
            DxioSubHeapData = (DXIO_ANCILLARYDATA_SUBHEAP *) AncillaryData;
            DxioSubHeapData->descriptorType = DXIO_ANCDATA_SUBHEAPTYPE_PHYVALUE;
            DxioSubHeapData->version = DXIO_ANCILLARYDATA_SUBHEAP_VERSION;
            DxioSubHeapData->numDWORDs = ( 1 + sizeof (DXIO_ANCILLARYDATA_SUBHEAP)) >> 2;
            AncillaryData += sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
            pAncillaryDataPhyValue = (DXIO_ANCILLARYDATA_PHYVALUE *) AncillaryData;
            pAncillaryDataPhyValue->paramId  = DXIO_ANCDATA_SUBHEAPTYPE_PHYVALUE;
            pAncillaryDataPhyValue->columnId = 0;
            pAncillaryDataPhyValue->value    = PcdGet8 (PcdCfgDxioPhySeqModeIqOffset);
            pAncillaryDataPhyValue->laneNum  = 0;
            AncillaryData += sizeof (DXIO_ANCILLARYDATA_PHYVALUE);
            AncillaryDataOffset += sizeof (DXIO_ANCILLARYDATA_PHYVALUE) + sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
          }
          if (StrapSize != 0) {
            if (StrapSize != DxioCfgAddStrapList(GnbHandle, AncillaryData, PortList)) {
              ASSERT (FALSE);
            }
            AncillaryData += StrapSize;
            AncillaryDataOffset += StrapSize;
          }
        } else {
          *AncData = (UINT8 *) NULL;
          *AncDataSize = 0;
        }
      } else {
        *AncData = (UINT8 *) NULL;
        *AncDataSize = 0;
      }

      // Initialize platform level entries for this structure
      LibAmdMemFill (NodePlatformDesc, 0, ThisNodeSize, (AMD_CONFIG_PARAMS *) NULL);
      NodePlatformDesc->PlatformType = SSP_PLATFORM_TYPE;
      NodePlatformDesc->NumEngineDesc = ThisNodeCount;
      // Parse structure and copy entries for this node to this node platform structure
      PortPointer = PortList;
      NodePlatformDesc ++;                                              // Point to first engine descriptor
      EnginePointer = (DXIO_ENGINE_DESCRIPTOR *)NodePlatformDesc;       // Initialize engine pointer to first engine
      NodePlatformDesc --;                                              // Restore NodePlatformDesc address
      AncillaryDataSize = 0;
      while (PortPointer != NULL) {
        if (PortPointer->EngineData.EngineType != DxioUnusedEngine) {
          if (DxioTranslateLanes (GnbHandle,
                                 SocketNumber,
                                 PortPointer->EngineData.StartLane,
                                 PortPointer->EngineData.EndLane,
                                 &PortStartLane,
                                 &PortEndLane,
                                 &Reverse
                                 )) {
            EnginePointer->StartLane = PortStartLane;
            EnginePointer->EndLane = PortEndLane;
            PortPointer->EngineData.DxioStartLane = PortStartLane;
            PortPointer->EngineData.DxioEndLane = PortEndLane;
            EnginePointer->GpioGroupID = 1;
            EnginePointer->searchDepth = PortPointer->EngineData.SearchDepth;
            if (PcdGetBool (PcdDxioSingleResetCallout) == TRUE) {
              EnginePointer->engineResetID = 1;
            } else {
              EnginePointer->engineResetID = PortPointer->EngineData.GpioGroupId;
            }
            EnginePointer->EngineType = PortPointer->EngineData.EngineType;
            EnginePointer->HotPluggable = PortPointer->EngineData.HotPluggable;
            switch (PortPointer->EngineData.EngineType) {
            case DxioPcieEngine:
              EnginePointer->LinkDesc.Pcie.Capabilities.earlyTrainLink = PortPointer->Port.MiscControls.SbLink;
              IDS_HDT_CONSOLE (PCIE_MISC, "PsppPolicy is %d and looking for %d\n", Pcie->PsppPolicy, AgesaPsppBalance);
              if ((EnginePointer->LinkDesc.Pcie.Capabilities.earlyTrainLink == 1) || (Pcie->PsppPolicy == AgesaPsppBalance)) {
                EnginePointer->LinkDesc.Pcie.Capabilities.maxLinkSpeedCap = DxioGen1;
                IDS_HDT_CONSOLE (PCIE_MISC, "  Forcing Gen1\n");
              } else {
                EnginePointer->LinkDesc.Pcie.Capabilities.maxLinkSpeedCap = PortPointer->Port.LinkSpeedCapability;
              }
              if (PortPointer->Port.EqPreset != 0) {
                EnginePointer->LinkDesc.Pcie.Capabilities.eqSearchModeOride = 1;
                EnginePointer->LinkDesc.Pcie.Capabilities.eqSearchMode = PortPointer->Port.EqPreset;
                IDS_HDT_CONSOLE (GNB_TRACE, "EqSearchMode: %d\n",
                    EnginePointer->LinkDesc.Pcie.Capabilities.eqSearchMode);
              }
              if (PortPointer->Port.EsmControl == TRUE) {
                EnginePointer->LinkDesc.Pcie.Capabilities.maxLinkSpeedCap = DxioGen3;
                IDS_HDT_CONSOLE (GNB_TRACE, "  Forcing Gen3 on this PCIe port for ESM sequence later. \n");
              }
              if (PortPointer->Port.TXDeEmphasisOride) {
                EnginePointer->LinkDesc.Pcie.Capabilities.TXDeEmphasisOride = 1;
                EnginePointer->LinkDesc.Pcie.Capabilities.TXDeEmphasis = PortPointer->Port.TXDeEmphasis;
              }
              EnginePointer->LinkDesc.Pcie.DevFunNumber = (PortPointer->Port.DeviceNumber << 3) | PortPointer->Port.FunctionNumber;
              EnginePointer->LinkDesc.Pcie.Capabilities.enableTurnOffSettings = 1;
              EnginePointer->LinkDesc.Pcie.Capabilities.turnOffUnusedLanes = PortPointer->Port.MiscControls.TurnOffUnusedLanes;
              EnginePointer->LinkDesc.Pcie.Capabilities.portPresent = PortPointer->Port.PortPresent;
              EnginePointer->LinkDesc.Pcie.Capabilities.linkHotplug = PortPointer->Port.LinkHotplug;
              EnginePointer->LinkDesc.Pcie.Capabilities.linkReversal = Reverse;
              EnginePointer->LinkDesc.Pcie.Capabilities.isMasterPLL = PortPointer->Port.IsMasterPLL;
              EnginePointer->LinkDesc.Pcie.Capabilities.linkComplianceMode = PortPointer->Port.MiscControls.LinkComplianceMode;
              EnginePointer->LinkDesc.Pcie.Capabilities.invertPolarity = PortPointer->Port.InvertPolarity;
              EnginePointer->LinkDesc.Pcie.Capabilities.invertPolarity2 = PortPointer->Port.InvertPolarity2;
              EnginePointer->LinkDesc.Pcie.Capabilities.targetLinkSpeed = PortPointer->Port.TargetLinkSpeed;
              EnginePointer->LinkDesc.Pcie.Capabilities.bypassGen3EQ = PortPointer->Port.BypassGen3EQ;
              EnginePointer->LinkDesc.Pcie.Capabilities.bypassGen4EQ = PortPointer->Port.BypassGen4EQ;

              if (PortPointer->Port.CsLink == 1) {
                EnginePointer->forceKpnpReset = 1;
                IDS_HDT_CONSOLE (PCIE_MISC, "Dev%d Func%d: forceKpnpReset=%d CsLink=%d \n", PortPointer->Port.DeviceNumber, PortPointer->Port.FunctionNumber, EnginePointer->forceKpnpReset, PortPointer->Port.CsLink);
              }
              EnginePointer->LinkDesc.Pcie.Capabilities.txVetting = PortPointer->Port.TxVetting;
              EnginePointer->LinkDesc.Pcie.Capabilities.rxVetting = PortPointer->Port.RxVetting;
              EnginePointer->LinkDesc.Pcie.Capabilities.ntbHotPlug = PortPointer->Port.NtbHotplug;

              DataSize = DxioAncDataPopulate (EnginePointer, PortPointer, AncillaryData);
              if (0 != DataSize) {
                EnginePointer->LinkDesc.Pcie.AncillaryDataOffset = AncillaryDataOffset >> 2;
                AncillaryData += DataSize;
                AncillaryDataOffset += DataSize;
              }
              break;
            case DxioSATAEngine:
              EnginePointer->LinkDesc.Sata.ChannelType = PortPointer->Port.MiscControls.ChannelType;
              EnginePointer->LinkDesc.Sata.Capabilities.invertPolarity = PortPointer->Port.InvertPolarity;
              EnginePointer->LinkDesc.Sata.Capabilities.invertPolarity2 = PortPointer->Port.InvertPolarity2;
              DataSize = DxioAncDataPopulate (EnginePointer, PortPointer, AncillaryData);
              if (0 != DataSize) {
                EnginePointer->LinkDesc.Sata.AncillaryDataOffset = AncillaryDataOffset >> 2;
                AncillaryData += DataSize;
                AncillaryDataOffset += DataSize;
              }
              break;
            case DxioEthernetEngine:
              EnginePointer->LinkDesc.Eth.AncillaryDataOffset = EtherNetPortOffset >> 2;
              EtherNetPortHeap += (PortPointer->EtherNet.EthPortProp0.PortNum * EthernetDataSize);
              LibAmdMemCopy ((VOID *) EtherNetPortHeap,
                           (VOID *) &PortPointer->EtherNet,
                           EthernetDataSize,
                           (AMD_CONFIG_PARAMS *)NULL
                           );
              EtherNetPortHeap -= (PortPointer->EtherNet.EthPortProp0.PortNum * EthernetDataSize);;
              break;
            default:
              break;
            }
          }
          EnginePointer++;
        }
        PortPointer = PcieConfigGetNextDataDescriptor(PortPointer);
      }
    } else {
      // Nothing found for this node, skip DXIO call
      *ThisNode = NULL;             // Save pointer to allocated memory
      *NodeSize = 0;      // Platfom Descriptor size in dwords
    }
  }
  // Return pointer to this node platform structure
  return (AgesaStatus);
}



