//;******************************************************************************
//;* Copyright (c) 2021, Insyde Software Corporation. All Rights Reserved.
//;*
//;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
//;* transmit, broadcast, present, recite, release, license or otherwise exploit
//;* any part of this publication in any form, by any means, without the prior
//;* written permission of Insyde Software Corporation.
//;*
//;******************************************************************************

#ifndef _AMD_MCM_INFO_H_
#define _AMD_MCM_INFO_H_

#define AMD_MCM_INFO_GUID \
  { \
    0xe64f96fa, 0x4436, 0x43b3, { 0xa8, 0x73, 0x25, 0x9b, 0x47, 0xd6, 0xc5, 0xa9 } \
  }

#pragma pack (1)

typedef struct {
  UINTN    BusBase;
  UINTN    BusLimit;
  UINTN    IoBase;
  UINTN    IoLimit;
  UINTN    MmioBase32;
  UINTN    MmioLimit32;
  UINTN    PMmioBase32;
  UINTN    PMmioLimit32;
  UINT64   MmioBase64;
  UINT64   MmioLimit64;
  UINT64   PMmioBase64;
  UINT64   PMmioLimit64;
  UINT64   NonPciMmioResBase;
  UINT32   RbUid;
  UINT8    RbExist;
} AMD_ROOT_BRIDGE_INFO;

typedef struct _AMD_MCM_INFO_DATAS {
  UINTN                  NumberOfSockets;
  UINTN                  NumberOfDies;
  UINTN                  NumberOfRootBridge;
  AMD_ROOT_BRIDGE_INFO   *RbInfo;
} AMD_MCM_INFO_DATAS;

#pragma pack ()

///
/// Provides a programatic means to access SMM save state.
///
typedef struct _AMD_MCM_INFO_PROTOCOL {
  AMD_MCM_INFO_DATAS  *AmdMcmInfoDatas;
} AMD_MCM_INFO_PROTOCOL;

extern EFI_GUID gAmdMcmInfoProtocolGuid;

#endif
