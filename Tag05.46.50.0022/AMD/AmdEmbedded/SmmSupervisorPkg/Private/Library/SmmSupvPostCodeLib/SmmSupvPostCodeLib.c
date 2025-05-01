/** @file
  x64 CPU Exception Handler.

  Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmSupvPostCodeLib.h>
#include <Library/ErrorDataLogLib.h>

/**
  Make a Progress Report to the User.
**/
VOID
SmmSupvTestPoint (
  IN       SMMSUPV_POST_CODE      TestPoint
  )
{
  UINT32   SizedTp;
  UINT32   TpPrefix;
  UINT64   ApicBar;
  BOOLEAN  IsBsp;

  TpPrefix = SMMSUPV_POST_CODE_PREFIX;
  SizedTp = (UINT32) TestPoint;
  SizedTp |= TpPrefix;

  IoWrite32 (SMMSUPV_DEBUG_PORT, SizedTp);
  //write test point to BIOS serial log
  //to avoid too many logs, write test point from BSP only
  ApicBar = AsmReadMsr64 (0x0000001B);
  IsBsp = (BOOLEAN) ((ApicBar & BIT8) != 0);
  if (IsBsp && (TestPoint != SMMSUPV_SMI_ENTRY && TestPoint != SMMSUPV_SMI_EXIT)) {
    DEBUG ((DEBUG_INFO, "%aSmmSupv TestPoint %x <<\n", PREFIX_LOG_ANALYZER, SizedTp));
  }
}
