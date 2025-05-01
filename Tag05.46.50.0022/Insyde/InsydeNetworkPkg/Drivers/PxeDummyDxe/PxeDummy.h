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
//;   PxeDummy.h
//;
//; Abstract:
//;
//;    Refer PxeDummy.c
//;


#ifndef _PXE_DUMMY_H_
#define _PXE_DUMMY_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/ServiceBinding.h>

#include <Protocol/DevicePath.h>
#include <Protocol/NetworkLocker.h>
#include <Protocol/LoadFile.h>
#include <Protocol/Ip4.h>
#include <Protocol/Dhcp4.h>
#include <Protocol/Ip4Config.h>
#include <Protocol/Ip4Config2.h>
#include <Protocol/Ip6.h>
#include <Protocol/Dhcp6.h>
#include <Protocol/Ip6Config.h>
#include <Protocol/NetworkInterfaceIdentifier.h>
#include <Protocol/PxeBaseCode.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/VlanConfig.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiConfigAccess.h>

#include <Protocol/H2ONetworkConfig.h>

#include <Uefi/UefiPxe.h>

#include <Guid/NetworkEventGroup.h>
#include <Guid/EventGroup.h>

#define VARIAB_NAME_BUFFER_LENGTH      100

typedef struct _COMMON_INSTANCE COMMON_INSTANCE;

enum _PXE_CAPABILITY {
  PXE_NONE,
  PXE_IPv4,
  PXE_IPv6,
  PXE_MNP,
  HTTP_IPv4,
  HTTP_IPv6,
  PXE_MAX
};

typedef struct _FAKE_VLAN_CONFIG_INSATNCE {
  EFI_VLAN_CONFIG_PROTOCOL             VlanConfig;
  EFI_VLAN_CONFIG_PROTOCOL             *RealVlanConfig;
  COMMON_INSTANCE                      *Instance;
} FAKE_VLAN_CONFIG_INSATNCE;

typedef struct _FAKE_MNP_SB_INSATNCE {
  BOOLEAN                              Enabled;
  EFI_SERVICE_BINDING_PROTOCOL         MnpSB;
  EFI_SERVICE_BINDING_PROTOCOL         *RealMnpSB;
  COMMON_INSTANCE                      *Instance;
  EFI_EVENT                            ReadyToBootEvent;
  VENDOR_DEVICE_PATH                   Ip4VendorDeviceNode;
  EFI_DEVICE_PATH_PROTOCOL             *Ip4HiiVendorDevicePath;
  EFI_HANDLE                           Ip4ChildHandle;
  EFI_HII_CONFIG_ACCESS_PROTOCOL       Ip4HiiConfigAccess;
  EFI_SERVICE_BINDING_PROTOCOL         *MnpSb;
} FAKE_MNP_SB_INSATNCE;

typedef struct _FAKE_IP_IP4_INSATNCE {
  EFI_IP4_PROTOCOL                     Ip4;
  EFI_IP4_PROTOCOL                     *RealIp4;
  COMMON_INSTANCE                      *Instance;
} FAKE_IP_IP4_INSATNCE;

typedef struct _FAKE_IP_IP4_SB_INSATNCE {
  EFI_SERVICE_BINDING_PROTOCOL         Ip4SB;
  EFI_SERVICE_BINDING_PROTOCOL         *RealIp4SB;
  COMMON_INSTANCE                      *Instance;
} FAKE_IP_IP4_SB_INSATNCE;

typedef struct _FAKE_IP_DHCP4_SB_INSATNCE {
  EFI_SERVICE_BINDING_PROTOCOL         Dhcp4SB;
  EFI_SERVICE_BINDING_PROTOCOL         *RealDhcp4SB;
  COMMON_INSTANCE                      *Instance;
} FAKE_IP_DHCP4_SB_INSATNCE;


typedef struct _FAKE_IP_IP4_CONFIG_INSATNCE {
  EFI_IP4_CONFIG_PROTOCOL              Ip4Config;
  EFI_IP4_CONFIG_PROTOCOL              *RealIp4Config;
  COMMON_INSTANCE                      *Instance;
} FAKE_IP_IP4_CONFIG_INSATNCE;

typedef struct _FAKE_IP_IP4_CONFIG2_INSATNCE {
  EFI_IP4_CONFIG2_PROTOCOL             Ip4Config2;
  EFI_IP4_CONFIG2_PROTOCOL             *RealIp4Config2;
  COMMON_INSTANCE                      *Instance;
} FAKE_IP_IP4_CONFIG2_INSATNCE;

typedef struct _FAKE_IP_IP6_INSATNCE {
  EFI_IP6_PROTOCOL                     Ip6;
  EFI_IP6_PROTOCOL                     *RealIp6;
  COMMON_INSTANCE                      *Instance;
} FAKE_IP_IP6_INSATNCE;

typedef struct _FAKE_IP_IP6_SB_INSATNCE {
  EFI_SERVICE_BINDING_PROTOCOL         Ip6SB;
  EFI_SERVICE_BINDING_PROTOCOL         *RealIp6SB;
  COMMON_INSTANCE                      *Instance;
} FAKE_IP_IP6_SB_INSATNCE;

typedef struct _FAKE_IP_DHCP6_SB_INSATNCE {
  EFI_SERVICE_BINDING_PROTOCOL         Dhcp6SB;
  EFI_SERVICE_BINDING_PROTOCOL         *RealDhcp6SB;
  COMMON_INSTANCE                      *Instance;
} FAKE_IP_DHCP6_SB_INSATNCE;

typedef struct _FAKE_IP_IP6_CONFIG_INSATNCE {
  EFI_IP6_CONFIG_PROTOCOL              Ip6Config;
  EFI_IP6_CONFIG_PROTOCOL              *RealIp6Config;
  COMMON_INSTANCE                      *Instance;
} FAKE_IP_IP6_CONFIG_INSATNCE;

typedef struct _FAKE_LOAD_FILE_INSTANCE {
  EFI_LOAD_FILE_PROTOCOL               LoadFile;
  UINTN                                PxeCapability;
  COMMON_INSTANCE                      *Instance;
  EFI_HANDLE                           FakePxeHandle;
  EFI_DEVICE_PATH_PROTOCOL             *FakePxeDevicePath;
  EFI_DEVICE_PATH_PROTOCOL             *RealPxeDevicePath;
  BOOLEAN                              Enable;
  BOOLEAN                              IsIpv4;
  BOOLEAN                              IsIpv6;
  BOOLEAN                              IsHttp;
  BOOLEAN                              IsPxe;
  EFI_HANDLE                           RealPxeHandle;
  EFI_LOAD_FILE_PROTOCOL               *RealEntry;
} FAKE_LOAD_FILE_INSTANCE;

//
// The alignment do not change, the DUMMY_DHCP_SIGNATURE maybe locate failed.
//
typedef struct _DUMMY_PXE_INSTANCE {
  FAKE_LOAD_FILE_INSTANCE              Ip4;
  FAKE_LOAD_FILE_INSTANCE              Ip6;
  FAKE_LOAD_FILE_INSTANCE              Http4;
  FAKE_LOAD_FILE_INSTANCE              Http6;
} DUMMY_PXE_INSTANCE;

typedef struct _DUMMY_IP_INSTANCE {
  FAKE_MNP_SB_INSATNCE                 MnpSB;
//  FAKE_IP_IP4_INSATNCE                 Ip4;
  FAKE_IP_IP4_SB_INSATNCE              Ip4SB;
  FAKE_IP_IP4_CONFIG_INSATNCE          Ip4Config;
  FAKE_IP_IP4_CONFIG2_INSATNCE         Ip4Config2;
  FAKE_IP_DHCP4_SB_INSATNCE            Dhcp4SB;
//  FAKE_IP_IP6_INSATNCE                 Ip6;
  FAKE_IP_IP6_SB_INSATNCE              Ip6SB;
  FAKE_IP_IP6_CONFIG_INSATNCE          Ip6Config;
  FAKE_IP_DHCP6_SB_INSATNCE            Dhcp6SB;

  FAKE_VLAN_CONFIG_INSATNCE            VlanConfig;
} DUMMY_IP_INSTANCE;

struct _COMMON_INSTANCE {
  UINT32                               Signature;
  EFI_HANDLE                           ImageHandle;
  EFI_HANDLE                           UndiHandle;
  DUMMY_PXE_INSTANCE                   Pxe;
  DUMMY_IP_INSTANCE                    Ip;
  EFI_DEVICE_PATH_PROTOCOL             *UndiDevicePath;
  VOID                                 *Interface;

  VOID                                 *RealMnpSb;

  //
  // Identifier dummy device connected or not.
  //
  BOOLEAN                              DeviceConnected;

  //
  // Network policy on device controller handle
  //
  NETWORK_LOCKER_PROTOCOL              *ControllerNetworkPolicy;
};

typedef struct _DRIVER_INSTANCE {
  UINT32                               Signature;
  EFI_DRIVER_BINDING_PROTOCOL          BindingProtocol;
  EFI_HANDLE                           ImageHandle;

  BOOLEAN                              IsPxeIpv4Enabled;
  BOOLEAN                              IsPxeIpv6Enabled;
  BOOLEAN                              IsIpv4Enabled;
  BOOLEAN                              IsIpv6Enabled;
  BOOLEAN                              IsHttpIpv4Enabled;
  BOOLEAN                              IsHttpIpv6Enabled;

  EFI_EVENT                            ReadyToBootEventReleaseNetworkLocker;

  //
  // Information for BindingProtocol.Stop
  //
  UINTN                                BindedCount;
  COMMON_INSTANCE                      **BindedList;

  NETWORK_LOCKER_PROTOCOL              *NetworkLocker;
  NETWORK_STACK_CALLBACK               OldNetworkLockerCallback;

  H2O_NETWORK_CONFIG_PROTOCOL          NetworkConfig;
} DRIVER_INSTANCE;

#define DUMMY_PXE_INSTANCE_SIGNATURE         SIGNATURE_32 ('D', 'P', 'X', 'E')
#define DUMMY_IP_INSTANCE_SIGNATURE          SIGNATURE_32 ('D', 'U', 'I', 'P')
#define DUMMY_PXE_DRIVER_INSTANCE_SIGNATURE  SIGNATURE_32 ('D', 'P', 'S', 'B')
#define DUMMY_PXE_INSTANCE_FROM_THIS(Sb)      \
  CR ((Sb), DRIVER_INSTANCE, BindingProtocol, DUMMY_PXE_DRIVER_INSTANCE_SIGNATURE)


extern NETWORK_LOCKER_PROTOCOL         *gNetworkLocker;
extern NETWORK_STACK_CALLBACK          gOldNetworkLockerCallback;
extern EFI_GUID                        gEfiIp6CallerIdGuid;
extern EFI_GUID                        gEfiNicIp4ConfigVariableGuid;
extern DRIVER_INSTANCE                 *gDriverInstance;

#endif //_PXE_DUMMY_H_
