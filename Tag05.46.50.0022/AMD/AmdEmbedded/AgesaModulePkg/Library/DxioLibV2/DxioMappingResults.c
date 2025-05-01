/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * DXIO port mapping update
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

#define FILECODE LIBRARY_DXIOLIBV2_DXIOMAPPINGRESULTS_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

///Port information needed for callback
typedef struct {
  UINT8                   SocketNumber;       ///< SocketNumber
  UINT8                   StartLane;          ///< Translated start lane
  UINT8                   EngineID;           ///< Index of EngineID
  UINT8                   MacPortID;          ///< MacPortID from DXIO firmware
  DXIO_PORT_DESCRIPTOR    *UserCfgEntry;      ///< Pointer into UserCfgPortList
  DXIO_ENGINE_DESCRIPTOR  *DxioEntry;         ///< Pointer into DXIO Engine Config
  UINT8                   *PortDevMap;        ///< Pointer to array of port device mapping
  PCIe_ENGINE_CONFIG      *EarlyTrainEngine;  ///< Pointer to early train engine
} PORT_INFO_CALLBACK;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/*
 *  Pcie SRIS Platform Configuration
 *
 * @param[in]       TopologyEntry   Pointer to platform config
 * @param[in, out]  Engine          Pointer to Engine config descriptor
 *
 */
STATIC
VOID
SrisPlatformConfig (
  IN      DXIO_PORT_DESCRIPTOR    *TopologyEntry,
  IN      PCIe_ENGINE_CONFIG      *Engine
  )
{

  // SRIS Mode
  // Default PCD config for all port
  if (PcdGet8 (PcdSrisEnableMode) != 0xFF) {
    Engine->Type.Port.SrisEnableMode = PcdGet8 (PcdSrisEnableMode);
    Engine->Type.Port.SrisSkipInterval = PcdGet8 (PcdSrisSkipInterval);
    Engine->Type.Port.LowerSkpOsGenSup = PcdGet8 (PcdSrisLowerSkpOsGenSup);
    Engine->Type.Port.LowerSkpOsRcvSup = PcdGet8 (PcdSrisLowerSkpOsRcvSup);
  }
  // CPM PORT_PARAM per-port config
  if (TopologyEntry->Port.SrisEnableMode) {
    Engine->Type.Port.SrisEnableMode = TopologyEntry->Port.SrisEnableMode;
    Engine->Type.Port.SrisSkipInterval = TopologyEntry->Port.SrisSkipInterval;
    Engine->Type.Port.LowerSkpOsGenSup = TopologyEntry->Port.LowerSkpOsGenSup;
    Engine->Type.Port.LowerSkpOsRcvSup = TopologyEntry->Port.LowerSkpOsRcvSup;
  }

  // SRIS Autodetect
  // Default PCD config for all port
  if (PcdGet8 (PcdSrisAutoDetectMode) != 0xFF) {
    Engine->Type.Port.SrisAutoDetectMode = PcdGet8 (PcdSrisAutoDetectMode);
    Engine->Type.Port.SrisSkpIntervalSel = PcdGet8 (PcdSrisSkpIntervalSel);
    Engine->Type.Port.SrisAutodetectFactor = PcdGet8 (PcdSrisAutodetectFactor);
  }
  // CPM PORT_PARAM per-port config
  if (TopologyEntry->Port.SrisAutoDetectMode) {
    Engine->Type.Port.SrisAutoDetectMode = TopologyEntry->Port.SrisAutoDetectMode;
    Engine->Type.Port.SrisSkpIntervalSel = TopologyEntry->Port.SrisSkpIntervalSel;
    Engine->Type.Port.SrisAutodetectFactor = TopologyEntry->Port.SrisAutodetectFactor;
  }
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
 *
 */
AGESA_STATUS
STATIC
DxioMapPortCallback (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,
  IN      GNB_HANDLE            *GnbHandle
  )
{
  PORT_INFO_CALLBACK      *PortInfo;
  PCIe_ENGINE_CONFIG      *Engine;
  DXIO_PORT_DESCRIPTOR    *UserCfg;
  DXIO_ENGINE_DESCRIPTOR  *DxioEntry;

  PortInfo = (PORT_INFO_CALLBACK *) Buffer;
  if (GnbHandle->SocketId == PortInfo->SocketNumber) {
    if ((PortInfo->StartLane >= Wrapper->StartDxioLane) && (PortInfo->StartLane <= Wrapper->EndDxioLane)) {
      // Find the PCIe structure entry for this DXIO engine
      Engine = PcieConfigGetChildEngine (Wrapper);
      while (Engine != NULL) {
        if (Engine->Type.Port.PortId == PortInfo->MacPortID) {
          UserCfg = PortInfo->UserCfgEntry;
          DxioEntry = PortInfo->DxioEntry;

          // From DxioEntry
          Engine->EngineData.EngineType = (UINT8) DxioEntry->EngineType;

          // From Platform Topology
          Engine->EngineData.StartLane = (UINT16) UserCfg->EngineData.StartLane;
          Engine->EngineData.EndLane = (UINT16) UserCfg->EngineData.EndLane;
          Engine->Type.Port.PortData.DeviceNumber = UserCfg->Port.DeviceNumber;
          Engine->Type.Port.PortData.FunctionNumber = UserCfg->Port.FunctionNumber;
          Engine->Type.Port.PortData.LinkSpeedCapability = UserCfg->Port.LinkSpeedCapability;
          Engine->Type.Port.PortData.LinkAspm = UserCfg->Port.LinkAspm;
          Engine->Type.Port.PortData.LinkHotplug = UserCfg->Port.LinkHotplug;
          Engine->Type.Port.LinkAspmL1_1 = UserCfg->Port.LinkAspmL1_1;
          Engine->Type.Port.LinkAspmL1_2 = UserCfg->Port.LinkAspmL1_2;
          Engine->Type.Port.DlfCapDisable = ((UserCfg->Port.Gen4Features & BIT1)? 1 : 0);
          Engine->Type.Port.DlfExchangeDisable = (UserCfg->Port.Gen4Features & BIT0);
          if (UserCfg->Port.SlotPowerLimit != 0) {
            Engine->Type.Port.SlotPowerLimit = UserCfg->Port.SlotPowerLimit;
          }
          if (UserCfg->Port.SlotPowerLimitScale != 0) {
            Engine->Type.Port.SlotPowerLimitScale = UserCfg->Port.SlotPowerLimitScale;
          }
          Engine->Type.Port.ClkReq = UserCfg->Port.ClkReq;
          Engine->Type.Port.PortData.MiscControls.ClkPmSupport = UserCfg->Port.MiscControls.ClkPmSupport;
          Engine->Type.Port.PortData.MiscControls.SbLink = UserCfg->Port.MiscControls.SbLink;
          Engine->Type.Port.PortData.MiscControls.CsLink = (UINT8) (UserCfg->Port.CsLink);
          Engine->Type.Port.IsMasterPLL = UserCfg->Port.IsMasterPLL;
          Engine->Type.Port.EqSearchMode = UserCfg->Port.EqPreset;
          Engine->Type.Port.PortData.SlotNum = UserCfg->Port.SlotNum;
          Engine->Type.Port.EsmControl = UserCfg->Port.EsmControl;
          Engine->Type.Port.CcixControl = UserCfg->Port.CcixControl;
          Engine->Type.Port.RxVetting = UserCfg->Port.RxVetting;
          Engine->Type.Port.TxVetting = UserCfg->Port.TxVetting;
          Engine->Type.Port.EsmSpeedBump = UserCfg->Port.EsmSpeedBump;
          if (UserCfg->Port.SetGen3LaneEq) {
            Engine->Type.Port.LaneEqualizationCntl.DsTxPreset = UserCfg->Port.LaneEqualizationCntl.DsTxPreset;
            Engine->Type.Port.LaneEqualizationCntl.DsRxPresetHint = UserCfg->Port.LaneEqualizationCntl.DsRxPresetHint;
            Engine->Type.Port.LaneEqualizationCntl.UsTxPreset = UserCfg->Port.LaneEqualizationCntl.UsTxPreset;
            Engine->Type.Port.LaneEqualizationCntl.UsRxPresetHint = UserCfg->Port.LaneEqualizationCntl.UsRxPresetHint;
          }
          if (UserCfg->Port.SetGen4LaneEq) {
            Engine->Type.Port.Gen4LaneEqualizationCntl.DsTxPreset = UserCfg->Port.Gen4LaneEqualizationCntl.DsTxPreset;
            Engine->Type.Port.Gen4LaneEqualizationCntl.UsTxPreset = UserCfg->Port.Gen4LaneEqualizationCntl.UsTxPreset;
          }
          if (UserCfg->Port.SetGen3FixedPreset) {
            Engine->Type.Port.SetGen3FixedPreset = UserCfg->Port.SetGen3FixedPreset;
            Engine->Type.Port.Gen3FixedPreset = UserCfg->Port.Gen3FixedPreset;
          }
          if (UserCfg->Port.SetGen4FixedPreset) {
            Engine->Type.Port.SetGen4FixedPreset = UserCfg->Port.SetGen4FixedPreset;
            Engine->Type.Port.Gen4FixedPreset = UserCfg->Port.Gen4FixedPreset;
          }
          Engine->Type.Port.PsppPolicyDC = UserCfg->Port.PsppPolicyDC;
          Engine->Type.Port.PsppPolicyAC = UserCfg->Port.PsppPolicyAC;
          Engine->Type.Port.InvertPolarity = UserCfg->Port.InvertPolarity;
          Engine->Type.Port.InvertPolarity2 = UserCfg->Port.InvertPolarity2;
          Engine->Type.Port.DisGen3EQPhase = UserCfg->Port.DisGen3EQPhase;
          Engine->Type.Port.DisGen4EQPhase = UserCfg->Port.DisGen4EQPhase;
          Engine->Type.Port.TargetLinkSpeed = UserCfg->Port.TargetLinkSpeed;
          Engine->Type.Port.BypassGen3EQ = UserCfg->Port.BypassGen3EQ;
          Engine->Type.Port.BypassGen4EQ = UserCfg->Port.BypassGen4EQ;

          SrisPlatformConfig(UserCfg, Engine);

          PcieConfigSetDescriptorFlags (Engine, DESCRIPTOR_ALLOCATED);
          Engine->Type.Port.PortData.PortPresent = UserCfg->Port.PortPresent;
          // If the PCIe port is disabled then don't add to the port mapping
          if(Engine->Type.Port.PortData.PortPresent){
            PcieMapPortPciAddress (Engine, PortInfo->PortDevMap + (18 * GnbHandle->RBIndex));
            Engine->Type.Port.Address.AddressValue = MAKE_SBDFO (
                                                 0,
                                                 GnbHandle->Address.Address.Bus,
                                                 Engine->Type.Port.PortData.DeviceNumber,
                                                 Engine->Type.Port.PortData.FunctionNumber,
                                                 0
                                                 );
            Engine->Type.Port.PcieBridgeId = PortInfo->EngineID;
          }
          if (DxioEntry->LinkDesc.Pcie.Capabilities.earlyTrainLink == 1) {
            PortInfo->EarlyTrainEngine = Engine;
          }
        }
        Engine = PcieLibGetNextDescriptor (Engine);
      }
    }
  }
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * DXIO Update Pcie Data Structure
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]  UserConfig     Pointer to the platform BIOS supplied platform configuration
 * @param[in]  ThisNode       Pointer to the platform descriptor for this node
 * @retval     AGESA_STATUS
 */
VOID
DxioPortMapping (
  IN      GNB_HANDLE                *GnbHandle,
  IN      DXIO_COMPLEX_DESCRIPTOR   *UserConfig,
  IN      DXIO_PLATFORM_DESCRIPTOR  *ThisNode,
  IN OUT  DXIO_ENGINE_DESCRIPTOR    **EarlyTrain,
  IN OUT  UINT32                    *EarlyTrainAddress,
  IN OUT  PCIe_ENGINE_CONFIG        **EarlyTrainEngine
  )
{
  // DXIO Firmware Structures
  DXIO_PLATFORM_DESCRIPTOR  *DxioNodePlatformDesc;
  DXIO_ENGINE_DESCRIPTOR    *DxioEnginePointer;
  UINT32                    DxioNodeCount;
  //User Config Pointers
  DXIO_PORT_DESCRIPTOR      *UserCfgPortList;              // Local pointer to start of User Config for this socket
  DXIO_PORT_DESCRIPTOR      *UserCfgPortPointer;           // Local updatable pointer for UserCfgPortList processing

  UINT8                     SocketNumber;
  UINT8                     Index;
  UINT8                     PortDevMap[18 * 4];
  PORT_INFO_CALLBACK        PortInfo;
  GNB_HANDLE                *LocalHandle;
  AGESA_STATUS              AgesaStatus;

  IDS_HDT_CONSOLE (PCIE_MISC, "DxioPortMapping enter\n");

  SocketNumber = GnbHandle->SocketId;
  AgesaStatus = DxioGetUserCfgForSocket (SocketNumber, UserConfig, &UserCfgPortList);

  if (AGESA_SUCCESS == AgesaStatus) {
    DxioNodePlatformDesc = ThisNode;
    DxioNodeCount = DxioNodePlatformDesc->NumEngineDesc;
    DxioNodePlatformDesc ++;                                              // Point to first engine descriptor
    DxioEnginePointer = (DXIO_ENGINE_DESCRIPTOR *)DxioNodePlatformDesc;       // Initialize engine pointer to first engine

    LibAmdMemFill (PortDevMap, 0, sizeof (PortDevMap), (AMD_CONFIG_PARAMS *) NULL);

    for (Index = 0;Index < DxioNodeCount;Index++) {
      if (DxioEnginePointer->EngineType == DxioPcieEngine) {
        IDS_HDT_CONSOLE (PCIE_MISC, "Looking for StartLane %d\n", DxioEnginePointer->StartLane);
        // Find the UserConfig entry for this DXIO engine
        UserCfgPortPointer = UserCfgPortList;       // Start of the port list for this socket
        while (UserCfgPortPointer != NULL) {
          IDS_HDT_CONSOLE (PCIE_MISC, "  Checking User Entry StartLane %d\n", UserCfgPortPointer->EngineData.DxioStartLane);
          if (UserCfgPortPointer->EngineData.EngineType == DxioPcieEngine) {
            if (UserCfgPortPointer->EngineData.DxioStartLane == DxioEnginePointer->StartLane) {
              break;
            }
          }
          UserCfgPortPointer = PcieConfigGetNextDataDescriptor(UserCfgPortPointer);
        }
        ASSERT (UserCfgPortPointer != NULL);
        if (UserCfgPortPointer != NULL) {
          PortInfo.SocketNumber = SocketNumber;
          PortInfo.StartLane = (UINT8)DxioEnginePointer->StartLane;
          PortInfo.MacPortID = (UINT8)DxioEnginePointer->LinkDesc.Pcie.MacPortID;
          PortInfo.EngineID = Index;
          PortInfo.UserCfgEntry = UserCfgPortPointer;
          PortInfo.DxioEntry = DxioEnginePointer;
          PortInfo.PortDevMap = PortDevMap;
          PortInfo.EarlyTrainEngine = NULL;
          LocalHandle = GnbHandle;
          while (LocalHandle != NULL) {
            PcieConfigRunProcForAllWrappersInNbio (DESCRIPTOR_ALL_WRAPPERS, (PCIe_RUN_ON_WRAPPER_CALLBACK2) DxioMapPortCallback, (VOID *) &PortInfo, LocalHandle);
            LocalHandle = GnbGetNextHandle (LocalHandle);
          }

          if (DxioEnginePointer->LinkDesc.Pcie.Capabilities.earlyTrainLink == 1) {
            *EarlyTrain = DxioEnginePointer;
            *EarlyTrainAddress = DxioEnginePointer->LinkDesc.Pcie.DevFunNumber;
            *EarlyTrainEngine = PortInfo.EarlyTrainEngine;
          }
        }
      }
      DxioEnginePointer++;
    }
    LocalHandle = GnbHandle;
    while (LocalHandle != NULL) {
      if (LocalHandle->SocketId == SocketNumber) {
        PcieSetPortPciAddressMap(LocalHandle, PortDevMap + (LocalHandle->RBIndex * 18));
      }
      LocalHandle = GnbGetNextHandle(LocalHandle);
    }
  }
  return;
}




