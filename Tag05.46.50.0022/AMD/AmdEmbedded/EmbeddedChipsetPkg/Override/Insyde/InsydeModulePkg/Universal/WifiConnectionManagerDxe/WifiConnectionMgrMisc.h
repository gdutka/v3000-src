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

#ifndef __EFI_WIFI_MGR_MISC_H__
#define __EFI_WIFI_MGR_MISC_H__

/**
  Empty function for event process function.

  @param[in] Event    The Event needs to be processed
  @param[in] Context  The context of the event

**/
VOID
EFIAPI
WifiMgrInternalEmptyFunction (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  );

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
  );

/**
  Convert the mac address into a hexadecimal encoded ":" seperated string.

  @param[in]  Mac     The mac address
  @param[in]  StrSize The size, in bytes, of the output buffer specified by Str
  @param[out] Str     The storage to return the mac string

**/
VOID
WifiMgrMacAddrToStr (
  IN  EFI_80211_MAC_ADDRESS     *Mac,
  IN  UINT32                    StrSize,
  OUT CHAR16                    *Str
  );

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
  );

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
  );

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
  );

/**
  Get the profile order list data for a Nic from variable storage.

  @param[in]      Nic           To identify which Nic the target profile order list belongs 
                                to, this list will be set to the Nic's profileOrder field.
 
  @retval EFI_SUCCESS           The profile order list data is written successfully.
  @retval EFI_OUT_OF_RESOURCES  Fail to allocate resource to complete the operation.
  @retval Others Errors         Returned errors when getting profile list from storage.

**/
EFI_STATUS
WifiMgrGetProfileDataFromNv (
  IN  WIFI_MGR_DEVICE_DATA      *Nic
  );

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
  );

/**
  Write the profile priority list to variable storage.

  @param[in]      Nic               To identify which Nic the profile order data belongs to.

  @retval EFI_SUCCESS               The profile order list data is written successfully.
  @retval EFI_OUT_OF_RESOURCES      Fail to allocate resource to complete the operation.
  @retval Other Errors              Returned error when setting variable to variable storage.

**/
EFI_STATUS
WifiMgrUpdatePriorityList (
  IN  WIFI_MGR_DEVICE_DATA      *Nic
  );


/**
  Get the Nic data by the NicIndex.

  @param[in]  Private        The pointer to the global private data structure.
  @param[in]  NicIndex       The index indicates the position of wireless NIC.

  @return     Pointer to the Nic data, or NULL if not found.

**/
WIFI_MGR_DEVICE_DATA *
WifiMgrGetNicByIndex (
  IN WIFI_MGR_PRIVATE_DATA      *Private,
  IN UINT32                     NicIndex
  );

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
  );

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
  );

/**
  Find a network profile through its' SSId and securit type, and the SSId is an ascii string.

  @param[in]  SSId                   The target network's SSId.
  @param[in]  SecurityType           The target network's security type.
  @param[in]  ProfileList            The profile list on a Nic.

  @return Pointer to a network profile, or NULL if not found.

**/
WIFI_MGR_NETWORK_PROFILE_NVDATA *
WifiMgrGetProfileByAsciiSSId (
  IN  CHAR8                          *SSId,
  IN  UINT8                          SecurityType,
  IN  LIST_ENTRY                     *ProfileList
  );

/**
  Find a network profile through its' profile index.

  @param[in]  ProfileIndex           The target network's profile index.
  @param[in]  ProfileList            The profile list on a Nic.

  @return Pointer to a network profile, or NULL if not found.

**/
WIFI_MGR_NETWORK_PROFILE_NVDATA *
WifiMgrGetProfileByProfileIndex (
  IN UINT32                     ProfileIndex,
  IN LIST_ENTRY                 *ProfileList
  );

/**
  To test if the AKMSuite is in supported AKMSuite list.

  @param[in]  SupportedAKMSuiteCount      The count of the supported AKMSuites.
  @param[in]  SupportedAKMSuiteList       The supported AKMSuite list.
  @param[in]  AKMSuite                    The AKMSuite to be tested.

  @return True if this AKMSuite is supported, or False if not.

**/
BOOLEAN
WifiMgrSupportAKMSuite (
  IN  UINT16                              SupportedAKMSuiteCount,
  IN  UINT32                              *SupportedAKMSuiteList,
  IN  UINT32                              *AKMSuite
  );

/**
  To check if the CipherSuite is in supported CipherSuite list.

  @param[in]  SupportedCipherSuiteCount   The count of the supported CipherSuites.
  @param[in]  SupportedCipherSuiteList    The supported CipherSuite list.
  @param[in]  CipherSuite                 The CipherSuite to be tested.

  @return True if this CipherSuite is supported, or False if not.

**/
BOOLEAN
WifiMgrSupportCipherSuite (
  IN  UINT16                              SupportedCipherSuiteCount,
  IN  UINT32                              *SupportedCipherSuiteList,
  IN  UINT32                              *CipherSuite
  );

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
  IN    EFI_80211_AKM_SUITE_SELECTOR      *AKMList,
  IN    EFI_80211_CIPHER_SUITE_SELECTOR   *CipherList,
  IN    WIFI_MGR_DEVICE_DATA              *Nic,
  OUT   UINT8                             *SecurityType,
  OUT   BOOLEAN                           *AKMSuiteSupported,
  OUT   BOOLEAN                           *CipherSuiteSupported
  );

/**
  To get the security type for a certain AKMSuite and CipherSuite.

  @param[in]   AKMSuite             An certain AKMSuite.
  @param[in]   CipherSuite          An certain CipherSuite.

  @return a security type if found, or SECURITY_TYPE_UNKNOWN.

**/
UINT8
WifiMgrGetSecurityType (
  IN    UINT32                          *AKMSuite,
  IN    UINT32                          *CipherSuite
  );

/**
  Get supported AKMSuites and CipherSuites from supplicant.

  @param[in]   Nic                      The Nic to operate.

  @retval EFI_SUCCESS                   Get the supported suite list successfully.
  @retval EFI_INVALID_PARAMETER         No Nic found or supplicant is NULL.
**/
EFI_STATUS
WifiMgrGetSupportedSuites (
  IN    WIFI_MGR_DEVICE_DATA            *Nic
  );

/**
  Free all network profiles in a profile list.

  @param[in]   ProfileList           The profile list to be freed.
**/
VOID
WifiMgrFreeProfileList (
  IN  LIST_ENTRY               *ProfileList
  );

/**
  Free the resources of a config token.

  @param[in]   ConfigToken          The config token to be freed.
**/
VOID
WifiMgrFreeToken (
  IN   WIFI_MGR_MAC_CONFIG_TOKEN        *ConfigToken
  );

/**
  Free all tokens in a token list.

  @param[in]   TokenList          The token list to be freed.
**/
VOID
WifiMgrFreeTokenList (
  IN   LIST_ENTRY              *TokenList
  );

#endif
