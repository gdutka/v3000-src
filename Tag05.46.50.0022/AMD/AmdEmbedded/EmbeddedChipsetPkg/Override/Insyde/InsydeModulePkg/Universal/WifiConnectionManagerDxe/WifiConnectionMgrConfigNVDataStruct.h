/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
 Define IFR NVData structures used by the Wireless Connection Manager.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef _WIFI_NVDATASTRUC_H_
#define _WIFI_NVDATASTRUC_H_

#include "WifiConnectionMgrConfigHii.h"

#define MANAGER_VARSTORE_ID         0x0802

#define WIFI_STR_MAX_SIZE             224
#define WIFI_FILENAME_STR_MAX_SIZE    224
#define WIFI_MGR_MAX_PROFILE_NUM      255
#define SSID_MIN_LEN                  1
#define SSID_MAX_LEN                  32
#define SSID_STORAGE_SIZE             33
#define PASSWORD_MIN_LEN              8
#define PASSWORD_MAX_LEN              63
#define PASSWORD_STORAGE_SIZE         64
#define CERT_GUID_LEN                 36
#define CERT_GUID_SIZE                37
#define EAP_IDENTITY_LEN              20
#define EAP_IDENTITY_SIZE             21

#define FORMID_NONE_FORM            0
#define FORMID_MAC_SELECTION        1
#define FORMID_WIFI_MAINPAGE        2
#define FORMID_NETWORK_LIST         3
#define FORMID_CONNECT_NETWORK      4
#define FORMID_ADD_NETWORK          5
#define FORMID_SAVED_LIST           6
#define FORMID_MANAGE_NETWORK       7
#define FORMID_CHANGE_PRIORITY      8
#define FORMID_ENROLL_CERT          9
#define FORMID_CA_LIST              10
#define FORMID_ENROLL_PRIVATE_KEY   11
#define FORMID_PRIVATE_KEY_LIST     12
#define FORMID_FILE_EXPLORER        13

//
// Mac List Form Key
//
#define KEY_MAC_LIST                              0x100

//
// Main Form Key
//
#define KEY_REFRESH_TITLE_CONNECTION_STATUS       0x101

//
// Network List Form Key
//
#define KEY_NETWORK_LIST                          0x102
#define KEY_REFRESH_NETWORK_LIST                  0x103

//
// Connect Network Form Key
//
#define KEY_PASSWORD_CONNECT_NETWORK              0x104
#define KEY_CONNECT_ACTION                        0x105
#define KEY_REFRESH_CONNECT_CONFIGURATION         0x106
#define KEY_EAP_AUTH_METHOD_CONNECT_NETWORK       0x107
#define KEY_EAP_SEAUTH_METHOD_CONNECT_NETWORK     0x108
#define KEY_ENROLL_CA_CERT_CONNECT_NETWORK        0x109
#define KEY_ENROLL_CLIENT_CERT_CONNECT_NETWORK    0x110
#define KEY_ENROLL_PRIVATE_KEY_CONNECT_NETWORK    0x111
#define KEY_EAP_IDENTITY_CONNECT_NETWORK          0x112
#define KEY_EAP_PASSWORD_CONNECT_NETWORK          0x113
//
//Add Network Form Key
//
#define KEY_ADD_NETWORK                           0x114
#define KEY_SSID_ADD_NETWORK                      0x115
#define KEY_SECURITY_TYPE_ADD_NETWORK             0x116
#define KEY_PASSWORD_ADD_NETWROK                  0x117
#define KEY_AUTO_CONNECT_ADD_NETWORK              0x118
#define KEY_SCAN_ANYWAY_ADD_NETWORK               0x119
#define KEY_ADD_NETWORK_ACTION                    0x120
#define KEY_EAP_AUTH_METHOD_ADD_NETWORK           0x121
#define KEY_EAP_SEAUTH_METHOD_ADD_NETWORK         0x122
#define KEY_ENROLL_CA_CERT_ADD_NETWORK            0x123
#define KEY_ENROLL_CLIENT_CERT_ADD_NETWORK        0x124
#define KEY_ENROLL_PRIVATE_KEY_ADD_NETWORK        0x125
#define KEY_EAP_IDENTITY_ADD_NETWORK              0x126
#define KEY_EAP_PASSWORD_ADD_NETWORK              0x127
//
//Manage Network Form Key
//
#define KEY_SAVED_LIST                            0x128
#define KEY_SSID_MANAGE_NETWORK                   0x129
#define KEY_SECURITY_TYPE_MANAGE_NETWORK          0x130
#define KEY_EAP_AUTH_METHOD_MANAGE_NETWORK        0x131
#define KEY_EAP_SEAUTH_METHOD_MANAGE_NETWORK      0x132
#define KEY_PASSWORD_MANAGE_NETWORK               0x133
#define KEY_AUTO_CONNECT_MANAGE_NETWORK           0x134
#define KEY_SCAN_ANYWAY_MANAGE_NETWORK            0x135
#define KEY_ENROLL_CA_CERT_MANAGE_NETWORK         0x136
#define KEY_ENROLL_CLIENT_CERT_MANAGE_NETWORK     0x137
#define KEY_ENROLL_PRIVATE_KEY_MANAGE_NETWORK     0x138
#define KEY_EAP_IDENTITY_MANAGE_NETWORK           0x139
#define KEY_EAP_PASSWORD_MANAGE_NETWORK           0x140
#define KEY_UPDATE_NETWORK_ACTION                 0x141
#define KEY_FORGET_NETWORK_ACTION                 0x142
#define KEY_CHANGE_PRIORITY                       0x143
#define KEY_SAVE_PRIORITY                         0x144

//
//Cert Form And Private Key Form
//
#define KEY_EAP_ENROLL_CERT_FROM_FILE             0x145
#define KEY_EAP_ENROLL_PRIVATE_KEY_FROM_FILE      0x146
#define KEY_SAVE_CERT                             0x147
#define KEY_NO_SAVE_CERT                          0x148
#define KEY_SAVE_PRIVATE_KEY                      0x149
#define KEY_NO_SAVE_PRIVATE_KEY                   0x150
#define KEY_CERT_GUID                             0x151
#define KEY_PRIVATE_KEY_PASSWORD                  0x152
#define KEY_ENROLLED_CERT_NAME                    0x153
#define KEY_ENROLLED_PRIVATE_KEY_NAME             0x154

#define LABEL_MAC_ENTRY                           0x1000
#define LABEL_NETWORK_LIST_ENTRY                  0x2000
#define LABEL_SAVED_LIST_ENTRY                    0x3000
#define LABEL_PRIORITY_ENTRY                      0x4000
#define KEY_FILE_EXPLORER_FILE_OPTION_BASE        0x4000
#define KEY_FILE_EXPLORER_FILE_OPTION_END         0x4FFF
#define KEY_MAC_ENTRY_BASE                        0x5000
#define KEY_AVAILABLE_NETWORK_ENTRY_BASE          0x6000
#define KEY_SAVED_NETWORK_ENTRY_BASE              0x7000
#define CONFIG_OPTION_OFFSET                      0x8000
#define LABEL_CA_FROM_FILE                        0x9000
#define LABEL_PRIVATE_KEY_FROM_FILE               0x9001
#define LABEL_FORM_FILE_EXPLORER_START_ID         0x9002
#define LABEL_FORM_FILE_EXPLORER_END_ID           0x9003
#define LABEL_CA_ENTRY                            0x9100
#define LABEL_PRIVATE_KEY_ENTRY                   0x9200
#define LABEL_END                                 0xffff

#define SECURITY_TYPE_NONE               0
#define SECURITY_TYPE_WPA_ENTERPRISE     1
#define SECURITY_TYPE_WPA2_ENTERPRISE    2
#define SECURITY_TYPE_WPA_PERSONAL       3
#define SECURITY_TYPE_WPA2_PERSONAL      4
#define SECURITY_TYPE_WEP                5
#define SECURITY_TYPE_UNKNOWN            6

#define EAP_AUTH_METHOD_TTLS             0
#define EAP_AUTH_METHOD_PEAP             1
#define EAP_AUTH_METHOD_TLS              2

#define EAP_SEAUTH_METHOD_MSCHAPV2       0
#define EAP_SEAUTH_METHOD_GTC            1

#pragma pack(1)
typedef struct _WIFI_MANAGER_IFR_NVDATA {

  UINT32      ProfileCount;
  CHAR16      SSId[SSID_STORAGE_SIZE];
  CHAR16      Password[PASSWORD_STORAGE_SIZE];
  UINT32      DynamicPriorityList[1020];  // WIFI_MGR_MAX_PROFILE_NUM * 4
  CHAR16      CertGuid[CERT_GUID_SIZE];   // Cert identifier GUID
  CHAR16      PrivateKeyPassword[PASSWORD_STORAGE_SIZE];  // Password to protect private key
  CHAR16      EapIdentity[EAP_IDENTITY_SIZE];
  CHAR16      EapPassword[PASSWORD_STORAGE_SIZE];

  BOOLEAN     AutoConnect;
  BOOLEAN     ScanAnyway;
  UINT8       SecurityType;
  UINT8       EapAuthMethod;
  UINT8       EapSecondAuthMethod;

} WIFI_MANAGER_IFR_NVDATA;
#pragma pack()

#endif
