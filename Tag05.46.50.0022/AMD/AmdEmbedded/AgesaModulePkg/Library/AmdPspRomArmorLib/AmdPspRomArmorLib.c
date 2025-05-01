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
#include "AGESA.h"
#include <Library/IdsLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmBase2.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Protocol/PspMboxSmmBufferAddressProtocol.h>
#include <Filecode.h>
#include <Library/AmdPspRomArmorLib.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define FILECODE LIBRARY_AMDPSPROMARMORLIB_AMDPSPROMARMORLIB_FILECODE


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
UINT8                   *mArmorLibPspMboxSmmBuffer = NULL;
BOOLEAN                 *mArmorLibPspMboxSmmFlagAddr = NULL;


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                            L O C A L   F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                                 F U N C T I O N S
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
  EFI_STATUS        Status;

  Status = EFI_INVALID_PARAMETER;
  Status = PspMboxBiosArmor (ArmorEnterSmmOnlyMode, 0, SpiCommunicationBuffer, mArmorLibPspMboxSmmFlagAddr);

  return Status;
}

/**
 * Loads the whitelist into the PSP.
 *
 * @param[in]       SpiWhitelist              SPI White List structure buffer pointer.
 * @param[in]       SmmBuffer                 Point to allocated Smmbuffer, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmBuffer
 * @param[in]       SmmFlag                   Point to allocated SmmFlag, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmFlagAddr
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
  EFI_STATUS        Status;

  Status = EFI_INVALID_PARAMETER;
  Status = PspMboxBiosArmor (ArmorEnforceWhitelist, 0, SpiWhitelist, mArmorLibPspMboxSmmFlagAddr);
  return Status;
}

/**
 * Request execute SPI command provide in TSEG comm buffer.
 *
 * @param[in]       None
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspExecuteSpiCommand (VOID)
{
  EFI_STATUS        Status;

  Status = EFI_INVALID_PARAMETER;
  Status = PspMboxBiosArmor (ArmorExecuteSpiCommand, 0, NULL, mArmorLibPspMboxSmmFlagAddr);
  return Status;
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
  EFI_STATUS        Status;

  Status = EFI_INVALID_PARAMETER;
  Status = PspMboxBiosArmor (ArmorSwitchCsMode, ChipSelect, NULL, mArmorLibPspMboxSmmFlagAddr);
  return Status;
}

/**
 * Initial Psp Storage required information
 *
 *
 * @param[in]  SystemTable Point to EFI system table structure
 *
 * @retval EFI_SUCCESS    Initial success
 * @retval Others         Error happens during initialize
 */
EFI_STATUS
EFIAPI
AmdPspRomArmorLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                          Status;
  PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL *PspMboxSmmBufferAddressProtocol;
  EFI_SMM_BASE2_PROTOCOL              *SmmBase2;
  BOOLEAN                             InSmm;
  EFI_SMM_SYSTEM_TABLE2               *Smst;

  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspRomArmorLibConstructor Entry\n");

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**) &SmmBase2);
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }
  Status = SmmBase2->InSmm (SmmBase2, &InSmm);
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }
  if (!InSmm) {
    return EFI_SUCCESS;
  }
  Status = SmmBase2->GetSmstLocation (SmmBase2, &Smst);
  if (EFI_ERROR (Status)) {
    return RETURN_SUCCESS;
  }

  // Save PspMboxSmmBuffer to globol variable
  Status = gSmst->SmmLocateProtocol (&gPspMboxSmmBufferAddressProtocolGuid, NULL, (VOID **)&PspMboxSmmBufferAddressProtocol);
  if (Status == EFI_SUCCESS) {
    mArmorLibPspMboxSmmBuffer = PspMboxSmmBufferAddressProtocol->PspMboxSmmBuffer;
    mArmorLibPspMboxSmmFlagAddr = PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
    IDS_HDT_CONSOLE_PSP_TRACE ("\tmPspMboxSmmBuffer 0x%x\n", mArmorLibPspMboxSmmBuffer);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tmPspMboxSmmFlagAddr 0x%x\n", mArmorLibPspMboxSmmFlagAddr);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\tLocate gPspMboxSmmBufferAddressProtocolGuid: %r - return to caller\n", Status);
    return Status;
  }

  return EFI_SUCCESS;
}

