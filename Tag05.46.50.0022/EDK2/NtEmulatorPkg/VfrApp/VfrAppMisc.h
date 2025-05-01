/** @file
 VFR application

;******************************************************************************
;* Copyright (c) 2015, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#ifndef _VFR_APP_MISC_H_
#define _VFR_APP_MISC_H_

#include "VfrApp.h"

EFI_STATUS
LoadNameValueNames (
  IN VFR_APP_PRIVATE_DATA               *PrivateData
  );

EFI_STATUS
GetValueOfNumber (
  IN EFI_STRING                    StringPtr,
  OUT UINT8                        **Number,
  OUT UINTN                        *Len
  );

EFI_STRING
CreateAltCfgString (
  IN     EFI_STRING     Result,
  IN     EFI_STRING     ConfigHdr,
  IN     UINTN          Offset,
  IN     UINTN          Width
  );

VOID
AppendAltCfgString (
  IN OUT EFI_STRING                       *RequestResult,
  IN     EFI_STRING                       ConfigRequestHdr
  );
#endif

