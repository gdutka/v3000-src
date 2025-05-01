/** @file
  Library instance to Capsule Update Recovery Criteria check

;******************************************************************************
;* Copyright (c) 2018 - 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MeShowBufferLib.h>
#include <Register/HeciRegs.h>
#include <Library/PciSegmentLib.h>
#include <Library/HeciInitLib.h>
#include <SecureFlash.h>
#include <Guid/FileInfo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Library/PrintLib.h>
#include <Library/ChipsetSignatureLib.h>
#include <Guid/ImageAuthentication.h>
#include <Library/VariableLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/ChipsetCapsuleLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <IndustryStandard/PeImage.h>
#include <Guid/MeUpdVariable.h>

#define MAX_STRING_LENGTH               128
#define ESRT_LAST_ATTEMPT_VERSION       L"EsrtLastAttemptVersion"
#define ESRT_LAST_ATTEMPT_STATUS        L"EsrtLastAttemptStatus"
#define CAPSULE_RESULT_VARIABLE         L"Capsule0000"
#define CAPSULE_RESULT_MAX_VARIABLE     L"CapsuleMax"
#define CAPSULE_RESULT_LAST_VARIABLE    L"CapsuleLast"
#define EFI_CAPSULE_REPORT_GUID         { 0x39b68c46, 0xf7fb, 0x441b, { 0xb6, 0xec, 0x16, 0xb0, 0xf6, 0x98, 0x21, 0xf3 }}
#define MAX_RECOVERY_RETRY              3

typedef struct {
  EFI_SIGNATURE_LIST    SignatureListHeader;
  EFI_SIGNATURE_DATA    SignatureData;
} CERTIFICATE_DATA;

typedef struct {
  UINT32                VariableTotalSize;
  UINT32                Reserved;       //for alignment
  EFI_GUID              CapsuleGuid;
  EFI_TIME              CapsuleProcessed;
  EFI_STATUS            CapsuleStatus;
} EFI_CAPSULE_RESULT_VARIABLE_HEADER;

/**
  This function provides a standard way to verify the HECI cmd and MBAR regs
  in its PCI cfg space are setup properly and that the local mHeciContext
  variable matches this info.

  @param[in] HeciDev              HECI device to be accessed.

  @retval HeciMemBar              HECI Memory BAR.
                                  0 - invalid BAR value returned.
**/
UINTN
CheckAndFixHeciForAccess (
  IN HECI_DEVICE HeciDev
  );

/**
  Determine whether ME state is in recovery mode.

  @retval TRUE                          Is in Recovery mode.
  @retval FALSE                         Not in Recovery mode.

**/
BOOLEAN
EFIAPI
IsMeStateRecovery (
  VOID
  )
{
  HECI_FWS_REGISTER               MeFirmwareStatus;

  MeFirmwareStatus.ul = 0;

  if (CheckAndFixHeciForAccess (HECI1_DEVICE) != 0) {
    MeFirmwareStatus.ul = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_ME_HFS));
    if (MeFirmwareStatus.r.CurrentState == ME_STATE_RECOVERY) {
      return TRUE;
    }
    DEBUG ((
      EFI_D_ERROR | EFI_D_INFO,
      "[HECI%d] ERROR: HfSts1 = 0x%08X, ME Current State = 0x%02X\n",
      HECI1_DEVICE + 1,
      MeFirmwareStatus.ul,
      MeFirmwareStatus.r.CurrentState
      ));
  }

  return FALSE;
}

/**
  Determine whether is up to maximum try of ME recovery.

  @param[in] Recorder                   If TRUE, set the variable to record the times of recovery try.

  @retval TRUE                          Is up to maximum try.
  @retval FALSE                         Not up to maximum try.

**/
BOOLEAN
EFIAPI
IsMaxRecoveryTry (
  IN  BOOLEAN                           Recorder
  )
{
  EFI_STATUS                            Status;
  UINTN                                 Size;
  UINT8                                 MaxRecoveryRetryCounter;

  Size = sizeof (MaxRecoveryRetryCounter);
  Status = CommonGetVariable (
             L"MeRecoveryRetry",
             &gMeUpdCountGuid,
             &Size,
             &MaxRecoveryRetryCounter
             );
  if (EFI_ERROR (Status)) {
    MaxRecoveryRetryCounter = 1;
  }

  MaxRecoveryRetryCounter++;
  if (MaxRecoveryRetryCounter > MAX_RECOVERY_RETRY) {
    return TRUE;
  }

  if (Recorder) {
    //
    // Set recovery retry counter
    //
    Size = sizeof (MaxRecoveryRetryCounter);
    Status = CommonSetVariable (
               L"MeRecoveryRetry",
               &gMeUpdCountGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
               Size,
               &MaxRecoveryRetryCounter
               );
  }

  return FALSE;
}

/**
  Clear the variable of maximum try of ME recovery.

  @retval EFI_SUCCESS                Get EFI variable Successful.
  @return Other                      Other errors cause get variable failed.

**/
EFI_STATUS
EFIAPI
ClearMaxRecoveryTryVariable (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINTN                                 Size;
  UINT8                                 MaxRecoveryRetryCounter;

  Size = sizeof (MaxRecoveryRetryCounter);
  Status = CommonGetVariable (
             L"MeRecoveryRetry",
             &gMeUpdCountGuid,
             &Size,
             &MaxRecoveryRetryCounter
             );
  if (!EFI_ERROR (Status)) {
    //
    // Clear recovery retry counter
    //
    Status = CommonSetVariable (
               L"MeRecoveryRetry",
               &gMeUpdCountGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
               0,
               NULL
               );
  }

  return Status;
}

/**
  Get the certificate from firmware volume

  @param NameGuid               Pointer to the file GUID of the certificate
  @param Buffer                 Returned the address of the certificate
  @param Size                   Pointer to the size of the certificate

  @retval EFI_SUCCESS           The certificate was successfully retrieved
  @retval EFI_NOT_FOUND         Failed to find the certificate
  @retval EFI_LOAD_ERROR        Firmware Volume Protocol error
**/
STATIC
EFI_STATUS
GetCertificateData (
  IN     EFI_GUID               *NameGuid,
  IN OUT VOID                   **Buffer,
  IN OUT UINTN                  *Size
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         HandleCount;
  UINTN                         Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *Fv;
  UINT32                        AuthenticationStatus;

  Fv = NULL;
  AuthenticationStatus = 0;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || (HandleCount == 0)) {
    return EFI_NOT_FOUND;
  }
  //
  // Find desired image in all Fvs
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **)&Fv
                    );
    if (EFI_ERROR (Status)) {
      return EFI_LOAD_ERROR;
    }
    *Buffer = NULL;
    *Size = 0;
    Status = Fv->ReadSection (
                   Fv,
                   NameGuid,
                   EFI_SECTION_RAW,
                   0,
                   Buffer,
                   Size,
                   &AuthenticationStatus
                   );
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  if (Index >= HandleCount) {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

/**
  Load the certificate data to "SecureFlashCertData" variable
  The certificate is used when the Capsule image is loaded via gBS->LoadImage()

  @param  None

  @retval EFI_SUCCESS          Certificate variable was successfully set
  @retval EFI_NOT_FOUND        Certificate data was not found
  @retval EFI_OUT_OF_RESOURCES Out of memory
**/
STATIC
EFI_STATUS
LoadCertToVariable (
  VOID
  )
{
  EFI_STATUS            Status;
  CERTIFICATE_DATA      *CertData;
  UINTN                 CertSize;

  Status = GetCertificateData (PcdGetPtr (PcdSecureFlashCertificateFile), (VOID **)&CertData, &CertSize);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Status = CommonSetVariable (
             L"SecureFlashCertData",
             &gSecureFlashInfoGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             CertSize,
             CertData
             );
  FreePool (CertData);
  return Status;
}

/**

  Enable/disable security check of Capsule images

  @param[in] Enabled           The switch of security check of Capsule images

  @retval EFI_SUCCESS          Security check of Capsule images is disabled
  @return others               Failed to disable Capsule security check

**/
STATIC
EFI_STATUS
CapsuleSecurityCheck (
  IN     BOOLEAN        Enabled
  )
{
  EFI_STATUS  Status;
  UINT8       SetupMode;

  if (Enabled) {
    Status = LoadCertToVariable();
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //
    // Set SecureFlashSetupMode variable to trigger image verification process.
    //
    SetupMode = USER_MODE;
    Status = CommonSetVariable (
               SECURE_FLASH_SETUP_MODE_NAME,
               &gSecureFlashInfoGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               sizeof(SetupMode),
               &SetupMode
               );
  } else {
    //
    // Clear ceritificate data variable
    //
    Status = CommonSetVariable (
               L"SecureFlashCertData",
               &gSecureFlashInfoGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               0,
               NULL
               );
    //
    // Clear SecureFlashSetupMode variable
    //
    Status = CommonSetVariable (
               SECURE_FLASH_SETUP_MODE_NAME,
               &gSecureFlashInfoGuid,
               EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
               0,
               NULL
               );
  }
  return Status;
}

/**
  Convert EFI Status to ESRT status

  @param[in] Status     The value of EFI status

  @retval ESRT_SUCCESS
  @retval ESRT_ERROR_UNSUCCESSFUL
  @retval ESRT_ERROR_INSUFFICIENT_RESOURCES
  @retval ESRT_ERROR_INCORRECT_VERSION
  @retval ESRT_ERROR_INVALID_IMAGE_FORMAT
  @retval ESRT_ERROR_AUTHENTICATION
  @retval ESRT_ERROR_AC_NOT_CONNECTED
  @retval ESRT_ERROR_INSUFFICIENT_BATTERY
**/
STATIC
ESRT_STATUS
ConvertEfiStatusToEsrtStatus (
  IN     EFI_STATUS     Status
  )
{
  ESRT_STATUS EsrtStatus;

  switch (Status) {
    case EFI_SUCCESS:
      EsrtStatus = ESRT_SUCCESS;
      break;
    case EFI_INCOMPATIBLE_VERSION:
      EsrtStatus = ESRT_ERROR_INCORRECT_VERSION;
      break;
    case EFI_OUT_OF_RESOURCES:
    case EFI_VOLUME_FULL:
      EsrtStatus = ESRT_ERROR_INSUFFICIENT_RESOURCES;
      break;
    case EFI_UNSUPPORTED:
    case EFI_LOAD_ERROR:
      EsrtStatus = ESRT_ERROR_INVALID_IMAGE_FORMAT;
      break;
    case EFI_SECURITY_VIOLATION:
      EsrtStatus = ESRT_ERROR_AUTHENTICATION;
      break;
    default:
      EsrtStatus = ESRT_ERROR_UNSUCCESSFUL;
      break;
  }
  return EsrtStatus;
}

/**
  Update the status when is it EFI_SECURITY_VIOLATION

  @param[in]  Status            Status for input

  @return                       The updated status if input status is EFI_SECURITY_VIOLATION

**/
STATIC
EFI_STATUS
SecurityViolationStatusUpdate (
  IN     EFI_STATUS     Status
  )
{
  EFI_IMAGE_EXECUTION_INFO_TABLE        *ImageExeInfoTable;
  EFI_IMAGE_EXECUTION_INFO              *ImageExeInfo;
  UINTN                                 Index;
  UINTN                                 NumberOfImages;

  if (Status != EFI_SECURITY_VIOLATION) {
    return Status;
  }
  ImageExeInfoTable = NULL;
  EfiGetSystemConfigurationTable (&gEfiImageSecurityDatabaseGuid, (VOID**)&ImageExeInfoTable);
  if (ImageExeInfoTable == NULL) {
    return Status;
  }
  NumberOfImages = ImageExeInfoTable->NumberOfImages;
  ImageExeInfo = (EFI_IMAGE_EXECUTION_INFO*)(ImageExeInfoTable + 1);
  for (Index = 0; Index <  NumberOfImages - 1; Index++) {
    ImageExeInfo = (EFI_IMAGE_EXECUTION_INFO *) (((UINT8 *) ImageExeInfo) + ImageExeInfo->InfoSize);
  }
  if (ImageExeInfo->Action == EFI_IMAGE_EXECUTION_AUTH_UNTESTED) {
    //
    // Change the status for the image without signature
    //
    Status = EFI_INVALID_PARAMETER;
  }
  return Status;
}

/**
  Update capsule status onto corresponding variables, including ESRT status
  and Capsule#### and CapsuleLast variables according to UEFI 2.4B 7.5.6

  @param[in] CapsuleGuid        The pointer of capsule GUID
  @param[in] AttemptVersion     The value of last attempt version
  @param[in] AttemptStatus      The value of last attempt status

**/
STATIC
VOID
UpdateCapsuleStatus (
  IN     EFI_GUID       *CapsuleGuid,
  IN     UINT32         AttemptVersion,
  IN     ESRT_STATUS    AttemptStatus,
  IN     EFI_STATUS     CapsuleStatus
  )
{
  EFI_STATUS                            Status;
  UINTN                                 Size;
  EFI_CAPSULE_RESULT_VARIABLE_HEADER    CapsuleResult;
  UINTN                                 CapsuleResultCounter;
  UINTN                                 CapsuleResultMaxCounter;
  CHAR16                                CapsuleResultVariableName[16];
  EFI_GUID                              CapsuleResultVariableGuid = EFI_CAPSULE_REPORT_GUID;
  UINT32                                Mask;

  //
  // Update ESRT version and status
  //
  Mask = 0x0000ffff;
  AttemptVersion &= Mask;
  CommonSetVariable (
    ESRT_LAST_ATTEMPT_VERSION,
    CapsuleGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
    sizeof(UINT32),
    &AttemptVersion
    );
  CommonSetVariable (
    ESRT_LAST_ATTEMPT_STATUS,
    CapsuleGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
    sizeof(ESRT_STATUS),
    &AttemptStatus
    );
  //
  // Calculate the variable name for UEFI 2.4B 7.5.6
  //
  if ((PcdGet64 (PcdOsIndicationsSupported) & EFI_OS_INDICATIONS_CAPSULE_RESULT_VAR_SUPPORTED) == 0) return;
  CapsuleResultMaxCounter = (UINTN)PcdGet16 (PcdCapsuleMaxResult);
  Size = sizeof (CHAR16) * 11;
  Status = CommonGetVariable (
             CAPSULE_RESULT_MAX_VARIABLE,
             &CapsuleResultVariableGuid,
             &Size,
             &CapsuleResultVariableName
             );
  if (Status == EFI_NOT_FOUND) {
    UnicodeSPrint (CapsuleResultVariableName, sizeof(CapsuleResultVariableName), L"Capsule%04x", CapsuleResultMaxCounter);
    CommonSetVariable (
      CAPSULE_RESULT_MAX_VARIABLE,
      &CapsuleResultVariableGuid,
      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      Size,
      &CapsuleResultVariableName
      );
  }
//[-start-190611-IB16990046-add]//
  StrCpyS (CapsuleResultVariableName, ARRAY_SIZE(CapsuleResultVariableName), CAPSULE_RESULT_VARIABLE);
//[-end-190611-IB16990046s-add]//
  Size = sizeof (CHAR16) * 11;
  Status = CommonGetVariable (
             CAPSULE_RESULT_LAST_VARIABLE,
             &CapsuleResultVariableGuid,
             &Size,
             &CapsuleResultVariableName
             );
  if (!EFI_ERROR (Status)) {
    CapsuleResultCounter = StrHexToUintn (CapsuleResultVariableName + 7);
    if (CapsuleResultCounter == CapsuleResultMaxCounter) {
      CapsuleResultCounter = 0;
    } else {
      CapsuleResultCounter ++;
    }
    UnicodeSPrint (CapsuleResultVariableName, sizeof(CapsuleResultVariableName), L"Capsule%04x", CapsuleResultCounter);
  }
  //
  // Update UEFI capsule result status
  //
  CapsuleResult.VariableTotalSize = sizeof(EFI_CAPSULE_RESULT_VARIABLE_HEADER);
  CapsuleResult.Reserved          = 0;
  CopyGuid (&CapsuleResult.CapsuleGuid, CapsuleGuid);
  gRT->GetTime (&CapsuleResult.CapsuleProcessed, NULL);
  CapsuleResult.CapsuleStatus     = CapsuleStatus;
  CommonSetVariable (
    CapsuleResultVariableName,
    &CapsuleResultVariableGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
    CapsuleResult.VariableTotalSize,
    &CapsuleResult
    );
  CommonSetVariable (
    CAPSULE_RESULT_LAST_VARIABLE,
    &CapsuleResultVariableGuid,
    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
    sizeof (CHAR16) * 11,
    &CapsuleResultVariableName
    );
}

/**
  Process UEFI image in capsule image

  This function updates the hardware with the new firmware image.
  This function returns EFI_UNSUPPORTED if the firmware image is not updatable.
  If the firmware image is updatable, the function should perform the following minimal validations
  before proceeding to do the firmware image update.
  - Validate the image authentication if image has attribute
    IMAGE_ATTRIBUTE_AUTHENTICATION_REQUIRED.  The function returns
    EFI_SECURITY_VIOLATION if the validation fails.
  - Validate the image is a supported image for this device.  The function returns EFI_ABORTED if
    the image is unsupported.  The function can optionally provide more detailed information on
    why the image is not a supported image.

  @param[in]  Image              Points to the new image.
  @param[in]  ImageSize          Size of the new image in bytes.

  @retval EFI_SUCCESS            The device was successfully updated with the new image.
  @retval EFI_ABORTED            The operation is aborted.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_SECURITY_VIOLATION The operation could not be performed due to an authentication failure.

**/
EFI_STATUS
EFIAPI
RecoverySetImage (
  IN     CONST VOID                     *Capsule,
  IN     UINTN                          CapsuleSize
  )
{
  EFI_STATUS                            Status;
  EFI_HANDLE                            CapsuleHandle;
  UINT32                                AttemptVersion;
  ESRT_STATUS                           AttemptStatus;

  CapsuleHandle = NULL;

  Status = CapsuleSecurityCheck (TRUE);
  if (!EFI_ERROR (Status)) {
    Status = gBS->LoadImage (
                    FALSE,
                    gImageHandle,
                    NULL,
                    (UINT8 *)Capsule + ((EFI_CAPSULE_HEADER*)Capsule)->HeaderSize,
                    CapsuleSize - ((EFI_CAPSULE_HEADER*)Capsule)->HeaderSize,
                    (VOID **)&CapsuleHandle
                    );
    if (!EFI_ERROR (Status)) {
      //
      // Set firmware update flag by flashing the signature onto flash part for seamless recovery
      //
      SetDeviceFirmwareUpdatingFlag (TRUE);
      //
      // Start flash process
      //
      Status = gBS->StartImage (CapsuleHandle, NULL, NULL);
      //
      // Clear firmware update flag by erase the signature in flash part for seamless recovery
      //
      SetDeviceFirmwareUpdatingFlag (FALSE);
    }
  }
  CapsuleSecurityCheck (FALSE);
  AttemptVersion = GetIntelMeCapsuleFirmwareVersion ((EFI_CAPSULE_HEADER*)Capsule);
  AttemptStatus  = ConvertEfiStatusToEsrtStatus (Status);
  //
  // Update capsule status onto corresponding variables
  //
  UpdateCapsuleStatus (&((EFI_CAPSULE_HEADER*)Capsule)->CapsuleGuid, AttemptVersion, AttemptStatus, SecurityViolationStatusUpdate (Status));
  //
  // Reset system after BIOS flash completed if needed
  //
  if (CapsuleHandle) {
    gBS->UnloadImage (CapsuleHandle);
  }

  return Status;
}

/**
  Verify if the image is signed(SecureFlashLib.c).

  @param[in] ImageBase       The address of the image.

  @retval EFI_SUCCESS        if the image is not signed
  @retval EFI_ACCESS_DENIED  if the image is signed
**/
EFI_STATUS
VerifySignedImage (
  IN UINT8                      *ImageBase
  )
{
  EFI_IMAGE_DOS_HEADER                *DosHeader;
  UINT32                              PeCoffHeaderOffset;
  UINT16                              Magic;
  EFI_IMAGE_DATA_DIRECTORY            *SectionDataDir;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION PeHeader;


  DosHeader = (EFI_IMAGE_DOS_HEADER *)(UINTN)(ImageBase);
  if (DosHeader->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
    //
    // DOS image header is present,
    // so read the PE header after the DOS image header.
    //
    PeCoffHeaderOffset = DosHeader->e_lfanew;
  } else {
    PeCoffHeaderOffset = 0;
  }
  PeHeader.Pe32 = (EFI_IMAGE_NT_HEADERS32 *) (ImageBase + PeCoffHeaderOffset);
  if (PeHeader.Pe32->Signature != EFI_IMAGE_NT_SIGNATURE) {
    //
    // It is not a valid Pe/Coff file.
    //
    return EFI_ACCESS_DENIED;
  }

  Magic = PeHeader.Pe32->OptionalHeader.Magic;
  if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    //
    // Use PE32 offset.
    //
    SectionDataDir = (EFI_IMAGE_DATA_DIRECTORY *) &PeHeader.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY];
  } else {
    //
    // Use PE32+ offset.
    //
    SectionDataDir = (EFI_IMAGE_DATA_DIRECTORY *) &PeHeader.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY];
  }

  if (SectionDataDir->Size == 0) {
    //
    // This image is not signed.
    //
    return EFI_ACCESS_DENIED;
  }

  return EFI_SUCCESS;
}

/**
  Search the recovery image file and do the image flash process.

  @retval EFI_SUCCESS                   The device was successfully updated with the new image.
  @retval EFI_NOT_FOUND                 The specified file could not be found on the device.

**/
EFI_STATUS
MeRecovery (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINTN                                 NumberOfHandles;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 Index;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL       *SimpleFileSystem;
  EFI_FILE_HANDLE                       SysDir;
  EFI_FILE_HANDLE                       File;
  BOOLEAN                               Found;
  CHAR16                                ImagePath[MAX_STRING_LENGTH];
  UINTN                                 FlashImageSize;
  UINTN                                 BufferSize;
  EFI_FILE_INFO                         *FileInfo;
  UINT8                                 *FlashImageBuffer;

  HandleBuffer = NULL;
  BufferSize = 0;
  FileInfo = NULL;
  SysDir = NULL;
  File = NULL;
  SimpleFileSystem = NULL;
  //
  // Search all simple file system
  //
  Status = gBS->LocateHandleBuffer(
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Combine the image file path to "EFI\\UpdateCapsule\\CapsuleUpdateFile1000.bin"
  //
  UnicodeSPrint (ImagePath, MAX_STRING_LENGTH, L"%s\\%s1000.bin", EFI_CAPSULE_FILE_PATH, EFI_CAPSULE_FILE_NAME);

  Found = FALSE;
  for (Index = 0; Index < NumberOfHandles; Index++) {
    Status = gBS->HandleProtocol(
                    HandleBuffer[Index],
                    &gEfiSimpleFileSystemProtocolGuid,
                    (VOID **)&SimpleFileSystem
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }
    //
    // Get the system dir
    //
    Status = SimpleFileSystem->OpenVolume (
                                 SimpleFileSystem,
                                 &SysDir
                                 );
    if (!EFI_ERROR (Status)) {
      //
      // check the flash image exist
      //
      Status = SysDir->Open (SysDir,
                             &File,
                             ImagePath, // "EFI\\UpdateCapsule\\CapsuleUpdateFile1000.bin"
                             EFI_FILE_MODE_READ,
                             0
                             );
      if (!EFI_ERROR (Status)) {
        Found = TRUE;
        break;
      }
    }
  }

  if (!Found) {
    goto CloseSysDirAndExit;
  }
  //
  // Get file info to check file attribute
  //
  Status = File->GetInfo (
                   File,
                   &gEfiFileInfoGuid,
                   &BufferSize,
                   FileInfo
                   );

  if (Status == EFI_BUFFER_TOO_SMALL) {
    FileInfo = AllocatePool (BufferSize);
    if (FileInfo == NULL) {
      goto CloseFileAndExit;
    }
    Status = File->GetInfo (
                     File,
                     &gEfiFileInfoGuid,
                     &BufferSize,
                     FileInfo
                     );
  }

  if (EFI_ERROR (Status) || FileInfo == NULL) {
    goto CloseFileAndExit;
  }
  //
  // If the attribute of the file is read only, remove the attribute.
  // Because the file will be remove.
  // The file attributes will be valid the next time the file is opened with Open().
  //
  if ((FileInfo->Attribute & EFI_FILE_READ_ONLY) == EFI_FILE_READ_ONLY) {
    FileInfo->Attribute &= ~EFI_FILE_READ_ONLY;
    Status = File->SetInfo (
                     File,
                     &gEfiFileInfoGuid,
                     BufferSize,
                     FileInfo
                     );
    if (EFI_ERROR (Status)) {
      goto CloseFileAndExit;
    }
  }
  //
  // Close the file with READ mode
  // Re-Open file with R/W mode.
  //
  Status = File->Close (File);
  Status = SysDir->Open (SysDir,
                         &File,
                         ImagePath, // "EFI\\UpdateCapsule\\CapsuleUpdateFile1000.bin"
                         EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE,
                         0
                         );
  if (EFI_ERROR (Status)) {
    goto CloseSysDirAndExit;
  }

  FlashImageSize = (UINTN)FileInfo->FileSize;
  FlashImageBuffer = AllocateZeroPool (FlashImageSize);
  if (FlashImageBuffer == NULL) {
    goto DeletFileAndExit;
  }
  Status = File->Read (File, &FlashImageSize, FlashImageBuffer);
  if (!EFI_ERROR (Status)) {
    Status = VerifySignedImage (FlashImageBuffer + ((EFI_CAPSULE_HEADER*)FlashImageBuffer)->HeaderSize );
    if (EFI_ERROR (Status)) {
      goto CloseFileAndExit;
    }
    Status = RecoverySetImage ((VOID *) FlashImageBuffer, FlashImageSize);
  }

DeletFileAndExit:
  File->Delete (File);
  goto CloseSysDirAndExit;

CloseFileAndExit:
  File->Close (File);

CloseSysDirAndExit:
  if (SysDir != NULL){
    SysDir->Close (SysDir);
  }

  if (FileInfo != NULL) {
    FreePool (FileInfo);
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }
  return Status;
}
