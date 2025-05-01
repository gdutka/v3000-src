/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * CCX ACPI Services for Zen3 family processors.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CCX
 * @e \$Revision$   @e \$Date$
 *
 */
#ifndef _CCX_ZEN3_ACPI_SERVICES_DXE_H_
#define _CCX_ZEN3_ACPI_SERVICES_DXE_H_

#pragma pack (push, 1)

typedef struct {
  UINT32      MtrrIndex;      ///< Mtrr Index
  UINT64      MtrrBase;       ///< Mtrr Base
  UINT64      MtrrMask;       ///< Mtrr Mask
} MTRR_INFO;

VOID
SetSharedMemoryToUC (
  IN       MTRR_INFO  *MtrrInfo
  );

EFI_STATUS
EFIAPI
CcxZen3AcpiCpuSsdtServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
CcxZen3CratServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
CcxZen3SratServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
CcxZen3RasServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
CcxZen3PcctServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

EFI_STATUS
EFIAPI
CcxZen3CppcServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  );

#pragma pack (pop)
#endif // _CCX_ZEN3_ACPI_SERVICES_DXE_H_

