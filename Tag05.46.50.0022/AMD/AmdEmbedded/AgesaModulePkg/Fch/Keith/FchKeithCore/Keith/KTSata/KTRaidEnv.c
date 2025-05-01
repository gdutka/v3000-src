/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTRAIDENV_FILECODE

//
// Declaration of local functions
//

/**
 * FchInitEnvSataRaid - Config SATA Raid controller before PCI
 * emulation
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSataRaidKT  (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT32                 Strap0;
  UINT32                 Strap13;
  UINT32                 PcicfgBase;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  Strap0 = FCH_KT_SMN_SATA0_NBIF_STRAP0;
  Strap13 = FCH_KT_SMN_SATA0_NBIF_STRAP13;
  PcicfgBase = FCH_KT_SATA0_SMN_PCICFG;

  //
  // Class code
  //
  FchSmnRW (DieBusNum, Strap13, 0x00, 0x00010400, StdHeader);
  //
  // Device ID
  //
  FchSmnRW (DieBusNum, Strap0, 0xFFFF0000, KEITH_FCH_SATA_RAID_DID, StdHeader);

  FchKTSataShutdownUnconnectedSataPortClock (DieBusNum, Controller, FchDataPtr);

  //
  // SSID
  //
  if (LocalCfgPtr->Sata[0].SataRaidSsid != NULL ) {
    FchSmnRW (DieBusNum, PcicfgBase + 0x4C, 0x00, LocalCfgPtr->Sata[0].SataRaidSsid, StdHeader);
  }
}

