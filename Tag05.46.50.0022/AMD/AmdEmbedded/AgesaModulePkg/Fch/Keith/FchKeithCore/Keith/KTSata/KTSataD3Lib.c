/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include "FchPlatform.h"

#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTSATAD3LIB_FILECODE

BOOLEAN FchSataD3ColdClockOn (
  IN UINT32 Controller
)
{
  UINT32    RsmuCmdReg1;

  FchSmnRead (0, FCH_KT_SATA_AOAC_CONTROL + Controller * FCH_KT_SMN_SATA_CONTROL_STEP, &RsmuCmdReg1, NULL);
  if (RsmuCmdReg1 & BIT8) {
    return FALSE;
  } else {
    return TRUE;
  }
}
