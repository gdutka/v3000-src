/** @file
  The header file related to the device information form.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef NVME_DETAIL_DISPLAY_H_
#define NVME_DETAIL_DISPLAY_H_

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>
#include <IndustryStandard/Nvme.h>
#include <Protocol/H2ONvmeInfo.h>

//
// Definitions for PCD PcdH2ONvmeDetailDisplayList.
//

#pragma pack(1)

typedef struct {
  UINT8   SectionNumber;
  UINT8   ItemNumber;
} NVME_DETAIL_DISPLAY_LIST_ELEMENT;

#pragma pack()

#define NVME_CONTROLLER_INFORMATION_SECTION_NUMBER    0x00
#define NVME_NAMESPACE_INFORMATION_SECTION_NUMBER     0x01
#define NVME_SMART_INFORMATION_SECTION_NUMBER         0x02

//
// Definitions for section-level updates.
//

EFI_STATUS
typedef
(*ADD_NVME_FORM_SECTION) (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry
  );

#pragma pack(1)

typedef struct {
  BOOLEAN                     IsSectionAdded;
  ADD_NVME_FORM_SECTION       AddNvmeFormSection;
} NVME_DETAIL_DISPLAY_SECTION_ENTRY;

#pragma pack()

EFI_STATUS
AddControllerInfoSection (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry
  );

EFI_STATUS
AddNamespaceInfoSection (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry
  );

EFI_STATUS
AddSmartInfoSection (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry
  );

//
// Definitions for the controller information section.
//

EFI_STATUS
typedef
(*ADD_CONTROLLER_INFO_ITEM) (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  );

#pragma pack(1)

typedef struct {
  ADD_CONTROLLER_INFO_ITEM                  AddControllerInfoItem;
} NVME_DETAIL_DISPLAY_CONTROLLER_ITEM_ENTRY;

#pragma pack()

EFI_STATUS
AddEfiDevicePathItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  );

EFI_STATUS
AddPciLocationItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  );

EFI_STATUS
AddTotalNamespacesItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  );

EFI_STATUS
AddSerialNumberItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  );

EFI_STATUS
AddModelNumberItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  );

EFI_STATUS
AddFirmwareRevisionItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  );

EFI_STATUS
AddIeeeOuiItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  );

//
// Definitions for the namespace information section.
//

EFI_STATUS
typedef
(*ADD_NAMESPACE_INFO_ITEM) (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    EFI_STRING_ID                       ValueStringId,
  IN    UINT32                              NamespaceId OPTIONAL,
  IN    NVME_ADMIN_NAMESPACE_DATA           *NamespaceData OPTIONAL
  );

#pragma pack(1)

typedef struct {
  ADD_NAMESPACE_INFO_ITEM                   AddNamespaceInfoItem;
} NVME_DETAIL_DISPLAY_NAMESPACE_ITEM_ENTRY;

#pragma pack()

EFI_STATUS
AddNamespaceIdItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    EFI_STRING_ID                       StringId,
  IN    UINT32                              NamespaceId OPTIONAL,
  IN    NVME_ADMIN_NAMESPACE_DATA           *NamespaceData OPTIONAL
  );

EFI_STATUS
AddIeeeEui64Item (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    EFI_STRING_ID                       StringId,
  IN    UINT32                              NamespaceId OPTIONAL,
  IN    NVME_ADMIN_NAMESPACE_DATA           *NamespaceData OPTIONAL
  );

EFI_STATUS
AddStorageCapacityItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    EFI_STRING_ID                       StringId,
  IN    UINT32                              NamespaceId OPTIONAL,
  IN    NVME_ADMIN_NAMESPACE_DATA           *NamespaceData OPTIONAL
  );

#endif // NVME_DETAIL_DISPLAY_H_