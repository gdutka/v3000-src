/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file AmdPspKvmServiceProtocol.h
 *
 * @brief  PSP KVM service prototype definition
 *
 * @details Provide generic PSP based dash KVM AGESA external interface accorss programs
 *
 */

#ifndef _AMD_PSP_KVM_SERVICE_PROTOCOL_H_
#define _AMD_PSP_KVM_SERVICE_PROTOCOL_H_

#include <Protocol/GraphicsOutput.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
//
// GUID definition
//
extern EFI_GUID gAmdPspKvmServiceProtocolGuid;

typedef struct _AMD_PSP_KVM_SERVICE_PROTOCOL AMD_PSP_KVM_SERVICE_PROTOCOL;

/**
 * @brief Struct defintion for IPV4 address for PSP KVM
 *
 */
typedef struct _KVM_IP_INFO {
  UINT32 NetworkType;              ///< 0 - dynamic, 1 - static. Default should be dynamic.
  UINT8  Ipv4Addr[20];                ///< Null terminated ascii string indicating the static IPV4 address. ex: 192.168.0.10
  UINT8  Ipv4SubMask[20];        ///< Null terminated ascii string indicating the Subnet Mask. ex: 255.255.255.0
  UINT8  Ipv4DefGateway[20];  ///< Null terminated ascii string indicating the Default Gateway. ex:  192.168.0.1
} KVM_IP_INFO;

/**
 * @brief Structure definition for SMBIOS KVM information
 *
 */
typedef union {
  struct {
    UINT8  KvmEnabled:1;           ///< [0] 0b: Disabled, 1b: Enabled
    UINT8  Protocol:3;             ///< [3:1] 001b: Other Protocol, 010b: Raw, 011b: RDP, 100b: VNC
    UINT8  Connectivity:2;         ///< [5:4] 10b: Exclusive Connectivity, 11b: Shared Connectivity
    UINT8  ConcurrentCount:2;      ///< [7:6] 01b: Singular KVM Redirection, 10b: Two Concurrent KVM session possible, 11b: Three Concurrent KVM session possible
    UINT8  KvmRequested:1;         ///< [8] 1b:KVM Requested, 0b:KVM not Requested
    UINT8  KvmBootType:3;          ///< [11:9] 001b: BIOS Setup, 010b: BIOS POST
    UINT8  KvmVersion:2;           ///< [13:12] KVM version, 00b: KVM legacy, 01b: KVM V1 (RMB & onwards), 02b: Reserved
    UINT8  Reserved:2;             ///< [15:14] Reserved
  } Field;
  UINT16 Value;
} KVM_SMBIOS_KVM_INFORMATION;

/**
 * @brief Structure definition for SMBIOS KVM information including standard SMBIOS header
 *
 */
typedef struct {
  SMBIOS_STRUCTURE              Hdr;
  KVM_SMBIOS_KVM_INFORMATION    KvmInformation;
} SMBIOS_TABLE_TYPEA0;

#define KVM_SMBIOS_TYPE_KVM_INFORMATION    0xA0
#define KVM_SMBIOS_KVM_DISABLED            0x0
#define KVM_SMBIOS_KVM_ENABLED             0x1
#define KVM_SMBIOS_PROTOCOL_OTHER          0x1
#define KVM_SMBIOS_PROTOCOL_RAW            0x2
#define KVM_SMBIOS_PROTOCOL_RDP            0x3
#define KVM_SMBIOS_PROTOCOL_VNC            0x4
#define KVM_SMBIOS_CONNECTIVITY_EXCLUSIVE  0x2
#define KVM_SMBIOS_CONNECTIVITY_SHARED     0x3
#define KVM_SMBIOS_CONCURRENTCOUNT_SINGLE  0x1
#define KVM_SMBIOS_CONCURRENTCOUNT_TWO     0x2
#define KVM_SMBIOS_CONCURRENTCOUNT_THREE   0x3

#ifndef KVM_NOT_REQUESTED
  #define KVM_NOT_REQUESTED 0x00000000ul
#endif

#ifndef KVM_REQUESTED
  #define KVM_REQUESTED 0x00000001ul
#endif

#ifndef TCR_NOT_REQUESTED
  #define TCR_NOT_REQUESTED 0x00000000ul
#endif

#ifndef TCR_REQUESTED
  #define TCR_REQUESTED 0x00000001ul
#endif

/**
 * @brief Get value of KvmRequest
 *
 * @details SBIOS send this request to query if any KVM pending request, if request detected,
 *   take corresponding actions, e.g. disable other network related service, force enter setup and etc
 *
 * @param KvmRequest Point to the address to hold KvmRequest
 * @return EFI_STATUS                 0: Success, NonZero Error
 */
typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_PROTOCOL_GET_REQUEST) (
  IN     UINT32                *KvmRequest
  );

/**
 * @brief Set value of KvmRequest. NOTE, this request only valid on PSP based KVM not MPM based KVM
 *
 * @details SBIOS send this request to set KVM  request
 *
 * @param KvmRequest Point to the address to hold KvmRequest
 * @return EFI_STATUS                 0: Success, NonZero Error
 */
typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_PROTOCOL_SET_REQUEST) (
  IN     UINT32                *KvmRequest
  );

/**
 * @brief Get value of KvmSessionProgress. NOTE, this request only valid on PSP based KVM not MPM based KVM
 *
 * @param KvmSessionProgress Point to the address to hold KvmSessionProgress
 * @return EFI_STATUS                  0: Success, NonZero Error
 */
typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_PROTOCOL_GET_SESSION_PROGRESS) (
  IN     UINT32                *KvmSessionProgress
  );

/**
 * @brief Set value of KvmSessionProgress. NOTE, this request only valid on PSP based KVM not MPM based KVM
 *
 * @param KvmSessionProgress Point to the address to hold KvmSessionProgress
 * @return EFI_STATUS                  0: Success, NonZero Error
 */
typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_PROTOCOL_SET_SESSION_PROGRESS) (
  IN     UINT32                *KvmSessionProgress
  );

/**
 * @brief Initiate Kvm
 *
 * @details If KVM request detected, SBIOS collect EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE info through UEFI GOP protocol
 *          collect KvmIpInfo through user input, and call this libraray to signal KVM engine to initialize KVM
 *
 * @param ModeInfo  Graphic mode related information, defined by EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE
 * @param KvmIpInfo  Not used in MPM case, keep it as NULL
 * @return EFI_STATUS                  0: Success, NonZero Error
 */
typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_PROTOCOL_INITIATE_KVM) (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *ModeInfo,
  IN KVM_IP_INFO                       *KvmIpInfo
  );

/**
 * @brief  Terminate KVM explicitly
 *
 * @details This usually used in the case where KVM need terminated explicitly,
 * e.g. In KVM mode User exit setup and going to restart the system
 * @return EFI_STATUS                  0: Success, NonZero Error
 */
typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_PROTOCOL_TERMINATE_KVM) (
  VOID
  );

/**
 * @brief  Get KVM SMBIOS table
 *
 * @details SBIOS call this interface with KVM enable/disable status, the routine will return the full
 *    copy of KVM SMBIOS table, SBIOS need push this SMBIOS through DASH. The SMBIOS date contain the
 *    KVM platform configuration data
 *
 * @param KvmEnabled  Denote KVM status, 0: disabled 1:enabled
 * @param SmbiosTableTypeA0 Point to SMBIOS_TABLE_TYPEA0 *, callee will allocate the memory for SMBIOS_TABLE_TYPEA0
 *
 * @return EFI_STATUS                  0: Success, NonZero Error
 */
typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_PROTOCOL_RETRIEVE_SMBIOS_TABLE) (
  IN    UINT8                   KvmEnabled,
  OUT   SMBIOS_TABLE_TYPEA0   **SmbiosTableTypeA0
  );

#define AMD_PSP_KVM_SERVICE_PROTOCOL_VERSION_1 (0x00000001ul)


/**
 * @brief Defines AMD_PSP_KVM_SERVICE_PROTOCOL. This protocol is used for provide all generic service related to AMD KVM
 *
 */
typedef struct _AMD_PSP_KVM_SERVICE_PROTOCOL {
  UINT32                                      Version;               ///< Protocol version
  AMD_PSP_KVM_PROTOCOL_GET_REQUEST            GetRequest;            ///< Get Kvm request
  AMD_PSP_KVM_PROTOCOL_SET_REQUEST            SetRequest;            ///< Set Kvm request
  AMD_PSP_KVM_PROTOCOL_GET_SESSION_PROGRESS   GetSessionProgress;    ///< Get Kvm SessionProgress
  AMD_PSP_KVM_PROTOCOL_SET_SESSION_PROGRESS   SetSessionProgress;    ///< Set Kvm SessionProgress
  AMD_PSP_KVM_PROTOCOL_INITIATE_KVM           InitiateKvm;           ///< Init KVM service
  AMD_PSP_KVM_PROTOCOL_TERMINATE_KVM          TerminateKvm;          ///< Terminate KVM service
  AMD_PSP_KVM_PROTOCOL_RETRIEVE_SMBIOS_TABLE  RetrieveSmbiosTable;   ///< Get KVM SMBIOS table
} AMD_PSP_KVM_SERVICE_PROTOCOL;

#endif //_AMD_PSP_KVM_SERVICE_PROTOCOL_H_

