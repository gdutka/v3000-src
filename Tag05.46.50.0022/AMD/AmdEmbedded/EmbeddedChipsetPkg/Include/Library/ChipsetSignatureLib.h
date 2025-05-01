/** @file
  Definition for Capsule Update Criteria Library Class

;******************************************************************************
;* Copyright (c) 2012 - 2016, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _CHIPSET_SIGNATURE_LIB_H_
#define _CHIPSET_SIGNATURE_LIB_H_

#include <Uefi.h>

/**
  Check whether it is in device firmware update process

  @param  None

  @retval TRUE      The system is in firmware failure recovery mode
          FALSE     The system is not in firmware failure recovery mode

**/
BOOLEAN
IsDeviceFirmwareUpdateProcess (
  VOID
  );

/**
  Set device firmware updating in process signature

  @param  None

  @returns None

**/
VOID
SetDeviceFirmwareUpdatingFlag (
  BOOLEAN   DeviceFirmwareUpdating
  );

#endif

