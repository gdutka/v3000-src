 /*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef DASH_PLDM_BASE_H
#define DASH_PLDM_BASE_H

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiLib.h>
#include <Library/CommonSetupLib.h>

EFI_STATUS
EFIAPI
DashPldmGetSmbiosStructureTableMetaData (
  AMD_DASH_PLDM_PROTOCOL                                        *This,
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT *Buffer,
  UINT32                                                   *BufferSize
);

EFI_STATUS
EFIAPI
DashPldmSetSmbiosStructureTableMetaData (
  AMD_DASH_PLDM_PROTOCOL *This
);

EFI_STATUS
EFIAPI
DashPldmGetSmbiosStructureTable (
  AMD_DASH_PLDM_PROTOCOL                               *This,
  DASH_PLDM_GET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                                          *BufferSize
);

EFI_STATUS
EFIAPI
DashPldmSetSmbiosStructureTable (
  AMD_DASH_PLDM_PROTOCOL *This
);

EFI_STATUS
EFIAPI
DashPldmGetSmbiosStructureByType (
  AMD_DASH_PLDM_PROTOCOL                                 *This,
  DASH_PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT *Buffer,
  UINT32                                            *BufferSize,
  UINT8                                             Type,
  UINT16                                            StructureInstanceId
);

EFI_STATUS
EFIAPI
DashPldmGetBiosTable (
  AMD_DASH_PLDM_PROTOCOL                   *This,
  UINT8                               TableType,
  DASH_PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                              *BufferSize
);

EFI_STATUS
EFIAPI
DashPldmSetBiosTable (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT8             TableType,
  VOID              *Buffer,
  UINT32            *BufferSize
);

EFI_STATUS
EFIAPI
DashPldmGetBiosTableTags (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            *TableTag
);

EFI_STATUS
EFIAPI
DashPldmSetBiosTableTags (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            TableTag
);

EFI_STATUS
EFIAPI
DashPldmAcceptBiosAttributesPendingValues (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT16            NumberOfAttributeHandles,
  UINT16            *AttributeHandles
);

EFI_STATUS
EFIAPI
DashPldmAcceptBiosAttributesPendingValues (
  AMD_DASH_PLDM_PROTOCOL *This,
  UINT16            NumberOfAttributeHandles,
  UINT16            *AttributeHandles
);

EFI_STATUS
EFIAPI
DashPldmBiosGetDateTime (
  AMD_DASH_PLDM_PROTOCOL                  *This,
  DASH_PLDM_GET_DATE_TIME_RESPONSE_FORMAT *Buffer,
  UINT32                             *BufferSize
);

EFI_STATUS
EFIAPI
DashPldmBiosSetDateTime (
  AMD_DASH_PLDM_PROTOCOL *This
);

#endif
