/** @file

  Callback Functions for Console Redirection Configuration

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
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
#include "CrConfig.h"

extern CR_CONFIG_MANAGER_PRIVATE_DATA         *mPrivateData;
extern CHAR16                                 *mVariableName;

CR_DEV_NODE                                   *mCurrentDevNode = NULL;

STATIC
EFI_STATUS
ForceSubmit (
  VOID
  )
{
  EFI_STATUS                                  Status;
  EFI_FORM_BROWSER2_PROTOCOL                  *FormBrowser2;
  CHAR16                                      *ConfigRequest;
  UINTN                                       ConfigRequestSize;
  EFI_GUID                                    *NameValueGuid;
  CHAR16                                      *NameValueName;
  STATIC UINT8                                NameValueValue = 0;

  //
  // Change name value data to make form brower will call RouteConfig () when SubmitForm ()
  //
  Status = gBS->LocateProtocol (&gEfiFormBrowser2ProtocolGuid, NULL, (VOID **) &FormBrowser2);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }

  NameValueGuid = &gH2OCrConfigurationGuid;
  NameValueName = HiiGetString (mPrivateData->HiiHandle, STRING_TOKEN(STR_NAME_VALUE_VAR_NAME0), NULL);
  if (NameValueValue < 0xFF) {
    NameValueValue ++;
  } else {
    NameValueValue = 0;
  }

  ConfigRequestSize = 0x100;
  ConfigRequest     = AllocateZeroPool (ConfigRequestSize);
  UnicodeSPrint (ConfigRequest, ConfigRequestSize, L"%s=%02x", NameValueName, NameValueValue);
  Status = FormBrowser2->BrowserCallback (
                           FormBrowser2,
                           &ConfigRequestSize,
                           ConfigRequest,
                           FALSE,
                           NameValueGuid,
                           NameValueName
                           );
  FreePool (NameValueName);
  FreePool (ConfigRequest);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_WARN, "[%a %d] Status = %r \n", __FUNCTION__, __LINE__, Status));
    DEBUG((EFI_D_WARN, "ConfigRequest = %s \n", ConfigRequest));
    DEBUG((EFI_D_WARN, "\n"));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
BOOLEAN
IsGlobalSetting (
  IN EFI_QUESTION_ID                          QuestionId
  )
{
  switch (QuestionId) {

  case KEY_GLOBAL_TERMINAL_TYPE:
  case KEY_GLOBAL_BAUD_RATE:
  case KEY_GLOBAL_DATA_BITS:
  case KEY_GLOBAL_PARITY:
  case KEY_GLOBAL_STOP_BITS:
  case KEY_GLOBAL_FLOW_CONTROL:
    return TRUE;

  default:
    break;
  }

  return FALSE;
}

STATIC
BOOLEAN
IsLocalSetting (
  IN EFI_QUESTION_ID                          QuestionId
  )
{
  if ((QuestionId & CR_DEVICE_QUESTION_ID_MASK) == CR_DEVICE_QUESTION_ID_MASK) {
    return TRUE;
  } else {
    return FALSE;
  }
}

EFI_STATUS
UpdateStatusStr (
  IN CR_DEV_NODE                              *DevNode
  )
{
  CHAR16                                      *Str;

  if (DevNode == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Str = GetStatusStr (&DevNode->Dev);
  HiiSetString (mPrivateData->HiiHandle, DevNode->StatusStrId, Str, NULL);
  FreePool (Str);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateGlobalToLocal (
  IN CR_CONFIGURATION                         *CrConfig
  )
{
  if ((CrConfig == NULL) || (mCurrentDevNode->Index >= MAX_CR_DEVICE_VARIABLE)) {
    return EFI_INVALID_PARAMETER;
  }

  if (mCurrentDevNode->Index < COMMON_CR_DEVICE_SUPPORT_NUM) {
    CrConfig->CrPortConfig[mCurrentDevNode->Index].TerminalType = CrConfig->GlobalTerminalType;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].BaudRate     = CrConfig->GlobalBaudRate;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].Parity       = CrConfig->GlobalParity;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].DataBits     = CrConfig->GlobalDataBits;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].StopBits     = CrConfig->GlobalStopBits;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].FlowControl  = CrConfig->GlobalFlowControl;
  } else {
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].TerminalType = CrConfig->GlobalTerminalType;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].BaudRate     = CrConfig->GlobalBaudRate;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].Parity       = CrConfig->GlobalParity;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].DataBits     = CrConfig->GlobalDataBits;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].StopBits     = CrConfig->GlobalStopBits;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].FlowControl  = CrConfig->GlobalFlowControl;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateDeviceToCrConfig (
  IN CR_DEVICE_SETTING                        *Device,
  IN CR_CONFIGURATION                         *CrConfig
  )
{
  if (Device == NULL || CrConfig == NULL || (mCurrentDevNode->Index >= MAX_CR_DEVICE_VARIABLE)) {
    return EFI_INVALID_PARAMETER;
  }

  if (mCurrentDevNode->Index < COMMON_CR_DEVICE_SUPPORT_NUM) {
    CrConfig->CrPortConfig[mCurrentDevNode->Index].PortEnable       = Device->Attribute.PortEnable;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].UseGlobalSetting = Device->Attribute.UseGlobalSetting;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].TerminalType     = Device->Attribute.TerminalType;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].BaudRate         = Device->Attribute.BaudRate;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].Parity           = Device->Attribute.Parity;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].DataBits         = Device->Attribute.DataBits;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].StopBits         = Device->Attribute.StopBits;
    CrConfig->CrPortConfig[mCurrentDevNode->Index].FlowControl      = Device->Attribute.FlowControl;
  } else {
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].PortEnable       = Device->Attribute.PortEnable;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].UseGlobalSetting = Device->Attribute.UseGlobalSetting;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].TerminalType     = Device->Attribute.TerminalType;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].BaudRate         = Device->Attribute.BaudRate;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].Parity           = Device->Attribute.Parity;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].DataBits         = Device->Attribute.DataBits;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].StopBits         = Device->Attribute.StopBits;
    CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].FlowControl      = Device->Attribute.FlowControl;
  }
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateDeviceFromCrConfig (
  IN CR_DEVICE_SETTING                        *Device,
  IN CR_CONFIGURATION                         *CrConfig
  )
{
  if (Device == NULL || CrConfig == NULL || (mCurrentDevNode->Index >= MAX_CR_DEVICE_VARIABLE)) {
    return EFI_INVALID_PARAMETER;
  }

  if (mCurrentDevNode->Index < COMMON_CR_DEVICE_SUPPORT_NUM) {
    Device->Attribute.PortEnable       = CrConfig->CrPortConfig[mCurrentDevNode->Index].PortEnable;
    Device->Attribute.UseGlobalSetting = CrConfig->CrPortConfig[mCurrentDevNode->Index].UseGlobalSetting;
    Device->Attribute.TerminalType     = CrConfig->CrPortConfig[mCurrentDevNode->Index].TerminalType;
    Device->Attribute.BaudRate         = CrConfig->CrPortConfig[mCurrentDevNode->Index].BaudRate;
    Device->Attribute.Parity           = CrConfig->CrPortConfig[mCurrentDevNode->Index].Parity;
    Device->Attribute.DataBits         = CrConfig->CrPortConfig[mCurrentDevNode->Index].DataBits;
    Device->Attribute.StopBits         = CrConfig->CrPortConfig[mCurrentDevNode->Index].StopBits;
    Device->Attribute.FlowControl      = CrConfig->CrPortConfig[mCurrentDevNode->Index].FlowControl;
  } else {
    Device->Attribute.PortEnable       = CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].PortEnable;
    Device->Attribute.UseGlobalSetting = CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].UseGlobalSetting;
    Device->Attribute.TerminalType     = CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].TerminalType;
    Device->Attribute.BaudRate         = CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].BaudRate;
    Device->Attribute.Parity           = CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].Parity;
    Device->Attribute.DataBits         = CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].DataBits;
    Device->Attribute.StopBits         = CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].StopBits;
    Device->Attribute.FlowControl      = CrConfig->ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].FlowControl;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateAllDevFromConfig (
  IN CR_CONFIGURATION                         *CrConfig
  )
{
  LIST_ENTRY                                  *Link;
  CR_DEV_NODE                                 *CrDevNode;

  if (IsListEmpty (&mPrivateData->CrDevListHead)) {
    return EFI_NOT_FOUND;
  }

  Link = GetFirstNode (&mPrivateData->CrDevListHead);
  while (!IsNull (&mPrivateData->CrDevListHead, Link)) {

    CrDevNode = CR_DEV_NODE_FROM_LINK (Link);
    Link = GetNextNode (&mPrivateData->CrDevListHead, Link);

    if (CrDevNode->Dev.Attribute.UseGlobalSetting) {
      CrDevNode->Dev.Attribute.TerminalType = CrConfig->GlobalTerminalType;
      CrDevNode->Dev.Attribute.BaudRate     = CrConfig->GlobalBaudRate;
      CrDevNode->Dev.Attribute.Parity       = CrConfig->GlobalParity;
      CrDevNode->Dev.Attribute.DataBits     = CrConfig->GlobalDataBits;
      CrDevNode->Dev.Attribute.StopBits     = CrConfig->GlobalStopBits;
      CrDevNode->Dev.Attribute.FlowControl  = CrConfig->GlobalFlowControl;
    }
    UpdateStatusStr (CrDevNode);
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateDevPage (
  VOID
  )
{
  EFI_STATUS                                  Status;
  CR_DEVICE_SETTING                           *Device;
  CR_CONFIGURATION                            CrConfig;
  BOOLEAN                                     IsSuccess;

  if (mCurrentDevNode == NULL) {
    return EFI_UNSUPPORTED;
  }

  Device = &mCurrentDevNode->Dev;

  Status = HiiSetString (mPrivateData->HiiHandle, STRING_TOKEN (STR_STR_CR_DEVICE_TITLE), Device->DevName, NULL);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }

  IsSuccess = HiiGetBrowserData (&gH2OCrConfigurationGuid, mVariableName, sizeof (CR_CONFIGURATION), (UINT8 *)&CrConfig);
  if (!IsSuccess) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  UpdateDeviceToCrConfig (Device, &CrConfig);

  IsSuccess = HiiSetBrowserData (&gH2OCrConfigurationGuid, mVariableName, sizeof (CR_CONFIGURATION), (UINT8 *)&CrConfig, NULL);
  if (!IsSuccess) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  return Status;
}

STATIC
EFI_STATUS
GotoDevCallback (
  IN EFI_QUESTION_ID                          QuestionId
  )
{
  EFI_STATUS                                  Status;
  CR_DEV_NODE                                 *DevNode;

  //
  // Current question is the "created goto question"
  //
  Status = GetDevNodeByQuestionId (QuestionId, &DevNode);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  mCurrentDevNode = DevNode;

  UpdateDevPage ();

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
GlobalSettingCallback (
  VOID
  )
{
  CR_CONFIGURATION                            CrConfig;
  BOOLEAN                                     IsSuccess;

  IsSuccess = HiiGetBrowserData (&gH2OCrConfigurationGuid, mVariableName, sizeof (CR_CONFIGURATION), (UINT8 *)&CrConfig);
  if (!IsSuccess) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  UpdateAllDevFromConfig (&CrConfig);

  IsSuccess = HiiSetBrowserData (&gH2OCrConfigurationGuid, mVariableName, sizeof (CR_CONFIGURATION), (UINT8 *)&CrConfig, NULL);
  if (!IsSuccess) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
LocalSettingCallback (
  VOID
  )
{
  CR_DEVICE_SETTING                           *Device;
  CR_CONFIGURATION                            CrConfig;
  BOOLEAN                                     IsSuccess;
  BOOLEAN                                     UseGlobalSetting;

  if (mCurrentDevNode == NULL) {
    return EFI_UNSUPPORTED;
  }

  IsSuccess = HiiGetBrowserData (&gH2OCrConfigurationGuid, mVariableName, sizeof (CR_CONFIGURATION), (UINT8 *)&CrConfig);
  if (!IsSuccess) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Device = &mCurrentDevNode->Dev;
  
  if (mCurrentDevNode->Index < COMMON_CR_DEVICE_SUPPORT_NUM) {
    UseGlobalSetting = CrConfig.CrPortConfig[mCurrentDevNode->Index].UseGlobalSetting;
  } else {
    UseGlobalSetting = CrConfig.ExCrPortConfig[mCurrentDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].UseGlobalSetting;
  }
  if (UseGlobalSetting) {
    UpdateGlobalToLocal (&CrConfig);
  }

  UpdateDeviceFromCrConfig (Device, &CrConfig);

  UpdateStatusStr (mCurrentDevNode);

  IsSuccess = HiiSetBrowserData (&gH2OCrConfigurationGuid, mVariableName, sizeof (CR_CONFIGURATION), (UINT8 *)&CrConfig, NULL);
  if (!IsSuccess) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
AfterLoadDefault (
  VOID
  )
{
  LIST_ENTRY        *Link;
  CR_DEV_NODE       *CrDevNode;

  
//[-start-220207-IB04880004-modify]//
  //
  // Update CR devices
  //
//-  UpdateDevices (TRUE);
  Link = GetFirstNode (&mPrivateData->CrDevListHead);

  while (!IsNull (&mPrivateData->CrDevListHead, Link)) {
    CrDevNode = CR_DEV_NODE_FROM_LINK (Link);
    
    CrDeviceSettingLoadDefault (CrDevNode->Index, &CrDevNode->Dev);

//-    CrDeviceLoadDefault ( CrDevNode);
    Link = GetNextNode (&mPrivateData->CrDevListHead, Link);
  }
//[-end-220207-IB04880004-modify]//

  //
  // Check use global setting
  //
  GlobalSettingCallback ();

  //
  // Update device page
  //
  UpdateDevPage ();

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
CrSubmit (
  VOID
  )
{
  return SetDevVariables ();
}

EFI_STATUS
EFIAPI
CrDriverCallback (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL     *This,
  IN EFI_BROWSER_ACTION                       Action,
  IN EFI_QUESTION_ID                          QuestionId,
  IN UINT8                                    Type,
  IN EFI_IFR_TYPE_VALUE                       *Value,
  OUT EFI_BROWSER_ACTION_REQUEST              *ActionRequest
  )
{
  EFI_STATUS                                  Status;
//[-start-180823-IB10920058-add]//
  CR_CONFIGURATION                            Configuration = {0};
  UINTN                                       ConfigSize;
//[-end-180823-IB10920058-add]//


  //
  // Ensure call RoutConfig () when submit even if no changing anything
  //
  if (IsListEmpty (&mPrivateData->CrDevListHead)) {
    return EFI_UNSUPPORTED;
  } else {
    ForceSubmit ();
  }

  if (((Value == NULL) && (Action != EFI_BROWSER_ACTION_FORM_OPEN) && (Action != EFI_BROWSER_ACTION_FORM_CLOSE))||
    (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;

  switch (Action) {

  case EFI_BROWSER_ACTION_CHANGING:
    DEBUG ((EFI_D_INFO, "CRCallBack->CHANGING               Qid:0x%x  Typ:0x%x\n", QuestionId, Type));
    Status = GotoDevCallback (QuestionId);
    break;

  case EFI_BROWSER_ACTION_CHANGED:
    DEBUG ((EFI_D_INFO, "CRCallBack->CHANGED                Qid:0x%x  Typ:0x%x\n", QuestionId, Type));
    if (IsGlobalSetting (QuestionId)) {
      Status = GlobalSettingCallback ();
    } else if (IsLocalSetting (QuestionId)) {
      Status = LocalSettingCallback ();
    }
    break;

  case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
    DEBUG ((EFI_D_INFO, "CRCallBack->DEFAULT_STANDARD       Qid:0x%x  Typ:0x%x\n", QuestionId, Type));
//[-start-180823-IB10920058-add]//
    if (QuestionId == KEY_SCAN_F9) {
      //
      // Last question in this formset
      //
      ConfigSize = sizeof (CR_CONFIGURATION);
      Status = LoadVarDefault (
                 mVariableName, 
                 &gH2OCrConfigurationGuid, 
                 &ConfigSize, 
                 &Configuration
                 );
    }
//[-end-180823-IB10920058-add]//      
    if (QuestionId == KEY_SCAN_F9) {
        Status = AfterLoadDefault ();
    } else {
      Status = EFI_UNSUPPORTED;
    }
    break;

  case EFI_BROWSER_ACTION_RETRIEVE:
    DEBUG ((EFI_D_INFO, "CRCallBack->RETRIEVE               Qid:0x%x  Typ:0x%x\n", QuestionId, Type));
    if (QuestionId == KEY_SCAN_F9) {
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_UNSUPPORTED;
    }
    break;

  default:
    DEBUG ((EFI_D_INFO, "CRCallBack->default:Action=0x%x    Qid:0x%x  Typ:0x%x\n", Action , QuestionId, Type));
    Status = EFI_UNSUPPORTED;
    break;
  }

  return Status;
}

