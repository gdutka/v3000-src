/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific PCIe definitions
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 312288 $   @e \$Date: 2015-02-04 00:39:01 -0600 (Wed, 04 Feb 2015) $
 *
 */
#ifndef _AMDNBIOPCIEPEI_H_
#define _AMDNBIOPCIEPEI_H_

#include <Ppi/FabricTopologyServices2Ppi.h>

// Different NBIF device types for use in calling DeviceControl functions
// Based off of table in PPR 13.6.9.1
typedef enum {
  NbifFunctionGfx = 0,
  NbifFunctionDceAz,
  NbifFunctionPspCcp,
  NbifFunctionUsb3Gen1_0,
  NbifFunctionUsb3Gen1_1,
  NbifFunctionAcp,
  NbifFunctionHdAudio,
  NbifFunctionMp2,
  NbifFunctionSata,
  NbifFunctionSata1,
  NbifFunctionXgbe0,
  NbifFunctionXgbe1,
  NbifFunctionWlan,
  NbifFunctionBt,
  NbifFunctionI2sAC97,
  NbifFunctionUnused = 0xFF,
} RMB_NBIF_FUNCTION_TYPES;


EFI_STATUS
EFIAPI
DxioInitializationCallbackPpi (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
);

VOID
PciePortsVisibilityControl (
  IN      PCIE_PORT_VISIBILITY    Control,
  IN      GNB_HANDLE              *GnbHandle
  );


VOID
NbioDisableNbifDevice (
  IN        GNB_HANDLE         *GnbHandle,
  IN        UINT8              DeviceType,
  IN        UINT8              StartLane
  );
  
    VOID
NbioDisableEnableNbifDevice (
  IN        GNB_HANDLE         *GnbHandle,
  IN        UINT8              DeviceType,
  IN        BOOLEAN            EnableOrDisable
  );
  

VOID
PcieConfigureHotplugPorts (
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  );

VOID
PcieHotplugConfigureUSB4 (
  IN       GNB_HANDLE  *GnbHandle
  );

EFI_STATUS
PcieDpcStatusServiceInit (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

VOID
PcieFixupPlatformConfig (
  IN     PCIe_PLATFORM_CONFIG                 *Pcie
  );

VOID
PcieFixupComplexDefaults (
  IN     PCIe_PLATFORM_CONFIG                 *Pcie
  );

VOID
PcieGenSpeedWorkaround (
  IN     DXIO_PORT_DESCRIPTOR      *EngineDescriptor
  );

VOID
PcieEarlyTrainFixups (
  IN       GNB_HANDLE                *GnbHandle,
  IN       DXIO_COMPLEX_DESCRIPTOR   *UserConfig
  );

VOID
PcieLaneMappingFixups (
  IN       GNB_HANDLE                *GnbHandle,
  IN       DXIO_COMPLEX_DESCRIPTOR   *UserConfig
  );

VOID
DxioManageTopology (
  IN      DXIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  );

VOID
DxioTopologyWorkarounds (
  IN      DXIO_COMPLEX_DESCRIPTOR     *ComplexDescriptor
  );

VOID
DxioCfgBeforeDxioInit (
  IN      PCIe_PLATFORM_CONFIG      *Pcie,
  IN      DXIO_COMPLEX_DESCRIPTOR   *ComplexDescriptor
  );

VOID
DxioCfgAfterDxioInit (
  IN     PCIe_PLATFORM_CONFIG                 *Pcie,
  IN     AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServicesPpi
  );

VOID
DxioTopologyAddUSB4 (
  IN     CONST EFI_PEI_SERVICES     **PeiServices,
  IN     UINTN                      ControllerCount,
  IN     DXIO_COMPLEX_DESCRIPTOR    **ComplexDescriptor
  );
#endif


