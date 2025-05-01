/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _CBS_FCH_ENTRY_DXE_H_
#define _CBS_FCH_ENTRY_DXE_H_

#include "CbsDxe.h"
#include  <Library/PcdLib.h>
#include "FchRegistersCommon.h"

VOID
CBS_FUNC_DECLARE(FchInitEnvParams,_RMB) (
  IN EFI_BOOT_SERVICES        *BootServices,
  IN OUT  VOID                *DataParams,
  IN AMD_SYSTEM_CONFIGURATION *SystemConfiguration
  );
#endif

