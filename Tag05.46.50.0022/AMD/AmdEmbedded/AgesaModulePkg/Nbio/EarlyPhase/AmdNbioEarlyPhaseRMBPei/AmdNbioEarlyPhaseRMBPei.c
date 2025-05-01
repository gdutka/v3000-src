/*****************************************************************************
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AmdNbioEarlyPhaseRVPei Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioEarlyPhaseRVPei
 * @e \$Revision: 312065 $   @e \$Date: 2019-05-29 17:09:05 +0800 (Wed, 29 May 2019) $
 *
 */
#include <AMD.h>
#include <Filecode.h>
#include <PiPei.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/IdsLib.h>

#define FILECODE  NBIO_EARLYPHASE_AMDNBIOEARLYPHASERMBPEI_AMDNBIOEARLYPHASERMBPEI_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                    P P I   N O T I F Y   D E S C R I P T O R S
 *----------------------------------------------------------------------------------------
 */




/*----------------------------------------------------------------------------------------*/
/**
 * AmdNbioEarlyPhase driver entry point for RV
 *
 *
 *
 * @param[in]  FileHandle  Standard configuration header
 * @param[in]  PeiServices Pointer to EFI_PEI_SERVICES pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
EFIAPI
AmdNbioEarlyPhaseRMBPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  UINT32   Value;
  UINTN    PciAddr;

  AGESA_TESTPOINT (TpEarlyEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "AmdNbioEarlyPhaseRMBPeiEntry Entry\n");

  PciAddr = MAKE_SBDFO (0, 0, 0, 0, 0xB8);
  PciWrite32 (PciAddr, 0x5D4DC); // Read FUSE_DATA_311

  PciAddr = MAKE_SBDFO (0, 0, 0, 0, 0xBC);
  Value = PciRead32 (PciAddr);

  Value = (Value >> 5) & 1; // Only need bit5
  PcdSet8S (PcdXgbeDisable, (UINT8) Value);
  IDS_HDT_CONSOLE (MAIN_FLOW, "PcdXgbeDisable = 0x%x\n", Value);

  IDS_HDT_CONSOLE (MAIN_FLOW, "AmdNbioEarlyPhaseRMBPeiEntry Exit\n");
  AGESA_TESTPOINT (TpEarlyExit, NULL);
  return EFI_SUCCESS;
}
