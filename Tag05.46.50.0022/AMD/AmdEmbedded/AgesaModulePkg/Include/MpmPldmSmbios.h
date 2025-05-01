/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
#include <Protocol/AmdPldmProtocol.h>

#ifndef  _MPM_DXE_H_
#define _MPM_DXE_H_
#pragma pack (push, 1)

/**
 * @brief Structure definition for AIM-T OEM Status
 *
 */
typedef union {
  struct {
    UINT8  AimtFactoryStatus:1;           ///< AIM-T Factory APCB Status. 1 - Enabled at factory; 0 - Disabled at factory
    UINT8  AimtEnableInBios:1;            ///< AIM-T Implementation in BIOS. 1 - Enabled in BIOS; 0 - Disabled in BIOS
    UINT8  WirelessManageabilityStatus:3; ///< Wireless Manageability Status
                                          ///<0h - Wireless Manageability Disabled
                                          ///<1h - DASH only Enabled
                                          ///<2h - KVM only Enabled
                                          ///<3h - DASH and KVM Enabled
                                          ///<4-7h - Reserved
    UINT8 WiredManageabilityStatus:3;     ///<Wired Manageability Status
                                          ///<0h - Wired Manageability Disabled
                                          ///<1h - DASH only Enabled
                                          ///<2h - KVM only Enabled
                                          ///<3h - DASH and KVM Enabled
                                          ///<4-7h - Reserved
  } Field;
  UINT8 Value;
} AIMT_OEM_STATUS;

/**
 * @brief Structure definition for BIOS option configuration
 *
 */
typedef union {
  struct {
    UINT8  AimtOption:1;                  ///< Aimt option in BIOS. 1 - Enabled in BIOS; 0 - Disabled in BIOS
    UINT8  Reserved:1;                    ///< must be zero
    UINT8  WirelessManageabilityOption:3; ///< Wireless Manageability Option in BIOS
                                          ///<0h - Wireless Manageability Disabled
                                          ///<1h - DASH only Enabled
                                          ///<2h - KVM only Enabled
                                          ///<3h - DASH and KVM Enabled
                                          ///<4-7h - Reserved
    UINT8 WiredManageabilityOption:3;     ///<Wired Manageability Status
                                          ///<0h - Wired Manageability Disabled
                                          ///<1h - DASH only Enabled
                                          ///<2h - KVM only Enabled
                                          ///<3h - DASH and KVM Enabled
                                          ///<4-7h - Reserved
  } Field;
  UINT8 Value;
} AIMT_BIOS_OPTION;

/**
 * @brief Structure definition for AIM-T State
 *
 */
typedef union {
  struct {
    UINT8  AimtState:3;                   ///< Aimt state
                                          ///<0h - Reserved
                                          ///<1h - Other
                                          ///<2h - Unknow
                                          ///<3h - Enabled
                                          ///<4h - Owned
                                          ///<5h - Managed and online
                                          ///<6h - Managed and offline
                                          ///<7h - Reserved
    UINT8 Reserved:5;                     ///<Reserved
  } Field;
  UINT8 Value;
} AIMT_STATE;

/**
 * @brief Structure definition for WIFI_TYPE
 *
 */
typedef union {
  struct {
    UINT8  WifiNetworkType:4;             ///< Wifi Network Type
                                          ///<0h - Reserved
                                          ///<1h - Other
                                          ///<2h - Unknow
                                          ///<3h - WPA2-PSK
                                          ///<4h - WPA2-Enterprise
                                          ///<5h - WPA3-PSK
                                          ///<6h - WPA3-Enterprise
                                          ///<7h - Reserved
    UINT8 Reserved:4;                     ///<Reserved
  } Field;
  UINT8 Value;
} WIFI_TYPE;

/**
 * @brief Structure definition for USER_STATUS
 *
 */
typedef union {
  struct {
    UINT8  UserStatus:1;                  ///< User Status
                                          ///<0h - Disabled
                                          ///<1h - Enabled

    UINT8  UserType:4;                    ///< User Type
                                          ///<0h - Reserved
                                          ///<1h - Other
                                          ///<2h - Unknow
                                          ///<3h - Administrator
                                          ///<4h - Standard
                                          ///<5h - Read Only
    UINT8 Reserved:3;                     ///<Reserved
  } Field;
  UINT8 Value;
} USER_STATUS;

/**
 * @brief Structure definition for DOMAIN_AUTHENTICATION
 *
 */
typedef union {
  struct {
    UINT8  AuthenticationStatus:1;        ///< Authentication Status
                                          ///<0h - Disabled
                                          ///<1h - Enabled
    UINT8 Reserved:7;                     ///<Reserved
  } Field;
  UINT8 Value;
} DOMAIN_AUTHENTICATION;

/**
 * @brief Structure definition for DHCP_STATUS
 *
 */
typedef union {
  struct {
    UINT8  DhcpStatus:1;                  ///< Dhcp Status
                                          ///<0h - Disabled
                                          ///<1h - Enabled
    UINT8 Reserved:7;                     ///<Reserved
  } Field;
  UINT8 Value;
} DHCP_STATUS;

/**
 * @brief Structure definition for ALERT_STATUS
 *
 */
typedef union {
  struct {
    UINT8  AlertStatus:1;                  ///<Alert Status
                                          ///<0h - Disabled
                                          ///<1h - Enabled
    UINT8 Reserved:7;                     ///<Reserved
  } Field;
  UINT8 Value;
} ALERT_STATUS;

/**
 * @brief Structure definition for MPM SMBIOS A1 information including standard SMBIOS header
 *
 */
typedef struct {
  SMBIOS_STRUCTURE              Hdr;
  UINT8                         MajorStructureVersion;
  UINT8                         MinorStructureVersion;
  AIMT_OEM_STATUS               AimtOemStatus;
  SMBIOS_TABLE_STRING           AimtSolutionVersion;
  AIMT_BIOS_OPTION              BiosOption;
  AIMT_STATE                    AimtState;
  SMBIOS_TABLE_STRING           DashProtocolVersion;
  SMBIOS_TABLE_STRING           WlanFirmwareVersion;
  SMBIOS_TABLE_STRING           KvmVersion;
  SMBIOS_TABLE_STRING           AeppVersion;
  SMBIOS_TABLE_STRING           AmsVersion;
} SMBIOS_TABLE_TYPEA1;

typedef struct {
  PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT ResponseFromatData;
  SMBIOS_TABLE_TYPEA1 SmbiosStructureA1;
} PLDM_GET_SMBIOS_BY_TYPE_A1;

/**
 * @brief Structure definition for MPM SMBIOS A2 information including standard SMBIOS header
 *
 */
typedef struct {
  SMBIOS_STRUCTURE              Hdr;
  UINT8                         MajorStructureVersion;
  UINT8                         MinorStructureVersion;
  SMBIOS_TABLE_STRING           ProvisioningConsole;
  SMBIOS_TABLE_STRING           ManageabilityConsole;
  SMBIOS_TABLE_STRING           CloudConsole;
  //wifi profile 0
  SMBIOS_TABLE_STRING           WifiNetworkNameProfile0;
  WIFI_TYPE                     WifiTypeProfile0;
  //wifi profile 1
  SMBIOS_TABLE_STRING           WifiNetworkNameProfile1;
  WIFI_TYPE                     WifiTypeProfile1;
  //wifi profile 2
  SMBIOS_TABLE_STRING           WifiNetworkNameProfile2;
  WIFI_TYPE                     WifiTypeProfile2;
  //wifi profile 3
  SMBIOS_TABLE_STRING           WifiNetworkNameProfile3;
  WIFI_TYPE                     WifiTypeProfile3;
  //wifi profile 4
  SMBIOS_TABLE_STRING           WifiNetworkNameProfile4;
  WIFI_TYPE                     WifiTypeProfile4;
  //Digest User 0
  SMBIOS_TABLE_STRING           UserName0;
  USER_STATUS                   UserStatus0;
  //Digest User 1
  SMBIOS_TABLE_STRING           UserName1;
  USER_STATUS                   UserStatus1;
  //Digest User 2
  SMBIOS_TABLE_STRING           UserName2;
  USER_STATUS                   UserStatus2;
  //Digest User 3
  SMBIOS_TABLE_STRING           UserName3;
  USER_STATUS                   UserStatus3;
  //Digest User 4
  SMBIOS_TABLE_STRING           UserName4;
  USER_STATUS                   UserStatus4;
  DOMAIN_AUTHENTICATION         DomainAuthentication;
  DHCP_STATUS                   DhcpStatus;
  SMBIOS_TABLE_STRING           StaticIpv4Address;
  SMBIOS_TABLE_STRING           StaticIpv4Subnet;
  SMBIOS_TABLE_STRING           StaticIpv4GateWay;
  SMBIOS_TABLE_STRING           OrganizationName;
  SMBIOS_TABLE_STRING           ContactName;
  SMBIOS_TABLE_STRING           ContactNumber;
  //Alert destination 0
  SMBIOS_TABLE_STRING           AlertName0;
  ALERT_STATUS                  AlertStatus0;
  SMBIOS_TABLE_STRING           AlertDestination0;
  //Alert destination 1
  SMBIOS_TABLE_STRING           AlertName1;
  ALERT_STATUS                  AlertStatus1;
  SMBIOS_TABLE_STRING           AlertDestination1;
  UINT64                        DashProfileStatus;
  UINT8                         DashPolicy;
} SMBIOS_TABLE_TYPEA2;

typedef struct {
  PLDM_GET_SMBIOS_STRUCTURE_BY_TYPE_RESPONSE_FORMAT ResponseFromatData;
  SMBIOS_TABLE_TYPEA2 SmbiosStructureA2;
} PLDM_GET_SMBIOS_BY_TYPE_A2;

#pragma pack (pop)
#endif //_MPM_DXE_H_