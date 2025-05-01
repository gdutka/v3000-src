/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_RESET_H_
#define _FCH_RESET_H_

//
// Module specific defines
//
#define FCH_RESET_PORT  0x0CF9

#define WARM_RESET 1
#define COLD_RESET 2      // Cold reset
#define RESET_CPU  4      // Triggers a CPU reset


//
// Functions Prototypes
//
EFI_STATUS
FchPeiReset (
  IN       CONST EFI_PEI_SERVICES  **PeiServices
  );

#endif // _FCH_RESET_H_

