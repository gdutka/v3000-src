/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTLPCSPI_KTLPCRESET_FILECODE

extern VOID  FchInitResetLpcProgram            (IN VOID  *FchDataPtr);

/**
 * FchInitResetLpc - Config Lpc controller during Power-On
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitResetLpc (
  IN  VOID     *FchDataPtr
  )
{
  AGESA_TESTPOINT (TpFchInitResetLpc, NULL);
  FchInitResetLpcProgram (FchDataPtr);
}


