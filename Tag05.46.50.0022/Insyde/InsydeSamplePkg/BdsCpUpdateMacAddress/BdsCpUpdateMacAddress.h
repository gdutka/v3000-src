/** @file
  REST protocol implementation.

;******************************************************************************
;* Copyright (c) 2012 - 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __BDS_CP_UPDATE_MAC_ADDRESS_H__
#define __BDS_CP_UPDATE_MAC_ADDRESS_H__

#include <Uefi.h>

//
// Libraries
//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/H2OCpLib.h>
#include <Library/DevicePathLib.h>

//
// Consumed Protocols
//
#include <Protocol/DevicePath.h>


//
// Consumed Protocols
//
#include <Guid/H2OCp.h>

typedef struct {
  UINT8           MacAddrLength;
  /// UINT8           OriginalMacAddr[MacAddrLength];
  /// UINT8           ModifyMacAddr[MacAddrLength];
  /// Next MAC_ADDR_VARIABLE;
} MAC_ADDR_VARIABLE;

#endif
