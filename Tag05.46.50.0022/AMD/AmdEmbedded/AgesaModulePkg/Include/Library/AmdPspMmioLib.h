/*
*****************************************************************************
*
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#ifndef _AMD_PSP_MMIO_LIB_H_
#define _AMD_PSP_MMIO_LIB_H_

#include "AMD.h"

/**
 * @brief Read from PSP MMIO ofset
 *
 * @param Offset  Offset to PSP MMIO base
 * @return UINT32 Value read from offset
 */
UINT32
EFIAPI
PspMmioRead (
  IN UINT32 Offset
  );

/**
 * @brief Read PSP MMIO with given offset
 *
 * @param Offset  Offset to PSP MMIO base
 * @param Value   Value to be write
 * @return VOID
 */
VOID
EFIAPI
PspMmioWrite (
  IN UINT32 Offset,
  IN UINT32 Value
  );


#endif


