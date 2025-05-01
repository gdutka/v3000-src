/** @file


;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Protocol/H2ODeviceInfoLookup.h>     // InsydePlatformInfoPkg

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

/**

**/
VOID
InitDeviceInfoLookupProtocol (
  IN OUT H2O_DEVICE_INFO_LOOKUP_PROTOCOL  **DeviceInfoLookupInterface
  )
{
  EFI_STATUS                       Status;
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *DeviceInfoLookup;

  if (DeviceInfoLookupInterface == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The DeviceInfoLookupInterface is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  if (*DeviceInfoLookupInterface != NULL) {
    return;
  }

  Status = gBS->LocateProtocol (
                  &gH2ODeviceInfoLookupProtocolGuid,
                  NULL, // Registration
                  (VOID **) &DeviceInfoLookup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: Failed to locate %a: %r\n",
      gEfiCallerBaseName,
      "H2ODeviceInfoLookupProtocol",
      Status
      ));
    return;
  }

  Status = DeviceInfoLookup->LoadDeviceInfoLookupData (DeviceInfoLookup);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      ((Status == EFI_OUT_OF_RESOURCES) ? EFI_D_WARN : EFI_D_INFO),
      "%a: DeviceInfoLookup->LoadDeviceInfoLookupData(): %r\n",
      gEfiCallerBaseName,
      Status
      ));
  }

  *DeviceInfoLookupInterface = DeviceInfoLookup;
}
