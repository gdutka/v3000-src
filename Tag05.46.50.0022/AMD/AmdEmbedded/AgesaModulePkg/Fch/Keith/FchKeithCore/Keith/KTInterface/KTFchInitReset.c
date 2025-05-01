/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/

#include    "FchPlatform.h"
#include    "KTFchTaskLauncher.h"
#include    "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTINTERFACE_KTFCHINITRESET_FILECODE

extern FCH_RESET_INTERFACE   FchResetInterfaceDefault;
FCH_TASK_ENTRY *FchInitResetTaskTable[] = {
  FchInitResetHwAcpiP,
  FchInitResetAb,
  FchInitResetSpi,
  FchInitResetHwAcpi,
  FchInitResetSata,
  FchInitResetLpc,
  FchInitResetEspi,
  FchInitResetXhci,
  FchInitResetUsb4,
  NULL
};

/**
 * FchInitReset - Config Fch during power on stage.
 *
 *
 *
 * @param[in] FchParams Fch configuration structure pointer
 *
 */
AGESA_STATUS
FchInitReset (
  FCH_RESET_DATA_BLOCK         *FchParams
  )
{
  // Override internal data with IDS (Optional, internal build only)
  //IDS_OPTION_CALLOUT (IDS_CALLOUT_FCH_INIT_RESET, FchParams, &ResetParams->StdHeader);

  //AgesaFchOemCallout (FchParams);
  return FchTaskLauncher (&FchInitResetTaskTable[0], FchParams, TpFchInitResetDispatching);
}



