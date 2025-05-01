
/*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_COMMON_FCHCOMMON_FILECODE

/**
 * ReadFchChipsetRevision - Get FCH chipset revision
 *
 *
 * @param[in] StdHeader - Amd Configuration Parameters.
 *
 */
UINT8
ReadFchChipsetRevision (
  IN AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8    FchChipSetRevision;
  UINT8    FchMiscRegister;
  ReadPmio (FCH_PMIOA_REGC8 + 1, AccessWidth8, &FchMiscRegister, StdHeader);
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC8 + 1, AccessWidth8, ~(UINT32) BIT7, 0);
  ReadPci ((SMBUS_BUS_DEV_FUN << 16) + FCH_CFG_REG08, AccessWidth8, &FchChipSetRevision, StdHeader);
  WritePmio (FCH_PMIOA_REGC8 + 1, AccessWidth8, &FchMiscRegister, StdHeader);
  return FchChipSetRevision;
}

/**
 * ReadSocType - Get Soc ID
 *
 *
 *
 */
UINT32
ReadSocType (
  VOID
  )
{
  UINT32    SocId;

  ReadPci ((AMD_D0F0 << 16) + FCH_CFG_REG00, AccessWidth32, &SocId, NULL);

  return SocId;
}


