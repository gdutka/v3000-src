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

#include <SetupFuncs.h>
#include <Library/BaseSetupDefaultLib.h>

EFI_STATUS
CheckLanguage (
  VOID
  )
{
  EFI_STATUS                            Status;
  CHAR8                                 *LangCode;
  UINT8                                 *BackupLang;
  UINTN                                 BufferSize;
  UINTN                                 BackupBufferSize;

  LangCode   = GetVariableAndSize (L"PlatformLang", &gEfiGlobalVariableGuid, &BufferSize);
  BackupLang = GetVariableAndSize (L"BackupPlatformLang", &gEfiGenericVariableGuid, &BackupBufferSize);
  if (LangCode == NULL || !SetupUtilityLibIsLangCodeSupport (LangCode)) {
    //
    // if cannot find current language, set default language as english
    //
    if (BackupLang == NULL || !SetupUtilityLibIsLangCodeSupport ((CHAR8 *)BackupLang)) {
      Status = gRT->SetVariable (
                      L"PlatformLang",
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                      AsciiStrSize ((CHAR8 *) PcdGetPtr (PcdUefiVariableDefaultPlatformLang)),
                      (VOID *) PcdGetPtr (PcdUefiVariableDefaultPlatformLang)
                      );
    } else {
      Status = gRT->SetVariable (
                      L"PlatformLang",
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                      BackupBufferSize,
                      BackupLang
                      );
    }

    gRT->SetVariable (
           L"BackupPlatformLang",
           &gEfiGenericVariableGuid,
           EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
           0,
           NULL
           );
    if (LangCode != NULL) {
      gBS->FreePool (LangCode);
    }
    if (BackupLang != NULL) {
      gBS->FreePool (BackupLang);
    }
    return Status;
  }

  if (BackupLang != NULL) {
    if (BackupBufferSize != BufferSize || CompareMem (BackupLang, LangCode, BufferSize)) {
      Status = gRT->SetVariable (
                      L"PlatformLang",
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                      BackupBufferSize,
                      BackupLang
                      );
    }

    Status = gRT->SetVariable (
                    L"BackupPlatformLang",
                    &gEfiGenericVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    0,
                    NULL
                    );
    gBS->FreePool (BackupLang);
  }
  gBS->FreePool (LangCode);
  return EFI_SUCCESS;
}

/**
 Read the EFI variable (VendorGuid/Name) and return a dynamically allocated
 buffer, and the size of the buffer. If failure return NULL.

 Name           String part of EFI variable name
 VendorGuid     GUID part of EFI variable name
 VariableSize   Returns the size of the EFI variable that was read

 @return Dynamically allocated memory that contains a copy of the EFI variable.
 @return Caller is responsible freeing the buffer.
 @retval NULL                   Variable was not read

**/
VOID *
GetVariableAndSize (
  IN  CHAR16              *Name,
  IN  EFI_GUID            *VendorGuid,
  OUT UINTN               *VariableSize
  )
{
  EFI_STATUS  Status;
  UINTN       BufferSize;
  VOID        *Buffer;

  Buffer = NULL;

  //
  // Pass in a zero size buffer to find the required buffer size.
  //
  BufferSize  = 0;

  Status      = gRT->GetVariable (Name, VendorGuid, NULL, &BufferSize, Buffer);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    //
    // Allocate the buffer to return
    //

    Buffer = AllocateZeroPool (BufferSize);
    if (Buffer == NULL) {
      return NULL;
    }

    //
    // Read variable into the allocated buffer.
    //

    Status = gRT->GetVariable (Name, VendorGuid, NULL, &BufferSize, Buffer);
    if (EFI_ERROR (Status)) {
      BufferSize = 0;
      gBS->FreePool (Buffer);
      Buffer = NULL;
    }

  }

  *VariableSize = BufferSize;

  return Buffer;
}

/**
 Save Setup Configuration to NV storage and call KERNEL_CALCULATE_WRITE_CMOS_CHECKSUM
 OEM service

 @param [in]   VariableName     The name of variable to load
 @param [in]   VendorGuid       It's vendor GUID
 @param [in]   Attributes       attributes of the data to be stored
 @param [in]   DataSize         The size of the data
 @param [in]   Buffer           Space to store data to be written to NVRam

 @retval EFI_SUCCESS            Save Setup Configuration successful
 @retval EFI_INVALID_PARAMETER  The input parameter is invalid
 @return Other                  Some other error occured

**/
EFI_STATUS
SaveSetupConfig (
  IN     CHAR16             *VariableName,
  IN     EFI_GUID           *VendorGuid,
  IN     UINT32             Attributes,
  IN     UINTN              DataSize,
  IN     VOID               *Buffer
  )
{
  EFI_STATUS                Status;


  if (Buffer == NULL || VariableName == NULL || VendorGuid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = SetVariableToSensitiveVariable (
                  VariableName,
                  VendorGuid,
                  Attributes,
                  DataSize,
                  Buffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}


/**
 Build a default variable value an save to a buffer according to platform requirement

 @param [out]  SetupNvData

 @retval EFI_SUCCESS            Build default setup successful.
 @retval EFI_INVALID_PARAMETER  Input value is invalid.

**/
EFI_STATUS
DefaultSetup (
  OUT CHIPSET_CONFIGURATION            *SetupNvData
  )
{
//  EFI_GUID                             SetupVariableGuidId = SYSTEM_CONFIGURATION_GUID;
  UINTN                                Size;

  if (SetupNvData == NULL) {
    return EFI_INVALID_PARAMETER;
  }


  Size = sizeof(SYSTEM_CONFIGURATION);
  ExtractSetupDefault ((UINT8 *) SetupNvData );
//  if (FeaturePcdGet (PcdMultiConfigSupported)) {
//    SetSCUDataFromMC (L"Setup", &SetupVariableGuidId, SETUP_FOR_LOAD_DEFAULT, (VOID *)SetupNvData, sizeof (SYSTEM_CONFIGURATION));
//  }

  SetupNvData->DefaultBootType   = DEFAULT_BOOT_FLAG;
  SetupNvData->Timeout = (UINT16) PcdGet16 (PcdPlatformBootTimeOut);
  GetLangIndex ((CHAR8 *) PcdGetPtr (PcdUefiVariableDefaultPlatformLang), &SetupNvData->Language);
  return EFI_SUCCESS;
}

