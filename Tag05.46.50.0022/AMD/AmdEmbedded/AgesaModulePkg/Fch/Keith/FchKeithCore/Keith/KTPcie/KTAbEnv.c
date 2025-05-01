/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTPCIE_KTABENV_FILECODE

/**
 * FchInitEnvAb - Config Ab Bridge before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvAb (
  IN  VOID     *FchDataPtr
  )
{
  AGESA_TESTPOINT (TpFchInitEnvAb, NULL);
  FchInitEnvAbLinkInit (FchDataPtr);
}

/**
 * FchInitEnvAbSpecial - Config Ab Bridge special timing
 *
 *  This routine must separate with FchInitEnvAb and give Ab
 *  bridge little time to get ready
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvAbSpecial (
  IN  VOID     *FchDataPtr
  )
{
}


