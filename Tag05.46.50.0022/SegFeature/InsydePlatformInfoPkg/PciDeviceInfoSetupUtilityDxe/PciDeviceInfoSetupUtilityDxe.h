/** @file
  Header file of PciDeviceInfoSetupUtilityDxe.

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef __PCI_DEVICE_INFO_SETUP_UTITLITY_DXE_H__
#define __PCI_DEVICE_INFO_SETUP_UTITLITY_DXE_H__

#include <Uefi.h>

#include <Protocol/DevicePath.h>
#include <Protocol/H2OPciDeviceInfo.h>
#include <Protocol/H2OPciLookupTable.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/H2ODeviceInfoLookup.h>

#include <Guid/PciDeviceInfoSetupUtilityHii.h>

typedef enum {
  AllDevice,
  PciDeviceOnly,
  PcieDeviceOnly
} FILTERING_MODE;

typedef struct {
  UINT32                            Signature;
  EFI_HANDLE                        DriverHandle;
  EFI_HII_HANDLE                    HiiHandle;
  H2O_PCI_DEVICE_INFO_PROTOCOL      *PciDeviceInfo;
  H2O_PCI_LOOKUP_TABLE_PROTOCOL     *PciLookupTable;
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL   *DeviceInfoLookup;
  EFI_HII_CONFIG_ROUTING_PROTOCOL   *HiiConfigRouting;
  EFI_HII_CONFIG_ACCESS_PROTOCOL    ConfigAccess;
  PCI_DEVICE_INFO_CONFIGURATION     Configuration;
} PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA;

typedef enum {
  CapabilityDisplayControlDefault       = 0,
  CapabilityDisplayControlHide          = 1,
  CapabilityDisplayControlHideIntpnOnly = 2
} CAPABILITY_DISPLAY_CONTROL;

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH        VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  End;
} HII_VENDOR_DEVICE_PATH;

typedef struct {
  UINT16  VendorId;
  UINT16  DeviceId;
} IGNORE_DEVICE_TABLE_ENTRY;

typedef struct {
  UINT8   IsPcieExtCapability;
  UINT16  CapabilityId;
  UINT8   DisplayControl;
} CAPABILITY_DISPLAY_CONTROL_TABLE_ENTRY;
#pragma pack()

#define PCI_DEVICE_INFO_SETUP_UTILITY_SIGNATURE SIGNATURE_32 ('p', 'c', 'i', 's')

#define PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA_FROM_THIS(a)  \
  CR (a, PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA, ConfigAccess, PCI_DEVICE_INFO_SETUP_UTILITY_SIGNATURE)

#endif