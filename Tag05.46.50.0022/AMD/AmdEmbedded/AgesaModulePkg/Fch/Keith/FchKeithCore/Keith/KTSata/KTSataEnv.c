/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTSATAENV_FILECODE

/**
 * FchInitEnvSata - Config SATA controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSata (
  IN  VOID     *FchDataPtr
  )
{
  UINT32                 SataController;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  AGESA_TESTPOINT (TpFchInitEnvSata, NULL);

  if (FchCheckAm5PkgType()){
    FchKTSataInitHideSataPci (0, FchDataPtr);
    return; // RMB AM5 not support SATA
  }

  for (SataController = 0; SataController < KEITH_SATA_CONTROLLER_NUM; SataController++) {
    if (LocalCfgPtr->Sata[SataController].SataEnable) {
      // Staggered spin up enable bits need to be cleared after KPMUX switch finish
      //not in PPR FchKTSataInitStaggeredSpinStep2 (0, SataController, FchDataPtr);
      SataEnableWriteAccessKT (0, SataController);
      FchKTInitEnvProgramSata (0, SataController, FchDataPtr);

      //
      // Call Sub-function for each Sata mode
      //
      if (( LocalCfgPtr->Sata[SataController].SataClass == SataAhci7804)
        || (LocalCfgPtr->Sata[SataController].SataClass == SataAhci ))
      {
        FchInitEnvSataAhciKT ( 0, SataController, FchDataPtr );
      }

      if ( LocalCfgPtr->Sata[SataController].SataClass == SataRaid) {
        FchInitEnvSataRaidKT ( 0, SataController, FchDataPtr );
      }

      SataDisableWriteAccessKT (0, SataController);
      FchKTSataAutoShutdownController (0, SataController, FchDataPtr);
    } else {
      continue;                                                //return if SATA controller is disabled.
    }
  }

  FchKTSataInitHideSataPci (0, FchDataPtr);

  //
  // SATA IRQ Resource
  //
  SataSetIrqIntResource (LocalCfgPtr, StdHeader);
}


