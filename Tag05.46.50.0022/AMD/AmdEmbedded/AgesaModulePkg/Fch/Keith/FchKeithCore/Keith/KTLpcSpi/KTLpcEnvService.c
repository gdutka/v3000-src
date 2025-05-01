/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTLPCSPI_KTLPCENVSERVICE_FILECODE

/**
 * FchInitKeithEnvLpcPciTable - PCI device registers initial
 * during early POST.
 *
 */
REG8_MASK FchInitKeithEnvLpcPciTable[] =
{
  //
  // LPC Device (Bus 0, Dev 20, Func 3)
  //
  {0x00, LPC_BUS_DEV_FUN, 0},
  //Keith default {FCH_LPC_REG40, 0xFB, BIT2},                     /// Enabling LPC DMA Function  0x40[2]=1b 0x78[0]=0b
  {FCH_LPC_REG78, 0xFC, 00},                                       /// 0x78[0]=0b / No Hog disable
  // Keith {FCH_LPC_REGBB, 0xF3, BIT2 + BIT3},                     /// Enabled SPI Prefetch from HOST.
  {0xFF, 0xFF, 0xFF},
};

/**
 * FchInitEnvLpcProgram - Config LPC controller before PCI
 * emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvLpcProgram (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  ProgramPciByteTable (
    (REG8_MASK*) (&FchInitKeithEnvLpcPciTable[0]),
    sizeof (FchInitKeithEnvLpcPciTable) / sizeof (REG8_MASK),
    StdHeader
    );
}


