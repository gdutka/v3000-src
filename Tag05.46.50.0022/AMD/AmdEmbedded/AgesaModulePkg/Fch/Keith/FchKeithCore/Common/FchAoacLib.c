/*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_COMMON_FCHAOACLIB_FILECODE

#define FCH_AOAC_RETRY   100

/**
 * FchAoacPowerOnDev - Power On/Off Fch Device
 *
 * @param[in] Device     - Device to be turn on or off
 * @param[in] On         - 1 to turn on, 0 to turn off
 *
 */
VOID
FchAoacPowerOnDev (
  IN  UINT8      Device,
  IN  UINT8      On
  )
{
  UINT8  Retry = 0;
  if (On == 1) {
    while (((ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x41 + (Device << 1)) & 7) != 7) && (Retry < FCH_AOAC_RETRY)) {
      ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)) &= ~(UINT8) (BIT0 + BIT1 + BIT7);   //HW control
      ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)) |= BIT3;
      FchStall (10, NULL);
      Retry++;
    }

    if ((ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x41 + (Device << 1)) & 7) != 7) {
      // Log timeout failure
      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchAoacPowerOnDev Power ON Device 0x%x failed!\n", Device);
      // Power On one more time
      ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)) |= BIT3;
    }
  } else {
    while (((ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x41 + (Device << 1)) & 7) != 0) && (Retry < FCH_AOAC_RETRY)) {
      ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)) &= ~(UINT8) (BIT3 + BIT7);          //HW control
      FchStall (10, NULL);
      Retry++;
    }

    if ((ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x41 + (Device << 1)) & 7) != 0) {
      // Log timeout fail
      IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchAoacPowerOnDev Power OFF Device 0x%x failed!\n", Device);
      // Power Off with SW control
      ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)) = 0x80;
    }

    if (Device != FCH_AOAC_ESPI) {  //PLAT-26858
      ACPIMMIO8 (ACPI_MMIO_BASE + AOAC_BASE + 0x40 + (Device << 1)) |= BIT0 + BIT1;
    }
  }
}


