/** @file
 Target info related functions for VFR/VFCF linktarget and importtarget opcodes.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "FBTargetInfo.h"
#include "FBProcessVfcf.h"

/**
 Create VFR target info for VFR linktarget opcode

 @param[in]      LinkTargetOpcode      Pointer to VFR linktarget opcode.
 @param[in, out] Form                  Pointer to form.

 @return The pointer of VFR target info or NULL if input parameter is NULL or allocate memory failed.
**/
H2O_VFR_TARGET_INFO *
CreateVfrTargetInfoByVfrLinkTarget (
  IN     H2O_IFR_GUID_LINK_TARGET             *LinkTargetOpcode,
  IN OUT FORM_BROWSER_FORM                    *Form
  )
{
  H2O_VFR_TARGET_INFO                         *VfrTargetInfo;

  if (LinkTargetOpcode == NULL || Form == NULL) {
    return NULL;
  }

  VfrTargetInfo = AllocateZeroPool (sizeof(H2O_VFR_TARGET_INFO));
  if (VfrTargetInfo == NULL) {
    return NULL;
  }
  VfrTargetInfo->Signature     = H2O_VFR_TARGET_INFO_SIGNATURE;
  VfrTargetInfo->TargetType    = VFR_LINK_TARGET;
  VfrTargetInfo->TargetLabelId = LinkTargetOpcode->TargetLabelId;
  VfrTargetInfo->TargetFormId  = LinkTargetOpcode->TargetFormId;
  CopyGuid (&VfrTargetInfo->TargetFormsetGuid, &LinkTargetOpcode->TargetFormsetGuid);
  InitializeListHead (&VfrTargetInfo->PropertyListHead);

  InsertTailList (&Form->TargetInfoListHead, &VfrTargetInfo->Link);
  return VfrTargetInfo;
}

/**
 Create VFR target info for VFR importtarget opcode

 @param[in]      ImportTargetOpcode    Pointer to VFR importtarget opcode.
 @param[in, out] Form                  Pointer to form.

 @return The pointer of VFR target info or NULL if input parameter is NULL or allocate memory failed.
**/
H2O_VFR_TARGET_INFO *
CreateVfrTargetInfoByVfrImportTarget (
  IN     H2O_IFR_GUID_IMPORT_TARGET           *ImportTargetOpcode,
  IN OUT FORM_BROWSER_FORM                    *Form
  )
{
  H2O_VFR_TARGET_INFO                         *VfrTargetInfo;

  if (ImportTargetOpcode == NULL) {
    return NULL;
  }

  VfrTargetInfo = AllocateZeroPool (sizeof(H2O_VFR_TARGET_INFO));
  if (VfrTargetInfo == NULL) {
    return NULL;
  }
  VfrTargetInfo->Signature     = H2O_VFR_TARGET_INFO_SIGNATURE;
  VfrTargetInfo->TargetType    = VFR_IMPORT_TARGET;
  VfrTargetInfo->TargetLabelId = ImportTargetOpcode->TargetLabelId;
  VfrTargetInfo->TargetFormId  = ImportTargetOpcode->TargetFormId;
  CopyGuid (&VfrTargetInfo->TargetFormsetGuid, &ImportTargetOpcode->TargetFormsetGuid);
  InitializeListHead (&VfrTargetInfo->PropertyListHead);

  InsertTailList (&Form->TargetInfoListHead, &VfrTargetInfo->Link);
  return VfrTargetInfo;
}

/**
 Destroy all VFR target info from form

 @param[in, out] Form                  Pointer to form.
**/
VOID
DestroyAllVfrTargetInfo (
  IN OUT FORM_BROWSER_FORM                    *Form
  )
{
  LIST_ENTRY                                  *Link;
  H2O_VFR_TARGET_INFO                         *VfrTargetInfo;
  H2O_PROPERTY_INFO                           *Property;

  if (Form == NULL) {
    return;
  }

  while (!IsListEmpty (&Form->TargetInfoListHead)) {
    Link          = GetFirstNode (&Form->TargetInfoListHead);
    VfrTargetInfo = H2O_VFR_TARGET_INFO_FROM_LINK (Link);
    RemoveEntryList (&VfrTargetInfo->Link);

    while (!IsListEmpty (&VfrTargetInfo->PropertyListHead)) {
      Link     = GetFirstNode (&VfrTargetInfo->PropertyListHead);
      Property = H2O_PROPERTY_INFO_NODE_FROM_LINK (Link);
      RemoveEntryList (&Property->Link);

      FreePool (Property);
    }
    FreePool (VfrTargetInfo);
  }
}

/**
 Enumerate all target info from VFR formset

 @param[in]      FormSet               Pointer to formset data.
 @param[in, out] Private               Pointer to form browser private data.

 @retval EFI_SUCCESS                   Successfully enumerate all target info from VFR formset
 @retval EFI_INVALID_PARAMETER         Input parameter is NULL
 @retval EFI_OUT_OF_RESOURCES          Failed to allocate memory
**/
STATIC
EFI_STATUS
EnumerateAllTargetInfoFromVfrFormset (
  IN     FORM_BROWSER_FORMSET           *FormSet,
  IN OUT H2O_FORM_BROWSER_PRIVATE_DATA  *Private
  )
{
  LIST_ENTRY                            *FormLink;
  FORM_BROWSER_FORM                     *Form;
  LIST_ENTRY                            *Link;
  H2O_VFR_TARGET_INFO                   *VfrTargetInfo;
  H2O_FORM_BROWSER_TARGET_INFO          *TargetInfo;
  H2O_PROPERTY_INFO                     *Property;
  H2O_PROPERTY_VALUE                    PropValue;
  EFI_STATUS                            Status;

  if (FormSet == NULL || Private == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (FormLink = GetFirstNode (&FormSet->FormListHead); !IsNull (&FormSet->FormListHead, FormLink); FormLink = GetNextNode (&FormSet->FormListHead, FormLink)) {
    Form = FORM_BROWSER_FORM_FROM_LINK (FormLink);
    if (IsListEmpty (&Form->TargetInfoListHead)) {
      continue;
    }

    for (Link = GetFirstNode (&Form->TargetInfoListHead); !IsNull (&Form->TargetInfoListHead, Link); Link = GetNextNode (&Form->TargetInfoListHead, Link)) {
      VfrTargetInfo = H2O_VFR_TARGET_INFO_FROM_LINK (Link);

      TargetInfo = AllocateZeroPool (sizeof (H2O_FORM_BROWSER_TARGET_INFO));
      if (TargetInfo == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      TargetInfo->Signature = H2O_FORM_BROWSER_TARGET_INFO_SIGNATURE;

      TargetInfo->FormId = Form->FormId;
      CopyGuid (&TargetInfo->FormsetGuid, &FormSet->Guid);

      TargetInfo->TargetType    = VfrTargetInfo->TargetType;
      TargetInfo->TargetLabelId = VfrTargetInfo->TargetLabelId;
      TargetInfo->TargetFormId  = VfrTargetInfo->TargetFormId;
      CopyGuid (&TargetInfo->TargetFormsetGuid, &VfrTargetInfo->TargetFormsetGuid);

      Status = GetPropertyFromPropertyList (&VfrTargetInfo->PropertyListHead, H2O_HII_TARGET_PRIORITY_PROPERTY_NAME, NULL, &Property, &PropValue);
      TargetInfo->TargetPriority = EFI_ERROR (Status) ? H2O_HII_TARGET_PRIORITY_PROPERTY_DEFAULT_VALUE : PropValue.H2OValue.Value.U16;
      Status = GetPropertyFromPropertyList (&VfrTargetInfo->PropertyListHead, "enable", NULL, &Property, &PropValue);
      TargetInfo->Enable         = EFI_ERROR (Status) ? TRUE : PropValue.H2OValue.Value.Bool;

      InsertTailList (&Private->TargetInfoList, &TargetInfo->Link);
    }
  }

  return EFI_SUCCESS;
}

/**
 Enumerate all target info from VFRs

 @param[in, out] Private               Pointer to form browser private data.

 @retval EFI_SUCCESS                   Successfully enumerate all target info from VFRs
 @retval EFI_INVALID_PARAMETER         Input parameter is NULL
 @retval EFI_NOT_FOUND                 There is no form package in HII database
**/
STATIC
EFI_STATUS
EnumerateAllTargetInfoFromVfr (
  IN OUT H2O_FORM_BROWSER_PRIVATE_DATA  *Private
  )
{
  EFI_STATUS                            Status;
  EFI_HII_HANDLE                        *HiiHandles;
  EFI_HII_PACKAGE_LIST_HEADER           *HiiPackageList;
  EFI_HII_PACKAGE_HEADER                *PackageHeader;
  UINTN                                 Index;
  UINTN                                 Size;
  UINT32                                Offset;
  UINT32                                FormPkgOffset;
  UINT8                                 *OpCodeData;
  UINT8                                 *Package;
  FORM_BROWSER_FORMSET                  *FormSet;
  HII_HANDLE_ENTRY                      *HiiHandleEntry;


  if (Private == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HiiHandles = HiiGetHiiHandlesByPackageType (EFI_HII_PACKAGE_FORMS);
  if (HiiHandles == NULL) {
    return EFI_NOT_FOUND;
  }

  for (Index = 0; HiiHandles[Index] != NULL; Index++) {
    Status = FBHiiExportPackageLists (HiiHandles[Index], &Size, &HiiPackageList);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Offset = sizeof (EFI_HII_PACKAGE_LIST_HEADER);
    while (Offset < HiiPackageList->PackageLength) {
      PackageHeader = (EFI_HII_PACKAGE_HEADER *) (((UINT8 *) HiiPackageList) + Offset);
      Offset += PackageHeader->Length;
      if (PackageHeader->Type != EFI_HII_PACKAGE_FORMS) {
        continue;
      }

      Package = (UINT8 *) PackageHeader;
      FormPkgOffset = sizeof (EFI_HII_PACKAGE_HEADER);
      while (FormPkgOffset < PackageHeader->Length) {
        OpCodeData = Package + FormPkgOffset;
        if (((EFI_IFR_OP_HEADER *) OpCodeData)->OpCode != EFI_IFR_FORM_SET_OP) {
          FormPkgOffset += ((EFI_IFR_OP_HEADER *) OpCodeData)->Length;
          continue;
        }

        FormSet = AllocateZeroPool (sizeof(FORM_BROWSER_FORMSET));
        if (FormSet == NULL) {
          break;
        }
        Status = InitializeFormSet (HiiHandles[Index], (GUID *)(VOID *) &((EFI_IFR_FORM_SET *) OpCodeData)->Guid, FormSet);
        if (EFI_ERROR (Status)) {
          FreePool (FormSet);
          break;
        }
        FormPkgOffset += (UINT32) FormSet->IfrBinaryLength;

        EnumerateAllTargetInfoFromVfrFormset (FormSet, Private);
        DestroyFormSet (FormSet);
      }
    }
    FreePool (HiiPackageList);
    HiiPackageList = NULL;
  }

  while (!IsListEmpty (&Private->NeedAddHiiHandleList)) {
    HiiHandleEntry = (HII_HANDLE_ENTRY *)GetFirstNode (&Private->NeedAddHiiHandleList);
    RemoveEntryList (&HiiHandleEntry->Link);
    FreePool (HiiHandleEntry);
  }
  FreePool (HiiHandles);
  return EFI_SUCCESS;
}

/**
 Enumerate all target info from VFCF

 @param[in, out] Private               Pointer to form browser private data.

 @retval EFI_SUCCESS                   Successfully enumerate all target info from VFCF
 @retval EFI_INVALID_PARAMETER         Input parameter is NULL
 @retval EFI_NOT_FOUND                 There is no formset data in VFCF
 @retval EFI_OUT_OF_RESOURCES          Failed to allocate memory
**/
STATIC
EFI_STATUS
EnumerateAllTargetInfoFromVfcf (
  IN OUT H2O_FORM_BROWSER_PRIVATE_DATA           *Private
  )
{
  EFI_STATUS                                     Status;
  LIST_ENTRY                                     *LayoutListHead;
  LIST_ENTRY                                     *VfrListHead;
  LIST_ENTRY                                     *VfrLink;
  H2O_VFR_INFO                                   *VfrInfo;
  LIST_ENTRY                                     *FormsetLink;
  H2O_FORMSET_INFO                               *FormsetInfo;
  LIST_ENTRY                                     *FormLink;
  LIST_ENTRY                                     *Link;
  H2O_FORM_INFO                                  *FormInfo;
  H2O_TARGET_INFO                                *VfcfTargetInfo;
  H2O_PROPERTY_INFO                              *Property;
  H2O_PROPERTY_VALUE                             PropValue;
  H2O_FORM_BROWSER_TARGET_INFO                   *TargetInfo;

  if (Private == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetLayoutTreeAndVfrTree (&LayoutListHead, &VfrListHead);
  if (EFI_ERROR (Status) || VfrListHead == NULL || IsListEmpty (VfrListHead)) {
    return EFI_NOT_FOUND;
  }

  for (VfrLink = GetFirstNode (VfrListHead); !IsNull (VfrListHead, VfrLink); VfrLink = GetNextNode (VfrListHead, VfrLink)) {
    VfrInfo = H2O_VFR_INFO_NODE_FROM_LINK (VfrLink);
    if (IsListEmpty (&VfrInfo->FormsetListHead)) {
      continue;
    }

    for (FormsetLink = GetFirstNode (&VfrInfo->FormsetListHead); !IsNull (&VfrInfo->FormsetListHead, FormsetLink); FormsetLink = GetNextNode (&VfrInfo->FormsetListHead, FormsetLink)) {
      FormsetInfo = H2O_FORMSET_INFO_NODE_FROM_LINK (FormsetLink);
      if (IsListEmpty (&FormsetInfo->FormListHead)) {
        continue;
      }

      for (FormLink = GetFirstNode (&FormsetInfo->FormListHead); !IsNull (&FormsetInfo->FormListHead, FormLink); FormLink = GetNextNode (&FormsetInfo->FormListHead, FormLink)) {
        FormInfo = H2O_FORM_INFO_NODE_FROM_LINK (FormLink);
        if (IsListEmpty (&FormInfo->TargetListHead)) {
          continue;
        }

        for (Link = GetFirstNode (&FormInfo->TargetListHead); !IsNull (&FormInfo->TargetListHead, Link); Link = GetNextNode (&FormInfo->TargetListHead, Link)) {
          VfcfTargetInfo = H2O_TARGET_INFO_NODE_FROM_LINK (Link);

          TargetInfo = AllocateZeroPool (sizeof (H2O_FORM_BROWSER_TARGET_INFO));
          if (TargetInfo == NULL) {
            return EFI_OUT_OF_RESOURCES;
          }
          TargetInfo->Signature = H2O_FORM_BROWSER_TARGET_INFO_SIGNATURE;

          TargetInfo->FormId = FormInfo->FormBlock->FormId;
          CopyGuid (&TargetInfo->FormsetGuid, &FormsetInfo->FormsetId);

          TargetInfo->TargetType    = (VfcfTargetInfo->VfcfOpcode == H2O_HII_LIBT_IMPORT_TARGET_BEGIN) ? VFCF_IMPORT_TARGET : VFCF_LINK_TARGET;
          TargetInfo->TargetLabelId = VfcfTargetInfo->TargetLabelId;
          TargetInfo->TargetFormId  = VfcfTargetInfo->TargetFormId;
          CopyGuid (&TargetInfo->TargetFormsetGuid, &VfcfTargetInfo->TargetFormsetGuid);

          Status = GetPropertyFromStyleList (&VfcfTargetInfo->StyleListHead, 0, H2O_STYLE_PSEUDO_CLASS_NORMAL, H2O_HII_TARGET_PRIORITY_PROPERTY_NAME, NULL, &Property, &PropValue);
          TargetInfo->TargetPriority = EFI_ERROR (Status) ? H2O_HII_TARGET_PRIORITY_PROPERTY_DEFAULT_VALUE : PropValue.H2OValue.Value.U16;
          Status = GetPropertyFromStyleList (&VfcfTargetInfo->StyleListHead, 0, H2O_STYLE_PSEUDO_CLASS_NORMAL, "enable", NULL, &Property, &PropValue);
          TargetInfo->Enable         = EFI_ERROR (Status) ? TRUE : PropValue.H2OValue.Value.Bool;

          InsertTailList (&Private->TargetInfoList, &TargetInfo->Link);
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**
 Initialize target info list

 @param[in, out] Private               Pointer to form browser private data.

 @retval EFI_SUCCESS                   Successfully initialize target info list
**/
EFI_STATUS
InitializeTargetInfoList (
  IN OUT H2O_FORM_BROWSER_PRIVATE_DATA        *Private
  )
{
  EnumerateAllTargetInfoFromVfr (Private);
  EnumerateAllTargetInfoFromVfcf (Private);
  return EFI_SUCCESS;
}

/**
 Destroy all target info

 @param[in, out] Private               Pointer to form browser private data.
**/
VOID
DestroyTargetInfoList (
  IN OUT H2O_FORM_BROWSER_PRIVATE_DATA        *Private
  )
{
  LIST_ENTRY                                  *Link;
  H2O_FORM_BROWSER_TARGET_INFO                *TargetInfo;

  Link = GetFirstNode (&Private->TargetInfoList);
  while (!IsNull (&Private->TargetInfoList, Link)) {
    TargetInfo = H2O_FORM_BROWSER_TARGET_INFO_FROM_LINK (Link);
    Link       = GetNextNode (&Private->TargetInfoList, Link);

    RemoveEntryList (&TargetInfo->Link);
    FreePool (TargetInfo);
  }
}

/**
 Check if there is a target info for specifid (formset, form, label ID).

 @param[in] FormSetGuid        Pointer to formset GUID
 @param[in] FormId             Form ID in specified formset
 @param[in] LabelId            Label ID in specified formset

 @retval TRUE                  There is a target info.
 @retval FALSE                 There is no match target info.
**/
BOOLEAN
HaveTargetInfo (
  IN EFI_GUID                                 *FormSetGuid,
  IN UINT16                                   FormId  OPTIONAL,
  IN UINT16                                   LabelId OPTIONAL
  )
{
  LIST_ENTRY                                  *Link;
  H2O_FORM_BROWSER_TARGET_INFO                *TargetInfo;

  for (Link = GetFirstNode (&mFBPrivate.TargetInfoList); !IsNull (&mFBPrivate.TargetInfoList, Link); Link = GetNextNode (&mFBPrivate.TargetInfoList, Link)) {
    TargetInfo = H2O_FORM_BROWSER_TARGET_INFO_FROM_LINK (Link);
    if (CompareGuid (&TargetInfo->TargetFormsetGuid, FormSetGuid) &&
        (FormId  == 0 || (TargetInfo->TargetFormId  == FormId)) &&
        (LabelId == 0 || (TargetInfo->TargetLabelId == LabelId))) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 Internal function to get import info list by specified target (formset, form, fake statement).

 @param[in]      TargetFormSetGuid            Pointer to targetformset GUID
 @param[in]      TargetFormId                 Target form ID
 @param[in]      TargetFakeStatement          Pointer to target fake statement which is for VFR label
 @param[in, out] ImportInfoList               Double pointer to output import info list
 @param[in, out] ImportInfoCount              Pointer to the count of output import info list

 @retval EFI_SUCCESS                          Successfully initialize target info list
 @retval EFI_INVALID_PARAMETER                TargetFakeStatement is invalid
 @retval EFI_NOT_FOUND                        There is no valid import info for target
 @retval EFI_OUT_OF_RESOURCES                 Failed to allocate memory
**/
STATIC
EFI_STATUS
InternalGetImportInfoListFromTargetInfo (
  IN     EFI_GUID                             *TargetFormSetGuid,
  IN     UINT16                               TargetFormId,
  IN     FORM_BROWSER_STATEMENT               *TargetFakeStatement OPTIONAL,
  IN OUT IMPORT_INFO                          **ImportInfoList,
  IN OUT UINT16                               *ImportInfoCount
  )
{
  UINT16                                      TargetLabelId;
  UINT16                                      FoundCount;
  H2O_FORM_BROWSER_TARGET_INFO                *FoundList;
  H2O_FORM_BROWSER_TARGET_INFO                *TargetInfo;
  LIST_ENTRY                                  *Link;
  H2O_FORM_BROWSER_TARGET_INFO                TempTargetInfo;
  UINT16                                      Index;
  UINT16                                      Index2;
  IMPORT_INFO                                 *NewImportInfoList;
  UINT16                                      NewImportInfoCount;
  FORM_BROWSER_FORMSET                        *FormSet;
  FORM_BROWSER_FORM                           *Form;

  if (TargetFakeStatement == NULL) {
    TargetLabelId = 0;
  } else {
    if (!IS_VFR_IMPORT_FAKE_STATEMENT(TargetFakeStatement) || TargetFakeStatement->VfrImportInfo->ImportInfo.Type != VFR_LABEL) {
      return EFI_INVALID_PARAMETER;
    }
    TargetLabelId = TargetFakeStatement->VfrImportInfo->ImportInfo.SrcId;
  }

  //
  // Get all match target info
  //
  FoundList  = NULL;
  FoundCount = 0;
  for (Link = GetFirstNode (&mFBPrivate.TargetInfoList); !IsNull (&mFBPrivate.TargetInfoList, Link); Link = GetNextNode (&mFBPrivate.TargetInfoList, Link)) {
    TargetInfo = H2O_FORM_BROWSER_TARGET_INFO_FROM_LINK (Link);
    if (TargetInfo->TargetLabelId != TargetLabelId ||
        TargetInfo->TargetFormId  != TargetFormId  ||
        !CompareGuid (&TargetInfo->TargetFormsetGuid, TargetFormSetGuid)) {
      continue;
    }

    for (Index = 0; Index < FoundCount; Index++) {
      if (((IS_LINK_TARGET (FoundList[Index].TargetType) && IS_LINK_TARGET(TargetInfo->TargetType)) ||
           (IS_IMPORT_TARGET (FoundList[Index].TargetType) && IS_IMPORT_TARGET(TargetInfo->TargetType))) &&
          FoundList[Index].FormId == TargetInfo->FormId &&
          CompareGuid (&FoundList[Index].FormsetGuid, &TargetInfo->FormsetGuid)) {
        //
        // Found exiting target info, update later one which has higher priority.
        //
        FoundList[Index].TargetPriority = TargetInfo->TargetPriority;
        FoundList[Index].Enable         = TargetInfo->Enable;
        break;
      }
    }
    if (Index == FoundCount) {
      FoundList = ReallocatePool (
                    sizeof(H2O_FORM_BROWSER_TARGET_INFO) * FoundCount,
                    sizeof(H2O_FORM_BROWSER_TARGET_INFO) * (FoundCount + 1),
                    FoundList
                    );
      if (FoundList == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CopyMem (&FoundList[FoundCount], TargetInfo, sizeof (H2O_FORM_BROWSER_TARGET_INFO));
      FoundCount++;
    }
  }
  if (FoundList == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Calculate required import info count.
  //
  NewImportInfoCount = 0;
  for (Index = 0; Index < FoundCount; Index++) {
    if (!FoundList[Index].Enable) {
      continue;
    }

    if (IS_IMPORT_TARGET(FoundList[Index].TargetType)) {
      FormSet = GetFormSetByFormSetGuid (&FoundList[Index].FormsetGuid, &mH2OBrowserImportFormSetList, NULL);
      if (FormSet == NULL) {
        FormSet = CreateImportFormSet (&FoundList[Index].FormsetGuid);
      }
      Form = (FormSet == NULL) ? NULL : IdToForm (FormSet, FoundList[Index].FormId);
      if (Form == NULL || Form->NumberOfStatement == 0) {
        continue;
      }

      NewImportInfoCount += Form->NumberOfStatement;
    } else {
      NewImportInfoCount++;
    }
  }
  if (NewImportInfoCount == 0) {
    FreePool (FoundList);
    return EFI_NOT_FOUND;
  }

  //
  // Sort by Priority. Higher priority value is located in front.
  //
  for (Index = 0; Index < FoundCount - 1; Index++) {
    for (Index2 = 1; Index2 < FoundCount; Index2++) {
      if (FoundList[Index2].TargetPriority > FoundList[Index2 - 1].TargetPriority) {
        CopyMem (&TempTargetInfo       , &FoundList[Index2 - 1], sizeof (H2O_FORM_BROWSER_TARGET_INFO));
        CopyMem (&FoundList[Index2 - 1], &FoundList[Index2]    , sizeof (H2O_FORM_BROWSER_TARGET_INFO));
        CopyMem (&FoundList[Index2]    , &TempTargetInfo       , sizeof (H2O_FORM_BROWSER_TARGET_INFO));
      }
    }
  }

  //
  // Create a new buffer and append new import info in the end.
  //
  NewImportInfoList = AllocateZeroPool ((*ImportInfoCount + NewImportInfoCount) * sizeof(IMPORT_INFO));
  if (NewImportInfoList == NULL) {
    FreePool (FoundList);
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (NewImportInfoList, *ImportInfoList, *ImportInfoCount * sizeof(IMPORT_INFO));

  NewImportInfoCount = *ImportInfoCount;
  for (Index = 0; Index < FoundCount; Index++) {
    if (!FoundList[Index].Enable) {
      continue;
    }

    NewImportInfoList[NewImportInfoCount].VfrFakeStatement = TargetFakeStatement;
    NewImportInfoList[NewImportInfoCount].Type             = FoundList[Index].TargetType;
    NewImportInfoList[NewImportInfoCount].DstFormId        = FoundList[Index].TargetFormId;
    NewImportInfoList[NewImportInfoCount].SrcFormId        = FoundList[Index].FormId;
    CopyGuid (&NewImportInfoList[NewImportInfoCount].SrcFormSetGuid, &FoundList[Index].FormsetGuid);

    if (IS_IMPORT_TARGET(FoundList[Index].TargetType)) {
      FormSet = GetFormSetByFormSetGuid (&FoundList[Index].FormsetGuid, &mH2OBrowserImportFormSetList, NULL);
      Form    = (FormSet == NULL) ? NULL : IdToForm (FormSet, FoundList[Index].FormId);
      if (Form == NULL || Form->NumberOfStatement == 0) {
        continue;
      }

      for (Index2 = 1; Index2 < Form->NumberOfStatement; Index2++) {
        CopyMem (
          &NewImportInfoList[NewImportInfoCount + Index2],
          &NewImportInfoList[NewImportInfoCount],
          sizeof(IMPORT_INFO)
          );
        NewImportInfoList[NewImportInfoCount + Index2].SrcId = Index2;
      }
      NewImportInfoCount += Form->NumberOfStatement;
    } else {
      NewImportInfoCount++;
    }
  }
  FreePool (FoundList);

  if (*ImportInfoList != NULL) {
    FreePool (*ImportInfoList);
  }
  *ImportInfoList  = NewImportInfoList;
  *ImportInfoCount = NewImportInfoCount;
  return EFI_SUCCESS;
}

/**
 Get import info list by specified target formset.

 @param[in]      FormSet                      Pointer to target formset
 @param[in, out] ImportInfoList               Double pointer to output import info list
 @param[in, out] ImportInfoCount              Pointer to the count of output import info list

 @retval EFI_SUCCESS                          Successfully get import info list
 @retval EFI_INVALID_PARAMETER                Input parameter is NULL
**/
EFI_STATUS
GetImportInfoListFromTargetInfo (
  IN     FORM_BROWSER_FORMSET                 *FormSet,
  IN OUT IMPORT_INFO                          **ImportInfoList,
  IN OUT UINT16                               *ImportInfoCount
  )
{
  LIST_ENTRY                                  *LinkForm;
  LIST_ENTRY                                  *LinkStatement;
  FORM_BROWSER_FORM                           *Form;
  FORM_BROWSER_STATEMENT                      *Statement;

  if (FormSet == NULL || ImportInfoList == NULL || ImportInfoCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!HaveTargetInfo (&FormSet->Guid, 0, 0)) {
    return EFI_SUCCESS;
  }

  //
  // Check each VFR label and form to see if there is import info.
  //
  for (LinkForm = GetFirstNode (&FormSet->FormListHead); !IsNull (&FormSet->FormListHead, LinkForm); LinkForm = GetNextNode (&FormSet->FormListHead, LinkForm)) {
    Form = FORM_BROWSER_FORM_FROM_LINK (LinkForm);

    LinkStatement = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, LinkStatement)) {
      Statement     = FORM_BROWSER_STATEMENT_FROM_LINK (LinkStatement);
      LinkStatement = GetNextNode (&Form->StatementListHead, LinkStatement);
      if (IS_VFR_IMPORT_FAKE_STATEMENT(Statement) && Statement->VfrImportInfo->ImportInfo.Type == VFR_LABEL) {
        InternalGetImportInfoListFromTargetInfo (
          &FormSet->Guid,
          Form->FormId,
          Statement,
          ImportInfoList,
          ImportInfoCount
          );
      }
    }

    InternalGetImportInfoListFromTargetInfo (
      &FormSet->Guid,
      Form->FormId,
      NULL,
      ImportInfoList,
      ImportInfoCount
      );
  }

  return EFI_SUCCESS;
}

