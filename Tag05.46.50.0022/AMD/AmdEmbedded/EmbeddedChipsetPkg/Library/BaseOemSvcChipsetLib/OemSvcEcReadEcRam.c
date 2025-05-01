/** @file
  Provide hook function for OEM to read the EC memory. 
  
;******************************************************************************
;* Copyright (c) 2015 - 2020, Insyde Software Corporation. All Rights Reserved.
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
 Read the EC memory.

 @param[out]      *ReadEcRamStatus  Read EC ram status.
 @param[in, out]  *DataBuffer       For input, DataBuffer means offset, 
                                    while it means data for output.
                                    
 @retval            EFI_UNSUPPORTED     Returns unsupported by default.
 @retval            EFI_MEDIA_CHANGED   Alter the Configuration Parameter.
 @retval            EFI_SUCCESS         The function performs the same operation as caller.
                                        The caller will skip the specified behavior and assuming
                                        that it has been handled completely by this function.
*/
EFI_STATUS
OemSvcEcReadEcRam (
  OUT EFI_STATUS *ReadEcRamStatus,
  IN OUT UINT8   *DataBuffer
  )
{
  /*++
    Todo:
      Add project specific code in here.
  --*/
  
  return EFI_UNSUPPORTED;
}

