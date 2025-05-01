/** @file

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
#include "CrConfig.h"

EFI_STRING_ID     SpaceStringId = 0;
EFI_STRING_ID     WarningStringId = 0;

EFI_STRING_ID     mCrDevTitleTokenIdTable[] = {
  STRING_TOKEN(STR_CR_DEVICE_0_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_1_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_2_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_3_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_4_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_5_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_6_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_7_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_8_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_9_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_10_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_11_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_12_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_13_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_14_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_15_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_16_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_17_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_18_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_19_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_20_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_21_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_22_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_23_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_24_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_25_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_26_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_27_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_28_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_29_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_30_TITLE),
  STRING_TOKEN(STR_CR_DEVICE_31_TITLE)
};

CHAR16 *mTermTypeValueToStringTable[] = {
  L"VT-100",
  L"VT-100+",
  L"VT-UTF8",
  L"PC-ANSI",
  L"LOG-TERM",
  L"TTY-TERM",
  L"LINUX-TERM",
  L"XTERM-R6",
  L"VT400",
  L"SCO",
  };

extern CR_CONFIG_MANAGER_PRIVATE_DATA         *mPrivateData;
extern CHAR16                                 *mVariableName;

//[-start-220208-IB04880004-add]//
EFI_STATUS
UpdateStatusStr (
  IN CR_DEV_NODE                              *DevNode
  );
//[-end-220208-IB04880004-add]//

EFI_STATUS
GetDevNodeByQuestionId (
  IN EFI_QUESTION_ID                          QuestionId,
  IN OUT CR_DEV_NODE                          **DevNode
  )
{
  LIST_ENTRY                                  *Link;
  CR_DEV_NODE                                 *CrDevNode;

  if (QuestionId == 0 || DevNode == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (IsListEmpty (&mPrivateData->CrDevListHead)) {
    return EFI_NOT_FOUND;
  }

  Link = GetFirstNode (&mPrivateData->CrDevListHead);
  while (!IsNull (&mPrivateData->CrDevListHead, Link)) {

    CrDevNode = CR_DEV_NODE_FROM_LINK (Link);
    Link = GetNextNode (&mPrivateData->CrDevListHead, Link);

    if (CrDevNode->QuestionId == QuestionId) {
      *DevNode = CrDevNode;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

CHAR16*
GetStatusStr (
  IN CR_DEVICE_SETTING                        *Device
  )
{
  CR_CONFIGURATION                            CrConfig;
  BOOLEAN                                     IsGet;
  UINT8                                       TermType;
  UINT8                                       BaudRate;
  UINT8                                       Parity;
  UINT8                                       DataBit;
  UINT8                                       StopBit;
  UINT8                                       PortStatus;
  CHAR16                                      *Str;
  CHAR16                                      *PortStatusTable [2] = {L"Disable", L"Enable"};
  CHAR16                                      *ParityTable [5] = {L"N", L"E", L"O", L"M", L"S"};
  UINT32                                      CRBaudRateTable [] = {
                                                1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
                                                };

  if (Device == NULL) {
    return CatSPrint (NULL, L" ");
  }

  IsGet = HiiGetBrowserData (&gH2OCrConfigurationGuid, mVariableName, sizeof (CR_CONFIGURATION), (UINT8 *)&CrConfig);
  if (!IsGet) {
    CopyMem (&CrConfig, &mPrivateData->Configuration, sizeof (CR_CONFIGURATION));
  }

  PortStatus = Device->Attribute.PortEnable;

  if (Device->Attribute.UseGlobalSetting) {
    TermType   = CrConfig.GlobalTerminalType;
    BaudRate   = CrConfig.GlobalBaudRate;
    DataBit    = CrConfig.GlobalDataBits;
    StopBit    = (CrConfig.GlobalStopBits == 1)? 1 : 2;
    Parity     = CrConfig.GlobalParity;
  }
  else {
    TermType   = Device->Attribute.TerminalType;
    BaudRate   = Device->Attribute.BaudRate;
    DataBit    = Device->Attribute.DataBits;
    StopBit    = (Device->Attribute.StopBits == 1)? 1 : 2;
    Parity     = Device->Attribute.Parity;
  }

  Str = CatSPrint (
          NULL,
          L" %s %s,%d,%s%d%d",
          PortStatusTable[PortStatus],
          mTermTypeValueToStringTable[TermType],
          CRBaudRateTable[BaudRate],
          ParityTable[Parity - 1],
          DataBit,
          StopBit
          );

  return Str;
};

EFI_STATUS
SetDevVariables (
  VOID
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

    UpdateOrDeleteCrDevVarWithDevInfo (CrDevNode->Index, &CrDevNode->Dev);
  }

  UpdateDeviceSettingToCrConfig ();

  return EFI_SUCCESS;
}

EFI_STATUS
DestroyDevices (
  VOID
  )
{
  LIST_ENTRY                                  *Link;
  CR_DEV_NODE                                 *CrDevNode;

  while (!IsListEmpty (&mPrivateData->CrDevListHead)) {

    Link = GetFirstNode (&mPrivateData->CrDevListHead);
    CrDevNode = CR_DEV_NODE_FROM_LINK (Link);

    RemoveEntryList (&CrDevNode->Link);
    FreePool (CrDevNode);
  }

  InitializeListHead (&mPrivateData->CrDevListHead);

  return EFI_SUCCESS;
}

VOID
UpdateDeviceSubtitleString (
  IN UINTN          DevIndex,
  IN CHAR16         *DevName
  )
{  
  UINTN Index;

  for (Index = 0; Index < sizeof (mCrDevTitleTokenIdTable)/sizeof (EFI_STRING_ID); Index++)  {
    if (DevIndex == Index) {
      HiiSetString (mPrivateData->HiiHandle, mCrDevTitleTokenIdTable[Index], DevName, NULL);
    }
  }
}

STATIC
EFI_STATUS
CreateDevices (
  VOID
  )
{
  UINT8                                       Index;
  CR_DEVICE_SETTING                           *CrDevSetting;
  CR_DEV_NODE                                 *CrDevNode;
  CHAR16                                      *Str;
  EFI_STATUS                                  Status;

  DestroyDevices ();
  CrDevSetting = NULL;
  //
  // Create Cr Device List Form Variables
  //
  for (Index = 0;  Index < MAX_CR_DEVICE_VARIABLE; Index ++) {
    Status = GetCrDevInfoFromVariable (Index, &CrDevSetting);
    if (EFI_ERROR(Status) || CrDevSetting == NULL) {
      continue;
    }

    CrDevNode = AllocateZeroPool (sizeof (CR_DEV_NODE) + StrSize (CrDevSetting->DevName));
    if (CrDevNode == NULL) {
      ASSERT (FALSE);
      break;
    }

    //
    // Update Cr Device Setting
    //
    CopyMem (&CrDevNode->Dev, CrDevSetting, sizeof (CR_DEVICE_SETTING) + StrSize (CrDevSetting->DevName));

    //
    // Update Cr Device Node
    //
    CrDevNode->Index      = Index;
//[-start-190612-IB07401110-modify]//
    CrDevNode->FormId     = CR_DEVICE_FORM_ID + Index;
//[-end-190612-IB07401110-modify]//
    CrDevNode->QuestionId = (EFI_QUESTION_ID)(CR_DEVICE_QUESTION_ID_BASE + Index);

    Str = CrDevNode->Dev.DevName;
    CrDevNode->PromptStrId = HiiSetString (mPrivateData->HiiHandle, 0, Str, NULL);

    UpdateDeviceSubtitleString (CrDevNode->Index, Str);

    Str = GetStatusStr (&CrDevNode->Dev);
    CrDevNode->StatusStrId = HiiSetString (mPrivateData->HiiHandle, 0, Str, NULL);
    FreePool (Str);

    InsertTailList (&mPrivateData->CrDevListHead, &CrDevNode->Link);
    FreePool (CrDevSetting);  
    //
    // To make sure CrDevSetting is NULL.
    //
    CrDevSetting = NULL;
  }

  if (CrDevSetting != NULL) {
    FreePool (CrDevSetting);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
UpdateDevices (
  IN BOOLEAN                                  LoadDefault
  )
{
  UINT8                                       Index;
  CR_DEVICE_SETTING                           *CrDevSetting;
  CR_DEV_NODE                                 *CrDevNode;
  LIST_ENTRY                                  *Link;
  EFI_STATUS                                  Status;

  CrDevSetting = NULL;
  //
  // Init Cr Device Variables
  //
  PcdDevicesToVariables (LoadDefault);

  if (IsListEmpty (&mPrivateData->CrDevListHead)) {
    //
    // Create New Cr Device List
    //
    return CreateDevices ();
  }

  //
  // Update Cr Device List Form Variables
  //
  for (Index = 0; Index < MAX_CR_DEVICE_VARIABLE; Index ++) {
    Status = GetCrDevInfoFromVariable (Index, &CrDevSetting);
    if (EFI_ERROR(Status) || CrDevSetting == NULL) {
      continue;
    }

    Link = GetFirstNode (&mPrivateData->CrDevListHead);
    while (!IsNull (&mPrivateData->CrDevListHead, Link)) {

      CrDevNode = CR_DEV_NODE_FROM_LINK (Link);
      Link = GetNextNode (&mPrivateData->CrDevListHead, Link);

      if (CrDevNode->Index == Index) {
        //
        // If finding index, update cr device setting
        //
        CopyMem (&CrDevNode->Dev, CrDevSetting, sizeof (CR_DEVICE_SETTING) + StrSize (CrDevSetting->DevName));
//[-start-220208-IB04880004-add]//
        UpdateStatusStr (CrDevNode);
//[-end-220208-IB04880004-add]//
      }
    }
    FreePool (CrDevSetting);    
    //
    // To make sure CrDevSetting is NULL.
    //
    CrDevSetting = NULL;
  }
  if (CrDevSetting != NULL) {
    FreePool (CrDevSetting);
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateDevMenu (
  IN EFI_HII_HANDLE                           HiiHandle,
  IN VOID                                     *StartOpCodeHandle
  )
{
  EFI_STATUS                                  Status;
  LIST_ENTRY                                  *Link;
  CR_DEV_NODE                                 *CrDevNode;
  CR_DEVICE_SETTING                           *Device;
  UINT32                                      Count;
  CHAR16                                      *Str;
//[-start-190616-IB07401112-add]//
  UINTN                                       BufferSize;
//[-end-190616-IB07401112-add]//

  if (HiiHandle == 0 || StartOpCodeHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

   Status = UpdateDevices (FALSE);
  if (EFI_ERROR (Status)) {
    return Status;
  }

//[-start-190616-IB07401112-add]//
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
//[-end-190616-IB07401112-add]//

  if (SpaceStringId == 0) {
    Str        = CatSPrint (NULL, L" ");
    SpaceStringId = HiiSetString (HiiHandle, 0, Str, NULL);
    FreePool (Str);
  }

  Count = 0;
  Link = GetFirstNode (&mPrivateData->CrDevListHead);
  while (!IsNull (&mPrivateData->CrDevListHead, Link)) {

    CrDevNode = CR_DEV_NODE_FROM_LINK (Link);
    Link = GetNextNode (&mPrivateData->CrDevListHead, Link);

    Device = &CrDevNode->Dev;

    if (!Device->Exist) {
      continue;
    }

    //
    // Add one Serial device item.
    //
    HiiCreateGotoOpCode (
      StartOpCodeHandle,
      CrDevNode->FormId,
      CrDevNode->PromptStrId,
      SpaceStringId,
      EFI_IFR_FLAG_CALLBACK,
      CrDevNode->QuestionId
      );

    HiiCreateSubTitleOpCode (
      StartOpCodeHandle,
      CrDevNode->StatusStrId,
      SpaceStringId,
      0,
      0
      );
    
    if (CrDevNode->Index < COMMON_CR_DEVICE_SUPPORT_NUM) {
      mPrivateData->Configuration.CrPortConfig[CrDevNode->Index].PortEnable       = Device->Attribute.PortEnable;
      mPrivateData->Configuration.CrPortConfig[CrDevNode->Index].UseGlobalSetting = Device->Attribute.UseGlobalSetting;
      mPrivateData->Configuration.CrPortConfig[CrDevNode->Index].TerminalType     = Device->Attribute.TerminalType;
      mPrivateData->Configuration.CrPortConfig[CrDevNode->Index].BaudRate         = Device->Attribute.BaudRate;
      mPrivateData->Configuration.CrPortConfig[CrDevNode->Index].Parity           = Device->Attribute.Parity;
      mPrivateData->Configuration.CrPortConfig[CrDevNode->Index].DataBits         = Device->Attribute.DataBits;
      mPrivateData->Configuration.CrPortConfig[CrDevNode->Index].StopBits         = Device->Attribute.StopBits;
      mPrivateData->Configuration.CrPortConfig[CrDevNode->Index].FlowControl      = Device->Attribute.FlowControl;
    } else {
      mPrivateData->Configuration.ExCrPortConfig[CrDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].PortEnable       = Device->Attribute.PortEnable;
      mPrivateData->Configuration.ExCrPortConfig[CrDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].UseGlobalSetting = Device->Attribute.UseGlobalSetting;
      mPrivateData->Configuration.ExCrPortConfig[CrDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].TerminalType     = Device->Attribute.TerminalType;
      mPrivateData->Configuration.ExCrPortConfig[CrDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].BaudRate         = Device->Attribute.BaudRate;
      mPrivateData->Configuration.ExCrPortConfig[CrDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].Parity           = Device->Attribute.Parity;
      mPrivateData->Configuration.ExCrPortConfig[CrDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].DataBits         = Device->Attribute.DataBits;
      mPrivateData->Configuration.ExCrPortConfig[CrDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].StopBits         = Device->Attribute.StopBits;
      mPrivateData->Configuration.ExCrPortConfig[CrDevNode->Index - COMMON_CR_DEVICE_SUPPORT_NUM].FlowControl      = Device->Attribute.FlowControl;
    }

    Count ++;
  }

  if (Count == 0) {
    //
    // Show the message of no CR device support.
    //
    HiiCreateSubTitleOpCode (
      StartOpCodeHandle,
      SpaceStringId,
      SpaceStringId,
      0,
      0
      );

    if (WarningStringId == 0) {
      Str = CatSPrint (NULL, L"Warning!! No serial device to support Console Redirection.");
      WarningStringId = HiiSetString (HiiHandle, 0, Str, NULL);
      FreePool (Str);
    }

    HiiCreateSubTitleOpCode (
      StartOpCodeHandle,
      WarningStringId,
      WarningStringId,
      0,
      0
      );
  }
  
//[-start-190616-IB07401112-add]//
  //
  // Update CrDevice variables to CrConfig variable
  //
  Status = SetVariableToSensitiveVariable (
             mVariableName,
             &gH2OCrConfigurationGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
             sizeof (CR_CONFIGURATION),
             &mPrivateData->Configuration
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
//[-end-190616-IB07401112-add]//

  return EFI_SUCCESS;
}

EFI_STATUS
CrUpdateMenu (
  IN EFI_HII_HANDLE                           HiiHandle
  )
{
  EFI_STATUS                                  Status;
  VOID                                        *StartOpCodeHandle;
  VOID                                        *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL                          *StartLabel;
  EFI_IFR_GUID_LABEL                          *EndLabel;

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();

  StartLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = CR_DEVICE_LABEL_START;

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();

  EndLabel               = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number       = CR_DEVICE_LABEL_END;

  UpdateDevMenu (HiiHandle, StartOpCodeHandle);

  Status = HiiUpdateForm (
             HiiHandle,
             NULL,
             CONSOLE_REDIRECTION_ROOT_FORM_ID,
             StartOpCodeHandle,
             EndOpCodeHandle
             );

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);

  return Status;
}

