/** @file
  CPU device info library definition.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _CPU_DEVICE_INFO_LIB_H_
#define _CPU_DEVICE_INFO_LIB_H_

#include <Uefi.h>

#include <Protocol/H2OCpuDeviceInfo.h>

/**
  The function are output customized current speed string format

  @param[in]      CurrentSpeed             The current speed value.

  @retval         Current speed string.
**/
CHAR16 *
CpuIdStringFormat (
  IN     UINT32               ProcessorSignature
  );

/**
  The function are output customized current speed string format

  @param[in]      CurrentSpeed             The current speed value.

  @retval         Current speed string.
**/
CHAR16 *
CurrentSpeedStringFormat (
  IN     UINT16               CurrentSpeed
  );

/**
  The function are output customized TDP string format

  @param[in]      CpuTDP                   The TDP value.

  @retval         TDP string.
**/
CHAR16 *
TDPStringFormat (
  IN     UINT32               CpuTDP
  );

/**
  The function are output customized voltage string format

  @param[in]      Voltage                  The voltage value.

  @retval         Voltage string.
**/
CHAR16 *
VoltageStringFormat (
  IN     UINT8                Voltage
  );

/**
  The function are output customized core L1 data cache string format.

  @param[in]      CacheSize                The cache size value.
  @param[in]      CacheAssociativity       The cache associativity value.

  @retval         Core L1 data cache string.
**/
CHAR16 *
CoreL1DataCacheStringFormat (
  IN     UINT32               CacheSize,
  IN     UINT32               CacheAssociativity
  );

/**
  The function are output customized core L1 insruction cache string format.

  @param[in]      CacheSize                The cache size value.
  @param[in]      CacheAssociativity       The cache associativity value.

  @retval         Core L1 insruction cache string.
**/
CHAR16 *
CoreL1InsructionCacheStringFormat (
  IN     UINT32               CacheSize,
  IN     UINT32               CacheAssociativity
  );

/**
  The function are output customized core L2 cache string format.

  @param[in]      CacheSize                The cache size value.
  @param[in]      CacheAssociativity       The cache associativity value.

  @retval         Core L2 cache string.
**/
CHAR16 *
CoreL2CacheStringFormat (
  IN     UINT32               CacheSize,
  IN     UINT32               CacheAssociativity
  );

/**
  The function are output customized core L3 cache string format.

  @param[in]      CacheSize                The cache size value.
  @param[in]      CacheAssociativity       The cache associativity value.

  @retval         Core L3 cache string.
**/
CHAR16 *
CoreL3CacheStringFormat (
  IN     UINT32               CacheSize,
  IN     UINT32               CacheAssociativity,
  IN     UINT32               CacheNumWays
  );

/**
  The function are output customized cache configuration string format.

  @param[in]      CacheConfiguration       The cache configuration value.

  @retval         Cache configuration string.
**/
CHAR16 *
CacheConfigurationStringFormat (
  IN     UINT16               CacheConfiguration
  );

/**
  The function are output customized microcode version string format

  @param[in]      MicrocodeVersion         The microcode version value.

  @retval         Microcode version string.
**/
CHAR16 *
MicrocodeVersionStringFormat (
  IN     UINT64               MicrocodeVersion
  );

/**
  The function are output customized CPU stepping string format

  @param[in]      CpuStepping              The CPU stepping value.

  @retval         CPU stepping string.
**/
CHAR16 *
CpuSteppingStringFormat (
  IN     UINT32               CpuStepping
  );
#endif
