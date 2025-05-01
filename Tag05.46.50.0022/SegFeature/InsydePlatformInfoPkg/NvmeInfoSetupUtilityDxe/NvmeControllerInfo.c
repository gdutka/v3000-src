/** @file
  This source file implements functions that update the controller section of
  the device information form.

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

#include "NvmeDetailDisplay.h"
#include "NvmeInfoSetupUtilityDxe.h"
#include "NvmeInfoFormset.h"

#include <Library/BaseMemoryLib.h>
#include <Library/BlockIoCapacityLib.h>
#include <Library/HiiExLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NvmeInfoLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

extern H2O_SETUP_NVME_INFO_PRIVATE_DATA mNvmeInfoData;

CONST NVME_DETAIL_DISPLAY_CONTROLLER_ITEM_ENTRY mControllerItemEntries[] = {
                                                  { AddEfiDevicePathItem },
                                                  { AddPciLocationItem },
                                                  { AddTotalNamespacesItem },
                                                  { AddSerialNumberItem },
                                                  { AddModelNumberItem },
                                                  { AddFirmwareRevisionItem },
                                                  { AddIeeeOuiItem },
                                                  };

EFI_STATUS
AddControllerInfoSection (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry
  )
{
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL        *NvmePassThru;
  NVME_ADMIN_CONTROLLER_DATA                *ControllerData;
  NVME_DETAIL_DISPLAY_LIST_ELEMENT          *DisplayList;
  UINTN                                     DisplayListSize;
  UINTN                                     DisplayListIndex;
  UINT16                                    QuestionIdCounter;
  UINT8                                     SectionNumber;
  UINT8                                     ItemNumber;

  if (ControllerEntry == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  NvmePassThru = NULL;
  gBS->HandleProtocol (
         ControllerEntry->Handle,
         &gEfiNvmExpressPassThruProtocolGuid,
         (VOID **) &NvmePassThru
         );

  ControllerData = NULL;
  if (NvmePassThru != NULL) {
    mNvmeInfoData.NvmeInfo->IdentifyController (
                              NvmePassThru,
                              ControllerEntry->NamespaceId,
                              &ControllerData
                              );
  }

  DisplayList     = (NVME_DETAIL_DISPLAY_LIST_ELEMENT *) PcdGetPtr (PcdH2ONvmeDetailDisplayList);
  DisplayListSize = PcdGetSize (PcdH2ONvmeDetailDisplayList) / sizeof (NVME_DETAIL_DISPLAY_LIST_ELEMENT);

  QuestionIdCounter = NVME_CONTROLLER_INFORMATION_QUESTION_ID_BASE;

  HiiCreateSubTitleOpCode (OpCodeHandle, STRING_TOKEN (STR_NVME_CONTROLLER_INFORMATION), 0, 0, 0);

  for (DisplayListIndex = 0; DisplayListIndex < DisplayListSize; DisplayListIndex++) {
    SectionNumber = DisplayList[DisplayListIndex].SectionNumber;
    ItemNumber    = DisplayList[DisplayListIndex].ItemNumber;

    if (!(SectionNumber == NVME_CONTROLLER_INFORMATION_SECTION_NUMBER &&
            ItemNumber < ARRAY_SIZE (mControllerItemEntries))) {
      continue;
    }

    mControllerItemEntries[ItemNumber].AddControllerInfoItem (
                                         OpCodeHandle,
                                         QuestionIdCounter++,
                                         ControllerEntry,
                                         ControllerData
                                         );
  }

  HiiCreateSubTitleOpCode (OpCodeHandle, STRING_TOKEN (0x0000), 0, 0, 0);

  if (ControllerData != NULL) {
    FreePool (ControllerData);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AddEfiDevicePathItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  )
{
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL        *NvmePassThru;
  CHAR16                                    *String;

  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_EFI_DEVICE_PATH),
    STRING_TOKEN (STR_EFI_DEVICE_PATH),
    STRING_TOKEN (STR_EFI_DEVICE_PATH_VALUE),
    0x00,
    STRING_TOKEN (0x0000)
    );

  String = NULL;
  if (ControllerEntry != NULL) {
    NvmePassThru = NULL;
    gBS->HandleProtocol (
           ControllerEntry->Handle,
           &gEfiNvmExpressPassThruProtocolGuid,
           (VOID **) &NvmePassThru
           );

    if (NvmePassThru != NULL) {
      CreateDevicePathTextFromControllerEntry (
        &mNvmeInfoData,
        ControllerEntry,
        NvmePassThru,
        &String
        );
    }
  }
  
  HiiSetString (
    mNvmeInfoData.HiiHandle,
    STRING_TOKEN (STR_EFI_DEVICE_PATH_VALUE),
    (String != NULL? String: L"N/A"),
    NULL
    );

  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AddPciLocationItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  )
{
  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_NVME_PCI_LOCATION_PROMPT),
    STRING_TOKEN (STR_NVME_PCI_LOCATION_HELP),
    STRING_TOKEN (STR_NVME_PCI_LOCATION_VALUE),
    0x00,
    STRING_TOKEN (0x0000)
    );

  if (ControllerEntry != NULL) {
    UpdatePciLocationHiiString (&mNvmeInfoData, ControllerEntry);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AddTotalNamespacesItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  )
{
  CHAR16                                    *String;

  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_NVME_TOTAL_NAMESPACE_PROMPT),
    STRING_TOKEN (STR_NVME_TOTAL_NAMESPACE_HELP),
    STRING_TOKEN (STR_NVME_TOTAL_NAMESPACE_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  String = NULL;
  if (ControllerEntry != NULL) {
    String = CatSPrint (NULL, L"%u", ControllerEntry->NumOfValidNamespaces);
  }

  HiiSetString (
    mNvmeInfoData.HiiHandle,
    STRING_TOKEN (STR_NVME_TOTAL_NAMESPACE_STRING),
    (String != NULL? String: L"N/A"),
    NULL
    );
  
  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AddSerialNumberItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  )
{
  CHAR16                                    *String;

  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_NVME_SERIAL_NUMBER_PROMPT),
    STRING_TOKEN (STR_NVME_SERIAL_NUMBER_HELP),
    STRING_TOKEN (STR_NVME_SERIAL_NUMBER_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  String = NULL;
  if (ControllerData != NULL) {
    String = GetNvmeSerialNumberFromIdentifyData (ControllerData);
  }

  HiiSetString (
    mNvmeInfoData.HiiHandle,
    STRING_TOKEN (STR_NVME_SERIAL_NUMBER_STRING),
    (String != NULL? String: L"N/A"),
    NULL
    );
  
  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AddModelNumberItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  )
{
  CHAR16                                    *String;

  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_NVME_PRODUCT_MODEL_NUMBER_PROMPT),
    STRING_TOKEN (STR_NVME_PRODUCT_MODEL_NUMBER_HELP),
    STRING_TOKEN (STR_NVME_PRODUCT_MODEL_NUMBER_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  String = NULL;
  if (ControllerData != NULL) {
    String = GetNvmeModelNumberFromIdentifyData (ControllerData);
  }

  HiiSetString (
    mNvmeInfoData.HiiHandle,
    STRING_TOKEN (STR_NVME_PRODUCT_MODEL_NUMBER_STRING),
    (String != NULL? String: L"N/A"),
    NULL
    );
  
  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AddFirmwareRevisionItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  )
{
  CHAR16                                    *String;

  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_NVME_FIRMWARE_REVISION_PROMPT),
    STRING_TOKEN (STR_NVME_FIRMWARE_REVISION_HELP),
    STRING_TOKEN (STR_NVME_FIRMWARE_REVISION_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  String = NULL;
  if (ControllerData != NULL) {
    String = GetNvmeFwRevisionFromIdentifyData (ControllerData);
  }

  HiiSetString (
    mNvmeInfoData.HiiHandle,
    STRING_TOKEN (STR_NVME_FIRMWARE_REVISION_STRING),
    (String != NULL? String: L"N/A"),
    NULL
    );
  
  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AddIeeeOuiItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry OPTIONAL,
  IN    NVME_ADMIN_CONTROLLER_DATA          *ControllerData OPTIONAL
  )
{
  CHAR16                                    *String;

  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_NVME_IEEE_OUI_PROMPT),
    STRING_TOKEN (STR_NVME_IEEE_OUI_HELP),
    STRING_TOKEN (STR_NVME_IEEE_OUI_STRING),
    0x00,
    STRING_TOKEN (0x0000)
    );

  String = NULL;
  if (ControllerData != NULL) {
    String = GetNvmeIeeeOuiFromIdentifyData (ControllerData);
  }

  HiiSetString (
    mNvmeInfoData.HiiHandle,
    STRING_TOKEN (STR_NVME_IEEE_OUI_STRING),
    (String != NULL? String: L"N/A"),
    NULL
    );
  
  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}