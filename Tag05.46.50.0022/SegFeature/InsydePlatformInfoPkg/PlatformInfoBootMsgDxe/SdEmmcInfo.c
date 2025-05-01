/** @file
  Implementation of SdEmmcInfo.

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
#include <Protocol/H2OSDEmmcInfo.h>           // InsydePlatformInfoPkg

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
#include <Library/SDEmmcInfoLib.h>            // InsydePlatformInfoPkg
#include <Library/SmbiosCommonLib.h>          // InsydePlatformInfoPkg

#include "PlatformInfoBootMsgDxe.h"

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

STATIC H2O_SD_EMMC_INFO_PROTOCOL      *mSdEmmcDriveInfo         = NULL;

STATIC UINTN                          mNumberOfDrives         = 0;
STATIC SD_EMMC_INFO                   *mDriveInfos            = NULL;

STATIC
EFI_STATUS
LookupSdEmmcPortName (
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
  (*DisplayName) = NULL;

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
  EFI_STATUS                Status;
  UINTN                     NumberOfDrives;
  SD_EMMC_INFO              *DriveInfos;

  NumberOfDrives = 0;
  DriveInfos     = NULL;

  Status = mSdEmmcDriveInfo->GetSDEmmcInfo (
                               mSdEmmcDriveInfo,
                               &NumberOfDrives,
                               &DriveInfos
                               );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to GetDriveInfo, Status: %r\n", __FUNCTION__, Status));
    if (Status != EFI_NOT_FOUND) {
      return Status;
    }
  }

  mNumberOfDrives         = NumberOfDrives;
  mDriveInfos             = DriveInfos;

  return EFI_SUCCESS;
}

/**
**/
STATIC
VOID
EFIAPI
SdEmmcInfoNotifier (
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
                    SdEmmcInfoOutput,
                    Context
                    );

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
}

/**
  Handler function for displaying Sd/eMMc information.

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
SdEmmcInfoOutput (
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

  UINTN       Index;
  CHAR16      *ModelNameString;
  CHAR16      *CapacityString;
  CHAR16      *CardTypeString;
  CHAR16      *DisplayName;
  CHAR16      PortName[MAX_STRING_SIZE];

  INT32  SdEmmcInfoTextAttributes[] = {
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

  if (!FeaturePcdGet (PcdH2OSDEmmcInfoSupported)) {
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
                  &gH2OSDEmmcInfoProtocolGuid,
                  NULL, // No registration
                  (VOID **) &mSdEmmcDriveInfo
                  );
  if (EFI_ERROR (Status)) {
    mSdEmmcDriveInfo = NULL;

    PlatformInfoBootMsgContext->RecordCursorPosition = TRUE;
    PlatformInfoBootMsgContext->RecordTailCursorCol  = Console->Mode->CursorColumn;
    PlatformInfoBootMsgContext->RecordTailCursorRow  = Console->Mode->CursorRow;
    if (PlatformInfoBootMsgContext->SdEmmcInfoEvent == NULL) {
      Status = gBS->CreateEvent (
                      EVT_NOTIFY_SIGNAL,
                      TPL_CALLBACK,
                      SdEmmcInfoNotifier,
                      Context,
                      &Event
                      );
      if (!EFI_ERROR (Status)) {
        PlatformInfoBootMsgContext->SdEmmcInfoEvent = Event;
      } else {
        DEBUG ((
          EFI_D_ERROR,
          "%a: Fail to create event\n",
          gEfiCallerBaseName
          ));
      }
    }
    if (PlatformInfoBootMsgContext->SdEmmcInfoEvent != NULL &&
        PlatformInfoBootMsgContext->SdEmmcInfoRegistration == NULL)
    {
      Status = gBS->RegisterProtocolNotify (
                      &gH2OSDEmmcInfoProtocolGuid,
                      PlatformInfoBootMsgContext->SdEmmcInfoEvent,
                      &Registration
                      );
      if (!EFI_ERROR (Status)) {
        PlatformInfoBootMsgContext->SdEmmcInfoRegistration = Registration;
      } else {
        DEBUG ((
          EFI_D_ERROR,
          "%a: Fail to register %a for event notify\n",
          gEfiCallerBaseName,
          "gH2OSDEmmcInfoProtocolGuid"
          ));
      }
    }

    return EFI_NOT_FOUND;
  }

  UpdateGlobalData ();

  PlatformInfoBootMsgContext->IsSdEmmcExist = (mNumberOfDrives == 0) ? FALSE : TRUE;
  if (PlatformInfoBootMsgContext->RecordCursorPosition ||
      ((PlatformInfoBootMsgContext->RecordTailCursorRow != 0) &&
       ((!PlatformInfoBootMsgContext->IsNvmeDeviceExist) && (!PlatformInfoBootMsgContext->IsSataDeviceExist)))) {
    Console->SetCursorPosition (Console, PlatformInfoBootMsgContext->RecordTailCursorCol, PlatformInfoBootMsgContext->RecordTailCursorRow);
    PlatformInfoBootMsgContext->RecordCursorPosition = FALSE;
  }
  for (Index = 0; Index < mNumberOfDrives; Index++) {
    Status = LookupSdEmmcPortName (
               PlatformInfoBootMsgContext,
               mDriveInfos[Index].DevicePath,
               &DisplayName
               );
    if (!EFI_ERROR (Status) && DisplayName != NULL) {
      UnicodeSPrint (PortName, sizeof (PortName), L"%s", DisplayName);
      FreePool (DisplayName);
    } else {
      UnicodeSPrint (PortName, sizeof (PortName), L"%s #%d", L"Port", Index);
    }

    CardTypeString   = SDEmmcDeviceCardTypeStr (&(mDriveInfos[Index]));
    ModelNameString  = SDEmmcDeviceNameStr (&(mDriveInfos[Index]));
    CapacityString   = SDEmmcDeviceCapacityStr (&(mDriveInfos[Index]));

    ConsoleBootMsg->ConsolePrintHiiString (
                      Console,
                      MaxColumn,
                      MaxRow,
                      ARRAY_SIZE (SdEmmcInfoTextAttributes),
                      SdEmmcInfoTextAttributes,
                      HiiHandle,
                      STRING_TOKEN (STR_SD_EMMC_INFORMATION_FORMAT),
                      NULL, // Language
                      PortName,
                      CardTypeString,
                      ModelNameString,
                      CapacityString
                      );

    FreePoolIfNonNull (ModelNameString);
    FreePoolIfNonNull (CardTypeString);
    FreePoolIfNonNull (CapacityString);
  }

  Console->SetAttribute (
             Console,
             (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK)
             );

  if (mDriveInfos != NULL) {
    FreePool (mDriveInfos);
    mDriveInfos = NULL;
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));
  return EFI_SUCCESS;
}
