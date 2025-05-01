/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef MPM_PLDM_BASE_H
#define MPM_PLDM_BASE_H

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiLib.h>

EFI_STATUS
EFIAPI
PldmGetSmbiosStructureTableMetaData (
  AMD_PLDM_PROTOCOL                                        *This,
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_METADATA_RESPONSE_FORMAT *Buffer,
  UINT32                                                   *BufferSize
);

EFI_STATUS
EFIAPI
PldmSetSmbiosStructureTableMetaData (
  AMD_PLDM_PROTOCOL *This
);

EFI_STATUS
EFIAPI
PldmGetSmbiosStructureTable (
  AMD_PLDM_PROTOCOL                               *This,
  PLDM_GET_SMBIOS_STRUCTURE_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                                          *BufferSize
);

EFI_STATUS
EFIAPI
PldmSetSmbiosStructureTable (
  AMD_PLDM_PROTOCOL *This
);

EFI_STATUS
EFIAPI
PldmGetSmbiosStructureByType (
  AMD_PLDM_PROTOCOL                                 *This,
  PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT *Buffer,
  UINT32                                            *BufferSize,
  UINT8                                             Type,
  UINT16                                            StructureInstanceId
);

EFI_STATUS
EFIAPI
PldmGetBiosTable (
  AMD_PLDM_PROTOCOL                   *This,
  UINT8                               TableType,
  PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT *Buffer,
  UINT32                              *BufferSize
);

EFI_STATUS
EFIAPI
PldmSetBiosTable (
  AMD_PLDM_PROTOCOL *This,
  UINT8             TableType,
  VOID              *Buffer,
  UINT32            *BufferSize
);

EFI_STATUS
EFIAPI
PldmGetBiosTableTags (
  AMD_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            *TableTag
);

EFI_STATUS
EFIAPI
PldmSetBiosTableTags (
  AMD_PLDM_PROTOCOL *This,
  UINT8             TableType,
  UINT32            TableTag
);

EFI_STATUS
EFIAPI
PldmAcceptBiosAttributesPendingValues (
  AMD_PLDM_PROTOCOL *This,
  UINT16            NumberOfAttributeHandles,
  UINT16            *AttributeHandles
);

EFI_STATUS
EFIAPI
PldmAcceptBiosAttributesPendingValues (
  AMD_PLDM_PROTOCOL *This,
  UINT16            NumberOfAttributeHandles,
  UINT16            *AttributeHandles
);

EFI_STATUS
EFIAPI
PldmBiosGetDateTime (
  AMD_PLDM_PROTOCOL                  *This,
  PLDM_GET_DATE_TIME_RESPONSE_FORMAT *Buffer,
  UINT32                             *BufferSize
);

EFI_STATUS
EFIAPI
PldmBiosSetDateTime (
  AMD_PLDM_PROTOCOL *This
);

EFI_STATUS
EFIAPI
PldmSensorPlatformEventMessage (
  AMD_PLDM_PROTOCOL *This,
  PLDM_SENSOR_T *SensorData

);
#endif
