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
#include "AGESA.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_FCHBASELIB_FCHSCILIB_FILECODE



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
 * @brief  FchSetFchSciMapTbl - Program FCH SCI Map table (8 bits data)
 *
 *
 *
 * @param[in] pSciMapTbl   - Table data pointer
 * @param[in] StdHeader
 *
 */
VOID
FchSetFchSciMapTbl (
  IN       SCI_MAP_CONTROL  *pSciMapTbl,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32  ddtempVar;

  if (pSciMapTbl != NULL) {
    while (pSciMapTbl->InputPin != 0xFF) {
      if ((pSciMapTbl->InputPin >= 0x40) && (pSciMapTbl->InputPin < 0x80) && (pSciMapTbl->GpeMap < 0x20)) {
        ddtempVar = ACPI_MMIO_BASE | SMI_BASE | pSciMapTbl->InputPin;
        LibAmdMemWrite (AccessWidth8, (UINT64) ddtempVar, &pSciMapTbl->GpeMap, StdHeader);
      } else {
        //Assert Warning "SCI map is invalid"
      }
      pSciMapTbl++;
    }
  }
}


