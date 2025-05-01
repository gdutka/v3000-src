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
#include <Library/IdsLib.h>
#include <Library/FchBaseLib.h>
#include "FchRegistersCommon.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_FCHBASELIB_FCHPMIOLIB_FILECODE


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
 * @brief  Read PMIO
 *
 *
 *
 * @param[in] Address       - PMIO Offset value
 * @param[in] AccessWidth   - Access sizes
 * @param[in] Value         - Read Data Buffer
 *
 */
VOID
FchReadPmio (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        AccessWidth,
  IN       VOID                *Value
  )
{
  UINT8    i;
  UINT8    ByteCount;
  AMD_CONFIG_PARAMS  *StdHeader;

  StdHeader = NULL;

  switch (AccessWidth) {
  case AccessWidth8:
    ByteCount = 1;
    break;
  case AccessWidth16:
    ByteCount = 2;
    break;
  case AccessWidth32:
    ByteCount = 4;
    break;
  default:
    ByteCount = 0;
    ASSERT (FALSE);
  }

  for (i = 0; i < ByteCount; i++) {
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGCD6, &Address, StdHeader);
    Address++;
    LibAmdIoRead (AccessWidth8, FCH_IOMAP_REGCD7, (UINT8 *)Value + i, StdHeader);
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  Read PMIO by Acpi Mmio
 *
 *
 *
 * @param[in] Address       - PMIO Offset value
 * @param[in] AccessWidth   - Access sizes
 * @param[in] Value         - Read Data Buffer
 *
 */
VOID
FchReadPmioV2 (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        AccessWidth,
  IN       VOID                *Value
  )
{
  UINT8    i;
  UINT8    ByteCount;
  AMD_CONFIG_PARAMS  *StdHeader;

  StdHeader = NULL;

  switch (AccessWidth) {
  case AccessWidth8:
    ByteCount = 1;
    break;
  case AccessWidth16:
    ByteCount = 2;
    break;
  case AccessWidth32:
    ByteCount = 4;
    break;
  default:
    ByteCount = 0;
    ASSERT (FALSE);
  }

  for (i = 0; i < ByteCount; i++, Address++, Value=(UINT8*)Value+1) {
    LibAmdMemRead (
      AccessWidth8,
      (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + (UINT64)Address),
      Value,
      StdHeader
      );
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  Write PMIO
 *
 *
 *
 * @param[in] Address       - PMIO Offset value
 * @param[in] AccessWidth   - Access sizes
 * @param[in] Value         - Write Data Buffer
 *
 */
VOID
FchWritePmio (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        AccessWidth,
  IN       VOID                *Value
  )
{
  UINT8    i;
  UINT8    ByteCount;
  AMD_CONFIG_PARAMS  *StdHeader;

  StdHeader = NULL;

  switch (AccessWidth) {
  case AccessWidth8:
    ByteCount = 1;
    break;
  case AccessWidth16:
    ByteCount = 2;
    break;
  case AccessWidth32:
    ByteCount = 4;
    break;
  default:
    ByteCount = 0;
    ASSERT (FALSE);
  }

  for (i = 0; i < ByteCount; i++) {
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGCD6, &Address, StdHeader);
    Address++;
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGCD7, (UINT8 *)Value + i, StdHeader);
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  Write PMIO by Acpi Mmio
 *
 *
 *
 * @param[in] Address       - PMIO Offset value
 * @param[in] AccessWidth   - Access sizes
 * @param[in] Value         - Write Data Buffer
 *
 */
VOID
FchWritePmioV2 (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        AccessWidth,
  IN       VOID                *Value
  )
{
  UINT8    i;
  UINT8    ByteCount;
  AMD_CONFIG_PARAMS  *StdHeader;

  StdHeader = NULL;

  switch (AccessWidth) {
  case AccessWidth8:
    ByteCount = 1;
    break;
  case AccessWidth16:
    ByteCount = 2;
    break;
  case AccessWidth32:
    ByteCount = 4;
    break;
  default:
    ByteCount = 0;
    ASSERT (FALSE);
  }

  for (i = 0; i < ByteCount; i++, Address++, Value=(UINT8*)Value+1) {
    LibAmdMemWrite (
      AccessWidth8,
      (UINT64)(ACPI_MMIO_BASE + PMIO_BASE + (UINT64)Address),
      Value,
      StdHeader
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  RwPmio - Read/Write PMIO
 *
 *
 *
 * @param[in] Address         - PMIO Offset value
 * @param[in] AccessWidth     - Access sizes
 * @param[in] AndMask         - Data And Mask 32 bits
 * @param[in] OrMask          - Data OR Mask 32 bits
 *
 */
VOID
FchRwPmio (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        AccessWidth,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask
  )
{
  UINT32 Result;

  FchReadPmio (Address, AccessWidth, &Result);
  Result = (Result & AndMask) | OrMask;
  FchWritePmio (Address, AccessWidth, &Result);
}


/*----------------------------------------------------------------------------------------*/
/**
 * @brief  RwPmio - Read/Write PMIO by Acpi Mmio
 *
 *
 *
 * @param[in] Address         - PMIO Offset value
 * @param[in] AccessWidth     - Access sizes
 * @param[in] AndMask         - Data And Mask 32 bits
 * @param[in] OrMask          - Data OR Mask 32 bits
 *
 */
VOID
FchRwPmioV2 (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        AccessWidth,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask
  )
{
  UINT32 Result;

  FchReadPmioV2 (Address, AccessWidth, &Result);
  Result = (Result & AndMask) | OrMask;
  FchWritePmioV2 (Address, AccessWidth, &Result);
}

