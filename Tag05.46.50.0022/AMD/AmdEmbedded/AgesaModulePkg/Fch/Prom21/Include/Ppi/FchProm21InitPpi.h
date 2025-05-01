/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_PROM21_INIT_PPI_H_
#define _FCH_PROM21_INIT_PPI_H_

//
// GUID definition
//
#define AMD_FCH_PROM21_INIT_PPI_GUID \
  { 0xB3CCB71A, 0x7E56, 0x452F, 0xAB, 0x6B, 0xBB, 0x64, 0x12, 0xCA, 0xD8, 0x8E }

extern EFI_GUID gAmdFchProm21InitPpiGuid;

typedef struct _FCH_PROM21_INIT_PPI  FCH_PROM21_INIT_PPI;

/// FCH INIT PPI definition
typedef struct _FCH_PROM21_INIT_PPI {
  UINTN                   Revision;           ///< Revision number
} FCH_PROM21_INIT_PPI;

//
// current PPI revision
//
#define FCH_PROM21_INIT_PPI_REV  0x01

/// Module data structure
typedef struct _FCH_PROM21_PEI_PRIVATE {
  UINTN                   Signature;          ///< Signature
  FCH_PROM21_INIT_PPI     FchProm21InitPpi;   ///< FCH INIT PPI
} FCH_PROM21_PEI_PRIVATE;

#define FCH_PROM21_PEI_PRIVATE_DATA_SIGNATURE SIGNATURE_32 ('$', 'P', '2', '1')

#endif

