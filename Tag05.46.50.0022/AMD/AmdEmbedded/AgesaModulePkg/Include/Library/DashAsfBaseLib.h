/*****************************************************************************
 * Copyright (C) 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************
*/

#ifndef _DASH_ASF_BASE_LIB_H_
#define _DASH_ASF_BASE_LIB_H_

#define PMIO_BASE       0x300
#define ACPI_MMIO_BASE  0xFED80000ul
#define FCH_PMIOA_REG00 0x00

/*----------------------------------------------------------------------------
 *                           INCLUDE STANDARD HEADER FILES
 *----------------------------------------------------------------------------
 */

#include<AMD.h>

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *----------------------------------------------------------------------------
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
);

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
DashAsfMasterEnable(IN BOOLEAN DashAsfSmMasterEn);

#endif

