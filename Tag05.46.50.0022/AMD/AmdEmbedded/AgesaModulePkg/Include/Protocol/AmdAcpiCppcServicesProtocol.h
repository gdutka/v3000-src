/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPPC Services Protocol prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Protocol
 * @e \$Revision:  $   @e \$Date:  $
 */

#ifndef _AMD_ACPI_CPPC_SERVICES_PROTOCOL_H_
#define _AMD_ACPI_CPPC_SERVICES_PROTOCOL_H_

#pragma pack (push, 1)

/*----------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *----------------------------------------------------------------------------------------
 */

/// Forward declaration for AMD_CCX_ACPI_CPPC_SERVICES_PROTOCOL
typedef struct _AMD_CCX_ACPI_CPPC_SERVICES_PROTOCOL AMD_CCX_ACPI_CPPC_SERVICES_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *AMD_CPPC_SERVICES_GET_REGISTER_DESC_MSR) (
  IN       AMD_CCX_ACPI_CPPC_SERVICES_PROTOCOL    *This,
     OUT   CPPC_ENTRIES_COMMON_TABLE              **CppcRegDescMsr
  );

/// When installed, the CPPC Services Protocol produces a collection of
/// services related to CPPC
struct _AMD_CCX_ACPI_CPPC_SERVICES_PROTOCOL {
  UINTN                                         Revision;                      ///< Revision Number
  AMD_CPPC_SERVICES_GET_REGISTER_DESC_MSR       GetRegisterDescriptorsMsr;     ///< Returns the list of Register Descriptors MSR
};

extern EFI_GUID gAmdCcxAcpiCppcServicesProtocolGuid;

#pragma pack (pop)
#endif // _AMD_ACPI_CPPC_SERVICES_PROTOCOL_H_
