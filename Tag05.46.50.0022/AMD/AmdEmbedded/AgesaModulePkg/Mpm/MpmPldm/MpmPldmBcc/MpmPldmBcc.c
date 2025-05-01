/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include "MpmPldmBcc.h"

#define FILECODE MPM_MPMPLDM_MPMPLDMBCC_MPMPLDMBCC_FILECODE


EFI_STATUS
EFIAPI
PldmBccEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
)
{
  IDS_HDT_CONSOLE_PSP_TRACE ("%a, Enter\n", __FUNCTION__);

  if (PcdGetBool (PcdMpmWirelessManageability) == FALSE) {
    return EFI_UNSUPPORTED;
  }

  // Check MPM AIM-T Enable
 if (!IsMpmEnabled()) {
    return EFI_UNSUPPORTED;
  }
  SetBiosTablesTags ();

  IDS_HDT_CONSOLE_PSP_TRACE ( "%a, Exit %r\n", __FUNCTION__);
  return EFI_SUCCESS;
}
