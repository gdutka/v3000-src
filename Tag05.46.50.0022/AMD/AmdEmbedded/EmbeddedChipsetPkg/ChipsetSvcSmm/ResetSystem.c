/** @file
 SMM Chipset Services Library.

 This file contains only one function that is SmmCsSvcResetSystem().
 The function SmmCsSvcResetSystem() use chipset services to reset system.
  
***************************************************************************
* Copyright (c) 2015, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************

*/

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#define ACPI_MMIO_BASE  0xFED80000ul
#define PMIO_BASE       0x300
#define FCH_PMIOA_REGD2 0xD2

/**
 Resets the system.

 @param[in]         ResetType           The type of reset to perform.

 @retval            EFI_UNSUPPORTED     ResetType is invalid.
 @retval            others              Error occurs
*/
EFI_STATUS
EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE   ResetType
  )
{
  UINT8               InitialData;
  UINT8               OutputData;

  DEBUG ((DEBUG_INFO, "CHIP-SMM-%a-Start\n", __FUNCTION__));
  switch(ResetType) {

  case EfiResetWarm:
    InitialData = 0x02;
    OutputData  = 0x06;
    break;

  case EfiResetCold:
    InitialData = 0x02;
    OutputData  = 0x0E;
    break;

  default:
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "CHIP-SMM-%a-Do CF9 Reset\n", __FUNCTION__));
  IoWrite8 (0xCF9, InitialData);
  IoWrite8 (0xCF9, OutputData);
  
  //
  // Given we should have reset getting here would be bad
  //
  if ((MmioRead8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGD2) & BIT6) == 0) {
    DEBUG ((DEBUG_INFO, "CHIP-SMM-%a-Cf9RstDisable = 0, do deadloop.\n", __FUNCTION__));
    CpuDeadLoop ();
  }
  DEBUG ((DEBUG_INFO, "CHIP-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}