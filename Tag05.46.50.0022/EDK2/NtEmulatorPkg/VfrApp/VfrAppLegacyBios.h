/** @file
 Header file for VFR app Legacy BIOS implementation

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

#ifndef _VFR_APP_LEGACY_BIOS_H_
#define _VFR_APP_LEGACY_BIOS_H_

#include <Uefi.h>
#include <Protocol/DevicePath.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/LegacyBios.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/VariableLib.h>
#include "KernelSetupConfig.h"
#include "VfrApp.h"

#define VFR_APP_LEGACY_BIOS_VENDOR_GUID              {0x91f659c7, 0x71ad, 0x423c, { 0xb0, 0x17, 0xc7, 0x22, 0x7e, 0x82, 0x96, 0x79 }}

EFI_STATUS
VfrAppLegacyBiosInit (
  VOID
  );

#endif
