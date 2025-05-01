/** @file
 Define function of statement.
;******************************************************************************
;* Copyright (c) 2013 - 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#include "InternalH2OFormBrowser.h"
#include <Library/H2ODisplayEngineLib.h>
#include <Guid/H2OFormDialog.h>

BOOLEAN      mInScopeSubtitle;

#define IS_PREDEFINED_STRING_ID(StringId) ((BOOLEAN) (StringId == STR_POPUP_CURRENT_Q_PROMPT) || \
                                                     (StringId == STR_POPUP_CURRENT_Q_HELP  ) || \
                                                     (StringId == STR_POPUP_CURRENT_F_TITLE ) || \
                                                     (StringId == STR_POPUP_CURRENT_FS_TITLE))

#define IS_PREDEFINED_IMAGE_ID(ImageId)   ((BOOLEAN) (ImageId == IMG_POPUP_CURRENT_Q_PROMPT) || \
                                                     (ImageId == IMG_POPUP_CURRENT_Q_HELP  ) || \
                                                     (ImageId == IMG_POPUP_CURRENT_F_TITLE ) || \
                                                     (ImageId == IMG_POPUP_CURRENT_FS_TITLE))

STATIC
EFI_IMAGE_INPUT *
GetPredefinedIdImage (
  IN EFI_IMAGE_ID                      ImageId
  )
{
  EFI_STATUS                           Status;
  H2O_FORM_BROWSER_SM                  *SMInfo;
  UINT32                               Index;
  EFI_IMAGE_INPUT                      *Image;
  EFI_IMAGE_INPUT                      *NewImage;

  SMInfo = NULL;
  Image  = NULL;
  switch (ImageId) {

  case IMG_POPUP_CURRENT_Q_PROMPT:
  case IMG_POPUP_CURRENT_Q_HELP:
    if (mFBPrivate.FB.CurrentQ != NULL) {
      Image = mFBPrivate.FB.CurrentQ->Image;
    }
    break;

  case IMG_POPUP_CURRENT_F_TITLE:
    if (mFBPrivate.FB.CurrentP != NULL) {
      Image = mFBPrivate.FB.CurrentP->Image;
    }
    break;

  case IMG_POPUP_CURRENT_FS_TITLE:
    if (mFBPrivate.FB.CurrentP == NULL) {
      break;
    }

    Status = mFBPrivate.FB.GetSMInfo (&mFBPrivate.FB, &SMInfo);
    if (EFI_ERROR (Status) || SMInfo == NULL) {
      break;
    }
    for (Index = 0; Index < SMInfo->NumberOfSetupMenus; Index++) {
      if ((SMInfo->SetupMenuInfoList[Index].PageId >> 16) == (mFBPrivate.FB.CurrentP->PageId >> 16)) {
        Image = SMInfo->SetupMenuInfoList[Index].PageImage;
        break;
      }
    }
    break;
  }
  if (Image == NULL || Image->Width == 0 || Image->Height == 0 || Image->Bitmap == NULL) {
    return NULL;
  }

  NewImage = AllocateCopyPool (sizeof (EFI_IMAGE_INPUT), Image);
  if (NewImage != NULL) {
    NewImage->Bitmap = AllocateCopyPool (
                         Image->Width * Image->Height * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL),
                         Image->Bitmap
                         );
    if (NewImage->Bitmap == NULL) {
      FreePool (NewImage);
      NewImage = NULL;
    }
  }

  if (SMInfo != NULL) {
    FreeSetupMenuData (SMInfo);
  }
  return NewImage;
}

STATIC
CHAR16 *
GetPredefinedIdString (
  IN EFI_STRING_ID                     StringId,
  IN EFI_HII_HANDLE                    HiiHandle
  )
{
  EFI_STATUS                           Status;
  H2O_FORM_BROWSER_SM                  *SMInfo;
  CHAR16                               *String;
  CHAR16                               *NewString;

  SMInfo = NULL;
  String = NULL;
  switch (StringId) {

  case STR_POPUP_CURRENT_Q_PROMPT:
    if (mFBPrivate.FB.CurrentQ != NULL) {
      String = mFBPrivate.FB.CurrentQ->Prompt;
    }
    break;

  case STR_POPUP_CURRENT_Q_HELP:
    if (mFBPrivate.FB.CurrentQ != NULL) {
      String = mFBPrivate.FB.CurrentQ->Help;
    }
    break;

  case STR_POPUP_CURRENT_F_TITLE:
    if (mFBPrivate.FB.CurrentP != NULL) {
      String = mFBPrivate.FB.CurrentP->PageTitle;
    }
    break;

  case STR_POPUP_CURRENT_FS_TITLE:
    Status = mFBPrivate.FB.GetSMInfo (&mFBPrivate.FB, &SMInfo);
    if (EFI_ERROR (Status) || SMInfo == NULL) {
      break;
    }

    String = SMInfo->TitleString;
    break;
  }
  if (String == NULL) {
    return NULL;
  }

  NewString = AllocateCopyPool (StrSize (String), String);

  if (SMInfo != NULL) {
    FreeSetupMenuData (SMInfo);
  }
  return NewString;
}

BOOLEAN
IsSelectable (
  IN FORM_BROWSER_STATEMENT     *Statement
  )
{
  EXPRESS_RESULT                ExpressResult;

  ASSERT (Statement);
  if (Statement == NULL) {
    return FALSE;
  }

  ExpressResult = EvaluateExpressionList(Statement->Expression, FALSE, NULL, NULL);
  if (ExpressResult == ExpressGrayOut || Statement->Operand == EFI_IFR_SUBTITLE_OP) {
    return FALSE;
  }
  if (Statement->Locked) {
    return FALSE;
  }
  if ((Statement->QuestionFlags & EFI_IFR_FLAG_READ_ONLY) != 0) {
    return FALSE;
  }

  if (Statement->QuestionId != 0 ||
      Statement->Operand == EFI_IFR_REF_OP ||
      Statement->Operand == EFI_IFR_ACTION_OP ||
      Statement->Operand == EFI_IFR_ONE_OF_OP ||
      Statement->Operand == EFI_IFR_NUMERIC_OP ||
      Statement->Operand == EFI_IFR_ORDERED_LIST_OP ||
      Statement->Operand == EFI_IFR_CHECKBOX_OP ||
      Statement->Operand == EFI_IFR_STRING_OP ||
      Statement->Operand == EFI_IFR_PASSWORD_OP ||
      Statement->Operand == EFI_IFR_DATE_OP ||
      Statement->Operand == EFI_IFR_TIME_OP ||
      Statement->Operand == EFI_IFR_RESET_BUTTON_OP) {
    return TRUE;
  }

  return FALSE;
}

/**
 Initialize statement data.

 @param[in]      FormSet          Specified FormSet
 @param[in]      Form             Specified Form
 @param[in]      ExpressResult    Result of the expression list
 @param[in]      Page             Specified page
 @param[in, out] Statement        Formbrowser page identifer

**/
VOID
InitH2OStatement (
  IN     FORM_BROWSER_FORMSET      *FormSet,
  IN     FORM_BROWSER_FORM         *Form,
  IN     EXPRESS_RESULT               ExpressResult,
  IN     H2O_FORM_BROWSER_P        *Page,
  IN OUT FORM_BROWSER_STATEMENT    *Statement
  )
{
  H2O_FORM_BROWSER_S            *H2OStatement;
  EFI_IMAGE_INPUT               *ImageIn;
  EFI_STATUS                    Status;
  BOOLEAN                       HavePassword;

  ZeroMem (&Statement->Statement, sizeof (H2O_FORM_BROWSER_S));
  H2OStatement                  = &Statement->Statement;
  H2OStatement->Signature       = H2O_FORM_BROWSER_STATEMENT_SIGNATURE;
  H2OStatement->Size            = sizeof (H2O_FORM_BROWSER_Q);
  H2OStatement->PageId          = Page->PageId;
  H2OStatement->StatementId     = Statement->StatementId;
  H2OStatement->Operand         = Statement->Operand;
  H2OStatement->IfrOpCode       = Statement->OpCode;

  if (ExpressResult == ExpressGrayOut || Statement->Operand == EFI_IFR_SUBTITLE_OP) {
    H2OStatement->GrayedOut = TRUE;
  }
  if (Statement->Locked) {
    H2OStatement->Locked = TRUE;
  }
  if ((Statement->QuestionFlags & EFI_IFR_FLAG_READ_ONLY) != 0) {
    H2OStatement->ReadOnly = TRUE;
  }

  H2OStatement->Selectable      = IsSelectable (Statement);

  if (Statement->Prompt == 0) {
    H2OStatement->Prompt        = AllocateZeroPool (sizeof (CHAR16));
  } else if (Form->ModalForm && IS_PREDEFINED_STRING_ID(Statement->Prompt)) {
    H2OStatement->Prompt        = GetPredefinedIdString (Statement->Prompt, Form->FormSet->HiiHandle);
  } else {
    H2OStatement->Prompt        = GetString (Statement->Prompt, Form->FormSet->HiiHandle);
  }
  if (Statement->Help == 0) {
    H2OStatement->Help          = AllocateZeroPool (sizeof (CHAR16));
  } else if (Form->ModalForm && IS_PREDEFINED_STRING_ID(Statement->Help)) {
    H2OStatement->Help          = GetPredefinedIdString (Statement->Help, Form->FormSet->HiiHandle);
  } else {
    H2OStatement->Help          = GetString (Statement->Help, Form->FormSet->HiiHandle);
  }
  if (Statement->TextTwo == 0) {
    H2OStatement->TextTwo       = NULL;
  } else if (Form->ModalForm && IS_PREDEFINED_STRING_ID(Statement->TextTwo)) {
    H2OStatement->TextTwo       = GetPredefinedIdString (Statement->TextTwo, Form->FormSet->HiiHandle);
  } else {
    H2OStatement->TextTwo       = GetString (Statement->TextTwo, Form->FormSet->HiiHandle);
  }

  H2OStatement->QuestionId      = Statement->QuestionId;
  H2OStatement->VarStoreId      = Statement->VarStoreId;
  H2OStatement->VariableName    = Statement->VariableName;
  H2OStatement->VariableOffset  = Statement->VarStoreInfo.VarOffset;
  H2OStatement->QuestionFlags   = Statement->QuestionFlags;

  if (H2OStatement->Operand == EFI_IFR_ORDERED_LIST_OP) {
    if (GetArrayData (Statement->BufferValue, Statement->ValueType, 0) == 0) {
      GetQuestionDefault (FormSet, Form, Statement, 0);
    }
  }

  CopyHiiValue (&H2OStatement->HiiValue, &Statement->HiiValue);
  if (H2OStatement->Operand == EFI_IFR_PASSWORD_OP) {
    //
    // In order to make display engine to hide password, change HII value to speace/empty string in H2O statement.
    //
    HavePassword = (*((CHAR16 *) Statement->HiiValue.Buffer) != CHAR_NULL) ? TRUE : FALSE;

    H2OStatement->HiiValue.Type         = EFI_IFR_TYPE_STRING;
    H2OStatement->HiiValue.Buffer       = AllocateZeroPool (H2OStatement->HiiValue.BufferLen);
    if (H2OStatement->HiiValue.Buffer != NULL) {
      if (HavePassword) {
        CopyMem (H2OStatement->HiiValue.Buffer, gSpaceString, StrSize (gSpaceString));
      } else {
        CopyMem (H2OStatement->HiiValue.Buffer, gEmptyString, StrSize (gEmptyString));
      }
    }
  }

  H2OStatement->Flags           = Statement->Flags;
  H2OStatement->ContainerCount  = Statement->MaxContainers;
  H2OStatement->Minimum         = Statement->Minimum;
  H2OStatement->Maximum         = Statement->Maximum;
  H2OStatement->Step            = Statement->Step;
  H2OStatement->DefaultId       = Statement->DefaultId;
  CopyGuid (&H2OStatement->RefreshGuid, &Statement->RefreshGuid);
  H2OStatement->RefreshInterval = Statement->RefreshInterval;

  H2OStatement->Image = NULL;
  if (Form->ModalForm && IS_PREDEFINED_IMAGE_ID(Statement->ImageId)) {
    H2OStatement->Image = GetPredefinedIdImage (Statement->ImageId);
  } else if (Statement->ImageId != 0) {
    ImageIn = AllocateZeroPool (sizeof (EFI_IMAGE_INPUT));
    ASSERT (ImageIn != NULL);
    if (ImageIn == NULL) {
      return ;
    }
    Status = gHiiImage->GetImage (
                          gHiiImage,
                          FormSet->HiiHandle,
                          (EFI_IMAGE_ID) Statement->ImageId,
                          ImageIn
                          );
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status)) {
      H2OStatement->Image = ImageIn;
    } else {
      FreePool (ImageIn);
    }
  }

  H2OStatement->Animation       = NULL;

  InitH2OOption (Form, Statement);
//H2OStatement->Inconsistent;
//H2OStatement->ErrorText;

  CopyGuid (&H2OStatement->FormsetGuid, &Form->FormSet->Guid);
  H2OStatement->FormId          = Form->FormId;

}

VOID
DestroyH2OStatement (
  IN OUT FORM_BROWSER_STATEMENT *Statement
  )
{
  H2O_FORM_BROWSER_S            *H2OStatement;


  H2OStatement                  = &Statement->Statement;

  FBFreePool ((VOID **) &H2OStatement->Prompt);
  FBFreePool ((VOID **) &H2OStatement->Help);
  FBFreePool ((VOID **) &H2OStatement->TextTwo);
  if (H2OStatement->Image != NULL) {
    FBFreePool ((VOID **) &(H2OStatement->Image->Bitmap));
  }
  FBFreePool ((VOID **) &H2OStatement->Image);
  if (H2OStatement->Operand == EFI_IFR_PASSWORD_OP) {
    FBFreePool ((VOID **) &H2OStatement->HiiValue.Buffer);
  }

  DestroyH2OOption (Statement);
}

/**
 Get the position of statement in the form

 @param[in] Form                       Pointer to the form
 @param[in] Statement                  Pointer to the statement

 @return The position of statement or 0xFFFF if input parameter is invalid or not found
**/
UINT16
GetStatementPosition (
  IN FORM_BROWSER_FORM                 *Form,
  IN FORM_BROWSER_STATEMENT            *Statement
  )
{
  UINT16                               Position;
  LIST_ENTRY                           *Link;
  FORM_BROWSER_STATEMENT               *CurrentStatement;

  if (Form == NULL || Statement == NULL) {
    return 0xFFFF;
  }

  Position = 0;
  Link     = GetFirstNode (&Form->StatementListHead);
  while (!IsNull (&Form->StatementListHead, Link)) {
    CurrentStatement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
    Link             = GetNextNode (&Form->StatementListHead, Link);
    if (CurrentStatement == Statement) {
      return Position;
    }

    Position++;
  }

  return 0xFFFF;
}

/**
 Get statement display order defined by VFCF file.

 @param[in] Form                       Pointer to the form
 @param[in] StatementId                Pointer to the statement

 @return The display order value or 0x8000 if VFCF property is not found
**/
UINT16
GetStatementDisplayOrder (
  IN FORM_BROWSER_FORM                 *Form,
  IN FORM_BROWSER_STATEMENT            *Statement
  )
{
  EFI_STATUS                           Status;
  BOOLEAN                              IsQuestion;
  UINT16                               Position;
  H2O_PROPERTY_INFO                    *Property;
  H2O_PROPERTY_VALUE                   PropValue;

  IsQuestion = (BOOLEAN) (Statement->QuestionId != 0);
  Position   = GetStatementPosition (Form, Statement);

  Status = GetVfcfStatementPropertyValue (
             &Form->FormSet->Guid,
             IsQuestion,
             (IsQuestion ? Statement->QuestionId : Position),
             H2O_IFR_STYLE_TYPE_VFR_TREE_FLAG,
             H2O_STYLE_PSEUDO_CLASS_NORMAL,
             "display-order",
             NULL,
             &Property,
             &PropValue
             );
  if (!EFI_ERROR (Status)) {
    return PropValue.H2OValue.Value.U16;
  }

  return 0x8000;
}

/**
 Get statement by statement ID.

 @param[in] Form                       Pointer to the form
 @param[in] StatementId                Statement ID

 @return The pointer of statement or NULL if form is NULL or not found
**/
FORM_BROWSER_STATEMENT *
GetStatementByStatementId (
  IN FORM_BROWSER_FORM                 *Form,
  IN H2O_STATEMENT_ID                  StatementId
  )
{
  LIST_ENTRY                           *Link;
  FORM_BROWSER_STATEMENT               *Statement;

  if (Form == NULL) {
    return NULL;
  }

  Link = GetFirstNode (&Form->StatementListHead);
  while (!IsNull (&Form->StatementListHead, Link)) {
    Statement = FORM_BROWSER_STATEMENT_FROM_LINK (Link);
    Link      = GetNextNode (&Form->StatementListHead, Link);
    if (Statement->StatementId == StatementId) {
      return Statement;
    }
  }

  return NULL;
}
