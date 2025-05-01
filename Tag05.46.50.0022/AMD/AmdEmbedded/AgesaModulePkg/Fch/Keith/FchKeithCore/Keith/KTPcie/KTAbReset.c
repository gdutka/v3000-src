/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTPCIE_KTABRESET_FILECODE

/**
 * FchInitResetAb - Config Ab Bridge during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetAb (
  IN  VOID     *FchDataPtr
  )
{
  AGESA_TESTPOINT (TpFchInitResetAb, NULL);
  FchProgramAbPowerOnReset (FchDataPtr);
}


