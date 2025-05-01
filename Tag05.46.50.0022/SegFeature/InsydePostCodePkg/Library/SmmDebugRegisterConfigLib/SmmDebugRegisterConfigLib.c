/** @file
  The instance of SmmDebugRegisterConfigLib Library

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/SmmDebugRegisterConfigLib.h>

SMM_DEBUG_REGISTER_CONFIG_PROTOCOL *mSmmDebugRegisterConfig = NULL;

/**
  Locate the gH2OPostCodeSmmDebugRegProtocolGuid and uses it to 
  configure the debug registers DR6 and DR7 to perform SMM PostCode features.

  The debug registers DR6 and DR7 will be cleaned when processor enters SMM by SMI.
  We need to configure the debug registers every time when processor enters SMM by SMI.
  Let SmmCorePlatformHookLibs call this fucntion and it will configure the debug registers every time when processor enters SMM by SMI. 

  @retval EFI_SUCCESS    Configure the debug registers successfully.
  @retval Other values   Locate the gH2OPostCodeSmmDebugRegProtocolGuid failure.
**/
EFI_STATUS
EFIAPI
SmmDebugRegisterConfig (
  VOID
  )
{
  EFI_STATUS Status;

  //
  // Locate Debug Register Configure protocol which is provided by PostCodeSmm driver
  //
  if (mSmmDebugRegisterConfig == NULL) {
    Status = gSmst->SmmLocateProtocol (
                      &gH2OPostCodeSmmDebugRegProtocolGuid,
                      NULL,
                      &mSmmDebugRegisterConfig
                      );
    if(EFI_ERROR(Status)) {
      return Status;
    }
  }

  //
  // Configure the debug registers to trap debug port (port 80)
  // It may not configure the debug registers when PostCodeSmm driver disable the PostCode feature 
  //
  mSmmDebugRegisterConfig->ConfigDebugRegister ();

  return EFI_SUCCESS;
}

