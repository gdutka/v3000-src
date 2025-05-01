/** @file
  Firmware Management Protocol implementation for Capsule Processor

;******************************************************************************
;* Copyright (c) 2012 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "EcCapsuleDxe.h"

#define VERSION_STRING_NOT_SUPPORTED  L"VERSION STRING NOT SUPPORTED"

EFI_FIRMWARE_MANAGEMENT_PROTOCOL  mChipsetFirmwareManagement;
EFI_FIRMWARE_IMAGE_DESCRIPTOR     mDescriptor;
BOOLEAN                           mDescriptorPopulated = FALSE;

/**
  Populates the EFI_FIRMWARE_IMAGE_DESCRIPTOR structure in the private
  context structure.

  @param[in,out] Descriptor  Pointer to EFI_FIRMWARE_IMAGE_DESCRIPTOR structure for the
                             Firmware Management Protocol instance.

**/
VOID
PopulateDescriptor (
  OUT EFI_FIRMWARE_IMAGE_DESCRIPTOR          *Descriptor
  )
{
  EFI_STATUS                    Status;
  UINT32                        LastAttemptVersion;
  UINT32                        LastAttemptStatus;
  UINTN                         Size;
  UINT8                         FrontNum;
  UINT8                         MediumNum;
  UINT8                         BackNum;
  UINT32                        EcVersion;

  if (Descriptor == NULL) {
    return;
  }

  if (mDescriptorPopulated) {
    return;
  }

  CopyGuid (&Descriptor->ImageTypeId, PcdGetPtr (PcdWindowsEcFirmwareCapsuleGuid));
  Descriptor->ImageIndex                  = 1;
  Descriptor->ImageId                     = Descriptor->ImageIndex;
  Descriptor->ImageIdName                 = NULL;
  Descriptor->HardwareInstance            = 0;
  Descriptor->LowestSupportedImageVersion = PcdGet16 (PcdLowestSupportedEcVersion);
  Descriptor->AttributesSupported         = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE | IMAGE_ATTRIBUTE_RESET_REQUIRED | IMAGE_ATTRIBUTE_IN_USE);
  Descriptor->AttributesSetting           = (IMAGE_ATTRIBUTE_IMAGE_UPDATABLE | IMAGE_ATTRIBUTE_RESET_REQUIRED | IMAGE_ATTRIBUTE_IN_USE);
  Descriptor->Compatibilities             = 0;
  Descriptor->Size                        = 0;
  
  //
  // Get the version.
  //
  DEBUG_OEM_SVC ((DEBUG_INFO, "Base OemChipsetServices Call: OemSvcEcVersion \n"));
  Status = OemSvcEcVersion (&FrontNum, &MediumNum, &BackNum);
  DEBUG_OEM_SVC ((DEBUG_INFO, "Base OemChipsetServices OemSvcEcVersion  Status: %r\n", Status));

  EcVersion = 0;
  if (Status == EFI_MEDIA_CHANGED) {
    //
    // Update EC Versions
    //
    FrontNum = AsciiToHexadecimal (FrontNum);
    MediumNum = AsciiToHexadecimal (MediumNum);
    BackNum = AsciiToHexadecimal (BackNum);    
    EcVersion = ((EcVersion | (UINT32) FrontNum) << 8) + ((UINT32) MediumNum << 4) + (UINT32) BackNum;
  }

  Descriptor->Version = EcVersion;

  //
  // Free the current version name.  Shouldn't really happen but this populate
  // function could be called multiple times (to refresh).
  //
  if (Descriptor->VersionName != NULL) {
    FreePool (Descriptor->VersionName);
    Descriptor->VersionName = NULL;
  }

  Descriptor->VersionName = AllocateCopyPool (sizeof (VERSION_STRING_NOT_SUPPORTED), VERSION_STRING_NOT_SUPPORTED);

  //
  // Get last attemp version & status.
  //
  Size = sizeof (UINT32);
  Status = CommonGetVariable (
             ESRT_LAST_ATTEMPT_VERSION,
             &Descriptor->ImageTypeId,
             &Size,
             &LastAttemptVersion
             );
  if (Status == EFI_SUCCESS) {
    Descriptor->LastAttemptVersion = LastAttemptVersion;
  }
  Size = sizeof (UINT32);
  Status = CommonGetVariable (
             ESRT_LAST_ATTEMPT_STATUS,
             &Descriptor->ImageTypeId,
             &Size,
             &LastAttemptStatus
             );
  if (Status == EFI_SUCCESS) {
    Descriptor->LastAttemptStatus = LastAttemptStatus;
  }

  mDescriptorPopulated = TRUE;
}

/**
  Returns information about the current firmware image(s) of the device.

  This function allows a copy of the current firmware image to be created and saved.
  The saved copy could later been used, for example, in firmware image recovery or rollback.

  @param[in]      This               A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[in, out] ImageInfoSize      A pointer to the size, in bytes, of the ImageInfo buffer.
                                     On input, this is the size of the buffer allocated by the caller.
                                     On output, it is the size of the buffer returned by the firmware
                                     if the buffer was large enough, or the size of the buffer needed
                                     to contain the image(s) information if the buffer was too small.
  @param[in, out] ImageInfo          A pointer to the buffer in which firmware places the current image(s)
                                     information. The information is an array of EFI_FIRMWARE_IMAGE_DESCRIPTORs.
  @param[out]     DescriptorVersion  A pointer to the location in which firmware returns the version number
                                     associated with the EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]     DescriptorCount    A pointer to the location in which firmware returns the number of
                                     descriptors or firmware images within this device.
  @param[out]     DescriptorSize     A pointer to the location in which firmware returns the size, in bytes,
                                     of an individual EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]     PackageVersion     A version number that represents all the firmware images in the device.
                                     The format is vendor specific and new version must have a greater value
                                     than the old version. If PackageVersion is not supported, the value is
                                     0xFFFFFFFF. A value of 0xFFFFFFFE indicates that package version comparison
                                     is to be performed using PackageVersionName. A value of 0xFFFFFFFD indicates
                                     that package version update is in progress.
  @param[out]     PackageVersionName A pointer to a pointer to a null-terminated string representing the
                                     package version name. The buffer is allocated by this function with
                                     AllocatePool(), and it is the caller's responsibility to free it with a call
                                     to FreePool().

  @retval EFI_SUCCESS                The device was successfully updated with the new image.
  @retval EFI_BUFFER_TOO_SMALL       The ImageInfo buffer was too small. The current buffer size
                                     needed to hold the image(s) information is returned in ImageInfoSize.
  @retval EFI_INVALID_PARAMETER      ImageInfoSize is NULL.
  @retval EFI_DEVICE_ERROR           Valid information could not be returned. Possible corrupted image.

**/
EFI_STATUS
EFIAPI
ChipsetEcFWUGetImageInfo (
  IN     EFI_FIRMWARE_MANAGEMENT_PROTOCOL       *This,
  IN OUT UINTN                                  *ImageInfoSize,
  IN OUT EFI_FIRMWARE_IMAGE_DESCRIPTOR          *ImageInfo,
  OUT    UINT32                                 *DescriptorVersion,
  OUT    UINT8                                  *DescriptorCount,
  OUT    UINTN                                  *DescriptorSize,
  OUT    UINT32                                 *PackageVersion,
  OUT    CHAR16                                 **PackageVersionName
  )
{
  //
  // Check for valid pointer
  //
  if (ImageInfoSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check the buffer size
  // NOTE: Check this first so caller can get the necessary memory size it must allocate.
  //
  if (*ImageInfoSize < (sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR))) {
    *ImageInfoSize = sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR);
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Confirm that buffer isn't null
  //
  if ( (ImageInfo == NULL) || (DescriptorVersion == NULL) || (DescriptorCount == NULL) || (DescriptorSize == NULL)
       || (PackageVersion == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Set the size to whatever we need
  //
  *ImageInfoSize = sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR);

  //
  // Make sure the descriptor has already been loaded or refreshed
  //
  PopulateDescriptor (&mDescriptor);

  //
  // Copy the image descriptor
  //
  CopyMem (ImageInfo, &mDescriptor, sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR));

  *DescriptorVersion = EFI_FIRMWARE_IMAGE_DESCRIPTOR_VERSION;
  *DescriptorCount   = 1;
  *DescriptorSize    = sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR);
  //
  // means unsupported
  //
  *PackageVersion = 0xFFFFFFFF;

  //
  // Do not update PackageVersionName since it is not supported in this instance.
  //

  return EFI_SUCCESS;
}

/**
  Retrieves a copy of the current firmware image of the device.

  This function allows a copy of the current firmware image to be created and saved.
  The saved copy could later been used, for example, in firmware image recovery or rollback.

  @param[in]  This               A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[in]  ImageIndex         A unique number identifying the firmware image(s) within the device.
                                 The number is between 1 and DescriptorCount.
  @param[out] Image              Points to the buffer where the current image is copied to.
  @param[out] ImageSize          On entry, points to the size of the buffer pointed to by Image, in bytes.
                                 On return, points to the length of the image, in bytes.

  @retval EFI_SUCCESS            The device was successfully updated with the new image.
  @retval EFI_BUFFER_TOO_SMALL   The buffer specified by ImageSize is too small to hold the
                                 image. The current buffer size needed to hold the image is returned
                                 in ImageSize.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_NOT_FOUND          The current image is not copied to the buffer.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_SECURITY_VIOLATIO  The operation could not be performed due to an authentication failure.

**/
EFI_STATUS
EFIAPI
ChipsetEcFWUGetImage (
  IN     EFI_FIRMWARE_MANAGEMENT_PROTOCOL       *This,
  IN     UINT8                                  ImageIndex,
  IN OUT VOID                                   *Image,
  IN OUT UINTN                                  *ImageSize
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Returns information about the firmware package.

  This function returns package information.

  @param[in]  This                     A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[out] PackageVersion           A version number that represents all the firmware images in the device.
                                       The format is vendor specific and new version must have a greater value
                                       than the old version. If PackageVersion is not supported, the value is
                                       0xFFFFFFFF. A value of 0xFFFFFFFE indicates that package version
                                       comparison is to be performed using PackageVersionName. A value of
                                       0xFFFFFFFD indicates that package version update is in progress.
  @param[out] PackageVersionName       A pointer to a pointer to a null-terminated string representing
                                       the package version name. The buffer is allocated by this function with
                                       AllocatePool(), and it is the caller's responsibility to free it with a
                                       call to FreePool().
  @param[out] PackageVersionNameMaxLen The maximum length of package version name if device supports update of
                                       package version name. A value of 0 indicates the device does not support
                                       update of package version name. Length is the number of Unicode characters,
                                       including the terminating null character.
  @param[out] AttributesSupported      Package attributes that are supported by this device. See 'Package Attribute
                                       Definitions' for possible returned values of this parameter. A value of 1
                                       indicates the attribute is supported and the current setting value is
                                       indicated in AttributesSetting. A value of 0 indicates the attribute is not
                                       supported and the current setting value in AttributesSetting is meaningless.
  @param[out] AttributesSetting        Package attributes. See 'Package Attribute Definitions' for possible returned
                                       values of this parameter

  @retval EFI_SUCCESS                  The package information was successfully returned.
  @retval EFI_UNSUPPORTED              The operation is not supported.

**/
EFI_STATUS
EFIAPI
ChipsetEcFWUGetPackageInfo (
  IN     EFI_FIRMWARE_MANAGEMENT_PROTOCOL       *This,
  OUT    UINT32                                 *PackageVersion,
  OUT    CHAR16                                 **PackageVersionName,
  OUT    UINT32                                 *PackageVersionNameMaxLen,
  OUT    UINT64                                 *AttributesSupported,
  OUT    UINT64                                 *AttributesSetting
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Updates information about the firmware package.

  This function updates package information.
  This function returns EFI_UNSUPPORTED if the package information is not updatable.
  VendorCode enables vendor to implement vendor-specific package information update policy.
  Null if the caller did not specify this policy or use the default policy.

  @param[in]  This               A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[in]  Image              Points to the authentication image.
                                 Null if authentication is not required.
  @param[in]  ImageSize          Size of the authentication image in bytes.
                                 0 if authentication is not required.
  @param[in]  VendorCode         This enables vendor to implement vendor-specific firmware
                                 image update policy.
                                 Null indicates the caller did not specify this policy or use
                                 the default policy.
  @param[in]  PackageVersion     The new package version.
  @param[in]  PackageVersionName A pointer to the new null-terminated Unicode string representing
                                 the package version name.
                                 The string length is equal to or less than the value returned in
                                 PackageVersionNameMaxLen.

  @retval EFI_SUCCESS            The device was successfully updated with the new package
                                 information.
  @retval EFI_INVALID_PARAMETER  The PackageVersionName length is longer than the value
                                 returned in PackageVersionNameMaxLen.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_SECURITY_VIOLATIO  The operation could not be performed due to an authentication failure.

**/
EFI_STATUS
EFIAPI
ChipsetEcFWUSetPackageInfo (
  IN     EFI_FIRMWARE_MANAGEMENT_PROTOCOL       *This,
  IN     CONST VOID                             *Image,
  IN     UINTN                                  ImageSize,
  IN     CONST VOID                             *VendorCode,
  IN     UINT32                                 PackageVersion,
  IN     CONST CHAR16                           *PackageVersionName
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Convert ESRT status to FMP CheckImage result

  @param[in] Status                     The value of ESRT status

  @retval IMAGE_UPDATABLE_VALID         Indicates SetImage() will accept the new image and update
                                        the device with the new image.
  @retval IMAGE_UPDATABLE_INVALID       Indicates SetImage() will reject the new image. No additional
                                        information is provided for the rejection
  @retval IMAGE_UPDATABLE_INVALID_TYPE  Indicates SetImage() will reject the new image. The rejection
                                        is due to the new image is not a firmware image recognized for this device
  @retval IMAGE_UPDATABLE_INVALID_OLD   indicates SetImage() will reject the new image. The rejection is due to
                                        the new image version is older than the current firmware image version in
                                        the device
**/
STATIC
UINT32
ConvertEsrtStatusToCheckImageResult (
  IN     ESRT_STATUS    EsrtStatus
  )
{
  UINT32 CheckImageResult;

  switch (EsrtStatus) {
    case ESRT_SUCCESS:
      CheckImageResult = IMAGE_UPDATABLE_VALID;
      break;
    case ESRT_ERROR_INVALID_IMAGE_FORMAT:
      CheckImageResult = IMAGE_UPDATABLE_INVALID_TYPE;
      break;
    case ESRT_ERROR_INCORRECT_VERSION:
      CheckImageResult = IMAGE_UPDATABLE_INVALID_OLD;
      break;
    default:
      CheckImageResult = IMAGE_UPDATABLE_INVALID;
      break;
  }
  return CheckImageResult;
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
EFI_STATUS
ConvertEsrtStatusToEfiStatus (
  IN     ESRT_STATUS    EsrtStatus
  )
{
  EFI_STATUS Status;

  switch (EsrtStatus) {
    case ESRT_SUCCESS:
      Status = EFI_SUCCESS;
      break;
    case ESRT_ERROR_INSUFFICIENT_RESOURCES:
      Status = EFI_OUT_OF_RESOURCES;
      break;
    case ESRT_ERROR_INCORRECT_VERSION:
    case ESRT_ERROR_INVALID_IMAGE_FORMAT:
      Status = EFI_INVALID_PARAMETER;
      break;
    case ESRT_ERROR_AUTHENTICATION:
      Status = EFI_ACCESS_DENIED;
      break;
    default:
      Status = EFI_ABORTED;
      break;
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
//[-end-190611-IB16990046-add]//
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
  EFI_STATUS                  Status;
  CERTIFICATE_DATA            *CertData;
  UINTN                       CertSize;

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
  Checks if the firmware image is valid for the device.

  This function allows firmware update application to validate the firmware image without
  invoking the SetImage() first.

  @param[in]  This               A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[in]  ImageIndex         A unique number identifying the firmware image(s) within the device.
                                 The number is between 1 and DescriptorCount.
  @param[in]  Image              Points to the new image.
  @param[in]  ImageSize          Size of the new image in bytes.
  @param[out] ImageUpdatable     Indicates if the new image is valid for update. It also provides,
                                 if available, additional information if the image is invalid.

  @retval EFI_SUCCESS            The image was successfully checked.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_SECURITY_VIOLATION The operation could not be performed due to an authentication failure.

**/
EFI_STATUS
EFIAPI
ChipsetEcFWUCheckImage (
  IN     EFI_FIRMWARE_MANAGEMENT_PROTOCOL       *This,
  IN     UINT8                                  ImageIndex,
  IN     CONST VOID                             *Capsule,
  IN     UINTN                                  CapsuleSize,
  OUT    UINT32                                 *ImageUpdatable
  )
{
  EFI_STATUS                    Status;
  UINT32                        CheckImageResult;
  EFI_HANDLE                    CapsuleHandle;
  UINT32                        AttemptVersion;
  ESRT_STATUS                   AttemptStatus;

  //
  // Make sure the descriptor has already been loaded or refreshed
  //
  PopulateDescriptor (&mDescriptor);

  AttemptVersion = GetCapsuleFirmwareVersion ((EFI_CAPSULE_HEADER *)Capsule);
  if (AttemptVersion < mDescriptor.LowestSupportedImageVersion) {
    //
    // Record the ESRT status for out of resources
    //
    CheckImageResult = IMAGE_UPDATABLE_INVALID_OLD;
    //
    // Update capsule status onto corresponding variables
    //
    AttemptStatus = ESRT_ERROR_INCORRECT_VERSION;
    UpdateCapsuleStatus (&((EFI_CAPSULE_HEADER*)Capsule)->CapsuleGuid, AttemptVersion, AttemptStatus, EFI_UNSUPPORTED);
    goto Exit;
  }
  if (CapsuleSize > 0x10000000) {
    //
    // Record the ESRT status for out of resources
    //
    CheckImageResult = IMAGE_UPDATABLE_INVALID;
    //
    // Update capsule status onto corresponding variables
    //
    AttemptStatus = ESRT_ERROR_INSUFFICIENT_RESOURCES;
    UpdateCapsuleStatus (&((EFI_CAPSULE_HEADER*)Capsule)->CapsuleGuid, AttemptVersion, AttemptStatus, EFI_OUT_OF_RESOURCES);
    goto Exit;
  }
  //
  // Try to load image to verify the image format
  //
  CapsuleHandle = NULL;
  CapsuleSecurityCheck (TRUE);
  Status = gBS->LoadImage (
                  FALSE,
                  gImageHandle,
                  NULL,
                  (UINT8*)Capsule + ((EFI_CAPSULE_HEADER*)Capsule)->HeaderSize,
                  CapsuleSize - ((EFI_CAPSULE_HEADER*)Capsule)->HeaderSize,
                  (VOID **)&CapsuleHandle
                  );
  CapsuleSecurityCheck (FALSE);
  if (EFI_ERROR (Status)) {
    //
    // Record the ESRT status for out of resources
    //
    AttemptStatus    = ConvertEfiStatusToEsrtStatus (Status);
    CheckImageResult = IMAGE_UPDATABLE_INVALID;
    //
    // Update capsule status onto corresponding variables
    //
    UpdateCapsuleStatus (&((EFI_CAPSULE_HEADER*)Capsule)->CapsuleGuid, AttemptVersion, AttemptStatus, SecurityViolationStatusUpdate (Status));
    goto Exit;
  }
  if (CapsuleHandle) {
    gBS->UnloadImage (CapsuleHandle);
  }
  AttemptStatus = PreInstallationCheck ((EFI_CAPSULE_HEADER*)Capsule);
  CheckImageResult = ConvertEsrtStatusToCheckImageResult (AttemptStatus);
  if (AttemptStatus != ESRT_SUCCESS) {
    //
    // Update capsule status onto corresponding variables
    //
    UpdateCapsuleStatus (&((EFI_CAPSULE_HEADER*)Capsule)->CapsuleGuid, AttemptVersion, AttemptStatus, ConvertEsrtStatusToEfiStatus (AttemptStatus));
  }
Exit:
  if (ImageUpdatable) {
    *ImageUpdatable = CheckImageResult;
  }
  return EFI_SUCCESS;
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
  - Validate the data from VendorCode if not null.  Image validation must be performed before
    VendorCode data validation.  VendorCode data is ignored or considered invalid if image
    validation failed.  The function returns EFI_ABORTED if the data is invalid.

  VendorCode enables vendor to implement vendor-specific firmware image update policy.  Null if
  the caller did not specify the policy or use the default policy.  As an example, vendor can implement
  a policy to allow an option to force a firmware image update when the abort reason is due to the new
  firmware image version is older than the current firmware image version or bad image checksum.
  Sensitive operations such as those wiping the entire firmware image and render the device to be
  non-functional should be encoded in the image itself rather than passed with the VendorCode.
  AbortReason enables vendor to have the option to provide a more detailed description of the abort
  reason to the caller.

  @param[in]  This               A pointer to the EFI_FIRMWARE_MANAGEMENT_PROTOCOL instance.
  @param[in]  ImageIndex         A unique number identifying the firmware image(s) within the device.
                                 The number is between 1 and DescriptorCount.
  @param[in]  Image              Points to the new image.
  @param[in]  ImageSize          Size of the new image in bytes.
  @param[in]  VendorCode         This enables vendor to implement vendor-specific firmware image update policy.
                                 Null indicates the caller did not specify the policy or use the default policy.
  @param[in]  Progress           A function used by the driver to report the progress of the firmware update.
  @param[out] AbortReason        A pointer to a pointer to a null-terminated string providing more
                                 details for the aborted operation. The buffer is allocated by this function
                                 with AllocatePool(), and it is the caller's responsibility to free it with a
                                 call to FreePool().

  @retval EFI_SUCCESS            The device was successfully updated with the new image.
  @retval EFI_ABORTED            The operation is aborted.
  @retval EFI_INVALID_PARAMETER  The Image was NULL.
  @retval EFI_UNSUPPORTED        The operation is not supported.
  @retval EFI_SECURITY_VIOLATION The operation could not be performed due to an authentication failure.

**/
EFI_STATUS
EFIAPI
ChipsetEcFWUSetImage (
  IN     EFI_FIRMWARE_MANAGEMENT_PROTOCOL               *This,
  IN     UINT8                                          ImageIndex,
  IN     CONST VOID                                     *Capsule,
  IN     UINTN                                          CapsuleSize,
  IN     CONST VOID                                     *VendorCode,
  IN     EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,
  OUT    CHAR16                                         **AbortReason
  )
{
  EFI_STATUS            Status;
  UINT32                CheckImageResult;
  UINT32                AttemptVersion;
  ESRT_STATUS           AttemptStatus;
  EFI_HANDLE            CapsuleHandle;
  //
  // Check Image before actual action to ensure the capsule is qualified
  //
  Status = ChipsetEcFWUCheckImage (
             This,
             ImageIndex,
             Capsule,
             CapsuleSize,
             &CheckImageResult
             );
  if (CheckImageResult != IMAGE_UPDATABLE_VALID) {
    return EFI_UNSUPPORTED;
  }
  CapsuleHandle = NULL;
  Status = CapsuleSecurityCheck (TRUE);
  if (!EFI_ERROR (Status)) {
    Status = gBS->LoadImage (
                    FALSE,
                    gImageHandle,
                    NULL,
                    (UINT8*)Capsule + ((EFI_CAPSULE_HEADER*)Capsule)->HeaderSize,
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
  AttemptVersion = GetCapsuleFirmwareVersion ((EFI_CAPSULE_HEADER*)Capsule);
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
  //
  // Need repopulate after SetImage is called to
  // update LastAttemptVersion and LastAttemptStatus.
  //
  mDescriptorPopulated = FALSE;
  return Status;
}

/**
  FirmwareManagement RuntimeDxe module entry point


  @param ImageHandle            A handle for the image that is initializing this driver
  @param SystemTable            A pointer to the EFI system table

  @retval EFI_SUCCESS:          Module initialized successfully
  @retval Others                Module initialized unsuccessfully
**/
EFI_STATUS
EFIAPI
ChipsetEcFWUEntryPoint (
  IN     EFI_HANDLE             ImageHandle,
  IN     EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                    Status;

  //
  // Setup instance
  //
  mChipsetFirmwareManagement.GetImageInfo   = ChipsetEcFWUGetImageInfo;
  mChipsetFirmwareManagement.GetImage       = ChipsetEcFWUGetImage;
  mChipsetFirmwareManagement.SetImage       = ChipsetEcFWUSetImage;
  mChipsetFirmwareManagement.CheckImage     = ChipsetEcFWUCheckImage;
  mChipsetFirmwareManagement.GetPackageInfo = ChipsetEcFWUGetPackageInfo;
  mChipsetFirmwareManagement.SetPackageInfo = ChipsetEcFWUSetPackageInfo;

  //
  // Install FMP into image handle
  //
  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gEfiFirmwareManagementProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mChipsetFirmwareManagement
                  );
  return Status;
}