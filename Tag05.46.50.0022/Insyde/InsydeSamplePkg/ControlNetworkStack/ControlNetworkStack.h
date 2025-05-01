/** @file

;******************************************************************************
;* Copyright (c) 2012 - 2014, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _CONTROL_NETWORK_STACK_H
#define _CONTROL_NETWORK_STACK_H

#include <Protocol/NetworkLocker.h>

#include <Library/DevicePathLib.h>

typedef struct {
  UINT8           MacAddrLength;
  /// UINT8           OriginalMacAddr[MacAddrLength];
  /// Next MAC_ADDR_VARIABLE;
} NETWORK_POLICY_OVERRIDE_VARIABLE;

#endif //_CONTROL_NETWORK_STACK_H
