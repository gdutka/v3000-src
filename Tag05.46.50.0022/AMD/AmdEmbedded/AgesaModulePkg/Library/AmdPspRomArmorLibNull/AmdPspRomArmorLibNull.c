/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include <Uefi.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Filecode.h>
#include <Library/AmdPspRomArmorLib.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define FILECODE LIBRARY_AMDPSPROMARMORLIBNULL_AMDPSPROMARMORLIBNULL_FILECODE

/*----------------------------------------------------------------------------------------
 *           F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/**
 * Request secures the SPI Controller
 *
 * @param[in, out]  SpiCommunicationBuffer    SPI Communication Structure Buffer pointer
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspEnterSmmOnlyMode (
  IN     SPI_COMMUNICATION_BUFFER  *SpiCommunicationBuffer
)
{
  return EFI_UNSUPPORTED;
}

/**
 * Loads the whitelist into the PSP.
 *
 * @param[in]       SpiWhitelist              SPI White List structure buffer pointer.
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspEnforceWhitelist (
  IN     SPI_WHITE_LIST  *SpiWhitelist
)
{
  return EFI_UNSUPPORTED;
}

/**
 * Request execute SPI command provide in TSEG comm buffer.
 *
 * @param[in]       NONE
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspExecuteSpiCommand (VOID)
{
  return EFI_UNSUPPORTED;
}

/**
 * Request PSP firmware swtich SPI controller chip select.
 *
 *
 * @param[in]       Chipset                   SPI controller chip select, 0= Allowed on all chip selects, 1= CS1, 2= CS2, all else invalid
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspSwitchChipSelect (
  IN     UINT8      ChipSelect
)
{
  return EFI_UNSUPPORTED;
}
