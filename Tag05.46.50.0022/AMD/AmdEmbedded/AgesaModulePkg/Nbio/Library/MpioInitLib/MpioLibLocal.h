/**
 *  @file MpioLibLocal.h
 *  @brief Prototypes of local functions in MpioInitLib and callbacks
 */
/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

#ifndef _MPIOANCDATAV1_H_
#define _MPIOANCDATAV1_H_



/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
// todo
#define   MAX_INSTANCE_ID   9

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

// typedef struct {
  // PCI_ADDR                  EarlyTrainAddress;  ///< PCI address of the early train port
  // DXIO_ENGINE_DESCRIPTOR    *EarlyTrainDesc;    ///< Pointer to the early train DXIO descriptor
  // PCIe_ENGINE_CONFIG        *EarlyTrainEngine;  ///< Pointer to the early train engine
  // BOOLEAN                   EarlyLinkMapped;    ///< Early Train status
// } EARLY_TRAIN_DATA;

typedef struct {
  FW_ASK_STRUCT     *MpioAsk;
  UINT32            MpioAskCount;
  UINT8             *ExtAttributes;
  UINT32            ExtAttributeSize;
} MPIO_DATA;


/*----------------------------------------------------------------------------------------
 *          P R O T O T Y P E S     O F     L I B R A R Y     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

// DXIO Library Functions
AGESA_STATUS
MpioParsePlatformTopology (
  IN       GNB_HANDLE                *GnbHandle,
  IN       DXIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
     OUT   MPIO_DATA                 *MpioData
  );

VOID
MpioPortMapping (
  IN      GNB_HANDLE                *GnbHandle,
  IN      DXIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  IN OUT  MPIO_DATA                 *MpioData
  );

VOID
MpioUpdatePortTrainingStatus (
  IN       GNB_HANDLE                *GnbHandle,
  IN       MPIO_DATA                 *MpioData
  );

UINT32
DxioAncDataCalcSize (
  IN       DXIO_PORT_DESCRIPTOR    *PortPointer
  );

UINT32
DxioAncDataPopulate (
  IN       FW_ASK_STRUCT           *AskEntry,
  IN       DXIO_PORT_DESCRIPTOR    *PortPointer,
  IN       UINT8                   *AncData
  );

// DXIO Debug Output Functions
VOID
MpioDebugDump (
  IN       MPIO_DATA       *MpioData
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


AGESA_STATUS
GetPhyOverrideTable (
  IN OUT   DXIO_PHYOVR_TABLE       *Data
  );

AGESA_STATUS
MpioGetTopologyForSocket (
  IN      UINT8                     SocketNumber,
  IN      DXIO_COMPLEX_DESCRIPTOR   *PlatformTopology,
  IN OUT  DXIO_PORT_DESCRIPTOR      **PortList
  );

// Common support functions
VOID
FindEngineCallback (
  IN     PCIe_ENGINE_CONFIG   *Engine,
  IN OUT VOID                 *Buffer,        // Pointer to ENGINE_LOCATION_CALLBACK
  IN     PCIe_WRAPPER_CONFIG  *Wrapper
  );

VOID
FindWrapperCallback (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,        // Pointer to ENGINE_LOCATION_CALLBACK
  IN      GNB_HANDLE            *GnbHandle
  );

PCIe_ENGINE_CONFIG *
MpioFindEngineForAsk (
  IN      GNB_HANDLE          *GnbHandle,
  IN      FW_ASK_STRUCT       *AskEntry
  );

/*----------------------------------------------------------------------------------------
 *          P R O T O T Y P E S    O F    E X T E R N A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

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
MpioCfgGlobalConfig (
  IN       GNB_HANDLE         *GnbHandle,
  IN       UINT32             *MpioArg
  );

VOID
MpioCfgBeforeReconfig (
  IN       GNB_HANDLE       *GnbHandle
  );

VOID
MpioCfgAfterReconfig (
  IN       GNB_HANDLE       *GnbHandle
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

UINT32
PcieGetDeviceMappingSize ();

#endif

