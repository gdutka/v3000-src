/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * NBIO Topology Services Protocol prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  NBIO
 * @e \$Revision$   @e \$Date$
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#ifndef _NBIO_TOPOLOGY_SERVICES_H_
#define _NBIO_TOPOLOGY_SERVICES_H_

#include <Library/BaseFabricTopologyLib.h>

/// Forward declaration for the AMD_NBIO_SERVICES_PROTOCOL.
typedef struct _AMD_NBIO_SERVICES_PROTOCOL AMD_NBIO_SERVICES_PROTOCOL;

/**
 * @brief System information through EFI call
 * @details 
 */
typedef
EFI_STATUS
(EFIAPI *AMD_NBIO_SERVICES_GET_SYSTEM_INFO) (
  IN       AMD_NBIO_SERVICES_PROTOCOL                 *This,
  OUT      UINTN                                      *NumberOfInstalledProcessors,
  OUT      UINTN                                      *TotalNumberOfDie,
  OUT      UINTN                                      *TotalNumberOfRootBridges
  );
/**
 * @brief EFI call for processor information
 * @details 
 */
typedef
EFI_STATUS
(EFIAPI *AMD_NBIO_SERVICES_GET_PROCESSOR_INFO) (
  IN       AMD_NBIO_SERVICES_PROTOCOL                 *This,
  IN       UINTN                                      Socket,
  OUT      UINTN                                      *NumberOfDie,
  OUT      UINTN                                      *NumberOfRootBridges
  );
/**
 * @brief EFI call to get root bridge information 
 * @details 
 */
typedef
EFI_STATUS
(EFIAPI *AMD_NBIO_SERVICES_GET_ROOT_BRIDGE_INFO) (
  IN       AMD_NBIO_SERVICES_PROTOCOL                 *This,
  IN       UINTN                                      Socket,
  IN       UINTN                                      Die,
  IN       UINTN                                      Index,
  OUT      UINTN                                      *NbioPhysicalID,
  OUT      UINTN                                      *NbioLogicalID,
  OUT      UINTN                                      *BusNumberBase,
  OUT      UINTN                                      *BusNumberLimit,
  OUT      UINTN                                      *PhysicalRootBridgeNumber
  );

/// When installed, the NBIO Services produces a collection of
/// services that provide information on NBIO topology and resources
struct _AMD_NBIO_SERVICES_PROTOCOL {
    AMD_NBIO_SERVICES_GET_SYSTEM_INFO        GetSystemInfo;         ///< get systems info
    AMD_NBIO_SERVICES_GET_PROCESSOR_INFO     GetProcessorInfo;      ///< get processor info
    AMD_NBIO_SERVICES_GET_ROOT_BRIDGE_INFO   GetRootBridgeInfo;     ///< bridge info
};

extern EFI_GUID gAmdNbioServicesProtocolGuid;         ///< Guid for calling

#endif


