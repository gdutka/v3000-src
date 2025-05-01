/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_PSP_HSTI_STATE_LIB_H_
#define _AMD_PSP_HSTI_STATE_LIB_H_

#include "Porting.h"

/**
 * @brief initialize HSTI state to save it to Heap for later use
 * this function must be called in non-SMM mode before we call GetHstiState
 *
 */
EFI_STATUS
EFIAPI
InitializeHstiState (
  );

/**
 * @brief Get the HSTI State
 * This function returns HSTI state, can be used in both non-SMM and SMM mode,
 * but InitializeHstiState should be called in non-SMM first so that we can get HSTI state from heap
 */
EFI_STATUS
EFIAPI
GetHstiState (
  UINT32   *HstiState
  );
#endif // _AMD_PSP_HSTI_STATE_LIB_H_


