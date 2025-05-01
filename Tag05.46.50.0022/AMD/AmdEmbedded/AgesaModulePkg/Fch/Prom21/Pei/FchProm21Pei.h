/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_PROM21_PEI_H_
#define _FCH_PROM21_PEI_H_

//----------------------------------------------------
//
//  Amd PT Init Done Ppi
//
//-------------------------------------------------------

typedef struct _PEI_PT_INIT__PPI {
UINT32    Revision;         ///< revision
} PEI_PT_INIT__PPI;

#define AMD_PT_INIT_REVISION   0x00

VOID
ScanSecXhciDidVid (
  IN      UINT8     GppBus,
  IN      UINT8     GppDev,
  IN      UINT8     GppFun,
  IN      UINT8     TempBusNum
  );

VOID
FwChecksum (
  IN      UINT8     *PtFwAddress,
  IN      UINT32    PtFwSize,
  IN OUT  UINT32    *Checksum
  );

EFI_STATUS
EFIAPI
FchProm21PeiInit (
  IN      EFI_PEI_FILE_HANDLE     FileHandle,
  IN      CONST EFI_PEI_SERVICES  **PeiServices
  );

EFI_STATUS
AmdPTPlusFwLoadPei (
  IN       EFI_PEI_SERVICES    **PeiServices
  );

#endif

