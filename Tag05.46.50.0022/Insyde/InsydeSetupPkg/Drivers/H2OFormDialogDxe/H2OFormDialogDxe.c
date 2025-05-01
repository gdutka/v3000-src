/** @file

  Initial and callback functions for H2O Form Dialog

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "H2OFormDialogDxe.h"

CHAR16                                    *mVariableName = H2O_FORM_DIALOG_VARSTORE_NAME;
EFI_GUID                                  mVariableGuid = H2O_FORM_DIALOG_CONFIGURATION_GUID;
HII_VENDOR_DEVICE_PATH                    mHiiVendorDevicePathTemplate = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    H2O_FORM_DIALOG_FORMSET_CLASS_ID_LMDE_COMMON_QUESTION
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  if (This == NULL || Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Request == NULL) {
    *Progress = NULL;
    *Results  = NULL;
    return EFI_NOT_FOUND;
  }

  *Progress = Request;

  if (!HiiIsConfigHdrMatch (Request, &mVariableGuid, mVariableName)) {
    return EFI_NOT_FOUND;
  }

  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  if (This == NULL || Configuration == NULL || Progress == NULL) {
   return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;

  if (!HiiIsConfigHdrMatch (Configuration, &mVariableGuid, mVariableName)) {
    return EFI_NOT_FOUND;
  }

  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
DriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest
  )
{
  EFI_STATUS                                 Status;

  if ((Value == NULL &&
      (Action != EFI_BROWSER_ACTION_FORM_OPEN) && (Action != EFI_BROWSER_ACTION_FORM_CLOSE))||
      (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;

  switch (Action) {

  case EFI_BROWSER_ACTION_FORM_OPEN:
    Status = EFI_SUCCESS;
    break;

  case EFI_BROWSER_ACTION_FORM_CLOSE:
    Status = EFI_SUCCESS;
    break;

  case EFI_BROWSER_ACTION_CHANGING:
    Status = EFI_SUCCESS;
    break;

  case EFI_BROWSER_ACTION_CHANGED:
    break;

  case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
    break;

  case EFI_BROWSER_ACTION_RETRIEVE:
    break;

  default:
    break;
  }

  return Status;
}

EFI_STATUS
InitConfiguration (
  IN EFI_HANDLE                         DriverHandle,
  IN H2O_FORM_DIALOG_CONFIGURATION      *Config
  )
{
  EFI_STATUS                            Status;
  UINTN                                 BufferSize;
  BOOLEAN                               ActionFlag;
  EFI_STRING                            ConfigRequestHdr;

  ZeroMem (Config, sizeof (H2O_FORM_DIALOG_CONFIGURATION));

  //
  // Try to read NV config EFI variable first
  //
  ConfigRequestHdr = HiiConstructConfigHdr (&mVariableGuid, mVariableName, DriverHandle);
  ASSERT (ConfigRequestHdr != NULL);
  if (ConfigRequestHdr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  BufferSize = sizeof (H2O_FORM_DIALOG_CONFIGURATION);
  Status = gRT->GetVariable (mVariableName, &mVariableGuid, NULL, &BufferSize, Config);
  if (EFI_ERROR (Status)) {
    //
    // Store zero data Buffer Storage to EFI variable
    //
    Status = CommonSetVariable (
               mVariableName,
               &mVariableGuid,
               EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
               sizeof (H2O_FORM_DIALOG_CONFIGURATION),
               Config
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //
    // EFI variable for NV config doesn't exit, we should build this variable
    // based on default values stored in IFR
    //
    ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!ActionFlag) {
      return EFI_INVALID_PARAMETER;
    }
  } else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    ActionFlag = HiiValidateSettings (ConfigRequestHdr);
    if (!ActionFlag) {
      return EFI_INVALID_PARAMETER;
    }
  }
  FreePool (ConfigRequestHdr);

  return Status;
}

typedef struct {
  UINT8                              Operand;
  EFI_GUID                           FormsetGuid;
  UINT8                              *VfrBin;
} H2O_FORM_DIALOG_OPERAND_VFR_INFO;


STATIC
VOID
DestroyH2OFormDialogInfo (
  IN H2O_FORM_DIALOG_INFO               *H2OFormDialogInfo
  )
{
  if (H2OFormDialogInfo == NULL) {
    return;
  }

  if (H2OFormDialogInfo->HiiHandle != NULL) {
    HiiRemovePackages (H2OFormDialogInfo->HiiHandle);
  }

  if (H2OFormDialogInfo->DriverHandle != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           H2OFormDialogInfo->DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &H2OFormDialogInfo->VendorDevicePath,
           &gEfiHiiConfigAccessProtocolGuid,
           &H2OFormDialogInfo->HiiConfigAccess,
           NULL
           );
  }

  FreePool (H2OFormDialogInfo);
}

STATIC
H2O_FORM_DIALOG_INFO *
CreateH2OFormDialogInfo (
  IN UINT8                              Operand,
  IN EFI_GUID                           *FormsetGuid,
  IN UINT8                              *VfrBin
  )
{
  EFI_STATUS                            Status;
  H2O_FORM_DIALOG_INFO                  *FormDialogInfo;

  FormDialogInfo = AllocateZeroPool (sizeof (H2O_FORM_DIALOG_INFO));
  if (FormDialogInfo == NULL) {
    return NULL;
  }

  CopyGuid (&FormDialogInfo->FormsetId, FormsetGuid);
  FormDialogInfo->FormId  = H2O_FORM_DIALOG_FORM_ID_ROOT;
  FormDialogInfo->Operand = Operand;

  CopyMem (&FormDialogInfo->VendorDevicePath, &mHiiVendorDevicePathTemplate, sizeof (HII_VENDOR_DEVICE_PATH));
  CopyGuid (&FormDialogInfo->VendorDevicePath.VendorDevicePath.Guid, FormsetGuid);

  FormDialogInfo->HiiConfigAccess.ExtractConfig = ExtractConfig;
  FormDialogInfo->HiiConfigAccess.RouteConfig   = RouteConfig;
  FormDialogInfo->HiiConfigAccess.Callback      = DriverCallback;

  FormDialogInfo->DriverHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &FormDialogInfo->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &FormDialogInfo->VendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &FormDialogInfo->HiiConfigAccess,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DestroyH2OFormDialogInfo(FormDialogInfo);
    return NULL;
  }

  //
  // Publish our HII data
  //
  FormDialogInfo->HiiHandle = HiiAddPackages (
                                FormsetGuid,
                                FormDialogInfo->DriverHandle,
                                H2OFormDialogDxeStrings,
                                H2OFormDialogDxeImages,
                                VfrBin,
                                NULL
                                );
  if (FormDialogInfo->HiiHandle == NULL) {
    DestroyH2OFormDialogInfo(FormDialogInfo);
    return NULL;
  }

  Status = InitConfiguration (FormDialogInfo->DriverHandle, &FormDialogInfo->Configuration);
  if (EFI_ERROR (Status)) {
    DestroyH2OFormDialogInfo(FormDialogInfo);
    return NULL;
  }

  return FormDialogInfo;
}

EFI_STATUS
EFIAPI
H2OFormDialogEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  UINT32                                Index;
  H2O_FORM_DIALOG_OPERAND_VFR_INFO      OperandList[] = {
                                          {EFI_IFR_CHECKBOX_OP    , H2O_FORM_DIALOG_FORMSET_ID_LMDE_CHECKBOX    , H2OFormDialogFsCheckboxVfrBin   },
                                          {EFI_IFR_DATE_OP        , H2O_FORM_DIALOG_FORMSET_ID_LMDE_DATE        , H2OFormDialogFsDateVfrBin       },
                                          {EFI_IFR_ONE_OF_OP      , H2O_FORM_DIALOG_FORMSET_ID_LMDE_ONE_OF      , H2OFormDialogFsOneOfVfrBin      },
                                          {EFI_IFR_ORDERED_LIST_OP, H2O_FORM_DIALOG_FORMSET_ID_LMDE_ORDERED_LIST, H2OFormDialogFsOrderedListVfrBin},
                                          {EFI_IFR_NUMERIC_OP     , H2O_FORM_DIALOG_FORMSET_ID_LMDE_NUMERIC     , H2OFormDialogFsNumericVfrBin    },
                                          {EFI_IFR_PASSWORD_OP    , H2O_FORM_DIALOG_FORMSET_ID_LMDE_PASSWORD    , H2OFormDialogFsPasswordVfrBin   },
                                          {EFI_IFR_STRING_OP      , H2O_FORM_DIALOG_FORMSET_ID_LMDE_STRING      , H2OFormDialogFsStringVfrBin     },
                                          {EFI_IFR_TIME_OP        , H2O_FORM_DIALOG_FORMSET_ID_LMDE_TIME        , H2OFormDialogFsTimeVfrBin       },
                                          };

  for (Index = 0; Index < ARRAY_SIZE(OperandList); Index++) {
    CreateH2OFormDialogInfo (OperandList[Index].Operand, &OperandList[Index].FormsetGuid, OperandList[Index].VfrBin);
  }

  return EFI_SUCCESS;
}

