/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTSATALATE_FILECODE

/**
 * FchInitLateSata - Prepare SATA controller to boot to OS.
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitLateSata (
  IN  VOID     *FchDataPtr
  )
{
  UINT32                 SataController;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  AGESA_TESTPOINT (TpFchInitLateSata, NULL);

  if (FchCheckAm5PkgType()){
    return; // RMB AM5 not support SATA
  }

  for (SataController = 0; SataController < KEITH_SATA_CONTROLLER_NUM; SataController++) {
    if (LocalCfgPtr->Sata[SataController].SataEnable) {
      SataEnableWriteAccessKT (0, SataController);
      //
      // Call Sub-function for each Sata mode
      //
      if (( LocalCfgPtr->Sata[SataController].SataClass == SataAhci7804)
        || (LocalCfgPtr->Sata[SataController].SataClass == SataAhci ))
      {
        FchInitLateSataAhciKT ( 0, SataController, FchDataPtr );
      }

      if ( LocalCfgPtr->Sata[SataController].SataClass == SataRaid) {
        FchInitLateSataRaidKT ( 0, SataController, FchDataPtr );
      }

      FchKTInitLateProgramSataRegs (0, SataController, FchDataPtr);
      SataDisableWriteAccessKT (0, SataController);
    } else {
      continue;
    }
  }
}


