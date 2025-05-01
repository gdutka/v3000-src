/** @file
  Implementation of SataDriveInfo.

;******************************************************************************
;* Copyright (c) 2023 - 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Protocol/SimpleTextOut.h>
#include <Protocol/H2OConsoleBootMessage.h>   // InsydePlatformInfoPkg
#include <Protocol/H2ODeviceInfoLookup.h>     // InsydePlatformInfoPkg
#include <Protocol/H2OSataDriveInfo.h>        // InsydePlatformInfoPkg

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HiiLib.h>                   // MdeModulePkg
#include <Library/DeviceInfoLookupDataLib.h>  // InsydePlatformInfoPkg
#include <Library/SataDriveInfoLib.h>         // InsydePlatformInfoPkg
#include <Library/SmbiosCommonLib.h>          // InsydePlatformInfoPkg

#include "PlatformInfoBootMsgDxe.h"

#ifndef BASE_LIST_FOR_EACH
#define BASE_LIST_FOR_EACH(Entry, ListHead)    \
  for(Entry = (ListHead)->ForwardLink; Entry != (ListHead); Entry = Entry->ForwardLink)
#endif

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

STATIC H2O_SATA_DRIVE_INFO_PROTOCOL   *mSataDriveInfo         = NULL;

STATIC UINTN                          mNumberOfControllers    = 0;
STATIC LIST_ENTRY                     *mControllerInfoPool    = NULL;
STATIC UINTN                          mNumberOfDrives         = 0;
STATIC LIST_ENTRY                     *mDriveInfoPool         = NULL;

STATIC
EFI_STRING
GetAtaNominalFormFactorString (
  IN CONST EFI_IDENTIFY_DATA  *IdentifyData,
  IN EFI_HII_HANDLE           HiiHandle,
  IN CONST CHAR8              *Language OPTIONAL
  )
{
  CONST ATA_IDENTIFY_DATA  *AtaIdentifyData;
  UINT16                   NominalFormFactor;
  EFI_STRING_ID            StringId;

  AtaIdentifyData   = (CONST ATA_IDENTIFY_DATA *) IdentifyData;
  NominalFormFactor = (AtaIdentifyData->device_nominal_form_factor &
                                          B_NOMINAL_FORM_FACTOR_FIELD);

  StringId = 0x0000;
  switch (NominalFormFactor) {

  case AtaNominalFormFactor5Inch25:
    StringId = STRING_TOKEN (STR_ATA_NOMINAL_FORM_FACTOR_5_25_INCH);
    break;

  case AtaNominalFormFactor3Inch5:
    StringId = STRING_TOKEN (STR_ATA_NOMINAL_FORM_FACTOR_3_5_INCH);
    break;

  case AtaNominalFormFactor2Inch5:
    StringId = STRING_TOKEN (STR_ATA_NOMINAL_FORM_FACTOR_2_5_INCH);
    break;

  case AtaNominalFormFactor1Inch8:
    StringId = STRING_TOKEN (STR_ATA_NOMINAL_FORM_FACTOR_1_8_INCH);
    break;

  case AtaNominalFormFactorLessThan1Inch8:
    StringId = STRING_TOKEN (STR_ATA_NOMINAL_FORM_FACTOR_LESS_THAN_1_8_INCH);
    break;

  case AtaNominalFormFactorMiniSata:
    StringId = STRING_TOKEN (STR_ATA_NOMINAL_FORM_FACTOR_MINI_SATA);
    break;

  case AtaNominalFormFactorMdot2:
    StringId = STRING_TOKEN (STR_ATA_NOMINAL_FORM_FACTOR_M_2);
    break;

  case AtaNominalFormFactorMicroSSD:
    StringId = STRING_TOKEN (STR_ATA_NOMINAL_FORM_FACTOR_SATA_MICROSSD);
    break;

  case AtaNominalFormFactorCFast:
    StringId = STRING_TOKEN (STR_ATA_NOMINAL_FORM_FACTOR_CFAST);
    break;

  case AtaNominalFormFactorNotReported:
  default:
    return NULL;
  }
  return HiiGetString (HiiHandle, StringId, Language);
}

/**
  Helper function to get S.M.A.R.T support status string.

  @param[in]  IdentifyData  Identify Data.

  @retval   String to describe the S.M.A.R.T support status.

**/
STATIC
VOID
GetSmartStatusStrings (
  IN EFI_IDENTIFY_DATA  *IdentifyData,
  IN EFI_STATUS         SmartReturnStatus,
  IN EFI_HII_HANDLE     HiiHandle,
  IN CONST CHAR8        *Language OPTIONAL,
  OUT INT32             *Attribute,
  OUT EFI_STRING        *String,
  OUT EFI_STRING        *String2
  )
{
  EFI_STRING_ID  StringId;
  EFI_STRING_ID  StringId2;

  if ((IdentifyData == NULL) || (HiiHandle == NULL) || (Attribute == NULL) || (String == NULL) || (String2 == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The IdentifyData/HiiHandle/Attribute/String/String2 is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  StringId = STRING_TOKEN (STR_SMART_NO_INFORMATION);
  StringId2 = STRING_TOKEN (0x0000);
  *Attribute = (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
  *String = NULL;
  *String2 = NULL;

  if (IsSmartSupported (IdentifyData)) {
    if (IsSmartEnabled (IdentifyData)) {
      *Attribute = (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK);
      StringId = STRING_TOKEN (STR_SMART_STATUS);
      StringId2 = STRING_TOKEN (STR_SMART_STATUS_OK);
      if (SmartReturnStatus == EFI_DEVICE_ERROR) {
        *Attribute = (EFI_LIGHTRED | EFI_BACKGROUND_BLACK);
        StringId2 = STRING_TOKEN (STR_SMART_STATUS_BAD);
      } else if (SmartReturnStatus == EFI_UNSUPPORTED) {
        *Attribute = (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
        StringId2 = STRING_TOKEN (STR_SMART_STATUS_NA);
      }
    } else {
      StringId = STRING_TOKEN (STR_SMART_DISABLED);
    }
  }
  if (StringId2 != STRING_TOKEN (0x0000)) {
    *String2 = HiiGetString (HiiHandle, StringId2, Language);
  } else {
    *String2 = (EFI_STRING) AllocateZeroPool (sizeof (L""));
  }
  if ((*String2) == NULL) {
    return;
  }
  *String = HiiGetString (HiiHandle, StringId, Language);
  if ((*String) == NULL) {
    FreePool (*String2);
  }
  return;
}

STATIC
EFI_STATUS
LookupSataPortName (
  IN OUT H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT  *Context,
  IN CONST EFI_DEVICE_PATH_PROTOCOL              *DevicePath,
  OUT CHAR16                                     **DisplayName
  )
{
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL  *DeviceInfoLookup;
  EFI_STATUS                       Status;
  UINTN                            AttributesValue;

  InitDeviceInfoLookupProtocol (&Context->DeviceInfoLookup);

  DeviceInfoLookup = Context->DeviceInfoLookup;
  if (DeviceInfoLookup == NULL) {
    return EFI_UNSUPPORTED;
  }

  Status = DeviceInfoLookup->GetDeviceInfoByDevicePath (
                               DeviceInfoLookup,
                               DevicePath,
                               NULL, // PcdTokenNumber
                               &AttributesValue,
                               NULL, // AcpiPath
                               DisplayName
                               );
  return Status;
}

/**
  This function updates global data.

  Buffers for storing String IDs will only be extended.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.
  @retval others                  Other failure occurs.

**/
STATIC
EFI_STATUS
UpdateGlobalData (
  VOID
  )
{
  EFI_STATUS  Status;
  LIST_ENTRY  *Node;

  Status = mSataDriveInfo->GetControllerInfoPool (
                             mSataDriveInfo,
                             &mControllerInfoPool
                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to GetBusControllerInfo, Status: %r\n", __FUNCTION__, Status));
    if (Status != EFI_NOT_FOUND) {
      return Status;
    }
  }

  mNumberOfControllers = 0;
  BASE_LIST_FOR_EACH (Node, mControllerInfoPool) {
    mNumberOfControllers++;
  }

  Status = mSataDriveInfo->GetDriveInfoPool (
                             mSataDriveInfo,
                             &mDriveInfoPool
                             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to GetDriveInfo, Status: %r\n", __FUNCTION__, Status));
    if (Status != EFI_NOT_FOUND) {
      return Status;
    }
  }

  mNumberOfDrives = 0;
  BASE_LIST_FOR_EACH (Node, mDriveInfoPool) {
    mNumberOfDrives++;
  }

  return EFI_SUCCESS;
}

/**
**/
STATIC
VOID
EFIAPI
SataDriveInfoNotifier (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL  *ConsoleBootMsg;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  ConsoleBootMsg =
    ((H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT *) Context)->ConsoleBootMsg;
  ConsoleBootMsg->AppendOutput (
                    ConsoleBootMsg,
                    SataDriveInfoOutput,
                    Context
                    );

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  Handler function for displaying SATA drive information.

  @param[in]  Console    A pointer to the instance of Simple Text Output protocol.
  @param[in]  MaxColumn  Maximum column count of current mode excluding reserved rows for
                         badging strings and hot key information.
  @param[in]  MaxRow     Maximum row count of current mode excluding reserved rows for
                         badging strings and hot key information.
  @param[in]  Context    A pointer to the instance of platform info boot message context.

  @retval EFI_SUCCESS             Successfully display system information.
  @retval EFI_UNSUPPORTED         This display system information is disabled.
  @retval EFI_INVALID_PARAMETER   There is a problem with the Context pointer.
  @retval EFI_NOT_FOUND           The informaion protocol not reday.

**/
EFI_STATUS
EFIAPI
SataDriveInfoOutput (
  IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *Console,
  IN UINTN                            MaxColumn,
  IN UINTN                            MaxRow,
  IN VOID                             *Context
  )
{
  H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT  *PlatformInfoBootMsgContext;
  H2O_CONSOLE_BOOT_MESSAGE_PROTOCOL       *ConsoleBootMsg;
  EFI_HII_HANDLE                          HiiHandle;
  EFI_STATUS                              Status;
  EFI_EVENT                               Event;
  VOID                                    *Registration;

  LIST_ENTRY        *Node;
  DISK_DRIVE_INFO   *DriveInfo;
  UINTN             DriveCount;
  CHAR16            *ModelNameString;
  CHAR16            *CapacityString;
  EFI_STRING        FormFactorString;
  EFI_STRING        SmartStatusString;
  EFI_STRING        SmartStatusString2;
  CHAR16            *DisplayName;
  CHAR16            PortName[MAX_STRING_SIZE];

  INT32  SataDriveInfoTextAttributes[] = {
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGRAY  | EFI_BACKGROUND_BLACK),
    (EFI_LIGHTGREEN | EFI_BACKGROUND_BLACK)
  };

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  if (!FeaturePcdGet (PcdH2OSataDriveInfoSupported)) {
    return EFI_UNSUPPORTED;
  }

  if (Context == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The Context is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PlatformInfoBootMsgContext = (H2O_PLATFORM_INFO_BOOT_MESSAGE_CONTEXT *) Context;

  ConsoleBootMsg = PlatformInfoBootMsgContext->ConsoleBootMsg;
  if (ConsoleBootMsg == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The ConsoleBootMsg is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  HiiHandle = PlatformInfoBootMsgContext->HiiHandle;
  if (HiiHandle == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The HiiHandle is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gH2OSataDriveInfoProtocolGuid,
                  NULL, // No registration
                  (VOID **) &mSataDriveInfo
                  );
  if (EFI_ERROR (Status)) {
    mSataDriveInfo = NULL;

    PlatformInfoBootMsgContext->RecordCursorPosition = TRUE;
    PlatformInfoBootMsgContext->RecordTailCursorCol  = Console->Mode->CursorColumn;
    PlatformInfoBootMsgContext->RecordTailCursorRow  = Console->Mode->CursorRow;

    if (PlatformInfoBootMsgContext->SataDriveInfoEvent == NULL) {
      Status = gBS->CreateEvent (
                      EVT_NOTIFY_SIGNAL,
                      TPL_CALLBACK,
                      SataDriveInfoNotifier,
                      Context,
                      &Event
                      );
      if (!EFI_ERROR (Status)) {
        PlatformInfoBootMsgContext->SataDriveInfoEvent = Event;
      } else {
        DEBUG ((
          EFI_D_ERROR,
          "%a: Fail to create event\n",
          gEfiCallerBaseName
          ));
      }
    }
    if (PlatformInfoBootMsgContext->SataDriveInfoEvent != NULL &&
        PlatformInfoBootMsgContext->SataDriveInfoRegistration == NULL)
    {
      Status = gBS->RegisterProtocolNotify (
                      &gH2OSataDriveInfoProtocolGuid,
                      PlatformInfoBootMsgContext->SataDriveInfoEvent,
                      &Registration
                      );
      if (!EFI_ERROR (Status)) {
        PlatformInfoBootMsgContext->SataDriveInfoRegistration = Registration;
      } else {
        DEBUG ((
          EFI_D_ERROR,
          "%a: Fail to register %a for event notify\n",
          gEfiCallerBaseName,
          "H2OSataDriveInfoProtocol"
          ));
      }
    }

    return EFI_NOT_FOUND;
  }

  UpdateGlobalData ();

  PlatformInfoBootMsgContext->IsSataDeviceExist = (mNumberOfDrives == 0) ? FALSE : TRUE;
  if (PlatformInfoBootMsgContext->RecordCursorPosition ||
      ((PlatformInfoBootMsgContext->RecordTailCursorRow != 0) &&
       ((!PlatformInfoBootMsgContext->IsNvmeDeviceExist) && (!PlatformInfoBootMsgContext->IsSdEmmcExist)))) {
    Console->SetCursorPosition (Console, PlatformInfoBootMsgContext->RecordTailCursorCol, PlatformInfoBootMsgContext->RecordTailCursorRow);
    PlatformInfoBootMsgContext->RecordCursorPosition = FALSE;
  }

  DriveCount = 0;
  BASE_LIST_FOR_EACH (Node, mDriveInfoPool) {
    DriveInfo = DISK_DRIVE_INFO_FROM_LINK (Node);

    Status = LookupSataPortName (
               PlatformInfoBootMsgContext,
               DriveInfo->DevicePath,
               &DisplayName
               );
    if (!EFI_ERROR (Status) && DisplayName != NULL) {
      UnicodeSPrint (PortName, sizeof (PortName), L"%s", DisplayName);
      FreePool (DisplayName);
    } else {
      UnicodeSPrint (PortName, sizeof (PortName), L"SATA Drive #%d", DriveCount++);
    }

    ModelNameString  = RetrieveModelNameFromIdentifyData ((EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData);
    CapacityString   = RetrieveCapacityFromIdentifyData ((EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData);
    FormFactorString = GetAtaNominalFormFactorString (
                         (EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData,
                         HiiHandle,
                         NULL // Language
                         );
    GetSmartStatusStrings (
      (EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData,
      DriveInfo->SmartReturnStatus,
      HiiHandle,
      NULL, // Language
      &SataDriveInfoTextAttributes[8],
      &SmartStatusString,
      &SmartStatusString2
      );

    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      ARRAY_SIZE (SataDriveInfoTextAttributes),
                      SataDriveInfoTextAttributes,
                      HiiHandle,
                      STRING_TOKEN (STR_SATA_DRIVE_INFORMATION_FORMAT),
                      NULL, // Language
                      PortName,
                      ModelNameString,
                      ((FormFactorString != NULL) ? FormFactorString : L""),
                      CapacityString,
                      ((SmartStatusString != NULL) ? SmartStatusString : L""),
                      ((SmartStatusString2 != NULL) ? SmartStatusString2 : L"")
                      );

    FreePoolIfNonNull (ModelNameString);
    FreePoolIfNonNull (FormFactorString);
    FreePoolIfNonNull (CapacityString);
    FreePoolIfNonNull (SmartStatusString);
    FreePoolIfNonNull (SmartStatusString2);
  }

  Console->SetAttribute (
             Console,
             (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK)
             );

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}
