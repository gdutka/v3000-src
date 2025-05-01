/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include "DashPldmBcc.h"


EFI_STATUS
EFIAPI
DashPldmBccEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
)
{
  EFI_STATUS            Status;
  COMMON_SETUP_OPTIONS  CommonSetupOptions;

  IDS_HDT_CONSOLE_PSP_TRACE ("%a, Enter\n", __FUNCTION__);

  Status = GetCommonSetupOptions (&CommonSetupOptions);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[DashPldmBccEntry] Get Dash variable fail.\n"));
    return Status;
  }

  if (CommonSetupOptions.DashOption != 1) {
    DEBUG ((DEBUG_INFO, "[DashPldmBccEntry] Dash variable is Disabled.\n"));
    return Status;
  }
  SetBiosTablesTags ();

  IDS_HDT_CONSOLE_PSP_TRACE ( "%a, Exit %r\n", __FUNCTION__);
  return EFI_SUCCESS;
}
