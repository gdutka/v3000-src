/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/**
 *  @file MpioMappingResults.c
 *  @brief Mpio port mapping update
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

#define FILECODE NBIO_LIBRARY_MPIOINITLIB_MPIOMAPPINGRESULTS_FILECODE
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
STATIC
DXIO_PORT_DESCRIPTOR *
MpioFindTopoForAsk (
  IN     DXIO_PORT_DESCRIPTOR  *SocketTopology,
  IN     FW_ASK_STRUCT         *AskEntry
  )
{
  DXIO_PORT_DESCRIPTOR      *TopologyEntry;               // Local updatable pointer for SocketTopology processing

  IDS_HDT_CONSOLE (GNB_TRACE, "Looking for StartLane %d\n", AskEntry->desc.startLaneIndex);
  // Find the PlatformTopology entry for this DXIO engine
  TopologyEntry = SocketTopology;       // Start of the port list for this socket
  while (TopologyEntry != NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "  Checking User Entry StartLane %d\n", TopologyEntry->EngineData.DxioStartLane);

    // Ignore USB4 OVER PCIE
    if (TopologyEntry->EngineData.EngineType == DxioUSB_OVER_PCIE) {
      break;
    }

    if (TopologyEntry->EngineData.EngineType == DxioPcieEngine) {
      if (TopologyEntry->EngineData.DxioStartLane == AskEntry->desc.startLaneIndex) {
        break;
      }
    }
    TopologyEntry = PcieConfigGetNextDataDescriptor(TopologyEntry);
  }
  ASSERT (TopologyEntry != NULL);
  return (TopologyEntry);
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Pcie SRIS Platform Configuration
 *
 * @param[in]       TopologyEntry   Pointer to platform config
 * @param[in, out]  Engine          Pointer to Engine config descriptor
 * @param[in]       AskEntry        Not used
 *
 */
STATIC
VOID
SrisPlatformConfig (
  IN      DXIO_PORT_DESCRIPTOR    *TopologyEntry,
  IN      PCIe_ENGINE_CONFIG      *Engine,
  IN      FW_ASK_STRUCT           *AskEntry
  )
{
  // SRIS Mode
  // Default PCD config for all port
  if ((PcdGet8 (PcdSrisEnableMode) != 0xFF) && ((PcdGet8 (PcdSrisCfgType) & 0x7) == 0)) {
    Engine->Type.Port.SrisEnableMode = PcdGet8 (PcdSrisEnableMode);
    Engine->Type.Port.SrisSkipInterval = PcdGet8 (PcdSrisSkipInterval);
    Engine->Type.Port.LowerSkpOsGenSup = PcdGet8 (PcdSrisLowerSkpOsGenSup);
    Engine->Type.Port.LowerSkpOsRcvSup = PcdGet8 (PcdSrisLowerSkpOsRcvSup);
  }
  // CPM PORT_PARAM per-port config
  if (TopologyEntry->Port.SrisEnableMode && ((PcdGet8 (PcdSrisCfgType) & 0x7) == 0)) {
    Engine->Type.Port.SrisEnableMode = TopologyEntry->Port.SrisEnableMode;
    Engine->Type.Port.SrisSkipInterval = TopologyEntry->Port.SrisSkipInterval;
    Engine->Type.Port.LowerSkpOsGenSup = TopologyEntry->Port.LowerSkpOsGenSup;
    Engine->Type.Port.LowerSkpOsRcvSup = TopologyEntry->Port.LowerSkpOsRcvSup;
  }
  // CBS Common, CBS Debug, PBS config
  if (PcdGet8 (PcdSrisCfgType) & 0x7) {
    Engine->Type.Port.SrisEnableMode = TopologyEntry->Port.SrisEnableMode;
  }
  // CBS Debug, PBS config
  if (PcdGet8 (PcdSrisCfgType) & 0x6) {
    Engine->Type.Port.SrisSkipInterval = TopologyEntry->Port.SrisSkipInterval;
    Engine->Type.Port.LowerSkpOsGenSup = TopologyEntry->Port.LowerSkpOsGenSup;
    Engine->Type.Port.LowerSkpOsRcvSup = TopologyEntry->Port.LowerSkpOsRcvSup;
  }

  // SRIS Autodetect
  // Default PCD config for all port
  if ((PcdGet8 (PcdSrisAutoDetectMode) != 0xFF) && ((PcdGet8 (PcdSrisCfgType) & 0x8) == 0)) {
    Engine->Type.Port.SrisAutoDetectMode = PcdGet8 (PcdSrisAutoDetectMode);
    Engine->Type.Port.SrisSkpIntervalSel = PcdGet8 (PcdSrisSkpIntervalSel);
    Engine->Type.Port.SrisAutodetectFactor = PcdGet8 (PcdSrisAutodetectFactor);
  }
  // CPM PORT_PARAM per-port config
  if (TopologyEntry->Port.SrisAutoDetectMode && ((PcdGet8 (PcdSrisCfgType) & 0x8) == 0)) {
    Engine->Type.Port.SrisAutoDetectMode = TopologyEntry->Port.SrisAutoDetectMode;
    Engine->Type.Port.SrisSkpIntervalSel = TopologyEntry->Port.SrisSkpIntervalSel;
    Engine->Type.Port.SrisAutodetectFactor = TopologyEntry->Port.SrisAutodetectFactor;
  }
  // PBS config
  if (PcdGet8 (PcdSrisCfgType) & 0x8) {
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
 * @param[in, out]  Buffer          Not used
 * @param[in]       GnbHandle       Pointer to GNB_HANDLE
 *
 */
STATIC
VOID
MpioInitEngineStruct (
  IN      DXIO_PORT_DESCRIPTOR    *TopologyEntry,
  IN      PCIe_ENGINE_CONFIG      *Engine,
  IN      FW_ASK_STRUCT           *AskEntry
  )
{

  Engine->EngineData.EngineType = DxioPcieEngine;

  // From Platform Topology
  Engine->EngineData.StartLane = (UINT16) TopologyEntry->EngineData.StartLane;
  Engine->EngineData.EndLane = (UINT16) TopologyEntry->EngineData.EndLane;

  Engine->Type.Port.PortData.DeviceNumber = TopologyEntry->Port.DeviceNumber;
  Engine->Type.Port.PortData.FunctionNumber = TopologyEntry->Port.FunctionNumber;
  Engine->Type.Port.PortData.LinkSpeedCapability = TopologyEntry->Port.LinkSpeedCapability;
  Engine->Type.Port.PortData.LinkAspm = TopologyEntry->Port.LinkAspm;
  Engine->Type.Port.PortData.LinkHotplug = TopologyEntry->Port.LinkHotplug;
  Engine->Type.Port.LinkAspmL1_1 = TopologyEntry->Port.LinkAspmL1_1;
  Engine->Type.Port.LinkAspmL1_2 = TopologyEntry->Port.LinkAspmL1_2;
  Engine->Type.Port.DlfCapDisable = ((TopologyEntry->Port.Gen4Features & BIT1)? 1 : 0);
  Engine->Type.Port.DlfExchangeDisable = (TopologyEntry->Port.Gen4Features & BIT0);
  if (TopologyEntry->Port.SlotPowerLimit != 0) {
    Engine->Type.Port.SlotPowerLimit = TopologyEntry->Port.SlotPowerLimit;
  }
  if (TopologyEntry->Port.SlotPowerLimitScale != 0) {
    Engine->Type.Port.SlotPowerLimitScale = TopologyEntry->Port.SlotPowerLimitScale;
  }
  Engine->Type.Port.ClkReq = TopologyEntry->Port.ClkReq;
  Engine->Type.Port.PortData.MiscControls.ClkPmSupport = TopologyEntry->Port.MiscControls.ClkPmSupport;
  Engine->Type.Port.PortData.MiscControls.SbLink = TopologyEntry->Port.MiscControls.SbLink;
  Engine->Type.Port.PortData.MiscControls.CsLink = (UINT8) (TopologyEntry->Port.CsLink);
  Engine->Type.Port.IsMasterPLL = TopologyEntry->Port.IsMasterPLL;
  Engine->Type.Port.EqSearchMode = TopologyEntry->Port.EqPreset;
  Engine->Type.Port.PortData.SlotNum = TopologyEntry->Port.SlotNum;
  Engine->Type.Port.EsmControl = TopologyEntry->Port.EsmControl;
  Engine->Type.Port.CcixControl = TopologyEntry->Port.CcixControl;
  Engine->Type.Port.CxlControl = TopologyEntry->Port.CcixControl; // Share the topology settingfor now
  Engine->Type.Port.RxVetting = TopologyEntry->Port.RxVetting;
  Engine->Type.Port.TxVetting = TopologyEntry->Port.TxVetting;
  Engine->Type.Port.EsmSpeedBump = TopologyEntry->Port.EsmSpeedBump;
  if (TopologyEntry->Port.SetGen3LaneEq) {
    Engine->Type.Port.LaneEqualizationCntl.DsTxPreset = TopologyEntry->Port.LaneEqualizationCntl.DsTxPreset;
    Engine->Type.Port.LaneEqualizationCntl.DsRxPresetHint = TopologyEntry->Port.LaneEqualizationCntl.DsRxPresetHint;
    Engine->Type.Port.LaneEqualizationCntl.UsTxPreset = TopologyEntry->Port.LaneEqualizationCntl.UsTxPreset;
    Engine->Type.Port.LaneEqualizationCntl.UsRxPresetHint = TopologyEntry->Port.LaneEqualizationCntl.UsRxPresetHint;
  }
  if (TopologyEntry->Port.SetGen4LaneEq) {
    Engine->Type.Port.Gen4LaneEqualizationCntl.DsTxPreset = TopologyEntry->Port.Gen4LaneEqualizationCntl.DsTxPreset;
    Engine->Type.Port.Gen4LaneEqualizationCntl.UsTxPreset = TopologyEntry->Port.Gen4LaneEqualizationCntl.UsTxPreset;
  }
  if (TopologyEntry->Port.SetGen3FixedPreset) {
    Engine->Type.Port.SetGen3FixedPreset = TopologyEntry->Port.SetGen3FixedPreset;
    Engine->Type.Port.Gen3FixedPreset = TopologyEntry->Port.Gen3FixedPreset;
  }
  if (TopologyEntry->Port.SetGen4FixedPreset) {
    Engine->Type.Port.SetGen4FixedPreset = TopologyEntry->Port.SetGen4FixedPreset;
    Engine->Type.Port.Gen4FixedPreset = TopologyEntry->Port.Gen4FixedPreset;
  }
  Engine->Type.Port.PsppPolicyDC = TopologyEntry->Port.PsppPolicyDC;
  Engine->Type.Port.PsppPolicyAC = TopologyEntry->Port.PsppPolicyAC;
  Engine->Type.Port.InvertPolarity = TopologyEntry->Port.InvertPolarity;
  Engine->Type.Port.InvertPolarity2 = TopologyEntry->Port.InvertPolarity2;
  Engine->Type.Port.DisGen3EQPhase = TopologyEntry->Port.DisGen3EQPhase;
  Engine->Type.Port.DisGen4EQPhase = TopologyEntry->Port.DisGen4EQPhase;
  Engine->Type.Port.TargetLinkSpeed = TopologyEntry->Port.TargetLinkSpeed;
  Engine->Type.Port.BypassGen3EQ = TopologyEntry->Port.BypassGen3EQ;
  Engine->Type.Port.BypassGen4EQ = TopologyEntry->Port.BypassGen4EQ;
  Engine->Type.Port.AlwaysExpose = TopologyEntry->Port.AlwaysExpose;
  Engine->Type.Port.I2CMuxInfo = TopologyEntry->Port.I2CMuxInfo;

  SrisPlatformConfig (TopologyEntry, Engine, AskEntry);

  PcieConfigSetDescriptorFlags (Engine, DESCRIPTOR_ALLOCATED);
  Engine->Type.Port.PortData.PortPresent = TopologyEntry->Port.PortPresent;
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * DXIO Update Pcie Data Structure
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]  PlatformTopology     Pointer to the platform BIOS supplied platform configuration
 * @param[in]  ThisNode       Pointer to the platform descriptor for this node
 * @retval     AGESA_STATUS
 */
VOID
MpioPortMapping (
  IN      GNB_HANDLE                *GnbHandle,
  IN      DXIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  IN      MPIO_DATA                 *MpioData
  )
{
  GNB_HANDLE                *Handle;
  AGESA_STATUS              AgesaStatus;
  UINT32                    PortDevSize;
  UINT8                     *PortDevMap;
  UINT8                     Index;

  // DXIO Firmware Structures
  FW_ASK_STRUCT             *AskEntry;
  // Platform Topology Pointers
  DXIO_PORT_DESCRIPTOR      *SocketTopology;              // Local pointer to start of User Config for this socket
  DXIO_PORT_DESCRIPTOR      *TopologyEntry;               // Local updatable pointer for SocketTopology processing
  // PCIe Complex Pointers
  PCIe_ENGINE_CONFIG        *Engine;


  IDS_HDT_CONSOLE (GNB_TRACE, "DxioPortMapping enter\n");


  AgesaStatus = MpioGetTopologyForSocket (GnbHandle->SocketId, PlatformTopology, &SocketTopology);

  if (AGESA_SUCCESS == AgesaStatus) {
    AskEntry = MpioData->MpioAsk;

    // Allocaate and Initialize PortDevMap
    PortDevSize = PcieGetDeviceMappingSize();
    PortDevMap = (UINT8 *) AllocatePool (MAX_INSTANCE_ID * PortDevSize);
    LibAmdMemFill (PortDevMap, 0, MAX_INSTANCE_ID * PortDevSize, (AMD_CONFIG_PARAMS *) NULL);

    // Sync Platform Topology, Ask, and Complex Entries
    for (Index = 0; Index < MpioData->MpioAskCount; Index++) {
      if (AskEntry->desc.ctrlType == ASK_TYPE_PCIe) {
        TopologyEntry = MpioFindTopoForAsk (SocketTopology, AskEntry);
        if (TopologyEntry != NULL) {
          Engine = MpioFindEngineForAsk (GnbHandle, AskEntry);
          if (Engine != NULL) {
            MpioInitEngineStruct (TopologyEntry, Engine, AskEntry);
            if(Engine->Type.Port.PortData.PortPresent) {
              Handle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);
              PcieMapPortPciAddress (Engine, PortDevMap + (Handle->RBIndex * PortDevSize));
               Engine->Type.Port.Address.AddressValue = MAKE_SBDFO (
                                         Handle->Address.Address.Segment,
                                         Handle->Address.Address.Bus,
                                         Engine->Type.Port.PortData.DeviceNumber,
                                         Engine->Type.Port.PortData.FunctionNumber,
                                         0
                                         );
            }
          }
        }
      }
      AskEntry++;
    }
    Handle = GnbHandle;
    while (Handle != NULL) {
      if (Handle->SocketId == GnbHandle->SocketId) {
        PcieSetPortPciAddressMap(Handle, PortDevMap + (Handle->RBIndex * PortDevSize));
      }
      Handle = GnbGetNextHandle(Handle);
    }
  }
  return;
}




