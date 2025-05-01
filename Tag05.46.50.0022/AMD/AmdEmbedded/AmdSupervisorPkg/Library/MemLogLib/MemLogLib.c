/*
*******************************************************************************

Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************
*/

#include <Library/DebugLib.h>
#include <Library/MemLogLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>

#define BIOS_RAM_INDEX                    0xCD4
#define BIOS_RAM_DATA                     0xCD5
#define BIOS_RAM_REG_MEM_LOG_BUFFER       0x84
#define BIOS_RAM_REG_MEM_LOG_LENGTH       4

UINTN   mMemLogBufferOffset = 0;
CHAR8   *mMemLogBuffer = NULL;
UINT32  mMemLogCount = 0;

VOID
BiosRamWrite (
  IN UINT8 RamIndex,
  IN UINT8 Value
  )
{
  IoWrite8 (BIOS_RAM_INDEX, RamIndex);
  IoWrite8 (BIOS_RAM_DATA, Value);
}

VOID
WriteLogBufferAddress (
  IN UINT32 LogBufferAddress
  )
{
  UINT8  Bytes;
  UINT8  Value;
  for (Bytes = 0; Bytes < BIOS_RAM_REG_MEM_LOG_LENGTH; Bytes++) {
    Value = (UINT8)((LogBufferAddress >> (Bytes * 8)) & 0xFF);
    BiosRamWrite (BIOS_RAM_REG_MEM_LOG_BUFFER + Bytes, Value);
  }
}

/**
 * allocate memory buffer and write the buffer pointer to BIOS RAM 0x84~0x87
 *
 *
 *
 * @retval EFI_SUCCESS    Initial success
 * @retval Others         Error happens during initialize
 */
EFI_STATUS
EFIAPI
MemLogLibConstructor (VOID)
{
  EFI_STATUS Status = EFI_SUCCESS;
  if (mMemLogBuffer == NULL) {
    DEBUG ((DEBUG_INFO, "Allocating memory log buffer...\n"));
    Status = gBS->AllocatePool (EfiReservedMemoryType, MEM_LOG_BUFFER_SIZE, &mMemLogBuffer);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Allocate memory log buffer failed, Status:%r\n", Status));
      return EFI_OUT_OF_RESOURCES;
    }

    DEBUG ((DEBUG_INFO, "Memory log buffer allocated at 0x%x, it can be retrieved at BIOS RAM 0x%x (len:%d).\n", \
            (UINT32)(UINTN)mMemLogBuffer, \
            BIOS_RAM_REG_MEM_LOG_BUFFER, BIOS_RAM_REG_MEM_LOG_LENGTH \
            ));

    ZeroMem (mMemLogBuffer, MEM_LOG_BUFFER_SIZE);
    WriteLogBufferAddress ((UINT32)(UINTN)mMemLogBuffer);
  }

  return EFI_SUCCESS;
}

/**
 *  Process debug string and print to memory
 *
 *  @param[in] *Format - format string
 *
**/
VOID
EFIAPI
MemLogInternal (
  IN      CONST CHAR8 *Format,
  ...
  )
{
  CHAR8 LocalBuffer[MAX_LOCAL_BUFFER_SIZE];
  UINTN OutPutStringLen;
  VA_LIST  Marker;

  ASSERT (mMemLogBuffer != NULL);

  VA_START (Marker, Format);
  OutPutStringLen = AsciiVSPrint (LocalBuffer, sizeof (LocalBuffer), Format, Marker);
  VA_END (Marker);

  MemVLog (OutPutStringLen,LocalBuffer);
}

/**
 *  print debug string to memory
 *
 *  @param[in] *Format - format string
 *
**/
VOID
EFIAPI
MemVLog (
  IN   UINTN OutPutStringLen,
  IN   CHAR8 *LocalBuffer
  )
{
  if (mMemLogBufferOffset + OutPutStringLen + 1 <= MEM_LOG_BUFFER_SIZE) { //remaining room is enough to add this log
    CopyMem (mMemLogBuffer + mMemLogBufferOffset, LocalBuffer, OutPutStringLen + 1);
    mMemLogBufferOffset += (OutPutStringLen + 1);
  } else { //remaining room is not enough, we need to split the log into two parts
    CopyMem (mMemLogBuffer + mMemLogBufferOffset, LocalBuffer, MEM_LOG_BUFFER_SIZE - mMemLogBufferOffset);
    CopyMem (mMemLogBuffer, LocalBuffer + (MEM_LOG_BUFFER_SIZE - mMemLogBufferOffset), OutPutStringLen + 1 - (MEM_LOG_BUFFER_SIZE - mMemLogBufferOffset));
    mMemLogBufferOffset = OutPutStringLen + 1 - (MEM_LOG_BUFFER_SIZE - mMemLogBufferOffset);
  }
}