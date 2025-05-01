/** @file

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

#include <SetupUtility.h>
#include <Protocol/SetupMouse.h>
#include <SetupConfig.h>

UINT8                           mFullResetFlag = 0;
EFI_HII_HANDLE                  mDriverHiiHandle;
EFI_GUID                        mFormSetGuid = SYSTEM_CONFIGURATION_GUID;
CHAR16                          mVariableName[] = L"SystemConfig";
SETUP_UTILITY_BROWSER_DATA      *gSUBrowser;
UINT16                          gCallbackKey;
USER_UNINSTALL_CALLBACK_ROUTINE mUninstallCallbackRoutine[] ={
  UninstallExitCallbackRoutine,     UninstallBootCallbackRoutine,
  UninstallPowerCallbackRoutine,    UninstallSecurityCallbackRoutine,
  UninstallAdvanceCallbackRoutine,  UninstallMainCallbackRoutine
  };

#pragma pack(1)
typedef struct {
  VENDOR_DEVICE_PATH             VendorDevicePath;
  UINT32                         Reserved;
  UINT64                         UniqueId;
} HII_VENDOR_DEVICE_PATH_NODE;
#pragma pack()

typedef struct {
  HII_VENDOR_DEVICE_PATH_NODE    Node;
  EFI_DEVICE_PATH_PROTOCOL       End;
} HII_TEMP_DEVICE_PATH;

//
// Hii vendor device path template
//
HII_TEMP_DEVICE_PATH  mHiiVendorDevicePathTemplate = {
  {
    {
      {
        HARDWARE_DEVICE_PATH,
        HW_VENDOR_DP,
        {(UINT8) (sizeof (HII_VENDOR_DEVICE_PATH_NODE)),
        (UINT8) ((sizeof (HII_VENDOR_DEVICE_PATH_NODE)) >> 8)}
      },
      EFI_IFR_TIANO_GUID,
    },
    0,
    0
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {END_DEVICE_PATH_LENGTH,
    0}
  }
};

/**
 The HII driver handle passed in for HiiDatabase.NewPackageList() requires
 that there should be DevicePath Protocol installed on it.
 This routine create a virtual Driver Handle by installing a vendor device
 path on it, so as to use it to invoke HiiDatabase.NewPackageList().

 @param [in]     DriverHandle         Handle to be returned

 @retval EFI_SUCCESS            Handle destroy success.
 @retval EFI_OUT_OF_RESOURCES   Not enough memory.

**/
EFI_STATUS
CreateHiiDriverHandle (
  OUT EFI_HANDLE               *DriverHandle
  )
{
  EFI_STATUS                   Status;
  HII_VENDOR_DEVICE_PATH_NODE  *VendorDevicePath;

  VendorDevicePath = AllocateCopyPool (sizeof (HII_TEMP_DEVICE_PATH), &mHiiVendorDevicePathTemplate);
  if (VendorDevicePath == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Use memory address as unique ID to distinguish from different device paths
  //
  VendorDevicePath->UniqueId = (UINT64) ((UINTN) VendorDevicePath);

  *DriverHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  VendorDevicePath,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
 Destroy the Driver Handle created by CreateHiiDriverHandle().

 @param [in]     DriverHandle   Handle returned by CreateHiiDriverHandle()

 @retval EFI_SUCCESS            Handle destroy success.
 @return other                  Handle destroy fail.

**/
EFI_STATUS
DestroyHiiDriverHandle (
  IN EFI_HANDLE                 DriverHandle
  )
{
  EFI_STATUS                   Status;
  EFI_DEVICE_PATH_PROTOCOL     *DevicePath;

  Status = gBS->HandleProtocol (
                  DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID**)&DevicePath
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->UninstallProtocolInterface (
                  DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  DevicePath
                  );
  gBS->FreePool (DevicePath);
  return Status;
}

EFI_STATUS
EFIAPI
SetupUtilityInit (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                            Status;
  SETUP_UTILITY_DATA                    *SetupData;
  EFI_EVENT                             Event;
  VOID                                  *Registration;
  CHIPSET_CONFIGURATION                 *SetupNvData;
  UINTN                                 VarSize;

  CheckLanguage ();

  SetupData = AllocatePool (sizeof(SETUP_UTILITY_DATA));
  if (SetupData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SetupData->Signature                    = EFI_SETUP_UTILITY_SIGNATURE;
  SetupData->SetupUtility.StartEntry      = NULL;
  SetupData->SetupUtility.PowerOnSecurity = PowerOnSecurity;
  mFullResetFlag = 0;

  //
  // The FirstIn flag is for that if there is the first time entering SCU, we should install
  // Vfr of Menu to Hii database.
  // After that, we shouldn't install the Vfr to Hii database again.
  //
  SetupData->SetupUtility.FirstIn         = TRUE;
  Status = GetSystemConfigurationVar (SetupData);

  SetupNvData = NULL;
  CommonGetVariableDataAndSize (SETUP_VARIABLE_NAME, &gSystemConfigurationGuid, &VarSize, (VOID**)(&SetupNvData));
  if (SetupNvData == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Install Setup Utility
  //
  SetupData->Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                 &SetupData->Handle,
                 &gEfiSetupUtilityProtocolGuid,
                 EFI_NATIVE_INTERFACE,
                 &SetupData->SetupUtility
                 );
  if (EFI_ERROR (Status)) {
    gBS->FreePool (SetupData);
    return Status;
  }

  //
  // When execute Setup Utility application, install HII data
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SetupUtilityNotifyFn,
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

  gBS->FreePool (SetupNvData);

  return Status;
}

EFI_STATUS
CreateScuData (
  BOOLEAN  DoClearScreen
  )
{
  EFI_STATUS                                Status;
  EFI_SETUP_UTILITY_BROWSER_PROTOCOL        *Interface;
  UINTN                                     BufferSize;
  UINT8                                     *Lang;
  EFI_SETUP_UTILITY_PROTOCOL                *This;

  Status = gBS->LocateProtocol (
                  &gEfiSetupUtilityBrowserProtocolGuid,
                  NULL,
                  (VOID **)&Interface
                  );
  //
  // If there was no error, assume there is an installation and fail to load
  //
  if (EFI_ERROR(Status) ) {
    Status = gBS->LocateProtocol (&gEfiSetupUtilityProtocolGuid, NULL, (VOID **) &This);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = InitSetupUtilityBrowser(This);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    Status = InstallSetupUtilityBrowserProtocol (This);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    ZeroMem (gSUBrowser->SUCInfo->MapTable, sizeof (HII_HANDLE_VARIABLE_MAP_TABLE) * MAX_HII_HANDLES);
    Status = InstallHiiData ();
    This->FirstIn = FALSE;
    if (EFI_ERROR(Status)) {
      return Status;
    }

    //
    // Load the variable data records
    //
    gSUBrowser->Interface.SCBuffer = (UINT8 *) gSUBrowser->SCBuffer;
  }
  Lang = GetVariableAndSize (
           L"PlatformLang",
           &gEfiGlobalVariableGuid,
           &BufferSize
           );
  if (Lang != NULL) {
    Status = gRT->SetVariable (
                    L"BackupPlatformLang",
                    &gEfiGenericVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    BufferSize,
                    Lang
                    );
    gBS->FreePool (Lang);
  }
  return Status;
}

EFI_STATUS
DestroyScuData (
  BOOLEAN  DoClearScreen
  )
{
  EFI_STATUS                                Status;
  UINTN                                     BufferSize;
  UINT8                                     *Lang;
  EFI_SYS_PASSWORD_SERVICE_PROTOCOL         *SysPasswordService;
  EFI_SETUP_UTILITY_PROTOCOL                *This;

  Status = gBS->LocateProtocol (
                  &gEfiSysPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **)&SysPasswordService
                  );
  ASSERT_EFI_ERROR (Status);
  SysPasswordService->LockPassword (SysPasswordService);
  Status = RemoveHiiData (
             SetupUtilityStrings,
             (HII_HANDLE_VARIABLE_MAP_TABLE *) &(gSUBrowser->SUCInfo->MapTable[ExitHiiHandle]),
             (HII_HANDLE_VARIABLE_MAP_TABLE *) &(gSUBrowser->SUCInfo->MapTable[BootHiiHandle]),
             (HII_HANDLE_VARIABLE_MAP_TABLE *) &(gSUBrowser->SUCInfo->MapTable[PowerHiiHandle]),
             (HII_HANDLE_VARIABLE_MAP_TABLE *) &(gSUBrowser->SUCInfo->MapTable[SecurityHiiHandle]),
             (HII_HANDLE_VARIABLE_MAP_TABLE *) &(gSUBrowser->SUCInfo->MapTable[AdvanceHiiHandle]),
             (HII_HANDLE_VARIABLE_MAP_TABLE *) &(gSUBrowser->SUCInfo->MapTable[MainHiiHandle]),
             NULL
             );

  Status = UninstallSetupUtilityBrowserProtocol ();
  Status = gBS->LocateProtocol (&gEfiSetupUtilityProtocolGuid, NULL, (VOID **) &This);
  if (!EFI_ERROR(Status)) {
    This->FirstIn = TRUE;
  }
  gSUBrowser = NULL;
  if (DoClearScreen) {
    if (gST->ConOut != NULL) {
      gST->ConOut->SetAttribute (gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
      gST->ConOut->ClearScreen (gST->ConOut);
    }
  }
  Lang = GetVariableAndSize (
           L"BackupPlatformLang",
           &gEfiGenericVariableGuid,
           &BufferSize
           );
  if (Lang != NULL) {
    Status = gRT->SetVariable (
                    L"PlatformLang",
                    &gEfiGlobalVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    BufferSize,
                    Lang
                    );
    gBS->FreePool (Lang);

    gRT->SetVariable (
           L"BackupPlatformLang",
           &gEfiGenericVariableGuid,
           EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
           0,
           NULL
           );
  }

  return Status;
}

EFI_STATUS
PowerOnSecurity (
  IN  EFI_SETUP_UTILITY_PROTOCOL        *SetupUtility
  )
{
  EFI_STATUS                            Status;
  EFI_HII_HANDLE                        HiiHandle;
  EFI_GUID                              StringPackGuid = STRING_PACK_GUID;
  SETUP_UTILITY_CONFIGURATION           *SUCInfo = NULL;
  EFI_SETUP_UTILITY_BROWSER_PROTOCOL    *Interface;
  EFI_HANDLE                            DriverHandle;

  HiiHandle = 0;

  //
  // There will be only one DeviceManagerSetup in the system.
  // If there is another out there, someone is trying to install us
  // again.  We fail in that scenario.
  //
  Status = gBS->LocateProtocol (
                  &gEfiSetupUtilityBrowserProtocolGuid,
                  NULL,
                  (VOID **)&Interface
                  );
  //
  // If there was no error, assume there is an installation and fail to load
  //
  if (!EFI_ERROR(Status) ) {
    Status = UninstallSetupUtilityBrowserProtocol ();
  }

  Status = InitSetupUtilityBrowser (SetupUtility);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = InstallSetupUtilityBrowserProtocol (SetupUtility);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  SUCInfo = gSUBrowser->SUCInfo;
  Status = CreateHiiDriverHandle (&DriverHandle);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  HiiHandle = HiiAddPackages (&StringPackGuid, DriverHandle, SecurityVfrBin ,SetupUtilityLibStrings, NULL);
  ASSERT(HiiHandle != NULL);

  gSUBrowser->SUCInfo->MapTable[SecurityHiiHandle].HiiHandle = HiiHandle;
  gSUBrowser->Interface.SCBuffer = (UINT8 *) gSUBrowser->SCBuffer;

  Status = gBS->LocateProtocol (
                  &gEfiSysPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **)&SUCInfo->SysPasswordService
                  );

  SUCInfo->SupervisorPassword = NULL;
  SUCInfo->UserPassword       = NULL;

  if (!EFI_ERROR(Status)) {

    //
    // Check password
    //
    if ((((CHIPSET_CONFIGURATION *) gSUBrowser->SCBuffer)->PowerOnPassword) == POWER_ON_PASSWORD) {
      Status = PasswordCheck (
                 SUCInfo,
                 (KERNEL_CONFIGURATION *) gSUBrowser->SCBuffer
                 );
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (SUCInfo->SupervisorPassword  != NULL) {
    if (SUCInfo->SupervisorPassword->NumOfEntry != 0) {
      gBS->FreePool (SUCInfo->SupervisorPassword->InputString);
      gBS->FreePool (SUCInfo->SupervisorPassword);
      SUCInfo->SupervisorPassword = NULL;
     }
  }

  if (SUCInfo->UserPassword  != NULL) {
    if (SUCInfo->UserPassword->NumOfEntry != 0) {
      gBS->FreePool (SUCInfo->UserPassword->InputString);
      gBS->FreePool (SUCInfo->UserPassword);
      SUCInfo->UserPassword = NULL;
     }
  }

  gSUBrowser->HiiDatabase->RemovePackageList (gSUBrowser->HiiDatabase, HiiHandle);
  DestroyHiiDriverHandle (DriverHandle);
  Status = UninstallSetupUtilityBrowserProtocol ();

  return Status;
}

/**
 Installs the SetupUtilityBrowser protocol including allocating
 storage for variable record data.

 @param [in]   This

 @retval EFI_SUCEESS            Protocol was successfully installed
 @retval EFI_OUT_OF_RESOURCES   Not enough resource to allocate data structures
 @return Other                  Some other error occured

**/
EFI_STATUS
InstallSetupUtilityBrowserProtocol (
  IN  EFI_SETUP_UTILITY_PROTOCOL            *This
  )
{
  EFI_STATUS                                 Status;

  gSUBrowser->Interface.AtRoot        = TRUE;
  gSUBrowser->Interface.Finished      = FALSE;
  gSUBrowser->Interface.Guid          = &mFormSetGuid;
  gSUBrowser->Interface.UseMenus      = FALSE;
  gSUBrowser->Interface.Direction     = NoChange;
  gSUBrowser->Interface.CurRoot       = FALSE;
  gSUBrowser->Interface.MenuItemCount = FALSE;
  gSUBrowser->Interface.Size          = PcdGet32 (PcdSetupConfigSize);
  gSUBrowser->Interface.Firstin       = TRUE;
  gSUBrowser->Interface.Changed       = FALSE;
  gSUBrowser->Interface.JumpToFirstOption = TRUE;

  gSUBrowser->Interface.SCBuffer      = NULL;
  gSUBrowser->Interface.MyIfrNVData = NULL;
  gSUBrowser->Interface.PreviousMenuEntry = 0;
  //
  // Install the Protocol
  //
  gSUBrowser->Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &gSUBrowser->Handle,
                  &gEfiSetupUtilityBrowserProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &gSUBrowser->Interface
                  );

  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
 Uninstalls the DeviceManagerSetup protocol and frees memory
 used for storing variable record data.

 @param None

 @retval EFI_SUCEESS            Protocol was successfully installed
 @retval EFI_ALREADY_STARTED    Protocol was already installed
 @retval EFI_OUT_OF_RESOURCES   Not enough resource to allocate data structures
 @return Other                  Some other error occured

**/
EFI_STATUS
UninstallSetupUtilityBrowserProtocol (
  VOID
  )
{
  EFI_STATUS                                Status;

  Status = gBS->UninstallProtocolInterface (
                  gSUBrowser->Handle,
                  &gEfiSetupUtilityBrowserProtocolGuid,
                  &gSUBrowser->Interface
                  );

  if ( EFI_ERROR(Status) ) {
    return Status;
  }
  if (gSUBrowser->SCBuffer != NULL) {
    gBS->FreePool (gSUBrowser->SCBuffer);
  }


  if (gSUBrowser->SUCInfo->HddPasswordScuData  != NULL) {
    if (gSUBrowser->SUCInfo->HddPasswordScuData->NumOfEntry != 0) {
      if (gSUBrowser->SUCInfo->HddPasswordScuData[0].HddInfo != NULL) {
        gBS->FreePool (gSUBrowser->SUCInfo->HddPasswordScuData[0].HddInfo);
      }
      gBS->FreePool (gSUBrowser->SUCInfo->HddPasswordScuData);
    }
  }
  if (gSUBrowser->SUCInfo->SupervisorPassword  != NULL) {
    if (gSUBrowser->SUCInfo->SupervisorPassword->NumOfEntry != 0) {
      gBS->FreePool (gSUBrowser->SUCInfo->SupervisorPassword->InputString);
      gBS->FreePool (gSUBrowser->SUCInfo->SupervisorPassword);
    }
  }

  if (gSUBrowser->SUCInfo->UserPassword  != NULL) {
    if (gSUBrowser->SUCInfo->UserPassword->NumOfEntry != 0) {
      gBS->FreePool (gSUBrowser->SUCInfo->UserPassword->InputString);
      gBS->FreePool (gSUBrowser->SUCInfo->UserPassword);
    }
  }

  gBS->FreePool (gSUBrowser->SUCInfo);
  gBS->FreePool (gSUBrowser);

  return EFI_SUCCESS;
}

EFI_STATUS
GetSystemConfigurationVar (
  IN OUT SETUP_UTILITY_DATA                 *SetupData
  )
{
  EFI_STATUS                                Status;
  UINTN                                     BufferSize;
  CHIPSET_CONFIGURATION                     *SetupNvData;

  //
  // Check the setup variable was create or not, if not then create default setup variable.
  //
  BufferSize = PcdGet32 (PcdSetupConfigSize);
  SetupNvData = AllocateZeroPool(BufferSize);
  if (SetupNvData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gRT->GetVariable (
                  L"Setup",
                  &mFormSetGuid,
                  NULL,
                  &BufferSize,
                  (VOID *)SetupNvData
                  );
  if (EFI_ERROR (Status)) {
    //
    // "Setup" Variable doesn't exist,so get a buffer with default variable
    //
    DefaultSetup (SetupNvData);

    BufferSize = PcdGet32 (PcdSetupConfigSize);
    Status = SaveSetupConfig (
               L"Setup",
               &mFormSetGuid,
               EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               BufferSize,
               (VOID *) SetupNvData
               );
  }

  FreePool ((VOID *)SetupNvData);
  return EFI_SUCCESS;
}

/**
 Installs a string and ifr pack set

 @param None

 @retval VOID

**/
EFI_STATUS
InstallHiiData (
  VOID
  )
{
  EFI_HII_HANDLE                 HiiHandle;

  EFI_HANDLE                     DriverHandle;
  UINTN                          Index;
  EFI_STATUS                     Status;
  UINTN                          HandleCnt;
  NEW_PACKAGE_INFO               NewPackageInfo [] =
                                 {{InstallExitCallbackRoutine,     ExitVfrBin,     SetupUtilityLibStrings, SetupUtilityLibImages},
                                  {InstallBootCallbackRoutine,     BootVfrBin,     SetupUtilityLibStrings, SetupUtilityLibImages},
                                  {InstallPowerCallbackRoutine,    PowerVfrBin,    SetupUtilityStrings,    SetupUtilityImages},
                                  {InstallSecurityCallbackRoutine, SecurityVfrBin, SetupUtilityLibStrings, SetupUtilityLibImages},
                                  {InstallAdvanceCallbackRoutine,  AdvanceVfrBin,  SetupUtilityStrings,    SetupUtilityImages},
                                  {InstallMainCallbackRoutine,     MainVfrBin,     SetupUtilityLibStrings, SetupUtilityLibImages}
                                 };

  HandleCnt = sizeof (NewPackageInfo) / sizeof (NEW_PACKAGE_INFO);
  if (HandleCnt > MAX_HII_HANDLES) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = EFI_SUCCESS;
  for (Index = 0; Index < HandleCnt; Index++) {
    Status = CreateHiiDriverHandle (&DriverHandle);
    if (EFI_ERROR (Status)) {
      break;
    }

    HiiHandle = HiiAddPackages (
                  &mFormSetGuid,
                  DriverHandle,
                  NewPackageInfo[Index].IfrPack,
                  NewPackageInfo[Index].StringPack,
                  NewPackageInfo[Index].ImagePack,
                  NULL
                  );
    ASSERT(HiiHandle != NULL);

    gSUBrowser->SUCInfo->MapTable[Index].HiiHandle = HiiHandle;
    gSUBrowser->SUCInfo->MapTable[Index].DriverHandle = DriverHandle;
    NewPackageInfo[Index].CallbackRoutine (DriverHandle, HiiHandle);

  }

  mDriverHiiHandle = gSUBrowser->SUCInfo->MapTable[AdvanceHiiHandle].HiiHandle;
  UpdateMemoryInfo (gSUBrowser);
  DisplayPlatformInfo (gSUBrowser);

  return Status;
}

EFI_STATUS
InitSetupUtilityBrowser(
  IN  EFI_SETUP_UTILITY_PROTOCOL            *This
  )
{
  EFI_STATUS                                Status;
  UINT8                                     *Setup = NULL;
  UINTN                                     VarSize;

  gSUBrowser = AllocateZeroPool (sizeof(SETUP_UTILITY_BROWSER_DATA));
  if (gSUBrowser == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  gSUBrowser->SUCInfo = AllocateZeroPool (sizeof(SETUP_UTILITY_CONFIGURATION));
  if (gSUBrowser->SUCInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Setup = NULL;
  CommonGetVariableDataAndSize (SETUP_VARIABLE_NAME, &gSystemConfigurationGuid, &VarSize, (VOID**)(&Setup));
  if (Setup == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = SaveSetupConfig (
              L"Setup",
              &mFormSetGuid,
              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
              VarSize,
              (VOID *)Setup
              );
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "SyncOemConfigurationVariable %r !!!\n",Status));
    return Status;
  }
  gSUBrowser->SCBuffer          = Setup;
  gSUBrowser->SUCInfo->SCBuffer = gSUBrowser->SCBuffer;
  gSUBrowser->Signature         = EFI_SETUP_UTILITY_BROWSER_SIGNATURE;
  gSUBrowser->ExtractConfig     = GenericExtractConfigHook;
  gSUBrowser->RouteConfig       = GenericRouteConfig;
  gSUBrowser->HotKeyCallback    = HotKeyCallBack;
  //
  // There should only be one HII protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiHiiDatabaseProtocolGuid,
                  NULL,
                  (VOID **)&gSUBrowser->HiiDatabase
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // There should only be one HII protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiHiiStringProtocolGuid,
                  NULL,
                  (VOID **)&gSUBrowser->HiiString
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // There should only be one HII protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiHiiConfigRoutingProtocolGuid,
                  NULL,
                  (VOID **)&gSUBrowser->HiiConfigRouting
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }


  //
  // There will be only one FormConfig in the system
  // If there is another out there, someone is trying to install us
  // again.  Fail that scenario.
  //
  Status = gBS->LocateProtocol (
                  &gEfiFormBrowser2ProtocolGuid,
                  NULL,
                  (VOID **)&gSUBrowser->Browser2
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = gBS->LocateProtocol (
                 &gH2ODialogProtocolGuid,
                 NULL,
                 (VOID **) &gSUBrowser->H2ODialog
                 );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
};

/**
 Installs a string and ifr pack set

 @param [in]   StringPack       string pack to store in database and associate with IfrPack
                                IrfPack - ifr pack to store in database (it will use StringPack data)
 @param        ...

 @retval VOID

**/
EFI_STATUS
RemoveHiiData (
  IN VOID     *StringPack,
  ...
  )
{
  VA_LIST                                   args;
  HII_HANDLE_VARIABLE_MAP_TABLE             *MapTable;
  EFI_HII_DATABASE_PROTOCOL                 *HiiDatabase;
  UINTN                                     Index;

  HiiDatabase = gSUBrowser->HiiDatabase;
  VA_START (args, StringPack );

  Index = 0;
  while (TRUE) {
    MapTable = VA_ARG( args, HII_HANDLE_VARIABLE_MAP_TABLE *);
    if (MapTable == NULL) {
      break;
    }
    HiiDatabase->RemovePackageList (HiiDatabase, MapTable->HiiHandle);
    mUninstallCallbackRoutine[Index++] (MapTable->DriverHandle);
    DestroyHiiDriverHandle (MapTable->DriverHandle);
  }

  if (gSUBrowser->IdeConfig != NULL) {
    gBS->FreePool (gSUBrowser->IdeConfig);
    gSUBrowser->IdeConfig = NULL;
  }

  return EFI_SUCCESS;

}

VOID
EFIAPI
SetupUtilityNotifyFn (
  IN EFI_EVENT                             Event,
  IN VOID                                  *Context
  )
{
  EFI_STATUS                               Status;
  EFI_SETUP_UTILITY_APPLICATION_PROTOCOL   *SetupUtilityApp;
  BOOLEAN                                  DoClearScreen;

  Status = gBS->LocateProtocol (
                  &gEfiSetupUtilityApplicationProtocolGuid,
                  NULL,
                  (VOID **) &SetupUtilityApp
                  );
  if (EFI_ERROR(Status)) {
    return;
  }

  if (SetupUtilityApp->VfrDriverState == InitializeSetupUtility) {
    POST_CODE (BDS_ENTER_SETUP); //PostCode = 0x29, Enter Setup Menu
  }

  DoClearScreen = TRUE;

  switch (SetupUtilityApp->VfrDriverState) {

  case InstallSetupUtilityHiiData:
    DoClearScreen = FALSE;
    CreateScuData (DoClearScreen);
    break;

  case UnInstallSetupUtilityHiiData:
    DoClearScreen = FALSE;
    DestroyScuData (DoClearScreen);
    break;

  case InitializeSetupUtility:
    CreateScuData (DoClearScreen);
    break;

  case ShutdownSetupUtility:
    DestroyScuData (DoClearScreen);
    break;

  default:
    break;
  }
}

