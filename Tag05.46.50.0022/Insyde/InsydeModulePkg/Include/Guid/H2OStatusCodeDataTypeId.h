/** @file
  Defination GUID of Efi Status Code Data Type H2OSecurityEvent guid

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

#ifndef __H2O_STATUS_CODE_DATA_TYPE_ID_GUID_H__
#define __H2O_STATUS_CODE_DATA_TYPE_ID_GUID_H__

#include <Uefi.h>
#include <Pi/PiMultiPhase.h>

#define EFI_STATUS_CODE_DATA_TYPE_H2O_SECURITY_EVENT_GUID \
  { 0x8b94d5ea, 0x523f, 0x462f, { 0x8b, 0x2b, 0xb1, 0x88, 0xaa, 0xb8, 0x42, 0xe1 }}

#define H2O_SECURITY_EVENT_CLASS_OTHER                   0x00000000
#define H2O_SECURITY_EVENT_CLASS_EXCEPTION               0x00000001
#define H2O_SECURITY_EVENT_CLASS_STACK_FRAME_CORRUPT     0x00000002
#define H2O_SECURITY_EVENT_CLASS_INTEGER_OVERFLOW        0x00000003
#define H2O_SECURITY_EVENT_CLASS_UNINITIALIZED_VARIABLE  0x00000004
#define H2O_SECURITY_EVENT_CLASS_HEAP_CORRUPT            0x00000005
#define H2O_SECURITY_EVENT_CLASS_ASSERT                  0x00000006
#define H2O_SECURITY_EVENT_CLASS_ASSERT_ERROR            0x00000007
#define H2O_SECURITY_EVENT_CLASS_CR                      0x00000008
#define H2O_SECURITY_EVENT_CLASS_CHASSIS_INTRUSION       0x00000009

#define H2O_SECURITY_EVENT_ACTION_SHUTDOWN     0x00000001
#define H2O_SECURITY_EVENT_ACTION_RESET        0x00000002
#define H2O_SECURITY_EVENT_ACTION_RECOVER      0x00000004
#define H2O_SECURITY_EVENT_ACTION_MITIGATE     0x00000008
#define H2O_SECURITY_EVENT_ACTION_LOG          0x00000100
#define H2O_SECURITY_EVENT_ACTION_POSTCODE     0x00000200
#define H2O_SECURITY_EVENT_ACTION_DISPLAY      0x00000400
#define H2O_SECURITY_EVENT_ACTION_STATUS_CODE  0x00000800

typedef struct _H2O_SECURITY_EVENT_DATA {
  UINT32                Flags;

  UINT32                Class;
  UINT32                Action;

  UINT32                PostCode;
  UINT32                StatusCodeInstance;
  EFI_STATUS_CODE_TYPE  StatusCodeType;
  EFI_STATUS_CODE_VALUE StatusCodeValue;
  EFI_GUID              StatusCodeCallerId;

  UINT64                Data;
  EFI_PHYSICAL_ADDRESS  CodeIp;
  UINT16                CodeSel;
  UINT16                EventDataSize;
  UINT32                ErrorCode;
} H2O_SECURITY_EVENT_DATA;

typedef struct _H2O_STATUS_CODE_SECURITY_EVENT_DATA {
  EFI_STATUS_CODE_DATA    Hdr;
  H2O_SECURITY_EVENT_DATA Event;
//UINT8                   EventData[];
//CHAR8                   EventDesc[];
} H2O_STATUS_CODE_SECURITY_EVENT_DATA;

extern EFI_GUID gEfiStatusCodeDataTypeH2OSecurityEventGuid;

#endif
