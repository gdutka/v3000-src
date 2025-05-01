/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_STALL_H_
#define _FCH_STALL_H_

//#include EFI_PPI_DEFINITION (Stall)
#include <Ppi/Stall.h>

//
// Module specific defines
//
#define FCH_STALL_RESOLUTION_USEC  1
#define NUM_TICK_PER_10NS             (UINTN) (358)

//
// Functions Prototypes
//
EFI_STATUS
FchPeiStall (
  IN       CONST EFI_PEI_SERVICES   **PeiServices,
  IN       CONST EFI_PEI_STALL_PPI  *This,
  IN       UINTN              Microseconds
  );

#endif // _FCH_STALL_H_

