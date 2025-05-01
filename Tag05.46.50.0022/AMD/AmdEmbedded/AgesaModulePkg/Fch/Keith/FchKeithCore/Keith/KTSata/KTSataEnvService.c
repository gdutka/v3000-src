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
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTSATAENVSERVICE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
//
// Local Routine
//


/**
 * FchKTInitEnvProgramSata - Sata Init before PCI scan
 *
 *
 * @param[in] DieBusNum - Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr - Fch configuration structure pointer
 *
 */
VOID
FchKTInitEnvProgramSata (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  VOID     *FchDataPtr
  )
{
  UINT32                 PortNum;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // Enable ports here
  //
  for (PortNum = 0; PortNum < KEITH_SATA_PORT_NUM; PortNum++) {
    if (((LocalCfgPtr->Sata[Controller].SataPortPower >> PortNum) & BIT0) == 0) {
      FchKTSataInitPortActive (DieBusNum, Controller, PortNum, FchDataPtr);
    }
  }

  // Do Sata init
  FchKTSataInitRsmuCtrl (DieBusNum, Controller, FchDataPtr);

  FchKTSataInitCtrlReg (DieBusNum, Controller, FchDataPtr);

  FchKTSataInitEsata (DieBusNum, Controller, FchDataPtr);

  FchKTSataInitEnableErr (DieBusNum, Controller, FchDataPtr);

  FchKTSataSetPortGenMode (DieBusNum, Controller, FchDataPtr);

  if (Controller == 0) {
    FchKTSataInitDevSlp (DieBusNum, FchDataPtr);
    FchKTSataInitMpssMap (DieBusNum, FchDataPtr);
  }
}

