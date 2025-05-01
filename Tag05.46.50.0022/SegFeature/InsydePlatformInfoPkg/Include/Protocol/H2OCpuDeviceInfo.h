/** @file
  H2O CPU Device Info Protocol Header File.

;******************************************************************************
;* Copyright (c) 2020 - 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#ifndef _H2O_CPU_DEVICE_INFO_H_
#define _H2O_CPU_DEVICE_INFO_H_

#include <IndustryStandard/SmBios.h>

typedef enum {
  EfiCacheLevel1,
  EfiCacheLevel2,
  EfiCacheLevel3,
  EfiCacheLevel4,
  EfiCacheLevel5,
  EfiCacheLevel6,
  EfiCacheLevel7,
  EfiCacheLevel8,
  EfiCacheLevelMaximum
} EFI_CACHE_LEVEL;

//
// H2O_CPU_DEVICE_INFO_PROTOCOL_GUID Protocol GUID
//
#define H2O_CPU_DEVICE_INFO_PROTOCOL_GUID \
  {0xC199E046, 0xDC95, 0x42E4, 0x81, 0x46, 0x18, 0xB8, 0x78, 0x45, 0xD6, 0xC9}

typedef struct _H2O_CPU_DEVICE_INFO_PROTOCOL H2O_CPU_DEVICE_INFO_PROTOCOL;

/**
  Get the number of SMBIOS type 4 and record.

  @param[out]     TotalSmbiosType4         The number of SMBIOS type 4 returned in Buffer.
  @param[out]     TotalInstallCpu          The number of CPU are installed returned in Buffer.
  @param[out]     SmbiosType4Record        A pointer to a allocated buffer that contains SmbiosType4Record
                                           It is caller's responsibility to free this buffer.

  @retval EFI_SUCCESS                      Get device information successfully.
  @retval EFI_NOT_FOUND                    Cannot find SMBIOS type 4.
  @retval EFI_OUT_OF_RESOURCES             Allocated buffer for SmbiosType4Record fail.

**/
typedef
EFI_STATUS
(EFIAPI *GET_ALL_CPU_DEVICE_INFO) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  OUT    UINTN                             *NumberOfCpuInfo,
  OUT    UINTN                             *NumberOfInstallCpu,
  OUT    SMBIOS_TABLE_TYPE4                ***SmbiosTableType4s OPTIONAL
  );

/**
  Get cpu install status.

  @param[in]      SmbiosType4Index         SMBIOS type 4 index.

  @retval         TRUE                     Installed.
  @retval         FALSE                    Not install.
**/
typedef
BOOLEAN
(EFIAPI *GET_CPU_INSTALL_STATUS) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex
  );

/**
  Get processor version string.

  @param[in]      SmbiosType4Index         SMBIOS type 4 index.
  @param[out]     ProcessorVersionString   The Processor Version string. Caller should free it after use.

  @retval         EFI_INVALID_PARAMETER    Request is NULL, illegal syntax, or unknown name.
  @retval         EFI_SUCCESS              Success.
**/
typedef
EFI_STATUS
(EFIAPI *GET_PROCESSOR_VERSION_STRING) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT CHAR16                            **ProcessorVersionString
  );

/**
  Get CPU ID.

  @param[in]      SmbiosType4Index         Which SMBIOS type 4 index to get.
  @param[out]     CpuId                    The contains CPU ID pointer.

  @retval         EFI_INVALID_PARAMETER    SmbiosType4Index over SMBIOS type 4 record array or CpuId is NULL.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get CPU ID successfully.
**/
typedef
EFI_STATUS
(EFIAPI *GET_CPU_ID) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT32                            *CpuId
  );

/**
  Get current speed.

  @param[in]      SmbiosType4Index         Which SMBIOS type 4 index to get.
  @param[out]     CurrentSpeed             The contains current speed pointer.

  @retval         EFI_INVALID_PARAMETER    SmbiosType4Index over SMBIOS type 4 record array or CurrentSpeed is NULL.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get current speed successfully.
**/
typedef
EFI_STATUS
(EFIAPI *GET_CURRENT_SPEED) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT16                            *CurrentSpeed
  );

/**
  Get CPU TDP.

  @param[in]      SmbiosType4Index         Which SMBIOS type 4 index to get.
  @param[out]     CpuTDP                   The contains CPU TDP pointer.

  @retval         EFI_INVALID_PARAMETER    SmbiosType4Index over SMBIOS type 4 record array or CpuTDP is NULL.
  @retval         EFI_SUCCESS              Get CPU TDP successfully.
**/
typedef
EFI_STATUS
(EFIAPI *GET_CPU_TDP) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
     OUT UINT32                            *CpuTDP
  );

/**
  Get voltage.

  @param[in]      SmbiosType4Index         Which SMBIOS type 4 index to get.
  @param[out]     Voltage                  The contains voltage pointer.

  @retval         EFI_INVALID_PARAMETER    SmbiosType4Index over SMBIOS type 4 record array or Voltage is NULL.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get voltage successfully.
**/
typedef
EFI_STATUS
(EFIAPI *GET_VOLTAGE) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT8                             *Voltage
  );

/**
  Get L1 data cache data (per core).

  @param[in]      This                     An H2O CPU Device Info instance.
  @param[in]      CpuInfoIndex             An index of CPU info entries.
  @param[out]     CacheSize                The L1 data cache's size.
  @param[out]     CacheAssociativity       The L1 data cache's associativity.

  @retval         EFI_SUCCESS              Success.
  @retval         !EFI_SUCCESS             An error occurred.

**/
typedef
EFI_STATUS
(EFIAPI *GET_CORE_L1_DATA_CACHE) (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  );

/**
  Get L1 instruction cache data (per core).

  @param[in]      This                     An H2O CPU Device Info instance.
  @param[in]      CpuInfoIndex             An index of CPU info entries.
  @param[out]     CacheSize                The L1 inst. cache's size.
  @param[out]     CacheAssociativity       The L1 inst. cache's associativity.

  @retval         EFI_SUCCESS              Success.
  @retval         !EFI_SUCCESS             An error occurred.

**/
typedef
EFI_STATUS
(EFIAPI *GET_CORE_L1_INSRUCTION_CACHE) (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  );

/**
  Get L2 cache data (per core).

  @param[in]      This                     An H2O CPU Device Info instance.
  @param[in]      CpuInfoIndex             An index of CPU info entries.
  @param[out]     CacheSize                The L2 cache's size.
  @param[out]     CacheAssociativity       The L2 cache's associativity.

  @retval         EFI_SUCCESS              Success.
  @retval         !EFI_SUCCESS             An error occurred.

**/
typedef
EFI_STATUS
(EFIAPI *GET_CORE_L2_CACHE) (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity
  );

/**
  Get L3 cache data (per socket).

  @param[in]      This                     An H2O CPU Device Info instance.
  @param[in]      CpuInfoIndex             An index of CPU info entries.
  @param[out]     CacheSize                The L3 cache's size.
  @param[out]     CacheAssociativity       The L3 cache's associativity.
  @param[out]     CacheNumWays             The L3 cache's number of ways.

  @retval         EFI_SUCCESS              Success.
  @retval         !EFI_SUCCESS             An error occurred.

**/
typedef
EFI_STATUS
(EFIAPI *GET_CORE_L3_CACHE) (
  IN H2O_CPU_DEVICE_INFO_PROTOCOL     *This,
  IN UINTN                            CpuInfoIndex,
  OUT UINT32                          *CacheSize,
  OUT UINT32                          *CacheAssociativity,
  OUT UINT32                          *CacheNumWays
  );

/**
  Get installed cache size.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[in]      CpuInfoIndex             Which CPU info index to get.
  @param[in]      CacheLevel               Value that specifies the cache level.
  @param[out]     InstalledSize            Installed cache size.

  @retval         EFI_INVALID_PARAMETER    The Param This or CacheConfiguration is NULL.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get cache configuration successfully.
**/
typedef
EFI_STATUS
(EFIAPI *GET_INSTALLED_CACHE_SIZE) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
  IN     UINT8                             CacheLevel,
     OUT UINT32                            *InstalledSize
  );

/**
  Get core thread num string.

  @param[in]      SmbiosType4Index         Which SMBIOS type 4 index to get.
  @param[out]     CoreThreadNumString      Get core thread num string.

  @retval         EFI_INVALID_PARAMETER    SmbiosType4Index over SMBIOS type 4 record array or CoreThreadNumString is NULL.
  @retval         EFI_OUT_OF_RESOURCES     Allocate memory failed.
  @retval         EFI_SUCCESS              Success.
**/
typedef
EFI_STATUS
(EFIAPI *GET_CORE_THREAD_NUM_STRING) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT CHAR16                            **CoreThreadNumString
  );

/**
  Get microcode version.

  @param[in]      SmbiosType4Index         Which SMBIOS type 4 index to get.
  @param[out]     MicrocodeVersion         The contains microcode version pointer.

  @retval         EFI_INVALID_PARAMETER    SmbiosType4Index over SMBIOS type 4 record array or MicrocodeVersion is NULL.
  @retval         EFI_SUCCESS              Get microcode version successfully.
**/
typedef
EFI_STATUS
(EFIAPI *GET_MICROCODE_VERSION) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
     OUT UINT64                            *MicrocodeVersion
  );

/**
  Get CPU stepping.

  @param[in]      SmbiosType4Index         Which SMBIOS type 4 index to get.
  @param[out]     CpuStepping              The contains CPU stepping pointer.

  @retval         EFI_INVALID_PARAMETER    SmbiosType4Index over SMBIOS type 4 record array or CpuStepping is NULL.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get CPU stepping successfully.
**/
typedef
EFI_STATUS
(EFIAPI *GET_CPU_STEPPING) (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT32                            *CpuStepping
  );

//
// H2O_MEMORY_DEVICE_INFO_PROTOCOL
//
struct _H2O_CPU_DEVICE_INFO_PROTOCOL {
  GET_ALL_CPU_DEVICE_INFO               GetAllCpuDeviceInfo;
  GET_CPU_INSTALL_STATUS                GetCpuInstallStatus;
  GET_PROCESSOR_VERSION_STRING          GetProcessorVersionString;
  GET_CPU_ID                            GetCpuId;
  GET_CURRENT_SPEED                     GetCurrentSpeed;
  GET_CPU_TDP                           GetCpuTDP;
  GET_VOLTAGE                           GetVoltage;
  GET_CORE_L1_DATA_CACHE                GetCoreL1DataCache;
  GET_CORE_L1_INSRUCTION_CACHE          GetCoreL1InsructionCache;
  GET_CORE_L2_CACHE                     GetCoreL2Cache;
  GET_CORE_L3_CACHE                     GetCoreL3Cache;
  GET_INSTALLED_CACHE_SIZE              GetInstalledCacheSize;
  GET_CORE_THREAD_NUM_STRING            GetCoreThreadNumString;
  GET_MICROCODE_VERSION                 GetMicrocodeVersion;
  GET_CPU_STEPPING                      GetCpuStepping;
};

//
// Extern the GUID for protocol use.
//
extern EFI_GUID gH2OCpuDeviceInfoProtocolGuid;

#endif

