/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/DebugLib.h>

#include "FchReset.h"
#define FILECODE FCH_KEITH_FCHKEITHPEI_FCHRESET_FILECODE

// Consumed PPIs


/*********************************************************************************
 * Name: FchPeiReset
 *
 * Description
 *   Reset the system using the chipset cf9 port
 *
 * Input
 *   PeiServices : a pointer to the PEI service table pointer
 *
 * Output
 *   none if the reset is successful
 *
 *********************************************************************************/
EFI_STATUS
FchPeiReset (
  IN    CONST   EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_PEI_CPU_IO_PPI           *CpuIo;

  // Issue the reset
  CpuIo = (*PeiServices)->CpuIo;
  CpuIo->IoWrite8 (
            PeiServices,
            CpuIo,
            FCH_RESET_PORT,
            (COLD_RESET + RESET_CPU)
            );

  // Reset unsuccessful
  CpuDeadLoop ();
  return (EFI_DEVICE_ERROR);
}

