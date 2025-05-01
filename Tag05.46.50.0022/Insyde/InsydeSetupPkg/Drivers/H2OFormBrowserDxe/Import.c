/** @file
For IFR Import Op Codes

;******************************************************************************
;* Copyright (c) 2019 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "InternalH2OFormBrowser.h"
#include "FBProcessVfcf.h"


EFI_STATUS
GetImportInfoListFromVfrFormSet (
  IN FORM_BROWSER_FORMSET                     *FormSet,
  OUT IMPORT_INFO                             **ImportInfoList,
  OUT UINT16                                  *ImportInfoCount
  )
{
  LIST_ENTRY                                  *LinkForm;
  LIST_ENTRY                                  *LinkStatement;
  FORM_BROWSER_FORM                           *Form;
  FORM_BROWSER_STATEMENT                      *Statement;
  UINT16                                      Count;
  IMPORT_INFO                                 *NewImportInfoList;
  IMPORT_INFO                                 *ImportInfo;

  if (FormSet == NULL || ImportInfoList == NULL || ImportInfoCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Count = 0;
  LinkForm = GetFirstNode (&FormSet->FormListHead);
  while (!IsNull (&FormSet->FormListHead, LinkForm)) {
    Form     = FORM_BROWSER_FORM_FROM_LINK (LinkForm);
    LinkForm = GetNextNode (&FormSet->FormListHead, LinkForm);

    LinkStatement = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, LinkStatement)) {
      Statement     = FORM_BROWSER_STATEMENT_FROM_LINK (LinkStatement);
      LinkStatement = GetNextNode (&Form->StatementListHead, LinkStatement);
      if (IS_VFR_IMPORT_FAKE_STATEMENT(Statement) && Statement->VfrImportInfo->ImportInfo.Type == VFR_IMPORT) {
        Count++;
      }
    }
  }
  if (Count == 0) {
    return EFI_NOT_FOUND;
  }

  //
  // Update ImportInfoList
  //
  NewImportInfoList = ReallocatePool (
                        sizeof (IMPORT_INFO) * (*ImportInfoCount),
                        sizeof (IMPORT_INFO) * (*ImportInfoCount + Count),
                        *ImportInfoList
                        );
  if (NewImportInfoList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Count = *ImportInfoCount;
  LinkForm = GetFirstNode (&FormSet->FormListHead);
  while (!IsNull (&FormSet->FormListHead, LinkForm)) {
    Form     = FORM_BROWSER_FORM_FROM_LINK (LinkForm);
    LinkForm = GetNextNode (&FormSet->FormListHead, LinkForm);

    LinkStatement = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, LinkStatement)) {
      Statement     = FORM_BROWSER_STATEMENT_FROM_LINK (LinkStatement);
      LinkStatement = GetNextNode (&Form->StatementListHead, LinkStatement);
      if (IS_VFR_IMPORT_FAKE_STATEMENT(Statement) && Statement->VfrImportInfo->ImportInfo.Type == VFR_IMPORT) {
        //
        // Update the ImportInfo which from CurrentQ
        //
        if (((Statement->VfrImportInfo->ImportInfo.Flags & H2O_HII_IMPORT_CURRENT_SELECTION) == H2O_HII_IMPORT_CURRENT_SELECTION) &&
            (mFBPrivate.FB.CurrentQ != NULL)) {
          CopyGuid (&Statement->VfrImportInfo->ImportInfo.SrcFormSetGuid, &mFBPrivate.FB.CurrentQ->FormsetGuid);
          Statement->VfrImportInfo->ImportInfo.SrcFormId = mFBPrivate.FB.CurrentQ->FormId;
          Statement->VfrImportInfo->ImportInfo.SrcId     = mFBPrivate.FB.CurrentQ->QuestionId;
        }

        ImportInfo = &NewImportInfoList[Count++];
        CopyMem (
          ImportInfo,
          &Statement->VfrImportInfo->ImportInfo,
          sizeof (IMPORT_INFO)
          );
        ImportInfo->VfrFakeStatement = (VOID *)Statement;
      }
    }
  }
  *ImportInfoList  = NewImportInfoList;
  *ImportInfoCount = Count;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CopyStatementProperties (
  IN FORM_BROWSER_STATEMENT                   *SrcStatement,
  IN OUT FORM_BROWSER_STATEMENT               *DstStatement
  )
{
  LIST_ENTRY                                  *Link;
  H2O_PROPERTY_INFO                           *SrcProperty;
  H2O_PROPERTY_INFO                           *DstProperty;

  if (DstStatement->PropertyListHead.ForwardLink == NULL) {
    InitializeListHead (&DstStatement->PropertyListHead);
  }

  Link = GetFirstNode (&SrcStatement->PropertyListHead);
  while (!IsNull (&SrcStatement->PropertyListHead, Link)) {
    SrcProperty = H2O_PROPERTY_INFO_NODE_FROM_LINK (Link);
    Link = GetNextNode (&SrcStatement->PropertyListHead, Link);

    DstProperty = AllocateCopyPool (sizeof (H2O_PROPERTY_INFO), SrcProperty);
    if (DstProperty == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    InsertTailList (&DstStatement->PropertyListHead, &DstProperty->Link);
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
CopyStatementExpression (
  IN FORM_BROWSER_FORMSET                     *FormSet,
  IN FORM_BROWSER_FORM                        *Form,
  IN FORM_BROWSER_STATEMENT                   *SrcStatement,
  IN OUT FORM_BROWSER_STATEMENT               *DstStatement
  )
{
  UINTN                                       Index;
  FORM_EXPRESSION_LIST                        *NewExpression;

  if (SrcStatement->Expression == NULL || SrcStatement->Expression->Count == 0) {
    return EFI_SUCCESS;
  }

  if (DstStatement->Expression == NULL) {
    NewExpression = AllocateZeroPool ((UINTN) (sizeof(FORM_EXPRESSION_LIST) + ((SrcStatement->Expression->Count - 1) * sizeof(FORM_EXPRESSION *))));
    if (NewExpression == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    NewExpression->Signature = FORM_EXPRESSION_LIST_SIGNATURE;
    NewExpression->Count     = 0;
  } else {
    NewExpression = AllocateZeroPool ((UINTN) (sizeof(FORM_EXPRESSION_LIST) + ((SrcStatement->Expression->Count + DstStatement->Expression->Count - 1) * sizeof(FORM_EXPRESSION *))));
    if (NewExpression == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    NewExpression->Signature = FORM_EXPRESSION_LIST_SIGNATURE;
    NewExpression->Count     = DstStatement->Expression->Count;
    if (DstStatement->Expression->Count > 0) {
      CopyMem (
        NewExpression->Expression,
        DstStatement->Expression->Expression,
        DstStatement->Expression->Count * sizeof(FORM_EXPRESSION *)
        );
    }
    FreePool (DstStatement->Expression);
  }
  DstStatement->Expression = NewExpression;

  for (Index = 0; Index < SrcStatement->Expression->Count; Index++) {
    CopyExpression (
      FormSet,
      SrcStatement->Expression->Expression[Index],
      FormSet,
      Form,
      &DstStatement->Expression->Expression[DstStatement->Expression->Count]
      );
    InsertTailList (&Form->ExpressionListHead, &DstStatement->Expression->Expression[DstStatement->Expression->Count]->Link);
    DstStatement->Expression->Count++;
  }


  return EFI_SUCCESS;
}

EFI_STATUS
InsertNewStatementAfterFakeStatement (
  IN FORM_BROWSER_FORMSET                     *FormSet,
  IN FORM_BROWSER_FORM                        *Form,
  IN FORM_BROWSER_STATEMENT                   *FakeStatement,
  IN FORM_BROWSER_STATEMENT                   *NewStatement
  )
{
  if (FormSet == NULL || FakeStatement == NULL || NewStatement == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CopyStatementProperties (FakeStatement, NewStatement);
  CopyStatementExpression (FormSet, Form, FakeStatement, NewStatement);

  InsertTailList (&FakeStatement->Link, &NewStatement->Link);
  return EFI_SUCCESS;
}

EFI_STATUS
DeleteAllVfrImportNewStatements (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  LIST_ENTRY                                  *LinkForm;
  LIST_ENTRY                                  *LinkStatement;
  FORM_BROWSER_FORM                           *Form;
  FORM_BROWSER_STATEMENT                      *Statement;
  FORM_BROWSER_STATEMENT                      *FakeStatement;

  if (FormSet == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  LinkForm = GetFirstNode (&FormSet->FormListHead);
  while (!IsNull (&FormSet->FormListHead, LinkForm)) {
    Form = FORM_BROWSER_FORM_FROM_LINK (LinkForm);
    LinkForm = GetNextNode (&FormSet->FormListHead, LinkForm);

    FakeStatement = NULL;
    LinkStatement = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, LinkStatement)) {
      Statement = FORM_BROWSER_STATEMENT_FROM_LINK (LinkStatement);
      LinkStatement = GetNextNode (&Form->StatementListHead, LinkStatement);

      //
      // Delete the import statement which is created by previous VFR import fake statement.
      //
      if (IS_VFR_IMPORT_FAKE_STATEMENT(Statement)) {
        FakeStatement = Statement;
        continue;
      }
      if (FakeStatement != NULL) {
        if (IS_IMPORT_STATEMENT(Statement) &&
            CompareGuid (&Statement->ImportInfo->SrcFormSet->Guid, &FakeStatement->VfrImportInfo->ImportInfo.SrcFormSetGuid) &&
            Statement->ImportInfo->SrcForm->FormId == FakeStatement->VfrImportInfo->ImportInfo.SrcFormId &&
            ((IMPORT_INFO_FLAG_IS_QUESTION(FakeStatement->VfrImportInfo->ImportInfo.Flags)  && Statement->ImportInfo->SrcStatement->QuestionId  == FakeStatement->VfrImportInfo->ImportInfo.SrcId) ||
             (!IMPORT_INFO_FLAG_IS_QUESTION(FakeStatement->VfrImportInfo->ImportInfo.Flags) && Statement->ImportInfo->SrcStatement->StatementId == FakeStatement->VfrImportInfo->ImportInfo.SrcId))) {
          RemoveEntryList (&Statement->Link);
          DestroyStatement (FormSet, Statement);
        }

        FakeStatement = NULL;
      }
    }
  }

  return EFI_SUCCESS;
}

STATIC
FB_VFR_IMPORT_INFO *
CreateVfrImportInfo (
  IN FORM_BROWSER_FORMSET                     *FormSet,
  IN FORM_BROWSER_FORM                        *Form,
  IN H2O_IFR_GUID_IMPORT                      *ImportOpcode
  )
{
  FB_VFR_IMPORT_INFO                          *VfrImportInfo;
  EFI_IFR_SUBTITLE                            *Subtitle;
  IMPORT_INFO                                 *ImportInfo;

  VfrImportInfo = AllocateZeroPool (sizeof (FB_VFR_IMPORT_INFO));
  if (VfrImportInfo == NULL) {
    return NULL;
  }

  //
  // Initialize IFR subtitle opcode
  //
  Subtitle               = &VfrImportInfo->IfrSubtitleOpcode;
  Subtitle->Header.OpCode = EFI_IFR_SUBTITLE_OP;
  Subtitle->Header.Length = sizeof(EFI_IFR_SUBTITLE);
  Subtitle->Header.Scope  = ImportOpcode->Header.Scope;

  //
  // Initialize ImportInfo
  //
  ImportInfo        = &VfrImportInfo->ImportInfo;
  ImportInfo->Type  = VFR_IMPORT;
  ImportInfo->Flags = ImportOpcode->Flags;
  if ((ImportOpcode->Flags & H2O_HII_IMPORT_CURRENT_SELECTION) == H2O_HII_IMPORT_CURRENT_SELECTION) {
    ImportInfo->Flags     |= H2O_HII_IMPORT_QUESTION;
    //
    // The SrcFormSetId and Src FormId will be assigned when processing this ImportInfo
    //
    ImportInfo->DstFormId = Form->FormId;
    ImportInfo->DstId     = H2O_IFR_GUID_IMPORT_LOCAL_ID_NON_CONFLICTING;
  } else {
    CopyGuid (&ImportInfo->SrcFormSetGuid, (EFI_GUID *)(UINT8 *)&ImportOpcode->TargetFormsetGuid);
    ImportInfo->SrcFormId = ImportOpcode->TargetFormId;
    ImportInfo->SrcId     = ImportOpcode->TargetId;

    ImportInfo->DstFormId = Form->FormId;
    ImportInfo->DstId     = ImportOpcode->LocalId;
  }
  ImportInfo->ExpressionQId = 0;

  return VfrImportInfo;
}

FORM_BROWSER_STATEMENT *
CreateFakeStatementByVfrImport (
  IN H2O_IFR_GUID_IMPORT                      *ImportOpcode,
  IN OUT FORM_BROWSER_FORMSET                 *FormSet,
  IN OUT FORM_BROWSER_FORM                    *Form
  )
{
  FB_VFR_IMPORT_INFO                          *VfrImportInfo;
  FORM_BROWSER_STATEMENT                      *Statement;

  if (ImportOpcode == NULL || FormSet == NULL || Form == NULL) {
    return NULL;
  }
  if (ImportOpcode->Header.OpCode != EFI_IFR_GUID_OP ||
      ImportOpcode->Function != H2O_IFR_EXT_IMPORT) {
    return NULL;
  }

  VfrImportInfo = CreateVfrImportInfo (FormSet, Form, ImportOpcode);
  if (VfrImportInfo == NULL) {
    return NULL;
  }

  Statement = CreateStatement ((UINT8 *) &VfrImportInfo->IfrSubtitleOpcode, FormSet, Form);
  if (Statement == NULL) {
    FreePool (VfrImportInfo);
    return NULL;
  }
  Statement->VfrImportInfo = VfrImportInfo;

  return Statement;
}

/**
 Create a fake statement for VFR label.

 @param[in]      Label                        Pointer to VFR label opcode
 @param[in, out] FormSet                      Pointer to formset data
 @param[in, out] Form                         Pointer to form data

 @return The pointer of statement data or NULL if input parameter is NULL or allocate memory failed.
**/
FORM_BROWSER_STATEMENT *
CreateFakeStatementByVfrLabel (
  IN     EFI_IFR_GUID_LABEL                   *Label,
  IN OUT FORM_BROWSER_FORMSET                 *FormSet,
  IN OUT FORM_BROWSER_FORM                    *Form
  )
{
  FB_VFR_IMPORT_INFO                          *VfrImportInfo;
  EFI_IFR_SUBTITLE                            *Subtitle;
  FORM_BROWSER_STATEMENT                      *Statement;

  if (Label == NULL || FormSet == NULL || Form == NULL) {
    return NULL;
  }

  VfrImportInfo = AllocateZeroPool (sizeof (FB_VFR_IMPORT_INFO));
  if (VfrImportInfo == NULL) {
    return NULL;
  }
  VfrImportInfo->ImportInfo.Type  = VFR_LABEL;
  VfrImportInfo->ImportInfo.SrcId = Label->Number;

  //
  // Initialize IFR subtitle opcode
  //
  Subtitle                = &VfrImportInfo->IfrSubtitleOpcode;
  Subtitle->Header.OpCode = EFI_IFR_SUBTITLE_OP;
  Subtitle->Header.Length = sizeof(EFI_IFR_SUBTITLE);
  Subtitle->Header.Scope  = Label->Header.Scope;

  Statement = CreateStatement ((UINT8 *) &VfrImportInfo->IfrSubtitleOpcode, FormSet, Form);
  if (Statement == NULL) {
    FreePool (VfrImportInfo);
    return NULL;
  }
  Statement->VfrImportInfo = VfrImportInfo;

  return Statement;
}

