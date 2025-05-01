/** @file
  SVM feature enable/disable function's header

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

#ifndef _AMD_SVM_H
#define _AMD_SVM_H

#include <Library/BaseLib.h>

#define AMD_MSR_VM_CR         0xC0010114
#define AMD_MSR_SVM_LOCK_KEY  0xC0010118

VOID
EnableDisableSVM (
  IN  BOOLEAN SVMEnable,
  IN  BOOLEAN SVMLock
);

#endif
