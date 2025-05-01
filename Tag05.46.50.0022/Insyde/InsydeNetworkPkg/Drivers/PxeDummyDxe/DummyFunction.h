//;******************************************************************************
//;* Copyright (c) 1983-2019, Insyde Software Corp. All Rights Reserved.
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


#ifndef _DUMMY_FUNCTION_H_
#define _DUMMY_FUNCTION_H_

EFI_STATUS
PxeNetworkStackCallback (
  NETWORK_STACK_CALLBACK_IDENTIFIER    Identifier
  );

//
// For LoadFile Protocol
//
EFI_STATUS
EFIAPI
FakePxeLoadFile (
  IN     EFI_LOAD_FILE_PROTOCOL        *This,
  IN     EFI_DEVICE_PATH_PROTOCOL      *FilePath,
  IN     BOOLEAN                       BootPolicy,
  IN OUT UINTN                         *BufferSize,
  IN     VOID                          *Buffer       OPTIONAL
  );

EFI_STATUS
EFIAPI
FakeHttpLoadFile (
  IN     EFI_LOAD_FILE_PROTOCOL        *This,
  IN     EFI_DEVICE_PATH_PROTOCOL      *CallerDevicePath,
  IN     BOOLEAN                       BootPolicy,
  IN OUT UINTN                         *BufferSize,
  IN     VOID                          *Buffer       OPTIONAL
  );

//
// For Ip4Config protocol
//
EFI_STATUS
FakeIp4ConfigStart (
  IN EFI_IP4_CONFIG_PROTOCOL           *This,
  IN EFI_EVENT                         DoneEvent,
  IN EFI_EVENT                         ReconfigEvent
  );

EFI_STATUS
FakeIp4ConfigStop(
  IN EFI_IP4_CONFIG_PROTOCOL           *This
  );

EFI_STATUS
FakeIp4ConfigGetData (
  IN EFI_IP4_CONFIG_PROTOCOL           *This,
  IN OUT UINTN                         *IpConfigDataSize,
  OUT EFI_IP4_IPCONFIG_DATA            *IpConfigData    OPTIONAL
  );

//
// For Ip6Config protocol
//
EFI_STATUS
FakeIp6ConfigSetData (
  IN EFI_IP6_CONFIG_PROTOCOL    *This,
  IN EFI_IP6_CONFIG_DATA_TYPE   DataType,
  IN UINTN                      DataSize,
  IN VOID                       *Data
  );

EFI_STATUS
FakeIp6ConfigGetData (
  IN EFI_IP6_CONFIG_PROTOCOL    *This,
  IN EFI_IP6_CONFIG_DATA_TYPE   DataType,
  IN OUT UINTN                  *DataSize,
  IN VOID                       *Data   OPTIONAL
  );

EFI_STATUS
FakeIp6ConfigRegisterDataNotify (
  IN EFI_IP6_CONFIG_PROTOCOL    *This,
  IN EFI_IP6_CONFIG_DATA_TYPE   DataType,
  IN EFI_EVENT                  Event
  );

EFI_STATUS
FakeIp6ConfigUnregisterDataNotify(
  IN EFI_IP6_CONFIG_PROTOCOL    *This,
  IN EFI_IP6_CONFIG_DATA_TYPE   DataType,
  IN EFI_EVENT                  Event
  );

//
// For Ip4Config2 protocol
//
EFI_STATUS
FakeIp4Config2SetData (
  IN EFI_IP4_CONFIG2_PROTOCOL   *This,
  IN EFI_IP4_CONFIG2_DATA_TYPE  DataType,
  IN UINTN                      DataSize,
  IN VOID                       *Data
  );

EFI_STATUS
FakeIp4Config2GetData (
  IN EFI_IP4_CONFIG2_PROTOCOL   *This,
  IN EFI_IP4_CONFIG2_DATA_TYPE  DataType,
  IN OUT UINTN                  *DataSize,
  IN VOID                       *Data   OPTIONAL
  );

EFI_STATUS
FakeIp4Config2RegisterDataNotify (
  IN EFI_IP4_CONFIG2_PROTOCOL   *This,
  IN EFI_IP4_CONFIG2_DATA_TYPE  DataType,
  IN EFI_EVENT                  Event
  );

EFI_STATUS
FakeIp4Config2UnregisterDataNotify(
  IN EFI_IP4_CONFIG2_PROTOCOL   *This,
  IN EFI_IP4_CONFIG2_DATA_TYPE  DataType,
  IN EFI_EVENT                  Event
  );

//
// For Ip4 protocol
//
EFI_STATUS
FakeIp4GetModeData (
  IN CONST  EFI_IP4_PROTOCOL                *This,
  OUT       EFI_IP4_MODE_DATA               *Ip4ModeData     OPTIONAL,
  OUT       EFI_MANAGED_NETWORK_CONFIG_DATA *MnpConfigData   OPTIONAL,
  OUT       EFI_SIMPLE_NETWORK_MODE         *SnpModeData     OPTIONAL
  );

EFI_STATUS
FakeIp4Configure (
  IN EFI_IP4_PROTOCOL    *This,
  IN EFI_IP4_CONFIG_DATA *IpConfigData     OPTIONAL
  );

EFI_STATUS
FakeIp4Groups (
  IN EFI_IP4_PROTOCOL    *This,
  IN BOOLEAN             JoinFlag,
  IN EFI_IPv4_ADDRESS    *GroupAddress  OPTIONAL
  );

EFI_STATUS
FakeIp4Routes (
  IN EFI_IP4_PROTOCOL    *This,
  IN BOOLEAN             DeleteRoute,
  IN EFI_IPv4_ADDRESS    *SubnetAddress,
  IN EFI_IPv4_ADDRESS    *SubnetMask,
  IN EFI_IPv4_ADDRESS    *GatewayAddress
  );

EFI_STATUS
FakeIp4Transmit (
  IN EFI_IP4_PROTOCOL          *This,
  IN EFI_IP4_COMPLETION_TOKEN  *Token
  );

EFI_STATUS
FakeIp4Receive (
  IN EFI_IP4_PROTOCOL          *This,
  IN EFI_IP4_COMPLETION_TOKEN  *Token
  );

EFI_STATUS
FakeIp4Cancel (
  IN EFI_IP4_PROTOCOL          *This,
  IN EFI_IP4_COMPLETION_TOKEN  *Token OPTIONAL
  );

EFI_STATUS
FakeIp4Poll (
  IN EFI_IP4_PROTOCOL          *This
  );

//
// For Ip6 protocol
//
EFI_STATUS
FakeIp6GetModeData (
  IN EFI_IP6_PROTOCOL                 *This,
  OUT EFI_IP6_MODE_DATA               *Ip6ModeData     OPTIONAL,
  OUT EFI_MANAGED_NETWORK_CONFIG_DATA *MnpConfigData   OPTIONAL,
  OUT EFI_SIMPLE_NETWORK_MODE         *SnpModeData     OPTIONAL
  );

EFI_STATUS
FakeIp6Configure (
  IN EFI_IP6_PROTOCOL            *This,
  IN EFI_IP6_CONFIG_DATA         *Ip6ConfigData OPTIONAL
  );

EFI_STATUS
FakeIp6Groups (
  IN EFI_IP6_PROTOCOL            *This,
  IN BOOLEAN                     JoinFlag,
  IN EFI_IPv6_ADDRESS            *GroupAddress  OPTIONAL
  );

EFI_STATUS
FakeIp6Routes (
  IN EFI_IP6_PROTOCOL            *This,
  IN BOOLEAN                     DeleteRoute,
  IN EFI_IPv6_ADDRESS            *Destination OPTIONAL,
  IN UINT8                       PrefixLength,
  IN EFI_IPv6_ADDRESS            *GatewayAddress OPTIONAL
  );

EFI_STATUS
FakeIp6Neighbors (
  IN EFI_IP6_PROTOCOL            *This,
  IN BOOLEAN                     DeleteFlag,
  IN EFI_IPv6_ADDRESS            *TargetIp6Address,
  IN EFI_MAC_ADDRESS             *TargetLinkAddress,
  IN UINT32                      Timeout,
  IN BOOLEAN                     Override
  );

EFI_STATUS
FakeIp6Transmit (
  IN EFI_IP6_PROTOCOL            *This,
  IN EFI_IP6_COMPLETION_TOKEN    *Token
  );

EFI_STATUS
FakeIp6Receive (
  IN EFI_IP6_PROTOCOL            *This,
  IN EFI_IP6_COMPLETION_TOKEN    *Token
  );

EFI_STATUS
FakeIp6Cancel (
  IN EFI_IP6_PROTOCOL            *This,
  IN EFI_IP6_COMPLETION_TOKEN    *Token    OPTIONAL
  );

EFI_STATUS
FakeIp6Poll (
  IN EFI_IP6_PROTOCOL            *This
  );

//
// For Ip4 ServiceBinding protocol
//
EFI_STATUS
FakeIp4CreateChild (
  IN     EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                    *ChildHandle
  );

EFI_STATUS
FakeIp4DestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL          *This,
  IN EFI_HANDLE                            ChildHandle
  );

//
// For Ip6 ServiceBinding protocol
//
EFI_STATUS
FakeIp6CreateChild (
  IN     EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                    *ChildHandle
  );

EFI_STATUS
FakeIp6DestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL          *This,
  IN EFI_HANDLE                            ChildHandle
  );

//
// For DHCP 4 Service binding
//
EFI_STATUS
FakeDhcp4CreateChild (
  IN     EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                    *ChildHandle
  );

EFI_STATUS
FakeDhcp4DestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL          *This,
  IN EFI_HANDLE                            ChildHandle
  );

//
// For DHCP 6 Service binding
//
EFI_STATUS
FakeDhcp6CreateChild (
  IN     EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                    *ChildHandle
  );

EFI_STATUS
FakeDhcp6DestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL          *This,
  IN EFI_HANDLE                            ChildHandle
  );

//
// For MNP ServiceBinding protocol
//
EFI_STATUS
FakeMnpDestroyChild (
  IN EFI_SERVICE_BINDING_PROTOCOL          *This,
  IN EFI_HANDLE                            ChildHandle
  );

EFI_STATUS
FakeMnpCreateChild (
  IN     EFI_SERVICE_BINDING_PROTOCOL  *This,
  IN OUT EFI_HANDLE                    *ChildHandle
  );

EFI_STATUS
EFIAPI
Ip4DeviceExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  );

EFI_STATUS
EFIAPI
Ip4DeviceRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  );

EFI_STATUS
EFIAPI
Ip4FormCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  );

EFI_STATUS
FakeHiiDatabaseNewList (
  IN CONST  EFI_HII_DATABASE_PROTOCOL   *This,
  IN CONST  EFI_HII_PACKAGE_LIST_HEADER *PackageList,
  IN        EFI_HANDLE                  DriverHandle, OPTIONAL
  OUT       EFI_HII_HANDLE               *Handle
  );

EFI_STATUS
PxeDummyDxeVlanConfigSet (
  IN  EFI_VLAN_CONFIG_PROTOCOL     *This,
  IN  UINT16                       VlanId,
  IN  UINT8                        Priority
  );

EFI_STATUS
PxeDummyDxeVlanConfigFind (
  IN  EFI_VLAN_CONFIG_PROTOCOL     *This,
  IN  UINT16                       *VlanId  OPTIONAL,
  OUT UINT16                       *NumberOfVlan,
  OUT EFI_VLAN_FIND_DATA           **Entries
  );

EFI_STATUS
PxeDummyDxeVlanConfigRemove (
  IN  EFI_VLAN_CONFIG_PROTOCOL     *This,
  IN  UINT16                       VlanId
  );

EFI_STATUS
ConnectDevice (
  IN   COMMON_INSTANCE                 *Instance
  );

extern BOOLEAN                         gDeviceConnecting;
extern EFI_HII_DATABASE_NEW_PACK       gOldNewPackFunction;
extern EFI_HII_DATABASE_PROTOCOL       *gHiiDataBaseProtocol;

extern EFI_DHCP4_PROTOCOL gDhcp4Dummy;
extern EFI_DHCP6_PROTOCOL gDhcp6Dummy;

#endif //_DUMMY_FUNCTION_H_
