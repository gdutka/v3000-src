/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#include <Library/AmdCapsuleLib.h>
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTUSB_KTXHCIRESET_FILECODE

/**
 * FchInitResetXhci - Config Xhci controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetXhci (
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  AGESA_TESTPOINT (TpFchInitResetUsb, NULL);
  IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchInitResetXhci - Entry\n");
  IDS_HDT_CONSOLE (
    FCH_TRACE,
    "[FCH]i2c4_padctrl_i2crxsel = 0x%x\n",
    LocalCfgPtr->i2c4_padctrl_i2crxsel
    );
  if (0xFF != LocalCfgPtr->i2c4_padctrl_i2crxsel) {
    RwMem (ACPI_MMIO_BASE + MISC_BASE + 0xE8, AccessWidth32, ~(UINT32) (BIT4 + BIT5), \
      ((LocalCfgPtr->i2c4_padctrl_i2crxsel & 3) << 4));
  }
  IDS_HDT_CONSOLE (
    FCH_TRACE,
    "[FCH]Xhci0Enable = 0x%x, Xhci1Enable = 0x%x\n",
    LocalCfgPtr->FchReset.Xhci0Enable,
    LocalCfgPtr->FchReset.Xhci1Enable
    );

  if (LocalCfgPtr->FchReset.Xhci0Enable == FALSE) {
    FchSmnRW (0, FCH_KT_USB0_NBIF_STRAP0, ~(UINT32) BIT28, 0, LocalCfgPtr->StdHeader);
  }
  if (LocalCfgPtr->FchReset.Xhci1Enable == FALSE) {
    FchSmnRW (0, FCH_KT_USB1_NBIF_STRAP0, ~(UINT32) BIT28, 0, LocalCfgPtr->StdHeader);
  }
  if (LocalCfgPtr->Xhci2Enable == FALSE) {
    FchKTDisableXhci2 (); //
  }

  if ((LocalCfgPtr->FchReset.Xhci0Enable || LocalCfgPtr->FchReset.Xhci1Enable || LocalCfgPtr->Xhci2Enable) || (FchCheckRmbB0())) {
    if (AmdCapsuleGetStatus ()){
      FchKTXhciInitBootProgram (0, FchDataPtr);
      AGESA_TESTPOINT (TpFchInitResetUsbReady, NULL);
    } else if (FchReadSleepType () == ACPI_SLPTYP_S3) {
      FchKTXhciInitS3ExitProgram (0, FchDataPtr);
    } else {
      FchKTXhciInitBootProgram (0, FchDataPtr);
      AGESA_TESTPOINT (TpFchInitResetUsbReady, NULL);
    }
  }
}

