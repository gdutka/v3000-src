/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSD_KTSDENV_FILECODE

extern VOID  FchInitEnvSdProgram            (IN VOID  *FchDataPtr);
/**
 * FchInitEnvSd - Config SD controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSd (
  IN  VOID     *FchDataPtr
  )
{
  AGESA_TESTPOINT (TpFchInitEnvSd, NULL);
  FchInitEnvSdProgram (FchDataPtr);
}


