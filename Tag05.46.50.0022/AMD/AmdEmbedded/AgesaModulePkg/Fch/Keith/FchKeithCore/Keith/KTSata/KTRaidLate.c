/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTRAIDLATE_FILECODE
//
// Declaration of local functions
//

/**
 * FchInitLateSataRaidKT - Prepare SATA Raid controller to boot
 * to OS.
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitLateSataRaidKT (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  VOID     *FchDataPtr
  )
{
}



