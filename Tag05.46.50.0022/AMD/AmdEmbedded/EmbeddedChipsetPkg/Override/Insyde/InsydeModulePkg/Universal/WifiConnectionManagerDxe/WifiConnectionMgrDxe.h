/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 The miscellaneous structure definitions for wireless connection driver.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef __EFI_WIFI_MGR_DXE_H__
#define __EFI_WIFI_MGR_DXE_H__

#include <Uefi.h>

//
// Libraries
//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/NetLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiHiiServicesLib.h>
#include <Library/FileExplorerLib.h>

#include <Library/UefiBootServicesTableLib.h>
//
// UEFI Driver Model Protocols
//
#include <Protocol/DriverBinding.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiPackageList.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/ComponentName.h>

//
// Consumed Protocols
//
#include <Protocol/WiFi2.h>
#include <Protocol/AdapterInformation.h>
#include <Protocol/Supplicant.h>
#include <Protocol/EapConfiguration.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/Kms.h>
#include <Protocol/SimpleFileSystem.h>

#include <Protocol/CryptoServices.h>
#include <Protocol/H2oDialog.h>
#include <Protocol/FormBrowser2.h>
#include <Protocol/FirmwareVolume2.h>

//
// Produced Protocols
//
#include <Protocol/HiiConfigAccess.h>

//
// Guids
//
#include <Guid/ImageAuthentication.h>
#include <Guid/MdeModuleHii.h>

//
// NvData struct definition
//
#include "WifiConnectionMgrConfigNVDataStruct.h"
#include "WifiConnectionMgrConfig.h"
#include "EapContext.h"
#include "Kms/KmsServiceSample.h"
#include "WifiConnectionMgrConfigHii.h"

//
// Driver Version
//
#define WIFI_MGR_DXE_VERSION  0xa

//---------------------

#pragma pack (1)

#define OUI_IEEE_80211I 0xAC0F00

typedef enum {
  Ieee80211PairwiseCipherSuiteUseGroupCipherSuite = 0,
  Ieee80211PairwiseCipherSuiteWEP40  = 1,
  Ieee80211PairwiseCipherSuiteTKIP   = 2,
  Ieee80211PairwiseCipherSuiteCCMP   = 4,
  Ieee80211PairwiseCipherSuiteWEP104 = 5,
  Ieee80211PairwiseCipherSuiteBIP    = 6,
  //...
} IEEE_80211_PAIRWISE_CIPHER_SUITE;

#define IEEE_80211_PAIRWISE_CIPHER_SUITE_USE_GROUP   (OUI_IEEE_80211I | (Ieee80211PairwiseCipherSuiteUseGroupCipherSuite << 24))
#define IEEE_80211_PAIRWISE_CIPHER_SUITE_WEP40       (OUI_IEEE_80211I | (Ieee80211PairwiseCipherSuiteWEP40 << 24))
#define IEEE_80211_PAIRWISE_CIPHER_SUITE_TKIP        (OUI_IEEE_80211I | (Ieee80211PairwiseCipherSuiteTKIP << 24))
#define IEEE_80211_PAIRWISE_CIPHER_SUITE_CCMP        (OUI_IEEE_80211I | (Ieee80211PairwiseCipherSuiteCCMP << 24))
#define IEEE_80211_PAIRWISE_CIPHER_SUITE_WEP104      (OUI_IEEE_80211I | (Ieee80211PairwiseCipherSuiteWEP104 << 24))
#define IEEE_80211_PAIRWISE_CIPHER_SUITE_BIP         (OUI_IEEE_80211I | (Ieee80211PairwiseCipherSuiteBIP << 24))

typedef enum {
  Ieee80211AkmSuite8021XOrPMKSA       = 1,
  Ieee80211AkmSuitePSK                = 2,
  Ieee80211AkmSuite8021XOrPMKSASHA256 = 5,
  Ieee80211AkmSuitePSKSHA256          = 6
  //...
} IEEE_80211_AKM_SUITE;

#define IEEE_80211_AKM_SUITE_8021X_OR_PMKSA         (OUI_IEEE_80211I | (Ieee80211AkmSuite8021XOrPMKSA << 24))
#define IEEE_80211_AKM_SUITE_PSK                    (OUI_IEEE_80211I | (Ieee80211AkmSuitePSK << 24))
#define IEEE_80211_AKM_SUITE_8021X_OR_PMKSA_SHA256  (OUI_IEEE_80211I | (Ieee80211AkmSuite8021XOrPMKSASHA256 << 24))
#define IEEE_80211_AKM_SUITE_PSK_SHA256             (OUI_IEEE_80211I | (Ieee80211AkmSuitePSKSHA256 << 24))

//------------------------------

#pragma pack ()

#define EFI_WIFI_MGR_PSK_PASSWORD_VARIABLE  L"WifiMgrPskPassword"
#define EFI_WIFI_MGR_EAP_PASSWORD_VARIABLE  L"WifiMgrEapPassword"
#define EFI_WIFI_MGR_PRIVATE_KEY_VARIABLE   L"WifiMgrPrivateKey"
#define EFI_WIFI_MGR_CERT_VARIABLE          L"WifiMgrCert"

//
// Protocol instances
//
extern EFI_DRIVER_BINDING_PROTOCOL       gWifiMgrDxeDriverBinding;
extern EFI_COMPONENT_NAME2_PROTOCOL      gWifiMgrDxeComponentName2;
extern EFI_COMPONENT_NAME_PROTOCOL       gWifiMgrDxeComponentName;
extern EFI_HII_CONFIG_ACCESS_PROTOCOL    gWifiMgrDxeHiiConfigAccess;

//
// Private Context Data Structure
//
typedef enum {
  WifiMgrDisconnected,
  WifiMgrConnectingToAp,
  WifiMgrConnectedToAp,
  WifiMgrDisconnectingToAp,
  WifiMgrConnectStateMaximum
} WIFI_MGR_CONNECT_STATE;

typedef enum {
  WifiMgrScanFinished,
  WifiMgrScanning,
  WifiMgrScanStateMaximum
} WIFI_MGR_SCAN_STATE;

#define  WIFI_SCAN_FREQUENCY    30

typedef struct _WIFI_MGR_SUPPORTED_SUITES {
  EFI_80211_AKM_SUITE_SELECTOR     *SupportedAKMSuites;
  EFI_80211_CIPHER_SUITE_SELECTOR  *SupportedSwCipherSuites;
  EFI_80211_CIPHER_SUITE_SELECTOR  *SupportedHwCipherSuites;
} WIFI_MGR_SUPPORTED_SUITES;

#define EFI_WIFIMGR_PRIVATE_GUID \
  { \
    0x99b7c019, 0x4789, 0x4829, { 0xa7, 0xbd, 0x0d, 0x4b, 0xaa, 0x62, 0x28, 0x72 } \
  }

typedef struct _WIFI_MGR_PRIVATE_DATA  WIFI_MGR_PRIVATE_DATA;

typedef struct _WIFI_MGR_PRIVATE_PROTOCOL {
  UINT32  Reserved;
} WIFI_MGR_PRIVATE_PROTOCOL;

typedef struct _WIFI_MGR_FILE_CONTEXT {
  EFI_FILE_HANDLE                   FHandle;
  UINT16                            *FileName;
} WIFI_MGR_FILE_CONTEXT;

typedef enum {
  FileTypeCACert,
  FileTypeClientCert,
  FileTypeMax
} WIFI_MGR_FILE_TYPE;

typedef struct {
  UINT32                                     Signature;
  EFI_HANDLE                                 DriverHandle;
  EFI_HANDLE                                 ControllerHandle;
  EFI_EVENT                                  TickTimer;
  WIFI_MGR_PRIVATE_DATA                      *Private;

  //
  // Pointers to consumed protocols
  //
  EFI_WIRELESS_MAC_CONNECTION_II_PROTOCOL    *Wmp;
  EFI_ADAPTER_INFORMATION_PROTOCOL           *Aip;
  EFI_SUPPLICANT_PROTOCOL                    *Supplicant;
  EFI_EAP_CONFIGURATION_PROTOCOL             *EapConfig;

  //
  // Produced protocols
  //
  WIFI_MGR_PRIVATE_PROTOCOL                   WifiMgrIdentifier;

  //
  // Private functions and data fields
  //
  LIST_ENTRY                                  Link;  // Link to the NicList in global private data structure.
  UINT32                                      NicIndex;
  EFI_80211_MAC_ADDRESS                       MacAddress;
  WIFI_MGR_SUPPORTED_SUITES                   SupportedSuites;
  EFI_ADAPTER_INFO_MEDIA_STATE                LastLinkState;

  //
  //Profile related data fields
  //
  LIST_ENTRY                                  ProfileList; // List of WIFI_MGR_NETWORK_PROFILE_NVDATA
  UINT32                                      ProfileCount;
  UINT32                                      AvailableCount;
  UINT32                                      MaxProfileIndex;
  UINT32                                      *PriorityList;
  WIFI_MGR_NETWORK_PROFILE_NVDATA             *AutoSelectedProfile;
  WIFI_MGR_NETWORK_PROFILE_NVDATA             *UserSelectedProfile;

  //
  // Data fields for Hii functionlity
  //
  LIST_ENTRY                                  TokenList;   // List of WIFI_MGR_MAC_CONFIG_TOKEN
  BOOLEAN                                     OneTimeScanRequest;
  BOOLEAN                                     OneTimeConnectRequest;
  BOOLEAN                                     OneTimeDisconnectRequest;
  WIFI_MGR_SCAN_STATE                         ScanState;
  UINTN                                       ScanTickTime;
  WIFI_MGR_CONNECT_STATE                      ConnectState;
  BOOLEAN                                     ConnectStateChanged;

  //
  // The network is currently connected, connecting or disconnecting.
  // Only one network can be operated at one time.
  //
  WIFI_MGR_NETWORK_PROFILE_NVDATA             *CurrentOperateNetwork;
  WIFI_MGR_NETWORK_PROFILE_NVDATA             *ConnectPendingNetwork;
  BOOLEAN                                     HasDisconnectPendingNetwork;

} WIFI_MGR_DEVICE_DATA;

#define WIFI_MGR_DEVICE_DATA_SIGNATURE  SIGNATURE_32 ('W','M','D','D')

#define WIFI_MGR_DEVICE_DATA_FROM_IDENTIFIER(Identifier) \
  CR ( \
    Identifier, \
    WIFI_MGR_DEVICE_DATA, \
    WifiMgrIdentifier, \
    WIFI_MGR_DEVICE_DATA_SIGNATURE \
    )

//
// Global private data struct
//
struct _WIFI_MGR_PRIVATE_DATA {
  UINT32                            Signature;
  EFI_HANDLE                        DriverHandle;
  EFI_HII_HANDLE                    RegisteredHandle;
  EFI_HII_CONFIG_ACCESS_PROTOCOL    ConfigAccess;

  UINT32                            NicCount;
  LIST_ENTRY                        NicList;

  WIFI_MGR_DEVICE_DATA              *CurrentNic;
  CHAR16                            PortString[WIFI_STR_MAX_SIZE];

  //
  // Data fields for Hii functionlity
  //
  UINT8                             CurrentFormId;
  EFI_EVENT                         NetworkListRefreshEvent;  // Event to refresh the network list form
  EFI_EVENT                         ConnectFormRefreshEvent;  // Event to refresh the connect form
  EFI_EVENT                         MainPageRefreshEvent;     // Event to refresh the main page

  //
  //User Input Record
  //
  BOOLEAN                           AutoConnect;
  BOOLEAN                           ScanAnyway;
  UINT8                             SecurityType;
  UINT8                             EapAuthMethod;
  UINT8                             EapSecondAuthMethod;
  CHAR16                            SSId[SSID_STORAGE_SIZE];
  CHAR16                            EapIdentity[EAP_IDENTITY_SIZE];
  CHAR16                            PasswordString[PASSWORD_STORAGE_SIZE];
  CHAR16                            EapPasswordString[PASSWORD_STORAGE_SIZE];
  WIFI_MGR_FILE_CONTEXT             *FileContext;
  WIFI_MGR_FILE_TYPE                FileType;
  EFI_GUID                          CACertGuid;
  CHAR16                            CACertName[WIFI_FILENAME_STR_MAX_SIZE];
  EFI_GUID                          ClientCertGuid;
  CHAR16                            ClientCertName[WIFI_FILENAME_STR_MAX_SIZE];
  CHAR16                            PrivateKeyName[WIFI_FILENAME_STR_MAX_SIZE];
  CHAR16                            PrivateKeyPassword[PASSWORD_STORAGE_SIZE];

  //
  // Data fields for Kms registeration
  //
  EFI_KMS_PROTOCOL                  *Kms;
  EFI_KMS_CLIENT_INFO               ClientInfo;
  UINTN                             ClientDataSize;
  VOID                              *ClientData;
};

//   H2O_DIALOG_PROTOCOL               *mH2oDialog;

#define WIFI_MGR_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('W','M','P','D')

#define WIFI_MGR_PRIVATE_DATA_FROM_CONFIG_ACCESS(This) \
  CR ( \
    This, \
    WIFI_MGR_PRIVATE_DATA, \
    ConfigAccess, \
    WIFI_MGR_PRIVATE_DATA_SIGNATURE \
    )
extern    WIFI_MGR_PRIVATE_DATA    *mPrivate;

typedef enum {
  TokenTypeGetNetworksToken,
  TokenTypeConnectNetworkToken,
  TokenTypeDisconnectNetworkToken,
  TokenTypeMax,
} WIFI_MGR_MAC_CONFIG_TOKEN_TYPE;

typedef union {
  EFI_80211_GET_NETWORKS_TOKEN         *GetNetworksToken;
  EFI_80211_CONNECT_NETWORK_TOKEN      *ConnectNetworkToken;
  EFI_80211_DISCONNECT_NETWORK_TOKEN   *DisconnectNetworkToken;
} MAC_CONNECTION2_ADAPTER_TOKEN;

typedef struct {
  UINT32                          Signature;
  WIFI_MGR_MAC_CONFIG_TOKEN_TYPE  Type;
  //
  // Link to the TokenList in device data structure.
  //
  LIST_ENTRY                      Link;
  WIFI_MGR_DEVICE_DATA            *Nic;

  MAC_CONNECTION2_ADAPTER_TOKEN   Token;
} WIFI_MGR_MAC_CONFIG_TOKEN;

#define WIFI_MGR_MAC_CONFIG_TOKEN_SIGNATURE  SIGNATURE_32 ('W','M','C','T')

//
// Include files with function prototypes
//
#include "WifiConnectionMgrDriverBinding.h"
#include "WifiConnectionMgrImpl.h"
#include "WifiConnectionMgrComponentName.h"
#include "WifiConnectionMgrHiiConfigAccess.h"
#include "WifiConnectionMgrMisc.h"
#include "WifiConnectionMgrFileUtil.h"
#include "WifiConnectionMgrFileExplorer.h"
#include "VariableEx.h"
#include "KmsService.h"

//
// CryptoServices
//
extern CRYPTO_SERVICES_PBKDF2_CREATE_KEY  mPbkdf2CreateKey;

#endif
