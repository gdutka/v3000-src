/** @file
  Memory Information Setup Dxe Header File

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/


#ifndef _MEMORY_INFO_SETUP_UTILITY_DXE_H_
#define _MEMORY_INFO_SETUP_UTILITY_DXE_H_

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>
#include <IndustryStandard/SmBios.h>

#include <Protocol/H2OMemoryDeviceInfo.h>
#include <Protocol/SetupUtilityApplication.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/Smbios.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HiiLib.h>
#include <Library/HiiExLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/VariableLib.h>
#include <Library/MemoryDeviceInfoLib.h>
#include <Library/LanguageDataLib.h>
#include <Library/DxeSetupVariableLib.h>

#include <Guid/MdeModuleHii.h>
#include <Guid/MemoryInfoSetupUtilityHii.h>

#include "MemoryInfoFormset.h"

//
// This is the generated IFR binary data for each formset defined in VFR.
// This data array is ready to be used as input of HiiAddPackages() to
// create a packagelist (which contains Form packages, String packages, etc).
//
extern UINT8 MemoryInfoVfrBin[];

#define MAX_STR_SIZE  0x100

/**
  Init the form data in the memory detail information form

  @param[in]  OpCodeHandle        The OpCodeHandle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
typedef
(EFIAPI *INIT_ITEM_FUNCTION) (
  IN    EFI_HII_HANDLE            OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

/**
  Update the form data in the memory detail information form

  @param[in]  HiiHandle           Hii Handle
  @param[in]  MemoryInfoIndex     Which memory info index to update.

  @retval EFI_SUCCESS             Update form data success
  @retval other                   Update form data fail
**/
EFI_STATUS
typedef
(EFIAPI *DISPLAY_ITEM_FUNCTION) (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     MemoryInfoIndex
  );

typedef struct {
  UINT32                             Signature;
  EFI_HANDLE                         SeviceHandle;
  EFI_HII_HANDLE                     HiiHandle;
  UINTN                              NumberOfMemoryInfo;
  UINTN                              NumberOfInstallMemory;
  UINTN                              NumberOfUnInstallMemory;
  MEMORY_INFO_SETUP                  Configuration;
  //
  // Produced protocols
  //
  EFI_HII_CONFIG_ACCESS_PROTOCOL     ConfigAccess;
  EFI_HII_CONFIG_ROUTING_PROTOCOL    *HiiConfigRouting;
} MEMORY_INFO_PRIVATE_DATA;

#define MEMORY_INFO_PRIVATE_SIGNATURE SIGNATURE_32 ('M', 'E', 'M', 'I')
#define MEMORY_INFO_PRIVATE_DATA_FROM_THIS(a)  \
  CR (a, MEMORY_INFO_PRIVATE_DATA, ConfigAccess, MEMORY_INFO_PRIVATE_SIGNATURE)

#pragma pack(1)
typedef struct {
  BOOLEAN                ItemDisplay;
  INIT_ITEM_FUNCTION     InitItemFunction;
  DISPLAY_ITEM_FUNCTION  DisplayItemFunction;
} MEMORY_DETAIL_DISPLAY_LIST;

typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

EFI_STATUS
InitDeviceLocatorInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
InitBankLocatorInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
InitManufacturerInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
InitMemoryTypeInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
InitDramTypeInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
InitSerialNumberInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
InitPartNumberInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
InitMemorySizeInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
InitMaximumCapableMemorySpeedInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
InitConfigureMemorySpeedInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
InitConfigureVoltageInfo (
  IN    VOID                      *OpCodeHandle,
  IN    EFI_QUESTION_ID           QuestionId
  );

EFI_STATUS
UpdateDeviceLocatorInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

EFI_STATUS
UpdateBankLocatorInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

EFI_STATUS
UpdateManufacturerInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

EFI_STATUS
UpdateMemoryTypeInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

EFI_STATUS
UpdateDramTypeInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

EFI_STATUS
UpdateSerialNumberInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

EFI_STATUS
UpdatePartNumberInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

EFI_STATUS
UpdateMemorySizeInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

EFI_STATUS
UpdateMaximumCapableMemorySpeedInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

EFI_STATUS
UpdateConfigureMemorySpeedInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

EFI_STATUS
UpdateConfigureVoltageInfo (
  IN    EFI_HII_HANDLE            HiiHandle,
  IN    UINTN                     SmbiosType17Index
  );

#endif

