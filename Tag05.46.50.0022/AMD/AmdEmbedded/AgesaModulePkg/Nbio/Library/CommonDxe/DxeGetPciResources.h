/******************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/
#ifndef __DXE_GET_PCI_RESOURCES_H__
#define __DXE_GET_PCI_RESOURCES_H__

#include <Filecode.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <GnbDxio.h>
#include <Library/PcieConfigLib.h>
#include <Library/NbioCommonLibDxe.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NbioHandleLib.h>
#include <Protocol/AmdPciResourcesProtocol.h>
#include <GnbRegistersRS.h>
#include <Library/SmnAccessLib.h>
#include <Library/GnbPciAccLib.h>
#include <Library/GnbPciLib.h>

#define NBIO_MAX_FIXED_RESOURCES 2

#define PCI_ROOT_BRIDGE_OBJECT_INSTANCE_SIGNATURE SIGNATURE_32 ('a', 'p', 'r', 'b')
typedef struct {
  UINTN                    Signature;
  PCI_ROOT_BRIDGE_OBJECT   RootBridgeObject;
  LIST_ENTRY               *RootPortList;
  UINTN                    NumberOfRootPorts;
  LIST_ENTRY               *FixedResourcesList;
  UINTN                    NumberOfFixedResources;
  LIST_ENTRY               Link;
} PCI_ROOT_BRIDGE_OBJECT_INSTANCE;

#define PCI_ROOT_BRIDGE_INSTANCE_FROM_LINK(a) \
        CR (a, PCI_ROOT_BRIDGE_OBJECT_INSTANCE, Link, \
        PCI_ROOT_BRIDGE_OBJECT_INSTANCE_SIGNATURE)

#define PCI_ROOT_PORT_OBJECT_INSTANCE_SIGNATURE SIGNATURE_32 ('a', 'p', 'r', 'p')
typedef struct {
  UINTN                 Signature;
  PCI_ROOT_PORT_OBJECT  RootPortObject;
  LIST_ENTRY            Link;
} PCI_ROOT_PORT_OBJECT_INSTANCE;

#define PCI_ROOT_PORT_INSTANCE_FROM_LINK(a) \
        CR (a, PCI_ROOT_PORT_OBJECT_INSTANCE, Link, \
        PCI_ROOT_PORT_OBJECT_INSTANCE_SIGNATURE)

#define PCI_FIXED_RESOURCES_OBJECT_INSTANCE_SIGNATURE SIGNATURE_32 ('a', 'p', 'r', 'r')
typedef struct {
  UINTN        Signature;
  FIXED_RESOURCES_OBJECT FixedResourceObject;
  LIST_ENTRY   Link;
} FIXED_RESOURCES_OBJECT_INSTANCE;

#define PCI_FIXED_RESOURCE_INSTANCE_FROM_LINK(a) \
        CR (a, FIXED_RESOURCES_OBJECT_INSTANCE, Link, \
        PCI_FIXED_RESOURCES_OBJECT_INSTANCE_SIGNATURE)

EFI_STATUS
EFIAPI
AmdCollectPciResourcesInit (
     IN       PCIe_PLATFORM_CONFIG         *Pcie
);

EFI_STATUS
EFIAPI
AmdPciResourcesGetNumberOfRootBridges (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    OUT      UINTN                                 *NumberOfRootBridges
);

EFI_STATUS
EFIAPI
AmdPciResourcesGetRootBridgeInfo (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    IN       UINTN                                 RootBridgeIndex,
    OUT      PCI_ROOT_BRIDGE_OBJECT                *RootBridgeInfo
);

EFI_STATUS
EFIAPI
AmdPciResourcesGetNumberOfRootPorts (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    IN       UINTN                                 RootBridgeIndex,
    OUT      UINTN                                 *NumberOfRootPorts
);

EFI_STATUS
EFIAPI
AmdPciResourcesGetRootPortInfo (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    IN       UINTN                                 RootBridgeIndex,
    IN       UINTN                                 RootPortIndex,
    OUT      PCI_ROOT_PORT_OBJECT                  *RootPortInfo
);

EFI_STATUS
EFIAPI
AmdPciResourcesGetNumberOfFixedResources (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    IN       UINTN                                 RootBridgeIndex,
    OUT      UINTN                                 *NumberOfFixedResources
);

EFI_STATUS
EFIAPI
AmdPciResourcesGetFixedResourceInfo (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    IN       UINTN                                 RootBridgeIndex,
    IN       UINTN                                 FixedResourceIndex,
    OUT      FIXED_RESOURCES_OBJECT                *FixedResourceInfo
);

EFI_STATUS
STATIC
CollectRootPortInfo (
  IN      GNB_HANDLE                      *GnbHandle,
  IN OUT  PCI_ROOT_BRIDGE_OBJECT_INSTANCE *RootBridge
);

VOID
STATIC
CollectInterruptInfo (
  IN      GNB_HANDLE                      *GnbHandle,
  IN OUT  PCI_ROOT_PORT_OBJECT_INSTANCE   *RootPort,
  IN      PCIe_ENGINE_CONFIG              *PcieEngine
);

EFI_STATUS
STATIC
CollectFixedResourcesInfo (
  IN      GNB_HANDLE                      *GnbHandle,
  IN OUT  PCI_ROOT_BRIDGE_OBJECT_INSTANCE *RootBridge
);

#endif
