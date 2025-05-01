/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTSATARESET_FILECODE

/**
 * FchInitResetSata - Config Sata controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetSata (
  IN  VOID     *FchDataPtr
  )
{
  AGESA_TESTPOINT (TpFchInitResetSata, NULL);

  if (FchCheckAm5PkgType()){
    return; // RMB AM5 not support SATA
  }

  //FchInitResetSataProgram (0, FchDataPtr );
}


