/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _AMD_CPM_PCI_HOT_PLUG_INIT_DXE_H_
#define _AMD_CPM_PCI_HOT_PLUG_INIT_DXE_H_

#include <IndustryStandard/Acpi.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciHotPlugInit.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Library/DevicePathLib.h>

#define EFI_PCIHOTPLUG_DRIVER_PRIVATE_SIGNATURE SIGNATURE_32 ('G', 'L', 'U', 'P')

#define ACPI(NodeUID) \
  { \
    ACPI_DEVICE_PATH, ACPI_DP, (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)), (UINT8) \
      ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8), EISA_PNP_ID (0x0A03), NodeUID\
  }

#define PCI(device, function) \
  { \
    HARDWARE_DEVICE_PATH, HW_PCI_DP, (UINT8) (sizeof (PCI_DEVICE_PATH)), (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8), \
      (UINTN) function, (UINTN) device \
  }

#define END \
  { \
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, END_DEVICE_PATH_LENGTH, 0 \
  }

typedef struct {
  UINT8   NodeUID;
  UINT8   BusNum;
  UINT8   DevNum;
  UINT8   FuncNum;
  UINT8   ReservedBusCount;
  UINT16  ReservedIoRange;
  UINT64  ReservedNonPrefetchableMmio;
  UINT64  AlignemntOfReservedNonPrefetchableMmio;
  UINT64  ReservedPrefetchableMmio;
  UINT64  AlignemntOfReservedPrefetchableMmio;
} HOT_PLUG_BRIDGE_INFO_NEW;

typedef struct PCIE_HOT_PLUG_DEVICE_PATH {
  ACPI_HID_DEVICE_PATH      PciRootBridgeNode;
  PCI_DEVICE_PATH           PciRootPortNode;
  EFI_DEVICE_PATH_PROTOCOL  EndDeviceNode;
} PCIE_HOT_PLUG_DEVICE_PATH;

typedef struct PCIE_HOT_PLUG_DEVICE_PATH_SWUS {
  ACPI_HID_DEVICE_PATH      PciRootBridgeNode;
  PCI_DEVICE_PATH           PciRootPortNode;
  PCI_DEVICE_PATH           PciEpSwUsNode;
  EFI_DEVICE_PATH_PROTOCOL  EndDeviceNode;
} PCIE_HOT_PLUG_DEVICE_PATH_SWUS;

typedef struct PCIE_HOT_PLUG_DEVICE_PATH_SWDS {
  ACPI_HID_DEVICE_PATH      PciRootBridgeNode;
  PCI_DEVICE_PATH           PciRootPortNode;
  PCI_DEVICE_PATH           PciEpSwUsNode;
  PCI_DEVICE_PATH           PciEpSwDsNode;
  EFI_DEVICE_PATH_PROTOCOL  EndDeviceNode;
} PCIE_HOT_PLUG_DEVICE_PATH_SWDS;

typedef struct {
  UINTN                           Signature;
  EFI_HANDLE                      Handle; // Handle for protocol this driver installs on
  EFI_PCI_HOT_PLUG_INIT_PROTOCOL  HotPlugInitProtocol;
} PCI_HOT_PLUG_INSTANCE;

EFI_STATUS
EFIAPI
AmdCpmGetRootHpcList (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL    *This,
  OUT UINTN                             *PhpcCount,
  OUT EFI_HPC_LOCATION                  **PhpcList
  );

EFI_STATUS
EFIAPI
AmdCpmInitializeRootHpc (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL  *This,
  IN  EFI_DEVICE_PATH_PROTOCOL        *PhpcDevicePath,
  IN  UINT64                          PhpcPciAddress,
  IN  EFI_EVENT                       Event, OPTIONAL
  OUT EFI_HPC_STATE                   *PhpcState
  );

EFI_STATUS
EFIAPI
AmdCpmGetResourcePadding (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL  *This,
  IN  EFI_DEVICE_PATH_PROTOCOL        *PhpcDevicePath,
  IN  UINT64                          PhpcPciAddress,
  OUT EFI_HPC_STATE                   *PhpcState,
  OUT VOID                            **Padding,
  OUT EFI_HPC_PADDING_ATTRIBUTES      *Attributes
  );

#endif
