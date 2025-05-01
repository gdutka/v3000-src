/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTLPCSPI_KTSPIENV_FILECODE

VOID
FchSetSpi (
  IN  VOID     *FchDataPtr
  );

/**
 * FchDisEspiMasCtlRegWr - Config eSPI master control registers can be programmed
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchDisEspiMasCtlRegWr (
  IN  VOID     *FchDataPtr
  )
{

  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT32                 SpiAddr = 0xFEC10000;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  if (LocalCfgPtr->Spi.DisEspiMasCtlRegWr == TRUE) {
    ReadPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGA0, AccessWidth32, &SpiAddr, NULL);
    SpiAddr &= 0xFFFFFF00;
    RwMem (SpiAddr + 0x10, AccessWidth32, ~(UINT32)BIT0, BIT0);
  }
}

/**
 * FchSetSpi - Config Spi controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchSetSpi (
  IN  VOID     *FchDataPtr
  )
{

  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
}

/**
 * FchInitEnvSpi - Config Spi controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSpi (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  AGESA_TESTPOINT (TpFchInitEnvSpi, NULL);
  if (LocalCfgPtr->Misc.AmdEnvironmentFlag == 0) {
    FchSetSpi (FchDataPtr);
  }
}

