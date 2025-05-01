/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#include "AodDxe.h"
AOD_MOUDLE_NAME ("AodIpml")
extern EFI_GUID mFormSetGuid;
BOOLEAN  mFirstEnterAODForm           = FALSE;

/**
  This function allows a caller to set defulat for AOD varstore

  @param[in]   This              Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]   Name              TBD

  @retval EFI_SUCCESS            The Results is filled with the requested values.

**/
EFI_STATUS
EFIAPI
AodLoadDefault (
  IN CONST AOD_FORMSET_MANAGE_PROTOCOL           *This,
  IN  CHAR16                                     *Name OPTIONAL
  )
{
  VOID                                    *AodConfig;
  EFI_STATUS                              Status;
  Status = AodGetConfiguration (&AodConfig);
  if (!EFI_ERROR (Status)) {
    Status = AodDispatchInitTable (AOD_CONFIG_LOAD_DEFAULT, AodConfig);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG ((EFI_D_ERROR, "  \"AodSetup\" Build Error, Status = %r\n", Status));
      return Status;
    }
    Status = AodSetConfiguration (AodConfig, ((AOD_VARIABLE_HEADER *)AodConfig)->AodVariableSize);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG ((EFI_D_ERROR, "  \"AodSetup\" Build Error,VarSize=%04x Status = %r\n", \
        ((AOD_VARIABLE_HEADER *)AodConfig)->AodVariableSize, Status));
      return Status;
    }
  }
  return Status;
}
/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.

  @param[in]   This              Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]   Request           A null-terminated Unicode string in
                                 <ConfigRequest> format.
  @param[out]  Progress          On return, points to a character in the Request
                                 string. Points to the string's null terminator if
                                 request was successful. Points to the most recent
                                 '&' before the first failing name/value pair (or
                                 the beginning of the string if the failure is in
                                 the first name/value pair) if the request was not
                                 successful.
  @param[out]  Results           A null-terminated Unicode string in
                                 <ConfigAltResp> format which has all values filled
                                 in for the names in the Request string. String to
                                 be allocated by the called function.

  @retval EFI_SUCCESS            The Results is filled with the requested values.

**/
EFI_STATUS
EFIAPI
AodExtractConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL        *This,
  IN CONST EFI_STRING                            Request,
       OUT EFI_STRING                            *Progress,
       OUT EFI_STRING                            *Results
  )
{
  EFI_STATUS                       Status;
  UINTN                            BufferSize;
  VOID                             *Buffer;
  AOD_DXE_PRIVATE_DATA             *Private;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;
  EFI_STRING                       ConfigRequestHdr;
  EFI_STRING                       ConfigRequest;
  BOOLEAN                          AllocatedRequest;
  UINTN                            Size;
  CHAR16                           *StrPointer;

  if (This == NULL || Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Initialize the local variables.
  //
  ConfigRequestHdr = NULL;
  ConfigRequest    = NULL;
  Size             = 0;
  *Progress = Request;
  AllocatedRequest = FALSE;

  Private = AOD_DXE_PRIVATE_FROM_CONFIGACCESS (This);
  HiiConfigRouting = Private->HiiConfigRouting;

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize   = 0;
  Buffer       = NULL;
  Status = gRT->GetVariable (
                  AOD_SETUP_VARIABLE_NAME,
                  &gAodSystemConfigurationGuid,
                  NULL,
                  &BufferSize,
                  Buffer
                  );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    Status = gRT->GetVariable (
                    AOD_SETUP_VARIABLE_NAME,
                    &gAodSystemConfigurationGuid,
                    NULL,
                    &BufferSize,
                    Private->IfrData
                    );
  }
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (Request == NULL) {
    //
    // Request is set to NULL, construct full request string.
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&gAodSystemConfigurationGuid, AOD_SETUP_VARIABLE_NAME, Private->DriverHandle);
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    if (ConfigRequest == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
    ConfigRequestHdr = NULL;
  } else {
    //
    // Check routing data in <ConfigHdr>.
    // Note: if only one Storage is used, then this checking could be skipped.
    //
    if (!HiiIsConfigHdrMatch (Request, &gAodSystemConfigurationGuid, AOD_SETUP_VARIABLE_NAME)) {
      return EFI_NOT_FOUND;
    }
    //
    // Set Request to the unified request string.
    //
    ConfigRequest = Request;
    //
    // Check whether Request includes Request Element.
    //
    if (StrStr (Request, L"OFFSET") == NULL) {
      //
      // Check Request Element does exist in Reques String
      //
      StrPointer = StrStr (Request, L"PATH");
      if (StrPointer == NULL) {
        return EFI_INVALID_PARAMETER;
      }
      if (StrStr (StrPointer, L"&") == NULL) {
        Size = (StrLen (Request) + 32 + 1) * sizeof (CHAR16);
        ConfigRequest = AllocateZeroPool (Size);
        ASSERT (ConfigRequest != NULL);
        AllocatedRequest = TRUE;
        UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", Request, (UINT64) BufferSize);
      }
    }
  }

  Status = HiiConfigRouting->BlockToConfig (
                                HiiConfigRouting,
                                ConfigRequest,
                                Private->IfrData,
                                BufferSize,
                                Results,
                                Progress
                                );

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
    ConfigRequest = NULL;
  }

  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

  return Status;
} // AodExtractConfig

/**
  This function is called to provide results data to the driver.

  @param[in]  This               Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  QuestionId         A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[out] ActionRequest      On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.

**/
EFI_STATUS
EFIAPI
AodCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL      *This,
  IN     EFI_BROWSER_ACTION                    Action,
  IN     EFI_QUESTION_ID                       QuestionId,
  IN     UINT8                                 Type,
  IN     EFI_IFR_TYPE_VALUE                    *Value,
     OUT EFI_BROWSER_ACTION_REQUEST            *ActionRequest
  )
{
  EFI_STATUS                       Status;
  AOD_DXE_PRIVATE_DATA             *Private;
  UINTN                            BufferSize;
  AOD_SETUP_CALLBACK               *pInstance;
  LIST_ENTRY                       *Link;
  STATIC BOOLEAN                   AodCallbackInvoked = FALSE;
  Status = EFI_SUCCESS;
  Private = AOD_DXE_PRIVATE_FROM_CONFIGACCESS (This);
  if ((Action == EFI_BROWSER_ACTION_FORM_OPEN) && (mFirstEnterAODForm == FALSE)) {
    //
    // Dispatch all instace in the cmd list
    //
    Link  = GetFirstNode (&Private->CallbackList);
    while (!IsNull (&Private->CallbackList, Link)) {
      pInstance = AOD_SETUP_CALLBACK_NODE_FROM_LINK (Link);
      if (pInstance->UpdateForm) {
        pInstance->Callback (Private->HiiHandle, 0, 0, 0, NULL, NULL, ActionRequest);
      }
      Link = GetNextNode (&Private->CallbackList, Link);
    }
    mFirstEnterAODForm = TRUE;
  }
  //
  // Don't need to do anything except for CHANGING, CHANGED, RETRIEVE, DEFAULT_STANDARD actions here.
  // Skip others to speed up response time.
  //
  if ((Action != EFI_BROWSER_ACTION_FORM_OPEN) && (Action != EFI_BROWSER_ACTION_CHANGING) && (Action != EFI_BROWSER_ACTION_CHANGED) &&
      (Action != EFI_BROWSER_ACTION_RETRIEVE) && (Action != EFI_BROWSER_ACTION_DEFAULT_STANDARD)) {
    return EFI_SUCCESS;
  }
  if ((Value == NULL) || (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((Type == EFI_IFR_TYPE_STRING) && (Value->string == 0)) {
    return EFI_INVALID_PARAMETER;
  }


  //
  // Retrive data from Browser
  //
  BufferSize =  ((AOD_VARIABLE_HEADER *)Private->IfrData)->AodVariableSize;
  if (!HiiGetBrowserData (&gAodSystemConfigurationGuid,
                         (CHAR16 *) AOD_SETUP_VARIABLE_NAME,
                         BufferSize,
                         (UINT8 *)Private->IfrData)) {
    return EFI_NOT_FOUND;
  }
  //
  // Dispatch all instace in the cmd list
  //
  Link  = GetFirstNode (&Private->CallbackList);
  while (!IsNull (&Private->CallbackList, Link)) {
    pInstance = AOD_SETUP_CALLBACK_NODE_FROM_LINK (Link);
    if (!pInstance->UpdateForm) {
      pInstance->Callback (Private->HiiHandle, Action, QuestionId, Type, Value, Private->IfrData, ActionRequest);
    }
    Link = GetNextNode (&Private->CallbackList, Link);
  }
  switch (Action) {
    case EFI_BROWSER_ACTION_RETRIEVE:
      AodCallbackInvoked = TRUE;
      break;

    case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
//[-start-220906-IB14740262-modify]//
      Status = EFI_UNSUPPORTED;
//[-end-220906-IB14740262-modify]//
      break;

    default:
      break;

  }
  if (!EFI_ERROR (Status)) {
    //
    // Pass changed uncommitted data back to Form Browser
    //
    if (HiiSetBrowserData (&gAodSystemConfigurationGuid,
                      (CHAR16 *) AOD_SETUP_VARIABLE_NAME,
                      BufferSize,
                      (UINT8 *)Private->IfrData,
                      NULL)) {
      return EFI_SUCCESS;
    }
  }
  return Status;
}

/**
  This function processes the results of changes in configuration.

  @param[in]  This               Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]  Configuration      A null-terminated Unicode string in <ConfigResp>
                                 format.
  @param[out] Progress           A pointer to a string filled in with the offset of
                                 the most recent '&' before the first failing
                                 name/value pair (or the beginning of the string if
                                 the failure is in the first name/value pair) or
                                 the terminating NULL if all was successful.

  @retval EFI_SUCCESS            The Results is processed successfully.
  @retval EFI_INVALID_PARAMETER  Configuration is NULL.
  @retval EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                 driver.

**/
EFI_STATUS
EFIAPI
AodRouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL      *This,
  IN CONST EFI_STRING                          Configuration,
       OUT EFI_STRING                          *Progress
  )
{
  EFI_STATUS                       Status;
  UINTN                            BufferSize;
  AOD_DXE_PRIVATE_DATA             *Private;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = AOD_DXE_PRIVATE_FROM_CONFIGACCESS (This);
  HiiConfigRouting = Private->HiiConfigRouting;
  *Progress = Configuration;

  //
  // Check routing data in <ConfigHdr>.
  // Note: if only one Storage is used, then this checking could be skipped.
  //
  if (!HiiIsConfigHdrMatch (Configuration, &gAodSystemConfigurationGuid, AOD_SETUP_VARIABLE_NAME)) {
    return EFI_NOT_FOUND;
  }

  //
  // Convert <ConfigResp> to buffer data by helper function ConfigToBlock()
  //
  BufferSize = ((AOD_VARIABLE_HEADER *)Private->IfrData)->AodVariableSize;
  Status = HiiConfigRouting->ConfigToBlock (
                               HiiConfigRouting,
                               Configuration,
                               Private->IfrData,
                               &BufferSize,
                               Progress
                               );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Store Buffer Storage back to EFI variable
  //
  BufferSize = ((AOD_VARIABLE_HEADER *)Private->IfrData)->AodVariableSize;
  Status = AodSetConfiguration (Private->IfrData, BufferSize);
  AOD_DEBUG ((EFI_D_INFO, "Status = %r\n", Status));
  if (!EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_INFO, "Start to Sync Apcb\n"));
    Status = AodSendCommunicateBuffer (AOD_SMM_COMM_VARIABLE_HOOK, Private->IfrData, BufferSize);
    AOD_DEBUG ((EFI_D_INFO, "End to Sync Apcb\n"));
  }
  return Status;
}


