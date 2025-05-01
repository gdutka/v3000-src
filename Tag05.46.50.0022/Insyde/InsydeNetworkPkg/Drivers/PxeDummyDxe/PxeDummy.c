// @file
//;******************************************************************************
//;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
//;*
//;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
//;* transmit, broadcast, present, recite, release, license or otherwise exploit
//;* any part of this publication in any form, by any means, without the prior
//;* written permission of Insyde Software Corporation.
//;*
//;******************************************************************************
//;


#include "PxeDummy.h"
#include "DummyFunction.h"
#include <Library/PcdLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/InsydeNetworkLib.h>
#include <Library/NetLib.h>
#include <Guid/BdsHii.h>

EFI_GUID gEfiIp6CallerIdGuid          = {0x5BEDB5CC, 0xD830, 0x4eb2, {0x87, 0x42, 0x2D, 0x4C, 0xC9, 0xB5, 0x4F, 0x2C}};
EFI_GUID gEfiNicIp4ConfigVariableGuid = {0xd8944553, 0xc4dd, 0x41f4, {0x9b, 0x30, 0xe1, 0x39, 0x7c, 0xfb, 0x26, 0x7b}};

DRIVER_INSTANCE                        *gDriverInstance;
NETWORK_LOCKER_PROTOCOL                *gNetworkLocker;
NETWORK_STACK_CALLBACK                 gOldNetworkLockerCallback;

EFI_STATUS
CheckMacDevicePathValidation (
  EFI_DEVICE_PATH_PROTOCOL             *UndiDevicePath
  )
{
  EFI_STATUS                           Status;
  EFI_DEVICE_PATH_PROTOCOL             *TmpDPPtr;
  MAC_ADDR_DEVICE_PATH                 *MacDP;

  Status   = EFI_UNSUPPORTED;
  MacDP    = NULL;
  TmpDPPtr = UndiDevicePath;
  while (!IsDevicePathEnd (TmpDPPtr)) {
    if ((DevicePathType (TmpDPPtr) == MESSAGING_DEVICE_PATH) &&
	    (DevicePathSubType (TmpDPPtr) == MSG_MAC_ADDR_DP)) {
      MacDP = (MAC_ADDR_DEVICE_PATH *)TmpDPPtr;
      if (((MAC_ADDR_DEVICE_PATH *)TmpDPPtr)->IfType == 0x01 || ((MAC_ADDR_DEVICE_PATH *)TmpDPPtr)->IfType == 0x00) {
        Status = EFI_SUCCESS;
      }
      break;
    }
    TmpDPPtr = NextDevicePathNode (TmpDPPtr);
  }
  return Status;
}

BOOLEAN
CheckVlanConfigurationVariableExist (
  )
{
  EFI_STATUS                           Status;
  BOOLEAN                              ReturnStatus;
  UINTN                                VariableNameSize;
  CHAR16                               *VariableName;
  UINTN                                BufferSize;
  EFI_GUID                             SearchVariableNameGuid;

  BufferSize   = sizeof (CHAR16) * VARIAB_NAME_BUFFER_LENGTH;
  VariableName = AllocateZeroPool (BufferSize);
  if (VariableName == NULL) {
    return TRUE;
  }
  ReturnStatus = FALSE;
  ZeroMem (&SearchVariableNameGuid, sizeof (EFI_GUID));
  while (TRUE) {
    VariableNameSize = BufferSize;
    Status = gRT->GetNextVariableName (&VariableNameSize, VariableName, &SearchVariableNameGuid);
    if (Status == EFI_BUFFER_TOO_SMALL) {
      VariableName = ReallocatePool (BufferSize, VariableNameSize, VariableName);
      if (VariableName == NULL) {
        ReturnStatus = TRUE;
        goto FUNCTION_EXIT;
      }
      BufferSize = VariableNameSize;
      Status = gRT->GetNextVariableName (&VariableNameSize, VariableName, &SearchVariableNameGuid);
    }
    if (EFI_ERROR (Status)) {
      break;
    }
    if (!CompareGuid (&SearchVariableNameGuid, &gEfiVlanConfigProtocolGuid)) {
      continue;
    }
    ReturnStatus = TRUE;
    break;
  }
FUNCTION_EXIT:
  if (VariableName != NULL) {
    FreePool (VariableName);
  }
  return ReturnStatus;
}

UINTN
FindSameBytesOfMem (
  VOID                                 *Mem1,
  UINTN                                Mem1Len,
  VOID                                 *Mem2,
  UINTN                                Mem2Len
  )
{
  UINTN                                Index;
  UINTN                                SameBytes;
  UINT8                                *Ptr1;
  UINT8                                *Ptr2;

  Ptr1 = Mem1;
  Ptr2 = Mem2;
  SameBytes = 0;
  for (Index = 0; (Index < Mem1Len) && (Index < Mem2Len); Index++) {
    if (Ptr1[Index] != Ptr2[Index]) {
      return SameBytes;
    }
    SameBytes++;
  }
  return SameBytes;
}

VOID
DummyIpInstallFakeVlanConfig (
  COMMON_INSTANCE                      *DummyInstance
  )
{
  EFI_STATUS                           Status;
  DUMMY_IP_INSTANCE                    *DummyIpInstance;

  DummyIpInstance = &(DummyInstance->Ip);
  DummyIpInstance->VlanConfig.VlanConfig.Set    = PxeDummyDxeVlanConfigSet;
  DummyIpInstance->VlanConfig.VlanConfig.Find   = PxeDummyDxeVlanConfigFind;
  DummyIpInstance->VlanConfig.VlanConfig.Remove = PxeDummyDxeVlanConfigRemove;
  DummyIpInstance->VlanConfig.Instance          = DummyInstance;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &DummyInstance->UndiHandle,
                  &gEfiVlanConfigProtocolGuid,
                  &(DummyIpInstance->VlanConfig.VlanConfig),
                  NULL,
                  NULL
                  );
}

VOID
DummyIpUninstallFakeVlanConfig (
  COMMON_INSTANCE                      *DummyInstance
  )
{
  DUMMY_IP_INSTANCE                    *DummyIpInstance;

  DummyIpInstance = &(DummyInstance->Ip);

  gBS->UninstallMultipleProtocolInterfaces (
         DummyInstance->UndiHandle,
         &gEfiVlanConfigProtocolGuid,
         &(DummyIpInstance->VlanConfig.VlanConfig),
         NULL,
         NULL
         );
}

VOID
DummyIpInstallFakeMnp (
  EFI_EVENT                            Event,
  VOID                                 *Context
  )
{
  EFI_STATUS                           Status;
  COMMON_INSTANCE                      *DummyInstance;
  DUMMY_IP_INSTANCE                    *DummyIpInstance;
  FAKE_MNP_SB_INSATNCE                 *MnpSB;

  DummyInstance   = Context;
  DummyIpInstance = &(DummyInstance->Ip);
  MnpSB           = &(DummyIpInstance->MnpSB);

  gBS->CloseEvent (Event);
  DummyIpInstance->MnpSB.ReadyToBootEvent = NULL;
  MnpSB->Enabled  = TRUE;

  DummyIpInstance->MnpSB.MnpSB.CreateChild   = FakeMnpCreateChild;
  DummyIpInstance->MnpSB.MnpSB.DestroyChild  = FakeMnpDestroyChild;
  DummyIpInstance->MnpSB.Instance            = DummyInstance;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &DummyInstance->UndiHandle,
                  &gEfiManagedNetworkServiceBindingProtocolGuid,
                  &(DummyIpInstance->MnpSB.MnpSB),
                  NULL
                  );
  //
  // Construct device path node for EFI HII Config Access protocol,
  // which consists of controller physical device path and one hardware
  // vendor guid node.
  //
  ZeroMem (&(MnpSB->Ip4VendorDeviceNode), sizeof (VENDOR_DEVICE_PATH));
  MnpSB->Ip4VendorDeviceNode.Header.Type    = HARDWARE_DEVICE_PATH;
  MnpSB->Ip4VendorDeviceNode.Header.SubType = HW_VENDOR_DP;

  CopyGuid (&(MnpSB->Ip4VendorDeviceNode.Guid), &gEfiCallerIdGuid);

  SetDevicePathNodeLength (&(MnpSB->Ip4VendorDeviceNode.Header), sizeof (VENDOR_DEVICE_PATH));

  MnpSB->Ip4HiiVendorDevicePath = AppendDevicePathNode (
                                    DummyInstance->UndiDevicePath,
                                    (EFI_DEVICE_PATH_PROTOCOL *) &(MnpSB->Ip4VendorDeviceNode)
                                    );

  MnpSB->Ip4ChildHandle         = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &(MnpSB->Ip4ChildHandle),
                  &gEfiDevicePathProtocolGuid,
                  MnpSB->Ip4HiiVendorDevicePath,
                  NULL
                  );

  if (!EFI_ERROR (Status)) {
    Status = gBS->OpenProtocol (
                    DummyInstance->UndiHandle,
                    &gEfiManagedNetworkServiceBindingProtocolGuid,
                    (VOID **) &(MnpSB->MnpSb),
                    DummyInstance->ImageHandle,
                    MnpSB->Ip4ChildHandle,
                    EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER
                    );
  }

  DummyIpInstallFakeVlanConfig (DummyInstance);
}

EFI_STATUS
DummyIpUninstallFakeMnp (
  COMMON_INSTANCE                      *DummyInstance
  )
{
  DUMMY_IP_INSTANCE                    *DummyIpInstance;
  FAKE_MNP_SB_INSATNCE                 *MnpSB;

  DummyIpInstance = &(DummyInstance->Ip);
  MnpSB           = &(DummyIpInstance->MnpSB);

  //
  // ReadyToBoot event should be close, to avoid it will be called after PxeDummy was be disconnected.
  //
  if (DummyIpInstance->MnpSB.ReadyToBootEvent != NULL) {
    gBS->CloseEvent (DummyIpInstance->MnpSB.ReadyToBootEvent);
    DummyIpInstance->MnpSB.ReadyToBootEvent = NULL;
  }

  if (!MnpSB->Enabled) {
    return EFI_SUCCESS;
  }

  gBS->CloseProtocol (
         DummyInstance->UndiHandle,
         &gEfiManagedNetworkServiceBindingProtocolGuid,
         DummyInstance->ImageHandle,
         MnpSB->Ip4ChildHandle
         );

  gBS->UninstallMultipleProtocolInterfaces (
         DummyInstance->UndiHandle,
         &gEfiManagedNetworkServiceBindingProtocolGuid,
         &(DummyIpInstance->MnpSB.MnpSB),
         NULL,
         NULL
         );

  gBS->UninstallMultipleProtocolInterfaces (
         (MnpSB->Ip4ChildHandle),
         &gEfiDevicePathProtocolGuid,
         MnpSB->Ip4HiiVendorDevicePath,
         NULL
         );


  return EFI_SUCCESS;
}

EFI_STATUS
DummyIpCreateFakeHandle (
  COMMON_INSTANCE                      *DummyInstance,
  UINTN                                IpCapability
  )
{
  EFI_STATUS                           Status;
  DUMMY_IP_INSTANCE                    *DummyIpInstance;

  DummyIpInstance = &(DummyInstance->Ip);
  if (IpCapability == PXE_IPv4) {
    DummyIpInstance->Ip4SB.Ip4SB.CreateChild      = FakeIp4CreateChild;
    DummyIpInstance->Ip4SB.Ip4SB.DestroyChild     = FakeIp4DestroyChild;
    DummyIpInstance->Ip4SB.Instance               = DummyInstance;

    DummyIpInstance->Dhcp4SB.Dhcp4SB.CreateChild  = FakeDhcp4CreateChild;
    DummyIpInstance->Dhcp4SB.Dhcp4SB.DestroyChild = FakeDhcp4DestroyChild;
    DummyIpInstance->Dhcp4SB.Instance             = DummyInstance;

    DummyIpInstance->Ip4Config.Ip4Config.GetData  = FakeIp4ConfigGetData;
    DummyIpInstance->Ip4Config.Ip4Config.Start    = FakeIp4ConfigStart;
    DummyIpInstance->Ip4Config.Ip4Config.Stop     = FakeIp4ConfigStop;
    DummyIpInstance->Ip4Config.Instance           = DummyInstance;

    DummyIpInstance->Ip4Config2.Ip4Config2.SetData              = FakeIp4Config2SetData;
    DummyIpInstance->Ip4Config2.Ip4Config2.GetData              = FakeIp4Config2GetData;
    DummyIpInstance->Ip4Config2.Ip4Config2.RegisterDataNotify   = FakeIp4Config2RegisterDataNotify;
    DummyIpInstance->Ip4Config2.Ip4Config2.UnregisterDataNotify = FakeIp4Config2UnregisterDataNotify;
    DummyIpInstance->Ip4Config2.Instance          = DummyInstance;

    Status = gBS->InstallMultipleProtocolInterfaces (
                    &DummyInstance->UndiHandle,
                    &gEfiIp4ServiceBindingProtocolGuid,
                    &(DummyIpInstance->Ip4SB.Ip4SB),
                    &gEfiDhcp4ServiceBindingProtocolGuid,
                    &(DummyIpInstance->Dhcp4SB.Dhcp4SB),
                    &gEfiIp4ConfigProtocolGuid,
                    &(DummyIpInstance->Ip4Config.Ip4Config),
                    &gEfiIp4Config2ProtocolGuid,
                    &(DummyIpInstance->Ip4Config2.Ip4Config2),
                    NULL
                    );

  } else if (IpCapability == PXE_IPv6) {
    DummyIpInstance->Ip6SB.Ip6SB.CreateChild                  = FakeIp6CreateChild;
    DummyIpInstance->Ip6SB.Ip6SB.DestroyChild                 = FakeIp6DestroyChild;
    DummyIpInstance->Ip6SB.Instance                           = DummyInstance;
    DummyIpInstance->Dhcp6SB.Dhcp6SB.CreateChild              = FakeDhcp6CreateChild;
    DummyIpInstance->Dhcp6SB.Dhcp6SB.DestroyChild             = FakeDhcp6DestroyChild;
    DummyIpInstance->Dhcp6SB.Instance                         = DummyInstance;
    DummyIpInstance->Ip6Config.Ip6Config.SetData              = FakeIp6ConfigSetData;
    DummyIpInstance->Ip6Config.Ip6Config.GetData              = FakeIp6ConfigGetData;
    DummyIpInstance->Ip6Config.Ip6Config.RegisterDataNotify   = FakeIp6ConfigRegisterDataNotify;
    DummyIpInstance->Ip6Config.Ip6Config.UnregisterDataNotify = FakeIp6ConfigUnregisterDataNotify;
    DummyIpInstance->Ip6Config.Instance                       = DummyInstance;

    Status = gBS->InstallMultipleProtocolInterfaces (
                    &DummyInstance->UndiHandle,
                    &gEfiIp6ServiceBindingProtocolGuid,
                    &(DummyIpInstance->Ip6SB.Ip6SB),
                    &gEfiDhcp6ServiceBindingProtocolGuid,
                    &(DummyIpInstance->Dhcp6SB.Dhcp6SB),
                    &gEfiIp6ConfigProtocolGuid,
                    &(DummyIpInstance->Ip6Config.Ip6Config),
                    NULL,
                    NULL
                    );

  } else if (IpCapability == PXE_MNP) {
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    DummyIpInstallFakeMnp,
                    DummyInstance,
                    &gEfiEventReadyToBootGuid,
                    &(DummyIpInstance->MnpSB.ReadyToBootEvent)
                    );

  } else {
    return EFI_INVALID_PARAMETER;
  }

  return Status;
}

EFI_STATUS
DummyIpDestroyFakeHandle (
  COMMON_INSTANCE                      *DummyInstance,
  UINTN                                IpCapability
  )
{
  DUMMY_IP_INSTANCE                    *DummyIpInstance;

  DummyIpInstance = &(DummyInstance->Ip);
  if (IpCapability == PXE_IPv4) {
    gBS->UninstallMultipleProtocolInterfaces (
           DummyInstance->UndiHandle,
           &gEfiIp4ServiceBindingProtocolGuid,
           &(DummyIpInstance->Ip4SB.Ip4SB),
           &gEfiDhcp4ServiceBindingProtocolGuid,
           &(DummyIpInstance->Dhcp4SB.Dhcp4SB),
           &gEfiIp4ConfigProtocolGuid,
           &(DummyIpInstance->Ip4Config.Ip4Config),
           &gEfiIp4Config2ProtocolGuid,
           &(DummyIpInstance->Ip4Config2.Ip4Config2),
           NULL,
           NULL
           );

  } else if (IpCapability == PXE_IPv6) {
    gBS->UninstallMultipleProtocolInterfaces (
           DummyInstance->UndiHandle,
           &gEfiIp6ServiceBindingProtocolGuid,
           &(DummyIpInstance->Ip6SB.Ip6SB),
           &gEfiDhcp6ServiceBindingProtocolGuid,
           &(DummyIpInstance->Dhcp6SB.Dhcp6SB),
           &gEfiIp6ConfigProtocolGuid,
           &(DummyIpInstance->Ip6Config.Ip6Config),
           NULL,
           NULL
           );

  } else {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
DummyPxeCreateFakeHandle (
  COMMON_INSTANCE                      *DummyInstance,
  UINTN                                PxeCapability
  )
{
  EFI_STATUS                           Status;
  DUMMY_PXE_INSTANCE                   *DummyPxeInstance;
  EFI_DEVICE_PATH                      *FakePxeDevicePath;
  EFI_DEVICE_PATH                      *FakeHttpDevicePath;
  EFI_HANDLE                           FakePxeHandle;
  IPv4_DEVICE_PATH                     *Ip4Node;
  IPv6_DEVICE_PATH                     *Ip6Node;
  URI_DEVICE_PATH                      *UriNode;
  FAKE_LOAD_FILE_INSTANCE              *Instance;

  EFI_LOAD_FILE                        FakeLoadFilePtr;

  Ip4Node            = NULL;
  Ip6Node            = NULL;
  FakePxeDevicePath  = NULL;
  UriNode            = NULL;
  FakeHttpDevicePath = NULL;
  DummyPxeInstance   = &(DummyInstance->Pxe);
  FakeLoadFilePtr    = FakePxeLoadFile;
  if ((PxeCapability == PXE_IPv4) || (PxeCapability == HTTP_IPv4)) {
    //
    // Create a device path node for Ipv4 virtual nic, and append it.
    //
    Ip4Node = AllocateZeroPool (sizeof (IPv4_DEVICE_PATH));
    if (Ip4Node == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Ip4Node->Header.Type     = MESSAGING_DEVICE_PATH;
    Ip4Node->Header.SubType  = MSG_IPv4_DP;
    Ip4Node->StaticIpAddress = FALSE;
    SetDevicePathNodeLength (&(Ip4Node->Header), sizeof (IPv4_DEVICE_PATH));
    FakePxeDevicePath        = (EFI_DEVICE_PATH_PROTOCOL *) Ip4Node;
    if (PxeCapability == PXE_IPv4) {
      Instance = &(DummyPxeInstance->Ip4);
      Instance->IsPxe  = TRUE;
    } else {
      Instance = &(DummyPxeInstance->Http4);
      Instance->IsHttp = TRUE;
      FakeLoadFilePtr  = FakeHttpLoadFile;
    }
    Instance->IsIpv4 = TRUE;
  } else if ((PxeCapability == PXE_IPv6) || (PxeCapability == HTTP_IPv6)) {
    //
    // Create a device path node for Ipv6 virtual nic, and append it.
    //
    Ip6Node = AllocateZeroPool (sizeof (IPv6_DEVICE_PATH));
    if (Ip6Node == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Ip6Node->Header.Type     = MESSAGING_DEVICE_PATH;
    Ip6Node->Header.SubType  = MSG_IPv6_DP;
    Ip6Node->PrefixLength    = 64;
    SetDevicePathNodeLength (&(Ip6Node->Header), sizeof (IPv6_DEVICE_PATH));
    FakePxeDevicePath        = (EFI_DEVICE_PATH_PROTOCOL *) Ip6Node;
    if (PxeCapability == PXE_IPv6) {
      Instance = &(DummyPxeInstance->Ip6);
      Instance->IsPxe  = TRUE;
    } else {
      Instance = &(DummyPxeInstance->Http6);
      Instance->IsHttp  = TRUE;
      FakeLoadFilePtr   = FakeHttpLoadFile;
    }
    Instance->IsIpv6 = TRUE;
  }else {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Prepare DevicePath to provide PXE handle.
  //
  FakePxeDevicePath = AppendDevicePathNode (DummyInstance->UndiDevicePath, FakePxeDevicePath);
  if (FakePxeDevicePath == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ERROR;
  }

  //
  // Append URI DevicePath at tail of PXE DevicePath, It's a HTTP boot handle.
  //
  if ((PxeCapability == HTTP_IPv4) || (PxeCapability == HTTP_IPv6)) {
    UriNode = AllocateZeroPool (sizeof (URI_DEVICE_PATH));
    if (UriNode == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto ERROR;
    }
    UriNode->Header.Type    = MESSAGING_DEVICE_PATH;
    UriNode->Header.SubType = MSG_URI_DP;
    SetDevicePathNodeLength (&(UriNode->Header), sizeof (URI_DEVICE_PATH));
    FakeHttpDevicePath      = AppendDevicePathNode (FakePxeDevicePath, (EFI_DEVICE_PATH_PROTOCOL *) UriNode);
    if (FakeHttpDevicePath == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto ERROR;
    }
    gBS->FreePool (FakePxeDevicePath);
    FakePxeDevicePath = FakeHttpDevicePath;
    FakeHttpDevicePath = NULL;
  }

  Instance->LoadFile.LoadFile = FakeLoadFilePtr;
  Instance->PxeCapability     = PxeCapability;
  Instance->Instance          = DummyInstance;

  //
  // Install the CallerId GUID to FakePxeHandle
  //
  FakePxeHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &FakePxeHandle,
                  &gEfiCallerIdGuid,
                  NULL,
                  &gEfiDevicePathProtocolGuid,
                  FakePxeDevicePath,
                  &gEfiLoadFileProtocolGuid,
                  &(Instance->LoadFile),
                  NULL,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    goto ERROR;
  }
  Instance->FakePxeHandle     = FakePxeHandle;
  Instance->FakePxeDevicePath = FakePxeDevicePath;
  Instance->Enable            = TRUE;

ERROR:
  if (EFI_ERROR (Status)) {
    if (FakePxeDevicePath != NULL) {
      gBS->FreePool (FakePxeDevicePath);
    }
    if (FakeHttpDevicePath != NULL) {
      gBS->FreePool (FakeHttpDevicePath);
    }
  }
  if (Ip4Node != NULL) {
    gBS->FreePool (Ip4Node);
  }
  if (Ip6Node != NULL) {
    gBS->FreePool (Ip6Node);
  }
  if (UriNode != NULL) {
    gBS->FreePool (UriNode);
  }
  return Status;
}

EFI_STATUS
DummyPxeDestroyFakeHandle (
  COMMON_INSTANCE                      *DummyInstance,
  UINTN                                PxeCapability
  )
{
  EFI_STATUS                           Status;
  DUMMY_PXE_INSTANCE                   *DummyPxeInstance;
  EFI_HANDLE                           FakePxeHandle;
  FAKE_LOAD_FILE_INSTANCE              *Instance;
  VOID                                 *ProtocolPointer;

  DummyPxeInstance   = &(DummyInstance->Pxe);
  if ((PxeCapability == PXE_IPv4) || (PxeCapability == HTTP_IPv4)) {
    if (PxeCapability == PXE_IPv4) {
      Instance = &(DummyPxeInstance->Ip4);
    } else {
      Instance = &(DummyPxeInstance->Http4);
    }
  } else if ((PxeCapability == PXE_IPv6) || (PxeCapability == HTTP_IPv6)) {
    if (PxeCapability == PXE_IPv6) {
      Instance = &(DummyPxeInstance->Ip6);
    } else {
      Instance = &(DummyPxeInstance->Http6);
    }
  }else {
    return EFI_INVALID_PARAMETER;
  }

  if (Instance->Instance->DeviceConnected) {
    Status = gBS->OpenProtocol (
                    Instance->FakePxeHandle,
                    &gEfiSimpleNetworkProtocolGuid,
                    (VOID **) &ProtocolPointer,
                    Instance->Instance->ImageHandle,
                    Instance->FakePxeHandle,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );
    if (!EFI_ERROR (Status)) {
      gBS->CloseProtocol (
             Instance->FakePxeHandle,
             &gEfiSimpleNetworkProtocolGuid,
             Instance->Instance->ImageHandle,
             Instance->FakePxeHandle
             );

      gBS->UninstallProtocolInterface (
             Instance->FakePxeHandle,
             &gEfiSimpleNetworkProtocolGuid,
             ProtocolPointer
             );
    }

    Status = gBS->HandleProtocol (
                    Instance->FakePxeHandle,
                    &gEfiPxeBaseCodeProtocolGuid,
                    (VOID **) &ProtocolPointer
                    );
    if (!EFI_ERROR (Status)) {
      gBS->UninstallProtocolInterface (
             Instance->FakePxeHandle,
             &gEfiPxeBaseCodeProtocolGuid,
             ProtocolPointer
             );
    }

    //
    // Try to find PxeBaseCode protocol from real handle and uninstall it.
    //
    if (Instance->RealPxeHandle != NULL) {
      Status = gBS->HandleProtocol (
                      Instance->RealPxeHandle,
                      &gEfiPxeBaseCodeProtocolGuid,
                      (VOID **) &ProtocolPointer
                      );
      if (!EFI_ERROR (Status)) {
        gBS->UninstallProtocolInterface (
               Instance->RealPxeHandle,
               &gEfiPxeBaseCodeProtocolGuid,
               ProtocolPointer
               );
      }
    }
  }

  if (DummyInstance->RealMnpSb != NULL) {
    gBS->CloseProtocol (
           DummyInstance->UndiHandle,
           &gEfiManagedNetworkServiceBindingProtocolGuid,
           DummyInstance->ImageHandle,
           Instance->FakePxeHandle
           );
    DummyInstance->RealMnpSb = NULL;
  }

  //
  // Uninstall the Dns Protocol onto ControlerHandle
  //
  FakePxeHandle = NULL;
  Status = gBS->UninstallMultipleProtocolInterfaces (
                  Instance->FakePxeHandle,
                  &gEfiCallerIdGuid,
                  NULL,
                  &gEfiDevicePathProtocolGuid,
                  Instance->FakePxeDevicePath,
                  &gEfiLoadFileProtocolGuid,
                  &(Instance->LoadFile),
                  NULL,
                  NULL
                  );
  if (Instance->FakePxeDevicePath != NULL) {
    FreePool (Instance->FakePxeDevicePath);
  }

  return Status;
}

EFI_STATUS
DummyPxeDestroyServices (
  DRIVER_INSTANCE                      *Instance,
  EFI_HANDLE                           ControllerHandle
  )
{
  UINTN                                Index;
  COMMON_INSTANCE                      *DummyInstance;

  //
  // Find the connected device handle, It will be destroy if handle is the same.
  //
  DummyInstance = NULL;
  for (Index = 0; Index < Instance->BindedCount; Index ++) {
    if ((Instance->BindedList[Index] == NULL) ||
        (Instance->BindedList[Index]->UndiHandle!= ControllerHandle)) {
      continue;
    }
    DummyInstance = Instance->BindedList[Index];
    Instance->BindedList[Index] = NULL;

    if (Index != (Instance->BindedCount - 1)) {
      CopyMem (
        Instance->BindedList + Index,
        Instance->BindedList + (Index + 1),
        (Instance->BindedCount - Index - 1) * sizeof (COMMON_INSTANCE *)
        );
    }

    Instance->BindedCount--;
    break;
  }

  if (DummyInstance == NULL) {
    return EFI_SUCCESS;
  }

  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiSimpleNetworkProtocolGuid,
         DummyInstance->ImageHandle,
         ControllerHandle
         );

  //
  // Install Fake LoadFile Protocol as boot entry.
  //
  if (Instance->IsPxeIpv4Enabled) {
    DummyPxeDestroyFakeHandle (DummyInstance, PXE_IPv4);
  }

  if (Instance->IsPxeIpv6Enabled) {
    DummyPxeDestroyFakeHandle (DummyInstance, PXE_IPv6);
  }

  if (Instance->IsHttpIpv4Enabled) {
    DummyPxeDestroyFakeHandle (DummyInstance, HTTP_IPv4);
  }

  if (Instance->IsHttpIpv6Enabled) {
    DummyPxeDestroyFakeHandle (DummyInstance, HTTP_IPv6);
  }

  //
  // Install Fake IP Protocol for Network stack services
  //
  if (Instance->IsIpv4Enabled | Instance->IsIpv6Enabled) {
    DummyIpUninstallFakeMnp (DummyInstance);
    DummyIpUninstallFakeVlanConfig (DummyInstance);

    if (Instance->IsIpv4Enabled) {
      DummyIpDestroyFakeHandle (DummyInstance, PXE_IPv4);
    }
    if (Instance->IsIpv6Enabled) {
      DummyIpDestroyFakeHandle (DummyInstance, PXE_IPv6);
    }
  }
  FreePool (DummyInstance);
  return EFI_SUCCESS;
}


/**
  Test to see if this driver supports ControllerHandle. This service
  is called by the EFI boot service ConnectController(). In
  order to make drivers as small as possible, there are a few calling
  restrictions for this service. ConnectController() must
  follow these calling restrictions. If any other agent wishes to call
  Supported() it must also follow these calling restrictions.

  @param[in]  This                Protocol instance pointer.
  @param[in]  ControllerHandle    Handle of device to test
  @param[in]  RemainingDevicePath Optional parameter use to pick a specific child
                                  device to start.

  @retval EFI_SUCCESS         This driver supports this device
  @retval EFI_ALREADY_STARTED This driver is already running on this device
  @retval other               This driver does not support this device

**/
EFI_STATUS
EFIAPI
DummyPxeDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS                   Status;
  EFI_SIMPLE_NETWORK_PROTOCOL  *Snp;

  if (gNetworkLocker == NULL) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiSimpleNetworkProtocolGuid,
                  (VOID **) &Snp,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiSimpleNetworkProtocolGuid,
         This->DriverBindingHandle,
         ControllerHandle
         );

  return Status;
}

/**
  Start this driver on ControllerHandle. This service is called by the
  EFI boot service ConnectController(). In order to make
  drivers as small as possible, there are a few calling restrictions for
  this service. ConnectController() must follow these
  calling restrictions. If any other agent wishes to call Start() it
  must also follow these calling restrictions.

  @param[in]  This                 Protocol instance pointer.
  @param[in]  ControllerHandle     Handle of device to bind driver to
  @param[in]  RemainingDevicePath  Optional parameter use to pick a specific child
                                   device to start.

  @retval EFI_SUCCESS          This driver is added to ControllerHandle
  @retval EFI_ALREADY_STARTED  This driver is already running on ControllerHandle
  @retval other                This driver does not support this device

**/
EFI_STATUS
EFIAPI
DummyPxeDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   UndiDeviceHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS                Status;
  DRIVER_INSTANCE           *Instance;
  COMMON_INSTANCE           *DummyInstance;
  COMMON_INSTANCE           **BindedList;
  EFI_DEVICE_PATH           *UndiDevicePath;
  VOID                      *BindingPoint;
  EFI_SIMPLE_NETWORK_PROTOCOL  *Snp;
  NETWORK_LOCKER_PROTOCOL      *ControllerNetworkPolicy;

  if (gNetworkLocker == NULL) {
    return EFI_UNSUPPORTED;
  }

  ControllerNetworkPolicy = NULL;
  Status = gBS->HandleProtocol (
                  UndiDeviceHandle,
                  &gNetworkLockerProtocolGuid,
                  (VOID **) &ControllerNetworkPolicy
                  );
  if (!EFI_ERROR (Status)) {
    /// If policy override, PxeDummyDxe will not handle this network device.
    return EFI_SUCCESS;
  }

  Status = gBS->OpenProtocol (
                  UndiDeviceHandle,
                  &gEfiCallerIdGuid,
                  NULL,
                  UndiDeviceHandle,
                  UndiDeviceHandle,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (!EFI_ERROR (Status)) {
    return EFI_ALREADY_STARTED;
  }

  Status = gBS->OpenProtocol (
                  UndiDeviceHandle,
                  &gEfiSimpleNetworkProtocolGuid,
                  (VOID **) &Snp,
                  This->DriverBindingHandle,
                  UndiDeviceHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DummyInstance = NULL;
  Instance = DUMMY_PXE_INSTANCE_FROM_THIS (This);
  if (Instance->Signature != DUMMY_PXE_DRIVER_INSTANCE_SIGNATURE) {
    Status = EFI_INVALID_PARAMETER;
    goto ON_ERROR;
  }

  Status = gBS->HandleProtocol (
                  UndiDeviceHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **) &UndiDevicePath
                  );
  if (EFI_ERROR (Status)) {
    goto ON_ERROR;
  }

  //
  // Check Device Path MAC IfType, if it's unable to handle, this device will be skip.
  //
  Status = CheckMacDevicePathValidation (UndiDevicePath);
  if (EFI_ERROR (Status)) {
    goto ON_ERROR;
  }

  DummyInstance = AllocateZeroPool (sizeof (COMMON_INSTANCE));
  if (DummyInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DummyInstance->Signature      = DUMMY_PXE_INSTANCE_SIGNATURE;
  DummyInstance->ImageHandle    = Instance->ImageHandle;
  DummyInstance->UndiHandle     = UndiDeviceHandle;
  DummyInstance->UndiDevicePath = UndiDevicePath;
  DummyInstance->Interface      = This;

  //
  // Install Fake LoadFile Protocol as boot entry.
  //
  if (Instance->IsPxeIpv4Enabled) {
    Status = DummyPxeCreateFakeHandle (DummyInstance, PXE_IPv4);
    if (EFI_ERROR (Status)) {
      goto ON_ERROR;
    }
  }

  if (Instance->IsPxeIpv6Enabled) {
    Status = DummyPxeCreateFakeHandle (DummyInstance, PXE_IPv6);
    if (EFI_ERROR (Status)) {
      goto ON_ERROR;
    }
  }

  if (Instance->IsHttpIpv4Enabled) {
    Status = DummyPxeCreateFakeHandle (DummyInstance, HTTP_IPv4);
    if (EFI_ERROR (Status)) {
      goto ON_ERROR;
    }
  }

  if (Instance->IsHttpIpv6Enabled) {
    Status = DummyPxeCreateFakeHandle (DummyInstance, HTTP_IPv6);
    if (EFI_ERROR (Status)) {
      goto ON_ERROR;
    }
  }

  //
  // Install Fake IP Protocol for Network stack services
  //
  if (Instance->IsIpv4Enabled | Instance->IsIpv6Enabled) {
    Status = DummyIpCreateFakeHandle (DummyInstance, PXE_MNP);
    if (EFI_ERROR (Status)) {
      goto ON_ERROR;
    }
    if (Instance->IsIpv4Enabled) {
      Status = DummyIpCreateFakeHandle (DummyInstance, PXE_IPv4);
      if (EFI_ERROR (Status)) {
        goto ON_ERROR;
      }
    }
    if (Instance->IsIpv6Enabled) {
      Status = DummyIpCreateFakeHandle (DummyInstance, PXE_IPv6);
      if (EFI_ERROR (Status)) {
        goto ON_ERROR;
      }
    }
  }

  //
  // Install trap for device manager
  //
  if (gOldNewPackFunction == NULL) {
    Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &(gHiiDataBaseProtocol));
    if (EFI_ERROR (Status)) {
      goto ON_ERROR;
    }
    gOldNewPackFunction = gHiiDataBaseProtocol->NewPackageList;
    gHiiDataBaseProtocol->NewPackageList = FakeHiiDatabaseNewList;
  }

  //
  // Record HANDLE address in instance.
  //
  BindedList = AllocateZeroPool (sizeof (COMMON_INSTANCE *) * (Instance->BindedCount + 1));
  if (BindedList == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ON_ERROR;
  }

  if (Instance->BindedCount != 0) {
    CopyMem (BindedList, Instance->BindedList, Instance->BindedCount * sizeof (COMMON_INSTANCE *));
    gBS->FreePool (Instance->BindedList);
  }
  BindedList[Instance->BindedCount] = (VOID *) DummyInstance;
  Instance->BindedList = BindedList;
  Instance->BindedCount++;

  Status = gBS->InstallProtocolInterface (
                  &UndiDeviceHandle,
                  &gEfiCallerIdGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    goto ON_ERROR;
  }

  //
  // Install CallerId and open it, to wait stop notify when disconnect processing.
  //
  Status = gBS->OpenProtocol (
                  UndiDeviceHandle,
                  &gEfiCallerIdGuid,
                  &BindingPoint,
                  This->DriverBindingHandle,
                  UndiDeviceHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );

ON_ERROR:
  if (EFI_ERROR (Status)) {
    gBS->CloseProtocol (
           UndiDeviceHandle,
           &gEfiSimpleNetworkProtocolGuid,
           This->DriverBindingHandle,
           UndiDeviceHandle
           );
    if (DummyInstance != NULL) {
      gBS->FreePool (DummyInstance);
    }
  }

  return Status;
}

/**
  Stop this driver on ControllerHandle. This service is called by the
  EFI boot service DisconnectController(). In order to
  make drivers as small as possible, there are a few calling
  restrictions for this service. DisconnectController()
  must follow these calling restrictions. If any other agent wishes
  to call Stop() it must also follow these calling restrictions.

  @param[in]  This              Protocol instance pointer.
  @param[in]  ControllerHandle  Handle of device to stop driver on
  @param[in]  NumberOfChildren  Number of Handles in ChildHandleBuffer. If number of
                                children is zero stop the entire bus driver.
  @param[in]  ChildHandleBuffer List of Child Handles to Stop.

  @retval EFI_SUCCESS       This driver is removed ControllerHandle
  @retval other             This driver was not removed from this device

**/
EFI_STATUS
EFIAPI
DummyPxeDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL       *This,
  IN  EFI_HANDLE                        ControllerHandle,
  IN  UINTN                             NumberOfChildren,
  IN  EFI_HANDLE                        *ChildHandleBuffer
  )
{
  EFI_STATUS                            Status;
  DRIVER_INSTANCE                       *Instance;

  Instance = DUMMY_PXE_INSTANCE_FROM_THIS (This);
  if (Instance->Signature != DUMMY_PXE_DRIVER_INSTANCE_SIGNATURE) {
    return EFI_INVALID_PARAMETER;
  }

  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiCallerIdGuid,
         This->DriverBindingHandle,
         ControllerHandle
         );

  //
  // Disconnect ControllerHandle if it exist in binded device list.
  //
  Status = DummyPxeDestroyServices (Instance, ControllerHandle);
  gBS->UninstallProtocolInterface (
         ControllerHandle,
         &gEfiCallerIdGuid,
         NULL
         );

  return Status;
}

/**
  This function returns whether the network stack is currently locked enabled(TRUE) for
  the specified device or disabled (FALSE) for the specified device(s).

  This function will scan and report the status of disabled devices.
  If user wants to know the status of a specific on the specific device, use DeviceHandle to specify it.

  For instance -
  Device Status:
    Device A -	Enabled
    Device B -	Disabled
    Device C -	Enabled

  Reported Status:
    DeviceHandle A is specified -  TRUE is returned.
    DeviceHandle B is specified -  FALSE is returned.
    DeviceHandle C is specified -  TRUE is returned.
    If not specified (NULL), then this scenario will report TRUE (because Device A and  Device C are enabled).

  @param[in]  DeviceHandle    Handle that specifies the device to enable or disable.
                              If NULL, then it will report whether any device is enabled.

  @retval TRUE                Device(s) is enabled.
  @retval FALSE               Device(s) is disabled.

**/
BOOLEAN
EFIAPI
NetworkConfigIsEnabled (
  IN    EFI_HANDLE                      DeviceHandle OPTIONAL
  )
{
  UINTN                                 Index;
  COMMON_INSTANCE                       *DeviceCommonInstance;

  if (gDriverInstance->BindedCount == 0) {
    return FALSE;
  }

  for (Index = 0; Index < gDriverInstance->BindedCount; Index++) {
    if (gDriverInstance->BindedList[Index] == NULL) {
      continue;
    }
    DeviceCommonInstance = gDriverInstance->BindedList[Index];
    if (DeviceHandle != NULL) {
      //
      // User select the specific network device by DeviceHandle.
      //
      if (DeviceCommonInstance->UndiHandle != DeviceHandle) {
        continue;
      }
      //
      // Return the device connected status.
      //
      return DeviceCommonInstance->DeviceConnected;
    }

    if (!DeviceCommonInstance->DeviceConnected) {
      continue;
    }
    return TRUE;
  }
  return FALSE;
}

/**
  This function will enable the device and bind the network protocols to the device.
  If user wants to specify specific devices, use DeviceHandle.

  @param[in]  DeviceHandle    Handle that specifies the device to enable or disable.
                              If NULL, then it will enable all devices.

  @retval EFI_SUCCESS           This function completed successfully.
  @retval EFI_DEVICE_ERROR      One or more network devices reported an error when enabled.
  @retval EFI_ALREADY_STARTED   The network stack was already enabled and cannot be disabled.
  @retval EFI_UNSUPPORTED       The network stack does not support configuration.

**/
EFI_STATUS
EFIAPI
NetworkConfigEnable (
  EFI_HANDLE                           DeviceHandle OPTIONAL
  )
{
  UINTN                                 Index;
  COMMON_INSTANCE                       *DeviceCommonInstance;

  if (gDriverInstance->BindedCount == 0) {
    return EFI_SUCCESS;
  }

  for (Index = 0; Index < gDriverInstance->BindedCount; Index++) {
    //
    // If device is already binded, skip it.
    //
    if (gDriverInstance->BindedList[Index] == NULL) {
      continue;
    }

    //
    // Find the specific device if user selected.
    //
    DeviceCommonInstance = gDriverInstance->BindedList[Index];
    if (DeviceHandle != NULL) {
      if (DeviceCommonInstance->UndiHandle != DeviceHandle) {
        continue;
      }
    }

    ConnectDevice (DeviceCommonInstance);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PxeDummyUnload (
  IN EFI_HANDLE                         ImageHandle
  )
{
  EFI_STATUS                            Status;

  if (gDriverInstance == NULL) {
    return EFI_SUCCESS;
  }

  if (ImageHandle != gDriverInstance->ImageHandle) {
    return EFI_INVALID_PARAMETER;
  }

  gBS->UninstallMultipleProtocolInterfaces (
         ImageHandle,
         &gEfiDhcp4ProtocolGuid,
         (VOID **) &gDhcp4Dummy,
         &gEfiDhcp6ProtocolGuid,
         (VOID **) &gDhcp6Dummy,
         NULL,
         NULL
         );

  gNetworkLocker->NetworkStackCallback = gOldNetworkLockerCallback;
  Status = NetLibDefaultUnload (ImageHandle);

  if (gDriverInstance->BindedList != NULL) {
    FreePool (gDriverInstance->BindedList);
    gDriverInstance->BindedList = NULL;
  }

  if (gDriverInstance->ReadyToBootEventReleaseNetworkLocker != NULL) {
    gBS->CloseEvent (gDriverInstance->ReadyToBootEventReleaseNetworkLocker);
  }

  if (gDriverInstance != NULL) {
    FreePool (gDriverInstance);
    gDriverInstance = NULL;
  }
  return Status;
}

/**
  The Dummy PXE will hook network locker protocol to control network stack.

**/
EFI_STATUS
EFIAPI
PxeDummyEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                   Status;

  Status = gBS->LocateProtocol (
                  &gNetworkLockerProtocolGuid,
                  NULL,
                  (VOID **) &gNetworkLocker
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // If MediaChanged, VLAN configuration is exist.
  //
  gDriverInstance = NULL;
  if (CheckVlanConfigurationVariableExist ()) {
    return EFI_SUCCESS;
  }

  gDriverInstance = AllocateZeroPool (sizeof (DRIVER_INSTANCE));
  if (gDriverInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  if (!EFI_ERROR (gNetworkLocker->NetworkStackCallback (NetworkStackPxe4Callback))) {
    gDriverInstance->IsPxeIpv4Enabled = TRUE;
  }
  if (!EFI_ERROR (gNetworkLocker->NetworkStackCallback (NetworkStackPxe6Callback))) {
    gDriverInstance->IsPxeIpv6Enabled = TRUE;
  }
  if (!EFI_ERROR (gNetworkLocker->NetworkStackCallback (NetworkStackIp4Callback))) {
    gDriverInstance->IsIpv4Enabled = TRUE;
  }
  if (!EFI_ERROR (gNetworkLocker->NetworkStackCallback (NetworkStackIp6Callback))) {
    gDriverInstance->IsIpv6Enabled = TRUE;
  }
  if (!EFI_ERROR (gNetworkLocker->NetworkStackCallback (NetworkStackHttpBoot4Callback))) {
    gDriverInstance->IsHttpIpv4Enabled = TRUE;
  }
  if (!EFI_ERROR (gNetworkLocker->NetworkStackCallback (NetworkStackHttpBoot6Callback))) {
    gDriverInstance->IsHttpIpv6Enabled = TRUE;
  }

  gDriverInstance->Signature                           = DUMMY_PXE_DRIVER_INSTANCE_SIGNATURE;
  gOldNetworkLockerCallback                            = gNetworkLocker->NetworkStackCallback;
  gDriverInstance->ImageHandle                         = ImageHandle;
  gDriverInstance->BindingProtocol.DriverBindingHandle = ImageHandle;
  gDriverInstance->BindingProtocol.ImageHandle         = ImageHandle;
  gDriverInstance->BindingProtocol.Supported           = DummyPxeDriverBindingSupported;
  gDriverInstance->BindingProtocol.Start               = DummyPxeDriverBindingStart;
  gDriverInstance->BindingProtocol.Stop                = DummyPxeDriverBindingStop;
  /// This version should be higher then PxeDummyDxe's version in binding protocol.
  gDriverInstance->BindingProtocol.Version             = 0xAA;

  gDriverInstance->NetworkConfig.IsEnabled             = NetworkConfigIsEnabled;
  gDriverInstance->NetworkConfig.Enable                = NetworkConfigEnable;

  gOldNewPackFunction                                  = NULL;
  gDeviceConnecting                                    = FALSE;

  // Install Fake DHCP4 and DHCP6 protocol.
  gBS->InstallMultipleProtocolInterfaces (
         &ImageHandle,
         &gEfiDhcp4ProtocolGuid,
         (VOID **) &gDhcp4Dummy,
         &gEfiDhcp6ProtocolGuid,
         (VOID **) &gDhcp6Dummy,
         &gH2ONetworkConfigProtocolGuid,
         (VOID **) &gDriverInstance->NetworkConfig,
         NULL,
         NULL
         );

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gEfiDriverBindingProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &(gDriverInstance->BindingProtocol)
                  );
  if (EFI_ERROR (Status)) {
    gBS->FreePool (gDriverInstance);
    return Status;
  }
  gNetworkLocker->NetworkStackCallback = PxeNetworkStackCallback;

  return Status;
}
