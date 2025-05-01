/** @file
  Provide OEM to add some tasks, before entering S3.

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

#include <Library/SmmOemSvcKernelLib.h>
#include <Library/IoLib.h>
#include <FchRegs.h>

/**
  This service provides OEM to add some tasks, before entering S3.

  @param  Base on OEM design.
  
  @retval EFI_UNSUPPORTED       Returns unsupported by default.
  @retval EFI_SUCCESS           The service is customized in the project.
  @retval EFI_MEDIA_CHANGED     The value of IN OUT parameter is changed. 
  @retval Others                Depends on customization.
**/
EFI_STATUS
OemSvcS3Callback (
  VOID
  )
{
  /*++
    Todo:
      Add project specific code in here.
  --*/
  UINT32  Value32;

  if (PcdGetBool(PcdH2OCrbOnlySupported)) {
    // 
    //Blink function
    //
    Value32 = MmioRead32 (ACPI_MMIO_BASE + 0x3E4);
    Value32 &= ~(BIT0 + BIT1);
    Value32 |= BIT0;
    MmioWrite32 (ACPI_MMIO_BASE + 0x3E4, Value32);
  }


  return EFI_UNSUPPORTED;
}
