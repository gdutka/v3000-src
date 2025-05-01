/** @file
  Implementation of MemoryDeviceInfoLib.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryDeviceInfoLib.h>
#include <Library/PrintLib.h>

#define MAX_STR_SIZE  0x100

/**
  The function are output customized memory size string format by one dimm

  @param[in]  MemorySize  The Memory size value.

  @retval Memory size string.

**/
CHAR16 *
SingleMemorySizeStringFormat (
  IN  UINT32  MemorySize
  )
{
  CHAR16      TempString[MAX_STR_SIZE] = {0};

  UnicodeSPrint (
    TempString,
    sizeof (TempString),
    L"%d MB (%d GB)",
    MemorySize,
    DivU64x32 (MultU64x32 ((UINT64)MemorySize, SIZE_1MB), SIZE_1GB)
    );

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized total memory size string format

  @param[in]  MemorySize  The Memory size value.

  @retval Memory size string.

**/
CHAR16 *
TotalMemorySizeStringFormat (
  IN  UINT64  MemorySize
  )
{
  CHAR16      TempString[MAX_STR_SIZE] = {0};

  UnicodeSPrint (
    TempString,
    sizeof (TempString),
    L"%d MB (%d GB)",
    DivU64x32 (MemorySize, SIZE_1MB),
    DivU64x32 (MemorySize, SIZE_1GB)
    );

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized memory speed string format

  @param[in]  MemorySpeed  The Memory speed value.

  @retval Memory speed string.

**/
CHAR16 *
MemorySpeedStringFormat (
  IN  UINT32  MemorySpeed
  )
{
  CHAR16      TempString[MAX_STR_SIZE] = {0};

  UnicodeSPrint (
    TempString,
    sizeof (TempString),
    L"%d MT/s",
    MemorySpeed
    );

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}

/**
  The function are output customized memory voltage string format

  @param[in]  MemoryVoltage  The Memory voltage value.

  @retval Memory voltage string.

**/
CHAR16 *
MemoryVoltageStringFormat (
  IN  UINT16  MemoryVoltage
  )
{
  CHAR16      TempString[MAX_STR_SIZE] = {0};

  UnicodeSPrint (
    TempString,
    sizeof (TempString),
    L"%d mV (%d.%d V)",
    MemoryVoltage,
    DivU64x32 (MultU64x32 ((UINT64)MemoryVoltage, 10), 1000) / 10,
    DivU64x32 (MultU64x32 ((UINT64)MemoryVoltage, 10), 1000) % 10
    );

  return (CHAR16 *) AllocateCopyPool (StrSize (TempString), TempString);
}