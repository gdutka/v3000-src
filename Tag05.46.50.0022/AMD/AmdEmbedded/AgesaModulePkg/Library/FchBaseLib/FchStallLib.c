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
#define FILECODE LIBRARY_FCHBASELIB_FCHSTALLLIB_FILECODE
#define FCHOEM_IO_DELAY_PORT               0x80
#define FCH_MAX_TIMER                      0xFFFFFFFFul
#define FCHOEM_ELAPSED_TIME_UNIT           28
#define FCHOEM_ELAPSED_TIME_DIVIDER        100

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
 * @brief  FchGetSocId - Get Soc ID
 *
 *
 * @param[in] uSec - the time in us
 * @param[in] StdHeader - Standard configuration header
 *
 */
VOID
FchStall (
  IN       UINT32              uSec,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT16 timerAddr;
  UINT32 startTime;
  UINT32 elapsedTime;

  LibAmdMemRead (AccessWidth16, (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG64), &timerAddr, StdHeader);
  if ( timerAddr == 0 ) {
    uSec = uSec / 2;
    while ( uSec != 0 ) {
      LibAmdIoRead (AccessWidth8, FCHOEM_IO_DELAY_PORT, (UINT8 *) (&startTime), StdHeader);
      uSec--;
    }
  } else {
    LibAmdIoRead (AccessWidth32, timerAddr, &startTime, StdHeader);
    for ( ;; ) {
      LibAmdIoRead (AccessWidth32, timerAddr, &elapsedTime, StdHeader);
      if ( elapsedTime < startTime ) {
        elapsedTime = elapsedTime + FCH_MAX_TIMER - startTime;
      } else {
        elapsedTime = elapsedTime - startTime;
      }
      if ( (elapsedTime * FCHOEM_ELAPSED_TIME_UNIT / FCHOEM_ELAPSED_TIME_DIVIDER) > uSec ) {
        break;
      }
    }
  }
}
