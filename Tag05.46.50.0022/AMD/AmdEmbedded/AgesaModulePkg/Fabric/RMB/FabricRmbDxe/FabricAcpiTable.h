/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Fabric ACPI TABLE definitions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Fabric
 * @e \$Revision$   @e \$Date$
 *
 */
#ifndef _FABRIC_ACPI_TABLE_H_
#define _FABRIC_ACPI_TABLE_H_
#include <Protocol/FabricNumaServices2.h>

#pragma pack (push, 1)

EFI_STATUS
EFIAPI
FabricRmbNumaServices2ProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
FabricRmbAcpiSlitProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
FabricRmbAcpiSratProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
FabricRmbAcpiSratV2ProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE *SystemTable
  );

EFI_STATUS
EFIAPI
FabricRmbAcpiCditProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
FabricRmbAcpiCratProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
FabricGetDistanceInfo (
  IN OUT   UINT8         *Distance,
  IN       UINT32         DomainCount,
  IN       DOMAIN_INFO2  *DomainInfo
  );

EFI_STATUS
EFIAPI
FabricRmbAcpiMsctProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

#pragma pack (pop)
#endif // _FABRIC_ACPI_TABLE_H_
