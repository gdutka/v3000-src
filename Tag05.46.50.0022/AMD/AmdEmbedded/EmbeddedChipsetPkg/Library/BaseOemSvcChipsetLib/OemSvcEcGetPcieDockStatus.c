/** @file
  Provide hook function for OEM to retrieve pcie dock status. 
  
;******************************************************************************
;* Copyright (c) 2014-2020, Insyde Software Corporation. All Rights Reserved.
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
 Get pcie dock status from EC. 
 
 @param[out]        *CommandStatus      Command status.
 @param[out]        *Dock               Dock Status.
                                        The bit0 is PCIe Dock Status, 1 = docked


 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcGetPcieDockStatus (
  OUT EFI_STATUS   *CommandStatus,
  OUT UINT8        *Dock
  )
{
  /*++
    Todo:
      Add project specific code in here.
  --*/

  return EFI_UNSUPPORTED;
}
