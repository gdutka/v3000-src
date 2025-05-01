/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The Mac Connection2 Protocol adapter functions for wireless connection manager.

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

EFI_EAP_TYPE mEapAuthMethod[] = {
  EFI_EAP_TYPE_TTLS,
  EFI_EAP_TYPE_PEAP,
  EFI_EAP_TYPE_EAPTLS,
};

EFI_EAP_TYPE mEapSecondAuthMethod[] = {
  EFI_EAP_TYPE_MSCHAPV2,
};

/**
  The callback function for scan operation.

  @param[in]  Event                 The GetNetworks token receive event.
  @param[in]  Context               The context of the GetNetworks token.

**/
VOID
EFIAPI
WifiMgrOnScanFinished (
  IN  EFI_EVENT                     Event,
  IN  VOID                          *Context
  )
{
  WIFI_MGR_MAC_CONFIG_TOKEN         *ConfigToken;
  WIFI_MGR_DEVICE_DATA              *Nic;
  WIFI_MGR_NETWORK_PROFILE_NVDATA   *Profile;
  EFI_80211_NETWORK                 *Network;
  UINTN                             DataSize;
  EFI_80211_NETWORK_DESCRIPTION     *NetworkDescription;
  EFI_80211_GET_NETWORKS_RESULT     *Result;
  LIST_ENTRY                        *Entry;
  UINT8                             SecurityType;
  BOOLEAN                           AKMSuiteSupported;
  BOOLEAN                           CipherSuiteSupported;
  CHAR8                             *AsciiSSId;
  UINTN                             Index;

  if (Context == NULL) {
    return;
  }

  ConfigToken = (WIFI_MGR_MAC_CONFIG_TOKEN *) Context;
  if (ConfigToken->Nic == NULL) {
    WifiMgrFreeToken(ConfigToken);
    return;
  }

  if (ConfigToken->Signature != WIFI_MGR_MAC_CONFIG_TOKEN_SIGNATURE ||
        ConfigToken->Type != TokenTypeGetNetworksToken) {
    WifiMgrFreeToken(ConfigToken);
    return;
  }

  //
  //It is the GetNetworks token, set scan state to finished
  //
  ConfigToken->Nic->ScanState = WifiMgrScanFinished;

  if (ConfigToken->Token.GetNetworksToken == NULL ||
        ConfigToken->Token.GetNetworksToken->Result == NULL) {
    WifiMgrFreeToken(ConfigToken);
    return;
  }

  Result  = ConfigToken->Token.GetNetworksToken->Result;
  Nic     = ConfigToken->Nic;

  //
  // Clean previous result, and update network list according to the scan result
  //
  Nic->AvailableCount = 0;
  NET_LIST_FOR_EACH (Entry, &Nic->ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);
    Profile->IsAvailable = FALSE;
  }

  if (Result == NULL) {
    gBS->SignalEvent (Nic->Private->NetworkListRefreshEvent);
    WifiMgrFreeToken(ConfigToken);
    return;
  }

  for (Index = 0; Index < Result->NumOfNetworkDesc; Index ++) {

    NetworkDescription = Result->NetworkDesc + Index;
    if (NetworkDescription == NULL) {
      continue;
    }

    Network = &NetworkDescription->Network;
    if (Network == NULL || Network->SSId.SSIdLen == 0) {
      continue;
    }

    if (EFI_ERROR (WifiMgrCheckRSN (Network->AKMSuite, Network->CipherSuite, Nic,
                     &SecurityType, &AKMSuiteSupported, &CipherSuiteSupported))) {
      SecurityType          = SECURITY_TYPE_UNKNOWN;
      AKMSuiteSupported     = FALSE;
      CipherSuiteSupported  = FALSE;
    }

    AsciiSSId = (CHAR8*) AllocateZeroPool(sizeof (CHAR8) * (Network->SSId.SSIdLen + 1));
    if (AsciiSSId == NULL) {
      continue;
    }
    CopyMem(AsciiSSId, (CHAR8 *) Network->SSId.SSId, sizeof (CHAR8) * Network->SSId.SSIdLen);
    *(AsciiSSId + Network->SSId.SSIdLen) = '\0';

    Profile = WifiMgrGetProfileByAsciiSSId (AsciiSSId, SecurityType, &Nic->ProfileList);
    if (Profile == NULL) {
      Profile = AllocateZeroPool (sizeof (WIFI_MGR_NETWORK_PROFILE_NVDATA));
      if (Profile == NULL) {
        FreePool (AsciiSSId);
        continue;
      }
      Profile->Signature    = WIFI_MGR_PROFILE_SIGNATURE;
      Profile->NicIndex     = Nic->NicIndex;
      Profile->IsSaved      = FALSE;
      Profile->ProfileIndex = Nic->MaxProfileIndex + 1;
      Nic->MaxProfileIndex ++;
      AsciiStrToUnicodeStrS (AsciiSSId, Profile->SSId, SSID_STORAGE_SIZE);
      InsertTailList (&Nic->ProfileList, &Profile->Link);
      Nic->ProfileCount ++;
    }
    FreePool (AsciiSSId);

    //
    //May receive duplicate networks in scan results, check if it has already
    //been processed.
    //
    if (Profile->IsAvailable == FALSE) {
      Profile->IsAvailable          = TRUE;
      Profile->SecurityType         = SecurityType;
      Profile->AKMSuiteSupported    = AKMSuiteSupported;
      Profile->CipherSuiteSupported = CipherSuiteSupported;
      Profile->NetworkQuality = NetworkDescription->NetworkQuality;
      Nic->AvailableCount ++;

      //
      //Copy BSSType and SSId
      //
      CopyMem(&Profile->Network, Network, sizeof (EFI_80211_NETWORK));

      //
      //Copy AKMSuite list
      //
      if (Network->AKMSuite != NULL) {

        if (Network->AKMSuite->AKMSuiteCount == 0) {
          DataSize = sizeof (EFI_80211_AKM_SUITE_SELECTOR);
        } else {
          DataSize = sizeof (EFI_80211_AKM_SUITE_SELECTOR) + sizeof (EFI_80211_SUITE_SELECTOR)
                       * (Network->AKMSuite->AKMSuiteCount - 1);
        }
        Profile->Network.AKMSuite = (EFI_80211_AKM_SUITE_SELECTOR *) AllocateZeroPool (DataSize);
        if (Profile->Network.AKMSuite == NULL) {
          continue;
        }
        CopyMem (Profile->Network.AKMSuite, Network->AKMSuite, DataSize);
      }
      //
      //Copy CipherSuite list
      //
      if (Network->CipherSuite != NULL) {

        if (Network->CipherSuite->CipherSuiteCount == 0) {
          DataSize = sizeof (EFI_80211_CIPHER_SUITE_SELECTOR);
        } else {
          DataSize = sizeof (EFI_80211_CIPHER_SUITE_SELECTOR) + sizeof (EFI_80211_SUITE_SELECTOR)
                       * (Network->CipherSuite->CipherSuiteCount - 1);
        }
        Profile->Network.CipherSuite = (EFI_80211_CIPHER_SUITE_SELECTOR *) AllocateZeroPool (DataSize);
        if (Profile->Network.CipherSuite == NULL) {
          continue;
        }
        CopyMem (Profile->Network.CipherSuite, Network->CipherSuite, DataSize);
      }
    } else {
      //
      //A duplicate one, just compare signal quality
      //
      if (Profile->NetworkQuality < NetworkDescription->NetworkQuality) {
        Profile->NetworkQuality = NetworkDescription->NetworkQuality;
      }
      continue;
    }
  }

  //
  // The current connected network should always be available until disconnection
  // happens in Wifi FW layer, even when it is not in this time's scan result.
  //
  if (Nic->ConnectState == WifiMgrConnectedToAp && Nic->CurrentOperateNetwork != NULL) {
    if (Nic->CurrentOperateNetwork->IsAvailable == FALSE) {
      Nic->CurrentOperateNetwork->IsAvailable = TRUE;
      Nic->AvailableCount ++;
    }
  }

  gBS->SignalEvent (Nic->Private->NetworkListRefreshEvent);
  WifiMgrFreeToken(ConfigToken);
}

/**
  Start scan operation, and send out a token to collect available networks.

  @param[in]  Nic                 Pointer to the device data of the selected NIC.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_ALREADY_STARTED     A former scan operation is already ongoing.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
  @retval Other Errors            Return errors when getting networks from low layer.

**/
EFI_STATUS
WifiMgrStartScan (
  IN      WIFI_MGR_DEVICE_DATA        *Nic
  )
{
  EFI_STATUS                          Status;
  EFI_TPL                             OldTpl;
  WIFI_MGR_MAC_CONFIG_TOKEN           *ConfigToken;
  EFI_80211_GET_NETWORKS_TOKEN        *GetNetworksToken;
  UINT32                              HiddenSSIdIndex;
  UINT32                              HiddenSSIdCount;
  LIST_ENTRY                          *Entry;
  WIFI_MGR_NETWORK_PROFILE_NVDATA     *Profile;
  EFI_80211_SSID                      *HiddenSSIdList;

  if (Nic == NULL || Nic->Wmp == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Nic->ScanState == WifiMgrScanning) {
    return EFI_ALREADY_STARTED;
  }

  Nic->ScanState  = WifiMgrScanning;
  OldTpl          = gBS->RaiseTPL (TPL_CALLBACK);
  HiddenSSIdIndex = 0;
  Status          = EFI_SUCCESS;

  //
  //Count how many hidden networks need to scan.
  //
  NET_LIST_FOR_EACH (Entry, &Nic->ProfileList) {
    Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                Link, WIFI_MGR_PROFILE_SIGNATURE);
    if (Profile->ScanAnyway == TRUE) {
      HiddenSSIdIndex ++;
    }
  }
  HiddenSSIdList  = NULL;
  HiddenSSIdCount = HiddenSSIdIndex;
  HiddenSSIdIndex = 0;

  //
  //create a new get network token
  //
  ConfigToken     = AllocateZeroPool (sizeof (WIFI_MGR_MAC_CONFIG_TOKEN));
  if (ConfigToken == NULL) {
    gBS->RestoreTPL (OldTpl);
    return EFI_OUT_OF_RESOURCES;
  }
  InsertTailList (&Nic->TokenList, &ConfigToken->Link);

  ConfigToken->Signature = WIFI_MGR_MAC_CONFIG_TOKEN_SIGNATURE;
  ConfigToken->Type      = TokenTypeGetNetworksToken;
  ConfigToken->Nic       = Nic;
  ConfigToken->Token.GetNetworksToken = AllocateZeroPool (sizeof (EFI_80211_GET_NETWORKS_TOKEN));
  if (ConfigToken->Token.GetNetworksToken == NULL) {
    WifiMgrFreeToken(ConfigToken);
    gBS->RestoreTPL (OldTpl);
    return EFI_OUT_OF_RESOURCES;
  }
  GetNetworksToken = ConfigToken->Token.GetNetworksToken;

  //
  //There are some hidden networks to scan, add them into scan list
  //
  if (HiddenSSIdCount > 0) {
    HiddenSSIdList = AllocateZeroPool(HiddenSSIdCount * sizeof (EFI_80211_SSID));
    if (HiddenSSIdList == NULL) {
      WifiMgrFreeToken(ConfigToken);
      gBS->RestoreTPL (OldTpl);
      return EFI_OUT_OF_RESOURCES;
    }

    HiddenSSIdIndex = 0;
    NET_LIST_FOR_EACH (Entry, &Nic->ProfileList) {
      Profile = NET_LIST_USER_STRUCT_S (Entry, WIFI_MGR_NETWORK_PROFILE_NVDATA,
                  Link, WIFI_MGR_PROFILE_SIGNATURE);
      if (Profile->ScanAnyway == TRUE) {
        HiddenSSIdList[HiddenSSIdIndex].SSIdLen = (UINT8) StrLen(Profile->SSId);
        UnicodeStrToAsciiStrS(Profile->SSId,
          (CHAR8 *)HiddenSSIdList[HiddenSSIdIndex].SSId, SSID_STORAGE_SIZE);
        HiddenSSIdIndex ++;
      }
    }
    GetNetworksToken->Data = AllocateZeroPool (sizeof (EFI_80211_GET_NETWORKS_DATA) +
                               (HiddenSSIdCount - 1) * sizeof (EFI_80211_SSID));
    if (GetNetworksToken->Data == NULL) {
      FreePool (HiddenSSIdList);
      WifiMgrFreeToken(ConfigToken);
      gBS->RestoreTPL (OldTpl);
      return EFI_OUT_OF_RESOURCES;
    }
    GetNetworksToken->Data->NumOfSSID = HiddenSSIdCount;
    CopyMem(GetNetworksToken->Data->SSIDList, HiddenSSIdList,
      HiddenSSIdCount * sizeof (EFI_80211_SSID));
    FreePool(HiddenSSIdList);
  } else {
    GetNetworksToken->Data = AllocateZeroPool (sizeof (EFI_80211_GET_NETWORKS_DATA));
    if (GetNetworksToken->Data == NULL) {
      WifiMgrFreeToken(ConfigToken);
      gBS->RestoreTPL (OldTpl);
      return EFI_OUT_OF_RESOURCES;
    }

    GetNetworksToken->Data->NumOfSSID = 0;
  }

  //
  //Create a handle when scan process ends
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  WifiMgrOnScanFinished,
                  ConfigToken,
                  &GetNetworksToken->Event
                  );
  if (EFI_ERROR (Status)) {
    WifiMgrFreeToken(ConfigToken);
    gBS->RestoreTPL (OldTpl);
    return Status;
  }

  //
  //Start scan ...
  //
  Status = Nic->Wmp->GetNetworks (Nic->Wmp, GetNetworksToken);
  if (EFI_ERROR (Status)) {
    Nic->ScanState  = WifiMgrScanFinished;
    WifiMgrFreeToken(ConfigToken);
    gBS->RestoreTPL (OldTpl);
    return Status;
  }

  gBS->RestoreTPL (OldTpl);
  return EFI_SUCCESS;
}

/**
  Configure password to supplicant before connecting to a secured network.

  @param[in]  Nic                 Pointer to the device data of the selected NIC.
  @param[in]  Profile             The target network to be connected.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
  @retval EFI_NOT_FOUND           No valid password is found to configure.
  @retval Other Errors            Returned errors when setting data to supplicant.
**/
EFI_STATUS
WifiMgrConfigPassword (
  IN    WIFI_MGR_DEVICE_DATA              *Nic,
  IN    WIFI_MGR_NETWORK_PROFILE_NVDATA   *Profile
  )
{
  EFI_STATUS                 Status;
  EFI_SUPPLICANT_PROTOCOL    *Supplicant;
  EFI_80211_SSID             SSId;
  UINT8                      *AsciiPassword;
  UINTN                      PasswordSize;
  CHAR16                     PasswordName[WIFI_STR_MAX_SIZE];

  if (Nic == NULL || Nic->Supplicant == NULL || Profile == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Supplicant = Nic->Supplicant;
  //
  //Set SSId to supplicant
  //
  SSId.SSIdLen = Profile->Network.SSId.SSIdLen;
  CopyMem(SSId.SSId, Profile->Network.SSId.SSId, sizeof (Profile->Network.SSId.SSId));
  Status = Supplicant->SetData(Supplicant,EfiSupplicant80211TargetSSIDName,
                         (void *)&SSId, sizeof(EFI_80211_SSID));
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  //Set password to supplicant
  //
  if (StrLen(Profile->Password) == 0) {
    //
    // Get password from variable
    //
    if (Profile->IsSaved == TRUE) {

      //
      // User didn't configure password, retrieve it from KMS
      //
      UnicodeSPrint (
        PasswordName,
        sizeof (PasswordName),
        L"%s-%s",
        EFI_WIFI_MGR_PSK_PASSWORD_VARIABLE,
        Profile->SSId
        );
      PasswordSize = sizeof (CHAR16) * PASSWORD_STORAGE_SIZE;
      Status = WifiMgrGetDataFromKMS (PasswordName, &PasswordSize, Profile->Password);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "ERROR: Failed to get Password for SSID: %s.\n", Profile->SSId));
      }
    }
  }

  if (StrLen(Profile->Password) < PASSWORD_MIN_LEN) {
    return EFI_NOT_FOUND;
  }
  AsciiPassword = AllocateZeroPool ((StrLen(Profile->Password) + 1) * sizeof (UINT8));
  if (AsciiPassword == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  UnicodeStrToAsciiStrS (Profile->Password, (CHAR8 *) AsciiPassword, PASSWORD_STORAGE_SIZE);
  Status = Supplicant->SetData (Supplicant, EfiSupplicant80211PskPassword,
                         AsciiPassword, (StrLen(Profile->Password) + 1) * sizeof (UINT8));
  if (EFI_ERROR(Status)) {
    FreePool(AsciiPassword);
    return Status;
  }
  FreePool(AsciiPassword);
  return Status;
}

/**
  Conduct EAP configuration to supplicant before connecting to a EAP network.

  @param[in]  Nic                 Pointer to the device data of the selected NIC.
  @param[in]  Profile             The target network to be connected.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_UNSUPPORTED         The expected EAP method is not supported.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
  @retval Other Errors            Returned errors when setting data to supplicant.
**/
EFI_STATUS
WifiMgrConfigEap (
  IN    WIFI_MGR_DEVICE_DATA              *Nic,
  IN    WIFI_MGR_NETWORK_PROFILE_NVDATA   *Profile
  )
{
  EFI_STATUS                        Status;
  EFI_EAP_CONFIGURATION_PROTOCOL    *EapConfig;
  EFI_EAP_TYPE                      EapAuthMethod;
  EFI_EAP_TYPE                      EapSecondAuthMethod;
  EFI_EAP_TYPE                      *AuthMethodList;
  CHAR8                             *Identity;
  UINTN                             IdentitySize;
  CHAR16                            *Password;
  UINTN                             PasswordSize;
  EFI_SIGNATURE_DATA                *CACert;
  UINTN                             CACertSize;
  EFI_SIGNATURE_DATA                *ClientCert;
  UINTN                             ClientCertSize;
  EFI_EAP_PRIVATE_KEY               *PrivateKey;
  UINTN                             PrivateKeySize;
  UINTN                             EncryptPasswordLen;
  CHAR8                             *AsciiEncryptPassword;
  UINTN                             AuthMethodListSize;
  UINTN                             Index;
  CHAR16                            PrivateKeyVariableName[WIFI_MGR_MAX_MAC_STRING_LEN];
  CHAR16                            PasswordName[WIFI_STR_MAX_SIZE];
  
  if (Nic == NULL || Nic->EapConfig == NULL || Profile == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  EapConfig           = Nic->EapConfig;

  EapAuthMethod       = mEapAuthMethod[Profile->EapAuthMethod];
  EapSecondAuthMethod = mEapSecondAuthMethod[Profile->EapSecondAuthMethod];

  //
  //The first time to get Supported Auth Method list, return the size.
  //
  AuthMethodListSize  = 0;
  AuthMethodList      = NULL;
  Status = EapConfig->GetData (EapConfig, EFI_EAP_TYPE_ATTRIBUTE, EfiEapConfigEapSupportedAuthMethod,
             (VOID *) AuthMethodList, &AuthMethodListSize);
  if (Status == EFI_SUCCESS) {
    //
    //No Supported Eap Auth Method
    //
    return EFI_UNSUPPORTED;
  } else if (Status != EFI_BUFFER_TOO_SMALL) {
    return Status;
  }

  //
  //The second time to get Supported Auth Method list, return the list.
  //In current design, only TTLS and PEAP are supported
  //
  AuthMethodList = (EFI_EAP_TYPE *) AllocateZeroPool(AuthMethodListSize);
  if (AuthMethodList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = EapConfig->GetData (EapConfig, EFI_EAP_TYPE_ATTRIBUTE, EfiEapConfigEapSupportedAuthMethod,
             (VOID *) AuthMethodList, &AuthMethodListSize);
  if (EFI_ERROR (Status)) {
    FreePool (AuthMethodList);
    return Status;
  }

  //
  //Check if EapAuthMethod is in supported Auth Method list, if found, skip the loop.
  //
  for (Index = 0; Index < AuthMethodListSize / sizeof (EFI_EAP_TYPE); Index ++) {
    if (EapAuthMethod == AuthMethodList[Index]) {
      break;
    }
  }
  if (Index == AuthMethodListSize / sizeof (EFI_EAP_TYPE)) {
    FreePool (AuthMethodList);
    return EFI_UNSUPPORTED;
  }
  FreePool (AuthMethodList);
  DEBUG ((DEBUG_INFO, "Get EfiEapConfigEapSupportedAuthMethod OK\n"));
  
  //
  //Set Identity to Eap peer, Mandatory field
  //
  if (StrLen(Profile->EapIdentity) == 0) {
    return EFI_INVALID_PARAMETER;
  }
  IdentitySize = sizeof(CHAR8) * (StrLen(Profile->EapIdentity) + 1);
  Identity = AllocateZeroPool (IdentitySize);
  if (Identity == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  UnicodeStrToAsciiStrS(Profile->EapIdentity, Identity, IdentitySize);
  Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_IDENTITY, EfiEapConfigIdentityString,
             (VOID *) Identity, IdentitySize - 1);
  if (EFI_ERROR(Status)) {
    FreePool (Identity);
    return Status;
  }
  FreePool (Identity);
  DEBUG ((DEBUG_INFO, "Set EfiEapConfigIdentityString OK\n"));

  //
  //Set Auth Method to Eap peer, Mandatory field
  //
  Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_ATTRIBUTE, EfiEapConfigEapAuthMethod,
             (VOID *) &EapAuthMethod, sizeof (EapAuthMethod));
  if (EFI_ERROR(Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Set EfiEapConfigEapAuthMethod OK\n"));

  if (EapAuthMethod == EFI_EAP_TYPE_TTLS) {

    //
    //For TTLS-MSChapV2, EfiEapConfigEap2ndAuthMethod, EfiEapConfigEapMSChapV2Password,
    //EfiEapConfigEapTlsCACert are required
    //
    Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_TTLS, EfiEapConfigEap2ndAuthMethod,
               (VOID *) &EapSecondAuthMethod, sizeof (EapSecondAuthMethod));
    if (EFI_ERROR(Status)) {
      return Status;
    }

    if (EapSecondAuthMethod == EFI_EAP_TYPE_MSCHAPV2) {
      //
      // Set Password to Eap peer
      //
      if (StrLen (Profile->EapPassword) == 0 && Profile->IsSaved == TRUE) {

        //
        // User didn't configure Eap password, retrieve it from KMS
        //
        UnicodeSPrint (
          PasswordName,
          sizeof (PasswordName),
          L"%s-%s",
          EFI_WIFI_MGR_EAP_PASSWORD_VARIABLE,
          Profile->SSId
          );
        PasswordSize = sizeof (CHAR16) * PASSWORD_STORAGE_SIZE;
        Status = WifiMgrGetDataFromKMS (PasswordName, &PasswordSize, Profile->EapPassword);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "ERROR: Failed to get Eap Password for SSID: %s.\n", Profile->SSId));
          return EFI_INVALID_PARAMETER;
        }
      }
      PasswordSize = sizeof (CHAR16) * (StrLen (Profile->EapPassword) + 1);
      Password = AllocateZeroPool (PasswordSize);
      if (Password == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      StrCpyS(Password, PasswordSize, Profile->EapPassword);;
      Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_MSCHAPV2, EfiEapConfigEapMSChapV2Password,
                 (VOID *) Password, PasswordSize);
      if (EFI_ERROR(Status)) {
        FreePool (Password);
        return Status;
      }
      FreePool (Password);
    }

    //
    // If CA cert is required, set it to Eap peer
    //
    if (!IsZeroGuid (&Profile->CACertGUID)) {
      CACert = GetFromCertDatabase (EFI_WIFI_MGR_CERT_VARIABLE, &Profile->CACertGUID, &CACertSize);
      if (CACert != NULL) {
        Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_TTLS, EfiEapConfigEapTlsCACert,
                   CACert->SignatureData, CACertSize);
        if (EFI_ERROR(Status)) {
          return Status;
        }
      } else {
        return EFI_INVALID_PARAMETER;
      }
    } else {
      return EFI_INVALID_PARAMETER;
    }
  } else if (EapAuthMethod == EFI_EAP_TYPE_PEAP) {

    //
    //For PEAP-MSChapV2, EfiEapConfigEap2ndAuthMethod, EfiEapConfigEapMSChapV2Password,
    //EfiEapConfigEapTlsCACert are required
    //
    Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_PEAP, EfiEapConfigEap2ndAuthMethod,
               (VOID *) &EapSecondAuthMethod, sizeof (EapSecondAuthMethod));
    if (EFI_ERROR(Status)) {
      return Status;
    }

    if (EapSecondAuthMethod == EFI_EAP_TYPE_MSCHAPV2) {
      //
      //Set Password to Eap peer
      //
      if (StrLen (Profile->EapPassword) == 0 && Profile->IsSaved == TRUE) {

        //
        // User didn't configure Eap password, retrieve it from KMS
        //
        UnicodeSPrint (
          PasswordName,
          sizeof (PasswordName),
          L"%s-%s",
          EFI_WIFI_MGR_EAP_PASSWORD_VARIABLE,
          Profile->SSId
          );
        PasswordSize = sizeof (CHAR16) * PASSWORD_STORAGE_SIZE;
        Status = WifiMgrGetDataFromKMS (PasswordName, &PasswordSize, Profile->EapPassword);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "ERROR: Failed to get Eap Password for SSID: %s.\n", Profile->SSId));
          return EFI_INVALID_PARAMETER;
        }
      }
      PasswordSize = sizeof (CHAR16) * (StrLen (Profile->EapPassword) + 1);
      Password = AllocateZeroPool (PasswordSize);
      if (Password == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      
      StrCpyS(Password, PasswordSize, Profile->EapPassword);
      Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_MSCHAPV2, EfiEapConfigEapMSChapV2Password,
                 (VOID *) Password, PasswordSize);
      if (EFI_ERROR(Status)) {
        FreePool (Password);
        return Status;
      }
      FreePool (Password);
    }

    //
    //If CA cert is required, set it to Eap peer
    //
    if (!IsZeroGuid (&Profile->CACertGUID)) {
      CACert = GetFromCertDatabase (EFI_WIFI_MGR_CERT_VARIABLE, &Profile->CACertGUID, &CACertSize);
      if (CACert != NULL) {
        Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_TTLS, EfiEapConfigEapTlsCACert,
                   CACert->SignatureData, CACertSize);
        if (EFI_ERROR(Status)) {
          return Status;
        }
      } else {
        return EFI_INVALID_PARAMETER;
      }
    } else {
      return EFI_INVALID_PARAMETER;
    }
  } else if (EapAuthMethod == EFI_EAP_TYPE_EAPTLS) {

    //
    //Set CA cert to Eap peer
    //
    if (IsZeroGuid (&Profile->CACertGUID)) {
      return EFI_INVALID_PARAMETER;
    }
    CACert = GetFromCertDatabase(EFI_WIFI_MGR_CERT_VARIABLE, &Profile->CACertGUID, &CACertSize);
    if (CACert == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_EAPTLS, EfiEapConfigEapTlsCACert,
               CACert->SignatureData, CACertSize);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    //
    //Set Client cert to Eap peer
    //
    if (IsZeroGuid (&Profile->ClientCertGUID)) {
      return EFI_INVALID_PARAMETER;
    }
    ClientCert = GetFromCertDatabase(EFI_WIFI_MGR_CERT_VARIABLE, &Profile->ClientCertGUID, &ClientCertSize);
    if (ClientCert == NULL) {
      return EFI_INVALID_PARAMETER;
    }
    Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_EAPTLS, EfiEapConfigEapTlsClientCert,
               ClientCert->SignatureData, ClientCertSize);
    if (EFI_ERROR(Status)) {
      return Status;
    }

    //
    //Set Private key to Eap peer
    //
    if (StrLen(Profile->PrivateKeyName) == 0) {
      return EFI_INVALID_PARAMETER;
    }
    PrivateKeySize = 0;
    UnicodeSPrint(
      PrivateKeyVariableName, 
      sizeof (PrivateKeyVariableName), 
      L"%s-%s",
      EFI_WIFI_MGR_PRIVATE_KEY_VARIABLE,
      Profile->PrivateKeyName);

    Status = WifiMgrGetDataFromKMS (PrivateKeyVariableName, &PrivateKeySize, NULL);
    if (Status != EFI_BUFFER_TOO_SMALL) {
      return Status;
    }
    
    PrivateKey = (EFI_EAP_PRIVATE_KEY *) AllocateZeroPool (PrivateKeySize);
    if (PrivateKey == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Status = WifiMgrGetDataFromKMS (PrivateKeyVariableName, &PrivateKeySize, PrivateKey);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = EapConfig->SetData (EapConfig, EFI_EAP_TYPE_EAPTLS, EfiEapConfigEapTlsClientPrivateKeyFile,
               PrivateKey->KeyData, PrivateKey->KeySize);
    if (EFI_ERROR(Status)) {
      FreePool (PrivateKey);
      return Status;
    }
    
    DEBUG ((DEBUG_INFO, "Set EfiEapConfigEapTlsClientPrivateKey OK\n"));

    if (PrivateKey->IsEncrypted) {
      EncryptPasswordLen = StrLen(PrivateKey->EncryptPassword);

      if (EncryptPasswordLen == 0) {
        return EFI_INVALID_PARAMETER;
      }
      AsciiEncryptPassword = AllocateZeroPool(EncryptPasswordLen + 1);
      if (AsciiEncryptPassword == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      UnicodeStrToAsciiStrS(PrivateKey->EncryptPassword, AsciiEncryptPassword, EncryptPasswordLen + 1);
      Status = EapConfig->SetData(EapConfig, EFI_EAP_TYPE_EAPTLS,
                                    EfiEapConfigEapTlsClientPrivateKeyFilePassword,
                                    (VOID *) AsciiEncryptPassword, EncryptPasswordLen + 1);
      if (EFI_ERROR(Status)) {
        FreePool (AsciiEncryptPassword);
        FreePool (PrivateKey);
        return Status;
      }
      FreePool (AsciiEncryptPassword);
      DEBUG ((DEBUG_INFO, "Set EfiEapConfigEapTlsClientPrivateKeyFilePassword OK\n"));
    }

    FreePool (PrivateKey);
  } else {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/**
  Get current link state from low layer.

  @param[in]   Nic                Pointer to the device data of the selected NIC.
  @param[out]  LinkState          The pointer to buffer to retrieve link state.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_UNSUPPORTED         Adapter information protocol is not supported.
  @retval Other Errors            Returned errors when retrieving link state from low layer.
**/
EFI_STATUS
WifiMgrGetLinkState (
  IN   WIFI_MGR_DEVICE_DATA            *Nic,
  OUT  EFI_ADAPTER_INFO_MEDIA_STATE    *LinkState
  )
{
  EFI_STATUS                           Status;
  UINTN                                DataSize;
  EFI_ADAPTER_INFO_MEDIA_STATE         *UndiState;
  EFI_ADAPTER_INFORMATION_PROTOCOL     *Aip;

  if (Nic == NULL || LinkState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Nic->Aip == NULL) {
    return EFI_UNSUPPORTED;
  }

  Aip = Nic->Aip;
  Status = Aip->GetInformation(
                  Aip,
                  &gEfiAdapterInfoMediaStateGuid,
                  (VOID **) &UndiState,
                  &DataSize
                  );

  *LinkState = *UndiState;

  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Conduct some configuration work before connecting to the target network.

  @param[in]  Nic                 Pointer to the device data of the selected NIC.
  @param[in]  Profile             The target network to be connected.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_UNSUPPORTED         This network is not supported.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.

**/
EFI_STATUS
WifiMgrPrepareConnection (
  IN    WIFI_MGR_DEVICE_DATA              *Nic,
  IN    WIFI_MGR_NETWORK_PROFILE_NVDATA   *Profile
  )
{
  EFI_STATUS           Status;
  UINT8                SecurityType;
  BOOLEAN              AKMSuiteSupported;
  BOOLEAN              CipherSuiteSupported;
  EFI_INPUT_KEY        Key;

  if (Profile == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WifiMgrCheckRSN (Profile->Network.AKMSuite, Profile->Network.CipherSuite,
             Nic, &SecurityType, &AKMSuiteSupported, &CipherSuiteSupported);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (AKMSuiteSupported && CipherSuiteSupported) {
    switch (SecurityType) {
      case SECURITY_TYPE_WPA2_PERSONAL:

        Status = WifiMgrConfigPassword (Nic, Profile);
        if (EFI_ERROR (Status)) {
          if (Status == EFI_NOT_FOUND) {
            if (Nic->OneTimeConnectRequest == TRUE) {
              CreatePopUp (
                EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
                &Key,
                L"Please make sure password is valid!",
                NULL
              );
            }
          }
          return Status;
        }
        break;

      case SECURITY_TYPE_WPA2_ENTERPRISE:

        Status = WifiMgrConfigEap (Nic, Profile);
        if (EFI_ERROR (Status)) {
          if (Status == EFI_INVALID_PARAMETER) {
            if (Nic->OneTimeConnectRequest == TRUE) {
              CreatePopUp (
                EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
                &Key,
                L"Please make sure all required information is valid!",
                NULL
              );
            }
          }
          return Status;
        }
        break;

      case SECURITY_TYPE_NONE:
        break;

      default:
        return EFI_UNSUPPORTED;
    }
  } else {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  The callback function for connect operation.

  @param[in]  Event                 The Connect token receive event.
  @param[in]  Context               The context of the connect token.
**/
VOID
EFIAPI
WifiMgrOnConnectFinished (
  IN  EFI_EVENT              Event,
  IN  VOID                   *Context
  )
{
  EFI_STATUS                         Status;
  WIFI_MGR_MAC_CONFIG_TOKEN          *ConfigToken;
  WIFI_MGR_NETWORK_PROFILE_NVDATA    *ConnectedProfile;
  UINT8                              SecurityType;
  UINT8                              SSIdLen;
  CHAR8                              *AsciiSSId;
  EFI_INPUT_KEY                      Key;

  if (Context == NULL) {
    return;
  }
  ConnectedProfile = NULL;
  ConfigToken = (WIFI_MGR_MAC_CONFIG_TOKEN*) Context;
  if (ConfigToken->Nic == NULL) {
    return;
  }

  if (ConfigToken->Signature != WIFI_MGR_MAC_CONFIG_TOKEN_SIGNATURE ||
    ConfigToken->Type != TokenTypeConnectNetworkToken ||
    ConfigToken->Token.ConnectNetworkToken == NULL) {

    ConfigToken->Nic->ConnectState          = WifiMgrDisconnected;
    ConfigToken->Nic->CurrentOperateNetwork = NULL;
    gBS->SignalEvent (ConfigToken->Nic->Private->ConnectFormRefreshEvent);
    ConfigToken->Nic->OneTimeConnectRequest = FALSE;
    UpdateConnectStatusStrToken ();
    return;
  }

  if (ConfigToken->Token.ConnectNetworkToken->Status != EFI_SUCCESS) {
    if (ConfigToken->Nic->OneTimeConnectRequest) {
      //
      // Only popup error message for user triggered connection
      //
      if (ConfigToken->Token.ConnectNetworkToken->Status == EFI_ACCESS_DENIED) {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Can't connect to this network: Permission Denied!",
          NULL
        );
      } else {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Can't connect to this network: Unexpected Error!",
          NULL
        );
      }
    }

    ConfigToken->Nic->ConnectState          = WifiMgrDisconnected;
    ConfigToken->Nic->CurrentOperateNetwork = NULL;
    gBS->SignalEvent (ConfigToken->Nic->Private->ConnectFormRefreshEvent);
    ConfigToken->Nic->OneTimeConnectRequest = FALSE;
    UpdateConnectStatusStrToken ();
    return;
  }

  if (ConfigToken->Token.ConnectNetworkToken->ResultCode != ConnectSuccess) {

    if (ConfigToken->Nic->OneTimeConnectRequest) {
      //
      // Only popup error message for user triggered connection
      //
      if (ConfigToken->Token.ConnectNetworkToken->ResultCode ==
        ConnectFailedReasonUnspecified) {
        
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Can't connect to this network: Wrong Password!",
          NULL
        );
      } else {
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Can't connect to this network: Connection Failed!",
          NULL
        );
      }
    }

    ConfigToken->Nic->ConnectState          = WifiMgrDisconnected;
    ConfigToken->Nic->CurrentOperateNetwork = NULL;
    gBS->SignalEvent (ConfigToken->Nic->Private->ConnectFormRefreshEvent);
    goto Exit;
  }
  if (ConfigToken->Token.ConnectNetworkToken->Data == NULL ||
    ConfigToken->Token.ConnectNetworkToken->Data->Network == NULL) {
    //
    //An unexpected error occurs, tell UNDI to disconnect
    //
    ConfigToken->Nic->HasDisconnectPendingNetwork = TRUE;
    goto Exit;
  }

  //
  //A correct connect token received, end the connect process
  //
  ConfigToken->Nic->ConnectState  = WifiMgrConnectedToAp;
  Status = WifiMgrCheckRSN(ConfigToken->Token.ConnectNetworkToken->Data->Network->AKMSuite,
             ConfigToken->Token.ConnectNetworkToken->Data->Network->CipherSuite,
             ConfigToken->Nic, &SecurityType, NULL, NULL);
  if (EFI_ERROR(Status)) {
    SecurityType = SECURITY_TYPE_UNKNOWN;
  }

  SSIdLen   = ConfigToken->Token.ConnectNetworkToken->Data->Network->SSId.SSIdLen;
  AsciiSSId = (CHAR8*) AllocateZeroPool(sizeof (CHAR8) * (SSIdLen + 1));
  if (AsciiSSId == NULL) {
    ConfigToken->Nic->HasDisconnectPendingNetwork = TRUE;
    goto Exit;
  }

  CopyMem(AsciiSSId, ConfigToken->Token.ConnectNetworkToken->Data->Network->SSId.SSId, SSIdLen);
  *(AsciiSSId + SSIdLen) = '\0';

  ConnectedProfile = WifiMgrGetProfileByAsciiSSId(AsciiSSId, SecurityType,
                       &ConfigToken->Nic->ProfileList);
  FreePool(AsciiSSId);
  if (ConnectedProfile == NULL) {
    ConfigToken->Nic->HasDisconnectPendingNetwork = TRUE;
    goto Exit;
  }
  //
  // Connect success, try to save this network.
  //
  Status = WifiMgrWriteProfileDataToNv (ConfigToken->Nic, ConnectedProfile, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Failed to save this network!\n"));
  } else {
    ConnectedProfile->IsSaved = TRUE;

    Status = WifiMgrUpdatePriorityList (ConfigToken->Nic);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Failed to update profile priority list!\n"));
    }
  }
  ConfigToken->Nic->ConnectState        = WifiMgrConnectedToAp;
  ConfigToken->Nic->ConnectStateChanged = TRUE;

Exit:
  ConfigToken->Nic->OneTimeConnectRequest = FALSE;
  WifiMgrFreeToken(ConfigToken);
  UpdateConnectStatusStrToken ();
}

/**
  Start connect operation, and send out a token to connect to a target network.

  @param[in]  Nic                 Pointer to the device data of the selected NIC.
  @param[in]  Profile             The target network to be connected.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_ALREADY_STARTED     Already in "connected" state, need to perform a disconnect
                                  operation first.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
  @retval Other Errors            Return errors when connecting network on low layer.

**/
EFI_STATUS
WifiMgrConnectToNetwork (
  IN    WIFI_MGR_DEVICE_DATA              *Nic,
  IN    WIFI_MGR_NETWORK_PROFILE_NVDATA   *Profile
  )
{
  EFI_STATUS                             Status;
  EFI_TPL                                OldTpl;
  EFI_ADAPTER_INFO_MEDIA_STATE           LinkState;
  WIFI_MGR_MAC_CONFIG_TOKEN              *ConfigToken;
  EFI_80211_CONNECT_NETWORK_TOKEN        *ConnectToken;
  EFI_INPUT_KEY                          Key;

  if (Nic == NULL || Nic->Wmp == NULL || Profile == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WifiMgrGetLinkState (Nic, &LinkState);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (LinkState.MediaState == EFI_SUCCESS) {
    return EFI_ALREADY_STARTED;
  }

  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  Status = WifiMgrPrepareConnection (Nic, Profile);
  if (EFI_ERROR (Status)) {
    gBS->RestoreTPL (OldTpl);
    return Status;
  }

  //
  //create a new connect token
  //
  ConfigToken = AllocateZeroPool (sizeof (WIFI_MGR_MAC_CONFIG_TOKEN));
  if (ConfigToken == NULL) {
    gBS->RestoreTPL (OldTpl);
    return EFI_OUT_OF_RESOURCES;
  }
  InsertTailList (&Nic->TokenList, &ConfigToken->Link);

  ConfigToken->Signature = WIFI_MGR_MAC_CONFIG_TOKEN_SIGNATURE;
  ConfigToken->Type      = TokenTypeConnectNetworkToken;
  ConfigToken->Nic       = Nic;
  ConfigToken->Token.ConnectNetworkToken  = AllocateZeroPool (sizeof (EFI_80211_CONNECT_NETWORK_TOKEN));
  if (ConfigToken->Token.ConnectNetworkToken == NULL) {
    WifiMgrFreeToken(ConfigToken);
    gBS->RestoreTPL (OldTpl);
    return EFI_OUT_OF_RESOURCES;
  }

  ConnectToken           = ConfigToken->Token.ConnectNetworkToken;
  ConnectToken->Data     = AllocateZeroPool (sizeof (EFI_80211_CONNECT_NETWORK_DATA));
  if (ConnectToken->Data == NULL) {
    WifiMgrFreeToken(ConfigToken);
    gBS->RestoreTPL (OldTpl);
    return EFI_OUT_OF_RESOURCES;
  }

  ConnectToken->Data->Network = AllocateZeroPool (sizeof (EFI_80211_NETWORK));
  if (ConnectToken->Data->Network == NULL) {
    WifiMgrFreeToken(ConfigToken);
    gBS->RestoreTPL (OldTpl);
    return EFI_OUT_OF_RESOURCES;
  }

  ConnectToken->Data->FailureTimeout = 100;
  CopyMem(ConnectToken->Data->Network, &Profile->Network, sizeof (EFI_80211_NETWORK));

  //
  //add event handle and start to connect
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  WifiMgrOnConnectFinished,
                  ConfigToken,
                  &ConnectToken->Event
                  );
  if (EFI_ERROR (Status)) {
    WifiMgrFreeToken(ConfigToken);
    gBS->RestoreTPL (OldTpl);
    return Status;
  }

  Nic->ConnectState          = WifiMgrConnectingToAp;

  //
  // Only connect form cares status transfer: Diconnected -> Connecting
  //
  Nic->CurrentOperateNetwork = Profile;
  gBS->SignalEvent (Nic->Private->ConnectFormRefreshEvent);

  //
  //Start Connecting ...
  //
  Status = Nic->Wmp->ConnectNetwork (Nic->Wmp, ConnectToken);
  if (EFI_ERROR (Status)) {
    if (Status == EFI_ALREADY_STARTED) {
      Nic->ConnectState         = WifiMgrConnectedToAp;
      Nic->ConnectStateChanged  = TRUE;
    } else {

      Nic->ConnectState          = WifiMgrDisconnected;
      Nic->CurrentOperateNetwork = NULL;
      gBS->SignalEvent (Nic->Private->ConnectFormRefreshEvent);

      if (Nic->OneTimeConnectRequest) {
        if (Status == EFI_NOT_FOUND) {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"Can't connect to this network: Not Available!",
            NULL
          );
        } else {
          CreatePopUp (
            EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
            &Key,
            L"Can't connect to this network: Unexpected Error!",
            NULL
          );
        }
      }
    }
    WifiMgrFreeToken(ConfigToken);
    UpdateConnectStatusStrToken ();
    gBS->RestoreTPL (OldTpl);
    return Status;
  }

  gBS->RestoreTPL (OldTpl);
  return Status;
}

/**
  The callback function for disconnect operation.

  @param[in]  Event                 The Disconnect token receive event.
  @param[in]  Context               The context of the Disconnect token.

**/
VOID
EFIAPI
WifiMgrOnDisconnectFinished (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  )
{
  WIFI_MGR_MAC_CONFIG_TOKEN         *ConfigToken;

  if (Context == NULL) {
    return;
  }

  ConfigToken = (WIFI_MGR_MAC_CONFIG_TOKEN*) Context;
  if (ConfigToken->Nic == NULL) {
    return;
  }

  if (ConfigToken->Signature != WIFI_MGR_MAC_CONFIG_TOKEN_SIGNATURE ||
      ConfigToken->Type != TokenTypeDisconnectNetworkToken) {
    ConfigToken->Nic->ConnectState          = WifiMgrConnectedToAp;
    ConfigToken->Nic->OneTimeDisconnectRequest = FALSE;
    gBS->SignalEvent (ConfigToken->Nic->Private->ConnectFormRefreshEvent);
    return;
  }

  if (ConfigToken->Token.DisconnectNetworkToken->Status != EFI_SUCCESS) {
    ConfigToken->Nic->ConnectState          = WifiMgrConnectedToAp;
    ConfigToken->Nic->OneTimeDisconnectRequest = FALSE;
    gBS->SignalEvent (ConfigToken->Nic->Private->ConnectFormRefreshEvent);
    goto Exit;
  }

  ConfigToken->Nic->ConnectState          = WifiMgrDisconnected;
  ConfigToken->Nic->CurrentOperateNetwork = NULL;
  ConfigToken->Nic->ConnectStateChanged   = TRUE;
  ConfigToken->Nic->OneTimeDisconnectRequest = FALSE;
  UpdateConnectStatusStrToken ();

  Exit:
    WifiMgrFreeToken(ConfigToken);
    return;
}

/**
  Start disconnect operation, and send out a token to disconnect from current connected
  network.

  @param[in]  Nic                 Pointer to the device data of the selected NIC.

  @retval EFI_SUCCESS             The operation is completed.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval Other Errors            Return errors when disconnecting a network on low layer.

**/
EFI_STATUS
WifiMgrDisconnectToNetwork (
  IN    WIFI_MGR_DEVICE_DATA             *Nic
  )
{
  EFI_STATUS                             Status;
  EFI_TPL                                OldTpl;
  WIFI_MGR_MAC_CONFIG_TOKEN              *ConfigToken;
  EFI_80211_DISCONNECT_NETWORK_TOKEN     *DisconnectToken;
  EFI_INPUT_KEY                          Key;

  if (Nic == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OldTpl      = gBS->RaiseTPL (TPL_CALLBACK);
  Status      = EFI_SUCCESS;
  ConfigToken = AllocateZeroPool (sizeof (WIFI_MGR_MAC_CONFIG_TOKEN));
  if (ConfigToken == NULL) {
    gBS->RestoreTPL (OldTpl);
    return EFI_OUT_OF_RESOURCES;
  }
  InsertTailList (&Nic->TokenList, &ConfigToken->Link);

  ConfigToken->Signature = WIFI_MGR_MAC_CONFIG_TOKEN_SIGNATURE;
  ConfigToken->Type      = TokenTypeDisconnectNetworkToken;
  ConfigToken->Nic       = Nic;
  ConfigToken->Token.DisconnectNetworkToken = AllocateZeroPool (sizeof (EFI_80211_DISCONNECT_NETWORK_TOKEN));
  if (ConfigToken->Token.DisconnectNetworkToken == NULL) {
    WifiMgrFreeToken(ConfigToken);
    gBS->RestoreTPL (OldTpl);
    return EFI_OUT_OF_RESOURCES;
  }

  DisconnectToken = ConfigToken->Token.DisconnectNetworkToken;

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  WifiMgrOnDisconnectFinished,
                  ConfigToken,
                  &DisconnectToken->Event
                  );
  if (EFI_ERROR (Status)) {
    WifiMgrFreeToken(ConfigToken);
    gBS->RestoreTPL (OldTpl);
    return Status;
  }

  Nic->ConnectState = WifiMgrDisconnectingToAp;
  gBS->SignalEvent (Nic->Private->ConnectFormRefreshEvent);

  Status = Nic->Wmp->DisconnectNetwork (Nic->Wmp, DisconnectToken);
  if (EFI_ERROR (Status)) {
    if (Status == EFI_NOT_FOUND) {

      Nic->ConnectState          = WifiMgrDisconnected;
      Nic->CurrentOperateNetwork = NULL;
      Nic->ConnectStateChanged   = TRUE;
      Status                     = EFI_SUCCESS;
      UpdateConnectStatusStrToken ();
    } else {

      if (Nic->OneTimeDisconnectRequest == TRUE) {
        //
        //This disconnect is triggered by user, popup error message
        //
        CreatePopUp (
          EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
          &Key,
          L"Can't disconnect from current connected network: Unexpected Error!",
          NULL
        );
      }
       
      Nic->ConnectState     = WifiMgrConnectedToAp;
      gBS->SignalEvent (Nic->Private->ConnectFormRefreshEvent);
    }
    WifiMgrFreeToken(ConfigToken);
  }

  gBS->RestoreTPL (OldTpl);
  return Status;
}

/**
  The state machine of the connection manager, periodically check the state and
  perform a corresponding operation.

  @param[in]  Event                   The timer event to be triggered.
  @param[in]  Context                 The context of the Nic device data.

**/
VOID
EFIAPI
WifiMgrOnTimerTick (
  IN EFI_EVENT                        Event,
  IN VOID                             *Context
  )
{
  WIFI_MGR_DEVICE_DATA                *Nic;
  EFI_STATUS                          Status;
  EFI_ADAPTER_INFO_MEDIA_STATE        LinkState;
  WIFI_MGR_NETWORK_PROFILE_NVDATA     *Profile;

  if (Context == NULL) {
    return;
  }

  Nic = (WIFI_MGR_DEVICE_DATA*) Context;
  NET_CHECK_SIGNATURE (Nic, WIFI_MGR_DEVICE_DATA_SIGNATURE);

  Status = WifiMgrGetLinkState (Nic, &LinkState);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "WifiMgrOnTimerTick, failed to get link state\n"));
    return;
  }

  if (Nic->LastLinkState.MediaState != LinkState.MediaState) {
    if (Nic->LastLinkState.MediaState == EFI_SUCCESS && LinkState.MediaState == EFI_NO_MEDIA) {
      Nic->HasDisconnectPendingNetwork = TRUE;
    }
    Nic->LastLinkState.MediaState = LinkState.MediaState;
  }

  if (Nic->Private != NULL && Nic->ConnectStateChanged == TRUE) {

    gBS->SignalEvent (Nic->Private->ConnectFormRefreshEvent);
    gBS->SignalEvent (Nic->Private->MainPageRefreshEvent);
    gBS->SignalEvent (Nic->Private->NetworkListRefreshEvent);
    Nic->ConnectStateChanged = FALSE;
  }

  Nic->ScanTickTime ++;
  if ((Nic->ScanTickTime > WIFI_SCAN_FREQUENCY || Nic->OneTimeScanRequest == TRUE) &&
    Nic->ScanState == WifiMgrScanFinished) {

    Nic->OneTimeScanRequest = FALSE;
    Nic->ScanTickTime = 0;

    DEBUG ((DEBUG_INFO, "WifiMgrOnTimerTick: trigger scan process\n"));
    WifiMgrStartScan (Nic);
  }

  if (Nic->AvailableCount > 0 && Nic->ScanState == WifiMgrScanFinished) {

    switch (Nic->ConnectState) {
    case WifiMgrDisconnected:

      if (Nic->HasDisconnectPendingNetwork == TRUE) {

        Nic->HasDisconnectPendingNetwork = FALSE;
      }

      if (Nic->ConnectPendingNetwork != NULL) {
        Profile   = Nic->ConnectPendingNetwork;
        Status    = WifiMgrConnectToNetwork(Nic, Profile);
        Nic->ConnectPendingNetwork = NULL;
        if (EFI_ERROR (Status)) {
          //
          // Some error happened, don't wait for a return connect token!
          //
          Nic->OneTimeConnectRequest = FALSE;
        }
        DEBUG ((DEBUG_INFO, "WifiMgrConnectToNetwork Status: %d\n", Status));
      } else {
        //
        // Will auto connect to a saved network!
        //
        Nic->AutoSelectedProfile = WifiMgrSelectNextProfile (Nic, Nic->AutoSelectedProfile);
        if (Nic->AutoSelectedProfile == NULL) {
          break;
        }
        Status    = WifiMgrConnectToNetwork (Nic, Nic->AutoSelectedProfile);
        DEBUG ((DEBUG_INFO, "WifiMgrConnectToNetwork Status: %d\n", Status));
      }
      break;

    case WifiMgrConnectingToAp:
      break;

    case WifiMgrDisconnectingToAp:
      break;

    case WifiMgrConnectedToAp:

      if (Nic->ConnectPendingNetwork != NULL ||
            Nic->HasDisconnectPendingNetwork == TRUE) {

        Status    = WifiMgrDisconnectToNetwork(Nic);
        if (EFI_ERROR (Status)) {
          //
          //Some error happened, don't wait for a return disconnect token!
          //
          Nic->OneTimeDisconnectRequest = FALSE;
        }
        DEBUG ((DEBUG_INFO, "WifiMgrDisconnectToNetwork Status: %d\n", Status));
      }
      break;

    default:
      break;
    }
  }
}
