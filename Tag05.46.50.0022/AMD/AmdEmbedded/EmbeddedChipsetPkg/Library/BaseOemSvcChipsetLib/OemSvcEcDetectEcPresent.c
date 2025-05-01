/** @file
  Provide hook function for OEM to detect EC Present. 
  
;******************************************************************************
;* Copyright (c) 2014 - 2020, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/BaseOemSvcChipsetLib.h>

/**
 Detect EC Present.
 
 @param[out]        *CommandStatus      Command status.
 @param[out]        Present             TRUE: EC is presnet; FALSE: EC is absent.

 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcDetectEcPresent (
  OUT EFI_STATUS   *CommandStatus,
  OUT BOOLEAN      *Present
  )
{
  /*++
    Todo:
      Add project specific code in here.
  --*/

  return EFI_UNSUPPORTED;
}
