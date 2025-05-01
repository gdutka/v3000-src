/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTAHCIENV_FILECODE

/**
 * FchInitEnvSataAhci - Config SATA Ahci controller before PCI
 * emulation
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSataAhciKT (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  VOID     *FchDataPtr
  )
{
  UINT32                 Strap0;
  UINT32                 Strap13;
  UINT32                 PcicfgBase;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  Strap0 = FCH_KT_SMN_SATA0_NBIF_STRAP0;
  Strap13 = FCH_KT_SMN_SATA0_NBIF_STRAP13;
  PcicfgBase = FCH_KT_SATA0_SMN_PCICFG;

  //
  // Class code
  //
  FchSmnRW (DieBusNum, Strap13, 0x00, 0x00010601, StdHeader);
  //
  // Device ID
  //
  if ( LocalCfgPtr->Sata[0].SataClass == SataAhci7804 ) {
    FchSmnRW (DieBusNum, Strap0, 0xFFFF0000, KEITH_FCH_SATA_AMDAHCI_DID, StdHeader);
  } else {
    FchSmnRW (DieBusNum, Strap0, 0xFFFF0000, KEITH_FCH_SATA_AHCI_DID, StdHeader);
  }

  //
  // SSID
  //
  if (LocalCfgPtr->Sata[0].SataAhciSsid != NULL ) {
    FchSmnRW (DieBusNum, PcicfgBase + 0x4C, 0x00, LocalCfgPtr->Sata[0].SataAhciSsid, StdHeader);
  }
}

