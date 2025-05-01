/** @file
  CPU Device Info Protocol Dxe Header File

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/


#ifndef _CPU_DEVICE_INFO_DXE_H_
#define _CPU_DEVICE_INFO_DXE_H_

#include <Uefi.h>

#include <Guid/H2OBdsCheckPoint.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuMiscLib.h>
#include <Library/DebugLib.h>
#include <Library/H2OCpLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/SmbiosCommonLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/H2OCpuDeviceInfo.h>
#include <Protocol/Smbios.h>

#define MAX_STR_SIZE                  0x100

#define B_SMBIOS_TABLE_TYPE7_CACHE_LEVEL  (BIT2 | BIT1 | BIT0)

typedef struct {
  BOOLEAN              CpuInstalled;
  SMBIOS_TABLE_TYPE4   *Processor;
  SMBIOS_TABLE_TYPE7   *Cache[EfiCacheLevelMaximum];
} CPU_INFO;

typedef struct {
  UINT32                           Signature;
  EFI_HANDLE                       Handle;
  H2O_CPU_DEVICE_INFO_PROTOCOL     CpuDeviceInfo;
  UINT8                            NumberOfCpuInfo;
  CPU_INFO                         *CpuInfo;
  UINT8                            NumberOfInstallCpu;
} H2O_CPU_DEVICE_INFO_PRIVATE_DATA;

#define H2O_CPU_DEVICE_INFO_PRIVATE_DATA_SIGNATURE  SIGNATURE_32 ('C', 'D', 'I', 'F')
#define H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS(a)    \
  CR (a, H2O_CPU_DEVICE_INFO_PRIVATE_DATA, CpuDeviceInfo, H2O_CPU_DEVICE_INFO_PRIVATE_DATA_SIGNATURE)

EFI_STATUS
EFIAPI
GetAllCpuDeviceInfo (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  OUT    UINTN                             *NumberOfCpuInfo,
  OUT    UINTN                             *NumberOfInstallCpu,
  OUT    SMBIOS_TABLE_TYPE4                ***SmbiosTableType4s OPTIONAL
  );

BOOLEAN
EFIAPI
GetCpuInstallStatus (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex
  );

EFI_STATUS
EFIAPI
GetProcessorVersionString (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT CHAR16                            **ProcessorVersionString
  );

EFI_STATUS
EFIAPI
GetCpuId (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT32                            *CpuId
  );

EFI_STATUS
EFIAPI
GetCurrentSpeed (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT16                            *CurrentSpeed
  );

EFI_STATUS
EFIAPI
GetCpuTDP (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
     OUT UINT32                            *CpuTDP
  );

EFI_STATUS
EFIAPI
GetVoltage (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT8                             *Voltage
  );

EFI_STATUS
EFIAPI
GetCoreL1DataCache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  );

EFI_STATUS
EFIAPI
GetCoreL1InsructionCache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  );

EFI_STATUS
EFIAPI
GetCoreL2Cache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  );

EFI_STATUS
EFIAPI
GetCoreL3Cache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity,
  OUT UINT32                          *CacheNumWays
  );

EFI_STATUS
EFIAPI
GetInstalledCacheSize (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
  IN     UINT8                             CacheLevel,
     OUT UINT32                            *InstalledSize
  );

EFI_STATUS
EFIAPI
GetCoreThreadNumString (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT CHAR16                            **CoreThreadNumString
  );

EFI_STATUS
EFIAPI
GetMicrocodeVersion (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
     OUT UINT64                            *MicrocodeVersion
  );

EFI_STATUS
EFIAPI
GetCpuStepping (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT32                            *CpuStepping
  );

#endif
