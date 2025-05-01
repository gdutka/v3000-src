/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTAHCILIB_FILECODE

/**
 * sataAhciSetDeviceNumMsi - Program AHCI controller support
 * device number cap & MSI cap
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
SataAhciSetDeviceNumMsi (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{

  FCH_INTERFACE   *LocalCfgPtr;

  LocalCfgPtr = (FCH_INTERFACE *)FchDataPtr;

}

