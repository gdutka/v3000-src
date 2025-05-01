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

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_FCHBASELIB_FCHCONFIGLIB_FILECODE



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

/**
 * @brief  FchRecordFchConfigPtr - Record the address of FCH configuration structure
 *
 *
 * @param[in] FchConfigPtr - the address of FCH configuration structure
 * @param[in] StdHeader - Standard configuration header
 *
 *
 */
VOID
FchRecordFchConfigPtr (
  IN       UINT32 FchConfigPtr,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT8 *pFchConfigPtr = NULL;

  pFchConfigPtr = (UINT8 *) &FchConfigPtr;
  //RwMem (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x08, AccessWidth8, 0, (UINT8) ((FchConfigPtr >> 0) & 0xFF) );
  LibAmdMemWrite (
    AccessWidth8,
    (UINT64) (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x08),
    (VOID *)((UINT8 *)pFchConfigPtr),
    StdHeader
    );
  //RwMem (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x09, AccessWidth8, 0, (UINT8) ((FchConfigPtr >> 8) & 0xFF) );
  LibAmdMemWrite (
    AccessWidth8,
    (UINT64) (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x09),
    (VOID *)((UINT8 *)pFchConfigPtr + 1),
    StdHeader
    );
  //RwMem (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x0A, AccessWidth8, 0, (UINT8) ((FchConfigPtr >> 16) & 0xFF) );
  LibAmdMemWrite (
    AccessWidth8,
    (UINT64) (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x0A),
    (VOID *)((UINT8 *)pFchConfigPtr + 2),
    StdHeader
    );
  //RwMem (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x0B, AccessWidth8, 0, (UINT8) ((FchConfigPtr >> 24) & 0xFF) );
  LibAmdMemWrite (
    AccessWidth8,
    (UINT64) (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x0B),
    (VOID *)((UINT8 *)pFchConfigPtr + 3),
    StdHeader
    );
}
