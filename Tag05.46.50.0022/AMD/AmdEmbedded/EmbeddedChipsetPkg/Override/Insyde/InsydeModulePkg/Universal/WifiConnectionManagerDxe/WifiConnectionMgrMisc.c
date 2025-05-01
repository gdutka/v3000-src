/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The Miscellaneous Routines for wireless connection manager.

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

extern EFI_GUID    mEfiWifiMgrPrivateGuid;

/**
  Empty function for event process function.

  @param Event    The Event need to be process
  @param Context  The context of the event.

**/
VOID
EFIAPI
WifiMgrInternalEmptyFunction (
  IN  EFI_EVENT    Event,
  IN  VOID         *Context
  )
{
  return;
}

/**
  Convert the mac address into a hexadecimal encoded ":" seperated string.

  @param[in]  Mac     The mac address.
  @param[in]  StrSize The size, in bytes, of the output buffer specified by Str.
  @param[out] Str     The storage to return the mac string.

**/
VOID
WifiMgrMacAddrToStr (
  IN  EFI_80211_MAC_ADDRESS  *Mac,
  IN  UINT32                 StrSize,
  OUT CHAR16                 *Str
  )
{
  if (Mac == NULL || Str == NULL) {
    return;
  }

  UnicodeSPrint (
    Str,
    StrSize,
    L"%02X:%02X:%02X:%02X:%02X:%02X",
    Mac->Addr[0], Mac->Addr[1], Mac->Addr[2],
    Mac->Addr[3], Mac->Addr[4], Mac->Addr[5]
    );
}

/**
  Require user to input a wifi admin password.

  @param[in]  Private                The pointer to the global private data structure.

  @retval EFI_SUCCESS                User has input the wifi admin password.
  @retval EFI_ACCESS_DENIED          Wifi admin password is not input correctly.
  @retval EFI_INVALID_PARAMETER      One or more of the parameters needed by the device or
                                     key server is invalid.
**/
EFI_STATUS
WifiMgrInputAdminPassword (
  IN  WIFI_MGR_PRIVATE_DATA    *Private
)
{
  EFI_KMS_PRIVATE_DATA         *KmsPrivate;
  EFI_STATUS                   Status;
  CHAR8                        AdminPwdUserInput[KMS_STORAGE_PWD_MAX_SIZE];
  CHAR8                        AdminPwdUserConfirm[KMS_STORAGE_PWD_MAX_SIZE];
  UINT8                        AdminPwdLen;
  EFI_INPUT_KEY                Key;

  if (Private->Kms == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  KmsPrivate = KMS_PRIVATE_DATA_FROM_PROTOCOL(Private->Kms);
  ASSERT (KmsPrivate != NULL);

  if (KmsPrivate->AdminPwdAvaiable == FALSE) {
    if (IsAdminPwdSet()) {

      //
      // Admin password has already been set, popup a window to input admin password
      //
      ZeroMem (KmsPrivate->AdminPwd, sizeof (KmsPrivate->AdminPwd));
      Status = PopupHddPasswordInputWindows (L"Input wifi admin password:", KmsPrivate->AdminPwd);
      if (EFI_ERROR (Status)) {
        return EFI_ACCESS_DENIED;
      }

      Status = KmsValidateAdminPwd (KmsPrivate, (UINT8*) KmsPrivate->AdminPwd);
      if (EFI_ERROR (Status)) {

        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Access Denied: Wifi admin password is not correct!",
          NULL
        );
        return EFI_ACCESS_DENIED;
      }
      KmsPrivate->AdminPwdAvaiable = TRUE;

    } else {

      //
      // Popup the admin password help message
      //
      CreatePopUp (
        EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
        &Key,
        L"You need to provide a wifi admin password !",
        L"",
        L"Wifi admin password is used to encrypt your valuable assets, it",
        L"is required next time you want to retrieve these assets. The   ",
        L"length of this password should be between 8 and 20 characters. ",
        L"",
        L"Don't forget this password for it can not be reset or cleaned !",
        NULL
      );
      
      //
      // Admin password has not been set yet, need to create a new one
      //
      while (TRUE) {

        ZeroMem (KmsPrivate->AdminPwd, sizeof (KmsPrivate->AdminPwd));
        Status = PopupHddPasswordInputWindows (L"Please create an admin password:", AdminPwdUserInput);
        if (EFI_ERROR (Status)) {
          return EFI_ACCESS_DENIED;
        }
        Status = PopupHddPasswordInputWindows (L"Confirm this new admin password:", AdminPwdUserConfirm);
        if (EFI_ERROR (Status)) {
          return EFI_ACCESS_DENIED;
        }

        AdminPwdLen = (UINT8) AsciiStrnLenS(AdminPwdUserInput, KMS_STORAGE_PWD_MAX_SIZE);
        if (AdminPwdLen > KMS_STORAGE_PWD_MAX_LEN || AdminPwdLen < KMS_STORAGE_PWD_MIN_LEN) {
          
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"The length should be between 8 and 20 characters!  ",
            NULL
          );
          continue;
        }

        if (AsciiStrCmp(AdminPwdUserInput, AdminPwdUserConfirm) == 0) {

          Status = KmsSetAdminPwd (KmsPrivate, (UINT8*) AdminPwdUserInput);
          if (EFI_ERROR (Status)) {
            return EFI_ACCESS_DENIED;
          }
          KmsPrivate->AdminPwdAvaiable = TRUE;
          AsciiStrCpyS(KmsPrivate->AdminPwd, KMS_STORAGE_PWD_MAX_SIZE, AdminPwdUserInput);
          break;
        } else {

          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"Input passwords must be the same, please try again!",
            NULL
          );
        }
      }
    }
  }
  
  if (KmsPrivate->AdminPwdAvaiable == TRUE) {
    return EFI_SUCCESS;
  } else {
    return EFI_ACCESS_DENIED;
  }
}

/**
  Retrieve the data from the device or key server.

  @param[in]       KeyIdentifierName The identifier name of the required data
  @param[in, out]  BufferSize        Size of the buffer in bytes pointed to by Buffer /
                                     Actual needed size of the required data
  @param[out]      Buffer            Pointer to the buffer for the data to be read

  @retval EFI_SUCCESS                Successfully retrieved the required data.
  @retval EFI_BUFFER_TOO_SMALL       The BufferSize was too small for the required data.
  @retval EFI_OUT_OF_RESOURCES       Could not allocate resources for the method processing.
  @retval EFI_ACCESS_DENIED          Access was denied by the device or the key server.
  @retval EFI_INVALID_PARAMETER      One or more of the parameters needed by the device or
                                     key server is invalid.
  @retval EFI_NOT_FOUND              The required data was not found on the device or key server.
**/
EFI_STATUS
WifiMgrGetDataFromKMS (
  IN      CHAR16                    *KeyIdentifierName,
  IN OUT  UINTN                     *BufferSize,
  OUT     VOID                      *Buffer
  )
{
  EFI_STATUS                        Status;
  EFI_KMS_PROTOCOL                  *Kms;
  EFI_KMS_KEY_DESCRIPTOR            KeyDescriptors;
  EFI_KMS_FORMAT_GENERIC_DYNAMIC    *KeyWrapper;
  UINT16                            KeyDescriptorCount;
  UINTN                             ClientDataSize;
  VOID                              *ClientData;

  if (KeyIdentifierName == NULL || BufferSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ASSERT (mPrivate != NULL);

  Status = EFI_NOT_FOUND;
  Kms = mPrivate->Kms;

  if (Kms != NULL) {
    ZeroMem (&KeyDescriptors, sizeof (KeyDescriptors));
    KeyDescriptors.KeyIdentifierSize = (UINT8)StrSize (KeyIdentifierName);
    KeyDescriptors.KeyIdentifier = AllocateCopyPool (KeyDescriptors.KeyIdentifierSize, KeyIdentifierName);
    if (KeyDescriptors.KeyIdentifier == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    KeyDescriptorCount = 1;
    ClientDataSize = mPrivate->ClientDataSize;
    ClientData = AllocateCopyPool (ClientDataSize, mPrivate->ClientData);
    if (ClientData == NULL) {
      FreePool (KeyDescriptors.KeyIdentifier);
      return EFI_OUT_OF_RESOURCES;
    }

    Status = Kms->GetKey (
                    Kms,
                    &mPrivate->ClientInfo,
                    &KeyDescriptorCount,
                    &KeyDescriptors,
                    &ClientDataSize,
                    &ClientData
                    );

    if (!EFI_ERROR(Status) && EFI_ERROR(KeyDescriptors.KeyStatus)) {
      Status = KeyDescriptors.KeyStatus;
    }

    if (!EFI_ERROR(Status)) {
      KeyWrapper = KeyDescriptors.KeyValue;

      //
      //KeyWrapper should not be NULL if no error status returned
      //
      ASSERT (KeyWrapper != NULL);
      
      if (*BufferSize < KeyWrapper->KeySize) {
        *BufferSize = KeyWrapper->KeySize;
        return EFI_BUFFER_TOO_SMALL;
      }

      //
      // Buffer should not be NULL when BufferSize is long enough
      //
      if (Buffer == NULL) {
        return EFI_INVALID_PARAMETER;
      }
      *BufferSize = KeyWrapper->KeySize;
      CopyMem (Buffer, KeyWrapper->KeyData, KeyWrapper->KeySize);
    }

    if (KeyDescriptors.KeyIdentifier != NULL) {
      FreePool (KeyDescriptors.KeyIdentifier);
    }

    if (ClientData != NULL) {
      FreePool (ClientData);
    }
  }

  return Status;
}

/**
  Save private key file to a device or key server.

  @param[in]  PrivateKeyName         The identifier name of the private key saving storage
  @param[in]  FileContext            The file context of private key file to be saved
  @param[in]  Password               The encrption password for private key file

  @retval EFI_SUCCESS                Successfully saved the private key file.
  @retval EFI_OUT_OF_RESOURCES       Could not allocate required resources.
  @retval EFI_ACCESS_DENIED          Access was denied by the device or the key server.
  @retval EFI_INVALID_PARAMETER      One or more of the parameters needed by the device or
                                     key server is invalid.
  @retval EFI_NOT_FOUND              Can't find the storage to save the private key file.
**/
EFI_STATUS
WifiMgrWritePrivateKeyToKMS (
  IN CHAR16                         *PrivateKeyName,
  IN WIFI_MGR_FILE_CONTEXT          *FileContext,
  IN CHAR16                         *Password
  )
{
  EFI_STATUS                        Status;
  EFI_KMS_PROTOCOL                  *Kms;
  EFI_KMS_FORMAT_GENERIC_DYNAMIC    *KmsKey;
  EFI_KMS_KEY_DESCRIPTOR            KeyDescriptors;
  UINT16                            KeyDescriptorCount;
  UINTN                             ClientDataSize;
  VOID                              *ClientData;
  EFI_EAP_PRIVATE_KEY               *WrapperData;  //Wrapper structure for private key storage
  UINTN                             WrapperDataSize;
  UINTN                             PrivateKeySize;
  VOID                              *PrivateKeyData;

  PrivateKeySize  = 0;
  PrivateKeyData  = NULL;

  Status = ReadFileContent (
             FileContext->FHandle,
             &PrivateKeyData,
             &PrivateKeySize,
             0
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  WrapperDataSize = sizeof (EFI_EAP_PRIVATE_KEY) + PrivateKeySize - 1;
  WrapperData     = (EFI_EAP_PRIVATE_KEY *) AllocateZeroPool (WrapperDataSize);
  if (WrapperData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Set encryption password
  //
  if (StrLen(Password) > 0) {
    
    StrCpyS(WrapperData->EncryptPassword, PASSWORD_STORAGE_SIZE, Password);
    WrapperData->IsEncrypted = TRUE;
  } else {
    WrapperData->IsEncrypted = FALSE;
  }

  //
  // Set private key data
  //
  WrapperData->KeySize = PrivateKeySize;
  CopyMem (WrapperData->KeyData, PrivateKeyData, PrivateKeySize);

  //
  // Set the private key wrapper data to KMS
  //
  Kms = mPrivate->Kms;
  if (Kms != NULL) {
    ZeroMem (&KeyDescriptors, sizeof (KeyDescriptors));
    KeyDescriptors.KeyIdentifierSize = (UINT8) StrSize (PrivateKeyName);
    KeyDescriptors.KeyIdentifier = AllocateCopyPool (KeyDescriptors.KeyIdentifierSize, PrivateKeyName);
    if (KeyDescriptors.KeyIdentifier == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    KmsKey = (EFI_KMS_FORMAT_GENERIC_DYNAMIC *) AllocateZeroPool(sizeof (EFI_KMS_FORMAT_GENERIC_DYNAMIC)
                                                        + WrapperDataSize - 1);
    if (KmsKey == NULL) {
      FreePool (KeyDescriptors.KeyIdentifier);
      return EFI_OUT_OF_RESOURCES;
    }
    KmsKey->KeySize = (UINT32) WrapperDataSize;
    CopyMem(KmsKey->KeyData, WrapperData, KmsKey->KeySize);
    KeyDescriptors.KeyValue = KmsKey;
    
    KeyDescriptorCount = 1;
    ClientDataSize = mPrivate->ClientDataSize;
    ClientData = AllocateCopyPool (ClientDataSize, mPrivate->ClientData);
    if (ClientData == NULL) {
      FreePool (KeyDescriptors.KeyIdentifier);
      FreePool (KmsKey);
      return EFI_OUT_OF_RESOURCES;
    }

    Status = Kms->AddKey (
                    Kms,
                    &mPrivate->ClientInfo,
                    &KeyDescriptorCount,
                    &KeyDescriptors,
                    &ClientDataSize,
                    &ClientData
                    );

    if (KeyDescriptors.KeyIdentifier != NULL) {
      FreePool (KeyDescriptors.KeyIdentifier);
    }

    if (ClientData != NULL) {
      FreePool (ClientData);
    }
  }

  return Status;
}

/**
  Save password to a device or key server.

  @param[in]  PasswordName           The identifier name of the password saving storage
  @param[in]  Password               The password to be saved to the device or key server

  @retval EFI_SUCCESS                Successfully saved the password.
  @retval EFI_OUT_OF_RESOURCES       Could not allocate required resources.
  @retval EFI_ACCESS_DENIED          Access was denied by the device or the key server.
  @retval EFI_INVALID_PARAMETER      One or more of the parameters needed by the device or
                                     key server is invalid.
  @retval EFI_NOT_FOUND              Can't find the storage to save the password.
**/
EFI_STATUS
WifiMgrWritePasswordToKMS (
  IN CHAR16                         *PasswordName,
  IN CHAR16                         *Password
  )
{
  EFI_STATUS                        Status;
  EFI_KMS_KEY_DESCRIPTOR            KeyDescriptors;
  UINT16                            KeyDescriptorCount;
  UINTN                             ClientDataSize;
  VOID                              *ClientData;
  EFI_KMS_FORMAT_GENERIC_DYNAMIC    *KeyWrapper;
  EFI_KMS_PROTOCOL                  *Kms;

  if (PasswordName == NULL || Password == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  ASSERT (mPrivate != NULL);

  Status = EFI_NOT_FOUND;
  Kms = mPrivate->Kms;
  if (Kms != NULL) {
    ZeroMem (&KeyDescriptors, sizeof (KeyDescriptors));
    KeyDescriptors.KeyIdentifierSize = (UINT8)StrSize (PasswordName);
    KeyDescriptors.KeyIdentifier = AllocateCopyPool (KeyDescriptors.KeyIdentifierSize, PasswordName);
    if (KeyDescriptors.KeyIdentifier == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    KeyWrapper = (EFI_KMS_FORMAT_GENERIC_DYNAMIC *) AllocateZeroPool(sizeof (EFI_KMS_FORMAT_GENERIC_DYNAMIC)
                                                        + (StrLen(Password) + 1) * sizeof (CHAR16) - 1);
    if (KeyWrapper == NULL) {
      FreePool (KeyDescriptors.KeyIdentifier);
      return EFI_OUT_OF_RESOURCES;
    }
    KeyWrapper->KeySize = (UINT32) ((StrLen(Password) + 1) * sizeof (CHAR16));
    CopyMem(KeyWrapper->KeyData, Password, KeyWrapper->KeySize);
    KeyDescriptors.KeyValue = KeyWrapper;

    KeyDescriptorCount = 1;
    ClientDataSize = mPrivate->ClientDataSize;
    ClientData = AllocateCopyPool (ClientDataSize, mPrivate->ClientData);
    if (ClientData == NULL) {
      FreePool (KeyDescriptors.KeyIdentifier);
      FreePool (KeyWrapper);
      return EFI_OUT_OF_RESOURCES;
    }

    Status = Kms->AddKey (
                    Kms,
                    &mPrivate->ClientInfo,
                    &KeyDescriptorCount,
                    &KeyDescriptors,
                    &ClientDataSize,
                    &ClientData
                    );

    if (KeyDescriptors.KeyIdentifier != NULL) {
      FreePool (KeyDescriptors.KeyIdentifier);
    }

    if (ClientData != NULL) {
      FreePool (ClientData);
    }
  }

  return Status;
}

/**
  Write the network profile data to variable storage, the password needs to be saved on
  a device or key server.

  @param[in]      Nic               To identify which Nic the network profile belongs to.
  @param[in]      Profile           The target profile to write to variable storage.
  @param[in]      DeleteFlag        To identify if this network profile needs to be deleted,
                                    if it is set to true, this network profile will be deleted.

  @retval EFI_SUCCESS               The network profile data is written successfully.
  @retval EFI_INVALID_PARAMETER     Nic not found or profile is null.
  @retval Other Errors              Returned errors when saving password or set profile variable
                                    to variable storage.
**/
EFI_STATUS
WifiMgrWriteProfileDataToNv (
  IN  WIFI_MGR_DEVICE_DATA               *Nic,
  IN  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile,
  IN  BOOLEAN                            DeleteFlag
  )
{
  EFI_STATUS    Status;
  CHAR16        MacString[WIFI_MGR_MAX_MAC_STRING_LEN];
  UINTN         DataSize;
  CHAR16        TempPassword[PASSWORD_STORAGE_SIZE];
  CHAR16        PasswordName[WIFI_STR_MAX_SIZE];

  if (Nic == NULL || Profile == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  WifiMgrMacAddrToStr (&Nic->MacAddress, sizeof (MacString), MacString);
  UnicodeSPrint (
    MacString + StrLen (MacString),
    sizeof (MacString) - sizeof (CHAR16) * StrLen (MacString),
    L"-%d",
    Profile->ProfileIndex
    );

  if (DeleteFlag) {
    DataSize = 0;
  } else {
    DataSize = sizeof (WIFI_MGR_NETWORK_PROFILE_NVDATA);

    if (Profile->SecurityType == SECURITY_TYPE_WPA2_PERSONAL) {
      if (StrLen (Profile->Password) > 0) {
        //
        // Save password to protected variable
        //
        UnicodeSPrint (
          PasswordName,
          sizeof (PasswordName),
          L"%s-%s",
          EFI_WIFI_MGR_PSK_PASSWORD_VARIABLE,
          Profile->SSId
          );
        
        Status = WifiMgrWritePasswordToKMS(PasswordName, Profile->Password);
        if (EFI_ERROR (Status)) {
          return Status;
        }

        //
        // Restore the password first
        //
        StrCpyS (TempPassword, PASSWORD_STORAGE_SIZE, Profile->Password);
      }
    } else if (Profile->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {
      if (Profile->EapAuthMethod == EAP_AUTH_METHOD_PEAP ||
        Profile->EapAuthMethod == EAP_AUTH_METHOD_TTLS) {

        if (StrLen (Profile->EapPassword) > 0) {
          //
          // Save EAP password to protected variable
          //
          UnicodeSPrint (
            PasswordName,
            sizeof (PasswordName),
            L"%s-%s",
            EFI_WIFI_MGR_EAP_PASSWORD_VARIABLE,
            Profile->SSId
            );

          Status = WifiMgrWritePasswordToKMS(PasswordName, Profile->EapPassword);
          if (EFI_ERROR (Status)) {
            return Status;
          }

          //
          // Restore the EAP password first
          //
          StrCpyS (TempPassword, PASSWORD_STORAGE_SIZE, Profile->EapPassword);
        }
      }
    }
  }

  //
  // Don't save password to network profile variable, erase it!
  //
  ZeroMem (Profile->Password, sizeof (CHAR16) * PASSWORD_STORAGE_SIZE);
  ZeroMem (Profile->EapPassword, sizeof (CHAR16) * PASSWORD_STORAGE_SIZE);

  Status = gRT->SetVariable (
                  MacString,
                  &mEfiWifiMgrPrivateGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  DataSize,
                  Profile
                  );

  //
  // We may need the password for use, recover it!
  //
  if (!DeleteFlag) {
    if (Profile->SecurityType == SECURITY_TYPE_WPA2_PERSONAL) {
      StrCpyS (Profile->Password, PASSWORD_STORAGE_SIZE, TempPassword);
    } else if (Profile->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {
      StrCpyS (Profile->EapPassword, PASSWORD_STORAGE_SIZE, TempPassword);
    }
  }

  return Status;
}

/**
  Write the profile priority list to variable storage.

  @param[in]      Nic               To identify which Nic the profile order data belongs to.

  @retval EFI_SUCCESS               The profile order list data is written successfully.
  @retval EFI_OUT_OF_RESOURCES      Fail to allocate resource to complete the operation.
  @retval EFI_INVALID_PARAMETER     Nic not found.
  @retval Other Errors              Returned error when setting variable to variable storage.
**/
EFI_STATUS
WifiMgrUpdatePriorityList (
  IN  WIFI_MGR_DEVICE_DATA          *Nic
  )
{
  WIFI_MGR_NETWORK_PROFILE_NVDATA   *Profile;
  UINT32                            *PriorityList;
  UINT32                            ProfileCount;
  UINT32                            Index;
  CHAR16                            MacString[WIFI_MGR_MAX_MAC_STRING_LEN];
  LIST_ENTRY                        *Entry;

  if (Nic == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Index = 0;
  NET_LIST_FOR_EACH (Entry, &Nic->ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);
    if (Profile->IsSaved) {
      Index ++;
    }
  }
  ProfileCount = Index;

  if (ProfileCount != 0) {

    PriorityList = AllocateZeroPool (ProfileCount * sizeof (UINT32));
    if (PriorityList == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    Index = 0;
    NET_LIST_FOR_EACH (Entry, &Nic->ProfileList) {
      Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                  Link, WIFI_MGR_PROFILE_SIGNATURE);
      if (Profile->IsSaved) {
        PriorityList[Index ++] = Profile->ProfileIndex;
      }
    }

    //
    //Free the old list, and will create a new one!
    //
    if (Nic->PriorityList != NULL) {
      FreePool (Nic->PriorityList);
    }
    Nic->PriorityList = PriorityList;

  } else {

    //
    //No profiles to be saved, free the old list.
    //
    if (Nic->PriorityList != NULL) {
      FreePool (Nic->PriorityList);
      Nic->PriorityList = NULL;
    }
  }

  WifiMgrMacAddrToStr (&Nic->MacAddress, sizeof (MacString), MacString);
  UnicodeSPrint (
    MacString + StrLen (MacString),
    sizeof (MacString) - sizeof (CHAR16) * StrLen (MacString),
    L"-order"
    );

  return gRT->SetVariable (
                MacString,
                &mEfiWifiMgrPrivateGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                sizeof (UINT32) * ProfileCount,
                Nic->PriorityList
                );
}

/**
  Get the profile order list data for a Nic from variable storage.

  @param[in]      Nic             To identify which Nic the target profiles belongs to.

  @retval EFI_SUCCESS             The profile order list data is written successfully.
  @retval EFI_INVALID_PARAMETER   Nic not found.
  @retval EFI_OUT_OF_RESOURCES    Fail to allocate resource to complete the operation.
  @retval Others Errors           Returned errors when getting profile list from storage.
**/
EFI_STATUS
WifiMgrGetProfileDataFromNv (
  IN  WIFI_MGR_DEVICE_DATA            *Nic
  )
{
  EFI_STATUS                          Status;
  WIFI_MGR_NETWORK_PROFILE_NVDATA     *Profile;
  CHAR16                              MacString[WIFI_MGR_MAX_MAC_STRING_LEN];
  CHAR16                              NvDataName[WIFI_MGR_MAX_MAC_STRING_LEN];
  UINT32                              *PriorityList;
  UINT8                               PriorityListCount;
  UINTN                               PriorityListSize;
  UINTN                               Index;
  UINTN                               ProfileSize;
  BOOLEAN                             PriorityListChanged;
  BOOLEAN                             IsAdminPasswordNeeded;

  if (Nic == NULL || Nic->Private == NULL ) {
    return EFI_INVALID_PARAMETER;
  }

  WifiMgrMacAddrToStr (&Nic->MacAddress, sizeof (MacString), MacString);
  UnicodeSPrint (
    NvDataName,
    sizeof (NvDataName),
    L"%s-order",
    MacString
    );

  Status = GetVariable2 (
             NvDataName,
             &mEfiWifiMgrPrivateGuid,
             (VOID **) &PriorityList,
             &PriorityListSize
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ASSERT (PriorityList != NULL);

  Nic->PriorityList = AllocateZeroPool (PriorityListSize);
  if (Nic->PriorityList == NULL) {
    FreePool (PriorityList);
    return EFI_OUT_OF_RESOURCES;
  }
  Nic->ProfileCount     = 0;
  Nic->AvailableCount   = 0;
  PriorityListCount     = 0;
  PriorityListChanged   = FALSE;
  IsAdminPasswordNeeded = FALSE;

  for (Index = 0; Index < PriorityListSize / sizeof (UINT32); Index ++) {
    UnicodeSPrint (
      NvDataName,
      sizeof (NvDataName),
      L"%s-%d",
      MacString, PriorityList[Index]
      );

    ProfileSize = 0;
    Profile     = NULL;
    Status      = GetVariable2 (
                    NvDataName,
                    &mEfiWifiMgrPrivateGuid,
                    (VOID **) &Profile,
                    &ProfileSize
                    );

    if (EFI_ERROR (Status) || Profile == NULL || Profile->Signature != WIFI_MGR_PROFILE_SIGNATURE) {
      //
      //Invalid profile, destroy it.
      //
      if (Profile != NULL) {
        FreePool (Profile);
      }
      continue;
    }

    if (Profile->SSId != NULL && StrLen(Profile->SSId) > 0) {

      Profile->NicIndex     = Nic->NicIndex;
      Profile->TitleToken   = 0;
      Profile->IsAvailable  = FALSE;
      Profile->IsSaved      = TRUE;

      if (Profile->ProfileIndex > Nic->MaxProfileIndex) {
        Nic->MaxProfileIndex   = Profile->ProfileIndex;
      }
      Nic->PriorityList[PriorityListCount ++] = Profile->ProfileIndex;
      InsertTailList (&Nic->ProfileList, &Profile->Link);
      Nic->ProfileCount ++;

      //
      // WPA2-Personal network and WPA2-Enterprise network require admin password in auto connect
      //
      if (Profile->AutoConnect == TRUE) {
        if (Profile->SecurityType == SECURITY_TYPE_WPA2_PERSONAL ||
          Profile->SecurityType == SECURITY_TYPE_WPA2_ENTERPRISE) {

          IsAdminPasswordNeeded = TRUE;
        }
      }
    } else {
      //
      //An invalid profile, try to delete it.
      //
      Status = WifiMgrWriteProfileDataToNv(Nic, Profile, TRUE);
      if (!EFI_ERROR (Status)) {
        PriorityListChanged = TRUE;
      }
    }
  }

  if (PriorityListChanged) {

    //
    //One or more profiles has been deleted just now, update the list.
    //
    WifiMgrUpdatePriorityList(Nic);
  }

  if (IsAdminPasswordNeeded) {
    Status = WifiMgrInputAdminPassword(Nic->Private);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Couldn't pass admin password verification!\n"));
    }
  }

  FreePool (PriorityList);
  return EFI_SUCCESS;
}

/**
  Get the Nic data by the NicIndex.

  @param[in]  Private        The pointer to the global private data structure.
  @param[in]  NicIndex       The index indicates the position of wireless NIC.

  @return     Pointer to the Nic data, or NULL if not found.
**/
WIFI_MGR_DEVICE_DATA *
WifiMgrGetNicByIndex (
  IN WIFI_MGR_PRIVATE_DATA   *Private,
  IN UINT32                  NicIndex
  )
{
  LIST_ENTRY             *Entry;
  WIFI_MGR_DEVICE_DATA   *Nic;

  if (Private == NULL) {
    return NULL;
  }

  NET_LIST_FOR_EACH (Entry, &Private->NicList) {
    Nic = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_DEVICE_DATA,
            Link, WIFI_MGR_DEVICE_DATA_SIGNATURE);
    if (Nic->NicIndex == NicIndex) {
      return Nic;
    }
  }

  return NULL;
}

/**
  For a certain network profile, get its' next neighbor in the profile list.

  @param[in]  Nic                        The Nic to operate.
  @param[in]  PreviousProfile            A certain network profile in the Nic's profile list,
                                         get its' next neighbor for this one.

  @return Pointer to a network profile, or NULL if not found.
**/
WIFI_MGR_NETWORK_PROFILE_NVDATA *
WifiMgrSelectNextProfile (
  IN  WIFI_MGR_DEVICE_DATA               *Nic,
  IN  WIFI_MGR_NETWORK_PROFILE_NVDATA    *PreviousProfile
  )
{
  LIST_ENTRY                         *Entry;
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile;

  if (Nic == NULL || Nic->ProfileCount == 0) {
    return NULL;
  }
  if (PreviousProfile == NULL) {
    Entry = GetFirstNode (&Nic->ProfileList);
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);

    //
    //A network will be selected to auto connect only when it meets all requirements:
    //1. It is in the saved network list
    //2. It is in the latest scanned network list
    //3. The Auto Connect attribute is enabled
    //4. It is a supported network
    //
    if (Profile->IsSaved && Profile->IsAvailable && Profile->AutoConnect &&
      Profile->AKMSuiteSupported && Profile->CipherSuiteSupported) {

      return Profile;
    } else {

      PreviousProfile = Profile;
    }
  }

  while (!IsNodeAtEnd(&Nic->ProfileList, &PreviousProfile->Link)) {
    Entry = GetNextNode (&Nic->ProfileList, &PreviousProfile->Link);

    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);

    if (Profile->IsSaved && Profile->IsAvailable && Profile->AutoConnect &&
        Profile->AKMSuiteSupported && Profile->CipherSuiteSupported) {

      return Profile;
    } else {

      PreviousProfile = Profile;
    }
  }

  return NULL;
}

/**
  Find a network profile through its' SSId and securit type, and the SSId is an unicode string.

  @param[in]  SSId                   The target network's SSId.
  @param[in]  SecurityType           The target network's security type.
  @param[in]  ProfileList            The profile list on a Nic.

  @return Pointer to a network profile, or NULL if not found.
**/
WIFI_MGR_NETWORK_PROFILE_NVDATA *
WifiMgrGetProfileByUnicodeSSId (
  IN  CHAR16                         *SSId,
  IN  UINT8                          SecurityType,
  IN  LIST_ENTRY                     *ProfileList
  )
{
  LIST_ENTRY                         *Entry;
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile;

  if (SSId == NULL || ProfileList == NULL || ProfileList->ForwardLink == NULL) {
    return NULL;
  }

  NET_LIST_FOR_EACH (Entry, ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);
    if (StrCmp (SSId, Profile->SSId) == 0 && SecurityType == Profile->SecurityType) {
      return Profile;
    }
  }

  return NULL;
}

/**
  Find a network profile through its' SSId and securit type, and the SSId is an ascii string.

  @param[in]  SSId               The target network's SSId.
  @param[in]  SecurityType       The target network's security type.
  @param[in]  ProfileList        The profile list on a Nic.

  @return Pointer to a network profile, or NULL if not found.
**/
WIFI_MGR_NETWORK_PROFILE_NVDATA *
WifiMgrGetProfileByAsciiSSId (
  IN  CHAR8                      *SSId,
  IN  UINT8                      SecurityType,
  IN  LIST_ENTRY                 *ProfileList
  )
{
  CHAR16   SSIdUniCode[SSID_STORAGE_SIZE];

  if (SSId == NULL) {
    return NULL;
  }
  if (AsciiStrToUnicodeStrS (SSId, SSIdUniCode, SSID_STORAGE_SIZE) != RETURN_SUCCESS) {
    return NULL;
  }

  return WifiMgrGetProfileByUnicodeSSId (SSIdUniCode, SecurityType, ProfileList);
}

/**
  Find a network profile through its' profile index.

  @param[in]  ProfileIndex           The target network's profile index.
  @param[in]  ProfileList            The profile list on a Nic.

  @return Pointer to a network profile, or NULL if not found.
**/
WIFI_MGR_NETWORK_PROFILE_NVDATA *
WifiMgrGetProfileByProfileIndex (
  IN  UINT32                         ProfileIndex,
  IN  LIST_ENTRY                     *ProfileList
  )
{
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile;
  LIST_ENTRY                         *Entry;

  if (ProfileList == NULL || ProfileList->ForwardLink == NULL) {
    return NULL;
  }
  NET_LIST_FOR_EACH (Entry, ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);
    if (Profile->ProfileIndex == ProfileIndex) {
      return Profile;
    }
  }
  return NULL;
}

/**
  To test if the AKMSuite is in supported AKMSuite list.

  @param[in]  SupportedAKMSuiteCount       The count of the supported AKMSuites.
  @param[in]  SupportedAKMSuiteList        The supported AKMSuite list.
  @param[in]  AKMSuite                     The AKMSuite to be tested.

  @return True if this AKMSuite is supported, or False if not.
**/
BOOLEAN
WifiMgrSupportAKMSuite (
  IN  UINT16                               SupportedAKMSuiteCount,
  IN  UINT32                               *SupportedAKMSuiteList,
  IN  UINT32                               *AKMSuite
  )
{
  UINT16    Index;

  if (AKMSuite == NULL || SupportedAKMSuiteList == NULL ||
    SupportedAKMSuiteCount == 0) {
    return FALSE;
  }

  for (Index = 0; Index < SupportedAKMSuiteCount; Index ++) {
    if (SupportedAKMSuiteList[Index] == *AKMSuite) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  To check if the CipherSuite is in supported CipherSuite list.

  @param[in]  SupportedCipherSuiteCount       The count of the supported CipherSuites.
  @param[in]  SupportedCipherSuiteList        The supported CipherSuite list.
  @param[in]  CipherSuite                     The CipherSuite to be tested.

  @return True if this CipherSuite is supported, or False if not.
**/
BOOLEAN
WifiMgrSupportCipherSuite (
  IN  UINT16                                  SupportedCipherSuiteCount,
  IN  UINT32                                  *SupportedCipherSuiteList,
  IN  UINT32                                  *CipherSuite
  )
{
  UINT16  Index;

  if (CipherSuite == NULL || SupportedCipherSuiteCount == 0 ||
    SupportedCipherSuiteList == NULL) {
    return FALSE;
  }

  for (Index = 0; Index < SupportedCipherSuiteCount; Index ++) {
    if (SupportedCipherSuiteList[Index] == *CipherSuite) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Check an AKM suite list and a Cipher suite list to see if one or more AKM suites or Cipher suites
  are supported and find the matchable security type.

  @param[in]   AKMList                     The target AKM suite list to be checked.
  @param[in]   CipherList                  The target Cipher suite list to be checked
  @param[in]   Nic                         The Nic to operate, contains the supported AKMSuite list
                                           and supported CipherSuite list
  @param[out]  SecurityType                To identify a security type from the AKM suite list and
                                           Cipher suite list
  @param[out]  AKMSuiteSupported           To identify if this security type is supported. If it is
                                           NULL, overcome this field
  @param[out]  CipherSuiteSupported        To identify if this security type is supported. If it is
                                           NULL, overcome this field

  @retval EFI_SUCCESS                      This operation has completed successfully.
  @retval EFI_INVALID_PARAMETER            No Nic found or the suite list is null.
**/
EFI_STATUS
WifiMgrCheckRSN (
  IN    EFI_80211_AKM_SUITE_SELECTOR       *AKMList,
  IN    EFI_80211_CIPHER_SUITE_SELECTOR    *CipherList,
  IN    WIFI_MGR_DEVICE_DATA               *Nic,
  OUT   UINT8                              *SecurityType,
  OUT   BOOLEAN                            *AKMSuiteSupported,
  OUT   BOOLEAN                            *CipherSuiteSupported
  )
{
  EFI_80211_AKM_SUITE_SELECTOR             *SupportedAKMSuites;
  EFI_80211_CIPHER_SUITE_SELECTOR          *SupportedSwCipherSuites;
  EFI_80211_CIPHER_SUITE_SELECTOR          *SupportedHwCipherSuites;
  EFI_80211_SUITE_SELECTOR                 *AKMSuite;
  EFI_80211_SUITE_SELECTOR                 *CipherSuite;
  UINT16                                   AKMIndex;
  UINT16                                   CipherIndex;

  if (Nic == NULL || AKMList == NULL || CipherList == NULL|| SecurityType == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SupportedAKMSuites      = Nic->SupportedSuites.SupportedAKMSuites;
  SupportedSwCipherSuites = Nic->SupportedSuites.SupportedSwCipherSuites;
  SupportedHwCipherSuites = Nic->SupportedSuites.SupportedHwCipherSuites;

  *SecurityType = SECURITY_TYPE_UNKNOWN;
  if (AKMSuiteSupported != NULL && CipherSuiteSupported != NULL) {
    *AKMSuiteSupported    = FALSE;
    *CipherSuiteSupported = FALSE;
  }

  if (AKMList->AKMSuiteCount == 0) {
    if (CipherList->CipherSuiteCount == 0) {
      *SecurityType = SECURITY_TYPE_NONE;
      if (AKMSuiteSupported != NULL && CipherSuiteSupported != NULL) {
        *AKMSuiteSupported    = TRUE;
        *CipherSuiteSupported = TRUE;
      }
    }

    return EFI_SUCCESS;
  }

  for (AKMIndex = 0; AKMIndex < AKMList->AKMSuiteCount; AKMIndex ++) {

    AKMSuite = AKMList->AKMSuiteList + AKMIndex;
    if (WifiMgrSupportAKMSuite(SupportedAKMSuites->AKMSuiteCount,
      (UINT32*) SupportedAKMSuites->AKMSuiteList, (UINT32*) AKMSuite)) {

      if (AKMSuiteSupported != NULL && CipherSuiteSupported != NULL) {
        *AKMSuiteSupported = TRUE;
      }
      for (CipherIndex = 0; CipherIndex < CipherList->CipherSuiteCount; CipherIndex ++) {

        CipherSuite = CipherList->CipherSuiteList + CipherIndex;

        if (SupportedSwCipherSuites != NULL) {

          if (WifiMgrSupportCipherSuite(SupportedSwCipherSuites->CipherSuiteCount,
            (UINT32*) SupportedSwCipherSuites->CipherSuiteList, (UINT32*) CipherSuite)) {

            *SecurityType = WifiMgrGetSecurityType ((UINT32*) AKMSuite, (UINT32*) CipherSuite);

            if (*SecurityType != SECURITY_TYPE_UNKNOWN) {

              if (AKMSuiteSupported != NULL && CipherSuiteSupported != NULL) {
                *CipherSuiteSupported = TRUE;
              }
              return EFI_SUCCESS;
            }
          }
        }

        if (SupportedHwCipherSuites != NULL) {

          if (WifiMgrSupportCipherSuite(SupportedHwCipherSuites->CipherSuiteCount,
            (UINT32*) SupportedHwCipherSuites->CipherSuiteList, (UINT32*) CipherSuite)) {

            *SecurityType = WifiMgrGetSecurityType ((UINT32*) AKMSuite, (UINT32*) CipherSuite);

            if (*SecurityType != SECURITY_TYPE_UNKNOWN) {

              if (AKMSuiteSupported != NULL && CipherSuiteSupported != NULL) {
                *CipherSuiteSupported = TRUE;
              }
              return EFI_SUCCESS;
            }
          }
        }
      }
    }
  }

  *SecurityType = WifiMgrGetSecurityType ((UINT32*) AKMList->AKMSuiteList,
                    (UINT32*) CipherList->CipherSuiteList);

  return EFI_SUCCESS;
}

/**
  To get the security type for a certain AKMSuite and CipherSuite.

  @param[in]   AKMSuite             An certain AKMSuite.
  @param[in]   CipherSuite          An certain CipherSuite.

  @return a security type if found, or SECURITY_TYPE_UNKNOWN.
**/
UINT8
WifiMgrGetSecurityType (
  IN  UINT32    *AKMSuite,
  IN  UINT32    *CipherSuite
  )
{
  if (CipherSuite == NULL) {
    
    if (AKMSuite == NULL) {
      return SECURITY_TYPE_NONE;
    } else {
      return SECURITY_TYPE_UNKNOWN;
    }
  } else if (*CipherSuite == IEEE_80211_PAIRWISE_CIPHER_SUITE_USE_GROUP) {
  
    if (AKMSuite == NULL) {
      return SECURITY_TYPE_NONE;
    } else {
      return SECURITY_TYPE_UNKNOWN;
    }
  } else if (*CipherSuite == IEEE_80211_PAIRWISE_CIPHER_SUITE_WEP40 ||
    *CipherSuite == IEEE_80211_PAIRWISE_CIPHER_SUITE_WEP104) {

    return SECURITY_TYPE_WEP;
  } else if (*CipherSuite == IEEE_80211_PAIRWISE_CIPHER_SUITE_CCMP) {

    if (AKMSuite == NULL) {
      return SECURITY_TYPE_UNKNOWN;
    } 

    if (*AKMSuite == IEEE_80211_AKM_SUITE_8021X_OR_PMKSA ||
      *AKMSuite == IEEE_80211_AKM_SUITE_8021X_OR_PMKSA_SHA256) {

      return SECURITY_TYPE_WPA2_ENTERPRISE;
    } else if (*AKMSuite == IEEE_80211_AKM_SUITE_PSK ||
        *AKMSuite == IEEE_80211_AKM_SUITE_PSK_SHA256){

      return SECURITY_TYPE_WPA2_PERSONAL;
    }else {
      return SECURITY_TYPE_UNKNOWN;
    }
  } else if (*CipherSuite == IEEE_80211_PAIRWISE_CIPHER_SUITE_TKIP) {

    if (AKMSuite == NULL) {
      return SECURITY_TYPE_UNKNOWN;
    }

    if (*AKMSuite == IEEE_80211_AKM_SUITE_8021X_OR_PMKSA ||
      *AKMSuite == IEEE_80211_AKM_SUITE_8021X_OR_PMKSA_SHA256) {

      return SECURITY_TYPE_WPA_ENTERPRISE;
    } else if (*AKMSuite == IEEE_80211_AKM_SUITE_PSK ||
        *AKMSuite == IEEE_80211_AKM_SUITE_PSK_SHA256){

      return SECURITY_TYPE_WPA_PERSONAL;
    }else {
      return SECURITY_TYPE_UNKNOWN;
    }
  } else {
    return SECURITY_TYPE_UNKNOWN;
  }
}

/**
  Get supported AKMSuites and CipherSuites from supplicant.

  @param[in]   Nic                      The Nic to operate.

  @retval EFI_SUCCESS                   Get the supported suite list successfully.
  @retval EFI_INVALID_PARAMETER         No Nic found or supplicant is NULL.

**/
EFI_STATUS
WifiMgrGetSupportedSuites (
  IN    WIFI_MGR_DEVICE_DATA            *Nic
  )
{
  EFI_STATUS                            Status;
  EFI_SUPPLICANT_PROTOCOL               *Supplicant;
  EFI_80211_AKM_SUITE_SELECTOR          *SupportedAKMSuites;
  EFI_80211_CIPHER_SUITE_SELECTOR       *SupportedSwCipherSuites;
  EFI_80211_CIPHER_SUITE_SELECTOR       *SupportedHwCipherSuites;
  UINTN                                 DataSize;

  SupportedAKMSuites      = NULL;
  SupportedSwCipherSuites = NULL;
  SupportedHwCipherSuites = NULL;

  if (Nic == NULL || Nic->Supplicant == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Supplicant = Nic->Supplicant;

  DataSize  = 0;
  Status = Supplicant->GetData (Supplicant, EfiSupplicant80211SupportedAKMSuites, NULL, &DataSize);
  if (Status == EFI_BUFFER_TOO_SMALL && DataSize > 0) {

    SupportedAKMSuites = AllocateZeroPool(DataSize);
    if (SupportedAKMSuites == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Status = Supplicant->GetData (Supplicant, EfiSupplicant80211SupportedAKMSuites,
                           (UINT8 *) SupportedAKMSuites, &DataSize);
    if (!EFI_ERROR (Status)) {
      Nic->SupportedSuites.SupportedAKMSuites = SupportedAKMSuites;
    } else {
      SupportedAKMSuites = NULL;
    }
  } else {
    SupportedAKMSuites = NULL;
  }

  DataSize  = 0;
  Status = Supplicant->GetData (Supplicant, EfiSupplicant80211SupportedSoftwareCipherSuites, NULL, &DataSize);
  if (Status == EFI_BUFFER_TOO_SMALL && DataSize > 0) {


    SupportedSwCipherSuites = AllocateZeroPool(DataSize);
    if (SupportedSwCipherSuites == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Status = Supplicant->GetData (Supplicant, EfiSupplicant80211SupportedSoftwareCipherSuites,
                           (UINT8 *) SupportedSwCipherSuites, &DataSize);
    if (!EFI_ERROR (Status)) {
      Nic->SupportedSuites.SupportedSwCipherSuites = SupportedSwCipherSuites;
    } else {
      SupportedSwCipherSuites = NULL;
    }
  } else {
    SupportedSwCipherSuites = NULL;
  }

  DataSize  = 0;
  Status = Supplicant->GetData (Supplicant, EfiSupplicant80211SupportedHardwareCipherSuites, NULL, &DataSize);
  if (Status == EFI_BUFFER_TOO_SMALL && DataSize > 0) {

    SupportedHwCipherSuites = AllocateZeroPool(DataSize);
    if (SupportedHwCipherSuites == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Status = Supplicant->GetData (Supplicant, EfiSupplicant80211SupportedHardwareCipherSuites,
                           (UINT8 *) SupportedHwCipherSuites, &DataSize);
    if (!EFI_ERROR (Status)) {

      Nic->SupportedSuites.SupportedHwCipherSuites = SupportedHwCipherSuites;
    } else {

      SupportedHwCipherSuites = NULL;
    }
  } else {

    SupportedHwCipherSuites = NULL;
  }

  return EFI_SUCCESS;
}

/**
  Free all network profiles in a profile list.

  @param[in]   ProfileList           The profile list to be freed.

**/
VOID
WifiMgrFreeProfileList (
  IN  LIST_ENTRY                     *ProfileList
  )
{
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *Profile;
  LIST_ENTRY                         *Entry;
  LIST_ENTRY                         *NextEntry;

  if (ProfileList == NULL) {
    return;
  }

  NET_LIST_FOR_EACH_SAFE (Entry, NextEntry, ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);

    if (Profile->Network.AKMSuite != NULL) {
      FreePool(Profile->Network.AKMSuite);
    }

    if (Profile->Network.CipherSuite != NULL) {
      FreePool(Profile->Network.CipherSuite);
    }

    FreePool (Profile);
  }
}

/**
  Free the resources of a config token.

  @param[in]   ConfigToken          The config token to be freed.
**/
VOID
WifiMgrFreeToken (
  IN   WIFI_MGR_MAC_CONFIG_TOKEN    *ConfigToken
  )
{
  EFI_80211_GET_NETWORKS_RESULT     *Result;

  if (ConfigToken == NULL) {
    return;
  }

  switch (ConfigToken->Type) {

    case TokenTypeGetNetworksToken:

      RemoveEntryList (&ConfigToken->Link);

      if (ConfigToken->Token.GetNetworksToken != NULL) {

        gBS->CloseEvent (ConfigToken->Token.GetNetworksToken->Event);
        if (ConfigToken->Token.GetNetworksToken->Data != NULL) {
          FreePool(ConfigToken->Token.GetNetworksToken->Data);
        }

        Result = ConfigToken->Token.GetNetworksToken->Result;
        if (Result != NULL) {
          FreePool (Result);
        }

        FreePool(ConfigToken->Token.GetNetworksToken);
      }

      FreePool (ConfigToken);
      break;

    case TokenTypeConnectNetworkToken:

      RemoveEntryList (&ConfigToken->Link);

      if (ConfigToken->Token.ConnectNetworkToken != NULL) {

        gBS->CloseEvent (ConfigToken->Token.ConnectNetworkToken->Event);
        if (ConfigToken->Token.ConnectNetworkToken->Data != NULL) {
          FreePool(ConfigToken->Token.ConnectNetworkToken->Data);
        }
        FreePool(ConfigToken->Token.ConnectNetworkToken);
      }
      FreePool (ConfigToken);
      break;

    case TokenTypeDisconnectNetworkToken:

      RemoveEntryList (&ConfigToken->Link);

      if (ConfigToken->Token.DisconnectNetworkToken != NULL) {

        FreePool(ConfigToken->Token.DisconnectNetworkToken);
      }

      FreePool (ConfigToken);
      break;

    default :
      break;
  }
}

/**
  Free all tokens in a token list.

  @param[in]   TokenList          The token list to be freed.
**/
VOID
WifiMgrFreeTokenList (
  IN   LIST_ENTRY                 *TokenList
  )
{
  WIFI_MGR_MAC_CONFIG_TOKEN       *ConfigToken;
  LIST_ENTRY                      *Entry;
  LIST_ENTRY                      *NextEntry;

  if (TokenList == NULL) {
    return;
  }

  NET_LIST_FOR_EACH_SAFE (Entry, NextEntry, TokenList) {

    ConfigToken = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_MAC_CONFIG_TOKEN,
                    Link, WIFI_MGR_MAC_CONFIG_TOKEN_SIGNATURE);
    WifiMgrFreeToken (ConfigToken);
  }
}
