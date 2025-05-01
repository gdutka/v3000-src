/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTLPCSPI_KTSPIMID_FILECODE

/**
 * FchInitMidSpi - Config Spi controller after PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitMidSpi (
  IN  VOID     *FchDataPtr
  )
{
  FchInitMidLpc (FchDataPtr);
}

