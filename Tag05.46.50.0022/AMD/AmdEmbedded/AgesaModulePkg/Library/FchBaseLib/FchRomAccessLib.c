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
#define FILECODE LIBRARY_FCHBASELIB_FCHROMACCESSLIB_FILECODE
#define FCH_ROMSIG_SIGNATURE                 0x55AA55AAul
#define FCH_ROMSIG_BASE_IO                   0x20000l

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
 * @brief  FchGetRomSigPtr - get the address of Rom signature
 *
 *
 * @param[in] RomSigPtr - the Pointer to Rom Signature
 * @param[in] StdHeader - Standard configuration header
 *
 */
VOID*
FchGetRomSigPtr (
  IN      UINTN               *RomSigPtr,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32   RomPtr;
  UINT32   RomSig;
  UINT16   MswAddr;

  *RomSigPtr = 0;
  MswAddr = 0xFF00;
  do {
    RomPtr = (MswAddr << 16) + FCH_ROMSIG_BASE_IO;
    LibAmdMemRead (AccessWidth32, (UINT64) RomPtr, &RomSig, StdHeader);
    if (RomSig == FCH_ROMSIG_SIGNATURE) {
      *RomSigPtr = RomPtr;
      break;
    }
    MswAddr >>= 1;
    MswAddr |= BIT15;
  } while (MswAddr != 0xFFFC);//Change from 0xFFF8 to 0xFFFC to cover ROM_SIG@0xFFFA0000
  if (*RomSigPtr == 0) {
    //Pointer not found
    IDS_HDT_CONSOLE (FCH_TRACE, "GetRomSigPtr failed to find Ptr HALT\n" );
    FCH_DEADLOOP();
  }
  return RomSigPtr;
}
