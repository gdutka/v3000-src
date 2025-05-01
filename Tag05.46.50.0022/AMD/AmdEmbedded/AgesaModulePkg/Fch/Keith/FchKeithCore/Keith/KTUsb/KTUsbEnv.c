/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTUSB_KTUSBENV_FILECODE

/**
 * FchInitEnvUsb - Config USB controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvUsb (
  IN  VOID     *FchDataPtr
  )
{
  FchKTSmuServiceOnly(0,0,0);
}


