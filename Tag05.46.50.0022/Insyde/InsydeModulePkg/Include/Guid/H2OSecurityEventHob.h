/** @file
  Defination GUID of H2O security event hob

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

#ifndef _H2O_SECURITY_EVENT_HOB_GUID_H_
#define _H2O_SECURITY_EVENT_HOB_GUID_H_

#include <Pi/PiMultiPhase.h>
#include <Base.h>
#include <Guid/H2OStatusCodeDataTypeId.h>

#define H2O_SECURITY_EVENT_HOB_GUID \
  { \
    0x5c2ce59e, 0x84de, 0x44e9, { 0x96, 0xf0, 0x24, 0x2c, 0xd1, 0x7a, 0xaa, 0x68 } \
  }

#define H2O_SECURITY_EVENT_VARIABLE_NAME                 L"SecurityEvent"

typedef struct _H2O_SECURITY_EVENT {
  UINT32                  Size;
  H2O_SECURITY_EVENT_DATA Event;
  VOID                    *EventData;
  CONST CHAR8             *EventDesc;
} H2O_SECURITY_EVENT;

typedef struct _H2O_SECURITY_EVENT_HOB {
  EFI_HOB_GUID_TYPE     Hdr;

  H2O_SECURITY_EVENT    Event;
//CHAR8                 EventData[];
//CHAR8                 EventDesc[];
} H2O_SECURITY_EVENT_HOB;

typedef struct _H2O_SECURITY_EVENT_VAR {
  H2O_SECURITY_EVENT_DATA Event;
  //CHAR8                 EventData[];
  //CHAR8                 EventDesc[];
} H2O_SECURITY_EVENT_VAR;

extern EFI_GUID gH2OSecurityEventHobGuid;

#endif
