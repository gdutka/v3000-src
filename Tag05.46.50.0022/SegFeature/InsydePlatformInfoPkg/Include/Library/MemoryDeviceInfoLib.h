/** @file
  Memory Device Info Library Definition.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __MEMORY_DEVICE_INFO_LIB_H__
#define __MEMORY_DEVICE_INFO_LIB_H__

#include <Uefi.h>

#include <Protocol/H2OMemoryDeviceInfo.h>

/**
  The function are output customized memory size string format by one dimm

  @param[in]  MemorySize  The Memory size value.

  @retval Memory size string.

**/
CHAR16 *
SingleMemorySizeStringFormat (
  IN  UINT32  MemorySize
  );

/**
  The function are output customized total memory size string format

  @param[in]  MemorySize  The Memory size value.

  @retval Memory size string.

**/
CHAR16 *
TotalMemorySizeStringFormat (
  IN  UINT64  MemorySize
  );

/**
  The function are output customized memory speed string format

  @param[in]  MemorySpeed  The Memory speed value.

  @retval Memory speed string.

**/
CHAR16 *
MemorySpeedStringFormat (
  IN  UINT32  MemorySpeed
  );

/**
  The function are output customized memory voltage string format

  @param[in]  MemoryVoltage  The Memory voltage value.

  @retval Memory voltage string.

**/
CHAR16 *
MemoryVoltageStringFormat (
  IN  UINT16  MemoryVoltage
  );
#endif