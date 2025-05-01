/** @file
  NVMe Information

;******************************************************************************
;* Copyright (c) 2021 - 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef NVME_INFO_SETUP_UTILITY_DXE_H_
#define NVME_INFO_SETUP_UTILITY_DXE_H_

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

#include <IndustryStandard/Nvme.h>

#include <Protocol/DevicePath.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/H2ODeviceInfoLookup.h>     // InsydePlatformInfoPkg
#include <Protocol/H2ONvmeInfo.h>             // InsydePlatformInfoPkg

#include <Library/DebugLib.h>
#include <Library/HiiStringIdManageLib.h>     // InsydePlatformInfoPkg

//
// Defined in AutoGen.c
//
extern UINT8 NvmeInfoSetupUtilityDxeStrings[];

//
// This is the generated IFR binary data for each formset defined in VFR.
// This data array is ready to be used as input of HiiAddPackages() to
// create a packagelist (which contains Form packages, String packages, etc).
//
extern UINT8 NvmeInfoVfrBin[];

#define H2O_SETUP_NVME_INFO_PRIVATE_SIGNATURE  SIGNATURE_32 ('S', 'N', 'E', 'I')

typedef struct {
  UINT32                                   Signature;
  EFI_HII_CONFIG_ACCESS_PROTOCOL           ConfigAccess;
  H2O_NVME_INFO_PROTOCOL                   *NvmeInfo;
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL          *DeviceInfoLookup;
  EFI_EVENT                                SetupUtilityEvent;
  EFI_HANDLE                               SeviceHandle;
  EFI_HII_HANDLE                           HiiHandle;
  HII_STRING_ID_MANAGER                    HiiStringIdManager;
  H2O_NVME_INFO_CONTROLLER_ENTRY           *NvmeControllerEntries;
  H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  *DeviceInfoLookupNameTable;
  UINTN                                    NvmeControllerCount;
  UINTN                                    DeviceInfoLookupNameCount;
} H2O_SETUP_NVME_INFO_PRIVATE_DATA;

#define H2O_SETUP_NVME_INFO_PRIVATE_FROM_CONFIG_ACCESS(a) \
  CR (a, H2O_SETUP_NVME_INFO_PRIVATE_DATA, ConfigAccess, H2O_SETUP_NVME_INFO_PRIVATE_SIGNATURE)

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH        VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

typedef struct {
  UINTN  NvmeControllerEntryIndex;
  UINTN  ControllerNameIndex;
  UINTN  NamespaceNameIndex;
} H2O_SETUP_NVME_INFO_DEVICE_NAME_RELATION;

typedef struct {
  CONST H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  *DeviceInfoLookupNameTable;
  UINTN                                          DevicePathMaxSize;
} H2O_SETUP_NVME_INFO_NAME_SEARCH_CONTEXT;

VOID
CreateDevicePathTextFromControllerEntry (
  IN OUT H2O_SETUP_NVME_INFO_PRIVATE_DATA  *PrivateData,
  IN CONST H2O_NVME_INFO_CONTROLLER_ENTRY  *ControllerEntry,
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmePassThru OPTIONAL,
  OUT CHAR16                               **DevicePathText
  );

VOID
UpdatePciLocationHiiString (
  IN OUT H2O_SETUP_NVME_INFO_PRIVATE_DATA  *PrivateData,
  IN H2O_NVME_INFO_CONTROLLER_ENTRY        *ControllerEntry
  );

EFI_STATUS
EFIAPI
NvmeInfoExtractConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Request,
  OUT EFI_STRING                           *Progress,
  OUT EFI_STRING                           *Results
  );

EFI_STATUS
EFIAPI
NvmeInfoRouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN CONST EFI_STRING                      Configuration,
  OUT EFI_STRING                           *Progress
  );

EFI_STATUS
EFIAPI
NvmeInfoFormCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN EFI_BROWSER_ACTION                    Action,
  IN EFI_QUESTION_ID                       QuestionId,
  IN UINT8                                 Type,
  IN OUT EFI_IFR_TYPE_VALUE                *Value,
  OUT EFI_BROWSER_ACTION_REQUEST           *ActionRequest
  );

#endif // NVME_INFO_SETUP_UTILITY_DXE_H_
