/*****************************************************************************
 * Copyright (C) 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
******************************************************************************
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/DashAsfBaseLib.h>
#include "Filecode.h"

#define FILECODE LIBRARY_DASHASFBASELIB_DASHASFBASELIB_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * @brief Dash function : DashAsfMmioWrite
 *
 * @details Mmio Write function
 *
 * @param AccessWidth Access width
 * @param MemAddress  Memory address
 *
 * @return Value Pointer to data
 */

VOID
DashAsfMmioWrite (
  IN ACCESS_WIDTH AccessWidth,
  IN UINT64 MemAddress,
  IN UINT32 Value
  )
{
  if (sizeof (UINTN) == sizeof (UINT32)) {
    ASSERT (MemAddress <= 0xFFFFFFFF);
  }
  switch (AccessWidth) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    MmioWrite8 ((UINTN) MemAddress, ((UINT8) Value));
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    MmioWrite16 ((UINTN) MemAddress, ((UINT16) Value));
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    MmioWrite32 ((UINTN) MemAddress, ((UINT32) Value));
    break;
  default:
    ASSERT (FALSE);
  }
}


/**
 * @brief Dash function : DashAsfMasterEnable
 *
 * @details Set Asf Smbus Master or Slave Mode
 *
 * @param DashAsfSmMasterEn - 1: Enable/ 0: disable
 *
 * @return Value The value after transferring
 */

VOID
DashAsfMasterEnable(
IN BOOLEAN DashAsfSmMasterEn)
{
  UINT8 Value;

  Value = 0;

  switch(DashAsfSmMasterEn){
    case FALSE:
        Value = 0x06;
      break;
    case TRUE:
        Value = 0x07;
      break;
    default:
      return;
      break;
  }

  DashAsfMmioWrite(AccessWidth8, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00 + 2), Value);
}

