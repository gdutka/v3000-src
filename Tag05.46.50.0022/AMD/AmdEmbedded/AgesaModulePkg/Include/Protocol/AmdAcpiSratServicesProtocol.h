/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD SRAT Services Protocol prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Library
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 */
#ifndef _AMD_ACPI_SRAT_SERVICES_PROTOCOL_H_
#define _AMD_ACPI_SRAT_SERVICES_PROTOCOL_H_

#pragma pack (push, 1)

#include "AmdAcpiSratServicesProtocolCommon.h"

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
typedef struct _AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *AMD_SRAT_SERVICES_CREATE_MEMORY) (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL     *This,
  IN       SRAT_HEADER                                *SratHeaderStructPtr,
  IN OUT   UINT8                                     **TableEnd
  );

typedef
EFI_STATUS
(EFIAPI *AMD_SRAT_SERVICES_GET_MEMORY_INFO) (
  IN       AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL      *This,
     OUT   UINT32                                      *NumberOfDomains,
     OUT   MEMORY_INFO                                **MemoryInfo
  );
///
/// When installed, the SRAT Services Protocol produces a collection of
/// services that return various information to generate SRAT
///
struct _AMD_FABRIC_ACPI_SRAT_SERVICES_PROTOCOL {
  UINTN                                Revision;       ///< Revision Number
  AMD_SRAT_SERVICES_CREATE_MEMORY      CreateMemory;   ///< Create Memory Affinity Structure
  AMD_SRAT_SERVICES_GET_MEMORY_INFO    GetMemoryInfo;  ///< Return Memory Affinity Information
};

extern EFI_GUID gAmdCcxAcpiSratServicesProtocolGuid;
extern EFI_GUID gAmdFabricAcpiSratServicesProtocolGuid;

#pragma pack (pop)
#endif // _AMD_ACPI_SRAT_SERVICES_PROTOCOL_H_


