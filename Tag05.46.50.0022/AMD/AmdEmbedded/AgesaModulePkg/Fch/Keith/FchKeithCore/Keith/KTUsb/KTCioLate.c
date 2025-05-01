/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/
#include <Library/DebugLib.h>
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTUSB_KTCIOLATE_FILECODE

/**
 * FchInitLateUsb4 - Config USB4 controller before OS Boot
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitLateUsb4 (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  AGESA_TESTPOINT (TpFchInitLateUsb4, NULL);
  if (!FchCheckRmbB0()) {
    IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]FchInitLateUsb4 No CIO on non-B0 chip!\n");
    return;
  }
}


