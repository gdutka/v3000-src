/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Base.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Protocol/PspMboxSmmBufferAddressProtocol.h>
#include <Protocol/AmdPspRomArmorProtocol.h>
#include <Filecode.h>

#define FILECODE PSP_AMDPSPROMARMORSMM_AMDPSPROMARMORSMM_FILECODE

PSP_ROM_ARMOR_PROTOCOL mPspRomArmorProtocol;
BOOLEAN                *mArmorLibPspMboxSmmFlagAddr = NULL;
BOOLEAN                mRomArmor1Enabled = FALSE;

/*----------------------------------------------------------------------------------------
 *                                 F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/**
 * Request secures the SPI Controller
 *
 * @param[in]  SpiCommunicationBuffer    SPI Communication Structure Buffer pointer
 * @param[in]  ChipSelect                SPI controller chip select, 0= Allowed on all chip selects, 1= CS1, 2= CS2, all else invalid
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspEnterSmmOnlyMode (
  IN  SPI_COMMUNICATION_BUFFER  *SpiCommunicationBuffer,
  IN  UINT8                     ChipSelect
)
{
  EFI_STATUS Status;

  Status = EFI_INVALID_PARAMETER;
  Status = PspMboxBiosArmor (ArmorEnterSmmOnlyMode, ChipSelect, SpiCommunicationBuffer, mArmorLibPspMboxSmmFlagAddr);
  if (!EFI_ERROR (Status)) {
     mPspRomArmorProtocol.gRomArmorActive = 1;
  }
  return Status;
}

/**
 * Loads the whitelist into the PSP.
 *
 * @param[in]  SpiWhitelist              SPI White List structure buffer pointer.
 * @param[in]  ChipSelect                SPI controller chip select, 0= Allowed on all chip selects, 1= CS1, 2= CS2, all else invalid
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspEnforceWhitelist (
  IN  SPI_WHITE_LIST  *SpiWhitelist,
  IN  UINT8           ChipSelect
)
{
  EFI_STATUS        Status;

  Status = EFI_INVALID_PARAMETER;
  if (!mRomArmor1Enabled) {
    return EFI_SUCCESS;
  }

  if (SpiWhitelist != NULL && (SpiWhitelist->AllowedCmdCount > 0 || SpiWhitelist->AllowedRegionCount > 0)) {
    Status = PspMboxBiosArmor (ArmorEnterSmmOnlyMode, ChipSelect, &mPspRomArmorProtocol.RomArmorCommBuff, mArmorLibPspMboxSmmFlagAddr);
    if (!EFI_ERROR (Status)) {
      mPspRomArmorProtocol.gRomArmorActive = 1;
      Status = PspMboxBiosArmor (ArmorEnforceWhitelist, ChipSelect, SpiWhitelist, mArmorLibPspMboxSmmFlagAddr);
    }
  }
  return Status;
}

/**
 * Request execute SPI command provide in TSEG comm buffer.
 *
 * @param[in]  None
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspExecuteSpiCommand (VOID)
{
  EFI_STATUS Status;

  Status = EFI_INVALID_PARAMETER;
  Status = PspMboxBiosArmor (ArmorExecuteSpiCommand, 0, NULL, mArmorLibPspMboxSmmFlagAddr);
  return Status;
}

/**
 * Request PSP firmware swtich SPI controller chip select.
 *
 *
 * @param[in]  ChipSelect                SPI controller chip select, 0= Allowed on all chip selects, 1= CS1, 2= CS2, all else invalid
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspSwitchChipSelect (
  IN UINT8 ChipSelect
)
{
  EFI_STATUS        Status;

  Status = EFI_INVALID_PARAMETER;
  Status = PspMboxBiosArmor (ArmorSwitchCsMode, ChipSelect, NULL, mArmorLibPspMboxSmmFlagAddr);
  return Status;
}

EFI_STATUS
EFIAPI
AmdPspRomArmorSmmEntry (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{

  EFI_HANDLE                          Handle;
  EFI_STATUS                          Status;
  PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL *PspMboxSmmBufferAddressProtocol;
  mRomArmor1Enabled = (PcdGet8 (PcdAmdPspRomArmorSelection) == 1);
  if (!mRomArmor1Enabled) {
    return EFI_SUCCESS;
  }

  AGESA_TESTPOINT (TpPspP2CmboxV2Entry, NULL);
  //We are now in SMM
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.Drv.AmdPspRomArmorSmmEntry Enter\n");

  Status = gSmst->SmmLocateProtocol (&gPspMboxSmmBufferAddressProtocolGuid, NULL, &PspMboxSmmBufferAddressProtocol);
  if (Status == EFI_SUCCESS) {
    mArmorLibPspMboxSmmFlagAddr = PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
    IDS_HDT_CONSOLE_PSP_TRACE ("\tmPspMboxSmmFlagAddr 0x%x\n", mArmorLibPspMboxSmmFlagAddr);
  }

  mPspRomArmorProtocol.gRomArmorActive = 0;
  mPspRomArmorProtocol.PspEnterSmmOnlyMode = PspEnterSmmOnlyMode;
  mPspRomArmorProtocol.PspEnforceWhitelist = PspEnforceWhitelist;
  mPspRomArmorProtocol.PspExecuteSpiCommand = PspExecuteSpiCommand;
  mPspRomArmorProtocol.PspSwitchChipSelect = PspSwitchChipSelect;
  ZeroMem (&mPspRomArmorProtocol.RomArmorCommBuff, sizeof(mPspRomArmorProtocol.RomArmorCommBuff));

  Handle = NULL;

  gSmst->SmmInstallProtocolInterface (
           &Handle,
           &gPspRomArmorProtocolGuid,
           EFI_NATIVE_INTERFACE,
           &mPspRomArmorProtocol
           );

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.Drv.AmdPspRomArmorSmmEntry Exit\n");
  AGESA_TESTPOINT (TpPspP2CmboxV2Exit, NULL);

  return EFI_SUCCESS;
}

