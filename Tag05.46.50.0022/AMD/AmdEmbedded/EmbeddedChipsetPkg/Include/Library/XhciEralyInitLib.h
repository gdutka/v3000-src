/** @file
  Provide xHCI controller initialize function
;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _XHCI_EARLY_INIT_LIB_H_
#define _XHCI_EARLY_INIT_LIB_H_

#include <Uefi.h>

/**
 Enable and initialize CPU internal xHCI controller

 @param[in]  DieBusNum           The BUS number of the CPU DIE.

 @retval EFI_SUCCESS             xHCI controller was initializing finished.
 @retval EFI_DEVICE_ERROR        xHCI controller can't be setting to D0 state.
**/
EFI_STATUS
XhciControllerInit (
  IN  UINT32     DieBusNum
  );

#endif
