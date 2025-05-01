/** @file
  H2O USB Text Lookup Protocol implementation.

;******************************************************************************
;* Copyright 2021 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/HiiLib.h>                   // MdeModulePkg

#include "UsbTextLookupDxe.h"

extern EFI_GUID gEfiCallerIdGuid; // From AutoGen.c

H2O_USB_TEXT_LOOKUP_PRIVATE_DATA  mUsbTextLookupPrivate = {
  H2O_USB_TEXT_LOOKUP_PRIVATE_SIGNATURE,
  {      // Protocol
    {    //   Flags
      1, //     ClassTextLookupSupported
      0  //     Reserved
    },
    {0}, //   Reserved
    GetUsbClassCodeString,
    GetNextUsbSubClassRange
  },
  mUsbClassDescTable,
  &mUsbSubClassRangeTable,
  NULL   // HiiHandle
};

/**
  The entry point for the driver.

  @implements EFI_IMAGE_ENTRY_POINT

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
UsbTextLookupEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  mUsbTextLookupPrivate.HiiHandle = HiiAddPackages (
                                      &gEfiCallerIdGuid,
                                      ImageHandle,
                                      UsbTextLookupDxeStrings,
                                      NULL
                                      );
  if (mUsbTextLookupPrivate.HiiHandle == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: Can't register HII package\n",
      gEfiCallerBaseName
      ));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gH2OUsbTextLookupProtocolGuid,
                  &mUsbTextLookupPrivate.Protocol,
                  (EFI_GUID *) NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: Can't install gH2OUsbTextLookupProtocolGuid: %r\n",
      gEfiCallerBaseName,
      Status
      ));
  }

  return Status;
}
