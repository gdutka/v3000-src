/** @file
  GUID used to identify status code logs recorded by storage status code modules.

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

#ifndef _H2O_STATUS_CODE_STORAGE_H_
#define _H2O_STATUS_CODE_STORAGE_H_

#define H2O_STATUS_CODE_STORAGE_HOB_GUID \
  { \
    0x30AE4CA7, 0x0F44, 0x4106, {0xA9, 0xA1, 0x7E, 0x42, 0x08, 0x52, 0xE0, 0x29}   \
  }

#pragma pack(1)
typedef struct {
  UINT32                                LogSize;
  UINT32                                TotalSize;
//CHAR8                                 LogAsciiStr[];
} H2O_STATUS_CODE_STORAGE_LOG;
#pragma pack()

extern EFI_GUID  gH2OStatusCodeStorageHobGuid;

#endif
