/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The temporary Kms solution low layer functions.

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

extern EFI_GUID gEfiKmsServiceSampleGuid;

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
  )
{
  EFI_INPUT_KEY  Key;
  UINTN          Length;
  CHAR16         String[8 + 64];
  CHAR16         Mask[KMS_STORAGE_PWD_MAX_SIZE];
  CHAR16         Unicode[KMS_STORAGE_PWD_MAX_SIZE];
  CHAR8          Ascii[KMS_STORAGE_PWD_MAX_SIZE];

  ZeroMem (Unicode, sizeof (Unicode));
  ZeroMem (Ascii, sizeof (Ascii));
  ZeroMem (Mask, sizeof (Mask));
  ZeroMem (String, sizeof (String));
  UnicodeSPrint(String, 8 + 64, L"%s", HddString);

  Length = 0;
  while (TRUE) {
    Mask[Length] = L'_';
    CreatePopUp (
      EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
      &Key,
      String,
      L"---------------------",
      Mask,
      NULL
      );
    //
    // Check key.
    //
    if (Key.ScanCode == SCAN_NULL) {
      if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
        //
        // Add the null terminator.
        //
        Unicode[Length] = 0;
        Length++;
        break;
      } else if ((Key.UnicodeChar == CHAR_NULL) ||
                 (Key.UnicodeChar == CHAR_TAB) ||
                 (Key.UnicodeChar == CHAR_LINEFEED)
                 ) {
        continue;
      } else {
        if (Key.UnicodeChar == CHAR_BACKSPACE) {
          if (Length > 0) {
            Unicode[Length] = 0;
            Mask[Length] = 0;
            Length--;
          }
        } else {
          Unicode[Length] = Key.UnicodeChar;
          Mask[Length] = L'*';
          Length++;
          if (Length == KMS_STORAGE_PWD_MAX_LEN) {
            //
            // Add the null terminator.
            //
            Unicode[Length] = 0;
            Mask[Length] = 0;
            break;
          }
        }
      }
    }

    if (Key.ScanCode == SCAN_ESC) {
      if (Length == 0) {
        return EFI_ABORTED;
      } else {
        return EFI_NOT_READY;
      }
    }
  }

  if (Length <= 1) {
    return EFI_NOT_READY;
  }

  UnicodeStrToAsciiStrS (Unicode, Ascii, KMS_STORAGE_PWD_MAX_SIZE);
  CopyMem (Password, Ascii, KMS_STORAGE_PWD_MAX_LEN);
  ZeroMem (Unicode, sizeof (Unicode));
  ZeroMem (Ascii, sizeof (Ascii));

  return EFI_SUCCESS;
}

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
  )
{
  LIST_ENTRY                  *Node;
  EFI_KMS_CLIENT_NODE         *ClientNode;

  Node = GetFirstNode (&KmsPrivate->RegisteredClientList);
  while (!IsNull (&KmsPrivate->RegisteredClientList, Node)) {
    ClientNode = KMS_CLIENT_NODE_FROM_LIST(Node);
    if (CompareGuid (&ClientNode->CliendId, Client->ClientId)) {
      return ClientNode;
    }
    Node = GetNextNode (&KmsPrivate->RegisteredClientList, Node);
  }

  return NULL;
}

/**
  Validate if the admin password has already been saved.

  @retval TRUE    The admin password has already been saved.
  @retval FALSE   There is no admin password been saved, need to create one.

**/
BOOLEAN
IsAdminPwdSet (
  )
{
  UINTN       DataSize;
  EFI_STATUS  Status;

  DataSize = 0;
  Status = gRT->GetVariable (
                  KMS_ADMIN_STORAGE_NAME,
                  &gEfiKmsServiceSampleGuid,
                  NULL,
                  &DataSize,
                  NULL
                  );
  if (Status == EFI_NOT_FOUND) {
    return FALSE;
  } else {
    return TRUE;
  }
}

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
  )
{
  KMS_VARIABLE_KEY_STRUCT        *VarPwdData;
  UINTN                          VarPwdSize;
  EFI_STATUS                     Status;

  Status    = EFI_SUCCESS;

  VarPwdSize = sizeof (KMS_VARIABLE_KEY_STRUCT) + sizeof (KMS_ADMIN_STORAGE_DATA) - 1;
  VarPwdData = AllocateZeroPool (VarPwdSize);
  if (VarPwdData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  VarPwdData->KeyHeader.KeyType = EDKII_VARIABLE_KEY_TYPE_ASCII;
  VarPwdData->KeyHeader.KeySize = KMS_STORAGE_PWD_MAX_LEN;
  CopyMem (VarPwdData->AsciiData, PwdString, KMS_STORAGE_PWD_MAX_LEN);
  CopyMem (VarPwdData->VarData, KMS_ADMIN_STORAGE_NAME, KMS_STORAGE_NAME_MAX_SIZE);

  Status = VariableServiceSetVariableEx(
             KMS_ADMIN_STORAGE_NAME,
             &gEfiKmsServiceSampleGuid,
             EFI_VARIABLE_NON_VOLATILE |
               EFI_VARIABLE_BOOTSERVICE_ACCESS,
             EDKII_VARIABLE_KEY_ENCRYPTED,
             VarPwdSize,
             VarPwdData
             );

  FreePool (VarPwdData);
  DEBUG ((DEBUG_INFO, "Set Admin Password Status: %d\n", Status));

  return Status;
}


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
  IN   EFI_KMS_PRIVATE_DATA      *KmsPrivate,
  IN   UINT8                     *PwdString
  )
{
  KMS_VARIABLE_KEY_STRUCT        *VarPwdData;
  UINTN                          VarPwdSize;
  EFI_STATUS                     Status;
  UINT32                         Attributes;
  UINT8                          AttributesEx;

  Status    = EFI_SUCCESS;

  //
  // First step, try to get the AdminStorage by using the password.
  //
  VarPwdSize = sizeof (KMS_VARIABLE_KEY_STRUCT) + sizeof (KMS_ADMIN_STORAGE_DATA) - 1;
  VarPwdData = AllocateZeroPool (VarPwdSize);
  if (VarPwdData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  VarPwdData->KeyHeader.KeyType = EDKII_VARIABLE_KEY_TYPE_ASCII;
  VarPwdData->KeyHeader.KeySize = KMS_STORAGE_PWD_MAX_LEN;
  CopyMem (VarPwdData->AsciiData, PwdString, KMS_STORAGE_PWD_MAX_LEN);
  AttributesEx = EDKII_VARIABLE_KEY_ENCRYPTED;

  Status = VariableServiceGetVariableEx(
             KMS_ADMIN_STORAGE_NAME,
             &gEfiKmsServiceSampleGuid,
             &Attributes,
             &AttributesEx,
             &VarPwdSize,
             VarPwdData
             );

  FreePool (VarPwdData);
  return Status;
}

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
  )
{
  EFI_STATUS                     Status;
  KMS_VARIABLE_KEY_STRUCT        *VarPwdData;
  UINTN                          VarPwdSize;

  ASSERT (KmsPrivate->AdminPwdAvaiable);

  VarPwdSize = sizeof (KMS_VARIABLE_KEY_STRUCT) + KeySize - 1;
  VarPwdData = AllocateZeroPool (VarPwdSize);
  if (VarPwdData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  VarPwdData->KeyHeader.KeyType = EDKII_VARIABLE_KEY_TYPE_ASCII;
  VarPwdData->KeyHeader.KeySize = KMS_STORAGE_PWD_MAX_LEN;
  CopyMem (VarPwdData->AsciiData, KmsPrivate->AdminPwd, KMS_STORAGE_PWD_MAX_LEN);
  CopyMem (VarPwdData->VarData, KeyData, KeySize);

  Status = VariableServiceSetVariableEx(
             KeyIdentifier,
             ClientId,
             EFI_VARIABLE_NON_VOLATILE |
               EFI_VARIABLE_BOOTSERVICE_ACCESS,
             EDKII_VARIABLE_KEY_ENCRYPTED,
             VarPwdSize,
             VarPwdData
             );

  FreePool (VarPwdData);
  return Status;
}

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
  IN   EFI_KMS_PRIVATE_DATA              *KmsPrivate,
  IN   CHAR16                            *KeyIdentifier,
  IN   EFI_GUID                          *ClientId,
  OUT  EFI_KMS_FORMAT_GENERIC_DYNAMIC    **KeyWrapper
  )
{
  EFI_STATUS                     Status;
  KMS_VARIABLE_KEY_STRUCT        *VarPwdData;
  UINTN                          VarPwdSize;
  UINTN                          VarSize;
  UINT32                         Attributes;
  UINT8                          AttributesEx;

  ASSERT (KmsPrivate->AdminPwdAvaiable);

  VarSize    = 0;
  VarPwdSize = sizeof (KMS_VARIABLE_KEY_STRUCT) - 1;
  VarPwdData = AllocateZeroPool (VarPwdSize);
  if (VarPwdData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  VarPwdData->KeyHeader.KeyType = EDKII_VARIABLE_KEY_TYPE_ASCII;
  VarPwdData->KeyHeader.KeySize = KMS_STORAGE_PWD_MAX_LEN;
  CopyMem (VarPwdData->AsciiData, KmsPrivate->AdminPwd, KMS_STORAGE_PWD_MAX_LEN);
  AttributesEx = EDKII_VARIABLE_KEY_ENCRYPTED;

  Status = VariableServiceGetVariableEx(
             KeyIdentifier,
             ClientId,
             &Attributes,
             &AttributesEx,
             &VarSize,
             VarPwdData
             );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    return Status;
  }
  FreePool (VarPwdData);

  VarPwdSize = sizeof (KMS_VARIABLE_KEY_STRUCT) + VarSize - 1;
  VarPwdData = AllocateZeroPool (VarPwdSize);
  if (VarPwdData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  VarPwdData->KeyHeader.KeyType = EDKII_VARIABLE_KEY_TYPE_ASCII;
  VarPwdData->KeyHeader.KeySize = KMS_STORAGE_PWD_MAX_LEN;
  CopyMem (VarPwdData->AsciiData, KmsPrivate->AdminPwd, KMS_STORAGE_PWD_MAX_LEN);
  AttributesEx = EDKII_VARIABLE_KEY_ENCRYPTED;

  Status = VariableServiceGetVariableEx(
             KeyIdentifier,
             ClientId,
             &Attributes,
             &AttributesEx,
             &VarSize,
             VarPwdData
             );
  if (EFI_ERROR (Status)) {
    FreePool (VarPwdData);
    return Status;
  }

  *KeyWrapper = AllocatePool (sizeof (EFI_KMS_FORMAT_GENERIC_DYNAMIC) + VarSize - 1);
  if (*KeyWrapper == NULL) {
    FreePool (VarPwdData);
    return EFI_OUT_OF_RESOURCES;
  }
  (*KeyWrapper)->KeySize = (UINT32) VarSize;
  CopyMem ((*KeyWrapper)->KeyData, VarPwdData, VarPwdSize);

  return EFI_SUCCESS;
}
