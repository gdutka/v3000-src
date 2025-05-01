/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file
 *
 * Fabric Pstate Service definitions
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Fabric
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */
#ifndef _AMD_NBIO_SMU_V13_DXE_H_
#define _AMD_NBIO_SMU_V13_DXE_H_

#include "smu13_bios_if_RMB.h"
#include "smu13_RMB.h"
#include "Smu13FeaturesEnable.h"

#include <GnbDxio.h>

#include <Protocol/AmdNbioSmuServicesProtocol.h>
#include <Protocol/AmdCoreTopologyV2Protocol.h>

EFI_STATUS
EFIAPI
AmdSmuReadFuseByName (
  IN  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL         *This,
  IN       UINT32                                InstanceId,
  IN       UINTN                                 FuseName,
  IN OUT   UINTN                                 *BufferSize,
     OUT   UINT8                                 *FuseValue
  );

EFI_STATUS
EFIAPI
AmdSmuReadFuseByNumber (
  IN  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL         *This,
  IN  UINT32                                     FuseChain,
  IN  UINT32                                     FuseStartBit,
  IN  UINT32                                     FuseSize,
  OUT UINT32                                     *FuseValue
  );

EFI_STATUS
EFIAPI
AmdSmuServiceRequest (
  IN  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL         *This,
  IN  UINT32                                     InstanceId,
  IN  UINT32                                     ServiceRequest,
  IN  UINT32                                     *InputParameters,
  OUT UINT32                                     *ReturnValues
  );

EFI_STATUS
EFIAPI
AmdSmuRegisterRead (
  IN  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL         *This,
  IN  UINT32                                     InstanceId,
  IN  UINT32                                     RegisterIndex,
  OUT UINT32                                     *RegisterValue
  );

EFI_STATUS
EFIAPI
AmdSmuRegisterWrite (
  IN  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL         *This,
  IN  UINT32                                     InstanceId,
  IN  UINT32                                     RegisterIndex,
  IN  UINT32                                     *RegisterValue
  );

EFI_STATUS
EFIAPI
AmdSmuRegisterRMW (
  IN  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL         *This,
  IN  UINT32                                     InstanceId,
  IN  UINT32                                     RegisterIndex,
  IN  UINT32                                     RegisterANDValue,
  IN  UINT32                                     RegisterORValue
  );

EFI_STATUS
EFIAPI
AmdSmuReadBrandString (
  IN  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL         *This,
  IN  UINT32                                     InstanceId,
  IN  UINT32                                     BrandStringLength,
  OUT UINT8                                      *BrandString
  );

EFI_STATUS
SmuV13GetCoreTopologyOnDie (
  IN       AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL *This,
  IN       UINTN                                  Socket,
  IN       UINTN                                  Die,
     OUT   UINTN                                  *NumberOfCcds,
     OUT   UINTN                                  *NumberOfComplexes,
     OUT   UINTN                                  *NumberOfCores,
     OUT   UINTN                                  *NumberOfThreads
  );

EFI_STATUS
SmuV13LaunchThread (
  IN       AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL *This,
  IN       UINTN                                  Socket,
  IN       UINTN                                  Die,
  IN       UINTN                                  LogicalCcd,
  IN       UINTN                                  LogicalComplex,
  IN       UINTN                                  LogicalCore,
  IN       UINTN                                  LogicalThread
  );

EFI_STATUS
AmdSmuReadCacWeights (
  IN  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL         *This,
  IN  UINT32                                     MaxNumWeights,
  OUT UINT64                                     *ApmWeights
  );

EFI_STATUS
AmdSmuReadThrottleThresholds (
  IN  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL         *This,
  OUT UINT32                                     *Thresholds
  );

EFI_STATUS
SmuGetGnbHandle (
  IN  UINT32                                     InstanceId,
  OUT GNB_HANDLE                                 **MyHandle
  );

EFI_STATUS
SmuDxeInitialize (
  OUT PCIe_PLATFORM_CONFIG                       **Pcie
  );

UINT32
SmuServiceRequest (
  GNB_HANDLE      *GnbHandle,
  UINT32          ServiceRequest,
  UINT32          Arg0,
  UINT32          Arg1,
  UINT32          Arg2,
  UINT32          Arg3,
  UINT32          Arg4,
  UINT32          Arg5
  );

VOID
DumpSmuFeatureControl (
  IN       UINT32             EnableFeatures,
  IN       UINT32             EnableFeaturesExt,
  IN       UINT32             EnableFeatures64
  );

VOID
DumpFanPolicyTable (
  IN       Smu13BiosIfTable_t      *PPTable
  );

VOID
DumpPPTable (
  IN       Smu13BiosIfTable_t      *PPTable
  );

VOID
SmuFixupPlatformConfig (
  IN     UINT32                     PackageType,
  IN     Smu13BiosIfTable_t         *PPTable
  );

VOID
SmuPackageTypeWorkarounds (
  IN     UINT32                     PackageType,
  IN     GNB_HANDLE                 *GnbHandle
  );

UINT8 
EFIAPI 
CheckOPN ();

#endif

