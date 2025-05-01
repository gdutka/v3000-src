/** @file
  When user enable PxeToLan.
  Control network stack behavior via callback.

;******************************************************************************
;* Copyright (c) 2012 - 2020, Insyde Software Corp. All Rights Reserved.
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
#include <Library/DxeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/KernelConfigLib.h>
#include <Library/H2OCpLib.h>
#include "NetworkLockerDxe.h"

NETWORK_LOCKER_INSTANCE           *NetworkLockerInstance;

/**
  Connect all handles recursively.

  @retval EFI_SUCCESS           All handles and it's child handle have been connected
  @retval Others                Error status returned by of gBS->LocateHandleBuffer().

**/
STATIC
EFI_STATUS
NetworkConnectAll (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       HandleCount;
  EFI_HANDLE  *HandleBuffer;
  UINTN       Index;

  Status = gBS->LocateHandleBuffer (
                  AllHandles,
                  NULL,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  return EFI_SUCCESS;
}

/**
  Enable network stack:
  1. Dispatch NIC drivers.
  2. Connect all.
  3. Call NetworkConfig->Enable() to bind the network protocols to NICs.

  @retval EFI_SUCCESS          Operation completed successfully.
  @retval Others               Operation failed.

**/
STATIC
EFI_STATUS
NetworkConnectNic (
  VOID
  )
{
  EFI_STATUS                           Status;
  UINTN                                HandleCount;
  EFI_HANDLE                           *HandleBuffer;
  UINTN                                Index;
  EFI_DEVICE_PATH_PROTOCOL             *DevicePath;
  H2O_NETWORK_CONFIG_PROTOCOL          *NetworkConfig;
  VOID                                 *Interface;

  Status = gBS->LocateProtocol (&gEfiNetworkStackUndiDependencyGuid, NULL, (VOID **)&Interface);
  if (EFI_ERROR (Status)) {
    gBS->InstallProtocolInterface (&NetworkLockerInstance->Handle, &gEfiNetworkStackUndiDependencyGuid, EFI_NATIVE_INTERFACE, NULL);
    Status = gDS->Dispatch ();
    if (!EFI_ERROR (Status)) {
      NetworkConnectAll ();
    }
  }

  Status = gBS->LocateProtocol (&gH2ONetworkConfigProtocolGuid, NULL, (VOID **)&NetworkConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HandleCount  = 0;
  HandleBuffer = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleNetworkProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID **)&DevicePath
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (FeaturePcdGet (PcdH2OBdsCpNetworkEnableAllEnableNicSupported)) {
      H2O_BDS_CP_NETWORK_ENABLE_ALL_ENABLE_NIC_DATA        CpNetworkEnableAllEnableNicData;
  
      CpNetworkEnableAllEnableNicData.Size          = sizeof (H2O_BDS_CP_NETWORK_ENABLE_ALL_ENABLE_NIC_DATA);
      CpNetworkEnableAllEnableNicData.Status        = H2O_CP_TASK_NORMAL;
      CpNetworkEnableAllEnableNicData.NicDevicePath = DevicePath;
  
      DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBdsCpNetworkEnableAllEnableNicGuid));
      H2OCpTrigger (&gH2OBdsCpNetworkEnableAllEnableNicGuid, &CpNetworkEnableAllEnableNicData);
      DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", CpNetworkEnableAllEnableNicData.Status));
      if (CpNetworkEnableAllEnableNicData.Status == H2O_CP_TASK_SKIP) {
        continue;
      }
    }

    //
    // Enable the network device and bind the network protocols to the device.
    //
    NetworkConfig->Enable (HandleBuffer[Index]);
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }

  return EFI_SUCCESS;
}

/**
  This function executed upon entering EFI Shell.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context

**/
VOID
EFIAPI
NetworkEnableForShell (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  EFI_STATUS                        Status;
  VOID                              *Interface;

  Status = gBS->LocateProtocol (&gEfiShellProtocolGuid, NULL, (VOID **)&Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  gBS->CloseEvent (Event);

  //
  // Enable network stack regardless of setup configuration.
  //
  NetworkLockerInstance->NetworkStackEnable = TRUE;
  NetworkLockerInstance->NetworkStackStatus = UEFI_NETWORK_BOOT_OPTION_HTTP_PXE_BOTH;
  NetworkConnectNic ();
}

/**
  This function executed upon entering Setup Utility APP.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context

**/
VOID
EFIAPI
NetworkEnableForSetupApp (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  EFI_STATUS                        Status;
  VOID                              *Interface;

  Status = gBS->LocateProtocol (&gEfiSetupUtilityApplicationProtocolGuid, NULL, (VOID **)&Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  gBS->CloseEvent (Event);

  //
  // Enable network stack regardless of setup configuration.
  //
  NetworkLockerInstance->NetworkStackEnable = TRUE;
  NetworkLockerInstance->NetworkStackStatus = UEFI_NETWORK_BOOT_OPTION_HTTP_PXE_BOTH;
  NetworkConnectNic ();
}

/**
  Register handlers for the network stack enablement based on PcdH2ONetworkEnableAllConfig.

  @retval EFI_SUCCESS          Operation completed successfully.
  @retval Others               Operation failed.

**/
STATIC
EFI_STATUS
RegisterNetworkEnableCases (
  VOID
  )
{
  VOID               *Registration;

  if (PcdGet32 (PcdH2ONetworkEnableAllConfig) & BIT0) { // Shell
    EfiCreateProtocolNotifyEvent (
      &gEfiShellProtocolGuid, 
      TPL_CALLBACK, 
      NetworkEnableForShell, 
      NULL, 
      (VOID **)&Registration
      );
  }
  if (PcdGet32 (PcdH2ONetworkEnableAllConfig) & BIT1) { // Setup
    EfiCreateProtocolNotifyEvent (
      &gEfiSetupUtilityApplicationProtocolGuid, 
      TPL_CALLBACK, 
      NetworkEnableForSetupApp, 
      NULL, 
      (VOID **)&Registration
      );
  }

  return EFI_SUCCESS;
}

/**
  Check expansion rom header, if network device's option, it will insert UNDI table in it.
  If not found, check class code in PCI configuration space.

  @param[in]  DeviceHandle          Will bypass to next caller
  @param[in]  ShadowAddress         Will bypass to next caller

  @retval     FALSE                 Not network device's OPROM
  @retval     TRUE                  network device's OPROM

**/
BOOLEAN
IdentifyNetworkDevice (
  IN  EFI_HANDLE                       DeviceHandle,
  IN  OUT UINTN                        *ShadowAddress
  )
{
  EFI_STATUS                           Status;
  EFI_PCI_IO_PROTOCOL                  *PciIo;
  UINT8                                ClassCode[4];
  LEGACY_PXE_EXPANSION_ROM_HEADER      *OptionRomHeader;
  UNDI_ROM_ID_STRUCTURE                *UndiRomIdStruct;

  //
  // PXE option rom will insert UNDI table in expansion rom header.
  //
  OptionRomHeader = (LEGACY_PXE_EXPANSION_ROM_HEADER *) (*ShadowAddress);
  if (OptionRomHeader->Signature == 0xAA55) {
    UndiRomIdStruct = (UNDI_ROM_ID_STRUCTURE *) ((UINTN) OptionRomHeader + OptionRomHeader->PxeRomIdOffset);
    if (UndiRomIdStruct->Signature == UNDI_SIGNATURE) {
      return TRUE;
    }
  }

  //
  // Identify Network device from Pci Class code.
  // Class Code in byte 01-03, PCI Configuration Space Header offset 0x08.
  //   ClassCode[2] BaseClass
  //   ClassCode[1] Sub-Class
  //   ClassCode[0] Interface
  //
  if (DeviceHandle == NULL) {
    return FALSE;
  }
  Status = gBS->HandleProtocol (
                  DeviceHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint32,
                        0x08,
                        1,
                        &ClassCode[0]
                        );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if (ClassCode[3] != PCI_CLASS_NETWORK) {
    return FALSE;
  }
  return TRUE;
}

/**
  This function check and block network device's option rom when LegacyPci install option rom.
  Return EFI_UNSUPPORTED to block LegacyPci OPROM.

  @param[in]  This                  Will bypass to next caller
  @param[in]  Mode                  Will bypass to next caller
  @param[in]  Type                  Will bypass to next caller
  @param[in]  DeviceHandle          Will bypass to next caller
  @param[in]  ShadowAddress         Will bypass to next caller
  @param[in]  Compatibility16Table  Will bypass to next caller
  @param[in]  AdditionalData        Will bypass to next caller

  @retval     EFI_SUCCESS           OPROM will loading
  @retval     EFI_UNSUPPORTED       OPROM will not loading in policy

**/
EFI_STATUS
EFIAPI
LegacyBiosPlatformNetworkHook (
  IN EFI_LEGACY_BIOS_PLATFORM_PROTOCOL *This,
  IN EFI_GET_PLATFORM_HOOK_MODE        Mode,
  IN UINT16                            Type,
  IN  EFI_HANDLE                       DeviceHandle,
  IN  OUT UINTN                        *ShadowAddress,
  IN  EFI_COMPATIBILITY16_TABLE        *Compatibility16Table,
  IN  VOID                             **AdditionalData OPTIONAL
  )
{
  EFI_STATUS                           Status;
  EFI_LEGACY_BIOS_PLATFORM_HOOKS       LegacyBiosPlatformHooks;
  BOOLEAN                              IsNetworkDevice;

  //
  // Network device option will only check in EfiPlatformHookPrepareToScanRom phase.
  //
  LegacyBiosPlatformHooks = NetworkLockerInstance->LegacyBiosPlatformHooks;
  if (Mode != EfiPlatformHookPrepareToScanRom) {
    goto EXIT;
  }


  if (NetworkLockerInstance->NetworkStackEnable) {
    if ((NetworkLockerInstance->BootType == LEGACY_BOOT_TYPE || NetworkLockerInstance->BootType == EFI_BOOT_TYPE)) {
      goto EXIT;
    }
    if (NetworkLockerInstance->NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_LEGACY) {
      goto EXIT; 
    }  
  }

  IsNetworkDevice = IdentifyNetworkDevice (DeviceHandle, ShadowAddress);
  if (IsNetworkDevice) {
    return EFI_UNSUPPORTED;
  }
  

EXIT:
  Status = LegacyBiosPlatformHooks (
             This,
             Mode,
             Type,
             DeviceHandle,
             ShadowAddress,
             Compatibility16Table,
             AdditionalData
             );

  return Status;
}

/**
  It will hook PlatformHooks service, to check Network OPROM is loaded or not need.

  @param                        NONE
  @retval EFI_SUCCESS           Function Hook success.
  @retval other                 Function Hook failed.

**/
EFI_STATUS
NetworkLockerHookLegacyBiosPlatform (
  )
{
  EFI_STATUS                           Status;
  EFI_LEGACY_BIOS_PLATFORM_PROTOCOL    *LegacyBiosPlatform;

  Status = gBS->LocateProtocol (
                  &gEfiLegacyBiosPlatformProtocolGuid,
                  NULL,
                  (VOID **) &LegacyBiosPlatform
                  );
  //
  // Block Legacy OPROM process if gEfiLegacyBiosPlatformProtocolGuid exist.
  //
  if (!EFI_ERROR (Status)) {
    NetworkLockerInstance->LegacyBiosPlatform      = LegacyBiosPlatform;
    NetworkLockerInstance->LegacyBiosPlatformHooks = LegacyBiosPlatform->PlatformHooks;
    LegacyBiosPlatform->PlatformHooks              = LegacyBiosPlatformNetworkHook;
  }
  return Status;
}


/**
  This function will be called when PlatformProtocol installed.

  @param                        NONE
  @retval                       NONE

**/
VOID
EFIAPI
NetworkLockerHookLegacyBiosPlatformCallback (
  IN EFI_EVENT                         Event,
  IN VOID                              *Context
  )
{
  gBS->CloseEvent (Event);
  NetworkLockerHookLegacyBiosPlatform ();
}

/**
  Control LegacyBiosPlatform OPTION rom callback.

  @param                        NONE
  @retval EFI_SUCCESS           LegacyBiosPlatform Callback function hooked.
  @retval EFI_NOT_READY         Function does not hook success, but register notify function to wait it install.

**/
EFI_STATUS
NetworkLockerControlLegacyOPRom (
  )
{
  EFI_STATUS                           Status;
  VOID                                 *Registration;
  EFI_EVENT                            LegacyBiosPlatformProtocolNotifyEvent;

  LegacyBiosPlatformProtocolNotifyEvent = NULL;

  //
  // Try to hook LegacyBiosPlatform protocol.
  //
  Status = NetworkLockerHookLegacyBiosPlatform ();
  if (!EFI_ERROR (Status)) {
    return Status;
  }

  //
  // LegacyBiosPlatform hook failed, retry when LegacyBiosPlatoform installed.
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  NetworkLockerHookLegacyBiosPlatformCallback,
                  NULL,
                  &LegacyBiosPlatformProtocolNotifyEvent
                  );
  if (!EFI_ERROR (Status)) {
    gBS->RegisterProtocolNotify (
           &gEfiLegacyBiosPlatformProtocolGuid,
           LegacyBiosPlatformProtocolNotifyEvent,
           &Registration
           );
  }
  return EFI_NOT_READY;
}

/**
  Network stack drivers will callback to this function with it's identify.
  This callback function will return the Status to control the driver binding behavior.

  Insyde network use this callback to do network policy.
  If project need to control specific network device behavior, to install this callback in NetworkLocker protocol to the specific device handle.
  It will override the top policy and ignore PxeDummyDxe's control.

  @param[in]  Identifier            Driver type identify.

  @retval     EFI_SUCCESS           Driver will binding up.
  @retval     EFI_UNSUPPORTED       Driver binding abord.

**/
EFI_STATUS
NetworkStackCallback (
  NETWORK_STACK_CALLBACK_IDENTIFIER    Identifier
  )
{
  BOOLEAN                              NetworkStackEnable;
  UINT8                                NetworkStackStatus;
  UINT8                                BootType;
  EFI_STATUS                           Status;

  NetworkStackEnable = NetworkLockerInstance->NetworkStackEnable;
  NetworkStackStatus = NetworkLockerInstance->NetworkStackStatus;
  BootType           = NetworkLockerInstance->BootType;
  Status             = EFI_UNSUPPORTED;

  if (!NetworkStackEnable) {
    return EFI_UNSUPPORTED;
  }

  if (BootType == SCU_LEGACY_BOOT_TYPE) {
    return EFI_UNSUPPORTED;
  }

  //
  // There will be DUAL and UEFI mode.
  //
  switch (Identifier) {
  case NetworkStackSnpCallback:
    Status = EFI_SUCCESS;
    break;

  case NetworkStackPxe4Callback:
  case NetworkStackMtftp4Callback:
    if (NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_PXE_IPV4 ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_PXE_BOTH ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_PXE_IPV4 ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_PXE_BOTH) {
      Status = EFI_SUCCESS;
    }
    break;

  case NetworkStackPxe6Callback:
  case NetworkStackMtftp6Callback:
    if (NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_PXE_IPV6 ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_PXE_BOTH ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_PXE_IPV6 ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_PXE_BOTH) {
      Status = EFI_SUCCESS;
    }
    break;

  case NetworkStackHttp4Callback:
  case NetworkStackHttpBoot4Callback:
    if (NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_IPV4 ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_BOTH ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_PXE_IPV4 ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_PXE_BOTH) {
      Status = EFI_SUCCESS;
    }
    break;

  case NetworkStackHttp6Callback:
  case NetworkStackHttpBoot6Callback:
    if (NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_IPV6 ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_BOTH ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_PXE_IPV6 ||
        NetworkStackStatus == UEFI_NETWORK_BOOT_OPTION_HTTP_PXE_BOTH) {
      Status = EFI_SUCCESS;
    }
    break;

  case NetworkStackIp4Callback:
  case NetworkStackIp6Callback:
  case NetworkStackUdp4Callback:
  case NetworkStackUdp6Callback:
  case NetworkStackTcp4Callback:
  case NetworkStackTcp6Callback:
  case NetworkStackDhcp4Callback:
  case NetworkStackDhcp6Callback:
    Status = EFI_SUCCESS;
    break;

  default:
    break;
  }

  return Status;
}

/**
  This driver will locate SetupUtilityProtocol
  To check PxeToLan and NetowrkProtocol configuration

  As the status table to install NetworkLockerProtocol.
  To control network driver need loaded in DXE

   Boot Type  | Network Stack | PXE Boot To LAN | PXE capability | DHCP                | UEFI   | Legacy    | UNDI
              |               |                 |                | (BitLocker Support) | PXE    | PXE OPROM | Driver
  ------------+---------------+-----------------+----------------+---------------------+--------+-----------+---------
  UEFI / Dual | Disable       |  Hide           | Gray out       |                     |        |           |
  UEFI / Dual | Enable        |  Hide           | IPv4           |  DHCP4              | PXE4   |           | Loaded
  UEFI / Dual | Enable        |  Hide           | IPv6           |  DHCP6              | PXE6   |           | Loaded
  UEFI / Dual | Enable        |  Hide           | IPv4/IPv6      |  DHCP4+6            | PXE4+6 |           | Loaded
  UEFI / Dual | Enable        |  Hide           | Disable        |  DHCP4+6            |        |           | Loaded
         Dual | Enable        |  Hide           | Legacy         |                     |        | Loaded    |
  Legacy      | Hide          |  Disable        | Hide           |                     |        |           |
  Legacy      | Hide          |  Enable         | Hide           |                     |        | Loaded    |


  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS           Driver loaded and initialize success
  @retval EFI_NOT_READY         The dependency protocol not ready
  @retval other                 The error from Protocol serverice

**/
EFI_STATUS
EFIAPI
NetworkLockerEntryPoint (
  IN EFI_HANDLE                        ImageHandle,
  IN EFI_SYSTEM_TABLE                  *SystemTable
  )
{
  EFI_STATUS                           Status;
  KERNEL_CONFIGURATION                 KernelConfiguration;
  NETWORK_LOCKER_PROTOCOL              *NetworkLockerProtocol;

  NETWORK_STACK_DHCP6_DUID_SELECTOR    *Selector;
  UINT8                                *Pool;
  UINTN                                Index;

  NetworkLockerInstance                 = NULL;

  if (gBS == NULL) {
    return EFI_NOT_READY;
  }

  Status = gBS->LocateProtocol (
                  &gNetworkLockerProtocolGuid,
                  NULL,
                  (VOID **) &NetworkLockerProtocol
                  );
  if (!EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }

  //
  // Initialize NetworkLocker Instance and get KernelConfiguration
  //
  NetworkLockerInstance = (NETWORK_LOCKER_INSTANCE *) AllocateZeroPool (sizeof (NETWORK_LOCKER_INSTANCE));
  if (NetworkLockerInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = GetKernelConfiguration (&KernelConfiguration);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_READY;
  }

  //
  // Process function to control Legacy OPROM via LegacyBiosPlatform.
  //
  NetworkLockerControlLegacyOPRom ();
  
  //
  // Parsing configuration from SetupVariable.KernelConfiguration
  //
  if (KernelConfiguration.PxeBootToLan == SCU_PXE_BOOT_TO_LAN_ENABLED) {
    NetworkLockerInstance->NetworkStackEnable = TRUE;
  }
  NetworkLockerInstance->Handle             = ImageHandle;
  NetworkLockerInstance->BootType           = KernelConfiguration.BootType;
  NetworkLockerInstance->NetworkStackStatus = KernelConfiguration.NetworkProtocol;
  NetworkLockerInstance->NetworkLockerProtocol.Signature = NETWORK_LOCKER_SIGNATURE;
  NetworkLockerInstance->NetworkLockerProtocol.Version   = NETWORK_LOCKER_VERSION;

  //
  // Network Stack controll callback function notify
  //
  NetworkLockerInstance->NetworkLockerProtocol.NetworkStackCallback = NetworkStackCallback;
  Status = gBS->InstallProtocolInterface (
                  &(NetworkLockerInstance->Handle),
                  &gNetworkLockerProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  (VOID *) &(NetworkLockerInstance->NetworkLockerProtocol)
                  );

  //
  // Initialize Dhcp6 DUID selector
  //
  NetworkLockerInstance->NetworkLockerProtocol.NetworkStackDhcp6DuidSelector = NULL;
  Pool = (UINT8 *) PcdGetPtr (PcdDhcp6DuidSelector);
  if ((Pool != NULL) && (Pool[0] != 0) && (Pool[0] < 4)) {
    Selector = AllocateZeroPool (sizeof (NETWORK_STACK_DHCP6_DUID_SELECTOR) + Pool[0] - 1);
    if (Selector != NULL) {
      Selector->Signature   = DHCP6_DUID_SELECTOR_SIGNATURE;
      Selector->SelectorCnt = Pool[0];
      for (Index = 0; Index < Pool[0]; Index++) {
        Selector->Pool[Index] = Pool[Index + 1];
      }
      NetworkLockerInstance->NetworkLockerProtocol.NetworkStackDhcp6DuidSelector = Selector;
    }

  }

  //
  // Setup UNDI driver dependency protocol, it will only installed in UEFI or DUAL boot mode.
  //
  if (NetworkLockerInstance->BootType == SCU_LEGACY_BOOT_TYPE) {
    return Status; 
  }
  if (KernelConfiguration.PxeBootToLan == SCU_PXE_BOOT_TO_LAN_ENABLED) {
    if (KernelConfiguration.NetworkProtocol != UEFI_NETWORK_BOOT_OPTION_LEGACY) {
      gBS->InstallProtocolInterface (&ImageHandle, &gEfiNetworkStackUndiDependencyGuid, EFI_NATIVE_INTERFACE, NULL);
    }
  }

  RegisterNetworkEnableCases ();

  return Status;
}
