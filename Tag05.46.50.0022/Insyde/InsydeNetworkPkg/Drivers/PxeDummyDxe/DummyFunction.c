//;******************************************************************************
//;* Copyright (c) 1983-2020, Insyde Software Corp. All Rights Reserved.
//;*
//;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
//;* transmit, broadcast, present, recite, release, license or otherwise exploit
//;* any part of this publication in any form, by any means, without the prior
//;* written permission of Insyde Software Corporation.
//;*
//;******************************************************************************
//;
//; Module Name:
//;
//;   DhcpDummy.h
//;
//; Abstract:
//;
//;    Refer DhcpDummy.c
//;

#include "PxeDummy.h"
#include <Guid/BdsHii.h>

BOOLEAN                                gDeviceConnecting;
EFI_HII_DATABASE_NEW_PACK              gOldNewPackFunction;
EFI_HII_DATABASE_PROTOCOL              *gHiiDataBaseProtocol;

EFI_STATUS
PxeNetworkStackCallback (
  NETWORK_STACK_CALLBACK_IDENTIFIER    Identifier
  )
{
  if (Identifier == NetworkStackSnpCallback) {
    return EFI_SUCCESS;
  }
  return EFI_UNSUPPORTED;
}

EFI_STATUS
FindRealPxeEntry (
  IN   FAKE_LOAD_FILE_INSTANCE         *LoadFileInstance,
  OUT  EFI_LOAD_FILE_PROTOCOL          **RealLoadFile OPTIONAL
  )
{
  EFI_STATUS                           Status;
  EFI_DEVICE_PATH                      *DevicePath;
  EFI_LOAD_FILE_PROTOCOL               *LoadFile;
  EFI_HANDLE                           LoadFileHandle;

  EFI_DEVICE_PATH                      *CallerDevicePath;

  EFI_PXE_BASE_CODE_PROTOCOL           *PxeBc;
  EFI_SIMPLE_NETWORK_PROTOCOL          *Snp;

  VOID                                 *MnpSb;

  if (LoadFileInstance->RealEntry != NULL) {
    if (RealLoadFile != NULL) {
      *RealLoadFile = LoadFileInstance->RealEntry;
    }
    return EFI_SUCCESS;
  }

  CallerDevicePath = LoadFileInstance->FakePxeDevicePath;
  Status = gBS->LocateDevicePath (
                  &gEfiLoadFileProtocolGuid,
                  &CallerDevicePath,
                  &LoadFileHandle
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol (
                  LoadFileHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &DevicePath
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol (
                  LoadFileHandle,
                  &gEfiLoadFileProtocolGuid,
                  (VOID **) &LoadFile
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Because uninstall device path from real handle will cause it UninsatllMultipleProtocol failed.
  // So keep the real PXE handle, PxeDummy can try to uninstall protocol from it.
  // To keep original handle, and install CallerIdGuid on it.
  //
  LoadFileInstance->RealPxeHandle     = LoadFileHandle;
  LoadFileInstance->RealPxeDevicePath = DevicePath;
  LoadFileInstance->RealEntry         = LoadFile;
  Status = gBS->InstallProtocolInterface (
                  &LoadFileHandle,
                  &gEfiCallerIdGuid,
                  EFI_NATIVE_INTERFACE,
                  (VOID *) LoadFileInstance->RealEntry
                  );
  ASSERT_EFI_ERROR (Status);
      
  //
  // Bypass PxeBc and SNP protocol to FakeLoadFileHandle
  //
  Status = gBS->HandleProtocol (
                  LoadFileHandle,
                  &gEfiPxeBaseCodeProtocolGuid,
                  (VOID **) &PxeBc
                  );
  if (!EFI_ERROR (Status)) {
    gBS->InstallProtocolInterface (
           &LoadFileInstance->FakePxeHandle,
           &gEfiPxeBaseCodeProtocolGuid,
           EFI_NATIVE_INTERFACE,
           (VOID *) PxeBc
           );
  }

  Status = gBS->HandleProtocol (
                  LoadFileHandle,
                  &gEfiSimpleNetworkProtocolGuid,
                  (VOID **) &Snp
                  );
  if (!EFI_ERROR (Status)) {
    Status = gBS->InstallProtocolInterface (
                    &LoadFileInstance->FakePxeHandle,
                    &gEfiSimpleNetworkProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    (VOID *) Snp
                    );
    if (!EFI_ERROR (Status)) {
      gBS->OpenProtocol (
             LoadFileInstance->FakePxeHandle,
             &gEfiSimpleNetworkProtocolGuid,
             (VOID **) &Snp,
             LoadFileInstance->Instance->ImageHandle,
             LoadFileInstance->FakePxeHandle,
             EFI_OPEN_PROTOCOL_BY_DRIVER
             );
    }
  }

  //
  // Uninstall real LoadFile protocol and DevicePath and store real LoadFileProtocol address
  //
  gBS->UninstallMultipleProtocolInterfaces (
         LoadFileHandle,
         &gEfiDevicePathProtocolGuid,
         DevicePath,
         &gEfiLoadFileProtocolGuid,
         LoadFile,
         NULL,
         NULL
         );

  //
  // Find the parent handle of PXE LoadFile, if VLAN enabled, the ManagedNetworkProtocol will be remove.
  // The fake PXE loader handle must be remove too. The MnpSb will be closed when DummyPxeDestroyFakeHandle function was be executed (Binding.stop).
  //
  if (LoadFileInstance->Instance->RealMnpSb == NULL) {
    Status = gBS->HandleProtocol (
                    LoadFileInstance->Instance->UndiHandle,
                    &gEfiManagedNetworkServiceBindingProtocolGuid,
                    &MnpSb
                    );
    if (!EFI_ERROR (Status)) {
      Status = gBS->OpenProtocol (
                    LoadFileInstance->Instance->UndiHandle,
                    &gEfiManagedNetworkServiceBindingProtocolGuid,
                    (VOID **) &MnpSb,
                    LoadFileInstance->Instance->ImageHandle,
                    LoadFileInstance->FakePxeHandle,
                    EFI_OPEN_PROTOCOL_BY_DRIVER
                    );
      //
      // If this ASSERT trigged, system maybe hang when boot to the PXE when VLAN configuration is enabled and not reboot.
      //
      ASSERT_EFI_ERROR (Status);
      LoadFileInstance->Instance->RealMnpSb = MnpSb;
    }
  }
  
  if (RealLoadFile != NULL) {
    *RealLoadFile = LoadFileInstance->RealEntry;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
UninstallFakeDevice (
  IN   COMMON_INSTANCE                 *Instance
  )
{
  DUMMY_PXE_INSTANCE                   *DummyPxeInstance;
  DUMMY_IP_INSTANCE                    *DummyIpInstance;

  if (Instance == NULL) {
    return EFI_NOT_READY;
  }

  DummyPxeInstance = &(Instance->Pxe);
  DummyIpInstance  = &(Instance->Ip);

  gBS->CloseProtocol (
         Instance->UndiHandle,
         &gEfiSimpleNetworkProtocolGuid,
         Instance->ImageHandle,
         Instance->UndiHandle
         );

  //
  // Kill the Fake device path first, make sure the real protocol can be binding success.
  //
  if (DummyPxeInstance->Ip4.Enable) {
    gBS->UninstallProtocolInterface (
           DummyPxeInstance->Ip4.FakePxeHandle,
           &gEfiDevicePathProtocolGuid,
           DummyPxeInstance->Ip4.FakePxeDevicePath
           );
  }
  if (DummyPxeInstance->Ip6.Enable) {
    gBS->UninstallProtocolInterface (
           DummyPxeInstance->Ip6.FakePxeHandle,
           &gEfiDevicePathProtocolGuid,
           DummyPxeInstance->Ip6.FakePxeDevicePath
           );
  }
  if (DummyPxeInstance->Http4.Enable) {
    gBS->UninstallProtocolInterface (
           DummyPxeInstance->Http4.FakePxeHandle,
           &gEfiDevicePathProtocolGuid,
           DummyPxeInstance->Http4.FakePxeDevicePath
           );
  }
  if (DummyPxeInstance->Http6.Enable) {
    gBS->UninstallProtocolInterface (
           DummyPxeInstance->Http6.FakePxeHandle,
           &gEfiDevicePathProtocolGuid,
           DummyPxeInstance->Http6.FakePxeDevicePath
           );
  }

  //
  // Kill the fake IP services.
  //
  gBS->CloseProtocol (
         Instance->UndiHandle,
         &gEfiManagedNetworkServiceBindingProtocolGuid,
         Instance->ImageHandle,
         DummyIpInstance->MnpSB.Ip4ChildHandle
         );

  gBS->UninstallMultipleProtocolInterfaces (
         Instance->UndiHandle,
         &gEfiManagedNetworkServiceBindingProtocolGuid,
         &(DummyIpInstance->MnpSB.MnpSB),
         NULL
         );

  gBS->UninstallMultipleProtocolInterfaces (
         Instance->UndiHandle,
         &gEfiVlanConfigProtocolGuid,
         &(DummyIpInstance->VlanConfig.VlanConfig),
         NULL
         );

  gBS->UninstallMultipleProtocolInterfaces (
         Instance->UndiHandle,
         &gEfiIp4ServiceBindingProtocolGuid,
         &(DummyIpInstance->Ip4SB.Ip4SB),
         &gEfiDhcp4ServiceBindingProtocolGuid,
         &(DummyIpInstance->Dhcp4SB.Dhcp4SB),
         &gEfiIp4ConfigProtocolGuid,
         &(DummyIpInstance->Ip4Config.Ip4Config),
         &gEfiIp4Config2ProtocolGuid,
         &(DummyIpInstance->Ip4Config2.Ip4Config2),
         &gEfiIp6ServiceBindingProtocolGuid,
         &(DummyIpInstance->Ip6SB.Ip6SB),
         &gEfiDhcp6ServiceBindingProtocolGuid,
         &(DummyIpInstance->Dhcp6SB.Dhcp6SB),
         &gEfiIp6ConfigProtocolGuid,
         &(DummyIpInstance->Ip6Config.Ip6Config),
         NULL
         );

  return EFI_SUCCESS;
}

EFI_STATUS
ReinstallFakeDevicePath (
  COMMON_INSTANCE                      *Instance
  )
{
  DUMMY_PXE_INSTANCE                   *DummyPxeInstance;

  if (Instance == NULL) {
    return EFI_SUCCESS;
  }

  DummyPxeInstance = &(Instance->Pxe);

  if (DummyPxeInstance->Ip4.Enable) {
    FindRealPxeEntry (&DummyPxeInstance->Ip4, NULL);
    gBS->InstallMultipleProtocolInterfaces (
           &(DummyPxeInstance->Ip4.FakePxeHandle),
           &gEfiDevicePathProtocolGuid,
           DummyPxeInstance->Ip4.FakePxeDevicePath,
           &gEfiLoadFileProtocolGuid,
           (VOID **) &DummyPxeInstance->Ip4.LoadFile,
           NULL,
           NULL
           );
  }
  if (DummyPxeInstance->Ip6.Enable) {
    FindRealPxeEntry (&DummyPxeInstance->Ip6, NULL);
    gBS->InstallMultipleProtocolInterfaces (
           &(DummyPxeInstance->Ip6.FakePxeHandle),
           &gEfiDevicePathProtocolGuid,
           DummyPxeInstance->Ip6.FakePxeDevicePath,
           &gEfiLoadFileProtocolGuid,
           (VOID **) &DummyPxeInstance->Ip6.LoadFile,
           NULL,
           NULL
           );
  }
  if (DummyPxeInstance->Http4.Enable) {
    FindRealPxeEntry (&DummyPxeInstance->Http4, NULL);
    gBS->InstallMultipleProtocolInterfaces (
           &(DummyPxeInstance->Http4.FakePxeHandle),
           &gEfiDevicePathProtocolGuid,
           DummyPxeInstance->Http4.FakePxeDevicePath,
           &gEfiLoadFileProtocolGuid,
           (VOID **) &DummyPxeInstance->Http4.LoadFile,
           NULL,
           NULL
           );
  }
  if (DummyPxeInstance->Http6.Enable) {
    FindRealPxeEntry (&DummyPxeInstance->Http6, NULL);
    gBS->InstallMultipleProtocolInterfaces (
           &(DummyPxeInstance->Http6.FakePxeHandle),
           &gEfiDevicePathProtocolGuid,
           DummyPxeInstance->Http6.FakePxeDevicePath,
           &gEfiLoadFileProtocolGuid,
           (VOID **) &DummyPxeInstance->Http6.LoadFile,
           NULL,
           NULL
           );
  }
  return EFI_SUCCESS;
}

EFI_STATUS
ReinstallFakeDevice (
  COMMON_INSTANCE                      *Instance
  )
{
  DUMMY_PXE_INSTANCE                   *DummyPxeInstance;
  DUMMY_IP_INSTANCE                    *DummyIpInstance;

  DummyPxeInstance = &(Instance->Pxe);
  DummyIpInstance  = &(Instance->Ip);

  //
  // If connect failed, restore Device path to make sure fake entry valid.
  //
  if (DummyPxeInstance->Ip4.Enable) {
    gBS->InstallProtocolInterface (
           &(DummyPxeInstance->Ip4.FakePxeHandle),
           &gEfiDevicePathProtocolGuid,
           EFI_NATIVE_INTERFACE,
           DummyPxeInstance->Ip4.FakePxeDevicePath
           );
  }
  if (DummyPxeInstance->Ip6.Enable) {
    gBS->InstallProtocolInterface (
           &(DummyPxeInstance->Ip6.FakePxeHandle),
           &gEfiDevicePathProtocolGuid,
           EFI_NATIVE_INTERFACE,
           DummyPxeInstance->Ip6.FakePxeDevicePath
           );
  }
  if (DummyPxeInstance->Http4.Enable) {
    gBS->InstallProtocolInterface (
           &(DummyPxeInstance->Http4.FakePxeHandle),
           &gEfiDevicePathProtocolGuid,
           EFI_NATIVE_INTERFACE,
           DummyPxeInstance->Http4.FakePxeDevicePath
           );
  }
  if (DummyPxeInstance->Http6.Enable) {
    gBS->InstallProtocolInterface (
           &(DummyPxeInstance->Http6.FakePxeHandle),
           &gEfiDevicePathProtocolGuid,
           EFI_NATIVE_INTERFACE,
           DummyPxeInstance->Http6.FakePxeDevicePath
           );
  }
  gBS->InstallMultipleProtocolInterfaces (
         &Instance->UndiHandle,
         &gEfiIp4ServiceBindingProtocolGuid,
         &(DummyIpInstance->Ip4SB.Ip4SB),
         &gEfiDhcp4ServiceBindingProtocolGuid,
         &(DummyIpInstance->Dhcp4SB.Dhcp4SB),
         &gEfiIp4ConfigProtocolGuid,
         &(DummyIpInstance->Ip4Config.Ip4Config),
         &gEfiIp4Config2ProtocolGuid,
         &(DummyIpInstance->Ip4Config2.Ip4Config2),
         &gEfiIp6ServiceBindingProtocolGuid,
         &(DummyIpInstance->Ip6SB.Ip6SB),
         &gEfiDhcp6ServiceBindingProtocolGuid,
         &(DummyIpInstance->Dhcp6SB.Dhcp6SB),
         &gEfiIp6ConfigProtocolGuid,
         &(DummyIpInstance->Ip6Config.Ip6Config),
         NULL,
         NULL
         );

  return EFI_SUCCESS;
}

EFI_STATUS
RelocateAllFunction (
  COMMON_INSTANCE                      *Instance
  )
{
  DUMMY_PXE_INSTANCE                   *DummyPxeInstance;
  DUMMY_IP_INSTANCE                    *DummyIpInstance;
  EFI_STATUS                           Status;

  if (Instance == NULL) {
    return EFI_SUCCESS;
  }

  DummyPxeInstance = &(Instance->Pxe);
  DummyIpInstance  = &(Instance->Ip);

  //
  // Kill fake PXE entry.
  //
  if (DummyPxeInstance->Ip4.Enable) {
    gBS->UninstallProtocolInterface (
           DummyPxeInstance->Ip4.FakePxeHandle,
           &gEfiLoadFileProtocolGuid,
           &(DummyPxeInstance->Ip4.LoadFile)
           );
  }
  if (DummyPxeInstance->Ip6.Enable) {
    gBS->UninstallProtocolInterface (
           DummyPxeInstance->Ip6.FakePxeHandle,
           &gEfiLoadFileProtocolGuid,
           &(DummyPxeInstance->Ip6.LoadFile)
           );
  }
  if (DummyPxeInstance->Http4.Enable) {
    gBS->UninstallProtocolInterface (
           DummyPxeInstance->Http4.FakePxeHandle,
           &gEfiLoadFileProtocolGuid,
           &(DummyPxeInstance->Http4.LoadFile)
           );
  }
  if (DummyPxeInstance->Http6.Enable) {
    gBS->UninstallProtocolInterface (
           DummyPxeInstance->Http6.FakePxeHandle,
           &gEfiLoadFileProtocolGuid,
           &(DummyPxeInstance->Http6.LoadFile)
           );
  }

  //
  // Check Network Stack behavior.
  // If protocol is not ready, return failed to exit PxeDummy.
  //
  Status = gBS->HandleProtocol (
                  Instance->UndiHandle,
                  &gEfiManagedNetworkServiceBindingProtocolGuid,
                  (VOID **) &(DummyIpInstance->MnpSB.RealMnpSB)
                  );

  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Try to get Vlan config protocol.
  //
  Status = gBS->HandleProtocol (
                  Instance->UndiHandle,
                  &gEfiVlanConfigProtocolGuid,
                  (VOID **) &(DummyIpInstance->VlanConfig.RealVlanConfig)
                  );
  DEBUG ((EFI_D_ERROR, "RelocateAllFunction: Get VlanConfigProtocol on Handle %x - %r\n", Instance->UndiHandle, Status));

  Status = gBS->HandleProtocol (
                  Instance->UndiHandle,
                  &gEfiIp4ServiceBindingProtocolGuid,
                  (VOID **) &(DummyIpInstance->Ip4SB.RealIp4SB)
                  );
  DEBUG ((EFI_D_ERROR, "RelocateAllFunction: Get Ip4ServiceBindingProtocol on Handle %x - %r\n", Instance->UndiHandle, Status));

  Status = gBS->HandleProtocol (
                  Instance->UndiHandle,
                  &gEfiDhcp4ServiceBindingProtocolGuid,
                  (VOID **) &(DummyIpInstance->Dhcp4SB.RealDhcp4SB)
                  );
  DEBUG ((EFI_D_ERROR, "RelocateAllFunction: Get Dhcp4ServiceBindingProtocol on Handle %x - %r\n", Instance->UndiHandle, Status));

  //
  // Ip4Config protocol will be remove after Ip4Config2 implement.
  // Do not check it when PxeDummy find real instance.
  //
  Status = gBS->HandleProtocol (
                  Instance->UndiHandle,
                  &gEfiIp4ConfigProtocolGuid,
                  (VOID **) &(DummyIpInstance->Ip4Config.RealIp4Config)
                  );
  DEBUG ((EFI_D_ERROR, "RelocateAllFunction: Get Ip4ConfigProtocol on Handle %x - %r\n", Instance->UndiHandle, Status));

  //
  // Try to locate Ip4Config2
  //
  Status =  gBS->HandleProtocol (
                   Instance->UndiHandle,
                   &gEfiIp4Config2ProtocolGuid,
                   (VOID **) &(DummyIpInstance->Ip4Config2.RealIp4Config2)
                   );
  DEBUG ((EFI_D_ERROR, "RelocateAllFunction: Get Ip4Config2Protocol on Handle %x - %r\n", Instance->UndiHandle, Status));

  Status = gBS->HandleProtocol (
                  Instance->UndiHandle,
                  &gEfiIp6ServiceBindingProtocolGuid,
                  (VOID **) &(DummyIpInstance->Ip6SB.RealIp6SB)
                  );
  DEBUG ((EFI_D_ERROR, "RelocateAllFunction: Get Ip6ServiceBindingProtocol on Handle %x - %r\n", Instance->UndiHandle, Status));

  Status = gBS->HandleProtocol (
                  Instance->UndiHandle,
                  &gEfiDhcp6ServiceBindingProtocolGuid,
                  (VOID **) &(DummyIpInstance->Dhcp6SB.RealDhcp6SB)
                  );
  DEBUG ((EFI_D_ERROR, "RelocateAllFunction: Get Dhcp6ServiceBindingProtocol on Handle %x - %r\n", Instance->UndiHandle, Status));

  Status = gBS->HandleProtocol (
                  Instance->UndiHandle,
                  &gEfiIp6ConfigProtocolGuid,
                  (VOID **) &(DummyIpInstance->Ip6Config.RealIp6Config)
                   );
  DEBUG ((EFI_D_ERROR, "RelocateAllFunction: Get Ip6ConfigProtocol on Handle %x - %r\n", Instance->UndiHandle, Status));

  return EFI_SUCCESS;
}

EFI_STATUS
ConnectDevice (
  IN   COMMON_INSTANCE                 *Instance
  )
{
  EFI_STATUS                           Status;

  if (Instance == NULL) {
    return EFI_SUCCESS;
  }

  if (gDeviceConnecting) {
    return EFI_SUCCESS;
  }

  if (Instance->DeviceConnected) {
    return EFI_SUCCESS;
  }

  Instance->DeviceConnected = TRUE;
  gDeviceConnecting         = TRUE;

  UninstallFakeDevice (Instance);

  //
  // re-binding Network Stack services.
  //
  gNetworkLocker->NetworkStackCallback = gOldNetworkLockerCallback;
  gBS->ConnectController (Instance->UndiHandle, NULL, NULL, TRUE);
  Status = RelocateAllFunction (Instance);
  if (EFI_ERROR(Status)) {
    goto CONNECT_FAILED;
  }

  if (gOldNewPackFunction != NULL) {
    gHiiDataBaseProtocol->NewPackageList = gOldNewPackFunction;
  }
  gDeviceConnecting = FALSE;

  //
  // Reinstall DevicePath to make sure LoadFile can be loaded.
  // And stop event of ReadyToBoot to avoid Mnp DestroyChild Failed.
  //
  ReinstallFakeDevicePath (Instance);
  if (Instance->Ip.MnpSB.ReadyToBootEvent != NULL) {
    gBS->CloseEvent (Instance->Ip.MnpSB.ReadyToBootEvent);
  }

  //
  // Waitting for next trigger of network stack callers.
  // To avoid some driver will binding on Ip.CreateChildren function, this lock should be backup.
  //
  gNetworkLocker->NetworkStackCallback = PxeNetworkStackCallback;
  return Status;

CONNECT_FAILED:
  //
  // Reinstall PxeDummy to make sure fake PXE can be calling.
  //
  ReinstallFakeDevice (Instance);
  Instance->DeviceConnected = FALSE;
  gDeviceConnecting = FALSE;
  return Status;
}

VOID
RemoveDuplicatedDevicePath (
  FAKE_LOAD_FILE_INSTANCE              *LoadInstance
  )
{
  EFI_STATUS                           Status;
  EFI_DEVICE_PATH_PROTOCOL             *UpdatedDevicePath;

  //
  // To avoid Locate.c (499) assert, Finding the same device path installed in two handles.
  //
  UpdatedDevicePath = NULL;
  Status = gBS->HandleProtocol (
                  LoadInstance->RealPxeHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &UpdatedDevicePath
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  if (LoadInstance->RealPxeDevicePath != UpdatedDevicePath) {
    LoadInstance->RealPxeDevicePath = UpdatedDevicePath;
    LoadInstance->FakePxeDevicePath = UpdatedDevicePath;
  }

  gBS->UninstallProtocolInterface (
         LoadInstance->RealPxeHandle,
         &gEfiDevicePathProtocolGuid,
         (VOID *) UpdatedDevicePath
         );

  gBS->InstallProtocolInterface (
         &LoadInstance->FakePxeHandle,
         &gEfiDevicePathProtocolGuid,
         EFI_NATIVE_INTERFACE,
         (VOID *) LoadInstance->FakePxeDevicePath
         );
}

///
// For PXE LoadFile Protocol
//
EFI_STATUS
EFIAPI
FakePxeLoadFile (
  IN     EFI_LOAD_FILE_PROTOCOL        *This,
  IN     EFI_DEVICE_PATH_PROTOCOL      *CallerDevicePath,
  IN     BOOLEAN                       BootPolicy,
  IN OUT UINTN                         *BufferSize,
  IN     VOID                          *Buffer       OPTIONAL
  )
{
  COMMON_INSTANCE                      *Instance;
  DUMMY_PXE_INSTANCE                   *DummyPxeInstance;
  FAKE_LOAD_FILE_INSTANCE              *LoadInstance;
  EFI_STATUS                           Status;
  EFI_LOAD_FILE_PROTOCOL               *RealEntry;

  //
  // Only support BootPolicy
  //
  if (!BootPolicy) {
    return EFI_UNSUPPORTED;
  }

  LoadInstance = (VOID *) This;
  Instance = LoadInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }
  DummyPxeInstance = &(Instance->Pxe);

  if (LoadInstance->RealEntry != NULL) {
    RealEntry = LoadInstance->RealEntry;
    return RealEntry->LoadFile (RealEntry, CallerDevicePath, BootPolicy, BufferSize, Buffer);
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Find real PXE entry.
  //
  Status = FindRealPxeEntry (LoadInstance, &RealEntry);
  if (EFI_ERROR (Status)) {
    goto FUNCTION_FAILED;
  }

  if (RealEntry == NULL) {
    return EFI_NOT_READY;
  }

  //
  // Bypass to real PXE entry.
  //
  LoadInstance->RealEntry = RealEntry;
  return RealEntry->LoadFile (RealEntry, CallerDevicePath, BootPolicy, BufferSize, Buffer);

FUNCTION_FAILED:
  return Status;
}

//
// For HTTP LoadFile Protocol
//
EFI_STATUS
EFIAPI
FakeHttpLoadFile (
  IN     EFI_LOAD_FILE_PROTOCOL        *This,
  IN     EFI_DEVICE_PATH_PROTOCOL      *CallerDevicePath,
  IN     BOOLEAN                       BootPolicy,
  IN OUT UINTN                         *BufferSize,
  IN     VOID                          *Buffer       OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  DUMMY_PXE_INSTANCE                   *DummyPxeInstance;
  FAKE_LOAD_FILE_INSTANCE              *LoadInstance;
  EFI_LOAD_FILE_PROTOCOL               *RealEntry;
  EFI_DEVICE_PATH_PROTOCOL             *RealDevicePath;

  if (This == NULL || BufferSize == NULL || CallerDevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Only support BootPolicy
  //
  if (!BootPolicy) {
    return EFI_UNSUPPORTED;
  }

  LoadInstance = (VOID *) This;
  Instance = LoadInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }
  DummyPxeInstance = &(Instance->Pxe);

  if (LoadInstance->RealEntry != NULL) {
    RealEntry = LoadInstance->RealEntry;
    goto BOOT_TO_HTTP_BOOT_LOAD_FILE;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Find real PXE entry.
  //
  Status = FindRealPxeEntry (LoadInstance, &RealEntry);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (RealEntry == NULL) {
    return EFI_NOT_READY;
  }

  LoadInstance->RealEntry = RealEntry;

BOOT_TO_HTTP_BOOT_LOAD_FILE:
  //
  // Because HttpBootDxe will ReInstall DevicePath to append finded file path.
  // Copy device path to original HttpBoot handle.
  //
  Status = gBS->HandleProtocol (
                  LoadInstance->RealPxeHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &RealDevicePath
                  );
  if (EFI_ERROR (Status)) {
    //
    // Install original DevicePath back when there is no DevicePath protocol.
    //
    Status = gBS->InstallProtocolInterface (
                    &LoadInstance->RealPxeHandle,
                    &gEfiDevicePathProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    LoadInstance->RealPxeDevicePath
                    );
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Try to avoid RegisterRamDisk failed when install device path protocol.
  //
  gBS->UninstallProtocolInterface (
         LoadInstance->FakePxeHandle,
         &gEfiDevicePathProtocolGuid,
         LoadInstance->FakePxeDevicePath
         );

  //
  // Boot to real HTTP boot entry.
  // Maybe ReturnStatus is WARN_FILE_SYSTEM
  //
  Status = RealEntry->LoadFile (RealEntry, CallerDevicePath, BootPolicy, BufferSize, Buffer);
  RemoveDuplicatedDevicePath (LoadInstance);
  return Status;
}

//
// For Ip4Config protocol
//
EFI_STATUS
FakeIp4ConfigStart (
  IN EFI_IP4_CONFIG_PROTOCOL           *This,
  IN EFI_EVENT                         DoneEvent,
  IN EFI_EVENT                         ReconfigEvent
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_CONFIG_INSATNCE          *Ip4ConfigInstance;

  Ip4ConfigInstance = (VOID *) This;
  Instance = Ip4ConfigInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4ConfigInstance->RealIp4Config == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4ConfigInstance->RealIp4Config->Start (Ip4ConfigInstance->RealIp4Config, DoneEvent, ReconfigEvent);
}

EFI_STATUS
FakeIp4ConfigStop(
  IN EFI_IP4_CONFIG_PROTOCOL           *This
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_CONFIG_INSATNCE          *Ip4ConfigInstance;

  Ip4ConfigInstance = (VOID *) This;
  Instance = Ip4ConfigInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4ConfigInstance->RealIp4Config == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4ConfigInstance->RealIp4Config->Stop (Ip4ConfigInstance->RealIp4Config);
}

EFI_STATUS
FakeIp4ConfigGetData (
  IN EFI_IP4_CONFIG_PROTOCOL           *This,
  IN OUT UINTN                         *IpConfigDataSize,
  OUT EFI_IP4_IPCONFIG_DATA            *IpConfigData    OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_CONFIG_INSATNCE          *Ip4ConfigInstance;

  Ip4ConfigInstance = (VOID *) This;
  Instance = Ip4ConfigInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4ConfigInstance->RealIp4Config == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4ConfigInstance->RealIp4Config->GetData (Ip4ConfigInstance->RealIp4Config, IpConfigDataSize, IpConfigData);
}

//
// For Ip6Config protocol
//
EFI_STATUS
FakeIp4Config2SetData (
  IN EFI_IP4_CONFIG2_PROTOCOL   *This,
  IN EFI_IP4_CONFIG2_DATA_TYPE  DataType,
  IN UINTN                      DataSize,
  IN VOID                       *Data
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_CONFIG2_INSATNCE         *Ip4Config2Instance;

  Ip4Config2Instance = (VOID *) This;
  Instance = Ip4Config2Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Config2Instance->RealIp4Config2 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Config2Instance->RealIp4Config2->SetData (Ip4Config2Instance->RealIp4Config2, DataType, DataSize, Data);
}

EFI_STATUS
FakeIp4Config2GetData (
  IN EFI_IP4_CONFIG2_PROTOCOL   *This,
  IN EFI_IP4_CONFIG2_DATA_TYPE  DataType,
  IN OUT UINTN                  *DataSize,
  IN VOID                       *Data   OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_CONFIG2_INSATNCE         *Ip4Config2Instance;

  Ip4Config2Instance = (VOID *) This;
  Instance = Ip4Config2Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Config2Instance->RealIp4Config2 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Config2Instance->RealIp4Config2->GetData (Ip4Config2Instance->RealIp4Config2, DataType, DataSize, Data);
}

EFI_STATUS
FakeIp4Config2RegisterDataNotify (
  IN EFI_IP4_CONFIG2_PROTOCOL   *This,
  IN EFI_IP4_CONFIG2_DATA_TYPE  DataType,
  IN EFI_EVENT                  Event
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_CONFIG2_INSATNCE         *Ip4Config2Instance;

  Ip4Config2Instance = (VOID *) This;
  Instance = Ip4Config2Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Config2Instance->RealIp4Config2 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Config2Instance->RealIp4Config2->RegisterDataNotify (Ip4Config2Instance->RealIp4Config2, DataType, Event);
}

EFI_STATUS
FakeIp4Config2UnregisterDataNotify(
  IN EFI_IP4_CONFIG2_PROTOCOL   *This,
  IN EFI_IP4_CONFIG2_DATA_TYPE  DataType,
  IN EFI_EVENT                  Event
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_CONFIG2_INSATNCE         *Ip4Config2Instance;

  Ip4Config2Instance = (VOID *) This;
  Instance = Ip4Config2Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Config2Instance->RealIp4Config2 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Config2Instance->RealIp4Config2->UnregisterDataNotify (Ip4Config2Instance->RealIp4Config2, DataType, Event);
}

//
// For Ip6Config protocol
//
EFI_STATUS
FakeIp6ConfigSetData (
  IN EFI_IP6_CONFIG_PROTOCOL    *This,
  IN EFI_IP6_CONFIG_DATA_TYPE   DataType,
  IN UINTN                      DataSize,
  IN VOID                       *Data
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_CONFIG_INSATNCE          *Ip6ConfigInstance;

  Ip6ConfigInstance = (VOID *) This;
  Instance = Ip6ConfigInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6ConfigInstance->RealIp6Config == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6ConfigInstance->RealIp6Config->SetData (Ip6ConfigInstance->RealIp6Config, DataType, DataSize, Data);
}

EFI_STATUS
FakeIp6ConfigGetData (
  IN EFI_IP6_CONFIG_PROTOCOL    *This,
  IN EFI_IP6_CONFIG_DATA_TYPE   DataType,
  IN OUT UINTN                  *DataSize,
  IN VOID                       *Data   OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_CONFIG_INSATNCE          *Ip6ConfigInstance;

  Ip6ConfigInstance = (VOID *) This;
  Instance = Ip6ConfigInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6ConfigInstance->RealIp6Config == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6ConfigInstance->RealIp6Config->GetData (Ip6ConfigInstance->RealIp6Config, DataType, DataSize, Data);
}

EFI_STATUS
FakeIp6ConfigRegisterDataNotify (
  IN EFI_IP6_CONFIG_PROTOCOL    *This,
  IN EFI_IP6_CONFIG_DATA_TYPE   DataType,
  IN EFI_EVENT                  Event
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_CONFIG_INSATNCE          *Ip6ConfigInstance;

  Ip6ConfigInstance = (VOID *) This;
  Instance = Ip6ConfigInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6ConfigInstance->RealIp6Config == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6ConfigInstance->RealIp6Config->RegisterDataNotify (Ip6ConfigInstance->RealIp6Config, DataType, Event);
}

EFI_STATUS
FakeIp6ConfigUnregisterDataNotify(
  IN EFI_IP6_CONFIG_PROTOCOL    *This,
  IN EFI_IP6_CONFIG_DATA_TYPE   DataType,
  IN EFI_EVENT                  Event
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_CONFIG_INSATNCE          *Ip6ConfigInstance;

  Ip6ConfigInstance = (VOID *) This;
  Instance = Ip6ConfigInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6ConfigInstance->RealIp6Config == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6ConfigInstance->RealIp6Config->UnregisterDataNotify (Ip6ConfigInstance->RealIp6Config, DataType, Event);
}

//
// For Ip4 protocol
//
EFI_STATUS
FakeIp4GetModeData (
  IN CONST  EFI_IP4_PROTOCOL                *This,
  OUT       EFI_IP4_MODE_DATA               *Ip4ModeData     OPTIONAL,
  OUT       EFI_MANAGED_NETWORK_CONFIG_DATA *MnpConfigData   OPTIONAL,
  OUT       EFI_SIMPLE_NETWORK_MODE         *SnpModeData     OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_INSATNCE                 *Ip4Instance;

  Ip4Instance = (VOID *) This;
  Instance = Ip4Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Instance->RealIp4 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Instance->RealIp4->GetModeData (Ip4Instance->RealIp4, Ip4ModeData, MnpConfigData, SnpModeData);
}

EFI_STATUS
FakeIp4Configure (
  IN EFI_IP4_PROTOCOL    *This,
  IN EFI_IP4_CONFIG_DATA *IpConfigData     OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_INSATNCE                 *Ip4Instance;

  Ip4Instance = (VOID *) This;
  Instance = Ip4Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Instance->RealIp4 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Instance->RealIp4->Configure (Ip4Instance->RealIp4, IpConfigData);
}

EFI_STATUS
FakeIp4Groups (
  IN EFI_IP4_PROTOCOL    *This,
  IN BOOLEAN             JoinFlag,
  IN EFI_IPv4_ADDRESS    *GroupAddress  OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_INSATNCE                 *Ip4Instance;

  Ip4Instance = (VOID *) This;
  Instance = Ip4Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Instance->RealIp4 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Instance->RealIp4->Groups (Ip4Instance->RealIp4, JoinFlag, GroupAddress);
}

EFI_STATUS
FakeIp4Routes (
  IN EFI_IP4_PROTOCOL    *This,
  IN BOOLEAN             DeleteRoute,
  IN EFI_IPv4_ADDRESS    *SubnetAddress,
  IN EFI_IPv4_ADDRESS    *SubnetMask,
  IN EFI_IPv4_ADDRESS    *GatewayAddress
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_INSATNCE                 *Ip4Instance;

  Ip4Instance = (VOID *) This;
  Instance = Ip4Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Instance->RealIp4 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Instance->RealIp4->Routes (Ip4Instance->RealIp4, DeleteRoute, SubnetAddress, SubnetMask, GatewayAddress);
}

EFI_STATUS
FakeIp4Transmit (
  IN EFI_IP4_PROTOCOL          *This,
  IN EFI_IP4_COMPLETION_TOKEN  *Token
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_INSATNCE                 *Ip4Instance;

  Ip4Instance = (VOID *) This;
  Instance = Ip4Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Instance->RealIp4 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Instance->RealIp4->Transmit (Ip4Instance->RealIp4, Token);
}

EFI_STATUS
FakeIp4Receive (
  IN EFI_IP4_PROTOCOL          *This,
  IN EFI_IP4_COMPLETION_TOKEN  *Token
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_INSATNCE                 *Ip4Instance;

  Ip4Instance = (VOID *) This;
  Instance = Ip4Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Instance->RealIp4 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Instance->RealIp4->Receive (Ip4Instance->RealIp4, Token);
}

EFI_STATUS
FakeIp4Cancel (
  IN EFI_IP4_PROTOCOL          *This,
  IN EFI_IP4_COMPLETION_TOKEN  *Token OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_INSATNCE                 *Ip4Instance;

  Ip4Instance = (VOID *) This;
  Instance = Ip4Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Instance->RealIp4 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Instance->RealIp4->Cancel (Ip4Instance->RealIp4, Token);
}

EFI_STATUS
FakeIp4Poll (
  IN EFI_IP4_PROTOCOL          *This
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_INSATNCE                 *Ip4Instance;

  Ip4Instance = (VOID *) This;
  Instance = Ip4Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4Instance->RealIp4 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4Instance->RealIp4->Poll (Ip4Instance->RealIp4);
}

//
// For Ip6 protocol
//
EFI_STATUS
FakeIp6GetModeData (
  IN EFI_IP6_PROTOCOL                 *This,
  OUT EFI_IP6_MODE_DATA               *Ip6ModeData     OPTIONAL,
  OUT EFI_MANAGED_NETWORK_CONFIG_DATA *MnpConfigData   OPTIONAL,
  OUT EFI_SIMPLE_NETWORK_MODE         *SnpModeData     OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_INSATNCE                 *Ip6Instance;

  Ip6Instance = (VOID *) This;
  Instance = Ip6Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6Instance->RealIp6 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6Instance->RealIp6->GetModeData (Ip6Instance->RealIp6, Ip6ModeData, MnpConfigData, SnpModeData);
}

EFI_STATUS
FakeIp6Configure (
  IN EFI_IP6_PROTOCOL            *This,
  IN EFI_IP6_CONFIG_DATA         *Ip6ConfigData OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_INSATNCE                 *Ip6Instance;

  Ip6Instance = (VOID *) This;
  Instance = Ip6Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6Instance->RealIp6== NULL) {
    return EFI_NOT_READY;
  }

  return Ip6Instance->RealIp6->Configure (Ip6Instance->RealIp6, Ip6ConfigData);
}

EFI_STATUS
FakeIp6Groups (
  IN EFI_IP6_PROTOCOL            *This,
  IN BOOLEAN                     JoinFlag,
  IN EFI_IPv6_ADDRESS            *GroupAddress  OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_INSATNCE                 *Ip6Instance;

  Ip6Instance = (VOID *) This;
  Instance = Ip6Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6Instance->RealIp6 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6Instance->RealIp6->Groups (Ip6Instance->RealIp6, JoinFlag, GroupAddress);
}

EFI_STATUS
FakeIp6Routes (
  IN EFI_IP6_PROTOCOL            *This,
  IN BOOLEAN                     DeleteRoute,
  IN EFI_IPv6_ADDRESS            *Destination OPTIONAL,
  IN UINT8                       PrefixLength,
  IN EFI_IPv6_ADDRESS            *GatewayAddress OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_INSATNCE                 *Ip6Instance;

  Ip6Instance = (VOID *) This;
  Instance = Ip6Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6Instance->RealIp6 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6Instance->RealIp6->Routes (Ip6Instance->RealIp6, DeleteRoute, Destination, PrefixLength, GatewayAddress);
}

EFI_STATUS
FakeIp6Neighbors (
  IN EFI_IP6_PROTOCOL            *This,
  IN BOOLEAN                     DeleteFlag,
  IN EFI_IPv6_ADDRESS            *TargetIp6Address,
  IN EFI_MAC_ADDRESS             *TargetLinkAddress,
  IN UINT32                      Timeout,
  IN BOOLEAN                     Override
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_INSATNCE                 *Ip6Instance;

  Ip6Instance = (VOID *) This;
  Instance = Ip6Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6Instance->RealIp6 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6Instance->RealIp6->Neighbors (Ip6Instance->RealIp6, DeleteFlag, TargetIp6Address, TargetLinkAddress, Timeout, Override);
}

EFI_STATUS
FakeIp6Transmit (
  IN EFI_IP6_PROTOCOL            *This,
  IN EFI_IP6_COMPLETION_TOKEN    *Token
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_INSATNCE                 *Ip6Instance;

  Ip6Instance = (VOID *) This;
  Instance = Ip6Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6Instance->RealIp6 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6Instance->RealIp6->Transmit (Ip6Instance->RealIp6, Token);
}

EFI_STATUS
FakeIp6Receive (
  IN EFI_IP6_PROTOCOL            *This,
  IN EFI_IP6_COMPLETION_TOKEN    *Token
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_INSATNCE                 *Ip6Instance;

  Ip6Instance = (VOID *) This;
  Instance = Ip6Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6Instance->RealIp6 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6Instance->RealIp6->Receive (Ip6Instance->RealIp6, Token);
}

EFI_STATUS
FakeIp6Cancel (
  IN EFI_IP6_PROTOCOL            *This,
  IN EFI_IP6_COMPLETION_TOKEN    *Token    OPTIONAL
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_INSATNCE                 *Ip6Instance;

  Ip6Instance = (VOID *) This;
  Instance = Ip6Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (Ip6Instance->RealIp6 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6Instance->RealIp6->Cancel (Ip6Instance->RealIp6, Token);
}

EFI_STATUS
FakeIp6Poll (
  IN EFI_IP6_PROTOCOL            *This
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_INSATNCE                 *Ip6Instance;

  Ip6Instance = (VOID *) This;
  Instance = Ip6Instance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6Instance->RealIp6 == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6Instance->RealIp6->Poll (Ip6Instance->RealIp6);
}

//
// For Ip4 ServiceBinding protocol
//
EFI_STATUS
FakeIp4CreateChild (
  IN     EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                    *ChildHandle
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_SB_INSATNCE              *Ip4SBInstance;

  Ip4SBInstance = (VOID *) This;
  Instance = Ip4SBInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  if (gDeviceConnecting) {
    return EFI_NOT_READY;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4SBInstance->RealIp4SB == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4SBInstance->RealIp4SB->CreateChild (Ip4SBInstance->RealIp4SB, ChildHandle);
}

EFI_STATUS
FakeIp4DestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL          *This,
  IN EFI_HANDLE                            ChildHandle
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP4_SB_INSATNCE              *Ip4SBInstance;

  Ip4SBInstance = (VOID *) This;
  Instance = Ip4SBInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  if (gDeviceConnecting) {
    return EFI_NOT_READY;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip4SBInstance->RealIp4SB == NULL) {
    return EFI_NOT_READY;
  }

  return Ip4SBInstance->RealIp4SB->DestroyChild (Ip4SBInstance->RealIp4SB, ChildHandle);
}

//
// For Ip6 ServiceBinding protocol
//
EFI_STATUS
FakeIp6CreateChild (
  IN     EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                    *ChildHandle
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_SB_INSATNCE              *Ip6SBInstance;

  Ip6SBInstance = (VOID *) This;
  Instance = Ip6SBInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  if (gDeviceConnecting) {
    return EFI_NOT_READY;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6SBInstance->RealIp6SB == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6SBInstance->RealIp6SB->CreateChild (Ip6SBInstance->RealIp6SB, ChildHandle);
}

EFI_STATUS
FakeIp6DestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL          *This,
  IN EFI_HANDLE                            ChildHandle
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_IP6_SB_INSATNCE              *Ip6SBInstance;

  Ip6SBInstance = (VOID *) This;
  Instance = Ip6SBInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  if (gDeviceConnecting) {
    return EFI_NOT_READY;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Ip6SBInstance->RealIp6SB == NULL) {
    return EFI_NOT_READY;
  }

  return Ip6SBInstance->RealIp6SB->DestroyChild (Ip6SBInstance->RealIp6SB, ChildHandle);
}

//
// For Dhcp4 ServiceBinding protocol
//
EFI_STATUS
FakeDhcp4CreateChild (
  IN     EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                    *ChildHandle
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_DHCP4_SB_INSATNCE            *Dhcp4SBInstance;

  Dhcp4SBInstance = (VOID *) This;
  Instance = Dhcp4SBInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Dhcp4SBInstance->RealDhcp4SB == NULL) {
    return EFI_NOT_READY;
  }

  return Dhcp4SBInstance->RealDhcp4SB->CreateChild (Dhcp4SBInstance->RealDhcp4SB, ChildHandle);
}

EFI_STATUS
FakeDhcp4DestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL          *This,
  IN EFI_HANDLE                            ChildHandle
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_DHCP4_SB_INSATNCE            *Dhcp4SBInstance;

  Dhcp4SBInstance = (VOID *) This;
  Instance = Dhcp4SBInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Dhcp4SBInstance->RealDhcp4SB == NULL) {
    return EFI_NOT_READY;
  }

  return Dhcp4SBInstance->RealDhcp4SB->DestroyChild (Dhcp4SBInstance->RealDhcp4SB, ChildHandle);
}

//
// For Dhcp6 ServiceBinding protocol
//
EFI_STATUS
FakeDhcp6CreateChild (
  IN     EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                    *ChildHandle
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_DHCP6_SB_INSATNCE            *Dhcp6SBInstance;

  Dhcp6SBInstance = (VOID *) This;
  Instance = Dhcp6SBInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  if (gDeviceConnecting) {
    return EFI_NOT_READY;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Dhcp6SBInstance->RealDhcp6SB == NULL) {
    return EFI_NOT_READY;
  }

  return Dhcp6SBInstance->RealDhcp6SB->CreateChild (Dhcp6SBInstance->RealDhcp6SB, ChildHandle);
}

EFI_STATUS
FakeDhcp6DestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL          *This,
  IN EFI_HANDLE                            ChildHandle
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_IP_DHCP6_SB_INSATNCE            *Dhcp6SBInstance;

  Dhcp6SBInstance = (VOID *) This;
  Instance = Dhcp6SBInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  if (gDeviceConnecting) {
    return EFI_NOT_READY;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Dhcp6SBInstance->RealDhcp6SB == NULL) {
    return EFI_NOT_READY;
  }

  return Dhcp6SBInstance->RealDhcp6SB->DestroyChild (Dhcp6SBInstance->RealDhcp6SB, ChildHandle);
}

//
// For MNP ServiceBinding protocol
//
EFI_STATUS
FakeMnpCreateChild (
  IN     EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                    *ChildHandle
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_MNP_SB_INSATNCE                 *MnpSBInstance;

  MnpSBInstance = (VOID *) This;
  Instance = MnpSBInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  if (gDeviceConnecting) {
    return EFI_NOT_READY;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (MnpSBInstance->RealMnpSB == NULL) {
    return EFI_NOT_READY;
  }

  return MnpSBInstance->RealMnpSB->CreateChild (MnpSBInstance->RealMnpSB, ChildHandle);
}

EFI_STATUS
FakeMnpDestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL          *This,
  IN EFI_HANDLE                            ChildHandle
  )
{
//  EFI_STATUS                           Status;
//  COMMON_INSTANCE                      *Instance;
//  FAKE_MNP_SB_INSATNCE                 *MnpSBInstance;
//
//  MnpSBInstance = (VOID *) This;
//  Instance = MnpSBInstance->Instance;
//  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
//    return EFI_INVALID_PARAMETER;
//  }
//
//  if (!gAllDeviceConnected) {
//    Status = ConnectDevice ();
//    if (EFI_ERROR (Status)) {
//      return Status;
//    }
//    gAllDeviceConnected = TRUE;
//  }
//
//  return MnpSBInstance->RealMnpSB->DestroyChild (MnpSBInstance->RealMnpSB, ChildHandle);
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
Ip4DeviceExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Ip4DeviceRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Ip4FormCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
FakeHiiDatabaseNewList (
  IN CONST  EFI_HII_DATABASE_PROTOCOL   *This,
  IN CONST  EFI_HII_PACKAGE_LIST_HEADER *PackageList,
  IN        EFI_HANDLE                  DriverHandle, OPTIONAL
  OUT       EFI_HII_HANDLE               *Handle
  )
{
  UINTN                                Index;
  COMMON_INSTANCE                      *Instance;

  if (!CompareGuid (&(PackageList->PackageListGuid), &gDeviceManagerFormSetGuid)) {
    return gOldNewPackFunction (This, PackageList, DriverHandle, Handle);
  }

  for (Index = 0; Index < gDriverInstance->BindedCount; Index++) {
    Instance = (COMMON_INSTANCE *) gDriverInstance->BindedList[Index];
    ConnectDevice (Instance);
  }

  return gOldNewPackFunction (This, PackageList, DriverHandle, Handle);
}

EFI_STATUS 
EFIAPI
Dhcp4DummyGetModeData (
  IN EFI_DHCP4_PROTOCOL     *This,
  OUT EFI_DHCP4_MODE_DATA   *Dhcp4ModeData
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp4DummyConfigure (
  IN EFI_DHCP4_PROTOCOL     *This,
  IN EFI_DHCP4_CONFIG_DATA  *Dhcp4CfgData  OPTIONAL
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp4DummyStart (
  IN EFI_DHCP4_PROTOCOL     *This,
  IN EFI_EVENT              CompletionEvent    OPTIONAL
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp4DummyRenewRebind (
  IN EFI_DHCP4_PROTOCOL     *This,
  IN BOOLEAN                RebindRequest,
  IN EFI_EVENT              CompletionEvent    OPTIONAL
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp4DummyRelease (
  IN EFI_DHCP4_PROTOCOL     *This
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp4DummyStop (
  IN EFI_DHCP4_PROTOCOL     *This
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp4DummyBuild (
  IN EFI_DHCP4_PROTOCOL       *This,
  IN EFI_DHCP4_PACKET         *SeedPacket,
  IN UINT32                   DeleteCount,
  IN UINT8                    *DeleteList        OPTIONAL,
  IN UINT32                   AppendCount,
  IN EFI_DHCP4_PACKET_OPTION  *AppendList[]      OPTIONAL,
  OUT EFI_DHCP4_PACKET        **NewPacket
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp4DummyTransmitReceive (
  IN EFI_DHCP4_PROTOCOL                *This,
  IN EFI_DHCP4_TRANSMIT_RECEIVE_TOKEN  *Token
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp4DummyParse (
  IN EFI_DHCP4_PROTOCOL        *This,
  IN EFI_DHCP4_PACKET          *Packet,
  IN OUT UINT32                *OptionCount,
  OUT EFI_DHCP4_PACKET_OPTION  *PacketOptionList[]  OPTIONAL
  )
{
  return EFI_NOT_READY;
}

EFI_DHCP4_PROTOCOL gDhcp4Dummy = {
                     Dhcp4DummyGetModeData,
                     Dhcp4DummyConfigure,
                     Dhcp4DummyStart,
                     Dhcp4DummyRenewRebind,
                     Dhcp4DummyRelease,
                     Dhcp4DummyStop,
                     Dhcp4DummyBuild,
                     Dhcp4DummyTransmitReceive,
                     Dhcp4DummyParse
                     };


EFI_STATUS 
EFIAPI
Dhcp6DummyGetModeData (
  IN EFI_DHCP6_PROTOCOL        *This,
  OUT EFI_DHCP6_MODE_DATA      *Dhcp6ModeData OPTIONAL,
  OUT EFI_DHCP6_CONFIG_DATA    *Dhcp6ConfigData OPTIONAL
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp6DummyConfigure (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN EFI_DHCP6_CONFIG_DATA     *Dhcp6CfgData OPTIONAL
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp6DummyStart (
  IN EFI_DHCP6_PROTOCOL        *This
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp6DummyInfoRequest (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN BOOLEAN                   SendClientId,
  IN EFI_DHCP6_PACKET_OPTION   *OptionRequest,
  IN UINT32                    OptionCount,
  IN EFI_DHCP6_PACKET_OPTION   *OptionList[] OPTIONAL,
  IN EFI_DHCP6_RETRANSMISSION  *Retransmission, 
  IN EFI_EVENT                 TimeoutEvent OPTIONAL,
  IN EFI_DHCP6_INFO_CALLBACK   ReplyCallback,
  IN VOID                      *CallbackContext OPTIONAL
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp6DummyRenewRebind (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN BOOLEAN                   RebindRequest
  )
{
  return EFI_NOT_READY;
}


EFI_STATUS
EFIAPI
Dhcp6DummyDecline (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN UINT32                    AddressCount,
  IN EFI_IPv6_ADDRESS          *Addresses
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp6DummyRelease (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN UINT32                    AddressCount,
  IN EFI_IPv6_ADDRESS          *Addresses
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp6DummyStop (
  IN EFI_DHCP6_PROTOCOL        *This
  )
{
  return EFI_NOT_READY;
}

EFI_STATUS
EFIAPI
Dhcp6DummyParse (
  IN EFI_DHCP6_PROTOCOL        *This,
  IN EFI_DHCP6_PACKET          *Packet,
  IN OUT UINT32                *OptionCount,
  OUT EFI_DHCP6_PACKET_OPTION  *PacketOptionList[] OPTIONAL
  )
{
  return EFI_NOT_READY;
}

EFI_DHCP6_PROTOCOL gDhcp6Dummy = {
                     Dhcp6DummyGetModeData,
                     Dhcp6DummyConfigure,
                     Dhcp6DummyStart,
                     Dhcp6DummyInfoRequest,
                     Dhcp6DummyRenewRebind,
                     Dhcp6DummyDecline,
                     Dhcp6DummyRelease,
                     Dhcp6DummyStop,
                     Dhcp6DummyParse
                     };

EFI_STATUS
PxeDummyDxeVlanConfigSet (
  IN  EFI_VLAN_CONFIG_PROTOCOL     *This,
  IN  UINT16                       VlanId,
  IN  UINT8                        Priority
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_VLAN_CONFIG_INSATNCE            *VlanConfigInstance;

  VlanConfigInstance = (VOID *) This;
  Instance = VlanConfigInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (VlanConfigInstance->RealVlanConfig == NULL) {
    return EFI_NOT_READY;
  }

  return VlanConfigInstance->RealVlanConfig->Set (VlanConfigInstance->RealVlanConfig, VlanId, Priority);
}

EFI_STATUS
PxeDummyDxeVlanConfigFind (
  IN  EFI_VLAN_CONFIG_PROTOCOL     *This,
  IN  UINT16                       *VlanId  OPTIONAL,
  OUT UINT16                       *NumberOfVlan,
  OUT EFI_VLAN_FIND_DATA           **Entries
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_VLAN_CONFIG_INSATNCE            *VlanConfigInstance;

  VlanConfigInstance = (VOID *) This;
  Instance = VlanConfigInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (VlanConfigInstance->RealVlanConfig == NULL) {
    return EFI_NOT_READY;
  }

  return VlanConfigInstance->RealVlanConfig->Find (VlanConfigInstance->RealVlanConfig, VlanId, NumberOfVlan, Entries);
}

EFI_STATUS
PxeDummyDxeVlanConfigRemove (
  IN  EFI_VLAN_CONFIG_PROTOCOL     *This,
  IN  UINT16                       VlanId
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *Instance;
  FAKE_VLAN_CONFIG_INSATNCE            *VlanConfigInstance;

  VlanConfigInstance = (VOID *) This;
  Instance = VlanConfigInstance->Instance;
  if (Instance->Signature != DUMMY_PXE_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ConnectDevice (Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (VlanConfigInstance->RealVlanConfig == NULL) {
    return EFI_NOT_READY;
  }

  return VlanConfigInstance->RealVlanConfig->Remove (VlanConfigInstance->RealVlanConfig, VlanId);
}

