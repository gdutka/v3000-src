/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_RESET2_H_
#define _FCH_RESET2_H_

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <FchRegistersCommon.h>


// Reset control register values
//
#define HARDRESET       0x06
#define SOFTRESET       0x04
#define HARDSTARTSTATE  0x02
#define SOFTSTARTSTATE  0x0

#define SUS_S3          0x0C00U     // S3
#define SUS_S5          0x1400U     // S5
#define SLP_TYPE        0x1C00U     // MASK
#define SLP_EN          0x2000U     // BIT13

#define SPECIFIC_WARM_RESET_STR L"Specific Warm Reset"

//
// Functions Prototypes
//

VOID
SpecificWarmResetSystem (
  IN EFI_RESET_TYPE ResetType
);

VOID
EFIAPI
FchPeiReset2 (
  IN EFI_RESET_TYPE     ResetType,
  IN EFI_STATUS         ResetStatus,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  );
#endif // _FCH_RESET2_H_

