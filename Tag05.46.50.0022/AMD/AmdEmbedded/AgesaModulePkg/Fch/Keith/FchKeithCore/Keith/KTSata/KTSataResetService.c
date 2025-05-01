/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTSATARESETSERVICE_FILECODE

/**
 * FchInitResetSataProgram - Config Sata controller during
 * Power-On
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetSataProgram (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32                    SataController;
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;

  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *) FchDataPtr;

  //
  // Enable the SATA controller.
  //
  for (SataController = 0; SataController < KEITH_SATA_CONTROLLER_NUM; SataController++) {
    if (LocalCfgPtr->SataEnable[SataController]) {
      FchKTSataInitBootUpSata (DieBusNum, TRUE, FchDataPtr);
      FchKTSataInitEnableSata (DieBusNum, SataController, FchDataPtr);
    }
  }
}



