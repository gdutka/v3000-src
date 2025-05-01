/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include <Library/DebugLib.h>
#include "FchPlatform.h"
#include "Filecode.h"
#include <Library/AmdCapsuleLib.h>
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTUSB_KTCIORESET_FILECODE

/**
 * FchInitResetUsb4 - Config USB4 controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetUsb4 (
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  AGESA_TESTPOINT (TpFchInitResetUsb4, NULL);

  if (!FchCheckRmbB0()) {
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchInitResetUsb4 No CIO on non-B0 chip!\n");
    return;
  }

  if ((LocalCfgPtr->Usb4Host[0].Usb4InitEnable == FALSE) || (LocalCfgPtr->Usb4Host[0].Usb4HostEnable == FALSE)) {
    FchSmnRW (0, FCH_KT_RT0_NBIF_STRAP0, ~(UINT32) BIT28, 0, NULL);
  }
  if ((LocalCfgPtr->Usb4Host[1].Usb4InitEnable == FALSE) || (LocalCfgPtr->Usb4Host[1].Usb4HostEnable == FALSE)) {
    FchSmnRW (0, FCH_KT_RT1_NBIF_STRAP0, ~(UINT32) BIT28, 0, NULL);
  }

  if (AmdCapsuleGetStatus ()){
    FchKTUsb4InitBootProgram (0, FchDataPtr);
    AGESA_TESTPOINT (TpFchInitResetUsb4Ready, NULL);
  } else if (FchReadSleepType () == ACPI_SLPTYP_S3) {
    FchKTUsb4InitS3ExitProgram (0, FchDataPtr);
  } else {
    FchKTUsb4InitBootProgram (0, FchDataPtr);
    AGESA_TESTPOINT (TpFchInitResetUsb4Ready, NULL);
  }
}

