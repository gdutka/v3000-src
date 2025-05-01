/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include <Library/AmdBaseLib.h>
#include <Library/AmdDirectoryBaseLib.h>
#include <Library/AmdPspFwImageHeaderLib.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Arch.h>

#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE  LIBRARY_AMDPSPFWIMAGEHEADERLIB_AMDPSPFWIMAGEHEADERLIB_FILECODE

#define APCB_SIGNATURE              0x42435041ul
#define APCB_FW_IMAGE_HEADER_LENGTH 0x100
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

/*----------------------------------------------------------------------------------------
 *                          G L O B A L        V A L U E S
 *----------------------------------------------------------------------------------------
 */

/**
 * @brief Check Apcb Data has PSP Fw Image Header or not
 *
 * @param[in]       UINT64  ApcbEntryAddress
 * @param[in,out]  UINT32* PSP Fw Image Header Length
 *
 * @return  TRUE    Apcb Data has PSP Fw Image Header
 * @return  FALSE   Apcb Data did not have PSP Fw Image Header
 */
BOOLEAN
ApcbFwImageHeaderCheck (
  UINT64  ApcbEntryAddress,
  UINT32  *PspFwImageHeaderLen
)
{
  APCB_V3_HEADER ApcbHeader;

  *PspFwImageHeaderLen = 0;

  if (ApcbEntryAddress != 0) {
    if (MapSpiDataToBuffer ((UINT32)(UINTN) ApcbEntryAddress, &ApcbHeader, sizeof (APCB_V3_HEADER))) {
      if (APCB_SIGNATURE == ApcbHeader.Signature) {
        return FALSE;
      }
    }

    ApcbEntryAddress += APCB_FW_IMAGE_HEADER_LENGTH;
    if (MapSpiDataToBuffer ((UINT32)(UINTN) ApcbEntryAddress, &ApcbHeader, sizeof (APCB_V3_HEADER))) {
      if (APCB_SIGNATURE == ApcbHeader.Signature) {
        *PspFwImageHeaderLen = APCB_FW_IMAGE_HEADER_LENGTH;
        return TRUE;
      }
    }
  }

  return FALSE;
}
