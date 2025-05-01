/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTPCIE_KTABRESETSERVICE_FILECODE


/**
 * FchProgramAbPowerOnReset - Config Ab Bridge during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchProgramAbPowerOnReset (
  IN  VOID     *FchDataPtr
  )
{
  //
  // Set A-Link bridge access address.
  // This is an I/O address. The I/O address must be on 16-byte boundary.
  //
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGE0, AccessWidth32, 00, ALINK_ACCESS_INDEX);
}


