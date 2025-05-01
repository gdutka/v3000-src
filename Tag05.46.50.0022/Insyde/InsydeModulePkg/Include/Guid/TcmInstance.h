/** @file
  TCM instance guid, used for PcdTpmInstanceGuid.

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
**/

#ifndef __TCM_INSTANCE_GUID_H__
#define __TCM_INSTANCE_GUID_H__

#define TPM_DEVICE_INTERFACE_TCM  \
  { 0x4a351dd0, 0x0674, 0x4482, { 0xa5, 0xdd, 0x16, 0xf8, 0x62, 0x1d, 0x13, 0xa } }

extern EFI_GUID  gEfiTpmDeviceInstanceTcmGuid;

#endif

