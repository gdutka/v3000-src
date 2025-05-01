/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
  GUIDs used as Kms solution service identifier in WifiMgrDxe driver.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef __KMS_SERVICE_SAMPLE_H__
#define __KMS_SERVICE_SAMPLE_H__

#define EFI_KMS_SERVICE_SAMPLE_GUID \
  {0x362ce9cf, 0x2314, 0x11e6, {0xbb, 0xfd, 0x58, 0x20, 0xb1, 0xd6, 0x52, 0x99}}

#ifndef EFI_KMS_FORMAT_GENERIC_DYNAMIC_GUID
//
// Below definitions have been added into edk2 MdePkg, but they are not in UDK2017.
// Add them here to be compatible with UDK2017
//
#define EFI_KMS_FORMAT_GENERIC_DYNAMIC_GUID \
  { \
    0x2156e996, 0x66de, 0x4b27, {0x9c, 0xc9, 0xb0, 0x9f, 0xac, 0x4d, 0x2, 0xbe } \
  }
typedef struct {
  ///
  /// Length in bytes of the KeyData.
  ///
  UINT32        KeySize;
  ///
  /// The data of the key.
  ///
  UINT8         KeyData[1];
} EFI_KMS_FORMAT_GENERIC_DYNAMIC;
#endif

#endif
