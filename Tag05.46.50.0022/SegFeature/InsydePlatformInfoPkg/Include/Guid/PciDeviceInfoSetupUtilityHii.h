/** @file
 This file stores Setup Fromset GUID of the PciDeviceInfoSetupUtilityDxe

 Note that the GUID MUST be the same as gPciDeviceInfoSetupFormsetGuid in
 InsydePlatformInfoPkg.dec

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef __PCI_DEVICE_INFO_SETUP_UTILITY_HII_H__
#define __PCI_DEVICE_INFO_SETUP_UTILITY_HII_H__

// {4900baed-e370-4663-8105-bf9412d5ca57}
#define PCI_DEVICE_INFO_SETUP_FORMSET_GUID \
  { \
    0x4900baed, 0xe370, 0x4663,  {0x81, 0x05, 0xbf, 0x94, 0x12, 0xd5, 0xca, 0x57 } \
  }

// {3c632ccc-a783-4171-ae20-58fb855c33b2}
#define PCI_DEVICE_INFO_CONFIGURATION_GUID \
  { \
    0x3c632ccc, 0xa783, 0x4171, { 0xae, 0x20, 0x58, 0xfb, 0x85, 0x5c, 0x33, 0xb2 } \
  }

#define PCI_DEVICE_INFO_CONFIGURATION_VARIABLE_NAME   L"PciDeviceInfoSetupConfig"

//
// Varstore Id
//
#define PCI_DEVICE_INFO_CONFIGURATION_VARSTORE_ID     0x4000

//
// Root Form Id
//
#define H2O_PCI_DEVICE_INFO_ROOT_FORM_ID              0x2000


#pragma pack(1)
typedef struct {
  UINT8   ShowActualDeviceName;
  UINT8   ShowDeviceTree;
  UINT8   FilteringMode;
  UINT8   DisplayCapabilityIntpn;
  UINT8   FormRefresh;
} PCI_DEVICE_INFO_CONFIGURATION;
#pragma pack()


extern EFI_GUID gH2OPciDeviceInfoSetupFormsetGuid;
extern EFI_GUID gH2OPciDeviceInfoConfigurationGuid;

#endif
