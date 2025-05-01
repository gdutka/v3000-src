/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
  Define network structure used by the Wireless Connection Manager.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef _WIFI_MGR_CONFIG_H_
#define _WIFI_MGR_CONFIG_H_

#include "WifiConnectionMgrConfigNVDataStruct.h"

extern UINT8        WifiConnectionManagerDxeBin[];
extern UINT8        WifiConnectionManagerDxeStrings[];

#define WIFI_MGR_MAX_MAC_STRING_LEN     96

#define VAR_OFFSET(Field)    \
  ((UINT16) ((UINTN) &(((WIFI_MANAGER_IFR_NVDATA *) 0)->Field)))

#define QUESTION_ID(Field)   \
  ((UINT16) (VAR_OFFSET (Field) + CONFIG_OPTION_OFFSET))

#define DYNAMIC_PRIORITY_LIST_QUESTION_ID    QUESTION_ID (DynamicPriorityList)
#define DYNAMIC_PRIORITY_LIST_VAR_OFFSET     VAR_OFFSET  (DynamicPriorityList)

//
// Save as EFI variable
// Name: MAC+ProfileIndex   eg: 11:22:33:44:55:66-9
//
typedef struct {
  UINT32               Signature;

  //
  // Link to the current profile list in NIC device data (WIFI_MGR_DEVICE_DATA)
  //
  LIST_ENTRY           Link;

  UINT32               NicIndex;
  UINT32               ProfileIndex;   // The unique identifier for network profile, starts from 1
  CHAR16               SSId[SSID_STORAGE_SIZE];
  CHAR16               PasswordName[SSID_STORAGE_SIZE];    //The key identifier for password storage
  CHAR16               Password[PASSWORD_STORAGE_SIZE];

  UINT8                SecurityType;
  UINT8                EapAuthMethod;
  EFI_GUID             CACertGUID;  //Identifier for CA cert
  CHAR16               CACertName[WIFI_FILENAME_STR_MAX_SIZE];
  EFI_GUID             ClientCertGUID;  //Identifier for Client cert
  CHAR16               ClientCertName[WIFI_FILENAME_STR_MAX_SIZE];
  CHAR16               PrivateKeyName[WIFI_FILENAME_STR_MAX_SIZE];
  CHAR16               PrivateKeyPassword[PASSWORD_STORAGE_SIZE];    //Password to protect private key file
  CHAR16               EapIdentity[EAP_IDENTITY_SIZE];
  CHAR16               EapPassword[PASSWORD_STORAGE_SIZE];
  UINT8                EapSecondAuthMethod;

  BOOLEAN              AutoConnect;
  BOOLEAN              ScanAnyway;
  BOOLEAN              AKMSuiteSupported;
  BOOLEAN              CipherSuiteSupported;
  BOOLEAN              IsSaved;
  BOOLEAN              IsAvailable;
  EFI_80211_NETWORK    Network;
  UINT8                NetworkQuality;
  EFI_STRING_ID        TitleToken;

} WIFI_MGR_NETWORK_PROFILE_NVDATA;

#define WIFI_MGR_PROFILE_SIGNATURE  SIGNATURE_32 ('W','M','N','P')

#pragma pack(1)
///
/// HII specific Vendor Device Path definition.
///
typedef struct {
  VENDOR_DEVICE_PATH               VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL         End;
} HII_VENDOR_DEVICE_PATH;
#pragma pack()

#endif
