/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
  Temporary Eap configuration data structure definition.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef __EFI_WIFI_EAP_CONTEXT_H__
#define __EFI_WIFI_EAP_CONTEXT_H__

typedef struct {

  BOOLEAN                   IsEncrypted;
  CHAR16                    EncryptPassword[PASSWORD_STORAGE_SIZE];
  UINTN                     KeySize;
  UINT8                     KeyData[1];

} EFI_EAP_PRIVATE_KEY;

#endif
