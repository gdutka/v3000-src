/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

#include <GnbDxio.h>
#include <AMD.h>
#include <GnbDxio.h>
#include <AmdPcieComplex.h>
#include <Library/NbioSmuV13Lib.h>
#include <Library/DxioLibV2.h>
#include <Library/AmdBaseLib.h>

#include "PcieStraps.h"

#define FILECODE        NBIO_PCIE_RMB_COMMON_PCIESTRAPS_FILECODE
/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to write pcie soft straps
 *
 *
 *
 * @param[in]  GnbHandle         The associated Gnb Handle
 * @param[in]  StrapIndex        Strap index
 * @param[out] Value             Pointer to byte
 * @param[in]  Wrapper           Pcie wrapper number
 */
VOID
WritePcieStrap (
  IN   GNB_HANDLE   *GnbHandle,
  IN   UINT16       StrapIndex,
  IN   UINT32       Value,
  IN   UINT8        Wrapper
  )
{
  UINT32  Index;
  UINT32  SmuArg[6];

  Index = FUSE_ACCESS_LOCATION;
  Index += StrapIndex;

  LibAmdMemFill(SmuArg, 0x00, 24, (AMD_CONFIG_PARAMS *)NULL);
  SmuArg[1] = 0x1;
  SmuArg[2] = 0x4;
  SmuArg[3] = Wrapper & 0x1;
  SmuArg[4] = StrapIndex;
  SmuArg[5] = Value;
  NbioDxioServiceRequestV13 (GnbHandle, DXIO_MSG_RNTM_MBOXROUTINES, SmuArg, 0);

    return;
}

