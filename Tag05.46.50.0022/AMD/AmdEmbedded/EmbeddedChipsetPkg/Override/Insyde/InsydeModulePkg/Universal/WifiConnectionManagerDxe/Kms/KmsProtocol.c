/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The Kms protocol temporary implementation.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#include "KmsService.h"

extern EFI_GUID gEfiKmsFormatGenericDynamicGuid;

/**
  Get the current status of the key management service.

  @param[in]      This              Pointer to the EFI_KMS_PROTOCOL instance.

  @retval EFI_SUCCESS               The KMS is ready for use.

**/
EFI_STATUS
EfiKmsGetServiceStatus (
  IN EFI_KMS_PROTOCOL           *This
  )
{
  return EFI_SUCCESS;
}

/**
  Register client information with the supported KMS.

  @param[in]      This              Pointer to the EFI_KMS_PROTOCOL instance.
  @param[in]      Client            Pointer to a valid EFI_KMS_CLIENT_INFO structure.
  @param[in, out] ClientDataSize    Pointer to the size, in bytes, of an arbitrary block of
                                    data specified by the ClientData parameter. This
                                    parameter may be NULL, in which case the ClientData
                                    parameter will be ignored and no data will be
                                    transferred to or from the KMS. If the parameter is
                                    not NULL, then ClientData must be a valid pointer.
                                    If the value pointed to is 0, no data will be transferred
                                    to the KMS, but data may be returned by the KMS.
                                    For all non-zero values *ClientData will be transferred
                                    to the KMS, which may also return data to the caller.
                                    In all cases, the value upon return to the caller will
                                    be the size of the data block returned to the caller,
                                    which will be zero if no data is returned from the KMS.
  @param[in, out] ClientData        Pointer to a pointer to an arbitrary block of data of
                                    *ClientDataSize that is to be passed directly to the
                                    KMS if it supports the use of client data. This
                                    parameter may be NULL if and only if the
                                    ClientDataSize parameter is also NULL. Upon return to
                                    the caller, *ClientData points to a block of data of
                                    *ClientDataSize that was returned from the KMS.
                                    If the returned value for *ClientDataSize is zero,
                                    then the returned value for *ClientData must be NULL
                                    and should be ignored by the caller. The KMS protocol
                                    consumer is responsible for freeing all valid buffers
                                    used for client data regardless of whether they are
                                    allocated by the caller for input to the function or by
                                    the implementation for output back to the caller.

  @retval EFI_SUCCESS               The client information has been accepted by the KMS.
  @retval EFI_ACCESS_DENIED         Access was denied by the device or the key server.
  @retval EFI_OUT_OF_RESOURCES      Required resources were not available to perform the function.
  @retval EFI_INVALID_PARAMETER     This is NULL.

**/
EFI_STATUS
EfiKmsRegisterClient (
  IN EFI_KMS_PROTOCOL           *This,
  IN EFI_KMS_CLIENT_INFO        *Client,
  IN OUT UINTN                  *ClientDataSize OPTIONAL,
  IN OUT VOID                   **ClientData OPTIONAL
  )
{
  EFI_KMS_PRIVATE_DATA        *KmsPrivate;
  EFI_KMS_CLIENT_NODE         *ClientNode;

  if (This == NULL || Client == NULL ||
      (Client->ClientIdSize != sizeof (EFI_GUID)) || (Client->ClientId == NULL) ||
      ClientDataSize == NULL || ClientData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  KmsPrivate = KMS_PRIVATE_DATA_FROM_PROTOCOL(This);

  //
  // No authentication required prior to the end of DXE.
  //
  if (KmsPrivate->EndOfDxe) {
    return EFI_ACCESS_DENIED;
  }

  //
  // No duplicate client is allowed.
  //
  ClientNode = KmsFindClientNode (KmsPrivate, Client);
  if (ClientNode != NULL) {
    return EFI_ACCESS_DENIED;
  }

  //
  // Create a new client node and assign a nonce number for it as the client data.
  //
  ClientNode = AllocatePool (sizeof (EFI_KMS_CLIENT_NODE));
  if (ClientNode == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ClientNode->Signature = KMS_CLIENT_SIGNATURE;
  CopyGuid (&ClientNode->CliendId, Client->ClientId);
  if (!RandomSeed (NULL, 0) ||
    !RandomBytes (ClientNode->Nonce, sizeof (ClientNode->Nonce))) {
    FreePool (ClientNode);
    return EFI_INVALID_PARAMETER;
  }

  *ClientDataSize = sizeof (ClientNode->Nonce);
  *ClientData = AllocatePool (*ClientDataSize);
  if (*ClientData == NULL) {
    FreePool (ClientNode);
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (*ClientData, &ClientNode->Nonce, sizeof (ClientNode->Nonce));

  InsertTailList (&KmsPrivate->RegisteredClientList, &ClientNode->List);

  return EFI_SUCCESS;
}

/**
  Request that the KMS generate one or more new keys and associate them with key identifiers.
  The key value(s) is returned to the caller.

  @param[in]      This               Pointer to the EFI_KMS_PROTOCOL instance.
  @param[in]      Client             Pointer to a valid EFI_KMS_CLIENT_INFO structure.
  @param[in, out] KeyDescriptorCount Pointer to a count of the number of key descriptors to be
                                     processed by this operation. On return, this number
                                     will be updated with the number of key descriptors
                                     successfully processed.
  @param[in, out] KeyDescriptors     Pointer to an array of EFI_KMS_KEY_DESCRIPTOR
                                     structures which describe the keys to be generated.
                                     On input, the KeyIdentifierSize and the KeyIdentifier
                                     may specify an identifier to be used for the key,
                                     but this is not required. The KeyFormat field must
                                     specify a key format GUID reported as supported by
                                     the KeyFormats field of the EFI_KMS_PROTOCOL.
                                     The value for this field in the first key descriptor will
                                     be considered the default value for subsequent key
                                     descriptors requested in this operation if those key
                                     descriptors have a NULL GUID in the key format field.
                                     On output, the KeyIdentifierSize and KeyIdentifier fields
                                     will specify an identifier for the key which will be either
                                     the original identifier if one was provided, or an identifier
                                     generated either by the KMS or the KMS protocol
                                     implementation. The KeyFormat field will be updated
                                     with the GUID used to generate the key if it was a
                                     NULL GUID, and the KeyValue field will contain a pointer
                                     to memory containing the key value for the generated
                                     key. Memory for both the KeyIdentifier and the KeyValue
                                     fields will be allocated with the BOOT_SERVICES_DATA
                                     type and must be freed by the caller when it is no longer
                                     needed. Also, the KeyStatus field must reflect the result
                                     of the request relative to that key.
  @param[in, out] ClientDataSize     Pointer to the size, in bytes, of an arbitrary block of
                                     data specified by the ClientData parameter. This
                                     parameter may be NULL, in which case the ClientData
                                     parameter will be ignored and no data will be
                                     transferred to or from the KMS. If the parameter is
                                     not NULL, then ClientData must be a valid pointer.
                                     If the value pointed to is 0, no data will be transferred
                                     to the KMS, but data may be returned by the KMS.
                                     For all non-zero values *ClientData will be transferred
                                     to the KMS, which may also return data to the caller.
                                     In all cases, the value upon return to the caller will
                                     be the size of the data block returned to the caller,
                                     which will be zero if no data is returned from the KMS.
  @param[in, out] ClientData         Pointer to a pointer to an arbitrary block of data of
                                     *ClientDataSize that is to be passed directly to the
                                     KMS if it supports the use of client data. This
                                     parameter may be NULL if and only if the
                                     ClientDataSize parameter is also NULL. Upon return to
                                     the caller, *ClientData points to a block of data of
                                     *ClientDataSize that was returned from the KMS.
                                     If the returned value for *ClientDataSize is zero,
                                     then the returned value for *ClientData must be NULL
                                     and should be ignored by the caller. The KMS protocol
                                     consumer is responsible for freeing all valid buffers
                                     used for client data regardless of whether they are
                                     allocated by the caller for input to the function or by
                                     the implementation for output back to the caller.

  @retval EFI_UNSUPPORTED            This function is not supported by the KMS. --OR--
                                     One (or more) of the key requests submitted is not supported by
                                     the KMS. Check individual key request(s) to see which ones
                                     may have been processed.

**/
EFI_STATUS
EfiKmsCreateKey (
  IN EFI_KMS_PROTOCOL           *This,
  IN EFI_KMS_CLIENT_INFO        *Client,
  IN OUT UINT16                 *KeyDescriptorCount,
  IN OUT EFI_KMS_KEY_DESCRIPTOR *KeyDescriptors,
  IN OUT UINTN                  *ClientDataSize OPTIONAL,
  IN OUT VOID                   **ClientData OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Retrieve an existing key.

  @param[in]      This               Pointer to the EFI_KMS_PROTOCOL instance.
  @param[in]      Client             Pointer to a valid EFI_KMS_CLIENT_INFO structure.
  @param[in, out] KeyDescriptorCount Pointer to a count of the number of key descriptors to be
                                     processed by this operation. On return, this number
                                     will be updated with the number of key descriptors
                                     successfully processed.
  @param[in, out] KeyDescriptors     Pointer to an array of EFI_KMS_KEY_DESCRIPTOR
                                     structures which describe the keys to be retrieved
                                     from the KMS.
                                     On input, the KeyIdentifierSize and the KeyIdentifier
                                     must specify an identifier to be used to retrieve a
                                     specific key. All other fields in the descriptor should
                                     be NULL.
                                     On output, the KeyIdentifierSize and KeyIdentifier fields
                                     will be unchanged, while the KeyFormat and KeyValue
                                     fields will be updated values associated with this key
                                     identifier. Memory for the KeyValue field will be
                                     allocated with the BOOT_SERVICES_DATA type and
                                     must be freed by the caller when it is no longer needed.
                                     Also, the KeyStatus field will reflect the result of the
                                     request relative to the individual key descriptor.
  @param[in, out] ClientDataSize     Pointer to the size, in bytes, of an arbitrary block of
                                     data specified by the ClientData parameter. This
                                     parameter may be NULL, in which case the ClientData
                                     parameter will be ignored and no data will be
                                     transferred to or from the KMS. If the parameter is
                                     not NULL, then ClientData must be a valid pointer.
                                     If the value pointed to is 0, no data will be transferred
                                     to the KMS, but data may be returned by the KMS.
                                     For all non-zero values *ClientData will be transferred
                                     to the KMS, which may also return data to the caller.
                                     In all cases, the value upon return to the caller will
                                     be the size of the data block returned to the caller,
                                     which will be zero if no data is returned from the KMS.
  @param[in, out] ClientData         Pointer to a pointer to an arbitrary block of data of
                                     *ClientDataSize that is to be passed directly to the
                                     KMS if it supports the use of client data. This
                                     parameter may be NULL if and only if the
                                     ClientDataSize parameter is also NULL. Upon return to
                                     the caller, *ClientData points to a block of data of
                                     *ClientDataSize that was returned from the KMS.
                                     If the returned value for *ClientDataSize is zero,
                                     then the returned value for *ClientData must be NULL
                                     and should be ignored by the caller. The KMS protocol
                                     consumer is responsible for freeing all valid buffers
                                     used for client data regardless of whether they are
                                     allocated by the caller for input to the function or by
                                     the implementation for output back to the caller.

  @retval EFI_SUCCESS                Successfully retrieved all requested keys.
  @retval EFI_ACCESS_DENIED          Access was denied by the device or the key server; OR a
                                     ClientId is required by the server and either none or an
                                     invalid id was provided.
  @retval EFI_INVALID_PARAMETER      This is NULL, ClientId is required but it is NULL,
                                     KeyDescriptorCount is NULL, or Keys is NULL.

**/
EFI_STATUS
EfiKmsGetKey (
  IN EFI_KMS_PROTOCOL           *This,
  IN EFI_KMS_CLIENT_INFO        *Client,
  IN OUT UINT16                 *KeyDescriptorCount,
  IN OUT EFI_KMS_KEY_DESCRIPTOR *KeyDescriptors,
  IN OUT UINTN                  *ClientDataSize OPTIONAL,
  IN OUT VOID                   **ClientData OPTIONAL
  )
{
  EFI_KMS_PRIVATE_DATA        *KmsPrivate;
  EFI_KMS_CLIENT_NODE         *ClientNode;
  UINT16                      Index;
  EFI_KMS_KEY_DESCRIPTOR      *KeyDesc;
  UINT16                      SuccessCount;

  if (This == NULL || Client == NULL ||
      (Client->ClientIdSize != sizeof (EFI_GUID)) || (Client->ClientId == NULL) ||
      ClientDataSize == NULL || *ClientDataSize != KMS_NONCE_SIZE ||
      ClientData == NULL || *ClientData == NULL ||
      KeyDescriptorCount == NULL || *KeyDescriptorCount == 0 ||
      KeyDescriptors == NULL) {

    return EFI_INVALID_PARAMETER;
  }

  KmsPrivate = KMS_PRIVATE_DATA_FROM_PROTOCOL(This);
  //
  // Admin password should be available before reading key
  //
  if (!KmsPrivate->AdminPwdAvaiable) {
    return EFI_ACCESS_DENIED;
  }

  //
  // In order to add key, the client must provide the correct nonce associated with the client Id.
  //
  ClientNode = KmsFindClientNode (KmsPrivate, Client);
  if (ClientNode == NULL) {
    return EFI_ACCESS_DENIED;
  }
  if (CompareMem (*ClientData, ClientNode->Nonce, KMS_NONCE_SIZE) != 0) {
    return EFI_ACCESS_DENIED;
  }

  //
  // Read Key
  //
  SuccessCount = 0;
  for (Index = 0; Index < *KeyDescriptorCount; Index++) {
    KeyDesc = KeyDescriptors + Index;
    if (KeyDesc->KeyIdentifierSize > This->KeyIdMaxSize ||
        KeyDesc->KeyIdentifierSize != StrSize ((CHAR16*)KeyDesc->KeyIdentifier)) {
      //
      // KeyId extend the max length.
      //
      KeyDesc->KeyStatus = EFI_INVALID_PARAMETER;
      continue;
    }
    CopyGuid (&KeyDesc->KeyFormat, &gEfiKmsFormatGenericDynamicGuid);

    KeyDesc->KeyStatus = KmsGetKeyFromProtectedVar (
                           KmsPrivate,
                           KeyDesc->KeyIdentifier,
                           Client->ClientId,
                           (EFI_KMS_FORMAT_GENERIC_DYNAMIC**)&KeyDesc->KeyValue
                           );
    if (!EFI_ERROR(KeyDesc->KeyStatus)) {
      SuccessCount++;
    }
  }

  *KeyDescriptorCount = SuccessCount;
  *ClientDataSize = 0;
  *ClientData = NULL;
  return EFI_SUCCESS;
}

/**
  Add a new key.

  @param[in]      This               Pointer to the EFI_KMS_PROTOCOL instance.
  @param[in]      Client             Pointer to a valid EFI_KMS_CLIENT_INFO structure.
  @param[in, out] KeyDescriptorCount Pointer to a count of the number of key descriptors to be
                                     processed by this operation. On normal return, this
                                     number will be updated with the number of key
                                     descriptors successfully processed.
  @param[in, out] KeyDescriptors     Pointer to an array of EFI_KMS_KEY_DESCRIPTOR
                                     structures which describe the keys to be added.
                                     On input, the KeyId field for first key must contain
                                     valid identifier data to be used for adding a key to
                                     the KMS. The values for these fields in this key
                                     definition will be considered default values for
                                     subsequent keys requested in this operation. A value
                                     of 0 in any subsequent KeyId field will be replaced
                                     with the current default value. The KeyFormat and
                                     KeyValue fields for each key to be added must contain
                                     consistent values to be associated with the given KeyId.
                                     On return, the KeyStatus field will reflect the result
                                     of the operation for each key request.
  @param[in, out] ClientDataSize     Pointer to the size, in bytes, of an arbitrary block of
                                     data specified by the ClientData parameter. This
                                     parameter may be NULL, in which case the ClientData
                                     parameter will be ignored and no data will be
                                     transferred to or from the KMS. If the parameter is
                                     not NULL, then ClientData must be a valid pointer.
                                     If the value pointed to is 0, no data will be transferred
                                     to the KMS, but data may be returned by the KMS.
                                     For all non-zero values *ClientData will be transferred
                                     to the KMS, which may also return data to the caller.
                                     In all cases, the value upon return to the caller will
                                     be the size of the data block returned to the caller,
                                     which will be zero if no data is returned from the KMS.
  @param[in, out] ClientData         Pointer to a pointer to an arbitrary block of data of
                                     *ClientDataSize that is to be passed directly to the
                                     KMS if it supports the use of client data. This
                                     parameter may be NULL if and only if the
                                     ClientDataSize parameter is also NULL. Upon return to
                                     the caller, *ClientData points to a block of data of
                                     *ClientDataSize that was returned from the KMS.
                                     If the returned value for *ClientDataSize is zero,
                                     then the returned value for *ClientData must be NULL
                                     and should be ignored by the caller. The KMS protocol
                                     consumer is responsible for freeing all valid buffers
                                     used for client data regardless of whether they are
                                     allocated by the caller for input to the function or by
                                     the implementation for output back to the caller.

  @retval EFI_SUCCESS                Successfully added all requested keys.
  @retval EFI_ACCESS_DENIED          Access was denied by the device or the key server; OR a
                                     ClientId is required by the server and either none or an
                                     invalid id was provided.
  @retval EFI_INVALID_PARAMETER      This is NULL, ClientId is required but it is NULL,
                                     KeyDescriptorCount is NULL, or Keys is NULL.

**/
EFI_STATUS
EfiKmsAddKey (
  IN EFI_KMS_PROTOCOL           *This,
  IN EFI_KMS_CLIENT_INFO        *Client,
  IN OUT UINT16                 *KeyDescriptorCount,
  IN OUT EFI_KMS_KEY_DESCRIPTOR *KeyDescriptors,
  IN OUT UINTN                  *ClientDataSize OPTIONAL,
  IN OUT VOID                   **ClientData OPTIONAL
  )
{
  EFI_KMS_PRIVATE_DATA            *KmsPrivate;
  EFI_KMS_CLIENT_NODE             *ClientNode;
  UINT16                          Index;
  EFI_KMS_KEY_DESCRIPTOR          *KeyDesc;
  EFI_KMS_FORMAT_GENERIC_DYNAMIC  *KeyWrapper;
  UINT16                          SuccessCount;

  //
  // Each key is stored as a triple as <ClientId, KeyId, KeyData> (attribute is not supported).
  //   KeyDescriptors->KeyIdentifier   =>   VariableName
  //     must be a CHAR16*
  //   Client->ClientId                =>   VariableGuid
  //   KeyDescriptors->KeyValue        =>   EFI_KMS_FORMAT_GENERIC_DYNAMIC (Size, Data)
  //

  if (This == NULL || Client == NULL ||
      (Client->ClientIdSize != sizeof (EFI_GUID)) || (Client->ClientId == NULL) ||
      ClientDataSize == NULL || *ClientDataSize != KMS_NONCE_SIZE ||
      ClientData == NULL || *ClientData == NULL ||
      KeyDescriptorCount == NULL || *KeyDescriptorCount == 0 ||
      KeyDescriptors == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CopyMem((UINT8 *) &KeyDescriptors->KeyFormat, (UINT8 *) &gEfiKmsFormatGenericDynamicGuid, sizeof (EFI_GUID));

  KmsPrivate = KMS_PRIVATE_DATA_FROM_PROTOCOL(This);
  //
  // Admin password should be available before adding key
  //
  if (!KmsPrivate->AdminPwdAvaiable) {
    return EFI_ACCESS_DENIED;
  }

  //
  // In order to add key, the client must provide the correct nonce associated with the client Id.
  //
  ClientNode = KmsFindClientNode (KmsPrivate, Client);
  if (ClientNode == NULL) {
    return EFI_ACCESS_DENIED;
  }
  if (CompareMem (*ClientData, ClientNode->Nonce, KMS_NONCE_SIZE) != 0) {
    return EFI_ACCESS_DENIED;
  }

  //
  // Save the key, protected by the admin password.
  //
  SuccessCount = 0;

  for (Index = 0; Index < *KeyDescriptorCount; Index++) {
    KeyDesc = KeyDescriptors + Index;
    if (!CompareGuid (&KeyDesc->KeyFormat, &gEfiKmsFormatGenericDynamicGuid)) {
      //
      // Unsupported KeyFormat, skip it.
      //
      KeyDesc->KeyStatus = EFI_UNSUPPORTED;
      ASSERT (FALSE);
      continue;
    }
    if (KeyDesc->KeyIdentifierSize > This->KeyIdMaxSize ||
        KeyDesc->KeyIdentifierSize != StrSize ((CHAR16*)KeyDesc->KeyIdentifier)) {
      //
      // KeyId extend the max length.
      //
      KeyDesc->KeyStatus = EFI_INVALID_PARAMETER;
      ASSERT (FALSE);
      continue;
    }
    if (KeyDesc->KeyValue == NULL) {
      //
      // Empty Key value, skip it.
      //
      KeyDesc->KeyStatus = EFI_UNSUPPORTED;
      ASSERT (FALSE);
      continue;
    }

    KeyWrapper = (EFI_KMS_FORMAT_GENERIC_DYNAMIC *) KeyDesc->KeyValue;
    KeyDesc->KeyStatus = KmsSetKeyToProtectedVar (
                           KmsPrivate,
                           KeyDesc->KeyIdentifier,
                           Client->ClientId,
                           KeyWrapper->KeySize,
                           KeyWrapper->KeyData
                           );

    DEBUG ((DEBUG_INFO, "AddKey Status:%r\n", KeyDesc->KeyStatus));
    if (!EFI_ERROR(KeyDesc->KeyStatus)) {
      SuccessCount++;
    }
  }

  *KeyDescriptorCount = SuccessCount;
  *ClientDataSize = 0;
  *ClientData = NULL;
  return EFI_SUCCESS;
}

/**
  Delete an existing key from the KMS database.

  @param[in]      This               Pointer to the EFI_KMS_PROTOCOL instance.
  @param[in]      Client             Pointer to a valid EFI_KMS_CLIENT_INFO structure.
  @param[in, out] KeyDescriptorCount Pointer to a count of the number of key descriptors to be
                                     processed by this operation. On normal return, this
                                     number will be updated with the number of key
                                     descriptors successfully processed.
  @param[in, out] KeyDescriptors     Pointer to an array of EFI_KMS_KEY_DESCRIPTOR
                                     structures which describe the keys to be deleted.
                                     On input, the KeyId field for first key must contain
                                     valid identifier data to be used for adding a key to
                                     the KMS. The values for these fields in this key
                                     definition will be considered default values for
                                     subsequent keys requested in this operation. A value
                                     of 0 in any subsequent KeyId field will be replaced
                                     with the current default value. The KeyFormat and
                                     KeyValue fields are ignored, but should be 0.
                                     On return, the KeyStatus field will reflect the result
                                     of the operation for each key request.
  @param[in, out] ClientDataSize     Pointer to the size, in bytes, of an arbitrary block of
                                     data specified by the ClientData parameter. This
                                     parameter may be NULL, in which case the ClientData
                                     parameter will be ignored and no data will be
                                     transferred to or from the KMS. If the parameter is
                                     not NULL, then ClientData must be a valid pointer.
                                     If the value pointed to is 0, no data will be transferred
                                     to the KMS, but data may be returned by the KMS.
                                     For all non-zero values *ClientData will be transferred
                                     to the KMS, which may also return data to the caller.
                                     In all cases, the value upon return to the caller will
                                     be the size of the data block returned to the caller,
                                     which will be zero if no data is returned from the KMS.
  @param[in, out] ClientData         Pointer to a pointer to an arbitrary block of data of
                                     *ClientDataSize that is to be passed directly to the
                                     KMS if it supports the use of client data. This
                                     parameter may be NULL if and only if the
                                     ClientDataSize parameter is also NULL. Upon return to
                                     the caller, *ClientData points to a block of data of
                                     *ClientDataSize that was returned from the KMS.
                                     If the returned value for *ClientDataSize is zero,
                                     then the returned value for *ClientData must be NULL
                                     and should be ignored by the caller. The KMS protocol
                                     consumer is responsible for freeing all valid buffers
                                     used for client data regardless of whether they are
                                     allocated by the caller for input to the function or by
                                     the implementation for output back to the caller.

  @retval EFI_UNSUPPORTED            The implementation/KMS does not support this function.

**/
EFI_STATUS
EfiKmsDeleteKey (
  IN EFI_KMS_PROTOCOL           *This,
  IN EFI_KMS_CLIENT_INFO        *Client,
  IN OUT UINT16                 *KeyDescriptorCount,
  IN OUT EFI_KMS_KEY_DESCRIPTOR *KeyDescriptors,
  IN OUT UINTN                  *ClientDataSize OPTIONAL,
  IN OUT VOID                   **ClientData OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}


/**
  Get one or more attributes associated with a specified key identifier.
  If none are found, the returned attributes count contains a value of zero.

  @param[in]      This               Pointer to the EFI_KMS_PROTOCOL instance.
  @param[in]      Client             Pointer to a valid EFI_KMS_CLIENT_INFO structure.
  @param[in]      KeyIdentifierSize  Pointer to the size in bytes of the KeyIdentifier variable.
  @param[in]      KeyIdentifier      Pointer to the key identifier associated with this key.
  @param[in, out] KeyAttributesCount Pointer to the number of EFI_KMS_KEY_ATTRIBUTE
                                     structures associated with the Key identifier. If none
                                     are found, the count value is zero on return.
                                     On input this value reflects the number of KeyAttributes
                                     that may be returned.
                                     On output, the value reflects the number of completed
                                     KeyAttributes structures found.
  @param[in, out] KeyAttributes      Pointer to an array of EFI_KMS_KEY_ATTRIBUTE
                                     structures associated with the Key Identifier.
                                     On input, the fields in the structure should be NULL.
                                     On output, the attribute fields will have updated values
                                     for attributes associated with this key identifier.
  @param[in, out] ClientDataSize     Pointer to the size, in bytes, of an arbitrary block of
                                     data specified by the ClientData parameter. This
                                     parameter may be NULL, in which case the ClientData
                                     parameter will be ignored and no data will be
                                     transferred to or from the KMS. If the parameter is
                                     not NULL, then ClientData must be a valid pointer.
                                     If the value pointed to is 0, no data will be transferred
                                     to the KMS, but data may be returned by the KMS.
                                     For all non-zero values *ClientData will be transferred
                                     to the KMS, which may also return data to the caller.
                                     In all cases, the value upon return to the caller will
                                     be the size of the data block returned to the caller,
                                     which will be zero if no data is returned from the KMS.
  @param[in, out] ClientData         Pointer to a pointer to an arbitrary block of data of
                                     *ClientDataSize that is to be passed directly to the
                                     KMS if it supports the use of client data. This
                                     parameter may be NULL if and only if the
                                     ClientDataSize parameter is also NULL. Upon return to
                                     the caller, *ClientData points to a block of data of
                                     *ClientDataSize that was returned from the KMS.
                                     If the returned value for *ClientDataSize is zero,
                                     then the returned value for *ClientData must be NULL
                                     and should be ignored by the caller. The KMS protocol
                                     consumer is responsible for freeing all valid buffers
                                     used for client data regardless of whether they are
                                     allocated by the caller for input to the function or by
                                     the implementation for output back to the caller.

  @retval EFI_UNSUPPORTED            The implementation/KMS does not support this function.

**/
EFI_STATUS
EfiKmsGetKeyAttributes (
  IN EFI_KMS_PROTOCOL           *This,
  IN EFI_KMS_CLIENT_INFO        *Client,
  IN UINT8                      *KeyIdentifierSize,
  IN CONST VOID                 *KeyIdentifier,
  IN OUT UINT16                 *KeyAttributesCount,
  IN OUT EFI_KMS_KEY_ATTRIBUTE  *KeyAttributes,
  IN OUT UINTN                  *ClientDataSize OPTIONAL,
  IN OUT VOID                   **ClientData OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}


/**
  Add one or more attributes to a key specified by a key identifier.

  @param[in]      This               Pointer to the EFI_KMS_PROTOCOL instance.
  @param[in]      Client             Pointer to a valid EFI_KMS_CLIENT_INFO structure.
  @param[in]      KeyIdentifierSize  Pointer to the size in bytes of the KeyIdentifier variable.
  @param[in]      KeyIdentifier      Pointer to the key identifier associated with this key.
  @param[in, out] KeyAttributesCount Pointer to the number of EFI_KMS_KEY_ATTRIBUTE
                                     structures to associate with the Key. On normal returns,
                                     this number will be updated with the number of key
                                     attributes successfully processed.
  @param[in, out] KeyAttributes      Pointer to an array of EFI_KMS_KEY_ATTRIBUTE
                                     structures providing the attribute information to
                                     associate with the key.
                                     On input, the values for the fields in the structure
                                     are completely filled in.
                                     On return the KeyAttributeStatus field will reflect the
                                     result of the operation for each key attribute request.
  @param[in, out] ClientDataSize     Pointer to the size, in bytes, of an arbitrary block of
                                     data specified by the ClientData parameter. This
                                     parameter may be NULL, in which case the ClientData
                                     parameter will be ignored and no data will be
                                     transferred to or from the KMS. If the parameter is
                                     not NULL, then ClientData must be a valid pointer.
                                     If the value pointed to is 0, no data will be transferred
                                     to the KMS, but data may be returned by the KMS.
                                     For all non-zero values *ClientData will be transferred
                                     to the KMS, which may also return data to the caller.
                                     In all cases, the value upon return to the caller will
                                     be the size of the data block returned to the caller,
                                     which will be zero if no data is returned from the KMS.
  @param[in, out] ClientData         Pointer to a pointer to an arbitrary block of data of
                                     *ClientDataSize that is to be passed directly to the
                                     KMS if it supports the use of client data. This
                                     parameter may be NULL if and only if the
                                     ClientDataSize parameter is also NULL. Upon return to
                                     the caller, *ClientData points to a block of data of
                                     *ClientDataSize that was returned from the KMS.
                                     If the returned value for *ClientDataSize is zero,
                                     then the returned value for *ClientData must be NULL
                                     and should be ignored by the caller. The KMS protocol
                                     consumer is responsible for freeing all valid buffers
                                     used for client data regardless of whether they are
                                     allocated by the caller for input to the function or by
                                     the implementation for output back to the caller.

  @retval EFI_UNSUPPORTED            The implementation/KMS does not support this function.

**/
EFI_STATUS
EfiKmsAddKeyAttributes (
  IN EFI_KMS_PROTOCOL           *This,
  IN EFI_KMS_CLIENT_INFO        *Client,
  IN UINT8                      *KeyIdentifierSize,
  IN CONST VOID                 *KeyIdentifier,
  IN OUT UINT16                 *KeyAttributesCount,
  IN OUT EFI_KMS_KEY_ATTRIBUTE  *KeyAttributes,
  IN OUT UINTN                  *ClientDataSize OPTIONAL,
  IN OUT VOID                   **ClientData OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}


/**
  Delete attributes to a key specified by a key identifier.

  @param[in]      This               Pointer to the EFI_KMS_PROTOCOL instance.
  @param[in]      Client             Pointer to a valid EFI_KMS_CLIENT_INFO structure.
  @param[in]      KeyIdentifierSize  Pointer to the size in bytes of the KeyIdentifier variable.
  @param[in]      KeyIdentifier      Pointer to the key identifier associated with this key.
  @param[in, out] KeyAttributesCount Pointer to the number of EFI_KMS_KEY_ATTRIBUTE
                                     structures to associate with the Key.
                                     On input, the count value is one or more.
                                     On normal returns, this number will be updated with
                                     the number of key attributes successfully processed.
  @param[in, out] KeyAttributes      Pointer to an array of EFI_KMS_KEY_ATTRIBUTE
                                     structures providing the attribute information to
                                     associate with the key.
                                     On input, the values for the fields in the structure
                                     are completely filled in.
                                     On return the KeyAttributeStatus field will reflect the
                                     result of the operation for each key attribute request.
  @param[in, out] ClientDataSize     Pointer to the size, in bytes, of an arbitrary block of
                                     data specified by the ClientData parameter. This
                                     parameter may be NULL, in which case the ClientData
                                     parameter will be ignored and no data will be
                                     transferred to or from the KMS. If the parameter is
                                     not NULL, then ClientData must be a valid pointer.
                                     If the value pointed to is 0, no data will be transferred
                                     to the KMS, but data may be returned by the KMS.
                                     For all non-zero values *ClientData will be transferred
                                     to the KMS, which may also return data to the caller.
                                     In all cases, the value upon return to the caller will
                                     be the size of the data block returned to the caller,
                                     which will be zero if no data is returned from the KMS.
  @param[in, out] ClientData         Pointer to a pointer to an arbitrary block of data of
                                     *ClientDataSize that is to be passed directly to the
                                     KMS if it supports the use of client data. This
                                     parameter may be NULL if and only if the
                                     ClientDataSize parameter is also NULL. Upon return to
                                     the caller, *ClientData points to a block of data of
                                     *ClientDataSize that was returned from the KMS.
                                     If the returned value for *ClientDataSize is zero,
                                     then the returned value for *ClientData must be NULL
                                     and should be ignored by the caller. The KMS protocol
                                     consumer is responsible for freeing all valid buffers
                                     used for client data regardless of whether they are
                                     allocated by the caller for input to the function or by
                                     the implementation for output back to the caller.

  @retval EFI_UNSUPPORTED            The implementation/KMS does not support this function.

**/
EFI_STATUS
EfiKmsDeleteKeyAttributes (
  IN EFI_KMS_PROTOCOL           *This,
  IN EFI_KMS_CLIENT_INFO        *Client,
  IN UINT8                      *KeyIdentifierSize,
  IN CONST VOID                 *KeyIdentifier,
  IN OUT UINT16                 *KeyAttributesCount,
  IN OUT EFI_KMS_KEY_ATTRIBUTE  *KeyAttributes,
  IN OUT UINTN                  *ClientDataSize OPTIONAL,
  IN OUT VOID                   **ClientData OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}


/**
  Retrieve one or more key that has matched all of the specified key attributes.

  @param[in]      This               Pointer to the EFI_KMS_PROTOCOL instance.
  @param[in]      Client             Pointer to a valid EFI_KMS_CLIENT_INFO structure.
  @param[in, out] KeyAttributeCount  Pointer to a count of the number of key attribute structures
                                     that must be matched for each returned key descriptor.
                                     On input the count value is one or more.
                                     On normal returns, this number will be updated with
                                     the number of key attributes successfully processed.
  @param[in, out] KeyAttributes      Pointer to an array of EFI_KMS_KEY_ATTRIBUTE
                                     structure to search for.
                                     On input, the values for the fields in the structure are
                                     completely filled in.
                                     On return the KeyAttributeStatus field will reflect the
                                     result of the operation for each key attribute request.
  @param[in, out] KeyDescriptorCount Pointer to a count of the number of key descriptors matched
                                     by this operation.
                                     On entry, this number will be zero.
                                     On return, this number will be updated to the number
                                     of key descriptors successfully found.
  @param[in, out] KeyDescriptors     Pointer to an array of EFI_KMS_KEY_DESCRIPTOR
                                     structures which describe the keys from the KMS
                                     having the KeyAttribute(s) specified.
                                     On input, this pointer will be NULL.
                                     On output, the array will contain an
                                     EFI_KMS_KEY_DESCRIPTOR structure for each key
                                     meeting the search criteria. Memory for the array
                                     and all KeyValue fields will be allocated with the
                                     EfiBootServicesData type and must be freed by the
                                     caller when it is no longer needed. Also, the KeyStatus
                                     field of each descriptor will reflect the result of the
                                     request relative to that key descriptor.
  @param[in, out] ClientDataSize     Pointer to the size, in bytes, of an arbitrary block of
                                     data specified by the ClientData parameter. This
                                     parameter may be NULL, in which case the ClientData
                                     parameter will be ignored and no data will be
                                     transferred to or from the KMS. If the parameter is
                                     not NULL, then ClientData must be a valid pointer.
                                     If the value pointed to is 0, no data will be transferred
                                     to the KMS, but data may be returned by the KMS.
                                     For all non-zero values *ClientData will be transferred
                                     to the KMS, which may also return data to the caller.
                                     In all cases, the value upon return to the caller will
                                     be the size of the data block returned to the caller,
                                     which will be zero if no data is returned from the KMS.
  @param[in, out] ClientData         Pointer to a pointer to an arbitrary block of data of
                                     *ClientDataSize that is to be passed directly to the
                                     KMS if it supports the use of client data. This
                                     parameter may be NULL if and only if the
                                     ClientDataSize parameter is also NULL. Upon return to
                                     the caller, *ClientData points to a block of data of
                                     *ClientDataSize that was returned from the KMS.
                                     If the returned value for *ClientDataSize is zero,
                                     then the returned value for *ClientData must be NULL
                                     and should be ignored by the caller. The KMS protocol
                                     consumer is responsible for freeing all valid buffers
                                     used for client data regardless of whether they are
                                     allocated by the caller for input to the function or by
                                     the implementation for output back to the caller.

  @retval EFI_UNSUPPORTED            The implementation/KMS does not support this function.

**/
EFI_STATUS
EfiKmsGetKeyByAttributes (
  IN EFI_KMS_PROTOCOL           *This,
  IN EFI_KMS_CLIENT_INFO        *Client,
  IN OUT UINTN                  *KeyAttributeCount,
  IN OUT EFI_KMS_KEY_ATTRIBUTE  *KeyAttributes,
  IN OUT UINTN                  *KeyDescriptorCount,
  IN OUT EFI_KMS_KEY_DESCRIPTOR *KeyDescriptors,
  IN OUT UINTN                  *ClientDataSize OPTIONAL,
  IN OUT VOID                   **ClientData OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

