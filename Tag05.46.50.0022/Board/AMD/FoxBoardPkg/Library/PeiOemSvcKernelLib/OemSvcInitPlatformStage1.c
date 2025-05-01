/** @file
  Project dependent initial code in PlatformStage1.

;******************************************************************************
;* Copyright (c) 2012, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/PeiOemSvcKernelLib.h>
#include <Library/IoLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include "SetupConfig.h"
#include <Library/PeiServicesLib.h>
#include "FchRegistersCommon.h"
//>>EnableAbove4GBMmio++
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
//<<EnableAbove4GBMmio++
#include <FchRegs.h>


#define TIEC_RAM_GPO2_OFFSET           0xAC

/**
  Project dependent initial code in PlatformStage1.

  @param  Base on OEM design.

  @retval EFI_UNSUPPORTED    Returns unsupported by default.
  @retval EFI_SUCCESS        The service is customized in the project.
  @retval EFI_MEDIA_CHANGED  The value of IN OUT parameter is changed.
  @retval Others             Depends on customization.
**/
EFI_STATUS
OemSvcInitPlatformStage1 (
  VOID
  )
{
  UINT32  Value32;

  if (PcdGetBool(PcdH2OCrbOnlySupported)) {
    // 
    //Blink function
    //
    Value32 = MmioRead32 (ACPI_MMIO_BASE + 0x3E4);
    Value32 &= ~(BIT0 + BIT1);
    MmioWrite32 (ACPI_MMIO_BASE + 0x3E4, Value32);
  }

  return EFI_SUCCESS;
}
