/** @file
  Dummy implementation for SMM related functions

;******************************************************************************
;* Copyright (c) 2012 - 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/


#include <Uefi.h>

UINT8
EFIAPI
GetPlatformInfo (
  IN OUT UINT8            *FbtsBuffer,
  IN     UINT16           SmiPort
  )
{
  return 0;
}

UINT8
EFIAPI
GetCmdBuffer (
  OUT    UINT32          *CmdBuffer,
  IN     UINT16           SmiPort
  )
{
  return 0;
}