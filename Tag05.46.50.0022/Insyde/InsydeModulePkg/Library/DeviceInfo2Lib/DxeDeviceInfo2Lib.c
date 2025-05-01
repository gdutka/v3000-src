/** @file
  This library class provides helper functions to access the Device Information.

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include <PiDxe.h>
#include <PiSmm.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmBase2.h>
#include "DeviceInfo2LibInternal.h"

H2O_DEVICE_INFO_PCD_DATA             *mDevInfoList = NULL;

/**
 Get device info PCD data.

 @param[in] PcdTokenNum         Unsigned integer that specifies the PCD token associated with a device's information.

 @return Pointer to the device info PCD data or NULL if not found.
**/
H2O_DEVICE_INFO_PCD_DATA *
GetDevInfoPcdData (
  IN UINTN                             PcdTokenNum
  )
{
  UINTN                                Index;

  if (mDevInfoList == NULL) {
    return NULL;
  }

  for (Index = 0; mDevInfoList[Index].PcdTokenNum != 0; Index++) {
    if (mDevInfoList[Index].PcdTokenNum == PcdTokenNum) {
      return &mDevInfoList[Index];
    }
  }

  return NULL;
}

/**
  Library Constructor.
  this function just identify it is a smm driver or non-smm driver linked against
  with the library.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval RETURN_SUCCESS    The constructor always returns RETURN_SUCCESS.

**/
RETURN_STATUS
EFIAPI
DeviceInfo2LibInitialize (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                           Status;
  EFI_HANDLE                           Handle;

  //
  // To ensure we won't touch BS memory in SMM, the share database is separated and apart from BS/SMM.
  //
  if (!InSmm ()) {
    //
    // In BS
    //
    //
    // Device info list pointer is saved in an internal protocol. All of library instances can access the same database.
    //
    Status = gBS->LocateProtocol (&mInternalDeviceInfo2ListGuid, NULL, (VOID **)&mDevInfoList);
    if (EFI_ERROR (Status)) {
      EnumerateAllDevInfoPcd (&mDevInfoList);

      Handle = NULL;
      gBS->InstallProtocolInterface (&Handle, &mInternalDeviceInfo2ListGuid, EFI_NATIVE_INTERFACE, (VOID *)mDevInfoList);
    }
  } else {
    //
    // In SMM
    //
    //
    // Device info list pointer is saved in an internal protocol. All of library instances can access the same database.
    //
    Status = gSmst->SmmLocateProtocol (&mInternalDeviceInfo2ListGuid, NULL, (VOID **)&mDevInfoList);
    if (EFI_ERROR (Status)) {
      EnumerateAllDevInfoPcd (&mDevInfoList);

      Handle = NULL;
      gSmst->SmmInstallProtocolInterface (&Handle, &mInternalDeviceInfo2ListGuid, EFI_NATIVE_INTERFACE, (VOID *)mDevInfoList);
    }
  }

  return RETURN_SUCCESS;
}

