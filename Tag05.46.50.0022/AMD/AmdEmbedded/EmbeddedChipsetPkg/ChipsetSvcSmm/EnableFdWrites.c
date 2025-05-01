/** @file
 SMM Chipset Services Library.

 This file contains only one function that is SmmCsSvcEnableFdWrites().
 The function SmmCsSvcEnableFdWrites() use chipset services to enable/disable 
 flash device write access.
  
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
#include <Library/PciCf8Lib.h>
#include <Library/IoLib.h>
#include <Protocol/SmmFwBlockService.h>

/**
 Platform specific function to enable/disable flash device write access. 

 @param[in]         EnableWrites        TRUE  - Enable
                                        FALSE - Disable
                    
 @retval            EFI_SUCCESS         Function returns successfully
 @return            others              Error occurs
*/
EFI_STATUS
EFIAPI
EnableFdWrites (
  IN  BOOLEAN           EnableWrites
  )
{
  return EFI_SUCCESS;
}