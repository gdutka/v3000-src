/** @file

  Initial and callback functions for H2O Form Panel Dxe

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

#include "H2OFormPanelDxe.h"

H2O_FORM_PANEL_PRIVATE_DATA               *mH2OFormPanelPrivate = NULL;
CHAR16                                    *mVariableName = H2O_FORM_PANEL_VARSTORE_NAME;
EFI_GUID                                  mVariableGuid = H2O_FORM_PANEL_CONFIGURATION_GUID;
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
    H2O_FORM_PANEL_FORMSET_ID
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

CHAR16  mWeekdayStr[7][4] = {L"SUN", L"MON", L"TUE", L"WED", L"THU", L"FRI", L"SAT"};

STATIC
CHAR16 *
GetWeekdayStr (
  IN EFI_TIME                             *Time
  )
{
  INTN                                    Adjustment;
  INTN                                    Month;
  INTN                                    Year;
  INTN                                    Weekday;

  ASSERT (Time != NULL);

  Adjustment = (14 - Time->Month) / 12;
  Month      = Time->Month + 12 * Adjustment - 2;
  Year       = Time->Year - Adjustment;

  Weekday = (Time->Day + (13 * Month - 1) / 5 + Year + Year / 4 - Year / 100 + Year / 400) % 7;

  return mWeekdayStr[Weekday];
}

STATIC
EFI_STATUS
UpdateDate (
  VOID
  )
{
  EFI_STATUS                               Status;
  EFI_TIME                                 Time;
  CHAR16                                   String[20];
  EFI_HII_HANDLE                           HiiHandle;

  Status = gRT->GetTime (&Time, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = mH2OFormPanelPrivate->HiiHandle;
  if (HiiHandle == NULL) {
    return EFI_NOT_READY;
  }

  UnicodeSPrint (String, sizeof (String), L"%04d/%02d/%02d", Time.Year, Time.Month, Time.Day);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_DATE_VALUE), (EFI_STRING)String, "en-US");

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateTime (
  VOID
  )
{
  EFI_STATUS                               Status;
  EFI_TIME                                 Time;
  CHAR16                                   String[20];
  EFI_HII_HANDLE                           HiiHandle;

  Status = gRT->GetTime (&Time, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = mH2OFormPanelPrivate->HiiHandle;
  if (HiiHandle == NULL) {
    return EFI_NOT_READY;
  }

  UnicodeSPrint (String, sizeof (String), L"%02d:%02d:%02d", Time.Hour, Time.Minute, Time.Second);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_TIME_VALUE), (EFI_STRING)String, "en-US");

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateWeek (
  VOID
  )
{
  EFI_STATUS                               Status;
  EFI_TIME                                 Time;
  CHAR16                                   String[20];
  EFI_HII_HANDLE                           HiiHandle;


  Status = gRT->GetTime (&Time, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = mH2OFormPanelPrivate->HiiHandle;
  if (HiiHandle == NULL) {
    return EFI_NOT_READY;
  }

  UnicodeSPrint (String, sizeof (String), L"%s", GetWeekdayStr (&Time));
  HiiSetString (HiiHandle, STRING_TOKEN (STR_WEEKDAY_PROMPT), (EFI_STRING)String, "en-US");

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateMainboardTemperature (
  VOID
  )
{
  CHAR16                                   String[20];
  EFI_HII_HANDLE                           HiiHandle;

  HiiHandle = mH2OFormPanelPrivate->HiiHandle;
  if (HiiHandle == NULL) {
    return EFI_NOT_READY;
  }

  UnicodeSPrint (String, sizeof (String), L"27\xB0" L"C");
  HiiSetString (HiiHandle, STRING_TOKEN (STR_MAINBOARD_TEMPERATURE_VALUE), (EFI_STRING)String, "en-US");

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateCpuTemperature (
  VOID
  )
{
  CHAR16                                   String[20];
  EFI_HII_HANDLE                           HiiHandle;
  EFI_STATUS                               Status;
  EFI_TIME                                 Time;
  STATIC UINT8                             LastTimeSecond = 0;
  STATIC UINT16                            Temperature = 30;


  HiiHandle = mH2OFormPanelPrivate->HiiHandle;
  if (HiiHandle == NULL) {
    return EFI_NOT_READY;
  }

  Status = gRT->GetTime (&Time, NULL);
  if (EFI_ERROR (Status) || Time.Second == LastTimeSecond) {
    return Status;
  }

  LastTimeSecond = Time.Second;
  if (Temperature >= 50) {
    Temperature = 30;
  } else {
    Temperature ++;
  }

  UnicodeSPrint (String, sizeof (String), L"%d\xB0" L"C", Temperature);
  HiiSetString (HiiHandle, STRING_TOKEN (STR_CPU_TEMPERATURE_VALUE), (EFI_STRING)String, "en-US");

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateProcessorInfo (
  VOID
  )
{
  CHAR16                                   String[20];
  EFI_HII_HANDLE                           HiiHandle;

  HiiHandle = mH2OFormPanelPrivate->HiiHandle;
  if (HiiHandle == NULL) {
    return EFI_NOT_READY;
  }

  UnicodeSPrint (String, sizeof (String), L"NT32 Emulated Processor");
  HiiSetString (HiiHandle, STRING_TOKEN (STR_PROCESSOR_SYSTEM_INFO), (EFI_STRING)String, "en-US");

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateMemoryInfo (
  VOID
  )
{
  CHAR16                                   String[20];
  EFI_HII_HANDLE                           HiiHandle;

  HiiHandle = mH2OFormPanelPrivate->HiiHandle;
  if (HiiHandle == NULL) {
    return EFI_NOT_READY;
  }

  UnicodeSPrint (String, sizeof (String), L"Memory Size: 256 MB");
  HiiSetString (HiiHandle, STRING_TOKEN (STR_MEMORY_SYSTEM_INFO), (EFI_STRING)String, "en-US");

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
UpdateOwnerDrawPanelInfo (
  VOID
  )
{
  UpdateDate ();
  UpdateTime ();
  UpdateWeek ();
  UpdateMainboardTemperature ();
  UpdateCpuTemperature ();
  UpdateProcessorInfo ();
  UpdateMemoryInfo ();

  return EFI_SUCCESS;
}

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

  if ((Value == NULL && (Action != EFI_BROWSER_ACTION_FORM_OPEN) && (Action != EFI_BROWSER_ACTION_FORM_CLOSE)) ||
      (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;

  switch (Action) {

  case EFI_BROWSER_ACTION_FORM_OPEN:
    UpdateOwnerDrawPanelInfo ();
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
    switch (QuestionId) {

    case DATE_REFRESH_QUESTION_ID:
      UpdateDate ();
      break;

    case WEEK_REFRESH_QUESTION_ID:
      UpdateWeek ();
      break;

    case TIME_REFRESH_QUESTION_ID:
      UpdateTime ();
      break;

    case MAIN_BOARD_TEMPERATURE_REFRESH_QUESTION_ID:
      UpdateMainboardTemperature ();
      break;

    case CPU_TEMPERATURE_REFRESH_QUESTION_ID:
      UpdateCpuTemperature ();
      break;

    default:
      break;
    }
    Status = EFI_SUCCESS;
    break;

  default:
    break;
  }

  return Status;
}

STATIC
EFI_STATUS
H2OFormPanelDxeUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  ASSERT (mH2OFormPanelPrivate != NULL);

  if (mH2OFormPanelPrivate->DriverHandle != NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           mH2OFormPanelPrivate->DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mHiiVendorDevicePath0,
           &gEfiHiiConfigAccessProtocolGuid,
           &mH2OFormPanelPrivate->HiiConfigAccess,
           NULL
           );
    mH2OFormPanelPrivate->DriverHandle = NULL;
  }

  if (mH2OFormPanelPrivate->HiiHandle != NULL) {
    HiiRemovePackages (mH2OFormPanelPrivate->HiiHandle);
  }

  FreePool (mH2OFormPanelPrivate);
  mH2OFormPanelPrivate = NULL;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
H2OFormPanelDxeEntry (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                            Status;
  EFI_GUID                              HiiPkgListGuid = H2O_FORM_PANEL_FORMSET_ID;
  H2O_FORM_PANEL_CONFIGURATION          Config;
  UINTN                                 BufferSize;
  BOOLEAN                               ActionFlag;
  EFI_STRING                            ConfigRequestHdr;

  //
  // Initialize driver private data
  //
  mH2OFormPanelPrivate = AllocateZeroPool (sizeof (H2O_FORM_PANEL_PRIVATE_DATA));
  if (mH2OFormPanelPrivate == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  mH2OFormPanelPrivate->Signature                     = H2O_FORM_PANEL_PRIVATE_SIGNATURE;
  mH2OFormPanelPrivate->HiiConfigAccess.ExtractConfig = ExtractConfig;
  mH2OFormPanelPrivate->HiiConfigAccess.RouteConfig   = RouteConfig;
  mH2OFormPanelPrivate->HiiConfigAccess.Callback      = DriverCallback;

  Status = gBS->LocateProtocol (&gH2OFormBrowserProtocolGuid, NULL, (VOID **) &mH2OFormPanelPrivate->H2OFB);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mH2OFormPanelPrivate->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath0,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mH2OFormPanelPrivate->HiiConfigAccess,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  mH2OFormPanelPrivate->HiiHandle = HiiAddPackages (
                                            &HiiPkgListGuid,
                                            mH2OFormPanelPrivate->DriverHandle,
                                            H2OFormPanelDxeStrings,
                                            H2OFormPanelDxeImages,
                                            H2OFormPanelVfrBin,
                                            NULL
                                            );
  if (mH2OFormPanelPrivate->HiiHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&Config, sizeof (H2O_FORM_PANEL_CONFIGURATION));

  //
  // Try to read NV config EFI variable first
  //
  ConfigRequestHdr = HiiConstructConfigHdr (&mVariableGuid, mVariableName, mH2OFormPanelPrivate->DriverHandle);
  ASSERT (ConfigRequestHdr != NULL);
  if (ConfigRequestHdr == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  BufferSize = sizeof (H2O_FORM_PANEL_CONFIGURATION);
  Status = gRT->GetVariable (mVariableName, &mVariableGuid, NULL, &BufferSize, &Config);
  if (EFI_ERROR (Status)) {
    //
    // Store zero data Buffer Storage to EFI variable
    //
    Status = CommonSetVariable (
               mVariableName,
               &mVariableGuid,
               EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
               sizeof (H2O_FORM_PANEL_CONFIGURATION),
               &Config
               );
    if (EFI_ERROR (Status)) {
      H2OFormPanelDxeUnload (ImageHandle);
      return Status;
    }
    //
    // EFI variable for NV config doesn't exit, we should build this variable
    // based on default values stored in IFR
    //
    ActionFlag = HiiSetToDefaults (ConfigRequestHdr, EFI_HII_DEFAULT_CLASS_STANDARD);
    if (!ActionFlag) {
      H2OFormPanelDxeUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  } else {
    //
    // EFI variable does exist and Validate Current Setting
    //
    ActionFlag = HiiValidateSettings (ConfigRequestHdr);
    if (!ActionFlag) {
      H2OFormPanelDxeUnload (ImageHandle);
      return EFI_INVALID_PARAMETER;
    }
  }
  FreePool (ConfigRequestHdr);

  return Status;
}

