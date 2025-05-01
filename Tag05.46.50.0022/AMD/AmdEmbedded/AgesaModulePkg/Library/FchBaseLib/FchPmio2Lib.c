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
#define FILECODE LIBRARY_FCHBASELIB_FCHPMIO2LIB_FILECODE



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
 * @brief  FchReadPmio2 - Read PMIO2
 *
 *
 *
 * @param[in] Address  - PMIO2 Offset value
 * @param[in] OpFlag   - Access sizes
 * @param[in] Value    - Read Data Buffer
 * @param[in] StdHeader
 *
 */
VOID
FchReadPmio2 (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        OpFlag,
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8 i;

  OpFlag = OpFlag & 0x7F;
  OpFlag = 1 << (OpFlag - 1);
  for ( i = 0; i < OpFlag; i++ ) {
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGCD0, &Address, StdHeader);
    Address++;
    LibAmdIoRead (AccessWidth8, FCH_IOMAP_REGCD1, (UINT8 *) Value + i, StdHeader);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchWritePmio2 - Write PMIO 2
 *
 *
 *
 * @param[in] Address  - PMIO2 Offset value
 * @param[in] OpFlag   - Access sizes
 * @param[in] Value    - Write Data Buffer
 * @param[in] StdHeader
 *
 */
VOID
FchWritePmio2 (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        OpFlag,
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8 i;

  OpFlag = OpFlag & 0x7F;
  OpFlag = 1 << (OpFlag - 1);

  for ( i = 0; i < OpFlag; i++ ) {
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGCD0, &Address, StdHeader);
    Address++;
    LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGCD1, (UINT8 *) Value + i, StdHeader);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief  FchRwPmio2 - Read/Write PMIO2
 *
 *
 *
 * @param[in] Address    - PMIO2 Offset value
 * @param[in] OpFlag     - Access sizes
 * @param[in] AndMask    - Data And Mask 32 bits
 * @param[in] OrMask     - Data OR Mask 32 bits
 * @param[in] StdHeader
 *
 */
VOID
FchRwPmio2 (
  IN       UINT8               Address,
  IN       ACCESS_WIDTH        OpFlag,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32 Result;

  FchReadPmio2 (Address, OpFlag, &Result, StdHeader);
  Result = (Result & AndMask) | OrMask;
  FchWritePmio2 (Address, OpFlag, &Result, StdHeader);
}
