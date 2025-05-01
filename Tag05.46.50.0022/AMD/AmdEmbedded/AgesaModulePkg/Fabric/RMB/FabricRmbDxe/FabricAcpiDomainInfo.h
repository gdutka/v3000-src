/*****************************************************************************
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Fabric NUMA domain definitions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Include
 * @e \$Revision$   @e \$Date$
 *
 */
#ifndef _FABRIC_ACPI_DOMAIN_INFO_H_
#define _FABRIC_ACPI_DOMAIN_INFO_H_

#include <Protocol/AmdAcpiSratServicesProtocol.h>
#include <FabricRegistersDf3.h>
#include <FabricInfoRmb.h>

#pragma pack (push, 1)

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define MAX_NPS                   4
#define MAX_CCX_PER_CCM           1
#define MAX_CCM_PER_DIE           RMB_NUM_CCM_BLOCKS
#define MAX_SLINK_PER_DIE         0

#define NORMALIZED_SOCKET_SHIFT   (1 << 4)

#define MAX_REPORTED_DOMAINS  ((MAX_CCX_PER_CCM * MAX_CCM_PER_DIE * RMB_MAX_DIES_PER_SOCKET * RMB_MAX_SOCKETS) + (MAX_SLINK_PER_DIE * RMB_MAX_DIES_PER_SOCKET * RMB_MAX_SOCKETS))
#define MAX_PHYSICAL_DOMAINS  ((RMB_MAX_SOCKETS * MAX_NPS) + (MAX_SLINK_PER_DIE * RMB_MAX_DIES_PER_SOCKET * RMB_MAX_SOCKETS))

typedef
VOID
F_PUBLISH_ACPI_NUMA_MEM_ENTRY (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    Domain,
  IN       UINT32    RegionBaseLow,
  IN       UINT32    RegionBaseHigh,
  IN       UINT32    RegionSizeLow,
  IN       UINT32    RegionSizeHigh
  );

typedef F_PUBLISH_ACPI_NUMA_MEM_ENTRY  *PF_PUBLISH_ACPI_NUMA_MEM_ENTRY;

typedef
VOID
F_PUBLISH_ACPI_NUMA_GIAS_ENTRY (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    ProximityDomain,
  IN       UINT8     DeviceHandleType,
  IN       UINT16    PciSegment,
  IN       UINT16    PciBdfNumber
  );

typedef F_PUBLISH_ACPI_NUMA_GIAS_ENTRY* PF_PUBLISH_ACPI_NUMA_GIAS_ENTRY;

typedef
VOID
F_PUBLISH_ACPI_NUMA_GPUMEM_ENTRY (
  IN OUT   UINT8   **TablePointer,
  IN       UINT32    Domain,
  IN       UINT32    RegionBaseLow,
  IN       UINT32    RegionBaseHigh,
  IN       UINT32    RegionSizeLow,
  IN       UINT32    RegionSizeHigh
  );

typedef F_PUBLISH_ACPI_NUMA_GPUMEM_ENTRY* PF_PUBLISH_ACPI_NUMA_GPUMEM_ENTRY;

EFI_STATUS
FabricCreateSystemAcpiDomainData (
  IN OUT   UINT8                             **TableEnd,
  IN       PF_PUBLISH_ACPI_NUMA_MEM_ENTRY      PublishMemEntry,
     OUT   UINT32                             *NumberOfEntriesPublished
  );

EFI_STATUS
FabricCreateGiasAcpiDomainData (
  IN OUT   UINT8                             **TableEnd,
  IN       PF_PUBLISH_ACPI_NUMA_GIAS_ENTRY     PublishGiasEntry,
     OUT   UINT32                             *NumberOfEntriesPublished
  );

EFI_STATUS
FabricCreateGpuMemAcpiDomainData (
  IN OUT   UINT8                             **TableEnd,
  IN       PF_PUBLISH_ACPI_NUMA_GPUMEM_ENTRY   PublishGpuMemEntry,
     OUT   UINT32                             *NumberOfEntriesPublished
  );

EFI_STATUS
FabricGetMemoryInfo (
     OUT   UINT32        *NumberOfDomains,
     OUT   MEMORY_INFO  **MemoryInfo
  );

VOID
InitMixedDimmConfig (
  VOID
  );

UINT8
FabricGetMaxDomains (
  VOID
  );

#pragma pack (pop)
#endif // _FABRIC_ACPI_DOMAIN_INFO_H_
