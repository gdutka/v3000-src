/** @file

  Initial and callback functions for Console Redirection Configuration

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

#include "CrConfigUtil.h"
#include "CrDeviceMenu.h"
#include "CrCallback.h"
#include "CrPolicyType.h"
#include "CrConfig.h"

CR_CONFIG_MANAGER_PRIVATE_DATA            *mPrivateData = NULL;
CHAR16                                    *mVariableName = CONSOLE_REDIRECTION_VARSTORE_NAME;
HII_VENDOR_DEVICE_PATH                    mHiiVendorDevicePath0 = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    CONSOLE_REDIRECTION_FORMSET_GUID
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

/**
 Callback function when enter BDS entry.
 This function will lock CrConfig variable by EdkiiVariableLockProtocol.

 @param[in]         Event               The Event associated with callback.
 @param[in]         Handle              Handle that is associated with the registered checkpoint handler.
 
*/
VOID
EFIAPI
BdsEntryInitEvent (
  IN EFI_EVENT                Event,
  IN H2O_CP_HANDLE            Handle
  )
{
  EFI_STATUS                    Status;
  EDKII_VARIABLE_LOCK_PROTOCOL  *VariableLock;

  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **) &VariableLock);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[CrConfigDxe] Fail to locate EdkiiVariableLockProtocol. (%r)\n", Status));
    return;
  }

  Status = VariableLock->RequestToLock (VariableLock, CONSOLE_REDIRECTION_VARSTORE_NAME, &gH2OCrConfigurationGuid);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[CrConfigDxe] Fail to lock %s variable. (%r)\n", CONSOLE_REDIRECTION_VARSTORE_NAME, Status));
  }

  Status = VariableLock->RequestToLock (VariableLock, CONSOLE_REDIRECTION_VARSTORE_DEFAULT_NAME, &gH2OCrConfigurationGuid);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[CrConfigDxe] Fail to lock %s variable. (%r)\n", CONSOLE_REDIRECTION_VARSTORE_DEFAULT_NAME, Status));
  }

  Status = LockCrDeviceVar ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[CrConfigDxe] Fail to lock CrDevice variables. (%r)\n", Status));
  }

}

/**

  Check need to load default CrConfig variable or not.
    
  @retval TRUE       Need load default setting.
          FALSE      Can use current variable setting.

**/
BOOLEAN
CheckIfNeedLoadDefaultCrConfigVariable (
  VOID
  )
{
  EFI_STATUS             Status;
  CR_CONFIGURATION       *CrConfig;
  UINTN                  VarSize;
  BOOLEAN                ReturnStatus;

  VarSize         = 0;
  CrConfig        = NULL;
  ReturnStatus    = TRUE;

  //
  // If CrConfig doesn't exist in variable store region, use default value to create CrConfig and CrDevVar#### variables.
  //
  if (!IsCrConfigVariableInVariableStoreRegion ()) {
    return TRUE;
  }

  //
  // If variable exists in variable store region, need to check variable size is the same as CR_CONGIFURATION structure size or not.
  // Since CR_CONFIGURATION structure size may be changed between different BIOS version, but didn't erase variable store region when 
  // updating BIOS. So, we need to verify variable size to make sure it is the same as structure size.
  //
  Status = CommonGetVariableDataAndSize ( 
             CONSOLE_REDIRECTION_VARSTORE_NAME, 
             &gH2OCrConfigurationGuid, 
             &VarSize, 
             (VOID **)&CrConfig
             );
  if (!EFI_ERROR (Status) && (VarSize == sizeof (CR_CONFIGURATION))) {
    //
    // If size is the same, we can keep to use current CrConfig variable.
    //
    ReturnStatus = FALSE;
  } else {
    //
    // CrConfig variable size shoule be greater or equal to OFFSET_OF(CR_CONFIGURATION,ExCrPortConfig).
    // If variable size is samller than OFFSET_OF(CR_CONFIGURATION,ExCrPortConfig), we should delete it to load default setting.
    //
    if (VarSize < OFFSET_OF(CR_CONFIGURATION,ExCrPortConfig)) {
      ReturnStatus = TRUE;
    } else {
      //
      // If size mismatch, create new CrConfig variable base on current CR_CONFIG structure and sync CrConfig setting from old CrConfig variable.
      //
      Status = DealCrConfigVariableUpdate ((UINT8 *)CrConfig, VarSize);
      if (!EFI_ERROR (Status)) {
        ReturnStatus = FALSE;
      }
    }
  }

  if (CrConfig != NULL) {
    FreePool (CrConfig);
  }

  return ReturnStatus;  
}

EFI_STATUS
CrExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  EFI_STATUS                                 Status;
  UINTN                                      BufferSize;
  EFI_HII_CONFIG_ROUTING_PROTOCOL            *HiiConfigRouting;
  EFI_STRING                                 ConfigRequest;
  EFI_STRING                                 ConfigRequestHdr;
  UINTN                                      Size;
  BOOLEAN                                    AllocatedRequest;
  EFI_STRING                                 Value;
  UINTN                                      ValueStrLen;
  UINT8                                      MyNameValue0;
  CHAR16                                     BackupChar;
  CHAR16                                     *StrPointer;

  DEBUG ((EFI_D_INFO, "CRExtract: Start\n"));

  if (Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Initialize the local variables.
  //
  ConfigRequestHdr  = NULL;
  ConfigRequest     = NULL;
  Size              = 0;
  *Progress         = Request;
  AllocatedRequest  = FALSE;

  HiiConfigRouting = mPrivateData->HiiConfigRouting;

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize = sizeof (CR_CONFIGURATION);
  Status = gRT->GetVariable (
                  mVariableName,
                  &gH2OCrConfigurationGuid,
                  NULL,
                  &BufferSize,
                  &mPrivateData->Configuration
                  );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  if (Request == NULL) {
    //
    // Request is set to NULL, construct full request string.
    //

    //
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&gH2OCrConfigurationGuid, mVariableName, mPrivateData->DriverHandle);
    if (ConfigRequestHdr == NULL) {
      ASSERT (FALSE);
      return EFI_OUT_OF_RESOURCES;
    }

    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    if (ConfigRequest == NULL) {
      ASSERT (FALSE);
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
    if (!HiiIsConfigHdrMatch (Request, &gH2OCrConfigurationGuid, NULL)) {
      return EFI_NOT_FOUND;
    }
    //
    // Check whether request for EFI Varstore. EFI varstore get data
    // through hii database, not support in this path.
    //
    if (!HiiIsConfigHdrMatch (Request, &gH2OCrConfigurationGuid, mVariableName)) {
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
        ConfigRequest    = AllocateZeroPool (Size);
        ASSERT (ConfigRequest != NULL);
        if (ConfigRequest == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
        AllocatedRequest = TRUE;
        UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", Request, (UINT64)BufferSize);
      }
    }
  }

  if (StrStr (ConfigRequest, L"OFFSET") == NULL) {
    //
    // If requesting Name/Value storage, return value 0.
    //
    //
    // Allocate memory for <ConfigResp>, e.g. Name0=0x11, Name1=0x1234, Name2="ABCD"
    // <Request>   ::=<ConfigHdr>&Name0&Name1&Name2
    // <ConfigResp>::=<ConfigHdr>&Name0=11&Name1=1234&Name2=0041004200430044
    //
    BufferSize = (StrLen (ConfigRequest) + 1 + sizeof (MyNameValue0) * 2 + 1) * sizeof (CHAR16);
    *Results = AllocateZeroPool (BufferSize);
    if (*Results == NULL) {
      ASSERT (FALSE);
      return EFI_OUT_OF_RESOURCES;
    }

    StrCpyS (*Results, BufferSize / sizeof(CHAR16), ConfigRequest);
    Value = *Results;

    //
    // Append value of NameValueVar0, type is UINT8
    //
    if ((Value = StrStr (*Results, L"MyNameValue0")) != NULL) {
      MyNameValue0 = 0;

      Value += StrLen (L"MyNameValue0");
      ValueStrLen = ((sizeof (MyNameValue0) * 2) + 1);
      CopyMem (&Value[ValueStrLen], Value, StrSize (Value));

      BackupChar = Value[ValueStrLen];
      *Value++   = L'=';
      UnicodeValueToStringS (
        Value,
        sizeof (CHAR16) * (1 + 1),
        PREFIX_ZERO | RADIX_HEX,
        MyNameValue0,
        sizeof (MyNameValue0) * 2
        );
      Value += StrnLenS (Value, sizeof (CHAR16) * (1 + 1));
      *Value = BackupChar;
    }
    return EFI_SUCCESS;
  }

  Status = HiiConfigRouting->BlockToConfig (
                               HiiConfigRouting,
                               ConfigRequest,
                               (UINT8 *)&mPrivateData->Configuration,
                               sizeof(CR_CONFIGURATION),
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

  DEBUG ((EFI_D_INFO, "CRExtract: End\n"));

  return Status;
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Request                A null-terminated Unicode string in
                                 <ConfigRequest> format.
  @param  Progress               On return, points to a character in the Request
                                 string. Points to the string's null terminator if
                                 request was successful. Points to the most recent
                                 '&' before the first failing name/value pair (or
                                 the beginning of the string if the failure is in
                                 the first name/value pair) if the request was not
                                 successful.
  @param  Results                A null-terminated Unicode string in
                                 <ConfigAltResp> format which has all values filled
                                 in for the names in the Request string. String to
                                 be allocated by the called function.

  @retval EFI_SUCCESS            The Results is filled with the requested values.
  @retval EFI_OUT_OF_RESOURCES   Not enough memory to store the results.
  @retval EFI_INVALID_PARAMETER  Request is illegal syntax, or unknown name.
  @retval EFI_NOT_FOUND          Routing data doesn't match any storage in this
                                 driver.

**/
EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  )
{
  EFI_STATUS                                 Status;
  EFI_STRING                                 ConfigRequest;
  EFI_HII_CONFIG_ROUTING_PROTOCOL            *HiiConfigRouting;
  EFI_SETUP_UTILITY_BROWSER_PROTOCOL         *SetupUtility;
  UINTN                                      BufferSize;
  UINTN                                      Size;
  UINT8                                      *Configuration;
  BOOLEAN                                    AllocatedRequest;
  CHAR16                                     *StrPointer;

  if (This == NULL || Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Initialize the local variables.
  //
  ConfigRequest     = NULL;
  Size              = 0;
  *Progress         = Request;
  AllocatedRequest  = FALSE;
  HiiConfigRouting  = mPrivateData->HiiConfigRouting;

  if (HiiIsConfigHdrMatch (Request, &gSystemConfigurationGuid, L"SystemConfig")) {
    BufferSize = sizeof (KERNEL_CONFIGURATION);
    Status = gBS->LocateProtocol (&gEfiSetupUtilityBrowserProtocolGuid, NULL, (VOID **) &SetupUtility);
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
    Configuration = SetupUtility->SCBuffer;
  } else {
    Status = CrExtractConfig (This, Request, Progress, Results);
    return Status;
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
      ConfigRequest    = AllocateZeroPool (Size);
      ASSERT (ConfigRequest != NULL);
      if (ConfigRequest == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      AllocatedRequest = TRUE;
      UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", Request, (UINT64)BufferSize);
    }
  }

  if (StrStr (ConfigRequest, L"OFFSET") == NULL) {
    //
    // If requesting Name/Value storage, return not found.
    //
    return EFI_NOT_FOUND;
  }

  Status = HiiConfigRouting->BlockToConfig (
                               HiiConfigRouting,
                               ConfigRequest,
                               (UINT8 *)Configuration,
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
}

/**
  This function processes the results of changes in configuration.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Configuration          A null-terminated Unicode string in <ConfigResp>
                                 format.
  @param  Progress               A pointer to a string filled in with the offset of
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
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  )
{
  EFI_STATUS                                    Status;
  EFI_HII_CONFIG_ROUTING_PROTOCOL               *HiiConfigRouting;
  UINTN                                         BufferSize;

  DEBUG ((EFI_D_INFO, "CRRoutet: Start\n"));

  if (Configuration == NULL || Progress == NULL || This == NULL) {
   return EFI_INVALID_PARAMETER;
  }

  *Progress = Configuration;

  if (!HiiIsConfigHdrMatch(Configuration, &gH2OCrConfigurationGuid, mVariableName)) {
    return EFI_NOT_FOUND;
  }

  HiiConfigRouting = mPrivateData->HiiConfigRouting;
  BufferSize = sizeof (CR_CONFIGURATION);
  Status = HiiConfigRouting->ConfigToBlock (
                               HiiConfigRouting,
                               Configuration,
                               (UINT8 *) &mPrivateData->Configuration,
                               &BufferSize,
                               Progress
                               );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Store broswer data Buffer Storage to EFI variable
  //
  Status = SetVariableToSensitiveVariable (
             mVariableName,
             &gH2OCrConfigurationGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
             sizeof (CR_CONFIGURATION),
             &mPrivateData->Configuration
             );
  DEBUG ((EFI_D_INFO, "CRRoutet: End\n"));

  CrSubmit ();

  return Status;
}

/**
  This function processes the results of changes in configuration.

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  QuestionId             A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.

**/
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
  return CrDriverCallback (This, Action, QuestionId, Type, Value, ActionRequest);
}

/**
  Unloads the application and its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
  @retval EFI_INVALID_PARAMETER The image hasn't been unloaded.
**/
EFI_STATUS
EFIAPI
SetupUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  if (mPrivateData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DestroyDevices ();

  if (mPrivateData->DriverHandle != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           mPrivateData->DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mHiiVendorDevicePath0,
           &gEfiHiiConfigAccessProtocolGuid,
           &mPrivateData->ConfigAccess,
           NULL
           );
    mPrivateData->DriverHandle = NULL;
  }

  if (mPrivateData->HiiHandle != NULL) {
    HiiRemovePackages (mPrivateData->HiiHandle);
  }

  FreePool (mPrivateData);
  mPrivateData = NULL;

  return EFI_SUCCESS;
}

/**
  Entering SetupUtility notify function.

  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.

  @retval EFI_SUCESS     This function always complete successfully.

**/
VOID
EFIAPI
SetupUtilityNotify (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  CrUpdateMenu (mPrivateData->HiiHandle);
}

VOID
CrCreatePolicyVariable (
  VOID
  )
{
  EFI_STATUS           Status;
  CR_POLICY_VARIABLE   *CrPolicyVar;
  CR_POLICY_PCD        *CrPolicyPcd;
  CR_CONFIGURATION     *CrConfig;
  UINTN                Index;
  UINT8                *PcdPtr8;
  UINTN                VarSize;

  CrPolicyVar      = NULL;
  CrPolicyPcd      = NULL;
  CrConfig         = NULL;
  VarSize          = 0;

  //
  // Get CR setting info from CrPolicy & CrConfig Variable
  //
  Status = CommonGetVariableDataAndSize ( 
             mVariableName, 
             &gH2OCrConfigurationGuid, 
             &VarSize, 
             (VOID **)&CrConfig
             );
  if (EFI_ERROR (Status)) {
    goto exit;
  }

  if (CrConfig->CREnable == FALSE) {
    goto exit;
  }

  Status = CommonGetVariableDataAndSize ( 
             CR_POLICY_NAME, 
             &gH2OCrConfigurationGuid, 
             &VarSize, 
             (VOID **)&CrPolicyVar
             );
  if (EFI_ERROR (Status)) {
    CrPolicyVar = AllocateZeroPool (sizeof (CR_POLICY_VARIABLE));
    if (CrPolicyVar == NULL) {
      goto exit;
    }
  }

  //
  // Update CrPolicy feature flag from PCD
  //
  CrPolicyPcd = (CR_POLICY_PCD*)PcdGetPtr (PcdH2OCrPolicy);
  PcdPtr8     = (UINT8*)CrPolicyPcd;

  for (Index = 0; PcdPtr8[Index] != 0xFF; Index++) {
    if (PcdPtr8[Index] == 1) {
      CrPolicyVar->Feature.Data32 |= (UINT32)(1 << Index);
    } else {
      CrPolicyVar->Feature.Data32 &= (UINT32)(~(1 << Index));
    }
  }
  //
  // Update CrPolicy setting from PCD
  //
  CrPolicyVar->CRWriteCharInterval = CrPolicyPcd->CRWriteCharInterval;
  CrPolicyVar->CRFifoLength        = CrPolicyPcd->CRFifoLength;

  //
  // Update CrPolicy setting from SCU(CrConfig)
  //
  CrPolicyVar->Feature.Bit.CRAutoRefresh      = CrConfig->AutoRefresh;
  CrPolicyVar->Feature.Bit.CRFailSaveBaudrate = CrConfig->FailSaveBaudRate;
  CrPolicyVar->Feature.Bit.CRAfterPost        = CrConfig->CRAfterPost;
  CrPolicyVar->Feature.Bit.CRTerminalResize   = CrConfig->TerminalResize;
  CrPolicyVar->CRInfoWaitTime                 = CrConfig->CRInfoWaitTime;

  CrPolicyVar->GlobalBaudRate                 = CrConfig->GlobalBaudRate;
  CrPolicyVar->GlobalParity                   = CrConfig->GlobalParity;
  CrPolicyVar->GlobalDataBits                 = CrConfig->GlobalDataBits;
  CrPolicyVar->GlobalStopBits                 = CrConfig->GlobalStopBits;
  CrPolicyVar->GlobalFlowControl              = CrConfig->GlobalFlowControl;
  CrPolicyVar->GlobalTerminalType             = CrConfig->GlobalTerminalType;

  switch (CrConfig->CRTextModeResolution) {

  case CR_TEXT_MODE_AUTO:
    CrPolicyVar->Feature.Bit.CRForce80x25 = FALSE;
    break;

  case CR_TEXT_MODE_80X25:
    CrPolicyVar->Feature.Bit.CRForce80x25 = TRUE;
    CrPolicyVar->Feature.Bit.CRTerminalRows = CR_TERMINAL_ROW_25;
    break;

  case CR_TEXT_MODE_80X24DF:
    CrPolicyVar->Feature.Bit.CRForce80x25 = TRUE;
    CrPolicyVar->Feature.Bit.CRTerminalRows = (CR_TERMINAL_ROW_24 == 0)? 0 : 1;
    CrPolicyVar->Feature.Bit.CR24RowsPolicy = (CR_24ROWS_POLICY_DEL_FIRST == 0)? 0 : 1;
    break;

  case CR_TEXT_MODE_80X24DL:
    CrPolicyVar->Feature.Bit.CRForce80x25 = TRUE;
    CrPolicyVar->Feature.Bit.CRTerminalRows = (CR_TERMINAL_ROW_24 == 0)? 0 : 1;
    CrPolicyVar->Feature.Bit.CR24RowsPolicy = (CR_24ROWS_POLICY_DEL_LAST == 0)? 0 : 1;
    break;
  case CR_TEXT_MODE_128X40:
    CrPolicyVar->Feature.Bit.CRForce80x25  = FALSE;
    CrPolicyVar->Feature.Bit.CRForce128x40 = TRUE;
    break;

  default:
    break;
  }

  //
  // Due to HyperTerm don't support graphic CharSet,
  // if CRForce80x25 as TRUE, always use ASCII CharSet
  //
  if (CrPolicyVar->Feature.Bit.CRForce80x25) {
    CrPolicyVar->Feature.Bit.CRTerminalCharSet = (CR_TERMINAL_CHARSET_ASCII == 0)? 0 : 1;
  } else {
    CrPolicyVar->Feature.Bit.CRTerminalCharSet = (CR_TERMINAL_CHARSET_GRAPHIC == 0)? 0 : 1;
  }

  if ((CrPolicyVar->Feature.Bit.CRAsyncTerm == CR_ASYNC_TERMINAL_DISABLE) ||
      (FeaturePcdGet(PcdH2OCrSrvManagerSupported))) {
    CrPolicyVar->Feature.Bit.CRAutoRefresh = (CR_AUTO_REFRESH_DISABLE == 0)? 0 : 1;
  }

  //
  //CrPolicy : if AutoRefresh support, the ManualRefresh need always support
  //
  if (CrPolicyVar->Feature.Bit.CRAutoRefresh == TRUE) {
    CrPolicyVar->Feature.Bit.CRManualRefresh = TRUE;
  }

  CommonSetVariable (
    CR_POLICY_NAME,
    &gH2OCrConfigurationGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    sizeof (CR_POLICY_VARIABLE),
    CrPolicyVar
    );

exit:
  if (CrPolicyVar != NULL) {
    FreePool(CrPolicyVar);
  }
  if (CrConfig != NULL) {
    FreePool(CrConfig);
  }
}

/**
  Initialization for the Setup related functions.

  @param ImageHandle     Image handle this driver.
  @param SystemTable     Pointer to SystemTable.

  @retval EFI_SUCESS     This function always complete successfully.

**/
EFI_STATUS
EFIAPI
CrConfigUtilEntry (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                            Status;
  EFI_HII_HANDLE                        HiiHandle;
  EFI_HII_STRING_PROTOCOL               *HiiString;
  EFI_HII_DATABASE_PROTOCOL             *HiiDatabase;
  EFI_HII_CONFIG_ROUTING_PROTOCOL       *HiiConfigRouting;
  CHAR16                                *NewString;
  CR_CONFIGURATION                      *CrConfig;
  VOID                                  *Registration;
  EFI_EVENT                             Event;
  UINTN                                 BufferSize;
  BOOLEAN                               ActionFlag;
  BOOLEAN                               LoadDefault;
  EFI_STRING                            ConfigRequestHdr;
  EFI_HANDLE                            CpHandle;

  //
  // Initialize the local variables.
  //
  ConfigRequestHdr = NULL;
  NewString        = NULL;
  LoadDefault      = CheckIfNeedLoadDefaultCrConfigVariable ();

  //
  // Initialize configuration data
  //
  CrConfigVarInit (LoadDefault);

  //
  // Init Cr device Variables
  //
  PcdDevicesToVariables (LoadDefault);

  //
  // Init CrPolicy Setting
  //
  CrCreatePolicyVariable ();

  //
  // Initialize driver private data
  //
  mPrivateData = AllocateZeroPool (sizeof (CR_CONFIG_MANAGER_PRIVATE_DATA));
  if (mPrivateData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mPrivateData->Signature = CR_CONFIG_MAMAGER_PRIVATE_SIGNATURE;

  mPrivateData->ConfigAccess.ExtractConfig = ExtractConfig;
  mPrivateData->ConfigAccess.RouteConfig = RouteConfig;
  mPrivateData->ConfigAccess.Callback = DriverCallback;

  //
  // Cr Device List
  //
  InitializeListHead (&mPrivateData->CrDevListHead);

  //
  // Locate Hii Database protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &HiiDatabase);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mPrivateData->HiiDatabase = HiiDatabase;

  //
  // Locate HiiString protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mPrivateData->HiiString = HiiString;

  //
  // Locate ConfigRouting protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID **) &HiiConfigRouting);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mPrivateData->HiiConfigRouting = HiiConfigRouting;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mPrivateData->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath0,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mPrivateData->ConfigAccess,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Publish our HII data
  //
  HiiHandle = HiiAddPackages (
                &gH2OCrFormsetGuid,
                mPrivateData->DriverHandle,
                CrConfigUtilStrings,
                CrConfigUtilVfrBin,
                NULL
                );
  if (HiiHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  mPrivateData->HiiHandle = HiiHandle;

  CrConfig = &mPrivateData->Configuration;
  ZeroMem (CrConfig, sizeof (CR_CONFIGURATION));

  //
  // Try to read NV config EFI variable first
  //
  ConfigRequestHdr = HiiConstructConfigHdr (&gH2OCrConfigurationGuid, mVariableName, mPrivateData->DriverHandle);
  ASSERT (ConfigRequestHdr != NULL);
  if (ConfigRequestHdr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  DEBUG ((DEBUG_INFO, "ConfigRequestHdr: %s\n", ConfigRequestHdr));

  BufferSize = sizeof (CR_CONFIGURATION);
  Status = gRT->GetVariable (mVariableName, &gH2OCrConfigurationGuid, NULL, &BufferSize, CrConfig);
  if (EFI_ERROR (Status) || (Status == EFI_SUCCESS && !IsVariableInVariableStoreRegion (mVariableName, &gH2OCrConfigurationGuid))) {
    BufferSize = sizeof (CR_CONFIGURATION);
    Status = InitVarDefault (mVariableName, &gH2OCrConfigurationGuid, &BufferSize, CrConfig);
    if (EFI_ERROR (Status)) {
      SetupUnload (ImageHandle);
      return Status;
    }
  } else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    ActionFlag = HiiValidateSettings (ConfigRequestHdr);
    if (!ActionFlag) {
      SetupUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  }
  FreePool (ConfigRequestHdr);

  //
  // When execute Setup Utility application, install HII data
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK - 1,
                  SetupUtilityNotify,
                  NULL,
                  &Event
                  );
  if (!EFI_ERROR (Status)) {
    Status = gBS->RegisterProtocolNotify (
                    &gEfiSetupUtilityApplicationProtocolGuid,
                    Event,
                    &Registration
                    );
  }

  H2OCpRegisterHandler (&gH2OBdsCpInitGuid, BdsEntryInitEvent, H2O_CP_MEDIUM, &CpHandle);

  return Status;
}
