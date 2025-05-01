/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD SRAT Services V2 Protocol prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Library
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 */
#ifndef _AMD_ACPI_SRAT_SERVICES_V2_PROTOCOL_H_
#define _AMD_ACPI_SRAT_SERVICES_V2_PROTOCOL_H_


#pragma pack (push, 1)

#include "AmdAcpiSratServicesProtocolCommon.h"
#include <IndustryStandard/Acpi63.h>
/*----------------------------------------------------------------------------------------
 *                    T Y P E D E F S     A N D     S T R U C T U R E S
 *                            System Resource Affinity Table
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------
 *                M E M O R Y
 *----------------------------------------------------------------------------------------
 */

///
/// Forward declaration for the AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL.
///
typedef struct _AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *AMD_SRAT_SERVICES_CREATE_GIAS) (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL  *This,
  IN       SRAT_HEADER                                *SratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  );

typedef
EFI_STATUS
(EFIAPI *AMD_SRAT_SERVICES_GET_GIAS_INFO) (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL           *This,
     OUT   EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE  **GiasInfo
  );

typedef
EFI_STATUS
(EFIAPI *AMD_SRAT_SERVICES_CREATE_GPUMEMORY) (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL  *This,
  IN       SRAT_HEADER                                *SratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  );

///
/// When installed, the SRAT Services V2 Protocol produces a collection of
/// services that return various information such as GIAS info to generate SRAT
///
struct _AMD_FABRIC_ACPI_SRAT_SERVICES_V2_PROTOCOL {
  UINTN                                Revision;         ///< Revision Number
  AMD_SRAT_SERVICES_CREATE_GPUMEMORY   CreateGpuMemory;  ///< Create GPU Memory Affinity Structure
  AMD_SRAT_SERVICES_CREATE_GIAS        CreateGias;       ///< Create Generic Initiator Affinity Structure
  AMD_SRAT_SERVICES_GET_GIAS_INFO      GetGiasInfo;      ///< Get Generic Initiator Affinity Structure
};

extern EFI_GUID gAmdCcxAcpiSratServicesProtocolGuid;
extern EFI_GUID gAmdFabricAcpiSratServicesV2ProtocolGuid;

#pragma pack (pop)
#endif // _AMD_ACPI_SRAT_SERVICES_V2_PROTOCOL_H_


