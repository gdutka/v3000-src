/** @file
  Routine for collecting data for Device Info Lookup Protocol.
  This is a no-op version.

;******************************************************************************
;* Copyright (c) 2021 - 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Library/DeviceInfoLookupDataLib.h>

EFI_STATUS
EFIAPI
LoadDeviceInfoLookupDataInternal (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *Protocol,
  IN EFI_HII_HANDLE                       HiiHandle,
  IN PCD_STRING_NAME_TO_ID                *StringTokenNameToIds
  )
{
  return EFI_UNSUPPORTED;
}
