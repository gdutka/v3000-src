/** @file
  GUIDs used as HII FormSet and HII Package list GUID in HTTP boot driver.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

**/

#ifndef __HTTP_BOOT_CONFIG_LINK_FORM_GUID_H__
#define __HTTP_BOOT_CONFIG_LINK_FORM_GUID_H__

#define HTTP_BOOT_CONFIG_LINK_FORM_GUID \
  { \
    0x43a98563, 0x8d7d, 0x4969, { 0xb1, 0x9f, 0xfc, 0x14, 0xb1, 0xf8, 0x36, 0xc9 } \
  }

extern EFI_GUID gHttpBootConfigLinkFormGuid;

#endif
