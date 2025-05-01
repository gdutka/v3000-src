/** @file
 VFR application

;******************************************************************************
;* Copyright (c) 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#ifndef _VFR_APP_CON_OUT_H_
#define _VFR_APP_CON_OUT_H_

#include "VfrApp.h"
#include <Protocol/DevicePathToText.h>

EFI_STATUS
VfrAppConOutUpdateDESetting (
  IN VFR_APP_PRIVATE_DATA                     *Private
  );

EFI_STATUS
VfrAppConOutUpdateDevLabel (
  IN VFR_APP_PRIVATE_DATA                     *Private
  );

EFI_STATUS
VfrAppConOutDevInit (
  VOID
  );

#endif

