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
#define FILECODE LIBRARY_FCHBASELIB_FCHCHIPSETLIB_FILECODE
#define FCHOEM_OUTPUT_80_PORT               0x80
#define FCHOEM_OUTPUT_1080_PORT             0x1080

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
/**
 * @brief  FchGetChipStrap - Get Chip Strap status
 *
 *
 * @param[in] Value - Return Chip strap status
 * @param[in] StdHeader
 *
 */
VOID
FchGetChipStrap (
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  LibAmdMemRead (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + MISC_BASE + FCH_MISC_REG80), Value, StdHeader);
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchGetEfuseStatus - Get Efuse status
 *
 *
 * @param[in] Value - Return Efuse Status
 * @param[in] StdHeader
 *
 */
VOID
FchGetEfuseStatus (
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8    Or8;
  UINT8    Mask8;

  Or8 = BIT5;
  Mask8 = BIT5;
  LibAmdMemRMW (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC8), &Or8, &Mask8, StdHeader);
  LibAmdMemWrite (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8), Value, StdHeader);
  LibAmdMemRead (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD8 + 1), Value, StdHeader);
  Or8 = 0;
  Mask8 = BIT5;
  LibAmdMemRMW (AccessWidth8, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGC8), &Or8, &Mask8, StdHeader);
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchAcLossControl - control the action of AC power loss
 *
 *
 * @param[in] AcLossControlValue - the value to control AC power loss
 * @param[in] StdHeader - Standard configuration header
 *
 *
 */
VOID
FchAcLossControl (
  IN       UINT8 AcLossControlValue,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8    Mask8;
  //[Replaced] RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5B, AccessWidth8, 0xF0, AcLossControlValue);
  Mask8 = 0x0F;
  AcLossControlValue &= 0x03;
  AcLossControlValue |= BIT2;
  LibAmdMemRMW (
    AccessWidth8,
    (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5B),
    &AcLossControlValue,
    &Mask8,
    StdHeader
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  OutPort80 - output post code to Port 80
 *
 *
 * @param[in] pcode - post code
 * @param[in] StdHeader - Standard configuration header
 *
 */
VOID
FchOutPort80 (
  IN       UINT32              pcode,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  LibAmdIoWrite (AccessWidth8, FCHOEM_OUTPUT_80_PORT, &pcode, StdHeader);
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  OutPort1080 - output post code to Port 1080
 *
 *
 * @param[in] pcode - post code
 * @param[in] StdHeader - Standard configuration header
 *
 */
VOID
FchOutPort1080 (
  IN       UINT32              pcode,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  LibAmdIoWrite (AccessWidth32, FCHOEM_OUTPUT_1080_PORT, &pcode, StdHeader);
  return;
}
