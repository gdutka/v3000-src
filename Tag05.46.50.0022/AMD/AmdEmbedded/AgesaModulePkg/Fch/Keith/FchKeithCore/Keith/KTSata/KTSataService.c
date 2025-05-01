/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "FchPlatform.h"
#include  "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTSATASERVICE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
UINT8  NumOfSataPorts = KEITH_SATA_PORT_NUM;

/**
 * FchKTInitMidProgramSataRegs - Sata Control register setting
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTInitMidProgramSataRegs (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

}

/**
 * FchKTInitLateProgramSataRegs - Sata Control register setting
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTInitLateProgramSataRegs (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  FchKTSataInitMMC (DieBusNum, Controller, FchDataPtr);
}
