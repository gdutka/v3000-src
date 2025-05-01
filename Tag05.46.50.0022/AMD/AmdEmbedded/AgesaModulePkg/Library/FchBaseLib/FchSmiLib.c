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
#define FILECODE LIBRARY_FCHBASELIB_FCHSMILIB_FILECODE


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
 * @brief  FchRecordSmiStatus - Record SMI Status
 *
 *
 * @param[in] StdHeader
 *
 */
VOID
FchRecordSmiStatus (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINTN   Index;
  UINT8   SwSmiValue;

  //ACPIMMIO8 (0xfed80304) |= 0x01;
  ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG04) |= BIOS_RAM_EN;
  for ( Index = 0; Index < 20; Index++ ) {
    //ACPIMMIO8 (0xfed10020 + Index) = ACPIMMIO8 (0xfed80280 + Index);
    ACPIMMIO8 (BIOS_RAM_MMIO_BASE + BIOS_RAM_REG20 + Index) =
      ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG80 + Index);
  }
  LibAmdIoRead (AccessWidth8, 0xB0, &SwSmiValue, StdHeader);
  //ACPIMMIO8 (0xfed10040) = SwSmiValue;
  ACPIMMIO8 (BIOS_RAM_MMIO_BASE + BIOS_RAM_REG40) = SwSmiValue;
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchClearAllSmiStatus - Clear All SMI Status
 *
 *
 * @param[in] StdHeader
 *
 */
VOID
FchClearAllSmiStatus (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINTN   Index;

  for ( Index = 0; Index < 20; Index++ ) {
    //ACPIMMIO8 (0xfed80280 + Index) |= 0;
    ACPIMMIO8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG80 + Index) |= 0;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchSleepTrapControl - FCH Sleep Trap Control
 *
 *
 *
 * @param[in] SleepTrap    - Whether sleep trap is enabled
 *
 */
VOID
FchSleepTrapControl (
  IN        BOOLEAN          SleepTrap
  )
{
  if (SleepTrap) {
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) &= ~(BIT2 + BIT3);
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) |= BIT2;

    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE) &= ~ (BIT5);
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) &= ~ (BIT0 + BIT1);
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) |= BIT1;
  } else {
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGBE) |= BIT5;
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) &= ~ (BIT0 + BIT1);
    ACPIMMIO8 (ACPI_MMIO_BASE + PMIO_BASE + 0xB) |= BIT0;

    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB0) &= ~(BIT2 + BIT3);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchSmiGenerateControl - FCH SMI Enablement
 *
 *
 *
 * @param[in] SmiControl  - Whether enable or disable SMI function
 *
 */
VOID
FchSmiGenerateControl (
  IN        BOOLEAN          SmiControl
  )
{
  if (SmiControl) {
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) &= ~(BIT31);
  } else {
    ACPIMMIO32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG98) |= BIT31;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchDisablePowerButtonSmi - Config Fch during ACPI_ON
 *
 *
 * @param[in] StdHeader - Standard configuration header
 *
 */
VOID
FchDisablePowerButtonSmi (
  IN  AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT8    Or8;
  UINT8    Mask8;
  //
  // Disable Power Button SMI
  //
  //RwMem (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAC, AccessWidth8, ~(UINT32) (BIT6), 0);
  Or8 = 0;
  Mask8 = BIT6;
  LibAmdMemRMW (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAC), &Or8, &Mask8, StdHeader);
}


