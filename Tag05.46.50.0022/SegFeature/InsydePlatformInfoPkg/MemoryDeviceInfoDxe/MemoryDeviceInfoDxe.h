/** @file
  Memory Device Info Protocol Dxe Header File

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef _MEMORY_DEVICE_INFO_DXE_H_
#define _MEMORY_DEVICE_INFO_DXE_H_

#include <Uefi.h>

#include <Protocol/Smbios.h>
#include <Protocol/H2OMemoryDeviceInfo.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/H2OCpLib.h>
#include <Library/SmbiosCommonLib.h>
#include <Library/HiiLib.h>

#include <Guid/H2OBdsCheckPoint.h>

typedef struct {
  UINT8          MemoryType;
  EFI_STRING_ID  MemoryTypeString;
} MEMORY_TYPE_INFO;

typedef EFI_STRING_ID MEMORY_TYPE_DETAIL_INFO;

typedef EFI_STRING_ID DRAM_TYPE_INFO;

typedef struct {
  UINT8          EccType;
  EFI_STRING_ID  EccTypeString;
} ECC_TYPE_INFO;

typedef union {
  UINT16                    MemoryTypeDetailWord;
  MEMORY_DEVICE_TYPE_DETAIL MemoryTypeDetailBit;
} MEMORY_DEVICE_TYPE_DETAIL_CONVERT;

typedef struct {
  BOOLEAN              MemoryInstalled;
  SMBIOS_TABLE_TYPE17  *MemoryDevice;
} MEMORY_INFO;

typedef struct {
  UINT32                           Signature;
  EFI_HANDLE                       Handle;
  H2O_MEMORY_DEVICE_INFO_PROTOCOL  MemoryDeviceInfo;
  UINTN                            NumberOfMemoryInfo;
  MEMORY_INFO                      *MemoryInfo;
  UINTN                            NumberOfPhysicalMemoryArray;
  SMBIOS_TABLE_TYPE16              **PhysicalMemoryArray;
  UINTN                            NumberOfMemoryArrayMappedAddress;
  SMBIOS_TABLE_TYPE19              **MemoryArrayMappedAddress;
  UINTN                            NumberOfInstallMemory;
  UINTN                            NumberOfUnInstallMemory;
} H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA;

#define H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('M', 'D', 'I', 'F')
#define H2O_MEMORY_DEVICE_INFO_PRIVATE_FROM_THIS(a)    \
  CR (a, H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA, MemoryDeviceInfo, H2O_MEMORY_DEVICE_INFO_PRIVATE_DATA_SIGNATURE)

#endif

