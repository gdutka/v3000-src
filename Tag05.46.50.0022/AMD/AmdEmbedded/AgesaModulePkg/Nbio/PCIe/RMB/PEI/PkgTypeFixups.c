/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

#include <AMD.h>
#include <GnbDxio.h>
#include <AmdSoc.h>
#include <AmdPcieComplex.h>
#include <CcxRegistersVh.h>
#include <GnbRegistersRMB.h>
#include <Library/AmdBaseLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/GnbPciAccLib.h>
#include <Library/BxbInitLib.h>
#include <Library/GnbPciLib.h>
#include <Library/AmdSocBaseLib.h>

#define FILECODE    NBIO_PCIE_RMB_PEI_PKGTYPEFIXUPS_FILECODE



/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define BRIDGE_DEVICE_TYPE        6
#define DISPLAY_CONTROLLER_TYPE   3


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
PcieEngineConfigDefaults (
  IN      PCIe_ENGINE_CONFIG                *PcieEngine,
  IN OUT  VOID                              *Buffer,
  IN      PCIe_PLATFORM_CONFIG              *Pcie
  );

AGESA_STATUS
PcieWrapperConfigDefaults (
  IN      PCIe_WRAPPER_CONFIG               *Wrapper,
  IN OUT  VOID                              *Buffer,
  IN      PCIe_PLATFORM_CONFIG              *Pcie
  );

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
AGESA_STATUS
STATIC
PcieFixupPlatformConfigFP7 (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
//  GNB_HANDLE            *GnbHandle;
//
//  GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Wrapper);
//  if (Wrapper->WrapId == 0) {
//    Wrapper->StartDxioLane = 0;
//    Wrapper->EndDxioLane = 11;
//    Wrapper->StartPhyLane = 0;
//    Wrapper->EndPhyLane = 11;
//    Wrapper->Features.CoreReversed = 0;
//  } else if (Wrapper->WrapId == 1) {
//    Wrapper->StartDxioLane = 12;
//    Wrapper->EndDxioLane = 19;
//    Wrapper->StartPhyLane = 12;
//    Wrapper->EndPhyLane = 19;
//    Wrapper->Features.CoreReversed = 0;
//  }
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
AGESA_STATUS
STATIC
PcieFixupPlatformConfigAM5 (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
//  GNB_HANDLE            *GnbHandle;
//
//  GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Wrapper);
//  if (Wrapper->WrapId == 1) {
//    Wrapper->StartDxioLane = 0;
//    Wrapper->EndDxioLane = 7;
//    Wrapper->StartPhyLane = 0;
//    Wrapper->EndPhyLane = 7;
//    Wrapper->Features.CoreReversed = 0;
//  } else if (Wrapper->WrapId == 0) {
//    Wrapper->StartDxioLane = 8;
//    Wrapper->EndDxioLane = 23;
//    Wrapper->StartPhyLane = 8;
//    Wrapper->EndPhyLane = 23;
//    Wrapper->Features.CoreReversed = 0;
//  }
  return AGESA_SUCCESS;
}

VOID
PcieEngineConfigRmbDefaults (
  IN      PCIe_ENGINE_CONFIG                *PcieEngine,
  IN OUT  VOID                              *Buffer,
  IN      PCIe_PLATFORM_CONFIG              *Pcie
  )
{

  LibAmdMemFill (&(PcieEngine->Type.Port.PortCapabilities), 0xFF, sizeof (PORT_CAPABILITIES), NULL);
  LibAmdMemFill (&(PcieEngine->Type.Port.PortFeatures), 0xFF, sizeof (PORT_FEATURES), NULL);
  PcieEngine->Type.Port.EqSearchMode = 3;
  PcieEngine->Type.Port.EqSearchModeGen4 = 3;
  PcieEngine->Type.Port.PortFeatures.L1PowerDown = 1;
  PcieEngine->Type.Port.ClkReqFilterEn = 1;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie L1.2 Disabled in Platform Topology for FP7 package
 *
 *
 *
 * @param[in]     PcieTopologyData    Platform Topology Data
 */
VOID
STATIC
FixupL1SSForFp7 (
  IN       DXIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  )
{
//  DXIO_PORT_DESCRIPTOR      *EngineDescriptor;
//
//  EngineDescriptor = PcieTopologyData->PciePortList;
//  while (EngineDescriptor != NULL) {
//    if (EngineDescriptor->EngineData.EngineType == DxioPcieEngine) {
////      EngineDescriptor->Port.LinkAspmL1_2 = 0;
//    }
//    EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
//  }
  return;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie SearchDepth in Platform Topology for AM5 package
 *
 *
 *
 * @param[in]     PcieTopologyData    Platform Topology Data
 */
VOID
STATIC
FixupSearchDepthForAM5 (
  IN       DXIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  )
{
//  DXIO_PORT_DESCRIPTOR      *EngineDescriptor;
//
//  EngineDescriptor = PcieTopologyData->PciePortList;
//  while (EngineDescriptor != NULL) {
//    if (EngineDescriptor->EngineData.EngineType == DxioPcieEngine) {
//      if (((EngineDescriptor->EngineData.StartLane > 7) && (EngineDescriptor->EngineData.StartLane < 16)) &&
//         ((EngineDescriptor->EngineData.EndLane > 7) && (EngineDescriptor->EngineData.EndLane < 16))) {
//        EngineDescriptor->EngineData.SearchDepth = 1;
//      }
//    }
//    EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
//  }
  return;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Lane mapping in Platform Topology for AM5 package
 *
 *
 *
 * @param[in]     PcieTopologyData    Platform Topology Data
 */
VOID
STATIC
FixupLaneMappingForAM5 (
  IN       DXIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  )
{
  DXIO_PORT_DESCRIPTOR      *EngineDescriptor;

  EngineDescriptor = PcieTopologyData->PciePortList;
  while (EngineDescriptor != NULL) {
    if (EngineDescriptor->EngineData.StartLane >=16) {
      EngineDescriptor->EngineData.StartLane -= 8;
    }
    if (EngineDescriptor->EngineData.EndLane >=16) {
      EngineDescriptor->EngineData.EndLane -= 8;
    }

    EngineDescriptor = PcieConfigGetNextDataDescriptor (EngineDescriptor);
  }
  return;
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to fixup Pcie Platform Configuration for package specific values
 *
 *
 *
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
PcieFixupPlatformConfig (
  IN     PCIe_PLATFORM_CONFIG                 *Pcie
  )
{

  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALL_ENGINES,
    PcieEngineConfigRmbDefaults,
    NULL,
    Pcie
    );

  return;
}

/**
  Initialize PcieComplexData defaults for all engines and wrappers

  @param Pcie Pointer to PCIe complex data
**/
VOID
PcieFixupComplexDefaults (
  IN     PCIe_PLATFORM_CONFIG                 *Pcie
  )
{
  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALL_ENGINES,
    PcieEngineConfigDefaults,
    NULL,
    Pcie
    );
  PcieConfigRunProcForAllWrappers (
    DESCRIPTOR_ALL_WRAPPERS,
    PcieWrapperConfigDefaults,
    NULL,
    Pcie
    );
}

/*----------------------------------------------------------------------------------------*/
/**
  Package specific fixups for PCIe speed

  @param Pcie Pointer to PCIe complex data
**/
VOID
PcieGenSpeedWorkaround (
  IN     DXIO_PORT_DESCRIPTOR      *EngineDescriptor
  )
{

  if ((EngineDescriptor->Port.LinkSpeedCapability == 0) || (EngineDescriptor->Port.LinkSpeedCapability > 2)) {
    EngineDescriptor->Port.EqPreset = 3;
  }
  return;
}

/*----------------------------------------------------------------------------------------*/
//                 E A R L Y   T R A I N   C L E A N U P
/*----------------------------------------------------------------------------------------*/
/**
 * Clean up bridge
 *
 *
 *
 * @param[in]     Endpoint       PCI Address of the bridge
 */
VOID
DxioCleanUpEarlyBridge (
  IN       PCI_ADDR              Endpoint
  )
{
  UINT32      Index;
  UINT32      Value;
  PCI_ADDR    Downstream;

  GnbLibPciRead (Endpoint.AddressValue | 0x08, AccessWidth32, &Value, NULL);
  if ((Value >> 24) == DISPLAY_CONTROLLER_TYPE) {
    Value = 0;
    GnbLibPciWrite (Endpoint.AddressValue | 0x04, AccessWidth32, &Value, NULL);
    for (Index = 0x10; Index < 0x28; Index += 4) {
      GnbLibPciWrite (Endpoint.AddressValue | Index, AccessWidth32, &Value, NULL);
    }
  } else if ((Value >> 24) == BRIDGE_DEVICE_TYPE) {
    GnbLibPciRead (Endpoint.AddressValue | PCICFG_SPACE_PRIMARY_BUS_OFFSET, AccessWidth32, &Value, NULL);
    if ((Value & 0x00FFFF00) != 0) {
      Downstream.AddressValue = MAKE_SBDFO (0, (Value & 0x0000FF00) >> 8, 0, 0, 0);
      DxioCleanUpEarlyBridge (Downstream);
      Value = 0;
      GnbLibPciWrite (Endpoint.AddressValue | PCICFG_SPACE_IO_BASE_OFFSET, AccessWidth16, &Value, NULL);
      GnbLibPciWrite (Endpoint.AddressValue | PCICFG_SPACE_MEMORY_BASE_OFFSET, AccessWidth32, &Value, NULL);
      GnbLibPciWrite (Endpoint.AddressValue | PCICFG_SPACE_PREFETCH_BASE_HIGH_OFFSET, AccessWidth32, &Value, NULL);
      GnbLibPciWrite (Endpoint.AddressValue | PCICFG_SPACE_PREFETCH_HIGH_OFFSET, AccessWidth32, &Value, NULL);
      GnbLibPciWrite (Endpoint.AddressValue | PCICFG_SPACE_PREFETCH_LOW_OFFSET, AccessWidth32, &Value, NULL);
      GnbLibPciWrite (Endpoint.AddressValue | PCICFG_SPACE_IO_BASE_LOW_OFFSET, AccessWidth32, &Value, NULL);
      GnbLibPciWrite (Endpoint.AddressValue | PCICFG_SPACE_COMMAND_OFFSET, AccessWidth32, &Value, NULL);
      GnbLibPciRead (Endpoint.AddressValue |  PCICFG_SPACE_PRIMARY_BUS_OFFSET, AccessWidth32, &Value, NULL);
      Value &= 0xFF0000FF;
      GnbLibPciWrite (Endpoint.AddressValue | PCICFG_SPACE_PRIMARY_BUS_OFFSET, AccessWidth32, &Value, NULL);
    }
  }
}

//----------------------------------------------------------------//
/**
  Package specific fixups for PCIe lane mapping

  @param Pcie Pointer to PCIe complex data
**/
VOID
PcieLaneMappingFixups (
  IN       GNB_HANDLE                *GnbHandle,
  IN       DXIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  )
{
  SOC_ID_STRUCT       SocId;
  SOC_ID_STRUCT       SocId2;

  // Check Hardware Identification RMB AM5
  SocId.SocFamilyID = F19_RMB_RAW_ID;
  SocId.PackageType = ZEN3_PKG_AM5;

  SocId2.SocFamilyID = F19_RMB_RAW_ID;
  SocId2.PackageType = ZEN3_PKG_AM5_B0;
  if (SocHardwareIdentificationCheck (&SocId) ||
      SocHardwareIdentificationCheck (&SocId2)) {
    FixupLaneMappingForAM5 (PcieTopologyData);
  }

}

//----------------------------------------------------------------//
/**
  Package specific fixups for early trained link

  @param Pcie Pointer to PCIe complex data
**/
VOID
PcieEarlyTrainFixups (
  IN       GNB_HANDLE                *GnbHandle,
  IN       DXIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  )
{
  PcdSetBoolS (PcdBixbyLinkFound, FALSE);

  PcieTopologyData->BmcLinkLocation = 0xFF;
  // Client support for Bixby
  PcdSetBoolS (PcdBixbyLinkFound, BixbyFindEarlyLink (PcieTopologyData, GnbHandle));
}

