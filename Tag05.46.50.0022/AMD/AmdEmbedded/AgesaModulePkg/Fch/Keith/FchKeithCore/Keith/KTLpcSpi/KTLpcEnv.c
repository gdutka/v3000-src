/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTLPCSPI_KTLPCENV_FILECODE

extern VOID  FchInitEnvLpcProgram            (IN VOID  *FchDataPtr);

/**
 * FchInitEnvLpc - Config LPC controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvLpc (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  AGESA_TESTPOINT (TpFchInitEnvLpc, NULL);
  //
  // LPC CFG programming
  //

  //
  // Initialization of pci config space
  //
  FchInitEnvLpcProgram (FchDataPtr);

  //
  // SSID for LPC Controller
  //
  if (LocalCfgPtr->Spi.LpcSsid != NULL ) {
    RwPci ((LPC_BUS_DEV_FUN << 16) + FCH_LPC_REG2C, AccessWidth32, 0x00, LocalCfgPtr->Spi.LpcSsid, StdHeader);
  }
}


