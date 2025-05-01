/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_KVM_STATIC_IP_INFO_PROTOCOL_H_
#define _AMD_KVM_STATIC_IP_INFO_PROTOCOL_H_

#include <Uefi.h>
#include <Base.h>

//
// Extern the GUID for protocol users
//
extern EFI_GUID gAmdKvmStaticIpInfoProtocolGuid;

typedef struct _AMD_KVM_STATIC_IP_INFO_PROTOCOL AMD_KVM_STATIC_IP_INFO_PROTOCOL;

/**
  KVM Static IP information

  @param[OUT] StaticIpInfo

  @retval EFI_SUCCESS      Successfully get the Static IP info
**/
typedef
EFI_STATUS
(EFIAPI *DASH_ADDR_INFO_QUERY) (
  IN  AMD_KVM_STATIC_IP_INFO_PROTOCOL  *This,
  OUT UINT8                            *DhcpV4Enable,
  OUT EFI_IPv4_ADDRESS                 *Ipv4Addr,
  OUT EFI_IPv4_ADDRESS                 *Ipv4SubMask,
  OUT EFI_IPv4_ADDRESS                 *Ipv4GateWay
);

//
// Protocol Interface Structure
//
typedef struct _AMD_KVM_STATIC_IP_INFO_PROTOCOL
{
  DASH_ADDR_INFO_QUERY      AddrQuery;
} AMD_KVM_STATIC_IP_INFO_PROTOCOL;

#endif
