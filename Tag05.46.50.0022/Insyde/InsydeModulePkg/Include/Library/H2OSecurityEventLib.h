/** @file
  This library provides wrapper functions for common platform security event actions.
  
;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_SECURITY_EVENT_LIB_H_
#define _H2O_SECURITY_EVENT_LIB_H_

#include <Guid/H2OSecurityEventHob.h>

EFI_STATUS
H2OSecurityEventReport (
  IN  CONST H2O_SECURITY_EVENT  *Event,
  OUT UINT32                    *EventAction
  );

#endif
