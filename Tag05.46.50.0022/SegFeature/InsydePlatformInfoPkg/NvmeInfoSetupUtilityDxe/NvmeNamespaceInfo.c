/** @file
  This source file implements functions that update the namespace section of
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

CONST NVME_DETAIL_DISPLAY_NAMESPACE_ITEM_ENTRY  mNamespaceItemEntries[] = {
                                                  { AddNamespaceIdItem },
                                                  { AddIeeeEui64Item },
                                                  { AddStorageCapacityItem },
                                                  };

/**
  Get the item number list of the namespace information section from PCD
  PcdH2ONvmeDetailDisplayList.

  @param ItemNumberList       Output buffer for the list.
  @param ItemNumberListSize   The number of list elements.

**/
STATIC
VOID
GetNamespaceItemNumberList (
  OUT    UINT8                              **ItemNumberList,
  OUT    UINTN                              *ItemNumberListSize
  )
{
  NVME_DETAIL_DISPLAY_LIST_ELEMENT          *DisplayList;
  UINTN                                     DisplayListSize;
  UINT8                                     *Buffer;
  UINTN                                     BufferSize;
  UINT8                                     ListIndex;
  UINT8                                     SectionNumber;
  UINT8                                     ItemNumber;

  if (ItemNumberList == NULL || ItemNumberListSize == NULL) {
    return;
  }

  DisplayList     = (NVME_DETAIL_DISPLAY_LIST_ELEMENT *) PcdGetPtr (PcdH2ONvmeDetailDisplayList);
  DisplayListSize = PcdGetSize (PcdH2ONvmeDetailDisplayList) / sizeof (NVME_DETAIL_DISPLAY_LIST_ELEMENT);

  Buffer = AllocatePool (DisplayListSize);
  if (Buffer == NULL) {
    *ItemNumberList = NULL;
    return;
  }

  BufferSize = 0;
  for (ListIndex = 0; ListIndex < DisplayListSize; ListIndex++) {
    SectionNumber = DisplayList[ListIndex].SectionNumber;
    ItemNumber    = DisplayList[ListIndex].ItemNumber;

    if (!(SectionNumber == NVME_NAMESPACE_INFORMATION_SECTION_NUMBER &&
            ItemNumber < ARRAY_SIZE (mNamespaceItemEntries))) {
      continue;
    }

    Buffer[BufferSize++] = ItemNumber;
  }

  *ItemNumberList     = Buffer;
  *ItemNumberListSize = BufferSize;

  return;
}

EFI_STATUS
AddNamespaceInfoSection (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    H2O_NVME_INFO_CONTROLLER_ENTRY      *ControllerEntry
  )
{
  EFI_STATUS                                Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL        *NvmePassThru;
  UINT8                                     *ItemNumberList;
  UINTN                                     ItemNumberListSize;
  CONST EFI_STRING_ID                       *StringIdList;
  UINT16                                    StringIdListCounter;
  UINT16                                    QuestionIdCounter;
  UINT32                                    NamespaceId;
  NVME_ADMIN_NAMESPACE_DATA                 *NamespaceData;
  UINTN                                     ListIndex;
  UINT8                                     ItemNumber;

  if (ControllerEntry == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (ControllerEntry->NamespaceId == 0 && ControllerEntry->IsRaidController) {
    return EFI_SUCCESS;
  }

  Status = gBS->HandleProtocol (
                  ControllerEntry->Handle,
                  &gEfiNvmExpressPassThruProtocolGuid,
                  (VOID **) &NvmePassThru
                  );
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  ItemNumberList = NULL;
  GetNamespaceItemNumberList (&ItemNumberList, &ItemNumberListSize);
  if (ItemNumberList == NULL) {
    return EFI_ABORTED;
  }

  StringIdList = NULL;
  SetFormDynamicStringCount (
    mNvmeInfoData.HiiStringIdManager,
    (H2O_NVME_INFO_CONTROLLER_INFO_FORM_ID - 1),
    (UINT16) (ControllerEntry->NumOfValidNamespaces * ARRAY_SIZE (mNamespaceItemEntries)),
    &StringIdList
    );
  if (StringIdList == NULL) {
    return EFI_ABORTED;
  }

  //
  // List the namespace information for the controller.
  //
  QuestionIdCounter   = NVME_NAMESPACE_INFORMATION_QUESTION_ID_BASE;
  StringIdListCounter = 0;
  NamespaceId         = 0xFFFFFFFF;
  
  HiiCreateSubTitleOpCode (OpCodeHandle, STRING_TOKEN (STR_NVME_NAMESPACE_INFORMATION), 0, 0, 0);

  while (TRUE) {
    if (ControllerEntry->NamespaceId > 0) {
      if (NamespaceId == ControllerEntry->NamespaceId) {
        break;
      }
      NamespaceId = ControllerEntry->NamespaceId;
    }
    else {
      Status = NvmePassThru->GetNextNamespace (NvmePassThru, &NamespaceId);
      if (EFI_ERROR (Status)) {
        break;
      }
    }

    NamespaceData = NULL;
    mNvmeInfoData.NvmeInfo->IdentifyNamespace (
                              NvmePassThru,
                              NamespaceId,
                              &NamespaceData
                              );
    if (NamespaceData == NULL) {
      continue;
    }

    if (StringIdListCounter > 0) { // Second and following items
      HiiCreateSubTitleOpCode (OpCodeHandle, STRING_TOKEN (0x0000), 0, 0, 0);
    }

    for (ListIndex = 0; ListIndex < ItemNumberListSize; ListIndex++) {
      ItemNumber = ItemNumberList[ListIndex];

      mNamespaceItemEntries[ItemNumber].AddNamespaceInfoItem (
                                          OpCodeHandle,
                                          QuestionIdCounter++,
                                          StringIdList[StringIdListCounter++],
                                          NamespaceId,
                                          NamespaceData
                                          );
    }

    FreePool (NamespaceData);
  }
  
  HiiCreateSubTitleOpCode (OpCodeHandle, STRING_TOKEN (0x0000), 0, 0, 0);

  FreePool (ItemNumberList);

  return EFI_SUCCESS;
}

EFI_STATUS
AddNamespaceIdItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    EFI_STRING_ID                       StringId,
  IN    UINT32                              NamespaceId OPTIONAL,
  IN    NVME_ADMIN_NAMESPACE_DATA           *NamespaceData OPTIONAL
  )
{
  CHAR16                                    *String;

  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_NVME_NAMESPACE_ID_PROMPT),
    STRING_TOKEN (STR_NVME_NAMESPACE_ID_HELP),
    StringId,
    0x00,
    STRING_TOKEN (0x0000)
    );
  
  String = CatSPrint (NULL, L"%u", NamespaceId);

  HiiSetString (
    mNvmeInfoData.HiiHandle,
    StringId,
    (String != NULL? String: L"N/A"),
    NULL
    );
  
  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AddIeeeEui64Item (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    EFI_STRING_ID                       StringId,
  IN    UINT32                              NamespaceId OPTIONAL,
  IN    NVME_ADMIN_NAMESPACE_DATA           *NamespaceData OPTIONAL
  )
{
  CHAR16                                    *String;

  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_NVME_IEEE_EUI64_PROMPT),
    STRING_TOKEN (STR_NVME_IEEE_EUI64_HELP),
    StringId,
    0x00,
    STRING_TOKEN (0x0000)
    );

  String = NULL;
  if (NamespaceData != NULL) {
    String = ConvertEui64ToString ((UINT8 *) &NamespaceData->Eui64);
  }

  HiiSetString (
    mNvmeInfoData.HiiHandle,
    StringId,
    (String != NULL? String: L"N/A"),
    NULL
    );
  
  if (String != NULL) {
    FreePool (String);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
AddStorageCapacityItem (
  IN    EFI_HII_HANDLE                      OpCodeHandle,
  IN    EFI_QUESTION_ID                     QuestionId,
  IN    EFI_STRING_ID                       StringId,
  IN    UINT32                              NamespaceId OPTIONAL,
  IN    NVME_ADMIN_NAMESPACE_DATA           *NamespaceData OPTIONAL
  )
{
  UINT32                                    BlockSize;
  UINT64                                    NumberOfBlocks;
  H2O_CAPACITY_REPRESENTATION               CapacityRep;
  CHAR8                                     CapacityStrAscii[CAPACITY_REP_STR_MAX_SIZE];
  CHAR16                                    CapacityStrUnicode[CAPACITY_REP_STR_MAX_SIZE];

  HiiCreateActionOpCodeEx (
    OpCodeHandle,
    QuestionId,
    STRING_TOKEN (STR_NVME_STORAGE_CAP_PROMPT),
    STRING_TOKEN (STR_NVME_STORAGE_CAP_HELP),
    StringId,
    0x00,
    STRING_TOKEN (0x0000)
    );

  AsciiSPrint (CapacityStrAscii, sizeof (CapacityStrAscii), "N/A");
  if (NamespaceData != NULL) {
    BlockSize      = 0;
    NumberOfBlocks = 0;
    GetNvmeNamespaceCapacity (NamespaceData, &BlockSize, &NumberOfBlocks);

    ZeroMem (&CapacityRep, sizeof (CapacityRep));
    if (NumberOfBlocks > 0) {
      ComputeBlockIoCapacity (
        BlockSize,
        (NumberOfBlocks - 1),
        FORMULA_NORMAL,
        &CapacityRep,
        NULL
        );
    }

    BlockIoCapacityRepToString (
      &CapacityRep,
      CAPACITY_REP_TO_STR_BASE_1000,
      CapacityStrAscii
      );
    UnicodeSPrint (
      CapacityStrUnicode,
      sizeof (CapacityStrUnicode),
      L"%a",
      CapacityStrAscii
      );
  }
  
  HiiSetString (
    mNvmeInfoData.HiiHandle,
    StringId,
    CapacityStrUnicode,
    NULL
    );

  return EFI_SUCCESS;
}