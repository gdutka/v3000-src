/** @file
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

#include "CpuDeviceInfoDxe.h"

STATIC
EFI_STATUS
GetSmbiosCacheSizeInternal (
  IN UINT8                      CacheLevel,
  IN UINT8                      CacheType,
  OUT UINT32                    *CacheSize
  )
{
  EFI_STATUS                    Status;
  EFI_SMBIOS_PROTOCOL           *Smbios;
  EFI_SMBIOS_HANDLE             SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER       *Record;
  SMBIOS_TABLE_TYPE7            *Type7;

  if (CacheSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  while (TRUE) {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (Record->Type != SMBIOS_TYPE_CACHE_INFORMATION) {
      continue;
    }

    Type7 = (SMBIOS_TABLE_TYPE7 *) Record;
    
    if ((Type7->CacheConfiguration & B_SMBIOS_TABLE_TYPE7_CACHE_LEVEL) != CacheLevel) {
      continue;
    } else if (Type7->SystemCacheType != CacheType) {
      continue;
    }

    if (Type7->InstalledSize == 0xFFFF) {
      *CacheSize  = (Type7->InstalledSize2 & ~BIT31);
      *CacheSize *= (Type7->InstalledSize2 & BIT31? 64: 1); // in Kbytes
    } else {
      *CacheSize  = (Type7->InstalledSize & ~BIT15);
      *CacheSize *= (Type7->InstalledSize & BIT15? 64: 1); // in Kbytes
    }

    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
EFIAPI
GetCpuTDP (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
     OUT UINT32                            *CpuTDP
  )
{
  if (This == NULL || CpuTDP == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *CpuTDP = 0;  // Unsupported.

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetCoreL1DataCache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA    *Private;
  EFI_STATUS                          Status;
  UINT32                              TotalSize;
  UINT32                              NumberOfCores;

  if (This == NULL || CacheSize == NULL || CacheAssociativity == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Some platforms may only have one L1 cache SMBIOS record, combining
  // information of both the instruction cache and the data cache.
  //
  Status = GetSmbiosCacheSizeInternal (EfiCacheLevel1, CacheTypeData, &TotalSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Private = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (!Private->CpuInfo[CpuInfoIndex].CpuInstalled) {
    return EFI_NOT_READY;
  }

  if (Private->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount != 0xFF) {
    NumberOfCores = Private->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount;
  } else {
    NumberOfCores = Private->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount2;
  }

  *CacheSize          = TotalSize / NumberOfCores;
  *CacheAssociativity = 0;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetCoreL1InsructionCache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA    *Private;
  EFI_STATUS                          Status;
  UINT32                              TotalSize;
  UINT32                              NumberOfCores;

  if (This == NULL || CacheSize == NULL || CacheAssociativity == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Some platforms may only have one L1 cache SMBIOS record, combining
  // information of both the instruction cache and the data cache.
  //
  Status = GetSmbiosCacheSizeInternal (EfiCacheLevel1, CacheTypeInstruction, &TotalSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Private = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (!Private->CpuInfo[CpuInfoIndex].CpuInstalled) {
    return EFI_NOT_READY;
  }

  if (Private->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount != 0xFF) {
    NumberOfCores = Private->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount;
  } else {
    NumberOfCores = Private->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount2;
  }

  *CacheSize          = TotalSize / NumberOfCores;
  *CacheAssociativity = 0;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetCoreL2Cache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA    *Private;
  EFI_STATUS                          Status;
  UINT32                              TotalSize;
  UINT32                              NumberOfCores;

  if (This == NULL || CacheSize == NULL || CacheAssociativity == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetInstalledCacheSize (This, CpuInfoIndex, EfiCacheLevel2, &TotalSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Private = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (!Private->CpuInfo[CpuInfoIndex].CpuInstalled) {
    return EFI_NOT_READY;
  }

  if (Private->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount != 0xFF) {
    NumberOfCores = Private->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount;
  } else {
    NumberOfCores = Private->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount2;
  }

  *CacheSize          = TotalSize / NumberOfCores;
  *CacheAssociativity = 0;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetCoreL3Cache (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity,
  OUT UINT32                          *CacheNumWays
  )
{
  EFI_STATUS                          Status;
  UINT32                              TotalSize;

  if (This == NULL || CacheSize == NULL || CacheAssociativity == NULL || CacheNumWays == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetInstalledCacheSize (This, CpuInfoIndex, EfiCacheLevel3, &TotalSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // For most processors, the socket has one L3 cache, shared by its all cores.
  //
  *CacheSize          = TotalSize;
  *CacheAssociativity = 0;
  *CacheNumWays       = 0;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetMicrocodeVersion (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
     OUT UINT64                            *MicrocodeVersion
  )
{
  if (This == NULL || MicrocodeVersion == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *MicrocodeVersion = 0; // Unsupported.

  return EFI_SUCCESS;
}