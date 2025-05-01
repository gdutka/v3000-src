/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The file operation functions for wireless connection driver.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#include "WifiConnectionMgrFileUtil.h"

CHAR16*  mDerPemEncodedSuffix[] = {
  L".cer",
  L".der",
  L".crt",
  L".pem",
  NULL
};

extern EFI_GUID  mEfiWifiMgrPrivateGuid;

/**
  This code checks if the FileSuffix is one of the possible DER/PEM-encoded certificate suffix.

  @param[in] FileSuffix     The suffix of the input certificate file

  @retval    TRUE           It's a DER/PEM-encoded certificate.
  @retval    FALSE          It's NOT a DER/PEM-encoded certificate.
**/
BOOLEAN
IsDerPemEncodeCertificate (
  IN CONST CHAR16         *FileSuffix
)
{
  UINTN     Index;
  for (Index = 0; mDerPemEncodedSuffix[Index] != NULL; Index++) {
    if (StrCmp (FileSuffix, mDerPemEncodedSuffix[Index]) == 0) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Worker function that prints an EFI_GUID into specified Buffer.

  @param[in]     Guid          Pointer to GUID to print.
  @param[in]     Buffer        Buffer to print Guid into.
  @param[in]     BufferSize    Size of Buffer.

  @retval    Number of characters printed.
**/
UINTN
GuidToString (
  IN  EFI_GUID  *Guid,
  IN  CHAR16    *Buffer,
  IN  UINTN     BufferSize
  )
{
  UINTN  Size;

  Size = UnicodeSPrint (
            Buffer,
            BufferSize,
            L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            (UINTN)Guid->Data1,
            (UINTN)Guid->Data2,
            (UINTN)Guid->Data3,
            (UINTN)Guid->Data4[0],
            (UINTN)Guid->Data4[1],
            (UINTN)Guid->Data4[2],
            (UINTN)Guid->Data4[3],
            (UINTN)Guid->Data4[4],
            (UINTN)Guid->Data4[5],
            (UINTN)Guid->Data4[6],
            (UINTN)Guid->Data4[7]
            );

  return Size;
}

/**
  Convert a String to Guid Value.

  @param[in]   Str        Specifies the String to be converted.
  @param[in]   StrLen     Number of Unicode Characters of String (exclusive \0)
  @param[out]  Guid       Return the result Guid value.

  @retval    EFI_SUCCESS           The operation is finished successfully.
  @retval    EFI_NOT_FOUND         Invalid string.

**/
EFI_STATUS
StringToGuid (
  IN   CHAR16           *Str,
  IN   UINTN            StrLen,
  OUT  EFI_GUID         *Guid
  )
{
  CHAR16             *PtrBuffer;
  CHAR16             *PtrPosition;
  UINT16             *Buffer;
  UINTN              Data;
  UINTN              Index;
  UINT16             Digits[3];

  Buffer = (CHAR16 *) AllocateZeroPool (sizeof (CHAR16) * (StrLen + 1));
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  StrCpyS (Buffer, (StrLen + 1), Str);

  //
  // Data1
  //
  PtrBuffer       = Buffer;
  PtrPosition     = PtrBuffer;
  while (*PtrBuffer != L'\0') {
    if (*PtrBuffer == L'-') {
      break;
    }
    PtrBuffer++;
  }
  if (*PtrBuffer == L'\0') {
    FreePool (Buffer);
    return EFI_NOT_FOUND;
  }

  *PtrBuffer      = L'\0';
  Data            = StrHexToUintn (PtrPosition);
  Guid->Data1     = (UINT32)Data;

  //
  // Data2
  //
  PtrBuffer++;
  PtrPosition     = PtrBuffer;
  while (*PtrBuffer != L'\0') {
    if (*PtrBuffer == L'-') {
      break;
    }
    PtrBuffer++;
  }
  if (*PtrBuffer == L'\0') {
    FreePool (Buffer);
    return EFI_NOT_FOUND;
  }
  *PtrBuffer      = L'\0';
  Data            = StrHexToUintn (PtrPosition);
  Guid->Data2     = (UINT16)Data;

  //
  // Data3
  //
  PtrBuffer++;
  PtrPosition     = PtrBuffer;
  while (*PtrBuffer != L'\0') {
    if (*PtrBuffer == L'-') {
      break;
    }
    PtrBuffer++;
  }
  if (*PtrBuffer == L'\0') {
    FreePool (Buffer);
    return EFI_NOT_FOUND;
  }
  *PtrBuffer      = L'\0';
  Data            = StrHexToUintn (PtrPosition);
  Guid->Data3     = (UINT16)Data;

  //
  // Data4[0..1]
  //
  for ( Index = 0 ; Index < 2 ; Index++) {
    PtrBuffer++;
    if ((*PtrBuffer == L'\0') || ( *(PtrBuffer + 1) == L'\0')) {
      FreePool (Buffer);
      return EFI_NOT_FOUND;
    }
    Digits[0]     = *PtrBuffer;
    PtrBuffer++;
    Digits[1]     = *PtrBuffer;
    Digits[2]     = L'\0';
    Data          = StrHexToUintn (Digits);
    Guid->Data4[Index] = (UINT8)Data;
  }

  //
  // skip the '-'
  //
  PtrBuffer++;
  if ((*PtrBuffer != L'-' ) || ( *PtrBuffer == L'\0')) {
    return EFI_NOT_FOUND;
  }

  //
  // Data4[2..7]
  //
  for ( ; Index < 8; Index++) {
    PtrBuffer++;
    if ((*PtrBuffer == L'\0') || ( *(PtrBuffer + 1) == L'\0')) {
      FreePool (Buffer);
      return EFI_NOT_FOUND;
    }
    Digits[0]     = *PtrBuffer;
    PtrBuffer++;
    Digits[1]     = *PtrBuffer;
    Digits[2]     = L'\0';
    Data          = StrHexToUintn (Digits);
    Guid->Data4[Index] = (UINT8)Data;
  }

  FreePool (Buffer);

  return EFI_SUCCESS;
}

/**
  Close an open file handle.

  @param[in] FileHandle           The file handle to close.
**/
VOID
CloseFile (
  IN EFI_FILE_HANDLE   FileHandle
  )
{
  if (FileHandle != NULL) {
    FileHandle->Close (FileHandle);
  }
}

/**
  Read file content into BufferPtr, the size of the allocate buffer
  is *FileSize plus AddtionAllocateSize.

  @param[in]       FileHandle            The file to be read.
  @param[in, out]  BufferPtr             Pointers to the pointer of allocated buffer.
  @param[out]      FileSize              Size of input file
  @param[in]       AddtionAllocateSize   Addtion size the buffer need to be allocated.
                                         In case the buffer need to contain others besides the file content.

  @retval   EFI_SUCCESS                  The file was read into the buffer.
  @retval   EFI_INVALID_PARAMETER        A parameter was invalid.
  @retval   EFI_OUT_OF_RESOURCES         A memory allocation failed.
  @retval   others                       Unexpected error.
**/
EFI_STATUS
ReadFileContent (
  IN      EFI_FILE_HANDLE           FileHandle,
  IN OUT  VOID                      **BufferPtr,
     OUT  UINTN                     *FileSize,
  IN      UINTN                     AddtionAllocateSize
  )
{
  UINTN      BufferSize;
  UINT64     SourceFileSize;
  VOID       *Buffer;
  EFI_STATUS Status;

  if ((FileHandle == NULL) || (FileSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Buffer = NULL;

  //
  // Get the file size
  //
  Status = FileHandle->SetPosition (FileHandle, (UINT64) -1);
  if (EFI_ERROR (Status)) {
    goto ON_EXIT;
  }

  Status = FileHandle->GetPosition (FileHandle, &SourceFileSize);
  if (EFI_ERROR (Status)) {
    goto ON_EXIT;
  }

  Status = FileHandle->SetPosition (FileHandle, 0);
  if (EFI_ERROR (Status)) {
    goto ON_EXIT;
  }

  BufferSize = (UINTN) SourceFileSize + AddtionAllocateSize;
  Buffer =  AllocateZeroPool(BufferSize);
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  BufferSize = (UINTN) SourceFileSize;
  *FileSize  = BufferSize;

  Status = FileHandle->Read (FileHandle, &BufferSize, Buffer);
  if (EFI_ERROR (Status) || BufferSize != *FileSize) {
    FreePool (Buffer);
    Buffer = NULL;
    Status  = EFI_BAD_BUFFER_SIZE;
    goto ON_EXIT;
  }

ON_EXIT:

  *BufferPtr = Buffer;
  return Status;
}

/**
  This function will open a file or directory referenced by DevicePath.

  This function opens a file with the open mode according to the file path. The
  Attributes is valid only for EFI_FILE_MODE_CREATE.

  @param[in, out]  FilePath        On input, the device path to the file.
                                   On output, the remaining device path.
  @param[out]      FileHandle      Pointer to the file handle.
  @param[in]       OpenMode        The mode to open the file with.
  @param[in]       Attributes      The file's file attributes.

  @retval EFI_SUCCESS              The information was set.
  @retval EFI_INVALID_PARAMETER    One of the parameters has an invalid value.
  @retval EFI_UNSUPPORTED          Could not open the file path.
  @retval EFI_NOT_FOUND            The specified file could not be found on the
                                   device or the file system could not be found on
                                   the device.
  @retval EFI_NO_MEDIA             The device has no medium.
  @retval EFI_MEDIA_CHANGED        The device has a different medium in it or the
                                   medium is no longer supported.
  @retval EFI_DEVICE_ERROR         The device reported an error.
  @retval EFI_VOLUME_CORRUPTED     The file system structures are corrupted.
  @retval EFI_WRITE_PROTECTED      The file or medium is write protected.
  @retval EFI_ACCESS_DENIED        The file was opened read only.
  @retval EFI_OUT_OF_RESOURCES     Not enough resources were available to open the
                                   file.
  @retval EFI_VOLUME_FULL          The volume is full.
**/
EFI_STATUS
EFIAPI
OpenFileByDevicePath (
  IN OUT EFI_DEVICE_PATH_PROTOCOL     **FilePath,
  OUT EFI_FILE_HANDLE                 *FileHandle,
  IN UINT64                           OpenMode,
  IN UINT64                           Attributes
  )
{
  EFI_STATUS                      Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *EfiSimpleFileSystemProtocol;
  EFI_FILE_PROTOCOL               *Handle1;
  EFI_FILE_PROTOCOL               *Handle2;
  EFI_HANDLE                      DeviceHandle;

  if ((FilePath == NULL || FileHandle == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateDevicePath (
                  &gEfiSimpleFileSystemProtocolGuid,
                  FilePath,
                  &DeviceHandle
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->OpenProtocol(
                  DeviceHandle,
                  &gEfiSimpleFileSystemProtocolGuid,
                  (VOID**)&EfiSimpleFileSystemProtocol,
                  gImageHandle,
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = EfiSimpleFileSystemProtocol->OpenVolume(EfiSimpleFileSystemProtocol, &Handle1);
  if (EFI_ERROR (Status)) {
    FileHandle = NULL;
    return Status;
  }

  //
  // go down directories one node at a time.
  //
  while (!IsDevicePathEnd (*FilePath)) {
    //
    // For file system access each node should be a file path component
    //
    if (DevicePathType    (*FilePath) != MEDIA_DEVICE_PATH ||
        DevicePathSubType (*FilePath) != MEDIA_FILEPATH_DP
       ) {
      FileHandle = NULL;
      return (EFI_INVALID_PARAMETER);
    }
    //
    // Open this file path node
    //
    Handle2  = Handle1;
    Handle1 = NULL;

    //
    // Try to test opening an existing file
    //
    Status = Handle2->Open (
                        Handle2,
                        &Handle1,
                        ((FILEPATH_DEVICE_PATH*)*FilePath)->PathName,
                        OpenMode &~EFI_FILE_MODE_CREATE,
                        0
                        );

    //
    // see if the error was that it needs to be created
    //
    if ((EFI_ERROR (Status)) && (OpenMode != (OpenMode &~EFI_FILE_MODE_CREATE))) {
      Status = Handle2->Open (
                          Handle2,
                          &Handle1,
                          ((FILEPATH_DEVICE_PATH*)*FilePath)->PathName,
                          OpenMode,
                          Attributes
                          );
    }
    //
    // Close the last node
    //
    Handle2->Close (Handle2);

    if (EFI_ERROR(Status)) {
      return (Status);
    }

    //
    // Get the next node
    //
    *FilePath = NextDevicePathNode (*FilePath);
  }

  //
  // This is a weak spot since if the undefined SHELL_FILE_HANDLE format changes this must change also!
  //
  *FileHandle = (VOID*)Handle1;
  return EFI_SUCCESS;
}

/**
  This function converts an input device structure to a Unicode string.

  @param[in] DevPath                  A pointer to the device path structure.

  @return A new allocated Unicode string that represents the device path.

**/
CHAR16 *
EFIAPI
DevicePathToStr (
  IN EFI_DEVICE_PATH_PROTOCOL     *DevPath
  )
{
  return ConvertDevicePathToText (
           DevPath,
           FALSE,
           TRUE
           );
}

/**
  Extract filename from device path. The returned buffer is allocated using AllocateCopyPool.
  The caller is responsible for freeing the allocated buffer using FreePool(). If return NULL
  means not enough memory resource.

  @param DevicePath       Device path.

  @retval NULL            Not enough memory resourece for AllocateCopyPool.
  @retval Other           A new allocated string that represents the file name.
**/
CHAR16 *
ExtractFileNameFromDevicePath (
  IN   EFI_DEVICE_PATH_PROTOCOL    *DevicePath
  )
{
  CHAR16          *String;
  CHAR16          *MatchString;
  CHAR16          *LastMatch;
  CHAR16          *FileName;
  UINTN           Length;

  ASSERT(DevicePath != NULL);

  String = DevicePathToStr(DevicePath);
  if (String == NULL) {
    return NULL;
  }
  MatchString = String;
  LastMatch   = String;
  FileName    = NULL;

  while(MatchString != NULL){
    LastMatch   = MatchString + 1;
    MatchString = StrStr(LastMatch,L"\\");
  }

  Length = StrLen(LastMatch);
  FileName = AllocateCopyPool ((Length + 1) * sizeof(CHAR16), LastMatch);
  if (FileName != NULL) {
    *(FileName + Length) = 0;
  }

  FreePool(String);

  return FileName;
}

/**
  Get the expected X509 cert from variable.

  @param[in]    VariableName        The variable name of the vendor's signature database.
  @param[in]    ExpectCertGuid      The GUID of expected cert.
  @param[out]   CertSize            The size of the cert returned, can be set to NULL.

  @return the expected X509 cert or NULL.
**/
EFI_SIGNATURE_DATA*
GetX509FromVariable (
  IN  CHAR16                  *VariableName,
  IN  EFI_GUID                *ExpectCertGuid,
  OUT UINTN                   *CertSize
  )
{
  EFI_STATUS                  Status;
  UINT32                      Index;
  UINTN                       CertCount;
  UINTN                       DataSize;
  UINT8                       *Data;
  EFI_SIGNATURE_LIST          *CertList;
  EFI_SIGNATURE_DATA          *Cert;
  UINT32                      ItemDataSize;
  CHAR16                      *GuidStr;

  Data     = NULL;
  CertList = NULL;
  Cert     = NULL;
  GuidStr = (CHAR16 *) AllocateZeroPool(sizeof (CHAR16) * 37);

  //
  // Read Variable.
  //
  DataSize = 0;
  Status = GetVariable2(VariableName, &mEfiWifiMgrPrivateGuid, (VOID **)&Data, &DataSize);
  if (EFI_ERROR (Status)) {
    return NULL;
  }
  ASSERT (Data != NULL);

  //
  // Enumerate all data.
  //
  ItemDataSize = (UINT32) DataSize;
  CertList = (EFI_SIGNATURE_LIST *) Data;

  while ((ItemDataSize > 0) && (ItemDataSize >= CertList->SignatureListSize)) {

    if (!CompareGuid (&CertList->SignatureType, &gEfiCertX509Guid)) {
      //
      // The signature type is not supported in current implementation.
      //
      ItemDataSize -= CertList->SignatureListSize;
      CertList = (EFI_SIGNATURE_LIST *) ((UINT8 *) CertList + CertList->SignatureListSize);
      continue;
    }

    if (CertList->SignatureSize == 0) {
      return NULL;
    }
    CertCount  = (CertList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) 
                    - CertList->SignatureHeaderSize) / CertList->SignatureSize;
    if (CertSize != NULL) {
      *CertSize = CertList->SignatureSize + 1 - sizeof(EFI_SIGNATURE_DATA);
    }

    for (Index = 0; Index < CertCount; Index++) {

      Cert = (EFI_SIGNATURE_DATA *) ((UINT8 *) CertList
                                              + sizeof (EFI_SIGNATURE_LIST)
                                              + CertList->SignatureHeaderSize
                                              + Index * CertList->SignatureSize);

      GuidToString(&Cert->SignatureOwner, GuidStr, sizeof (CHAR16) * 37);

      if (CompareGuid(&Cert->SignatureOwner, ExpectCertGuid)) {

        return Cert;
      }
    }

    ItemDataSize -= CertList->SignatureListSize;
    CertList = (EFI_SIGNATURE_LIST *) ((UINT8 *) CertList + CertList->SignatureListSize);
  }

  if (Data != NULL) {
    FreePool (Data);
  }

  if (CertSize != NULL) {
    *CertSize = 0;
  }
  return NULL;
}

/**
  Get the expected cert from cert database.

  @param[in]    VariableName        The variable name of the vendor's signature database.
  @param[in]    ExpectCertGuid      The GUID of expected cert.
  @param[out]   CertSize            The size of the cert returned, can be set to NULL.

  @return the expected cert or NULL.
**/
EFI_SIGNATURE_DATA*
GetFromCertDatabase (
  IN  CHAR16                   *VariableName,
  IN  EFI_GUID                 *ExpectCertGuid,
  OUT UINTN                    *CertSize
  )
{
  if (VariableName == NULL || ExpectCertGuid == NULL) {
    return NULL;
  }

  return GetX509FromVariable (VariableName, ExpectCertGuid, CertSize);
}

/**
  Enroll a new X509 certificate into Variable.

  @param[in] FileContext     A file hander points to the X509 cert.
  @param[in] CertGuid        The GUID for this X509 cert.
  @param[in] VariableName    Variable name of CA database.

  @retval   EFI_SUCCESS            New X509 is enrolled successfully.
  @retval   EFI_OUT_OF_RESOURCES   Could not allocate needed resources.
**/
EFI_STATUS
EnrollX509toVariable (
  IN  WIFI_MGR_FILE_CONTEXT          *FileContext,
  IN  EFI_GUID                       *CertGuid,
  IN  CHAR16                         *VariableName
  )
{
  EFI_STATUS                        Status;
  UINTN                             X509DataSize;
  VOID                              *X509Data;
  EFI_SIGNATURE_LIST                *Cert;
  EFI_SIGNATURE_DATA                *CertData;
  VOID                              *Data;
  UINTN                             DataSize;
  UINTN                             SigDataSize;
  UINT32                            Attr;

  X509DataSize  = 0;
  SigDataSize   = 0;
  DataSize      = 0;
  X509Data      = NULL;
  Cert          = NULL;
  CertData      = NULL;
  Data          = NULL;

  Status = ReadFileContent (
             FileContext->FHandle,
             &X509Data,
             &X509DataSize,
             0
             );
  if (EFI_ERROR (Status)) {
    goto ON_EXIT;
  }
  ASSERT (X509Data != NULL);

  SigDataSize = sizeof(EFI_SIGNATURE_LIST) + sizeof(EFI_SIGNATURE_DATA) - 1 + X509DataSize;

  Data = AllocateZeroPool (SigDataSize);
  if (Data == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ON_EXIT;
  }

  //
  // Fill Certificate Database parameters.
  //
  Cert = (EFI_SIGNATURE_LIST*) Data;
  Cert->SignatureListSize   = (UINT32) SigDataSize;
  Cert->SignatureHeaderSize = 0;
  Cert->SignatureSize       = (UINT32) (sizeof(EFI_SIGNATURE_DATA) - 1 + X509DataSize);
  CopyGuid (&Cert->SignatureType, &gEfiCertX509Guid);

  CertData = (EFI_SIGNATURE_DATA*) ((UINT8* ) Cert + sizeof (EFI_SIGNATURE_LIST));
  CopyGuid (&CertData->SignatureOwner, CertGuid);
  CopyMem (CertData->SignatureData, X509Data, X509DataSize);

  //
  // Check if signature database entry has been already existed.
  // If true, use EFI_VARIABLE_APPEND_WRITE attribute to append the
  // new signature data to original variable
  //
  Attr = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;

  Status = gRT->GetVariable(
                  VariableName,
                  &mEfiWifiMgrPrivateGuid,
                  NULL,
                  &DataSize,
                  NULL
                  );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    Attr |= EFI_VARIABLE_APPEND_WRITE;
  } else if (Status != EFI_NOT_FOUND) {
    goto ON_EXIT;
  }

  Status = gRT->SetVariable(
                  VariableName,
                  &mEfiWifiMgrPrivateGuid,
                  Attr,
                  SigDataSize,
                  Data
                  );
  if (EFI_ERROR (Status)) {
    goto ON_EXIT;
  }

ON_EXIT:

  CloseFile (FileContext->FHandle);
  FileContext->FHandle = NULL;

  if (Data != NULL) {
    FreePool (Data);
  }

  if (X509Data != NULL) {
    FreePool (X509Data);
  }

  return Status;
}

/**
  Enroll Cert into cert database.

  @param[in] FileContext     A file hander points to the cert.
  @param[in] CertGuid        The GUID for this cert.
  @param[in] VariableName    Variable name of CA database.

  @retval   EFI_SUCCESS            New Cert enrolled successfully.
  @retval   EFI_INVALID_PARAMETER  The parameter is invalid.
  @retval   EFI_UNSUPPORTED        The Cert file is unsupported type.
  @retval   Others                 Fail to enroll Cert data.

**/
EFI_STATUS
EnrollToCertDatabase (
  IN  WIFI_MGR_FILE_CONTEXT          *FileContext,
  IN  EFI_GUID                       *CertGuid,
  IN  CHAR16                         *VariableName
  )
{
  UINT16*      FilePostFix;
  UINTN        NameLength;

  if (FileContext->FileName == NULL || FileContext->FHandle == NULL ||
    CertGuid == NULL) {

    return EFI_INVALID_PARAMETER;
  }


  //
  // Parse the file's postfix.
  //
  NameLength = StrLen (FileContext->FileName);
  if (NameLength <= 4) {
    return EFI_INVALID_PARAMETER;
  }

  FilePostFix = FileContext->FileName + NameLength - 4;

  if (IsDerPemEncodeCertificate (FilePostFix)) {
    //
    // Supports DER-encoded X509 certificate.
    //
    return EnrollX509toVariable (FileContext, CertGuid, VariableName);
  }

  return EFI_UNSUPPORTED;
}

/**
  Update the form base on the selected file.

  @param[in]  Private             The pointer to the global private data structure.
  @param[in]  FilePath            Point to the file path.
  @param[in]  FormId              The form needs to display.

  @retval TRUE   Exit caller function.
  @retval FALSE  Not exit caller function.

**/
BOOLEAN
UpdatePage(
  IN  WIFI_MGR_PRIVATE_DATA       *Private,
  IN  EFI_DEVICE_PATH_PROTOCOL    *FilePath,
  IN  EFI_FORM_ID                 FormId
  )
{
  CHAR16           *FileName;
  EFI_STATUS       Status;

  FileName = NULL;

  if (FilePath != NULL) {
    FileName = ExtractFileNameFromDevicePath(FilePath);
  }
  if (FileName == NULL) {
    //
    // FileName = NULL has two case:
    // 1. FilePath == NULL, not select file.
    // 2. FilePath != NULL, but ExtractFileNameFromDevicePath return NULL not enough memory resource.
    // In these two case, no need to update the form, and exit the caller function.
    //
    return TRUE;
  }

  //
  // Close the previous file handle before open a new one.
  //
  if (Private->FileContext->FHandle != NULL) {
    CloseFile (Private->FileContext->FHandle);
  }

  Status = OpenFileByDevicePath (
             &FilePath,
             &Private->FileContext->FHandle,
             EFI_FILE_MODE_READ,
             0
             );
  if (EFI_ERROR (Status)) {
    if (FormId == FORMID_ENROLL_CERT) {
      HiiSetString (Private->RegisteredHandle,
        STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME), L"", NULL);
    } else if (FormId == FORMID_ENROLL_PRIVATE_KEY){
      HiiSetString (Private->RegisteredHandle,
        STRING_TOKEN (STR_EAP_ENROLLED_PRIVATE_KEY_NAME), L"", NULL);
    }
  } else {
    Private->FileContext->FileName = FileName;
    if (FormId == FORMID_ENROLL_CERT) {
      HiiSetString (Private->RegisteredHandle,
        STRING_TOKEN (STR_EAP_ENROLLED_CERT_NAME), FileName, NULL);
    } else if (FormId == FORMID_ENROLL_PRIVATE_KEY){
      HiiSetString (Private->RegisteredHandle,
        STRING_TOKEN (STR_EAP_ENROLLED_PRIVATE_KEY_NAME), FileName, NULL);
    }
  }

  return TRUE;
}

/**
  Update the CA form base on the input file path info.

  @param[in]  Private             The pointer to the global private data structure.
  @param[in]  FilePath            Point to the file path.

  @retval TRUE   Exit caller function.
  @retval FALSE  Not exit caller function.
**/
BOOLEAN
UpdateCAFromFile (
  IN  WIFI_MGR_PRIVATE_DATA       *Private,
  IN  EFI_DEVICE_PATH_PROTOCOL    *FilePath
  )
{
  return UpdatePage(Private, FilePath, FORMID_ENROLL_CERT);
}

/**
  Update the Private Key form base on the input file path info.

  @param[in]  Private             The pointer to the global private data structure.
  @param[in]  FilePath            Point to the file path.

  @retval TRUE   Exit caller function.
  @retval FALSE  Not exit caller function.
**/
BOOLEAN
UpdatePrivateKeyFromFile (
  IN  WIFI_MGR_PRIVATE_DATA       *Private,
  IN  EFI_DEVICE_PATH_PROTOCOL    *FilePath
  )
{
  return UpdatePage(Private, FilePath, FORMID_ENROLL_PRIVATE_KEY);
}

