/** @file
  When user enable PxeToLan.
  Control network stack behavior via callback.

;******************************************************************************
;* Copyright (c) 2012 - 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "ControlNetworkStack.h"

NETWORK_LOCKER_PROTOCOL           mNetworkLockerProtocol;
NETWORK_LOCKER_PROTOCOL           *mSystemNetworkLockerProtocol = NULL;
EFI_MAC_ADDRESS                   mMacAddress;
CHAR16                            *mOverrideNetworkPolicyVarString = L"PolicyOverride";
UINT8                             *mNetworkPolicyOverride;
UINTN                             mNetworkPolicyOverrideLen;

EFI_STATUS
NetworkStackCallback (
  NETWORK_STACK_CALLBACK_IDENTIFIER    Identifier
  )
{
  if (mSystemNetworkLockerProtocol == NULL) {
    /// This situation shouldn't be happen.
    return EFI_UNSUPPORTED;
  }
  switch (Identifier) {
  case NetworkStackPxe4Callback:
  case NetworkStackPxe6Callback:
  case NetworkStackMtftp4Callback:
  case NetworkStackMtftp6Callback:
  case NetworkStackHttpBoot4Callback:
  case NetworkStackHttpBoot6Callback:
    return EFI_UNSUPPORTED;
    break;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ControlNetworkStackDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS                      Status;
  
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiSimpleNetworkProtocolGuid,
                  NULL,
                  ControllerHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    Status = EFI_UNSUPPORTED;
  }
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiDevicePathProtocolGuid,
                  NULL,
                  ControllerHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  return Status;
}

EFI_STATUS
FindNetworkDeviceHandle (
  IN   EFI_DEVICE_PATH_PROTOCOL        *DevicePath,
  IN   EFI_MAC_ADDRESS                 *MacAddr
  )
{
  EFI_DEVICE_PATH_PROTOCOL             *TempDevicePath;
  MAC_ADDR_DEVICE_PATH                 *MacDevicePath;

  BOOLEAN                              IsMacDevicePath;
  BOOLEAN                              IsIPv4BootOption;
  BOOLEAN                              IsIPv6BootOption;
  
  IsMacDevicePath  = FALSE;
  IsIPv4BootOption = FALSE;
  IsIPv6BootOption = FALSE;
  TempDevicePath = DevicePath;
  while (!IsDevicePathEnd (TempDevicePath)) {
    if (TempDevicePath->Type == MESSAGING_DEVICE_PATH) {
      switch (TempDevicePath->SubType) {
      case MSG_MAC_ADDR_DP:
        MacDevicePath = (MAC_ADDR_DEVICE_PATH *) TempDevicePath;
        if (CompareMem ((VOID *) &MacDevicePath->MacAddress, (VOID *) MacAddr, sizeof (EFI_MAC_ADDRESS)) == 0) {
          IsMacDevicePath = TRUE;
        } else {
          return EFI_NOT_FOUND;
        }
        break;
      case MSG_IPv4_DP:
        return EFI_NOT_FOUND;
        break;
      case MSG_IPv6_DP:
        return EFI_NOT_FOUND;
        break;
      };
    }
    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }
  if (!IsMacDevicePath) {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ControlNetworkStackDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS                      Status;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  VOID                            *TestProtocol;
  EFI_MAC_ADDRESS                 TargetMacAddress;
  UINTN                           MacAddrLen;
  UINTN                           ConfigurationIndex;
  UINT8                           *MacAddrPtr;
  
  Status = gBS->HandleProtocol (
                  ControllerHandle,
                  &gNetworkLockerProtocolGuid,
                  (VOID **) &TestProtocol
                  );
  if (!EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }
  
  Status = gBS->HandleProtocol (
                  ControllerHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &DevicePath
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ConfigurationIndex = 0;
  Status = EFI_NOT_FOUND;
  while (ConfigurationIndex < mNetworkPolicyOverrideLen) {
    MacAddrLen = mNetworkPolicyOverride[ConfigurationIndex];
    if ((ConfigurationIndex + MacAddrLen) >= mNetworkPolicyOverrideLen) {
      break;
    }
    MacAddrPtr = mNetworkPolicyOverride + ConfigurationIndex + 1;
    ZeroMem (&TargetMacAddress, sizeof (EFI_MAC_ADDRESS));
    CopyMem (&TargetMacAddress, MacAddrPtr, MacAddrLen);
    Status = FindNetworkDeviceHandle (DevicePath, &TargetMacAddress);
    ConfigurationIndex += 1 + MacAddrLen;
    if (EFI_ERROR (Status)) {
      continue;
    }
    Status = EFI_SUCCESS;
    break;
  }
  if (!EFI_ERROR (Status)) {
    Status = gBS->InstallProtocolInterface (
                    &ControllerHandle,
                    &gNetworkLockerProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    (VOID **) &mNetworkLockerProtocol
                    );
  }
  return Status;
}

EFI_STATUS
EFIAPI
ControlNetworkStackDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

///
/// Driver Binding Protocol instance
///
EFI_DRIVER_BINDING_PROTOCOL gControlNetworkStackDriverBinding = {
  ControlNetworkStackDriverBindingSupported,
  ControlNetworkStackDriverBindingStart,
  ControlNetworkStackDriverBindingStop,
  0xFF,
  NULL,
  NULL
};

EFI_STATUS
EFIAPI
EntryPoint (
  IN EFI_HANDLE                        ImageHandle,
  IN EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_STATUS                           Status;
  NETWORK_LOCKER_PROTOCOL              *SystemNetworkLocker;
  
  mNetworkPolicyOverride    = NULL;
  mNetworkPolicyOverrideLen = 0;
  Status = gBS->LocateProtocol (&gNetworkLockerProtocolGuid, NULL, (VOID **) &SystemNetworkLocker);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetVariable2 (
             mOverrideNetworkPolicyVarString,
             &gNetworkLockerProtocolGuid,
             (VOID **) &mNetworkPolicyOverride,
             &mNetworkPolicyOverrideLen
             );
  if (EFI_ERROR (Status)) {
    //
    // No configuration need to process.
    //
    return EFI_SUCCESS;
  }
  ZeroMem (&mNetworkLockerProtocol, sizeof (NETWORK_LOCKER_PROTOCOL));
  mNetworkLockerProtocol.NetworkStackCallback = NetworkStackCallback;
  mSystemNetworkLockerProtocol = SystemNetworkLocker;

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gEfiDriverBindingProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &gControlNetworkStackDriverBinding
                  );
  return Status;
}
