/** @file
  Process VFCF related functions.

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

#include "FBProcessVfcf.h"
#include "FBTargetInfo.h"
#include "Setup.h"

LIST_ENTRY                             mH2OBrowserImportFormSetList      = INITIALIZE_LIST_HEAD_VARIABLE (mH2OBrowserImportFormSetList);
LIST_ENTRY                             mH2OBrowserStaleImportFormSetList = INITIALIZE_LIST_HEAD_VARIABLE (mH2OBrowserStaleImportFormSetList);
LIST_ENTRY                             mH2OBrowserFormSetHiiResourceList = INITIALIZE_LIST_HEAD_VARIABLE (mH2OBrowserFormSetHiiResourceList);
H2O_FORM_BROWSER_FORMSET_HII_RESOURCE  *mFormSetHiiResource              = NULL;
UINT16                                 mImportInfoCount                  = 0;
IMPORT_INFO                            *mImportInfoList                  = NULL;
H2O_STATEMENT_ID                       mImportStatementId                = 0;
EFI_HANDLE                             mImportFormUpdateNotifyHandle     = NULL;

FORM_EXPRESSION *
CreateExpression (
  IN OUT FORM_BROWSER_FORM        *Form,
  IN     UINT8                    *OpCode
  );

/**
 Copy string from the string ID of source Hii handle to the string ID of destination Hii handle.
 If the string ID of destination Hii handle, it will create a new string ID.

 @param[in] SrcHiiHandle        Source Hii handle
 @param[in] SrcStringId         String ID of source Hii handle
 @param[in] DstHiiHandle        Destination Hii handle
 @param[in] DstStringId         Pointer to string ID of destination Hii handle

 @retval EFI_SUCCESS            Successfully copy string
 @retval EFI_OUT_OF_RESOURCES   Allocate pool fail
 @retval Other                  Fail to copy string
**/
STATIC
EFI_STATUS
CopyHiiString (
  IN     EFI_HII_HANDLE                       SrcHiiHandle,
  IN     EFI_STRING_ID                        SrcStringId,
  IN     EFI_HII_HANDLE                       DstHiiHandle,
  IN OUT EFI_STRING_ID                        *DstStringId
  )
{
  EFI_STRING_ID                               StringId;
  EFI_STATUS                                  Status;

  if (*DstStringId != 0 ||
      mFormSetHiiResource == NULL ||
      mFormSetHiiResource->HiiHandle != DstHiiHandle) {
    return HiiCopyStringByStringId (SrcHiiHandle, SrcStringId, DstHiiHandle, DstStringId);
  }

  if (mFormSetHiiResource->NewStringIdIndex >= mFormSetHiiResource->NewStringIdCount) {
    mFormSetHiiResource->NewStringIdList = ReallocatePool (
                                             sizeof (EFI_STRING_ID *) * (mFormSetHiiResource->NewStringIdCount),
                                             sizeof (EFI_STRING_ID *) * (mFormSetHiiResource->NewStringIdCount + FORMSET_HII_RESOURCE_STRING_ID_INCREMENT),
                                             mFormSetHiiResource->NewStringIdList
                                             );
    if (mFormSetHiiResource->NewStringIdList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    mFormSetHiiResource->NewStringIdCount += FORMSET_HII_RESOURCE_STRING_ID_INCREMENT;
    StringId = 0;
  } else {
    StringId = mFormSetHiiResource->NewStringIdList[mFormSetHiiResource->NewStringIdIndex];
  }

  Status = HiiCopyStringByStringId (SrcHiiHandle, SrcStringId, DstHiiHandle, &StringId);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mFormSetHiiResource->NewStringIdList[mFormSetHiiResource->NewStringIdIndex++] = StringId;

  *DstStringId = StringId;
  return EFI_SUCCESS;
}

/**
 Copy image from the image ID of source Hii handle to the image ID of destination Hii handle.
 If the image ID of destination Hii handle is zero, it will create a new string ID.

 @param[in] SrcHiiHandle        Source Hii handle
 @param[in] SrcImageId          Image ID of source Hii handle
 @param[in] DstHiiHandle        Destination Hii handle
 @param[in] DstImageId          Pointer to image ID of destination Hii handle

 @retval EFI_SUCCESS            Successfully copy image
 @retval EFI_OUT_OF_RESOURCES   Allocate pool fail
 @retval Other                  Fail to copy image
**/
EFI_STATUS
CopyHiiImage (
  IN     EFI_HII_HANDLE                       SrcHiiHandle,
  IN     EFI_IMAGE_ID                         SrcImageId,
  IN     EFI_HII_HANDLE                       DstHiiHandle,
  IN OUT EFI_IMAGE_ID                         *DstImageId
  )
{
  EFI_IMAGE_ID                                ImageId;
  EFI_STATUS                                  Status;

  if (*DstImageId != 0 ||
      mFormSetHiiResource == NULL ||
      mFormSetHiiResource->HiiHandle != DstHiiHandle) {
    return HiiCopyImageByImageId (SrcHiiHandle, SrcImageId, DstHiiHandle, DstImageId);
  }

  if (mFormSetHiiResource->NewImageIdIndex == mFormSetHiiResource->NewImageIdCount) {
    mFormSetHiiResource->NewImageIdList = ReallocatePool (
                                            sizeof (EFI_STRING_ID *) * (mFormSetHiiResource->NewImageIdCount),
                                            sizeof (EFI_STRING_ID *) * (mFormSetHiiResource->NewImageIdCount + FORMSET_HII_RESOURCE_IMAGE_ID_INCREMENT),
                                            mFormSetHiiResource->NewImageIdList
                                            );
    if (mFormSetHiiResource->NewImageIdList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    mFormSetHiiResource->NewImageIdCount += FORMSET_HII_RESOURCE_IMAGE_ID_INCREMENT;
    ImageId = 0;
  } else {
    ImageId = mFormSetHiiResource->NewImageIdList[mFormSetHiiResource->NewImageIdIndex];
  }
  Status = HiiCopyImageByImageId (SrcHiiHandle, SrcImageId, DstHiiHandle, &ImageId);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mFormSetHiiResource->NewImageIdList[mFormSetHiiResource->NewImageIdIndex++] = ImageId;

  *DstImageId = ImageId;
  return EFI_SUCCESS;
}

/**
 Create disableif expression with TRUE expression opcode.

 @param[in, out] ExpressionOpCode             Pointer to expression code.

 @return Expression pointer or NULL if invalid input parameter or fail to allocate pool.
**/
FORM_EXPRESSION *
CreateDisableIfExpression (
  IN OUT EXPRESSION_OPCODE                    *ExpressionOpCode
  )
{
  FORM_EXPRESSION                             *Expression;

  if (ExpressionOpCode == NULL) {
    return NULL;
  }

  Expression = AllocateZeroPool (sizeof (FORM_EXPRESSION));
  if (Expression == NULL) {
    return NULL;
  }

  Expression->Signature = FORM_EXPRESSION_SIGNATURE;
  Expression->Type      = EFI_HII_EXPRESSION_DISABLE_IF;
  InitializeListHead (&Expression->OpCodeListHead);

  ZeroMem (ExpressionOpCode, sizeof (EXPRESSION_OPCODE));
  ExpressionOpCode->Signature     = EXPRESSION_OPCODE_SIGNATURE;
  ExpressionOpCode->Operand       = EFI_IFR_TRUE_OP;
  ExpressionOpCode->Value.Type    = EFI_IFR_TYPE_BOOLEAN;
  ExpressionOpCode->Value.Value.b = TRUE;
  InsertTailList (&Expression->OpCodeListHead, &ExpressionOpCode->Link);

  return Expression;
}

/**
 Insert disableif TRUE expression into expression list of statement.

 @param[in]      FormSet        Pointer to formset
 @param[in]      Form           Pointer to form
 @param[in, out] Statement      Pointer to statement

 @retval EFI_SUCCESS            Successfully insert disableif TRUE expression
 @retval EFI_ABORTED            Fail to create disableif TRUE expression
 @retval EFI_OUT_OF_RESOURCES   Fail to allocate pool
**/
EFI_STATUS
InsertDisableIfExpression (
  IN     FORM_BROWSER_FORMSET                 *FormSet,
  IN     FORM_BROWSER_FORM                    *Form,
  IN OUT FORM_BROWSER_STATEMENT               *Statement
  )
{
  FORM_EXPRESSION                             *Expression;
  FORM_EXPRESSION_LIST                        *ExpressionList;

  Expression = CreateDisableIfExpression (GetNewExpressionOpCode (FormSet));
  if (Expression == NULL) {
    return EFI_ABORTED;
  }
  InsertTailList (&Form->ExpressionListHead, &Expression->Link);

  if (Statement->Expression == NULL) {
    Statement->Expression = (FORM_EXPRESSION_LIST *) AllocateZeroPool (sizeof (FORM_EXPRESSION_LIST));
    if (Statement->Expression == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Statement->Expression->Count         = 1;
    Statement->Expression->Signature     = FORM_EXPRESSION_LIST_SIGNATURE;
    Statement->Expression->Expression[0] = Expression;
  } else {
    ExpressionList = ReallocatePool (
                       (sizeof(FORM_EXPRESSION_LIST) + ((Statement->Expression->Count - 1) * sizeof(FORM_EXPRESSION *))),
                       (sizeof(FORM_EXPRESSION_LIST) + ((Statement->Expression->Count) * sizeof(FORM_EXPRESSION *))),
                       Statement->Expression
                       );
    if (ExpressionList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Statement->Expression = ExpressionList;
    ExpressionList->Expression[ExpressionList->Count++] = Expression;
  }

  return EFI_SUCCESS;
}

/**
 Check if there is a matched name value node in the linked list.

 @param[in] NameValueListHead         Pointer to name value linked list.
 @param[in] VariableName              Pointer to request name

 @retval TRUE                         There is a matched name value node in the linked list
 @retval FALSE                        There is not a matched name value node in the linked list
**/
BOOLEAN
IsNameValueNodeExistence (
  IN LIST_ENTRY                               *NameValueListHead,
  IN CHAR16                                   *VariableName
  )
{
  NAME_VALUE_NODE                             *NameValueNode;
  LIST_ENTRY                                  *Link;

  if (!IsListEmpty(NameValueListHead)) {
    Link = GetFirstNode (NameValueListHead);
    while (!IsNull (NameValueListHead, Link)) {
      NameValueNode = NAME_VALUE_NODE_FROM_LINK (Link);

      if (StrCmp (VariableName, NameValueNode->Name) == 0) {
        return TRUE;
      }

      Link = GetNextNode (NameValueListHead, Link);
    }
  }

  return FALSE;
}

/**
 Check if there is a imported question in form.

 @param[in] Form         Pointer to form

 @retval TRUE            There is a imported question in form
 @retval FALSE           There is not a imported question in form
**/
BOOLEAN
HaveImportQuestion (
  IN FORM_BROWSER_FORM                        *Form
  )
{
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_STATEMENT                      *Question;

  Link = GetFirstNode (&Form->StatementListHead);
  while (!IsNull (&Form->StatementListHead, Link)) {
    Question = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
    Link     = GetNextNode (&Form->StatementListHead, Link);
    if (IS_IMPORT_STATEMENT (Question)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 Check if there is a imported SCU question in form or formset.

 @param[in] FormSet      Pointer to formset
 @param[in] Form         Pointer to form

 @retval TRUE            There is a imported SCU question in form or formset
 @retval FALSE           There is not a imported SCU question in form or formset
**/
BOOLEAN
HaveImportScuQuestion (
  IN FORM_BROWSER_FORMSET                     *FormSet,
  IN FORM_BROWSER_FORM                        *Form
  )
{
  LIST_ENTRY                                  *Link;
  LIST_ENTRY                                  *FormLink;
  FORM_BROWSER_STATEMENT                      *Question;
  FORM_BROWSER_FORM                           *FormPtr;

  if (Form != NULL) {
    Link = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, Link)) {
      Question = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
      Link     = GetNextNode (&Form->StatementListHead, Link);
      if (IS_IMPORT_STATEMENT (Question) && CompareGuid (Question->ImportInfo->SrcFormSet->ClassGuid, &mScuFormSetGuid)) {
        return TRUE;
      }
    }
  }

  if (FormSet != NULL) {
    FormLink = GetFirstNode (&FormSet->FormListHead);
    while (!IsNull (&FormSet->FormListHead, FormLink)) {
      FormPtr  = FORM_BROWSER_FORM_FROM_LINK (FormLink);
      FormLink = GetNextNode (&FormSet->FormListHead, FormLink);
      if (HaveImportScuQuestion (NULL, FormPtr)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
 Check if specific formset is imported in current formset.

 @param[in] CurrentFormSet    Pointer to current formset
 @param[in] ImportedFormset   Pointer to imported formset

 @retval TRUE                 Specific formset is imported in current formset
 @retval FALSE                Specific formset is not imported in current formset
**/
STATIC
BOOLEAN
HaveImportThisFormset (
  IN FORM_BROWSER_FORMSET                     *CurrentFormSet,
  IN FORM_BROWSER_FORMSET                     *ImportedFormset
  )
{
  LIST_ENTRY                                  *FormLink;
  FORM_BROWSER_FORM                           *Form;
  LIST_ENTRY                                  *QuestionLink;
  FORM_BROWSER_STATEMENT                      *Question;

  FormLink = GetFirstNode (&CurrentFormSet->FormListHead);
  while (!IsNull (&CurrentFormSet->FormListHead, FormLink)) {
    Form     = FORM_BROWSER_FORM_FROM_LINK (FormLink);
    FormLink = GetNextNode (&CurrentFormSet->FormListHead, FormLink);

    QuestionLink = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, QuestionLink)) {
      Question     = FORM_BROWSER_STATEMENT_FROM_LINK (QuestionLink);
      QuestionLink = GetNextNode (&Form->StatementListHead, QuestionLink);
      if (IS_IMPORT_STATEMENT (Question) && Question->ImportInfo->SrcFormSet == ImportedFormset) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
 Get available varstore ID from formset.

 @param[in] FormSet             Pointer to formset.

 @return Varstore ID or zero if there is no available varstore ID.
**/
UINT16
GetAvailableVarstoreId (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  LIST_ENTRY                                  *Link;
  FORMSET_STORAGE                             *Storage;
  UINT32                                      VarStoreId;

  for (VarStoreId = 1; VarStoreId <= 0xFFFF; VarStoreId++) {
    Link = GetFirstNode (&FormSet->StorageListHead);
    while (!IsNull (&FormSet->StorageListHead, Link)) {
      Storage = FORMSET_STORAGE_FROM_LINK (Link);
      if (Storage->VarStoreId == VarStoreId) {
        break;
      }

      Link = GetNextNode (&FormSet->StorageListHead, Link);
    }

    if (IsNull (&FormSet->StorageListHead, Link)) {
      return (UINT16) VarStoreId;
    }
  }

  return 0;
}

/**
 Get available statement ID from formset.

 @param[in] FormSet             Pointer to formset.

 @return Statement ID.
**/
H2O_STATEMENT_ID
GetAvailableStatementId (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  H2O_STATEMENT_ID                            MaxStatementId;
  FORM_BROWSER_FORM                           *Form;
  LIST_ENTRY                                  *FormLink;
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_STATEMENT                      *Statement;

  MaxStatementId = 0;
  FormLink = GetFirstNode (&FormSet->FormListHead);
  while (!IsNull (&FormSet->FormListHead, FormLink)) {
    Form     = FORM_BROWSER_FORM_FROM_LINK (FormLink);
    FormLink = GetNextNode (&FormSet->FormListHead, FormLink);

    Link = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, Link)) {
      Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
      Link      = GetNextNode (&Form->StatementListHead, Link);
      if (Statement->StatementId >= MaxStatementId) {
        MaxStatementId = Statement->StatementId;
      }
    }
  }

  Link = GetFirstNode (&FormSet->StatementListOSF);
  while (!IsNull (&FormSet->StatementListOSF, Link)) {
    Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
    Link      = GetNextNode (&FormSet->StatementListOSF, Link);
    if (Statement->StatementId >= MaxStatementId) {
      MaxStatementId = Statement->StatementId;
    }
  }

  return (MaxStatementId + 1);
}

/**
 Get available question ID from formset.

 @param[in] FormSet             Pointer to formset.

 @return Question ID or zero if there is no available question ID.
**/
EFI_QUESTION_ID
GetAvailableQuestionId (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  LIST_ENTRY                                  *FormLink;
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_STATEMENT                      *Statement;
  FORM_BROWSER_FORM                           *Form;
  EFI_QUESTION_ID                             QuestionId;
  BOOLEAN                                     IsValid;

  for (QuestionId = 1; QuestionId < 0xFFFF; QuestionId++) {
    IsValid = TRUE;

    FormLink = GetFirstNode (&FormSet->FormListHead);
    while (!IsNull (&FormSet->FormListHead, FormLink) && IsValid) {
      Form     = FORM_BROWSER_FORM_FROM_LINK (FormLink);
      FormLink = GetNextNode (&FormSet->FormListHead, FormLink);

      Link = GetFirstNode (&Form->StatementListHead);
      while (!IsNull (&Form->StatementListHead, Link)) {
        Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
        Link      = GetNextNode (&Form->StatementListHead, Link);
        if (Statement->QuestionId == QuestionId) {
          IsValid = FALSE;
          break;
        }
      }
    }

    Link = GetFirstNode (&FormSet->StatementListOSF);
    while (!IsNull (&FormSet->StatementListOSF, Link) && IsValid) {
      Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
      Link      = GetNextNode (&FormSet->StatementListOSF, Link);
      if (Statement->QuestionId == QuestionId) {
        IsValid = FALSE;
        break;
      }
    }

    if (IsValid) {
      return QuestionId;
    }
  }

  return 0;
}

/**
 Get available fake question ID from formset.

 @param[in] FormSet             Pointer to formset.

 @return Fake question ID.
**/
EFI_QUESTION_ID
GetAvailableFakeQuestionId (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  EFI_QUESTION_ID                             FakeQuestionId;
  LIST_ENTRY                                  *FormLink;
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_STATEMENT                      *Statement;
  FORM_BROWSER_FORM                           *Form;

  FakeQuestionId = 0;
  FormLink = GetFirstNode (&FormSet->FormListHead);
  while (!IsNull (&FormSet->FormListHead, FormLink)) {
    Form     = FORM_BROWSER_FORM_FROM_LINK (FormLink);
    FormLink = GetNextNode (&FormSet->FormListHead, FormLink);

    Link = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, Link)) {
      Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
      Link      = GetNextNode (&Form->StatementListHead, Link);
      if (Statement->FakeQuestionId >= FakeQuestionId) {
        FakeQuestionId = Statement->FakeQuestionId;
      }
    }
  }

  Link = GetFirstNode (&FormSet->StatementListOSF);
  while (!IsNull (&FormSet->StatementListOSF, Link)) {
    Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
    Link      = GetNextNode (&FormSet->StatementListOSF, Link);
    if (Statement->FakeQuestionId >= FakeQuestionId) {
      FakeQuestionId = Statement->FakeQuestionId;
    }
  }

  return (FakeQuestionId + 1);
}

/**
 Based on destination question ID of import info, get question ID.

 @param[in] DstFormSet          Pointer to formset.
 @param[in] ImportInfo          Pointer to question import info.

 @return Question ID.
**/
EFI_QUESTION_ID
GetQuestionId (
  IN FORM_BROWSER_FORMSET                     *DstFormSet,
  IN IMPORT_INFO                              *ImportInfo
  )
{
  switch (ImportInfo->DstId) {

  case H2O_IFR_GUID_IMPORT_LOCAL_ID_USE_TARGET_ID:
    if (ImportInfo->SrcId != 0 && GetStatementByQuestionId (DstFormSet, NULL, ImportInfo->SrcId) == NULL) {
      return ImportInfo->SrcId;
    }
    return GetAvailableQuestionId (DstFormSet);

  case H2O_IFR_GUID_IMPORT_LOCAL_ID_NON_CONFLICTING:
    return GetAvailableQuestionId (DstFormSet);

  default:
    return ImportInfo->DstId;
  }
}

/**
 Based on source statement, get question ID.

 @param[in] SrcFormSet          Pointer to source formset.
 @param[in] SrcStatement        Pointer to source statement.
 @param[in] DstFormSet          Pointer to destination formset.

 @return Question ID or zero if input parameter is NULL.
**/
EFI_QUESTION_ID
GetQuestionIdBySrcStatement (
  IN FORM_BROWSER_FORMSET                     *SrcFormSet,
  IN FORM_BROWSER_STATEMENT                   *SrcStatement,
  IN FORM_BROWSER_FORMSET                     *DstFormSet
  )
{
  UINT16                                      Index;
  FORM_BROWSER_STATEMENT                      *DstStatement;
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_STATEMENT_BUFFER               *FBStatementBuffer;
  FORM_BROWSER_STATEMENT                      *Statement;

  if (SrcFormSet == NULL || SrcStatement == NULL) {
    return 0;
  }

  for (Index = 0; Index < mImportInfoCount; Index++) {
    if (IMPORT_INFO_FLAG_IS_QUESTION(mImportInfoList[Index].Flags) &&
        mImportInfoList[Index].SrcId == SrcStatement->QuestionId &&
        CompareGuid (&mImportInfoList[Index].SrcFormSetGuid, &SrcFormSet->Guid)) {
      mImportInfoList[Index].DstId = GetQuestionId (DstFormSet, &mImportInfoList[Index]);
      return mImportInfoList[Index].DstId;
    }
  }

  Link = GetFirstNode (&DstFormSet->FBStatementBufferListHead);
  while (!IsNull (&DstFormSet->FBStatementBufferListHead, Link)) {
    FBStatementBuffer = FORM_BROWSER_STATEMENT_BUFFER_FROM_LINK (Link);
    Link              = GetNextNode (&DstFormSet->FBStatementBufferListHead, Link);

    for (Index = 0; Index < FBStatementBuffer->StatementBufferListIndex; Index++) {
      Statement = &FBStatementBuffer->StatementBufferList[Index];
      if (Statement->ImportInfo != NULL &&
          Statement->ImportInfo->SrcStatement != NULL &&
          Statement->ImportInfo->SrcStatement->QuestionId == SrcStatement->QuestionId &&
          Statement->ImportInfo->SrcFormSet != NULL &&
          Statement->ImportInfo->SrcFormSet->HiiHandle == SrcFormSet->HiiHandle &&
          CompareGuid (&Statement->ImportInfo->SrcFormSet->Guid, &SrcFormSet->Guid)) {
        return Statement->QuestionId;
      }
    }
  }

  DstStatement = CreateDisabledStatementInLastForm (SrcFormSet, SrcStatement, DstFormSet);
  if (DstStatement != NULL) {
    return DstStatement->QuestionId;
  }

  return 0;
}

/**
 Get statement by the position in the form.

 @param[in] Form                Pointer to form.
 @param[in] Position            The position in the form.

 @return Statement pointer or NULL if input parameter is NULL or not found.
**/
FORM_BROWSER_STATEMENT *
GetStatementByPosition (
  IN FORM_BROWSER_FORM                        *Form,
  IN UINT16                                   Position
  )
{
  UINT16                                      Count;
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_STATEMENT                      *Statement;

  if (Form == NULL) {
    return NULL;
  }

  Count = 0;
  Link  = GetFirstNode (&Form->StatementListHead);
  while (!IsNull (&Form->StatementListHead, Link)) {
    Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
    if (Count == Position) {
      return Statement;
    }

    Link = GetNextNode (&Form->StatementListHead, Link);
    Count++;
  }

  return NULL;
}

/**
 Get statement by the question ID in the form or formset.

 @param[in] FormSet             Pointer to formset.
 @param[in] Form                Pointer to form.
 @param[in] QuestionId          Query question ID.

 @return Statement pointer or NULL if invalid input parameter or not found.
**/
FORM_BROWSER_STATEMENT *
GetStatementByQuestionId (
  IN FORM_BROWSER_FORMSET                     *FormSet,
  IN FORM_BROWSER_FORM                        *Form OPTIONAL,
  IN EFI_QUESTION_ID                          QuestionId
  )
{
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_STATEMENT                      *Statement;
  LIST_ENTRY                                  *FormLink;
  FORM_BROWSER_FORM                           *FormPtr;

  if (FormSet == NULL || QuestionId == 0) {
    return NULL;
  }

  if (Form != NULL) {
    Link = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, Link)) {
      Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
      if (Statement->QuestionId  == QuestionId) {
        return Statement;
      }

      Link = GetNextNode (&Form->StatementListHead, Link);
    }
  }

  FormLink = GetFirstNode (&FormSet->FormListHead);
  while (!IsNull (&FormSet->FormListHead, FormLink)) {
    FormPtr = FORM_BROWSER_FORM_FROM_LINK (FormLink);

    Link = GetFirstNode (&FormPtr->StatementListHead);
    while (!IsNull (&FormPtr->StatementListHead, Link)) {
      Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
      if (Statement->QuestionId == QuestionId) {
        return Statement;
      }

      Link = GetNextNode (&FormPtr->StatementListHead, Link);
    }

    FormLink = GetNextNode (&FormSet->FormListHead, FormLink);
  }

  return NULL;
}

/**
 Get form by the question ID.

 @param[in] FormSet             Pointer to formset.
 @param[in] QuestionId          Query question ID.

 @return Form pointer or NULL if invalid input parameter or not found.
**/
STATIC
FORM_BROWSER_FORM *
GetFormByQuestionId (
  IN FORM_BROWSER_FORMSET                     *FormSet,
  IN EFI_QUESTION_ID                          QuestionId
  )
{
  LIST_ENTRY                                  *FormLink;
  FORM_BROWSER_FORM                           *Form;
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_STATEMENT                      *Statement;

  if (FormSet == NULL || QuestionId == 0) {
    return NULL;
  }

  for (FormLink = GetFirstNode (&FormSet->FormListHead);
       !IsNull (&FormSet->FormListHead, FormLink);
       FormLink = GetNextNode (&FormSet->FormListHead, FormLink)) {
    Form = FORM_BROWSER_FORM_FROM_LINK (FormLink);

    for (Link = GetFirstNode (&Form->StatementListHead);
         !IsNull (&Form->StatementListHead, Link);
         Link = GetNextNode (&Form->StatementListHead, Link)) {
      Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
      if (Statement->QuestionId == QuestionId) {
        return Form;
      }
    }
  }

  return NULL;
}

/**
 Get new statement from pre-allocated buffer of formset.

 @param[in] FormSet             Pointer to formset.

 @return Statement or NULL if input parameter is NULL or fail to allocate pool.
**/
FORM_BROWSER_STATEMENT *
GetNewStatement (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_STATEMENT_BUFFER               *FBStatementBuffer;
  FORM_BROWSER_STATEMENT                      *Statement;

  if (FormSet == NULL) {
    return NULL;
  }

  if (FormSet->FBStatementBufferListHead.ForwardLink == NULL) {
    InitializeListHead (&FormSet->FBStatementBufferListHead);
  }

  Link = GetFirstNode (&FormSet->FBStatementBufferListHead);
  while (!IsNull (&FormSet->FBStatementBufferListHead, Link)) {
    FBStatementBuffer = FORM_BROWSER_STATEMENT_BUFFER_FROM_LINK (Link);
    Link              = GetNextNode (&FormSet->FBStatementBufferListHead, Link);
    if (FBStatementBuffer->StatementBufferListIndex < FBStatementBuffer->StatementBufferListCount) {
      Statement = &FBStatementBuffer->StatementBufferList[FBStatementBuffer->StatementBufferListIndex++];
      return Statement;
    }
  }

  FBStatementBuffer = AllocateZeroPool (sizeof (FORM_BROWSER_STATEMENT_BUFFER));
  if (FBStatementBuffer == NULL) {
    return NULL;
  }
  FBStatementBuffer->StatementBufferListCount = 100;
  FBStatementBuffer->StatementBufferList      = AllocateZeroPool (sizeof (FORM_BROWSER_STATEMENT) * FBStatementBuffer->StatementBufferListCount);
  if (FBStatementBuffer->StatementBufferList == NULL) {
    FreePool (FBStatementBuffer);
    return NULL;
  }
  FBStatementBuffer->Signature = FORM_BROWSER_STATEMENT_BUFFER_SIGNATURE;
  InsertTailList (&FormSet->FBStatementBufferListHead, &FBStatementBuffer->Link);

  Statement = &FBStatementBuffer->StatementBufferList[FBStatementBuffer->StatementBufferListIndex++];
  return Statement;
}

/**
 Get new expression opcode from pre-allocated buffer of formset.

 @param[in] FormSet             Pointer to formset.

 @return Expression opcode or NULL if input parameter is NULL or fail to allocate pool.
**/
EXPRESSION_OPCODE *
GetNewExpressionOpCode (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_EXPRESSION_BUFFER              *FBExpressionBuffer;
  EXPRESSION_OPCODE                           *ExpressionOpCode;

  if (FormSet == NULL) {
    return NULL;
  }

  if (FormSet->FBExpressionBufferListHead.ForwardLink == NULL) {
    InitializeListHead (&FormSet->FBExpressionBufferListHead);
  }

  Link = GetFirstNode (&FormSet->FBExpressionBufferListHead);
  while (!IsNull (&FormSet->FBExpressionBufferListHead, Link)) {
    FBExpressionBuffer = FORM_BROWSER_EXPRESSION_BUFFER_FROM_LINK (Link);
    Link               = GetNextNode (&FormSet->FBExpressionBufferListHead, Link);
    if (FBExpressionBuffer->ExpressionBufferListIndex < FBExpressionBuffer->ExpressionBufferListCount) {
      ExpressionOpCode = &FBExpressionBuffer->ExpressionBufferList[FBExpressionBuffer->ExpressionBufferListIndex++];
      return ExpressionOpCode;
    }
  }

  FBExpressionBuffer = AllocateZeroPool (sizeof (FORM_BROWSER_EXPRESSION_BUFFER));
  if (FBExpressionBuffer == NULL) {
    return NULL;
  }
  FBExpressionBuffer->ExpressionBufferListCount = 100;
  FBExpressionBuffer->ExpressionBufferList      = AllocateZeroPool (sizeof (EXPRESSION_OPCODE) * FBExpressionBuffer->ExpressionBufferListCount);
  if (FBExpressionBuffer->ExpressionBufferList == NULL) {
    FreePool (FBExpressionBuffer);
    return NULL;
  }
  FBExpressionBuffer->Signature = FORM_BROWSER_EXPRESSION_BUFFER_SIGNATURE;
  InsertTailList (&FormSet->FBExpressionBufferListHead, &FBExpressionBuffer->Link);

  ExpressionOpCode = &FBExpressionBuffer->ExpressionBufferList[FBExpressionBuffer->ExpressionBufferListIndex++];
  return ExpressionOpCode;
}

/**
 Get corresponding formset storage by input question.

 @param[in] FormSet             Pointer to formset.
 @param[in] Question            Pointer to question.

 @return Formset storage pointer or NULL if invalid input parameter or not found.
**/
FORMSET_STORAGE *
GetStorageByQuestion (
  IN FORM_BROWSER_FORMSET                     *FormSet,
  IN FORM_BROWSER_STATEMENT                   *Question

  )
{
  LIST_ENTRY                                  *Link;
  FORMSET_STORAGE                             *Storage;

  if (FormSet == NULL || Question == NULL) {
    return NULL;
  }

  Link = GetFirstNode (&FormSet->StorageListHead);
  while (!IsNull (&FormSet->StorageListHead, Link)) {
    Storage = FORMSET_STORAGE_FROM_LINK (Link);
    if (Storage->VarStoreId == Question->VarStoreId) {
      return Storage;
    }

    Link = GetNextNode (&FormSet->StorageListHead, Link);
  }

  return NULL;
}

/**
 Get formset by formset GUID.

 @param[in] FormSetGuid         Pointer to formset GUID.
 @param[in] ...                 A list of formset. A NULL terminates the list.

 @return Formset pointer or NULL if invalid input parameter or not found.
**/
FORM_BROWSER_FORMSET *
GetFormSetByFormSetGuid (
  IN EFI_GUID                                 *FormSetGuid,
  ...
  )
{
  VA_LIST                                     Args;
  LIST_ENTRY                                  *FormSetList;
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_FORMSET                        *FormSet;

  if (FormSetGuid == NULL) {
    return NULL;
  }

  ZeroMem (&Args, sizeof (Args));
  VA_START (Args, FormSetGuid);
  while ((FormSetList = VA_ARG (Args, LIST_ENTRY *)) != NULL) {
    Link = GetFirstNode (FormSetList);

    while (!IsNull (FormSetList, Link)) {
      FormSet = FORM_BROWSER_FORMSET_FROM_LINK (Link);
      Link    = GetNextNode (FormSetList, Link);
      if (!ValidateFormSet(FormSet)) {
        continue;
      }

      if (CompareGuid (FormSetGuid, &FormSet->Guid)) {
        return FormSet;
      }
    }
  }

  return NULL;
}

/**
 Get source data (formset/form/statement/storage) by import info.

 @param[in]  ImportInfo          Pointer to import info.
 @param[out] SrcFormSet          Double pointer to source formset.
 @param[out] SrcForm             Double pointer to source forms.
 @param[out] SrcStatement        Double pointer to source statement.
 @param[out] SrcStorage          Double pointer to source storage.

 @retval EFI_SUCCESS             Successfully get source data
 @retval EFI_NOT_FOUND           Can not find source data
**/
EFI_STATUS
GetSrcData (
  IN  IMPORT_INFO                             *ImportInfo,
  OUT FORM_BROWSER_FORMSET                    **SrcFormSet,
  OUT FORM_BROWSER_FORM                       **SrcForm,
  OUT FORM_BROWSER_STATEMENT                  **SrcStatement,
  OUT FORMSET_STORAGE                         **SrcStorage
  )
{
  FORM_BROWSER_FORMSET                        *FormSet;
  FORM_BROWSER_FORM                           *Form;
  FORM_BROWSER_STATEMENT                      *Statement;
  FORMSET_STORAGE                             *Storage;

  FormSet = GetFormSetByFormSetGuid (
              &ImportInfo->SrcFormSetGuid,
              &mH2OBrowserImportFormSetList,
              &gBrowserFormSetList,
              NULL
              );
  if (FormSet == NULL) {
    return EFI_NOT_FOUND;
  }

  Form = IdToForm (FormSet, ImportInfo->SrcFormId);
  if (Form == NULL) {
    return EFI_NOT_FOUND;
  }

  if (IS_LINK_TARGET (ImportInfo->Type)) {
    Statement = NULL;
    Storage   = NULL;
  } else {
    if (IMPORT_INFO_FLAG_IS_QUESTION (ImportInfo->Flags)) {
      Statement = GetStatementByQuestionId (FormSet, Form, ImportInfo->SrcId);
    } else {
      Statement = GetStatementByPosition (Form, ImportInfo->SrcId);
    }
    if (Statement == NULL) {
      return EFI_NOT_FOUND;
    }

    if (Statement->QuestionId != 0 && Statement->VarStoreId != 0) {
      Storage = GetStorageByQuestion (FormSet, Statement);
      if (Storage == NULL) {
        return EFI_NOT_FOUND;
      }
    } else {
      Storage = NULL;
    }
  }

  *SrcFormSet   = FormSet;
  *SrcForm      = Form;
  *SrcStatement = Statement;
  *SrcStorage   = Storage;
  return EFI_SUCCESS;
}

/**
 Get corresponding formset storage by input browser storage.

 @param[in] FormSet             Pointer to formset.
 @param[in] BrowserStorage      Pointer to browser storage.

 @return Formset storage pointer or NULL if not found.
**/
FORMSET_STORAGE *
GetFormSetStorage (
  IN FORM_BROWSER_FORMSET                     *FormSet,
  IN BROWSER_STORAGE                          *BrowserStorage
  )
{
  LIST_ENTRY                                  *Link;
  FORMSET_STORAGE                             *FormSetStorage;

  Link = GetFirstNode (&FormSet->StorageListHead);
  while (!IsNull (&FormSet->StorageListHead, Link)) {
    FormSetStorage = FORMSET_STORAGE_FROM_LINK (Link);
    if (FormSetStorage->BrowserStorage == BrowserStorage) {
      return FormSetStorage;
    }
    Link = GetNextNode (&FormSet->StorageListHead, Link);
  }

  return NULL;
}

/**
 Create imported formset by formset GUID.

 @param[in] ImportFormsetGuid   Pointer to formset GUID.

 @return Formset pointer or NULL if formset is not found or fail to allocate pool.
**/
FORM_BROWSER_FORMSET *
CreateImportFormSet (
  IN EFI_GUID                                 *ImportFormsetGuid
  )
{
  EFI_STATUS                                  Status;
  EFI_HII_HANDLE                              HiiHandle;
  FORM_BROWSER_FORMSET                        *ImportFormSet;

  HiiHandle = HiiGetHiiHandleByFormSetGuid (ImportFormsetGuid);
  if (HiiHandle == NULL) {
    return NULL;
  }

  ImportFormSet = AllocateZeroPool (sizeof (FORM_BROWSER_FORMSET));
  if (ImportFormSet == NULL) {
    return NULL;
  }

  Status = InitializeFormSet (HiiHandle, ImportFormsetGuid, ImportFormSet);
  if (EFI_ERROR (Status) || IsListEmpty (&ImportFormSet->FormListHead)) {
    DestroyFormSet (ImportFormSet);
    return NULL;
  }
  InsertTailList (&mH2OBrowserImportFormSetList, &ImportFormSet->Link);

  //
  // Remove imported formset from mFBPrivate.FormSetList because it may has duplicate formset in list.
  //
  RemoveEntryList (&ImportFormSet->DisplayLink);
  InitializeListHead (&ImportFormSet->DisplayLink);

  return ImportFormSet;
}

/**
 Create imported formset storage.

 @param[in]      SrcFormSet          Pointer to source formset.
 @param[in]      SrcFormSetStorage   Pointer to source formset storage.
 @param[in, out] DstFormSet          Pointer to destination formset.

 @return Formset storage pointer or NULL if fail to allocate pool.
**/
FORMSET_STORAGE *
CreateImportStorage (
  IN     FORM_BROWSER_FORMSET                 *SrcFormSet,
  IN     FORMSET_STORAGE                      *SrcFormSetStorage,
  IN OUT FORM_BROWSER_FORMSET                 *DstFormSet
  )
{
  FORMSET_STORAGE                             *DstFormSetStorage;
  BROWSER_STORAGE                             *SrcBrowserStorage;

  DstFormSetStorage = AllocateZeroPool (sizeof (FORMSET_STORAGE));
  if (DstFormSetStorage == NULL) {
    return NULL;
  }
  DstFormSetStorage->Signature = FORMSET_STORAGE_SIGNATURE;
  InsertTailList (&DstFormSet->StorageListHead, &DstFormSetStorage->Link);

  SrcBrowserStorage                 = SrcFormSetStorage->BrowserStorage;
  DstFormSetStorage->BrowserStorage = SrcBrowserStorage;
  DstFormSetStorage->ConfigRequest  = AllocateCopyPool (StrSize (SrcBrowserStorage->ConfigHdr), SrcBrowserStorage->ConfigHdr);
  DstFormSetStorage->SpareStrLen    = 0;
  DstFormSetStorage->VarStoreId     = GetAvailableVarstoreId (DstFormSet);;

  return DstFormSetStorage;
}

/**
 Create statement with disableif TRUE expression in last form of formset.

 @param[in] SrcFormSet          Pointer to source formset.
 @param[in] SrcStatement        Pointer to source statement.
 @param[in] DstFormSet          Pointer to destination formset.

 @return Statement pointer or NULL if invalid parameter or fail to get question info.
**/
FORM_BROWSER_STATEMENT *
CreateDisabledStatementInLastForm (
  IN FORM_BROWSER_FORMSET                     *SrcFormSet,
  IN FORM_BROWSER_STATEMENT                   *SrcStatement,
  IN FORM_BROWSER_FORMSET                     *DstFormSet
  )
{
  LIST_ENTRY                                  *Link;
  FORMSET_STORAGE                             *SrcStorage;
  FORM_BROWSER_FORM                           *DstLastForm;
  FORM_BROWSER_STATEMENT                      *DstStatement;
  FORMSET_STORAGE                             *DstStorage;

  if (SrcFormSet == NULL || SrcStatement == NULL || DstFormSet == NULL) {
    return NULL;
  }

  SrcStorage = GetStorageByQuestion (SrcFormSet, SrcStatement);
  if (SrcStorage == NULL) {
    return NULL;
  }

  if (IsListEmpty (&DstFormSet->FormListHead)) {
    return NULL;
  }

  DstLastForm = NULL;
  Link = GetFirstNode (&DstFormSet->FormListHead);
  while (!IsNull (&DstFormSet->FormListHead, Link)) {
    DstLastForm = FORM_BROWSER_FORM_FROM_LINK (Link);
    Link        = GetNextNode (&DstFormSet->FormListHead, Link);
  }
  if (DstLastForm == NULL) {
    return NULL;
  }

  DstStatement = GetNewStatement (DstFormSet);
  if (DstStatement == NULL) {
    return NULL;
  }
  DstStatement->StatementId = mImportStatementId++;

  //
  // Copy Statement info
  //
  InitializeListHead (&DstStatement->DefaultListHead);
  InitializeListHead (&DstStatement->OptionListHead);
  InitializeListHead (&DstStatement->InconsistentListHead);
  InitializeListHead (&DstStatement->NoSubmitListHead);
  InitializeListHead (&DstStatement->WarningListHead);
  InitializeListHead (&DstStatement->PropertyListHead);

  DstStatement->Signature = SrcStatement->Signature;
  DstStatement->Operand   = SrcStatement->Operand;
  DstStatement->OpCode    = SrcStatement->OpCode;

  CopyHiiString (SrcFormSet->HiiHandle, SrcStatement->Prompt, DstFormSet->HiiHandle, &DstStatement->Prompt);
  CopyHiiString (SrcFormSet->HiiHandle, SrcStatement->Help  , DstFormSet->HiiHandle, &DstStatement->Help);

  DstStatement->Expression = AllocateZeroPool (sizeof(FORM_EXPRESSION_LIST));
  if (DstStatement->Expression == NULL) {
    return NULL;
  }
  DstStatement->Expression->Count         = 1;
  DstStatement->Expression->Signature     = FORM_EXPRESSION_LIST_SIGNATURE;
  DstStatement->Expression->Expression[0] = CreateDisableIfExpression (GetNewExpressionOpCode (DstFormSet));
  if (DstStatement->Expression->Expression[0] == NULL) {
    return NULL;
  }
  InsertTailList (&DstLastForm->ExpressionListHead, &DstStatement->Expression->Expression[0]->Link);

  //
  // Copy Question info
  //
  DstStatement->QuestionId             = GetAvailableQuestionId (DstFormSet);
  DstStatement->VarStoreInfo.VarOffset = SrcStatement->VarStoreInfo.VarOffset;
  DstStatement->QuestionFlags          = SrcStatement->QuestionFlags;

  if (SrcStatement->VarStoreId != 0) {
    DstStorage = GetFormSetStorage (DstFormSet, SrcStorage->BrowserStorage);
    if (DstStorage == NULL) {
      DstStorage = CreateImportStorage (SrcFormSet, SrcStorage, DstFormSet);
    }
    if (DstStorage == NULL) {
      return NULL;
    }

    DstStatement->Storage    = DstStorage->BrowserStorage;
    DstStatement->VarStoreId = DstStorage->VarStoreId;
  }

  //
  // Copy Question HII Value info
  //
  switch (SrcStatement->Operand) {

  case EFI_IFR_REF_OP:
    DstStatement->HiiValue.Type = EFI_IFR_TYPE_REF;

    if (SrcStatement->HiiValue.Value.ref.DevicePath != 0) {
      CopyHiiString (SrcFormSet->HiiHandle, SrcStatement->HiiValue.Value.ref.DevicePath, DstFormSet->HiiHandle, &DstStatement->HiiValue.Value.ref.DevicePath);
    }

    CopyMem (
      &DstStatement->HiiValue.Value.ref.FormSetGuid,
      &SrcStatement->HiiValue.Value.ref.FormSetGuid,
      sizeof (EFI_GUID)
      );

    if (DstStatement->HiiValue.Value.ref.DevicePath == 0 && CompareGuid (&DstStatement->HiiValue.Value.ref.FormSetGuid, &gZeroGuid)) {
      //
      // Originally, it is going into form or question in self formset.
      // But here, it should use REF3 to describe.
      //
      CopyMem (
        &DstStatement->HiiValue.Value.ref.FormSetGuid,
        &SrcFormSet->Guid,
        sizeof (EFI_GUID)
        );
    }
    DstStatement->HiiValue.Value.ref.FormId     = SrcStatement->HiiValue.Value.ref.FormId;
    DstStatement->HiiValue.Value.ref.QuestionId = SrcStatement->HiiValue.Value.ref.QuestionId;

    DstStatement->StorageWidth = SrcStatement->StorageWidth;
    InitializeRequestElement (DstFormSet, DstStatement, DstLastForm);
    break;

  case EFI_IFR_STRING_OP:
  case EFI_IFR_PASSWORD_OP:
    DstStatement->Minimum      = SrcStatement->Minimum;
    DstStatement->Maximum      = SrcStatement->Maximum;
    DstStatement->StorageWidth = SrcStatement->StorageWidth;
    DstStatement->Flags        = SrcStatement->Flags;

    DstStatement->HiiValue.Type         = EFI_IFR_TYPE_STRING;
    DstStatement->BufferValue           = AllocateZeroPool (DstStatement->StorageWidth + sizeof (CHAR16));
    DstStatement->HiiValue.Value.string = NewString ((CHAR16*) DstStatement->BufferValue, DstFormSet->HiiHandle);

    DstStatement->HiiValue.Buffer    = DstStatement->BufferValue;
    DstStatement->HiiValue.BufferLen = DstStatement->StorageWidth + sizeof (CHAR16);
    InitializeRequestElement (DstFormSet, DstStatement, DstLastForm);
    break;

  case EFI_IFR_CHECKBOX_OP:
    DstStatement->Flags         = SrcStatement->Flags;
    DstStatement->StorageWidth  = (UINT16) sizeof (BOOLEAN);
    DstStatement->HiiValue.Type = EFI_IFR_TYPE_BOOLEAN;
    InitializeRequestElement (DstFormSet, DstStatement, DstLastForm);
    break;

  case EFI_IFR_DATE_OP:
    DstStatement->Flags         = SrcStatement->Flags;
    DstStatement->HiiValue.Type = EFI_IFR_TYPE_DATE;

    if ((DstStatement->Flags & EFI_QF_DATE_STORAGE) == QF_DATE_STORAGE_NORMAL) {
      DstStatement->StorageWidth = (UINT16) sizeof (EFI_HII_DATE);

      InitializeRequestElement (DstFormSet, DstStatement, DstLastForm);
    } else {
      //
      // Don't assign storage for RTC type of date/time
      //
      DstStatement->Storage      = NULL;
      DstStatement->StorageWidth = 0;
    }
    break;

  case EFI_IFR_TIME_OP:
    DstStatement->Flags         = SrcStatement->Flags;
    DstStatement->HiiValue.Type = EFI_IFR_TYPE_TIME;

    if ((DstStatement->Flags & QF_TIME_STORAGE) == QF_TIME_STORAGE_NORMAL) {
      DstStatement->StorageWidth = (UINT16) sizeof (EFI_HII_TIME);

      InitializeRequestElement (DstFormSet, DstStatement, DstLastForm);
    } else {
      //
      // Don't assign storage for RTC type of date/time
      //
      DstStatement->Storage      = NULL;
      DstStatement->StorageWidth = 0;
    }
    break;

  case EFI_IFR_ONE_OF_OP:
  case EFI_IFR_NUMERIC_OP:
    DstStatement->Flags         = SrcStatement->Flags;
    DstStatement->Minimum       = SrcStatement->Minimum;
    DstStatement->Maximum       = SrcStatement->Maximum;
    DstStatement->Step          = SrcStatement->Step;
    DstStatement->StorageWidth  = SrcStatement->StorageWidth;
    DstStatement->HiiValue.Type = SrcStatement->HiiValue.Type;

    InitializeRequestElement (DstFormSet, DstStatement, DstLastForm);
    break;

  case EFI_IFR_ORDERED_LIST_OP:
    DstStatement->Flags         = SrcStatement->Flags;
    DstStatement->MaxContainers = SrcStatement->MaxContainers;
    DstStatement->HiiValue.Type = EFI_IFR_TYPE_BUFFER;
    DstStatement->BufferValue   = NULL;

    if (SrcStatement->BufferValue != NULL) {
      DstStatement->StorageWidth = SrcStatement->StorageWidth;
      DstStatement->BufferValue  = AllocateZeroPool (SrcStatement->StorageWidth);
      DstStatement->ValueType    = SrcStatement->ValueType;
      if (DstStatement->HiiValue.Type == EFI_IFR_TYPE_BUFFER) {
        DstStatement->HiiValue.Buffer    = DstStatement->BufferValue;
        DstStatement->HiiValue.BufferLen = DstStatement->StorageWidth;
      }

      InitializeRequestElement (DstFormSet, DstStatement, DstLastForm);
    }
    break;

  default:
    break;
  }

  DstStatement->ImportInfo = AllocateZeroPool (sizeof (FB_IMPORT_INFO));
  if (DstStatement->ImportInfo == NULL) {
    return NULL;
  }
  DstStatement->ImportInfo->SrcFormSet   = SrcFormSet;
  DstStatement->ImportInfo->SrcForm      = GetFormByQuestionId(SrcFormSet, SrcStatement->QuestionId);
  DstStatement->ImportInfo->SrcStatement = SrcStatement;

  InsertTailList (&DstLastForm->StatementListHead, &DstStatement->Link);
  DstLastForm->NumberOfStatement++;

  return DstStatement;
}

/**
 Copy expression opcode.

 @param[in] SrcFormSet            Pointer to source formset.
 @param[in] SrcExpressionOpCode   Pointer to source expression opcode.
 @param[in] DstFormSet            Pointer to destination formset.
 @param[in] DstForm               Pointer to destination form.
 @param[in] DstExpressionOpCode   Pointer to destination expression opcode.

 @retval EFI_SUCCESS              Successfully copy expression opcode
 @retval EFI_INVALID_PARAMETER    Input parameter is NULL
 @retval EFI_NOT_FOUND            Fail to find source expression data
**/
EFI_STATUS
CopyExpressionOpCode (
  IN  FORM_BROWSER_FORMSET                    *SrcFormSet,
  IN  EXPRESSION_OPCODE                       *SrcExpressionOpCode,
  IN  FORM_BROWSER_FORMSET                    *DstFormSet,
  IN  FORM_BROWSER_FORM                       *DstForm,
  OUT EXPRESSION_OPCODE                       *DstExpressionOpCode
  )
{
  FORM_BROWSER_STATEMENT                      *SrcStatement;
  FORM_BROWSER_STATEMENT                      *SrcStatement2;
  EFI_QUESTION_ID                             DstQuestionId;
  EFI_QUESTION_ID                             DstQuestionId2;
  FORMSET_STORAGE                             *DstStorage;
  FORMSET_STORAGE                             *SrcStorage;
  LIST_ENTRY                                  *Link;
  FORM_EXPRESSION                             *SrcSubExpression;
  FORM_EXPRESSION                             *DstSubExpression;
  EFI_STATUS                                  Status;

  if (SrcFormSet == NULL || SrcExpressionOpCode == NULL || DstFormSet == NULL || DstForm == NULL || DstExpressionOpCode == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DstQuestionId  = 0;
  DstQuestionId2 = 0;

  if (SrcExpressionOpCode->QuestionId != 0) {
    SrcStatement = GetStatementByQuestionId (SrcFormSet, NULL, SrcExpressionOpCode->QuestionId);
    if (SrcStatement == NULL) {
      return EFI_NOT_FOUND;
    }
    DstQuestionId = GetQuestionIdBySrcStatement (SrcFormSet, SrcStatement, DstFormSet);
    if (DstQuestionId == 0) {
      return EFI_NOT_FOUND;
    }
  }
  if (SrcExpressionOpCode->QuestionId2 != 0) {
    SrcStatement2 = GetStatementByQuestionId (SrcFormSet, NULL, SrcExpressionOpCode->QuestionId2);
    if (SrcStatement2 == NULL) {
      return EFI_NOT_FOUND;
    }
    DstQuestionId2 = GetQuestionIdBySrcStatement (SrcFormSet, SrcStatement2, DstFormSet);
    if (DstQuestionId2 == 0) {
      return EFI_NOT_FOUND;
    }
  }

  CopyMem (DstExpressionOpCode, SrcExpressionOpCode, sizeof (EXPRESSION_OPCODE));

  switch (SrcExpressionOpCode->Operand) {

  case EFI_IFR_EQ_ID_VAL_OP:
    if (SrcFormSet->HiiHandle == DstFormSet->HiiHandle) {
      break;
    }
    DstExpressionOpCode->QuestionId = DstQuestionId;
    break;

  case EFI_IFR_EQ_ID_ID_OP:
    DstExpressionOpCode->QuestionId  = DstQuestionId;
    DstExpressionOpCode->QuestionId2 = DstQuestionId2;
    break;

  case EFI_IFR_EQ_ID_VAL_LIST_OP:
    DstExpressionOpCode->QuestionId = DstQuestionId;
    DstExpressionOpCode->ValueList  = AllocateCopyPool (SrcExpressionOpCode->ListLength * sizeof (UINT16), SrcExpressionOpCode->ValueList);
    break;

  case EFI_IFR_TO_STRING_OP:
  case EFI_IFR_FIND_OP:
    break;

  case EFI_IFR_STRING_REF1_OP:
    Status = CopyHiiString (SrcFormSet->HiiHandle, SrcExpressionOpCode->Value.Value.string, DstFormSet->HiiHandle, &DstExpressionOpCode->Value.Value.string);
    if (EFI_ERROR (Status)) {
      DstExpressionOpCode->Value.Value.string = 0;
    }
    break;

  case EFI_IFR_RULE_REF_OP:
  case EFI_IFR_SPAN_OP:
    break;

  case EFI_IFR_THIS_OP:
    DstExpressionOpCode->QuestionId = DstQuestionId;
    break;

  case EFI_IFR_SECURITY_OP:
    break;

  case EFI_IFR_GET_OP:
  case EFI_IFR_SET_OP:
    if (SrcExpressionOpCode->VarStorage == NULL) {
      return EFI_ABORTED;
    }

    DstStorage = GetFormSetStorage (DstFormSet, SrcExpressionOpCode->VarStorage);
    if (DstStorage == NULL) {
      if (SrcFormSet->StorageListHead.ForwardLink != NULL) {
        Link = GetFirstNode (&SrcFormSet->StorageListHead);
        while (!IsNull (&SrcFormSet->StorageListHead, Link)) {
          SrcStorage = FORMSET_STORAGE_FROM_LINK (Link);
          if ((UINTN) SrcStorage->BrowserStorage == (UINTN) SrcExpressionOpCode->VarStorage) {
            DstStorage = CreateImportStorage (SrcFormSet, SrcStorage, DstFormSet);
            break;
          }
          Link = GetNextNode (&SrcFormSet->StorageListHead, Link);
        }
      }

    }
    if (DstStorage == NULL) {
      return EFI_NOT_FOUND;
    }
    DstExpressionOpCode->VarStorage = DstStorage->BrowserStorage;

    if (SrcExpressionOpCode->VarStoreInfo.VarName != 0) {
      CopyHiiString (SrcFormSet->HiiHandle, SrcExpressionOpCode->VarStoreInfo.VarName, DstFormSet->HiiHandle, &DstExpressionOpCode->VarStoreInfo.VarName);
    }
    if (SrcExpressionOpCode->ValueName != NULL) {
      DstExpressionOpCode->ValueName = AllocateCopyPool (StrSize (SrcExpressionOpCode->ValueName), SrcExpressionOpCode->ValueName);
    }
    break;

  case EFI_IFR_QUESTION_REF1_OP:
    DstExpressionOpCode->QuestionId = DstQuestionId;
    break;


  case EFI_IFR_QUESTION_REF3_OP:
    CopyHiiString (SrcFormSet->HiiHandle, SrcExpressionOpCode->DevicePath, DstFormSet->HiiHandle, &DstExpressionOpCode->DevicePath);
    break;

  case EFI_IFR_TRUE_OP:
  case EFI_IFR_FALSE_OP:
  case EFI_IFR_ONE_OP:
  case EFI_IFR_ZERO_OP:
  case EFI_IFR_ONES_OP:
  case EFI_IFR_UINT8_OP:
  case EFI_IFR_UINT16_OP:
  case EFI_IFR_UINT32_OP:
  case EFI_IFR_UINT64_OP:
  case EFI_IFR_UNDEFINED_OP:
  case EFI_IFR_VERSION_OP:
  default:
    break;
  }

  if (SrcExpressionOpCode->MapExpressionList.ForwardLink != NULL &&
      SrcExpressionOpCode->MapExpressionList.BackLink    != NULL &&
      !IsListEmpty (&SrcExpressionOpCode->MapExpressionList)) {
    InitializeListHead (&DstExpressionOpCode->MapExpressionList);

    Link = GetFirstNode(&SrcExpressionOpCode->MapExpressionList);
    while (!IsNull (&SrcExpressionOpCode->MapExpressionList, Link)) {
      SrcSubExpression = FORM_EXPRESSION_FROM_LINK (Link);
      Link             = GetNextNode (&SrcExpressionOpCode->MapExpressionList, Link);

      Status = CopyExpression (
                 SrcFormSet,
                 SrcSubExpression,
                 DstFormSet,
                 DstForm,
                 &DstSubExpression
                 );
      if (!EFI_ERROR (Status)) {
        InsertTailList (&DstExpressionOpCode->MapExpressionList, &DstSubExpression->Link);
      }
    }
  }

  return EFI_SUCCESS;
}

/**
 Copy expression.

 @param[in] SrcFormSet            Pointer to source formset.
 @param[in] SrcExpression         Pointer to source expression.
 @param[in] DstFormSet            Pointer to destination formset.
 @param[in] DstForm               Pointer to destination form.
 @param[in] DstExpression         Double pointer to destination expression.

 @retval EFI_SUCCESS              Successfully copy expression
 @retval EFI_OUT_OF_RESOURCES     Fail to allocate pool
**/
EFI_STATUS
CopyExpression (
  IN  FORM_BROWSER_FORMSET                    *SrcFormSet,
  IN     FORM_EXPRESSION                      *SrcExpression,
  IN     FORM_BROWSER_FORMSET                 *DstFormSet,
  IN     FORM_BROWSER_FORM                    *DstForm,
  IN OUT FORM_EXPRESSION                      **DstExpression
  )
{
  LIST_ENTRY                                  *Link;
  FORM_EXPRESSION                             *Expression;
  EXPRESSION_OPCODE                           *SrcExpressionOpCode;
  EXPRESSION_OPCODE                           *DstExpressionOpCode;
  EFI_STATUS                                  Status;

  Expression = AllocateCopyPool (sizeof (FORM_EXPRESSION), SrcExpression);
  if (Expression == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  InitializeListHead (&Expression->OpCodeListHead);
  if (SrcExpression->Error != 0) {
    CopyHiiString (SrcFormSet->HiiHandle, SrcExpression->Error, DstFormSet->HiiHandle, &Expression->Error);
  }

  Link = GetFirstNode (&SrcExpression->OpCodeListHead);
  while (!IsNull (&SrcExpression->OpCodeListHead, Link)) {
    SrcExpressionOpCode = EXPRESSION_OPCODE_FROM_LINK (Link);
    Link                = GetNextNode (&SrcExpression->OpCodeListHead, Link);

    DstExpressionOpCode = GetNewExpressionOpCode (DstFormSet);
    if (DstExpressionOpCode == NULL) {
      continue;
    }
    Status = CopyExpressionOpCode (SrcFormSet, SrcExpressionOpCode, DstFormSet, DstForm, DstExpressionOpCode);
    if (EFI_ERROR (Status)) {
      continue;
    }

    InsertTailList (&Expression->OpCodeListHead, &DstExpressionOpCode->Link);
  }

  *DstExpression = Expression;

  return EFI_SUCCESS;
}

/**
 Copy default list of statement.

 @param[in] SrcFormSet            Pointer to source formset.
 @param[in] SrcStatement          Pointer to source statement.
 @param[in] DstFormSet            Pointer to destination formset.
 @param[in] DstForm               Pointer to destination form.
 @param[in] DstStatement          Pointer to destination statement.

 @retval EFI_SUCCESS              Successfully copy default list
 @retval EFI_OUT_OF_RESOURCES     Fail to allocate pool
**/
EFI_STATUS
CopyDefaultList (
  IN  FORM_BROWSER_FORMSET                    *SrcFormSet,
  IN     FORM_BROWSER_STATEMENT               *SrcStatement,
  IN     FORM_BROWSER_FORMSET                 *DstFormSet,
  IN     FORM_BROWSER_FORM                    *DstForm,
  IN OUT FORM_BROWSER_STATEMENT               *DstStatement
  )
{
  LIST_ENTRY                                  *Link;
  QUESTION_DEFAULT                            *SrcDefault;
  QUESTION_DEFAULT                            *DstDefault;

  Link = GetFirstNode (&SrcStatement->DefaultListHead);
  while (!IsNull (&SrcStatement->DefaultListHead, Link)) {
    SrcDefault = QUESTION_DEFAULT_FROM_LINK (Link);
    Link       = GetNextNode (&SrcStatement->DefaultListHead, Link);

    DstDefault = AllocateCopyPool (sizeof (QUESTION_DEFAULT), SrcDefault);
    if (DstDefault == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    if (SrcDefault->ValueExpression != NULL) {
      CopyExpression (SrcFormSet, SrcDefault->ValueExpression, DstFormSet, DstForm, &DstDefault->ValueExpression);
      InsertTailList (&DstForm->ExpressionListHead, &DstDefault->ValueExpression->Link);
    }

    InsertTailList (&DstStatement->DefaultListHead, &DstDefault->Link);
  }

  return EFI_SUCCESS;
}

/**
 Copy common expression list (inconsistent/warningif/nosubmitif) of statement.

 @param[in] Type                  Expression type.
 @param[in] SrcFormSet            Pointer to source formset.
 @param[in] SrcStatement          Pointer to source statement.
 @param[in] DstFormSet            Pointer to destination formset.
 @param[in] DstForm               Pointer to destination form.
 @param[in] DstStatement          Pointer to destination statement.

 @retval EFI_SUCCESS              Successfully copy common expression list
 @retval EFI_UNSUPPORTED          Unexpected expression type
 @retval Other                    Fail to copy expression
**/
EFI_STATUS
CopyCommonListByType (
  IN     UINT32                               Type,
  IN     FORM_BROWSER_FORMSET                 *SrcFormSet,
  IN     FORM_BROWSER_STATEMENT               *SrcStatement,
  IN     FORM_BROWSER_FORMSET                 *DstFormSet,
  IN     FORM_BROWSER_FORM                    *DstForm,
  IN OUT FORM_BROWSER_STATEMENT               *DstStatement
  )
{
  LIST_ENTRY                                  *SrcListHead;
  LIST_ENTRY                                  *DstListHead;
  LIST_ENTRY                                  *Link;
  FORM_EXPRESSION                             *SrcExpression;
  FORM_EXPRESSION                             *DstExpression;
  EFI_STATUS                                  Status;

  switch (Type) {

  case EFI_HII_EXPRESSION_INCONSISTENT_IF:
    SrcListHead = &SrcStatement->InconsistentListHead;
    DstListHead = &DstStatement->InconsistentListHead;
    break;

  case EFI_HII_EXPRESSION_WARNING_IF:
    SrcListHead = &SrcStatement->WarningListHead;
    DstListHead = &DstStatement->WarningListHead;
    break;

  case EFI_HII_EXPRESSION_NO_SUBMIT_IF:
    SrcListHead = &SrcStatement->NoSubmitListHead;
    DstListHead = &DstStatement->NoSubmitListHead;
    break;

  default:
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Link = GetFirstNode (SrcListHead);
  while (!IsNull (SrcListHead, Link)) {
    SrcExpression = FORM_EXPRESSION_FROM_LINK (Link);
    Link = GetNextNode (SrcListHead, Link);

    Status = CopyExpression (SrcFormSet, SrcExpression, DstFormSet, DstForm, &DstExpression);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    InsertTailList (DstListHead, &DstExpression->Link);
  }

  return EFI_SUCCESS;
}

/**
 Copy options of statement.

 @param[in] SrcFormSet            Pointer to source formset.
 @param[in] SrcStatement          Pointer to source statement.
 @param[in] DstFormSet            Pointer to destination formset.
 @param[in] DstForm               Pointer to destination form.
 @param[in] DstStatement          Pointer to destination statement.

 @retval EFI_SUCCESS              Successfully copy options
 @retval EFI_OUT_OF_RESOURCES     Fail to allocate pool
**/
EFI_STATUS
CopyOptions (
  IN     FORM_BROWSER_FORMSET                 *SrcFormSet,
  IN     FORM_BROWSER_STATEMENT               *SrcStatement,
  IN     FORM_BROWSER_FORMSET                 *DstFormSet,
  IN     FORM_BROWSER_FORM                    *DstForm,
  IN OUT FORM_BROWSER_STATEMENT               *DstStatement
  )
{
  LIST_ENTRY                                  *Link;
  QUESTION_OPTION                             *SrcOption;
  QUESTION_OPTION                             *DstOption;
  UINTN                                       Index;

  DstStatement->NumberOfOptions = 0;
  InitializeListHead (&DstStatement->OptionListHead);

  Link = GetFirstNode (&SrcStatement->OptionListHead);
  while (!IsNull (&SrcStatement->OptionListHead, Link)) {
    SrcOption = QUESTION_OPTION_FROM_LINK (Link);
    Link      = GetNextNode (&SrcStatement->OptionListHead, Link);

    DstOption = AllocateZeroPool (sizeof (QUESTION_OPTION));
    if (DstOption == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    DstOption->Signature  = SrcOption->Signature;
    DstOption->OpCode     = SrcOption->OpCode;
    DstOption->Flags      = SrcOption->Flags;
    DstOption->Value.Type = SrcOption->Value.Type;
    CopyHiiString (SrcFormSet->HiiHandle, SrcOption->Text, DstFormSet->HiiHandle, &DstOption->Text);
    CopyMem (&DstOption->Value.Value, &SrcOption->Value.Value, sizeof (EFI_IFR_TYPE_VALUE));
    ExtendValueToU64 (&DstOption->Value);
    InitializeListHead (&DstOption->PropertyListHead);

    if (SrcOption->ImageId != 0) {
      CopyHiiImage (SrcFormSet->HiiHandle, SrcOption->ImageId, DstFormSet->HiiHandle, &DstOption->ImageId);
    }

    if (SrcOption->SuppressExpression != NULL) {
      DstOption->SuppressExpression = AllocateCopyPool(
                                        (UINTN) (sizeof(FORM_EXPRESSION_LIST) + ((SrcOption->SuppressExpression->Count -1) * sizeof(FORM_EXPRESSION *))),
                                        SrcOption->SuppressExpression
                                        );
      if (DstOption->SuppressExpression == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      for (Index = 0; Index < SrcOption->SuppressExpression->Count; Index++) {
        CopyExpression (
          SrcFormSet,
          SrcOption->SuppressExpression->Expression[Index],
          DstFormSet,
          DstForm,
          &DstOption->SuppressExpression->Expression[Index]
          );
        InsertTailList (&DstForm->ExpressionListHead, &DstOption->SuppressExpression->Expression[Index]->Link);
      }
    }


    DstStatement->NumberOfOptions++;
    InsertTailList (&DstStatement->OptionListHead, &DstOption->Link);
  }

  return EFI_SUCCESS;
}

/**
 Copy statement.

 @param[in] SrcFormSet            Pointer to source formset.
 @param[in] SrcForm               Pointer to source form.
 @param[in] SrcStatement          Pointer to source statement.
 @param[in] DstFormSet            Pointer to destination formset.
 @param[in] DstForm               Pointer to destination form.
 @param[in] DstStatement          Pointer to destination statement.

 @retval EFI_SUCCESS              Successfully copy statement
 @retval EFI_INVALID_PARAMETER    Input parameter is NULL
 @retval EFI_OUT_OF_RESOURCES     Fail to allocate pool
**/
EFI_STATUS
CopyStatement (
  IN  FORM_BROWSER_FORMSET                    *SrcFormSet,
  IN  FORM_BROWSER_FORM                       *SrcForm,
  IN  FORM_BROWSER_STATEMENT                  *SrcStatement,
  IN  FORM_BROWSER_FORMSET                    *DstFormSet,
  IN  FORM_BROWSER_FORM                       *DstForm,
  OUT FORM_BROWSER_STATEMENT                  *DstStatement
  )
{
  UINTN                                       Index;

  if (SrcFormSet == NULL || SrcStatement == NULL || DstFormSet == NULL || DstStatement == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  InitializeListHead (&DstStatement->DefaultListHead);
  InitializeListHead (&DstStatement->OptionListHead);
  InitializeListHead (&DstStatement->InconsistentListHead);
  InitializeListHead (&DstStatement->NoSubmitListHead);
  InitializeListHead (&DstStatement->WarningListHead);
  InitializeListHead (&DstStatement->PropertyListHead);

  DstStatement->Signature = SrcStatement->Signature;
  DstStatement->Operand   = SrcStatement->Operand;
  DstStatement->OpCode    = SrcStatement->OpCode;

  CopyHiiString (SrcFormSet->HiiHandle, SrcStatement->Prompt, DstFormSet->HiiHandle, &DstStatement->Prompt);
  CopyHiiString (SrcFormSet->HiiHandle, SrcStatement->Help  , DstFormSet->HiiHandle, &DstStatement->Help);

  DstStatement->RefreshInterval = SrcStatement->RefreshInterval;
  CopyMem (&DstStatement->RefreshGuid, &SrcStatement->RefreshGuid, sizeof (EFI_GUID));
  DstStatement->Locked = SrcStatement->Locked;

  if (SrcStatement->TextTwo != 0) {
    CopyHiiString (SrcFormSet->HiiHandle, SrcStatement->TextTwo, DstFormSet->HiiHandle, &DstStatement->TextTwo);
  }

  if (SrcStatement->ImageId != 0) {
    CopyHiiImage (SrcFormSet->HiiHandle, SrcStatement->ImageId, DstFormSet->HiiHandle, &DstStatement->ImageId);
  }

  if (!IsListEmpty (&SrcStatement->DefaultListHead)) {
    CopyDefaultList (SrcFormSet, SrcStatement, DstFormSet, DstForm, DstStatement);
  }
  CopyCommonListByType (EFI_HII_EXPRESSION_INCONSISTENT_IF, SrcFormSet, SrcStatement, DstFormSet, DstForm, DstStatement);
  CopyCommonListByType (EFI_HII_EXPRESSION_NO_SUBMIT_IF   , SrcFormSet, SrcStatement, DstFormSet, DstForm, DstStatement);
  CopyCommonListByType (EFI_HII_EXPRESSION_WARNING_IF     , SrcFormSet, SrcStatement, DstFormSet, DstForm, DstStatement);
  if (SrcStatement->ReadExpression != NULL) {
    CopyExpression (SrcFormSet, SrcStatement->ReadExpression, DstFormSet, DstForm, &DstStatement->ReadExpression);
    InsertTailList (&DstForm->ExpressionListHead, &DstStatement->ReadExpression->Link);
  }
  if (SrcStatement->WriteExpression != NULL) {
    CopyExpression (SrcFormSet, SrcStatement->WriteExpression, DstFormSet, DstForm, &DstStatement->WriteExpression);
    InsertTailList (&DstForm->ExpressionListHead, &DstStatement->WriteExpression->Link);
  }
  if (SrcStatement->ValueExpression != NULL) {
    CopyExpression (SrcFormSet, SrcStatement->ValueExpression, DstFormSet, DstForm, &DstStatement->ValueExpression);
    InsertTailList (&DstForm->ExpressionListHead, &DstStatement->ValueExpression->Link);
  }
  if (SrcStatement->Expression != NULL) {
    DstStatement->Expression = AllocateCopyPool (
                                 (UINTN) (sizeof(FORM_EXPRESSION_LIST) + ((SrcStatement->Expression->Count - 1) * sizeof(FORM_EXPRESSION *))),
                                 SrcStatement->Expression
                                 );
    if (DstStatement->Expression == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    for (Index = 0; Index < SrcStatement->Expression->Count; Index++) {
      CopyExpression (
        SrcFormSet,
        SrcStatement->Expression->Expression[Index],
        DstFormSet,
        DstForm,
        &DstStatement->Expression->Expression[Index]
        );
      InsertTailList (&DstForm->ExpressionListHead, &DstStatement->Expression->Expression[Index]->Link);
    }
  }

  DstStatement->ImportInfo = AllocateZeroPool (sizeof (FB_IMPORT_INFO));
  if (DstStatement->ImportInfo != NULL) {
    DstStatement->ImportInfo->SrcFormSet   = SrcFormSet;
    DstStatement->ImportInfo->SrcForm      = SrcForm;
    DstStatement->ImportInfo->SrcStatement = SrcStatement;
  }

  return EFI_SUCCESS;
}

/**
 Copy question.

 @param[in] SrcFormSet            Pointer to source formset.
 @param[in] SrcForm               Pointer to source form.
 @param[in] SrcStatement          Pointer to source statement.
 @param[in] SrcStorage            Pointer to source formset storage.
 @param[in] DstFormSet            Pointer to destination formset.
 @param[in] DstForm               Pointer to destination form.
 @param[in] DstStatement          Pointer to destination statement.

 @retval EFI_SUCCESS              Successfully copy question
 @retval EFI_INVALID_PARAMETER    Input parameter is NULL
 @retval EFI_ABORTED              Fail to create formset storage
 @retval Other                    Fail to create statement
**/
EFI_STATUS
CopyQuestion (
  IN  FORM_BROWSER_FORMSET                    *SrcFormSet,
  IN  FORM_BROWSER_FORM                       *SrcForm,
  IN  FORM_BROWSER_STATEMENT                  *SrcStatement,
  IN  FORMSET_STORAGE                         *SrcStorage,
  IN  FORM_BROWSER_FORMSET                    *DstFormSet,
  IN  FORM_BROWSER_FORM                       *DstForm,
  OUT FORM_BROWSER_STATEMENT                  *DstStatement
  )
{
  EFI_STATUS                                  Status;
  FORMSET_STORAGE                             *DstStorage;
  NAME_VALUE_NODE                             *NameValueNode;

  if (SrcFormSet == NULL || SrcStatement == NULL || DstFormSet == NULL || DstStatement == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = CopyStatement (
             SrcFormSet,
             SrcForm,
             SrcStatement,
             DstFormSet,
             DstForm,
             DstStatement
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DstStatement->QuestionId             = SrcStatement->QuestionId;
  DstStatement->VarStoreInfo.VarOffset = SrcStatement->VarStoreInfo.VarOffset;
  DstStatement->QuestionFlags          = SrcStatement->QuestionFlags;

  if (SrcStatement->VarStoreId == 0) {
    return EFI_SUCCESS;
  }

  DstStorage = GetFormSetStorage (DstFormSet, SrcStorage->BrowserStorage);
  if (DstStorage == NULL) {
    DstStorage = CreateImportStorage (SrcFormSet, SrcStorage, DstFormSet);
  }
  if (DstStorage == NULL) {
    return EFI_ABORTED;
  }

  DstStatement->Storage    = DstStorage->BrowserStorage;
  DstStatement->VarStoreId = DstStorage->VarStoreId;

  //
  // Initialize varname for Name/Value or EFI Variable
  //
  if ((DstStatement->Storage->Type == EFI_HII_VARSTORE_NAME_VALUE) ||
      (DstStatement->Storage->Type == EFI_HII_VARSTORE_EFI_VARIABLE)) {
    DstStatement->VariableName = GetString (SrcStatement->VarStoreInfo.VarName, SrcFormSet->HiiHandle);
    ASSERT (DstStatement->VariableName != NULL);
    if (DstStatement->VariableName == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    if (DstStatement->Storage->Type == EFI_HII_VARSTORE_NAME_VALUE) {
      if (!IsNameValueNodeExistence (&DstStatement->Storage->NameValueListHead, DstStatement->VariableName)) {
        //
        // Insert to Name/Value varstore list
        //
        NameValueNode = AllocateZeroPool (sizeof (NAME_VALUE_NODE));
        ASSERT (NameValueNode != NULL);
        if (NameValueNode == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
        NameValueNode->Signature = NAME_VALUE_NODE_SIGNATURE;
        NameValueNode->Name = AllocateCopyPool (StrSize (DstStatement->VariableName), DstStatement->VariableName);
        ASSERT (NameValueNode->Name != NULL);
        NameValueNode->Value = AllocateZeroPool (0x10);
        ASSERT (NameValueNode->Value != NULL);
        NameValueNode->EditValue = AllocateZeroPool (0x10);
        ASSERT (NameValueNode->EditValue != NULL);

        InsertTailList (&DstStatement->Storage->NameValueListHead, &NameValueNode->Link);
      }
    }
  }

  return EFI_SUCCESS;
}

/**
 Check if question ID of import question is conflict in destination formset.

 @param[in] DstFormSet            Pointer to destination formset.
 @param[in] ImportInfo            Pointer to import info.

 @retval TRUE                     Question ID of import question is conflict in destination formset.
 @retval FALSE                    Question ID of import question is not conflict in destination formset.
**/
BOOLEAN
IsIdConflict (
  IN FORM_BROWSER_FORMSET                     *DstFormSet,
  IN IMPORT_INFO                              *ImportInfo
  )
{
  if (DstFormSet == NULL || ImportInfo == NULL) {
    return FALSE;
  }

  if (IMPORT_INFO_FLAG_IS_QUESTION (ImportInfo->Flags)) {
    if ((ImportInfo->DstId == 0 && GetStatementByQuestionId (DstFormSet, NULL, ImportInfo->SrcId) != NULL && !CompareGuid (&ImportInfo->SrcFormSetGuid, &DstFormSet->Guid)) ||
        (ImportInfo->DstId != 0 &&
         ImportInfo->DstId != 0xFFFF &&
         GetStatementByQuestionId (DstFormSet, NULL, ImportInfo->DstId) != NULL)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 Get formset HII resource by formset.

 @param[in] FormSet               Pointer to formset.

 @return Formset HII resource pointer or NULL if not found.
**/
H2O_FORM_BROWSER_FORMSET_HII_RESOURCE *
GetFormSetHiiResource (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  LIST_ENTRY                                  *Link;
  H2O_FORM_BROWSER_FORMSET_HII_RESOURCE       *FormSetHiiResource;

  Link = GetFirstNode (&mH2OBrowserFormSetHiiResourceList);
  while (!IsNull (&mH2OBrowserFormSetHiiResourceList, Link)) {
    FormSetHiiResource = H2O_FORM_BROWSER_FORMSET_HII_RESOURCE_FROM_LINK (Link);
    Link               = GetNextNode (&mH2OBrowserFormSetHiiResourceList, Link);

    if (FormSetHiiResource->HiiHandle == FormSet->HiiHandle &&
        CompareGuid (&FormSetHiiResource->FormSetGuid, &FormSet->Guid)) {
      return FormSetHiiResource;
    }
  }

  return NULL;
}

/**
 Create formset HII resource by formset.

 @param[in] FormSet               Pointer to formset.

 @return Formset HII resource pointer or NULL if fail to allocate pool.
**/
H2O_FORM_BROWSER_FORMSET_HII_RESOURCE *
CreateFormSetHiiResource (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  H2O_FORM_BROWSER_FORMSET_HII_RESOURCE       *FormSetHiiResource;

  FormSetHiiResource = AllocateZeroPool (sizeof (H2O_FORM_BROWSER_FORMSET_HII_RESOURCE));
  if (FormSetHiiResource == NULL) {
    return NULL;
  }

  FormSetHiiResource->Signature = H2O_FORM_BROWSER_FORMSET_HII_RESOURCE_SIGNATURE;
  FormSetHiiResource->HiiHandle = FormSet->HiiHandle;
  CopyGuid (&FormSetHiiResource->FormSetGuid, &FormSet->Guid);

  FormSetHiiResource->NewStringIdCount = 50;
  FormSetHiiResource->NewStringIdList  = AllocateZeroPool (FormSetHiiResource->NewStringIdCount * sizeof (EFI_STRING_ID *));
  if (FormSetHiiResource->NewStringIdList == NULL) {
    FormSetHiiResource->NewStringIdCount = 0;
  }

  FormSetHiiResource->NewImageIdCount = 50;
  FormSetHiiResource->NewImageIdList  = AllocateZeroPool (FormSetHiiResource->NewImageIdCount * sizeof (EFI_IMAGE_ID *));
  if (FormSetHiiResource->NewImageIdList == NULL) {
    FormSetHiiResource->NewImageIdCount = 0;
  }

  InsertTailList (&mH2OBrowserFormSetHiiResourceList, &FormSetHiiResource->Link);

  return FormSetHiiResource;
}

/**
 Destroy invalid formset HII resource by checkinf HII handle.

**/
VOID
DestroyFormSetHiiResource (
  VOID
  )
{
  EFI_HII_HANDLE                              *HiiHandles;
  LIST_ENTRY                                  *Link;
  H2O_FORM_BROWSER_FORMSET_HII_RESOURCE       *FormSetHiiResource;
  UINTN                                       Index;

  if (IsListEmpty (&mH2OBrowserFormSetHiiResourceList)) {
    return;
  }

  HiiHandles = HiiGetHiiHandles (NULL);
  if (HiiHandles == NULL) {
    return;
  }

  Link = GetFirstNode (&mH2OBrowserFormSetHiiResourceList);
  while (!IsNull (&mH2OBrowserFormSetHiiResourceList, Link)) {
    FormSetHiiResource = H2O_FORM_BROWSER_FORMSET_HII_RESOURCE_FROM_LINK (Link);
    Link               = GetNextNode (&mH2OBrowserFormSetHiiResourceList, Link);

    for (Index = 0; HiiHandles[Index] != NULL; Index++) {
      if (HiiHandles[Index] == FormSetHiiResource->HiiHandle) {
        break;
      }
    }
    if (HiiHandles[Index] == NULL) {
      RemoveEntryList (&FormSetHiiResource->Link);
      FBFreePool ((VOID **) &FormSetHiiResource->NewStringIdList);
      FBFreePool ((VOID **) &FormSetHiiResource->NewImageIdList);
      FreePool (FormSetHiiResource);
    }
  }

  FreePool (HiiHandles);
}

/**
 Destroy all created imported formsets.

**/
VOID
DestroyImportFormSet (
  VOID
  )
{
  LIST_ENTRY                                  *Link;
  FORM_BROWSER_FORMSET                        *OrgFormset;
  FORM_BROWSER_FORMSET                        *FormSet;

  if (mImportFormUpdateNotifyHandle != NULL) {
    gHiiDatabase->UnregisterPackageNotify (gHiiDatabase, mImportFormUpdateNotifyHandle);
    mImportFormUpdateNotifyHandle = NULL;
  }

  OrgFormset = mSystemLevelFormSet;
  Link = GetFirstNode (&mH2OBrowserImportFormSetList);
  while (!IsNull (&mH2OBrowserImportFormSetList, Link)) {
    FormSet = FORM_BROWSER_FORMSET_FROM_LINK (Link);
    Link    = GetNextNode (&mH2OBrowserImportFormSetList, Link);

    mSystemLevelFormSet = FormSet;
    RemoveEntryList (&FormSet->Link);
    DestroyFormSet (FormSet);
  }
  Link = GetFirstNode (&mH2OBrowserStaleImportFormSetList);
  while (!IsNull (&mH2OBrowserStaleImportFormSetList, Link)) {
    FormSet = FORM_BROWSER_FORMSET_FROM_LINK (Link);
    Link    = GetNextNode (&mH2OBrowserStaleImportFormSetList, Link);

    RemoveEntryList (&FormSet->Link);
    DestroyFormSet (FormSet);
  }
  mSystemLevelFormSet = OrgFormset;

  DestroyFormSetHiiResource ();
}

/**
 Initialize formset HII resource.

 @param[in] FormSet               Pointer to formset.

 @retval EFI_SUCCESS              Successfully initialize formset HII resource
 @retval EFI_ABORTED              Fail to create formset HII resource
**/
EFI_STATUS
InitializeFormSetHiiResource (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  mFormSetHiiResource = GetFormSetHiiResource (FormSet);
  if (mFormSetHiiResource == NULL) {
    mFormSetHiiResource = CreateFormSetHiiResource (FormSet);
  }
  if (mFormSetHiiResource == NULL) {
    return EFI_ABORTED;
  }

  mFormSetHiiResource->NewStringIdIndex = 0;
  mFormSetHiiResource->NewImageIdIndex  = 0;

  return EFI_SUCCESS;
}

/**
  Remove the import formset data, which is about to be removed in HII database, from list.

  @param [in] PackageType  Package type of the notification.
  @param [in] PackageGuid  If PackageType is EFI_HII_PACKAGE_TYPE_GUID, then this is the pointer to the GUID from the Guid
                           field of EFI_HII_PACKAGE_GUID_HEADER. Otherwise, it must be NULL.
  @param [in] Package      Points to the package referred to by the notification Handle The handle of the package
                           list which contains the specified package.
  @param [in] Handle       The HII handle.
  @param [in] NotifyType   The type of change concerning the database. See EFI_HII_DATABASE_NOTIFY_TYPE.
**/
EFI_STATUS
EFIAPI
ImportFormUpdateNotifyFn (
  IN UINT8                              PackageType,
  IN CONST EFI_GUID                     *PackageGuid,
  IN CONST EFI_HII_PACKAGE_HEADER       *Package,
  IN EFI_HII_HANDLE                     Handle,
  IN EFI_HII_DATABASE_NOTIFY_TYPE       NotifyType
  )
{
  LIST_ENTRY                            *Link;
  FORM_BROWSER_FORMSET                  *FormSet;

  Link = GetFirstNode (&mH2OBrowserImportFormSetList);
  while (!IsNull (&mH2OBrowserImportFormSetList, Link)) {
    FormSet = FORM_BROWSER_FORMSET_FROM_LINK (Link);
    Link    = GetNextNode (&mH2OBrowserImportFormSetList, Link);

    if (FormSet->HiiHandle == Handle) {
      RemoveEntryList (&FormSet->Link);
      InsertTailList (&mH2OBrowserStaleImportFormSetList, &FormSet->Link);
    }
  }

  return EFI_SUCCESS;
}

/**
 If there is a stale imported formset, reload the HII formsets which import this stale formset.
**/
VOID
ReloadHiiFormsetWithStaleImportData (
  VOID
  )
{
  LIST_ENTRY                            *StaleFormSetLink;
  FORM_BROWSER_FORMSET                  *StaleFormSet;
  LIST_ENTRY                            *FormSetLink;
  FORM_BROWSER_FORMSET                  *FormSet;
  EFI_HII_HANDLE                        *HandleList;
  UINTN                                 HandleCount;

  if (IsListEmpty (&mH2OBrowserStaleImportFormSetList)) {
    return;
  }

  //
  // Record the HiiHandle of formset which import the stale formset.
  //
  HandleList  = NULL;
  HandleCount = 0;

  StaleFormSetLink = GetFirstNode (&mH2OBrowserStaleImportFormSetList);
  while (!IsNull (&mH2OBrowserStaleImportFormSetList, StaleFormSetLink)) {
    StaleFormSet     = FORM_BROWSER_FORMSET_FROM_LINK (StaleFormSetLink);
    StaleFormSetLink = GetNextNode (&mH2OBrowserStaleImportFormSetList, StaleFormSetLink);

    FormSetLink = GetFirstNode (&gBrowserFormSetList);
    while (!IsNull (&gBrowserFormSetList, FormSetLink)) {
      FormSet     = FORM_BROWSER_FORMSET_FROM_LINK (FormSetLink);
      FormSetLink = GetNextNode (&gBrowserFormSetList, FormSetLink);

      if (HaveImportThisFormset (FormSet, StaleFormSet)) {
        HandleList = ReallocatePool (HandleCount * sizeof (EFI_HII_HANDLE), (HandleCount + 1) * sizeof (EFI_HII_HANDLE), HandleList);
        if (HandleList == NULL) {
          return;
        }

        HandleList[HandleCount++] = FormSet->HiiHandle;
        break;
      }
    }
  }

  //
  // Destroy stale imported formset.
  //
  StaleFormSetLink = GetFirstNode (&mH2OBrowserStaleImportFormSetList);
  while (!IsNull (&mH2OBrowserStaleImportFormSetList, StaleFormSetLink)) {
    StaleFormSet     = FORM_BROWSER_FORMSET_FROM_LINK (StaleFormSetLink);
    StaleFormSetLink = GetNextNode (&mH2OBrowserStaleImportFormSetList, StaleFormSetLink);

    RemoveEntryList (&StaleFormSet->Link);
    DestroyFormSet (StaleFormSet);
  }

  if (HandleList != NULL) {
    LoadHiiFormset (HandleList, HandleCount);
    FreePool (HandleList);
  }
}

#define EFI_BITS_SHIFT_PER_UINT32          0x5
#define EFI_BITS_PER_UINT32                (1 << EFI_BITS_SHIFT_PER_UINT32)
#define EFI_FORM_ID_MAX                    0xFFFF
#define EFI_FREE_FORM_ID_BITMAP_SIZE       ((EFI_FORM_ID_MAX + 1) / EFI_BITS_PER_UINT32)

/**

 Return a free Form ID

 @param[in] FormSet               Pointer to FormSet.

 @retval UINT16                   Return the free Form ID

**/
UINT16
GetFreeFormId (
  FORM_BROWSER_FORMSET                        *FormSet
  )
{
  FORM_BROWSER_FORM                           *Form;
  LIST_ENTRY                                  *FormLink;
  UINT32                                       FreeFormIdBitMap[EFI_FREE_FORM_ID_BITMAP_SIZE];
  UINT32                                       Index;
  UINT32                                       Offset;
  UINT32                                       Mask;

  for (Index = 0; Index < EFI_FREE_FORM_ID_BITMAP_SIZE; Index++) {
    FreeFormIdBitMap[Index] = 0;
  }

  // Form ID 0 is reserved.
  FreeFormIdBitMap[0] = 0x80000000;


  FormLink = GetFirstNode (&FormSet->FormListHead);
  while (!IsNull (&FormSet->FormListHead, FormLink)) {
    Form     = FORM_BROWSER_FORM_FROM_LINK (FormLink);
    FormLink = GetNextNode (&FormSet->FormListHead, FormLink);

    Index  = (Form->FormId / EFI_BITS_PER_UINT32);
    Offset = (Form->FormId % EFI_BITS_PER_UINT32);
    FreeFormIdBitMap[Index] |= (0x80000000 >> Offset);
  }

  for (Index = 0; Index < EFI_FREE_FORM_ID_BITMAP_SIZE; Index++) {
    if (FreeFormIdBitMap[Index] != 0xFFFFFFFF) {
      break;
    }
  }

  ASSERT (Index != EFI_FREE_FORM_ID_BITMAP_SIZE);
  if (Index == EFI_FREE_FORM_ID_BITMAP_SIZE) {
    return 0;
  }

  for (Offset = 0, Mask = 0x80000000; Mask != 0; Mask >>= 1, Offset++) {
    if ((FreeFormIdBitMap[Index] & Mask) == 0) {
      FreeFormIdBitMap[Index] |= Mask;
      return (UINT16)((Index << EFI_BITS_SHIFT_PER_UINT32) + Offset);
    }
  }

  return 0;
}

EFI_STATUS
CreateExpressionQIdMapForm (
  FORM_BROWSER_FORMSET                        *FormSet,
  FORM_BROWSER_FORM                           **Form
  )
{
  FORM_BROWSER_FORM                           *CurrentForm;
  UINT16                                       FreeFormId;

  CurrentForm = AllocateZeroPool (sizeof (FORM_BROWSER_FORM));
  ASSERT (CurrentForm != NULL);
  if (CurrentForm == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  FreeFormId = GetFreeFormId (FormSet);
  if (FreeFormId == 0) {
    return EFI_INVALID_PARAMETER;
  }

  CurrentForm->Signature = FORM_BROWSER_FORM_SIGNATURE;
  CurrentForm->FormSet   = FormSet;

  InitializeListHead (&CurrentForm->ExpressionListHead);
  InitializeListHead (&CurrentForm->StatementListHead);
  InitializeListHead (&CurrentForm->ConfigRequestHead);
  InitializeListHead (&CurrentForm->PropertyListHead);
  InitializeListHead (&CurrentForm->TargetInfoListHead);

  CurrentForm->FormType  = STANDARD_MAP_FORM_TYPE;
  CurrentForm->FormTitle = (EFI_STRING_ID) 1;
  CurrentForm->FormId    = FreeFormId;

  InsertTailList (&FormSet->FormListHead, &CurrentForm->Link);

  *Form = CurrentForm;

  return EFI_SUCCESS;
}

/**
 Set Statement to a goto question which can go to the specified TargetForm of TargetFormSet.

 @param[in]      TargetFormSet         Pointer to target formset of goto question.
 @param[in]      TargetForm            Pointer to target form of goto question.
 @param[in]      FormSet               Pointer to formset.
 @param[in, out] Statement             Pointer to statement.

 @retval EFI_SUCCESS                   Successfully set Statement to a goto question
 @retval EFI_INVALID_PARAMETER         Input parameter is NULL
 @retval EFI_OUT_OF_RESOURCES          Fail to allocate memory
**/
EFI_STATUS
SetGotoQuestion (
  IN     FORM_BROWSER_FORMSET                 *TargetFormSet,
  IN     FORM_BROWSER_FORM                    *TargetForm,
  IN     FORM_BROWSER_FORMSET                 *FormSet,
  IN OUT FORM_BROWSER_STATEMENT               *Statement
  )
{
  UINT8                                       *Buffer;
  EFI_IFR_REF3                                *Ref3Opcode;

  if (TargetFormSet == NULL || TargetForm == NULL || FormSet == NULL || Statement == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Buffer = AllocateZeroPool (sizeof(FB_IMPORT_INFO) + sizeof(EFI_IFR_REF3));
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Statement->ImportInfo = (FB_IMPORT_INFO *) Buffer;
  Statement->OpCode     = (EFI_IFR_OP_HEADER *) (Buffer + sizeof (FB_IMPORT_INFO));

  InitializeListHead (&Statement->DefaultListHead);
  InitializeListHead (&Statement->OptionListHead);
  InitializeListHead (&Statement->InconsistentListHead);
  InitializeListHead (&Statement->NoSubmitListHead);
  InitializeListHead (&Statement->WarningListHead);
  InitializeListHead (&Statement->PropertyListHead);

  Statement->Signature                     = FORM_BROWSER_STATEMENT_SIGNATURE;
  Statement->Operand                       = EFI_IFR_REF_OP;
  Statement->QuestionId                    = GetAvailableQuestionId (FormSet);
  Statement->RefreshInterval               = 0;
  Statement->Locked                        = FALSE;
  Statement->HiiValue.Type                 = EFI_IFR_TYPE_REF;
  Statement->HiiValue.Value.ref.QuestionId = 0;
  Statement->HiiValue.Value.ref.FormId     = TargetForm->FormId;
  CopyGuid (&Statement->HiiValue.Value.ref.FormSetGuid, &TargetFormSet->Guid);

  CopyHiiString (TargetFormSet->HiiHandle, TargetForm->FormTitle, FormSet->HiiHandle, &Statement->Prompt);
  CopyHiiString (TargetFormSet->HiiHandle, TargetForm->FormTitle, FormSet->HiiHandle, &Statement->Help);

  Statement->ImportInfo->SrcFormSet   = TargetFormSet;
  Statement->ImportInfo->SrcForm      = TargetForm;
  Statement->ImportInfo->SrcStatement = NULL;

  Ref3Opcode                         = (EFI_IFR_REF3 *) Statement->OpCode;
  Ref3Opcode->Header.OpCode          = EFI_IFR_REF_OP;
  Ref3Opcode->Header.Length          = sizeof (EFI_IFR_REF3);
  Ref3Opcode->Question.Header.Prompt = Statement->Prompt;
  Ref3Opcode->Question.Header.Help   = Statement->Help;
  Ref3Opcode->QuestionId             = Statement->QuestionId;
  Ref3Opcode->FormId                 = TargetForm->FormId;
  CopyGuid ((VOID *) &Ref3Opcode->FormSetId, &TargetFormSet->Guid);

  return EFI_SUCCESS;
}

/**
 Process VFCF/VFR import for formset.

 @param[in] DstFormSet            Pointer to formset.

 @retval EFI_SUCCESS              Successfully process VFCF/VFR import
 @retval EFI_INVALID_PARAMETER    Input parameter is NULL
 @retval EFI_ABORTED              Fail to create formset HII resource
 @retval Other                    Import info is not found or fail to initialize import required buffer
**/
EFI_STATUS
FBProcessImport (
  IN OUT FORM_BROWSER_FORMSET                 *DstFormSet
  )
{
  EFI_STATUS                                  Status;
  UINT16                                      Index;
  IMPORT_INFO                                 *ImportInfo;
  FORM_BROWSER_FORM                           *DstForm;
  FORM_BROWSER_STATEMENT                      *DstStatement;
  FORM_BROWSER_FORMSET                        *SrcFormSet;
  FORM_BROWSER_FORM                           *SrcForm;
  FORM_BROWSER_STATEMENT                      *SrcStatement;
  FORMSET_STORAGE                             *SrcStorage;
  EFI_QUESTION_ID                             DstFakeQuestionId;
  H2O_FORM_BROWSER_FORMSET_HII_RESOURCE       *FormSetHiiResource;
  FORM_BROWSER_FORM                           *ExpressionQIdMapForm;
  UINTN                                       ExpressionQIdMapCount;

  if (DstFormSet == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DeleteAllVfrImportNewStatements (DstFormSet);

  mImportInfoCount = 0;
  mImportInfoList = NULL;
  GetImportInfoList (&DstFormSet->Guid, &mImportInfoList, &mImportInfoCount);
  GetImportInfoListFromVfrFormSet (DstFormSet, &mImportInfoList, &mImportInfoCount);
  GetImportInfoListFromTargetInfo (DstFormSet, &mImportInfoList, &mImportInfoCount);
  if (mImportInfoCount == 0) {
    return EFI_NOT_FOUND;
  }

  InitializeFormSetHiiResource (DstFormSet);

  for (Index = 0; Index < mImportInfoCount; Index++) {
    if (GetFormSetByFormSetGuid (&mImportInfoList[Index].SrcFormSetGuid, &mH2OBrowserImportFormSetList, &gBrowserFormSetList, NULL) == NULL) {
      CreateImportFormSet (&mImportInfoList[Index].SrcFormSetGuid);
    }
  }

  mImportStatementId = GetAvailableStatementId (DstFormSet);
  DstFakeQuestionId = GetAvailableFakeQuestionId (DstFormSet);

  FormSetHiiResource = GetFormSetHiiResource (DstFormSet);
  if (FormSetHiiResource == NULL) {
    return EFI_ABORTED;
  }

  ExpressionQIdMapForm = NULL;

  for (Index = 0; Index < mImportInfoCount; Index++) {
    ImportInfo = &mImportInfoList[Index];

    if (IsIdConflict (DstFormSet, ImportInfo)) {
      continue;
    }

    if (ImportInfo->ExpressionQId != 0) {
      //
      // create a form to store expression questions
      //
      if (ExpressionQIdMapForm == NULL) {
        Status = CreateExpressionQIdMapForm (DstFormSet, &ExpressionQIdMapForm);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR (Status)) {
          return EFI_ABORTED;
        }
      }
      ImportInfo->DstFormId = ExpressionQIdMapForm->FormId;
      DstForm = ExpressionQIdMapForm;
    } else {
      DstForm = IdToForm (DstFormSet, ImportInfo->DstFormId);
      if (DstForm == NULL) {
        continue;
      }
    }

    Status = GetSrcData (ImportInfo, &SrcFormSet, &SrcForm, &SrcStatement, &SrcStorage);
    if (EFI_ERROR (Status)) {
      continue;
    }

    DstStatement = GetNewStatement (DstFormSet);
    if (DstStatement == NULL) {
      continue;
    }

    DstStatement->StatementId = mImportStatementId++;

    if (IS_LINK_TARGET (ImportInfo->Type)) {
      Status = SetGotoQuestion (SrcFormSet, SrcForm, DstFormSet, DstStatement);
      if (EFI_ERROR (Status)) {
        continue;
      }
      ImportInfo->DstId = DstStatement->QuestionId;
    } else if (SrcStatement->Operand == EFI_IFR_SUBTITLE_OP ||
               SrcStatement->Operand == EFI_IFR_TEXT_OP     ||
               SrcStatement->Operand == EFI_IFR_RESET_BUTTON_OP) {
      Status = CopyStatement (SrcFormSet, SrcForm, SrcStatement, DstFormSet, DstForm, DstStatement);
      if (EFI_ERROR (Status)) {
        continue;
      }

      switch (SrcStatement->Operand) {

      case EFI_IFR_SUBTITLE_OP:
        DstStatement->Flags = SrcStatement->Flags;
        break;

      case EFI_IFR_TEXT_OP:
        break;

      case EFI_IFR_RESET_BUTTON_OP:
        DstStatement->DefaultId = SrcStatement->DefaultId;
        break;
      }

      DstStatement->FakeQuestionId = DstFakeQuestionId++;
    } else {
      Status = CopyQuestion (SrcFormSet, SrcForm, SrcStatement, SrcStorage, DstFormSet, DstForm, DstStatement);
      if (EFI_ERROR (Status)) {
        continue;
      }

      switch (SrcStatement->Operand) {

      case EFI_IFR_ACTION_OP:
        DstStatement->HiiValue.Type = EFI_IFR_TYPE_ACTION;
        if (SrcStatement->QuestionConfig != 0) {
          CopyHiiString (SrcFormSet->HiiHandle, SrcStatement->QuestionConfig, DstFormSet->HiiHandle, &DstStatement->QuestionConfig);
        }
        break;

      case EFI_IFR_REF_OP:
        DstStatement->HiiValue.Type = EFI_IFR_TYPE_REF;

        if (SrcStatement->HiiValue.Value.ref.DevicePath != 0) {
          CopyHiiString (SrcFormSet->HiiHandle, SrcStatement->HiiValue.Value.ref.DevicePath, DstFormSet->HiiHandle, &DstStatement->HiiValue.Value.ref.DevicePath);
        }

        CopyMem (
          &DstStatement->HiiValue.Value.ref.FormSetGuid,
          &SrcStatement->HiiValue.Value.ref.FormSetGuid,
          sizeof (EFI_GUID)
          );

        if (DstStatement->HiiValue.Value.ref.DevicePath == 0 && CompareGuid (&DstStatement->HiiValue.Value.ref.FormSetGuid, &gZeroGuid)) {
          //
          // Originally, it is going into form or question in self formset.
          // But here, it should use REF3.
          //
          CopyMem (
            &DstStatement->HiiValue.Value.ref.FormSetGuid,
            &SrcFormSet->Guid,
            sizeof (EFI_GUID)
            );
        }
        DstStatement->HiiValue.Value.ref.FormId     = SrcStatement->HiiValue.Value.ref.FormId;
        DstStatement->HiiValue.Value.ref.QuestionId = SrcStatement->HiiValue.Value.ref.QuestionId;

        DstStatement->StorageWidth = SrcStatement->StorageWidth;
        InitializeRequestElement (DstFormSet, DstStatement, DstForm);
        break;

      case EFI_IFR_STRING_OP:
      case EFI_IFR_PASSWORD_OP:
        DstStatement->Minimum      = SrcStatement->Minimum;
        DstStatement->Maximum      = SrcStatement->Maximum;
        DstStatement->StorageWidth = SrcStatement->StorageWidth;
        DstStatement->Flags        = SrcStatement->Flags;

        DstStatement->HiiValue.Type         = EFI_IFR_TYPE_STRING;
        DstStatement->BufferValue           = AllocateZeroPool (DstStatement->StorageWidth + sizeof (CHAR16));
        DstStatement->HiiValue.Value.string = NewString ((CHAR16*) DstStatement->BufferValue, DstFormSet->HiiHandle);

        DstStatement->HiiValue.Buffer    = DstStatement->BufferValue;
        DstStatement->HiiValue.BufferLen = DstStatement->StorageWidth + sizeof (CHAR16);
        InitializeRequestElement (DstFormSet, DstStatement, DstForm);
        break;

      case EFI_IFR_CHECKBOX_OP:
        DstStatement->Flags         = SrcStatement->Flags;
        DstStatement->StorageWidth  = (UINT16) sizeof (BOOLEAN);
        DstStatement->HiiValue.Type = EFI_IFR_TYPE_BOOLEAN;
        InitializeRequestElement (DstFormSet, DstStatement, DstForm);
        break;

      case EFI_IFR_DATE_OP:
        DstStatement->Flags         = SrcStatement->Flags;
        DstStatement->HiiValue.Type = EFI_IFR_TYPE_DATE;

        if ((DstStatement->Flags & EFI_QF_DATE_STORAGE) == QF_DATE_STORAGE_NORMAL) {
          DstStatement->StorageWidth = (UINT16) sizeof (EFI_HII_DATE);

          InitializeRequestElement (DstFormSet, DstStatement, DstForm);
        } else {
          //
          // Don't assign storage for RTC type of date/time
          //
          DstStatement->Storage      = NULL;
          DstStatement->StorageWidth = 0;
        }
        break;

      case EFI_IFR_TIME_OP:
        DstStatement->Flags         = SrcStatement->Flags;
        DstStatement->HiiValue.Type = EFI_IFR_TYPE_TIME;

        if ((DstStatement->Flags & QF_TIME_STORAGE) == QF_TIME_STORAGE_NORMAL) {
          DstStatement->StorageWidth = (UINT16) sizeof (EFI_HII_TIME);

          InitializeRequestElement (DstFormSet, DstStatement, DstForm);
        } else {
          //
          // Don't assign storage for RTC type of date/time
          //
          DstStatement->Storage      = NULL;
          DstStatement->StorageWidth = 0;
        }
        break;

      case EFI_IFR_ONE_OF_OP:
      case EFI_IFR_NUMERIC_OP:
        DstStatement->Flags         = SrcStatement->Flags;

        DstStatement->Minimum       = SrcStatement->Minimum;
        DstStatement->Maximum       = SrcStatement->Maximum;
        DstStatement->Step          = SrcStatement->Step;
        DstStatement->StorageWidth  = SrcStatement->StorageWidth;
        DstStatement->HiiValue.Type = SrcStatement->HiiValue.Type;

        if (!IsListEmpty (&SrcStatement->OptionListHead)) {
          CopyOptions (
            SrcFormSet,
            SrcStatement,
            DstFormSet,
            DstForm,
            DstStatement
            );
        }

        InitializeRequestElement (DstFormSet, DstStatement, DstForm);
        break;

      case EFI_IFR_ORDERED_LIST_OP:
        DstStatement->Flags         = SrcStatement->Flags;
        DstStatement->MaxContainers = SrcStatement->MaxContainers;
        DstStatement->HiiValue.Type = EFI_IFR_TYPE_BUFFER;
        DstStatement->BufferValue   = NULL;

        if (!IsListEmpty (&SrcStatement->OptionListHead)) {
          CopyOptions (
            SrcFormSet,
            SrcStatement,
            DstFormSet,
            DstForm,
            DstStatement
            );
        }

        if (SrcStatement->BufferValue != NULL) {
          DstStatement->StorageWidth = SrcStatement->StorageWidth;
          DstStatement->BufferValue  = AllocateZeroPool (SrcStatement->StorageWidth);
          DstStatement->ValueType    = SrcStatement->ValueType;
          if (DstStatement->HiiValue.Type == EFI_IFR_TYPE_BUFFER) {
            DstStatement->HiiValue.Buffer    = DstStatement->BufferValue;
            DstStatement->HiiValue.BufferLen = DstStatement->StorageWidth;
          }

          InitializeRequestElement (DstFormSet, DstStatement, DstForm);
        }
        break;

      default:
        break;
      }

      if (IMPORT_INFO_FLAG_IS_QUESTION (ImportInfo->Flags)) {
        DstStatement->QuestionId = GetQuestionId (DstFormSet, ImportInfo);
      } else {
        DstStatement->QuestionId = GetAvailableQuestionId (DstFormSet);
      }
      ImportInfo->DstId = DstStatement->QuestionId;
    }

    if (ImportInfo->VfrFakeStatement != NULL) {
      InsertNewStatementAfterFakeStatement (DstFormSet, DstForm, (FORM_BROWSER_STATEMENT *)ImportInfo->VfrFakeStatement, DstStatement);
    } else {
      InsertTailList (&DstForm->StatementListHead, &DstStatement->Link);
    }
    DstForm->NumberOfStatement++;
  }


  //
  // update expression qid map list
  //
  ExpressionQIdMapCount = 0;
  for (Index = 0, ImportInfo = mImportInfoList; Index < mImportInfoCount; Index++, ImportInfo++) {
    if (ImportInfo->ExpressionQId == 0) {
      continue;
    }
    ExpressionQIdMapCount++;
  }

  DstFormSet->ExpressionQIdMapList[0] = AllocateZeroPool (sizeof (EFI_QUESTION_ID) * ExpressionQIdMapCount);
  DstFormSet->ExpressionQIdMapList[1] = AllocateZeroPool (sizeof (EFI_QUESTION_ID) * ExpressionQIdMapCount);

  ExpressionQIdMapCount = 0;
  for (Index = 0, ImportInfo = mImportInfoList; Index < mImportInfoCount; Index++, ImportInfo++) {
    if (ImportInfo->ExpressionQId == 0) {
      continue;
    }

    DstFormSet->ExpressionQIdMapList[0][ExpressionQIdMapCount] = ImportInfo->ExpressionQId;
    DstFormSet->ExpressionQIdMapList[1][ExpressionQIdMapCount] = ImportInfo->DstId;
    ExpressionQIdMapCount++;
  }

  DstFormSet->ExpressionQIdMapListCount = ExpressionQIdMapCount;

  if (mImportFormUpdateNotifyHandle == NULL) {
    gHiiDatabase->RegisterPackageNotify (
                    gHiiDatabase,
                    EFI_HII_PACKAGE_FORMS,
                    NULL,
                    ImportFormUpdateNotifyFn,
                    EFI_HII_DATABASE_NOTIFY_REMOVE_PACK,
                    &mImportFormUpdateNotifyHandle
                    );
  }

  FBFreePool ((VOID **) &mImportInfoList);

  return EFI_SUCCESS;
}

/**
 Process VFCF property for formset.

 @param[in] FormSet               Pointer to formset.

 @retval EFI_SUCCESS              Successfully process VFCF property
 @retval EFI_INVALID_PARAMETER    Input parameter is NULL
 @retval Other                    Fail to get layout
**/
EFI_STATUS
FBProcessVfcfProperty (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  EFI_STATUS                                  Status;
  LIST_ENTRY                                  *FormLink;
  FORM_BROWSER_FORM                           *Form;
  LIST_ENTRY                                  *StatementLink;
  FORM_BROWSER_STATEMENT                      *Statement;
  BOOLEAN                                     IsQuestion;
  H2O_PROPERTY_INFO                           *Property;
  H2O_PROPERTY_VALUE                          PropValue;
  UINT16                                      Position;

  if (FormSet == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!HaveVfcfFormsetInfo (&FormSet->Guid)) {
    return EFI_SUCCESS;
  }

  //
  // Process enable property from VFCF.
  // If form is disabled, destroy it.
  // If statement is disabled, insert a disableif TRUE expression into statement.
  //
  FormLink = GetFirstNode (&FormSet->FormListHead);
  while (!IsNull (&FormSet->FormListHead, FormLink)) {
    Form     = FORM_BROWSER_FORM_FROM_LINK (FormLink);
    FormLink = GetNextNode (&FormSet->FormListHead, FormLink);

    if (!HaveVfcfFormInfo (&FormSet->Guid, Form->FormId)) {
      continue;
    }

    //
    // Check if form is enable or not.
    //
    Status = GetVfcfFormPropertyValue (
               &FormSet->Guid,
               Form->FormId,
               H2O_IFR_STYLE_TYPE_FORM,
               H2O_STYLE_PSEUDO_CLASS_NORMAL,
               "enable",
               NULL,
               &Property,
               &PropValue
               );
    if (!EFI_ERROR (Status) && !PropValue.H2OValue.Value.Bool) {
      RemoveEntryList (&Form->Link);
      DestroyForm (FormSet, Form);
      continue;
    }

    //
    // Check if statement is enable or not.
    //
    Position = 0;
    StatementLink = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, StatementLink)) {
      Statement     = FORM_BROWSER_STATEMENT_FROM_LINK (StatementLink);
      StatementLink = GetNextNode (&Form->StatementListHead, StatementLink);

      IsQuestion = (BOOLEAN) (Statement->QuestionId != 0);
      Status = GetVfcfStatementPropertyValue (
                 &FormSet->Guid,
                 IsQuestion,
                 (IsQuestion ? Statement->QuestionId : Position),
                 H2O_IFR_STYLE_TYPE_VFR_TREE_FLAG,
                 H2O_STYLE_PSEUDO_CLASS_NORMAL,
                 "enable",
                 NULL,
                 &Property,
                 &PropValue
                 );
      if (!EFI_ERROR (Status) && !PropValue.H2OValue.Value.Bool) {
        InsertDisableIfExpression (FormSet, Form, Statement);
      }
      Position++;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
ReplaceExpressionOfStatement (
  IN     FORM_BROWSER_FORM                    *Form,
  IN OUT FORM_BROWSER_STATEMENT               *Statement,
  IN     FORM_EXPRESSION                      *Expression
  )
{
  UINTN                                       Index;
  FORM_EXPRESSION_LIST                        *ExpressionList;

  InsertTailList (&Form->ExpressionListHead, &Expression->Link);

  if (Statement->Expression == NULL) {
    Statement->Expression = (FORM_EXPRESSION_LIST *) AllocateZeroPool (sizeof (FORM_EXPRESSION_LIST));
    if (Statement->Expression == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Statement->Expression->Count         = 1;
    Statement->Expression->Signature     = FORM_EXPRESSION_LIST_SIGNATURE;
    Statement->Expression->Expression[0] = Expression;

    return EFI_SUCCESS;

  }

  //
  // find the same type expression, replace it
  //
  for (Index = 0; Index < Statement->Expression->Count; Index++) {
    if (Statement->Expression->Expression[Index]->Type == Expression->Type) {
      Statement->Expression->Expression[Index] = Expression;
      return EFI_SUCCESS;
    }
  }

  //
  // add a new expression
  //
  ExpressionList = ReallocatePool (
                     (sizeof(FORM_EXPRESSION_LIST) + ((Statement->Expression->Count - 1) * sizeof(FORM_EXPRESSION *))),
                     (sizeof(FORM_EXPRESSION_LIST) + ((Statement->Expression->Count) * sizeof(FORM_EXPRESSION *))),
                     Statement->Expression
                     );
  if (ExpressionList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Statement->Expression = ExpressionList;
  ExpressionList->Expression[ExpressionList->Count++] = Expression;

  return EFI_SUCCESS;
}

EFI_STATUS
ConvertExpressionQIdToQuestionId (
  IN     FORM_BROWSER_FORMSET                 *FormSet,
  IN     EFI_QUESTION_ID                      *ExpressionQId
  )
{
  EFI_QUESTION_ID         TmpExpressionQId;
  EFI_QUESTION_ID         *QuestionIdPtr;
  UINTN                   Index;

  TmpExpressionQId = *ExpressionQId;
  for (Index = 0, QuestionIdPtr = FormSet->ExpressionQIdMapList[0]; Index < FormSet->ExpressionQIdMapListCount; Index++, QuestionIdPtr++) {
    if (TmpExpressionQId == *QuestionIdPtr) {
      break;
    }
  }

  //
  // Maybe VfcfCompile.exe generate incorrect expression question id map
  //
  ASSERT (Index != FormSet->ExpressionQIdMapListCount);
  if (Index == FormSet->ExpressionQIdMapListCount) {
    *ExpressionQId = 0;
    return EFI_NOT_FOUND;
  }

  //
  // .vfcf file may use incorrect destination formset / form id / questions id
  //
  ASSERT (FormSet->ExpressionQIdMapList[1][Index] != 0xFFFF);
  if (FormSet->ExpressionQIdMapList[1][Index] == 0xFFFF) {
    *ExpressionQId = 0;
    return EFI_NOT_FOUND;
  }

  *ExpressionQId = FormSet->ExpressionQIdMapList[1][Index];

  return EFI_SUCCESS;
}



EFI_STATUS
ReplaceExpression (
  IN     FORM_BROWSER_FORMSET                 *FormSet,
  IN     FORM_BROWSER_FORM                    *Form,
  IN OUT FORM_BROWSER_STATEMENT               *Statement,
  IN     H2O_EXPRESSION_INFO                  *ExpressionInfo
  )
{
  UINT8                   Operand;
  UINT8                   *OpCodeData;
  UINTN                   OpCodeLength;
  UINTN                   OpCodeOffset;
  EFI_HII_VALUE           *Value;
  EXPRESSION_OPCODE       *ExpressionOpCode;
  FORM_EXPRESSION         *FormExpression;
  UINTN                   OpCodeArrayLength;
  UINT8                   *OpCodeArray;
  EFI_STATUS              Status;

  OpCodeArray        = ExpressionInfo->ExpressionOpcodes;
  OpCodeArrayLength  = ExpressionInfo->ExpressionSize;

  FormExpression = CreateExpression (Form, OpCodeArray);
  if (FormExpression == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  InitializeListHead (&FormExpression->OpCodeListHead);
  switch (((EFI_IFR_OP_HEADER *) OpCodeArray)->OpCode) {

  case EFI_IFR_GRAY_OUT_IF_OP:
    FormExpression->Type = EFI_HII_EXPRESSION_GRAY_OUT_IF;
    break;

  case EFI_IFR_SUPPRESS_IF_OP:
    FormExpression->Type = EFI_HII_EXPRESSION_SUPPRESS_IF;
    break;

  case EFI_IFR_DISABLE_IF_OP:
    FormExpression->Type = EFI_HII_EXPRESSION_DISABLE_IF;
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }

  Status = ReplaceExpressionOfStatement (Form, Statement, FormExpression);
  if (EFI_ERROR (Status)) {
     return Status;
  }

  //
  // pass fisrt opcode
  //
  OpCodeArrayLength -= ((EFI_IFR_OP_HEADER *) OpCodeArray)->Length;
  OpCodeOffset       = ((EFI_IFR_OP_HEADER *) OpCodeArray)->Length;

  while (OpCodeOffset <= OpCodeArrayLength) {
    OpCodeData = OpCodeArray + OpCodeOffset;

    OpCodeLength = ((EFI_IFR_OP_HEADER *) OpCodeData)->Length;
    OpCodeOffset += OpCodeLength;
    Operand = ((EFI_IFR_OP_HEADER *) OpCodeData)->OpCode;


    //
    // BUGBUG: it should check whether need EFI_IFR_END_OP
    //
    if (Operand == EFI_IFR_END_OP && OpCodeOffset > OpCodeArrayLength) {
      continue ;
    }

    ExpressionOpCode = GetNewExpressionOpCode (FormSet);
    if (ExpressionOpCode == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    ExpressionOpCode->Signature = EXPRESSION_OPCODE_SIGNATURE;
    ExpressionOpCode->Operand = Operand;
    InitializeListHead (&ExpressionOpCode->Link);

    Value = &ExpressionOpCode->Value;

    switch (Operand) {
      case EFI_IFR_EQ_ID_VAL_OP:
        CopyMem (&ExpressionOpCode->QuestionId, &((EFI_IFR_EQ_ID_VAL *) OpCodeData)->QuestionId, sizeof (EFI_QUESTION_ID));
        ConvertExpressionQIdToQuestionId (FormSet, &ExpressionOpCode->QuestionId);

        Value->Type = EFI_IFR_TYPE_NUM_SIZE_16;
        CopyMem (&Value->Value.u16, &((EFI_IFR_EQ_ID_VAL *) OpCodeData)->Value, sizeof (UINT16));
        break;

      case EFI_IFR_EQ_ID_ID_OP:
        CopyMem (&ExpressionOpCode->QuestionId, &((EFI_IFR_EQ_ID_ID *) OpCodeData)->QuestionId1, sizeof (EFI_QUESTION_ID));
        CopyMem (&ExpressionOpCode->QuestionId2, &((EFI_IFR_EQ_ID_ID *) OpCodeData)->QuestionId2, sizeof (EFI_QUESTION_ID));
        ConvertExpressionQIdToQuestionId (FormSet, &ExpressionOpCode->QuestionId);
        ConvertExpressionQIdToQuestionId (FormSet, &ExpressionOpCode->QuestionId2);
        break;

      case EFI_IFR_EQ_ID_VAL_LIST_OP:
        CopyMem (&ExpressionOpCode->QuestionId, &((EFI_IFR_EQ_ID_VAL_LIST *) OpCodeData)->QuestionId, sizeof (EFI_QUESTION_ID));
        ConvertExpressionQIdToQuestionId (FormSet, &ExpressionOpCode->QuestionId);

        CopyMem (&ExpressionOpCode->ListLength, &((EFI_IFR_EQ_ID_VAL_LIST *) OpCodeData)->ListLength, sizeof (UINT16));
        ExpressionOpCode->ValueList = AllocateCopyPool (ExpressionOpCode->ListLength * sizeof (UINT16), &((EFI_IFR_EQ_ID_VAL_LIST *) OpCodeData)->ValueList);
        break;

      case EFI_IFR_TO_STRING_OP:
      case EFI_IFR_FIND_OP:
        ExpressionOpCode->Format = (( EFI_IFR_TO_STRING *) OpCodeData)->Format;
        break;

      case EFI_IFR_STRING_REF1_OP:
        Value->Type = EFI_IFR_TYPE_STRING;
        CopyMem (&Value->Value.string, &(( EFI_IFR_STRING_REF1 *) OpCodeData)->StringId, sizeof (EFI_STRING_ID));
        break;

      case EFI_IFR_RULE_REF_OP:
        ExpressionOpCode->RuleId = (( EFI_IFR_RULE_REF *) OpCodeData)->RuleId;
        break;

      case EFI_IFR_SPAN_OP:
        ExpressionOpCode->Flags = (( EFI_IFR_SPAN *) OpCodeData)->Flags;
        break;

      case EFI_IFR_SECURITY_OP:
        CopyMem (&ExpressionOpCode->Guid, &((EFI_IFR_SECURITY *) OpCodeData)->Permissions, sizeof (EFI_GUID));
        break;

      case EFI_IFR_MATCH2_OP:
        CopyMem (&ExpressionOpCode->Guid, &((EFI_IFR_MATCH2 *) OpCodeData)->SyntaxType, sizeof (EFI_GUID));
        break;

      case EFI_IFR_QUESTION_REF1_OP:
        CopyMem (&ExpressionOpCode->QuestionId, &((EFI_IFR_EQ_ID_VAL_LIST *) OpCodeData)->QuestionId, sizeof (EFI_QUESTION_ID));
        ConvertExpressionQIdToQuestionId (FormSet, &ExpressionOpCode->QuestionId);
        break;

      case EFI_IFR_QUESTION_REF3_OP:
        if (OpCodeLength >= sizeof (EFI_IFR_QUESTION_REF3_2)) {
          CopyMem (&ExpressionOpCode->DevicePath, &(( EFI_IFR_QUESTION_REF3_2 *) OpCodeData)->DevicePath, sizeof (EFI_STRING_ID));

          if (OpCodeLength >= sizeof (EFI_IFR_QUESTION_REF3_3)) {
            CopyMem (&ExpressionOpCode->Guid, &(( EFI_IFR_QUESTION_REF3_3 *) OpCodeData)->Guid, sizeof (EFI_GUID));
          }
        }
        break;

      //
      // constant
      //
      case EFI_IFR_TRUE_OP:
        Value->Type = EFI_IFR_TYPE_BOOLEAN;
        Value->Value.b = TRUE;
        break;

      case EFI_IFR_FALSE_OP:
        Value->Type = EFI_IFR_TYPE_BOOLEAN;
        Value->Value.b = FALSE;
        break;

      case EFI_IFR_ONE_OP:
        Value->Type = EFI_IFR_TYPE_NUM_SIZE_8;
        Value->Value.u8 = 1;
        break;

      case EFI_IFR_ZERO_OP:
        Value->Type = EFI_IFR_TYPE_NUM_SIZE_8;
        Value->Value.u8 = 0;
        break;

      case EFI_IFR_ONES_OP:
        Value->Type = EFI_IFR_TYPE_NUM_SIZE_64;
        Value->Value.u64 = 0xffffffffffffffffULL;
        break;

      case EFI_IFR_UINT8_OP:
        Value->Type = EFI_IFR_TYPE_NUM_SIZE_8;
        Value->Value.u8 = (( EFI_IFR_UINT8 *) OpCodeData)->Value;
        break;

      case EFI_IFR_UINT16_OP:
        Value->Type = EFI_IFR_TYPE_NUM_SIZE_16;
        CopyMem (&Value->Value.u16, &(( EFI_IFR_UINT16 *) OpCodeData)->Value, sizeof (UINT16));
        break;

      case EFI_IFR_UINT32_OP:
        Value->Type = EFI_IFR_TYPE_NUM_SIZE_32;
        CopyMem (&Value->Value.u32, &(( EFI_IFR_UINT32 *) OpCodeData)->Value, sizeof (UINT32));
        break;

      case EFI_IFR_UINT64_OP:
        Value->Type = EFI_IFR_TYPE_NUM_SIZE_64;
        CopyMem (&Value->Value.u64, &(( EFI_IFR_UINT64 *) OpCodeData)->Value, sizeof (UINT64));
        break;

      case EFI_IFR_UNDEFINED_OP:
        Value->Type = EFI_IFR_TYPE_UNDEFINED;
        break;

      case EFI_IFR_VERSION_OP:
        Value->Type = EFI_IFR_TYPE_NUM_SIZE_16;
        Value->Value.u16 = EFI_IFR_SPECIFICATION_VERSION;
        break;

      default:
        break;
    }

    InsertTailList (&FormExpression->OpCodeListHead, &ExpressionOpCode->Link);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
ReaplceExpressionList (
  IN     FORM_BROWSER_FORMSET                 *FormSet,
  IN     FORM_BROWSER_FORM                    *Form,
  IN OUT FORM_BROWSER_STATEMENT               *Statement,
  UINTN                                       NumberExpression,
  H2O_EXPRESSION_INFO                         **ExpressionInfoList
  )
{
  EFI_STATUS                          Status;
  UINTN                               Index;
  UINTN                               LastExpressionIdx;

  //
  // only reserve the last expression when expression type is equal
  //
  for (LastExpressionIdx = NumberExpression - 1; LastExpressionIdx > 0; LastExpressionIdx--) {
    if (ExpressionInfoList[LastExpressionIdx] == NULL) {
      continue;
    }
    for (Index = 0; Index < LastExpressionIdx; Index++) {
      if (ExpressionInfoList[Index] != NULL &&
          ((EFI_IFR_OP_HEADER *)ExpressionInfoList[Index]->ExpressionOpcodes)->OpCode
        == ((EFI_IFR_OP_HEADER *)ExpressionInfoList[LastExpressionIdx]->ExpressionOpcodes)->OpCode) {
        ExpressionInfoList[Index] = NULL;
      }
    }
  }

  for (Index = 0; Index < NumberExpression; Index++) {
    if (ExpressionInfoList[Index] == NULL) {
      continue;
    }

    Status = ReplaceExpression (FormSet, Form, Statement, ExpressionInfoList[Index]);
    if (EFI_ERROR (Status)){
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
 Process VFCF expression for formset.

 @param[in] FormSet               Pointer to formset.

 @retval EFI_SUCCESS              Successfully process VFCF property
 @retval EFI_INVALID_PARAMETER    Input parameter is NULL
 @retval Other                    Fail to get layout
**/
EFI_STATUS
FBProcessVfcfExpression (
  IN FORM_BROWSER_FORMSET                     *FormSet
  )
{
  EFI_STATUS                                  Status;
  LIST_ENTRY                                  *FormLink;
  FORM_BROWSER_FORM                           *Form;
  LIST_ENTRY                                  *StatementLink;
  FORM_BROWSER_STATEMENT                      *Statement;
  UINT16                                      Position;
  UINTN                                       NumberExpression;
  H2O_EXPRESSION_INFO                         **ExpressionInfoList;

  ASSERT (FormSet != NULL);
  if (FormSet == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!HaveVfcfFormsetInfo (&FormSet->Guid)) {
    return EFI_SUCCESS;
  }

  FormLink = GetFirstNode (&FormSet->FormListHead);
  while (!IsNull (&FormSet->FormListHead, FormLink)) {
    Form     = FORM_BROWSER_FORM_FROM_LINK (FormLink);
    FormLink = GetNextNode (&FormSet->FormListHead, FormLink);
    if (!HaveVfcfFormInfo (&FormSet->Guid, Form->FormId)) {
      continue;
    }

    Position = 0;
    StatementLink = GetFirstNode (&Form->StatementListHead);
    while (!IsNull (&Form->StatementListHead, StatementLink)) {
      Statement     = FORM_BROWSER_STATEMENT_FROM_LINK (StatementLink);
      StatementLink = GetNextNode (&Form->StatementListHead, StatementLink);

      Status = GetVfcfStatementExpressionList (
                 &FormSet->Guid,
                 Form->FormId,
                 (BOOLEAN) (Statement->QuestionId != 0),
                 Statement->QuestionId,
                 Position,
                 &NumberExpression,
                 &ExpressionInfoList
                 );
      if (EFI_ERROR (Status) && Status == EFI_NOT_FOUND) {
        continue;
      }

      if (EFI_ERROR (Status)) {
        return Status;
      }

      Status = ReaplceExpressionList (
                 FormSet,
                 Form,
                 Statement,
                 NumberExpression,
                 ExpressionInfoList
                 );
      if (EFI_ERROR (Status)) {
        return Status;
      }

      FreePool (ExpressionInfoList);

      Position++;
    }
  }

  return EFI_SUCCESS;
}

