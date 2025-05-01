/** @file
  Header file of SataDriveInfoSetupUtilityDxe.

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

#ifndef __SATA_DRIVE_INFO_SETUP_UTITLITY_DXE_H__
#define __SATA_DRIVE_INFO_SETUP_UTITLITY_DXE_H__

#include <Uefi.h>

#include <Protocol/AtaPassThru.h>
#include <Protocol/DevicePath.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiConfigRouting.h>

#include <Protocol/H2OSataDriveInfo.h>
#include <Protocol/H2ODeviceInfoLookup.h>
#include <Protocol/H2OPciLookupTable.h>

#include "SataDriveInfoSetupNVDataStruc.h"

typedef enum {
  ByControllers   = 0,
  ByPhysicalPorts
} DISPLAY_MODE;

typedef struct {
  UINT32                            Signature;
  EFI_HANDLE                        DriverHandle;
  EFI_HII_HANDLE                    HiiHandle;
  H2O_SATA_DRIVE_INFO_PROTOCOL      *SataDriveInfo;
  H2O_PCI_LOOKUP_TABLE_PROTOCOL     *PciLookupTable;
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL   *DeviceInfoLookup;
  EFI_HII_CONFIG_ROUTING_PROTOCOL   *HiiConfigRouting;
  EFI_HII_CONFIG_ACCESS_PROTOCOL    ConfigAccess;
  SATA_DRIVE_INFO_CONFIGURATION     Configuration;
} SATA_DRIVE_INFO_SETUP_PRIVATE_DATA;

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH        VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

#define SATA_DRIVE_INFO_SETUP_SIGNATURE       SIGNATURE_32 ('S', 'D', 'I', 'S')
#define SATA_DRIVE_INFO_PRIVATE_FROM_THIS(a)  \
  CR (a, SATA_DRIVE_INFO_SETUP_PRIVATE_DATA, ConfigAccess, SATA_DRIVE_INFO_SETUP_SIGNATURE)

#endif