/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * DXIO Ancillary Data handlers
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */

#ifndef _DXIOANCDATAV2_H_
#define _DXIOANCDATAV2_H_

typedef struct {
  PCI_ADDR                  EarlyTrainAddress;  ///< PCI address of the early train port
  DXIO_ENGINE_DESCRIPTOR    *EarlyTrainDesc;    ///< Pointer to the early train DXIO descriptor
  PCIe_ENGINE_CONFIG        *EarlyTrainEngine;  ///< Pointer to the early train engine
  BOOLEAN                   EarlyLinkMapped;    ///< Early Train status
} EARLY_TRAIN_DATA;


// Configuration Points from APU specific driver
UINT32
DxioCfgGetStrapListSize (
  IN       GNB_HANDLE            *GnbHandle,
  IN       DXIO_PORT_DESCRIPTOR  *PortPointer
  );

UINT32
DxioCfgAddStrapList (
  IN       GNB_HANDLE             *GnbHandle,
  IN       VOID                   *AncillaryData,
  IN       DXIO_PORT_DESCRIPTOR   *PortList
  );

VOID
DxioCfgAfterInitInit (
  IN       GNB_HANDLE                     *GnbHandle,
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *SmuServicesPpi,
  IN       UINT8                          InstanceId
  );

VOID
DxioCfgBeforeReconfig (
  IN       GNB_HANDLE       *GnbHandle
  );

VOID
DxioCfgAfterReconfig (
  IN       GNB_HANDLE       *GnbHandle
  );

BOOLEAN
SetPCIeTrainingTimers(
  IN OUT   UINT32*  LinkResetToTrainingTime,
  IN OUT   UINT32*  LinkReceiverDetectionPolling,
  IN OUT   UINT32*  LinkL0Polling
  );

// DXIO Debug Output Functions
VOID
DxioDebugDump (
  IN       DXIO_PLATFORM_DESCRIPTOR  *PlatformDescriptor,
  IN       UINT32                    PlatformDescSize,
  IN       UINT8                     *AncillaryData,
  IN       UINT32                    AncillaryDataSize
  );

// DXIO Library Functions
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
  );

AGESA_STATUS
DxioProcessResults (
  IN       GNB_HANDLE                      *GnbHandle,
  IN       DXIO_COMPLEX_DESCRIPTOR         *UserConfig,
  IN       DXIO_PLATFORM_DESCRIPTOR        *ThisNode,
  IN       UINT32                          ThisNodeSize,
  IN       EARLY_TRAIN_DATA                *EarlyTrainData,
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi
  );

VOID
DxioProcessEarlyTrain (
  IN       GNB_HANDLE                      *GnbHandle,
  IN       DXIO_PLATFORM_DESCRIPTOR        *ThisNode,
  IN       UINT32                          ThisNodeSize,
  IN       EARLY_TRAIN_DATA                *EarlyTrainData,
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi
  );

VOID
DxioPortMapping (
  IN      GNB_HANDLE                *GnbHandle,
  IN      DXIO_COMPLEX_DESCRIPTOR   *UserConfig,
  IN      DXIO_PLATFORM_DESCRIPTOR  *ThisNode,
  IN OUT  DXIO_ENGINE_DESCRIPTOR    **EarlyTrain,
  IN OUT  UINT32                    *EarlyTrainAddress,
  IN OUT  PCIe_ENGINE_CONFIG        **EarlyTrainEngine
  );

VOID
DxioUpdatePortTrainingStatus (
  IN       GNB_HANDLE                *GnbHandle,
  IN       DXIO_COMPLEX_DESCRIPTOR   *UserConfig,
  OUT      DXIO_PLATFORM_DESCRIPTOR  *ThisNode
  );

AGESA_STATUS
PcieMapPortPciAddress (
  IN      PCIe_ENGINE_CONFIG     *Engine,
  IN      UINT8*                 PortDevMap
  );

VOID
PcieSetPortPciAddressMap (
  IN      GNB_HANDLE    *GnbHandle,
  IN      UINT8         *PortDevMap
  );

VOID
NbioEnableNbifDevice (
  IN        GNB_HANDLE         *GnbHandle,
  IN        UINT8              DeviceType,
  IN        UINT8              StartLane
  );

VOID
NbioDisableNbifDevice (
  IN        GNB_HANDLE         *GnbHandle,
  IN        UINT8              DeviceType,
  IN        UINT8              StartLane
  );

UINT32
DxioAncDataCalcSize (
  IN       DXIO_PORT_DESCRIPTOR    *PortPointer
  );

UINT32
DxioAncDataPopulate (
  IN       DXIO_ENGINE_DESCRIPTOR  *EnginePointer,
  IN       DXIO_PORT_DESCRIPTOR    *PortPointer,
  IN       UINT8                   *AncData
  );

AGESA_STATUS
GetPhyOverrideTable (
  IN OUT   DXIO_PHYOVR_TABLE       *Data
  );

UINT8
DxioLibServiceRequest (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi,
  IN       UINT32                          InstanceId,
  IN       UINT32                          ServiceRequest,
  IN       UINT32                          DxioArg1,
  IN       UINT32                          DxioArg2,
  IN       UINT32                          DxioArg3,
  IN       UINT32                          DxioArg4,
  IN       UINT32                          DxioArg5
  );

AGESA_STATUS
DxioGetUserCfgForSocket (
  IN      UINT8                     SocketNumber,
  IN      DXIO_COMPLEX_DESCRIPTOR   *UserComplex,
  IN OUT  DXIO_PORT_DESCRIPTOR      **PortList
  );

#endif


