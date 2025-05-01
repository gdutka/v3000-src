/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
  The common variable operation routines.

  VariableServiceGetVariable() and VariableServiceSetVariable() are external API
  to receive datasize and data buffer. The size should be checked carefully.

  VariableServiceSetVariable() should also check authenticate data to avoid buffer overflow,
  integer overflow. It should also check attribute to avoid authentication bypass.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#include "WifiConnectionMgrDxe.h"

/**

  This function dump raw data.

  @param[in]  Data  raw data
  @param[in]  Size  raw data size

**/
VOID
InternalDumpData (
  IN UINT8  *Data,
  IN UINTN  Size
  )
{
  UINTN  Index;
  for (Index = 0; Index < Size; Index++) {
    DEBUG ((DEBUG_INFO, "%02x", (UINTN)Data[Index]));
  }
}

/**

  Calculate the real variable size.

  @param[in]  Variable        The protected variable to be processed.
  @param[in]  VariableSize    The protected variable size.

**/
UINTN
RealDataSizeOfVariable (
  IN  VOID      *Variable,
  IN  UINTN     VariableSize
  )
{
  UINTN                       RealDataSize;
  VARIABLE_KEY_DATA_HEADER    *KeyDataHeader;

  RealDataSize = VariableSize;

  KeyDataHeader = (VARIABLE_KEY_DATA_HEADER *)((UINT8 *)Variable + sizeof(VARIABLE_KEY_HASH_HEADER));
  RealDataSize  = KeyDataHeader->KeyPlainDataSize;

  return RealDataSize;
}

/**

  This code finds variable in storage blocks (Volatile or Non-Volatile).

  Caution: This function may receive untrusted input.
  This function may be invoked in SMM mode, and datasize is external input.
  This function will do basic validation, before parse the data.

  @param[in] VariableName               Name of Variable to be found.
  @param[in] VendorGuid                 Variable vendor GUID.
  @param[out] Attributes                Attribute value of the variable found.
  @param[in,out] AttributesEx           AttributeEx value of the variable found.
  @param[in,out] DataSize               Size of Data found. If size is less than the
                                          data, this value contains the required size.
  @param[out] Data                      The buffer to return the contents of the variable. May be NULL
                                          with a zero DataSize in order to determine the size buffer needed.

  @return EFI_INVALID_PARAMETER         Invalid parameter.
  @return EFI_SUCCESS                   Find the specified variable.
  @return EFI_NOT_FOUND                 Not found.
  @return EFI_BUFFER_TO_SMALL           DataSize is too small for the result.

**/
EFI_STATUS
EFIAPI
VariableServiceGetVariableEx (
  IN      CHAR16            *VariableName,
  IN      EFI_GUID          *VendorGuid,
  OUT     UINT32            *Attributes OPTIONAL,
  IN OUT  UINT8             *AttributesEx OPTIONAL,
  IN OUT  UINTN             *DataSize,
  OUT     VOID              *Data OPTIONAL
  )
{
  UINTN                         RawDataSize;
  EFI_STATUS                    Status;
  UINTN                         EncRawDataSize;
  EDKII_VARIABLE_KEY_DATA       *KeyData;
  VARIABLE_KEY_HASH_HEADER      VariableKeyHashHeader;
  VARIABLE_KEY_HASH_HEADER      *OldVariableKeyHashHeader;
  BOOLEAN                       Result;
  VOID                          *Variable;
  UINTN                         VariableSize;
  VOID                          *Buffer;

  VariableSize = 0;
  Variable     = NULL;

  Status = GetVariable2 (
             VariableName,
             VendorGuid,
             (VOID **) &Variable,
             &VariableSize
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ASSERT (Variable != NULL);

  //
  // Update Data and DataSize
  //
  KeyData     = (EDKII_VARIABLE_KEY_DATA *) Data;

  RawDataSize = RealDataSizeOfVariable (Variable, VariableSize);

  if (*DataSize < RawDataSize) {
    *DataSize = RawDataSize;
    FreePool (Variable);
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Update Data and DataSize
  //
  VariableKeyHashHeader.KeyHashType = HASH_TYPE_SHA256;
  VariableKeyHashHeader.KeyHashHeadSize = sizeof(VARIABLE_KEY_HASH_HEADER);

  OldVariableKeyHashHeader = (VARIABLE_KEY_HASH_HEADER *) Variable;
  DEBUG((DEBUG_INFO, "Old SALT - "));
  InternalDumpData(OldVariableKeyHashHeader->KeySalt, sizeof(OldVariableKeyHashHeader->KeySalt));
  DEBUG((DEBUG_INFO, "\n"));
  CopyMem(VariableKeyHashHeader.KeySalt, OldVariableKeyHashHeader->KeySalt, sizeof(VariableKeyHashHeader.KeySalt));

  DEBUG((DEBUG_INFO, "KeyData size:%d \n", KeyData->KeySize));

  Result = KeyLibGenerateHash (
             HASH_TYPE_SHA256,
             KeyData + 1,
             KeyData->KeySize,
             VariableKeyHashHeader.KeySalt,
             sizeof(VariableKeyHashHeader.KeySalt),
             VariableKeyHashHeader.KeyHash,
             sizeof(VariableKeyHashHeader.KeyHash)
             );
  if (!Result) {
    Status = EFI_OUT_OF_RESOURCES;
    FreePool (Variable);
    return Status;
  }

  //
  // Validation
  //
  OldVariableKeyHashHeader = (VARIABLE_KEY_HASH_HEADER *) Variable;
  DEBUG((DEBUG_INFO, "Compare KEY_ENCRYPTED Variable HASH\n"));
  DEBUG((DEBUG_INFO, "Input    HASH - "));
  InternalDumpData(VariableKeyHashHeader.KeyHash, sizeof(VariableKeyHashHeader.KeyHash));
  DEBUG((DEBUG_INFO, "\n"));
  DEBUG((DEBUG_INFO, "Expected HASH - "));
  InternalDumpData(OldVariableKeyHashHeader->KeyHash, sizeof(OldVariableKeyHashHeader->KeyHash));
  DEBUG((DEBUG_INFO, "\n"));
  if (CompareMem(OldVariableKeyHashHeader->KeyHash, VariableKeyHashHeader.KeyHash, sizeof(VariableKeyHashHeader.KeyHash)) != 0) {
    DEBUG((DEBUG_INFO, "HASH mismatch!\n"));
    Status = EFI_SECURITY_VIOLATION;
    FreePool (Variable);
    return Status;
  } else {
    DEBUG((DEBUG_INFO, "HASH match!\n"));
  }

  //
  // Get data
  //
  EncRawDataSize = ALIGN_VALUE(RawDataSize, AES_BLOCK_SIZE);
  Buffer = AllocateZeroPool(EncRawDataSize);
  if (Buffer == NULL) {
    FreePool (Variable);
    return EFI_OUT_OF_RESOURCES;
  }
  Result = KeyLibDecrypt (
             SYM_TYPE_AES,
             KeyData + 1,
             KeyData->KeySize,
             VariableKeyHashHeader.KeySalt,
             sizeof(VariableKeyHashHeader.KeySalt),
             (VOID *) ((UINT8 *)Variable + sizeof (VARIABLE_KEY_HASH_HEADER) + sizeof (VARIABLE_KEY_DATA_HEADER)),
             EncRawDataSize,
             Buffer,
             EncRawDataSize
             );
  if (!Result) {
    Status = EFI_OUT_OF_RESOURCES;
    FreePool (Variable);
    return Status;
  }

  CopyMem(Data, Buffer, RawDataSize);
  FreePool(Buffer);
  
  *DataSize = RawDataSize;

  DEBUG((DEBUG_INFO, "Get Variable Success!\n"));

  FreePool (Variable);
  return EFI_SUCCESS;
}

/**

  This code sets variable in storage blocks (Volatile or Non-Volatile).

  Caution: This function may receive untrusted input.
  This function may be invoked in SMM mode, and datasize and data are external input.
  This function will do basic validation, before parse the data.
  This function will parse the authentication carefully to avoid security issues, like
  buffer overflow, integer overflow.
  This function will check attribute carefully to avoid authentication bypass.

  @param[in] VariableName                     Name of Variable to be found.
  @param[in] VendorGuid                       Variable vendor GUID.
  @param[in] Attributes                       Attribute value of the variable found
  @param[in] AttributesEx                     AttributeEx value of the variable found
  @param[in] DataSize                         Size of Data found. If size is less than the
                                                data, this value contains the required size.
  @param[in] Data                             Data pointer.

  @return EFI_INVALID_PARAMETER               Invalid parameter.
  @return EFI_SUCCESS                         Set successfully.
  @return EFI_OUT_OF_RESOURCES                Resource not enough to set variable.
  @return EFI_NOT_FOUND                       Not found.
  @return EFI_WRITE_PROTECTED                 Variable is read-only.

**/
EFI_STATUS
EFIAPI
VariableServiceSetVariableEx (
  IN CHAR16                   *VariableName,
  IN EFI_GUID                 *VendorGuid,
  IN UINT32                   Attributes,
  IN UINT8                    AttributesEx,
  IN UINTN                    DataSize,
  IN VOID                     *Data
  )
{
  EFI_STATUS                  Status;
  VOID                        *RawData;
  UINTN                       RawDataSize;
  UINTN                       EncRawDataSize;
  EDKII_VARIABLE_KEY_DATA     *KeyData;
  UINTN                       KeyDataSize;
  VARIABLE_KEY_HASH_HEADER    VariableKeyHashHeader;
  VARIABLE_KEY_HASH_HEADER    *OldVariableKeyHashHeader;
  VARIABLE_KEY_DATA_HEADER    VariableKeyDataHeader;
  BOOLEAN                     Result;
  VOID                        *Buffer;
  VOID                        *OldVariable;
  UINTN                       OldVariableSize;

  OldVariableSize = 0;
  OldVariable     = NULL;

  Status = GetVariable2 (
             VariableName,
             VendorGuid,
             (VOID **) &OldVariable,
             &OldVariableSize
             );
  if (Status != EFI_SUCCESS && Status != EFI_NOT_FOUND) {
    return Status;
  }

  //
  // Update Data and DataSize
  //
  KeyData     = (EDKII_VARIABLE_KEY_DATA *) Data;
  KeyDataSize = sizeof(EDKII_VARIABLE_KEY_DATA) + KeyData->KeySize;

  RawData = (VOID *)((UINT8 *)Data + KeyDataSize);
  RawDataSize = DataSize - KeyDataSize;

  EncRawDataSize = ALIGN_VALUE(RawDataSize, AES_BLOCK_SIZE);
  Buffer = AllocateZeroPool(sizeof (VARIABLE_KEY_HASH_HEADER) + sizeof (VARIABLE_KEY_DATA_HEADER) + EncRawDataSize);
  if (Buffer == NULL) {
    if (OldVariable != NULL) {
      FreePool (OldVariable);
    }
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem ((UINT8 *)Buffer + sizeof (VARIABLE_KEY_HASH_HEADER) + sizeof (VARIABLE_KEY_DATA_HEADER), RawData, RawDataSize);
  RawData = (UINT8 *)Buffer + sizeof (VARIABLE_KEY_HASH_HEADER) + sizeof (VARIABLE_KEY_DATA_HEADER);

  //
  // Calculate Hash
  //
  VariableKeyHashHeader.KeyHashType = HASH_TYPE_SHA256;
  VariableKeyHashHeader.KeyHashHeadSize = sizeof(VARIABLE_KEY_HASH_HEADER);
  if (OldVariable != NULL) {
    DEBUG((DEBUG_INFO, "Old KEY_ENCRYPTED Variable Found!\n"));
    OldVariableKeyHashHeader = (VARIABLE_KEY_HASH_HEADER *) OldVariable;
    DEBUG((DEBUG_INFO, "Old SALT - "));
    InternalDumpData(OldVariableKeyHashHeader->KeySalt, sizeof(OldVariableKeyHashHeader->KeySalt));
    DEBUG((DEBUG_INFO, "\n"));
    CopyMem(VariableKeyHashHeader.KeySalt, OldVariableKeyHashHeader->KeySalt, sizeof(VariableKeyHashHeader.KeySalt));

    Result = KeyLibGenerateHash (
               HASH_TYPE_SHA256,
               KeyData + 1,
               KeyData->KeySize,
               VariableKeyHashHeader.KeySalt,
               sizeof(VariableKeyHashHeader.KeySalt),
               VariableKeyHashHeader.KeyHash,
               sizeof(VariableKeyHashHeader.KeyHash)
               );
    if (!Result) {
      Status = EFI_OUT_OF_RESOURCES;
      FreePool (OldVariable);
      return Status;
    }

    //
    // Validation
    //
    OldVariableKeyHashHeader = (VARIABLE_KEY_HASH_HEADER *) OldVariable;
    DEBUG((DEBUG_INFO, "Compare KEY_ENCRYPTED Variable HASH\n"));
    DEBUG((DEBUG_INFO, "Input    HASH - "));
    InternalDumpData(VariableKeyHashHeader.KeyHash, sizeof(VariableKeyHashHeader.KeyHash));
    DEBUG((DEBUG_INFO, "\n"));
    DEBUG((DEBUG_INFO, "Expected HASH - "));
    InternalDumpData(OldVariableKeyHashHeader->KeyHash, sizeof(OldVariableKeyHashHeader->KeyHash));
    DEBUG((DEBUG_INFO, "\n"));
    if (CompareMem(OldVariableKeyHashHeader->KeyHash, VariableKeyHashHeader.KeyHash, sizeof(VariableKeyHashHeader.KeyHash)) != 0) {
      DEBUG((DEBUG_INFO, "HASH mismatch!\n"));
      Status = EFI_SECURITY_VIOLATION;
      FreePool (OldVariable);
      return Status;
    } else {
      DEBUG((DEBUG_INFO, "HASH match!\n"));
    }

    FreePool (OldVariable);
  } else {
    DEBUG((DEBUG_INFO, "Old KEY_ENCRYPTED Variable NOT Found!\n"));
  }

  //
  // Update Data
  //
  if (RawDataSize != 0) {
    DEBUG((DEBUG_INFO, "Update KEY_ENCRYPTED Variable\n"));
    //
    // Re-generate salt
    //
    Result = KeyLibGenerateSalt(VariableKeyHashHeader.KeySalt, sizeof(VariableKeyHashHeader.KeySalt));
    if (!Result) {
      Status = EFI_OUT_OF_RESOURCES;
      return Status;
    }
    DEBUG((DEBUG_INFO, "New SALT - "));
    InternalDumpData(VariableKeyHashHeader.KeySalt, sizeof(VariableKeyHashHeader.KeySalt));
    DEBUG((DEBUG_INFO, "\n"));

    DEBUG((DEBUG_INFO, "KeyData size:%d \n", KeyData->KeySize));

    Result = KeyLibGenerateHash(
               HASH_TYPE_SHA256,
               KeyData + 1,
               KeyData->KeySize,
               VariableKeyHashHeader.KeySalt,
               sizeof(VariableKeyHashHeader.KeySalt),
               VariableKeyHashHeader.KeyHash,
               sizeof(VariableKeyHashHeader.KeyHash)
               );
    if (!Result) {
      Status = EFI_OUT_OF_RESOURCES;
      return Status;
    }

    DEBUG((DEBUG_INFO, "New HASH - "));
    InternalDumpData(VariableKeyHashHeader.KeyHash, sizeof(VariableKeyHashHeader.KeyHash));
    DEBUG((DEBUG_INFO, "\n"));

    CopyMem(Buffer, &VariableKeyHashHeader, sizeof(VariableKeyHashHeader));

    VariableKeyDataHeader.KeyDataType      = SYM_TYPE_AES;
    VariableKeyDataHeader.KeyDataHeadSize  = sizeof(VARIABLE_KEY_DATA_HEADER);
    VariableKeyDataHeader.KeyPlainDataSize = (UINT32)RawDataSize;
    VariableKeyDataHeader.KeyDataSize      = (UINT32)EncRawDataSize;
    CopyMem((UINT8 *)Buffer + sizeof (VARIABLE_KEY_HASH_HEADER), &VariableKeyDataHeader,
      sizeof(VARIABLE_KEY_DATA_HEADER));

    Result = KeyLibEncrypt (
               SYM_TYPE_AES,
               KeyData + 1,
               KeyData->KeySize,
               VariableKeyHashHeader.KeySalt,
               sizeof(VariableKeyHashHeader.KeySalt),
               RawData,
               EncRawDataSize,
               (VOID *)((UINT8 *)Buffer + sizeof (VARIABLE_KEY_HASH_HEADER) + sizeof(VARIABLE_KEY_DATA_HEADER)),
               EncRawDataSize
               );
    if (!Result) {
      Status = EFI_OUT_OF_RESOURCES;
      return Status;
    }

    Data = Buffer;
    DataSize = sizeof(VARIABLE_KEY_HASH_HEADER) + sizeof(VARIABLE_KEY_DATA_HEADER) + EncRawDataSize;
  } else {
    DEBUG((DEBUG_INFO, "Delete KEY_ENCRYPTED Variable\n"));
    Data = NULL;
    DataSize = 0;
  }

  Status = gRT->SetVariable (
                  VariableName,
                  VendorGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  DataSize,
                  Data
                  );

  if (Status == EFI_SUCCESS) {
    DEBUG((DEBUG_INFO, "Set Variable Success!\n"));
  }

  return Status;
}

