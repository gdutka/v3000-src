/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTPCIE_KTABLATE_FILECODE

/**
 * FchInitLateAb - Prepare Ab Bridge to boot to OS.
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitLateAb (
  IN  VOID     *FchDataPtr
  )
{
  AGESA_TESTPOINT (TpFchInitLateAb, NULL);
  FchAbLateProgram (FchDataPtr);
}


