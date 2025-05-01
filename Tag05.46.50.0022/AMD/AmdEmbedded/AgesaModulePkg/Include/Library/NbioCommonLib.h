/*
*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
#ifndef _NBIOCOMMONLIB_H_
#define _NBIOCOMMONLIB_H_

#include <Ppi/FabricTopologyServices2Ppi.h>

#define CHIPSET_SOCKET_INDEX  5


// Functions called from NbioCommonLib
VOID*
GetPcieComplexPointer(
  UINTN   SocketIndex,
  UINTN   DieIndex,
  UINTN   RootBridgeIndex
  );

UINTN
GetPcieComplexSize (
  UINTN   SocketIndex,
  UINTN   DieIndex,
  UINTN   RootBridgeIndex
  );

BOOLEAN
IsAlternateComplex (
  UINTN   SocketIndex,
  UINTN   DieIndex,
  UINTN   RootBridgeIndex
  );
VOID
PcieFixupComplexDefaults (
  IN     PCIe_PLATFORM_CONFIG     *Pcie
  );

VOID
PcieFixupPlatformConfig (
  IN     PCIe_PLATFORM_CONFIG     *Pcie
  );


// Functions implemented in NbioCommonLib
EFI_STATUS
NbioBuildPcieComplex (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

EFI_STATUS
EFIAPI
PcieInstallTopologyPpi (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

EFI_STATUS
EFIAPI
PcieInstallServicesPpi (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

EFI_STATUS
PcieGetPciePei (
     OUT     PCIe_PLATFORM_CONFIG          **Pcie
  );

AGESA_STATUS
HideWrapper (
  IN      GNB_HANDLE     *GnbHandle,
  IN      UINT32         WrapperNum
  );

BOOLEAN
IsRootBridgePresent (
  IN UINT32   Bus,
  IN UINTN    RbIndex
  );

VOID
EnumerateAndHarvestWrappers (
  IN      PCIe_PLATFORM_CONFIG        *Pcie
  );

EFI_STATUS
PcieGetLogicalId (
     OUT   SOC_LOGICAL_ID           *LogicalId
  );
#endif


