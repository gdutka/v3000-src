/** @file
  Header file for SmmDebugRegisterConfigLib
  
  This library provides a function that uses the gH2OPostCodeSmmDebugRegProtocolGuid 
  to configure the debug registers in SMM

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

#ifndef _SMM_DEBUG_REGISTER_CONFIG_LIB_H_
#define _SMM_DEBUG_REGISTER_CONFIG_LIB_H_

#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmDebugRegisterConfig.h>

/**
  Locate the gH2OPostCodeSmmDebugRegProtocolGuid and uses it to 
  configure the debug registers DR6 and DR7 to perform SMM PostCode features.

  The debug registers DR6 and DR7 will be cleaned when processor enters SMM by SMI.
  We need to configure the debug registers every time when processor enters SMM by SMI.
  Let SmmCorePlatformHookLib call this fucntion and it will configure the debug registers every time when processor enters SMM by SMI. 

  @retval EFI_SUCCESS    Configure the debug registers successfully.
  @retval Other values   Locate the gH2OPostCodeSmmDebugRegProtocolGuid failure.
**/
EFI_STATUS
EFIAPI
SmmDebugRegisterConfig (
  VOID
  );

#endif 
