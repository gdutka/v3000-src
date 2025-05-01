/*****************************************************************************
 *
 * Copyright (C) 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/PrintLib.h>

#include "TestKey.h"

#define AMD_VARIABLE_PROTECTION_VARIABLE_NAME  L"AmdVariableProtection"

#define AMD_VARIABLE_PROTECTION_GUID \
  {0x408f573d, 0x65ee, 0x49ed, {0x8b, 0xc5, 0x5a, 0x32, 0xbb, 0xea, 0xe7, 0x45}}

// {408F573D-65EE-49ED-8BC5-5A32BBEAE745}
#define AMD_VARIABLE_PROTECTION_VARIABLE_GUID \
  { \
    0x408f573d, 0x65ee, 0x49ed, {0x8b, 0xc5, 0x5a, 0x32, 0xbb, 0xea, 0xe7, 0x45 } \
  }

EFI_GUID  gAmdVariableProtectionVariableGuid = AMD_VARIABLE_PROTECTION_VARIABLE_GUID;



//
// refer edk2\SecurityPkg\Library\SecureBootVariableLib\SecureBootVariableLib.c:(335) DeleteVariable
// DeletePayload is prepared by the following procedure
// 1. Variable name = AMD_VARIABLE_PROTECTION_VARIABLE_NAME
// 2. Variable guid = AMD_VARIABLE_PROTECTION_VARIABLE_GUID
// 3. Variable Attribute = EFI_VARIABLE_BOOTSERVICE_ACCESS |
//                         EFI_VARIABLE_RUNTIME_ACCESS |
//                         EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS
// 4. EFI_TIME = mMaxTimestamp (come from SecureBootVariableLib.c)
// 5. Variable size is 0, content is a NULL pointer
// 6. Signed with the TestKeyPem and TestCert (come from RsaPkcs7Test.c)
//
#if 0


// This time can be used when deleting variables, as it should be greater than any variable time.
EFI_TIME  mMaxTimestamp = {
  0xFFFF,     // Year
  0xFF,       // Month
  0xFF,       // Day
  0xFF,       // Hour
  0xFF,       // Minute
  0xFF,       // Second
  0x00,
  0x00000000, // Nanosecond
  0,
  0,
  0x00
};

#else
EFI_TIME  mMaxTimestamp = {
  0xFFFF,     // Year
  1,          // Month
  1,          // Day
  0,          // Hour
  0,          // Minute
  0,          // Second
  0,
  0,          // Nanosecond
  0,
  0,
  0
};

#endif


//
// Follow UEFI spec chapter 8.2
//   Using the EFI_VARIABLE_AUTHENTICATION_2 descriptor
//   digest = hash (Variable-Name, VendorGuid, Attributes, TimeStamp, DataNew_variable_content)
//
// CreatePayload is prepared by the following procedure
// 1. Variable name = AMD_VARIABLE_PROTECTION_VARIABLE_NAME
// 2. Variable guid = AMD_VARIABLE_PROTECTION_VARIABLE_GUID
// 3. Variable Attribute = EFI_VARIABLE_BOOTSERVICE_ACCESS |
//                         EFI_VARIABLE_RUNTIME_ACCESS |
//                         EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS
// 4. EFI_TIME = mDefaultPayloadTimestamp (come from SecureBootVariableLib.c)
// 5. Variable size is UINT8, value = 1
// 6. Signed with the TestKeyPem and TestCert (come from RsaPkcs7Test.c)
//
// EFI_VARIABLE_AUTHENTICATION_2
//


//
// This epoch time is the date that is used when creating SecureBoot default variables.
// NOTE: This is a placeholder date that doesn't correspond to anything else.
//
EFI_TIME  mDefaultPayloadTimestamp = {
  1970, // Year (1970)
  1,    // Month (Jan)
  1,    // Day (1)
  0,    // Hour
  0,    // Minute
  0,    // Second
  0,    // Pad1
  0,    // Nanosecond
  0,    // Timezone (Dummy value)
  0,    // Daylight (Dummy value)
  0     // Pad2
};

/**
  Get a PKCS#7 signedData as described in "PKCS #7: Cryptographic Message
  Syntax Standard, version 1.5". This interface is only intended to be used for
  application to perform PKCS#7 functionality validation.

  @param[in]  Payload          Pointer to the content to be signed.
  @param[in]  PayloadSize      Size of InData in bytes.
  @param[out] P7SignedData     Pointer to output PKCS#7 signedData. It's caller's
                               responsibility to free the buffer with FreePool().
  @param[out] P7SignedDataSize Size of SignedData in bytes.

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
GetPkcs7SignedData (
  IN  UINT8  *Payload,
  IN  UINTN  PayloadSize,
  OUT UINT8  **P7SignedData,
  OUT UINTN  *P7SignedDataSize
  )
{
  BOOLEAN  Status;
  UINT8    *SignCert;

  SignCert = NULL;

  //
  // Construct Signer Certificate from RAW data.
  //
  Status = X509ConstructCertificate (TestCert, sizeof (TestCert), (UINT8 **)&SignCert);
  if (!Status || (SignCert == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a : X509ConstructCertificate() failed: %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

  //
  // Create PKCS#7 signedData on Payload.
  // Note: Caller should release P7SignedData manually.
  //
  Status = Pkcs7Sign (
             TestKeyPem,
             sizeof (TestKeyPem),
             (CONST UINT8 *)PemPass,
             Payload,
             PayloadSize,
             SignCert,
             NULL,
             P7SignedData,
             P7SignedDataSize
             );
  if (!Status || (P7SignedDataSize == 0)) {
    DEBUG ((DEBUG_ERROR, "%a : Pkcs7Sign() failed: %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

  if (SignCert != NULL) {
    X509Free (SignCert);
  }

  return EFI_SUCCESS;
}

/**
  Create a time based data payload by concatenating the EFI_VARIABLE_AUTHENTICATION_2
  descriptor with the input data. Authentication is required in this function.

  @param[in]        SignDataSize   The size of sign Data buffer in bytes.
  @param[int]       SignData       Pointer to sign data buffer to be wrapped or
                                   pointer to NULL to wrap an empty payload.
  @param[in, out]   DataSize       On input, the size of Data buffer in bytes.
                                   On output, the size of data returned in Data
                                   buffer in bytes.
  @param[in, out]   Data           On input, Pointer to data buffer to be wrapped or
                                   pointer to NULL to wrap an empty payload.
                                   On output, Pointer to the new payload date buffer allocated from pool,
                                   it's caller's responsibility to free the memory when finish using it.
  @param[in]        Time           Pointer to time information to created time based payload.

  @retval EFI_SUCCESS              Create time based payload successfully.
  @retval EFI_OUT_OF_RESOURCES     There are not enough memory resources to create time based payload.
  @retval EFI_INVALID_PARAMETER    The parameter is invalid.
  @retval Others                   Unexpected error happens.

--*/
EFI_STATUS
EFIAPI
CreateTimeBasedPayloadWithSign (
  IN UINTN      SignDataSize,
  IN UINT8      *SignData,
  IN OUT UINTN  *DataSize,
  IN OUT UINT8  **Data,
  IN EFI_TIME   *Time
  )
{
  UINT8                          *NewData;
  UINT8                          *Payload;
  UINTN                          PayloadSize;
  EFI_VARIABLE_AUTHENTICATION_2  *DescriptorData;
  UINTN                          DescriptorSize;

  EFI_STATUS                     Status;
  UINT8                          *P7SignedData;
  UINTN                          P7SignedDataSize;

  Payload = NULL;

  P7SignedData = NULL;
  P7SignedDataSize = 0;

  if ((Data == NULL) || (DataSize == NULL) || (Time == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a : Invalid arg\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Generate sign information.
  //
  Status = GetPkcs7SignedData (SignData, SignDataSize, &P7SignedData, &P7SignedDataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a : Fail to sign data: %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Create EFI_VARIABLE_AUTHENTICATED_2 descriptor with certificate data.
  //
  Payload     = *Data;
  PayloadSize = *DataSize;

  DescriptorSize = OFFSET_OF (EFI_VARIABLE_AUTHENTICATION_2, AuthInfo) + OFFSET_OF (WIN_CERTIFICATE_UEFI_GUID, CertData) + P7SignedDataSize;
  NewData        = (UINT8 *)AllocateZeroPool (DescriptorSize + PayloadSize);
  if (NewData == NULL) {
    DEBUG ((DEBUG_ERROR, "%a : Out of resources.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  if ((Payload != NULL) && (PayloadSize != 0)) {
    CopyMem (NewData + DescriptorSize, Payload, PayloadSize);
  }

  DescriptorData = (EFI_VARIABLE_AUTHENTICATION_2 *)(NewData);

  CopyMem (&DescriptorData->TimeStamp, Time, sizeof (EFI_TIME));

  DescriptorData->AuthInfo.Hdr.dwLength         = OFFSET_OF (WIN_CERTIFICATE_UEFI_GUID, CertData) + (UINT32)P7SignedDataSize;
  DescriptorData->AuthInfo.Hdr.wRevision        = 0x0200;
  DescriptorData->AuthInfo.Hdr.wCertificateType = WIN_CERT_TYPE_EFI_GUID;
  CopyGuid (&DescriptorData->AuthInfo.CertType, &gEfiCertPkcs7Guid);

  CopyMem (&DescriptorData->AuthInfo.CertData, P7SignedData, P7SignedDataSize);

  if (Payload != NULL) {
    FreePool (Payload);
    Payload = NULL;
  }

  if (P7SignedData != NULL) {
    FreePool (P7SignedData);
  }

  *DataSize = DescriptorSize + PayloadSize;
  *Data     = NewData;
  return EFI_SUCCESS;
}

/**
  A helper function to take in a variable payload, wrap it in the
  proper authenticated variable structure, and install it in the
  EFI variable space.

  @param[in]  VariableName  The name of the key/database.
  @param[in]  VendorGuid    The namespace (ie. vendor GUID) of the variable
  @param[in]  DataSize      Size parameter for target variable.
  @param[in]  Data          Pointer to signature list formatted variable content.

  @retval EFI_SUCCESS              The enrollment for authenticated variable was successful.
  @retval EFI_OUT_OF_RESOURCES     There are not enough memory resources to create time based payload.
  @retval EFI_INVALID_PARAMETER    The parameter is invalid.
  @retval Others                   Unexpected error happens.

**/
EFI_STATUS
EFIAPI
UpdateAuthVariable (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid,
  IN UINTN     DataSize,
  IN VOID      *Data
  )
{
  VOID                           *Payload;
  UINTN                          PayloadSize;
  EFI_STATUS                     Status;

  EFI_TIME                       Time;

  UINT8                          *SignData;
  UINTN                          SignDataSize;
  UINT8                          *Buffer;
  UINTN                          Length;

  UINT32                         Attributes;
  UINTN                          Index;

  Payload = NULL;

  if ((VariableName == NULL) || (VendorGuid == 0)) {
    DEBUG ((DEBUG_ERROR, "%a : Input vendor variable invalid: %p and %p\n", __FUNCTION__, VariableName, VendorGuid));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Allocate memory for Payload buffer.
  //
  PayloadSize = DataSize;
  Payload     = AllocateZeroPool (DataSize);
  if (Payload == NULL) {
    return EFI_OUT_OF_RESOURCES;
  } else {
    CopyMem (Payload, Data, DataSize);
  }

  //
  // Get TimeStamp data.
  //
  if (Data > 0) {
    CopyMem (&Time, &mDefaultPayloadTimestamp, sizeof (EFI_TIME));
  } else {
    CopyMem (&Time, &mMaxTimestamp, sizeof (EFI_TIME));
  }

  //
  // Set auth variable attributes.
  //
  Attributes =  EFI_VARIABLE_BOOTSERVICE_ACCESS |
                EFI_VARIABLE_RUNTIME_ACCESS |
                EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS;

  //
  // Construct a serialization buffer of the values of the VariableName, VendorGuid and Attributes
  // parameters of the SetVariable() call and the TimeStamp component of the
  // EFI_VARIABLE_AUTHENTICATION_2 descriptor followed by the variable's new value
  // i.e. (VariableName, VendorGuid, Attributes, TimeStamp, Data)
  //
  SignDataSize = PayloadSize + sizeof (EFI_TIME) + sizeof (UINT32) +
                    sizeof (EFI_GUID) + StrSize (VariableName) - sizeof (CHAR16);

  SignData = (UINT8 *)AllocateZeroPool (SignDataSize);
  if (SignData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Buffer = SignData;
  Length = StrLen (VariableName) * sizeof (CHAR16);
  CopyMem (Buffer, VariableName, Length);
  Buffer += Length;

  Length = sizeof (EFI_GUID);
  CopyMem (Buffer, VendorGuid, Length);
  Buffer += Length;

  Length = sizeof (UINT32);
  CopyMem (Buffer, &Attributes, Length);
  Buffer += Length;

  Length = sizeof (EFI_TIME);
  CopyMem (Buffer, &Time, Length);
  Buffer += Length;

  CopyMem (Buffer, Payload, PayloadSize);

  //
  // Create a time based data payload by concatenating the EFI_VARIABLE_AUTHENTICATION_2
  // descriptor with the input data.
  //
  Status = CreateTimeBasedPayloadWithSign (SignDataSize, SignData, &PayloadSize, (UINT8 **)&Payload, &Time);
  if (EFI_ERROR (Status) || (Payload == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a : Fail to create time-based data payload: %r\n", __FUNCTION__, Status));
    Payload = NULL;
    Status  = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }


  if (DataSize > 0) {
    Print (L"UINT8 mCreatePayload[] = {\n  ");
  } else {
    Print (L"UINT8 mDeletePayload[] = {\n  ");
  }

  for (Index = 0; Index < PayloadSize; Index++) {

    if (Index != 0) {
      if ((Index % 16) != 0) {
        Print (L" ");
      } else {
        Print (L"\n  ");
      }
    }

    if (Index == (PayloadSize - 1)) {
      Print (L"0x%02X", ((UINT8 *)Payload)[Index]);
      break;
    }

    Print (L"0x%02X,", ((UINT8 *)Payload)[Index]);
  }

  Print (L"\n};\n\n");

Exit:

  if (Payload != NULL) {
    FreePool (Payload);
    Payload = NULL;
  }

  if (SignData != NULL) {
    FreePool (SignData);
  }

  return Status;
}


EFI_STATUS
EFIAPI
GenerateTimeBaseVariableUefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT8         AmdVarState;
  EFI_STATUS    Status;

  Status   = UpdateAuthVariable (
               AMD_VARIABLE_PROTECTION_VARIABLE_NAME,
               &gAmdVariableProtectionVariableGuid,
               0,
               (VOID *)NULL
               );

  AmdVarState = 1;
  Status   = UpdateAuthVariable (
               AMD_VARIABLE_PROTECTION_VARIABLE_NAME,
               &gAmdVariableProtectionVariableGuid,
               sizeof (UINT8),
               (VOID *)&AmdVarState
               );

  return EFI_SUCCESS;
}

