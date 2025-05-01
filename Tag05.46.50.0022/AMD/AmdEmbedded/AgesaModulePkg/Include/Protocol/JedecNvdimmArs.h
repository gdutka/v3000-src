/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/** @file NvdimmArs.h
    Header file for Nvdimm ARS Driver

**/

#ifndef _NVDIMM_ARS_H_
#define _NVDIMM_ARS_H_

#include <IndustryStandard/Acpi.h>

//---------------------------------------------------------------------------

#define   NFIT_SPA_RANGE_STRUC_TYPE         EFI_ACPI_6_2_NFIT_SYSTEM_PHYSICAL_ADDRESS_RANGE_STRUCTURE_TYPE
#define   NFIT_NVDIMM_REG_MAP_STRUC_TYPE    EFI_ACPI_6_2_NFIT_NVDIMM_REGION_MAPPING_STRUCTURE_TYPE
#define   NFIT_NVDIMM_CTRL_REG_STRUC_TYPE   EFI_ACPI_6_2_NFIT_NVDIMM_CONTROL_REGION_STRUCTURE_TYPE
#define   NFIT_SIGNATURE                    EFI_ACPI_6_2_NVDIMM_FIRMWARE_INTERFACE_TABLE_STRUCTURE_SIGNATURE

typedef
EFI_STATUS
(EFIAPI *QUERY_ARS_UNIT_LENGTH) (
    OUT   UINT64  *UnitLength
    );

typedef
EFI_STATUS
(EFIAPI *CLEAR_UNCORRECTABLE_ERROR) (
    IN    UINT64   SpaRangeBase,
    IN    UINT64   SpaRangeLength
    );

typedef
EFI_STATUS
(EFIAPI *START_ARS) (
    IN    UINT64    SpaStartAddress,
    IN    UINT64    SpaEndAddress,
    IN    UINT64    ScrubGranularity,
    IN    BOOLEAN   HighPriority
    );

typedef
EFI_STATUS
(EFIAPI *STOP_ARS) (
    );

typedef
EFI_STATUS
(EFIAPI *SPA_IS_PERSISTENT) (
    IN    UINT64                                                      Spa,
    OUT   BOOLEAN                                                     *Presistent,
    OUT   UINT32                                                      *NvdimmNfitHandle,
    OUT   EFI_ACPI_6_2_NFIT_SYSTEM_PHYSICAL_ADDRESS_RANGE_STRUCTURE   *NfitSpaRangeStruc
    );

typedef
EFI_STATUS
(EFIAPI *SPA_IS_MIRRORED_INTERLEAVE_SET) (
    IN    UINT64      Spa
    );

struct _AMD_NVDIMM_ARS_SMM_PROTOCOL {
  QUERY_ARS_UNIT_LENGTH             QueryArsUnitLength;
  CLEAR_UNCORRECTABLE_ERROR         ClearUncorrectableError;
  START_ARS                         StartArs;
  STOP_ARS                          StopArs;
  SPA_IS_PERSISTENT                 SpaIsPersistent;
  SPA_IS_MIRRORED_INTERLEAVE_SET    SpaIsMirroredInterleaveSet;
};

typedef struct _AMD_NVDIMM_ARS_SMM_PROTOCOL  AMD_NVDIMM_ARS_SMM_PROTOCOL;

extern EFI_GUID gAmdJedecNvdimmArsSmmProtocolGuid;

#endif  // _NVDIMM_ARS_H_
