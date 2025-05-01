/*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTRAIDMID_FILECODE
//
// Declaration of local functions
//

/**
 * FchInitMidSataRaid  - Config SATA Raid controller after PCI
 * emulation
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitMidSataRaid (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;

  SataRaidSetDeviceNumMsi (DieBusNum, LocalCfgPtr);

}


