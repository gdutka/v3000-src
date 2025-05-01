/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The temporary Kms solution low layer function head file.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef __EFI_KMS_IMPL_H__
#define __EFI_KMS_IMPL_H__

/**
  Get password input from the popup windows.

  @param[in]       HddString    On entry, point to UserName buffer lengh, in bytes.
                                On exit, point to input user name length, in bytes.
  @param[in, out]  Password     The buffer to hold the input user name.

  @retval EFI_ABORTED           It is given up by pressing 'ESC' key.
  @retval EFI_NOT_READY         Not a valid input at all.
  @retval EFI_SUCCESS           Get a user name successfully.

**/
EFI_STATUS
PopupHddPasswordInputWindows (
  IN     CHAR16      *HddString,
  IN OUT CHAR8       *Password
  );

/**
  Find the expected client node from Kms client list.

  @param[in]   KmsPrivate         The global Kms private data.
  @param[in]   Client             Points to the expected client node.

  @return the found client data or NULL.

**/
EFI_KMS_CLIENT_NODE *
KmsFindClientNode (
  IN EFI_KMS_PRIVATE_DATA        *KmsPrivate,
  IN EFI_KMS_CLIENT_INFO         *Client
  );

/**
  Validate if the admin password has already been saved.

  @retval TRUE    The admin password has already been saved.
  @retval FALSE   There is no admin password been saved, need to create one.

**/
BOOLEAN
IsAdminPwdSet (
  );

/**
  Set new admin password to KMS.

  @param[in]   KmsPrivate         The global Kms private data.
  @param[in]   PwdString          The admin password.

  @retval EFI_OUT_OF_RESOURCES    Memory can't be allocated.
  @retval EFI_SUCCESS             New admin password has been set to KMS.
  @retval Others                  An unexpected error occurs.

**/
EFI_STATUS
KmsSetAdminPwd (
  IN   EFI_KMS_PRIVATE_DATA      *KmsPrivate,
  IN   UINT8                     *PwdString
  );

/**
  Validate if the provided admin password is valid.

  @param[in]   KmsPrivate         The global Kms private data.
  @param[in]   PwdString          The admin password.

  @retval EFI_OUT_OF_RESOURCES    Memory can't be allocated.
  @retval EFI_SECURITY_VIOLATION  The provided admin password is not valid.
  @retval EFI_SUCCESS             The provided admin password is valid.
  @return EFI_NOT_FOUND           Admin password not found.
  @retval Others                  An unexpected error occurs.

**/
EFI_STATUS
KmsValidateAdminPwd (
  IN   EFI_KMS_PRIVATE_DATA    *KmsPrivate,
  IN   UINT8                   *PwdString
  );

/**
  Set key to protected variable.

  @param[in]   KmsPrivate       The global Kms private data.
  @param[in]   KeyIdentifier    Pointer to the key identifier associated with this key.
  @param[in]   ClientId         The Identifier of a valid Kms client.
  @param[in]   KeySize          The size of key data
  @param[in]   KeyData          The key data to be set to protected variable.

  @retval EFI_OUT_OF_RESOURCES  Memory can't be allocated.
  @retval EFI_SUCCESS           Successfully set key to the protected variable.
  @retval Others                An unexpected error occurs.

**/
EFI_STATUS
KmsSetKeyToProtectedVar (
  IN   EFI_KMS_PRIVATE_DATA       *KmsPrivate,
  IN   CHAR16                     *KeyIdentifier,
  IN   EFI_GUID                   *ClientId,
  IN   UINT32                     KeySize,
  IN   VOID                       *KeyData
  );

/**
  Retrieve key from protected variable.

  @param[in]   KmsPrivate       The global Kms private data.
  @param[in]   KeyIdentifier    Pointer to the key identifier associated with this key.
  @param[in]   ClientId         The Identifier of a valid Kms client.
  @param[out]  KeyWrapper       The key wrapper with key size and key data.

  @retval EFI_OUT_OF_RESOURCES  Memory can't be allocated.
  @retval EFI_SUCCESS           Successfully get key from the protected variable.
  @retval Others                An unexpected error occurs.

**/
EFI_STATUS
KmsGetKeyFromProtectedVar (
  IN     EFI_KMS_PRIVATE_DATA              *KmsPrivate,
  IN     CHAR16                            *KeyIdentifier,
  IN     EFI_GUID                          *ClientId,
  OUT    EFI_KMS_FORMAT_GENERIC_DYNAMIC    **KeyWrapper
  );

#endif
