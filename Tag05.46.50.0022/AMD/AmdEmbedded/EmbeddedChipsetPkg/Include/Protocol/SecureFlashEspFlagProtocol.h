/** @file

;*******************************************************************************
;* Copyright (c) 2019, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#ifndef _SECURE_FLASH_ESP_FLAG_PROTOCOL_H_
#define _SECURE_FLASH_ESP_FLAG_PROTOCOL_H_


typedef struct _SECURE_FLASH_ESP_FLAG_PROTOCOL SECURE_FLASH_ESP_FLAG_PROTOCOL;

struct _SECURE_FLASH_ESP_FLAG_PROTOCOL {
  BOOLEAN             ViaEspFlag;
} ;

extern EFI_GUID gEfiSecureFlashEspFlagProtocolGuid;
#endif
