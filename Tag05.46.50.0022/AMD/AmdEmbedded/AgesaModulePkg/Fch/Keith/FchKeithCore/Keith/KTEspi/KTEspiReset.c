/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTESPI_KTESPIRESET_FILECODE

/**
 * FchInitResetEspi - Config ESPI controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetEspi (
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;

  AGESA_TESTPOINT (TpFchInitResetEspi, NULL);

  if (LocalCfgPtr->EspiEnable) {
    FchAoacPowerOnDev (FCH_AOAC_ESPI, 1);
  } else {
    FchAoacPowerOnDev (FCH_AOAC_ESPI, 0);
    return;
  }
}


