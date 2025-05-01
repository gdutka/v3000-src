/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/FchBaseLib.h>
#include "FchRegistersCommon.h"
#include "FchBiosRamUsage.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_FCHBASELIB_FCHBIOSRAMLIB_FILECODE



/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
 /*----------------------------------------------------------------------------------------*/

 /*----------------------------------------------------------------------------------------*/

 /**
 * @brief  FchReadBiosram - Read BIOSRAM
 *
 *
 *
 * @param[in] Address  - BIOSRAM Offset value
 * @param[in] OpFlag   - Access sizes
 * @param[in] Value    - Read Data Buffer
 * @param[in] StdHeader
 *
 */
VOID
FchReadBiosram (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        OpFlag,
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8    Index, Width;
  UINT8    Pmio04;
  UINT8    Or, Mask;

  Or = BIOS_RAM_EN;
  Mask = BIOS_RAM_EN;

  LibAmdMemRead (AccessWidth8, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG04), &Pmio04, StdHeader);
  if (!(Pmio04 & BIOS_RAM_EN)) {
    LibAmdMemRMW (AccessWidth8, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG04), &Or, &Mask, StdHeader);
  }

  Width = OpFlag & 0x7F;
  Width = 1 << (Width - 1);
  for (Index = 0; Index < Width; Index++) {
    LibAmdMemRead (AccessWidth8, (UINT64)(BIOS_RAM_MMIO_BASE + Address + Index), (UINT8 *)Value + Index, StdHeader);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchWriteBiosram - Write BIOSRAM
 *
 *
 *
 * @param[in] Address  - BIOSRAM Offset value
 * @param[in] OpFlag   - Access sizes
 * @param[in] Value    - Write Data Buffer
 * @param[in] StdHeader
 *
 */
VOID
FchWriteBiosram (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        OpFlag,
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8    Index, Width;
  UINT8    Pmio04;
  UINT8    Or, Mask;

  Or = BIOS_RAM_EN;
  Mask = BIOS_RAM_EN;

  LibAmdMemRead (AccessWidth8, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG04), &Pmio04, StdHeader);
  if (!(Pmio04 & BIOS_RAM_EN)) {
    LibAmdMemRMW (AccessWidth8, (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG04), &Or, &Mask, StdHeader);
  }

  Width = OpFlag & 0x7F;
  Width = 1 << (Width - 1);
  for (Index = 0; Index < Width; Index++) {
    LibAmdMemWrite (AccessWidth8, (UINT64)(BIOS_RAM_MMIO_BASE + Address + Index), (UINT8 *)Value + Index, StdHeader);
  }
}
