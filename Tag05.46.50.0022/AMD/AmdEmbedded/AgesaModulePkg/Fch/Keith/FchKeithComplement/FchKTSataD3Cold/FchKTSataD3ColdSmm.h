/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _FCH_KT_SATA_D3COLD_H_
#define _FCH_KT_SATA_D3COLD_H_

#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include "GnbDxio.h"
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>
#include <Protocol/FchSmmSxDispatch2.h>
#include <Protocol/FchSmmSwDispatch2.h>
#include <Library/FchSmmLib.h>
#include "FchPlatform.h"

#define  SATA_POWERDOWN_MESSAGE_ENTRY             0x7800
#define  SATA_POWERDOWN_MESSAGE_EXIT              0x7900
#define  SATA_POWERUP_MESSAGE_ENTRY               0x8900
#define  SATA_POWERUP_MESSAGE_EXIT                0x8A00

EFI_STATUS
EFIAPI
AmdSmiSataD3ColdEntryCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
);

EFI_STATUS
EFIAPI
AmdSmiSataD3ColdExitCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
);

EFI_STATUS
FchSataD3ColdSmmEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
);

#endif


